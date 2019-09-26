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
#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

template<class HandlerT>
class	CLayeredDefaultHandler
{
public:
	CLayeredDefaultHandler();
	virtual	~CLayeredDefaultHandler();

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
CLayeredDefaultHandler<HandlerT>::CLayeredDefaultHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
CLayeredDefaultHandler<HandlerT>::~CLayeredDefaultHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
bool	CLayeredDefaultHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;
	
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if(region == kXYRegionOut)
	{
		pT->GetPlot()->UnselectAll(true, true, true);
		pT->SetMsgHandled(true);
		if(region != pT->m_nOldRegionLBD)needUpdate = true;

		pT->m_nOldRegionLBD = region;
	}
	return needUpdate;
}

template<class HandlerT>
bool	CLayeredDefaultHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLayeredDefaultHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	
	int top = pT->GetPlot()->GetTop();
	if(top>=0)
	{
		needUpdate = DefPlotSettings(pT->GetPlot()->GetLayer(top), hDC);
	}
	else
	{
		if(pT->GetPlot()->GetSubPlotCount()>0)
		{
			needUpdate = DefPlotSettings(pT->GetPlot()->GetLayer(0), hDC);
		}
		else
		{
			needUpdate = DefPlotSettings(pT->GetPlot(), hDC);
		}
	}
	pT->SetMsgHandled(true);
	
	return needUpdate;
}

template<class HandlerT>
bool	CLayeredDefaultHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	bool needUpdate = false;
	
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if(region == kXYRegionOut)
	{
		pT->GetPlot()->UnlightAll(true, true, true);
		pT->GetPlot()->SetLightDataRegion(false);

		pT->SetMsgHandled(true);
		if(region != pT->m_nOldRegionMM)needUpdate = true;
		
		pT->m_nOldRegionMM = region;
	}
	return needUpdate;
}

template<class HandlerT>
bool	CLayeredDefaultHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLayeredDefaultHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	bool needUpdate = false;

	if(pT->IsMsgHandled())return needUpdate;

	int top = pT->GetPlot()->GetTop();

	if(top>=0)
	{
		if( DefContextMenu( pT->GetPlot()->GetLayer(top), hMenu, hDC, point )==kPopupUpdate )
		{
			pT->SetMsgHandled(true);
			needUpdate = true ;
		}
	}
	else
	{
		if( DefContextMenu( pT->GetPlot(), hMenu, hDC, point )==kPopupUpdate )
		{
			pT->SetMsgHandled(true);
			needUpdate = true ;
		}
	}
	

	return needUpdate;
}

template<class HandlerT>
bool	CLayeredDefaultHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);
	
//	int nDataIndex;
	
	bool needUpdate = false;
	switch(key)
	{
	case VK_SPACE:
		pT->GetPlot()->SwitchSelection();
		needUpdate = true;
		break;
	case VK_RETURN:
		pT->GetPlot()->ShowDlgBySelection(hDC);
		break;
	default:
		needUpdate = false;
	}
	
	return needUpdate;
}

Declare_Namespace_End
