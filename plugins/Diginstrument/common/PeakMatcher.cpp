#include "PeakMatcher.h"

//tmp
#include <iostream>

std::vector<Diginstrument::Match> Diginstrument::PeakMatcher::makeAllSortedMatches(
    const std::vector<Diginstrument::Component<double>> & leftComponents,
    const std::vector<Diginstrument::Component<double>> & rightComponents,
    std::function<double(const Diginstrument::Component<double>&, const Diginstrument::Component<double>&)> distanceFunction
    )
{
    //TODO: include a limit?
    std::vector<Match> allMatches;
    allMatches.reserve(leftComponents.size()*rightComponents.size());
    //calculate distances of all potential matches
    for(int lIndex = 0; lIndex<leftComponents.size(); lIndex++)
    {
        for(int rIndex = 0; rIndex<rightComponents.size(); rIndex++)
        {   
            const double distance = distanceFunction(leftComponents[lIndex], rightComponents[rIndex]);
            allMatches.emplace_back(lIndex, rIndex, distance);
        }
    }
    //sort matches by distance
    std::sort(allMatches.begin(), allMatches.end(), [](const Match &left, const Match &right) { return left.distance < right.distance; });
    return allMatches;
}

std::vector<Diginstrument::Match> Diginstrument::PeakMatcher::matchPeaks(
    const std::vector<Diginstrument::Component<double>> & leftComponents,
    const std::vector<Diginstrument::Component<double>> & rightComponents,
    std::function<double(const Diginstrument::Component<double>&, const Diginstrument::Component<double>&)> distanceFunction
    )
{
    //TODO: can the cycle abort if we find a low number? or if the number is higher than the previous?
    //maybe: frequency diff. limit?
    //DONE: what if amounts of peaks dont match?
    //DONE: it must be unique on both left AND right
    //TODO: test results
    //TODO: bug hunt: the "0 ratio split" is caused by mismatching a neighbouring(?) pair (example: ~500-~700;~700-~500)
    //->they slide onto eachother: they change order ... maybe this can even work?
    //CONCLUSION: this in not a bug, but a feature. need to make it work (if the spline is too small, remove it)
    
    auto allMatches = makeAllSortedMatches(leftComponents, rightComponents, distanceFunction);

    std::set<unsigned int> chosenLeftPeaks;
    std::set<unsigned int> chosenRightPeaks;
    std::vector<Match> matches;
    int i = 0;
    //pick a match with peaks that have no matches yet
    //while neither side has all its peaks matched and we have matches left
    while( chosenLeftPeaks.size()!=leftComponents.size() && chosenRightPeaks.size()!=rightComponents.size() && i<allMatches.size())
    {
        const Match & match = allMatches[i];
        //if neither peak was chosen already
        if (chosenLeftPeaks.find(match.left) == chosenLeftPeaks.end() && chosenRightPeaks.find(match.right) == chosenRightPeaks.end())
        {
            //add the match and the peaks
            matches.push_back(match);
            chosenLeftPeaks.emplace(match.left);
            chosenRightPeaks.emplace(match.right);
        }
        i++;
    }

    return matches;
}

//TODO: refactor
std::vector<Diginstrument::Match> Diginstrument::PeakMatcher::matchPeaks(const std::vector<Diginstrument::Component<double>> & leftComponents,
                                         const std::vector<Diginstrument::Component<double>> & rightComponents,
                                         std::vector<unsigned int> & leftUnmatched,
                                         std::vector<unsigned int> & rightUnmatched,
                                         std::function<double(const Diginstrument::Component<double>&, const Diginstrument::Component<double>&)> distanceFunction,
                                         double threshold
)
{
    auto allMatches = makeAllSortedMatches(leftComponents, rightComponents, distanceFunction);
    std::set<unsigned int> chosenLeftPeaks;
    std::set<unsigned int> chosenRightPeaks;
    std::set<unsigned int> unmatchedLeft;
    for(int i = 0 ; i<leftComponents.size(); i++)
    {
        unmatchedLeft.emplace(i);
    }
    std::set<unsigned int> unmatchedRight;
    for(int i = 0 ; i<rightComponents.size(); i++)
    {
        unmatchedRight.emplace(i);
    }
    std::vector<Match> matches;
    int i = 0;
    //pick a match with peaks that have no matches yet
    //while neither side has all its peaks matched and we have matches left
    while( chosenLeftPeaks.size()!=leftComponents.size() && chosenRightPeaks.size()!=rightComponents.size() && i<allMatches.size())
    {
        const Match & match = allMatches[i];
        //if neither peak was chosen already and the match reaches the threshold
        if (match.distance<=threshold && chosenLeftPeaks.find(match.left) == chosenLeftPeaks.end() && chosenRightPeaks.find(match.right) == chosenRightPeaks.end())
        {
            //add the match and the peaks
            matches.push_back(match);
            unmatchedLeft.erase(match.left);
            unmatchedRight.erase(match.right);
            chosenLeftPeaks.emplace(match.left);
            chosenRightPeaks.emplace(match.right);
        }
        i++;
    }
    leftUnmatched.reserve(unmatchedLeft.size()+leftUnmatched.size());
    for (auto it = unmatchedLeft.begin(); it != unmatchedLeft.end(); )
    {
        leftUnmatched.push_back(std::move(unmatchedLeft.extract(it++).value()));
    }
    rightUnmatched.reserve(unmatchedRight.size()+rightUnmatched.size());
    for (auto it = unmatchedRight.begin(); it != unmatchedRight.end(); )
    {
        rightUnmatched.push_back(std::move(unmatchedRight.extract(it++).value()));
    }
    return matches;
}                                         