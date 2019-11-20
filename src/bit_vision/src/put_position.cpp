///////////////////////////////////////////////////////////////////////////////
// File generated by HDevelop for HALCON/C++ Version 18.05
///////////////////////////////////////////////////////////////////////////////



#ifndef __APPLE__
#  include "HalconCpp.h"
#  include "HDevThread.h"
#  if defined(__linux__) && !defined(__arm__) && !defined(NO_EXPORT_APP_MAIN)
#    include <X11/Xlib.h>
#  endif
#else
#  ifndef HC_LARGE_IMAGES
#    include <HALCONCpp/HalconCpp.h>
#    include <HALCONCpp/HDevThread.h>
#  else
#    include <HALCONCppxl/HalconCpp.h>
#    include <HALCONCppxl/HDevThread.h>
#  endif
#  include <stdio.h>
#  include <HALCON/HpThread.h>
#  include <CoreFoundation/CFRunLoop.h>
#endif

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include "HalconCpp.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include "ros/ros.h"
#include <sstream>
#include <math.h>
#include "halcon_image.h"
#include "sensor_msgs/Image.h"
#include "std_msgs/Empty.h"
#include "tf/transform_broadcaster.h"
#include "bit_vision/BrickPosition.h"
#include <opencv2/core/core.hpp>

#include<iostream>
#undef Success  
#include<eigen3/Eigen/Core>
#include<eigen3/Eigen/Geometry>
#include <eigen_conversions/eigen_msg.h>


using namespace std;
using namespace HalconCpp;
using namespace cv;


//定义全局变量 用于三维坐标的传递
static HTuple xL1,yL1,xL2,yL2;
static HTuple xR3,yR3,xR4,yR4; 
//全局变量 用于传递由当前点到指定点需要移动的方向与距离
static HTuple X1,Y1,Z1,X2,Y2,Z2;
static HObject brickregion;
int Proc_states = 0;
string brick_color;

// Procedure declarations 
// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'. 
void dev_update_off ();

// Procedures 
// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'. 
void dev_update_off ()
{

  return;
}


// Main procedure 
//如果一个程序需要输入序列图像应该如何写输入参数呢?
//第一个函数 输入为只有拾取砖块的图像以及砖块的颜色,输出为砖块对应的区域,以及砖块边缘直线与法线的交点
void action1(HObject ho_Image1,HObject *ho_BrickRegion,HTuple *hv_x1,HTuple *hv_y1)
{

  // Local iconic variables
  HObject  ho_ClassRegions;
  HObject  ho_Contours, ho_SelectedContours;
  HObject  ho_SortedContours, ho_Line1, ho_Image2, ho_ClassRegions2;
  HObject  ho_RegionDifference, ho_WallRegion, ho_RegionIntersection;
  HObject  ho_ImageReduced, ho_WallContours, ho_WallLines;
  HObject  ho_Lines, ho_wall, ho_Line2;

  // Local control variables
  HTuple  hv_pathFile, hv_MLPHandle, hv_Width, hv_Height;
  HTuple  hv_PI, hv_Number, hv_Attrib, hv_RowBegin, hv_ColBegin;
  HTuple  hv_RowEnd, hv_ColEnd, hv_Nr1, hv_Nc1, hv_Dist1;
  HTuple  hv_Number2, hv_RowBegin2, hv_ColBegin2;
  HTuple  hv_RowEnd2, hv_ColEnd2, hv_Nr2, hv_Nc2, hv_Dist2;
  HTuple  hv_x3, hv_y3;
  HTuple  hv_index;

  //input at least 2 images
  dev_update_off();
  if (HDevWindowStack::IsOpen())
    CloseWindow(HDevWindowStack::Pop());
  //读入训练好的分割模型
  hv_pathFile = "/home/moyang/mbzirc_ws/src/bit_vision/model/box_segment_mlp_retrain.mlp";
  ReadClassMlp(hv_pathFile, &hv_MLPHandle);
  //读入第一张图像 用于识别砖块的轮廓
  
  GetImageSize(ho_Image1, &hv_Width, &hv_Height);
  
  ClassifyImageClassMlp(ho_Image1, &ho_ClassRegions, hv_MLPHandle, 0.5);
  //基于先前举起砖块时做的颜色分类结果 先选择砖块对应的区域
  //根据输入颜色 给index赋值
  
  if (0 != (brick_color=="red"))
  {
    hv_index = 1;
  }
  else if (0 != (brick_color=="green"))
  {
    hv_index = 2;
  }
  else if (0 != (brick_color=="blue"))
  {
    hv_index = 3;
  }

  SelectObj(ho_ClassRegions, ho_BrickRegion, hv_index);
  ReduceDomain(ho_Image1, *ho_BrickRegion, &ho_ImageReduced);
  //*****************************************************************************
  //提取剩余区域的轮廓
  hv_PI = 3.14;
  GenContourRegionXld(ho_ImageReduced, &ho_Contours, "border");
  SegmentContoursXld(ho_Contours, &ho_SelectedContours, "lines", 2, 4, 1);
  CountObj(ho_SelectedContours, &hv_Number);
  //如果检测到的直线个数为0 则处理下一张图像
 
  //去掉竖直的线段
  SelectContoursXld(ho_SelectedContours, &ho_SelectedContours, "direction", (-hv_PI)/6, 
      hv_PI/6, -0.5, 0.5);
  CountObj(ho_SelectedContours, &hv_Number);
  //可以增加条件 选出最长的两条直线
  //根据视野进行线段的筛选
  SortContoursXld(ho_SelectedContours, &ho_SortedContours, "upper_left", "true", 
      "row");
  GenEmptyObj(&ho_Line1);
  SelectObj(ho_SortedContours, &ho_Line1, 1);
  //拟合拾取砖块的上边缘直线方程
  GetContourGlobalAttribXld(ho_Line1, "cont_approx", &hv_Attrib);
  if (0 != (hv_Attrib==-1))
  {
    //得到直线上两个点的参数
    FitLineContourXld(ho_Line1, "tukey", 2, 0, 5, 2, &hv_RowBegin, &hv_ColBegin, 
        &hv_RowEnd, &hv_ColEnd, &hv_Nr1, &hv_Nc1, &hv_Dist1);
  }
  //得到左图中直线方程
  //计算第一条直线与法向量的交点
  (*hv_x1) = hv_Nr1*hv_Dist1;
  (*hv_y1) = hv_Nc1*hv_Dist1;
  //还需要结合右图的输入来确定这个点的三维坐标
  //***************************************************************************
}

//*********************************************************************************//
//需要设置flag来决定何时运行action1或action2

//action2函数的输入是包含墙体的图像,墙体的颜色,拾取砖块在图像中的区域,输出是墙体边缘与法线的交点
void action2(HObject ho_Image2,HObject ho_BrickRegion,HTuple *hv_x3,HTuple *hv_y3)
{
  // Local iconic variables
  HObject  ho_Image1, ho_ClassRegions;
  HObject  ho_ImageReduced, ho_Contours, ho_SelectedContours;
  HObject  ho_SortedContours, ho_Line1, ho_ClassRegions2;
  HObject  ho_RegionDifference, ho_WallRegion, ho_RegionIntersection;
  HObject  ho_ImageReduced2, ho_WallContours, ho_WallLines;
  HObject  ho_Lines, ho_wall, ho_Line2;

  // Local control variables
  HTuple  hv_pathFile, hv_MLPHandle, hv_Width, hv_Height;
  HTuple  hv_PI, hv_Number, hv_Attrib, hv_RowBegin, hv_ColBegin;
  HTuple  hv_RowEnd, hv_ColEnd, hv_Nr1, hv_Nc1, hv_Dist1;
  HTuple  hv_x1, hv_y1, hv_Number2, hv_RowBegin2, hv_ColBegin2;
  HTuple  hv_RowEnd2, hv_ColEnd2, hv_Nr2, hv_Nc2, hv_Dist2;
  HTuple  index;

  //读入第二张图像

  GetImageSize(ho_Image2, &hv_Width, &hv_Height);
  //读入分类器
  hv_pathFile = "/home/moyang/mbzirc_ws/src/bit_vision/model/box_segment_mlp_retrain.mlp";
  ReadClassMlp(hv_pathFile, &hv_MLPHandle);
  ClassifyImageClassMlp(ho_Image2, &ho_ClassRegions2, hv_MLPHandle, 0.5);
  //将拾取砖块对应的区域去除
  Difference(ho_Image2, ho_BrickRegion, &ho_RegionDifference);
  //基于墙体砖块颜色选择区域
  if (0 != (brick_color=="red"))
  {
    index = 1;
  }
  else if (0 != (brick_color=="green"))
  {
    index = 2;
  }
  else if (0 != (brick_color=="blue"))
  {
    index = 3;
  }
  //取基于颜色判断的区域交集
  SelectObj(ho_ClassRegions2, &ho_WallRegion, index);
  Intersection(ho_WallRegion, ho_RegionDifference, &ho_RegionIntersection);
  ReduceDomain(ho_Image2, ho_RegionIntersection, &ho_ImageReduced2);
  //*******************************************************************************
  //提取剩余区域的轮廓
  hv_PI = 3.14;
  GenContourRegionXld(ho_ImageReduced2, &ho_WallContours, "border");
  SegmentContoursXld(ho_WallContours, &ho_WallLines, "lines", 2, 4, 1);
  CountObj(ho_WallLines, &hv_Number2);
  //如果检测到的直线个数为0 则处理下一张图像
  
  //去掉竖直的线段
  SelectContoursXld(ho_WallLines, &ho_Lines, "direction", (-hv_PI)/6, hv_PI/6, -0.5, 
      0.5);
  CountObj(ho_Lines, &hv_Number2);
  //可以增加条件 选出最长的两条直线
  //根据视野进行线段的筛选
  SortContoursXld(ho_Lines, &ho_wall, "upper_left", "true", "row");
  GenEmptyObj(&ho_Line2);
  SelectObj(ho_wall, &ho_Line2, 1);
  //拟合拾取砖块的上边缘直线方程
  GetContourGlobalAttribXld(ho_Line2, "cont_approx", &hv_Attrib);
  if (0 != (hv_Attrib==-1))
  {
    //得到直线上两个点的参数
    FitLineContourXld(ho_Line2, "tukey", 2, 0, 5, 2, &hv_RowBegin2, &hv_ColBegin2, 
        &hv_RowEnd2, &hv_ColEnd2, &hv_Nr2, &hv_Nc2, &hv_Dist2);
  }
  //得到左图中墙体边缘的直线方程
  //***************************************************************************************************
  //得到墙体边缘直线与法向量的交点
  (*hv_x3) = hv_Nr2*hv_Dist2;
  (*hv_y3) = hv_Nc2*hv_Dist2;
}


void LeftCallback(const sensor_msgs::Image::ConstPtr& msg) 
{
    //初始化halcon对象
    HObject  ho_Image;
    //获取halcon-bridge图像指针
    halcon_bridge::HalconImagePtr halcon_bridge_imagePointer = halcon_bridge::toHalconCopy(msg);
    ho_Image = *halcon_bridge_imagePointer->image;
    // 处理左图图像

    //根据flag分别判断运行action1 或action2函数
    if (Proc_states == 1)
    {
      action1(ho_Image,&brickregion,&xL1,&yL1); //获取左图像中的拾取砖块边缘上的点
    }
    else if (Proc_states == 2)
    {
      action2(ho_Image,brickregion,&xL2,&yL2); //获取左图像中的墙体砖块边缘上的点
    }  

}

void RightCallback(const sensor_msgs::Image::ConstPtr& msg) 
{
    //初始化halcon对象
    HObject  ho_Image;
    //获取halcon-bridge图像指针
    halcon_bridge::HalconImagePtr halcon_bridge_imagePointer = halcon_bridge::toHalconCopy(msg);
    ho_Image = *halcon_bridge_imagePointer->image;
    
    // 处理右图图像

    //根据flag判断使用action1函数或action2函数
    if (Proc_states == 1)
    {
      action1(ho_Image,&brickregion,&xR3,&yR3);//右图拾取砖块边缘上的点
    }
    else if (Proc_states == 2)
    {
      action2(ho_Image,brickregion,&xR4,&yR4);//右图墙体砖块边缘上的点
    }   
}

//定义根据标定参数 定位三维点的函数
//以左右视图中的行列作为输入,以定位的三维点X,Y,Z作为输出
int stero_location(HTuple row_L, HTuple column_L, HTuple row_R, HTuple column_R,HTuple *Line_X, HTuple *Line_Y, HTuple *Line_Z)
{
  HTuple  hv_CameraParameters1,hv_CameraParameters2, hv_RealPose;
  HTuple  hv_X, hv_Y, hv_Z,hv_Dist;
  
  //三维定位
  try
  {
    ReadCamPar("/home/moyang/mbzirc_ws/src/bit_vision/model/campar1.dat", &hv_CameraParameters1);
    ReadCamPar("/home/moyang/mbzirc_ws/src/bit_vision/model/campar2.dat", &hv_CameraParameters2);
    ReadPose("/home/moyang/mbzirc_ws/src/bit_vision/model/relpose.dat", &hv_RealPose);

    IntersectLinesOfSight(hv_CameraParameters1, hv_CameraParameters2, hv_RealPose, 
    row_L, column_L, row_R, column_R, &hv_X, &hv_Y, &hv_Z, &hv_Dist);

    (*Line_X) = HTuple(hv_X);
    (*Line_Y) = HTuple(hv_Y);
    (*Line_Z) = HTuple(hv_Z);

    return 0;
  }
  catch (HException &exception)
  {
    ROS_ERROR("  Error #%u in %s: %s\n", exception.ErrorCode(),
            (const char *)exception.ProcName(),
            (const char *)exception.ErrorMessage());
    return 1;
  }
}

//已知P1,P2 利用eigen求相对位姿转换

int calculate_pose(geometry_msgs::Quaternion& q)
{
    //1.p1 world position
    double p1yaw=0;
    double p1x=X1;
    double p1y=Y1;
    double p1z=Z1;
    Eigen::AngleAxisd rotzp1(p1yaw*M_PI/180, Eigen::Vector3d::UnitZ());
    Eigen::Vector3d  t1= Eigen::Vector3d(p1x,p1y, p1z);
    Eigen::Quaterniond q1=Eigen::Quaterniond(rotzp1);

   //2.p2 world position
    double p2yaw=0;
    double p2x=X2;
    double p2y=Y2;
    double p2z=Z2;
    Eigen::AngleAxisd rotzp2(p2yaw*M_PI/180, Eigen::Vector3d::UnitZ());
    Eigen::Vector3d  t2= Eigen::Vector3d(p2x,p2y, p2z);
    Eigen::Quaterniond q2=Eigen::Quaterniond(rotzp2);

    // 3.calculate T12
    //q1*q12=q2
    //求解姿态为q12
    Eigen::Quaterniond q12=q1.inverse()*q2;
    Eigen::Vector3d  t12=q1.toRotationMatrix().inverse()*(t2-t1);
    /// Converts an Eigen Quaternion into a Quaternion message
    tf::quaternionEigenToMsg(q12, q);

}



// service 回调函数，输入参数req，输出参数res
bool GetPutData(bit_vision::BrickPosition::Request& req,
                bit_vision::BrickPosition::Response& res)
{
  ROS_INFO_STREAM("BrickType:["<<req.BrickType<<"], Proc_states:["<<req.Proc_states<<"]");
  ROS_INFO("%d",req.Proc_states);
  brick_color = req.BrickType;
  if (req.Proc_states == 1)
  {
    Proc_states = 1;
  }
  else if (req.Proc_states == 2)
  {
    Proc_states = 2;
    ros::Duration(0.2).sleep();  // 等待双目线程处理结果
    if (stero_location(xL1,yL1,xR3,yR3,&X1,&Y1,&Z1)==0&&stero_location(xL2,yL2,xR4,yR4,&X2,&Y2,&Z2)==0)   // 如果有识别结果
    {
      res.PositionData.header.stamp = ros::Time().now();
      res.PositionData.header.frame_id = "zed_link";
      res.PositionData.Flag = true;

      res.PositionData.Pose.position.x = X2-X1;
      res.PositionData.Pose.position.y = Y2-Y1;
      res.PositionData.Pose.position.z = Z2-Z1;

      geometry_msgs::Quaternion q;
      calculate_pose(q);
      res.PositionData.Pose.orientation = q;

      // 发布TF   zed_link——>target_link
      static tf::TransformBroadcaster br;
      geometry_msgs::TransformStamped transformStamped;
      transformStamped.header.stamp = ros::Time().now();
      transformStamped.header.frame_id = "zed_link";
      transformStamped.child_frame_id = "target_link";
      transformStamped.transform.translation.x = X2-X1;
      transformStamped.transform.translation.y = Y2-Y1;
      transformStamped.transform.translation.z = Z2-Z1;
      transformStamped.transform.rotation = q;

      br.sendTransform(transformStamped);

      ROS_INFO_STREAM("Location is : "<<X2-X1<<","<<Y2-Y1<<","<<Z2-Z1);
    }
    else    // 如果没有识别结果
    {
      res.PositionData.header.stamp = ros::Time().now();
      res.PositionData.header.frame_id = "zed_link";

      res.PositionData.Flag = false;
      res.PositionData.BrickType = "NULL";
      res.PositionData.Pose.position.x = 0.0;
      res.PositionData.Pose.position.y = 0.0;
      res.PositionData.Pose.position.z = 0.0;
      res.PositionData.Pose.orientation.x = 0.0;
      res.PositionData.Pose.orientation.y = 0.0;
      res.PositionData.Pose.orientation.z = 0.0;
      res.PositionData.Pose.orientation.w = 0.0;
    }
  }
}

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "put_position");

  ros::NodeHandle nh("~"); 

  // 接收zed左右相机图像
  ros::Subscriber subLeft  = nh.subscribe("/zed/zed_node/left/image_rect_color", 1, LeftCallback);
  ros::Subscriber subRight = nh.subscribe("/zed/zed_node/right/image_rect_color", 1, RightCallback);  
  // 服务-计算砖堆位置
  ros::ServiceServer service = nh.advertiseService("GetPutData",GetPutData);

  // 初始化左右相机定位数据

  ROS_INFO_STREAM("Ready to get brick locate info");

  ros::spin();

  return 0;
}


