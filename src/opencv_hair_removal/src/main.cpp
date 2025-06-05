/**
 * Copyright 2022 xiaozhuai
 */

#include "opencv2/core/utils/logger.hpp"
#include "opencv2/opencv.hpp"

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    cv::Mat origin = cv::imread("assets/hair_test1.jpg");
    cv::imshow("[0] origin", origin);

    cv::Mat gray;
    cv::cvtColor(origin, gray, cv::COLOR_BGR2GRAY);
    cv::imshow("[1] gray", gray);

    cv::Mat kernel = cv::getStructuringElement(1, cv::Size(17, 17));
    cv::Mat blackhat;
    cv::morphologyEx(gray, blackhat, cv::MORPH_BLACKHAT, kernel);
    cv::imshow("[2] blackhat", blackhat);

    cv::Mat threshold;
    cv::threshold(blackhat, threshold, 10, 255, cv::THRESH_BINARY);
    cv::imshow("[3] threshold", threshold);

    cv::Mat result;
    cv::inpaint(origin, threshold, result, 1, cv::INPAINT_TELEA);
    cv::imshow("[4] result", result);

    cv::waitKey(0);

    return 0;
}
