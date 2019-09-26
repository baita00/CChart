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
#include <time.h>

namespace NsCChart{
	
//用于动态链接库的时候，注释掉#else中的第二行
//用于静态库或者源码时，注释掉#else中的第一行
#ifdef CKLINECHART_EXPORTS
#define CKLINECHART_API __declspec(dllexport)
#else
//#define CKLINECHART_API __declspec(dllimport)
#define CKLINECHART_API
#endif

class	CKLINECHART_API	CTemplateChart
{
public:	
	// CTemplateChart内部数据结构
	struct stTemplateChartPara;
	// 获得内部数据的指针
	stTemplateChartPara	*GetPara();
protected:
	// 内部数据
	stTemplateChartPara	*m_pPara;
	// 释放内部资源
	void		Release();	
	
public:
	CTemplateChart();
	~CTemplateChart();


	// 获得标识符，架构所需
	int			GetPlotID();
	// 设置标识符，架构所需
	void		SetPlotID(int nID);
public:
	// 获得预置窗口
	HWND		GetWnd();
	// 设置预置窗口
	void		SetWnd(HWND hWnd);
	// 测试是否限制绘图区
	bool		IsRectConfined();
	// 设置是否限制绘图区
	void		SetRectConfined(bool confine);
	// 获取限制区域
	RECT		GetConfineRect(); 
	// 设置限制区域
	void		SetConfineRect(RECT ConfineRect);
	
	// 测试内部是否无数据
	bool		IsEmpty();

public:
	// 对SetWnd()设置的窗口句柄绘图
	void		OnDraw();
	// 对窗口句柄绘图
	void		OnDraw(HWND hWnd);
	// 对设备上下文绘图
	void		OnDraw(HDC hDC);
	// 在窗口上选择区域绘图
	void		OnDraw(HWND hWnd, RECT destRect);
	// 在设备上下文对应的窗口上，选择区域绘图
	void		OnDraw(HDC hDC, RECT destRect);

public:
	// 测试是否响应鼠标键盘消息
	bool		IsReact();
	// 设置是否响应鼠标键盘消息
	void		SetReact(bool react);
	// 单个的鼠标键盘消息，但不重绘，由用户根据返回值重绘
	bool		OnLButtonDown( HWND hWnd, POINT point, UINT ctrlKey );
	bool		OnLButtonUp( HWND hWnd, POINT point, UINT ctrlKey );
	bool		OnLButtonDblClk( HWND hWnd, POINT point, UINT ctrlKey );
	bool		OnMouseMove( HWND hWnd, POINT point, UINT ctrlKey );
	bool		OnMouseLeave( HWND hWnd, POINT point, UINT ctrlKey );
	bool		OnContextMenu( HMENU hMenu, HWND hWnd, POINT point );
	bool		OnKeyDown( HWND hWnd, UINT key );
	// 响应消息，但不重绘，由用户根据返回值重绘
	bool		OnEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	// 响应消息，在AutoRedraw为真情况下重绘
	void		Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:

public:
	// 设置标题
	void		SetTitle(TCHAR* title);
	// 设置标题是否显示
	void		SetTitleShow(bool show);
	// 设置标题颜色
	void		SetTitleColor(COLORREF color);
	// 设置标题字体
	void		SetTitleFont(LOGFONT logFont);
	// 设置标题字体高度
	void		SetTitleHeight(int height);
	// 设置标题字体名称
	void		SetTitleFaceName(TCHAR* facename);

public:
	// 获得背景颜色
	COLORREF	GetBkgndColor();
	// 设置背景颜色
	void		SetBkgndColor(COLORREF color);
	// 设置是否绘制边缘
	void		SetEdgeShow(bool show);
};

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Declaration of class CTemplateChartWnd

// 窗口类，包装了消息处理函数
class	CKLINECHART_API	CTemplateChartWnd
{
public:
	// CTemplateChartWnd内部所用的数据结构
	struct	stTemplateChartWndPara;
protected:
	// 内部数据
	stTemplateChartWndPara *m_pPara;
	// 释放内部资源
	void	Release();
	
public:
	CTemplateChartWnd();
	~CTemplateChartWnd();
	
public:
	// 根据标识获得序号
	int		GetChartIndex(int nChartID);
	// 根据序号获得标识
	int		GetChartID(int nChartIndex);
	// 获取CTemplateChart类的指针，以调用其函数
	CTemplateChart	*GetChart(int nChartIndex=0);
	// 设置CChart类对象，主要用于子类化，返回值为原CChart类指针
	CTemplateChart	*SetChart(CTemplateChart *pChart, int nChartIndex=0);
	// 获得已Attach的次数
	int		GetChartCount();
	
	// 粘窗口，最基本形式
	int		Attach(HWND hWnd);
	// 粘窗口，同时设置绘图类型，并限制绘图区
	int		Attach(HWND hWnd, RECT rtClient);
	// 粘窗口，粘到对话框上, 给定控件句柄
	int		AttachDlg(HWND hDlg, HWND hCtrl);
	// 粘窗口，粘到对话框上，给定控件ID
	int		AttachDlg(HWND hDlg, UINT nCtrlID);
	// 卸窗口, 所有函数的nChartIndex，均按照成功Attach的顺序依次为0, 1, 2, ...
	bool	Detach();
	bool	Detach(HWND hWnd);
	
	// 重绘
	void	ReDraw();
	void	ReDraw(HWND hWnd);
	void	ReDraw(int nChartIndex);
	void	ReDraw(CTemplateChart *chart);		
};


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
// namespace ending
}