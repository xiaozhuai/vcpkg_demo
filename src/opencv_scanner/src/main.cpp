/**
 * Copyright 2022 xiaozhuai
 */

#include <vector>

#include "opencv2/core/utils/logger.hpp"
#include "opencv2/opencv.hpp"

static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0) {
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
}

// NOLINTNEXTLINE
void findSquares(const cv::Mat &image, std::vector<std::vector<cv::Point>> &squares) {
    int thresh = 50, N = 5;
    squares.clear();

    cv::Mat src, dst, gray_one, gray;

    src = image.clone();
    gray_one = cv::Mat(src.size(), CV_8U);
    // 滤波增强边缘检测
    cv::medianBlur(src, dst, 9);
    // bilateralFilter(src, dst, 25, 25 * 2, 35);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    // 在图像的每个颜色通道中查找矩形
    for (int c = 0; c < image.channels(); c++) {
        int ch[] = {c, 0};

        // 通道分离
        cv::mixChannels(&dst, 1, &gray_one, 1, ch, 1);

        // 尝试几个阈值
        for (int l = 0; l < N; l++) {
            // 用canny()提取边缘
            if (l == 0) {
                // 检测边缘
                cv::Canny(gray_one, gray, 5, thresh, 5);
                // 膨胀
                cv::dilate(gray, gray, cv::Mat(), cv::Point(-1, -1));
            } else {
                gray = gray_one >= (l + 1) * 255 / N;
            }

            // 轮廓查找
            cv::findContours(gray, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

            std::vector<cv::Point> approx;

            // 检测所找到的轮廓
            for (const auto &contour : contours) {
                // 使用图像轮廓点进行多边形拟合
                cv::approxPolyDP(cv::Mat(contour), approx, cv::arcLength(cv::Mat(contour), true) * 0.02, true);

                // 计算轮廓面积后，得到矩形4个顶点
                if (approx.size() == 4 && fabs(cv::contourArea(cv::Mat(approx))) > 1000 &&
                    cv::isContourConvex(cv::Mat(approx))) {
                    double maxCosine = 0;

                    for (int j = 2; j < 5; j++) {
                        // 求轮廓边缘之间角度的最大余弦
                        double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    if (maxCosine < 0.3) {
                        squares.push_back(approx);
                    }
                }
            }
        }
    }
}

class Visualization {
public:
    Visualization(std::vector<cv::Point> srcPoints, const cv::Mat &originImage)
        : srcPoints(srcPoints), originImage(originImage) {}

    void update() {
        markImage = originImage.clone();
        int n = static_cast<int>(srcPoints.size());
        auto pointsPtr = srcPoints.data();
        cv::polylines(markImage, &pointsPtr, &n, 1, true, cv::Scalar(0, 120, 255), 1, cv::LINE_AA);
        int i = 0;
        for (const auto &point : srcPoints) {
            // printf("(%d, %d) ", point.x, point.y);
            auto color = cv::Scalar(0, 0, 255);
            if (i == moving) {
                color = cv::Scalar(255, 0, 0);
            }
            cv::circle(markImage, point, 11, color, -1, cv::LINE_AA);
            i++;
        }
        // printf("\n");

        cv::Size dstSize(960, 604);
        std::vector<cv::Point> dstPoints = {
            {960,   0},
            {  0,   0},
            {  0, 604},
            {960, 604}
        };

        auto matrix = cv::findHomography(srcPoints, dstPoints, cv::RANSAC);
        cv::warpPerspective(originImage, resultImage, matrix, dstSize);

        roundedResultImage = resultImage.clone();
        // TODO(xiaozhuai) rounded mask

        // imshow("Origin Image", originImage);
        imshow("Mark Image", markImage);
        imshow("Result Image", resultImage);
        // imshow("Rounded Result Image", roundedResultImage);
    }

public:
    std::vector<cv::Point> srcPoints;
    cv::Mat originImage;
    cv::Mat markImage;
    cv::Mat resultImage;
    cv::Mat roundedMaskImage;
    cv::Mat roundedResultImage;
    int moving = -1;
};

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    cv::Mat originImage = cv::imread("assets/idcard.jpg");
    cv::Mat markImage = originImage.clone();
    cv::Mat resultImage;

    std::vector<std::vector<cv::Point>> squares;
    findSquares(originImage, squares);

    std::vector<cv::Point> srcPoints;
    // TODO(xiaozhuai) pick up square
    for (const auto &s : squares) {
        if (s[0].x < 3 || s[0].y < 3) continue;
        srcPoints = s;
        break;
    }

    Visualization visual(srcPoints, originImage);
    visual.update();

    cv::setMouseCallback(
        "Mark Image",
        [](int event, int x, int y, int flags, void *userdata) {
            Visualization &visual = *(reinterpret_cast<Visualization *>(userdata));
            if (event == cv::EVENT_LBUTTONUP) {
                int i = 0;
                for (const auto &p : visual.srcPoints) {
                    auto distance = cv::norm(cv::Point(x, y) - p);
                    if (distance <= 11.0) {
                        visual.moving = i;
                        visual.update();
                        return;
                    }
                    i++;
                }
                visual.moving = -1;
                visual.update();
            }
        },
        &visual);

    while (true) {
        int key = cv::waitKey(1);
        if (key == -1) continue;
        if (key >= 49 && key <= 52) {  // 1 2 3 4
            int index = key - 49;
            visual.moving = visual.moving == index ? -1 : index;
            visual.update();
        } else if (key == 0) {  // up
            visual.srcPoints[visual.moving] += cv::Point(0, -1);
            visual.update();
        } else if (key == 1) {  // down
            visual.srcPoints[visual.moving] += cv::Point(0, 1);
            visual.update();
        } else if (key == 2) {  // left
            visual.srcPoints[visual.moving] += cv::Point(-1, 0);
            visual.update();
        } else if (key == 3) {  // right
            visual.srcPoints[visual.moving] += cv::Point(1, 0);
            visual.update();
        } else if (key == 27 || key == 113) {  // esc or q
            break;
        }
    }

    return 0;
}
