#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    auto image = cv::imread("assets/test_input_seg_3_4.png");
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    cv::threshold(image, image, 128, 255, cv::THRESH_BINARY);
    cv::imwrite("assets/1_threshold.png", image);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(11, 11));
    cv::dilate(image, image, kernel);
    cv::imwrite("assets/2_dilate.png", image);

    cv::erode(image, image, kernel);
    cv::imwrite("assets/3_erode.png", image);

    cv::GaussianBlur(image, image, cv::Size(11, 11), 0);
    cv::imwrite("assets/4_gaussian.png", image);

    // auto image = cv::imread("assets/test.jpg");
    // cv::resize(image, image, cv::Size(960, 540));
    // cv::imshow("vcpkg_demo_opencv", image);
    // cv::waitKey(0);
    return 0;
}
