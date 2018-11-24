#include "dft_impls.h"
#include <iostream>
#include <iterator>

using namespace DFT;

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
    vector<complex<int>> coeffs = {{-1, 0}, {0, -11}, {12,-3}, {1,0}, {2,-7}};
    //vector<complex<double>> coeffs = {{-1, 0.5}, {0, -0.13}, {0,0}, {1,0}, {16.47,0}};
    
    std::cout << "src polynomial coeffs:\n" << coeffs << std::endl;   
    FFT<> transformer(coeffs.begin(), coeffs.end());//by copy
    
    auto result = transformer.transform();
    std::cout << "fourier coeffs:\n" << result << std::endl;   
    
    FFT<> transformer2(std::make_move_iterator(result.begin()), std::make_move_iterator(result.end())); //by move
    auto backRes = transformer2.inverseTransform();
    std::cout << "revealed polynomial coeffs:\n" << backRes << std::endl;   
    return 0;
}
