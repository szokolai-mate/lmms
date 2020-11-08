#pragma once

#include <vector>
#include <fftw3.h>

namespace Diginstrument
{
template <typename T>
class FFT
{
  public:
    std::vector<std::pair<T, T>> operator()(const std::vector<double> & signal, unsigned int sampleRate)
    {
        //TODO: exception
        if(signal.size()!=in.size()) return {};
        std::copy(signal.begin(), signal.end(), in.begin());
        fftw_execute(plan);
        
        const unsigned int outSize = floor(N/2)+1;
        std::vector<std::pair<T, T>> res(outSize);
        for(int i = 0; i<outSize; i++)
        {
            const T re = out[i][0]/outSize;
            const T im = out[i][1]/outSize;
            res[i] = std::make_pair((T)i/((T)N/(T)sampleRate), sqrt(re*re + im*im));
        }
        return res;
    }

    FFT(unsigned int signalLength) : N(signalLength)
    {
        in.resize(N);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2+1));
        plan = fftw_plan_dft_r2c_1d(N, &in[0], out, FFTW_DESTROY_INPUT | FFTW_ESTIMATE);
    }

    ~FFT()
    {
        fftw_destroy_plan(plan);
        fftw_free(out);
        fftw_cleanup();
    }

  private:
    unsigned int N;
    fftw_plan plan;
    std::vector<double> in;
    fftw_complex * out;
};
}