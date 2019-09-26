/*============================================================================*/
/*                                                                            */
/*                            C O P Y R I G H T                               */
/*                                                                            */
/*                          (C) Copyright 2019 by                             */
/*                              Yang Guojun                                   */
/*                           All Rights Reserved                              */
/*                                                                            */
/*      The author assumes no responsibility for the use or reliability of    */
/*      his software.                                                         */
/*                                                                            */
/*============================================================================*/
////////////////////////////////////////////////////////////////////////////////
//                                  版权申明                                  //
//                         版权所有(C)2006-2019，杨国君                       //
//                                保留全部权利                                //
////////////////////////////////////////////////////////////////////////////////
/* ############################################################################################################################## */

#pragma once

#include <windows.h>
#include <tchar.h>

namespace NsCChart{

//用于动态链接库的时候，注释掉#else中的第二行
//用于静态库或者源码时，注释掉#else中的第一行
#ifdef CChart_EXPORTS
#	define CChart_API __declspec(dllexport)
#else
#	define CChart_API __declspec(dllimport)
//#	define CChart_API
#endif
	
	
// CChart包装类支持的绘图类型
enum
{
	kTypeXY,							//0 折线图
	kTypePie,							//1 饼图
	kTypeStem,							//2 柱图
	kTypeOscillo,						//3 模拟示波器图
	kTypeContourLine,					//4 等高线图
	kTypeContourMap,					//5 云图
	kTypeContour,						//6 等高线云图
	kTypeXY3D	,						//7 瀑布图

	kTypeSingleLayerCount,				// 单层视图数目
	
	kTypeSplit = kTypeSingleLayerCount,	//8 分裂视图
	kTypeShareX,						//9 共享X轴视图
	kTypeLayered,						//10 分层视图
	
	kType2DCount,						// 二维视图数目

	kType3DLine = kType2DCount,			//11 3维折线图
	kType3DSurface,						//12 3维曲面图
	
	kTypeCount			//CChart所支持的视图类型数
};

const struct stChartType
{
	int nChartType;
	TCHAR szChartType[24];
}ChartTypes[] =
{
	{kTypeXY			,_T("TypeXY")},
	{kTypePie			,_T("TypePie")},
	{kTypeStem			,_T("TypeStem")},
	{kTypeOscillo		,_T("TypeOscillo")},
	{kTypeContourLine	,_T("TypeContourLine")},
	{kTypeContourMap	,_T("TypeContourMap")},
	{kTypeContour		,_T("TypeContour")},
	{kTypeXY3D			,_T("TypeXY3D")},
	{kTypeSplit			,_T("TypeSplit")},
	{kTypeShareX		,_T("TypeShareX")},
	{kTypeLayered		,_T("TypeLayered")},
	{kType3DLine		,_T("Type3DLine")},
	{kType3DSurface		,_T("Type3DSurface")},
};

// 图像整体缩放时的缩放中心
// 如果取消注释，可以直接用代号，否则用代码
enum
{
	kZoomCenterLT,		//0 左上
	kZoomCenterLB,		//1 左下
	kZoomCenterRT,		//2 右上
	kZoomCenterRB,		//3 右下
	kZoomCenterCT,		//4 中心
	kZoomCenterARB,		//5 任意点，主要用于鼠标位置
	
	kZoomCenterCount
};

/*

// 曲线绘制方式
enum
{
	kXYPlotScatter = 0,		//0 散点图
	kXYPlotConnect = 1,		//1 连线图
	kXYPlotStepHV = 2,		//2 台阶图先水平再竖直
	kXYPlotStepVH = 3,		//3 台阶图先竖直再水平
	kXYPlotStepHVH = 4,		//4 台阶图水平竖直水平
	kXYPlotStepVHV = 5,		//5 台阶图竖直水平竖直
	kXYPlotBezier = 6,		//6 贝塞尔连线图
	kXYPlotBar = 7,			//7 每个点只绘制从坐标轴开始的数据棒
  
	kPlotTypeCount
};

 // 曲线多色模式下颜色节点的输入方式（AddSegColor第一个参数的含义）
 enum
 {
	kSegColorPointRatio = 0,	//0 数据点的比例
	kSegColorXVal = 1,			//1 X数值
	kSegColorYVal,				//2 Y数值
  
	kSegColorCount
};

// 数据标记的形状
enum
{
	kXYMarkerNone = 0,			//0 无标记
	kXYMarkerCircle = 1,		//1 圆圈标记
	kXYMarkerSquareUpright = 2,	//2 正正方形
	kXYMarkerSquareOblique = 3,	//3 斜正方形
	kXYMarkerTriangleLeft = 4,	//4 左向三角形
	kXYMarkerTriangleRight = 5,	//5 右向三角形
	kXYMarkerTriangleUp = 6,	//6 上三角形
	kXYMarkerTriangleDown = 7,	//7 下三角形
	kXYMarkerX = 8,				//8 斜十字
	kXYMarkerCross = 9,			//9 正十字
	kXYMarkerBar = 10,			//10颜色棒
	kXYMarkerDot = 11,			//11点
		
	kMarkerTypeCount
};

 // 曲线填充的方式
enum
{
	kDataFillClosed = 0,			//0 封闭填充
	kDataFillFromBottomAxis = 1,	//1 底轴填充
	kDataFillFromTopAxis = 2,		//2 顶轴填充
	kDataFillFromLeftAxis = 3,      //3 左轴填充
	kDataFillFromRightAxis = 4,		//4 右轴填充
  
	kDataFillModeCount
};
// 数据棒显示方式
enum
{
	kDataBarBaseBottom,		//0 从底部开始
	kDataBarBaseTop,		//1 从顶部开始
	kDataBarBaseLeft,		//2 从左部开始
	kDataBarBaseRight,		//3 从右部开始
		
	kDataBarBaseCount
};

// 分裂视图的分裂模式
// 如果取消注释，可以直接用代号，否则用代码，例如2表示左一右二的三分裂
enum
{
	kSplitNot=0,		//0 不分裂
	kSplitNM=1,			//1 行列分割
	kSplit3L1R2=2,		//2 左1右2
	kSplit3L2R1=3,		//3 左2右1
	kSplit3T1B2=4,		//4 上1下2
	kSplit3T2B1=5,		//5 上2下1
		
	kSplitModeCount
};

// 坐标轴位置
// 如果取消注释，可以直接用代号，否则用代码
enum
{
	kLocationLeft = 0,	//0 左轴
	kLocationBottom,	//1 下轴
	kLocationRight,		//2 右轴
	kLocationTop,		//3 上轴
	kLocationCenterVL,	//4 中轴竖直偏左
	kLocationCenterVR,	//5 中轴竖直偏右
	kLocationCenterHB,	//6 中轴水平偏下
	kLocationCenterHT,	//7 中轴水平偏上
  
	kLocationTDX,		//8 三维X轴
	kLocationTDY,		//9 三维Y轴
	kLocationTDZ,		//10三维Z轴
	
	kLocationCount
};

// 图例位置
enum
{
	kLegendArbitrary,									//0 内部任意位置，可拖动
  
	kLegendInnerLeft,									//1 内左
	kLegendInnerLeftTop,								//2 内左上
	kLegendInnerTopLeft = kLegendInnerLeftTop,			//2 内上左
	kLegendInnerLeftBottom,								//3 内左下
	kLegendInnerBottomLeft = kLegendInnerLeftBottom,	//3 内下左
	kLegendInnerRight,									//4 内右
	kLegendInnerRightTop,								//5 内右上
	kLegendInnerTopRight = kLegendInnerRightTop,		//5 内上右
	kLegendInnerRightBottom,							//6 内右下
	kLegendInnerBottomRight = kLegendInnerRightBottom,	//6 内下右
	kLegendInnerTop,									//7 内上
	kLegendInnerBottom,									//8 内下
	
	kLegendLeft,										//9 左
	kLegendLeftTop,										//10左上
	kLegendLeftBottom,									//11左下
	kLegendRight,										//12右
	kLegendRightTop,									//13右上
	kLegendRightBottom,									//14右下
	kLegendTop,											//15上
	kLegendTopLeft,										//16上左
	kLegendTopRight,									//17上右
	kLegendBottom,										//18下
	kLegendBottomLeft,									//19下左
	kLegendBottomRight,									//20下右
  
	kLegendPostionCount									//共21种选项
};

// 柱图起始位置
enum
{
	kStemBaseBottom		=	0,		//下
	kStemBaseTop		=	1,		//上
	kStemBaseLeft		=	2,		//左
	kStemBaseRight		=	3,		//右
  
	kStemBaseCount		=	4
};
 // how to show the mouse tool tip
 enum
{
	kDesCoords,
	kDesXY,
	kDesElements,
	kDesXAndYAll,
		
	kDesCount
};

// 对话框、右键菜单使用的语言
enum
{
	kLangEnglish,		//0 英语
	kLangChinese,		//1 汉语
	kLangCount
};
*/

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Declaration of class CChart

class	CChart_API	CChart
{
public:	
	// CChart内部数据结构
	struct stChartPara;
	// 获得内部数据的指针
	stChartPara *GetPara();
protected:
	// 内部数据
	stChartPara	*m_pPara;
	// 释放内部资源
	void		Release();	
	
public:
	CChart();
	~CChart();

public:
	// 设置对话框、右键菜单使用的语言
	static	void	SetLangurage(int nLang);
	// 设置数据处理菜单是否显示
	static	void	SetDataProcessing(bool process);
	
	// 测试是否启用工具提示
	static	bool	IsEnableToolTip();
	// 设置是否启用工具提示
	static	void	SetEnableToolTip(bool enable);
	// 检查视图号是否有效
	static	bool	CheckPlotType(int nType);	
	
	// 设置默认颜色配置表
	static	void	SetColorTable(int nTableIndex);

	// 设置是否显示图像处理的右键菜单
	static	void	SetImagePopupMenu(bool bShow);

public:
	// 拷贝图像到剪贴板
	void		CopyImageToClipBoard(HWND hWnd);
	// 保存图像到文件
	void		SaveImageToFile(HWND hWnd, TCHAR *strFileName, TCHAR *strFileExt);
	// 打印图像
	void		PrintImage(HWND hWnd);

public:
	// 获取内部Plot指针
	void		*GetPlot();
	// 获取图像类型编码
	int			GetType();
	// 设置图像类型编码，注意将清空所有数据
	bool		SetType(int nType);
	// 设置图像类型编码，并设置绘图窗口，主要用于三维视图，二维视图将忽略hWnd
	bool		SetType(int nType, HWND hWnd);
	// 设置图像类型编码，注意将清空所有数据
	bool		CreateChart(int nType);
	// 设置图像类型编码，并设置绘图窗口，主要用于三维视图，二维视图将忽略hWnd
	bool		CreateChart(int nType, HWND hWnd);

	// 获得标识符，用于CChartWnd
	int			GetPlotID();
	// 设置标识符，用于CChartWnd
	void		SetPlotID(int nID);
	// 获得预置窗口
	HWND		GetWnd();
	// 设置预置窗口
	void		SetWnd(HWND hWnd);
	// 测试是否限制绘图区
	bool		IsRectConfined();
	// 设置是否限制绘图区
	void		SetRectConfined(bool confine);
	// 设置限制区域
	bool		SetConfineRect(RECT ConfineRect);
	// 获取限制区域
	RECT		GetConfineRect(); 

	// 测试内部是否无数据
	bool		IsEmpty();
	
	// 重置外观数据
	void		ResetApperance();
	// 获得外观模式序号
	int			GetApperanceMode();
	// 设置外观模式
	void		SetApperanceMode(int mode);

	// 设置绘图完成后，用户代码可以接着绘图
	void		SetContinuePaint(bool ctn);
	// 设置消息处理后，用户代码可以接着处理
	void		SetContinueReact(bool ctn);

public:
	// 测试是否响应鼠标键盘消息
	bool		IsReact();
	// 设置是否响应鼠标键盘消息
	void		SetReact(bool react);
	// 设置是否响应左键单击按下消息
	void		SetReactLButtonDown(bool react);
	// 设置是否响应左键单击抬起消息
	void		SetReactLButtonUp(bool react);
	// 设置是否响应左键双击消息
	void		SetReactLButtonDblClk(bool react);
	// 设置是否响应鼠标移动消息
	void		SetReactMouseMove(bool react);
	// 设置是否响应右键菜单消息
	void		SetReactContextMenu(bool react);
	// 设置是否响应按键消息
	void		SetReactKeyDown(bool react);
	// 设置是否响应标题
	void		SetReactTitle(bool react);
	// 设置是否响应坐标轴
	void		SetReactAxes(bool react);
	// 设置是否响应图例
	void		SetReactLegend(bool react);
	// 测试消息响应时是否自动刷新，起作用于末尾带R的几个函数和Interactive
	// 主要用于实时曲线显示,此时设置为false
	bool		IsAutoRedraw();
	// 设置消息响应时是否自动刷新，起作用于末尾带R的几个函数和Interactive
	// 主要用于实时曲线显示,此时设置为false
	void		SetAutoRedraw(bool redraw);
	// 设置绘图区偏移
	void		SetOffsetClient(int offsetX, int offsetY);

public:
	// 对SetWnd()设置的窗口句柄绘图
	virtual	void		OnDraw();
	// 对窗口句柄绘图
	virtual	void		OnDraw(HWND hWnd);
	// 对设备上下文绘图
	virtual	void		OnDraw(HDC hDC);
	// 在窗口上选择区域绘图
	virtual	void		OnDraw(HWND hWnd, RECT destRect);
	// 在设备上下文对应的窗口上，选择区域绘图
	virtual	void		OnDraw(HDC hDC, RECT destRect);

public:
	// 单个的鼠标键盘消息，但不重绘，由用户根据返回值重绘
	virtual	bool		OnLButtonDown( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnLButtonUp( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnLButtonDblClk( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnMouseMove( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnMouseLeave( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnMouseWheel( HWND hWnd, POINT point, UINT ctrlKey );
	virtual	bool		OnContextMenu( HMENU hMenu, HWND hWnd, POINT point );
	virtual	bool		OnKeyDown( HWND hWnd, UINT key );

	// 单个的鼠标键盘消息，在AutoRedraw为真情况下重绘
	void				OnLButtonDownR( HWND hWnd, POINT point, UINT ctrlKey );
	void				OnLButtonUpR( HWND hWnd, POINT point, UINT ctrlKey );
	void				OnLButtonDblClkR( HWND hWnd, POINT point, UINT ctrlKey );
	void				OnMouseMoveR( HWND hWnd, POINT point, UINT ctrlKey );
	void				OnMouseLeaveR( HWND hWnd, POINT point, UINT ctrlKey );
	void				OnMouseWheelR( HWND hWnd, POINT point, UINT ctrlKey );
	void				OnContextMenuR( HMENU hMenu, HWND hWnd, POINT point );
	void				OnKeyDownR( HWND hWnd, UINT key );

	// 响应消息，但不重绘，由用户根据返回值重绘
	virtual	bool		OnEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// 响应消息，在AutoRedraw为真情况下重绘
	void				Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	// 增加右键菜单项，插件菜单功能
	void		SetPluginMenu(void (*fcnAddPluginMenu)( void *plot, HMENU addMenu, void *pPara ), int (*fcnInterpretPluginMenu)( void *plot, HWND hWnd, int result, void *pPara ), void *pParaAdd, void *pParaInt);
	// 隐藏或显示缺省右键菜单，结合插件菜单功能可实现右键菜单全部自定义
	void		SetDefMenus(bool bShow);
	// 用户自定义绘图操作
	void		SetUserDrawingFunc(void	(*fcnUserDrawing)( void *plot, HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara);
	// 用户自定义绘图操作
	void		SetUserDrawingFunc(void	(*fcnUserDrawing)( void *plot, HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara, int nPlotIndex);
	// 用户自定义更新操作
	void		SetUserUpdateFunc(void	(*fcnUserUpdate)( void *plot, void *pPara ), void *pPara);
	// 用户自定义更新操作
	void		SetUserUpdateFunc(void	(*fcnUserUpdate)( void *plot, void *pPara ), void *pPara, int nPlotIndex);
	
	// 用户自定义鼠标移动前操作，bContinue设置为false跳过默认例程
	void		SetPreMouseMoveFunc(bool	(*fcnPreMouseMove)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// 用户自定义鼠标单击前操作，bContinue设置为false跳过默认例程
	void		SetPreLButtonDownFunc(bool	(*fcnPreLButtonDown)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// 用户自定义鼠标抬起前操作，bContinue设置为false跳过默认例程
	void		SetPreLButtonUpFunc(bool	(*fcnPreLButtonUp)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// 用户自定义鼠标双击前操作，bContinue设置为false跳过默认例程
	void		SetPreLButtonDblClkFunc(bool	(*fcnPreLButtonDblClk)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// 用户自定义按键前操作，bContinue设置为false跳过默认例程
	void		SetPreKeyDownFunc(bool	(*fcnPreKeyDown)( void *plot, HDC hDC, UINT key, void *para, bool &bContinue ), void *pPara);
	
	// 用户自定义鼠标移动后操作，bContinue被忽略
	void		SetPostMouseMoveFunc(bool	(*fcnPostMouseMove)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// 用户自定义鼠标单击后操作，bContinue被忽略
	void		SetPostLButtonDownFunc(bool	(*fcnPostLButtonDown)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// 用户自定义鼠标抬起后操作，bContinue被忽略
	void		SetPostLButtonUpFunc(bool	(*fcnPostLButtonUp)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// 用户自定义鼠标双击后操作，bContinue被忽略
	void		SetPostLButtonDblClkFunc(bool	(*fcnPostLButtonDblClk)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara);
	// 用户自定义按键后操作，bContinue被忽略
	void		SetPostKeyDownFunc(bool	(*fcnPreKeyDown)( void *plot, HDC hDC, UINT key, void *para, bool &bContinue ), void *pPara);

public:
	// 获得子图序号，仅仅用于kTypeSplit
	int			GetPlotIndexByMousePos(POINT point);
	
public:
	// 设置在没有数据的时候是否画坐标轴及标题
	void		SetDrawBasicAnyway(bool draw);
	// 设置在没有数据的时候子图是否画坐标轴及标题
	void		SetDrawBasicAnyway(bool draw, int nPlotIndex);
	// 设置无数据时默认屏幕
	void		SetDefScreen( void (*fcnDefScreen)( void *plot, HDC hDC, RECT plotRect, void *pPara ), void *pPara );
	// 设置无数据时子图默认屏幕
	void		SetDefScreen( void (*fcnDefScreen)( void *plot, HDC hDC, RECT plotRect, void *pPara ), void *pPara, int nPlotIndex );
	// 设置默认屏幕的主字符串
	void		SetDefMainString(TCHAR *str);
	// 设置子图默认屏幕的主字符串
	void		SetDefMainString(TCHAR *str, int nPlotIndex);
	//  设置默认屏幕的副字符串
	void		SetDefSubString(TCHAR *str);
	//  设置子图默认屏幕的副字符串
	void		SetDefSubString(TCHAR *str, int nPlotIndex);
	//  设置默认屏幕的背景图
	void		SetDefImage(HBITMAP hbm);
	//  设置子图默认屏幕的背景图
	void		SetDefImage(HBITMAP hbm, int nPlotIndex);


public:
	// 检测子图序号是否在范围内
	bool		CheckSubPlotIndex(int nPlotIndex);

public:
	// 获得工具提示字符串
	TCHAR		*GetDescriptionByMousePos(HDC hDC, POINT point);
	// 设置工具提示字符串类型
	void		SetToolTipType(int type);

public:
	// 设置内置缩放模式，打开后可响应鼠标拖放
	void		SetZoomModeBuildIn(bool zoom);
	// 重置缩放范围
	void		ResetZoomedRanges();
	// 设置图像整体缩放模式，只是绘制，适用于不采用CChartWnd的情况
	void		SetZoomMode(bool zoom);
	// 设置图像整体缩放时X方向的缩放系数
	void		SetZoomFactorX(double fact);
	// 设置图像整体缩放时Y方向的缩放系数
	void		SetZoomFactorY(double fact);
	// 设置图像整体缩放时两个方向的缩放系数
	void		SetZoomFactor(double fact);
	// 设置图像整体缩放时的缩放中心，中心的代号见本文件开始被注释掉的enum
	void		SetZoomCenterMode(int center);
protected:
	// 内部调用函数
	RECT		GetZoomedRect(RECT destRect);

public:	
	// 添加曲线，前三个参数是曲线数据
	// 第四个参数主要用于分裂视图、分层视图、共享X轴视图，表示子图序号。后续函数相同
	// 返回值是曲线的ID号，注意不是序号，返回-1表示添加曲线失败。后续函数相同
	int			AddCurve(double *pX, double *pY, int nLen, int nPlotIndex=0);
	// 添加曲线，其中X坐标为时间，用字符串形式输入
	// 字符串格式由fmt确定，参考C库函数strftime
	// 时间只能精确到秒，因为内部使用time_t
	int			AddCurve(TCHAR **pStrTime, TCHAR *fmt, double *pY, int nLen, int nPlotIndex=0);
	// 添加空曲线
	int			AddCurve(int nPlotIndex=0);
	// 更新曲线数据
	int			UpdateCurve(int nDataIndex, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// 根据ID号更新曲线数据
	int			UpdateCurveByID(int nDataID, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// 替换曲线数据，不更新范围
	int			ReplaceCurve(int nDataIndex, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// 根据ID号替换曲线数据，不更新范围
	int			ReplaceCurveByID(int nDataID, double *pX, double *pY, int nLen, int nPlotIndex=0);
	// 从曲线的DataID获得Index
	int			GetIndex(int dataID, int nPlotIndex=0);
	// 从曲线的Index获得DataID
	int			GetDataID(int nIndex, int nPlotIndex=0);
	// 设置曲线的dataID，特殊用途，属于倚天剑屠龙刀，不要随便调用
	void		SetDataID(int newID, int nIndex, int nPlotIndex=0);

	// 添加2维曲线的一个数据点
	// nDataIndex表示曲线的序号,
	// nDataIndex如果等于当前曲线的条数，则新增一条曲线，如果小于0或者大于当前曲线条数，则添加失败
	int			AddPoint2D(double x, double y, int nDataIndex=0, int nPlotIndex=0);
	// 添加数据点，其中X坐标为时间，用字符串形式输入
	// 字符串格式由fmt确定，参考C库函数strftime
	// 时间只能精确到秒，因为内部使用time_t
	int			AddPoint2D(TCHAR *strTime, TCHAR *fmt, double y, int nDataIndex=0, int nPlotIndex=0);
	// 插入2维曲线的一个数据点
	// nPos表示需要插入的位置,
	int			InsertPoint2D(double x, double y, int nPos, int nDataIndex=0, int nPlotIndex=0);

	// 添加三维曲线的一个数据点
	int			AddPoint3D(double x, double y, double z, int nDataIndex=0);
	// 添加一条三维曲线
	int			AddCurve(double *pX, double *pY, double *pZ, int nLen);	

	// 清除所有曲线数据
	void		ClrAllData();
	// 清除单子图全部曲线数据
	void		ClrPlotData(int nPlotIndex=0);
	// 清除单条曲线数据
	void		ClrSingleData(int nDataIndex, int nPlotIndex=0);
	// 清空单条曲线数据
	void		EmptySingleData(int nDataIndex, int nPlotIndex=0);
	// 为曲线预留内存空间。主要目的是加快速度
	// CChart内部数据采用vector。如果数据点过多，随着数据点的增加，CChart将不断地为vector重新分配内存，影响速度
	// 如果根据实际情况预留了内存，则会一次性成功分配内存
	void		SetReservedDataLength(int nLen, int nPlotIndex=0);
	// 获得曲线的数据点数
	int			GetDataPointsCount(int nDataIndex, int nPlotIndex=0);
	// 获取数据点x
	double		GetDataPointX(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 获取数据点y
	double		GetDataPointY(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	//获取数据点
	bool		GetDataPoint(double data[2], int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 去除曲线上单个数据点
	int			RemoveSinglePoint(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 去除曲线上第一个数据点
	int			RemoveFirstPoint(int nDataIndex, int nPlotIndex=0);
	// 去除曲线上第一个数据点，同时内部数据左移，可保证占用内存的位置不变，但效率比RemoveFirstPoint低。
	int			RemoveFirstPointAndShift(int nDataIndex, int nPlotIndex=0);
	// 去除曲线上最后一个数据点
	int			RemoveLastPoint(int nDataIndex, int nPlotIndex=0);
	// 滑动X数据
	int			SlipXValue(int nStep, int nDataIndex, int nPlotIndex=0);
	// 滑动Y数据
	int			SlipYValue(int nStep, int nDataIndex, int nPlotIndex=0);

public:
	// 添加饼图的一个值
	int			AddPie(double value);
	// 添加饼图的一个值，同时设置该块饼的标题
	int			AddPie(double value, TCHAR* title);
	// 添加柱图的一个序列
	int			AddStems(double *pData, int nLen);
	// 添加柱图的一个序列，同时设置该序列的标题
	int			AddStems(double *pData, int nLen, TCHAR* title);

public:
	// 设置场函数，用于等高线图、云图
	void		SetFieldFcn(double (*_pFieldFcn) (double, double));
#ifndef _WIN64
	// 设置场函数，用于等高线图、云图
	void		SetFieldFcn(double (_stdcall *_pFieldFcn) (double, double));
#endif
	// 获得场函数指针，用于等高线图、云图
	double		(*GetFieldFcn( ))( double, double );
	// 添加等高线数据点
	void		AddContourPoint(double x, double y, double h);
	// 清除等高线数据点
	void		ClrContourPoints();
public:
	// 设置饼图单个饼的标题
	void		SetPieTitle(const TCHAR* title, int which);
	// 设置饼图单个饼的颜色
	void		SetPieColor(COLORREF color, int which);
	// 设置饼图最大饼数
	void		SetMaxPies(int maxp);
	// 获得饼图饼标题字体
	LOGFONT		&GetPieTitleFont();
	
public:
	// 设置柱图单根柱子的标签
	void		SetStemLabel(const TCHAR* label, int which);
	// 设置柱图单个序列的标题
	void		SetStemTitle(const TCHAR* title, int nDataIndex);
	// 设置柱图单根柱子所占比例
	void		SetStemRatio(double ratio);
	// 设置柱图从那条坐标轴开始画
	void		SetStemBase(int base);
	// 设置柱图的最大数量
	void		SetStemMaxNum(int num);

public:
	// 设置等高线图等高线的数量
	void		SetContourLineNum(int num);
	// 设置等高线图等高线绘制的精度，n越大，精度越高，一般5就可以了
	void		SetContourPrecision(int n);
	// 设置显示等高线数值
	void		SetContourValueShow(bool show);
	
	// 设置云图的起始颜色
	void		SetContourMapColor1(COLORREF cr);
	// 设置云图的最终颜色
	void		SetContourMapColor2(COLORREF cr);
	
	// 设置由数据点输入，而不是场函数
	void		SetContourByPoints();
	
	// 设置云图的右坐标轴作为图例
	void		SetRightAxisAsLegend(bool as);
	// 设置云图的上坐标轴作为图例
	void		SetTopAxisAsLegend(bool as);

public:
	// 增加一个图层，用于分层视图、共享X轴视图
	int			AddLayer();
	// 设置子图数量，用于分裂视图，其中mode表示分裂模式，nRows表示列数，nCols表示行数
	// 分裂模式参加本文件头部被注释掉的enum。可以解除注释，就能直接使用这些enum了。
	void		ResizePlots(int mode, int nRows, int nCols);
	// 设置图层数量，用于分层视图、共享X轴视图
	void		ResizePlots(int nLayers);
	// 设置分裂视图子图数
	static	bool	ResizeSplit(CChart *pChart, int nSubPlots);

	// 获得子图数量
	int			GetSubPlotCount();

	// 有子图时获得当前子图序号
	int			GetTop();

	// 设置多层视图的图层是否颜色归一化
	void		SetUniteLayerColor(bool bUnite);
	// 设置多层视图图层颜色归一化时是否包括曲线
	void		SetExcludeDataWhenUniteColor(bool bExclude, int nPlotIndex);
public:
	// 获得曲线数量，对于多层视图为子图曲线条数
	int			GetPlotDataCount(int nPlotIndex=0);
	// 设置单条曲线是否响应鼠标键盘消息
	void		SetDataReact(bool react, int nDataIndex, int nPlotIndex=0);
	// 强制使用GDI+进行渲染
	void		SetDataForceGdiPlus(bool bForce, int nDataIndex, int nPlotIndex=0);
	// 获得数据曲线的标题,nDataIndex表示曲线的序号
	const TCHAR*		GetDataTitle(int nDataIndex, int nPlotIndex=0);
	// 设置数据标题
	void		SetDataTitle(const TCHAR* title, int nDataIndex, int nPlotIndex=0);
	// 所有数据自动设置颜色
	void		SetAutoDataColorGeneral(bool bAuto, int nPlotIndex=0);
	// 数据自动设置颜色
	void		SetAutoDataColor(bool bAuto, int nDataIndex, int nPlotIndex=0);
	// 获得数据颜色
	COLORREF	GetDataColor(int nDataIndex, int nPlotIndex=0);
	// 设置数据颜色
	void		SetDataColor(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// 是否双色渐变模式
	bool		IsBiColorMode(int nDataIndex, int nPlotIndex=0);
	// 设置双色渐变模式
	void		SetBiColorMode(bool bBi, int nDataIndex, int nPlotIndex=0);
	// 获得双色渐变模式下第二数据颜色
	COLORREF	GetDataColor2(int nDataIndex, int nPlotIndex=0);
	// 设置双色渐变模式下第二数据颜色
	void		SetDataColor2(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// 是否多色渐变模式，优先于双色渐变
	bool		IsMultiColorMode(int nDataIndex, int nPlotIndex=0);
	// 设置多色渐变模式
	void		SetMultiColorMode(bool bMul, int nDataIndex, int nPlotIndex=0);
	// 获得多色渐变模式下输入节点的方法，0：数据点比例0.0-1.0，1：X数据值，2：Y数据值
	int			GetMultiColorInputType(int nDataIndex, int nPlotIndex=0);
	// 设置多色渐变模式下输入节点的方法
	void		SetMultiColorInputType(int nType, int nDataIndex, int nPlotIndex=0);
	// 设置多色渐变模式的节点，其中ratio的的范围由输入方法决定
	void		AddSegmentColor(double ratio, COLORREF color, int nDataIndex, int nPlotIndex=0);
	// 是否逐数据点定色
	bool		IsColorPtByPt(int nDataIndex, int nPlotIndex=0);
	// 是否逐数据点定色
	void		SetColorPtByPt(bool bBy, int nDataIndex, int nPlotIndex=0);
	// 添加数据点颜色，注意，添加的颜色数据点必须和数据点个数完全一样，不然没有作用
	void		AddDataPointColor(COLORREF color, int nDataIndex, int nPlotIndex=0);
	// 设置数据点颜色
	void		SetDataPointColor(COLORREF color, int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 删除一个数据点颜色
	void		EraseDataPointColor(int nPointIndex, int nDataIndex, int nPlotIndex=0);
	// 插入一个数据点颜色
	void		InsertDataPointColor(COLORREF color, int nPointIndex, int nDataIndex, int nPlotIndex=0);

	// 获得数据线型
	int			GetDataLineStyle(int nDataIndex, int nPlotIndex=0);
	// 设置数据线型
	void		SetDataLineStyle(int nStyle, int nDataIndex, int nPlotIndex=0);
	// 获得数据线宽
	int			GetDataLineSize(int nDataIndex, int nPlotIndex=0);
	// 设置数据线宽
	void		SetDataLineSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// 设置曲线的绘制方式
	void		SetPlotType(int nType, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点标记是否显示
	void		SetMarkerShow(bool bShow, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点标记是否填充
	void		SetMarkerFill(bool bFill, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点标记的形状
	void		SetMarkerType(int nType, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点的大小
	void		SetMarkerSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点标记频率，0表示不标记，正n表示每隔n个点标记一次，负n表示一共标记n个点
	void		SetMarkerFreq(int nFreq, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点标记的起始点，用于不是全部标记时
	void		SetMarkerStartIndex(int nStartIndex, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点第一个点标记
	void		SetMarkFirstPoint(bool bMark, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点最后一个点标记
	void		SetMarkLastPoint(bool bMark, int nDataIndex, int nPlotIndex=0);

	// 设置曲线散点显示模式
	void		SetScatter(int nDataIndex, int nPlotIndex=0);
	// 设置曲线填充颜色
	void		SetDataFillColor(bool bFill, COLORREF color, int nOpaque, int nFillMode, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据棒显示
	void		SetDataBarMode(bool bShowBar, int nBarBase, int nBarSize, int nDataIndex, int nPlotIndex=0);
	// 设置选择和高亮模式
	void		SetSLMode(int nMode, int nDataIndex, int nPlotIndex=0);
	// 获取数据的标志，特殊用途
	bool		GetDataFlag(int nDataIndex, int nPlotIndex=0);
	// 设置数据的标志，特殊用途
	void		SetDataFlag(bool bFlag, int nDataIndex, int nPlotIndex=0);
	// 获取数据可见标志
	bool		IsDataVisible(int nDataIndex, int nPlotIndex=0);
	//设置数据是否可见
	void		SetDataVisible(bool bVis, int nDataIndex, int nPlotIndex=0);
	// 设置压缩显示模式，主要用于大数据量时的显示
	// 压缩显示模式中，如果相邻数据点在屏幕上显示时的X相同，将压缩成一个数据点，值取平均，
	void		SetDataCompactDraw(bool bCompact, int nDataIndex, int nPlotIndex=0);
	// 设置数据按直方图模式显示
	void		SetDataHistro(bool bHistro, int nDataIndex, int nPlotIndex=0);

	// 获得高亮曲线的序号，目前只适用于折线图
	int			GetIndexOfLighted();
	
	// 设置曲线数据点数值是否显示
	void		SetDataValueShow(bool bShow, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点数值显示方式，1为X，2为Y，3为X:Y，其它为不显示
	void		SetDataValueType(int nType, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点数值的字体大小
	void		SetDataValueFontSize(int nSize, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点数值显示频率，0表示不显示，正n表示每隔n个点显示一次，负n表示一共显示n个点
	void		SetDataValueFreq(int nFreq, int nDataIndex, int nPlotIndex=0);
	// 设置曲线数据点数值显示的起始点，用于不是全部显示时
	void		SetDataValueStartIndex(int nStartIndex, int nDataIndex, int nPlotIndex=0);

public:
	// 检测示波器模式
	bool		IsOscilloMode(int nPlotIndex=0);
	// 设置示波器模式
	void		SetOscilloMode(bool bOscillo, int nPlotIndex=0);
	// 检测是否已启动示波器模式
	bool		IsStartOscilloScope(int nPlotIndex=0);
	// 设置是否已启动示波器模式
	void		SetStartOscilloScope(bool bOscillo, int nPlotIndex=0);
	// 检测是否可以拖动曲线
	bool		IsEnableDataDrag();
	// 设置是否可以拖动曲线
	void		SetEnableDataDrag(bool enable);
	// 检测是否可以拖动区间边线
	bool		IsEnableZoneDrag();
	// 设置是否可以拖动区间边线
	void		SetEnableZoneDrag(bool enable);
	// 移动曲线
	bool		MoveData(double offsetx, double offsety, int nDataIndex, int nPlotIndex=0);
	// 获得曲线的水平偏移
	double		GetDataOffsetX(int nDataIndex, int nPlotIndex=0);
	// 设置曲线的水平偏移
	void		SetDataOffsetX(double offset, int nDataIndex, int nPlotIndex=0);
	// 获得曲线的竖直偏移
	double		GetDataOffsetY(int nDataIndex, int nPlotIndex=0);
	// 设置曲线的竖直偏移
	void		SetDataOffsetY(double offset, int nDataIndex, int nPlotIndex=0);
	// 设置X方向单位，仅仅用于模拟示波器
	void		SetXUnit(TCHAR *unit, int nDataIndex);
	// 设置Y方向单位，仅仅用于模拟示波器
	void		SetYUnit(TCHAR *unit, int nDataIndex);
	// 在图例中仅仅显示Y方向的比例尺，仅仅用于模拟示波器
	void		SetShowYSCaleOnly(bool only);
	// 检测光标绑定到鼠标
	bool		IsHoldCursorToMouse(int nPlotIndex=0);
	// 设置光标绑定到鼠标
	void		SetHoldCursorToMouse(bool hold, int nPlotIndex=0);
	// 检测光标绑定到曲线
	bool		IsHoldCursorToCurve(int nPlotIndex=0);
	// 设置光标绑定到曲线
	void		SetHoldCursorToCurve(bool hold, int nPlotIndex=0);
	// 获得光标绑定的曲线序号
	int			GetIndexToHoldCursor(int nPlotIndex=0);
	// 设置光标绑定的曲线序号
	void		SetIndexToHoldCursor(int nCurveIndex, int nPlotIndex=0);
	// 检测零点箭头绘制
	bool		IsShowZeroArrow(int nPlotIndex=0);
	// 设置零点箭头绘制
	void		SetShowZeroArrow(bool show, int nPlotIndex=0);
	// 检测触发箭头绘制
	bool		IsShowTrigArrow(int nPlotIndex=0);
	// 设置触发箭头绘制
	void		SetShowTrigArrow(bool show, int nPlotIndex=0);
	// 显示水平光标线，仅仅用于模拟示波器
	void		SetShowHCursor(bool show, int nPlotIndex=0);
	// 显示竖直光标线，仅仅用于模拟示波器
	void		SetShowVCursor(bool show, int nPlotIndex=0);
	// 获得光标线颜色，仅仅用于模拟示波器
	COLORREF	GetCursorColor(int nPlotIndex=0);
	// 设置光标线颜色，仅仅用于模拟示波器
	void		SetCursorColor(COLORREF color, int nPlotIndex=0);
	// 获得光标线线宽，仅仅用于模拟示波器
	int			GetCursorSize(int nPlotIndex=0);
	// 设置光标线线宽，仅仅用于模拟示波器
	void		SetCursorSize(int size, int nPlotIndex=0);
	// 获得光标线线型，仅仅用于模拟示波器
	int			GetCursorStyle(int nPlotIndex=0);
	// 设置光标线线型，仅仅用于模拟示波器
	void		SetCursorStyle(int style, int nPlotIndex=0);
	// 获得水平光标值，仅仅用于模拟示波器
	double		GetCursorX(int nPlotIndex=0);
	// 设置水平光标值，仅仅用于模拟示波器
	void		SetCursorX( double cursor, int nPlotIndex=0);
	// 获得竖直光标值，仅仅用于模拟示波器
	double		GetCursorY(int nPlotIndex=0);
	// 设置竖直光标值，仅仅用于模拟示波器
	void		SetCursorY( double cursor, int nPlotIndex=0);
	// 显示水平选择区，仅仅用于模拟示波器
	void		SetShowHSel(bool show, int nPlotIndex=0);
	// 显示竖直选择区，仅仅用于模拟示波器
	void		SetShowVSel(bool show, int nPlotIndex=0);
	// 获得水平选择区的下限，仅仅用于模拟示波器
	double		GetHSelLower(int nPlotIndex=0);
	// 设置水平选择区的下限，仅仅用于模拟示波器
	void		SetHSelLower(int val, int nPlotIndex=0);
	// 获得水平选择区的上限，仅仅用于模拟示波器
	double		GetHSelUpper(int nPlotIndex=0);
	// 设置水平选择区的上限，仅仅用于模拟示波器
	void		SetHSelUpper(int val, int nPlotIndex=0);
	// 获得竖直选择区的下限，仅仅用于模拟示波器
	double		GetVSelLower(int nPlotIndex=0);
	// 设置竖直选择区的下限，仅仅用于模拟示波器
	void		SetVSelLower(int val, int nPlotIndex=0);
	// 获得竖直选择区的上限，仅仅用于模拟示波器
	double		GetVSelUpper(int nPlotIndex=0);
	// 设置竖直选择区的上限，仅仅用于模拟示波器
	void		SetVSelUpper(int val, int nPlotIndex=0);
	// 获得水平选择区的宽度，仅仅用于模拟示波器
	double		GetHSelWidth(int nPlotIndex=0);
	// 获得竖直选择区的宽度，仅仅用于模拟示波器
	double		GetVSelWidth(int nPlotIndex=0);


public:
	// 获得图像标题
	TCHAR*		GetTitle();
	// 获得子图标题，可用于分裂视图、分层视图、共享X轴视图
	TCHAR*		GetTitle(int nPlotIndex);
	// 设置图像标题
	void		SetTitle(const TCHAR* title);
	// 设置子图标题
	void		SetTitle(const TCHAR* title, int nPlotIndex);
	// 获得图像标题位置
	int			GetTitlePosition();
	// 获得子图标题位置
	int			GetTitlePosition(int nPlotIndex);
	// 设置图像标题位置。负数：左；0：中；正数：右
	void		SetTitlePosition(int position);
	// 设置子图标题位置。负数：左；0：中；正数：右
	void		SetTitlePosition(int position, int nPlotIndex);
	// 图像标题是否显示
	bool		IsTitleShow();
	// 子图标题是否显示
	bool		IsTitleShow(int nPlotIndex);
	// 设置显示图像标题
	void		SetTitleShow(bool show);
	// 设置显示子图标题
	void		SetTitleShow(bool show, int nPlotIndex);
	// 获得图像标题颜色
	COLORREF	GetTitleColor();
	// 获得子图标题颜色
	COLORREF	GetTitleColor(int nPlotIndex);
	// 设置图像标题颜色
	void		SetTitleColor(COLORREF color);
	// 设置子图标题颜色
	void		SetTitleColor(COLORREF color, int nPlotIndex);
	// 获得标题字体
	LOGFONT		GetTitleFont();
	// 获得子图标题字体
	LOGFONT		GetTitleFont(int nPlotIndex);
	// 设置标题字体
	void		SetTitleFont(LOGFONT logFont);
	// 设置子图标题字体
	void		SetTitleFont(LOGFONT logFont, int nPlotIndex);
	// 分层视图显示主标题
	void		SetShowParentTitle(bool set);

	// 获得图像副标题
	TCHAR*		GetSubTitle();
	// 获得子图副标题，可用于分裂视图、分层视图、共享X轴视图
	TCHAR*		GetSubTitle(int nPlotIndex);
	// 设置图像副标题
	void		SetSubTitle(const TCHAR* title);
	// 设置子图副标题
	void		SetSubTitle(const TCHAR* title, int nPlotIndex);
	// 获得图像副标题位置
	int			GetSubTitlePosition();
	// 获得子图副标题位置
	int			GetSubTitlePosition(int nPlotIndex);
	// 设置图像副标题位置。负数：左；0：中；正数：右
	void		SetSubTitlePosition(int position);
	// 设置子图副标题位置。负数：左；0：中；正数：右
	void		SetSubTitlePosition(int position, int nPlotIndex);
	// 图像副标题是否显示
	bool		IsSubTitleShow();
	// 子图副标题是否显示
	bool		IsSubTitleShow(int nPlotIndex);
	// 设置显示图像副标题
	void		SetSubTitleShow(bool show);
	// 设置显示子图副标题
	void		SetSubTitleShow(bool show, int nPlotIndex);
	// 获得图像副标题颜色
	COLORREF	GetSubTitleColor();
	// 获得子图副标题颜色
	COLORREF	GetSubTitleColor(int nPlotIndex);
	// 设置图像副标题颜色
	void		SetSubTitleColor(COLORREF color);
	// 设置子图副标题颜色
	void		SetSubTitleColor(COLORREF color, int nPlotIndex);
	// 获得副标题字体
	LOGFONT		GetSubTitleFont();
	// 获得子图副标题字体
	LOGFONT		GetSubTitleFont(int nPlotIndex);
	// 设置副标题字体
	void		SetSubTitleFont(LOGFONT logFont);
	// 设置子图副标题字体
	void		SetSubTitleFont(LOGFONT logFont, int nPlotIndex);
	
	// 设置高亮边框颜色
	void		SetLightColor(COLORREF color);
	// 设置子图高亮边框颜色
	void		SetLightColor(COLORREF color, int nPlotIndex);
	// 设置高亮边框线宽
	void		SetLightLineSize(int nSize);
	// 设置子图高亮边框线宽
	void		SetLightLineSize(int nSize, int nPlotIndex);
	// 设置高亮边框线型
	void		SetLightLineStyle(int nStyle);
	// 设置子图高亮边框线型
	void		SetLightLineStyle(int nStyle, int nPlotIndex);

private:
	// 内部调用函数
	void		*GetAxis(int location);
	void		*GetAxis(int location, int nPlotIndex);

public:
	// 增加坐标轴，坐标轴的位置location见本文件头部被注释掉的enum
	void		AddAxis(int location, int nPlotIndex=0);
	// 设置子图坐标轴标题
	void		SetAxisTitle(const TCHAR* title, int location, int nPlotIndex);
	// 设置坐标轴标题
	void		SetAxisTitle(const TCHAR* title, int location);
	// 设置子图坐标轴标题字体
	void		SetAxisTitleFont(LOGFONT logFont, int location, int nPlotIndex);
	// 设置坐标轴标题字体
	void		SetAxisTitleFont(LOGFONT logFont, int location);
	// 设置子图坐标轴标签字体
	void		SetAxisLabelFont(LOGFONT logFont, int location, int nPlotIndex);
	// 设置坐标轴标签字体
	void		SetAxisLabelFont(LOGFONT logFont, int location);
	// 设置子图坐标轴标题位置
	void		SetAxisTitlePosition(int position, int location, int nPlotIndex);
	// 设置坐标轴标题位置
	void		SetAxisTitlePosition(int position, int location);
	// 设置子图坐标轴与绘图区的间隙
	void		SetAxisGap(int gap, int location, int nPlotIndex);
	// 设置坐标轴与绘图区的间隙
	void		SetAxisGap(int gap, int location);
	// 设置子图坐标轴刻度线位置
	void		SetAxisTickPosition(int pos, int location, int nPlotIndex);
	// 设置坐标轴刻度线位置
	void		SetAxisTickPosition(int pos, int location);
	// 设置子图坐标轴副刻度线位置
	void		SetAxisMinorTickPosition(int pos, int location, int nPlotIndex);
	// 设置坐标轴副刻度线位置
	void		SetAxisMinorTickPosition(int pos, int location);
	// 设置子图时间坐标轴
	void		SetAxisToTime(bool bTime, int location, int nPlotIndex);
	// 设置时间坐标轴
	void		SetAxisToTime(bool bTime, int location);
	// 设置子图时间坐标轴格式,"%Y%m%d%H%M%S",参考strftime函数或CTime类的格式化方法
	void		SetAxisTimeFormat(TCHAR *format, int location, int nPlotIndex);
	// 设置时间坐标轴格式,"%Y%m%d%H%M%S",参考strftime函数或CTime类的格式化方法
	void		SetAxisTimeFormat(TCHAR *format, int location);
	// 设置子图显示颜色棒
	void		SetAxisColorBar(bool bShow, int location, int nPlotIndex);
	// 设置显示颜色棒
	void		SetAxisColorBar(bool bShow, int location);
	// 设置子图坐标轴颜色
	void		SetAxisColor(COLORREF color, int location, int nPlotIndex);
	// 设置坐标轴颜色
	void		SetAxisColor(COLORREF color, int location);
	// 设置坐标轴是否显示刻度值
	void		SetAxisLabelShow(bool bShow, int location);
	// 设置子图坐标轴是否显示刻度值
	void		SetAxisLabelShow(bool bShow, int location, int nPlotIndex);
	// 设置子图坐标值的格式,参考printf的格式化方法
	void		SetAxisLabelFormat(TCHAR *format, int location, int nPlotIndex);
	// 设置坐标值的格式,参考printf的格式化方法
	void		SetAxisLabelFormat(TCHAR *format, int location);
	// 设置子图坐标轴是否显示
	void		SetAxisShow(bool bShow, int location, int nPlotIndex);
	// 设置坐标轴是否显示
	void		SetAxisShow(bool bShow, int location);
	// 子图坐标轴是否显示
	bool		IsAxisShow(int location, int nPlotIndex);
	//坐标轴是否显示
	bool		IsAxisShow(int location);
	// 设置坐标轴紧凑模式
	void		SetAxesCompact(int nPlotIndex=0);
	// 添加子图隐藏坐标轴标签的一个数据范围
	void		AddRange4LabelHiden(double low, double high, int location, int nPlotIndex);
	// 添加隐藏坐标轴标签的一个数据范围
	void		AddRange4LabelHiden(double low, double high, int location);
	// 设置坐标轴标签替换
	void		SetReplaceLabel(bool bReplace, int location);
	// 设置子图坐标轴标签替换
	void		SetReplaceLabel(bool bReplace, int location, int nPlotIndex);
	// 添加一个替换的标签
	void		AddAlternateLabel(TCHAR *label, int location);
	// 子图添加一个替换的标签
	void		AddAlternateLabel(TCHAR *label, int location, int nPlotIndex);
	// 清空坐标轴替换标签
	void		ClrAlternateLabel(int location);
	// 清空子图坐标轴替换标签
	void		ClrAlternateLabel(int location, int nPlotIndex);

public:
	// 子图自动设置X坐标轴刻度数
	void		SetXAutoTicks(bool bAuto, int nPlotIndex);
	// 自动设置X坐标轴刻度数
	void		SetXAutoTicks(bool bAuto);
	// 子图自动设置Y坐标轴刻度数
	void		SetYAutoTicks(bool bAuto, int nPlotIndex);
	// 自动设置Y坐标轴刻度数
	void		SetYAutoTicks(bool bAuto);
	// 设置子图X坐标轴刻度数
	void		SetXTickCount(int count, int nPlotIndex);
	// 设置坐标X轴刻度数
	void		SetXTickCount(int count);
	// 设置子图Y坐标轴刻度数
	void		SetYTickCount(int count, int nPlotIndex);
	// 设置坐标Y轴刻度数
	void		SetYTickCount(int count);
	// 子图自动设置X坐标轴副刻度数
	void		SetXAutoMinorTicks(bool bAuto, int nPlotIndex);
	// 自动设置X坐标轴副刻度数
	void		SetXAutoMinorTicks(bool bAuto);
	// 子图自动设置Y坐标轴副刻度数
	void		SetYAutoMinorTicks(bool bAuto, int nPlotIndex);
	// 自动设置Y坐标轴副刻度数
	void		SetYAutoMinorTicks(bool bAuto);
	// 设置子图X坐标轴副刻度数
	void		SetXMinorTickCount(int count, int nPlotIndex);
	// 设置X坐标轴副刻度数
	void		SetXMinorTickCount(int count);
	// 设置子图Y坐标轴副刻度数
	void		SetYMinorTickCount(int count, int nPlotIndex);
	// 设置Y坐标轴副刻度数
	void		SetYMinorTickCount(int count);
	// 设置子图X坐标轴浮动标记
	void		SetXFloatTicks(bool flt, int nPlotIndex);
	// 设置X坐标轴浮动标记
	void		SetXFloatTicks(bool flt);
	// 设置子图Y坐标轴浮动标记
	void		SetYFloatTicks(bool flt, int nPlotIndex);
	// 设置Y坐标轴浮动标记
	void		SetYFloatTicks(bool flt);
	// 设置子图X坐标轴成指数
	void		SetXLogarithm(bool bLog, int nPlotIndex);
	// 设置X坐标轴成指数
	void		SetXLogarithm(bool bLog);
	// 设置子图Y坐标轴成指数
	void		SetYLogarithm(bool bLog, int nPlotIndex);
	// 设置Y坐标轴成指数
	void		SetYLogarithm(bool bLog);
	// 设置子图X坐标反向绘制
	void		SetXOpposite(bool bOpposite, int nPlotIndex);
	// 设置X坐标反向绘制
	void		SetXOpposite(bool bOpposite);
	// 设置子图Y坐标反向绘制
	void		SetYOpposite(bool bOpposite, int nPlotIndex);
	// 设置Y坐标反向绘制
	void		SetYOpposite(bool bOpposite);
	// 设置子图坐标横轴间隔是某数的倍数
	void		SetAtomX(double atom, int nPlotIndex);
	// 设置坐标横轴间隔是某数的倍数
	void		SetAtomX(double atom);
	// 设置子图坐标纵轴间隔是某数的倍数
	void		SetAtomY(double atom, int nPlotIndex);
	// 设置坐标纵轴间隔是某数的倍数
	void		SetAtomY(double atom);
	// 设置子图坐标横轴起点是某数的倍数
	void		SetAtomZeroX(double zero, int nPlotIndex);
	// 设置坐标横轴起点是某数的倍数
	void		SetAtomZeroX(double zero);
	// 设置子图坐标纵轴起点是某数的倍数
	void		SetAtomZeroY(double zero, int nPlotIndex);
	// 设置坐标纵轴起点是某数的倍数
	void		SetAtomZeroY(double zero);
	// 在设置坐标范围是某数倍数情况下，子图坐标横轴分割份数
	void		SetSegsPreferX(int segs, int nPlotIndex);
	// 在设置坐标范围是某数倍数情况下，坐标横轴分割份数
	void		SetSegsPreferX(int segs);
	// 在设置坐标范围是某数倍数情况下，子图坐标纵轴分割份数
	void		SetSegsPreferY(int segs, int nPlotIndex);
	// 在设置坐标范围是某数倍数情况下，坐标纵轴分割份数
	void		SetSegsPreferY(int segs);

public:
	// 设置坐标轴范围
	void		SetPlotRange(double xRange[2], double yRange[2]);
	void		SetPlotRange(double xl, double xu, double yl, double yu);
	void		SetXRange(double low, double high, int nPlotIndex);
	void		SetXRange(double low, double high);
	void		SetYRange(double low, double high, int nPlotIndex);
	void		SetYRange(double low, double high);
	// 设置坐标轴范围自动计算
	void		SetXAutoRange(bool bAuto);
	void		SetYAutoRange(bool bAuto);
	void		SetXAutoRange(bool bAuto, int nPlotIndex);
	void		SetYAutoRange(bool bAuto, int nPlotIndex);
	// 设置坐标轴范围与数据范围相同，不向两端扩展
	void		SetExactXRange(bool bExact, int nPlotIndex=0);
	void		SetExactYRange(bool bExact, int nPlotIndex=0);
	// 设置坐标轴范围尽量与数据范围相同
	void		SetOptimalExactXRange(bool bExact, int nPlotIndex=0);
	void		SetOptimalExactYRange(bool bExact, int nPlotIndex=0);
	// 设置坐标轴范围尽量向两端扩展一格
	void		SetOptimalXExtend(bool bExtend, int nPlotIndex=0);
	void		SetOptimalYExtend(bool bExtend, int nPlotIndex=0);
	// 设置坐标轴范围一致不变
	void		SetStaticRange(bool sr, int nPlotIndex=0);
	// 设置X坐标轴范围一致不变
	void		SetStaticXRange(bool sr, int nPlotIndex=0);
	// 设置Y坐标轴范围一致不变
	void		SetStaticYRange(bool sr, int nPlotIndex=0);
	
	// 设置坐标轴等长，只对kTypeXY有效
	void		SetEqualXYAxis(bool bEqual);
	// 设置坐标轴等范围，只对kTypeXY有效
	void		SetEqualXYRange(bool bEqual);
	// 设置坐标轴等比例尺，只对kTypeXY有效
	void		SetEqualXYScale(bool bEqual);
	
	// X坐标超出屏幕的点不参与计算，以优化速度
	void		SetClipXData(bool clip, int nDataIndex, int nPlotIndex = 0);
	// Y坐标超出屏幕的点不参与计算，以优化速度
	void		SetClipYData(bool clip, int nDataIndex, int nPlotIndex = 0);

	// 设置X时间轴后，对话框中X数据范围设置时减去一个初值，以避免数据过大
	void		SetXStartTime(TCHAR *strTime, TCHAR *fmt, int nPlotIndex = 0);
	// 设置Y时间轴后，对话框中Y数据范围设置时减去一个初值，以避免数据过大
	void		SetYStartTime(TCHAR *strTime, TCHAR *fmt, int nPlotIndex = 0);

public:
	// 设置背景网格与哪一个子图绑定，用于共享X轴视图
	void		SetGridBindLayer(int nPlotIndex);
	// 检测子图的网格线
	bool		GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV, int nPlotIndex);
	// 检测网格线
	bool		GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV);
	// 设置子图网格线的绘制
	void		SetGridLine(bool MajorH, bool MajorV, bool MinorH, bool MinorV, int nPlotIndex);
	// 设置网格线的绘制
	void		SetGridLine(bool MajorH=true, bool MajorV=true, bool MinorH=false, bool MinorV=false);
	// 设置主网格线颜色
	void		SetMajorGridColor(COLORREF color);
	// 设置子图主网格线颜色
	void		SetMajorGridColor(COLORREF color, int nPlotIndex);
	// 设置副网格线颜色
	void		SetMinorGridColor(COLORREF color);
	// 设置子图副网格线颜色
	void		SetMinorGridColor(COLORREF color, int nPlotIndex);
	// 设置主网格线线宽
	void		SetMajorGridLineSize(int nSize);
	// 设置子图主网格线线宽
	void		SetMajorGridLineSize(int nSize, int nPlotIndex);
	// 设置副网格线线宽
	void		SetMinorGridLineSize(int nSize);
	// 设置子图副网格线线宽
	void		SetMinorGridLineSize(int nSize, int nPlotIndex);
	// 设置主网格线线型
	void		SetMajorGridLineStyle(int nStyle);
	// 设置子图主网格线线型
	void		SetMajorGridLineStyle(int nStyle, int nPlotIndex);
	// 设置副网格线线型
	void		SetMinorGridLineStyle(int nStyle);
	// 设置子图副网格线线型
	void		SetMinorGridLineStyle(int nStyle, int nPlotIndex);
	// 获取子图背景颜色
	COLORREF	GetBkgndColor(int nPlotIndex);
	// 获取背景颜色
	COLORREF	GetBkgndColor();
	// 设置子图背景颜色
	void		SetBkgndColor(COLORREF color, int nPlotIndex);
	// 设置背景颜色
	void		SetBkgndColor(COLORREF color);
	// 背景色是否渐变
	bool		IsGradientBkgnd();
	// 背景色渐变
	void		SetGradientBkgnd(bool bGrad);

public:
	// 测试图像边框是否显示
	bool		IsEdgeShow();
	// 设置图像边框显示
	void		SetEdgeShow(bool bShow);
	// 设置图像边框显示
	void		SetEdgeRoundConor(bool bRound);
	// 设置图像边框颜色
	void		SetEdgeColor(COLORREF color);
	// 设置图像边框宽度
	void		SetEdgeWidth(int width);

	// 设置图像左空白
	void		SetMarginLeft(int margin);
	// 设置分裂视图左空白
	void		SetMarginLeft(int margin, int nPlotIndex);
	// 设置图像右空白
	void		SetMarginRight(int margin);
	// 设置分裂视图右空白
	void		SetMarginRight(int margin, int nPlotIndex);
	// 设置图像上空白
	void		SetMarginTop(int margin);
	// 设置分裂视图上空白
	void		SetMarginTop(int margin, int nPlotIndex);
	// 设置图像下空白
	void		SetMarginBottom(int margin);
	// 设置分裂视图下空白
	void		SetMarginBottom(int margin, int nPlotIndex);
	
	// 设置数据区边框显示
	void		SetBorderShow(bool bShow);
	// 设置数据区边框颜色
	void		SetBorderColor(COLORREF color);
	// 设置数据区边框宽度
	void		SetBorderWidth(int width);

public:
	// 检测是否使用图例
	bool		IsLegendShow(int nPlotIndex=0);
	// 设置是否使用图例
	void		SetLegendShow(bool bUse, int nPlotIndex=0);
	// 设置图例的最大行数
	void		SetLegendMaxRows(int nRows, int nPlotIndex=0);
	// 设置每行显示的图例数
	void		SetLegendMaxCols(int nCols, int nPlotIndex=0);
	// 设置图例位置
	void		SetLegendPosition(int nPos, int nPlotIndex=0);
	// 设置图例字体
	void		SetLegendFont(LOGFONT logFont, int nPlotIndex=0);
	// 设置图例字体高度
	void		SetLegendFontHeight(int nHeight, int nPlotIndex=0);

public:
	// 在分层视图和分裂视图中，设置第一层显示左坐标轴，第二层显示右坐标轴
	void		SetLRAxis();
	// 在分层视图中，设置第一层显示下坐标轴，第二层显示上坐标轴
	void		SetTBAxis();

public:
	// 底层函数，获取子图的绘图区域
	RECT		GetLastClientRect(int nPlotIndex);
	// 底层函数，获取绘图区域
	RECT		GetLastClientRect();
	// 底层函数，获取子图的数据区域
	RECT		GetLastPlotRect(int nPlotIndex);
	// 底层函数，获取数据区域
	RECT		GetLastPlotRect();
	// 底层函数，获取坐标轴的范围
	void		GetLastPlotRange(double xRange[2], double yRange[2], int nPlotIndex=0);
	// 底层函数，获取数据的范围
	void		GetLastDataRange(double xRange[2], double yRange[2], int nPlotIndex=0);
	// 底层函数，根据屏幕坐标计算数据坐标
	void		ClientToData(POINT *point, double *data, int nPlotIndex=0);
	// 底层函数，根据数据坐标计算屏幕坐标
	void		DataToClient(double *data, POINT *point, int nPlotIndex=0);

public:
	// 设置三维曲线的网格
	void		SetGridLine(bool MajorXY, bool MinorXY, bool MajorYZ, bool MinorYZ, bool MajorZX, bool MinorZX);
	// 设置XY屏幕主网格
	void		SetShowXYMajorGrid( bool show );
	// 设置YZ屏幕主网格
	void		SetShowYZMajorGrid( bool show );
	// 设置ZX屏幕主网格
	void		SetShowZXMajorGrid( bool show );
	// 设置XY屏幕副网格
	void		SetShowXYMinorGrid( bool show );
	// 设置YZ屏幕副网格
	void		SetShowYZMinorGrid( bool show );
	// 设置ZX屏幕副网格
	void		SetShowZXMinorGrid( bool show );

public:
	// 保存子图数据到文件
	bool		WriteToFile(TCHAR *pathName, int nPlotIndex=0);
	// 保存子图某曲线数据到文件
	bool		WriteOneToFile(TCHAR *pathName, int nDataIndex, int nPlotIndex=0);

public:
	// 获得内部特殊指针，此指针一般没用，只用于在某些情况下传递参数，例如线程函数
	void		*GetUserPointer();
	// 设置内部特殊指针
	void		SetUserPointer(void *pUser);
	// 获得内部特殊标志状态，特殊用途
	bool		GetUserFlag();
	// 设置内部特殊标志状态，特殊用途
	void		SetUserFlag(bool flag);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Declaration of class CChartWnd

// 窗口类，包装了消息处理函数
class	CChart_API	CChartWnd
{
public:
	// CChartWnd内部所用的数据结构
	struct	stChartWndPara;
protected:
	// 内部数据
	stChartWndPara *m_pPara;
	// 释放内部资源
	void	Release();
	
public:
	CChartWnd();
	~CChartWnd();
	
public:
	// 根据标识获得序号
	int		GetChartIndex(int nChartID);
	// 根据序号获得标识
	int		GetChartID(int nChartIndex);
	// 根据序号获取CChart类的指针，以调用其函数
	CChart	*GetChart(int nChartIndex=0);
	// 设置CChart类对象，主要用于子类化，返回值为原CChart类指针
	CChart	*SetChart(CChart *pChart, int nChartIndex=0);
	// 获得视图类型
	int		GetChartType(int nChartIndex=0);
	// 改变视图类型
	bool	SetChartType(int nType, int nChartIndex=0);
	// 获得已Attach的次数
	int		GetChartCount();
	
	// 粘窗口，最基本形式，默认折线图
	// 返回值是CChart对象的唯一标识符，-1表示失败。下同
	int		Attach(HWND hWnd);
	// 粘窗口，并限制绘图区，默认折线图
	int		Attach(HWND hWnd, RECT rtClient);
	// 粘窗口，粘到对话框上, 给定控件句柄，默认折线图
	int		AttachDlg(HWND hDlg, HWND hCtrl);
	// 粘窗口，粘到对话框上，给定控件ID，默认折线图
	int		AttachDlg(HWND hDlg, UINT nCtrlID);

	// 粘窗口，同时设置绘图类型
	int		Attach(HWND hWnd, int nType);
	// 粘窗口，同时设置绘图类型，并限制绘图区
	int		Attach(HWND hWnd, RECT rtClient, int nType);
	// 粘窗口，粘到对话框上, 给定控件句柄
	int		AttachDlg(HWND hDlg, HWND hCtrl, int nType);
	// 粘窗口，粘到对话框上，给定控件ID
	int		AttachDlg(HWND hDlg, UINT nCtrlID, int nType);

	// 卸窗口, 所有函数的nChartIndex，均按照成功Attach的顺序依次为0, 1, 2, ...
	bool	Detach();
	bool	Detach(HWND hWnd);
	
	// 重绘
	void	ReDraw();
	void	ReDraw(HWND hWnd);
	void	ReDraw(int nChartIndex);
	void	ReDraw(CChart *chart);
		
};

///////////////////////////////////////////////////////////////////////////////////////////
// CChart Ctrl
// A Standard windows control, whose class name is "ChartCtrl"
// 此注册函数一般不需要调用，因为动态库里面已经调用了
CChart_API	ATOM	RegisterChartControl();

// 请确认hWnd是一个ChartCtrl窗口
CChart_API	CChart	*GetChart(HWND hWnd);

// 创建一个弹出窗口
CChart_API	HWND	CreatePopupChartWnd(HWND hWndParent, int nChartType, TCHAR *wndTitle=_TEXT(""), int x=0, int y=0, int cx=CW_USEDEFAULT, int cy=CW_USEDEFAULT);

// 一个消息循环，用于控制台程序中和CreatePopupChartWnd配合使用。注意这个函数一般不会返回，一般放在代码的最后面
CChart_API	int		SimpleMsgLoop();

// 创建一个包含消息循环的弹出窗口，相当于以上两个函数打包
CChart_API	HWND	CreatePopupChartWndWithMsgLoop(HWND hWndParent, int nChartType, TCHAR *wndTitle=_TEXT(""), int x=0, int y=0, int cx=CW_USEDEFAULT, int cy=CW_USEDEFAULT);

// 创建一个子窗口
CChart_API	HWND	CreateSubChartWnd(HWND hWndParent, int nChartType, TCHAR *wndTitle=_TEXT(""), int x=0, int y=0, int cx=CW_USEDEFAULT, int cy=CW_USEDEFAULT);

///////////////////////////////////////////////////////////////////////////////////////////
// 实用函数

// 用于将时间串字符转换为time_t。
CChart_API	double StringToTime(TCHAR *str, TCHAR *fmt);


///////////////////////////////////////////////////////////////////////////////////////////
// aliases
typedef	CChart		MyChart;
typedef	CChartWnd	MyChartWnd; 


///////////////////////////////////////////////////////////////////////////////////////////
// namespace ended
}

//#ifdef CChart_API
//#	undef CChart_API
//#endif //CChart_API

///////////////////////////////////////////////////////////////////////////////////////////
// file ended


