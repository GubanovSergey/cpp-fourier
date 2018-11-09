#ifndef __DFT_IMPLS_H
#define __DFT_IMPLS_H
#include "dft.h"

namespace DFT {

// Example DFT class
template <typename PointType>
class NaiveDFT: public TransformerInterface<PointType, NaiveDFT> {
    Datatype countPolynom(Datatype at) {
        Datatype result(0,0);
        Datatype pt_pow(1,0);
        for (auto cur: this->coeffs_) {
            result += cur*pt_pow;
            pt_pow *= at;
        }
        return result;
    };
    Datatype calcFourierCoeff(unsigned nCoeff, bool backward) {
        int sign = (backward) ? 1 : -1;
        int divider = (backward) ? this->size_: 1;
        Datatype nthFourierX = fracUnityPow(nCoeff * sign * 1. / this->size_);
        return countPolynom(nthFourierX) / Datatype(divider, 0);
    }
public:
    template <typename RandomAccessIter>
    NaiveDFT(RandomAccessIter st, RandomAccessIter fin): TransformerInterface<PointType, NaiveDFT>(st, fin) {}

    Container<Datatype> doTransform(bool inverse = false) {//TODO make private
        //naive O(n^2) transform
        Container<Datatype> res;
        res.reserve(this->size_);
        auto writeIter = std::inserter(res, res.begin());
        for (unsigned i = 0; i < this->size_; i++) {
            (*writeIter)++ = calcFourierCoeff(i, inverse);
        }
        return res;
    }
};

//Implement a Cooley–Tukey FFT algorithm with a 2-radix butterfly diagram
//TODO consider specializing "radixness" of a diagram to check perf improvement
template <typename PointTp>
class FFT: public TransformerInterface<PointTp, FFT> {
    //dist is index(fin)-index(st)
template <typename FwdIter>
    static void doFFTStep(FwdIter st, FwdIter fin, unsigned dist, bool inverse) {
 	    //TODO this implementation is memory-intensive. Consider making memory-saving one. It will require RandomAccess iterator though
        //TODO type review to separate internal and external types
        //TODO not to be so mad about iterators
        if (dist == 1)
            return;
        assert (fin!=st);
        assert (((dist-1)&dist) == 0); //power of 2
        
        Container<Datatype> part0,  part1;
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
        Datatype unitySqrt = fracUnityPow(sign * 1. / dist);
        Datatype sqrtPow(1,0); 
        
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

public:
    template <typename RandomAccessIter>
    FFT(RandomAccessIter st, RandomAccessIter fin): TransformerInterface<PointTp, FFT>(st, fin) {}

    Container<Datatype> doTransform(bool inverse = false) {
	    Container<Datatype> res; //TODO not do redundant copies, optimise in-place
        res.reserve(this->size_);
        std::copy(this->coeffs_.begin(), this->coeffs_.end(), std::inserter(res, res.begin()));

        //extend res_ with zeros to be of pow2 size
        unsigned iter_num = this->size_ - this->in_size_; //or res_.size()
        for (unsigned i = 0; i < iter_num; i++)
            res.push_back(Datatype());

        doFFTStep(res.begin(), res.end(), this->size_, inverse); 
        return res;
    }//TODO make private 
};

}
#endif
