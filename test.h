#ifndef __DFT_TEST_H
#define __DFT_TEST_H

#include "global.h"
#include "dft_impls.h"

void case1 () {
    using namespace DFT;
    vector<complex<double>> coeffs = {{1}, {3}, {-8}};  
    std::cout << "src polynomial coeffs:\n" << coeffs << std::endl;   

    FFT<> transformer(std::move(coeffs));
    auto result = transformer.transform();
    std::cout << "fourier coeffs:\n" << result << std::endl;   
    
    FFT<> transformer2(std::make_move_iterator(result.begin()), std::make_move_iterator(result.end())); //by element-wise move
    //here we have result element type - complex<double>, so default ImplicitConvertion may be used
    auto backRes = transformer2.inverseTransform();
    std::cout << "revealed polynomial coeffs:\n" << backRes << std::endl;   
}

void case2() {
    using namespace DFT;
    vector<std::pair<int, int>> coeffs = {{-1, 0}, {0, -11}, {12,-3}, {1,0}, {2,-7}};
    std::cout << "src polynomial coeffs:\n" << coeffs << std::endl;   
    
    FFT<PairConversion> transformer(coeffs.begin(), coeffs.end());
    auto result = transformer.transform();
    std::cout << "fourier coeffs:\n" << result << std::endl;   
    
    FFT<> transformer2(std::move(result)); 
    auto backRes = transformer2.inverseTransform();
    std::cout << "revealed polynomial coeffs:\n" << backRes << std::endl;   
}

void case3() {
    using namespace DFT;
    vector<int> coeffs = {1,2,3};
    std::cout << "src polynomial coeffs:\n" << coeffs << std::endl;   
    
    FFT<ByCastConversion> transformer(coeffs.begin(), coeffs.end());//by copy
    auto result = transformer.transform();
    std::cout << "fourier coeffs:\n" << result << std::endl;   
    
    FFT<> transformer2(std::move(result)); 
    auto backRes = transformer2.inverseTransform();
    std::cout << "revealed polynomial coeffs:\n" << backRes << std::endl;   
}

void test_main() {
    case1();
    std::cout << std::endl;
    case2();
    std::cout << std::endl;
    case3();
}

#endif //__DFT_TEST_H
