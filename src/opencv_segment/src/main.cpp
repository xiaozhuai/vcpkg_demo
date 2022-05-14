/**
 * Copyright 2022 xiaozhuai
 */

#include "opencv2/core/utils/logger.hpp"
#include "opencv2/opencv.hpp"

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
    cv::Mat src = cv::imread("assets/idcard2.jpg");
    cv::Mat dst;
    cv::imshow("idcard", src);

    // 转灰度图
    cvtColor(src, dst, cv::COLOR_RGB2GRAY);
    cv::imshow("idcard_0_gray", dst);

    // 高斯模糊，降噪
    cv::GaussianBlur(dst, dst, cv::Size(3, 3), 0);
    cv::imshow("idcard_1_gaussian", dst);

    // 二值化
    cv::threshold(dst, dst, 165, 255, cv::THRESH_BINARY);
    cv::imshow("idcard_2_threshold", dst);

    // 中值滤波，降噪
    cv::medianBlur(dst, dst, 3);
    cv::imshow("idcard_3_median_blur", dst);

    // 腐蚀
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Point(9, 9));
    cv::erode(dst, dst, kernel);
    cv::imshow("idcard_4_erode", dst);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(dst, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < hierarchy.size(); i++) {
        cv::Rect rect = cv::boundingRect(contours.at(i));

        // 身份证号位置大于图片的一半，并且宽度是高度的8倍以上
        if (rect.y > src.rows / 2 && rect.width / rect.height > 8) {
            cv::Mat result = src(rect).clone();
            cv::imshow("idcard_5_result", result);

            cvtColor(result, result, cv::COLOR_RGB2GRAY);
            cv::threshold(result, result, 110, 255, cv::THRESH_BINARY);
            cv::imshow("idcard_6_result_threshold", result);
        }

        // 标记区域
        cv::rectangle(src, rect, cv::Scalar(255, 0, 255));
    }

    cv::imshow("idcard_7_src_and_rect", src);

    cv::waitKey(0);
    return 0;
}
