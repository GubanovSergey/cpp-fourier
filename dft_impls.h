#include "dft.h"

namespace DFT {

// Example DFT class
class NaiveDFT: public TransformerInterface<NaiveDFT> {
    Datatype countPolynom(Datatype at) {
        Datatype result(0,0);
        Datatype pt_pow(1,0);
        for (auto cur: coeffs_) {
            result += cur*pt_pow;
            pt_pow *= at;
        }
        return result;
    };

    Datatype calcFourierCoeff(unsigned nCoeff, bool backward) {
        auto dim_ = dim();
        assert(dim_ > nCoeff);
        int sign = (backward) ? 1 : -1;
        int divider = (backward) ? dim_: 1;
        Datatype nthFourierX = fracUnityPow(nCoeff * sign * 1. / dim_);
        return countPolynom(nthFourierX) / Datatype(divider, 0);
    }
public:
    NaiveDFT(Container<Datatype> &&points_in): TransformerInterface(std::move(points_in)) {}

    void doTransform(bool inverse = false) {//TODO make private
        //naive O(n^2) transform
        auto dim_ = dim();
        auto writeIter = std::inserter(res_, res_.begin());
        for (unsigned i = 0; i < dim_; i++) {
            (*writeIter)++ = calcFourierCoeff(i, inverse);
        }
    }
};

//Implement a Cooley–Tukey FFT algorithm with a 2-radix butterfly diagram
//TODO consider specializing "radixness" of a diagram to check perf improvement
class FFT: public TransformerInterface<FFT> {
    //dist is index(fin)-index(st)
template <typename FwdIter>
    void doTransformStep(FwdIter st, FwdIter fin, unsigned dist, bool inverse) {
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
        doTransformStep(part0.begin(), part0.end(), dist/2, inverse);
        doTransformStep(part1.begin(), part1.end(), dist/2, inverse);

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
    FFT(Container<Datatype> &&points_in): TransformerInterface(std::move(points_in)) {}

    void doTransform(bool inverse = false) {
    //void fft (vector<base> & a, bool invert) {
    	auto size = dim();
	    res_ = coeffs_; //TODO not do redundant copies, optimise in-place

        //extend res_ with zeros to be of pow2 size
        unsigned neededSize = 1<<(1+int(std::log2(size-1))); 
        assert(neededSize>=size);
        unsigned iter_num = neededSize - size; //or res_.size()
        for (unsigned i = 0; i<iter_num; i++)
            res_.push_back(Datatype());

        doTransformStep(res_.begin(), res_.end(), neededSize, inverse); 
    }//TODO make private 
};

}
