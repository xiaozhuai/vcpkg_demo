//
// Created by xiaozhuai on 2023/2/1.
//

#include "task.h"

////////////////////////////////////////////////////////
//// Comment out the code below when edit this file ////
////////////////////////////////////////////////////////
#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "task.cpp"
#include <hwy/foreach_target.h>




#include <hwy/highway.h>

HWY_BEFORE_NAMESPACE();

namespace HWY_NAMESPACE {

namespace hn = hwy::HWY_NAMESPACE;





////////////////////////////////////////////////////////
////              value = value / a                 ////
////////////////////////////////////////////////////////
template<class D>
void one_div(const D d, float *HWY_RESTRICT value, float a) {
    auto x = hn::Load(d, value);
    x = hn::Div(x, hn::Set(d, a));
    hn::Store(x, d, value);
}

void div(float *HWY_RESTRICT array, size_t size, float a) {
    const hn::ScalableTag<float> df;
    const size_t N = hn::Lanes(df);
    size_t i = 0;
    for (; i + N <= size; i += N) {
        one_div(df, array + i, a);
    }
    for (; i < size; ++i) {
        hn::CappedTag<float, 1> d1;
        one_div(d1, array + i, a);
    }
}







}  // namespace HWY_NAMESPACE

HWY_AFTER_NAMESPACE();


////////////////////////////////////////////////////////
////               Export functions                 ////
////////////////////////////////////////////////////////
#if HWY_ONCE


HWY_EXPORT(div);
void task_hwy(std::vector<float> &data) {
    return HWY_DYNAMIC_DISPATCH(div)(data.data(), data.size(), 2.0f);
}


#endif
////////////////////////////////////////////////////////