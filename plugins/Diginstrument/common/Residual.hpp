#pragma once

#include <vector>

#include "Spectrum.hpp"

template <typename T>
class Residual
{
private:
    std::vector<std::pair<unsigned int, std::vector<Diginstrument::Component<T>>>> momentarySpectra;
    std::vector<std::pair<std::string, T>> labels;

public:
    //default constructor
    Residual() {}
    //empty constructor
    Residual(const std::vector<std::pair<std::string, T>> &labels) : labels(labels) {}
    //copy constructor
    Residual(const Residual<T> &other) : momentarySpectra(other.momentarySpectra), labels(other.labels) {}
    //move constructor
    Residual(Residual<T> &&other) : momentarySpectra(std::move(other.momentarySpectra)), labels(std::move(other.labels)) {}
    //parameters by reference
    Residual(const std::vector<std::pair<unsigned int, std::vector<Diginstrument::Component<T>>>> &spectra, const std::vector<std::pair<std::string, T>> &labels)
        : momentarySpectra(spectra), labels(labels) {}
    //move spectra
    Residual(std::vector<std::pair<unsigned int, std::vector<Diginstrument::Component<T>>>> &&spectra, const std::vector<std::pair<std::string, T>> &labels)
        : momentarySpectra(std::move(spectra)), labels(labels) {}
    //move spectra and labels
    Residual(std::vector<std::pair<unsigned int, std::vector<Diginstrument::Component<T>>>> &&spectra, std::vector<std::pair<std::string, T>> &&labels)
        : momentarySpectra(std::move(spectra)), labels(std::move(labels)) {}

    void add(const std::vector<Diginstrument::Component<T>> &spectrum, unsigned int frame)
    {
        momentarySpectra.push_back(std::make_pair(frame, spectrum));
    }

    void add(std::vector<Diginstrument::Component<T>> &&partial, unsigned int frame)
    {
        momentarySpectra.push_back(std::make_pair(frame, std::move(partial)));
    }

    const std::vector<std::pair<unsigned int, std::vector<Diginstrument::Component<T>>>> &get() const
    {
        return momentarySpectra;
    }

    std::vector<std::pair<std::string, T>> getLabels() const
    {
        return labels;
    }

    bool empty() const
    {
        return momentarySpectra.empty();
    }

    unsigned int size() const
    {
        return momentarySpectra.size();
    }

    Residual<T> getSlice(unsigned int startFrame, unsigned int frames) const
    {
        //TODO
        constexpr auto comparator = [](const std::pair<unsigned int, std::vector<Diginstrument::Component<T>>> &left, unsigned int right) -> bool {
            return left.first < right;
        };
        const auto itStart = std::lower_bound(momentarySpectra.begin(), momentarySpectra.end(), startFrame, comparator);
        const auto itEnd = std::lower_bound(momentarySpectra.begin(), momentarySpectra.end(), startFrame + frames, comparator);

        return Residual<T>(std::vector<std::pair<unsigned int, std::vector<Diginstrument::Component<T>>>>(itStart, itEnd), this->labels);
    }
};