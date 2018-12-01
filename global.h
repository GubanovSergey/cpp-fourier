#ifndef __DFT_GLOBAL_H
#define __DFT_GLOBAL_H
#include <type_traits>
#include <iostream>
#include <iterator>

#include <vector>
#include <complex>
#include <cmath>
#include <cassert>


namespace DFT{

using std::vector;
using std::complex;
//using ComplexFP=double;
//using Datatype=complex<ComplexFP>;
//using SizeType = Container<Datatype>::size_type; 

constexpr double PI = std::acos(-1);

complex<double> fracUnityPow(double pow) {
    double angle = 2. * PI * pow;
    return complex<double>(std::cos(angle), std::sin(angle)); 
}

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

}

#endif //__DFT_GLOBAL_H

