#ifndef __DFT_H
#define __DFT_H

#include "global.h"
#include "policies.h"
#include "traits.h"

#include <cassert>
//#include <functional>
#include <complex>
#include <vector>
#include <algorithm>

namespace DFT {

template <
    class Impl,                                 
//some Transformation deriving TransformerBase and using its fields and methods
    template <typename To> class Conversion = ByImplicitConversion,    
//Functor to be called on elements of the input Sequence to transform them to PointType
    ExtensionFunc Extension = NoExtension,
//function to determine the size of coeffs_ needed to perform a transform by in size provided
    class TfTypes = DefaultTransformationTypes
>

//TODO abstract Impl to any functor
//TODO work out the link from forward to inverse transform
class TransformerBase {
protected:
    //using SizeType = ContainerType<PointType>::size_type;
    using SizeType = size_t;
    using PointType = typename TfTypes::PointType;
    using Container = typename TfTypes::Container;

    SizeType size_;
    SizeType in_size_;
    Container coeffs_;

    template <typename RandomAccessIterator> 
    TransformerBase(RandomAccessIterator st, RandomAccessIterator fin): in_size_(fin-st), coeffs_()
    {
        size_ = Extension(in_size_);
        assert(size_ >= in_size_);
        coeffs_.reserve(size_);
       
        std::transform(st, fin, std::inserter(coeffs_, coeffs_.begin()), 
            [](auto && in) { 
                return Conversion<PointType>::convert(std::forward<decltype(in)>(in)); 
            } 
        );
        for (SizeType rem_fill = size_-in_size_; rem_fill > 0; --rem_fill) {
            auto back_inserter = std::back_inserter(coeffs_);
            (*back_inserter)++ = PointType();
        }  
    }

public:   
//TODO make a trick of in-PointType carrying to inverseTransform
    Container transform() {
        return static_cast<Impl *>(this)->doTransform();
    } 
    Container inverseTransform() {
        bool reverse = true;
        return static_cast<Impl *>(this)->doTransform(reverse);
    } 
};

};

#endif //__DFT_H
