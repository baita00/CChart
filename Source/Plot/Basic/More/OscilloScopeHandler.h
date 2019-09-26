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
#include "../CChartNameSpace.h"

Declare_Namespace_CChart

template<class HandlerT>
class	COscilloScopeHandler
{
public:
	COscilloScopeHandler();
	virtual	~COscilloScopeHandler();

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
COscilloScopeHandler<HandlerT>::COscilloScopeHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
COscilloScopeHandler<HandlerT>::~COscilloScopeHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
bool	COscilloScopeHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;

	int region = pT->GetPlot()->LegendRegionIdentify(hDC, point);
	if(region>=0)return false;

	// deal with oscilloscope
	if(pT->GetPlot()->IsOscilloScopeMode())
	{
		if(pT->GetPlot()->IsEnableZoneDrag())
		{
			int edge;
			if(pT->GetPlot()->HitZoneEdge(hDC, point, edge))
			{
				//memset(pT->GetPlot()->m_bDragZoneEdge, 0, 4*sizeof(bool));
				///pT->GetPlot()->m_bDragZoneEdge[edge] = true;
				pT->GetPlot()->SetCurZoneEdge(edge);
				
				HWND hWnd = WindowFromDC(hDC);
				RECT plotRect = pT->GetPlot()->GetLastPlotRect();
				ClientToScreen(hWnd, (LPPOINT)&plotRect);
				ClientToScreen(hWnd, (LPPOINT)&plotRect+1);
				ClipCursor(&plotRect);
				if(edge>=0 && edge<=1)
					SetCursor(LoadCursor(NULL, IDC_SIZEWE));
				else if(edge>=2 && edge<=3)
					SetCursor(LoadCursor(NULL, IDC_SIZENS));
				else
					return false;
				
				pT->GetPlot()->m_ptOrigin = pT->GetPlot()->m_ptCurr = point;
				
				pT->GetPlot()->m_nSteppingZoneEdgeIdx = edge;
				
				needUpdate = true;
				pT->SetMsgHandled(true);
				return needUpdate;
			}
			else
			{
				pT->GetPlot()->m_nSteppingZoneEdgeIdx = -1;
			}
		}
	}
	if(pT->GetPlot()->IsOscilloScopeMode())
	{
		// for oscilloscope
		if(pT->GetPlot()->IsStartOscilloScope() && pT->GetPlot()->IsEnableDataDrag())
		{
			int region;
			region=pT->GetPlot()->RegionIdentify(hDC,point);
			if(region == kXYRegionData)
			{
				int curve_id=pT->GetPlot()->SelectByPoint(hDC,point);
				if(curve_id >= 0)
				{
					pT->GetPlot()->UnsetPlotDataDragMode();
					
					DPtoLP( hDC, &point, 1);
					pT->GetPlot()->SetSelected(curve_id, !pT->GetPlot()->IsSelected(curve_id));
					pT->GetPlot()->SetDataDragMode(curve_id, true, point);
					pT->GetPlot()->m_nDraggingID = curve_id;
					//pT->GetPlot()->m_nTopID = curve_id;
					pT->GetPlot()->m_nIndexToHoldCursor = pT->GetPlot()->GetIndex(curve_id);
					LPtoDP( hDC, &point, 1);
					needUpdate = true;
				}
				
				//Keybord stepping
				if(curve_id>=0)
				{
					if(pT->GetPlot()->m_nSteppingID<0)
					{
						pT->GetPlot()->m_nSteppingID = curve_id;
					}
					else if(pT->GetPlot()->m_nSteppingID == curve_id)
					{
						pT->GetPlot()->m_nSteppingID = -1;
					}
					else
					{
						pT->GetPlot()->m_nSteppingID = curve_id;
					}
				}
				else
				{
					pT->GetPlot()->m_nSteppingID = -1;
				}
				pT->SetMsgHandled(true);
			}
		}
	}

	return needUpdate;
}

template<class HandlerT>
bool	COscilloScopeHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	
	if(pT->GetPlot()->IsOscilloScopeMode())
	{
		// zone
		if(pT->GetPlot()->m_bEnableZoneDrag)
		{
			if(pT->GetPlot()->m_nCurZoneEdge>=0 && pT->GetPlot()->m_nCurZoneEdge<4)
			{
				//memset(pT->GetPlot()->m_bDragZoneEdge, 0, 4*sizeof(bool));
				pT->GetPlot()->m_nCurZoneEdge = -1;
				//ClipCursor(NULL);
				//return true;
				needUpdate = true;
				pT->SetMsgHandled(true);
			}
		}
		
		//
		// for oscilloscope
		if(pT->GetPlot()->IsStartOscilloScope() && pT->GetPlot()->IsEnableDataDrag())
		{
			if(pT->GetPlot()->m_nDraggingID >= 0)
			{
				pT->GetPlot()->SetDataDragMode(pT->GetPlot()->m_nDraggingID, false, point);
				pT->GetPlot()->m_nDraggingID = -1;

				pT->SetMsgHandled(true);
			}
		}
	}
	

	return needUpdate;
}

template<class HandlerT>
bool	COscilloScopeHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	
	
	return needUpdate;
}

template<class HandlerT>
bool	COscilloScopeHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;
	
	int region = pT->GetPlot()->LegendRegionIdentify(hDC, point);
	if(region>=0)return false;

	if(pT->GetPlot()->IsOscilloScopeMode())
	{
		if(pT->GetPlot()->m_bEnableZoneDrag)
		{
			if(pT->GetPlot()->m_nCurZoneEdge>=0 && pT->GetPlot()->m_nCurZoneEdge<4)
			{
				pT->GetPlot()->m_ptCurr = point;
				double data[2];
				pT->GetPlot()->LPToData(&point, data );
				
				if(pT->GetPlot()->m_nCurZoneEdge == 0)
				{
					pT->GetPlot()->m_fXZoneLower = data[0];
				}
				else if(pT->GetPlot()->m_nCurZoneEdge == 1)
				{
					pT->GetPlot()->m_fXZoneUpper = data[0];
				}
				else if(pT->GetPlot()->m_nCurZoneEdge == 2)
				{
					pT->GetPlot()->m_fYZoneLower = data[1];
				}
				else if(pT->GetPlot()->m_nCurZoneEdge == 3)
				{
					pT->GetPlot()->m_fYZoneUpper = data[1];
				}
				
				needUpdate = true;
				pT->SetMsgHandled(true);
				return needUpdate;
			}
			// SetCursor
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			int index;
			if(pT->GetPlot()->HitZoneEdge(hDC, point, index))
			{
				if( (index == 0 || index == 1) && pT->GetPlot()->IsEnableXZone() )
				{
					SetCursor(LoadCursor(NULL, IDC_SIZEWE));
				}
				else if( (index == 2 || index == 3) && pT->GetPlot()->IsEnableYZone() )
				{
					SetCursor(LoadCursor(NULL, IDC_SIZENS));
				}
			}
		}
		
		// for oscilloscope
		if(pT->GetPlot()->IsStartOscilloScope() && pT->GetPlot()->IsEnableDataDrag())
		{
			if(pT->GetPlot()->m_nDraggingID >= 0)
			{
				//if(pT->GetPlot()->IsDragMode(pT->GetPlot()-))
				//			DPtoLP( hDC, &point,1);
				
				double dt0[2], dtn[2];
				
				pT->GetPlot()->m_ptCurr = point;
				pT->GetPlot()->LPToData(&pT->GetPlot()->m_ptOrigin, dt0);
				pT->GetPlot()->LPToData(&pT->GetPlot()->m_ptCurr, dtn);
				
				double offsetx, offsety;
				offsetx = pT->GetPlot()->m_fOffsetXOrigin + dtn[0] - dt0[0];
				offsety = pT->GetPlot()->m_fOffsetYOrigin + dtn[1] - dt0[1];
				
				pT->GetPlot()->SetPlotDataOffset(pT->GetPlot()->m_nDraggingID, offsetx, offsety);
				
				//			LPtoDP( hDC, &point,1);
				needUpdate = true;
				pT->SetMsgHandled(true);
			}
		}
	}
	if(pT->GetPlot()->IsOscilloScopeMode())
	{
		// for oscilloscope
		if(pT->GetPlot()->IsCursorByMouse())
		{
			double data[2];
			//			DPtoLP( hDC, &point,1);
			
			pT->GetPlot()->LPToData(&point, data);
			pT->GetPlot()->SetCursor(data);
			
			//			LPtoDP( hDC, &point,1);
			needUpdate = true;
			//needUpdate = false;//此处存疑，必须返回false，不然fcnPostMoustMove执行不正常，但在Legend区域内又可以。原因尚未跟踪完毕
		}
	}
	
	
	return needUpdate;
}

template<class HandlerT>
bool	COscilloScopeHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	COscilloScopeHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	COscilloScopeHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	int nDataIndex;

	bool needUpdate = false;
	switch(key)
	{
	case VK_LEFT:
		if(pT->GetPlot()->IsOscilloScopeMode() && !pT->GetPlot()->IsRangeZoomming())
		{
			// for oscilloscope
			if(pT->GetPlot()->IsStartOscilloScope())
			{
				if(pT->GetPlot()->m_nDraggingID>=0)break;

				if(pT->GetPlot()->m_nSteppingID<0)break;
				pT->GetPlot()->SteppingPlotData(pT->GetPlot()->m_nSteppingID, true, false);
				needUpdate = true;
			}
			if(pT->GetPlot()->m_nDraggingID<0 && pT->GetPlot()->m_nSteppingID<0)
			{
				// step cursor
				if(pT->GetPlot()->m_bShowVCursor)
				{
					pT->GetPlot()->SteppingCursor(true, false);
					needUpdate = true;
				}
				// step zone edge
				if(pT->GetPlot()->m_bEnableXZone)
				{
					if(pT->GetPlot()->m_nSteppingZoneEdgeIdx==0 || pT->GetPlot()->m_nSteppingZoneEdgeIdx==1)
					{
						pT->GetPlot()->SteppingZoneEdge(pT->GetPlot()->m_nSteppingZoneEdgeIdx, false);
						needUpdate = true;
					}
				}
			}
		}
		break;
	case VK_RIGHT:
		if(pT->GetPlot()->IsOscilloScopeMode() && !pT->GetPlot()->IsRangeZoomming())
		{
			// for oscilloscope
			if(pT->GetPlot()->IsStartOscilloScope())
			{
				if(pT->GetPlot()->m_nDraggingID>=0)break;
				
				if(pT->GetPlot()->m_nSteppingID<0)break;
				pT->GetPlot()->SteppingPlotData(pT->GetPlot()->m_nSteppingID, true, true);
				needUpdate = true;
			}
			if(pT->GetPlot()->m_nDraggingID<0 && pT->GetPlot()->m_nSteppingID<0)
			{
				// step cursor
				if(pT->GetPlot()->m_bShowVCursor)
				{
					pT->GetPlot()->SteppingCursor(true, true);
					needUpdate = true;
				}
				// step zone edge
				if(pT->GetPlot()->m_bEnableXZone)
				{
					if(pT->GetPlot()->m_nSteppingZoneEdgeIdx==0 || pT->GetPlot()->m_nSteppingZoneEdgeIdx==1)
					{
						pT->GetPlot()->SteppingZoneEdge(pT->GetPlot()->m_nSteppingZoneEdgeIdx, true);
						needUpdate = true;
					}
				}
			}
		}
		break;
	case VK_DOWN:
		if(pT->GetPlot()->IsOscilloScopeMode() && !pT->GetPlot()->IsRangeZoomming())
		{
			// for oscilloscope
			if(pT->GetPlot()->IsStartOscilloScope())
			{
				if(pT->GetPlot()->m_nDraggingID>=0)break;
				
				if(pT->GetPlot()->m_nSteppingID<0)break;
				pT->GetPlot()->SteppingPlotData(pT->GetPlot()->m_nSteppingID, false, false);
				needUpdate = true;
			}
			if(pT->GetPlot()->m_nDraggingID<0 && pT->GetPlot()->m_nSteppingID<0)
			{
				// step cursor
				if(pT->GetPlot()->m_bShowHCursor)
				{
					pT->GetPlot()->SteppingCursor(false, false);
					needUpdate = true;
				}
				// step zone edge
				if(pT->GetPlot()->m_bEnableXZone)
				{
					if(pT->GetPlot()->m_nSteppingZoneEdgeIdx==2 || pT->GetPlot()->m_nSteppingZoneEdgeIdx==3)
					{
						pT->GetPlot()->SteppingZoneEdge(pT->GetPlot()->m_nSteppingZoneEdgeIdx, false);
						needUpdate = true;
					}
				}
			}
		}
		break;
	case VK_UP:
		if(pT->GetPlot()->IsOscilloScopeMode() && !pT->GetPlot()->IsRangeZoomming())
		{
			// for oscilloscope
			if(pT->GetPlot()->IsStartOscilloScope())
			{
				if(pT->GetPlot()->m_nDraggingID>=0)break;
				
				if(pT->GetPlot()->m_nSteppingID<0)break;
				pT->GetPlot()->SteppingPlotData(pT->GetPlot()->m_nSteppingID, false, true);
				needUpdate = true;
			}
			if(pT->GetPlot()->m_nDraggingID<0 && pT->GetPlot()->m_nSteppingID<0)
			{
				// step cursor
				if(pT->GetPlot()->m_bShowHCursor)
				{
					pT->GetPlot()->SteppingCursor(false, true);
					needUpdate = true;
				}
				// step zone edge
				if(pT->GetPlot()->m_bEnableXZone)
				{
					if(pT->GetPlot()->m_nSteppingZoneEdgeIdx==2 || pT->GetPlot()->m_nSteppingZoneEdgeIdx==3)
					{
						pT->GetPlot()->SteppingZoneEdge(pT->GetPlot()->m_nSteppingZoneEdgeIdx, true);
						needUpdate = true;
					}
				}
			}
		}
		break;
	default:
		needUpdate = false;
	}

	return needUpdate;
}

Declare_Namespace_End
