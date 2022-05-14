/**
 * Copyright 2022 xiaozhuai
 */

#include <algorithm>
#include <cassert>
#include <vector>

#include "nanobench.h"
#include "task.h"

int main() {
    using ankerl::nanobench::Bench;
    using ankerl::nanobench::doNotOptimizeAway;
    const int ITER_COUNT = 1000;

    std::vector<float> data;
    data.resize(1000 * 1000);
    std::generate(data.begin(), data.end(), []() { return float((rand() % 1000) * 2); });

    std::vector<float> output(data.size());
    std::vector<float> output_hwy(data.size());

    Bench().minEpochIterations(ITER_COUNT).run("task", [&] {
        task(data, output, 3.0f);
        doNotOptimizeAway(output);
    });
    Bench().minEpochIterations(ITER_COUNT).run("task_hwy", [&] {
        task_hwy(data, output_hwy, 3.0f);
        doNotOptimizeAway(output);
    });

    for (int i = 0; i < data.size(); ++i) {
        assert(output[i] == output_hwy[i]);
    }

    return 0;
}
