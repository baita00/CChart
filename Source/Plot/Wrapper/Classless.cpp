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

#include "Chart.h"
#include "Classless.h"//the dll cannot export functions without this line

#include "../Basic/Handler.h"
#include "../Basic/Plot.h"
#include "../Extended/ContourLinePlot/ContourBasicPlot.h"

#define DeclareFunc_0P_Ret(TP, FN) \
	TP FN( )\
	{\
		return pGlobalChartWnd->GetChart()->FN();\
	}

#define DeclareFunc_0P_Noret(TP, FN) \
	TP FN( )\
	{\
		pGlobalChartWnd->GetChart()->FN();\
	}

#define DeclareFunc_1P_Ret(TP, FN, T1, P1) \
	TP FN(T1 P1)\
	{\
		return pGlobalChartWnd->GetChart()->FN(P1);\
	}

#define DeclareFunc_1P_Noret(TP, FN, T1, P1) \
	TP FN(T1 P1)\
	{\
		pGlobalChartWnd->GetChart()->FN(P1);\
	}

#define DeclareFunc_2P_Ret(TP, FN, T1, P1, T2, P2) \
	TP FN(T1 P1, T2 P2)\
	{\
		return pGlobalChartWnd->GetChart()->FN(P1, P2);\
	}

#define DeclareFunc_2P_Noret(TP, FN, T1, P1, T2, P2) \
	TP FN(T1 P1, T2 P2)\
	{\
		pGlobalChartWnd->GetChart()->FN(P1, P2);\
	}

#define DeclareFunc_3P_Ret(TP, FN, T1, P1, T2, P2, T3, P3) \
	TP FN(T1 P1, T2 P2, T3 P3)\
	{\
		return pGlobalChartWnd->GetChart()->FN(P1, P2, P3);\
	}

#define DeclareFunc_3P_Noret(TP, FN, T1, P1, T2, P2, T3, P3) \
	TP FN(T1 P1, T2 P2, T3 P3)\
	{\
		pGlobalChartWnd->GetChart()->FN(P1, P2, P3);\
	}

#define DeclareFunc_4P_Ret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4) \
	TP FN(T1 P1, T2 P2, T3 P3, T4 P4)\
	{\
		return pGlobalChartWnd->GetChart()->FN(P1, P2, P3, P4);\
	}

#define DeclareFunc_4P_Noret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4) \
	TP FN(T1 P1, T2 P2, T3 P3, T4 P4)\
	{\
		pGlobalChartWnd->GetChart()->FN(P1, P2, P3, P4);\
	}

#define DeclareFunc_5P_Ret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5) \
	TP FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5)\
	{\
	return pGlobalChartWnd->GetChart()->FN(P1, P2, P3, P4, P5);\
	}

#define DeclareFunc_5P_Noret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5) \
	TP FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5)\
	{\
	pGlobalChartWnd->GetChart()->FN(P1, P2, P3, P4, P5);\
	}

#define DeclareFunc_6P_Ret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6) \
	TP FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6)\
	{\
	return pGlobalChartWnd->GetChart()->FN(P1, P2, P3, P4, P5, P6);\
	}

#define DeclareFunc_6P_Noret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6) \
	TP FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6)\
	{\
	pGlobalChartWnd->GetChart()->FN(P1, P2, P3, P4, P5, P6);\
	}

using namespace NsCChart; 

namespace Classless
{
	// 全局的CChartWnd变量
	static CChartWnd *pGlobalChartWnd = 0;
	static int nCreateCount = 0;

	void	CreateChart()
	{
		if(nCreateCount==0 && !pGlobalChartWnd)
		{
			pGlobalChartWnd = new CChartWnd;
		}
		nCreateCount++;
	}
	void	DestroyChart()
	{
		nCreateCount--;
		if(nCreateCount<=0 && pGlobalChartWnd)
		{
			delete pGlobalChartWnd;
			pGlobalChartWnd=0;
		}
	}
	bool	IsCreated()
	{
		return pGlobalChartWnd != 0;
	}
	// 创建一个弹出窗口
	HWND	CreatePopupChartWndGlobal(HWND hWndParent, int nChartType, TCHAR *wndTitle/*=_TEXT("")*/, int x/*=0*/, int y/*=0*/, int cx/*=CW_USEDEFAULT*/, int cy/*=CW_USEDEFAULT*/)
	{
		if(hWndParent)
		{
			POINT point;
			point.x = x;
			point.y = y;
			ClientToScreen(hWndParent, &point);
			x = point.x;
			y = point.y;
		}
		HWND hWnd = CreateWindow(_TEXT("button"), wndTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP,
			x, y, cx, cy, hWndParent, NULL, GetModuleHandle(NULL), NULL);
		if(hWnd)
		{
			if(!IsCreated())CreateChart();
			pGlobalChartWnd->Attach(hWnd, nChartType);
			pGlobalChartWnd->GetChart()->SetContinuePaint(false);
			pGlobalChartWnd->GetChart()->SetContinueReact(false);

			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);
		}
		return hWnd;
	}

	typedef struct stCreateWndParam
	{
		HWND hWndParent;
		int nChartType;
		TCHAR *wndTitle;
		int x, y, cx, cy;
		
		HWND hWndCreated;
	}CreateWndParam, *pCreateWndParam;
	
	HANDLE hEvent4CreateWnd = 0;
	
	DWORD WINAPI MyCreateWndThread(LPVOID pArg)
	{
		if(!pArg)return -1;
		
		CreateWndParam *pCwp = (CreateWndParam *)pArg;
		HWND hWnd = CreatePopupChartWndGlobal(pCwp->hWndParent, pCwp->nChartType, pCwp->wndTitle, pCwp->x, pCwp->y, pCwp->cx, pCwp->cy);
		pCwp->hWndCreated = hWnd;
		
		SetEvent(hEvent4CreateWnd);
		
		//The message loop    
		MSG msg;
		while(GetMessage(&msg,NULL,0,0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		return 0;
	}
	// 创建一个包含消息循环的弹出窗口
	HWND	CreatePopupChartWndGlobalWithMsgLoop(HWND hWndParent, int nChartType, TCHAR *wndTitle/*=_TEXT("")*/, int x/*=0*/, int y/*=0*/, int cx/*=CW_USEDEFAULT*/, int cy/*=CW_USEDEFAULT*/)
	{
		CreateWndParam *pCwp = new CreateWndParam;
		pCwp->hWndParent = hWndParent;
		pCwp->nChartType = nChartType;
		pCwp->wndTitle = wndTitle;
		pCwp->x = x;
		pCwp->y = y;
		pCwp->cx = cx;
		pCwp->cy = cy;
		
		hEvent4CreateWnd = CreateEvent(NULL, TRUE, FALSE, NULL);
		
		HANDLE hThread = CreateThread(NULL, 0, MyCreateWndThread, pCwp, 0, NULL);
		WaitForSingleObject(hEvent4CreateWnd, INFINITE);
		CloseHandle(hThread);
		CloseHandle(hEvent4CreateWnd);
		
		return pCwp->hWndCreated;
	}
	// 创建一个子窗口
	HWND	CreateSubChartWndGlobal(HWND hWndParent, int nChartType, TCHAR *wndTitle/*=_TEXT("")*/, int x/*=0*/, int y/*=0*/, int cx/*=CW_USEDEFAULT*/, int cy/*=CW_USEDEFAULT*/)
	{
		HWND hWnd = CreateWindow(_TEXT("ChartCtrl"), wndTitle, WS_CHILD | WS_VISIBLE,
			x, y, cx, cy, hWndParent, NULL, GetModuleHandle(NULL), NULL);
		if(hWnd)
		{
			if(!IsCreated())CreateChart();
			pGlobalChartWnd->Attach(hWnd, nChartType);
			pGlobalChartWnd->GetChart()->SetContinuePaint(false);
			pGlobalChartWnd->GetChart()->SetContinueReact(false);

			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);
		}
		return hWnd;
	}
	/////////////////////////////////////////////////////////////////////////////
	// 以下函数来自CChartWnd
	// 根据标识获得序号
	int		GetChartIndex(int nChartID)
	{
		return pGlobalChartWnd->GetChartIndex(nChartID);
	}
	// 根据序号获得标识
	int		GetChartID(int nChartIndex)
	{
		return pGlobalChartWnd->GetChartID(nChartIndex);
	}
	// 获得视图类型
	int		GetChartType(int nChartIndex)
	{
		return pGlobalChartWnd->GetChartType(nChartIndex);
	}
	// 改变视图类型
	bool	SetChartType(int nType, int nChartIndex)
	{
		return pGlobalChartWnd->SetChartType(nType, nChartIndex);
	}
	// 获得已Attach的次数
	int		GetChartCount()
	{
		return pGlobalChartWnd->GetChartCount();
	}
	
	// 粘窗口，最基本形式
	int		Attach(HWND hWnd)
	{
		return pGlobalChartWnd->Attach(hWnd);
	}
	// 粘窗口，并限制绘图区
	int		Attach(HWND hWnd, RECT rtClient)
	{
		return pGlobalChartWnd->Attach(hWnd, rtClient);
	}
	// 粘窗口，粘到对话框上, 给定控件句柄
	int		AttachDlg(HWND hDlg, HWND hCtrl)
	{
		return pGlobalChartWnd->AttachDlg(hDlg, hCtrl);
	}
	// 粘窗口，粘到对话框上，给定控件ID
	int		AttachDlg(HWND hDlg, UINT nCtrlID)
	{
		return pGlobalChartWnd->AttachDlg(hDlg, nCtrlID);
	}
	// 粘窗口，同时设置绘图类型
	int		Attach(HWND hWnd, int nType)
	{
		return pGlobalChartWnd->Attach(hWnd, nType);
	}
	// 粘窗口，同时设置绘图类型，并限制绘图区
	int		Attach(HWND hWnd, RECT rtClient, int nType)
	{
		return pGlobalChartWnd->Attach(hWnd, rtClient, nType);
	}
	// 粘窗口，粘到对话框上, 给定控件句柄
	int		AttachDlg(HWND hDlg, HWND hCtrl, int nType)
	{
		return pGlobalChartWnd->AttachDlg(hDlg, hCtrl, nType);
	}
	// 粘窗口，粘到对话框上，给定控件ID
	int		AttachDlg(HWND hDlg, UINT nCtrlID, int nType)
	{
		return pGlobalChartWnd->AttachDlg(hDlg, nCtrlID, nType);
	}
	// 卸窗口
	bool	Detach()
	{
		return pGlobalChartWnd->Detach();
	}
	bool	Detach(HWND hWnd)
	{
		return pGlobalChartWnd->Detach(hWnd);
	}
	// 重绘
	void	ReDraw()
	{
		pGlobalChartWnd->ReDraw();
	}
	void	ReDraw(HWND hWnd)
	{
		pGlobalChartWnd->ReDraw(hWnd);
	}
	void	ReDraw(int nChartIndex)
	{
		pGlobalChartWnd->ReDraw(nChartIndex);
	}
	//void	ReDraw(CChart *chart)
	//{
	//	pGlobalChartWnd->ReDraw(chart);
	//}
	
	/////////////////////////////////////////////////////////////////////////////
	//CChart_API  以下函数来自CChart

	// 设置对话框、右键菜单使用的语言
	DeclareFunc_1P_Noret(void, SetLangurage, int, nLang);
	
	// 设置数据处理菜单是否显示
	DeclareFunc_1P_Noret(void, SetDataProcessing, bool, process);
	
	// 测试是否启用工具提示
	DeclareFunc_0P_Ret(bool, IsEnableToolTip);
	// 设置是否启用工具提示
	DeclareFunc_1P_Noret(void, SetEnableToolTip, bool, enable);
	// 检查视图号是否有效
	DeclareFunc_1P_Ret(bool, CheckPlotType, int, nType);	
	
	// 设置默认颜色配置表
	DeclareFunc_1P_Noret(void, SetColorTable, int, nTableIndex);

	// 设置是否显示图像处理的右键菜单
	DeclareFunc_1P_Noret(void, SetImagePopupMenu, bool, bShow);

	// 拷贝图像到剪贴板
	DeclareFunc_1P_Noret(void, CopyImageToClipBoard, HWND, hWnd);
	// 保存图像到文件
	DeclareFunc_3P_Noret(void, SaveImageToFile, HWND, hWnd, TCHAR *, strFileName, TCHAR *, strFileExt);
	// 打印图像
	DeclareFunc_1P_Noret(void, PrintImage, HWND, hWnd);

	// 获取内部Plot指针
	DeclareFunc_0P_Ret(void	*, GetPlot);
	// 获取图像类型编码
	DeclareFunc_0P_Ret(int,	GetType);
	// 设置图像类型编码，注意将清空所有数据
	DeclareFunc_1P_Ret(bool, SetType, int, nType);
	// 设置图像类型编码，并设置绘图窗口，主要用于三维视图，二维视图将忽略hWnd
	DeclareFunc_2P_Ret(bool, SetType, int, nType, HWND, hWnd);
	// 设置图像类型编码，注意将清空所有数据
	DeclareFunc_1P_Ret(bool, CreateChart, int, nType);
	// 设置图像类型编码，并设置绘图窗口，主要用于三维视图，二维视图将忽略hWnd
	DeclareFunc_2P_Ret(bool, CreateChart, int, nType, HWND, hWnd);

	// 测试是否限制绘图区
	DeclareFunc_0P_Ret(bool, IsRectConfined);
	// 设置是否限制绘图区
	DeclareFunc_1P_Noret(void, SetRectConfined, bool, confine);
	// 设置限制区域
	DeclareFunc_1P_Ret(bool, SetConfineRect, RECT, ConfineRect);
	// 获取限制区域
	DeclareFunc_0P_Ret(RECT, GetConfineRect);

	// 测试内部是否无数据
	DeclareFunc_0P_Ret(bool, IsEmpty);
	
	// 重置外观数据
	DeclareFunc_0P_Noret(void, ResetApperance);
	// 获得外观模式序号
	DeclareFunc_0P_Ret(int, GetApperanceMode);
	// 设置外观模式
	DeclareFunc_1P_Noret(void, SetApperanceMode, int, mode);

	// 设置绘图完成后，用户代码可以接着绘图
	DeclareFunc_1P_Noret(void, SetContinuePaint, bool, ctn);
	// 设置消息处理后，用户代码可以接着处理
	DeclareFunc_1P_Noret(void, SetContinueReact, bool, ctn);

	// 测试是否响应鼠标键盘消息
	DeclareFunc_0P_Ret(bool, IsReact);
	// 设置是否响应鼠标键盘消息
	DeclareFunc_1P_Noret(void, SetReact, bool, react);
	// 设置是否响应左键单击按下消息
	DeclareFunc_1P_Noret(void, SetReactLButtonDown, bool, react);
	// 设置是否响应左键单击抬起消息
	DeclareFunc_1P_Noret(void, SetReactLButtonUp, bool, react);
	// 设置是否响应左键双击消息
	DeclareFunc_1P_Noret(void, SetReactLButtonDblClk, bool, react);
	// 设置是否响应鼠标移动消息
	DeclareFunc_1P_Noret(void, SetReactMouseMove, bool, react);
	// 设置是否响应右键菜单消息
	DeclareFunc_1P_Noret(void, SetReactContextMenu, bool, react);
	// 设置是否响应按键消息
	DeclareFunc_1P_Noret(void, SetReactKeyDown, bool, react);
	// 设置是否响应标题
	DeclareFunc_1P_Noret(void, SetReactTitle, bool, react);
	// 设置是否响应坐标轴
	DeclareFunc_1P_Noret(void, SetReactAxes, bool, react);
	// 设置是否响应图例
	DeclareFunc_1P_Noret(void, SetReactLegend, bool, react);
	// 测试消息响应时是否自动刷新，起作用于末尾带R的几个函数和Interactive
	// 主要用于实时曲线显示,此时设置为false
	DeclareFunc_0P_Ret(bool, IsAutoRedraw);
	// 设置消息响应时是否自动刷新，起作用于末尾带R的几个函数和Interactive
	// 主要用于实时曲线显示,此时设置为false
	DeclareFunc_1P_Noret(void, SetAutoRedraw, bool, redraw);
	// 设置绘图区偏移
	DeclareFunc_2P_Noret(void, SetOffsetClient, int, offsetX, int, offsetY);

	// 对SetWnd()设置的窗口句柄绘图
	DeclareFunc_0P_Noret(void, OnDraw);
	// 对窗口句柄绘图
	DeclareFunc_1P_Noret(void, OnDraw, HWND, hWnd);
	// 对设备上下文绘图
	DeclareFunc_1P_Noret(void, OnDraw, HDC, hDC);
	// 在窗口上选择区域绘图
	DeclareFunc_2P_Noret(void, OnDraw, HDC, hDC, RECT, destRect);
	// 在设备上下文对应的窗口上，选择区域绘图
	DeclareFunc_2P_Noret(void, OnDraw, HWND, hWnd, RECT, destRect);

	// 单个的鼠标键盘消息，但不重绘，由用户根据返回值重绘
	DeclareFunc_3P_Ret(bool, OnLButtonDown, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Ret(bool, OnLButtonUp, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Ret(bool, OnLButtonDblClk, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Ret(bool, OnMouseMove, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Ret(bool, OnMouseLeave, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Ret(bool, OnMouseWheel, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Ret(bool, OnContextMenu, HMENU, hMenu, HWND, hWnd, POINT, point );
	DeclareFunc_2P_Ret(bool, OnKeyDown, HWND, hWnd, UINT, key );

	// 单个的鼠标键盘消息，在AutoRedraw为真情况下重绘
	DeclareFunc_3P_Noret(void, OnLButtonDownR, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Noret(void, OnLButtonUpR, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Noret(void, OnLButtonDblClkR, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Noret(void, OnMouseMoveR, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Noret(void, OnMouseLeaveR, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Noret(void, OnMouseWheelR, HWND, hWnd, POINT, point, UINT, ctrlKey );
	DeclareFunc_3P_Noret(void, OnContextMenuR, HMENU, hMenu, HWND, hWnd, POINT, point );
	DeclareFunc_2P_Noret(void, OnKeyDownR, HWND, hWnd, UINT, key );

	// 响应消息，但不重绘，由用户根据返回值重绘
	DeclareFunc_4P_Ret(bool, OnEvent, HWND, hWnd, UINT, message, WPARAM, wParam, LPARAM, lParam);
	// 响应消息，在AutoRedraw为真情况下重绘
	DeclareFunc_4P_Noret(void, Interactive, HWND, hWnd, UINT, message, WPARAM, wParam, LPARAM, lParam);

	// 增加右键菜单项，插件菜单功能
	DeclareFunc_4P_Noret(void, SetPluginMenu, FcnAddPluginPopupMenu, fcnAddPluginMenu, FcnInterpretPluginPopupMenu, fcnInterpretPluginMenu, void *, pParaAdd, void *, pParaInt);
	// 隐藏或显示缺省右键菜单，结合插件菜单功能可实现右键菜单全部自定义
	DeclareFunc_1P_Noret(void, SetDefMenus, bool, bShow);
	// 用户自定义绘图操作
	DeclareFunc_2P_Noret(void, SetUserDrawingFunc, FcnUserDrawing, fcnUserDrawing, void *, pPara);
	// 用户自定义绘图操作
	DeclareFunc_3P_Noret(void, SetUserDrawingFunc, FcnUserDrawing, fcnUserDrawing, void *, pPara, int, nPlotIndex);
	// 用户自定义更新操作
	DeclareFunc_2P_Noret(void, SetUserUpdateFunc, FcnUserUpdate, fcnUserUpdate, void *, pPara);
	// 用户自定义更新操作
	DeclareFunc_3P_Noret(void, SetUserUpdateFunc, FcnUserUpdate, fcnUserUpdate, void *, pPara, int, nPlotIndex);
	
	// 用户自定义鼠标移动前操作，bContinue设置为false跳过默认例程
	DeclareFunc_2P_Noret(void, SetPreMouseMoveFunc, FcnAddonMouseAction, fcnPreMouseMove, void *, pPara);
	// 用户自定义鼠标单击前操作，bContinue设置为false跳过默认例程
	DeclareFunc_2P_Noret(void, SetPreLButtonDownFunc, FcnAddonMouseAction, fcnPreLButtonDown, void *, pPara);
	// 用户自定义鼠标抬起前操作，bContinue设置为false跳过默认例程
	DeclareFunc_2P_Noret(void, SetPreLButtonUpFunc, FcnAddonMouseAction, fcnPreLButtonUp, void *, pPara);
	// 用户自定义鼠标双击前操作，bContinue设置为false跳过默认例程
	DeclareFunc_2P_Noret(void, SetPreLButtonDblClkFunc, FcnAddonMouseAction, fcnPreLButtonDblClk, void *, pPara);
	// 用户自定义按键前操作，bContinue设置为false跳过默认例程
	DeclareFunc_2P_Noret(void, SetPreKeyDownFunc, FcnAddonKeyAction, fcnPreKeyDown, void *, pPara);
	
	// 用户自定义鼠标移动后操作，bContinue被忽略
	DeclareFunc_2P_Noret(void, SetPostMouseMoveFunc, FcnAddonMouseAction, fcnPostMouseMove, void *, pPara);
	// 用户自定义鼠标单击后操作，bContinue被忽略
	DeclareFunc_2P_Noret(void, SetPostLButtonDownFunc, FcnAddonMouseAction, fcnPostLButtonDown, void *, pPara);
	// 用户自定义鼠标抬起后操作，bContinue被忽略
	DeclareFunc_2P_Noret(void, SetPostLButtonUpFunc, FcnAddonMouseAction, fcnPostLButtonUp, void *, pPara);
	// 用户自定义鼠标双击后操作，bContinue被忽略
	DeclareFunc_2P_Noret(void, SetPostLButtonDblClkFunc, FcnAddonMouseAction, fcnPostLButtonDblClk, void *, pPara);
	// 用户自定义按键后操作，bContinue被忽略
	DeclareFunc_2P_Noret(void, SetPostKeyDownFunc, FcnAddonKeyAction, fcnPostKeyDown, void *, pPara);

	// 获得子图序号，仅仅用于kTypeSplit
	DeclareFunc_1P_Ret(int, GetPlotIndexByMousePos, POINT, point);
	
	// 设置在没有数据的时候是否画坐标轴及标题
	DeclareFunc_1P_Noret(void, SetDrawBasicAnyway, bool, draw);
	// 设置在没有数据的时候子图是否画坐标轴及标题
	DeclareFunc_2P_Noret(void, SetDrawBasicAnyway, bool, draw, int, nPlotIndex);
	// 设置无数据时默认屏幕
	DeclareFunc_2P_Noret(void, SetDefScreen, FcnDefScreen, fcnDefScreen, void *, pPara );
	// 设置无数据时子图默认屏幕
	DeclareFunc_3P_Noret(void, SetDefScreen, FcnDefScreen, fcnDefScreen, void *, pPara, int, nPlotIndex );
	// 设置默认屏幕的主字符串
	DeclareFunc_1P_Noret(void, SetDefMainString, TCHAR *, str);
	// 设置子图默认屏幕的主字符串
	DeclareFunc_2P_Noret(void, SetDefMainString, TCHAR *, str, int, nPlotIndex);
	//  设置默认屏幕的副字符串
	DeclareFunc_1P_Noret(void, SetDefSubString, TCHAR *, str);
	//  设置子图默认屏幕的副字符串
	DeclareFunc_2P_Noret(void, SetDefSubString, TCHAR *, str, int, nPlotIndex);
	//  设置默认屏幕的背景图
	DeclareFunc_1P_Noret(void, SetDefImage, HBITMAP, hbm);
	//  设置子图默认屏幕的背景图
	DeclareFunc_2P_Noret(void, SetDefImage, HBITMAP, hbm, int, nPlotIndex);


	// 检测子图序号是否在范围内
	DeclareFunc_1P_Ret(bool, CheckSubPlotIndex, int, nPlotIndex);

	// 获得工具提示字符串
	DeclareFunc_2P_Ret(TCHAR*, GetDescriptionByMousePos, HDC, hDC, POINT, point);
	// 设置工具提示字符串类型
	DeclareFunc_1P_Noret(void, SetToolTipType, int, type);

	// 设置内置缩放模式，打开后可响应鼠标拖放
	DeclareFunc_1P_Noret(void, SetZoomModeBuildIn, bool, zoom);
	// 重置缩放范围
	DeclareFunc_0P_Noret(void, ResetZoomedRanges);
	// 设置图像整体缩放模式，只是绘制，适用于不采用CChartWnd的情况
	DeclareFunc_1P_Noret(void, SetZoomMode, bool, zoom);
	// 设置图像整体缩放时X方向的缩放系数
	DeclareFunc_1P_Noret(void, SetZoomFactorX, double, fact);
	// 设置图像整体缩放时Y方向的缩放系数
	DeclareFunc_1P_Noret(void, SetZoomFactorY, double, fact);
	// 设置图像整体缩放时两个方向的缩放系数
	DeclareFunc_1P_Noret(void, SetZoomFactor, double, fact);
	// 设置图像整体缩放时的缩放中心，中心的代号见本文件开始被注释掉的enum
	DeclareFunc_1P_Noret(void, SetZoomCenterMode, int, center);

	// 添加曲线，前三个参数是曲线数据
	// 第四个参数主要用于分裂视图、分层视图、共享X轴视图，表示子图序号。后续函数相同
	// 返回值是曲线的ID号，注意不是序号，返回-1表示添加曲线失败。后续函数相同
	DeclareFunc_4P_Ret(int, AddCurve, double *, pX, double *, pY, int, nLen, int, nPlotIndex);
	// 添加曲线，其中X坐标为时间，用字符串形式输入
	// 字符串应为"20130528234428"，即年月日时分秒，共14位数字，否则出错
	// 时间只能精确到秒，因为内部使用time_t
	DeclareFunc_5P_Ret(int, AddCurve, TCHAR **, pStrTime, TCHAR *, fmt, double *, pY, int, nLen, int, nPlotIndex);
	// 添加空曲线
	DeclareFunc_1P_Ret(int, AddCurve, int, nPlotIndex);
	// 更新曲线数据
	DeclareFunc_5P_Ret(int, UpdateCurve, int, nDataIndex, double *, pX, double *, pY, int, nLen, int, nPlotIndex);
	// 根据ID号更新曲线数据
	DeclareFunc_5P_Ret(int, UpdateCurveByID, int, nDataID, double *, pX, double *, pY, int, nLen, int, nPlotIndex);
	// 替换曲线数据，不更新范围
	DeclareFunc_5P_Ret(int, ReplaceCurve, int, nDataIndex, double *, pX, double *, pY, int, nLen, int, nPlotIndex);
	// 根据ID号替换曲线数据，不更新范围
	DeclareFunc_5P_Ret(int, ReplaceCurveByID, int, nDataID, double *, pX, double *, pY, int, nLen, int, nPlotIndex);
	// 从曲线的DataID获得Index
	DeclareFunc_2P_Ret(int, GetIndex, int, dataID, int, nPlotIndex);
	// 从曲线的Index获得DataID
	DeclareFunc_2P_Ret(int, GetDataID, int, nIndex, int, nPlotIndex);
	// 设置曲线的dataID，特殊用途，属于倚天剑屠龙刀，不要随便调用
	DeclareFunc_3P_Noret(void, SetDataID, int, newID, int, nIndex, int, nPlotIndex);

	// 添加2维曲线的一个数据点
	// nDataIndex表示曲线的序号,
	// nDataIndex如果等于当前曲线的条数，则新增一条曲线，如果小于0或者大于当前曲线条数，则添加失败
	DeclareFunc_4P_Ret(int, AddPoint2D, double, x, double, y, int, nDataIndex, int, nPlotIndex);
	// 添加数据点，其中X坐标为时间，用字符串形式输入
	// 字符串格式由fmt确定，参考C库函数strftime
	// 时间只能精确到秒，因为内部使用time_t
	DeclareFunc_5P_Ret(int, AddPoint2D, TCHAR *, strTime, TCHAR *, fmt, double, y, int, nDataIndex, int, nPlotIndex);
	// 插入2维曲线的一个数据点
	// nPos表示需要插入的位置,
	DeclareFunc_5P_Ret(int, InsertPoint2D, double, x, double, y, int, nPos, int, nDataIndex, int, nPlotIndex);

	// 添加三维曲线的一个数据点
	DeclareFunc_4P_Ret(int, AddPoint3D, double, x, double, y, double, z, int, nDataIndex);
	// 添加一条三维曲线
	DeclareFunc_4P_Ret(int, AddCurve, double *, pX, double *, pY, double *, pZ, int, nLen);	

	// 清除所有曲线数据
	DeclareFunc_0P_Noret(void, ClrAllData);
	// 清除单子图全部曲线数据
	DeclareFunc_1P_Noret(void, ClrPlotData, int, nPlotIndex);
	// 清除单条曲线数据
	DeclareFunc_2P_Noret(void, ClrSingleData, int, nDataIndex, int, nPlotIndex);
	// 清空单条曲线数据
	DeclareFunc_2P_Noret(void, EmptySingleData, int, nDataIndex, int, nPlotIndex);
	// 为曲线预留内存空间。主要目的是加快速度
	// CChart内部数据采用vector。如果数据点过多，随着数据点的增加，CChart将不断地为vector重新分配内存，影响速度
	// 如果根据实际情况预留了内存，则会一次性成功分配内存
	DeclareFunc_2P_Noret(void, SetReservedDataLength, int, nLen, int, nPlotIndex);
	// 获得曲线的数据点数
	DeclareFunc_2P_Ret(int, GetDataPointsCount, int, nDataIndex, int, nPlotIndex);
	// 获取数据点x
	DeclareFunc_3P_Ret(double, GetDataPointX, int, nPointIndex, int, nDataIndex, int, nPlotIndex);
	// 获取数据点y
	DeclareFunc_3P_Ret(double, GetDataPointY, int, nPointIndex, int, nDataIndex, int, nPlotIndex);
	//获取数据点
	DeclareFunc_4P_Ret(bool, GetDataPoint, double *, data, int, nPointIndex, int, nDataIndex, int, nPlotIndex);
	// 去除曲线上单个数据点
	DeclareFunc_3P_Ret(int, RemoveSinglePoint, int, nPointIndex, int, nDataIndex, int, nPlotIndex);
	// 去除曲线上第一个数据点
	DeclareFunc_2P_Ret(int, RemoveFirstPoint, int, nDataIndex, int, nPlotIndex);
	// 去除曲线上第一个数据点，同时内部数据左移，可保证占用内存的位置不变，但效率比RemoveFirstPoint低。
	DeclareFunc_2P_Ret(int, RemoveFirstPointAndShift, int, nDataIndex, int, nPlotIndex);
	// 去除曲线上最后一个数据点
	DeclareFunc_2P_Ret(int, RemoveLastPoint, int, nDataIndex, int, nPlotIndex);
	// 滑动X数据
	DeclareFunc_3P_Ret(int, SlipXValue, int, nStep, int, nDataIndex, int, nPlotIndex);
	// 滑动Y数据
	DeclareFunc_3P_Ret(int, SlipYValue, int, nStep, int, nDataIndex, int, nPlotIndex);

	// 添加饼图的一个值
	DeclareFunc_1P_Ret(int, AddPie, double, value);
	// 添加饼图的一个值，同时设置该块饼的标题
	DeclareFunc_2P_Ret(int, AddPie, double, value, TCHAR*, title);
	// 添加柱图的一个序列
	DeclareFunc_2P_Ret(int, AddStems, double *, pData, int, nLen);
	// 添加柱图的一个序列，同时设置该序列的标题
	DeclareFunc_3P_Ret(int, AddStems, double *, pData, int, nLen, TCHAR*, title);

	// 设置场函数，用于等高线图、云图
	DeclareFunc_1P_Noret(void, SetFieldFcn, FcnField, _pFieldFcn);
#ifndef _WIN64
	void		SetFieldFcn(double (_stdcall *_pFieldFcn) (double, double))
	{
		pGlobalChartWnd->GetChart()->SetFieldFcn(_pFieldFcn);
	}
#endif
	// 获得场函数指针，用于等高线图、云图
	DeclareFunc_0P_Ret(FcnField*, GetFieldFcn );
	// 添加等高线数据点
	DeclareFunc_3P_Noret(void, AddContourPoint, double, x, double, y, double, h);
	// 清除等高线数据点
	DeclareFunc_0P_Noret(void, ClrContourPoints);
	// 设置饼图单个饼的标题
	DeclareFunc_2P_Noret(void, SetPieTitle, const TCHAR*, title, int, which);
	// 设置饼图单个饼的颜色
	DeclareFunc_2P_Noret(void, SetPieColor, COLORREF, color, int, which);
	// 设置饼图最大饼数
	DeclareFunc_1P_Noret(void, SetMaxPies, int, maxp);
	// 获得饼图饼标题字体
	DeclareFunc_0P_Ret(LOGFONT	&, GetPieTitleFont);
	
	// 设置柱图单根柱子的标签
	DeclareFunc_2P_Noret(void, SetStemLabel, const TCHAR*, label, int, which);
	// 设置柱图单个序列的标题
	DeclareFunc_2P_Noret(void, SetStemTitle, const TCHAR*, title, int, nDataIndex);
	// 设置柱图单根柱子所占比例
	DeclareFunc_1P_Noret(void, SetStemRatio, double, ratio);
	// 设置柱图从那条坐标轴开始画
	DeclareFunc_1P_Noret(void, SetStemBase, int, base);
	// 设置柱图的最大数量
	DeclareFunc_1P_Noret(void, SetStemMaxNum, int, num);

	// 设置等高线图等高线的数量
	DeclareFunc_1P_Noret(void, SetContourLineNum, int, num);
	// 设置等高线图等高线绘制的精度，n越大，精度越高，一般5就可以了
	DeclareFunc_1P_Noret(void, SetContourPrecision, int, n);
	// 设置显示等高线数值
	DeclareFunc_1P_Noret(void, SetContourValueShow, bool, show);
	
	// 设置云图的起始颜色
	DeclareFunc_1P_Noret(void, SetContourMapColor1, COLORREF, cr);
	// 设置云图的最终颜色
	DeclareFunc_1P_Noret(void, SetContourMapColor2,COLORREF, cr);
	
	// 设置由数据点输入，而不是场函数
	DeclareFunc_0P_Noret(void, SetContourByPoints);
	
	// 设置云图的右坐标轴作为图例
	DeclareFunc_1P_Noret(void, SetRightAxisAsLegend, bool, as);
	// 设置云图的上坐标轴作为图例
	DeclareFunc_1P_Noret(void, SetTopAxisAsLegend, bool, as);

	// 增加一个图层，用于分层视图、共享X轴视图
	DeclareFunc_0P_Ret(int, AddLayer);
	// 设置子图数量，用于分裂视图，其中mode表示分裂模式，nRows表示列数，nCols表示行数
	// 分裂模式参加本文件头部被注释掉的enum。可以解除注释，就能直接使用这些enum了。
	DeclareFunc_3P_Noret(void, ResizePlots, int, mode, int, nRows, int, nCols);
	// 设置图层数量，用于分层视图、共享X轴视图
	DeclareFunc_1P_Noret(void, ResizePlots, int, nLayers);

	// 获得子图数量
	DeclareFunc_0P_Ret(int, GetSubPlotCount);
	// 有子图时获得当前子图序号
	DeclareFunc_0P_Ret(int, GetTop);

	// 设置多层视图的图层是否颜色归一化
	DeclareFunc_1P_Noret(void, SetUniteLayerColor, bool, bUnite);
	// 设置多层视图图层颜色归一化时是否包括曲线
	DeclareFunc_2P_Noret(void, SetExcludeDataWhenUniteColor, bool, bExclude, int, nPlotIndex);

	// 获得曲线数量，对于多层视图为子图曲线条数
	DeclareFunc_1P_Ret(int, GetPlotDataCount, int, nPlotIndex);
	// 设置单条曲线是否响应鼠标键盘消息
	DeclareFunc_3P_Noret(void, SetDataReact, bool, react, int, nDataIndex, int, nPlotIndex);
	// 强制使用GDI+进行渲染
	DeclareFunc_3P_Noret(void, SetDataForceGdiPlus, bool, bForce, int, nDataIndex, int, nPlotIndex);
	// 获得数据曲线的标题,nDataIndex表示曲线的序号
	DeclareFunc_2P_Ret(const TCHAR*, GetDataTitle, int, nDataIndex, int, nPlotIndex);
	// 设置数据标题
	DeclareFunc_3P_Noret(void, SetDataTitle, const TCHAR*, title, int, nDataIndex, int, nPlotIndex);
	// 所有数据自动设置颜色
	DeclareFunc_2P_Noret(void, SetAutoDataColorGeneral, bool, bAuto, int, nPlotIndex);
	// 数据自动设置颜色
	DeclareFunc_3P_Noret(void, SetAutoDataColor, bool, bAuto, int, nDataIndex, int, nPlotIndex);
	// 获得数据颜色
	DeclareFunc_2P_Ret(COLORREF, GetDataColor, int, nDataIndex, int, nPlotIndex);
	// 设置数据颜色
	DeclareFunc_3P_Noret(void, SetDataColor, COLORREF, color, int, nDataIndex, int, nPlotIndex);
	// 是否双色渐变模式
	DeclareFunc_2P_Ret(bool, IsBiColorMode, int, nDataIndex, int, nPlotIndex);
	// 设置双色渐变模式
	DeclareFunc_3P_Noret(void, SetBiColorMode, bool, bBi, int, nDataIndex, int, nPlotIndex);
	// 获得双色渐变模式下第二数据颜色
	DeclareFunc_2P_Ret(COLORREF, GetDataColor2, int, nDataIndex, int, nPlotIndex);
	// 设置双色渐变模式下第二数据颜色
	DeclareFunc_3P_Noret(void, SetDataColor2, COLORREF, color, int, nDataIndex, int, nPlotIndex);
	// 是否多色渐变模式，优先于双色渐变
	DeclareFunc_2P_Ret(bool, IsMultiColorMode, int, nDataIndex, int, nPlotIndex);
	// 设置多色渐变模式
	DeclareFunc_3P_Noret(void, SetMultiColorMode, bool, bMul, int, nDataIndex, int, nPlotIndex);
	// 获得多色渐变模式下输入节点的方法，0：数据点比例0.0-1.0，1：X数据值，2：Y数据值
	DeclareFunc_2P_Ret(int, GetMultiColorInputType, int, nDataIndex, int, nPlotIndex);
	// 设置多色渐变模式下输入节点的方法
	DeclareFunc_3P_Noret(void, SetMultiColorInputType, int, nType, int, nDataIndex, int, nPlotIndex);
	// 设置多色渐变模式的节点，其中ratio的的范围由输入方法决定
	DeclareFunc_4P_Noret(void, AddSegmentColor, double, ratio, COLORREF, color, int, nDataIndex, int, nPlotIndex);
	// 是否逐数据点定色
	DeclareFunc_2P_Ret(bool, IsColorPtByPt, int, nDataIndex, int, nPlotIndex);
	// 是否逐数据点定色
	DeclareFunc_3P_Noret(void, SetColorPtByPt, bool, bBy, int, nDataIndex, int, nPlotIndex);
	// 添加数据点颜色，注意，添加的颜色数据点必须和数据点个数完全一样，不然没有作用
	DeclareFunc_3P_Noret(void, AddDataPointColor, COLORREF, color, int, nDataIndex, int, nPlotIndex);
	// 设置数据点颜色
	DeclareFunc_4P_Noret(void, SetDataPointColor, COLORREF, color, int, nPointIndex, int, nDataIndex, int, nPlotIndex);
	// 删除一个数据点颜色
	DeclareFunc_3P_Noret(void, EraseDataPointColor, int, nPointIndex, int, nDataIndex, int, nPlotIndex);
	// 插入一个数据点颜色
	DeclareFunc_4P_Noret(void, InsertDataPointColor, COLORREF, color, int, nPointIndex, int, nDataIndex, int, nPlotIndex);

	// 获得数据线型
	DeclareFunc_2P_Ret(int, GetDataLineStyle, int, nDataIndex, int, nPlotIndex);
	// 设置数据线型
	DeclareFunc_3P_Noret(void, SetDataLineStyle, int, nStyle, int, nDataIndex, int, nPlotIndex);
	// 获得数据线宽
	DeclareFunc_2P_Ret(int, GetDataLineSize, int, nDataIndex, int, nPlotIndex);
	// 设置数据线宽
	DeclareFunc_3P_Noret(void, SetDataLineSize, int, nSize, int, nDataIndex, int, nPlotIndex);
	// 设置曲线的绘制方式
	DeclareFunc_3P_Noret(void, SetPlotType, int, nType, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点标记是否显示
	DeclareFunc_3P_Noret(void, SetMarkerShow, bool, bShow, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点标记是否填充
	DeclareFunc_3P_Noret(void, SetMarkerFill, bool, bFill, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点标记的形状
	DeclareFunc_3P_Noret(void, SetMarkerType, int, nType, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点的大小
	DeclareFunc_3P_Noret(void, SetMarkerSize, int, nSize, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点标记频率，0表示不标记，正n表示每隔n个点标记一次，负n表示一共标记n个点
	DeclareFunc_3P_Noret(void, SetMarkerFreq, int, nFreq, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点标记的起始点，用于不是全部标记时
	DeclareFunc_3P_Noret(void, SetMarkerStartIndex, int, nStartIndex, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点第一个点标记
	DeclareFunc_3P_Noret(void, SetMarkFirstPoint, bool, bMark, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点最后一个点标记
	DeclareFunc_3P_Noret(void, SetMarkLastPoint, bool, bMark, int, nDataIndex, int, nPlotIndex);

	// 设置曲线散点显示模式
	DeclareFunc_2P_Noret(void, SetScatter, int, nDataIndex, int, nPlotIndex);
	// 设置曲线填充颜色
	DeclareFunc_6P_Noret(void, SetDataFillColor, bool, bFill, COLORREF, color, int, nOpaque, int, nFillMode, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据棒显示
	DeclareFunc_5P_Noret(void, SetDataBarMode, bool, bShowBar, int, nBarBase, int, nBarSize, int, nDataIndex, int, nPlotIndex);
	// 设置选择和高亮模式
	DeclareFunc_3P_Noret(void, SetSLMode, int, nMode, int, nDataIndex, int, nPlotIndex);
	// 获取数据的标志，特殊用途
	DeclareFunc_2P_Ret(bool, GetDataFlag, int, nDataIndex, int, nPlotIndex);
	// 设置数据的标志，特殊用途
	DeclareFunc_3P_Noret(void, SetDataFlag, bool, bFlag, int, nDataIndex, int, nPlotIndex);
	// 获取数据可见标志
	DeclareFunc_2P_Ret(bool, IsDataVisible, int, nDataIndex, int, nPlotIndex);
	//设置数据是否可见
	DeclareFunc_3P_Noret(void, SetDataVisible, bool, bVis, int, nDataIndex, int, nPlotIndex);
	// 设置压缩显示模式，主要用于大数据量时的显示
	// 压缩显示模式中，如果相邻数据点在屏幕上显示时的X相同，将压缩成一个数据点，值取平均，
	DeclareFunc_3P_Noret(void, SetDataCompactDraw, bool, bCompact, int, nDataIndex, int, nPlotIndex);
	// 设置数据按直方图模式显示
	DeclareFunc_3P_Noret(void, SetDataHistro, bool, bHistro, int, nDataIndex, int, nPlotIndex);

	// 获得高亮曲线的序号，目前只适用于折线图
	DeclareFunc_0P_Ret(int, GetIndexOfLighted);
	
	// 设置曲线数据点数值是否显示
	DeclareFunc_3P_Noret(void, SetDataValueShow, bool, bShow, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点数值显示方式，1为X，2为Y，3为X:Y，其它为不显示
	DeclareFunc_3P_Noret(void, SetDataValueType, int, nType, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点数值的字体大小
	DeclareFunc_3P_Noret(void, SetDataValueFontSize, int, nSize, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点数值显示频率，0表示不显示，正n表示每隔n个点显示一次，负n表示一共显示n个点
	DeclareFunc_3P_Noret(void, SetDataValueFreq, int, nFreq, int, nDataIndex, int, nPlotIndex);
	// 设置曲线数据点数值显示的起始点，用于不是全部显示时
	DeclareFunc_3P_Noret(void, SetDataValueStartIndex, int, nStartIndex, int, nDataIndex, int, nPlotIndex);

	// 检测示波器模式
	DeclareFunc_1P_Ret(bool, IsOscilloMode, int, nPlotIndex);
	// 设置示波器模式
	DeclareFunc_2P_Noret(void, SetOscilloMode, bool, bOscillo, int, nPlotIndex);
	// 检测是否已启动示波器模式
	DeclareFunc_1P_Ret(bool, IsStartOscilloScope, int, nPlotIndex);
	// 设置是否已启动示波器模式
	DeclareFunc_2P_Noret(void, SetStartOscilloScope, bool, bOscillo, int, nPlotIndex);
	// 检测是否可以拖动曲线
	DeclareFunc_0P_Ret(bool, IsEnableDataDrag);
	// 设置是否可以拖动曲线
	DeclareFunc_1P_Noret(void, SetEnableDataDrag, bool, enable);
	// 检测是否可以拖动区间边线
	DeclareFunc_0P_Ret(bool, IsEnableZoneDrag);
	// 设置是否可以拖动区间边线
	DeclareFunc_1P_Noret(void, SetEnableZoneDrag, bool, enable);
	// 移动曲线
	DeclareFunc_4P_Ret(bool, MoveData, double, offsetx, double, offsety, int, nDataIndex, int, nPlotIndex);
	// 获得曲线的水平偏移
	DeclareFunc_2P_Ret(double, GetDataOffsetX, int, nDataIndex, int, nPlotIndex);
	// 设置曲线的水平偏移
	DeclareFunc_3P_Noret(void, SetDataOffsetX, double, offset, int, nDataIndex, int, nPlotIndex);
	// 获得曲线的竖直偏移
	DeclareFunc_2P_Ret(double, GetDataOffsetY, int, nDataIndex, int, nPlotIndex);
	// 设置曲线的竖直偏移
	DeclareFunc_3P_Noret(void, SetDataOffsetY, double, offset, int, nDataIndex, int, nPlotIndex);
	// 设置X方向单位，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetXUnit, TCHAR *, unit, int, nDataIndex);
	// 设置Y方向单位，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetYUnit, TCHAR *, unit, int, nDataIndex);
	// 在图例中仅仅显示Y方向的比例尺，仅仅用于模拟示波器
	DeclareFunc_1P_Noret(void, SetShowYSCaleOnly, bool, only);
	// 检测光标绑定到鼠标
	DeclareFunc_1P_Ret(bool, IsHoldCursorToMouse, int, nPlotIndex);
	// 设置光标绑定到鼠标
	DeclareFunc_2P_Noret(void, SetHoldCursorToMouse, bool, hold, int, nPlotIndex);
	// 检测光标绑定到曲线
	DeclareFunc_1P_Ret(bool, IsHoldCursorToCurve, int, nPlotIndex);
	// 设置光标绑定到曲线
	DeclareFunc_2P_Noret(void, SetHoldCursorToCurve, bool, hold, int, nPlotIndex);
	// 获得光标绑定的曲线序号
	DeclareFunc_1P_Ret(int, GetIndexToHoldCursor, int, nPlotIndex);
	// 设置光标绑定的曲线序号
	DeclareFunc_2P_Noret(void, SetIndexToHoldCursor, int, nCurveIndex, int, nPlotIndex);
	// 检测零点箭头绘制
	DeclareFunc_1P_Ret(bool, IsShowZeroArrow, int, nPlotIndex);
	// 设置零点箭头绘制
	DeclareFunc_2P_Noret(void, SetShowZeroArrow, bool, show, int, nPlotIndex);
	// 检测触发箭头绘制
	DeclareFunc_1P_Ret(bool, IsShowTrigArrow, int, nPlotIndex);
	// 设置触发箭头绘制
	DeclareFunc_2P_Noret(void, SetShowTrigArrow, bool, show, int, nPlotIndex);
	// 显示水平光标线，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetShowHCursor, bool, show, int, nPlotIndex);
	// 显示竖直光标线，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetShowVCursor, bool, show, int, nPlotIndex);
	// 获得光标线颜色，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(COLORREF, GetCursorColor, int, nPlotIndex);
	// 设置光标线颜色，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetCursorColor, COLORREF, color, int, nPlotIndex);
	// 获得光标线线宽，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(int, GetCursorSize, int, nPlotIndex);
	// 设置光标线线宽，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetCursorSize, int, size, int, nPlotIndex);
	// 获得光标线线型，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(int, GetCursorStyle, int, nPlotIndex);
	// 设置光标线线型，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetCursorStyle, int, style, int, nPlotIndex);
	// 获得水平光标值，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(double, GetCursorX, int, nPlotIndex);
	// 设置水平光标值，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetCursorX, double, cursor, int, nPlotIndex);
	// 获得竖直光标值，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(double, GetCursorY, int, nPlotIndex);
	// 设置竖直光标值，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetCursorY, double, cursor, int, nPlotIndex);
	// 显示水平选择区，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetShowHSel, bool, show, int, nPlotIndex);
	// 显示竖直选择区，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetShowVSel, bool, show, int, nPlotIndex);
	// 获得水平选择区的下限，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(double, GetHSelLower, int, nPlotIndex);
	// 设置水平选择区的下限，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetHSelLower, int, val, int, nPlotIndex);
	// 获得水平选择区的上限，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(double, GetHSelUpper, int, nPlotIndex);
	// 设置水平选择区的上限，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetHSelUpper, int, val, int, nPlotIndex);
	// 获得竖直选择区的下限，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(double, GetVSelLower, int, nPlotIndex);
	// 设置竖直选择区的下限，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetVSelLower, int, val, int, nPlotIndex);
	// 获得竖直选择区的上限，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(double, GetVSelUpper, int, nPlotIndex);
	// 设置竖直选择区的上限，仅仅用于模拟示波器
	DeclareFunc_2P_Noret(void, SetVSelUpper, int, val, int, nPlotIndex);
	// 获得水平选择区的宽度，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(double, GetHSelWidth, int, nPlotIndex);
	// 获得竖直选择区的宽度，仅仅用于模拟示波器
	DeclareFunc_1P_Ret(double, GetVSelWidth, int, nPlotIndex);


	// 获得图像标题
	DeclareFunc_0P_Ret(TCHAR*, GetTitle);
	// 获得子图标题，可用于分裂视图、分层视图、共享X轴视图
	DeclareFunc_1P_Ret(TCHAR*, GetTitle, int, nPlotIndex);
	// 设置图像标题
	DeclareFunc_1P_Noret(void, SetTitle, const TCHAR*, title);
	// 设置子图标题
	DeclareFunc_2P_Noret(void, SetTitle, const TCHAR*, title, int, nPlotIndex);
	// 获得图像标题位置
	DeclareFunc_0P_Ret(int, GetTitlePosition);
	// 获得子图标题位置
	DeclareFunc_1P_Ret(int, GetTitlePosition, int, nPlotIndex);
	// 设置图像标题位置。负数：左；0：中；正数：右
	DeclareFunc_1P_Noret(void, SetTitlePosition, int, position);
	// 设置子图标题位置。负数：左；0：中；正数：右
	DeclareFunc_2P_Noret(void, SetTitlePosition, int, position, int, nPlotIndex);
	// 图像标题是否显示
	DeclareFunc_0P_Ret(bool, IsTitleShow);
	// 子图标题是否显示
	DeclareFunc_1P_Ret(bool, IsTitleShow, int, nPlotIndex);
	// 设置显示图像标题
	DeclareFunc_1P_Noret(void, SetTitleShow, bool, show);
	// 设置显示子图标题
	DeclareFunc_2P_Noret(void, SetTitleShow, bool, show, int, nPlotIndex);
	// 获得图像标题颜色
	DeclareFunc_0P_Ret(COLORREF, GetTitleColor);
	// 获得子图标题颜色
	DeclareFunc_1P_Ret(COLORREF, GetTitleColor, int, nPlotIndex);
	// 设置图像标题颜色
	DeclareFunc_1P_Noret(void, SetTitleColor, COLORREF, color);
	// 设置子图标题颜色
	DeclareFunc_2P_Noret(void, SetTitleColor, COLORREF, color, int, nPlotIndex);
	// 获得标题字体
	DeclareFunc_0P_Ret(LOGFONT, GetTitleFont);
	// 获得子图标题字体
	DeclareFunc_1P_Ret(LOGFONT, GetTitleFont, int, nPlotIndex);
	// 设置标题字体
	DeclareFunc_1P_Noret(void, SetTitleFont, LOGFONT, logFont);
	// 设置子图标题字体
	DeclareFunc_2P_Noret(void, SetTitleFont, LOGFONT, logFont, int, nPlotIndex);
	// 分层视图显示主标题
	DeclareFunc_1P_Noret(void, SetShowParentTitle, bool, set);

	// 获得图像副标题
	DeclareFunc_0P_Ret(TCHAR*, GetSubTitle); 
	// 获得子图副标题，可用于分裂视图、分层视图、共享X轴视图
	DeclareFunc_1P_Ret(TCHAR*, GetSubTitle, int, nPlotIndex);
	// 设置图像副标题
	DeclareFunc_1P_Noret(void, SetSubTitle, const TCHAR*, title);
	// 设置子图副标题
	DeclareFunc_2P_Noret(void, SetSubTitle, const TCHAR*, title, int, nPlotIndex);
	// 获得图像副标题位置
	DeclareFunc_0P_Ret(int, GetSubTitlePosition);
	// 获得子图副标题位置
	DeclareFunc_1P_Ret(int, GetSubTitlePosition, int, nPlotIndex);
	// 设置图像副标题位置。负数：左；0：中；正数：右
	DeclareFunc_1P_Noret(void, SetSubTitlePosition, int, position);
	// 设置子图副标题位置。负数：左；0：中；正数：右
	DeclareFunc_2P_Noret(void, SetSubTitlePosition, int, position, int, nPlotIndex);
	// 图像副标题是否显示
	DeclareFunc_0P_Ret(bool, IsSubTitleShow);
	// 子图副标题是否显示
	DeclareFunc_1P_Ret(bool, IsSubTitleShow, int, nPlotIndex);
	// 设置显示图像副标题
	DeclareFunc_1P_Noret(void, SetSubTitleShow, bool, show);
	// 设置显示子图副标题
	DeclareFunc_2P_Noret(void, SetSubTitleShow, bool, show, int, nPlotIndex);
	// 获得图像副标题颜色
	DeclareFunc_0P_Ret(COLORREF, GetSubTitleColor);
	// 获得子图副标题颜色
	DeclareFunc_1P_Ret(COLORREF, GetSubTitleColor, int, nPlotIndex);
	// 设置图像副标题颜色
	DeclareFunc_1P_Noret(void, SetSubTitleColor, COLORREF, color);
	// 设置子图副标题颜色
	DeclareFunc_2P_Noret(void, SetSubTitleColor, COLORREF, color, int, nPlotIndex);
	// 获得副标题字体
	DeclareFunc_0P_Ret(LOGFONT, GetSubTitleFont);
	// 获得子图副标题字体
	DeclareFunc_1P_Ret(LOGFONT, GetSubTitleFont, int, nPlotIndex);
	// 设置副标题字体
	DeclareFunc_1P_Noret(void, SetSubTitleFont, LOGFONT, logFont);
	// 设置子图副标题字体
	DeclareFunc_2P_Noret(void, SetSubTitleFont, LOGFONT, logFont, int, nPlotIndex);
	
	// 设置高亮边框颜色
	DeclareFunc_1P_Noret(void, SetLightColor, COLORREF, color);
	// 设置子图高亮边框颜色
	DeclareFunc_2P_Noret(void, SetLightColor, COLORREF, color, int, nPlotIndex);
	// 设置高亮边框线宽
	DeclareFunc_1P_Noret(void, SetLightLineSize, int, nSize);
	// 设置子图高亮边框线宽
	DeclareFunc_2P_Noret(void, SetLightLineSize, int, nSize, int, nPlotIndex);
	// 设置高亮边框线型
	DeclareFunc_1P_Noret(void, SetLightLineStyle, int, nStyle);
	// 设置子图高亮边框线型
	DeclareFunc_2P_Noret(void, SetLightLineStyle, int, nStyle, int, nPlotIndex);


	// 增加坐标轴，坐标轴的位置location见本文件头部被注释掉的enum
	DeclareFunc_2P_Noret(void, AddAxis, int, location, int, nPlotIndex);
	// 设置子图坐标轴标题
	DeclareFunc_3P_Noret(void, SetAxisTitle, const TCHAR*, title, int, location, int, nPlotIndex);
	// 设置坐标轴标题
	DeclareFunc_2P_Noret(void, SetAxisTitle, const TCHAR*, title, int, location);
	// 设置子图坐标轴标题字体
	DeclareFunc_3P_Noret(void, SetAxisTitleFont, LOGFONT, logFont, int, location, int, nPlotIndex);
	// 设置坐标轴标题字体
	DeclareFunc_2P_Noret(void, SetAxisTitleFont, LOGFONT, logFont, int, location);
	// 设置子图坐标轴标签字体
	DeclareFunc_3P_Noret(void, SetAxisLabelFont, LOGFONT, logFont, int, location, int, nPlotIndex);
	// 设置坐标轴标签字体
	DeclareFunc_2P_Noret(void, SetAxisLabelFont, LOGFONT, logFont, int, location);
	// 设置子图坐标轴标题位置
	DeclareFunc_3P_Noret(void, SetAxisTitlePosition, int, position, int, location, int, nPlotIndex);
	// 设置坐标轴标题位置
	DeclareFunc_2P_Noret(void, SetAxisTitlePosition, int, position, int, location);
	// 设置子图坐标轴与绘图区的间隙
	DeclareFunc_3P_Noret(void, SetAxisGap, int, gap, int, location, int, nPlotIndex);
	// 设置坐标轴与绘图区的间隙
	DeclareFunc_2P_Noret(void, SetAxisGap, int, gap, int, location);
	// 设置子图坐标轴刻度线位置
	DeclareFunc_3P_Noret(void, SetAxisTickPosition, int, pos, int, location, int, nPlotIndex);
	// 设置坐标轴刻度线位置
	DeclareFunc_2P_Noret(void, SetAxisTickPosition, int, pos, int, location);
	// 设置子图坐标轴副刻度线位置
	DeclareFunc_3P_Noret(void, SetAxisMinorTickPosition, int, pos, int, location, int, nPlotIndex);
	// 设置坐标轴副刻度线位置
	DeclareFunc_2P_Noret(void, SetAxisMinorTickPosition, int, pos, int, location);
	// 设置子图时间坐标轴
	DeclareFunc_3P_Noret(void, SetAxisToTime, bool, bTime, int, location, int, nPlotIndex);
	// 设置时间坐标轴
	DeclareFunc_2P_Noret(void, SetAxisToTime, bool, bTime, int, location);
	// 设置子图时间坐标轴格式,"%Y%m%d%H%M%S",参考strftime函数或CTime类的格式化方法
	DeclareFunc_3P_Noret(void, SetAxisTimeFormat, TCHAR *, format, int, location, int, nPlotIndex);
	// 设置时间坐标轴格式,"%Y%m%d%H%M%S",参考strftime函数或CTime类的格式化方法
	DeclareFunc_2P_Noret(void, SetAxisTimeFormat, TCHAR *, format, int, location);
	// 设置子图显示颜色棒
	DeclareFunc_3P_Noret(void, SetAxisColorBar, bool, bShow, int, location, int, nPlotIndex);
	// 设置显示颜色棒
	DeclareFunc_2P_Noret(void, SetAxisColorBar, bool, bShow, int, location);
	// 设置子图坐标轴颜色
	DeclareFunc_3P_Noret(void, SetAxisColor, COLORREF, color, int, location, int, nPlotIndex);
	// 设置坐标轴颜色
	DeclareFunc_2P_Noret(void, SetAxisColor, COLORREF, color, int, location);
	// 设置坐标轴是否显示刻度值
	DeclareFunc_2P_Noret(void, SetAxisLabelShow, bool, bShow, int, location);
	// 设置子图坐标轴是否显示刻度值
	DeclareFunc_3P_Noret(void, SetAxisLabelShow, bool, bShow, int, location, int, nPlotIndex);
	// 设置子图坐标值的格式,参考printf的格式化方法
	DeclareFunc_3P_Noret(void, SetAxisLabelFormat, TCHAR *, format, int, location, int, nPlotIndex);
	// 设置坐标值的格式,参考printf的格式化方法
	DeclareFunc_2P_Noret(void, SetAxisLabelFormat, TCHAR *, format, int, location);
	// 设置子图坐标轴是否显示
	DeclareFunc_3P_Noret(void, SetAxisShow, bool, bShow, int, location, int, nPlotIndex);
	// 设置坐标轴是否显示
	DeclareFunc_2P_Noret(void, SetAxisShow, bool, bShow, int, location);
	// 子图坐标轴是否显示
	DeclareFunc_2P_Ret(bool, IsAxisShow, int, location, int, nPlotIndex);
	//坐标轴是否显示
	DeclareFunc_1P_Ret(bool, IsAxisShow, int, location);
	// 设置坐标轴紧凑模式
	DeclareFunc_1P_Noret(void, SetAxesCompact, int, nPlotIndex);
	// 添加子图隐藏坐标轴标签的一个数据范围
	DeclareFunc_4P_Noret(void, AddRange4LabelHiden, double, low, double, high, int, location, int, nPlotIndex);
	// 添加隐藏坐标轴标签的一个数据范围
	DeclareFunc_3P_Noret(void, AddRange4LabelHiden, double, low, double, high, int, location);
	// 设置坐标轴标签替换
	DeclareFunc_2P_Noret(void, SetReplaceLabel, bool, bReplace, int, location);
	// 设置子图坐标轴标签替换
	DeclareFunc_3P_Noret(void, SetReplaceLabel, bool, bReplace, int, location, int, nPlotIndex);
	// 添加一个替换的标签
	DeclareFunc_2P_Noret(void, AddAlternateLabel, TCHAR *, label, int, location);
	// 子图添加一个替换的标签
	DeclareFunc_3P_Noret(void, AddAlternateLabel, TCHAR *, label, int, location, int, nPlotIndex);
	// 清空坐标轴替换标签
	DeclareFunc_1P_Noret(void, ClrAlternateLabel, int, location);
	// 清空子图坐标轴替换标签
	DeclareFunc_2P_Noret(void, ClrAlternateLabel, int, location, int, nPlotIndex);

	// 子图自动设置X坐标轴刻度数
	DeclareFunc_2P_Noret(void, SetXAutoTicks, bool, bAuto, int, nPlotIndex);
	// 自动设置X坐标轴刻度数
	DeclareFunc_1P_Noret(void, SetXAutoTicks, bool, bAuto);
	// 子图自动设置Y坐标轴刻度数
	DeclareFunc_2P_Noret(void, SetYAutoTicks, bool, bAuto, int, nPlotIndex);
	// 自动设置Y坐标轴刻度数
	DeclareFunc_1P_Noret(void, SetYAutoTicks, bool, bAuto);
	// 设置子图X坐标轴刻度数
	DeclareFunc_2P_Noret(void, SetXTickCount, int, count, int, nPlotIndex);
	// 设置坐标X轴刻度数
	DeclareFunc_1P_Noret(void, SetXTickCount, int, count);
	// 设置子图Y坐标轴刻度数
	DeclareFunc_2P_Noret(void, SetYTickCount, int, count, int, nPlotIndex);
	// 设置坐标Y轴刻度数
	DeclareFunc_1P_Noret(void, SetYTickCount, int, count);
	// 子图自动设置X坐标轴副刻度数
	DeclareFunc_2P_Noret(void, SetXAutoMinorTicks, bool, bAuto, int, nPlotIndex);
	// 自动设置X坐标轴副刻度数
	DeclareFunc_1P_Noret(void, SetXAutoMinorTicks, bool, bAuto);
	// 子图自动设置Y坐标轴副刻度数
	DeclareFunc_2P_Noret(void, SetYAutoMinorTicks, bool, bAuto, int, nPlotIndex);
	// 自动设置Y坐标轴副刻度数
	DeclareFunc_1P_Noret(void, SetYAutoMinorTicks, bool, bAuto);
	// 设置子图X坐标轴副刻度数
	DeclareFunc_2P_Noret(void, SetXMinorTickCount, int, count, int, nPlotIndex);
	// 设置X坐标轴副刻度数
	DeclareFunc_1P_Noret(void, SetXMinorTickCount, int, count);
	// 设置子图Y坐标轴副刻度数
	DeclareFunc_2P_Noret(void, SetYMinorTickCount, int, count, int, nPlotIndex);
	// 设置Y坐标轴副刻度数
	DeclareFunc_1P_Noret(void, SetYMinorTickCount, int, count);
	// 设置子图X坐标轴浮动标记
	DeclareFunc_2P_Noret(void, SetXFloatTicks, bool, flt, int, nPlotIndex);
	// 设置X坐标轴浮动标记
	DeclareFunc_1P_Noret(void, SetXFloatTicks, bool, flt);
	// 设置子图Y坐标轴浮动标记
	DeclareFunc_2P_Noret(void, SetYFloatTicks, bool, flt, int, nPlotIndex);
	// 设置Y坐标轴浮动标记
	DeclareFunc_1P_Noret(void, SetYFloatTicks, bool, flt);
	// 设置子图X坐标轴成指数
	DeclareFunc_2P_Noret(void, SetXLogarithm, bool, bLog, int, nPlotIndex);
	// 设置X坐标轴成指数
	DeclareFunc_1P_Noret(void, SetXLogarithm, bool, bLog);
	// 设置子图Y坐标轴成指数
	DeclareFunc_2P_Noret(void, SetYLogarithm, bool, bLog, int, nPlotIndex);
	// 设置Y坐标轴成指数
	DeclareFunc_1P_Noret(void, SetYLogarithm, bool, bLog);
	// 设置子图X坐标反向绘制
	DeclareFunc_2P_Noret(void, SetXOpposite, bool, bOpposite, int, nPlotIndex);
	// 设置X坐标反向绘制
	DeclareFunc_1P_Noret(void, SetXOpposite, bool, bOpposite);
	// 设置子图Y坐标反向绘制
	DeclareFunc_2P_Noret(void, SetYOpposite, bool, bOpposite, int, nPlotIndex);
	// 设置Y坐标反向绘制
	DeclareFunc_1P_Noret(void, SetYOpposite, bool, bOpposite);
	// 设置子图坐标横轴两端是某数的倍数
	DeclareFunc_2P_Noret(void, SetAtomX, double, atom, int, nPlotIndex);
	// 设置坐标横轴两端是某数的倍数
	DeclareFunc_1P_Noret(void, SetAtomX, double, atom);
	// 设置子图坐标纵轴两端是某数的倍数
	DeclareFunc_2P_Noret(void, SetAtomY, double, atom, int, nPlotIndex);
	// 设置坐标纵轴两端是某数的倍数
	DeclareFunc_1P_Noret(void, SetAtomY, double, atom);
	// 设置子图坐标横轴起点是某数的倍数
	DeclareFunc_2P_Noret(void, SetAtomZeroX, double, zero, int, nPlotIndex);
	// 设置坐标横轴起点是某数的倍数
	DeclareFunc_1P_Noret(void, SetAtomZeroX, double, zero);
	// 设置子图坐标纵轴起点是某数的倍数
	DeclareFunc_2P_Noret(void, SetAtomZeroY, double, zero, int, nPlotIndex);
	// 设置坐标纵轴起点是某数的倍数
	DeclareFunc_1P_Noret(void, SetAtomZeroY, double, zero);
	// 在设置坐标范围是某数倍数情况下，子图坐标横轴分割份数
	DeclareFunc_2P_Noret(void, SetSegsPreferX, int, segs, int, nPlotIndex);
	// 在设置坐标范围是某数倍数情况下，坐标横轴分割份数
	DeclareFunc_1P_Noret(void, SetSegsPreferX, int, segs);
	// 在设置坐标范围是某数倍数情况下，子图坐标纵轴分割份数
	DeclareFunc_2P_Noret(void, SetSegsPreferY, int, segs, int, nPlotIndex);
	// 在设置坐标范围是某数倍数情况下，坐标纵轴分割份数
	DeclareFunc_1P_Noret(void, SetSegsPreferY, int, segs);

	// 设置坐标轴范围
	DeclareFunc_2P_Noret(void, SetPlotRange, double*, xRange, double*, yRange);
	DeclareFunc_4P_Noret(void, SetPlotRange, double, xl, double, xu, double, yl, double, yu);
	DeclareFunc_3P_Noret(void, SetXRange, double, low, double, high, int, nPlotIndex);
	DeclareFunc_2P_Noret(void, SetXRange, double, low, double, high);
	DeclareFunc_3P_Noret(void, SetYRange, double, low, double, high, int, nPlotIndex);
	DeclareFunc_2P_Noret(void, SetYRange, double, low, double, high);
	// 设置坐标轴范围自动计算
	DeclareFunc_1P_Noret(void, SetXAutoRange, bool, bAuto);
	DeclareFunc_1P_Noret(void, SetYAutoRange, bool, bAuto);
	DeclareFunc_2P_Noret(void, SetXAutoRange, bool, bAuto, int, nPlotIndex);
	DeclareFunc_2P_Noret(void, SetYAutoRange, bool, bAuto, int, nPlotIndex);
	// 设置坐标轴范围与数据范围相同，不向两端扩展
	DeclareFunc_2P_Noret(void, SetExactXRange, bool, bExact, int, nPlotIndex);
	DeclareFunc_2P_Noret(void, SetExactYRange, bool, bExact, int, nPlotIndex);
	// 设置坐标轴范围尽量与数据范围相同
	DeclareFunc_2P_Noret(void, SetOptimalExactXRange, bool, bExact, int, nPlotIndex);
	DeclareFunc_2P_Noret(void, SetOptimalExactYRange, bool, bExact, int, nPlotIndex);
	// 设置坐标轴范围尽量向两端扩展一格
	DeclareFunc_2P_Noret(void, SetOptimalXExtend, bool, bExtend, int, nPlotIndex);
	DeclareFunc_2P_Noret(void, SetOptimalYExtend, bool, bExtend, int, nPlotIndex);
	// 设置坐标轴范围一致不变
	DeclareFunc_2P_Noret(void, SetStaticRange, bool, sr, int, nPlotIndex);
	// 设置X坐标轴范围一致不变
	DeclareFunc_2P_Noret(void, SetStaticXRange, bool, sr, int, nPlotIndex);
	// 设置Y坐标轴范围一致不变
	DeclareFunc_2P_Noret(void, SetStaticYRange, bool, sr, int, nPlotIndex);
	
	// 设置坐标轴等长，只对kTypeXY有效
	DeclareFunc_1P_Noret(void, SetEqualXYAxis, bool, bEqual);
	// 设置坐标轴等范围，只对kTypeXY有效
	DeclareFunc_1P_Noret(void, SetEqualXYRange, bool, bEqual);
	// 设置坐标轴等比例尺，只对kTypeXY有效
	DeclareFunc_1P_Noret(void, SetEqualXYScale, bool, bEqual);

	// X坐标超出屏幕的点不参与计算，以优化速度
	DeclareFunc_3P_Noret(void, SetClipXData, bool, clip, int, nDataIndex, int, nPlotIndex);
	// Y坐标超出屏幕的点不参与计算，以优化速度
	DeclareFunc_3P_Noret(void, SetClipYData, bool, clip, int, nDataIndex, int, nPlotIndex);

	// 设置X时间轴后，对话框中X数据范围设置时减去一个初值，以避免数据过大
	DeclareFunc_3P_Noret(void, SetXStartTime, TCHAR *, strTime, TCHAR *, fmt, int, nPlotIndex);
	// 设置Y时间轴后，对话框中Y数据范围设置时减去一个初值，以避免数据过大
	DeclareFunc_3P_Noret(void, SetYStartTime, TCHAR *, strTime, TCHAR *, fmt, int, nPlotIndex);

	// 设置背景网格与哪一个子图绑定，用于共享X轴视图
	DeclareFunc_1P_Noret(void, SetGridBindLayer, int, nPlotIndex);
	// 检测子图的网格线
	DeclareFunc_5P_Ret(bool, GetGridLine, bool &, MajorH, bool &, MajorV, bool &, MinorH, bool &, MinorV, int, nPlotIndex);
	// 检测网格线
	DeclareFunc_4P_Ret(bool, GetGridLine, bool &, MajorH, bool &, MajorV, bool &, MinorH, bool &, MinorV);
	// 设置子图网格线的绘制
	DeclareFunc_5P_Noret(void, SetGridLine, bool, MajorH, bool, MajorV, bool, MinorH, bool, MinorV, int, nPlotIndex);
	// 设置网格线的绘制
	DeclareFunc_4P_Noret(void, SetGridLine, bool, MajorH, bool, MajorV, bool, MinorH, bool, MinorV);
	// 设置主网格线颜色
	DeclareFunc_1P_Noret(void, SetMajorGridColor, COLORREF, color);
	// 设置子图主网格线颜色
	DeclareFunc_2P_Noret(void, SetMajorGridColor, COLORREF, color, int, nPlotIndex);
	// 设置副网格线颜色
	DeclareFunc_1P_Noret(void, SetMinorGridColor, COLORREF, color);
	// 设置子图副网格线颜色
	DeclareFunc_2P_Noret(void, SetMinorGridColor, COLORREF, color, int, nPlotIndex);
	// 设置主网格线线宽
	DeclareFunc_1P_Noret(void, SetMajorGridLineSize, int, nSize);
	// 设置子图主网格线线宽
	DeclareFunc_2P_Noret(void, SetMajorGridLineSize, int, nSize, int, nPlotIndex);
	// 设置副网格线线宽
	DeclareFunc_1P_Noret(void, SetMinorGridLineSize, int, nSize);
	// 设置子图副网格线线宽
	DeclareFunc_2P_Noret(void, SetMinorGridLineSize, int, nSize, int, nPlotIndex);
	// 设置主网格线线型
	DeclareFunc_1P_Noret(void, SetMajorGridLineStyle, int, nStyle);
	// 设置子图主网格线线型
	DeclareFunc_2P_Noret(void, SetMajorGridLineStyle, int, nStyle, int, nPlotIndex);
	// 设置副网格线线型
	DeclareFunc_1P_Noret(void, SetMinorGridLineStyle, int, nStyle);
	// 设置子图副网格线线型
	DeclareFunc_2P_Noret(void, SetMinorGridLineStyle, int, nStyle, int, nPlotIndex);
	// 获取子图背景颜色
	DeclareFunc_1P_Ret(COLORREF, GetBkgndColor, int, nPlotIndex);
	// 获取背景颜色
	DeclareFunc_0P_Ret(COLORREF, GetBkgndColor);
	// 设置子图背景颜色
	DeclareFunc_2P_Noret(void, SetBkgndColor, COLORREF, color, int, nPlotIndex);
	// 设置背景颜色
	DeclareFunc_1P_Noret(void, SetBkgndColor, COLORREF, color);
	// 背景色是否渐变
	DeclareFunc_0P_Ret(bool, IsGradientBkgnd);
	// 背景色渐变
	DeclareFunc_1P_Noret(void, SetGradientBkgnd, bool, bGrad);
	
	// 设置图像边框显示
	DeclareFunc_1P_Noret(void, SetEdgeShow, bool, bShow);
	// 设置图像边框显示
	DeclareFunc_1P_Noret(void, SetEdgeRoundConor, bool, bRound);
	// 设置图像边框颜色
	DeclareFunc_1P_Noret(void, SetEdgeColor, COLORREF, color);
	// 设置图像边框宽度
	DeclareFunc_1P_Noret(void, SetEdgeWidth, int, width);

	// 设置图像左空白
	DeclareFunc_1P_Noret(void, SetMarginLeft, int, margin);
	// 设置分裂视图左空白
	DeclareFunc_2P_Noret(void, SetMarginLeft, int, margin, int, nPlotIndex);
	// 设置图像右空白
	DeclareFunc_1P_Noret(void, SetMarginRight, int, margin);
	// 设置分裂视图右空白
	DeclareFunc_2P_Noret(void, SetMarginRight, int, margin, int, nPlotIndex);
	// 设置图像上空白
	DeclareFunc_1P_Noret(void, SetMarginTop, int, margin);
	// 设置分裂视图上空白
	DeclareFunc_2P_Noret(void, SetMarginTop, int, margin, int, nPlotIndex);
	// 设置图像下空白
	DeclareFunc_1P_Noret(void, SetMarginBottom, int, margin);
	// 设置分裂视图下空白
	DeclareFunc_2P_Noret(void, SetMarginBottom, int, margin, int, nPlotIndex);
	
	// 设置数据区边框显示
	DeclareFunc_1P_Noret(void, SetBorderShow, bool, bShow);
	// 设置数据区边框颜色
	DeclareFunc_1P_Noret(void, SetBorderColor, COLORREF, color);
	// 设置数据区边框宽度
	DeclareFunc_1P_Noret(void, SetBorderWidth, int, width);
	
	// 检测是否使用图例
	DeclareFunc_1P_Ret(bool, IsLegendShow, int, nPlotIndex);
	// 设置是否使用图例
	DeclareFunc_2P_Noret(void, SetLegendShow, bool, bUse, int, nPlotIndex);
	// 设置图例的最大行数
	DeclareFunc_2P_Noret(void, SetLegendMaxRows, int, nRows, int, nPlotIndex);
	// 设置每行显示的图例数
	DeclareFunc_2P_Noret(void, SetLegendMaxCols, int, nCols, int, nPlotIndex);
	// 设置图例位置
	DeclareFunc_2P_Noret(void, SetLegendPosition, int, nPos, int, nPlotIndex);
	// 设置图例字体
	DeclareFunc_2P_Noret(void, SetLegendFont, LOGFONT, logFont, int, nPlotIndex);
	// 设置图例字体高度
	DeclareFunc_2P_Noret(void, SetLegendFontHeight, int, nHeight, int, nPlotIndex);
	
	// 在分层视图和分裂视图中，设置第一层显示左坐标轴，第二层显示右坐标轴
	DeclareFunc_0P_Noret(void, SetLRAxis);
	// 在分层视图中，设置第一层显示下坐标轴，第二层显示上坐标轴
	DeclareFunc_0P_Noret(void, SetTBAxis);
	// 底层函数，获取子图的绘图区域
	DeclareFunc_1P_Ret(RECT, GetLastClientRect, int, nPlotIndex);
	// 底层函数，获取绘图区域
	DeclareFunc_0P_Ret(RECT, GetLastClientRect);
	// 底层函数，获取子图的绘图区域
	DeclareFunc_1P_Ret(RECT, GetLastPlotRect, int, nPlotIndex);
	// 底层函数，获取绘图区域
	DeclareFunc_0P_Ret(RECT, GetLastPlotRect);
	// 底层函数，获取坐标轴的范围
	DeclareFunc_3P_Noret(void, GetLastPlotRange, double*, xRange, double*, yRange, int, nPlotIndex);
	// 底层函数，获取数据的范围
	DeclareFunc_3P_Noret(void, GetLastDataRange, double*, xRange, double*, yRange, int, nPlotIndex);
	// 底层函数，根据屏幕坐标计算数据坐标
	DeclareFunc_3P_Noret(void, ClientToData, POINT *, point, double *, data, int, nPlotIndex);
	// 底层函数，根据数据坐标计算屏幕坐标
	DeclareFunc_3P_Noret(void, DataToClient, double *, data, POINT *, point, int, nPlotIndex);

	// 设置三维曲线的网格
	DeclareFunc_6P_Noret(void, SetGridLine, bool, MajorXY, bool, MinorXY, bool, MajorYZ, bool, MinorYZ, bool, MajorZX, bool, MinorZX);
	// 设置XY屏幕主网格
	DeclareFunc_1P_Noret(void, SetShowXYMajorGrid, bool, show );
	// 设置YZ屏幕主网格
	DeclareFunc_1P_Noret(void, SetShowYZMajorGrid, bool, show );
	// 设置ZX屏幕主网格
	DeclareFunc_1P_Noret(void, SetShowZXMajorGrid, bool, show );
	// 设置XY屏幕副网格
	DeclareFunc_1P_Noret(void, SetShowXYMinorGrid, bool, show );
	// 设置YZ屏幕副网格
	DeclareFunc_1P_Noret(void, SetShowYZMinorGrid, bool, show );
	// 设置ZX屏幕副网格
	DeclareFunc_1P_Noret(void, SetShowZXMinorGrid, bool, show );
	
	// 保存子图数据到文件
	DeclareFunc_2P_Ret(bool, WriteToFile, TCHAR *, pathName, int, nPlotIndex);
	// 保存子图某曲线数据到文件
	DeclareFunc_3P_Ret(bool, WriteOneToFile, TCHAR *, pathName, int, nDataIndex, int, nPlotIndex);
	
		
	// 获得内部特殊指针，此指针一般没用，只用于在某些情况下传递参数，例如线程函数
	DeclareFunc_0P_Ret(void	*, GetUserPointer);
	// 设置内部特殊指针
	DeclareFunc_1P_Noret(void, SetUserPointer, void *, pUser);
	// 获得内部特殊标志状态，特殊用途
	DeclareFunc_0P_Ret(bool, GetUserFlag);
	// 设置内部特殊标志状态，特殊用途
	DeclareFunc_1P_Noret(void, SetUserFlag, bool, flag);

}

