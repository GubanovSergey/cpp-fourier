#include "dft_impls.h"
#include <type_traits>
#include <iostream>
#include <iterator>

using namespace DFT;

template<template <typename ...> class ContainerType, 
         typename T1, typename T2,
         class = typename std::enable_if<
            std::is_same_v<T1, T2>
         >::type 
        >
std::ostream& operator<< (std::ostream& out, const ContainerType<std::pair<T1, T2>>& v) {
    if ( !v.empty() ) { 
        out << '[';
        for (auto& elem: v) {
           out << "{" << elem.first << ", " << elem.second << "}, "; 
        } 
        out << "\b\b]";
    }
    return out;
}

template<typename DataType, template <typename ...> class ContainerType>
std::ostream& operator<< (std::ostream& out, const ContainerType<DataType>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<DataType>(out, ", "));
    out << "\b\b]";
  }
  return out;
}


int main() {    
//    vector<std::pair<int, int>> coeffs = {{-1, 0}, {0, -11}, {12,-3}, {1,0}, {2,-7}};
//    vector<complex<double>> coeffs = {{-1, 0.5}, {0, -0.13}, {0,0}, {1,0}, {16.47,0}};
  
    vector<complex<double>> coeffs = {{1}, {3}, {-8}};  
  //  std::cout << "src polynomial coeffs:\n" << coeffs << std::endl;   
    //FFT<ByCastConversion> transformer(coeffs.begin(), coeffs.end());//by copy
//    FFT<PairConversion> transformer(coeffs.begin(), coeffs.end());

    FFT<> transformer(std::move(coeffs));
    auto result = transformer.transform();
    std::cout << "fourier coeffs:\n" << result << std::endl;   
    
    FFT<> transformer2(std::make_move_iterator(result.begin()), std::make_move_iterator(result.end())); //by move
    //here we have result element type - complex<double>, so default ImplicitConvertion may be used
    auto backRes = transformer2.inverseTransform();
    std::cout << "revealed polynomial coeffs:\n" << backRes << std::endl;   
    return 0;
}
