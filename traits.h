#ifndef __DFT_TRAITS_H
#define __DFT_TRAITS_H

#include "global.h"
#include "policies.h"

namespace DFT {
//Container is a template class, when instantiated with Container<T>
//stores objects of type T
//std::back_inserter(class) needs to be provided to copy (move) values of input and if Extension policy is applied

//PointType is a type stored by a transformation in Container<PointType> for coefficients
//Needs to provide default constructor for coefficients to be extended 

template <
    typename PointTypeInner,  //type, with which Impl wants to work with
    template <typename ...> class ContainerTypeInner
//type which is able to store PointType variable and on which implementation is working
> struct TransformationTypes {
    typedef PointTypeInner                     PointType;
    typedef ContainerTypeInner<PointTypeInner> Container; 
};

    using DefaultTransformationTypes = TransformationTypes<complex<double>, vector>;

}

#endif //__DFT_TRAITS_H
