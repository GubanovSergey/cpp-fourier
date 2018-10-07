#include <cmath>
#include <cassert>
#include <functional>
#include <complex>
#include <vector>

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

template <typename Impl>
class TransformerInterface {
protected:
    unsigned dim_;
    Container<Datatype> res_;
public:
    const Container<Datatype> & results() {//TODO guarantee transform has been done
        return res_;
    }
        
public:    
    TransformerInterface(unsigned dim): dim_(dim), res_() {
        res_.reserve(dim_);
    } 
    
    TransformerInterface & transform() {
        static_cast<Impl *>(this)->doTransform();
        return *this;
    } //in-place
    TransformerInterface & inverseTransform() {
        bool reverse = true;
        static_cast<Impl *>(this)->doTransform(reverse);
        return *this;
    } 
    void clear() {//TODO check if results were read
        res_.clear();
    }
};

// Perform dft on incoming points
//TransformerImpl should have non-trivial constructor and optional move constructor 
//with special params needed
//Also it have to derive TransformerInterface 
template <typename TransformerImpl>
class FourierTransformer: public TransformerImpl {
    
public:
    template <typename... Ts>
    FourierTransformer(Ts... args): TransformerImpl(args...) {}
    //template <typename... Ts>
    //FourierTransformer(Ts...&& args): TransformerImpl(std::forward<Ts> (args)...) {}
    //TODO how to make optional move semantic???
};

// Example DFT class
class NaiveDFT: public TransformerInterface<NaiveDFT> {
    std::function<Datatype(const Datatype)> polynom_;
    
    Datatype calcFourierCoeff(unsigned nCoeff, bool backward) {
        assert(dim_ > nCoeff);
        int sign = (backward) ? 1 : -1;
        int divider = (backward) ? dim_: 1;
        Datatype nthFourierX = fracUnityPow(nCoeff * sign * 1. / dim_);
        return polynom_(nthFourierX) / Datatype(divider, 0);
    }
public:
    NaiveDFT(std::function<Datatype(const Datatype)> srcFunc, unsigned dim): TransformerInterface(dim), polynom_(srcFunc) {
    }

    void doTransform(bool inverse = false) {//TODO make private
        //naive O(n^2) transform
        auto writeIter = std::inserter(res_, res_.begin());
        for (unsigned i = 0; i < dim_; i++) {
            (*writeIter)++ = calcFourierCoeff(i, inverse);
        }
    }
    void clear() {}
};

}
