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
#pragma warning(disable:4786)

#include "Handler.h"
#include "TitleHandler.h"
#include "AxesHandler.h"
#include "DataHandler.h"
#include "LegendHandler.h"
#include "PreferHandler.h"
#include "DefaultHandler.h"
#include "More/CommentsHandler.h"
#include "More/RangeManageHandler.h"
#include "More/OscilloScopeHandler.h"
#include "More/TooltipHandler.h"

#include "Handlers/Dialogs/DefPlotSettingDialogs.h"
#include "Handlers/Dialogs/DefAxisSettingDialogs.h"
#include "Handlers/Dialogs/DefDataSettingDialogs.h"
#include "Handlers/ContextMenu/DefContextMenu.h"

Declare_Namespace_CChart

template<class PlotImplT>
class	CXYHandler :	public CHandler,
						public CTitleHandler<CXYHandler<PlotImplT> >,
						public CAxesHandler<CXYHandler<PlotImplT> >,
						public CDataHandler<CXYHandler<PlotImplT> >,
						public CLegendHandler<CXYHandler<PlotImplT> >,
						public CCommentsHandler<CXYHandler<PlotImplT> >,
						public CRangeManageHandler<CXYHandler<PlotImplT> >,
						public CDefaultHandler<CXYHandler<PlotImplT> >,
						public COscilloScopeHandler<CXYHandler<PlotImplT> >,
						public CTooltipHandler<CXYHandler<PlotImplT> >
{
public:
	CXYHandler();
	virtual	~CXYHandler();
public:
	PlotImplT			*GetPlot();

public:
	virtual bool		OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey );
	virtual bool		OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey );
	virtual bool		OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey );
	virtual bool		OnMouseMove( HDC hDC, POINT point, UINT ctrlKey );
	virtual bool		OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey );
	virtual bool		OnContextMenu( HMENU hMenu, HDC hDC, POINT point );
	virtual bool		OnKeyDown( HDC hDC, UINT key );

	virtual	void		Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

template<class PlotImplT>
CXYHandler<PlotImplT>::CXYHandler()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	m_nOldRegionLBD = -1;
	m_nOldRegionMM = -1;
}

template<class PlotImplT>
CXYHandler<PlotImplT>::~CXYHandler()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
PlotImplT	*CXYHandler<PlotImplT>::GetPlot()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	return pT;
}

template<class PlotImplT>
bool	CXYHandler<PlotImplT>::OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	if(pT->IsEmpty()) return false;
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	HWND hWnd = WindowFromDC(hDC);
	if(!hWnd)return false;

	SetMsgHandled(false);
	
	SetMyCapture(hWnd);
	
	bool needUpdate = false;
	
	DPtoLP( hDC, &point, 1);

	bool bContinue = true;
	if(m_fcnPreLButtonDown)
	{
		needUpdate = (*m_fcnPreLButtonDown)(pT, hDC, point, ctrlKey, m_pParaPreLButtonDown, bContinue) || needUpdate ;
		//if(bContinue)SetMsgHandled(false);
		if(!bContinue)return needUpdate;
	}
	
	needUpdate = pT->CRangeManageHandler<CXYHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->COscilloScopeHandler<CXYHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CTitleHandler<CXYHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CXYHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CXYHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CXYHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CXYHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CTooltipHandler<CXYHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CXYHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostLButtonDown)
	{
		needUpdate = (*m_fcnPostLButtonDown)(pT, hDC, point, ctrlKey, m_pParaPostLButtonDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}

	return needUpdate;
}

template<class PlotImplT>
bool	CXYHandler<PlotImplT>::OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	HWND hWnd = WindowFromDC(hDC);
	if(!hWnd)return false;

	SetMsgHandled(false);

	DPtoLP(hDC, &point, 1);
		
	bool	needUpdate = false;
	if(!PtInRect(&pT->GetLastClientRect(), point))
	{
		if(GetCapture())ReleaseCapture();
		ClipCursor(NULL);
		return false;
	}

	bool bContinue = true;
	if(m_fcnPreLButtonUp)
	{
		needUpdate = (*m_fcnPreLButtonUp)(pT, hDC, point, ctrlKey, m_pParaPreLButtonUp, bContinue) || needUpdate;
		//if(bContinue)SetMsgHandled(false);
		if(!bContinue)
		{
			if(GetCapture())ReleaseCapture();
			ClipCursor(NULL);
			return needUpdate;
		}
	}
	needUpdate = pT->CRangeManageHandler<CXYHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->COscilloScopeHandler<CXYHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CTitleHandler<CXYHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CXYHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CXYHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CXYHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CXYHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CTooltipHandler<CXYHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CXYHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostLButtonUp)
	{
		needUpdate = (*m_fcnPostLButtonUp)(pT, hDC, point, ctrlKey, m_pParaPostLButtonUp, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}

	if(GetCapture())ReleaseCapture();
	ClipCursor(NULL);
	return needUpdate;
}

template<class PlotImplT>
bool	CXYHandler<PlotImplT>::OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	HWND hWnd = WindowFromDC(hDC);
	if(!hWnd)return false;

	SetMsgHandled(false);

	DPtoLP(hDC, &point, 1);
		
	bool needUpdate = false;
	if(!PtInRect(&pT->GetLastClientRect(), point))
	{
		//ReleaseDC(hWnd, hDC);
		if(GetCapture())ReleaseCapture();
		ClipCursor(NULL);
		return false;
	}

	bool bContinue = true;;
	if(m_fcnPreLButtonDblClk)
	{
		needUpdate = (*m_fcnPreLButtonDblClk)(pT, hDC, point, ctrlKey, m_pParaPreLButtonDblClk, bContinue) || needUpdate;
		//if(bContinue)SetMsgHandled(false);
		if(!bContinue)
		{
			if(GetCapture())ReleaseCapture();
			ClipCursor(NULL);
			return needUpdate;
		}
	}
	needUpdate = pT->CRangeManageHandler<CXYHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->COscilloScopeHandler<CXYHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CTitleHandler<CXYHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CXYHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CXYHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CXYHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CXYHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CTooltipHandler<CXYHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CXYHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostLButtonDblClk)
	{
		needUpdate = (*m_fcnPostLButtonDblClk)(pT, hDC, point, ctrlKey, m_pParaPostLButtonDblClk, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}

	if(GetCapture())ReleaseCapture();
	ClipCursor(NULL);
	return needUpdate;
}

template<class PlotImplT>
bool	CXYHandler<PlotImplT>::OnMouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	HWND hWnd = WindowFromDC(hDC);
	if(!hWnd)return false;

	SetMsgHandled(false);

	DPtoLP( hDC, &point,1);
	
	if(!PtInRect(&pT->GetLastClientRect(), point) || pT->IsEmpty())
	{
		return false;
	}

	if (m_bMouseTrack)     // 若允许 追踪，则。 
	{
		TRACKMOUSEEVENT csTME;
		csTME.cbSize = sizeof (csTME);
		csTME.dwFlags = TME_LEAVE|TME_HOVER;
		csTME.hwndTrack = hWnd ;// 指定要 追踪 的窗口 
		csTME.dwHoverTime = 10;  // 鼠标在按钮上停留超过 10ms ，才认为状态为 HOVER
		::_TrackMouseEvent (&csTME); // 开启 Windows 的 WM_MOUSELEAVE ， WM_MOUSEHOVER 事件支持
		
		
		m_bMouseTrack=FALSE ;   // 若已经 追踪 ，则停止 追踪 
	}

	bool needUpdate = false;

	bool bContinue = true;;
	if(m_fcnPreMouseMove)
	{
		needUpdate = (*m_fcnPreMouseMove)(pT, hDC, point, ctrlKey, m_pParaPreMouseMove, bContinue) || needUpdate;
		//if(bContinue)SetMsgHandled(false);
		if(!bContinue)return needUpdate;
	}
	needUpdate = pT->CRangeManageHandler<CXYHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->COscilloScopeHandler<CXYHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CTitleHandler<CXYHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CXYHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CXYHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CXYHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CXYHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CTooltipHandler<CXYHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CXYHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostMouseMove)
	{
		needUpdate = (*m_fcnPostMouseMove)(pT, hDC, point, ctrlKey, m_pParaPostMouseMove, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}

	return needUpdate;
}

template<class PlotImplT>
bool	CXYHandler<PlotImplT>::OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	HWND hWnd = WindowFromDC(hDC);
	if(!hWnd)return false;

	bool needUpdate = false;

	needUpdate = pT->CTooltipHandler<CXYHandler>::MouseLeave(hDC, point, ctrlKey) || needUpdate;

	m_bMouseTrack = true;

	return needUpdate;
}

template<class PlotImplT>
bool	CXYHandler<PlotImplT>::OnContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	HWND hWnd = WindowFromDC(hDC);
	if(!hWnd)return false;

	SetMsgHandled(false);
	
	//HWND hWnd = WindowFromDC(hDC);
	//if(hWnd)ScreenToClient(hWnd, &point);
	
	DPtoLP(hDC, &point, 1);
	
	POINT pt_screen=point;
	ScreenToClient(hWnd, &pt_screen);
	if(!PtInRect(&pT->GetLastClientRect(), pt_screen))
	{
		return false;
	}
	
	bool needUpdate = false;
	needUpdate = pT->CDefaultHandler<CXYHandler>::ContextMenu(hMenu, hDC, point) || needUpdate;

	return needUpdate;
}

template<class PlotImplT>
bool	CXYHandler<PlotImplT>::OnKeyDown( HDC hDC, UINT key )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	HWND hWnd = WindowFromDC(hDC);
	if(!hWnd)return false;

	SetMsgHandled(false);

	bool needUpdate = false;

	bool bContinue = true;;
	if(m_fcnPreKeyDown)
	{
		needUpdate = (*m_fcnPreKeyDown)(pT, hDC, key, m_pParaPreKeyDown, bContinue) || needUpdate;
		//if(bContinue)SetMsgHandled(false);
		if(!bContinue)return needUpdate;
	}
	needUpdate = pT->CDataHandler<CXYHandler>::KeyDown(hDC, key) || needUpdate;
	needUpdate = pT->CDefaultHandler<CXYHandler>::KeyDown(hDC, key) || needUpdate;
	if(m_fcnPostKeyDown)
	{
		needUpdate = (*m_fcnPostKeyDown)(pT, hDC, key, m_pParaPostKeyDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template <class PlotImplT>
void	CXYHandler<PlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PlotImplT *pT=static_cast<PlotImplT*>(this);
	if(!m_bReact)return;
	//pT->MyLock();
	if(OnEvent(hWnd, message, wParam, lParam))pT->OnDraw(hWnd, pT->GetLastClientRect());
	//pT->MyUnlock();
}

Declare_Namespace_End
