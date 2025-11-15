#include <iostream>
#include <vector>
#include <complex>
#include <string>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "fft.hpp"

using cpx = std::complex<double>;

enum Method { BLUR, TIKHONOV, WIENER };

int main(int argc, char** argv)
{
    if (argc < 6) {
        std::cout << "Usage:\n";
        std::cout << "./deconv input.jpg psf.png lambda blur|tik|wiener output.png\n";
        return 0;
    }

    std::string imgPath = argv[1];
    std::string psfPath = argv[2];
    double lambda = std::stod(argv[3]);
    std::string methodStr = argv[4];
    std::string outPath = argv[5];

    Method method;
    if (methodStr == "blur") method = BLUR;
    else if (methodStr == "tik") method = TIKHONOV;
    else method = WIENER;

    int w1, h1, c1;
    unsigned char* imgData = stbi_load(imgPath.c_str(), &w1, &h1, &c1, 1);
    if (!imgData) { std::cerr << "Error loading input image\n"; return 1; }

    int w2, h2, c2;
    unsigned char* psfData = stbi_load(psfPath.c_str(), &w2, &h2, &c2, 1);
    if (!psfData) { std::cerr << "Error loading PSF\n"; return 1; }

    int rows = h1, cols = w1;

    // Convert to double
    std::vector<double> I(rows * cols);
    for (int i = 0; i < rows * cols; i++)
        I[i] = imgData[i] / 255.0;

    std::vector<double> H(rows * cols, 0.0);
    for (int y = 0; y < h2; y++)
        for (int x = 0; x < w2; x++)
            H[y * cols + x] = psfData[y*w2 + x] / 255.0;

    // Normalize PSF
    double sum = 0;
    for (double v : H) sum += v;
    if (sum > 0)
        for (double& v : H) v /= sum;

    // FFT
    std::vector<cpx> F_I, F_H;
    fft2(I, F_I, rows, cols);
    fft2(H, F_H, rows, cols);

    std::vector<cpx> F_OUT = F_I; // placeholder

    if (method == BLUR)
    {
        for (int i = 0; i < (int)F_I.size(); i++)
            F_OUT[i] = F_I[i] * F_H[i];
    }
    else 
    {
        for (int i = 0; i < (int)F_I.size(); i++) {
            double Hmag2 = std::norm(F_H[i]);
            cpx numerator = std::conj(F_H[i]) * F_I[i];
            F_OUT[i] = numerator / (Hmag2 + lambda);
        }
    }

    // IFFT
    std::vector<double> result;
    ifft2(F_OUT, result, rows, cols);

    // Clamp output
    std::vector<unsigned char> out(rows * cols);
    for (int i = 0; i < rows * cols; i++)
        out[i] = (unsigned char)(std::clamp(result[i], 0.0, 1.0) * 255);

    stbi_write_png(outPath.c_str(), cols, rows, 1, out.data(), cols);

    stbi_image_free(imgData);
    stbi_image_free(psfData);

    return 0;
}
