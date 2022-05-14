#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
    auto image = cv::imread("assets/test.jpg");
    cv::imshow("Image", image);
    cv::waitKey(0);
    return 0;
}
