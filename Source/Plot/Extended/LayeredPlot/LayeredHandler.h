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

#include "../../Basic/Handler.h"
#include "../../Basic/TitleHandler.h"
#include "LayeredAxesHandler.h"
#include "LayeredDataHandler.h"
#include "LayeredLegendHandler.h"
#include "../../Basic/PreferHandler.h"
#include "LayeredDefaultHandler.h"
#include "../../Basic/More/CommentsHandler.h"
#include "../../Basic/More/RangeManageHandler.h"
#include "../../Basic/More/OscilloScopeHandler.h"
#include "../../Basic/More/TooltipHandler.h"

#include "../../Basic/Handlers/Dialogs/DefPlotSettingDialogs.h"
#include "../../Basic/Handlers/Dialogs/DefAxisSettingDialogs.h"
#include "../../Basic/Handlers/Dialogs/DefDataSettingDialogs.h"
#include "../../Basic/Handlers/ContextMenu/DefContextMenu.h"

Declare_Namespace_CChart

template<class PlotImplT>
class	CLayeredHandler :	public CHandler,
							public CTitleHandler<CLayeredHandler<PlotImplT> >,
							public CLayeredAxesHandler<CLayeredHandler<PlotImplT> >,
							public CLayeredDataHandler<CLayeredHandler<PlotImplT> >,
							public CLayeredLegendHandler<CLayeredHandler<PlotImplT> >,
							public CCommentsHandler<CLayeredHandler<PlotImplT> >,
							public CLayeredDefaultHandler<CLayeredHandler<PlotImplT> >
{
public:
	CLayeredHandler();
	virtual	~CLayeredHandler();
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
CLayeredHandler<PlotImplT>::CLayeredHandler()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	m_nOldRegionLBD = -1;
	m_nOldRegionMM = -1;
}

template<class PlotImplT>
CLayeredHandler<PlotImplT>::~CLayeredHandler()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
PlotImplT	*CLayeredHandler<PlotImplT>::GetPlot()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	return pT;
}

template<class PlotImplT>
bool	CLayeredHandler<PlotImplT>::OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	if(pT->IsEmpty()) return false;
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	SetMsgHandled(false);
	
	HWND hWnd = WindowFromDC(hDC);
	
	if(hWnd)
	{
		SetMyCapture(hWnd);
	}
	
	bool needUpdate = false;
	
	DPtoLP( hDC, &point, 1);

	bool bContinue = true;
	if(m_fcnPreLButtonDown)
	{
		needUpdate = (*m_fcnPreLButtonDown)(pT, hDC, point, ctrlKey, m_pParaPreLButtonDown, bContinue) || needUpdate;
		//if(bContinue)SetMsgHandled(false);
		if(!bContinue)return needUpdate;
	}
	
	needUpdate = pT->CTitleHandler<CLayeredHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredLegendHandler<CLayeredHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
//	needUpdate = pT->CCommentsHandler<CLayeredHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredAxesHandler<CLayeredHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredDataHandler<CLayeredHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredDefaultHandler<CLayeredHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostLButtonDown)
	{
		needUpdate = (*m_fcnPostLButtonDown)(pT, hDC, point, ctrlKey, m_pParaPostLButtonDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}

	return needUpdate;
}

template<class PlotImplT>
bool	CLayeredHandler<PlotImplT>::OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

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

	needUpdate = pT->CTitleHandler<CLayeredHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredLegendHandler<CLayeredHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
//	needUpdate = pT->CCommentsHandler<CLayeredHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredAxesHandler<CLayeredHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredDataHandler<CLayeredHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredDefaultHandler<CLayeredHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
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
bool	CLayeredHandler<PlotImplT>::OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

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

	needUpdate = pT->CTitleHandler<CLayeredHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredLegendHandler<CLayeredHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
//	needUpdate = pT->CCommentsHandler<CLayeredHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredAxesHandler<CLayeredHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredDataHandler<CLayeredHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredDefaultHandler<CLayeredHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
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
bool	CLayeredHandler<PlotImplT>::OnMouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	SetMsgHandled(false);

	DPtoLP( hDC, &point,1);
	
	if(!PtInRect(&pT->GetLastClientRect(), point) || pT->IsEmpty())
	{
		return false;
	}

	bool needUpdate = false;

	bool bContinue = true;;
	if(m_fcnPreMouseMove)
	{
		needUpdate = (*m_fcnPreMouseMove)(pT, hDC, point, ctrlKey, m_pParaPreMouseMove, bContinue) || needUpdate;
		//if(bContinue)SetMsgHandled(false);
		if(!bContinue)return needUpdate;
	}

	needUpdate = pT->CTitleHandler<CLayeredHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredLegendHandler<CLayeredHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
//	needUpdate = pT->CCommentsHandler<CLayeredHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredAxesHandler<CLayeredHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredDataHandler<CLayeredHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLayeredDefaultHandler<CLayeredHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostMouseMove)
	{
		needUpdate = (*m_fcnPostMouseMove)(pT, hDC, point, ctrlKey, m_pParaPostMouseMove, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}

	return needUpdate;
}

template<class PlotImplT>
bool	CLayeredHandler<PlotImplT>::OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	bool needUpdate = false;


	return needUpdate;
}

template<class PlotImplT>
bool	CLayeredHandler<PlotImplT>::OnContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	SetMsgHandled(false);
	
	HWND hWnd = WindowFromDC(hDC);
	if(hWnd)ScreenToClient(hWnd, &point);
	
	DPtoLP(hDC, &point, 1);
	
	if(!PtInRect(&pT->GetLastClientRect(), point))
	{
		return false;
	}

	if(hWnd)ClientToScreen(hWnd, &point);
	
	bool needUpdate = false;
	needUpdate = pT->CLayeredDefaultHandler<CLayeredHandler>::ContextMenu(hMenu, hDC, point) || needUpdate;

	return needUpdate;
}

template<class PlotImplT>
bool	CLayeredHandler<PlotImplT>::OnKeyDown( HDC hDC, UINT key )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	SetMsgHandled(false);

	bool needUpdate = false;

	bool bContinue = true;;
	if(m_fcnPreKeyDown)
	{
		needUpdate = (*m_fcnPreKeyDown)(pT, hDC, key, m_pParaPreKeyDown, bContinue) || needUpdate;
		//if(bContinue)SetMsgHandled(false);
		if(!bContinue)return needUpdate;
	}
//	needUpdate = pT->CLayeredDataHandler<CLayeredHandler>::KeyDown(hDC, key);
	needUpdate = pT->CLayeredDefaultHandler<CLayeredHandler>::KeyDown(hDC, key) || needUpdate;
	if(m_fcnPostKeyDown)
	{
		needUpdate = (*m_fcnPostKeyDown)(pT, hDC, key, m_pParaPostKeyDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template <class PlotImplT>
void	CLayeredHandler<PlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PlotImplT *pT=static_cast<PlotImplT*>(this);
	if(!m_bReact)return;
	if(OnEvent(hWnd, message, wParam, lParam))pT->OnDraw(hWnd, pT->GetLastClientRect());
}

Declare_Namespace_End
