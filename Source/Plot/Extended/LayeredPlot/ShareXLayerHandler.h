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
#include "../../Basic/PreferHandler.h"
#include "../../Basic/DefaultHandler.h"

#include "../../Basic/Handlers/Dialogs/DefPlotSettingDialogs.h"
#include "../../Basic/Handlers/Dialogs/DefAxisSettingDialogs.h"
#include "../../Basic/Handlers/Dialogs/DefDataSettingDialogs.h"
#include "../../Basic/Handlers/ContextMenu/DefContextMenu.h"

Declare_Namespace_CChart

template<class PlotImplT>
class	CShareXLayerHandler :	public CHandler,
								public CTitleHandler<CShareXLayerHandler<PlotImplT> >,
								public CAxesHandler<CShareXLayerHandler<PlotImplT> >,
								public CDataHandler<CShareXLayerHandler<PlotImplT> >,
								public CDefaultHandler<CShareXLayerHandler<PlotImplT> >
{
public:
	CShareXLayerHandler();
	virtual	~CShareXLayerHandler();
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
CShareXLayerHandler<PlotImplT>::CShareXLayerHandler()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	m_nOldRegionLBD = -1;
	m_nOldRegionMM = -1;
}

template<class PlotImplT>
CShareXLayerHandler<PlotImplT>::~CShareXLayerHandler()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
PlotImplT	*CShareXLayerHandler<PlotImplT>::GetPlot()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	return pT;
}

template<class PlotImplT>
bool	CShareXLayerHandler<PlotImplT>::OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey )
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
		needUpdate = needUpdate || (*m_fcnPreLButtonDown)(pT, hDC, point, ctrlKey, m_pParaPreLButtonDown, bContinue);
		if(bContinue)SetMsgHandled(false);
	}
	
	needUpdate = needUpdate || pT->CTitleHandler<CShareXLayerHandler>::LButtonDown(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CAxesHandler<CShareXLayerHandler>::LButtonDown(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CDataHandler<CShareXLayerHandler>::LButtonDown(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CDefaultHandler<CShareXLayerHandler>::LButtonDown(hDC, point, ctrlKey);
	if(m_fcnPostLButtonDown)
	{
		needUpdate = needUpdate || (*m_fcnPostLButtonDown)(pT, hDC, point, ctrlKey, m_pParaPostLButtonDown, bContinue);
		if(bContinue)SetMsgHandled(false);
	}

	return needUpdate;
}

template<class PlotImplT>
bool	CShareXLayerHandler<PlotImplT>::OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey )
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
		needUpdate = needUpdate || (*m_fcnPreLButtonUp)(pT, hDC, point, ctrlKey, m_pParaPreLButtonUp, bContinue);
		if(bContinue)SetMsgHandled(false);
	}
	needUpdate = needUpdate || pT->CTitleHandler<CShareXLayerHandler>::LButtonUp(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CAxesHandler<CShareXLayerHandler>::LButtonUp(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CDataHandler<CShareXLayerHandler>::LButtonUp(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CDefaultHandler<CShareXLayerHandler>::LButtonUp(hDC, point, ctrlKey);
	if(m_fcnPostLButtonUp)
	{
		needUpdate = needUpdate || (*m_fcnPostLButtonUp)(pT, hDC, point, ctrlKey, m_pParaPostLButtonUp, bContinue);
		if(bContinue)SetMsgHandled(false);
	}

	if(GetCapture())ReleaseCapture();
	ClipCursor(NULL);
	return needUpdate;
}

template<class PlotImplT>
bool	CShareXLayerHandler<PlotImplT>::OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
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
		needUpdate = needUpdate || (*m_fcnPreLButtonDblClk)(pT, hDC, point, ctrlKey, m_pParaPreLButtonDblClk, bContinue);
		if(bContinue)SetMsgHandled(false);
	}
	needUpdate = needUpdate || pT->CTitleHandler<CShareXLayerHandler>::LButtonDblClk(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CAxesHandler<CShareXLayerHandler>::LButtonDblClk(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CDataHandler<CShareXLayerHandler>::LButtonDblClk(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CDefaultHandler<CShareXLayerHandler>::LButtonDblClk(hDC, point, ctrlKey);
	if(m_fcnPostLButtonDblClk)
	{
		needUpdate = needUpdate || (*m_fcnPostLButtonDblClk)(pT, hDC, point, ctrlKey, m_pParaPostLButtonDblClk, bContinue);
		if(bContinue)SetMsgHandled(false);
	}

	if(GetCapture())ReleaseCapture();
	ClipCursor(NULL);
	return needUpdate;
}

template<class PlotImplT>
bool	CShareXLayerHandler<PlotImplT>::OnMouseMove( HDC hDC, POINT point, UINT ctrlKey )
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
		needUpdate = needUpdate || (*m_fcnPreMouseMove)(pT, hDC, point, ctrlKey, m_pParaPreMouseMove, bContinue);
		if(bContinue)SetMsgHandled(false);
	}
	needUpdate = needUpdate || pT->CTitleHandler<CShareXLayerHandler>::MouseMove(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CAxesHandler<CShareXLayerHandler>::MouseMove(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CDataHandler<CShareXLayerHandler>::MouseMove(hDC, point, ctrlKey);
	needUpdate = needUpdate || pT->CDefaultHandler<CShareXLayerHandler>::MouseMove(hDC, point, ctrlKey);
	if(m_fcnPostMouseMove)
	{
		needUpdate = needUpdate || (*m_fcnPostMouseMove)(pT, hDC, point, ctrlKey, m_pParaPostMouseMove, bContinue);
		if(bContinue)SetMsgHandled(false);
	}

	return needUpdate;
}

template<class PlotImplT>
bool	CShareXLayerHandler<PlotImplT>::OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	bool needUpdate = false;

	return needUpdate;
}

template<class PlotImplT>
bool	CShareXLayerHandler<PlotImplT>::OnContextMenu( HMENU hMenu, HDC hDC, POINT point )
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
	needUpdate = needUpdate || pT->CDefaultHandler<CShareXLayerHandler>::ContextMenu(hMenu, hDC, point);

	return needUpdate;
}

template<class PlotImplT>
bool	CShareXLayerHandler<PlotImplT>::OnKeyDown( HDC hDC, UINT key )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	SetMsgHandled(false);

	bool needUpdate = false;

	bool bContinue = true;;
	if(m_fcnPreKeyDown)
	{
		needUpdate = needUpdate || (*m_fcnPreKeyDown)(pT, hDC, key, m_pParaPreKeyDown, bContinue);
		if(bContinue)SetMsgHandled(false);
	}
	needUpdate = needUpdate || pT->CDataHandler<CShareXLayerHandler>::KeyDown(hDC, key);
	needUpdate = needUpdate || pT->CDefaultHandler<CShareXLayerHandler>::KeyDown(hDC, key);
	if(m_fcnPostKeyDown)
	{
		needUpdate = needUpdate || (*m_fcnPostKeyDown)(pT, hDC, key, m_pParaPostKeyDown, bContinue);
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template <class PlotImplT>
void	CShareXLayerHandler<PlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PlotImplT *pT=static_cast<PlotImplT*>(this);
	if(OnEvent(hWnd, message, wParam, lParam))pT->OnDraw(hWnd, pT->GetLastClientRect());
}

Declare_Namespace_End
