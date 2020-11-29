#pragma once

#include <vector>

#include "Spectrum.hpp"

template <typename T>
class Residual
{
private:
    std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<T>>>> channels;
    std::vector<std::pair<std::string, T>> labels;

public:
    //default constructor
    Residual() {}
    //empty constructor
    Residual(const std::vector<std::pair<std::string, T>> &labels) : labels(labels) {}
    //copy constructor
    Residual(const Residual<T> &other) : channels(other.channels), labels(other.labels) {}
    //move constructor
    Residual(Residual<T> &&other) : channels(std::move(other.channels)), labels(std::move(other.labels)) {}
    //parameters by reference
    Residual(const std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<T>>>> &channels, const std::vector<std::pair<std::string, T>> &labels)
        : channels(channels), labels(labels) {}
    //move channels
    Residual(std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<T>>>> &&channels, const std::vector<std::pair<std::string, T>> &labels)
        : channels(std::move(channels)), labels(labels) {}
    //move channels and labels
    Residual(std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<T>>>> &&channels, std::vector<std::pair<std::string, T>> &&labels)
        : channels(std::move(channels)), labels(std::move(labels)) {}

    void add(const std::vector<std::pair<unsigned int, Diginstrument::Component<T>>> & channel)
    {
        channels.push_back(channel);
    }

    void add(std::vector<std::pair<unsigned int, Diginstrument::Component<T>>> && channel)
    {
        channels.push_back(std::move(channel));
    }

    const std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<T>>>> &get() const
    {
        return channels;
    }

    std::vector<std::pair<std::string, T>> getLabels() const
    {
        return labels;
    }

    bool empty() const
    {
        return channels.empty();
    }

    Residual<T> getSlice(unsigned int startFrame, unsigned int frames) const
    {
        //TODO: slicing neccessity detection
        constexpr auto comparator = [](const std::pair<unsigned int, Diginstrument::Component<T>> &left, unsigned int right) -> bool {
            return left.first < right;
        };
        std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<T>>>> res;
        for(const auto & channel : channels)
        {
            auto itStart = std::lower_bound(channel.begin(), channel.end(), startFrame, comparator);
            auto itEnd = std::lower_bound(channel.begin(), channel.end(), startFrame + frames, comparator);

            //if the slice is incomplete, add the closest missing point
            if(itStart!=channel.begin() && itStart->first>startFrame) itStart--;
            if(itEnd!=channel.end() && itEnd->first<startFrame+frames) itEnd++;
            //if start is higher than end frame, give start only
            if(itStart!=channel.end() && itStart->first>startFrame+frames)
            {
                res.push_back({*itStart});
                continue;
            }
            //if start frame is higher than the last available frame, give that
            if(startFrame>channel.back().first)
            {
                res.push_back({channel.back()});
                continue;
            }
            //if first available frame is higher than end frame, give that
            if(startFrame+frames<=itStart->first)
            {
                res.push_back({channel.front()});
                continue;
            }

            res.push_back(std::vector<std::pair<unsigned int, Diginstrument::Component<T>>>(itStart, itEnd));
        }
        return Residual<T>(std::move(res), this->labels);
    }
};