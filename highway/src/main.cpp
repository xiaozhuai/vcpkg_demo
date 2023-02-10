/**
 * Copyright 2022 xiaozhuai
 */

#include <cassert>

#include <vector>
#include <algorithm>

#include <nanobench.h>

#include "task.h"

// NOLINTNEXTLINE
void task(const std::vector<float> &data, std::vector<float> &output) {
    const float *p = data.data();
    const float *p_end = p + data.size();
    float *(p_out) = output.data();
    while (p < p_end) {
        *(p_out++) = *(p++) / 2.0f;
    }
}

int main() {
    using ankerl::nanobench::Bench;
    const int ITER_COUNT = 1000;

    std::vector<float> data;
    data.resize(1000 * 1000);
    // NOLINTNEXTLINE
    std::generate(data.begin(), data.end(), []() { return float((rand() % 1000) * 2); });

    std::vector<float> output(data.size());
    std::vector<float> output_hwy(data.size());

    Bench().minEpochIterations(ITER_COUNT).run("task", [&] {
        task(data, output);
    });
    Bench().minEpochIterations(ITER_COUNT).run("task_hwy", [&] {
        task_hwy(data, output_hwy);
    });

    for (int i = 0; i < data.size(); ++i) {
        assert(output[i] == output_hwy[i]);
    }

    return 0;
}
