#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
    auto image = cv::imread("assets/test.jpg");
    cv::imshow("vcpkg_demo_opencv", image);
    cv::namedWindow("vcpkg_demo_opencv", cv::WINDOW_NORMAL);
    cv::resizeWindow("vcpkg_demo_opencv", 960, 540);
    cv::waitKey(0);
    return 0;
}
