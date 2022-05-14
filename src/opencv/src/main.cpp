/**
 * Copyright 2022 xiaozhuai
 */

#include "opencv2/core/utils/logger.hpp"
#include "opencv2/opencv.hpp"

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    auto image = cv::imread("assets/test.jpg");
    cv::resize(image, image, cv::Size(960, 540));
    cv::imshow("vcpkg_demo_opencv", image);
    cv::waitKey(0);
    return 0;
}
