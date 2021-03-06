#include "PeakMatcher.h"

template <typename T>
std::vector<Diginstrument::Match> Diginstrument::PeakMatcher<T>::makeAllSortedMatches(
    const std::vector<Diginstrument::Component<T>> & leftComponents,
    const std::vector<Diginstrument::Component<T>> & rightComponents,
    std::function<double(const Diginstrument::Component<T>&, const Diginstrument::Component<T>&)> distanceFunction
    )
{
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

template <typename T>
std::vector<Diginstrument::Match> Diginstrument::PeakMatcher<T>::matchPeaks(const std::vector<Diginstrument::Component<T>> & leftComponents,
                                         const std::vector<Diginstrument::Component<T>> & rightComponents,
                                         std::vector<unsigned int> & leftUnmatched,
                                         std::vector<unsigned int> & rightUnmatched,
                                         std::function<double(const Diginstrument::Component<T>&, const Diginstrument::Component<T>&)> distanceFunction,
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

template class Diginstrument::PeakMatcher<float>;
template class Diginstrument::PeakMatcher<double>;