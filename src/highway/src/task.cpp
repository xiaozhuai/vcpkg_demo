/**
 * Copyright 2022 xiaozhuai
 */

#include "task.h"

////////////////////////////////////////////////////////
//// Comment out the code below when edit this file ////
////////////////////////////////////////////////////////
// #undef HWY_TARGET_INCLUDE
// #define HWY_TARGET_INCLUDE "task.cpp"
// #include "hwy/foreach_target.h"


#include "hwy/highway.h"

HWY_BEFORE_NAMESPACE();

namespace HWY_NAMESPACE {

namespace hn = hwy::HWY_NAMESPACE;





////////////////////////////////////////////////////////
////               output = src / a                 ////
////////////////////////////////////////////////////////
template<class D>
void one_div(const D d, const float *HWY_RESTRICT src, float *HWY_RESTRICT output, float a) {
    auto x = hn::Load(d, src);
    const auto o = hn::Div(x, hn::Set(d, a));
    hn::Store(o, d, output);
}

void div(const float *HWY_RESTRICT src, float *HWY_RESTRICT output, size_t size, float a) {
    const hn::ScalableTag<float> df;
    const size_t N = hn::Lanes(df);
    size_t i = 0;
    for (; i + N <= size; i += N) {
        one_div(df, src + i, output + i, a);
    }
    for (; i < size; ++i) {
        hn::CappedTag<float, 1> d1;
        one_div(d1, src + i, output + i, a);
    }
}







}  // namespace HWY_NAMESPACE

HWY_AFTER_NAMESPACE();


////////////////////////////////////////////////////////
////               Export functions                 ////
////////////////////////////////////////////////////////
#if HWY_ONCE


// HWY_EXPORT(div);
// NOLINTNEXTLINE
void task_hwy(const std::vector<float> &data, std::vector<float> &output) {
    return HWY_STATIC_DISPATCH(div)(data.data(), output.data(), data.size(), 2.0f);
}

#endif
////////////////////////////////////////////////////////
