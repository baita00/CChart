#include "TemplateChart.h"
#include "TempMonitor.h"
#include "../../Source2/Plot/Extended/PlotWnd/PlotWnd.h"

namespace NsCChart{

#define DeclareFunc_0P_Ret(TP, FN) \
	TP CTemplateChart::FN( )\
	{\
		return m_pPara->pPlot->FN();\
	}
	
#define DeclareFunc_0P_Noret(TP, FN) \
	TP CTemplateChart::FN( )\
	{\
		m_pPara->pPlot->FN();\
	}

#define DeclareFunc_1P_Ret(TP, FN, T1, P1) \
	TP CTemplateChart::FN(T1 P1)\
	{\
		return m_pPara->pPlot->FN(P1);\
	}
	
#define DeclareFunc_1P_Noret(TP, FN, T1, P1) \
	TP CTemplateChart::FN(T1 P1)\
	{\
		m_pPara->pPlot->FN(P1);\
	}

#define DeclareFunc_2P_Ret(TP, FN, T1, P1, T2, P2) \
	TP CTemplateChart::FN(T1 P1, T2, P2)\
	{\
		return m_pPara->pPlot->FN(P1, P2);\
	}
#define DeclareFunc_2PH_Ret(TP, FN, T1, P1, T2, P2) \
	TP CTemplateChart::FN(T1 P1, T2 P2)\
	{\
		return m_pPara->pPlot->CHandler::FN(P1, P2);\
	}
	
#define DeclareFunc_2P_Noret(TP, FN, T1, P1, T2, P2) \
	TP CTemplateChart::FN(T1 P1, T2 P2)\
	{\
		m_pPara->pPlot->FN(P1, P2);\
	}

#define DeclareFunc_3P_Ret(TP, FN, T1, P1, T2, P2, T3, P3) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3)\
	{\
		return m_pPara->pPlot->FN(P1, P2, P3);\
	}

#define DeclareFunc_3PH_Ret(TP, FN, T1, P1, T2, P2, T3, P3) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3)\
	{\
		return m_pPara->pPlot->CHandler::FN(P1, P2, P3);\
	}
	
#define DeclareFunc_3P_Noret(TP, FN, T1, P1, T2, P2, T3, P3) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3)\
	{\
		m_pPara->pPlot->FN(P1, P2, P3);\
	}

#define DeclareFunc_4P_Ret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3, T4 P4)\
	{\
		return m_pPara->pPlot->FN(P1, P2, P3, P4);\
	}
	
#define DeclareFunc_4P_Noret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3, T4 P4)\
	{\
		m_pPara->pPlot->FN(P1, P2, P3, P4);\
	}

#define DeclareFunc_5P_Ret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5)\
	{\
		return m_pPara->pPlot->FN(P1, P2, P3, P4, P5);\
	}
	
#define DeclareFunc_5P_Noret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5)\
	{\
		m_pPara->pPlot->FN(P1, P2, P3, P4, P5);\
	}

#define DeclareFunc_6P_Ret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6)\
	{\
		return m_pPara->pPlot->FN(P1, P2, P3, P4, P5, P6);\
	}
	
#define DeclareFunc_6P_Noret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6)\
	{\
		m_pPara->pPlot->FN(P1, P2, P3, P4, P5, P6);\
	}

#define DeclareFunc_7P_Ret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7)\
	{\
	return m_pPara->pPlot->FN(P1, P2, P3, P4, P5, P6, P7);\
	}

#define DeclareFunc_7P_Noret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7)\
	{\
	m_pPara->pPlot->FN(P1, P2, P3, P4, P5, P6, P7);\
	}

#define DeclareFunc_8P_Ret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7, T8, P8) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8)\
	{\
	return m_pPara->pPlot->FN(P1, P2, P3, P4, P5, P6, P7, P8);\
	}

#define DeclareFunc_8P_Noret(TP, FN, T1, P1, T2, P2, T3, P3, T4, P4, T5, P5, T6, P6, T7, P7, T8, P8) \
	TP CTemplateChart::FN(T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8)\
	{\
	m_pPara->pPlot->FN(P1, P2, P3, P4, P5, P6, P7, P8);\
	}
	

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//

struct CTemplateChart::stTemplateChartPara
{
	CTemplateitor			*pPlot;
	bool					bAutoRedraw;
};

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Implementation of CTemplateChart

CTemplateChart::CTemplateChart()
{
	m_pPara = new stTemplateChartPara;
	m_pPara->pPlot = new CTemplatePlotImplI;

	m_pPara->bAutoRedraw = true;
}

CTemplateChart::~CTemplateChart()
{
	if(m_pPara)
	{
		delete m_pPara;
		m_pPara = 0;
	}
}

void	CTemplateChart::Release()
{
	if(m_pPara->pPlot)
	{
		delete m_pPara->pPlot;
		m_pPara->pPlot = 0;
	}
}

DeclareFunc_0P_Ret(int, GetPlotID);
DeclareFunc_1P_Noret(void, SetPlotID, int, nID);
DeclareFunc_0P_Ret(HWND, GetWnd);
DeclareFunc_1P_Noret(void, SetWnd, HWND, hWnd);
DeclareFunc_0P_Ret(bool, IsRectConfined);
DeclareFunc_1P_Noret(void, SetRectConfined, bool, confine);
DeclareFunc_0P_Ret(RECT, GetConfineRect); 
DeclareFunc_1P_Noret(void, SetConfineRect, RECT, ConfineRect);
DeclareFunc_0P_Ret(bool, IsEmpty);

DeclareFunc_0P_Noret(void, OnDraw);
DeclareFunc_1P_Noret(void, OnDraw, HWND, hWnd);
DeclareFunc_1P_Noret(void, OnDraw, HDC, hDC);
DeclareFunc_2P_Noret(void, OnDraw, HWND, hWnd, RECT, destRect);
DeclareFunc_2P_Noret(void, OnDraw, HDC, hDC, RECT, destRect);

DeclareFunc_0P_Ret(bool, IsReact);
DeclareFunc_1P_Noret(void, SetReact, bool, react);
DeclareFunc_3PH_Ret(bool, OnLButtonDown, HWND, hWnd, POINT, point, UINT, ctrlKey );
DeclareFunc_3PH_Ret(bool, OnLButtonUp, HWND, hWnd, POINT, point, UINT, ctrlKey );
DeclareFunc_3PH_Ret(bool, OnLButtonDblClk, HWND, hWnd, POINT, point, UINT, ctrlKey );
DeclareFunc_3PH_Ret(bool, OnMouseMove, HWND, hWnd, POINT, point, UINT, ctrlKey );
DeclareFunc_3PH_Ret(bool, OnMouseLeave, HWND, hWnd, POINT, point, UINT, ctrlKey );
DeclareFunc_3PH_Ret(bool, OnContextMenu, HMENU, hMenu, HWND, hWnd, POINT, point );
DeclareFunc_2PH_Ret(bool, OnKeyDown, HWND, hWnd, UINT, key );
DeclareFunc_4P_Ret(bool, OnEvent, HWND, hWnd, UINT, message, WPARAM, wParam, LPARAM, lParam);
DeclareFunc_4P_Noret(void, Interactive, HWND, hWnd, UINT, message, WPARAM, wParam, LPARAM, lParam);

DeclareFunc_1P_Noret(void, SetTitle, TCHAR*, title);
DeclareFunc_1P_Noret(void, SetTitleShow, bool, show);
DeclareFunc_1P_Noret(void, SetTitleColor, COLORREF, color);
DeclareFunc_1P_Noret(void, SetTitleFont, LOGFONT, logFont);
DeclareFunc_1P_Noret(void, SetTitleHeight, int, height);
DeclareFunc_1P_Noret(void, SetTitleFaceName, TCHAR*, facename);

DeclareFunc_0P_Ret(COLORREF, GetBkgndColor);
DeclareFunc_1P_Noret(void, SetBkgndColor, COLORREF, color);
DeclareFunc_1P_Noret(void, SetEdgeShow, bool, show);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Implementation of CTemplateChartWnd

struct	CTemplateChartWnd::stTemplateChartWndPara
{
	CPlotWnd<CTemplateChart>*	pPlotWnd;
};

CTemplateChartWnd::CTemplateChartWnd()
{
	m_pPara = new stTemplateChartWndPara;
	m_pPara->pPlotWnd = new CPlotWnd<CTemplateChart>;
}

CTemplateChartWnd::~CTemplateChartWnd()
{
	Release();
	delete m_pPara;
	m_pPara = 0;
}

void	CTemplateChartWnd::Release()
{
	if(!m_pPara)return;
	if(m_pPara->pPlotWnd)
	{
		delete m_pPara->pPlotWnd;
		m_pPara->pPlotWnd = 0;
	}
}

int		CTemplateChartWnd::GetChartIndex(int nChartID)
{
	return m_pPara->pPlotWnd->GetPlotIndex(nChartID);
}

int		CTemplateChartWnd::GetChartID(int nChartIndex)
{
	return m_pPara->pPlotWnd->GetPlotID(nChartIndex);
}

CTemplateChart	*CTemplateChartWnd::GetChart(int nChartIndex)
{
	return m_pPara->pPlotWnd->GetPlot(nChartIndex);
}

CTemplateChart	*CTemplateChartWnd::SetChart(CTemplateChart *pChart, int nChartIndex)
{
	return m_pPara->pPlotWnd->SetPlot(pChart, nChartIndex);
}

int		CTemplateChartWnd::GetChartCount()
{
	return m_pPara->pPlotWnd->GetPlotCount();
}

int		CTemplateChartWnd::Attach(HWND hWnd)
{
	return m_pPara->pPlotWnd->Attach(hWnd);
}

int		CTemplateChartWnd::Attach(HWND hWnd, RECT rtClient)
{
	return m_pPara->pPlotWnd->Attach(hWnd, rtClient);
}

int		CTemplateChartWnd::AttachDlg(HWND hDlg, HWND hCtrl)
{
	return m_pPara->pPlotWnd->AttachDlg(hDlg, hCtrl);
}

int		CTemplateChartWnd::AttachDlg(HWND hDlg, UINT nCtrlID)
{
	return m_pPara->pPlotWnd->AttachDlg(hDlg, nCtrlID);
}

bool	CTemplateChartWnd::Detach()
{
	return m_pPara->pPlotWnd->Detach();
}

bool	CTemplateChartWnd::Detach(HWND hWnd)
{
	return m_pPara->pPlotWnd->Detach(hWnd);
}

void	CTemplateChartWnd::ReDraw()
{
	m_pPara->pPlotWnd->ReDraw();
}

void	CTemplateChartWnd::ReDraw(HWND hWnd)
{
	m_pPara->pPlotWnd->ReDraw(hWnd);
}

void	CTemplateChartWnd::ReDraw(int nChartIndex)
{
	m_pPara->pPlotWnd->ReDraw(nChartIndex);
}

void	CTemplateChartWnd::ReDraw(CTemplateChart *chart)
{
	m_pPara->pPlotWnd->ReDraw(chart);
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// namespace ending
}