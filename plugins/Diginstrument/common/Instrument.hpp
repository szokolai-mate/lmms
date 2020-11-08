#pragma once

#include "Residual.hpp"
#include "PartialSet.hpp"
#include "toJSON.hpp"
#include "Dimension.h"

namespace Diginstrument
{
template <typename T>
class Instrument
{
  public:
    void add(Residual<T> && residual)
    {
        this->residuals.push_back(std::move(residual));
    }

    void add(PartialSet<T> && partialSet)
    {
        partialSets.push_back(std::move(partialSet));
    }

    const vector<Residual<T>> & getResiduals() const
    {
        return residuals;
    }

    const vector<PartialSet<T>> & getPartialSets() const
    {
        return partialSets;
    }

    void reserve(unsigned int n)
    {
        residuals.reserve(n);
        partialSets.reserve(n);
    }

    void clear()
    {
        residuals.clear();
        partialSets.clear();
    }

    std::string toString(unsigned int spaces)
    {
        return Diginstrument::JSONConverter<T>::toJSON(name, dimensions, residuals, partialSets).dump(spaces);
    }

    static Diginstrument::Instrument<T> fromJSON(ordered_json object)
    {
        Diginstrument::Instrument<T> res;
        res.name = object["name"];
        res.dimensions.reserve(object["dimensions"].size());
        for(const auto & d : object["dimensions"])
        {
            res.dimensions.push_back(Diginstrument::JSONConverter<T>::dimensionFromJSON(d));
        }
        
        res.reserve(object["residuals"].size());
        for(const auto & s : object["residuals"])
        {
            res.add(Diginstrument::JSONConverter<T>::residualFromJSON(s));
        }
        res.reserve(object["partial_sets"].size());
        for(const auto & p : object["partial_sets"])
        {
            res.add(Diginstrument::JSONConverter<T>::partialSetFromJSON(p));
        }
        return res;
    }

    std::string name;
    std::vector<Diginstrument::Dimension> dimensions;

  private:
    std::vector<Residual<T>> residuals;
    std::vector<PartialSet<T>> partialSets;
};
};