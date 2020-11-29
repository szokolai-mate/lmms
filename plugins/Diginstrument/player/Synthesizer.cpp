#include "Synthesizer.h"

//tmp
#include <iostream>

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
    for(const auto & r : residual.get())
    {
        for(const auto & c : r.second)
        {
            //tmp: debug
            std::cout<<std::fixed<<r.first<<" "<<c.frequency<<" "<<c.phase<<" "<<c.amplitude<<std::endl;
            res[r.first-offset] += cos(c.phase) * c.amplitude; 
        }
    }
    return res;
}

template <typename T>
std::vector<float> Diginstrument::Synthesizer<T>::playResidualByFrequency(const ResidualByFrequency<T> & residual, const unsigned int frames, const unsigned int offset, const unsigned int & sampleRate)
{
    std::vector<float> res(frames, 0);
    for(const auto & channel : residual.get())
    {
        for(const auto & frame : channel)
        {
            //tmp: debug
            //std::cout<<std::fixed<<frame.first<<" "<<frame.second.frequency<<" "<<frame.second.phase<<" "<<frame.second.amplitude<<std::endl;
            if(frame.first<offset || frame.first>offset+frames)
            {
                //TODO
                //tmp:
                //std::cout<<"UNIMPLEMENTED: residual out of bounds in synth"<<std::endl;
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