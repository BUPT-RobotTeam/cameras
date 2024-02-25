#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include "hikcam.hpp"

#define CAMERAS_CHECK(state, str) \
    do { \
        if (!(state)) { \
            std::cerr << (str) << std::endl; \
            return -1; \
        } \
    } while (false)

class cameras{
public:
    cameras() : _cam_type("i") {}
    cameras(std::string cam_type) {
        if (!this->type_is_useful(cam_type)) {
            std::cerr << "param: " << cam_type << " is not useful" << std::endl;
            std::cerr << "use default param : i" << std::endl;
            this->_cam_type = "i";
        }
        else 
            this->_cam_type = cam_type;
    }

    bool open() {
        if (this->_cam_type == "i") {
            this->_cam_industry = cv::VideoCapture(0);                  // TODO: 修改成可以处理多摄像头
            return this->_cam_industry.isOpened();
        }
        else if (this->_cam_type == "h") {
            return this->_cam_hik.open(0);                              // TODO: 修改成可以处理多摄像头
        }
        else if (this->_cam_type == "d") { 
            rs2::context ctx;
            auto list = ctx.query_devices(); 
            if (list.size() > 0)  {
                this->_cam_realsense_pipe = rs2::pipeline(ctx);
                return true;
            }
        }
        return false;
    }

    bool start() {
        if (this->_cam_type == "i") {
            return true;
        }
        else if (this->_cam_type == "h") {
            return this->_cam_hik.start();
        }
        else if (this->_cam_type == "d") {
            this->_cam_realsense_pipe.start();
            // return this->_cam_realsense_pipe.is_started();
            return true;
        }
        return false;
    }

    bool stop() {
        if (this->_cam_type == "i") {
            this->_cam_industry.release();
            return !this->_cam_industry.isOpened();
        }
        else if (this->_cam_type == "h") {
            return this->_cam_hik.stop();
        }
        else if (this->_cam_type == "d") {
            return true;
        }
        return false;
    }

    cv::Mat get_frame() {
        if (this->_cam_type == "i") {
            this->_cam_industry.read(this->_frame);
        }
        else if (this->_cam_type == "h") {
            this->_frame = this->_cam_hik.read();
        }
        else if (this->_cam_type == "d") {
            rs2::frameset data = this->_cam_realsense_pipe.wait_for_frames();
            rs2::frame color_frame = data.get_color_frame();

            const int w = color_frame.as<rs2::video_frame>().get_width();
            const int h = color_frame.as<rs2::video_frame>().get_height();

            cv::Mat image(cv::Size(w, h), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);
            cv::cvtColor(image, this->_frame, cv::COLOR_RGB2BGR);
        }
        else {
            this->_frame.release();
        }

        return this->_frame;
    }
    void check(bool state, std::string& str) {
        if (!state)
            std::cerr << str << std::endl;
    }
private:
    bool type_is_useful(std::string cam_type) {
        if (cam_type == "h" || cam_type == "i" || cam_type == "d")
            return true;
        return false;
    }

private:
    std::string _cam_type;                              // 摄像头的类型: h - hik摄像头; i - 工业摄像头; d - 深度摄像头
    cv::Mat _frame;                                     // 图片处理后的opencv frame
    hikcam _cam_hik;                                    // 海康摄像头
    cv::VideoCapture _cam_industry;                     // 工业摄像头
    rs2::pipeline _cam_realsense_pipe;
};