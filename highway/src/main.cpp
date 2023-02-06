#define ANKERL_NANOBENCH_IMPLEMENT

#include "task.h"
#include <nanobench.h>
#include <vector>
#include <algorithm>
#include <cassert>

int main() {
    using ankerl::nanobench::Bench;

    std::vector<float> data;
    data.resize(100 * 1000 * 1000);
    std::generate(data.begin(), data.end(), []() { return float((rand() % 1000) * 2); });
    std::vector<float> data2 = data;

    Bench().run("task", [&] {
        task(data);
    });
    Bench().run("task_hwy", [&] {
        task_hwy(data2);
    });

    for (int i = 0; i < data.size(); ++i) {
        assert(data[i] == data2[i]);
    }

    return 0;
}
