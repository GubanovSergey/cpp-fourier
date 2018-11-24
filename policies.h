#ifndef DFT_POLICIES_H
#define DFT_POLICIES_H

#include "global.h"

//____________CONVERSION
//Just interface to obey by convention => no implementation
template <typename To>
class ConversionPolicy {
private:
    ~ConversionPolicy() {}
public:
    template <typename From>
    static To convert(From && val);
};
///

//Possible implementation
template <typename To>
struct ByImplicitConversion {
private:
    ~ByImplicitConversion() {}
public:
    template <typename From>
    static To convert(From && val) {
        return To(std::forward<From>(val));
    }
};

template <typename To>
struct ByCastConversion {
private:
    ~ByCastConversion() {}
public:
    template <typename From>
    static To convert(From && val) {
        return static_cast<To>(std::forward<From>(val));
    }
};

//"User-provided" example
template <typename To> 
struct PairConversion;

template <typename ComplexFP>
struct PairConversion<std::complex<ComplexFP>> {
private:
    ~PairConversion() {}

    using ToType = std::complex<ComplexFP>;

public:
    template <typename From>
    static ToType convert(From && val) {
        return ToType(val.first, val.second);
    }
};
//___________\CONVERSION

//__________EXTENSION

using ExtensionFunc = size_t(*)(size_t);
size_t NoExtension(size_t in_size) {
    return in_size;
}

size_t Pow2Extension(size_t in_size) {
    return 1<<(1+int(std::log2(in_size-1)));//nearest up power of 2
}

//__________\EXTENSION

#endif //DFT_POLICIES_H
