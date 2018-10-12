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
    Container<Datatype> coeffs_;
    //vector of coefficients for a polynom, f.e.
    //from lowest degree coefficient to the highest
    Container<Datatype> res_;

    Container<Datatype>::size_type dim() {
        return coeffs_.size();
    }

public:
    const Container<Datatype> & results() {//TODO guarantee transform has been done
        return res_;
    }
        
public:    
    TransformerInterface(Container<Datatype> &&points_in): coeffs_(points_in), res_() { //TODO rewrite in_points to template
        auto sz = points_in.size();
        assert(sz > 0);
        unsigned nearestUpPow2 = 1<<(1+int(std::log2(sz)-1)); 
        res_.reserve(nearestUpPow2);
    }
    
    TransformerInterface & transform() {
        static_cast<Impl *>(this)->doTransform();
        return *this;
    } 
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
//TransformerImpl should have a constructor with moved in-coeffs and optional other args by value 
//TransformerImpl is supposed to derive TransformerInterface<TransformerImpl> 
template <typename TransformerImpl>
class FourierTransformer: public TransformerImpl {
    
public:
    template <typename... Ts>
    FourierTransformer(Container<Datatype> &&points_in, Ts... args): TransformerImpl(std::move(points_in), args...) {}
    //template <typename... Ts>
    //FourierTransformer(Ts...&& args): TransformerImpl(std::forward<Ts> (args)...) {}
    //TODO how to make optional move semantic???
};

}
