Image Deconvolution (FFT Blur & Deblur)

This is a small personal project I built to practice concepts from my imaging class.
I wanted to understand how FFT-based blurring and deblurring actually work in code,
so I implemented a simple pipeline using C++, FFTW, and custom PSFs.

The project supports:

blur an image using a PSF (FFT convolution)

Tikhonov deconvolution

Wiener filtering

custom Gaussian PSF generator

Everything is kept lightweight: stb_image for I/O, FFTW for FFTs, and no external frameworks.

To run the code:
./deconv input.jpg psf.png lambda blur|tik|wiener output.png

Example: 
./deconv kitty.jpg psf.png 0.01 blur kitty_blur.png
./deconv kitty_blur.png psf.png 0.01 tik kitty_restore.png

Build:
brew install fftw
mkdir build && cd build
cmake ..
make
