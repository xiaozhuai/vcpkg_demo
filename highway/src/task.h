/**
 * Copyright 2022 xiaozhuai
 */

#pragma once

#include <vector>

#include "hwy/base.h"

#if defined(__clang__)
#define OPT_NONE    __attribute__((optnone))
#elif defined(__GNUC__) || defined(__GNUG__)
#define OPT_NONE    __attribute__((optimize("O0")))
#else
#define OPT_NONE
#endif

#if defined(_MSC_VER)
#pragma optimize("", off)
#endif

// NOLINTNEXTLINE
OPT_NONE inline void task(std::vector<float> &data) {
    float *p = data.data();
    float *pEnd = p + data.size();
    while (p < pEnd) {
        *(p++) /= 2.0f;
    }
}

#if defined(_MSC_VER)
#pragma optimize("", on)
#endif

// NOLINTNEXTLINE
void task_hwy(std::vector<float> &data);
