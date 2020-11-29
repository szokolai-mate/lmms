#pragma once

#include <vector>
#include <numeric>
#include <algorithm>

#include "Spectrum.hpp"

template <typename T>
class PartialSet
{
  private:
    std::vector<std::vector<Diginstrument::Component<T>>> partials;
    std::vector<std::pair<std::string, T>> labels;
    unsigned int sampleRate;

  public:
    //default constructor
    PartialSet(){}
    //empty constructor
    PartialSet(const std::vector<std::pair<std::string, T>> & labels, unsigned int sampleRate): labels(labels), sampleRate(sampleRate){}
    //copy constructor
    PartialSet(const PartialSet<T> & other) : partials(other.partials), labels(other.labels), sampleRate(other.sampleRate){}
    //move constructor
    PartialSet(PartialSet<T> && other) : partials(std::move(other.partials)), labels(std::move(other.labels)), sampleRate(other.sampleRate){}
    //parameters by reference
    PartialSet(const std::vector<std::vector<Diginstrument::Component<T>>> & partials, const std::vector<std::pair<std::string, T>> & labels, unsigned int sampleRate)
        : partials(partials), labels(labels), sampleRate(sampleRate){}
    //move partials
    PartialSet(std::vector<std::vector<Diginstrument::Component<T>>> && partials, const std::vector<std::pair<std::string, T>> & labels, unsigned int sampleRate)
        : partials(std::move(partials)), labels(labels), sampleRate(sampleRate){}
    //move partials and labels
    PartialSet(std::vector<std::vector<Diginstrument::Component<T>>> && partials, std::vector<std::pair<std::string, T>> && labels, unsigned int sampleRate)
        : partials(std::move(partials)), labels(std::move(labels)), sampleRate(sampleRate){}


    void add(const std::vector<Diginstrument::Component<T>> & partial)
    {
        partials.push_back(partial);
    }

    void add(std::vector<Diginstrument::Component<T>> && partial)
    {
        partials.push_back(std::move(partial));
    }

    const std::vector<std::vector<Diginstrument::Component<T>>> & get() const
    {
        return partials;
    }

    std::vector<std::pair<std::string, T>> getLabels() const
    {
        return labels;
    }

    unsigned int getSampleRate() const
    {
        return sampleRate;
    }

    bool empty() const
    {
        return partials.empty();
    }

    PartialSet<T> getSlice(unsigned int startFrame, unsigned int frames) const
    {
        //TODO: actually use sampleRate
        if(partials.size() == 0 || partials.front().size()==0 || startFrame>=partials.front().size()) return PartialSet<T>(this->labels, sampleRate);
        std::vector<std::vector<Diginstrument::Component<T>>> slice;
        for(const auto & p : this->partials)
        {
            if(p.size()<=startFrame)
            {
                continue;
            }
            if(p.size()<=startFrame+frames)
            {
                slice.emplace_back(p.begin()+startFrame, p.end());
            }
            else{ slice.emplace_back(p.begin()+startFrame, p.begin()+startFrame+frames); }
        }
        return PartialSet<T>(std::move(slice), this->labels, sampleRate);
    }

    std::vector<Diginstrument::Component<T>> getMatchables() const
    {
        std::vector<Diginstrument::Component<T>> res;
        res.reserve(this->partials.size());
        for(const auto & p : partials)
        {
            res.push_back(p.front());
        }
        return res;
    }

    T getFundamentalFrequency() const
    {
        constexpr T maxDistance = 0.1;
        //initialize
        std::vector<std::pair<T, unsigned int>> harmonicScores;
        harmonicScores.reserve(partials.size());
        for(const auto & partial: partials)
        {
            harmonicScores.emplace_back(partial.front().frequency, 0);
        }
        //sort by frequency
        std::sort(harmonicScores.begin(), harmonicScores.end(), [](const auto & left, const auto & right){return left.first<right.first;});
        //calculate overtone numbers
        for(int i = 0; i<harmonicScores.size(); i++)
        {
            //consider every higher frequency partial
            for(int j = i+1; j<harmonicScores.size(); j++)
            {
                //see how far they are from being an overtone
                const T ratio = harmonicScores[j].first / harmonicScores[i].first;
                const T distance = ratio - floor(ratio);
                if(distance<maxDistance) harmonicScores[i].second++;
            }
        }
        //sort by score
        std::sort(harmonicScores.begin(), harmonicScores.end(), [](const auto & left, const auto & right){return left.second>right.second;});
        return harmonicScores.front().first;
    }
};