/**
 * Copyright 2022 xiaozhuai
 */

#include <cstdio>

#include "xorstr.hpp"

int main() {
    printf("%s\n", xorstr_("Hello World!"));
    auto encrypted = xorstr("Hello World!");
    printf("%s\n", encrypted.crypt_get());
    return 0;
}
