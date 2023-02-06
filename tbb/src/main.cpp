#define ANKERL_NANOBENCH_IMPLEMENT

#include <nanobench.h>
#include <oneapi/tbb.h>
#include <vector>
#include <algorithm>
#include <cassert>

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

OPT_NONE void task(std::vector<float> &data) {
    float *p = data.data();
    float *pEnd = p + data.size();
    while (p < pEnd) {
        *(p++) /= 2.0f;
    }
}

OPT_NONE void task_tbb(std::vector<float> &data) {
    using oneapi::tbb::parallel_for;
    using oneapi::tbb::blocked_range;
    float *ptr = data.data();
    parallel_for(
            blocked_range<size_t>(0, data.size()),
            [ptr](const blocked_range<size_t> &r) {
                float *p = ptr + r.begin();
                float *pEnd = ptr + r.end();
                while (p < pEnd) {
                    *(p++) /= 2.0f;
                }
            }
    );
}

#if defined(_MSC_VER)
#pragma optimize("", on)
#endif

int main() {
    using ankerl::nanobench::Bench;

    std::vector<float> data;
    data.resize(100 * 1000 * 1000);
    std::generate(data.begin(), data.end(), []() { return float((rand() % 1000) * 2); });
    std::vector<float> data2 = data;

    Bench().run("task", [&] {
        task(data);
    });
    Bench().run("task_tbb", [&] {
        task_tbb(data2);
    });

    for (int i = 0; i < data.size(); ++i) {
        assert(data[i] == data2[i]);
    }

    return 0;
}
