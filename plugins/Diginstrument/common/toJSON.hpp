#pragma once

#include <nlohmann/json.hpp>
#include "Residual.hpp"
#include "Dimension.h"

using ordered_json = nlohmann::basic_json<nlohmann::ordered_map, std::vector, std::string, bool, std::int64_t, std::uint64_t, float>;

namespace Diginstrument
{
template <typename T>
class JSONConverter
{
  public:

    static ordered_json toJSON(const MomentarySpectrum<T> & spline)
    {
        ordered_json res;

        for(const auto & c : spline.getLabels())
        {
            res[c.first] = c.second;
        }

        ordered_json components = ordered_json::array();
        for(const auto & c : spline.getComponents(0))
        {
            ordered_json component = ordered_json::array();
            component.push_back(c.frequency);
            component.push_back(c.phase);
            component.push_back(c.amplitude);
            components.push_back(std::move(component));
        }
        res["components"] = components;
        
        return res;
    }

    static ordered_json toJSON(const PartialSet<T> & partialSet)
    {
        ordered_json res;
        for(auto & c : partialSet.getLabels())
        {
            res[c.first] = c.second;
        }

        res["sample_rate"] = partialSet.getSampleRate();

        res["partials"] = ordered_json::array();
        for(const auto & p : partialSet.get())
        {
            ordered_json partialObject;
            vector<T> phases, amps;
            partialObject["frequency"] = p.front().frequency;
            phases.reserve(p.size());
            amps.reserve(p.size());
            for(const auto & c : p)
            {
                phases.push_back(c.phase);
                amps.push_back(c.amplitude);
            }
            partialObject["phases"] = phases;
            partialObject["magnitudes"] = amps;
            res["partials"].push_back(std::move(partialObject));
        }
        return res;
    }

    static ordered_json toJSON(const Residual<T> & residual)
    {
        ordered_json res;
        for(auto & c : residual.getLabels())
        {
            res[c.first] = c.second;
        }

        res["residual_channels"] = ordered_json::array();
        for(const auto & s : residual.get())
        {
            ordered_json channelObject;
            channelObject["frequency"] = s.front().second.frequency;
            channelObject["frames"] = ordered_json::array();
            for(const auto & c : s)
            {
                channelObject["frames"].push_back({c.first, {c.second.phase, c.second.amplitude}});
            }
            res["residual_channels"].push_back(std::move(channelObject));
        }
        return res;
    }

    static ordered_json toJSON(const Diginstrument::Dimension & dimension)
    {
        ordered_json res;
        res["label"] = dimension.name;
        res["min"] = dimension.min;
        res["max"] = dimension.max;
        res["default"] = dimension.defaultValue;
        res["shifting"] = dimension.shifting;
        return res;
    }

    static ordered_json toJSON(
        std::string name,
        const std::vector<Diginstrument::Dimension> & dimensions,
        const std::vector<Residual<T>> & residuals,
        const std::vector<PartialSet<T>> & partials)
    {
        //TODO: "coordinates" not included, as they are not used anywhere anyway
        ordered_json res;

        res["residuals"] = ordered_json::array();
        for(const auto & r : residuals)
        {
           res["residuals"].push_back(toJSON(r));
        }

        res["dimensions"] = ordered_json::array();
        for(const auto & d : dimensions)
        {
           res["dimensions"].push_back(toJSON(d));
        }

        res["partial_sets"] = ordered_json::array();
        for(const auto & p : partials)
        {
           res["partial_sets"].push_back(toJSON(p));
        }

        res["type"] = "residual_channels";
        res["name"] = name;

        return res;
    }

    static ordered_json toJSON(
        std::string name,
        const std::vector<Diginstrument::Dimension> & dimensions,
        const std::vector<Diginstrument::MomentarySpectrum<T>> & spectra,
        const std::vector<PartialSet<T>> & partials)
    {
        //TODO: "coordinates" not included, as they are not used anywhere anyway
        ordered_json res;

        res["spectra"] = ordered_json::array();
        for(const auto & s : spectra)
        {
           res["spectra"].push_back(toJSON(s));
        }

        res["dimensions"] = ordered_json::array();
        for(const auto & d : dimensions)
        {
           res["dimensions"].push_back(toJSON(d));
        }

        res["partial_sets"] = ordered_json::array();
        for(const auto & p : partials)
        {
           res["partial_sets"].push_back(toJSON(p));
        }

        res["spectrum_type"] = "partials";
        res["name"] = name;

        return res;
    }

    static Residual<T> residualFromJSON(ordered_json object)
    {
        std::vector<std::vector<std::pair<unsigned int, Diginstrument::Component<T>>>> res;
        for(auto & channelObject : object["residual_channels"])
        {
            std::vector<std::pair<unsigned int, Diginstrument::Component<T>>> channel;
            channel.reserve(channelObject["frames"].size());
            for(const auto & frame : channelObject["frames"])
            {
                channel.push_back(
                    std::make_pair(
                        frame[0],
                        Diginstrument::Component<T>(
                            channelObject["frequency"],
                            frame[1][0],
                            frame[1][1]
                        )
                    )
                );
            }
            res.push_back(std::move(channel));
        }

        vector<pair<string, T>> labels;
        for(auto & e : object.items())
        {
            if(e.value().is_number()) labels.emplace_back(e.key(), e.value());
        }
       
        return Residual<T>(std::move(res), std::move(labels));
    }

    static PartialSet<T> partialSetFromJSON(ordered_json object)
    {
        vector<vector<Diginstrument::Component<T>>> partials;
        vector<pair<string, T>> labels;
        for(auto & p : object["partials"])
        {
            std::vector<Diginstrument::Component<T>> partial;
            partial.reserve(p["magnitudes"].size());
            for(int i = 0; i<p["magnitudes"].size(); i++)
            {
                partial.emplace_back(p["frequency"], p["phases"][i], p["magnitudes"][i]);
            }
            partials.push_back(std::move(partial));
        }
        for(auto & e : object.items())
        {
            if(e.value().is_number() && e.key()!="sample_rate") labels.emplace_back(e.key(), e.value());
        }
        return PartialSet<T>(std::move(partials), std::move(labels), object["sample_rate"]);
    }

    static Diginstrument::Dimension dimensionFromJSON(ordered_json object)
    {
        if(!object["default"].is_null()) return Diginstrument::Dimension(object["label"], object["min"], object["max"], object["shifting"]);
        else return Diginstrument::Dimension(object["label"], object["min"], object["max"], object["shifting"], object["default"]);
    }
    
};
};