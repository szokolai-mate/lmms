#pragma once

#include "MultidimensionalNeighbourMap.hpp"
#include "../common/PartialSet.hpp"
#include "../common/Residual.hpp"
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
  Residual<T> getResidual(const std::vector<T> &coordinates, unsigned int startFrame, unsigned int frames);

  void clear();

  void setDimensions(const std::vector<Dimension> & dimensions);
  const std::vector<Dimension> & getDimensions() const;

  void addPartialSets(const std::vector<PartialSet<T>> & partialSets);
  void addResiduals(const std::vector<Residual<T>> & partialSets);

  Interpolator() {}

private:  
  MultidimensionalNeighbourMap<T, Residual<T>> residual;
  MultidimensionalNeighbourMap<T, PartialSet<T>> partials;
  std::vector<Dimension> dimensions;

  PartialSet<T> interpolatePartialSet(const PartialSet<T> &left, const PartialSet<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const bool shifting);
  Residual<T> interpolateResidual(const Residual<T> &left, const Residual<T> &right, const T &target, const T &leftLabel, const T &rightLabel, const bool shifting);
};
}; // namespace Diginstrument