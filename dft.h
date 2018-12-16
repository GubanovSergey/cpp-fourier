#ifndef __DFT_H
#define __DFT_H

#include "global.h"
#include "policies.h"
#include "traits.h"

#include <complex>
#include <vector>
#include <algorithm>
//#include <iostream>

namespace DFT {

template <
    class Impl,                                 
//some Transformation deriving TransformerBase and using its fields and methods
    class TfTypes = DefaultTransformationTypes,
    class Conversion = ByImplicitConversion<typename TfTypes:: PointType>,    
//Functor to be called on elements of the input Sequence to transform them to PointType
    class Extension = NoExtension
//function to determine the size of coeffs_ needed to perform a transform by in size provided
>

//TODO abstract Impl to any functor
//TODO work out the link from forward to inverse transform
class TransformerBase: Extension, 
                       Conversion 
{
protected:
    //using SizeType = ContainerType<PointType>::size_type;
    using SizeType = size_t;
    using PointType = typename TfTypes::PointType;
    using Container = typename TfTypes::Container;
    using Conversion::convert;
    using Extension::calcSize;

    SizeType size_;
    SizeType in_size_;
    Container coeffs_;

private:
    static auto conversionWrapper() {
        return [](auto && in) {
            return convert(std::forward<decltype(in)>(in)); 
        };
    } 

    void prepare() {
        static_assert(std::is_same_v<typename Conversion:: ToType, PointType>);
        size_ = calcSize(in_size_);
        assert(size_ >= in_size_);
        coeffs_.reserve(size_);
       
        auto back_inserter = std::back_inserter(coeffs_);
        for (SizeType rem_fill = size_-in_size_; rem_fill > 0; --rem_fill) {
            (*back_inserter)++ = PointType();
        } 
    }

protected:
    template <typename RandomAccessIterator> 
    TransformerBase(RandomAccessIterator st, RandomAccessIterator fin): 
        in_size_(std::distance(st, fin)), 
        coeffs_()
    {
        if constexpr(std::is_same_v<
                typename std::iterator_traits<RandomAccessIterator>::value_type,
                PointType>) {
            std::copy(st, fin, std::back_inserter(coeffs_));
        } else {
            std::transform(st, fin, std::back_inserter(coeffs_), conversionWrapper());
        }
        prepare();
    }

    TransformerBase(Container && coeffs):
        in_size_(coeffs.size()),
        coeffs_(coeffs) 
    {
        prepare();    
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
