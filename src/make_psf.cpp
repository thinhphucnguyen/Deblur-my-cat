#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <cmath>
#include <vector>

int main() {
    int size = 64;
    double sigma = 6.0;  // blur amount

    std::vector<unsigned char> img(size * size);

    // Generate a Gaussian PSF
    double sum = 0.0;
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            double dx = x - size / 2;
            double dy = y - size / 2;
            double g = std::exp(-(dx*dx + dy*dy) / (2 * sigma * sigma));
            img[y * size + x] = (unsigned char)(g * 255);
            sum += g;
        }
    }

    // Normalize image
    for (int i = 0; i < size*size; i++) {
        img[i] = (unsigned char)((img[i] / sum) * 255.0f);
    }

    // Save PSF
    stbi_write_png("psf.png", size, size, 1, img.data(), size);

    return 0;
}
