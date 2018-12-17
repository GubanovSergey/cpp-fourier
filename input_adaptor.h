#ifndef __DFT_INPUT_H
#define __DFT_INPUT_H

#include "global.h"
#include "policies.h"
#include "traits.h"

#include <complex>
#include <vector>
#include <algorithm>
//TODO make a trick of in-PointType carrying to inverseTransform

namespace DFT {

template <
    class TfTypes = DefaultTransformationTypes,
    class Conversion = ByImplicitConversion<typename TfTypes:: PointType>,    
//Functor to be called on elements of the input Sequence to transform them to PointType
    class Extension = NoExtension
//function to determine the size of coeffs_ needed to perform a transform by in size provided
>

//TODO work out the link from forward to inverse transform
class InputAdaptor: Extension, 
                    Conversion 
{
protected:
    //using SizeType = ContainerType<PointType>::size_type;
    using SizeType = size_t;
    using PointType = typename TfTypes::PointType;
    using Container = typename TfTypes::Container;
    using Conversion::convert;
    using Extension::calcSize;

private:
    static auto conversionWrapper() {
        return [](auto && in) {
            return convert(std::forward<decltype(in)>(in)); 
        };
    } 

    static void extend(Container & coeffs) {
        static_assert(std::is_same_v<typename Conversion:: ToType, PointType>);
        auto in_size = std::distance(coeffs.begin(), coeffs.end());
        auto size = calcSize(in_size);
        assert(size >= in_size);
        coeffs.reserve(size);
       
        auto back_inserter = std::back_inserter(coeffs);
        for (SizeType rem_fill = size-in_size; rem_fill > 0; --rem_fill) {
            (*back_inserter)++ = PointType();
        } 
    }

protected:
    template <typename RandomAccessIterator> 
    static Container 
    adaptData(RandomAccessIterator st, RandomAccessIterator fin) {
        SizeType in_size = std::distance(st, fin); 
        Container coeffs;
        if constexpr(std::is_same_v<
                typename std::iterator_traits<RandomAccessIterator>::value_type,
                PointType>) {
            std::copy(st, fin, std::back_inserter(coeffs));
        } else {
            std::transform(st, fin, std::back_inserter(coeffs), conversionWrapper());
        }
        extend(coeffs);
        return coeffs;
    }

    static Container adaptData (Container && in) {
        Container coeffs = in;
        extend(coeffs);
        return coeffs;    
    }
};

};

#endif //__DFT_INPUT_H
