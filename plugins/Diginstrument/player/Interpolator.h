#pragma once

#include "MultidimensionalNeighbourMap.hpp"
#include "../common/PartialSet.hpp"
#include "../common/ResidualByFrequency.hpp"
#include "../common/Interpolation.hpp"
#include "../common/PeakMatcher.h"
#include "../common/Dimension.h"

#include <algorithm>

namespace Diginstrument
{
template <typename T>
class Interpolator
{
public:
  PartialSet<T> getPartials(const std::vector<T> &coordinates, unsigned int startFrame, unsigned int frames);
  ResidualByFrequency<T> getResidual(const std::vector<T> &coordinates, unsigned int startFrame, unsigned int frames);

  void clear();

  void setDimensions(const std::vector<Dimension> & dimensions);
  const std::vector<Dimension> & getDimensions() const;

  void addPartialSets(const std::vector<PartialSet<T>> & partialSets);
  void addResiduals(const std::vector<ResidualByFrequency<T>> & partialSets);

  Interpolator() {}

private:  
  MultidimensionalNeighbourMap<T, ResidualByFrequency<T>> residual;
  MultidimensionalNeighbourMap<T, PartialSet<T>> partials;
  std::vector<Dimension> dimensions;

  PartialSet<T> interpolatePartialSet(const PartialSet<T> &left, const PartialSet<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const bool shifting);
  ResidualByFrequency<T> interpolateResidual(const ResidualByFrequency<T> &left, const ResidualByFrequency<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const bool shifting);
};
}; // namespace Diginstrument