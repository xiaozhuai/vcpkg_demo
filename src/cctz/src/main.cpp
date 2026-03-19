/**
 * Copyright 2022 xiaozhuai
 */

#include <iostream>

#include "cctz/civil_time.h"
#include "cctz/time_zone.h"

void hello_february_2016() {
    for (cctz::civil_day d(2016, 2, 1); d < cctz::civil_month(2016, 3); ++d) {
        std::cout << "Hello " << d;
        if (d.day() == 29) {
            std::cout << " <- leap day is a " << cctz::get_weekday(d);
        }
        std::cout << "\n";
    }
}

void one_giant_leap() {
    cctz::time_zone nyc;
    cctz::load_time_zone("America/New_York", &nyc);
    // Converts the input civil time in NYC to an absolute time.
    const auto moon_walk = cctz::convert(cctz::civil_second(1969, 7, 20, 22, 56, 0), nyc);
    std::cout << "Moon walk in NYC: " << cctz::format("%Y-%m-%d %H:%M:%S %Ez\n", moon_walk, nyc);
    cctz::time_zone syd;
    if (!cctz::load_time_zone("Australia/Sydney", &syd)) {
        std::cerr << "Failed to load time zone: Australia/Sydney\n";
        return;
    }
    std::cout << "Moon walk in SYD: " << cctz::format("%Y-%m-%d %H:%M:%S %Ez\n", moon_walk, syd);
}

int main() {
    hello_february_2016();
    one_giant_leap();
    return 0;
}
