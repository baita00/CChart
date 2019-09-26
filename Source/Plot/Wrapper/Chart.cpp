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
#include "../PlotHeaders.h"
using namespace NsCChart;

#include <windowsx.h>
#include <commctrl.h>
#include <list>

#include "mmsystem.h"  //head file
#pragma comment(lib,"winmm")  //lib file

#define WM_MOUSEWHEEL 0x020A
#define WHEEL_DELTA                     120
#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#define GET_KEYSTATE_WPARAM(wParam)     (LOWORD(wParam))

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


#define PrefixT(A) (\
	(m_pPara->nType==kTypeXY)?A ##(CXYPlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kTypePie)?A ##(CPiePlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kTypeStem)?A ##(CStemPlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kTypeOscillo)?A ##(CFixAxisPlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kTypeContourLine)?A ##(CContourLinePlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kTypeContourMap)?A ##(CCloudPlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kTypeContour)?A ##(CContourPlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kTypeXY3D)?A ##(CXY3DPlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kTypeSplit)?A ##(CSplitPlotImplI<CXYPlotImplI> *)(m_pPara->pPlot):(\
	(m_pPara->nType==kTypeShareX)?A ##(CShareXPlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kTypeLayered)?A ##(CLayeredPlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kType3DLine)?A ##(CTDLinePlotImplI *)(m_pPara->pPlot):(\
	(m_pPara->nType==kType3DSurface)?A ##(CTDSurfacePlotImplI *)(m_pPara->pPlot):\
	0\
	)))))))))))))

#define SuffixT_S(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourLine)?((CContourLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourMap)?((CCloudPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContour)?((CContourPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:\
	0\
	))))))))

#define SuffixT_S_Def(A, D) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourLine)?((CContourLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourMap)?((CCloudPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContour)?((CContourPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:\
	D\
	))))))))

#define SuffixT_S_NF(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:\
	0\
	)))))

#define SuffixT_S_NF_N3D(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:\
	0\
	))))

#define SuffixT_S_L(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:\
	0\
	)))
#define SuffixT_S_L_Def(A, D) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:\
	D\
	)))

#define SuffixT_S_F(A) (\
	(m_pPara->nType==kTypeContourLine)?((CContourLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourMap)?((CCloudPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContour)?((CContourPlotImplI *)m_pPara->pPlot)##A:\
	0\
	)))

#define SuffixT_M(A) (\
	(m_pPara->nType==kTypeSplit)?((CSplitPlotImplI<CXYPlotImplI> *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:\
	0\
	)))

#define SuffixT_M_Def(A, D) (\
	(m_pPara->nType==kTypeSplit)?((CSplitPlotImplI<CXYPlotImplI> *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:\
	D\
	)))

#define SuffixT_A(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourLine)?((CContourLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourMap)?((CCloudPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContour)?((CContourPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeSplit)?((CSplitPlotImplI<CXYPlotImplI> *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DLine)?((CTDLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DSurface)?((CTDSurfacePlotImplI *)m_pPara->pPlot)##A:\
	0\
	)))))))))))))

#define SuffixT_A_NR3D(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourLine)?((CContourLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourMap)?((CCloudPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContour)?((CContourPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeSplit)?((CSplitPlotImplI<CXYPlotImplI> *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:\
	0\
	)))))))))))

#define SuffixT_A_NF(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeSplit)?((CSplitPlotImplI<CXYPlotImplI> *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DLine)?((CTDLinePlotImplI *)m_pPara->pPlot)##A:\
	0\
	)))))))))

#define SuffixT_A_NF_NR3D(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeSplit)?((CSplitPlotImplI<CXYPlotImplI> *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:\
	0\
	))))))))

#define SuffixT_A_NS(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourLine)?((CContourLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourMap)?((CCloudPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContour)?((CContourPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DLine)?((CTDLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DSurface)?((CTDSurfacePlotImplI *)m_pPara->pPlot)##A:\
	0\
	))))))))))))

#define SuffixT_A_NS_NF(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DLine)?((CTDLinePlotImplI *)m_pPara->pPlot)##A:\
	0\
	))))))))

#define SuffixT_A_NS_NF_NR3D(A) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:\
	0\
	)))))))

#define SuffixT_A_Def(A, D) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourLine)?((CContourLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourMap)?((CCloudPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContour)?((CContourPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeSplit)?((CSplitPlotImplI<CXYPlotImplI> *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DLine)?((CTDLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DSurface)?((CTDSurfacePlotImplI *)m_pPara->pPlot)##A:\
	D\
	)))))))))))))

#define SuffixT_A_NR3D_Def(A, D) (\
	(m_pPara->nType==kTypeXY)?((CXYPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypePie)?((CPiePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeStem)?((CStemPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeOscillo)?((CFixAxisPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourLine)?((CContourLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourMap)?((CCloudPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContour)?((CContourPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeSplit)?((CSplitPlotImplI<CXYPlotImplI> *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeShareX)?((CShareXPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeLayered)?((CLayeredPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:\
	D\
	)))))))))))

#define SuffixT_3D(A) (\
	(m_pPara->nType==kTypeXY3D)?((CXY3DPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DLine)?((CTDLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DSurface)?((CTDSurfacePlotImplI *)m_pPara->pPlot)##A:\
	0\
	)))

#define SuffixT_R3D(A) (\
	(m_pPara->nType==kType3DLine)?((CTDLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DSurface)?((CTDSurfacePlotImplI *)m_pPara->pPlot)##A:\
	0\
	))

#define SuffixT_R3D_F(A) (\
	(m_pPara->nType==kType3DSurface)?((CTDSurfacePlotImplI *)m_pPara->pPlot)##A:\
	0\
	)

#define SuffixT_R3D_L(A) (\
	(m_pPara->nType==kType3DLine)?((CTDLinePlotImplI *)m_pPara->pPlot)##A:\
	0\
	)

#define SuffixT_F(A) (\
	(m_pPara->nType==kTypeContourLine)?((CContourLinePlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContourMap)?((CCloudPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kTypeContour)?((CContourPlotImplI *)m_pPara->pPlot)##A:(\
	(m_pPara->nType==kType3DSurface)?((CTDSurfacePlotImplI *)m_pPara->pPlot)##A:\
	0\
	))))

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

namespace NsCChart{

bool			IsSingleLayer(int nType)
{
	if(nType>=0 && nType<kTypeSingleLayerCount)
		return true;
	else
		return false;
}

bool			IsSingleLineLayer(int nType)
{
	if(nType==kTypeXY || nType==kTypeOscillo || nType == kTypeXY3D)
		return true;
	else
		return false;
}

bool			IsMultiLayer(int nType)
{
	if(nType>=kTypeSingleLayerCount && nType<kType2DCount)
		return true;
	else
		return false;
}

bool			Is2DFieldChart(int nType)
{
	if(nType==kTypeContourLine || nType==kTypeContourMap || nType==kTypeContour)
		return true;
	else
		return false;
}

bool			IsFieldChart(int nType)
{
	if(nType==kTypeContourLine || nType==kTypeContourMap || nType==kTypeContour || nType == kType3DSurface)
		return true;
	else
		return false;
}

bool			IsReal3DChart(int nType)
{
	if(nType == kType3DLine || nType == kType3DSurface)
		return true;
	else
		return false;
}

bool			IsPseudo3DChart(int nType)
{
	if(nType == kTypeXY3D)
		return true;
	else
		return false;
}

bool			Is3DChart(int nType)
{
	return IsReal3DChart(nType) || IsPseudo3DChart(nType);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

struct stChartEnv 
{
	int			nID;
	HWND		hWnd;
	bool		bRectConfined;
	RECT		rectConfine;
	COLORREF	crBkgnd;
	bool		bEdgeShow;
};

struct stMsgSwitch
{
	bool				bReact;
	bool				bReactLButtonDown;
	bool				bReactLButtonUp;
	bool				bReactLButtonDblClk;
	bool				bReactMouseMove;
	bool				bReactContextMenu;
	bool				bReactKeyDown;

	// 消息响应的自动刷新,用于末尾R的消息响应函数
	bool				bAutoRedraw;
};

struct stZoomFact 
{
	bool				bZoomMode;
	double				fZoomFactorX;
	double				fZoomFactorY;
	int					nZoomCenterMode;
	POINT				ptZoomCenter;
};

struct CChart::stChartPara
{
	void				*pPlot;
	int					nType;
	// 指向父类CChart的指针
	CChart				*pChart;

	// 消息开关
	stMsgSwitch			MsgSwitch;
	// 缩放参数
	stZoomFact			ZoomFact;

	// 绘图区偏移
	SIZE				sizeOffsetClient;
	
	// 用户自定义
	void				*pUser;
	bool				bUser;
};

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

stChartEnv g_ChartEnv;

void	SaveChartEnv(CChart *pChart)
{
	if(!pChart)return;

	g_ChartEnv.nID = pChart->GetPlotID();
	g_ChartEnv.hWnd = pChart->GetWnd();
	g_ChartEnv.bRectConfined = pChart->IsRectConfined();
	g_ChartEnv.rectConfine = pChart->GetConfineRect();
	g_ChartEnv.crBkgnd = pChart->GetBkgndColor();
	g_ChartEnv.bEdgeShow = pChart->IsEdgeShow();
}

void	RestoreChartEnv(CChart *pChart)
{
	if(!pChart)return;
	pChart->SetPlotID(g_ChartEnv.nID);
	pChart->SetWnd(g_ChartEnv.hWnd);
	pChart->SetRectConfined(g_ChartEnv.bRectConfined);
	pChart->SetConfineRect(g_ChartEnv.rectConfine);
	pChart->SetBkgndColor(g_ChartEnv.crBkgnd);
	pChart->SetEdgeShow(g_ChartEnv.bEdgeShow);
}
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

CChart::CChart()
{
	//static int nChartCount=0;
	CGlobal::m_bAddDefAxes = true;
	m_pPara = new stChartPara;
	
	m_pPara->nType = kTypeXY;
	m_pPara->pPlot = new CXYPlotImplI;
	
	m_pPara->pChart = this;

	m_pPara->MsgSwitch.bReact = true;
	m_pPara->MsgSwitch.bReactLButtonDown = true;
	m_pPara->MsgSwitch.bReactLButtonUp = true;
	m_pPara->MsgSwitch.bReactLButtonDblClk = true;
	m_pPara->MsgSwitch.bReactMouseMove = true;
	m_pPara->MsgSwitch.bReactContextMenu = true;
	m_pPara->MsgSwitch.bReactKeyDown = true;
	m_pPara->MsgSwitch.bAutoRedraw = true;

	m_pPara->ZoomFact.bZoomMode = false;
	m_pPara->ZoomFact.fZoomFactorX = 1.0;
	m_pPara->ZoomFact.fZoomFactorY = 1.0;
	m_pPara->ZoomFact.nZoomCenterMode = kZoomCenterARB;
	m_pPara->ZoomFact.ptZoomCenter.x = m_pPara->ZoomFact.ptZoomCenter.y = 0;

	m_pPara->sizeOffsetClient.cx = 0;
	m_pPara->sizeOffsetClient.cy = 0;

	m_pPara->pUser = 0;
	m_pPara->bUser = false;
	
	//nChartCount++;

	CGlobal::InitGdiPlus();
}

CChart::~CChart()
{
	Release();
	
	delete m_pPara;
	m_pPara = 0;

	CGlobal::FreeGdiPlus();
}

CChart::stChartPara	*CChart::GetPara()
{
	return m_pPara;
}

void	CChart::Release()
{
	if(!m_pPara)return;
	
	if(m_pPara->pPlot)
	{
		PrefixT(delete);
		
		m_pPara->pPlot = 0;
		m_pPara->nType = -1;
	}
}

void	CChart::SetLangurage(int nLang)
{
	if(nLang<0 || nLang>=kLangCount)
		return;
	CGlobal::m_nLangurage = nLang;
}

void		CChart::SetDataProcessing(bool process)
{
	CGlobal::m_bShowDataProcessingMenu = process;
}

bool	CChart::IsEnableToolTip()
{
	return CGlobal::m_bEnableToolTip;
}

void	CChart::SetEnableToolTip(bool enable)
{
	CGlobal::m_bEnableToolTip = enable;
}

bool	CChart::CheckPlotType(int nType)
{
	if(nType<0 || nType>=kTypeCount)
		return false;
	else
		return true;
}

void	CChart::SetColorTable(int nTableIndex)
{
	CPlot::SetColorTable(nTableIndex);
}

void	CChart::SetImagePopupMenu(bool bShow)
{
	CGlobal::m_bImagePopupMenu = bShow;
}

void	CChart::CopyImageToClipBoard(HWND hWnd)
{
	OnDraw(hWnd);
	CopyImageToClipBrd(hWnd);
}

void	CChart::SaveImageToFile(HWND hWnd, TCHAR *strFileName, TCHAR *strFileExt)
{
	OnDraw(hWnd);
	HBITMAP hBm = HBitmapFromWnd(hWnd);
	if(!hBm)return;
	SaveHBitmapToFile(hBm, strFileName, strFileExt);
	DeleteObject(hBm);
}

void	CChart::PrintImage(HWND hWnd)
{
	ShowPrintDialog(hWnd, false);
}

void	*CChart::GetPlot()
{
	if(!m_pPara)return 0;
	return m_pPara->pPlot;
}

int		CChart::GetType()
{
	if(!m_pPara)return -1;
	return m_pPara->nType;
}

bool	CChart::SetType(int nType)
{
	return CreateChart(nType);
}

bool	CChart::SetType(int nType, HWND hWnd)
{
	return CreateChart(nType, hWnd);
}

bool	CChart::CreateChart(int nType)
{
	if(!m_pPara)return false;
	if(nType<0 || nType>=kTypeCount)
		return false;
	
	if(m_pPara->nType == nType)
		return true;

	HWND hWnd = GetWnd();

	Release();

	m_pPara->nType = nType;

	switch(nType)
	{
	case kTypeXY:
		m_pPara->pPlot = new CXYPlotImplI;
		break;
	case kTypePie:
		m_pPara->pPlot = new CPiePlotImplI;
		break;
	case kTypeStem:
		m_pPara->pPlot = new CStemPlotImplI;
		break;
	case kTypeOscillo:
		m_pPara->pPlot = new CFixAxisPlotImplI;
		break;
	case kTypeContourLine:
		m_pPara->pPlot = new CContourLinePlotImplI;
		break;
	case kTypeContourMap:
		m_pPara->pPlot = new CCloudPlotImplI;
		break;
	case kTypeContour:
		m_pPara->pPlot = new CContourPlotImplI;
		break;
	case kTypeSplit:
		m_pPara->pPlot = new CSplitPlotImplI<CXYPlotImplI>;
		break;
	case kTypeShareX:
		m_pPara->pPlot = new CShareXPlotImplI;
		break;
	case kTypeLayered:
		m_pPara->pPlot = new CLayeredPlotImplI;
		break;
	case kTypeXY3D:
		m_pPara->pPlot = new CXY3DPlotImplI;
		break;
	case kType3DLine:
	case kType3DSurface:
		return CreateChart(nType, hWnd);
	default:
		return false;
	}

	if(hWnd)SetWnd(hWnd);
	
	return true;
}

bool	CChart::CreateChart(int nType, HWND hWnd)
{
	if(!m_pPara)return false;
	if(nType<0 || nType>=kTypeCount)
		return false;
	
	if(nType>=kTypeXY && nType<kType2DCount)
	{
		CreateChart(nType);
		return true;
	}
	
	Release();
	switch(nType)
	{
	case kType3DLine:
		m_pPara->pPlot = new CTDLinePlotImplI(hWnd);
		break;
	case kType3DSurface:
		m_pPara->pPlot = new CTDSurfacePlotImplI(hWnd);
		break;
	default:
		return false;
	}
	m_pPara->nType = nType;
	//m_pPara->hWnd = hWnd;
	SuffixT_A(->SetWnd(hWnd));
	return true;
}

int		CChart::GetPlotID()
{
	if(!m_pPara)return false;
	//return m_pPara->hWnd;
	return SuffixT_A_Def(->GetPlotID(), 0);
}

void	CChart::SetPlotID(int nID)
{
	if(!m_pPara)return;
	//m_pPara->hWnd = hWnd;
	SuffixT_A(->SetPlotID(nID));
}

HWND	CChart::GetWnd()
{
	if(!m_pPara)return false;
	//return m_pPara->hWnd;
	return SuffixT_A_Def(->GetWnd(), 0);
}

void	CChart::SetWnd(HWND hWnd)
{
	if(!m_pPara)return;
	//m_pPara->hWnd = hWnd;
	SuffixT_A(->SetWnd(hWnd));
}

bool	CChart::IsRectConfined()
{
	if(!m_pPara)return false;
	//return m_pPara->bRectConfined;
	return SuffixT_A_Def(->IsRectConfined(), false);
}

void	CChart::SetRectConfined(bool confine)
{
	if(!m_pPara)return;
	//m_pPara->bRectConfined = confine;
	SuffixT_A(->SetRectConfined(confine));
}

bool	CChart::SetConfineRect(RECT ConfineRect)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount)
		return false;
	if(!m_pPara->pPlot)
		return false;
	
	//m_pPara->rctConfined = ConfineRect;
	SuffixT_A(->SetConfineRect(ConfineRect));
	//m_pPara->bRectConfined = true;
	//SuffixT_A(->SetRectConfined(true));
	
	return true;
}

RECT	CChart::GetConfineRect()
{
	//return m_pPara->rctConfined;
	RECT rct={0, 0, 0, 0};
	return SuffixT_A_Def(->GetConfineRect(), rct);
}

bool	CChart::IsEmpty()
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return true;

	return SuffixT_S_NF(->IsEmpty());
}


void	CChart::ResetApperance()
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->ResetApperance());
}

int		CChart::GetApperanceMode()
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;

	return SuffixT_A_NR3D(->GetApperanceMode());
}

void	CChart::SetApperanceMode(int mode)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	SuffixT_A_NR3D(->SetApperanceMode(mode));
}

void	CChart::SetContinuePaint(bool ctn)
{
	CGlobal::m_bContinuePaint = ctn;
}

void	CChart::SetContinueReact(bool ctn)
{
	CGlobal::m_bContinueReact = ctn;
}

bool	CChart::IsReact()
{
	return m_pPara->MsgSwitch.bReact;
}

void	CChart::SetReact(bool react)
{
	if(!m_pPara)return;
	m_pPara->MsgSwitch.bReact = react;
}

bool	CChart::IsAutoRedraw()
{
	return m_pPara->MsgSwitch.bAutoRedraw;
}

void	CChart::SetAutoRedraw(bool redraw)
{
	if(!m_pPara)return;
	m_pPara->MsgSwitch.bAutoRedraw = redraw;
}

void	CChart::SetReactLButtonDown(bool react)
{
	if(!m_pPara)return;
	m_pPara->MsgSwitch.bReactLButtonDown = react;
}

void	CChart::SetReactLButtonUp(bool react)
{
	if(!m_pPara)return;
	m_pPara->MsgSwitch.bReactLButtonUp = react;
}

void	CChart::SetReactLButtonDblClk(bool react)
{
	if(!m_pPara)return;
	m_pPara->MsgSwitch.bReactLButtonDblClk = react;
}

void	CChart::SetReactMouseMove(bool react)
{
	if(!m_pPara)return;
	m_pPara->MsgSwitch.bReactMouseMove = react;
}

void	CChart::SetReactContextMenu(bool react)
{
	if(!m_pPara)return;
	m_pPara->MsgSwitch.bReactContextMenu = react;
}

void	CChart::SetReactKeyDown(bool react)
{
	if(!m_pPara)return;
	m_pPara->MsgSwitch.bReactKeyDown = react;
}

void	CChart::SetReactTitle(bool react)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(m_pPara->nType==kTypeSplit)
	{
		int i, cnt;
		cnt = ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlotCount();
		for(i=0; i<cnt; i++)
		{
			((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(i)->SetReactTitle(react);
		}
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->SetReactTitle(react);
	}
	else
	{
		SuffixT_A_NS_NF_NR3D(->SetReactTitle(react));
	}
}

void	CChart::SetReactAxes(bool react)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(m_pPara->nType==kTypeSplit)
	{
		int i, cnt;
		cnt = ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlotCount();
		for(i=0; i<cnt; i++)
		{
			((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(i)->SetReactAxes(react);
		}
	}
	else
	{
		SuffixT_A_NS_NF_NR3D(->SetReactAxes(react));
	}
}

void	CChart::SetReactLegend(bool react)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType==kTypeSplit)
	{
		int i, cnt;
		cnt = ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlotCount();
		for(i=0; i<cnt; i++)
		{
			((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(i)->SetReactLegend(react);
		}
	}
	else
	{
		SuffixT_A_NS_NF_NR3D(->SetReactLegend(react));
	}
}

void	CChart::SetOffsetClient(int offsetX, int offsetY)
{
	if(!m_pPara)return;
	m_pPara->sizeOffsetClient.cx = offsetX;
	m_pPara->sizeOffsetClient.cy = offsetY;
}

void	CChart::OnDraw()
{
	HWND hWnd = GetWnd();
	if(hWnd)OnDraw(hWnd);
}

void	CChart::OnDraw(HWND hWnd)
{
	RECT rt;
	GetClientRect(hWnd, &rt);
	OnDraw(hWnd, rt);
}

void	CChart::OnDraw(HDC hDC)
{
	HWND hWnd = WindowFromDC(hDC);
	if(!hWnd)return;
	RECT rt;
	GetClientRect(hWnd, &rt);
	OnDraw(hDC, rt);
}

void	CChart::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CChart::OnDraw(HDC hDC, RECT destRect)
{
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	RECT finalRect;
	RECT NoZoomRect = destRect;
	//if(m_pPara->bRectConfined)
	if(IsRectConfined())
	{
		//NoZoomRect = m_pPara->rctConfined;
		NoZoomRect = GetConfineRect();
	}
	// deal with offset
	if(m_pPara->sizeOffsetClient.cx !=0 || m_pPara->sizeOffsetClient.cy != 0)
	{
		OffsetRect(&NoZoomRect, m_pPara->sizeOffsetClient.cx, m_pPara->sizeOffsetClient.cy);
	}
	
	// deal with window scroll
	int xscroll, yscroll;
	HWND hWnd = WindowFromDC(hDC);
	if(hWnd)
	{
		xscroll = GetScrollPos(hWnd, SB_HORZ);
		yscroll = GetScrollPos(hWnd, SB_VERT);
		if(xscroll != 0 || yscroll != 0)
		{
			OffsetRect(&NoZoomRect, -xscroll, -yscroll);
		}
	}
	
	// deal with zoom
	finalRect = NoZoomRect;
	if(m_pPara->ZoomFact.bZoomMode)
	{
		finalRect = GetZoomedRect(NoZoomRect);
		EraseRectGap(hDC, NoZoomRect, finalRect, GetBkgndColor());
	}

	SuffixT_A(->OnDraw(hDC, finalRect));
}

bool	CChart::OnLButtonDown( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	
	if(!m_pPara->MsgSwitch.bReact)return false;
	if(!m_pPara->MsgSwitch.bReactLButtonDown)return false;
	
	bool bDraw = false;

	HDC hDC = GetDC(hWnd); 
	bDraw = SuffixT_A(->OnLButtonDown(hDC, point, ctrlKey));
	ReleaseDC(hWnd, hDC);

	return bDraw;
}

bool	CChart::OnLButtonUp( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	if(!m_pPara->MsgSwitch.bReact)return false;
	if(!m_pPara->MsgSwitch.bReactLButtonUp)return false;
	
	bool bDraw = false;

	HDC hDC = GetDC(hWnd); 
	bDraw = SuffixT_A(->OnLButtonUp(hDC, point, ctrlKey));
	ReleaseDC(hWnd, hDC);

	return bDraw;
}

bool	CChart::OnLButtonDblClk( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	if(!m_pPara->MsgSwitch.bReact)return false;
	if(!m_pPara->MsgSwitch.bReactLButtonDblClk)return false;
	
	bool bDraw = false;

	HDC hDC = GetDC(hWnd); 
	bDraw = SuffixT_A(->OnLButtonDblClk(hDC, point, ctrlKey));
	ReleaseDC(hWnd, hDC);

	return bDraw;
}

bool	CChart::OnMouseMove( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	if(!m_pPara->MsgSwitch.bReact)return false;
	if(!m_pPara->MsgSwitch.bReactMouseMove)return false;
	
	bool bDraw = false;
	
	HDC hDC = GetDC(hWnd); 
	bDraw = SuffixT_A(->OnMouseMove(hDC, point, ctrlKey));
	ReleaseDC(hWnd, hDC);

	return bDraw;
}

bool	CChart::OnMouseLeave( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	
	if(!m_pPara->MsgSwitch.bReact)return false;
	if(!m_pPara->MsgSwitch.bReactMouseMove)return false;
	
	bool bDraw = false;
	
	HDC hDC = GetDC(hWnd); 
	bDraw = SuffixT_A(->OnMouseLeave(hDC, point, ctrlKey));
	ReleaseDC(hWnd, hDC);
	
	return bDraw;
}

bool	CChart::OnMouseWheel( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(!m_pPara)return false;
	if(!m_pPara->ZoomFact.bZoomMode)
		return false;

	int		fwKeys = GET_KEYSTATE_WPARAM(ctrlKey);
	short	zDelta = GET_WHEEL_DELTA_WPARAM(ctrlKey);
	if(zDelta > 0)
	{
		m_pPara->ZoomFact.fZoomFactorX *= 1.024;
		m_pPara->ZoomFact.fZoomFactorY *= 1.024;
	}
	else
	{
		if(m_pPara->ZoomFact.fZoomFactorX>0.2)m_pPara->ZoomFact.fZoomFactorX *= 0.9765625;
		if(m_pPara->ZoomFact.fZoomFactorY>0.2)m_pPara->ZoomFact.fZoomFactorY *= 0.9765625;
	}
	ScreenToClient(hWnd, &point);
	m_pPara->ZoomFact.ptZoomCenter = point;

	return true;
}

bool	CChart::OnContextMenu( HMENU hMenu, HWND hWnd, POINT point )
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	if(!m_pPara->MsgSwitch.bReact)return false;
	if(!m_pPara->MsgSwitch.bReactContextMenu)return false;
	
	bool bDraw = false;
	
	HDC hDC = GetDC(hWnd); 
	bDraw = SuffixT_A(->OnContextMenu(hMenu, hDC, point));
	ReleaseDC(hWnd, hDC);

	return bDraw;
}

bool	CChart::OnKeyDown( HWND hWnd, UINT key )
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	if(!m_pPara->MsgSwitch.bReact)return false;
	if(!m_pPara->MsgSwitch.bReactKeyDown)return false;
	
	bool bDraw = false;

	HDC hDC = GetDC(hWnd); 
	bDraw = SuffixT_A(->OnKeyDown(hDC, key));
	ReleaseDC(hWnd, hDC);

	return bDraw;
}

void	CChart::OnLButtonDownR( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(OnLButtonDown( hWnd, point, ctrlKey ))
	{
		if(m_pPara->MsgSwitch.bAutoRedraw)OnDraw(hWnd);
	}
}
void	CChart::OnLButtonUpR( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(OnLButtonUp( hWnd, point, ctrlKey ))
	{
		if(m_pPara->MsgSwitch.bAutoRedraw)OnDraw(hWnd);
	}
}
void	CChart::OnLButtonDblClkR( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(OnLButtonDblClk( hWnd, point, ctrlKey ))
	{
		if(m_pPara->MsgSwitch.bAutoRedraw)OnDraw(hWnd);
	}
}
void	CChart::OnMouseMoveR( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(OnMouseMove( hWnd, point, ctrlKey ))
	{
		if(m_pPara->MsgSwitch.bAutoRedraw)OnDraw(hWnd);
	}
}
void	CChart::OnMouseLeaveR( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(OnMouseLeave( hWnd, point, ctrlKey ))
	{
		if(m_pPara->MsgSwitch.bAutoRedraw)OnDraw(hWnd);
	}
}
void	CChart::OnMouseWheelR( HWND hWnd, POINT point, UINT ctrlKey )
{
	if(OnMouseWheel( hWnd, point, ctrlKey ))
	{
		if(m_pPara->MsgSwitch.bAutoRedraw)OnDraw(hWnd);
	}
}
void	CChart::OnContextMenuR( HMENU hMenu, HWND hWnd, POINT point )
{
	if(OnContextMenu( hMenu, hWnd, point ))
	{
		if(m_pPara->MsgSwitch.bAutoRedraw)OnDraw(hWnd);
	}
}
void	CChart::OnKeyDownR( HWND hWnd, UINT key )
{
	if(OnKeyDown( hWnd, key ))
	{
		if(m_pPara->MsgSwitch.bAutoRedraw)OnDraw(hWnd);
	}
}

bool	CChart::OnEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(!m_pPara)return false;
	if(!m_pPara->MsgSwitch.bReact)return false;

	POINT	point;
	point.x = GET_X_LPARAM(lParam);
	point.y = GET_Y_LPARAM(lParam);
	UINT	ctrlKey = (UINT)wParam;
	UINT	key = (UINT)wParam;
	switch(message)
	{
	case WM_LBUTTONDOWN:
		return OnLButtonDown(hWnd, point, ctrlKey);
	case WM_LBUTTONUP:
		return OnLButtonUp(hWnd, point, ctrlKey);
	case WM_LBUTTONDBLCLK:
		return OnLButtonDblClk(hWnd, point, ctrlKey);
	case WM_MOUSEMOVE:
		return OnMouseMove(hWnd, point, ctrlKey);
	case WM_MOUSELEAVE:
		return OnMouseLeave(hWnd, point, ctrlKey);
	case WM_MOUSEWHEEL:
		return OnMouseWheel(hWnd, point, ctrlKey);
	case WM_CONTEXTMENU:
		return OnContextMenu(NULL, hWnd, point);
	case WM_KEYDOWN:
		return OnKeyDown(hWnd, key);
	default:
		break;
	}
	return false;
}

void	CChart::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	if(OnEvent(hWnd, message, wParam, lParam))
	{
		if(m_pPara->MsgSwitch.bAutoRedraw)OnDraw(hWnd); 
	}
}

void	CChart::SetPluginMenu(void	(*fcnAddPluginMenu)(void *plot, HMENU addMenu, void *pPara ), int (*fcnInterpretPluginMenu)(void *plot, HWND hWnd, int result, void *pPara ), void *pParaAdd, void *pParaInt)
{
	if(!fcnAddPluginMenu || !fcnInterpretPluginMenu)
		return;

	SuffixT_A(->m_pFcnAddPluginMenu = (FcnAddPluginPopupMenu *)fcnAddPluginMenu);
	SuffixT_A(->m_pFcnInterpretPluginMenu = (FcnInterpretPluginPopupMenu *)fcnInterpretPluginMenu);
	SuffixT_A(->m_pParaAddPlugInMenu = pParaAdd);
	SuffixT_A(->m_pParaInterpretPlugInMenu = pParaInt); 
}

void	CChart::SetDefMenus(bool bShow)
{
	SuffixT_A(->m_bContextMenu = bShow);
}

void	CChart::SetUserDrawingFunc(void	(*fcnUserDrawing)(void *plot, HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	SuffixT_A_NR3D(->SetFcnUserDrawing((FcnUserDrawing*)fcnUserDrawing));
	SuffixT_A_NR3D(->SetParaUserDrawing(pPara));
}

void	CChart::SetUserDrawingFunc(void	(*fcnUserDrawing)(void *plot, HDC hDC, RECT clientRect, RECT plotRect, void *pPara ), void *pPara, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
		SetUserDrawingFunc(fcnUserDrawing, pPara);
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetFcnUserDrawing((FcnUserDrawing*)fcnUserDrawing));
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetParaUserDrawing(pPara));
	}
}

void		CChart::SetUserUpdateFunc(void	(*fcnUserUpdate)( void *plot, void *pPara ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnUserUpdate((FcnUserUpdate*)fcnUserUpdate));
	SuffixT_A_NR3D(->SetParaUserUpdate(pPara));
}

void		CChart::SetUserUpdateFunc(void	(*fcnUserUpdate)( void *plot, void *pPara ), void *pPara, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
		SetUserUpdateFunc(fcnUserUpdate, pPara);
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetFcnUserUpdate((FcnUserUpdate*)fcnUserUpdate));
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetParaUserUpdate(pPara));
	}
}

void	CChart::SetPreMouseMoveFunc(bool (*fcnPreMouseMove)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnPreMouseMove(fcnPreMouseMove));
	SuffixT_A_NR3D(->SetParaPreMouseMove(pPara));
}

void	CChart::SetPreLButtonDownFunc(bool (*fcnPreLButtonDown)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnPreLButtonDown(fcnPreLButtonDown));
	SuffixT_A_NR3D(->SetParaPreLButtonDown(pPara));
}

void	CChart::SetPreKeyDownFunc(bool (*fcnPreKeyDown)( void *plot, HDC hDC, UINT key, void *para, bool &bContinue ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnPreKeyDown(fcnPreKeyDown));
	SuffixT_A_NR3D(->SetParaPreKeyDown(pPara));
}

void	CChart::SetPreLButtonUpFunc(bool (*fcnPreLButtonUp)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnPreLButtonUp(fcnPreLButtonUp));
	SuffixT_A_NR3D(->SetParaPreLButtonUp(pPara));
}

void	CChart::SetPreLButtonDblClkFunc(bool (*fcnPreLButtonDblClk)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnPreLButtonDblClk(fcnPreLButtonDblClk));
	SuffixT_A_NR3D(->SetParaPreLButtonDblClk(pPara));
}

void	CChart::SetPostMouseMoveFunc(bool (*fcnPostMouseMove)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnPostMouseMove(fcnPostMouseMove));
	SuffixT_A_NR3D(->SetParaPostMouseMove(pPara));
}

void	CChart::SetPostLButtonDownFunc(bool (*fcnPostLButtonDown)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnPostLButtonDown(fcnPostLButtonDown));
	SuffixT_A_NR3D(->SetParaPostLButtonDown(pPara));
}

void	CChart::SetPostLButtonUpFunc(bool (*fcnPostLButtonUp)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnPostLButtonUp(fcnPostLButtonUp));
	SuffixT_A_NR3D(->SetParaPostLButtonUp(pPara));
}

void	CChart::SetPostLButtonDblClkFunc(bool (*fcnPostLButtonDblClk)( void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnPostLButtonDblClk(fcnPostLButtonDblClk));
	SuffixT_A_NR3D(->SetParaPostLButtonDblClk(pPara));
}

void	CChart::SetPostKeyDownFunc(bool (*fcnPostKeyDown)( void *plot, HDC hDC, UINT key, void *para, bool &bContinue ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnPostKeyDown(fcnPostKeyDown));
	SuffixT_A_NR3D(->SetParaPostKeyDown(pPara));
}

int		CChart::GetPlotIndexByMousePos(POINT point)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<kTypeSplit || m_pPara->nType>kTypeLayered)return -1;

	switch(m_pPara->nType)
	{
	case kTypeSplit:
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetPlotIndex(point);
	case kTypeShareX:
		return ((CShareXPlotImplI*)m_pPara->pPlot)->GetTop();
	case kTypeLayered:
		return ((CLayeredPlotImplI*)m_pPara->pPlot)->GetTop();
	default:
		return -1;
	}
	
}

void	CChart::SetDrawBasicAnyway(bool draw)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetDrawBasicAnyway(draw));
}

void	CChart::SetDrawBasicAnyway(bool draw, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetDrawBasicAnyway(draw));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetDrawBasicAnyway(draw));
}

void	CChart::SetDefScreen(void (*fcnDefScreen)( void *plot, HDC hDC, RECT plotRect, void *pPara ), void *pPara)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetFcnDefScreen(fcnDefScreen));
	SuffixT_A_NR3D(->SetParaDefScreen(pPara));
}

void	CChart::SetDefScreen(void (*fcnDefScreen)( void *plot, HDC hDC, RECT plotRect, void *pPara ), void *pPara, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLayer(m_pPara->nType))
		SetDefScreen(fcnDefScreen, pPara);
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetFcnDefScreen(fcnDefScreen));
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetParaDefScreen(pPara));
	}
}

void	CChart::SetDefMainString(TCHAR *str)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetDefaultMain(str));
}

void	CChart::SetDefMainString(TCHAR *str, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SetDefMainString(str);
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetDefaultMain(str));
	}
}

void	CChart::SetDefSubString(TCHAR *str)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetDefaultSub(str));
}

void	CChart::SetDefSubString(TCHAR *str, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SetDefSubString(str);
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetDefaultSub(str));
	}
}

void	CChart::SetDefImage(HBITMAP hbm)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetDefScreenImage(hbm));
}

void	CChart::SetDefImage(HBITMAP hbm, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SetDefImage(hbm);
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetDefScreenImage(hbm));
	}
}

bool	CChart::CheckSubPlotIndex(int nPlotIndex)
{
	if(!m_pPara)return false;
	
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	if(IsSingleLayer(m_pPara->nType))
		return true;
	else if(IsMultiLayer(m_pPara->nType))
	{
		if(nPlotIndex<0 || nPlotIndex>=SuffixT_M(->GetSubPlotCount()))return false;
		return true;
	}
	else
		return false;
}

TCHAR*	CChart::GetDescriptionByMousePos(HDC hDC, POINT point)
{
	if(!m_pPara)return _TEXT("");
	
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return _TEXT("");
	
	static tstring str;

	str = SuffixT_A_NF_NR3D(->GetDescriptionByMousePos(hDC, point));
	
	return (TCHAR *)str.c_str();
}

void	CChart::SetToolTipType(int type)
{
	if(!m_pPara)return;
	
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetPointDesType(type));
}

void	CChart::SetZoomModeBuildIn(bool zoom)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

//	SuffixT_A_NR3D(->SetRangeZoomMode(zoom));
	switch(m_pPara->nType)
	{
	case kTypeXY:
		((CXYPlotImplI*)m_pPara->pPlot)->SetRangeZoomMode(zoom);
		break;;
	case kTypeOscillo:
		((CFixAxisPlotImplI*)m_pPara->pPlot)->SetRangeZoomMode(zoom);
		break;
	}
}

void	CChart::ResetZoomedRanges()
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	//	SuffixT_A_NR3D(->SetRangeZoomMode(zoom));
	switch(m_pPara->nType)
	{
	case kTypeXY:
		((CXYPlotImplI*)m_pPara->pPlot)->ResetRanges();
		break;;
	case kTypeOscillo:
		((CFixAxisPlotImplI*)m_pPara->pPlot)->ResetRanges();
		break;
	}
}

void	CChart::SetZoomMode(bool zoom)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	m_pPara->ZoomFact.bZoomMode = zoom;
}

void	CChart::SetZoomFactorX(double fact)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	m_pPara->ZoomFact.fZoomFactorX = fact;
}

void	CChart::SetZoomFactorY(double fact)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	m_pPara->ZoomFact.fZoomFactorY = fact;
}

void	CChart::SetZoomFactor(double fact)
{
	SetZoomFactorX(fact);
	SetZoomFactorY(fact);
}

void	CChart::SetZoomCenterMode(int center)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	m_pPara->ZoomFact.nZoomCenterMode = center;
}

RECT	CChart::GetZoomedRect(RECT destRect)
{
	if(!m_pPara)return destRect;
	if(!m_pPara->ZoomFact.bZoomMode)
		return destRect;
	
	int width = Width(destRect);
	int	height = Height(destRect);
	double fx = m_pPara->ZoomFact.fZoomFactorX;
	double fy = m_pPara->ZoomFact.fZoomFactorY;
	
	RECT rt = destRect;
	switch(m_pPara->ZoomFact.nZoomCenterMode)
	{
	case kZoomCenterLT:
		rt.right = rt.left + long(fx * width);
		rt.bottom = rt.top + long(fy * height);
		break;
	case kZoomCenterLB:
		rt.right = rt.left + long(fx * width);
		rt.top = rt.bottom - long(fy * height);
		break;
	case kZoomCenterRB:
		rt.left = rt.right - long(fx * width);
		rt.top = rt.bottom - long(fy * height);
		break;
	case kZoomCenterRT:
		rt.left = rt.right - long(fx * width);
		rt.bottom = rt.top + long(fy * height);
		break;
	case kZoomCenterCT:
		rt.left = (destRect.left + destRect.right)/2 - long(fx*width/2);
		rt.right = (destRect.left + destRect.right)/2 + long(fx*width/2);
		rt.top = (destRect.bottom + destRect.top)/2 - long(fy*height/2);
		rt.bottom = (destRect.bottom + destRect.top)/2 + long(fy*height/2);
		break;
	case kZoomCenterARB:
		rt.left = m_pPara->ZoomFact.ptZoomCenter.x + long(fx*(destRect.left - m_pPara->ZoomFact.ptZoomCenter.x));
		rt.right = m_pPara->ZoomFact.ptZoomCenter.x + long(fx*(destRect.right - m_pPara->ZoomFact.ptZoomCenter.x));
		rt.top = m_pPara->ZoomFact.ptZoomCenter.y + long(fy*(destRect.top - m_pPara->ZoomFact.ptZoomCenter.y));
		rt.bottom = m_pPara->ZoomFact.ptZoomCenter.y + long(fy*(destRect.bottom - m_pPara->ZoomFact.ptZoomCenter.y));
		break;
	default:
		return destRect;
	}
	return rt;
}

int		CChart::AddCurve(double *pX, double *pY, int nLen, int nPlotIndex)
{
	if(!m_pPara)return -1;
	
	if(nPlotIndex < 0)
		return -1;
	if(!pX || !pY || nLen <=0)
		return -1;
	
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	
	int subNum;
	int dataID;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
	{
		dataID = SuffixT_S_NF_N3D(->AddCurve(pX, pY, nLen));
	}
	else if(m_pPara->nType==kTypeSplit)
	{
		subNum = ((CSplitPlotImplI<CXYPlotImpl> *)m_pPara->pPlot)->GetSubPlotCount();
		if(nPlotIndex>=subNum)return -1;
		dataID = ((CSplitPlotImplI<CXYPlotImplI> *)m_pPara->pPlot)->AddCurve(pX, pY, nLen, nPlotIndex);
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		subNum = SuffixT_M(->GetSubPlotCount());
		if(nPlotIndex>=subNum)return -1;
		dataID = SuffixT_M(->AddCurve(pX, pY, nLen, nPlotIndex));
	}
	else
	{
		dataID = -1;
	}
	return dataID;
}

int		CChart::AddCurve(TCHAR **pStrTime, TCHAR *fmt, double *pY, int nLen, int nPlotIndex/* =0 */)
{
	if(!m_pPara)return -1;
	if(nPlotIndex < 0)
		return -1;
	if(!pStrTime || !pY || nLen <=0)
		return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;

	double *pX = new double[nLen];
	for(int i=0; i<nLen; i++)
	{
		if(!MyStr2Time(pStrTime[i], fmt, pX[i]))
		{
			delete []pX;
			return -1;
		}
	}
	int dataID = AddCurve(pX, pY, nLen, nPlotIndex);
	delete []pX;

	return dataID;
}

int		CChart::AddCurve(int nPlotIndex)
{
	if(!m_pPara)return -1;
	if(nPlotIndex < 0)
		return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;

	if(!CheckSubPlotIndex(nPlotIndex))return -1;
	
	int dataID = -1;
	if(IsSingleLayer(m_pPara->nType))
	{
		dataID = SuffixT_S_NF(->AddEmpty());
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		dataID = SuffixT_M(->GetSubPlot(nPlotIndex)->AddEmpty());
	}
	else if(IsReal3DChart(m_pPara->nType))
	{
		dataID = SuffixT_R3D_L(->AddEmpty());
	}
	return dataID;
}


int		CChart::UpdateCurve(int nDataIndex, double *pX, double *pY, int nLen, int nPlotIndex)
{
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)return dataID;

	return UpdateCurveByID(dataID, pX, pY, nLen, nPlotIndex);
}

int		CChart::UpdateCurveByID(int nDataID, double *pX, double *pY, int nLen, int nPlotIndex)
{
	if(!m_pPara)return -1;
	if(!pX || !pY || nLen <=0)
		return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;

	if(!CheckSubPlotIndex(nPlotIndex))
		return -1;

	int dataID = -1;
	if(m_pPara->nType==kTypeXY)
	{
		dataID = ((CXYPlotImplI*)m_pPara->pPlot)->UpdateCurve(nDataID, pX, pY,nLen);
		if(dataID>=0)
		{
			((CXYPlotImplI*)m_pPara->pPlot)->SetXAutoRange(true);
			((CXYPlotImplI*)m_pPara->pPlot)->SetYAutoRange(true);
		}
	}
	else if(m_pPara->nType==kTypeOscillo)
	{
		dataID = ((CFixAxisPlotImplI*)m_pPara->pPlot)->UpdateCurve(nDataID, pX, pY,nLen);
		if(dataID>=0)
		{
			((CFixAxisPlotImplI*)m_pPara->pPlot)->SetXAutoRange(true);
			((CFixAxisPlotImplI*)m_pPara->pPlot)->SetYAutoRange(true);
		}
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		dataID = SuffixT_M(->GetSubPlot(nPlotIndex)->UpdateCurve(nDataID, pX, pY, nLen));
		if(dataID>=0)
		{
			SuffixT_M(->GetSubPlot(nPlotIndex)->SetXAutoRange(true));
			SuffixT_M(->GetSubPlot(nPlotIndex)->SetYAutoRange(true));
		}
	}
	//LeaveCriticalSection(&g_cs);
	return dataID;
}

int		CChart::ReplaceCurve(int nDataIndex, double *pX, double *pY, int nLen, int nPlotIndex)
{
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)return dataID;
	
	return ReplaceCurveByID(dataID, pX, pY, nLen, nPlotIndex);
}

int		CChart::ReplaceCurveByID(int nDataID, double *pX, double *pY, int nLen, int nPlotIndex)
{
	if(!m_pPara)return -1;
	if(!pX || !pY || nLen <=0)
		return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	
	if(!CheckSubPlotIndex(nPlotIndex))
		return -1;
	
	int dataID = -1;
	if(IsSingleLineLayer(m_pPara->nType))
	{
		SuffixT_S_NF(->UpdateCurve(nDataID, pX, pY, nLen));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		dataID = SuffixT_M(->GetSubPlot(nPlotIndex)->UpdateCurve(nDataID, pX, pY, nLen));
	}
	//LeaveCriticalSection(&g_cs);
	return dataID;
}

int		CChart::GetIndex(int dataID, int nPlotIndex)
{
	if(!m_pPara)return -1;
	if(nPlotIndex < 0)
		return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;

	if(!CheckSubPlotIndex(nPlotIndex))return -1;
	
	int index;
	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		index = SuffixT_S_NF(->GetIndex(dataID));
	else if(IsMultiLayer(m_pPara->nType))
		index = SuffixT_M(->GetSubPlot(nPlotIndex)->GetIndex(dataID));
	else if(m_pPara->nType == kType3DLine)
		index = SuffixT_R3D_L(->GetIndex(dataID));
	else
		index = -1;

	return index;
}

int		CChart::GetDataID(int nIndex, int nPlotIndex)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount|| !m_pPara->pPlot)
		return -1;
	if(!CheckSubPlotIndex(nPlotIndex))return -1;
	
	int dataID;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		dataID = SuffixT_S_NF(->GetDataID(nIndex));
	else if(IsMultiLayer(m_pPara->nType))
		dataID = SuffixT_M(->GetSubPlot(nPlotIndex)->GetDataID(nIndex));
	else if(m_pPara->nType == kType3DLine)
		dataID = SuffixT_R3D_L(->GetDataID(nIndex));
	else
		dataID = -1;
	return dataID;
}

void		CChart::SetDataID(int newID, int nIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount|| !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID;
	
	dataID = GetDataID(nIndex, nPlotIndex);
	if(dataID<0)return;

	if(IsSingleLayer(m_pPara->nType))if(IsSingleLayer(m_pPara->nType))
	{
		SuffixT_S_NF(->SetDataID(dataID, newID));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetDataID(dataID, newID));
	}
	else if(m_pPara->nType == kType3DLine)
	{
		SuffixT_R3D_L(->SetDataID(dataID, newID));
	}
}

int		CChart::AddPoint2D(double x, double y, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kType2DCount || !m_pPara->pPlot)
		return -1;

	if(!CheckSubPlotIndex(nPlotIndex))return -1;

	int dataID = -1;

	if(m_pPara->nType == kTypeXY || m_pPara->nType == kTypeStem || m_pPara->nType == kTypeOscillo)
		dataID = SuffixT_S_NF_N3D(->AddPoint(x, y, nDataIndex));
	else if(IsMultiLayer(m_pPara->nType))
		dataID = SuffixT_M(->GetSubPlot(nPlotIndex)->AddPoint(x, y, nDataIndex));
	else
		dataID = -1;
	return dataID;
}

int		CChart::AddPoint2D(TCHAR *strTime, TCHAR *fmt, double y, int nDataIndex/* =0 */, int nPlotIndex/* =0 */)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kType2DCount || !m_pPara->pPlot)
		return -1;

	if(!CheckSubPlotIndex(nPlotIndex))return -1;

	double x;
	if(!MyStr2Time(strTime, fmt, x))return -1;
	return AddPoint2D(x, y, nDataIndex, nPlotIndex);
}

int		CChart::InsertPoint2D(double x, double y, int nPos, int nDataIndex/* =0 */, int nPlotIndex/* =0 */)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kType2DCount || !m_pPara->pPlot)
		return -1;

	if(!CheckSubPlotIndex(nPlotIndex))return -1;
	
	int dataID;

	if(m_pPara->nType == kTypeXY || m_pPara->nType == kTypeStem || m_pPara->nType == kTypeOscillo)
		dataID = SuffixT_S_NF(->InsertPoint(x, y, nPos, nDataIndex));
	else if(IsMultiLayer(m_pPara->nType))
		dataID = SuffixT_M(->GetSubPlot(nPlotIndex)->InsertPoint(x, y, nPos, nDataIndex));
	else
		dataID = -1;
	return dataID;
}

int			CChart::AddPoint3D(double x, double y, double z, int nDataIndex)
{
	if(!m_pPara)return -1;
	if( (m_pPara->nType<kType2DCount && m_pPara->nType!=kTypeXY3D) || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	switch(m_pPara->nType)
	{
	case kType3DLine:
		return ((CTDLinePlotImpl *)m_pPara->pPlot)->AddPoint(x, y, z, nDataIndex);
	case kTypeXY3D:
		return ((CXY3DPlotImplI *)m_pPara->pPlot)->AddPoint(x, y, z, nDataIndex);
	}
	return -1;
}
int			CChart::AddCurve(double *pX, double *pY, double *pZ, int nLen)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	
	switch(m_pPara->nType)
	{
	case kType3DLine:
		return ((CTDLinePlotImpl *)m_pPara->pPlot)->AddCurve(pX, pY, pZ, nLen);
	case kTypeXY3D:
		return ((CXY3DPlotImpl *)m_pPara->pPlot)->AddCurve(pX, pY, pZ, nLen);
	}
	return -1;
}

void		CChart::ClrAllData()
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		ClrPlotData();
	else if(IsMultiLayer(m_pPara->nType))
	{
		int i, nPlots;
		nPlots = SuffixT_M(->GetSubPlotCount());
		if(nPlots<=0)return;
		for(i=0; i<nPlots; i++)
		{
			ClrPlotData(i);
		}
	}
	else if(m_pPara->nType == kType3DLine)
	{
		ClrPlotData();
	}
}

void		CChart::ClrPlotData(int nPlotIndex/*=0*/)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		SuffixT_S_NF(->DeleteAllData());
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->DeleteAllData());//DeteleAllData());
	else if(m_pPara->nType == kType3DLine)
		SuffixT_R3D_L(->DeleteAllData());
}

void		CChart::ClrSingleData(int nDataIndex, int nPlotIndex/*=0*/)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	int dataID = GetDataID(nDataIndex);
	if(dataID<0)return;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		SuffixT_S_NF(->DeleteData(dataID));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->DeleteData(dataID));//DeteleData(dataID));
	else if(m_pPara->nType == kType3DLine)
		SuffixT_R3D_L(->DeleteData(dataID));
}

void		CChart::EmptySingleData(int nDataIndex, int nPlotIndex/*=0*/)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex);
	if(dataID<0)return;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		SuffixT_S_NF(->EmptyData(dataID));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->EmptyData(dataID));
	else if(m_pPara->nType == kType3DLine)
		SuffixT_R3D_L(->EmptyData(dataID));
}

void	CChart::SetReservedDataLength(int nLen, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetReservedDataLength(nLen));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetReservedDataLength(nLen));
	else if(m_pPara->nType == kType3DLine)
		SuffixT_R3D_L(->SetReservedDataLength(nLen));
}

int		CChart::GetDataPointsCount(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID < 0)
		return 0;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		return (int)SuffixT_S_NF(->GetAbsData(nDataIndex).size());
	else if(IsMultiLayer(m_pPara->nType))
		return (int)SuffixT_M(->GetSubPlot(nPlotIndex)->GetAbsData(nDataIndex).size());
	else if(m_pPara->nType == kType3DLine)
		return (int)SuffixT_R3D_L(->GetAbsData(nDataIndex).size());
	else
		return 0;
}

double		CChart::GetDataPointX(int nPointIndex, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return 0.0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0.0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0.0;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID < 0)
		return 0.0;

	if(nPointIndex<0 || nPointIndex>=GetDataPointsCount(nDataIndex, nPlotIndex))return 0.0;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		return SuffixT_S_NF(->GetAbsData(nDataIndex)[nPointIndex].val[0]);
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetAbsData(nDataIndex)[nPointIndex].val[0]);
	else
		return 0.0;
}

double		CChart::GetDataPointY(int nPointIndex, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return 0.0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0.0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0.0;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID < 0)
		return 0.0;
	
	if(nPointIndex<0 || nPointIndex>=GetDataPointsCount(nDataIndex, nPlotIndex))return 0.0;
	
	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		return SuffixT_S_NF(->GetAbsData(nDataIndex)[nPointIndex].val[1]);
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetAbsData(nDataIndex)[nPointIndex].val[1]);
	else
		return 0.0;
}

bool		CChart::GetDataPoint(double data[2], int nPointIndex, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))return false;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID < 0)
		return false;
	
	if(nPointIndex<0 || nPointIndex>=GetDataPointsCount(nDataIndex, nPlotIndex))return false;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		memcpy(data, SuffixT_S_NF(->GetAbsData(nDataIndex)[nPointIndex].val), 2*sizeof(double) );
	else if(IsMultiLayer(m_pPara->nType))
		memcpy(data, SuffixT_M(->GetSubPlot(nPlotIndex)->GetAbsData(nDataIndex)[nPointIndex].val), 2*sizeof(double) );
	else
		return false;

	return true;
}

int		CChart::RemoveSinglePoint(int nPointIndex, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	if(!CheckSubPlotIndex(nPlotIndex))return -1;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID < 0)
		return -1;
	
	if(nPointIndex<0 || nPointIndex>=GetDataPointsCount(nDataIndex, nPlotIndex))
		return -1;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		return SuffixT_S_NF(->RemovePoint(dataID, nPointIndex));
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->RemovePoint(dataID, nPointIndex));
	else
		return -1;
}

int		CChart::RemoveFirstPoint(int nDataIndex, int nPlotIndex)
{
	return RemoveSinglePoint(0, nDataIndex, nPlotIndex);
}

int		CChart::RemoveFirstPointAndShift(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	if(!CheckSubPlotIndex(nPlotIndex))return -1;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID < 0)
		return -1;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		return SuffixT_S_NF(->RemoveFirstPointAndShift(dataID));
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->RemoveFirstPointAndShift(dataID));
	else
		return -1;
}

int		CChart::RemoveLastPoint(int nDataIndex, int nPlotIndex)
{
	return RemoveSinglePoint(GetDataPointsCount(nDataIndex, nPlotIndex)-1, nDataIndex, nPlotIndex);
}

int		CChart::SlipXValue(int nStep, int nDataIndex, int nPlotIndex/* =0 */)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	if(!CheckSubPlotIndex(nPlotIndex))return -1;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID < 0)
		return -1;
	
	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		SuffixT_S_NF(->SlipXValue(nStep, dataID));
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->SlipXValue(nStep, dataID));
	else
		return -1;

	return dataID;
}

int		CChart::SlipYValue(int nStep, int nDataIndex, int nPlotIndex/* =0 */)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	if(!CheckSubPlotIndex(nPlotIndex))return -1;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID < 0)
		return -1;
	
	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		SuffixT_S_NF(->SlipYValue(nStep, dataID));
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->SlipYValue(nStep, dataID));
	else
		return -1;
	
	return dataID;
}

int		CChart::AddPie(double value)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType!=kTypePie|| !m_pPara->pPlot)
		return -1;
	return ((CPiePlotImplI *)m_pPara->pPlot)->AddPie(value);
}

int		CChart::AddPie(double value, TCHAR* title)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType!=kTypePie|| !m_pPara->pPlot)
		return -1;
	int dataID = ((CPiePlotImplI *)m_pPara->pPlot)->AddPie(value);
	((CPiePlotImplI *)m_pPara->pPlot)->SetDataTitle(dataID, title);
	return dataID;
}

int		CChart::AddStems(double *pData, int nLen)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType!=kTypeStem|| !m_pPara->pPlot)
		return -1;
	return ((CStemPlotImplI *)m_pPara->pPlot)->AddData(pData, nLen);
}

int		CChart::AddStems(double *pData, int nLen, TCHAR* title)
{
	if(!m_pPara)return -1;
	if(m_pPara->nType!=kTypeStem|| !m_pPara->pPlot)
		return -1;
	int dataID = ((CStemPlotImplI *)m_pPara->pPlot)->AddData(pData, nLen);
	((CStemPlotImplI *)m_pPara->pPlot)->SetDataTitle(dataID, title);
	return dataID;
}

void		CChart::SetFieldFcn(double (*_pFieldFcn) (double, double))
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(IsFieldChart(m_pPara->nType))
		SuffixT_F(->SetFieldFcn(_pFieldFcn));
}

#ifndef _WIN64
void		CChart::SetFieldFcn(double (_stdcall *_pFieldFcn) (double, double))
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(IsFieldChart(m_pPara->nType))
	{
		switch(m_pPara->nType)
		{
		case kTypeContourLine:
			((CContourLinePlotImplI *)m_pPara->pPlot)->SetFieldFcn(_pFieldFcn);
			break;
		case kTypeContourMap:
			((CCloudPlotImplI *)m_pPara->pPlot)->SetFieldFcn(_pFieldFcn);
			break;
		case kTypeContour:
			((CContourPlotImplI *)m_pPara->pPlot)->SetFieldFcn(_pFieldFcn);
			break;
		}
	}
}
#endif

double		(*CChart::GetFieldFcn( ))( double, double )
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(IsFieldChart(m_pPara->nType))
		return SuffixT_F(->GetFieldFcn());
	else
		return 0;
}

void		CChart::AddContourPoint(double x, double y, double h)
{
	if(!m_pPara)return;
	if(!m_pPara->pPlot)return;
	switch(m_pPara->nType)
	{
	case kTypeContourLine:
		((CContourLinePlotImplI *)m_pPara->pPlot)->AddContourPoint(x, y, h);
		break;
	case kTypeContourMap:
		((CCloudPlotImplI *)m_pPara->pPlot)->AddContourPoint(x, y, h);
		break;
	case kTypeContour:
	//	((CContourLinePlotImplI *)(CContourPlotImplI *)m_pPara->pPlot)->AddContourPoint(x, y, h);
	//	((CCloudPlotImplI *)(CContourPlotImplI *)m_pPara->pPlot)->AddContourPoint(x, y, h);
		((CContourPlotImplI *)m_pPara->pPlot)->AddContourPoint(x, y, h);
		break;
	default:
		return;
	}
}

void		CChart::ClrContourPoints()
{
	if(!m_pPara)return;
	if(!m_pPara->pPlot)return;
	switch(m_pPara->nType)
	{
	case kTypeContourLine:
		((CContourLinePlotImplI *)m_pPara->pPlot)->ClrContourPoints();
		break;
	case kTypeContourMap:
		((CCloudPlotImplI *)m_pPara->pPlot)->ClrContourPoints();
		break;
	case kTypeContour:
		((CContourLinePlotImplI *)(CContourPlotImplI *)m_pPara->pPlot)->ClrContourPoints();
		((CCloudPlotImplI *)(CContourPlotImplI *)m_pPara->pPlot)->ClrContourPoints();
		break;
	default:
		return;
	}
}

void		CChart::SetPieTitle(const TCHAR* title, int which)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypePie || !m_pPara->pPlot)
		return;
	((CPiePlotImplI*)m_pPara->pPlot)->SetDataTitle(((CPiePlotImplI*)m_pPara->pPlot)->GetDataID(which), title);
}

void		CChart::SetPieColor(COLORREF color, int which)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypePie || !m_pPara->pPlot)
		return;
	((CPiePlotImplI*)m_pPara->pPlot)->SetDataColor(((CPiePlotImplI*)m_pPara->pPlot)->GetDataID(which), color);
}

void		CChart::SetMaxPies(int maxp)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypePie || !m_pPara->pPlot)
		return;
	((CPiePlotImplI*)m_pPara->pPlot)->SetMaxPies(maxp);
}

LOGFONT		&CChart::GetPieTitleFont()
{
	static LOGFONT lfFont = GetTitleFont();
	if(!m_pPara)return lfFont;
	if(m_pPara->nType!=kTypePie || !m_pPara->pPlot)
		return lfFont;
	return ((CPiePlotImplI*)m_pPara->pPlot)->GetPieTitleFont();
}

void		CChart::SetStemLabel(const TCHAR* label, int which)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypeStem || !m_pPara->pPlot)
		return;
	((CStemPlotImplI*)m_pPara->pPlot)->SetXLabel(label, which);
}

void		CChart::SetStemTitle(const TCHAR* title, int nDataIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypeStem || !m_pPara->pPlot)
		return;
	SetDataTitle(title, nDataIndex, 0);
}

void		CChart::SetStemRatio(double ratio)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypeStem || !m_pPara->pPlot)
		return;
	if(ratio <=0.0 || ratio > 1.0)return;
	
	((CStemPlotImplI*)m_pPara->pPlot)->SetRatioOccupy(ratio);
}

void		CChart::SetStemBase(int base)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypeStem || !m_pPara->pPlot)
		return;
	if(base <=0 || base > 3)return;
	
	((CStemPlotImplI*)m_pPara->pPlot)->SetStemBase(base);
}

void		CChart::SetStemMaxNum(int num)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypeStem || !m_pPara->pPlot)
		return;
	if(num <=1)return;
	
	((CStemPlotImplI*)m_pPara->pPlot)->SetMaxDataSets(num);
}

void		CChart::SetContourLineNum(int num)
{
	if(!m_pPara)return;
	if(!m_pPara->pPlot)
		return;
	if(m_pPara->nType==kTypeContourLine )
		((CContourLinePlotImplI*)m_pPara->pPlot)->SetNContour(num);
	else if(m_pPara->nType==kTypeContour )
		((CContourPlotImplI*)m_pPara->pPlot)->SetNContour(num);
}

void		CChart::SetContourPrecision(int n)
{
	if(!m_pPara)return;
	if(!m_pPara->pPlot)
		return;
	if(m_pPara->nType==kTypeContourLine )
		((CContourLinePlotImplI*)m_pPara->pPlot)->SetContourPrecision(n);
	else if(m_pPara->nType==kTypeContour )
		((CContourPlotImplI*)m_pPara->pPlot)->SetContourPrecision(n);
}

void		CChart::SetContourValueShow(bool show)
{
	if(!m_pPara)return;
	if(!m_pPara->pPlot)
		return;
	if(m_pPara->nType==kTypeContourLine )
		((CContourLinePlotImplI*)m_pPara->pPlot)->SetDataInfoShow(show);
	else if(m_pPara->nType==kTypeContour )
		((CContourPlotImplI*)m_pPara->pPlot)->SetDataInfoShow(show);
}

void		CChart::SetContourMapColor1(COLORREF cr)
{
	if(!m_pPara)return;
	if(!m_pPara->pPlot)
		return;
	if(m_pPara->nType==kTypeContourLine )
		((CContourLinePlotImplI*)m_pPara->pPlot)->SetColor1(cr);
	else if(m_pPara->nType==kTypeContourLine )
		((CCloudPlotImplI*)m_pPara->pPlot)->SetColor1(cr);
	else if(m_pPara->nType==kTypeContour )
	{
		((CContourPlotImplI*)m_pPara->pPlot)->SetColor1(cr);
	}
}

void		CChart::SetContourMapColor2(COLORREF cr)
{
	if(!m_pPara)return;
	if(!m_pPara->pPlot)
		return;
	if(m_pPara->nType==kTypeContourLine )
		((CContourLinePlotImplI*)m_pPara->pPlot)->SetColor2(cr);
	else if(m_pPara->nType==kTypeContourLine )
		((CCloudPlotImplI*)m_pPara->pPlot)->SetColor2(cr);
	else if(m_pPara->nType==kTypeContour )
	{
		((CContourPlotImplI*)m_pPara->pPlot)->SetColor2(cr);
	}
}

void		CChart::SetContourByPoints()
{
	if(!m_pPara)return;
	if(!m_pPara->pPlot)
		return;
	switch(m_pPara->nType)
	{
	case kTypeContourLine:
		((CContourLinePlotImplI*)m_pPara->pPlot)->SetToPointContour();
		break;
	case kTypeContourMap:
		((CCloudPlotImplI*)m_pPara->pPlot)->SetToPointContour();
		break;
	case kTypeContour:
		((CContourPlotImplI*)m_pPara->pPlot)->SetToPointContour();
		break;
	}
}

void		CChart::SetRightAxisAsLegend(bool as)
{
	if(!m_pPara)return;
	if(!m_pPara->pPlot)
		return;
	switch(m_pPara->nType)
	{
	case kTypeContourMap:
		((CCloudPlotImplI*)m_pPara->pPlot)->SetRightAxisAsLegend(as);
		break;
	case kTypeContour:
		((CContourPlotImplI*)m_pPara->pPlot)->SetRightAxisAsLegend(as);
		break;
	}
}

void		CChart::SetTopAxisAsLegend(bool as)
{
	if(!m_pPara)return;
	if(!m_pPara->pPlot)
		return;
	switch(m_pPara->nType)
	{
	case kTypeContourMap:
		((CCloudPlotImplI*)m_pPara->pPlot)->SetTopAxisAsLegend(as);
		break;
	case kTypeContour:
		((CContourPlotImplI*)m_pPara->pPlot)->SetTopAxisAsLegend(as);
		break;
	}
}

int		CChart::AddLayer()
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	
	int	nLayers;
	switch(m_pPara->nType)
	{
	case kTypeShareX:
		nLayers = ((CShareXPlotImplI*)m_pPara->pPlot)->AddLayer();
		return nLayers;
	case kTypeLayered:
		nLayers = ((CLayeredPlotImplI*)m_pPara->pPlot)->AddLayer();
		return nLayers;
	default:
		return -1;
	}
}

void	CChart::ResizePlots(int mode, int nRows, int nCols)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	switch(m_pPara->nType)
	{
	case kTypeSplit:
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->ResizePlots(mode, nRows, nCols);
		return;
	default:
		return;
	}
}

void	CChart::ResizePlots(int nLayers)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(nLayers<=0)
		return;
	
	switch(m_pPara->nType)
	{
	case kTypeLayered:
		((CLayeredPlotImplI*)m_pPara->pPlot)->ResizePlots(nLayers);
		return;
	case kTypeShareX:
		((CShareXPlotImplI*)m_pPara->pPlot)->ResizePlots(nLayers);
		return;
	}
}

bool	CChart::ResizeSplit(CChart *pChart, int nSubPlots)
{
	if(!pChart)return false;
	if(!pChart->GetPara())return false;
	if(pChart->GetType()!=kTypeSplit)return false;
	
	if(nSubPlots <= 1)
	{
		pChart->ResizePlots(kSplitNot, nSubPlots, 1);
	}
	else if(nSubPlots == 3)
	{
		pChart->ResizePlots(kSplit3L2R1, nSubPlots, 1);
	}
	else if(nSubPlots == 4)
	{
		pChart->ResizePlots(kSplitNM, 2, 2);
	}
	else// if(plotcount == 2 || plotcount > 4)
	{
		pChart->ResizePlots(kSplitNM, 1, nSubPlots);
	}
	return true;
}

int		CChart::GetSubPlotCount()
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;

	if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlotCount());
	else
		return 0;
}

int		CChart::GetTop()
{
	if(!m_pPara)return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	
	if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetTop());
	else
		return -1;
}

void	CChart::SetUniteLayerColor(bool bUnite)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	switch(m_pPara->nType)
	{
	case kTypeLayered:
		((CLayeredPlotImplI*)m_pPara->pPlot)->SetUniteLayerColor(bUnite);
		return;
	case kTypeShareX:
		((CShareXPlotImplI*)m_pPara->pPlot)->SetUniteLayerColor(bUnite);
		return;
	}
}

void	CChart::SetExcludeDataWhenUniteColor(bool bExclude, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	switch(m_pPara->nType)
	{
	case kTypeLayered:
		((CLayeredPlotImplI*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetExcludeDataWhenUniteColor(bExclude);
		return;
	case kTypeShareX:
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetExcludeDataWhenUniteColor(bExclude);
		return;
	}
}

int		CChart::GetPlotDataCount(int nPlotIndex/* =0 */)
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;

	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		return SuffixT_S_NF(->GetPlotDataCount());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotDataCount());
	else
		return 0;
}

void		CChart::SetDataReact(bool react, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_NF(->GetPlotData(dataID)->SetReact(react));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetReact(react));
}

void		CChart::SetDataForceGdiPlus(bool bForce, int nDataIndex, int nPlotIndex/* =0 */)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_NF(->GetPlotData(dataID)->SetForceGdiPlus(bForce));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetForceGdiPlus(bForce));
}

const TCHAR*		CChart::GetDataTitle(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return _TEXT("");
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return _TEXT("");

	if(!CheckSubPlotIndex(nPlotIndex))return _TEXT("");
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return _TEXT("");

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_NF(->GetDataTitle(dataID).c_str());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetDataTitle(dataID).c_str());
	else if(m_pPara->nType == kType3DLine)
		return SuffixT_R3D_L(->GetDataTitle(dataID).c_str());
	else 
		return _TEXT("");
}

void		CChart::SetDataTitle(const TCHAR* title, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		SuffixT_S_NF(->SetDataTitle(dataID, title));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetDataTitle(dataID, title));
	else if(m_pPara->nType == kType3DLine)
		SuffixT_R3D_L(->SetDataTitle(dataID, title));
}

void		CChart::SetAutoDataColorGeneral(bool bAuto, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(IsSingleLineLayer(m_pPara->nType))
	{
		SuffixT_S_NF(->SetAutoDataColorGeneral(false));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAutoDataColorGeneral(false));
	}
}

void		CChart::SetAutoDataColor(bool bAuto, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;
	
	if(IsSingleLineLayer(m_pPara->nType))
	{
		SuffixT_S_NF(->SetAutoColor(dataID, false));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAutoColor(dataID, false));
	}
}

COLORREF	CChart::GetDataColor(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return RGB(0, 0, 0);
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return RGB(0, 0, 0);
	if(!CheckSubPlotIndex(nPlotIndex))return RGB(0, 0, 0);
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return RGB(0, 0, 0);

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_NF(->GetDataColor(dataID));
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetDataColor(dataID));
	else 
		return RGB(0, 0, 0);
}

void	CChart::SetDataColor(COLORREF color, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
	{
		SuffixT_S_NF(->SetDataColor(dataID, color));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetDataColor(dataID, color));
	}
}

bool	CChart::IsBiColorMode(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))return false;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetPlotData(dataID)->IsBiColorMode());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->IsBiColorMode());
	else
		return false;
}

void	CChart::SetBiColorMode(bool bBi, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetBiColorMode(bBi));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetBiColorMode(bBi));
}


COLORREF	CChart::GetDataColor2(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return RGB(0, 0, 0);
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return RGB(0, 0, 0);
	if(!CheckSubPlotIndex(nPlotIndex))return RGB(0, 0, 0);
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return RGB(0, 0, 0);
	
	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_NF(->GetPlotData(dataID)->GetDataColor2());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->GetDataColor2());
	else 
		return RGB(0, 0, 0);
}

void	CChart::SetDataColor2(COLORREF color, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetDataColor2(color));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataColor2(color));
}

bool	CChart::IsMultiColorMode(int nDataIndex, int nPlotIndex/* =0 */)
{
	if(!m_pPara)return RGB(0, 0, 0);
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))return false;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetPlotData(dataID)->IsMultiColorMode());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->IsMultiColorMode());
	else
		return false;
}

void	CChart::SetMultiColorMode(bool bMul, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetMultiColorMode(bMul));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetMultiColorMode(bMul));
}

int		CChart::GetMultiColorInputType(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return kInputColorByPointRatio;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return kInputColorByPointRatio;
	if(!CheckSubPlotIndex(nPlotIndex))return kInputColorByPointRatio;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return kInputColorByPointRatio;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetPlotData(dataID)->GetMultiColorInputType());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->GetMultiColorInputType());
	else
		return false;
}

void	CChart::SetMultiColorInputType(int nType, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetMultiColorInputType(nType));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetMultiColorInputType(nType));
}

void	CChart::AddSegmentColor(double ratio, COLORREF color, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->AddSegColor(ratio, color));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->AddSegColor(ratio, color));
}

bool	CChart::IsColorPtByPt(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))return false;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetPlotData(dataID)->IsColorPtByPt());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->IsColorPtByPt());
	else
		return false;
}

void	CChart::SetColorPtByPt(bool bBy, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetColorPtByPt(bBy));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetColorPtByPt(bBy));
}

void	CChart::AddDataPointColor(COLORREF color, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->AddPointColor(color));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->AddPointColor(color));
}

void	CChart::SetDataPointColor(COLORREF color, int nPointIndex, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetPointColor(nPointIndex, color));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetPointColor(nPointIndex, color));
}

void	CChart::EraseDataPointColor(int nPointIndex, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;
	if(nPointIndex<0 || nPointIndex>=GetDataPointsCount(nDataIndex, nPlotIndex))
		return;

	vector<COLORREF> vCr;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L_Def(->GetPlotData(dataID)->GetPointColors(), vCr).erase(SuffixT_S_L_Def(->GetPlotData(dataID)->GetPointColors(), vCr).begin()+nPointIndex);
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M_Def(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->GetPointColors(), vCr).erase(SuffixT_M_Def(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->GetPointColors(), vCr).begin()+nPointIndex);
}

void	CChart::InsertDataPointColor(COLORREF color, int nPointIndex, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;
	if(nPointIndex<0 || nPointIndex>=GetDataPointsCount(nDataIndex, nPlotIndex))
		return;

	vector<COLORREF> vCr;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L_Def(->GetPlotData(dataID)->GetPointColors(), vCr).insert(SuffixT_S_L_Def(->GetPlotData(dataID)->GetPointColors(), vCr).begin()+nPointIndex, color);
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M_Def(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->GetPointColors(), vCr).insert(SuffixT_M_Def(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->GetPointColors(), vCr).begin()+nPointIndex, color);
}

int		CChart::GetDataLineStyle(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return PS_SOLID;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return PS_SOLID;
	if(!CheckSubPlotIndex(nPlotIndex))return PS_SOLID;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return PS_SOLID;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetPlotData(dataID)->GetDataLineStyle());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->GetDataLineStyle());
	else
		return PS_SOLID;
}

void	CChart::SetDataLineStyle(int nStyle, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
	{
		SuffixT_S_L(->GetPlotData(dataID)->SetDataLineStyle(nStyle));
		SuffixT_S_L(->GetPlotData(dataID)->SetAutoLineStyle(false));
		SuffixT_S_L(->SetAutoLineStyleGeneral(false));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataLineStyle(nStyle));
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetAutoLineStyle(false));
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAutoLineStyleGeneral(false));
	}
}

int		CChart::GetDataLineSize(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return 1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 1;
	if(!CheckSubPlotIndex(nPlotIndex))return 1;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return 1;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetPlotData(dataID)->GetDataLineSize());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->GetDataLineSize());
	else
		return 1;
}

void	CChart::SetDataLineSize(int nSize, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetDataLineSize(nSize));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataLineSize(nSize));
}

void	CChart::SetPlotType(int nType, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetPlotType(nType));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetPlotType(nType));
}

void	CChart::SetMarkerShow(bool bShow, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetMarkerShow(bShow));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetMarkerShow(bShow));
}

void	CChart::SetMarkerFill(bool bFill, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetMarkerFill(bFill));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetMarkerFill(bFill));
}

void	CChart::SetMarkerType(int nType, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetMarkerType(nType));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetMarkerType(nType));
}

void	CChart::SetMarkerSize(int nSize, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetMarkerSize(nSize));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetMarkerSize(nSize));
}

void	CChart::SetMarkerFreq(int nFreq, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetMarkerFrequency(nFreq));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetMarkerFrequency(nFreq));
}

void	CChart::SetMarkerStartIndex(int nStartIndex, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetMarkerStartIndex(nStartIndex));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetMarkerStartIndex(nStartIndex));
}

void	CChart::SetMarkFirstPoint(bool bMark, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetMarkFirstPoint(bMark));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetMarkFirstPoint(bMark));
}

void	CChart::SetMarkLastPoint(bool bMark, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetMarkLastPoint(bMark));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetMarkLastPoint(bMark));
}

void	CChart::SetScatter(int nDataIndex, int nPlotIndex)
{
	//SetPlotType(kXYPlotScatter, nDataIndex, nPlotIndex);
	//SetMarkerShow(true, nDataIndex, nPlotIndex);
	//SetMarkerFreq(1, nDataIndex, nPlotIndex);

	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetScatter(dataID));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetScatter(dataID));
}

void	CChart::SetDataFillColor(bool bFill, COLORREF color, int nOpaque, int nFillMode, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
	{
		SuffixT_S_L(->GetPlotData(dataID)->SetDataFill(bFill));
		SuffixT_S_L(->GetPlotData(dataID)->SetDataFillColor(color));
		SuffixT_S_L(->GetPlotData(dataID)->SetDataFillAlpha(nOpaque));
		SuffixT_S_L(->GetPlotData(dataID)->SetDataFillMode(nFillMode));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataFill(bFill));
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataFillColor(color));
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataFillAlpha(nOpaque));
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataFillMode(nFillMode));
	}
}

void	CChart::SetDataBarMode(bool bShowBar, int nBarBase, int nBarSize, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
	{
		SuffixT_S_L(->GetPlotData(dataID)->SetDataBarShow(bShowBar));
		SuffixT_S_L(->GetPlotData(dataID)->SetDataBarBase(nBarBase));
		SuffixT_S_L(->GetPlotData(dataID)->SetDataBarLineSize(nBarSize));
		SuffixT_S_L(->GetPlotData(dataID)->SetDataBarSameColor(true));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataBarShow(bShowBar));
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataBarBase(nBarBase));
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataBarLineSize(nBarSize));
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataBarSameColor(true));
	}
}

void	CChart::SetSLMode(int nMode, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetSLMode(nMode));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetSLMode(nMode));
}

bool	CChart::GetDataFlag(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))return false;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetPlotData(dataID)->GetFlag());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->GetFlag());
	else
		return false;
}

void	CChart::SetDataFlag(bool bFlag, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetFlag(bFlag));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetFlag(bFlag));
}

bool	CChart::IsDataVisible(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))return false;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetPlotData(dataID)->IsVisible());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->IsVisible());
	else
		return false;
}

void	CChart::SetDataVisible(bool bVis, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetVisible(bVis));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetVisible(bVis));
}

void	CChart::SetDataCompactDraw(bool bCompact, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetCompactDraw(bCompact));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetCompactDraw(bCompact));
}

void	CChart::SetDataHistro(bool bHistro, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
	{
		SuffixT_S_L(->GetPlotData(dataID)->SetHisto(bHistro));
		SuffixT_S_L(->GetPlotData(dataID)->SetPlotType(kXYPlotStepHV));
		SuffixT_S_L(->GetPlotData(dataID)->SetDataFill(true));
		SuffixT_S_L(->GetPlotData(dataID)->SetDataFillMode(kDataFillFromBottomAxis));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetHisto(bHistro));
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetPlotType(kXYPlotStepHV));
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataFill(true));
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataFillMode(kDataFillFromBottomAxis));
	}
}

int		CChart::GetIndexOfLighted()
{
	if(!m_pPara)return -1;
	if(m_pPara->nType != kTypeXY || !m_pPara->pPlot)
		return -1;
	
	return ((CXYPlotImplI*)m_pPara->pPlot)->GetIndexOfLighted();
}

void	CChart::SetDataValueShow(bool bShow, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetDataPointInfoShow(bShow));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataPointInfoShow(bShow));
}

void	CChart::SetDataValueType(int nType, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetDataPointInfoType(nType));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataPointInfoType(nType));
}

void	CChart::SetDataValueFontSize(int nSize, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetInfoFontsize(nSize));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetInfoFontsize(nSize));
}

void	CChart::SetDataValueFreq(int nFreq, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetDataPointInfoFrequency(nFreq));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataPointInfoFrequency(nFreq));
}

void	CChart::SetDataValueStartIndex(int nStartIndex, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->GetPlotData(dataID)->SetDataPointInfoStartIndex(nStartIndex));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotData(dataID)->SetDataPointInfoStartIndex(nStartIndex));
}

bool		CChart::IsOscilloMode(int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->IsOscilloScopeMode());
	else
		return false;
}

void		CChart::SetOscilloMode(bool bOscillo, int nPlotIndex)
{	
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetOscilloScopeMode(bOscillo));
}

bool		CChart::IsStartOscilloScope(int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	
	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->IsStartOscilloScope());
	else
		return false;
}

void		CChart::SetStartOscilloScope(bool bOscillo, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetStartOscilloScope(bOscillo));
}

bool		CChart::IsEnableDataDrag()
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->IsEnableDataDrag());
	else
		return false;
}

void		CChart::SetEnableDataDrag(bool enable)
{	
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetEnableDataDrag(enable));
}

bool		CChart::IsEnableZoneDrag()
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->IsEnableZoneDrag());
	else
		return false;
}

void		CChart::SetEnableZoneDrag(bool enable)
{	
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetEnableZoneDrag(enable));
}

bool	CChart::MoveData(double offsetx, double offsety, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)
		return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))return false;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->MovePlotData(dataID, offsetx, offsety));
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->MovePlotData(dataID, offsetx, offsety));
	else
		return false;
}

double	CChart::GetDataOffsetX(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetPlotDataOffsetX(dataID));
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotDataOffsetX(dataID));
	else
		return 0;
}

void	CChart::SetDataOffsetX(double offset, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;

	double offsetY = GetDataOffsetY(nDataIndex, nPlotIndex);

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetPlotDataOffset(dataID, offset, offsetY));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetPlotDataOffset(dataID, offset, offsetY));
}

double	CChart::GetDataOffsetY(int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetPlotDataOffsetY(dataID));
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetPlotDataOffsetY(dataID));
	else
		return 0;
}

void	CChart::SetDataOffsetY(double offset, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex, nPlotIndex);
	if(dataID<0)
		return;
	
	double offsetX = GetDataOffsetX(nDataIndex, nPlotIndex);

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetPlotDataOffset(dataID, offsetX, offset));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetPlotDataOffset(dataID, offsetX, offset));
}

void	CChart::SetXUnit(TCHAR *unit, int nDataIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType != kTypeOscillo || !m_pPara->pPlot)
		return;
	if(nDataIndex<0 || nDataIndex>=GetDataPointsCount(nDataIndex))
		return;
	
	((CFixAxisPlotImplI*)m_pPara->pPlot)->GetAbsPlotData(nDataIndex)->SetXUnit(unit);
}

void	CChart::SetYUnit(TCHAR *unit, int nDataIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType != kTypeOscillo || !m_pPara->pPlot)
		return;
	if(nDataIndex<0 || nDataIndex>=GetDataPointsCount(nDataIndex))
		return;
	
	((CFixAxisPlotImplI*)m_pPara->pPlot)->GetAbsPlotData(nDataIndex)->SetYUnit(unit);
}

void	CChart::SetShowYSCaleOnly(bool only)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType != kTypeOscillo || !m_pPara->pPlot)
		return;
	
	((CFixAxisPlotImplI*)m_pPara->pPlot)->SetShowYScaleOnly(only);
}

bool	CChart::IsHoldCursorToMouse(int nPlotIndex)
{
	if(!m_pPara)
		return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))return false;
	
	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->IsCursorByMouse());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->IsCursorByMouse();
	else
		return false;
}

void	CChart::SetHoldCursorToMouse(bool hold, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetCursorByMouse(hold));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetCursorByMouse(hold);
}

bool	CChart::IsHoldCursorToCurve(int nPlotIndex)
{
	if(!m_pPara)
		return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->IsHoldCursorToCurve());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->IsHoldCursorToCurve();
	else
		return false;
}

void	CChart::SetHoldCursorToCurve(bool hold, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetHoldCursorToCurve(hold));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetHoldCursorToCurve(hold);
}

int		CChart::GetIndexToHoldCursor(int nPlotIndex)
{
	if(!m_pPara)
		return -1;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return -1;
	if(!CheckSubPlotIndex(nPlotIndex))return -1;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetIndexToHoldCursor());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetIndexToHoldCursor();
	else
		return -1;
}

void	CChart::SetIndexToHoldCursor(int nCurveIndex, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetIndexToHoldCursor(nCurveIndex));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetIndexToHoldCursor(nCurveIndex);
}

bool		CChart::IsShowZeroArrow(int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	if(!CheckSubPlotIndex(nPlotIndex))return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->IsShowZeroArrow());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->IsShowZeroArrow();
	else
		return false;
}

void		CChart::SetShowZeroArrow(bool show, int nPlotIndex)
{	
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetShowZeroArrow(show));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetShowZeroArrow(show);
}

bool		CChart::IsShowTrigArrow(int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	
	if(!CheckSubPlotIndex(nPlotIndex))return false;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->IsShowTrigArrow());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->IsShowTrigArrow();
	else
		return false;
}

void		CChart::SetShowTrigArrow(bool show, int nPlotIndex)
{	
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetShowTrigArrow(show));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetShowTrigArrow(show);
}

void	CChart::SetShowHCursor(bool show, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetShowHCursor(show));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetShowHCursor(show);
}

void	CChart::SetShowVCursor(bool show, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetShowVCursor(show));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetShowVCursor(show);
}

COLORREF	CChart::GetCursorColor(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetCursorColor());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetCursorColor();
	else
		return 0;
}

void	CChart::SetCursorColor( COLORREF color, int nPlotIndex )
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetCursorColor(color));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetCursorColor(color);
}

int		CChart::GetCursorSize(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetCursorSize());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetCursorSize();
	else
		return 0;
}

void	CChart::SetCursorSize( int size, int nPlotIndex )
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetCursorSize(size));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetCursorSize(size);
}

int		CChart::GetCursorStyle(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;
	
	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetCursorStyle());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetCursorStyle();
	else
		return 0;
}

void	CChart::SetCursorStyle( int style, int nPlotIndex )
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetCursorStyle(style));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetCursorStyle(style);
}

double	CChart::GetCursorX(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetCursorX());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetCursorX();
	else
		return 0;
}

void	CChart::SetCursorX( double cursor, int nPlotIndex )
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetCursorX(cursor));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetCursorX(cursor);
}

double	CChart::GetCursorY(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetCursorY());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetCursorY();
	else
		return 0;
}

void	CChart::SetCursorY( double cursor, int nPlotIndex )
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetCursorY(cursor));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetCursorY(cursor);
}

void	CChart::SetShowHSel(bool show, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetEnableXZone(show));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetEnableXZone(show);
}

void	CChart::SetShowVSel(bool show, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetEnableYZone(show));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetEnableYZone(show);
}

double	CChart::GetHSelLower(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetXZoneLower());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetXZoneLower();
	else
		return 0;
}

void	CChart::SetHSelLower(int val, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetXZoneLower(val));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetXZoneLower(val);
}

double	CChart::GetHSelUpper(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetXZoneUpper());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetXZoneUpper();
	else
		return 0;
}

void	CChart::SetHSelUpper(int val, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetXZoneUpper(val));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetXZoneUpper(val);
}

double	CChart::GetVSelLower(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetYZoneLower());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetYZoneLower();
	else
		return 0;
}

void	CChart::SetVSelLower(int val, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetYZoneLower(val));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetYZoneLower(val);
}

double	CChart::GetVSelUpper(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetYZoneUpper());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetYZoneUpper();
	else
		return 0;
}

void	CChart::SetVSelUpper(int val, int nPlotIndex)
{
	if(!m_pPara)
		return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetYZoneUpper(val));
	else if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetYZoneUpper(val);
}

double	CChart::GetHSelWidth(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetXZoneGap());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetXZoneGap();
	else
		return 0;
}

double	CChart::GetVSelWidth(int nPlotIndex)
{
	if(!m_pPara)
		return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLineLayer(m_pPara->nType))
		return SuffixT_S_L(->GetYZoneGap());
	else if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetYZoneGap();
	else
		return 0;
}

TCHAR*	CChart::GetTitle()
{
	if(!m_pPara)return _TEXT("");
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return _TEXT("");

	return (TCHAR *)SuffixT_A(->GetTitle().c_str());
}

TCHAR*	CChart::GetTitle(int nPlotIndex)
{
	if(!m_pPara)return _TEXT("");
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return _TEXT("");

	if(!CheckSubPlotIndex(nPlotIndex))return _TEXT("");

	if(IsSingleLayer(m_pPara->nType))return (TCHAR *)SuffixT_S(->GetTitle().c_str());
	else if(IsMultiLayer(m_pPara->nType))return (TCHAR *)SuffixT_M(->GetSubPlot(nPlotIndex)->GetTitle().c_str());
	else return _TEXT("");
}

void	CChart::SetTitle(const TCHAR* title)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A(->SetTitle(title));
}

void	CChart::SetTitle(const TCHAR* title, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLayer(m_pPara->nType))SuffixT_S(->SetTitle(title));
	else if(IsMultiLayer(m_pPara->nType))SuffixT_M(->GetSubPlot(nPlotIndex)->SetTitle(title));
	else return;
}

int	CChart::GetTitlePosition()
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	return SuffixT_A_NR3D(->GetTitlePosition());
}

int	CChart::GetTitlePosition(int nPlotIndex)
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLayer(m_pPara->nType))
		return SuffixT_S(->GetTitlePosition());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetTitlePosition());
	else
		return 0;
}

void	CChart::SetTitlePosition(int position)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	SuffixT_A_NR3D(->SetTitlePosition(position));
}

void	CChart::SetTitlePosition(int position, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetTitlePosition(position));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetTitlePosition(position));
	else
		return;
}

bool	CChart::IsTitleShow()
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;

	return SuffixT_A(->IsTitleShow());
}

bool	CChart::IsTitleShow(int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	
	if(!CheckSubPlotIndex(nPlotIndex))return false;

	if(IsSingleLayer(m_pPara->nType))
		return SuffixT_S(->IsTitleShow());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->IsTitleShow());
	else
		return false;
}

void	CChart::SetTitleShow(bool show)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A(->SetTitleShow(show));
}

void	CChart::SetTitleShow(bool show, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetTitleShow(show));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetTitleShow(show));
	else
		return;
}

COLORREF	CChart::GetTitleColor()
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;

	return SuffixT_A_NR3D(->GetTitleColor());
}

COLORREF	CChart::GetTitleColor(int nPlotIndex)
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	
	if(!CheckSubPlotIndex(nPlotIndex))return 0;

	if(IsSingleLayer(m_pPara->nType))
		return SuffixT_S(->GetTitleColor());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetTitleColor());
	else
		return 0;
}

void		CChart::SetTitleColor(COLORREF color)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	SuffixT_A_NR3D(->SetTitleColor(color));
}

void		CChart::SetTitleColor(COLORREF color, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetTitleColor(color));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetTitleColor(color));
	else
		return;
}

LOGFONT	CChart::GetTitleFont()
{
	LOGFONT logFont={0};
	HFONT hfont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
	if (hfont!=NULL)
	{
		::GetObject(hfont, sizeof(LOGFONT), &logFont);
		::DeleteObject(hfont);
	}

	if(!m_pPara)return logFont;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return logFont;
	
	return SuffixT_A_NR3D_Def(->GetTitleFont(), logFont);
}

LOGFONT	CChart::GetTitleFont(int nPlotIndex)
{
	LOGFONT logFont={0};
	HFONT hfont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
	if (hfont!=NULL)
	{
		::GetObject(hfont, sizeof(LOGFONT), &logFont);
		::DeleteObject(hfont);
	}

	if(!m_pPara)return logFont;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return logFont;
	
	if(!CheckSubPlotIndex(nPlotIndex))return logFont;
	
	if(IsSingleLayer(m_pPara->nType))
		return SuffixT_S_Def(->GetTitleFont(), logFont);
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M_Def(->GetSubPlot(nPlotIndex)->GetTitleFont(), logFont);
	else
		return logFont;
}

void	CChart::SetTitleFont(LOGFONT logFont)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetTitleFont(logFont));
}

void	CChart::SetTitleFont(LOGFONT logFont, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetTitleFont(logFont));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetTitleFont(logFont));
	else
		return;
}

void	CChart::SetShowParentTitle(bool set)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType == kTypeLayered)((CLayeredPlotImplI*)m_pPara->pPlot)->SetShowParentTitle(set);
	else if(m_pPara->nType == kTypeShareX)((CShareXPlotImplI*)m_pPara->pPlot)->SetShowParentTitle(set);
}

// 获得图像副标题
TCHAR*	CChart::GetSubTitle()
{
	if(!m_pPara)return _TEXT("");
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return _TEXT("");
	
	return (TCHAR *)SuffixT_A_NR3D(->GetSubTitle().c_str());
}
// 获得子图副标题，可用于分裂视图、分层视图、共享X轴视图
TCHAR*	CChart::GetSubTitle(int nPlotIndex)
{
	if(!m_pPara)return _TEXT("");
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return _TEXT("");
	
	if(!CheckSubPlotIndex(nPlotIndex))return _TEXT("");
	
	if(IsSingleLayer(m_pPara->nType))return (TCHAR *)SuffixT_S(->GetSubTitle().c_str());
	else if(IsMultiLayer(m_pPara->nType))return (TCHAR *)SuffixT_M(->GetSubPlot(nPlotIndex)->GetSubTitle().c_str());
	else return _TEXT("");
}
// 设置图像副标题
void	CChart::SetSubTitle(const TCHAR* title)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	SuffixT_A_NR3D(->SetSubTitle(title));
}
// 设置子图副标题
void	CChart::SetSubTitle(const TCHAR* title, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))SuffixT_S(->SetSubTitle(title));
	else if(IsMultiLayer(m_pPara->nType))SuffixT_M(->GetSubPlot(nPlotIndex)->SetSubTitle(title));
	else return;
}
// 获得图像副标题位置
int		CChart::GetSubTitlePosition()
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	return SuffixT_A_NR3D(->GetSubTitlePosition());
}
// 获得子图副标题位置
int		CChart::GetSubTitlePosition(int nPlotIndex)
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	
	if(!CheckSubPlotIndex(nPlotIndex))return 0;
	
	if(IsSingleLayer(m_pPara->nType))
		return SuffixT_S(->GetSubTitlePosition());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetSubTitlePosition());
	else
		return 0;
}
// 设置图像副标题位置。负数：左；0：中；正数：右
void	CChart::SetSubTitlePosition(int position)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	SuffixT_A_NR3D(->SetSubTitlePosition(position));
}
// 设置子图副标题位置。负数：左；0：中；正数：右
void	CChart::SetSubTitlePosition(int position, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetSubTitlePosition(position));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetSubTitlePosition(position));
	else
		return;
}
// 图像副标题是否显示
bool	CChart::IsSubTitleShow()
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	
	return SuffixT_A_NR3D(->IsSubTitleShow());
}
// 子图副标题是否显示
bool	CChart::IsSubTitleShow(int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	
	if(!CheckSubPlotIndex(nPlotIndex))return false;
	
	if(IsSingleLayer(m_pPara->nType))
		return SuffixT_S(->IsSubTitleShow());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->IsSubTitleShow());
	else
		return false;
}
// 设置显示图像副标题
void	CChart::SetSubTitleShow(bool show)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	SuffixT_A_NR3D(->SetSubTitleShow(show));
}
// 设置显示子图副标题
void	CChart::SetSubTitleShow(bool show, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetSubTitleShow(show));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetSubTitleShow(show));
	else
		return;
}
// 获得图像副标题颜色
COLORREF	CChart::GetSubTitleColor()
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	
	return SuffixT_A_NR3D(->GetSubTitleColor());
}
// 获得子图副标题颜色
COLORREF	CChart::GetSubTitleColor(int nPlotIndex)
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	
	if(!CheckSubPlotIndex(nPlotIndex))return 0;
	
	if(IsSingleLayer(m_pPara->nType))
		return SuffixT_S(->GetSubTitleColor());
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetSubTitleColor());
	else
		return 0;
}
// 设置图像副标题颜色
void	CChart::SetSubTitleColor(COLORREF color)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	SuffixT_A_NR3D(->SetSubTitleColor(color));
}
// 设置子图副标题颜色
void	CChart::SetSubTitleColor(COLORREF color, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetSubTitleColor(color));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetSubTitleColor(color));
	else
		return;
}
// 获得副标题字体
LOGFONT	CChart::GetSubTitleFont()
{
	LOGFONT logFont={0};
	HFONT hfont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
	if (hfont!=NULL)
	{
		::GetObject(hfont, sizeof(LOGFONT), &logFont);
		::DeleteObject(hfont);
	}
	
	if(!m_pPara)return logFont;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return logFont;
	
	return SuffixT_A_NR3D_Def(->GetSubTitleFont(), logFont);
}
// 获得子图副标题字体
LOGFONT	CChart::GetSubTitleFont(int nPlotIndex)
{
	LOGFONT logFont={0};
	HFONT hfont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
	if (hfont!=NULL)
	{
		::GetObject(hfont, sizeof(LOGFONT), &logFont);
		::DeleteObject(hfont);
	}
	
	if(!m_pPara)return logFont;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return logFont;
	
	if(!CheckSubPlotIndex(nPlotIndex))return logFont;
	
	if(IsSingleLayer(m_pPara->nType))
		return SuffixT_S_Def(->GetSubTitleFont(), logFont);
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M_Def(->GetSubPlot(nPlotIndex)->GetSubTitleFont(), logFont);
	else
		return logFont;
}
// 设置副标题字体
void	CChart::SetSubTitleFont(LOGFONT logFont)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetSubTitleFont(logFont));
}
// 设置子图副标题字体
void	CChart::SetSubTitleFont(LOGFONT logFont, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetSubTitleFont(logFont));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetSubTitleFont(logFont));
	else
		return;
}

void	CChart::SetLightColor(COLORREF color)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetLightColor(color));
}

void	CChart::SetLightColor(COLORREF color, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetLightColor(color));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetLightColor(color));
	else
		return;
}

void	CChart::SetLightLineSize(int nSize)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetLightLineSize(nSize));
}

void	CChart::SetLightLineSize(int nSize, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetLightLineSize(nSize));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetLightLineSize(nSize));
	else
		return;
}

void	CChart::SetLightLineStyle(int nStyle)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetLightLineStyle(nStyle));
}

void	CChart::SetLightLineStyle(int nStyle, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetLightLineStyle(nStyle));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetLightLineStyle(nStyle));
	else
		return;
}

void	*CChart::GetAxis(int location, int nPlotIndex)
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	if(!CheckSubPlotIndex(nPlotIndex))return 0;
		
	if(IsSingleLayer(m_pPara->nType))
	{
		return SuffixT_S(->GetAxisByLocation(location));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		return SuffixT_M(->GetSubPlot(nPlotIndex)->GetAxisByLocation(location));
	}
	else
	{
		return 0;
	}
}

void	*CChart::GetAxis(int location)
{
	if(!m_pPara)return 0;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return 0;
	
	if(m_pPara->nType!=kTypeShareX)
		return GetAxis(location, 0);
	else
	{
		if(!CAxis::IsXAxis(location))
		{
			return ((CShareXPlotImplI*)m_pPara->pPlot)->GetLayer(0)->GetAxisByLocation(location);
		}
		else
		{
			return ((CShareXPlotImplI*)m_pPara->pPlot)->GetAxisByLocation(location);
		}
	}
}

void	CChart::AddAxis(int location, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->AddAxis(location));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->AddAxis(location));
}

void	CChart::SetAxisTitle(const TCHAR* title, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetTitle(title);
}

void	CChart::SetAxisTitle(const TCHAR* title, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetTitle(title);
}

void	CChart::SetAxisTitleFont(LOGFONT logFont, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetTitleFont(logFont);
}

void	CChart::SetAxisTitleFont(LOGFONT logFont, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetTitleFont(logFont);
}

void	CChart::SetAxisLabelFont(LOGFONT logFont, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetLabelFont(logFont);
}

void	CChart::SetAxisLabelFont(LOGFONT logFont, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetLabelFont(logFont);
}

void	CChart::SetAxisTitlePosition(int position, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetTitlePosition(position);
}

void	CChart::SetAxisTitlePosition(int position, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetTitlePosition(position);
}

void	CChart::SetAxisGap(int gap, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetAxisGap(gap);
}

void	CChart::SetAxisGap(int gap, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetAxisGap(gap);
}

void	CChart::SetAxisTickPosition(int pos, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetTickPosition(pos);
}

void	CChart::SetAxisTickPosition(int pos, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetTickPosition(pos);
}

void	CChart::SetAxisMinorTickPosition(int pos, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetMinorTickPosition(pos);
}

void	CChart::SetAxisMinorTickPosition(int pos, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetMinorTickPosition(pos);
}

void	CChart::SetAxisToTime(bool bTime, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetTime(bTime);
}

void	CChart::SetAxisToTime(bool bTime, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetTime(bTime);
}

void	CChart::SetAxisTimeFormat(TCHAR *format, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetTimeFormat(format);
}

void	CChart::SetAxisTimeFormat(TCHAR *format, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetTimeFormat(format);
}

void	CChart::SetAxisColorBar(bool bShow, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetShowColorBar(bShow);
}

void	CChart::SetAxisColorBar(bool bShow, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetShowColorBar(bShow);
}

void	CChart::SetAxisColor(COLORREF color, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetAxisColor(color);
}

void	CChart::SetAxisColor(COLORREF color, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetAxisColor(color);
}

void	CChart::SetAxisLabelShow(bool bShow, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetTickLabelShow(bShow);
}

void	CChart::SetAxisLabelShow(bool bShow, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetTickLabelShow(bShow);
}

void	CChart::SetAxisLabelFormat(TCHAR *format, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetLabelFormat(format);
}
void	CChart::SetAxisLabelFormat(TCHAR *format, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetLabelFormat(format);
}

void	CChart::SetAxisShow(bool bShow, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetVisible(bShow);
}

void	CChart::SetAxisShow(bool bShow, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetVisible(bShow);
}

bool	CChart::IsAxisShow(int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)return axis->IsVisible();
	else return false;
}

bool	CChart::IsAxisShow(int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)return axis->IsVisible();
	else return false;
}

void	CChart::SetAxesCompact(int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetAxesCompact());
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAxesCompact());
}

void	CChart::AddRange4LabelHiden(double low, double high, int location, int nPlotIndex)
{
	DataRange range;
	range.val[0] = low;
	range.val[1] = high;
	
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->GetRanges4LabelHiden().push_back(range);
}

void	CChart::AddRange4LabelHiden(double low, double high, int location)
{
	DataRange range;
	range.val[0] = low;
	range.val[1] = high;

	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->GetRanges4LabelHiden().push_back(range);
}

void	CChart::SetReplaceLabel(bool bReplace, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->SetReplaceLabel(bReplace);
}

void	CChart::SetReplaceLabel(bool bReplace, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->SetReplaceLabel(bReplace);
}

void	CChart::AddAlternateLabel(TCHAR *label, int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->GetAlternateLabels().push_back(label);
}

void	CChart::AddAlternateLabel(TCHAR *label, int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->GetAlternateLabels().push_back(label);
}

void	CChart::ClrAlternateLabel(int location)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location);
	if(axis)axis->GetAlternateLabels().clear();
}

void	CChart::ClrAlternateLabel(int location, int nPlotIndex)
{
	CAxisImpl *axis = (CAxisImpl *)GetAxis(location, nPlotIndex);
	if(axis)axis->GetAlternateLabels().clear();
}

void	CChart::SetXAutoTicks(bool bAuto, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetAutoMajorTickX(bAuto));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAutoMajorTickX(bAuto));
}

void	CChart::SetXAutoTicks(bool bAuto)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(m_pPara->nType!=kTypeShareX)
		SetXAutoTicks(bAuto, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->SetAutoMajorTickX(bAuto);
}

void	CChart::SetYAutoTicks(bool bAuto, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetAutoMajorTickY(bAuto));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAutoMajorTickY(bAuto));
}

void	CChart::SetYAutoTicks(bool bAuto)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetXAutoTicks(bAuto, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetAutoMajorTickY(bAuto);
}

void	CChart::SetXTickCount(int count, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetXTickCount(count));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetXTickCount(count));
}

void	CChart::SetXTickCount(int count)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetXTickCount(count, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetXTickCount(count);
}

void	CChart::SetYTickCount(int count, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetYTickCount(count));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetYTickCount(count));
}

void	CChart::SetYTickCount(int count)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetYTickCount(count, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetYTickCount(count);
}

void	CChart::SetXAutoMinorTicks(bool bAuto, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetAutoMinorTickX(bAuto));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAutoMinorTickX(bAuto));
}

void	CChart::SetXAutoMinorTicks(bool bAuto)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(m_pPara->nType!=kTypeShareX)
		SetXAutoMinorTicks(bAuto, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetAutoMinorTickX(bAuto);
}
// 子图自动设置Y坐标轴副刻度数
void	CChart::SetYAutoMinorTicks(bool bAuto, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetAutoMinorTickY(bAuto));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAutoMinorTickY(bAuto));
}

void	CChart::SetYAutoMinorTicks(bool bAuto)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetYAutoMinorTicks(bAuto, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetAutoMinorTickY(bAuto);
}

void	CChart::SetXMinorTickCount(int count, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetXMinorTickCount(count));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetXMinorTickCount(count));
}

void	CChart::SetXMinorTickCount(int count)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetXMinorTickCount(count, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetXMinorTickCount(count);
}

void	CChart::SetYMinorTickCount(int count, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetYMinorTickCount(count));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetYMinorTickCount(count));
}

void	CChart::SetYMinorTickCount(int count)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetYMinorTickCount(count, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetYMinorTickCount(count);
}

void	CChart::SetXFloatTicks(bool flt, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetFloatXTicks(flt));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetFloatXTicks(flt));
}

void	CChart::SetXFloatTicks(bool flt)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetXFloatTicks(flt, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetFloatXTicks(flt);
}

void	CChart::SetYFloatTicks(bool flt, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetFloatYTicks(flt));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetFloatYTicks(flt));
}

void	CChart::SetYFloatTicks(bool flt)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetYFloatTicks(flt, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetFloatYTicks(flt);
}

void	CChart::SetXLogarithm(bool bLog, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetXLogarithm(bLog));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetXLogarithm(bLog));
}
void	CChart::SetXLogarithm(bool bLog)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetXLogarithm(bLog, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetXLogarithm(bLog);
}

void	CChart::SetYLogarithm(bool bLog, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetYLogarithm(bLog));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetYLogarithm(bLog));
}

void	CChart::SetYLogarithm(bool bLog)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetYLogarithm(bLog, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetYLogarithm(bLog);
}

void	CChart::SetXOpposite(bool bOpposite, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetXOpposite(bOpposite));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetXOpposite(bOpposite));
}

void	CChart::SetXOpposite(bool bOpposite)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetXOpposite(bOpposite, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetXOpposite(bOpposite);
}

void	CChart::SetYOpposite(bool bOpposite, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetYOpposite(bOpposite));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetYOpposite(bOpposite));
}

void	CChart::SetYOpposite(bool bOpposite)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetYOpposite(bOpposite, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetYOpposite(bOpposite);
}

void	CChart::SetAtomX(double atom, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetAtomX(atom));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAtomX(atom));
}

void	CChart::SetAtomX(double atom)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetAtomX(atom, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetAtomX(atom);
}

void	CChart::SetAtomY(double atom, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetAtomY(atom));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAtomY(atom));
}

void	CChart::SetAtomY(double atom)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetAtomY(atom, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetAtomY(atom);
}

void	CChart::SetAtomZeroX(double zero, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetAtomZeroX(zero));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAtomZeroX(zero));
}

void	CChart::SetAtomZeroX(double zero)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetAtomZeroX(zero, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetAtomZeroX(zero);
}

void	CChart::SetAtomZeroY(double zero, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetAtomZeroY(zero));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetAtomZeroY(zero));
}

void	CChart::SetAtomZeroY(double zero)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetAtomZeroY(zero, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetAtomZeroY(zero);
}

void	CChart::SetSegsPreferX(int segs, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetSegsPreferX(segs));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetSegsPreferX(segs));
}

void	CChart::SetSegsPreferX(int segs)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetSegsPreferX(segs, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetSegsPreferX(segs);
}

void	CChart::SetSegsPreferY(int segs, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetSegsPreferY(segs));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetSegsPreferY(segs));
}

void	CChart::SetSegsPreferY(int segs)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType!=kTypeShareX)
		SetSegsPreferY(segs, 0);
	else
		((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(0)->SetSegsPreferY(segs);
}

void		CChart::SetPlotRange(double xRange[2], double yRange[2])
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(xRange[0] > xRange[1])swap(xRange[0], xRange[1]);
	if(yRange[0] > yRange[1])swap(yRange[0], yRange[1]);

	if(IsSingleLineLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
	{
		SuffixT_S_L(->SetXRange(xRange[0], xRange[1]));
		SuffixT_S_L(->SetYRange(yRange[0], yRange[1]));
	}
	else if(Is2DFieldChart(m_pPara->nType))
	{
		SuffixT_S_F(->SetContourRange(xRange, yRange));
	}
	else if(m_pPara->nType == kType3DSurface)
	{
		SuffixT_R3D_F(->SetContourRange(xRange, yRange));
	}
}

void		CChart::SetPlotRange(double xl, double xu, double yl, double yu)
{
	double xRange[2] = {xl, xu};
	double yRange[2] = {yl, yu};
	SetPlotRange(xRange, yRange);
}

void		CChart::SetXRange(double low, double high, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetXRange(low, high));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetXRange(low, high));
}

void		CChart::SetXRange(double low, double high)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(m_pPara->nType!=kTypeShareX)
		SetXRange(low, high, 0);
	else
	{
		((CShareXPlotImplI*)m_pPara->pPlot)->SetXRange(low, high);
		for(int i=0; i<((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlotCount(); i++)
		{
			((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(i)->SetXRange(low, high);
		}
		((CShareXPlotImplI*)m_pPara->pPlot)->SetOriginRange(false);
		((CShareXPlotImplI*)m_pPara->pPlot)->SetShareExact(true);
	}
}

void		CChart::SetYRange(double low, double high, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetYRange(low, high));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetYRange(low, high));
}

void		CChart::SetYRange(double low, double high)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(m_pPara->nType!=kTypeShareX)
		SetYRange(low, high, 0);
	else
	{
		((CShareXPlotImplI*)m_pPara->pPlot)->SetYRange(low, high);
		for(int i=0; i<((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlotCount(); i++)
		{
			((CShareXPlotImplI*)m_pPara->pPlot)->GetSubPlot(i)->SetYRange(low, high);
		}
	}
}

void		CChart::SetXAutoRange(bool bAuto)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NS(->SetXAutoRange(bAuto));
}

void		CChart::SetYAutoRange(bool bAuto)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	SuffixT_A_NS(->SetYAutoRange(bAuto));
}

void		CChart::SetXAutoRange(bool bAuto, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetXAutoRange(bAuto));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetXAutoRange(bAuto));
}

void		CChart::SetYAutoRange(bool bAuto, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->SetYAutoRange(bAuto));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetYAutoRange(bAuto));
}

void		CChart::SetExactXRange(bool bExact, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetExactXRange(bExact));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetExactXRange(bExact));
}

void		CChart::SetExactYRange(bool bExact, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetExactYRange(bExact));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetExactYRange(bExact));
}

void		CChart::SetOptimalExactXRange(bool bExact, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetOptimalExactXRange(bExact));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetOptimalExactXRange(bExact));
}

void		CChart::SetOptimalExactYRange(bool bExact, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetOptimalExactYRange(bExact));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetOptimalExactYRange(bExact));
}

void		CChart::SetOptimalXExtend(bool bExtend, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetOptimalXExtend(bExtend));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetOptimalXExtend(bExtend));
}

void		CChart::SetOptimalYExtend(bool bExtend, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetOptimalYExtend(bExtend));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetOptimalYExtend(bExtend));
}

void	CChart::SetStaticRange(bool sr, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetStaticRange(sr));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetStaticRange(sr));
}

void	CChart::SetStaticXRange(bool sr, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetStaticXRange(sr));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetStaticXRange(sr));
}

void	CChart::SetStaticYRange(bool sr, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetStaticYRange(sr));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetStaticYRange(sr));
}

void	CChart::SetEqualXYAxis(bool bEqual)
{
	if(!m_pPara)return;
	if(m_pPara->nType != kTypeXY || !m_pPara->pPlot)
		return;
	((CXYPlotImplI*)m_pPara->pPlot)->SetEqualXYAxis(bEqual);
}

void	CChart::SetEqualXYRange(bool bEqual)
{
	if(!m_pPara)return;
	if(m_pPara->nType != kTypeXY || !m_pPara->pPlot)
		return;
	((CXYPlotImplI*)m_pPara->pPlot)->SetEqualXYRange(bEqual);
}

void	CChart::SetEqualXYScale(bool bEqual)
{
	if(!m_pPara)return;
	if(m_pPara->nType != kTypeXY || !m_pPara->pPlot)
		return;
	((CXYPlotImplI*)m_pPara->pPlot)->SetEqualXYScale(bEqual);
}

void	CChart::SetClipXData(bool clip, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex);
	if(dataID<0)return;
	
	if(IsSingleLineLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		SuffixT_S_L(->GetAbsPlotData(nDataIndex)->SetClipXData(clip));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetAbsPlotData(nDataIndex)->SetClipXData(clip));
}

void	CChart::SetClipYData(bool clip, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	int dataID = GetDataID(nDataIndex);
	if(dataID<0)return;
	
	if(IsSingleLineLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		SuffixT_S_L(->GetAbsPlotData(nDataIndex)->SetClipYData(clip));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetAbsPlotData(nDataIndex)->SetClipYData(clip));
}

void	CChart::SetXStartTime(TCHAR *strTime, TCHAR *fmt, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;

	time_t tmt;
	if(!MyStr2Time(strTime, fmt, tmt))return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetXStartTime(tmt));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetXStartTime(tmt));
}

void	CChart::SetYStartTime(TCHAR *strTime, TCHAR *fmt, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))return;
	
	time_t tmt;
	if(!MyStr2Time(strTime, fmt, tmt))return;
	
	if(IsSingleLineLayer(m_pPara->nType))
		SuffixT_S_L(->SetYStartTime(tmt));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetYStartTime(tmt));
}

void	CChart::SetGridBindLayer(int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType != kTypeShareX || !m_pPara->pPlot)
		return;

	((CShareXPlotImplI*)m_pPara->pPlot)->SetGridBindLayer(nPlotIndex);
}

bool	CChart::GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV, int nPlotIndex/* =0 */)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))
		return false;
	if(IsSingleLayer(m_pPara->nType))
	{
		MajorH = SuffixT_S(->IsMajorHGrids());
		MajorV = SuffixT_S(->IsMajorVGrids());
		MinorH = SuffixT_S(->IsMinorHGrids());
		MinorV = SuffixT_S(->IsMinorVGrids());
		return true;
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		MajorH = SuffixT_M(->GetSubPlot(nPlotIndex)->IsMajorHGrids());
		MajorV = SuffixT_M(->GetSubPlot(nPlotIndex)->IsMajorVGrids());
		MinorH = SuffixT_M(->GetSubPlot(nPlotIndex)->IsMinorHGrids());
		MinorV = SuffixT_M(->GetSubPlot(nPlotIndex)->IsMinorVGrids());
		return true;
	}
	return false;
}

bool	CChart::GetGridLine(bool &MajorH, bool &MajorV, bool &MinorH, bool &MinorV)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(m_pPara->nType==kTypeShareX)
	{
		MajorH = ((CShareXPlotImplI*)m_pPara->pPlot)->IsMajorHGrids();
		MajorV = ((CShareXPlotImplI*)m_pPara->pPlot)->IsMajorVGrids();
		MinorH = ((CShareXPlotImplI*)m_pPara->pPlot)->IsMinorHGrids();
		MinorV = ((CShareXPlotImplI*)m_pPara->pPlot)->IsMinorVGrids();
		return true;
	}
	else
		return GetGridLine(MajorH, MajorV, MinorH, MinorV, 0);
}

void	CChart::SetGridLine(bool MajorH, bool MajorV, bool MinorH, bool MinorV, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
	{
		SuffixT_S(->SetMajorHGrids(MajorH));
		SuffixT_S(->SetMajorVGrids(MajorV));
		SuffixT_S(->SetMinorHGrids(MinorH));
		SuffixT_S(->SetMinorVGrids(MinorV));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetMajorHGrids(MajorH));
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetMajorVGrids(MajorV));
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetMinorHGrids(MinorH));
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetMinorVGrids(MinorV));
	}
}

void	CChart::SetGridLine(bool MajorH, bool MajorV, bool MinorH, bool MinorV)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(m_pPara->nType==kTypeShareX)
	{
		((CShareXPlotImplI*)m_pPara->pPlot)->SetMajorHGrids(MajorH);
		((CShareXPlotImplI*)m_pPara->pPlot)->SetMajorVGrids(MajorV);
		((CShareXPlotImplI*)m_pPara->pPlot)->SetMinorHGrids(MinorH);
		((CShareXPlotImplI*)m_pPara->pPlot)->SetMinorVGrids(MinorV);
	}
	else
		SetGridLine(MajorH, MajorV, MinorH, MinorV, 0);
}

void		CChart::SetMajorGridColor(COLORREF color)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(m_pPara->nType==kTypeShareX)
	{
		((CShareXPlotImplI*)m_pPara->pPlot)->SetMajorGridColor(color);
	}
	else
		SetMajorGridColor(color, 0);
}

void		CChart::SetMajorGridColor(COLORREF color, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
	{
		SuffixT_S(->SetMajorGridColor(color));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetMajorGridColor(color));
	}
}

void		CChart::SetMinorGridColor(COLORREF color)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType==kTypeShareX)
	{
		((CShareXPlotImplI*)m_pPara->pPlot)->SetMinorGridColor(color);
	}
	else
		SetMinorGridColor(color, 0);
}

void		CChart::SetMinorGridColor(COLORREF color, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
	{
		SuffixT_S(->SetMinorGridColor(color));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetMinorGridColor(color));
	}
}

void		CChart::SetMajorGridLineSize(int nSize)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(m_pPara->nType==kTypeShareX)
	{
		((CShareXPlotImplI*)m_pPara->pPlot)->SetMajorGridLineSize(nSize);
	}
	else
		SetMajorGridLineSize(nSize, 0);
}

void		CChart::SetMajorGridLineSize(int nSize, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
	{
		SuffixT_S(->SetMajorGridLineSize(nSize));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetMajorGridLineSize(nSize));
	}
}

void		CChart::SetMinorGridLineSize(int nSize)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(m_pPara->nType==kTypeShareX)
	{
		((CShareXPlotImplI*)m_pPara->pPlot)->SetMinorGridLineSize(nSize);
	}
	else
		SetMinorGridLineSize(nSize, 0);
}

void		CChart::SetMinorGridLineSize(int nSize, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
	{
		SuffixT_S(->SetMinorGridLineSize(nSize));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetMinorGridLineSize(nSize));
	}
}

void		CChart::SetMajorGridLineStyle(int nStyle)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(m_pPara->nType==kTypeShareX)
	{
		((CShareXPlotImplI*)m_pPara->pPlot)->SetMajorGridLineStyle(nStyle);
	}
	else
		SetMajorGridLineStyle(nStyle, 0);
}

void		CChart::SetMajorGridLineStyle(int nStyle, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
	{
		SuffixT_S(->SetMajorGridLineStyle(nStyle));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetMajorGridLineStyle(nStyle));
	}
}

void		CChart::SetMinorGridLineStyle(int nStyle)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(m_pPara->nType==kTypeShareX)
	{
		((CShareXPlotImplI*)m_pPara->pPlot)->SetMinorGridLineStyle(nStyle);
	}
	else
		SetMinorGridLineStyle(nStyle, 0);
}

void		CChart::SetMinorGridLineStyle(int nStyle, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))
	{
		SuffixT_S(->SetMinorGridLineStyle(nStyle));
	}
	else if(IsMultiLayer(m_pPara->nType))
	{
		SuffixT_M(->GetSubPlot(nPlotIndex)->SetMinorGridLineStyle(nStyle));
	}
}

COLORREF	CChart::GetBkgndColor(int nPlotIndex)
{
	if(!m_pPara)return RGB(0, 0, 0);
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return RGB(0, 0, 0);
	if(!CheckSubPlotIndex(nPlotIndex))
		return RGB(0, 0, 0);

	if(IsSingleLayer(m_pPara->nType))return SuffixT_S(->GetBkgndColor());
	else if(IsMultiLayer(m_pPara->nType))return SuffixT_M(->GetSubPlot(nPlotIndex)->GetBkgndColor());
	else return RGB(0,0,0);
}

COLORREF	CChart::GetBkgndColor()
{
	if(!m_pPara)return RGB(0, 0, 0);
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return RGB(0, 0, 0);
	
	return SuffixT_A_NR3D(->GetBkgndColor());
}

void	CChart::SetBkgndColor(COLORREF color, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(IsSingleLayer(m_pPara->nType))SuffixT_S(->SetBkgndColor(color));
	else if(IsMultiLayer(m_pPara->nType))SuffixT_M(->GetSubPlot(nPlotIndex)->SetBkgndColor(color));
	else return;
}

void	CChart::SetBkgndColor(COLORREF color)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	 SuffixT_A_NR3D(->SetBkgndColor(color));
}

bool	CChart::IsGradientBkgnd()
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	
	return SuffixT_A_NR3D(->IsGradientBkgnd());
}

void	CChart::SetGradientBkgnd(bool bGrad)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetGradientBkgnd(bGrad));
}

bool	CChart::IsEdgeShow()
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	return SuffixT_A_NR3D(->IsEdgeShow());
}

void	CChart::SetEdgeShow(bool bShow)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetEdgeShow(bShow));
}

void	CChart::SetEdgeRoundConor(bool bRound)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetRoundEdgeConor(bRound));
}

void	CChart::SetEdgeColor(COLORREF color)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NR3D(->SetEdgeColor(color));
}

void	CChart::SetEdgeWidth(int width)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(width<=0)return;

	SuffixT_A_NR3D(->SetEdgeSize(width));
}

void	CChart::SetMarginLeft(int margin)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetMarginLeft(margin));
}

void	CChart::SetMarginLeft(int margin, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypeSplit || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetMarginLeft(margin);
}

void	CChart::SetMarginRight(int margin)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetMarginRight(margin));
}

void	CChart::SetMarginRight(int margin, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypeSplit || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;
	
	((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetMarginRight(margin);
}

void	CChart::SetMarginTop(int margin)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetMarginTop(margin));
}

void	CChart::SetMarginTop(int margin, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypeSplit || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;
	
	((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetMarginTop(margin);
}

void	CChart::SetMarginBottom(int margin)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetMarginBottom(margin));
}

void	CChart::SetMarginBottom(int margin, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType!=kTypeSplit || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;
	
	((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetMarginBottom(margin);
}

void	CChart::SetBorderShow(bool bShow)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetBorderShow(bShow));
}

void	CChart::SetBorderColor(COLORREF color)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NR3D(->SetBorderColor(color));
}

void	CChart::SetBorderWidth(int width)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(width<=0)return;
	SuffixT_A_NR3D(->SetBorderSize(width));
}

bool	CChart::IsLegendShow(int nPlotIndex/* =0 */)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))
		return false;

	if(m_pPara->nType==kTypeSplit)
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->IsLegendShow();
	else
		return SuffixT_A_NS_NF_NR3D(->IsLegendShow());
}

void	CChart::SetLegendShow(bool bUse, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetLegendShow(bUse);
	else
		SuffixT_A_NS_NF_NR3D(->SetLegendShow(bUse));
}

void		CChart::SetLegendMaxRows(int nCols, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetMaxVerticalNumOfLegend(nCols);
	else
		SuffixT_A_NS_NF_NR3D(->SetMaxVerticalNumOfLegend(nCols));
}

void		CChart::SetLegendMaxCols(int nRows, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetHorizontalNumOfLegend(nRows);
	else
		SuffixT_A_NS_NF_NR3D(->SetHorizontalNumOfLegend(nRows));
}

void	CChart::SetLegendPosition(int nPos, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetLegendPosition(nPos);
	else
		SuffixT_A_NS_NF_NR3D(->SetLegendPosition(nPos));
}

void		CChart::SetLegendFont(LOGFONT logFont, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;

	if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetLegendFont(logFont);
	else
		SuffixT_A_NS_NF_NR3D(->SetLegendFont(logFont));
}

void		CChart::SetLegendFontHeight(int nHeight, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	if(!CheckSubPlotIndex(nPlotIndex))
		return;
	
	if(m_pPara->nType==kTypeSplit)
		((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->SetLegendFontHeight(nHeight);
	else
		SuffixT_A_NS_NF_NR3D(->SetLegendFontHeight(nHeight));
}

void	CChart::SetLRAxis()
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	SuffixT_A_NS_NF_NR3D(->SetLRAxis());
}

void	CChart::SetTBAxis()
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	SuffixT_A_NS_NF_NR3D(->SetTBAxis());
}

RECT	CChart::GetLastClientRect(int nPlotIndex)
{
	RECT clientRect = {0, 0, 0, 0};
	if(!m_pPara)return clientRect;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return clientRect;
	
	if(!CheckSubPlotIndex(nPlotIndex))return clientRect;
	
	if(m_pPara->nType==kTypeSplit)
	{
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetLastClientRect();
	}
	else
	{
		return GetLastClientRect();
	}
}

RECT	CChart::GetLastClientRect()
{
	RECT clientRect = {0, 0, 0, 0};
	if(!m_pPara)return clientRect;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return clientRect;
	
	return SuffixT_A_Def(->GetLastClientRect(), clientRect);
}

RECT	CChart::GetLastPlotRect(int nPlotIndex)
{
	RECT plotRect = {0, 0, 0, 0};
	if(!m_pPara)return plotRect;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return plotRect;

	if(!CheckSubPlotIndex(nPlotIndex))return plotRect;

	if(m_pPara->nType==kTypeSplit)
	{
		return ((CSplitPlotImplI<CXYPlotImplI>*)m_pPara->pPlot)->GetSubPlot(nPlotIndex)->GetLastPlotRect();
	}
	else
	{
		return GetLastPlotRect();
	}
}

RECT	CChart::GetLastPlotRect()
{
	RECT plotRect = {0, 0, 0, 0};
	if(!m_pPara)return plotRect;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return plotRect;

	return SuffixT_A_Def(->GetLastPlotRect(), plotRect);
}

void	CChart::GetLastPlotRange(double xRange[2], double yRange[2], int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->GetLastPlotRange(xRange, yRange));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetLastPlotRange(xRange, yRange));
}

void	CChart::GetLastDataRange(double xRange[2], double yRange[2], int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;

	if(!CheckSubPlotIndex(nPlotIndex))return;
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->GetDataRanges(xRange, yRange));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->GetDataRanges(xRange, yRange));
}

void		CChart::ClientToData(POINT *point, double *data, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->LPToData(point, data));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->LPToData(point, data));
}

void		CChart::DataToClient(double *data, POINT *point, int nPlotIndex)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return;
	
	if(!CheckSubPlotIndex(nPlotIndex))return;
	if(IsSingleLayer(m_pPara->nType))
		SuffixT_S(->DataToLP(data, point));
	else if(IsMultiLayer(m_pPara->nType))
		SuffixT_M(->GetSubPlot(nPlotIndex)->DataToLP(data, point));
}

void		CChart::SetShowXYMajorGrid( bool show )
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount)
		return ;

	if(!m_pPara->pPlot)return;

	switch(m_pPara->nType)
	{
	case kType3DLine:
	case kType3DSurface:
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowXYMajorGrid(show);
		break;
	}
}

void		CChart::SetShowYZMajorGrid( bool show )
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount)
		return ;
	
	switch(m_pPara->nType)
	{
	case kType3DLine:
	case kType3DSurface:
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowYZMajorGrid(show);
		break;
	}
}

void		CChart::SetShowZXMajorGrid( bool show )
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount)
		return ;
	
	if(!m_pPara->pPlot)return;

	switch(m_pPara->nType)
	{
	case kType3DLine:
	case kType3DSurface:
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowZXMajorGrid(show);
		break;
	}
}

void		CChart::SetShowXYMinorGrid( bool show )
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount)
		return ;
	
	if(!m_pPara->pPlot)return;
	
	switch(m_pPara->nType)
	{
	case kType3DLine:
	case kType3DSurface:
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowXYMinorGrid(show);
		break;
	}
}

void		CChart::SetShowYZMinorGrid( bool show )
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount)
		return ;
	
	if(!m_pPara->pPlot)return;
	
	switch(m_pPara->nType)
	{
	case kType3DLine:
	case kType3DSurface:
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowYZMinorGrid(show);
		break;
	}
}

void		CChart::SetShowZXMinorGrid( bool show )
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount)
		return ;
	
	if(!m_pPara->pPlot)return;
	
	switch(m_pPara->nType)
	{
	case kType3DLine:
	case kType3DSurface:
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowZXMinorGrid(show);
		break;
	}
}

void		CChart::SetGridLine(bool MajorXY, bool MinorXY, bool MajorYZ, bool MinorYZ, bool MajorZX, bool MinorZX)
{
	if(!m_pPara)return;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount)
		return ;
	
	switch(m_pPara->nType)
	{
	case kType3DLine:
	case kType3DSurface:
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowXYMajorGrid(MajorXY);
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowXYMinorGrid(MinorXY);
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowYZMajorGrid(MajorYZ);
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowYZMinorGrid(MinorYZ);
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowZXMajorGrid(MajorZX);
		((CTDPlotImpl *)m_pPara->pPlot)->SetShowZXMinorGrid(MinorZX);
		break;
	}
}

bool		CChart::WriteToFile(TCHAR *pathName, int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))
		return false;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		return SuffixT_S_NF(->WriteToFile(pathName));
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->WriteToFile(pathName));
	else
		return false;
}

bool		CChart::WriteOneToFile(TCHAR *pathName, int nDataIndex, int nPlotIndex)
{
	if(!m_pPara)return false;
	if(m_pPara->nType<0 || m_pPara->nType>=kTypeCount || !m_pPara->pPlot)
		return false;
	if(!CheckSubPlotIndex(nPlotIndex))
		return false;
	int dataID = GetDataID(nDataIndex);
	if(dataID<0)return false;

	if(IsSingleLayer(m_pPara->nType) && !IsFieldChart(m_pPara->nType))
		return SuffixT_S_NF(->WriteToFile(nDataIndex, pathName));
	else if(IsMultiLayer(m_pPara->nType))
		return SuffixT_M(->GetSubPlot(nPlotIndex)->WriteToFile(nDataIndex, pathName));
	else
		return false;
}

void	*CChart::GetUserPointer()
{
	if(!m_pPara)return 0;
	return m_pPara->pUser;
}

void	CChart::SetUserPointer(void *pUser)
{
	if(!m_pPara)return;
	m_pPara->pUser = pUser;
}

bool	CChart::GetUserFlag()
{
	if(!m_pPara)return false;
	return m_pPara->bUser;
}

void	CChart::SetUserFlag(bool flag)
{
	if(!m_pPara)return;
	m_pPara->bUser = flag;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Class CChartWnd

struct	CChartWnd::stChartWndPara
{
	CPlotWnd<CChart>*	pPlotWnd;
};

CChartWnd::CChartWnd()
{
	m_pPara = new stChartWndPara;
	m_pPara->pPlotWnd = new CPlotWnd<CChart>;
}

CChartWnd::~CChartWnd()
{
	Release();
	delete m_pPara;
	m_pPara = 0;
}

void	CChartWnd::Release()
{
	if(!m_pPara)return;
	if(m_pPara->pPlotWnd)
	{
		delete m_pPara->pPlotWnd;
		m_pPara->pPlotWnd = 0;
	}
}

int		CChartWnd::GetChartIndex(int nChartID)
{
	return m_pPara->pPlotWnd->GetPlotIndex(nChartID);
}

int		CChartWnd::GetChartID(int nChartIndex)
{
	return m_pPara->pPlotWnd->GetPlotID(nChartIndex);
}

CChart	*CChartWnd::GetChart(int nChartIndex)
{
	return m_pPara->pPlotWnd->GetPlot(nChartIndex);
}

CChart	*CChartWnd::SetChart(CChart *pChart, int nChartIndex)
{
	SaveChartEnv(GetChart(nChartIndex));
	CChart *OldChart = m_pPara->pPlotWnd->SetPlot(pChart, nChartIndex);
	RestoreChartEnv(GetChart(nChartIndex));
	return OldChart;
}

int		CChartWnd::GetChartType(int nChartIndex)
{
	CChart *pChart = GetChart(nChartIndex);
	if(pChart)
		return pChart->GetType();
	else
		return -1;
}

bool	CChartWnd::SetChartType(int nType, int nChartIndex)
{
	CChart *pChart = GetChart(nChartIndex);
	if(pChart)
		return pChart->CreateChart(nType);
	else
		return false;
}

int		CChartWnd::GetChartCount()
{
	return m_pPara->pPlotWnd->GetPlotCount();
}

int		CChartWnd::Attach(HWND hWnd)
{
	return m_pPara->pPlotWnd->Attach(hWnd);
}

int		CChartWnd::Attach(HWND hWnd, RECT rtClient)
{
	return m_pPara->pPlotWnd->Attach(hWnd, rtClient);
}

int		CChartWnd::AttachDlg(HWND hDlg, HWND hCtrl)
{
	return m_pPara->pPlotWnd->AttachDlg(hDlg, hCtrl);
}

int		CChartWnd::AttachDlg(HWND hDlg, UINT nCtrlID)
{
	return m_pPara->pPlotWnd->AttachDlg(hDlg, nCtrlID);
}

#define  AttchAction \
	if(ret>=0)\
	{\
		CChart *pChart =GetChart(GetChartCount()-1);\
		if(!pChart)return -1;\
		SaveChartEnv(pChart);\
		pChart->CreateChart(nType);\
		RestoreChartEnv(pChart);\
	}

int		CChartWnd::Attach(HWND hWnd, int nType)
{
	int ret = Attach(hWnd); 
	AttchAction;
	return ret;
}

int		CChartWnd::Attach(HWND hWnd, RECT rtClient, int nType)
{
	int ret = Attach(hWnd, rtClient); 
	AttchAction;
	return ret;
}

int		CChartWnd::AttachDlg(HWND hDlg, HWND hCtrl, int nType)
{
	int ret = AttachDlg(hDlg, hCtrl); 
	AttchAction;
	return ret;
}

int		CChartWnd::AttachDlg(HWND hDlg, UINT nCtrlID, int nType)
{
	int ret = AttachDlg(hDlg, nCtrlID); 
	AttchAction;
	return ret;
}


bool	CChartWnd::Detach()
{
	return m_pPara->pPlotWnd->Detach();
}

bool	CChartWnd::Detach(HWND hWnd)
{
	return m_pPara->pPlotWnd->Detach(hWnd);
}

void	CChartWnd::ReDraw()
{
	m_pPara->pPlotWnd->ReDraw();
}

void	CChartWnd::ReDraw(HWND hWnd)
{
	m_pPara->pPlotWnd->ReDraw(hWnd);
}

void	CChartWnd::ReDraw(int nChartIndex)
{
	m_pPara->pPlotWnd->ReDraw(nChartIndex);
}

void	CChartWnd::ReDraw(CChart *chart)
{
	m_pPara->pPlotWnd->ReDraw(chart);
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Implementation of ChartCtrl

static	vector<CChart *> vChart4Ctrl;
WNDPROC fcnOldWndProc4ChartCtrl;

static LRESULT CALLBACK ChartControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	
	PAINTSTRUCT ps;
	HDC hDC;
	
	switch (message) 
	{
	case WM_CREATE:
		CChart *chart;
		chart = new CChart;
		chart->CreateChart(kTypeXY);
		chart->SetBkgndColor(GetSysColor(COLOR_3DFACE));
		chart->SetEdgeShow(true);
		vChart4Ctrl.push_back(chart);
		chart->SetWnd( hWnd );
		break;
	case WM_DESTROY:
		for(i=0; i<(int)vChart4Ctrl.size(); i++)
		{
			if(vChart4Ctrl[i]->GetWnd() == hWnd)
			{
				delete vChart4Ctrl[i];
				vChart4Ctrl.erase(vChart4Ctrl.begin()+i);
				break;
			}
		}
		break;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		for(i=0; i<(int)vChart4Ctrl.size(); i++)
		{
			if(vChart4Ctrl[i]->GetWnd() == hWnd)
			{
				vChart4Ctrl[i]->OnDraw(hWnd);
				break;
			}
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MOUSEMOVE:
	case WM_MOUSELEAVE:
	case 0x020A://WM_MOUSEWHEEL:
	case WM_CONTEXTMENU:
	case WM_KEYDOWN:
	//case WM_SIZE:
		for(i=0; i<(int)vChart4Ctrl.size(); i++)
		{
			if(vChart4Ctrl[i]->GetWnd() == hWnd)
			{
				vChart4Ctrl[i]->Interactive(hWnd, message, wParam, lParam);
				break;
			}
		}
		break;
	case WM_ERASEBKGND:
		break;
	default:
		//return DefWindowProc(hWnd, message, wParam, lParam);
		return CallWindowProc(fcnOldWndProc4ChartCtrl, hWnd, message, wParam, lParam);
	}
	return 0;
}

CChart	*GetChart(HWND hWnd)
{
	CChart *chart=0;
	for(int i=0; i<(int)vChart4Ctrl.size(); i++)
	{
		if(vChart4Ctrl[i]->GetWnd() == hWnd)
		{
			chart = vChart4Ctrl[i];
			break;
		}
	}
	return chart;
}

// Attention to GetModuleHandle compares to MyGetModuleHandle,
// when in dll, MyGetModuleHandle returns the dll module,
// but GetModuleHandle(NULL) returns the caller module,
// so we cannot use MyGetModuleHandle, 
// otherwise the window class is invalid in the caller module

ATOM RegisterChartControl()
{	
	WNDCLASS wcls;
	static const TCHAR szClass[] = _TEXT("ChartCtrl");
	if (GetClassInfo(GetModuleHandle(NULL), szClass, &wcls))
	{
		return 1;
	}
	GetClassInfo(NULL, _TEXT("button"), &wcls);
	fcnOldWndProc4ChartCtrl = wcls.lpfnWndProc;
	wcls.style |=  CS_DBLCLKS; // Make it to receive double clicks
	wcls.lpfnWndProc = (WNDPROC)ChartControlProc;
	wcls.hInstance = GetModuleHandle(NULL);
	wcls.lpszClassName = szClass;
	
	return RegisterClass(&wcls);
}

HWND	CreatePopupChartWnd(HWND hWndParent, int nChartType, TCHAR *wndTitle/*=_TEXT("")*/, int x/* =0 */, int y/* =0 */, int cx/* =CW_USEDEFAULT */, int cy/* =CW_USEDEFAULT */)
{
	RegisterChartControl();

	if(hWndParent)
	{
		POINT point;
		point.x = x;
		point.y = y;
		ClientToScreen(hWndParent, &point);
		x = point.x;
		y = point.y;
	}
	
	HWND hWnd = CreateWindow(_TEXT("ChartCtrl"), wndTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_TABSTOP,
		x, y, cx, cy, hWndParent, NULL, GetModuleHandle(NULL), NULL);
	if(hWnd)
	{
		GetChart(hWnd)->CreateChart(nChartType);
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}
	return hWnd;
}

int		SimpleMsgLoop()
{
	BOOL gret;
	MSG msg;
	while((gret = GetMessage(&msg, NULL, 0, 0)))
	{		
		if(gret == -1)return -1;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
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
    HWND hWnd = CreatePopupChartWnd(pCwp->hWndParent, pCwp->nChartType, pCwp->wndTitle, pCwp->x, pCwp->y, pCwp->cx, pCwp->cy);
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

HWND	CreatePopupChartWndWithMsgLoop(HWND hWndParent, int nChartType, TCHAR *wndTitle/*=_TEXT("")*/, int x/* =0 */, int y/* =0 */, int cx/* =CW_USEDEFAULT */, int cy/* =CW_USEDEFAULT */)
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

HWND	CreateSubChartWnd(HWND hWndParent, int nChartType, TCHAR *wndTitle/*=_TEXT("")*/, int x/* =0 */, int y/* =0 */, int cx/* =CW_USEDEFAULT */, int cy/* =CW_USEDEFAULT */)
{
	RegisterChartControl();
	
	HWND hWnd = CreateWindow(_TEXT("ChartCtrl"), wndTitle, WS_CHILD | WS_VISIBLE,
		x, y, cx, cy, hWndParent, NULL, GetModuleHandle(NULL), NULL);
	if(hWnd)
	{
		GetChart(hWnd)->CreateChart(nChartType);
		ShowWindow(hWnd, SW_SHOW);
		UpdateWindow(hWnd);
	}
	return hWnd;
}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
//

double StringToTime(TCHAR *str, TCHAR *fmt)
{
	time_t ttime;
	MyStr2Time(str, fmt, ttime);
	return (double)ttime;
}


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// NameSpace ending
}