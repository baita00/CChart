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
#include "../../Basic/XYHandler.h"

Declare_Namespace_CChart

template<class PlotImplT>
class	CSplitHandler :	public CHandler,
						public CTitleHandler<CSplitHandler<PlotImplT> >
{
public:
	CSplitHandler();
	virtual	~CSplitHandler();
public:
	int					m_nOldRegionLBD, m_nOldRegionMM;
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
CSplitHandler<PlotImplT>::CSplitHandler()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	m_nOldRegionLBD = -1;
	m_nOldRegionMM = -1;
}

template<class PlotImplT>
CSplitHandler<PlotImplT>::~CSplitHandler()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
PlotImplT	*CSplitHandler<PlotImplT>::GetPlot()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	return pT;
}

template<class PlotImplT>
bool	CSplitHandler<PlotImplT>::OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	if(pT->IsSingleMode() && pT->GetTop()>=0 && pT->GetTop()<pT->GetPlotCount())
	{
		return pT->GetHandler(pT->GetTop())->OnLButtonDown(hDC, point, ctrlKey);
	}

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

	int rowspliter, colspliter;
	bool bcol = pT->HitColSpliter(point, colspliter);
	bool brow = pT->HitRowSpliter(point, rowspliter);
	if( bcol || brow )
	{
		RECT clientRect, clipRect;
		//GetClientRect(hWnd, &clientRect);
		clientRect = pT->GetLastClientRect();
		clipRect = clientRect;
		if(bcol && colspliter > 0)
		{
			pT->m_bDragColSpliter = true;
			pT->m_bDragRowSpliter = false;
			pT->m_nSpliterIndex = colspliter;
			pT->m_nPositionOrigin = pT->m_vnColSpliter[colspliter];
			
			clipRect.left = pT->m_vnColSpliter[colspliter-1]+40;
			clipRect.right = pT->m_vnColSpliter[colspliter+1]-40;
			ClientToScreen(hWnd,(LPPOINT)&clipRect);
			ClientToScreen(hWnd,(LPPOINT)&clipRect+1);
			ClipCursor(&clipRect);
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		}
		else if(brow && rowspliter > 0)
		{
			pT->m_bDragColSpliter = false;
			pT->m_bDragRowSpliter = true;
			pT->m_nSpliterIndex = rowspliter;
			pT->m_nPositionOrigin = pT->m_vnRowSpliter[rowspliter];
			
			clipRect.top = pT->m_vnRowSpliter[rowspliter-1]+40;
			clipRect.bottom = pT->m_vnRowSpliter[rowspliter+1]-40;
			ClientToScreen(hWnd,(LPPOINT)&clipRect);
			ClientToScreen(hWnd,(LPPOINT)&clipRect+1);
			ClipCursor(&clipRect);
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
		}
		pT->m_ptOrigin = pT->m_ptCurr = point;
		
		//ReleaseDC(hWnd, hDC);
		return true;
	}
	int idx = pT->GetPlotIndex( point );
	if( idx < 0 )
	{
		//ReleaseDC(hWnd, hDC);
		needUpdate = false;
	}
	else
	{
		bool bSel = pT->GetSubPlot(idx)->IsPlotSelected();
		pT->UnselectAll();
		pT->GetSubPlot(idx)->SetPlotSelected(!bSel);
		
		if(pT->m_vpHandlers[idx])needUpdate = pT->m_vpHandlers[idx]->OnLButtonDown(hDC, point, ctrlKey) || needUpdate;
		//pT->GetSubPlot(idx)->OnDraw(hDC, pT->GetSubPlot(idx)->GetLastClientRect());
		needUpdate = true;
	}

	needUpdate = pT->CTitleHandler<CSplitHandler>::LButtonDown(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostLButtonDown)
	{
		needUpdate = (*m_fcnPostLButtonDown)(pT, hDC, point, ctrlKey, m_pParaPostLButtonDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}

	return needUpdate;
}

template<class PlotImplT>
bool	CSplitHandler<PlotImplT>::OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	if(pT->IsSingleMode() && pT->GetTop()>=0 && pT->GetTop()<pT->GetPlotCount())
	{
		return pT->GetHandler(pT->GetTop())->OnLButtonUp(hDC, point, ctrlKey);
	}

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

	pT->m_bDragColSpliter = false;
	pT->m_bDragRowSpliter = false;
	
	int idx = pT->GetPlotIndex( point );
	if( idx < 0 )
	{
		//ReleaseDC(hWnd, hDC);
		needUpdate = false;
	}
	else
	{
		if(pT->m_vpHandlers[idx])needUpdate = pT->m_vpHandlers[idx]->OnLButtonUp(hDC, point, ctrlKey) || needUpdate;
	}

	needUpdate = pT->CTitleHandler<CSplitHandler>::LButtonUp(hDC, point, ctrlKey) || needUpdate;
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
bool	CSplitHandler<PlotImplT>::OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	if(pT->IsSingleMode() && pT->GetTop()>=0 && pT->GetTop()<pT->GetPlotCount())
	{
		if(::GetKeyState(VK_CONTROL) < 0)
		{
			pT->SetSingleMode(false);
			pT->SetTop(-1);
			return true;
		}
		return pT->GetHandler(pT->GetTop())->OnLButtonDblClk(hDC, point, ctrlKey);
	}
	if(::GetKeyState(VK_CONTROL) < 0)
	{
		pT->SetSingleMode(true);
		pT->SetTop(pT->GetPlotIndex(point));
		return true;
	}

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

	pT->m_bDragColSpliter = false;
	pT->m_bDragRowSpliter = false;
	
	int idx = pT->GetPlotIndex( point );
	if( idx < 0 )
	{
		//ReleaseDC(hWnd, hDC);
		needUpdate = false;
	}
	else
	{
		if(pT->m_vpHandlers[idx])needUpdate = pT->m_vpHandlers[idx]->OnLButtonDblClk(hDC, point, ctrlKey) || needUpdate;
	}

	needUpdate = pT->CTitleHandler<CSplitHandler>::LButtonDblClk(hDC, point, ctrlKey) || needUpdate;
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

template<class PlotImplT>
bool	CSplitHandler<PlotImplT>::OnMouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	if(pT->IsSingleMode() && pT->GetTop()>=0 && pT->GetTop()<pT->GetPlotCount())
	{
		return pT->GetHandler(pT->GetTop())->OnMouseMove(hDC, point, ctrlKey);
	}

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
	
	// deal with title rect
	RECT titleRect = pT->GetTitleRect(hDC);
	
	if(PtInRect(&titleRect, point))
	{
		pT->UnlightAll();
		pT->SetLightTitle(true);
		//ReleaseDC(hWnd, hDC);
		return true;
	}
	else
	{
		pT->SetLightTitle(false);
	}
	
	// Drag the edge
	if(pT->m_bDragColSpliter)
	{
		pT->m_ptCurr = point;
		
		int pos = pT->m_nPositionOrigin;
		pos+=(pT->m_ptCurr.x-pT->m_ptOrigin.x);
		pT->m_vnColSpliter[pT->m_nSpliterIndex] = pos;
		
		pT->CalcColOffsets(pT->GetLastPlotRect());
		
		return true;
	}
	else if(pT->m_bDragRowSpliter)
	{
		pT->m_ptCurr = point;
		
		int pos = pT->m_nPositionOrigin;
		pos+=(pT->m_ptCurr.y-pT->m_ptOrigin.y);
		pT->m_vnRowSpliter[pT->m_nSpliterIndex] = pos;
		
		pT->CalcRowOffsets(pT->GetLastPlotRect());
		//ReleaseDC(hWnd, hDC);
		return true;
	}
	
	// SetCursor
	int spliter;
	if( pT->HitColSpliter(point, spliter) )
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	}
	else if( pT->HitRowSpliter(point, spliter) )
	{
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
	}
	int idx = pT->GetPlotIndex( point );
	if( idx < 0 )
	{
		//ReleaseDC(hWnd, hDC);
		needUpdate = false;
	}
	else
	{
		if(pT->m_vpHandlers[idx])needUpdate = pT->m_vpHandlers[idx]->OnMouseMove(hDC, point, ctrlKey) || needUpdate;
	}

	needUpdate = pT->CTitleHandler<CSplitHandler>::MouseMove(hDC, point, ctrlKey) || needUpdate;
	if(m_fcnPostMouseMove)
	{
		needUpdate = (*m_fcnPostMouseMove)(pT, hDC, point, ctrlKey, m_pParaPostMouseMove, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}

	return needUpdate;
}

template<class PlotImplT>
bool	CSplitHandler<PlotImplT>::OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	if(pT->IsSingleMode() && pT->GetTop()>=0 && pT->GetTop()<pT->GetPlotCount())
	{
		return pT->GetHandler(pT->GetTop())->OnMouseLeave(hDC, point, ctrlKey);
	}

	bool needUpdate = false;

	return needUpdate;
}

template<class PlotImplT>
bool	CSplitHandler<PlotImplT>::OnContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	if(pT->IsSingleMode() && pT->GetTop()>=0 && pT->GetTop()<pT->GetPlotCount())
	{
		return pT->GetHandler(pT->GetTop())->OnContextMenu(hMenu, hDC, point);
	}
	
	SetMsgHandled(false);
	
	HWND hWnd = WindowFromDC(hDC);
	if(hWnd)ScreenToClient(hWnd, &point);
	
	DPtoLP(hDC, &point, 1);
	
	if(!PtInRect(&pT->GetLastClientRect(), point))
	{
		return false;
	}
	
	bool needUpdate = false;
	int idx = pT->GetPlotIndex( point );
	if( idx < 0 ) return false;

	if(hWnd)ClientToScreen(hWnd, &point);
	LPtoDP(hDC, &point, 1);
	if(pT->m_vpHandlers[idx])needUpdate = pT->m_vpHandlers[idx]->OnContextMenu(hMenu, hDC, point) || needUpdate;

	return needUpdate;
}

template<class PlotImplT>
bool	CSplitHandler<PlotImplT>::OnKeyDown( HDC hDC, UINT key )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!m_bReact)return false;

	if(pT->IsSingleMode() && pT->GetTop()>=0 && pT->GetTop()<pT->GetPlotCount())
	{
		switch(key)
		{
		case 'S':
		case 's':
		case VK_ESCAPE:
			pT->SetSingleMode(false);
			pT->SetTop(-1);
			return true;
		}
		return pT->GetHandler(pT->GetTop())->OnKeyDown(hDC, key);
	}

	SetMsgHandled(false);

	bool needUpdate = false;

	bool bContinue = true;;
	if(m_fcnPreKeyDown)
	{
		needUpdate = (*m_fcnPreKeyDown)(pT, hDC, key, m_pParaPreKeyDown, bContinue) || needUpdate;
		//if(bContinue)SetMsgHandled(false);
		if(!bContinue)return needUpdate;
	}
	int idx;
	switch(key)
	{
	case VK_TAB:
		pT->SwitchSelection();
		needUpdate = true;
		break;
	case VK_SPACE:
	case VK_RETURN:
		idx = pT->GetIndexOfSelectedSubPlot();
		if( idx < 0 ) return false;
		if(pT->m_vpHandlers[idx])needUpdate = pT->m_vpHandlers[idx]->OnKeyDown(hDC, key);
		break;
	case 'S':
	case 's':
		pT->SetSingleMode(true);
		pT->SetTop(pT->GetIndexOfSelectedSubPlot()>=0?pT->GetIndexOfSelectedSubPlot():0);
		needUpdate = true;
		break;
	}
	if(m_fcnPostKeyDown)
	{
		needUpdate = (*m_fcnPostKeyDown)(pT, hDC, key, m_pParaPostKeyDown, bContinue) || needUpdate;
		if(bContinue)SetMsgHandled(false);
	}
	
	return needUpdate;
}

template <class PlotImplT>
void	CSplitHandler<PlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PlotImplT *pT=static_cast<PlotImplT*>(this);
	if(!m_bReact)return;
	if(OnEvent(hWnd, message, wParam, lParam))pT->OnDraw(hWnd, pT->GetLastClientRect());
}

Declare_Namespace_End
