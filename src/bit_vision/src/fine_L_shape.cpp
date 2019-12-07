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
#include "bit_vision/BrickLocate.h"

using namespace std;
using namespace HalconCpp;

// 左右相机定位目标的像素位置
static HTuple xL1,yL1,xR1,yR1;    
static HTuple xL2,yL2,xR2,yR2; 
//定义检测到关键点的个数
int point_num = 0;
//定义传递三维坐标的全局变量
static HTuple WallX_1,WallY_1,WallZ_1;
static HTuple WallX_2,WallY_2,WallZ_2;

// Procedure declarations 
// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'. 
void dev_update_off ();
// Local procedures 
void classify_color_regions (HObject ho_Image, HObject *ho_RegionSelected, HTuple hv_MLPHandle, 
    HTuple *hv_index, HTuple *hv_row, HTuple *hv_column);
void extract_region_lines (HObject ho_Image, HObject *ho_SelectedContours, HTuple *hv_Number);
void find_pose_from_region (HObject ho_RectangularRegions, HObject ho_Image, HTuple hv_CamParam, 
    HTuple hv_WindowHandle, HTuple hv_RectWidth, HTuple hv_RectHeight, HTuple hv_FileName, 
    HTuple *hv_PoseOut);
void gen_region_contours_lines (HObject ho_region, HObject *ho_SelectedContours, 
    HTuple hv_min1, HTuple hv_max1, HTuple *hv_Number2);
void select_max_area_region (HObject ho_Region, HObject *ho_ObjectSelected, HTuple *hv_Area, 
    HTuple *hv_Row, HTuple *hv_Column);

// Procedures 
// Chapter: Develop
// Short Description: Switch dev_update_pc, dev_update_var and dev_update_window to 'off'. 
void dev_update_off ()
{

  //This procedure sets different update settings to 'off'.
  //This is useful to get the best performance and reduce overhead.
  //
  // dev_update_pc(...); only in hdevelop
  // dev_update_var(...); only in hdevelop
  // dev_update_window(...); only in hdevelop
  return;
}

// Local procedures 
void classify_color_regions (HObject ho_Image, HObject *ho_RegionSelected, HTuple hv_MLPHandle, 
    HTuple *hv_index, HTuple *hv_row, HTuple *hv_column)
{

  // Local iconic variables
  HObject  ho_ClassRegions, ho_ClassRed, ho_ClassGreen;
  HObject  ho_ClassBLue, ho_ConnectedRegions1, ho_ConnectedRegions2;
  HObject  ho_ConnectedRegions3, ho_ObjectSelectedRed, ho_ObjectSelectedGreen;
  HObject  ho_ObjectSelectedBlue, ho_region1, ho_region2;

  // Local control variables
  HTuple  hv_Area_1, hv_Row_1, hv_Column_1, hv_Area_2;
  HTuple  hv_Row_2, hv_Column_2, hv_Area_3, hv_Row_3, hv_Column_3;
  HTuple  hv_areas, hv_rows, hv_columns, hv_Indices, hv_num;
  HTuple  hv_index2, hv_class;

  ClassifyImageClassMlp(ho_Image, &ho_ClassRegions, hv_MLPHandle, 0.9);

  SelectObj(ho_ClassRegions, &ho_ClassRed, 1);
  SelectObj(ho_ClassRegions, &ho_ClassGreen, 2);
  SelectObj(ho_ClassRegions, &ho_ClassBLue, 3);

  Connection(ho_ClassRed, &ho_ConnectedRegions1);
  Connection(ho_ClassGreen, &ho_ConnectedRegions2);
  Connection(ho_ClassBLue, &ho_ConnectedRegions3);

  select_max_area_region(ho_ConnectedRegions1, &ho_ObjectSelectedRed, &hv_Area_1, 
      &hv_Row_1, &hv_Column_1);
  select_max_area_region(ho_ConnectedRegions2, &ho_ObjectSelectedGreen, &hv_Area_2, 
      &hv_Row_2, &hv_Column_2);
  select_max_area_region(ho_ConnectedRegions3, &ho_ObjectSelectedBlue, &hv_Area_3, 
      &hv_Row_3, &hv_Column_3);

  //比较3种region的面积 面积最大的作为分类结果
  hv_areas.Clear();
  hv_areas.Append(hv_Area_1);
  hv_areas.Append(hv_Area_2);
  hv_areas.Append(hv_Area_3);
  hv_rows.Clear();
  hv_rows.Append(hv_Row_1);
  hv_rows.Append(hv_Row_2);
  hv_rows.Append(hv_Row_3);
  hv_columns.Clear();
  hv_columns.Append(hv_Column_1);
  hv_columns.Append(hv_Column_2);
  hv_columns.Append(hv_Column_3);

  TupleSortIndex(hv_areas, &hv_Indices);
  hv_num = hv_Indices.TupleLength();
  (*hv_index) = HTuple(hv_Indices[hv_num-1]);
  hv_index2 = HTuple(hv_Indices[hv_num-2]);

  if (0 != ((*hv_index)==0))
  {
    hv_class = "red";

  }
  else if (0 != ((*hv_index)==1))
  {
    hv_class = "green";

  }
  else if (0 != ((*hv_index)==2))
  {
    hv_class = "blue";

  }
  //选取第一种和第二种区域
  SelectObj(ho_ClassRegions, &ho_region1, (*hv_index)+1);
  SelectObj(ho_ClassRegions, &ho_region2, hv_index2+1);
  ConcatObj(ho_region1, ho_region2, &(*ho_RegionSelected));
  (*hv_row) = HTuple(hv_rows[(*hv_index)]);
  (*hv_column) = HTuple(hv_columns[(*hv_index)]);
  return;
}

void extract_region_lines (HObject ho_Image, HObject *ho_SelectedContours, HTuple *hv_Number)
{

  // Local iconic variables
  HObject  ho_ClassRegions, ho_ObjectSelected3;
  HObject  ho_ImageReduced, ho_Contours, ho_ContoursSplit;
  HObject  ho_SelectedContours1;

  // Local control variables
  HTuple  hv_PI, hv_Width, hv_Height, hv_pathFile;
  HTuple  hv_MLPHandle, hv_Number2;

  //输入是Image
  hv_PI = 3.14;
  GetImageSize(ho_Image, &hv_Width, &hv_Height);
  hv_pathFile = "box_segment_mlp.mlp";
  ReadClassMlp(hv_pathFile, &hv_MLPHandle);
  ClassifyImageClassMlp(ho_Image, &ho_ClassRegions, hv_MLPHandle, 0.5);
  //分割
  //提取的index由颜色分类决定
  SelectObj(ho_ClassRegions, &ho_ObjectSelected3, 3);
  ReduceDomain(ho_Image, ho_ObjectSelected3, &ho_ImageReduced);
  //输出是提取的轮廓
  GenContourRegionXld(ho_ObjectSelected3, &ho_Contours, "border");
  SegmentContoursXld(ho_Contours, &ho_ContoursSplit, "lines", 2, 4, 1);
  CountObj(ho_ContoursSplit, &(*hv_Number));
  hv_Number2 = (*hv_Number);
  while (0 != (hv_Number2!=2))
  {
    SelectContoursXld(ho_ContoursSplit, &(*ho_SelectedContours), "contour_length", 
        hv_Width/6, hv_Width, 0, 0);
    CountObj((*ho_SelectedContours), &hv_Number2);
    SelectContoursXld((*ho_SelectedContours), &ho_SelectedContours1, "direction", 
        (-hv_PI)/6, hv_PI/6, -0.5, 0.5);
    CountObj(ho_SelectedContours1, &hv_Number2);
  }
  return;
}

void find_pose_from_region (HObject ho_RectangularRegions, HObject ho_Image, HTuple hv_CamParam, 
    HTuple hv_WindowHandle, HTuple hv_RectWidth, HTuple hv_RectHeight, HTuple hv_FileName, 
    HTuple *hv_PoseOut)
{
  // Local iconic variables
  HObject  ho_RectDilated, ho_RectEroded, ho_RectBorders;
  HObject  ho_RectSelected, ho_ImageReduced, ho_Edges, ho_ContoursSplit;
  HObject  ho_SelectedEdges, ho_UnionContours, ho_Cross, ho_Edge;
  HObject  ho_Quadrangle, ho_ClosedContours;

  // Local control variables
  HTuple  hv_ErrorVar;
  HTuple  hv_Number, hv_I, hv_NumberEdges, hv_NumberFilteredEdges;
  HTuple  hv_RowBegin, hv_ColumnBegin, hv_RowEnd, hv_ColumnEnd;
  HTuple  hv_Nr, hv_Nc, hv_Dist, hv_J, hv_Row, hv_Column;
  HTuple  hv_IsOverlapping, hv_Rows, hv_Columns, hv_FinalRow;
  HTuple  hv_FinalColumn, hv_RowEdge, hv_ColumnEdge, hv_Poses;
  HTuple  hv_PoseCov, hv_Error, hv_ErrorAgain, hv_Pose, hv_HomMat3D;
  HTuple  hv_Qx, hv_Qy, hv_Qz;

  // dev_set_check ("~give_error")
  // Error variable 'hv_ErrorVar' activated
  hv_ErrorVar = 2;
  (*hv_PoseOut) = HTuple();
  //
  //Reduce the image to the border of the region and extract edges
  try
  {
    hv_ErrorVar = 2;
    DilationRectangle1(ho_RectangularRegions, &ho_RectDilated, 9, 9);
  }
  catch(HException e)
  {
    hv_ErrorVar = (int)e.ErrorCode();
    if (hv_ErrorVar < 0)
      throw e;
  }
  try
  {
    hv_ErrorVar = 2;
    ErosionRectangle1(ho_RectangularRegions, &ho_RectEroded, 9, 9);
  }
  catch(HException e)
  {
    hv_ErrorVar = (int)e.ErrorCode();
    if (hv_ErrorVar < 0)
      throw e;
  }
  try
  {
    hv_ErrorVar = 2;
    Difference(ho_RectDilated, ho_RectEroded, &ho_RectBorders);
  }
  catch(HException e)
  {
    hv_ErrorVar = (int)e.ErrorCode();
    if (hv_ErrorVar < 0)
      throw e;
  }
  try
  {
    hv_ErrorVar = 2;
    SelectShape(ho_RectBorders, &ho_RectBorders, "area", "and", 600, 99999);
  }
  catch(HException e)
  {
    hv_ErrorVar = (int)e.ErrorCode();
    if (hv_ErrorVar < 0)
      throw e;
  }
  try
  {
    hv_ErrorVar = 2;
    CountObj(ho_RectBorders, &hv_Number);
  }
  catch(HException e)
  {
    hv_ErrorVar = (int)e.ErrorCode();
    if (hv_ErrorVar < 0)
      throw e;
  }
  {
  HTuple end_val10 = hv_Number;
  HTuple step_val10 = 1;
  for (hv_I=1; hv_I.Continue(end_val10, step_val10); hv_I += step_val10)
  {
    try
    {
      hv_ErrorVar = 2;
      SelectObj(ho_RectBorders, &ho_RectSelected, hv_I);
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    try
    {
      hv_ErrorVar = 2;
      ReduceDomain(ho_Image, ho_RectSelected, &ho_ImageReduced);
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    //Extract contours and make them quadrangular
    try
    {
      hv_ErrorVar = 2;
      EdgesSubPix(ho_ImageReduced, &ho_Edges, "canny", 0.7, 20, 30);
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    try
    {
      hv_ErrorVar = 2;
      CountObj(ho_Edges, &hv_NumberEdges);
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    if (0 != (hv_NumberEdges==0))
    {
      return;
    }
    try
    {
      hv_ErrorVar = 2;
      SegmentContoursXld(ho_Edges, &ho_ContoursSplit, "lines", 7, 4, 2);
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    try
    {
      hv_ErrorVar = 2;
      SelectContoursXld(ho_ContoursSplit, &ho_SelectedEdges, "contour_length", 70, 
          1000, -0.5, 0.5);
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    try
    {
      hv_ErrorVar = 2;
      UnionAdjacentContoursXld(ho_SelectedEdges, &ho_UnionContours, 10, 1, "attr_keep");
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    try
    {
      hv_ErrorVar = 2;
      CountObj(ho_UnionContours, &hv_NumberFilteredEdges);
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    if (0 != (hv_NumberFilteredEdges!=4))
    {
      return;
    }
    try
    {
      hv_ErrorVar = 2;
      FitLineContourXld(ho_UnionContours, "tukey", -1, 0, 5, 2, &hv_RowBegin, &hv_ColumnBegin, 
          &hv_RowEnd, &hv_ColumnEnd, &hv_Nr, &hv_Nc, &hv_Dist);
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    //Find intersection points [Rows, Columns]
    for (hv_J=0; hv_J<=3; hv_J+=1)
    {
      try
      {
        hv_ErrorVar = 2;
        IntersectionLines(HTuple(hv_RowBegin[hv_J]), HTuple(hv_ColumnBegin[hv_J]), 
            HTuple(hv_RowEnd[hv_J]), HTuple(hv_ColumnEnd[hv_J]), HTuple(hv_RowBegin[(hv_J+1)%4]), 
            HTuple(hv_ColumnBegin[(hv_J+1)%4]), HTuple(hv_RowEnd[(hv_J+1)%4]), HTuple(hv_ColumnEnd[(hv_J+1)%4]), 
            &hv_Row, &hv_Column, &hv_IsOverlapping);
      }
      catch(HException e)
      {
        hv_ErrorVar = (int)e.ErrorCode();
        if (hv_ErrorVar < 0)
          throw e;
      }
      try
      {
        hv_ErrorVar = 2;
        GenCrossContourXld(&ho_Cross, hv_Row, hv_Column, 6, 0.785398);
      }
      catch(HException e)
      {
        hv_ErrorVar = (int)e.ErrorCode();
        if (hv_ErrorVar < 0)
          throw e;
      }
      hv_Rows[hv_J] = hv_Row;
      hv_Columns[hv_J] = hv_Column;
    }
    //Merge points
    hv_FinalRow = HTuple();
    hv_FinalColumn = HTuple();
    for (hv_J=1; hv_J<=4; hv_J+=1)
    {
      try
      {
        hv_ErrorVar = 2;
        SelectObj(ho_UnionContours, &ho_Edge, hv_J);
      }
      catch(HException e)
      {
        hv_ErrorVar = (int)e.ErrorCode();
        if (hv_ErrorVar < 0)
          throw e;
      }
      try
      {
        hv_ErrorVar = 2;
        GetContourXld(ho_Edge, &hv_RowEdge, &hv_ColumnEdge);
      }
      catch(HException e)
      {
        hv_ErrorVar = (int)e.ErrorCode();
        if (hv_ErrorVar < 0)
          throw e;
      }
      hv_FinalRow = (hv_FinalRow.TupleConcat(hv_RowEdge.TupleSelectRange(5,(hv_RowEdge.TupleLength())-5))).TupleConcat(HTuple(hv_Rows[hv_J-1]));
      hv_FinalColumn = (hv_FinalColumn.TupleConcat(hv_ColumnEdge.TupleSelectRange(5,(hv_ColumnEdge.TupleLength())-5))).TupleConcat(HTuple(hv_Columns[hv_J-1]));
    }
    try
    {
      hv_ErrorVar = 2;
      GenContourPolygonXld(&ho_Quadrangle, hv_FinalRow, hv_FinalColumn);
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    //Process contour
    //dev_set_color ('medium slate blue')
    //dev_display (Quadrangle)
    try
    {
      hv_ErrorVar = 2;
      CountObj(ho_Quadrangle, &hv_NumberEdges);
    }
    catch(HException e)
    {
      hv_ErrorVar = (int)e.ErrorCode();
      if (hv_ErrorVar < 0)
        throw e;
    }
    if (0 != (hv_NumberEdges>0))
    {
      try
      {
        hv_ErrorVar = 2;
        CloseContoursXld(ho_Quadrangle, &ho_ClosedContours);
      }
      catch(HException e)
      {
        hv_ErrorVar = (int)e.ErrorCode();
        if (hv_ErrorVar < 0)
          throw e;
      }
      try
      {
        hv_ErrorVar = 2;
        GetRectanglePose(ho_ClosedContours, hv_CamParam, hv_RectWidth, hv_RectHeight, 
            "tukey", 2, &hv_Poses, &hv_PoseCov, &hv_Error);
      }
      catch(HException e)
      {
        hv_ErrorVar = (int)e.ErrorCode();
        if (hv_ErrorVar < 0)
          throw e;
      }
      hv_ErrorAgain = hv_ErrorVar;
      if (0 != (hv_ErrorAgain==2))
      {
        //Store pose
        hv_Pose = hv_Poses.TupleSelectRange(0,6);
        try
        {
          hv_ErrorVar = 2;
          PoseToHomMat3d(hv_Pose, &hv_HomMat3D);
        }
        catch(HException e)
        {
          hv_ErrorVar = (int)e.ErrorCode();
          if (hv_ErrorVar < 0)
            throw e;
        }
        try
        {
          hv_ErrorVar = 2;
          AffineTransPoint3d(hv_HomMat3D, (HTuple(0).Append(0)), (HTuple(0).Append(0)), 
              (HTuple(0).Append(1)), &hv_Qx, &hv_Qy, &hv_Qz);
        }
        catch(HException e)
        {
          hv_ErrorVar = (int)e.ErrorCode();
          if (hv_ErrorVar < 0)
            throw e;
        }
        //Invert (if necessary) direction of z axis to point towards the camera
        if (0 != ((HTuple(hv_Qz[1])-HTuple(hv_Qz[0]))>0))
        {
          hv_Pose[4] = HTuple(hv_Pose[4])+180;
          hv_Pose[5] = -HTuple(hv_Pose[5]);
        }
        (*hv_PoseOut) = (*hv_PoseOut).TupleConcat(hv_Pose);
      }
    }
  }
  }
  // Error variable 'hv_ErrorVar' deactivated
  // dev_set_check ("give_error")
  return;
}

void gen_region_contours_lines (HObject ho_region, HObject *ho_SelectedContours, 
    HTuple hv_min1, HTuple hv_max1, HTuple *hv_Number2)
{

  // Local iconic variables
  HObject  ho_Contours, ho_SortedContours, ho_Lines;
  HObject  ho_LinesSelected;

  // Local control variables
  HTuple  hv_PI, hv_Number, hv_I;

  hv_PI = 3.14;
  GenContourRegionXld(ho_region, &ho_Contours, "border");
  SegmentContoursXld(ho_Contours, &(*ho_SelectedContours), "lines", 2, 4, 1);
  CountObj((*ho_SelectedContours), &hv_Number);
  //可以根据方向和长度来对边缘线段进行选择
  //可以在循环中选择 直到只有两个线段被选择出来
  (*hv_Number2) = hv_Number;
  while (0 != ((*hv_Number2)!=2))
  {
    SelectContoursXld((*ho_SelectedContours), &(*ho_SelectedContours), "contour_length", 
        hv_min1, hv_max1, 0, 0);
    CountObj((*ho_SelectedContours), &(*hv_Number2));
    SelectContoursXld((*ho_SelectedContours), &(*ho_SelectedContours), "direction", 
        (-hv_PI)/6, hv_PI/6, -0.5, 0.5);
    CountObj((*ho_SelectedContours), &(*hv_Number2));
    //可以增加条件 选出最长的两条直线
    //根据视野进行线段的筛选
    SortContoursXld((*ho_SelectedContours), &ho_SortedContours, "upper_left", "true", 
        "row");
    GenEmptyObj(&ho_Lines);
    for (hv_I=1; hv_I<=2; hv_I+=1)
    {
      SelectObj(ho_SortedContours, &ho_LinesSelected, hv_I);
      if (HDevWindowStack::IsOpen())
        DispObj(ho_LinesSelected, HDevWindowStack::GetActive());
      ConcatObj(ho_LinesSelected, ho_Lines, &ho_Lines);
      CountObj(ho_Lines, &(*hv_Number2));
    }

  }
  return;
}

void select_max_area_region (HObject ho_Region, HObject *ho_ObjectSelected, HTuple *hv_Area, 
    HTuple *hv_Row, HTuple *hv_Column)
{

  // Local iconic variables

  // Local control variables
  HTuple  hv_Area1, hv_Row1, hv_Column1, hv_Indices;
  HTuple  hv_num;

  AreaCenter(ho_Region, &hv_Area1, &hv_Row1, &hv_Column1);
  TupleSortIndex(hv_Area1, &hv_Indices);
  hv_num = hv_Indices.TupleLength();
  SelectObj(ho_Region, &(*ho_ObjectSelected), HTuple(hv_Indices[hv_num-1])+1);
  AreaCenter((*ho_ObjectSelected), &(*hv_Area), &(*hv_Row), &(*hv_Column));
  return;
  return;
}

// 以rgb为输入,输出左端点的上下两点坐标
void action(HObject ho_Image,HTuple *hv_x1, HTuple *hv_x2, HTuple *hv_y1,HTuple *hv_y2)
{

  // Local iconic variables
  HObject  ho_Image1, ho_Image2, ho_Image3;
  HObject  ho_Regions, ho_ConnectedRegions, ho_SelectedRegions;
  HObject  ho_RegionOpening, ho_ImageReduced, ho_Cross, ho_Contours;
  HObject  ho_Rectangle1;


  // Local control variables
  HTuple  hv_CameraParam, hv_Width1, hv_Height1;
  HTuple  hv_WindowHandle, hv_Row1, hv_Column, hv_SigmaGrad;
  HTuple  hv_SigmaSmooth, hv_Threshold, hv_Col1, hv_Number;
  HTuple  hv_Row, hv_Phi1, hv_Length1, hv_Length2, hv_PointOrder;
  HTuple  hv_angle, hv_Row2, hv_Col2;

  //读入相机参数模型
  ReadCamPar("/home/srt/test_ws/src/bit_vision/model/campar1.dat", 
      &hv_CameraParam);
  //读入第一张图像 用于识别砖块的轮廓
  GetImageSize(ho_Image, &hv_Width1, &hv_Height1);
  Decompose3(ho_Image, &ho_Image1, &ho_Image2, &ho_Image3);
  //阈值分割的范围可能稍后还要调
  Threshold(ho_Image1, &ho_Regions, 15, 72);
  Connection(ho_Regions, &ho_ConnectedRegions);
  SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("rectangularity").Append("area")), 
      "and", (HTuple(0.9).Append(20000)), (HTuple(1).Append(1000000)));
  OpeningCircle(ho_SelectedRegions, &ho_RegionOpening, 3.5);

  ReduceDomain(ho_Image, ho_RegionOpening, &ho_ImageReduced);
  hv_SigmaGrad = 1.0;
  hv_SigmaSmooth = 2.0;
  hv_Threshold = 100000;
  //
  PointsHarris(ho_ImageReduced, hv_SigmaGrad, hv_SigmaSmooth, 0.04, hv_Threshold, 
      &hv_Row1, &hv_Col1);
  GenCrossContourXld(&ho_Cross, hv_Row1, hv_Col1, 6, HTuple(45).TupleRad());

  //计数点的个数是否等于2
  CountObj(ho_Cross, &hv_Number);
  if (0 != (hv_Number!=2))
  {
    GenContourRegionXld(ho_RegionOpening, &ho_Contours, "border");
    FitRectangle2ContourXld(ho_Contours, "regression", -1, 0, 0, 3, 2, &hv_Row, &hv_Column, 
        &hv_Phi1, &hv_Length1, &hv_Length2, &hv_PointOrder);
    GenRectangle2ContourXld(&ho_Rectangle1, hv_Row, hv_Column, hv_Phi1, hv_Length1, 
        hv_Length2);
    hv_angle = (hv_Phi1*180)/3.14;
    //
    //获取拟合矩形的左侧两个端点
    GetContourXld(ho_Rectangle1, &hv_Row2, &hv_Col2);
    //记住后一个是横坐标,前一个是列坐标
    (*hv_x1) = ((const HTuple&)hv_Col2)[1];
    (*hv_x2) = ((const HTuple&)hv_Col2)[2];

    (*hv_y1) = ((const HTuple&)hv_Row2)[1];
    (*hv_y2) = ((const HTuple&)hv_Row2)[2];
  }
  else if (0 != (hv_Number==2))
  {
    (*hv_x1) = ((const HTuple&)hv_Col1)[0];
    (*hv_x2) = ((const HTuple&)hv_Col1)[1];

   (*hv_y1) = ((const HTuple&)hv_Row1)[0];
   (*hv_y2)= ((const HTuple&)hv_Row1)[1];
  }
  point_num = 2;

}

void LeftCallback(const sensor_msgs::Image::ConstPtr& msg) 
{
    //初始化halcon对象
    HObject  ho_Image;
    //获取halcon-bridge图像指针
    halcon_bridge::HalconImagePtr halcon_bridge_imagePointer = halcon_bridge::toHalconCopy(msg);
    ho_Image = *halcon_bridge_imagePointer->image;
    
    // 处理左图图像
    action(ho_Image,&xL1,&xL2,&yL1,&yL2);
}

void RightCallback(const sensor_msgs::Image::ConstPtr& msg) 
{
    
    //初始化halcon对象
    HObject  ho_Image;
    //获取halcon-bridge图像指针
    halcon_bridge::HalconImagePtr halcon_bridge_imagePointer = halcon_bridge::toHalconCopy(msg);
    ho_Image = *halcon_bridge_imagePointer->image;
    
    // 处理右图图像
    action(ho_Image,&xR1,&xR2,&yR1,&yR2);

}

//定义根据标定参数 定位三维点的函数
int stero_location(HTuple row_L, HTuple column_L, HTuple row_R, HTuple column_R,
HTuple *hv_X, HTuple *hv_Y, HTuple *hv_Z)
{
   
  HTuple  hv_CameraParameters1,hv_CameraParameters2, hv_RealPose;
  HTuple  hv_Dist;
  
  //三维定位

    ReadCamPar("/home/srt/test_ws/src/bit_vision/model/campar1.dat", &hv_CameraParameters1);
    ReadCamPar("/home/srt/test_ws/src/bit_vision/model/campar2.dat", &hv_CameraParameters2);
    ReadPose("/home/srt/test_ws/src/bit_vision/model/relpose.dat", &hv_RealPose);

    IntersectLinesOfSight(hv_CameraParameters2, hv_CameraParameters2, hv_RealPose, 
    row_L, column_L, row_R, column_R, hv_X, hv_Y, hv_Z, &hv_Dist);
    return 0;
 }
  


// service 回调函数，输入参数req，输出参数res
bool GetLocateData(bit_vision::BrickLocate::Request& ,
                   bit_vision::BrickLocate::Response& res)
{
  //双目定位墙架的端点三维坐标
  stero_location(xL1,yL1,xR1,yR1,&WallX_1,&WallY_1,&WallZ_1);
  stero_location(xL2,yL2,xR2,yR2,&WallX_2,&WallY_2,&WallZ_2);
  if (point_num == 2)   // 检测到角点
  {
    res.LocateData.header.stamp = ros::Time().now();
    res.LocateData.header.frame_id = "zed_link";

    res.LocateData.Flag = true;

    res.LocateData.position.x = WallX_1.D();
    res.LocateData.position.y = WallY_1.D();
    res.LocateData.position.z = WallZ_1.D();

    //但实际上有2个点,要怎么填进来?

    // 发布TF   zed_link——>target_link
    static tf::TransformBroadcaster br;
    tf::Transform transform;
    transform.setOrigin(tf::Vector3(WallX_1.D(), WallY_1.D(), WallZ_1.D()));
    tf::Quaternion q;
    q.setRPY(0, 0, 0);
    transform.setRotation(q);
    br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "zed_link", "target_link"));

  }
  else    // 如果没有识别结果
  {
    res.LocateData.header.stamp = ros::Time().now();
    res.LocateData.header.frame_id = "zed_link";

    res.LocateData.Flag = false;
    res.LocateData.BrickType = "NULL";
    res.LocateData.position.x = 0.0;
    res.LocateData.position.y = 0.0;
    res.LocateData.position.z = 0.0;
  }
}



int main(int argc, char *argv[])
{
  ros::init(argc, argv, "wall_locate");

  ros::NodeHandle nh; 

  // 接收zed左右相机图像
  ros::Subscriber subLeft  = nh.subscribe("/zed/zed_node/left/image_rect_color", 1, LeftCallback);
  ros::Subscriber subRight = nh.subscribe("/zed/zed_node/right/image_rect_color", 1, RightCallback);  
  // 服务-计算砖堆位置
  ros::ServiceServer service = nh.advertiseService("GetLocateData",GetLocateData);

  ROS_INFO_STREAM("Ready to get brick locate info");

  ros::spin();

  return 0;
}

