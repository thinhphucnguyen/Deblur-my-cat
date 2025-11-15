cmake_minimum_required(VERSION 3.10)
project(image_deconvolution)

set(CMAKE_CXX_STANDARD 17)

find_package(FFTW3 REQUIRED)

include_directories(${FFTW3_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Manual path fix for macOS Homebrew (FFTW)
set(FFTW3_INCLUDE_DIR /opt/homebrew/include)
set(FFTW3_LIBRARY /opt/homebrew/lib/libfftw3.dylib)

include_directories(${FFTW3_INCLUDE_DIR})
link_directories(/opt/homebrew/lib)

add_executable(deconv
    src/main.cpp
    src/fft.cpp
)

target_link_libraries(deconv ${FFTW3_LIBRARY})
