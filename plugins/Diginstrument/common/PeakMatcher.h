#pragma once

#include <cmath>
#include <vector>
#include <set>
#include <algorithm>

#include "Spectrum.hpp"

namespace Diginstrument
{
struct Match
{
    unsigned int left;
    unsigned int right;
    double distance;

    Match(unsigned int left, unsigned int right, double distance): left(left), right(right), distance(distance){}
};

template <typename T>
class PeakMatcher
{
private:
    static std::vector<Match> makeAllSortedMatches(const std::vector<Diginstrument::Component<T>> & leftComponents,
                                             const std::vector<Diginstrument::Component<T>> & rightComponents,
                                             std::function<double(const Diginstrument::Component<T>&, const Diginstrument::Component<T>&)> distanceFunction
                                            );
public:
    static std::vector<Match> matchPeaks(const std::vector<Diginstrument::Component<T>> & leftComponents,
                                         const std::vector<Diginstrument::Component<T>> & rightComponents,
                                         std::function<double(const Diginstrument::Component<T>&, const Diginstrument::Component<T>&)> distanceFunction
                                         );
    static std::vector<Match> matchPeaks(const std::vector<Diginstrument::Component<T>> & leftComponents,
                                         const std::vector<Diginstrument::Component<T>> & rightComponents,
                                         std::vector<unsigned int> & leftUnmatched,
                                         std::vector<unsigned int> & rightUnmatched,
                                         std::function<double(const Diginstrument::Component<T>&, const Diginstrument::Component<T>&)> distanceFunction,
                                         double threshold = 0
                                         );
};
} // namespace Diginstrument