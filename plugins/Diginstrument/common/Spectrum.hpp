#pragma once

#include <vector>

namespace Diginstrument
{

template <typename T>
struct  Component{
  T frequency;
  T phase;
  T amplitude;

  Component(const T frequency, const T phase, const T amplitude) : frequency(frequency), phase(phase), amplitude(amplitude) {}
  Component(const Component<T> & other) : frequency(other.frequency), phase(other.phase), amplitude(other.amplitude) {}
  Component() : frequency(0), phase(0), amplitude(0) {}

  //sorting functors
  static constexpr auto sortByAmplitudeDescending = [] (const auto &left, const auto &right) -> bool { return left.amplitude >= right.amplitude; };
  static constexpr auto sortByFrequencyAscending = [] (const auto &left, const auto &right) -> bool { return left.frequency < right.frequency; };

  bool operator<(const Component<T> & other) const
  {
    return frequency<other.frequency;
  }
};

template <typename T>
class Spectrum
{
public:
  virtual std::vector<Component<T>> getComponents(const T quality) const = 0;
  virtual std::vector<Component<T>> getMatchables() const = 0;
  //TMP: removed these methods, as the new method doesn't need them
  //virtual std::vector<Component<T>> getUnmatchables() const = 0;
  //virtual Component<T> operator[](const T frequency) const = 0;
  virtual bool empty() const = 0;

  std::vector<std::pair<std::string, T>> getLabels() const
  {
    return labels;
  }

  bool operator<(const Spectrum<T> & other) const
  {
    if(labels.size()==0) return true;
    if(other.labels.size()==0) return false;
    return labels.front().second<other.labels.front().second;
  }

  Spectrum(const std::vector<std::pair<std::string, T>> & labels) : labels(labels){}

  std::vector<std::pair<std::string, T>> labels;
};

template <typename T>
class MomentarySpectrum : public Spectrum<T>
{
public:
  std::vector<Component<T>> getMatchables() const
  {
    return components;
  }
  
  std::vector<Component<T>> getComponents(const T quality) const
  {
    //TODO: use quality
    return components;
  }

  bool empty() const
  {
    return components.empty();
  }

  MomentarySpectrum(const std::vector<Component<T>> &components, const std::vector<std::pair<std::string, T>> &labels)
      :  Spectrum<T>(labels), components(components) {}
  MomentarySpectrum(std::vector<Component<T>> && components, const std::vector<std::pair<std::string, T>> &labels)
      :  Spectrum<T>(labels), components(std::move(components)) {}
  MomentarySpectrum(std::vector<Component<T>> && components, std::vector<std::pair<std::string, T>> &&labels)
      :  Spectrum<T>(std::move(labels)), components(std::move(components)) {}
  MomentarySpectrum() : Spectrum<T>({}) {}

private:
  std::vector<Component<T>> components;
};
} // namespace Diginstrument