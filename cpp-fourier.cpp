#include "dft_impls.h"
#include <iostream>
#include <iterator>

using namespace DFT;

std::ostream& operator<< (std::ostream& out, const Container<Datatype>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<Datatype>(out, ", "));
    out << "\b\b]";
  }
  return out;
}


int main() {
    vector<complex<double>> coeffs = {{-1, 0.5}, {0, -0.13}, {1,0}, {16.47,0}};
    std::cout << "src polynomial coeffs:\n" << coeffs << std::endl;   
    FourierTransformer<FFT> transformer(std::move(coeffs));
    
    auto result = transformer.transform().results();
    std::cout << "fourier coeffs:\n" << result << std::endl;   
    
    FourierTransformer<FFT> transformer2(std::move(result));
    auto backRes = transformer2.inverseTransform().results();
    std::cout << "revealed polynomial coeffs:\n" << backRes << std::endl;   
    return 0;
}
