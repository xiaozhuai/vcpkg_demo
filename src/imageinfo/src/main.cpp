/**
 * Copyright 2022 xiaozhuai
 */

#include <iostream>

#include "imageinfo.hpp"

int main() {
    auto file = "assets/test.jpg";
    auto imageInfo = imageinfo::parse<imageinfo::FilePathReader>(file);
    std::cout << "File: " << file << "\n";
    std::cout << "  - Error    : " << imageInfo.error_msg() << "\n";
    std::cout << "  - Width    : " << imageInfo.size().width << "\n";
    std::cout << "  - Height   : " << imageInfo.size().height << "\n";
    std::cout << "  - Format   : " << imageInfo.format() << "\n";
    std::cout << "  - Ext      : " << imageInfo.ext() << "\n";
    std::cout << "  - Full Ext : " << imageInfo.full_ext() << "\n";
    std::cout << "  - Mimetype : " << imageInfo.mimetype() << "\n\n";
    return 0;
}
