/**
 * Copyright 2022 xiaozhuai
 */

#include <cassert>

#include "libyuv.h"
#include "stb_image.h"

int main() {
    int width, height, channels;
    auto *image = stbi_load("assets/test.jpg", &width, &height, &channels, 4);

    assert(width % 2 == 0);
    assert(height % 2 == 0);

    auto *i420Image = new uint8_t[width * height * 3 / 2];
    auto *i420Y = i420Image;
    auto *i420U = i420Y + width * height;
    auto *i420V = i420U + width * height / 4;

    libyuv::RGBAToI420(image, width * 4, i420Y, width, i420U, width / 2, i420V, width / 2, width, height);

    stbi_image_free(image);
    delete[] i420Image;
    return 0;
}
