#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);
    cv::Mat src = cv::imread("assets/idcard.jpg");
    cv::Mat dst;

    // 转灰度图
    cvtColor(src, dst, cv::COLOR_RGB2GRAY);
    cv::imwrite("output/idcard_0_gray.png", dst);

    // 高斯模糊，降噪
    cv::GaussianBlur(dst, dst, cv::Size(3, 3), 0);
    cv::imwrite("output/idcard_1_gaussian.png", dst);

    // 二值化图，主要将灰色部分转成白色，使内容为黑色
    cv::threshold(dst, dst, 165, 255, cv::THRESH_BINARY);
    cv::imwrite("output/idcard_2_threshold.png", dst);

    // 中值滤波，降噪
    cv::medianBlur(dst, dst, 3);
    cv::imwrite("output/idcard_3_median_blur.png", dst);

    // 腐蚀
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Point(9, 9));
    cv::erode(dst, dst, kernel, cv::Point(-1, -1), 1);
    cv::imwrite("output/idcard_4_erode.png", dst);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(dst, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

    for (int i = 0; i < hierarchy.size(); i++) {
        cv::Rect rect = cv::boundingRect(contours.at(i));

        // 身份证号位置大于图片的一半，并且宽度是高度的6倍以上
        if (rect.y > src.rows / 2 && rect.width / rect.height > 6) {
            cv::Mat result = src(rect);
            cv::imwrite("output/idcard_5_result.png", result);
        }

        // 标记区域
        cv::rectangle(src, rect, cv::Scalar(255, 0, 255));
    }

    cv::imwrite("output/idcard_6_src_and_rect.png", src);

    return 0;
}
