#include "dft.h"
#include <iostream>
#include <iterator>

using namespace DFT;

template <typename ForwardIterator>
auto polynomialFuncGen = [](ForwardIterator coeffs_st, ForwardIterator coeffs_fin) { 
    return [coeffs_st, coeffs_fin](Datatype point) -> Datatype {
        Datatype result(0,0);
        Datatype pt_pow(1,0);
        for (auto cur = coeffs_st; cur != coeffs_fin; ++cur) {
            result += (*cur)*pt_pow;
            pt_pow *= point;
        }
        return result;
    };
};

std::ostream& operator<< (std::ostream& out, const Container<Datatype>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<Datatype>(out, ", "));
    out << "\b\b]";
  }
  return out;
}


int main() {
    vector<complex<double>> coeffs = {{-1, 0}, {0, 0}, {1,0}, {0,0}};
    std::cout << "src polynomial coeffs:\n" << coeffs << std::endl;   
    auto polynomialFunc = polynomialFuncGen<vector<complex<double>>::iterator>(coeffs.begin(), coeffs.end()); //x^2-1
    std::cout << "polynomial func(2):\n" << polynomialFunc({2,0}) << std::endl;   
    
    auto sz = coeffs.size();
    FourierTransformer<NaiveDFT> transformer(polynomialFunc,sz);
    
    auto result = transformer.transform().results();
    std::cout << "fourier coeffs:\n" << result << std::endl;   
    
    auto polynomialFuncBack = polynomialFuncGen<Container<Datatype>::iterator>(result.begin(), result.end());
    FourierTransformer<NaiveDFT> transformer2(polynomialFuncBack,sz);
    auto backRes = transformer2.inverseTransform().results();
    std::cout << "revealed polynomial coeffs:\n" << backRes << std::endl;   
    return 0;
}
