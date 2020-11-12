
#include "utils.h"

#include "globals.h"

extern State state;
#include <chrono>
#include <complex>
#include <iostream>
#include <thread>
float filter_2_low_01_real(float xn) {
  static float yn1 = (0.f);
  static float yn2 = (0.f);
  static float xn1 = (0.f);
  static float xn2 = (0.f);
  float yn = (0.f);
  yn = (((((8.664870e-4f)) * (xn))) + ((((1.7326780e-3f)) * (xn1))) +
        ((((8.663387e-4f)) * (xn2))) + ((((1.919129f)) * (yn1))) +
        ((((-0.9225943f)) * (yn2))));
  xn2 = xn1;
  xn1 = xn;
  yn2 = yn1;
  yn1 = yn;
  return yn;
}
float filter_2_low_10_real(float xn) {
  static float yn1 = (0.f);
  static float yn2 = (0.f);
  static float xn1 = (0.f);
  static float xn2 = (0.f);
  float yn = (0.f);
  yn = (((((6.37280e-2f)) * (xn))) + ((((0.1274560f)) * (xn1))) +
        ((((6.37280e-2f)) * (xn2))) + ((((1.1943650f)) * (yn1))) +
        ((((-0.4492774f)) * (yn2))));
  xn2 = xn1;
  xn1 = xn;
  yn2 = yn1;
  yn1 = yn;
  return yn;
}
float filter_2_low_40_real(float xn) {
  static float yn1 = (0.f);
  static float yn2 = (0.f);
  static float xn1 = (0.f);
  static float xn2 = (0.f);
  float yn = (0.f);
  yn = (((((0.6362308f)) * (xn))) + ((((1.2724620f)) * (xn1))) +
        ((((0.6362308f)) * (xn2))) + ((((-1.125379f)) * (yn1))) +
        ((((-0.4195441f)) * (yn2))));
  xn2 = xn1;
  xn1 = xn;
  yn2 = yn1;
  yn1 = yn;
  return yn;
}
float filter_2_high_01_real(float xn) {
  static float yn1 = (0.f);
  static float yn2 = (0.f);
  static float xn1 = (0.f);
  static float xn2 = (0.f);
  float yn = (0.f);
  yn = (((((0.9567529f)) * (xn))) + ((((-1.9135060f)) * (xn1))) +
        ((((0.9567529f)) * (xn2))) + ((((1.9114370f)) * (yn1))) +
        ((((-0.9155749f)) * (yn2))));
  xn2 = xn1;
  xn1 = xn;
  yn2 = yn1;
  yn1 = yn;
  return yn;
}
float filter_2_high_40_real(float xn) {
  static float yn1 = (0.f);
  static float yn2 = (0.f);
  static float xn1 = (0.f);
  static float xn2 = (0.f);
  float yn = (0.f);
  yn = (((((6.37280e-2f)) * (xn))) + ((((-0.1274560f)) * (xn1))) +
        ((((6.37280e-2f)) * (xn2))) + ((((-1.1943650f)) * (yn1))) +
        ((((-0.4492774f)) * (yn2))));
  xn2 = xn1;
  xn1 = xn;
  yn2 = yn1;
  yn1 = yn;
  return yn;
}
std::complex<float> filter_2_low_01_complex(std::complex<float> xn) {
  auto yn1 = std::complex<float>((0.f), (0.f));
  auto yn2 = std::complex<float>((0.f), (0.f));
  auto xn1 = std::complex<float>((0.f), (0.f));
  auto xn2 = std::complex<float>((0.f), (0.f));
  auto yn = std::complex<float>((0.f), (0.f));
  yn = (((((8.664870e-4f)) * (xn))) + ((((1.7326780e-3f)) * (xn1))) +
        ((((8.663387e-4f)) * (xn2))) + ((((1.919129f)) * (yn1))) +
        ((((-0.9225943f)) * (yn2))));
  xn2 = xn1;
  xn1 = xn;
  yn2 = yn1;
  yn1 = yn;
  return yn;
}
std::complex<float> filter_2_low_10_complex(std::complex<float> xn) {
  auto yn1 = std::complex<float>((0.f), (0.f));
  auto yn2 = std::complex<float>((0.f), (0.f));
  auto xn1 = std::complex<float>((0.f), (0.f));
  auto xn2 = std::complex<float>((0.f), (0.f));
  auto yn = std::complex<float>((0.f), (0.f));
  yn = (((((6.37280e-2f)) * (xn))) + ((((0.1274560f)) * (xn1))) +
        ((((6.37280e-2f)) * (xn2))) + ((((1.1943650f)) * (yn1))) +
        ((((-0.4492774f)) * (yn2))));
  xn2 = xn1;
  xn1 = xn;
  yn2 = yn1;
  yn1 = yn;
  return yn;
}
std::complex<float> filter_2_low_40_complex(std::complex<float> xn) {
  auto yn1 = std::complex<float>((0.f), (0.f));
  auto yn2 = std::complex<float>((0.f), (0.f));
  auto xn1 = std::complex<float>((0.f), (0.f));
  auto xn2 = std::complex<float>((0.f), (0.f));
  auto yn = std::complex<float>((0.f), (0.f));
  yn = (((((0.6362308f)) * (xn))) + ((((1.2724620f)) * (xn1))) +
        ((((0.6362308f)) * (xn2))) + ((((-1.125379f)) * (yn1))) +
        ((((-0.4195441f)) * (yn2))));
  xn2 = xn1;
  xn1 = xn;
  yn2 = yn1;
  yn1 = yn;
  return yn;
}
std::complex<float> filter_2_high_01_complex(std::complex<float> xn) {
  auto yn1 = std::complex<float>((0.f), (0.f));
  auto yn2 = std::complex<float>((0.f), (0.f));
  auto xn1 = std::complex<float>((0.f), (0.f));
  auto xn2 = std::complex<float>((0.f), (0.f));
  auto yn = std::complex<float>((0.f), (0.f));
  yn = (((((0.9567529f)) * (xn))) + ((((-1.9135060f)) * (xn1))) +
        ((((0.9567529f)) * (xn2))) + ((((1.9114370f)) * (yn1))) +
        ((((-0.9155749f)) * (yn2))));
  xn2 = xn1;
  xn1 = xn;
  yn2 = yn1;
  yn1 = yn;
  return yn;
}
std::complex<float> filter_2_high_40_complex(std::complex<float> xn) {
  auto yn1 = std::complex<float>((0.f), (0.f));
  auto yn2 = std::complex<float>((0.f), (0.f));
  auto xn1 = std::complex<float>((0.f), (0.f));
  auto xn2 = std::complex<float>((0.f), (0.f));
  auto yn = std::complex<float>((0.f), (0.f));
  yn = (((((6.37280e-2f)) * (xn))) + ((((-0.1274560f)) * (xn1))) +
        ((((6.37280e-2f)) * (xn2))) + ((((-1.1943650f)) * (yn1))) +
        ((((-0.4492774f)) * (yn2))));
  xn2 = xn1;
  xn1 = xn;
  yn2 = yn1;
  yn1 = yn;
  return yn;
}