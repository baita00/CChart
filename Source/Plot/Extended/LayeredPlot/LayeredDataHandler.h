#pragma once
#include <windows.h>

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

template<class HandlerT>
class	CLayeredDataHandler
{
public:
	CLayeredDataHandler();
	virtual	~CLayeredDataHandler();

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
CLayeredDataHandler<HandlerT>::CLayeredDataHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
CLayeredDataHandler<HandlerT>::~CLayeredDataHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
bool	CLayeredDataHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	bool needUpdate = false;

	int curve_id, idxOfLayer, idxInLayer;
	bool bSel;

	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if(region == kXYRegionData)
	{
		curve_id=pT->GetPlot()->SelectByPoint(hDC, point, idxOfLayer, idxInLayer);
		
		if(idxOfLayer>=0)pT->GetPlot()->SetTop(idxOfLayer);
		if(curve_id>=0 )
		{
			bSel = pT->GetPlot()->GetLayer(idxOfLayer)->IsSelected(curve_id);
			pT->GetPlot()->UnselectAll();
			pT->GetPlot()->GetLayer(idxOfLayer)->SetSelected(curve_id, bSel);
			needUpdate = true;

		}
		else
		{
			pT->GetPlot()->UnselectAll();
			needUpdate = true;
		}
		
		pT->SetMsgHandled(true);

		pT->m_nOldRegionLBD = region;
	}

	return needUpdate;
}

template<class HandlerT>
bool	CLayeredDataHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLayeredDataHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);
	bool needUpdate = false;
	
	int curve_id, which, idxOfLayer, idxInLayer;
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if(region == kXYRegionData)
	{
		curve_id=pT->GetPlot()->GetNeareastPlotData(hDC,point,idxOfLayer,idxInLayer);
		
		if(idxOfLayer>=0)pT->GetPlot()->SetTop(idxOfLayer);
		if(curve_id>=0)
		{
			which=pT->GetPlot()->GetLayer(idxOfLayer)->GetIndex(curve_id);
			if(DefDataSettings(pT->GetPlot()->GetLayer(idxOfLayer),hDC,which))
			{
				needUpdate = true;
			}
			pT->SetMsgHandled(true);
		}
	}
	return needUpdate;
}

template<class HandlerT>
bool	CLayeredDataHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	static int curve_id0 = -1, idxOfLayer0 = -1, idxInLayer0 = -1;

	int curve_id, idxOfLayer, idxInLayer;
	bool bDataLighted;
	
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if( region == kXYRegionData )
	{
		if(pT->GetPlot()->IsAxisLighted() || pT->GetPlot()->IsLightTitle())
		{
			needUpdate = true;
		}
		pT->GetPlot()->UnlightAll(true, true, false);

		if(pT->GetPlot()->IsPlotDataLighted())
		{
			curve_id0 = pT->GetPlot()->GetIndexOfLightedPlotData(idxOfLayer0, idxInLayer0);

		}

		bDataLighted = pT->GetPlot()->IsPlotDataLighted();
		curve_id=pT->GetPlot()->LightByPoint(hDC,point,idxOfLayer,idxInLayer);
		pT->GetPlot()->SetTop(idxOfLayer);

		if(idxInLayer>=0 && idxInLayer != idxInLayer0)
		{
			needUpdate = true;
		}
		if(curve_id>=0)
		{
			if(idxOfLayer!=idxOfLayer0 || idxInLayer!=idxInLayer0)
			{
				pT->GetPlot()->UnlightAllPlotData();
				pT->GetPlot()->GetLayer(idxOfLayer)->SetLighted(curve_id,true);
				needUpdate = true;

				idxOfLayer0 = idxOfLayer;
				idxInLayer0 = idxInLayer;
			}
		}
		else
		{
			if(bDataLighted)
			{
				pT->GetPlot()->UnlightAllPlotData();
				needUpdate = true;
				idxInLayer0 = -1;
				idxOfLayer0 = -1;
			}
		}
		
		pT->SetMsgHandled(true);
		
		pT->m_nOldRegionMM = region;
		
	}

	return needUpdate;
}

template<class HandlerT>
bool	CLayeredDataHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLayeredDataHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLayeredDataHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

Declare_Namespace_End
