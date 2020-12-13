#include "Interpolator.h"

using namespace std;
using namespace Diginstrument;

/**
 * Interpolate a time-slice of the partials corresponding to the given coordinates, frame offset and amount of frames
 */
template <typename T>
PartialSet<T> Diginstrument::Interpolator<T>::getPartials(const std::vector<T> &coordinates, unsigned int startFrame, unsigned int frames) const
{
    return partials.processIntoRoot(coordinates,
            [this, startFrame, frames](const PartialSet<T> &left, const PartialSet<T> &right, const T &target, const T &leftLabel, const T &rightLabel)
            ->PartialSet<T>
            {
                //TODO: better slicing detection
                if(left.get().size()>0 && left.get().front().size()>frames)
                {
                    return interpolatePartialSet(left.getSlice(startFrame, frames), right.getSlice(startFrame, frames), target, leftLabel, rightLabel);
                }
                return interpolatePartialSet(left, right, target, leftLabel, rightLabel);
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
            [this, startFrame, frames](const Residual<T> &left, const Residual<T> &right, const T &target, const T &leftLabel, const T &rightLabel)
            ->Residual<T>
            {
                return interpolateResidual(left.getSlice(startFrame, frames), right.getSlice(startFrame, frames), target, leftLabel, rightLabel);
            },
            [startFrame, frames](const Residual<T> & single)->Residual<T> 
            {
                return single.getSlice(startFrame, frames);
            }
    );
}

template <typename T>
PartialSet<T> Diginstrument::Interpolator<T>::interpolatePartialSet(const PartialSet<T> &left, const PartialSet<T> &right, const T &target, const T &leftLabel, const T &rightLabel) const
{
    if(left.empty() && right.empty()) return PartialSet<T>({},{});

    vector<Match> matches;
    vector<unsigned int> unmatchedLeft;
    vector<unsigned int> unmatchedRight;

    //get FFs and decide whether pitch shifting is applied in this interpolation
    const T FFl = left.getFundamentalFrequency();
    const T FFr = right.getFundamentalFrequency();

    bool shifting = true;
    if(abs(FFl/FFr-1) < 0.02)
    {
        shifting = false;
    }
    //match partials
    if(!right.empty() && !left.empty())
    {
        //fundamental frequency ratio
        const double componentRatio = FFl/FFr;
        constexpr double maxRatioDeviance = 0.02;

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
    const double leftRatio = target/leftLabel;
    const double rightRatio = target/rightLabel;

    //construct new partials based on matches
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
            partial.emplace_back(leftComponent.frequency*leftWeight+rightComponent.frequency*rightWeight ,
             (leftComponent.phase*leftWeight+rightComponent.phase*rightWeight),
             (leftComponent.amplitude*leftWeight+rightComponent.amplitude*rightWeight)
            );
        }
        res.add(std::move(partial));
    }

    //add in unmatched partials attenuated
    for(auto k : unmatchedLeft)
    {
        std::vector<Diginstrument::Component<T>> partial;
        const auto & leftPartial = left.get()[k];
        partial.reserve(leftPartial.size());
        for(int i = 0; i<leftPartial.size(); i++)
        { 
            const Diginstrument::Component<T> component = leftPartial[i];
            if(shifting)
            {
                partial.emplace_back(
                    component.frequency*leftRatio,
                    component.phase*leftRatio,
                    //NOTE: amp uses weight!
                    (component.amplitude*leftWeight)
                );
            }
            else
            {
                partial.emplace_back(
                    component.frequency,
                    component.phase,
                    //NOTE: amp uses weight!
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
                    //NOTE: amp uses weight!
                    (component.amplitude*rightWeight)
                );
            }
            else
            {
                partial.emplace_back(
                    component.frequency,
                    component.phase,
                    //NOTE: amp uses weight!
                    (component.amplitude*rightWeight)
                );
            }   
        }
        res.add(std::move(partial));
    }
    return res;
}

template <typename T>
Residual<T> Diginstrument::Interpolator<T>::interpolateResidual(const Residual<T> &left, const Residual<T> &right, const T &target, const T &leftLabel, const T &rightLabel) const
{
    //TODO: if one is empty, other should still be attenuated!
    if(right.get().size()==0) return left;
    if(left.get().size()==0) return right;
    const T rightWeight = (target-leftLabel) / (rightLabel - leftLabel);
    const T leftWeight = 1.0f - rightWeight;

    auto leftChannel = left.get().begin();
    auto rightChannel = right.get().begin();
    std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<T>>>> res;

    //while there are channels to compare
    //NOTE: channels are sorted descending in frequency
    while (leftChannel!=left.get().end() && rightChannel!=right.get().end())
    {
        //if left is higher, process and step left
        if(leftChannel->front().second.frequency > rightChannel->front().second.frequency)
        {
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
            res.push_back(std::move(channel));
            leftChannel++;
            continue;
        }
        //if right is higher, process and step right
        if(rightChannel->front().second.frequency > leftChannel->front().second.frequency)
        {
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
            res.push_back(std::move(channel));
            rightChannel++;
            continue;
        }
        //if frequencies match, interpolate by linear combination
        if(leftChannel->front().second.frequency == rightChannel->front().second.frequency)
        {
            std::vector<std::pair<unsigned int, Diginstrument::Component<T>>> channel;
            channel.reserve(max(leftChannel->size(), rightChannel->size()));
            auto leftFrame = leftChannel->begin();
            auto rightFrame = rightChannel->begin();
            //NOTE: channels are sorted ascending by frame position
            //while there are frames to compare
            while(leftFrame!=leftChannel->end() && rightFrame!=rightChannel->end())
            {
                //if positions match, interpolate
                if(leftFrame->first == rightFrame->first)
                {
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
                //gap in right channel
                if(leftFrame->first<rightFrame->first)
                {
                    //interpolate right sample at lefts position
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
                //gap in left channel
                if(rightFrame->first<leftFrame->first)
                {
                    //interpolate left sample at rights position
                    const T leftPhase = Interpolation::Linear((T)(leftFrame-1)->first, (leftFrame-1)->second.phase, (T)leftFrame->first, leftFrame->second.phase, (T)rightFrame->first);
                    const T leftAmp = Interpolation::Linear((T)(leftFrame-1)->first, (leftFrame-1)->second.amplitude, (T)leftFrame->first, leftFrame->second.amplitude, (T)rightFrame->first);
                    channel.emplace_back(rightFrame->first,
                                    Diginstrument::Component<T>(
                                        rightFrame->second.frequency,
                                        rightFrame->second.phase*rightWeight + leftPhase*leftWeight,
                                        rightFrame->second.amplitude*rightWeight + leftAmp*leftWeight
                                    ));
                    rightFrame++;
                    continue;
                }
            }
            //TODO: remainder, in case one channel is shorter
            //NOTE: this could be a problem, as when we synthesise piece-by-piece, we will just give back the longer one attenuated
            res.push_back(std::move(channel));
            leftChannel++;
            rightChannel++;
            continue;
        }
    }
    //TODO: remainder, we have to account for channels left in one neighbour when the other neighbour has reached end

    //TODO: labels, not really neccessary
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