# Camera
> 代码可以随意更改，完成这个项目的时候作者C++的水平不是很高，你有觉得写的不合适的地方请修改完成后Push到仓库中
> 
> 如果有新摄像头的话建议也添加到这个项目中
> 
> 文章写于: 2024年3月13日
## 效果演示
https://github.com/BUPT-RobotTeam/cameras/assets/129849375/5ee9385b-6201-4331-a445-fcb4dec62171

## 环境要求
> 1. OpenCV
> 2. realsense2 SDK
> 3. MvCameraControl SDK

## 你可能需要修改
```camke
# 你可能需要手动修改以下两个路径
set(MVCAM_COMMON_RUNENV /opt/MVS/lib)
set(MVCAM_COMMON_PATH /opt/MVS)
```

## 效果说明
> 目的: 为了解决深度相机和海康摄像头C++二次重复开发的问题
> 
> 效果: 统一提供了get_frame() 接口(提供的是opencv的Mat类型数据), 可以自动完成三种摄像头的探测(工业相机，深度摄像头，海康摄像头)


## 各个函数说明
### 宏函数
```text
1. CAMERAS_CHECK(state, str)
函数功能: 根据state(bool)来判断是否输出str
state: True: 输出str, 并且return -1
       False: 不做任何事

使用示例:
CAMERAS_CHECK(cam.open(), "camera open error");
CAMERAS_CHECK(cam.start(), "camera start error");

```
### 类函数
```text
------------------------------初始化函数------------------------------
1. 默认初始化函数 cameras()
函数功能: 自动完成可用摄像头的探测, 初始化fps计算相关数据, 并打印你当前所正在使用的摄像头
使用示例: cameras cam;

2. 初始化函数  cameras(std::string cam_type)
函数功能: 根据字符串 cam_type 完成对指定类型摄像头的调用, 如果 cam_type 无效, 则自动检测设备上可用的摄像头, 同时完成初始化fps计算相关的数据, 打印你当前所正在使用的摄像头
使用示例: cameras cam("h");

需要注意: 
1. 在这两个初始化函数中都把改变了OpenCV的日志输出等级, 使他只能输出Warn级别以上(不包括Warn)的信息
2. 目前可支持的cam_type有三个，分别是 "i"(代表工业摄像头), "h"(代表海康工业摄像头), "d"(代表d435i深度摄像头)


------------------------------公有成员函数------------------------------
1. open()
函数功能: 完成打开摄像头的步骤
参数    : 无
返回值  : True - 摄像头打开成功, False - 摄像头打开失败
使用示例: CAMERAS_CHECK(cam.open(), "camera open error");


2. start()
函数功能: 完成开启摄像头抓图的步骤
参数    : 无
返回值  : True - 摄像头开始抓图成功, False - 摄像头开始抓图失败
使用示例: CAMERAS_CHECK(cam.start(), "camera start error");

3. stop()
函数功能: 关闭摄像头，并释放摄像头相关的资源
参数    : 无
返回值  : True - 摄像头关闭成功, False - 摄像头关闭失败
使用示例: CAMERAS_CHECK(cam.stop(), "camera stop error");

4. get_frame()
函数功能: 获取一张图片
参数    : 无
返回值  : CV::Mat 类型的图片数据
使用示例: frame = cam.get_frame();

5. get_depth()
函数功能: 获取指定坐标的深度数据(仅支持深度相机)
参数    : x - x坐标
          y - y坐标
返回值  : 指定坐标的深度数据, 如果不是深度相机的话, 将返回0.0
使用示例: depth = cam.get_depth(300, 300);

6. get_depth(int x, int y)
函数功能: 获取当前视频流的帧数
参数    : 无
返回值  : 当前视频流的帧数数据
使用示例: fps = cam.get_fps();

7. draw_mark(int x, int y, MarkType mark_type = MARK_CROSS, cv::Scalar color = cv::Scalar(0, 0, 255), int line_length = 20, int line_width = 3)
函数功能: 在指定的坐标绘制标记
参数    : x - x坐标
          y - y坐标
          mark_type - 标记类型 (目前支持 MARK_CROSS -> "十", MARK_X -> "X"), 默认为 MARK_CROSS
          color - 标记的颜色, 默认为(0, 0, 255)
          line_length - 线的长度, 默认为20个像素点
          line_width - 线的宽度, 默认为3个像素点
返回值  : 无
使用示例: cam.draw_mark(300, 300, cameras::MARK_X);


------------------------------私有成员函数------------------------------
1. type_is_useful(std::string cam_type)
函数功能: 判断相机的类型参数是否有效
参数    : cam_type - 相机类型(目前支持的类型有 "i"(代表工业摄像头), "h"(代表海康工业摄像头), "d"(代表d435i深度摄像头))
返回值  : True - 相机类型有效, False - 相机类型无效
使用示例: this->type_is_useful(this->_cam_type);

2. auto_detect_cam()
函数功能: 完成可用相机的自动检测
参数    : 无
返回值  : True - 摄像头可用, False - 摄像头不可用
使用示例: this->auto_detect_cam();

3. cam_is_accessible_industry()
函数功能: 检测工业摄像头是否可用
参数    : 无
返回值  : True - 工业摄像头可用, False - 工业摄像头不可用
使用示例: if (this->cam_is_accessible_industry()) {...}
原理    : 用OpenCV的VideoCapture打开摄像头, 如何摄像头被打开了, 说明可用(目前支持一个工业摄像头), 然后释放资源, 返回true

4. cam_is_accessible_hik()
函数功能: 检测海康摄像头是否可用
参数    : 无
返回值  : True - 海康摄像头可用, False - 海康摄像头不可用
使用示例: else if (this->cam_is_accessible_hik()) {...}
原理    : 枚举可用的海康设备, 并打开默认的(也就是0号位 - 也就是说目前也仅仅支持一个海康摄像头),  调用函数判断设备是否连接, 如果连接, 如果设备连接, 那就说明可以用, 然后释放资源, 返回True, 否则就返回False

5. cam_is_accessible_realsense()
函数功能: 检测深度摄像头是否可用
参数    : 无
返回值  : Ture - 深度摄像头可用, False - 深度摄像头不可用
使用示例: else if (this->cam_is_accessible_realsense()) {...}
原理    : 如果一条pipeline已经被start的话重复start的话会抛出异常, 去捕获这个异常, 然后判断设备上是否有可用的深度摄像头(目前深度摄像头也就仅仅支持一个)

6. print_cam_info(bool state)
函数功能: 打印你正在使用那个相机
参数    : state - type_is_useful() 的返回值
返回值  : 无
使用示例: this->print_cam_info(this->type_is_useful(this->_cam_type));

需要注意: 
1. 函数的功能总体上是按照上面所说的去写的，但是因为本质上三个摄像头使用的SDK还是不一样的，所有可能会有些出入



```





