#ifndef VIS_02_FILTERS_H
#define VIS_02_FILTERS_H
#include "utils.h"
;
#include "globals.h"
;
#include <chrono>
#include <complex>
#include <iostream>
#include <thread>
;
float filter_2_low_01_real(float xn);
float filter_2_low_10_real(float xn);
float filter_2_low_40_real(float xn);
float filter_2_high_01_real(float xn);
float filter_2_high_40_real(float xn);
std-- complex<float> filter_2_low_01_complex(std::complex<float> xn);
std-- complex<float> filter_2_low_10_complex(std::complex<float> xn);
std-- complex<float> filter_2_low_40_complex(std::complex<float> xn);
std-- complex<float> filter_2_high_01_complex(std::complex<float> xn);
std-- complex<float> filter_2_high_40_complex(std::complex<float> xn);
#endif