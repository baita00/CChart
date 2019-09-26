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
class CPieHandler :	public CHandler,
					public CTitleHandler<CPieHandler<PlotImplT> >,
					public CAxesHandler<CPieHandler<PlotImplT> >,
					public CDataHandler<CPieHandler<PlotImplT> >,
					public CLegendHandler<CPieHandler<PlotImplT> >,
					public CCommentsHandler<CPieHandler<PlotImplT> >,
					public CDefaultHandler<CPieHandler<PlotImplT> >
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
	CPieHandler();
	virtual	~CPieHandler();
};

///////////////////////////////////////////////////////////////////////////////////
//

template<typename PlotImplT>
CPieHandler<PlotImplT>::CPieHandler()
{
}

template<typename PlotImplT>
CPieHandler<PlotImplT>::~CPieHandler()
{
}

template<typename PlotImplT>
PlotImplT *CPieHandler<PlotImplT>::GetPlot()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	return pT;
}

template<typename PlotImplT>
bool	CPieHandler<PlotImplT>::OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey )
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

	RECT plotRect = pT->GetLastPlotRect( );
	RECT PieTitleRect;
	int i;
	for(i=0; i<pT->GetPlotDataCount(); i++ )
	{
		PieTitleRect = pT->GetPieTitleRect( hDC, plotRect, i );
		if( PtInRect( &PieTitleRect, point ) )
		{
			// Select
			if(!pT->IsMultiSelect() && pT->IsSelected(pT->GetDataID(i)))
			{
				pT->SelectAllPlotData(false);
			}
			else
			{
				pT->SetSelected(pT->GetDataID(i), !pT->IsSelected(pT->GetDataID(i)));
			}
			
			// drag
			pT->UnsetPieDragMode();
			
			pT->SetPieDragMode(pT->GetDataID(i), true);
			pT->BeginDragging(point, pT->GetPieTitleOffset(i));
			
			//ReleaseDC(hWnd, hDC);
			return true;
		}
	}
	
	if(pT->PtInPieArea(hDC, point, plotRect))
	{
		pT->SetDragMode(true);
		
		pT->BeginDragging(point,  pT->CPiePlot::m_sizeOffset);
	}
	needUpdate = pT->CTitleHandler<CPieHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CPieHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CPieHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CPieHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CPieHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CPieHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostLButtonDown)
	{
		needUpdate = (*m_fcnPostLButtonDown)(pT, hDC, point, ctrlKey, m_pParaPostLButtonDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template<typename PlotImplT>
bool	CPieHandler<PlotImplT>::OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey )
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

	pT->SetDragMode(false);
	pT->UnsetPieDragMode();

	needUpdate = pT->CTitleHandler<CPieHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CPieHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CPieHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CPieHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CPieHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CPieHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
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
bool	CPieHandler<PlotImplT>::OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
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
	
	needUpdate = pT->CTitleHandler<CPieHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CPieHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CPieHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CPieHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CPieHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	if(false)needUpdate = DefPlotSettings(GetPlot(), hDC) || needUpdate;
	needUpdate = pT->CDefaultHandler<CPieHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	
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
bool	CPieHandler<PlotImplT>::OnMouseMove( HDC hDC, POINT point, UINT ctrlKey )
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
	int title_idx = pT->GetPieDragMode();
	if(title_idx >= 0)
	{
		pT->SetDraggingPoint(point, pT->GetPieTitleOffset(title_idx));
		
		//ReleaseDC(hWnd, hDC);
		return true;
	}
	
	if(pT->IsDragMode())
	{
		pT->SetDraggingPoint(point, pT->CPiePlot::m_sizeOffset);
		
		//ReleaseDC(hWnd, hDC);
		return true;
	}
	needUpdate = pT->CTitleHandler<CPieHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CAxesHandler<CPieHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CLegendHandler<CPieHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CCommentsHandler<CPieHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDataHandler<CPieHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	needUpdate = pT->CDefaultHandler<CPieHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostMouseMove)
	{
		needUpdate = (*m_fcnPostMouseMove)(pT, hDC, point, ctrlKey, m_pParaPostMouseMove, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template<typename PlotImplT>
bool	CPieHandler<PlotImplT>::OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT *pT=static_cast<PlotImplT*>(this);

	return false;
}

template<typename PlotImplT>
bool	CPieHandler<PlotImplT>::OnContextMenu(HMENU hMenu, HDC hDC, POINT point  )
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
	needUpdate = pT->CDefaultHandler<CPieHandler>::ContextMenu(hMenu, hDC, point) || needUpdate;
	
	return needUpdate;
}

template<typename PlotImplT>
bool	CPieHandler<PlotImplT>::OnKeyDown(HDC hDC, UINT key)
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
	needUpdate = pT->CDefaultHandler<CPieHandler>::KeyDown(hDC, key) || needUpdate;
	if(m_fcnPostKeyDown)
	{
		needUpdate = (*m_fcnPostKeyDown)(pT, hDC, key, m_pParaPostKeyDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template<typename PlotImplT>
void	CPieHandler<PlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PlotImplT *pT=static_cast<PlotImplT*>(this);
	if(OnEvent(hWnd, message, wParam, lParam))pT->OnDraw(hWnd, pT->GetLastClientRect());
}

Declare_Namespace_End
