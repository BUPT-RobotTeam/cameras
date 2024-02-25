#include <iostream>
#include <opencv2/opencv.hpp>
#include "cameras.hpp"

int main() {
    cv::Mat frame;
    cv::namedWindow("Cam", cv::WINDOW_NORMAL);
    cameras cam("d");

    if (!cam.open()) {
        std::cerr << "camera open error" << std::endl;
        return -1;
    }

    if (!cam.start()) {
        std::cerr << "camera start error" << std::endl;
        return -1;
    }

    while (true) {
        frame = cam.get_frame();
        if (frame.empty())
            continue;

        cv::imshow("Cam", frame);

        if (cv::waitKey(1) == 27)
            break;
    }

    cam.stop();
    cv::destroyAllWindows();
    return 0;
}