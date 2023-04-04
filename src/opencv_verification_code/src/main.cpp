/**
 * Copyright 2022 xiaozhuai
 */

#include "opencv2/core/utils/logger.hpp"
#include "opencv2/opencv.hpp"

void proc(const std::string &name) {
    cv::Mat src = cv::imread("assets/" + name + ".png");
    cv::Mat dst;
    cv::imshow("verification", src);

    // 转灰度图
    cvtColor(src, dst, cv::COLOR_RGB2GRAY);
    cv::imshow(name + "_0_gray", dst);

    // 高斯模糊，降噪
    cv::GaussianBlur(dst, dst, cv::Size(3, 3), 0);
    cv::imshow(name + "_1_gaussian", dst);

    // 二值化
    cv::threshold(dst, dst, 165, 255, cv::THRESH_BINARY);
    cv::imshow(name + "_2_threshold", dst);

    // 中值滤波，降噪
    cv::medianBlur(dst, dst, 3);
    cv::imshow(name + "_3_median_blur", dst);

    // 腐蚀
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Point(11, 11));
    cv::dilate(dst, dst, kernel);
    cv::imshow(name + "_4_dilate", dst);

    // 膨胀
    cv::erode(dst, dst, kernel);
    cv::imshow(name + "_5_erode", dst);
}

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
    proc("test_verify_code");
    proc("test_verify_code2");
    cv::waitKey(0);
    return 0;
}
