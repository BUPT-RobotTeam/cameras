#include <iostream>
#include <opencv2/opencv.hpp>
#include "cameras.hpp"

int main() {
    cv::Mat frame;
    cv::namedWindow("Cam", cv::WINDOW_NORMAL);
    cameras cam;
    CAMERAS_CHECK(cam.open(), "camera open error");
    CAMERAS_CHECK(cam.start(), "camera start error");

    while (true) {
        frame = cam.get_frame();
        if (frame.empty())
            continue;

        cv::Scalar color(0, 0, 255);  // BGR颜色，这里是红色

        cv::line(frame, cv::Point(600, 0), cv::Point(600, frame.rows - 1), color, 2);

        cv::line(frame, cv::Point(0, 400), cv::Point(frame.cols - 1, 400), color, 2);
        std::cout << "distance: " << cam.get_depth(600, 400) << std::endl;
        cv::imshow("Cam", frame);
        if (cv::waitKey(1) == 27)
            break;
    }

    cam.stop();
    cv::destroyAllWindows();
    return 0;
}