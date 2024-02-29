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
        cam.draw_mark(300, 300, cameras::MARK_X);
        cv::imshow("Cam", frame);
        std::cout << "fps: " << cam.get_fps() << std::endl;
        if (cv::waitKey(1) == 27)
            break;
    }

    cam.stop();
    cv::destroyAllWindows();
    return 0;
}
