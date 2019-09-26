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
#include <windows.h>

#include "CChartNameSpace.h"

Declare_Namespace_CChart

template<class HandlerT>
class	CLegendHandler
{
public:
	CLegendHandler();
	virtual	~CLegendHandler();

public:
	bool		LButtonDown( HDC hDC, POINT point, UINT ctrlKey );
	bool		LButtonUp( HDC hDC, POINT point, UINT ctrlKey );
	bool		LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey );
	bool		MouseMove( HDC hDC, POINT point, UINT ctrlKey );
	bool		MouseLeave( HDC hDC, POINT point, UINT ctrlKey );
	bool		ContextMenu( HMENU hMenu, HDC hDC, POINT point );
	bool		KeyDown( HDC hDC, UINT key );
};

template<class HandlerT>
CLegendHandler<HandlerT>::CLegendHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
CLegendHandler<HandlerT>::~CLegendHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
bool	CLegendHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactLegend())return false;

	bool needUpdate = false;
	
	int region = pT->GetPlot()->LegendRegionIdentify(hDC, point);
	if(region>=0)
	{
		if(region<pT->GetPlot()->GetPlotDataCount())
		{
			int old_id = pT->GetPlot()->GetSelectedID();
			
			pT->GetPlot()->UnselectAll(true, true, false);
			if(region != pT->GetPlot()->GetIndex(old_id))needUpdate = true;
			
			if(pT->GetPlot()->GetAbsPlotData(region)->IsReact())
			{
				bool bCur = pT->GetPlot()->GetAbsPlotData(region)->IsSelected();
				pT->GetPlot()->SelectAllPlotData(false);
				pT->GetPlot()->GetAbsPlotData(region)->SetSelected(!bCur);
			}
		}
		if(pT->GetPlot()->IsLegendConfined())
		{
			RECT plotRect = pT->GetPlot()->GetLastPlotRect();
			ClientToScreen(WindowFromDC(hDC), (LPPOINT)&plotRect);
			ClientToScreen(WindowFromDC(hDC), (LPPOINT)&plotRect+1);
			ClipCursor(&plotRect);
		}
		pT->GetPlot()->SetLegendDragMode(true);
		pT->GetPlot()->BeginDragging(point, pT->GetPlot()->GetLegendOffset());
		pT->SetMsgHandled(true);
		needUpdate = true;
	}
/*	if(region == pT->GetPlot()->GetPlotDataCount())
	{
		if(pT->GetPlot()->IsLegendConfined())
		{
			RECT plotRect = pT->GetPlot()->GetLastPlotRect();
			ClientToScreen(WindowFromDC(hDC), (LPPOINT)&plotRect);
			ClientToScreen(WindowFromDC(hDC), (LPPOINT)&plotRect+1);
			ClipCursor(&plotRect);
		}
		pT->GetPlot()->SetLegendDragMode(true);
		pT->GetPlot()->BeginDragging(point, pT->GetPlot()->GetLegendOffset());
		pT->SetMsgHandled(true);
		needUpdate = true;
	}*/
	return needUpdate;
}

template<class HandlerT>
bool	CLegendHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactLegend())return false;
	
	bool needUpdate = false;
	
	if(pT->GetPlot()->IsLegendDragMode())
	{
		pT->GetPlot()->SetLegendDragMode(false);
		pT->SetMsgHandled(true);
		needUpdate = true;
		ClipCursor(NULL);
	}

	return needUpdate;
}

template<class HandlerT>
bool	CLegendHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactLegend())return false;
	
	bool needUpdate = false;
	
	int region = pT->GetPlot()->LegendRegionIdentify(hDC, point);
	if(region >=0 && region < pT->GetPlot()->GetPlotDataCount())
	{
		needUpdate = DefDataSettings(pT->GetPlot(), hDC, region);
		pT->SetMsgHandled(true);
	}

	return false;
}

template<class HandlerT>
bool	CLegendHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactLegend())return false;

	bool needUpdate = false;
	
	if(pT->GetPlot()->IsLegendDragMode())
	{
		pT->GetPlot()->SetDraggingPoint(point, pT->GetPlot()->GetLegendOffset());
		needUpdate = true;
		pT->SetMsgHandled(true);
	}
	else
	{
		bool bLighed = pT->GetPlot()->IsLegendLighted();
		int region = pT->GetPlot()->LegendRegionIdentify(hDC, point);
		if(region>=0)
		{
			pT->GetPlot()->SetLegendLighted(true);
			if(!bLighed)needUpdate=true;

			if(region<pT->GetPlot()->GetPlotDataCount())
			{
				int old_id = pT->GetPlot()->GetLightedID();
				if(!pT->GetPlot()->GetAbsPlotData(region)->IsReact())
				{
					needUpdate = false;
				}
				else if(region!=pT->GetPlot()->GetIndex(old_id))
				{
					pT->GetPlot()->UnlightAllPlotData();
					pT->GetPlot()->SetLighted(pT->GetPlot()->GetDataID(region),true);
					needUpdate = true;
					pT->SetMsgHandled(true);
				}
			}
		}
		else
		{
			pT->GetPlot()->SetLegendLighted(false);
			if(bLighed)needUpdate=true;
		}
	}

	return needUpdate;
}

template<class HandlerT>
bool	CLegendHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLegendHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLegendHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

Declare_Namespace_End
