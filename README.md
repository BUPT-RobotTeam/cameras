# Camera
> 代码可以随意更改，完成这个项目的时候作者C++的水平不是很高，你有觉得写的不合适的地方请修改完成后Push到仓库中
> 如果有新摄像头的话建议也添加到这个项目中
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

> 效果: 统一提供了get_frame() 接口(提供的是opencv的Mat类型数据), 可以自动完成三种摄像头的探测(工业相机，深度摄像头，海康摄像头)


## 各个函数说明
### 宏函数
1. CAMERAS_CHECK(state, str)
函数功能: 根据state(bool)来判断是否输出str
          state: 
                True: 输出str, 并且return -1
                False: 不做任何事
### 类函数





