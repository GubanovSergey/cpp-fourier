#ifndef DFT_POLICIES_H
#define DFT_POLICIES_H

#include "global.h"

namespace DFT {
//____________CONVERSION
//Just interface to obey by convention => no implementation
template <typename To>
class ConversionPolicy {
protected:
    ~ConversionPolicy() {}
public:
    template <typename From>
    static To convert(From && val);
};
///

//Possible implementation
template <typename To>
struct ByImplicitConversion {
protected:
    ~ByImplicitConversion() {}
public:
    using ToType=To;
    template <typename From>
    static To convert(From && val) {
        return std::forward<From>(val);
    }
};

template <typename To>
struct ByCastConversion {
protected:
    ~ByCastConversion() {}
public:
    using ToType=To;
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
protected:
    ~PairConversion() {}
public:
    using ToType = std::complex<ComplexFP>;
    template <typename From>
    static ToType convert(From && val) {
        return ToType(val.first, val.second);
    }
};
//___________\CONVERSION

//__________EXTENSION

//using ExtensionFunc = size_t(*)(size_t);
struct NoExtension {
protected:
    ~NoExtension() {}
public:
    template <typename SizeType>
    static SizeType calcSize(SizeType sz) {
        return sz;
    }
};

struct Pow2Extension {
protected:
    ~Pow2Extension() {}
public:
    template <typename SizeType>
    static SizeType calcSize(SizeType sz) {
        assert(sz > 1);
        return 1<<(1+int(std::log2(sz-1)));//nearest up power of 2
    }
};

//__________\EXTENSION

}
#endif //DFT_POLICIES_H
