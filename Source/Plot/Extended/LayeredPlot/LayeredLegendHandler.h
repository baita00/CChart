#pragma once
#include <windows.h>

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

template<class HandlerT>
class	CLayeredLegendHandler
{
public:
	CLayeredLegendHandler();
	virtual	~CLayeredLegendHandler();

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
CLayeredLegendHandler<HandlerT>::CLayeredLegendHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
CLayeredLegendHandler<HandlerT>::~CLayeredLegendHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
bool	CLayeredLegendHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactLegend())return false;

	bool needUpdate = false;

	int idxOfLayer0, idxInLayer0, curve_id0;
	int idxOfLayer, idxInLayer, curve_id;
	
	int region = pT->GetPlot()->LegendRegionIdentify(hDC, point);
	if(region>=0)
	{
		if(region<pT->GetPlot()->GetPlotDataCount())
		{
			curve_id0 = pT->GetPlot()->GetIndexOfSelectedPlotData(idxOfLayer0, idxInLayer0);
			curve_id = pT->GetPlot()->GetLocalIdx(region, idxOfLayer, idxInLayer);
			
			pT->GetPlot()->UnselectAll(true, true, false);
			if(idxOfLayer != idxInLayer0 || idxInLayer != idxInLayer0)needUpdate = true;
			
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
bool	CLayeredLegendHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
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
bool	CLayeredLegendHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactLegend())return false;
	
	bool needUpdate = false;

	int idxOfLayer, idxInLayer, curve_id;
	
	int region = pT->GetPlot()->LegendRegionIdentify(hDC, point);
	if(region >=0 && region < pT->GetPlot()->GetPlotDataCount())
	{
		curve_id = pT->GetPlot()->GetLocalIdx(region, idxOfLayer, idxInLayer);
		if(curve_id>=0)
		{
			needUpdate = DefDataSettings(pT->GetPlot()->GetLayer(idxOfLayer), hDC, idxInLayer);
			pT->SetMsgHandled(true);
		}
		
	}

	return false;
}

template<class HandlerT>
bool	CLayeredLegendHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactLegend())return false;

	bool needUpdate = false;

	int idxOfLayer0, idxInLayer0, curve_id0;
	int idxOfLayer, idxInLayer, curve_id;
	
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
				curve_id0 = pT->GetPlot()->GetIndexOfLightedPlotData(idxOfLayer0, idxInLayer0);
				curve_id = pT->GetPlot()->GetLocalIdx(region, idxOfLayer, idxInLayer);
				pT->GetPlot()->SetTop(idxOfLayer);
				
				pT->GetPlot()->UnlightAll(true, true, false);
				if(idxOfLayer != idxInLayer0 || idxInLayer != idxInLayer0)needUpdate = true;
				
				if(pT->GetPlot()->GetAbsPlotData(region)->IsReact())
				{
					pT->GetPlot()->UnlightAllPlotData();
					pT->GetPlot()->GetAbsPlotData(region)->SetLighted(true);
					needUpdate = true;
				}
				pT->SetMsgHandled(true);
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
bool	CLayeredLegendHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLayeredLegendHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLayeredLegendHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

Declare_Namespace_End
