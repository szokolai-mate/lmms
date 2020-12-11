#include "Interpolator.h"

//tmp
#include <iostream>

using namespace std;
using namespace Diginstrument;

/**
 * Interpolate a time-slice of the partials corresponding to the given coordinates, frame offset and amount of frames
 */
template <typename T>
PartialSet<T> Diginstrument::Interpolator<T>::getPartials(const std::vector<T> &coordinates, unsigned int startFrame, unsigned int frames) const
{
    return partials.processIntoRoot(coordinates,
            [this, startFrame, frames](const PartialSet<T> &left, const PartialSet<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const unsigned int dimension)
            ->PartialSet<T>
            {
                //TODO: better slicing detection
                if(left.get().size()>0 && left.get().front().size()>frames)
                {
                    return interpolatePartialSet(left.getSlice(startFrame, frames), right.getSlice(startFrame, frames), target, leftLabel, rightLabel, /*TMP: unused parameter*/ false);
                }
                return interpolatePartialSet(left, right, target, leftLabel, rightLabel, /*TMP: unused parameter*/ false);
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
Residual<T> Diginstrument::Interpolator<T>::getResidual(const std::vector<T> &coordinates, unsigned int startFrame, unsigned int frames) const
{
    return residual.processIntoRoot(coordinates,
            [this, startFrame, frames](const Residual<T> &left, const Residual<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const unsigned int dimension)
            ->Residual<T>
            {
                return interpolateResidual(left.getSlice(startFrame, frames), right.getSlice(startFrame, frames), target, leftLabel, rightLabel, dimensions[dimension].shifting);
            },
            [startFrame, frames](const Residual<T> & single)->Residual<T> 
            {
                return single.getSlice(startFrame, frames);
            }
    );
}

template <typename T>
PartialSet<T> Diginstrument::Interpolator<T>::interpolatePartialSet(const PartialSet<T> &left, const PartialSet<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const bool /*tmp*/ shifting2) const
{
    //TODO: remove shifting parameter
    //TODO
    //tmp: i may still need to attenuate!
    if(left.empty()) return right;
    if(right.empty()) return left;
    //TMP: copied from spectrum
    vector<Match> matches;
    vector<unsigned int> unmatchedLeft;
    vector<unsigned int> unmatchedRight;

    //get FFs
    const T FFl = left.getFundamentalFrequency();
    const T FFr = right.getFundamentalFrequency();

    //tmp: local shifting
    //this looks GOOD!
    bool shifting = true;
    if(abs(FFl/FFr-1) < 0.02)
    {
        shifting = false;
    }
    if(/*shifting && */!right.empty() && !left.empty())
    {
        //TODO: actually get fundamental frequency
        //fundamental frequency ratio
        const double componentRatio = FFl/FFr;
        const double maxRatioDeviance = 0.02;

        matches = PeakMatcher<T>::matchPeaks(left.getMatchables(), right.getMatchables(), unmatchedLeft, unmatchedRight,
            [componentRatio, maxRatioDeviance](const Diginstrument::Component<T> & left, const Diginstrument::Component<T>& right)->double
            {
                return abs((left.frequency/right.frequency)-componentRatio);
            },
            maxRatioDeviance
        );
    }

    //TODO: refactor whole process; too wordy now

    //calculate shifting metrics
    const T rightWeight = (target-leftLabel) / (rightLabel - leftLabel);
    const T leftWeight = 1.0f - rightWeight;
    const double leftRatio = target/leftLabel;
    const double rightRatio = target/rightLabel;
    //todo: labels or sample rate needed?
    //TODO: only if not empty
    PartialSet<T> res;
    for(const auto & match : matches)
    {
        std::vector<Diginstrument::Component<T>> partial;
        //while we have both partials
        const int limit = std::min(left.get()[match.left].size(), right.get()[match.right].size());
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
        //flute not good for this, no trimming
        //then process the remainder
        /*if(left.get()[match.left].size()<right.get()[match.right].size())
        {
            for(int i = limit; i<right.get()[match.right].size(); i++)
            {
                const Diginstrument::Component<T> component = right.get()[match.right][i];
                if(shifting)
                {
                    partial.emplace_back(
                        component.frequency*rightRatio,
                        component.phase*rightRatio,
                        //amp uses weight!
                        (component.amplitude*rightWeight)
                    );
                }
                else
                {
                    partial.emplace_back(
                        component.frequency,
                        component.phase,
                        //amp uses weight!
                        (component.amplitude*rightWeight)
                    );
                }
            }
        }
        if(left.get()[match.left].size()>right.get()[match.right].size())
        {
            for(int i = limit; i<left.get()[match.left].size(); i++)
            {
                const Diginstrument::Component<T> component = left.get()[match.left][i];
                if(shifting)
                {
                    partial.emplace_back(
                        component.frequency*leftRatio,
                        component.phase*leftRatio,
                        //amp uses weight!
                        (component.amplitude*leftWeight)
                    );
                }
                else
                {
                    partial.emplace_back(
                        component.frequency,
                        component.phase,
                        //amp uses weight!
                        (component.amplitude*leftWeight)
                    );
                }
            }
        }*/

        res.add(std::move(partial));
    }

    //based on flute: unmatched adds definition
    for(auto k : unmatchedLeft)
    {
        std::vector<Diginstrument::Component<T>> partial;
        const auto & leftPartial = left.get()[k];
        partial.reserve(leftPartial.size());
        for(int i = 0; i<leftPartial.size(); i++)
        { 
            const Diginstrument::Component<T> component = leftPartial[i];
            //TODO: currently rethinking this
            if(shifting)
            {
                partial.emplace_back(
                    component.frequency*leftRatio,
                    component.phase*leftRatio,
                    //amp uses weight!
                    (component.amplitude*leftWeight)
                );
            }
            else
            {
                partial.emplace_back(
                    component.frequency,
                    component.phase,
                    //amp uses weight!
                    (component.amplitude*leftWeight)
                );
            }            
        }
        res.add(std::move(partial));
    }

    
    for(auto k : unmatchedRight)
    {
        std::vector<Diginstrument::Component<T>> partial;
        const auto & rightPartial = right.get()[k];
        partial.reserve(rightPartial.size());
        for(int i = 0; i<rightPartial.size(); i++)
        { 
            const Diginstrument::Component<T> component = rightPartial[i];
            if(shifting)
            {
                partial.emplace_back(
                    component.frequency*rightRatio,
                    component.phase*rightRatio,
                    //amp uses weight!
                    (component.amplitude*rightWeight)
                );
            }
            else
            {
                partial.emplace_back(
                    component.frequency,
                    component.phase,
                    //amp uses weight!
                    (component.amplitude*rightWeight)
                );
            }   
        }
        res.add(std::move(partial));
    }
    return res;
}

template <typename T>
Residual<T> Diginstrument::Interpolator<T>::interpolateResidual(const Residual<T> &left, const Residual<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const bool shifting) const
{
    //TODO
    if(right.get().size()==0) return left;
    if(left.get().size()==0) return right;
    const T rightWeight = (target-leftLabel) / (rightLabel - leftLabel);
    const T leftWeight = 1.0f - rightWeight;
    //tmp: we can just comb them together, the frame index can be present multiple times!
    //TODO: too slow!
    //cout<<left.size()<<" "<<right.size()<<endl;
    
    auto leftChannel = left.get().begin();
    auto rightChannel = right.get().begin();
    //std::vector<std::pair<unsigned int, std::vector<Diginstrument::Component<T>>>> res;
    std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<T>>>> res;
    //tmp
    //cout<<left.get().size()<<" "<<right.get().size()<<endl;
    int both = 0;
    int leftOnly = 0;
    int rightOnly = 0;
    //TODO: the channels might be longer cuz of the extra ends i give while slicing?
    while (leftChannel!=left.get().end() && rightChannel!=right.get().end())
    {
        if(leftChannel->front().second.frequency < rightChannel->front().second.frequency)
        {
            //step left
            //TODO: attenuate
            std::vector<std::pair<unsigned int, Diginstrument::Component<T>>> channel;
            channel.reserve(leftChannel->size());
            for(const auto & p : *leftChannel)
            {
                channel.emplace_back(p.first,
                                    Diginstrument::Component<T>(
                                        p.second.frequency,
                                        p.second.phase,
                                        p.second.amplitude*leftWeight
                                    ));
            }
            //cout<<leftChannel->front().second.frequency<<endl;
            res.push_back(std::move(channel));
            leftOnly++;
            leftChannel++;
            continue;
        }
        if(rightChannel->front().second.frequency < leftChannel->front().second.frequency)
        {
            //step right
            //TODO
            //cout<<rightChannel->front().second.frequency<<endl;
            std::vector<std::pair<unsigned int, Diginstrument::Component<T>>> channel;
            channel.reserve(rightChannel->size());
            for(const auto & p : *rightChannel)
            {
                channel.emplace_back(p.first,
                                    Diginstrument::Component<T>(
                                        p.second.frequency,
                                        p.second.phase,
                                        p.second.amplitude*rightWeight
                                    ));
            }
            //cout<<leftChannel->front().second.frequency<<endl;
            res.push_back(std::move(channel));
            rightOnly++;
            rightChannel++;
            continue;
        }
        if(leftChannel->front().second.frequency == rightChannel->front().second.frequency)
        {
            //cout<<"("<<leftChannel->front().second.frequency<<", "<<leftChannel->front().second.amplitude<<") - ("<<rightChannel->front().second.frequency<<", "<<rightChannel->front().second.amplitude<<")"<<endl;
            //same frequency
            //TODO
            std::vector<std::pair<unsigned int, Diginstrument::Component<T>>> channel;
            channel.reserve(max(leftChannel->size(), rightChannel->size()));
            auto leftFrame = leftChannel->begin();
            auto rightFrame = rightChannel->begin();
            //TODO: are the channels sorted by frame?
            while(leftFrame!=leftChannel->end() && rightFrame!=rightChannel->end())
            {
                //frames should be sorted ascending
                if(leftFrame->first == rightFrame->first)
                {
                    //same frame
                    //cout<<"frame "<<leftFrame->first<<": ("<<leftFrame->second.frequency<<", "<<leftFrame->second.amplitude<<") - ("<<rightFrame->second.frequency<<", "<<rightFrame->second.amplitude<<")"<<endl;
                    //TODO
                    const auto & leftComponent = leftFrame->second;
                    const auto & rightComponent = rightFrame->second;
                    channel.emplace_back(leftFrame->first,
                                        Diginstrument::Component<T>(
                                            leftComponent.frequency,
                                            leftComponent.phase*leftWeight + rightComponent.phase*rightWeight,
                                            leftComponent.amplitude*leftWeight + rightComponent.amplitude*rightWeight
                                        ));
                    leftFrame++;
                    rightFrame++;
                    continue;
                }
                if(leftFrame->first<rightFrame->first)
                {
                    //tmp: why did i even check channel sizes?
                    //is this when right channel is empty here?
                    const T rightPhase = Interpolation::Linear((T)(rightFrame-1)->first, (rightFrame-1)->second.phase, (T)rightFrame->first, rightFrame->second.phase, (T)leftFrame->first);
                    const T rightAmp = Interpolation::Linear((T)(rightFrame-1)->first, (rightFrame-1)->second.amplitude, (T)rightFrame->first, rightFrame->second.amplitude, (T)leftFrame->first);
                    channel.emplace_back(leftFrame->first,
                                    Diginstrument::Component<T>(
                                        leftFrame->second.frequency,
                                        leftFrame->second.phase*leftWeight + rightPhase*rightWeight,
                                        leftFrame->second.amplitude*leftWeight + rightAmp*rightWeight
                                    ));
                    leftFrame++;
                    continue;
                    
                }
                if(rightFrame->first<leftFrame->first)
                {
                    //right only
                    const T leftPhase = Interpolation::Linear((T)(leftFrame-1)->first, (leftFrame-1)->second.phase, (T)leftFrame->first, leftFrame->second.phase, (T)rightFrame->first);
                    const T leftAmp = Interpolation::Linear((T)(leftFrame-1)->first, (leftFrame-1)->second.amplitude, (T)leftFrame->first, leftFrame->second.amplitude, (T)rightFrame->first);
                    channel.emplace_back(rightFrame->first,
                                    Diginstrument::Component<T>(
                                        rightFrame->second.frequency,
                                        rightFrame->second.phase*rightWeight + leftPhase*leftWeight,
                                        rightFrame->second.amplitude*rightWeight + leftAmp*leftWeight
                                    ));
                    //TODO
                    rightFrame++;
                    continue;
                }
            }
            //TODO: remainder
            res.push_back(std::move(channel));

            both++;
            leftChannel++;
            rightChannel++;
            continue;
        }
    }
    //TODO: remainder

    //TODO: labels
    return Residual<T>(res,{});
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
void Diginstrument::Interpolator<T>::addPartialSet(const PartialSet<T> & partialSet)
{
    //TODO:test, check, better
    vector<T> labels;
    for(const auto & l : partialSet.getLabels())
    {
        labels.push_back(l.second);
    }
    partials.insert(partialSet, labels);
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
void Diginstrument::Interpolator<T>::addResidual(const Residual<T> & residual)
{
    //TODO:test, check, better
    vector<T> labels;
    for(const auto & l : residual.getLabels())
    {
        labels.push_back(l.second);
    }
    this->residual.insert(residual, labels);
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