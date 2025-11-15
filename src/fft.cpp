#include "fft.hpp"
#include <fftw3.h>

void fft2(const std::vector<double>& in,
          std::vector<cpx>& out,
          int rows, int cols)
{
    int nFreq = rows * (cols/2 + 1);

    fftw_complex* freq = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*nFreq);
    double* spatial = (double*) fftw_malloc(sizeof(double)*rows*cols);

    for (int i = 0; i < rows*cols; i++)
        spatial[i] = in[i];

    fftw_plan p = fftw_plan_dft_r2c_2d(rows, cols, spatial, freq, FFTW_ESTIMATE);
    fftw_execute(p);

    out.resize(nFreq);
    for (int i = 0; i < nFreq; i++)
        out[i] = cpx(freq[i][0], freq[i][1]);

    fftw_destroy_plan(p);
    fftw_free(freq);
    fftw_free(spatial);
}

void ifft2(const std::vector<cpx>& in,
           std::vector<double>& out,
           int rows, int cols)
{
    int nFreq = rows * (cols/2 + 1);

    fftw_complex* freq = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*nFreq);
    double* spatial = (double*) fftw_malloc(sizeof(double)*rows*cols);

    for (int i = 0; i < nFreq; i++) {
        freq[i][0] = in[i].real();
        freq[i][1] = in[i].imag();
    }

    fftw_plan p = fftw_plan_dft_c2r_2d(rows, cols, freq, spatial, FFTW_ESTIMATE);
    fftw_execute(p);

    out.resize(rows*cols);
    double scale = 1.0 / (rows*cols);

    for (int i = 0; i < rows*cols; i++)
        out[i] = spatial[i] * scale;

    fftw_destroy_plan(p);
    fftw_free(freq);
    fftw_free(spatial);
}
