/**
 * Copyright 2022 xiaozhuai
 */

#include <array>
#include <vector>

#include "opencv2/core/utils/logger.hpp"
#include "opencv2/opencv.hpp"

struct Size {
    int x;
    int y;
};

int main() {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        printf("ERROR! Unable to open camera\n");
        return -1;
    }

    std::vector<Size> supportResolutions = {
        {1920, 1440},
        {1920, 1080},
        {1440, 1080},
        {1440,  810},
        {1280,  960},
        {1280,  720},
        { 640,  480},
        { 640,  360},
    };

    Size cameraSize = {0, 0};

    for (const auto &resolution : supportResolutions) {
        if (camera.set(cv::CAP_PROP_FRAME_WIDTH, resolution.x) && camera.set(cv::CAP_PROP_FRAME_HEIGHT, resolution.y)) {
            cameraSize = resolution;
            break;
        }
    }

    if (cameraSize.x == 0 && cameraSize.y == 0) {
        printf("ERROR! Unable to find a support resolution\n");
        return -1;
    }
    printf("Resolution: %d %d\n", cameraSize.x, cameraSize.y);

    cv::Mat image;
    while (true) {
        if (!camera.read(image)) {
            printf("ERROR! Unable to read camera\n");
            continue;
        }
        cv::imshow("camera", image);
        if (cv::waitKey(5) >= 0) break;
    }
    return 0;
}
