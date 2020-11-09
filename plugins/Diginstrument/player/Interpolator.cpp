#include "Interpolator.h"

//tmp
#include <iostream>

using namespace std;
using namespace Diginstrument;

/**
 * Interpolate a time-slice of the partials corresponding to the given coordinates, frame offset and amount of frames
 */
template <typename T>
PartialSet<T> Diginstrument::Interpolator<T>::getPartials(const std::vector<T> &coordinates, unsigned int startFrame, unsigned int frames)
{
    return partials.processIntoRoot(coordinates,
            [this, startFrame, frames](const PartialSet<T> &left, const PartialSet<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const unsigned int dimension)
            ->PartialSet<T>
            {
                //TODO: better slicing detection
                if(left.get().size()>0 && left.get().front().size()>frames)
                {
                    return interpolatePartialSet(left.getSlice(startFrame, frames), right.getSlice(startFrame, frames), target, leftLabel, rightLabel, dimensions[dimension].shifting);
                }
                return interpolatePartialSet(left, right, target, leftLabel, rightLabel, dimensions[dimension].shifting);
            },
            [startFrame, frames](const PartialSet<T> & single)->PartialSet<T> 
            {
                if(single.get().size()>0 && single.get().front().size()>frames)
                {
                    return single.getSlice(startFrame, frames);
                }
                return single;
            }
    );
}

template <typename T>
Residual<T> Diginstrument::Interpolator<T>::getResidual(const std::vector<T> &coordinates, unsigned int startFrame, unsigned int frames)
{
    return residual.processIntoRoot(coordinates,
            [this, startFrame, frames](const Residual<T> &left, const Residual<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const unsigned int dimension)
            ->Residual<T>
            {
                //TODO: what if they dont match in size?
                if(left.size()>frames)
                {
                    return interpolateResidual(left.getSlice(startFrame, frames), right.getSlice(startFrame, frames), target, leftLabel, rightLabel, dimensions[dimension].shifting);
                }
                return interpolateResidual(left, right, target, leftLabel, rightLabel, dimensions[dimension].shifting);
            },
            [startFrame, frames](const Residual<T> & single)->Residual<T> 
            {
                if(single.size()>frames)
                {
                    return single.getSlice(startFrame, frames);
                }
                return single;
            }
    );
}

template <typename T>
PartialSet<T> Diginstrument::Interpolator<T>::interpolatePartialSet(const PartialSet<T> &left, const PartialSet<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const bool shifting)
{
    //TODO
    //tmp
    if(left.empty()) return right;
    if(right.empty()) return left;
    //TMP: copied from spectrum
    vector<Match> matches;
    vector<unsigned int> unmatchedLeft;
    vector<unsigned int> unmatchedRight;

    if (left.empty() || !shifting)
    {
        //return attenuated right
        unmatchedRight.resize(right.getMatchables().size());
        for(int i = 0; i<unmatchedRight.size(); i++)
        {
            unmatchedRight[i]=i;
        }
    }
    if (right.empty() || !shifting)
    {
        //return attenuated left
        unmatchedLeft.resize(left.getMatchables().size());
        for(int i = 0; i<unmatchedLeft.size(); i++)
        {
            unmatchedLeft[i]=i;
        }
    }
    if(shifting && !right.empty() && !left.empty())
    {
        //TODO: actually get fundamental frequency
        //fundamental frequency ratio
        const double componentRatio = left.getMatchables().front().frequency/right.getMatchables().front().frequency;
        const double maxRatioDeviance = 0.02;

        matches = PeakMatcher<T>::matchPeaks(left.getMatchables(), right.getMatchables(), unmatchedLeft, unmatchedRight,
            [componentRatio, maxRatioDeviance](const Diginstrument::Component<T> & left, const Diginstrument::Component<T>& right)->double
            {
                return abs((left.frequency/right.frequency)-componentRatio);
            },
            maxRatioDeviance
        );
    }

    //calculate shifting metrics
    const T rightWeight = (target-leftLabel) / (rightLabel - leftLabel);
    const T leftWeight = 1.0f - rightWeight;
    //todo: labels or sample rate needed?
    //TODO: only if not empty
    const int limit = std::min(left.get().front().size(), right.get().front().size());
    PartialSet<T> res;
    for(const auto & match : matches)
    {
        std::vector<Diginstrument::Component<T>> partial;
        partial.reserve(limit);
        for(int i = 0; i<limit; i++)
        {
            const Diginstrument::Component<T> leftComponent = left.get()[match.left][i];
            const Diginstrument::Component<T> rightComponent = right.get()[match.right][i];
            //TMP: simple linear interpolation and avg amp
            //TODO: frequency! sliding? what?!
            partial.emplace_back(leftComponent.frequency*leftWeight+rightComponent.frequency*rightWeight ,
             (leftComponent.phase*leftWeight+rightComponent.phase*rightWeight),
             (leftComponent.amplitude*leftWeight+rightComponent.amplitude*rightWeight)
            );
        }
        res.add(std::move(partial));
    }
    //tmp: disable unmatched, they were sliding all over the place!
    /*const double leftRatio = target/leftLabel;
    for(auto k : unmatchedLeft)
    {
        std::vector<Diginstrument::Component<T>> partial;
        partial.reserve(left.get()[k].size());
        for(int i = 0; i<left.get()[k].size(); i++)
        { 
            const Diginstrument::Component<T> leftComponent = left.get()[k][i];
            partial.emplace_back(
             leftComponent.frequency*leftRatio,
             leftComponent.phase*leftRatio,
             //amp uses weight!
             (leftComponent.amplitude*leftWeight)
            );
        }
        res.add(std::move(partial));
    }

    const double rightRatio = target/rightLabel;
    for(auto k : unmatchedRight)
    {
        std::vector<Diginstrument::Component<T>> partial;
        partial.reserve(right.get()[k].size());
        for(int i = 0; i<right.get()[k].size(); i++)
        { 
            const Diginstrument::Component<T> rightComponent = right.get()[k][i];
            partial.emplace_back(
             rightComponent.frequency * rightRatio,
             (rightComponent.phase*rightRatio),
             //amp uses weight!
             (rightComponent.amplitude*rightWeight)
            );
        }
        res.add(std::move(partial));
    }*/
    return res;
}

template <typename T>
Residual<T> Diginstrument::Interpolator<T>::interpolateResidual(const Residual<T> &left, const Residual<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const bool shifting)
{
    //TODO
    if(right.get().size()==0) return left;
    if(left.get().size()==0) return right;
    const T rightWeight = (target-leftLabel) / (rightLabel - leftLabel);
    const T leftWeight = 1.0f - rightWeight;
    //tmp: we can just comb them together, the frame index can be present multiple times!
    //TODO: too slow!
    Residual<T> res;
    res.reserve(left.size()+right.size());
    for(const auto & s : left.get())
    {
        std::vector<Diginstrument::Component<T>> components = s.second;
        for(auto & c : components)
        {
            c.amplitude*=leftWeight;
        }
        res.add(std::make_pair(s.first, std::move(components)));
    }
    for(const auto & s : right.get())
    {
        std::vector<Diginstrument::Component<T>> components = s.second;
        for(auto & c : components)
        {
            c.amplitude*=rightWeight;
        }
        res.add(std::make_pair(s.first, std::move(components)));
    }
    return res;
}

template <typename T>
void Diginstrument::Interpolator<T>::addPartialSets(const std::vector<PartialSet<T>> & partialSets)
{
    //TODO:test, check, better
    for(const auto & p : partialSets)
    {
        vector<T> labels;
        for(const auto & l : p.getLabels())
        {
            labels.push_back(l.second);
        }
        partials.insert(p, labels);
    }
}

template <typename T>
void Diginstrument::Interpolator<T>::addResiduals(const std::vector<Residual<T>> & residuals)
{
    //TODO:test, check, better
    for(const auto & r : residuals)
    {
        vector<T> labels;
        for(const auto & l : r.getLabels())
        {
            labels.push_back(l.second);
        }
        residual.insert(r, labels);
    }
}


template <typename T>
void Diginstrument::Interpolator<T>::clear()
{
    partials.clear();
    residual.clear();
    dimensions.clear();
}

template <typename T>
void Diginstrument::Interpolator<T>::setDimensions(const std::vector<Diginstrument::Dimension> &dimensions)
{
    this->dimensions = dimensions;
}

template <typename T>
const std::vector<Dimension> & Diginstrument::Interpolator<T>::getDimensions() const
{
    return this->dimensions;
}

template class Diginstrument::Interpolator<float>;
template class Diginstrument::Interpolator<double>;