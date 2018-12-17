#ifndef __DFT_IMPLS_H
#define __DFT_IMPLS_H

#include "global.h"
#include "policies.h"

//TODO finish the idea by implementing outputAdaptor, common transformation class and hiding convert(...) into the common interface


namespace DFT {
template <typename Transformation>
struct TfReqs {
    //typedef Extension;
};

// Example DFT class
template <
    class TfTs = DefaultTransformationTypes
>
class NaiveDFT {
public:
    using TfTypes=TfTs;
    using PointType = typename TfTypes::PointType;
    using Container = typename TfTypes::Container;

private:
    PointType countPolynom(Container & pts, PointType at) {
        PointType result(0,0);
        PointType pt_pow(1,0);
        for (auto cur: pts) {
            result += cur*pt_pow;
            pt_pow *= at;
        }
        return result;
    }

    PointType calcFourierCoeff(Container & pts, unsigned nCoeff, bool backward) {
        auto size = containerSize(pts) ;
        int sign = (backward) ? 1 : -1;
        int divider = (backward) ? size: 1;
        PointType nthFourierX = fracUnityPow(nCoeff * sign * 1. / size);
        return countPolynom(pts, nthFourierX) / PointType(divider, 0);
    }
public:
    Container doTransform(Container && in, bool inverse = false) {//TODO make private
        //naive O(n^2) transform
        Container coeffs = in;
        auto size = containerSize(coeffs);
        
        Container res;
        res.reserve(size);
        auto writeIter = std::inserter(res, res.begin());
        for (unsigned i = 0; i < size; i++) {
            (*writeIter)++ = calcFourierCoeff(coeffs, i, inverse);
        }
        return res;
    }
};

template <typename ... Ts>
struct TfReqs<NaiveDFT<Ts...>> {
    typedef NoExtension Extension;
};

/*
//Implement a Cooley–Tukey FFT algorithm with a 2-radix butterfly diagram
//TODO consider specializing "radixness" of a diagram to check perf improvement
template <
    template <typename> class Conversion = ByImplicitConversion
>
class FFT: public
TransformerBase<
    FFT,
    DefaultTransformationTypes,
    Conversion<DefaultTransformationTypes::PointType>,
    Pow2Extension
> {
    typedef TransformerBase<FFT, DefaultTransformationTypes, Conversion<DefaultTransformationTypes::PointType>, Pow2Extension> Base;
    typedef DefaultTransformationTypes::PointType           PointType;
    typedef DefaultTransformationTypes::Container           Container;

template <typename FwdIter>
    static void doFFTStep(FwdIter st, FwdIter fin, unsigned dist, bool inverse) {
 	    //TODO this implementation is memory-intensive. Consider making memory-saving one
        //TODO not to be so mad about iterators
        if (dist == 1)
            return;
        assert (fin!=st);
        assert (((dist-1)&dist) == 0); //power of 2
        
        Container part0,  part1;
        part0.reserve(dist/2); part1.reserve(dist/2);
        
        auto write0 = std::inserter(part0, part0.begin());
        auto write1 = std::inserter(part1, part1.begin());
        unsigned index = 0;
        unsigned hw = dist/2;
        FwdIter halfway;
        for (auto cur = st; cur != fin; ++cur, index++) {
            if ((index & 0b1) == 0)
                *(write0)++ = *cur;
            else
                *(write1)++ = *cur;
            if (index == hw)
                halfway = cur;
        } 
        doFFTStep(part0.begin(), part0.end(), dist/2, inverse);
        doFFTStep(part1.begin(), part1.end(), dist/2, inverse);

        int sign = (inverse ? -1: 1); 
        PointType unitySqrt = fracUnityPow(sign * 1. / dist);
        PointType sqrtPow(1,0); 
        
        auto h0_cur = st, h1_cur = halfway; 
        auto p0_cur = part0.begin(), p1_cur = part1.begin();
        auto p0_end = part0.end();
        for (; p0_cur!= p0_end; ++p0_cur, ++p1_cur, ++h0_cur, ++h1_cur) {
            *(h0_cur) = *p0_cur + sqrtPow * *p1_cur;
            *(h1_cur) = *p0_cur - sqrtPow * *p1_cur;
            if (inverse)
			    *h0_cur /= 2,  *h1_cur /= 2;
		    sqrtPow *= unitySqrt;
	    }
    }
    
    Container doTransform(bool inverse = false) {
        Container res = std::move(this->coeffs_);
        doFFTStep(res.begin(), res.end(), this->size_, inverse); 
        return res;
    }
public:
    template <typename RandomAccessIter>
    FFT(RandomAccessIter st, RandomAccessIter fin): Base(st, fin) {}
    
    FFT(Container && coeffs): Base(std::move(coeffs)) {}

    Container transform() {
        return doTransform();
    }

    Container inverseTransform() {
        return doTransform(true);
    }
};
*/
}
#endif
