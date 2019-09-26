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
#include "../../Basic/AxesHandler.h"
#include "../../Basic/DataHandler.h"
#include "../../Basic/LegendHandler.h"
#include "../../Basic/DefaultHandler.h"
#include "../../Basic/More/CommentsHandler.h"

#include "../../Basic/Handlers/Dialogs/DefPlotSettingDialogs.h"
#include "../../Basic/Handlers/Dialogs/DefAxisSettingDialogs.h"
#include "../../Basic/Handlers/ContextMenu/DefContextMenu.h"

Declare_Namespace_CChart

template<typename PlotImplT>
class CStemHandler :	public CHandler,
					public CTitleHandler<CStemHandler<PlotImplT> >,
					public CAxesHandler<CStemHandler<PlotImplT> >,
					public CDataHandler<CStemHandler<PlotImplT> >,
					public CLegendHandler<CStemHandler<PlotImplT> >,
					public CCommentsHandler<CStemHandler<PlotImplT> >,
					public CDefaultHandler<CStemHandler<PlotImplT> >
{
public:
	PlotImplT			*GetPlot();
public:
	bool		OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey );
	bool		OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey );
	bool		OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey );
	bool		OnMouseMove( HDC hDC, POINT point, UINT ctrlKey );
	bool		OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey );
	bool		OnContextMenu( HMENU hMenu, HDC hDC, POINT point );
	bool		OnKeyDown(HDC hDC, UINT key);
	
	void		Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
public:
	CStemHandler();
	virtual	~CStemHandler();
};

///////////////////////////////////////////////////////////////////////////////////
//

template<typename PlotImplT>
CStemHandler<PlotImplT>::CStemHandler()
{
}

template<typename PlotImplT>
CStemHandler<PlotImplT>::~CStemHandler()
{
}

template<typename PlotImplT>
PlotImplT *CStemHandler<PlotImplT>::GetPlot()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	return pT;
}

template<typename PlotImplT>
bool	CStemHandler<PlotImplT>::OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
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
	needUpdate = pT->CTitleHandler<CStemHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CStemHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CStemHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CStemHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CStemHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CStemHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostLButtonDown)
	{
		needUpdate = (*m_fcnPostLButtonDown)(pT, hDC, point, ctrlKey, m_pParaPostLButtonDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template<typename PlotImplT>
bool	CStemHandler<PlotImplT>::OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
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
	needUpdate = pT->CTitleHandler<CStemHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CStemHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CStemHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CStemHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CStemHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CStemHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostLButtonUp)
	{
		needUpdate = (*m_fcnPostLButtonUp)(pT, hDC, point, ctrlKey, m_pParaPostLButtonUp, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	if(GetCapture())ReleaseCapture();
	ClipCursor(NULL);
	return needUpdate;
}

template<typename PlotImplT>
bool	CStemHandler<PlotImplT>::OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
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
	
	needUpdate = pT->CTitleHandler<CStemHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CStemHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CStemHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CStemHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CStemHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CStemHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	if(false)needUpdate = DefPlotSettings(GetPlot(), hDC);
	if(m_fcnPostLButtonDblClk)
	{
		needUpdate = (*m_fcnPostLButtonDblClk)(pT, hDC, point, ctrlKey, m_pParaPostLButtonDblClk, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	if(GetCapture())ReleaseCapture();
	ClipCursor(NULL);
	return needUpdate;
}

template<typename PlotImplT>
bool	CStemHandler<PlotImplT>::OnMouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
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
	needUpdate = pT->CTitleHandler<CStemHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CStemHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CStemHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CStemHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CStemHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CStemHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostMouseMove)
	{
		needUpdate = (*m_fcnPostMouseMove)(pT, hDC, point, ctrlKey, m_pParaPostMouseMove, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template<typename PlotImplT>
bool	CStemHandler<PlotImplT>::OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT *pT=static_cast<PlotImplT*>(this);

	return false;
}

template<typename PlotImplT>
bool	CStemHandler<PlotImplT>::OnContextMenu(HMENU hMenu, HDC hDC, POINT point  )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
	SetMsgHandled(false);
	
	//HWND hWnd = WindowFromDC(hDC);
	//if(hWnd)ScreenToClient(hWnd, &point);
	
	DPtoLP(hDC, &point, 1);
	
	if(!PtInRect(&pT->GetLastClientRect(), point))
	{
		return false;
	}
	
	bool needUpdate = false;
	needUpdate = pT->CDefaultHandler<CStemHandler>::ContextMenu(hMenu, hDC, point) || needUpdate;
	
	return needUpdate;
}

template<typename PlotImplT>
bool	CStemHandler<PlotImplT>::OnKeyDown(HDC hDC, UINT key)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	SetMsgHandled(false);

	bool needUpdate = false;

	bool bContinue = true;;
	if(m_fcnPreKeyDown)
	{
		needUpdate = (*m_fcnPreKeyDown)(pT, hDC, key, m_pParaPreKeyDown, bContinue) || needUpdate;
		//if(bContinue)SetMsgHandled(false);
		if(!bContinue)return needUpdate;
	}
	needUpdate = pT->CDefaultHandler<CStemHandler>::KeyDown(hDC, key) || needUpdate;
	if(m_fcnPostKeyDown)
	{
		needUpdate = (*m_fcnPostKeyDown)(pT, hDC, key, m_pParaPostKeyDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template<typename PlotImplT>
void	CStemHandler<PlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PlotImplT *pT=static_cast<PlotImplT*>(this);
	if(OnEvent(hWnd, message, wParam, lParam))pT->OnDraw(hWnd, pT->GetLastClientRect());
}

Declare_Namespace_End
