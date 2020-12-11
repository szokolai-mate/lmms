#include "Synthesizer.h"

template <typename T>
std::vector<float> Diginstrument::Synthesizer<T>::playNote(const PartialSet<T> & slice, const unsigned int frames, const unsigned int offset, const unsigned int & sampleRate)
{
    std::vector<float> res(frames, 0);
    for(const auto & partial : slice.get())
    {
        for(int i = 0; i<partial.size(); i++)
        {
            res[i] += cos(partial[i].phase) * partial[i].amplitude;
        }
    }
    return res;
}

template <typename T>
std::vector<float> Diginstrument::Synthesizer<T>::playResidual(const Residual<T> & residual, const unsigned int frames, const unsigned int offset, const unsigned int & sampleRate)
{
    std::vector<float> res(frames, 0);
    for(const auto & channel : residual.get())
    {
        for(const auto & frame : channel)
        {
            if(frame.first<offset || frame.first>offset+frames)
            {
                //TODO: EXCEPTION - should not happen
            }
            else
            {
                res[frame.first-offset] += cos(frame.second.phase) * frame.second.amplitude; 
            }
        }
    }
    return res;
}

template class Diginstrument::Synthesizer<float>;