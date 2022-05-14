/**
 * Copyright 2022 xiaozhuai
 */

#include <algorithm>
#include <cassert>
#include <vector>

#include "nanobench.h"
#include "oneapi/tbb.h"

// NOLINTNEXTLINE
void task(const std::vector<float> &data, std::vector<float> &output) {
    const float *p = data.data();
    const float *p_end = p + data.size();
    float *(p_out) = output.data();
    while (p < p_end) {
        *(p_out++) = *(p++) / 2.0f;
    }
}

// NOLINTNEXTLINE
void task_tbb(const std::vector<float> &data, std::vector<float> &output) {
    using oneapi::tbb::blocked_range;
    using oneapi::tbb::parallel_for;
    const float *ptr = data.data();
    float *ptr_out = output.data();
    parallel_for(blocked_range<size_t>(0, data.size()), [ptr, ptr_out](const blocked_range<size_t> &r) {
        const float *p = ptr + r.begin();
        const float *p_end = ptr + r.end();
        float *p_out = ptr_out + r.begin();
        while (p < p_end) {
            *(p_out++) = *(p++) / 2.0f;
        }
    });
}

int main() {
    using ankerl::nanobench::Bench;
    const int ITER_COUNT = 1000;

    std::vector<float> data;
    data.resize(1000 * 1000);
    // NOLINTNEXTLINE
    std::generate(data.begin(), data.end(), []() { return float((rand() % 1000) * 2); });

    std::vector<float> output(data.size());
    std::vector<float> output_tbb(data.size());

    Bench().minEpochIterations(ITER_COUNT).run("task", [&] { task(data, output); });
    Bench().minEpochIterations(ITER_COUNT).run("task_tbb", [&] { task_tbb(data, output_tbb); });

    for (int i = 0; i < data.size(); ++i) {
        assert(output[i] == output_tbb[i]);
    }

    return 0;
}
