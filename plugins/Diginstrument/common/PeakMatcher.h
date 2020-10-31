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

class PeakMatcher
{
private:
    static std::vector<Match> makeAllSortedMatches(const std::vector<Diginstrument::Component<double>> & leftComponents,
                                             const std::vector<Diginstrument::Component<double>> & rightComponents,
                                             std::function<double(const Diginstrument::Component<double>&, const Diginstrument::Component<double>&)> distanceFunction
                                            );
public:
    static std::vector<Match> matchPeaks(const std::vector<Diginstrument::Component<double>> & leftComponents,
                                         const std::vector<Diginstrument::Component<double>> & rightComponents,
                                         std::function<double(const Diginstrument::Component<double>&, const Diginstrument::Component<double>&)> distanceFunction
                                         );
    static std::vector<Match> matchPeaks(const std::vector<Diginstrument::Component<double>> & leftComponents,
                                         const std::vector<Diginstrument::Component<double>> & rightComponents,
                                         std::vector<unsigned int> & leftUnmatched,
                                         std::vector<unsigned int> & rightUnmatched,
                                         std::function<double(const Diginstrument::Component<double>&, const Diginstrument::Component<double>&)> distanceFunction,
                                         double threshold = 0
                                         );
};
} // namespace Diginstrument