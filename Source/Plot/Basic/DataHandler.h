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
class	CDataHandler
{
public:
	CDataHandler();
	virtual	~CDataHandler();

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
CDataHandler<HandlerT>::CDataHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
CDataHandler<HandlerT>::~CDataHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
bool	CDataHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;
	
	int curve_id, old_id, point_idx, old_idx;
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if(region == kXYRegionData)
	{
		old_id = pT->GetPlot()->GetSelectedID();

		pT->GetPlot()->UnselectAll(true, true, false);
		curve_id=pT->GetPlot()->SelectByPoint(hDC,point);
		if(curve_id != old_id)needUpdate = true;

		if(curve_id>=0)
		{
			if(!pT->GetPlot()->GetPlotData(curve_id)->IsReact())pT->GetPlot()->SelectAllPlotData(false);
		
			if(pT->GetPlot()->IsPointSelectionMode())
			{
				old_idx = pT->GetPlot()->GetPlotData(curve_id)->GetIdxPtSelected();
				point_idx = pT->GetPlot()->GetNeareastDP( curve_id, hDC, point);
				if(point_idx != old_idx)
				{
					pT->GetPlot()->GetPlotData(curve_id)->SetIdxPtSelected(point_idx);
					needUpdate = true;
				}
			}
			
		}
		else
		{
			if(pT->GetPlot()->IsPointSelectionMode())
			{
				pT->GetPlot()->UnselectDataPoints();
			}
			
		}
		pT->SetMsgHandled(true);
		
		if(region != pT->m_nOldRegionLBD)needUpdate = true;

		pT->m_nOldRegionLBD = region;
		
	}
	return needUpdate;
}

template<class HandlerT>
bool	CDataHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CDataHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);
	
	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	
	int curve_id;
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if(region == kXYRegionData)
	{
		curve_id=pT->GetPlot()->GetDataIDByPoint(hDC,point);
		if(curve_id>=0)
		{
			needUpdate = DefDataSettings(pT->GetPlot(), hDC, pT->GetPlot()->GetIndex(curve_id));
			pT->SetMsgHandled(true);
		}
	}
	return needUpdate;
}

template<class HandlerT>
bool	CDataHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;

	static int old_id = -1, old_idx = -1;
	int curve_id, point_idx;
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if(region == kXYRegionData)
	{
		pT->GetPlot()->UnlightAll(true, true, false);
		
		curve_id=pT->GetPlot()->GetDataIDByPoint(hDC,point);
				
		if(curve_id>=0)
		{
			if(!pT->GetPlot()->GetPlotData(curve_id)->IsReact())
			{
				needUpdate = false;
			}
			else if(curve_id!=old_id)
			{
				pT->GetPlot()->UnlightAllPlotData();
				pT->GetPlot()->SetLighted(curve_id,true);
				needUpdate = true;
			}

			// add light point
			if(pT->GetPlot()->IsPointSelectionMode())
			{
				old_idx = pT->GetPlot()->GetPlotData(curve_id)->GetIdxPtLighted();
				point_idx = pT->GetPlot()->GetNeareastDP( curve_id, hDC, point);
				if(point_idx != old_idx)
				{
					pT->GetPlot()->GetPlotData(curve_id)->SetIdxPtLighted(point_idx);
					needUpdate = true;
				}
			}
			
		}
		else
		{
			pT->GetPlot()->UnlightAllPlotData();
			if(curve_id!=old_id)
			{
				needUpdate = true;
			}

			if(pT->GetPlot()->IsPointSelectionMode())
			{
				pT->GetPlot()->UnlightDataPoints();
				if(old_idx>=0)
				{
					old_idx = -1;
					needUpdate = true;
				}
			}
			
		}
		
		old_id=curve_id;

		bool lightDR = pT->GetPlot()->IsLightDataRegion();
		pT->GetPlot()->SetLightDataRegion(true);
		if(!lightDR)needUpdate = true;

		pT->SetMsgHandled(true);

		pT->m_nOldRegionMM = region;
	}
	return needUpdate;
}

template<class HandlerT>
bool	CDataHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CDataHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CDataHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);
	
	int nDataIndex;
	
	bool needUpdate = false;
	switch(key)
	{
	case VK_SPACE:
		pT->GetPlot()->SwitchSelection();
		needUpdate = true;
		break;
	case VK_RETURN:
		if(pT->GetPlot()->IsSelectTitle())
		{
			if(DefPlotSettings(pT->GetPlot(),hDC))
			{
				needUpdate = true;
			}
		}
		else if(pT->GetPlot()->IsAxisSelected())
		{
			int index, location;
			index = pT->GetPlot()->GetIndexOfSelectedAxis();
			location = pT->GetPlot()->GetAxis(index)->GetLocation();
			if(DefAxisSettings(pT->GetPlot(),hDC,location))
			{
				needUpdate = true;
			}
		}
		else if(pT->GetPlot()->IsPlotDataSelected())
		{
			int which;
			which = pT->GetPlot()->GetIndexOfSelected();
			if(DefDataSettings(pT->GetPlot(),hDC,which))
			{
				needUpdate = true;
			}
		}
		else
		{
			if(DefPlotSettings(pT->GetPlot(),hDC))
			{
				needUpdate = true;
			}
		}
		break;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case VK_NUMPAD0:
	case VK_NUMPAD1:
	case VK_NUMPAD2:
	case VK_NUMPAD3:
	case VK_NUMPAD4:
	case VK_NUMPAD5:
	case VK_NUMPAD6:
	case VK_NUMPAD7:
	case VK_NUMPAD8:
	case VK_NUMPAD9:
		if(key>='0' && key<='9')
			nDataIndex = key - '0';
		else
			nDataIndex = key - VK_NUMPAD0;
		
		if(nDataIndex>=0 && nDataIndex<pT->GetPlot()->GetPlotDataCount())
		{
			if(!pT->GetPlot()->IsSelected(pT->GetPlot()->GetDataID(nDataIndex)))
			{
				pT->GetPlot()->UnlightAll();
				pT->GetPlot()->UnselectAll();
				
				pT->GetPlot()->SetSelected(pT->GetPlot()->GetDataID(nDataIndex), true);
			}
			else
			{
				pT->GetPlot()->SetSelected(pT->GetPlot()->GetDataID(nDataIndex), false);
			}
		}
		needUpdate = true;
		break;
	default:
		needUpdate = false;
	}
	
	return needUpdate;
}

Declare_Namespace_End