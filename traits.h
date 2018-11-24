#ifndef __DFT_TRAITS_H
#define __DFT_TRAITS_H

#include "global.h"
#include "policies.h"

namespace DFT {
//TODO provide Container required interface
template <
    typename PointTypeInner,  //type, with which Impl wants to work with
    template <typename ...> class ContainerTypeInner
//type which is able to store PointType variable and on which implementation is working
> struct TransformationTypes {
    typedef PointTypeInner                     PointType;
    
    //template <typename ...>
    //using ContainerType = ContainerTypeInner;
    //TODO it seems like we cannot use something like this

    typedef ContainerTypeInner<PointTypeInner> Container; 
};

    using DefaultTransformationTypes = TransformationTypes<complex<double>, vector>;

}

#endif //__DFT_TRAITS_H
