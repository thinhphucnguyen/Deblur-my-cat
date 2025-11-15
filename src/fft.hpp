#ifndef FFT_HPP
#define FFT_HPP

#include <vector>
#include <complex>

using cpx = std::complex<double>;

void fft2(const std::vector<double>& in,
          std::vector<cpx>& out,
          int rows, int cols);

void ifft2(const std::vector<cpx>& in,
           std::vector<double>& out,
           int rows, int cols);

#endif
