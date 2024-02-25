#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include <opencv2/core/utils/logger.hpp>
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
    //------------------------------初始化函数------------------------------
    cameras() {                                                         
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_FATAL);                   // 设置opencv打印日志的等级, 不再打印warn信息
        this->auto_detect_cam();
        this->print_cam(this->type_is_useful(this->_cam_type));
    }
    cameras(std::string cam_type) {
        cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_FATAL);
        this->_cam_type = "x";
        if (!this->type_is_useful(cam_type)) {
            std::cerr << "param: " << cam_type << " is not useful" << std::endl;
            std::cerr << "automatically find available devices" << std::endl;
            auto_detect_cam();
        }
        else 
            this->_cam_type = cam_type;
        this->print_cam(this->type_is_useful(this->_cam_type));
    }

    //------------------------------打开摄像头------------------------------
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

    //------------------------------开始采集------------------------------
    bool start() {
        if (this->_cam_type == "i") {
            return true;
        }
        else if (this->_cam_type == "h") {
            return this->_cam_hik.start();
        }
        else if (this->_cam_type == "d") {
            this->_cam_realsense_pipe.start();                          // TODO: 无法判断是否开启成功
            return true;
        }
        return false;
    }

    //------------------------------停止采集------------------------------
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

    //------------------------------获取一帧图片------------------------------
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

private:
    bool type_is_useful(std::string cam_type) {
        if (cam_type == "h" || cam_type == "i" || cam_type == "d")
            return true;
        return false;
    }

    bool auto_detect_cam() {
        this->_cam_type = "x";
        do {
            //------------------------------检查工业摄像头是否可用------------------------------
            cv::VideoCapture cap(0);
            if (cap.isOpened()) {
                cap.release();
                this->_cam_type = "i";
                break;
            }

            //------------------------------检查海康摄像头是否可用------------------------------
            MV_CC_DEVICE_INFO_LIST stDeviceList;
            memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

            int nRet = MV_OK;
            nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
            if (MV_OK != nRet)
            {
                printf("MV_CC_EnumDevices fail! nRet [%x]\n", nRet);
                break;
            }

            if (stDeviceList.nDeviceNum > 0) {
                this->_cam_type = "h";
                break;
            }

            //------------------------------检查深度摄像头是否可用------------------------------
            rs2::context ctx;
            rs2::device_list dev_list = ctx.query_devices(); 
            if (dev_list.size() > 0) {
                this->_cam_type = "d";
                break;
            }
        }while(0);
        return type_is_useful(this->_cam_type);
    }

    void print_cam(bool state) {
        if (state) {
            if (this->_cam_type == "i") {
                std::cout << "Now you are using industrial camera" << std::endl;
            }
            else if (this->_cam_type == "h") {
                std::cout << "Now you are using hik camera" << std::endl;
            }
            else if (this->_cam_type == "d") {
                std::cout << "Now you are using realsense camera" << std::endl;
            }
        }
        else {
            std::cout << "No cameras are available" << std::endl;
        }
    }

private:
    std::string _cam_type;                              // 摄像头的类型: h - hik摄像头; i - 工业摄像头; d - 深度摄像头
    cv::Mat _frame;                                     // 图片处理后的opencv frame
    hikcam _cam_hik;                                    // 海康摄像头
    cv::VideoCapture _cam_industry;                     // 工业摄像头
    rs2::pipeline _cam_realsense_pipe;
};