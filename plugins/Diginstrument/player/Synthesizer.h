#pragma once

#include <vector>
#include <math.h>

#include "../common/Residual.hpp"
#include "../common/PartialSet.hpp"

namespace Diginstrument
{
template <typename T>
class Synthesizer
{
public:
  std::vector<float> playNote(const PartialSet<T> & slice, const unsigned int frames, const unsigned int offset, const unsigned int & sampleRate);
  std::vector<float> playResidual(const Residual<T> & residual, const unsigned int frames, const unsigned int offset, const unsigned int & sampleRate);
};
}; // namespace Diginstrument