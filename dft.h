#ifndef __DFT_H
#define __DFT_H

#include <cmath>
#include <cassert>
#include <functional>
#include <complex>
#include <vector>
#include <algorithm>

namespace DFT{

using std::vector;
using std::complex;
using Datatype=complex<double>;
template <typename T>
using Container=vector<T>;

constexpr double PI = std::acos(-1);

complex<double> fracUnityPow(double pow) {
    double angle = 2. * PI * pow;
    return complex<double>(std::cos(angle), std::sin(angle)); 
}


template <typename PointType, template<typename> class Impl>
class TransformerInterface {
protected:
    using SizeType = Container<Datatype>::size_type; 
    //using PointType
    SizeType size_;
    SizeType in_size_;
    //vector of coefficients for a polynom, f.e.
    //from lowest degree coefficient to the highest
    Container<Datatype> coeffs_;

    template <typename RandomAccessIterator> 
    TransformerInterface(RandomAccessIterator st, RandomAccessIterator fin): in_size_(fin-st), coeffs_()
    { 
        size_ = 1<<(1+int(std::log2(in_size_-1)));//nearest up power of 2
//TODO reserve may be segregated into policy (we may want not to extend coeffs). Along with extension with zeros in Impl class
        coeffs_.reserve(size_);
        
        auto back_inserter = std::back_inserter(coeffs_);
        for (auto cur = st; cur!=fin; ++cur) {
            (*back_inserter)++ = static_cast<Datatype>(*cur);
        }
        std::copy(st, fin, std::inserter(coeffs_, coeffs_.begin()));  
    }

public:   //TODO return result - Container <Datatype>
//TODO make a trick of in-PointType carrying to inverseTransform
//TODO think of putting the result to user-provided datatype
    Container<Datatype> transform() {
        return static_cast<Impl<PointType> *>(this)->doTransform();
    } //TODO check here
    Container<Datatype> inverseTransform() {
        bool reverse = true;
        return static_cast<Impl<PointType> *>(this)->doTransform(reverse);
    } 
};

}

#endif //__DFT_H
