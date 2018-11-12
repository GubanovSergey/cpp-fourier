#include <iostream>
#include <complex>
#include <functional>

using std::complex;

namespace ConvMethod {
enum ConversionMethod {
    Default, Callback 
};

template <typename In, ConversionMethod Method = ConversionMethod::Default, typename ComplexFP = double>
class Conversion;

template <typename In, typename ComplexFP>
struct Conversion <In, ConversionMethod::Default, ComplexFP> {
    complex<ComplexFP> operator()(In& a) {
        std::cout << "in static conv" << std::endl;
        return static_cast<complex<ComplexFP>>(a);
    }
};

template <typename In, typename ComplexFP>
struct Conversion <In, ConversionMethod::Callback, ComplexFP>: public std::function<complex<ComplexFP>(In &&)> {};

};

struct A {
    operator complex<double>() { return {3,-0.15}; }
};  

int main() {
    using namespace ConvMethod;
    Conversion<complex<float>, Default> conv;

    complex<float> one(5, -2.5);
    conv(one);
    
    Conversion<complex<long double>, Default> conv1;
    complex <long double> two(1, -0.785);
    conv1(two);//TODO restrict

    A a;
    Conversion<A, Default> conv2;
    conv2(a);

    //std::pair<int, double> b(3, -6.5);
    //conversion(b);
    
    return 0;
}
