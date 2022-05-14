/**
 * Copyright 2022 xiaozhuai
 */

#include "task.h"

// #define HWY_COMPILE_ONLY_STATIC

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "task.cpp"
#include "hwy/foreach_target.h"
#include "hwy/highway.h"

namespace HWY_NAMESPACE {

namespace hn = hwy::HWY_NAMESPACE;

////////////////////////////////////////////////////////
////               output = src / a                 ////
////////////////////////////////////////////////////////
template <class D, class A>
HWY_INLINE void one_div(const D d, const float *HWY_RESTRICT src, float *HWY_RESTRICT output, const A &a) {
    auto x = hn::Load(d, src);
    const auto o = hn::Div(x, a);
    hn::Store(o, d, output);
}

void div(const float *HWY_RESTRICT src, float *HWY_RESTRICT output, size_t size, float a) {
    constexpr hn::ScalableTag<float> df;
    constexpr hn::CappedTag<float, 1> d1;
    constexpr size_t N = hn::Lanes(df);
    auto *dst_ptr = output;
    const auto *src_ptr = src;
    const auto *const dst_end = output + size;
    const auto af = hn::Set(df, a);
    for (; dst_ptr + N <= dst_end; dst_ptr += N, src_ptr += N) {
        one_div(df, src_ptr, dst_ptr, af);
    }
    const auto a1 = hn::Set(d1, a);
    for (; dst_ptr < dst_end; ++dst_ptr, ++src_ptr) {
        one_div(d1, src_ptr, dst_ptr, a1);
    }
}

}  // namespace HWY_NAMESPACE

////////////////////////////////////////////////////////
////               Export functions                 ////
////////////////////////////////////////////////////////
#if HWY_ONCE

HWY_EXPORT(div);

void task_hwy(const std::vector<float> &data, std::vector<float> &output, float a) {
    return HWY_DYNAMIC_DISPATCH(div)(data.data(), output.data(), data.size(), a);
}

void task(const std::vector<float> &data, std::vector<float> &output, float a) {
    const float *p = data.data();
    const float *p_end = p + data.size();
    float *(p_out) = output.data();
    while (p < p_end) {
        *(p_out++) = *(p++) / a;
    }
}

#endif
////////////////////////////////////////////////////////
