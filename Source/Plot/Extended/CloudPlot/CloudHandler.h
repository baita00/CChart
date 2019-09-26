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
#include "../../Basic/DefaultHandler.h"
#include "CloudLegendHandler.h"

#include "../../Basic/Handlers/Dialogs/DefPlotSettingDialogs.h"
#include "../../Basic/Handlers/Dialogs/DefAxisSettingDialogs.h"
#include "../../Basic/Handlers/ContextMenu/DefContextMenu.h"

Declare_Namespace_CChart

template<typename PlotImplT>
class CCloudHandler :	public CHandler,
						public CTitleHandler<CCloudHandler<PlotImplT> >,
						public CAxesHandler<CCloudHandler<PlotImplT> >,
						public CCloudLegendHandler<CCloudHandler<PlotImplT> >,
						public CDefaultHandler<CCloudHandler<PlotImplT> >
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
	CCloudHandler();
	virtual	~CCloudHandler();
};

///////////////////////////////////////////////////////////////////////////////////
//

template<typename PlotImplT>
CCloudHandler<PlotImplT>::CCloudHandler()
{
}

template<typename PlotImplT>
CCloudHandler<PlotImplT>::~CCloudHandler()
{
}

template<typename PlotImplT>
PlotImplT *CCloudHandler<PlotImplT>::GetPlot()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	return pT;
}

template<typename PlotImplT>
bool	CCloudHandler<PlotImplT>::OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey )
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
	needUpdate = pT->CTitleHandler<CCloudHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCloudLegendHandler<CCloudHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CCloudHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CCloudHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostLButtonDown)
	{
		needUpdate = (*m_fcnPostLButtonDown)(pT, hDC, point, ctrlKey, m_pParaPostLButtonDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;

	/*PlotImplT *pT=static_cast<PlotImplT *>(this);
	
	//HDC hDC = GetDC( hWnd );
	DPtoLP( hDC, &point,1);
	
	if(!PtInRect(&pT->GetLastClientRect(), point))
	{
		//ReleaseDC(hWnd, hDC);
		return false;
	}
	if(pT->IsEmpty())
	{
		//ReleaseDC(hWnd, hDC);
		return false;
	}
	
	// Capture
	HWND hWnd = WindowFromDC(hDC);
	if(hWnd)
	{
		SetMyCapture(hWnd);
	}
	
	// end

	bool needUpdate = false;	
	
	//Set Drag mode
	int which = ((CLegendImpl *)(pT->GetLegend()))->RegionIdentify(hDC,point);
	if(which>=0)
	{
		pT->GetLegend()->SetDragMode(true,point);

		ReleaseDC(hWnd, hDC);
		return true;
	}

	// Select
	CAxis *axis;
	int location;
	int region;
	region=pT->RegionIdentify(hDC,point);
	switch(region)
	{
	case kXYRegionTitle:
		pT->UnselectAllAxis();
		pT->SetSelectTitle(!pT->IsSelectTitle());
		needUpdate = true;
		break;
	case kXYRegionLeft:
		location=kLocationLeft;
		goto SelectAxis;
	case kXYRegionRight:
		location=kLocationRight;
		goto SelectAxis;
	case kXYRegionTop:
		location=kLocationTop;
		goto SelectAxis;
	case kXYRegionBottom:
		location=kLocationBottom;
SelectAxis:
		pT->SetSelectTitle(false);
		axis=pT->GetAxisByLocation(location);
		if(axis)
		{
			if(!axis->IsSelected())
				pT->UnselectAllAxis();
			axis->SetSelected(!axis->IsSelected());
		}
		needUpdate = true;
		break;
	default:
		needUpdate = false;
	}

	//	
	//ReleaseDC(hWnd, hDC);
	return false;*/
}

template<typename PlotImplT>
bool	CCloudHandler<PlotImplT>::OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey )
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
	needUpdate = pT->CTitleHandler<CCloudHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCloudLegendHandler<CCloudHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CCloudHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CCloudHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
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
bool	CCloudHandler<PlotImplT>::OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
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
	needUpdate = pT->CTitleHandler<CCloudHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCloudLegendHandler<CCloudHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CCloudHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CCloudHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
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
bool	CCloudHandler<PlotImplT>::OnMouseMove( HDC hDC, POINT point, UINT ctrlKey )
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
	needUpdate = pT->CTitleHandler<CCloudHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCloudLegendHandler<CCloudHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CCloudHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CCloudHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostMouseMove)
	{
		needUpdate = (*m_fcnPostMouseMove)(pT, hDC, point, ctrlKey, m_pParaPostMouseMove, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template<typename PlotImplT>
bool	CCloudHandler<PlotImplT>::OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT *pT=static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	return false;
}

template<typename PlotImplT>
bool	CCloudHandler<PlotImplT>::OnContextMenu(HMENU hMenu, HDC hDC, POINT point  )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;
	
	SetMsgHandled(false);
	
	//HWND hWnd = WindowFromDC(hDC);
	//if(hWnd)ScreenToClient(hWnd, &point);
	
	DPtoLP(hDC, &point, 1);
	
	if(!PtInRect(&pT->GetLastClientRect(), point))
	{
		return false;
	}
	
	bool needUpdate = false;
	needUpdate = pT->CDefaultHandler<CCloudHandler>::ContextMenu(hMenu, hDC, point) || needUpdate;
	
	return needUpdate;
}

template<typename PlotImplT>
bool	CCloudHandler<PlotImplT>::OnKeyDown(HDC hDC, UINT key)
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
	needUpdate = pT->CDefaultHandler<CCloudHandler>::KeyDown(hDC, key) || needUpdate;
	if(m_fcnPostKeyDown)
	{
		needUpdate = (*m_fcnPostKeyDown)(pT, hDC, key, m_pParaPostKeyDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template<typename PlotImplT>
void	CCloudHandler<PlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PlotImplT *pT=static_cast<PlotImplT*>(this);
	if(!m_bReact)return;
	if(OnEvent(hWnd, message, wParam, lParam))pT->OnDraw(hWnd, pT->GetLastClientRect());
}

Declare_Namespace_End
