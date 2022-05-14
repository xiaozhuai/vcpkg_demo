/**
 * Copyright 2022 xiaozhuai
 */

#include <cstdio>
#include <memory>

#include "uvw.hpp"

int main() {
    auto loop = uvw::loop::get_default();
    auto timer = loop->resource<uvw::timer_handle>();

    timer->on<uvw::timer_event>([](const auto &, uvw::timer_handle &handle) { printf("Hello\n"); });

    timer->start(uvw::timer_handle::time{1000}, uvw::timer_handle::time{0});
    loop->run();
    return 0;
}
