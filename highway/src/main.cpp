#include "task.h"
#include <vector>
#include <chrono>
#include <algorithm>
#include <cassert>

int64_t now() {
    return std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

int main() {
    std::vector<float> data;
    data.resize(10 * 1000 * 1000);
    std::generate(data.begin(), data.end(), []() { return float((rand() % 1000) * 2); });
    std::vector<float> data2 = data;

    int64_t t0, t1;

    {
        t0 = now();
        task(data);
        t1 = now();
        printf("task     : %lld us\n", t1 - t0);
    }

    {
        t0 = now();
        task_hwy(data2);
        t1 = now();
        printf("task_hwy : %lld us\n", t1 - t0);
    }

    for (int i = 0; i < data.size(); ++i) {
        assert(data[i] == data2[i]);
    }

    return 0;
}
