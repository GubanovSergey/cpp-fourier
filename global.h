#ifndef __DFT_GLOBAL_H
#define __DFT_GLOBAL_H
#include <vector>
#include <complex>
#include <cmath>


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

}

#endif //__DFT_GLOBAL_H

