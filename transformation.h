#ifndef __DFT_TRANSFORMATION_H
#define __DFT_TRANSFORMATION_H

#include "global.h"
#include "policies.h"
#include "input_adaptor.h"
#include "dft_impls.h"

//TODO adjust interfaces doTransform and adaptData before moving on
namespace DFT {

template <
    class ForwardImpl, //and TfReqs<Impl> and Impl::TfTypes
    class Conversion = ByImplicitConversion<typename ForwardImpl ::PointType> //may be None
>
class Transformation: private ForwardImpl,
                      private InputAdaptor<
                          typename ForwardImpl:: TfTypes,
                          Conversion,
                          typename TfReqs<ForwardImpl>::Extension 
                      > {
    using TfTypes = typename ForwardImpl::TfTypes;
    using PointType = typename TfTypes::PointType;
    using Container = typename TfTypes::Container;
    //using InputAdaptor::adaptData
    //using ForwardImpl::doTransform
    
public:
    template <typename ... InParam>
    Container transform(InParam&&... params) {
        Container in = this->adaptData(std::forward<InParam>(params)...);
        return this->doTransform(std::move(in));
    }
};
/*
template <
    class InverseImpl, //and TfReqs<Impl> and Impl::TfTypes
    class Conversion  //with ToType already defined by a user
>
class ITransformation: private InverseImpl,
                       private OutputAdaptor<
                          ForwardImpl::TfTypes,
                          Conversion
                       > {
    using TfTypes = ForwardImpl::TfTypes;
    using PointType = TfTypes::PointType;
    using Container = TfTypes::Container;
    using OutputAdaptor::adaptData;

public:
    template <typename ... OutParams>
    auto inverseTransform(Container && in, OutParams... params) {
        Container out = doTransform(in, true);
        return adaptData(out, params...);
    }
};
*/

}

#endif

