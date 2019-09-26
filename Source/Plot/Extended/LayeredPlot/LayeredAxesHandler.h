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
class	CLayeredAxesHandler
{
public:
	CLayeredAxesHandler();
	virtual	~CLayeredAxesHandler();

//protected:
	int			GetLocationFromRegion( int region );
	bool		SelectAxisFromRegion( int region );
	bool		LightAxisFromRegion( int region );

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
CLayeredAxesHandler<HandlerT>::CLayeredAxesHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
CLayeredAxesHandler<HandlerT>::~CLayeredAxesHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template <typename HandlerT>
int		CLayeredAxesHandler<HandlerT>::GetLocationFromRegion( int region )
{
	int location;
	switch(region)
	{
	case kXYRegionLeft:
		location = kLocationLeft;
		break;
	case kXYRegionRight:
		location = kLocationRight;
		break;
	case kXYRegionTop:
		location = kLocationTop;
		break;
	case kXYRegionBottom:
		location = kLocationBottom;
		break;
	default:
		location = -1;
	}
	return location;
}

template <typename HandlerT>
bool	CLayeredAxesHandler<HandlerT>::SelectAxisFromRegion( int region )
{
	HandlerT *pT=static_cast<HandlerT*>(this);
	
	CAxisImpl *axis;
	
	int location = GetLocationFromRegion(region);
	if(location<0)return false;
	axis=pT->GetPlot()->GetAxisByLocation(location);
	if(axis)
	{
		if(!axis->IsSelected())
			pT->GetPlot()->UnselectAllAxis();
		axis->SetSelected(!axis->IsSelected());
	}
	return true;
}

template <typename HandlerT>
bool	CLayeredAxesHandler<HandlerT>::LightAxisFromRegion( int region )
{
	HandlerT *pT=static_cast<HandlerT*>(this);
	
	CAxis *axis;
	
	int location = GetLocationFromRegion(region);
	if(location<0)return false;

	axis=pT->GetPlot()->GetAxisByLocation(location);
	if(axis)
	{
		pT->GetPlot()->UnlightAllAxis();
		axis->SetLighted(true);
		return true;
	}
	return false;
}

template<class HandlerT>
bool	CLayeredAxesHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactAxes())return false;

	bool needUpdate = false;

	int top = pT->GetPlot()->GetIdxOfLayerByPointInAxisRegion(hDC, pT->GetPlot()->GetLastPlotRect(), point);
	pT->GetPlot()->SetTop(top);

	int location;
	CAxisImpl *axis;
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if( (region == kXYRegionLeft || region == kXYRegionRight || region == kXYRegionTop || region == kXYRegionBottom) && top>=0 )
	{
		location = GetLocationFromRegion(region);
		axis = pT->GetPlot()->GetLayer(top)->GetAxisByLocation(location);
		if(axis)
		{
			pT->GetPlot()->UnselectAll();
			pT->GetPlot()->GetLayer(top)->SelectAxisFromRegion(region);
		}
		
		needUpdate = true;

		pT->SetMsgHandled(true);

		pT->m_nOldRegionLBD = region;
	}

	return needUpdate;
}

template<class HandlerT>
bool	CLayeredAxesHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLayeredAxesHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);
	if(!pT->GetPlot()->IsReactAxes())return false;

	bool needUpdate = false;
	
	int top = pT->GetPlot()->GetIdxOfLayerByPointInAxisRegion(hDC, pT->GetPlot()->GetLastPlotRect(), point);
	pT->GetPlot()->SetTop(top);

	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if( (region == kXYRegionLeft || region == kXYRegionRight|| region == kXYRegionTop || region == kXYRegionBottom) && top>=0 )
	{
		needUpdate = DefAxisSettings(pT->GetPlot()->GetLayer(top), hDC, GetLocationFromRegion(region));
		pT->SetMsgHandled(true);
	}
	return needUpdate;
}

template<class HandlerT>
bool	CLayeredAxesHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactAxes())return false;
	
	bool needUpdate = false;
	int top = pT->GetPlot()->GetIdxOfLayerByPointInAxisRegion(hDC, pT->GetPlot()->GetLastPlotRect(), point);
	pT->GetPlot()->SetTop(top);
	
	int location;
	CAxisImpl *axis;
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if( (region == kXYRegionLeft || region == kXYRegionRight || region == kXYRegionTop || region == kXYRegionBottom) && top >=0 )
	{
		location = GetLocationFromRegion(region);
		axis = pT->GetPlot()->GetLayer(top)->GetAxisByLocation(location);
		if(axis)
		{
			pT->GetPlot()->UnlightAll();
			pT->GetPlot()->SetLightDataRegion(false);
			pT->GetPlot()->GetLayer(top)->LightAxisFromRegion(region);
		}
		
		needUpdate = true;
		
		pT->SetMsgHandled(true);
		
		pT->m_nOldRegionMM = region;
	}
/*	else if(region == kXYRegionData)
	{
		int centerAxis;
		bool bLegend;
		pT->GetPlot()->RegionIdentifyEx(hDC, point, bLegend, centerAxis);
		if(centerAxis>=0)
		{
			if(!pT->GetPlot()->GetAxisByLocation(centerAxis)->IsLighted())needUpdate = true;
			pT->GetPlot()->UnlightAllAxis();
			pT->GetPlot()->GetAxisByLocation(centerAxis)->SetLighted(true);
			pT->SetMsgHandled(true);
			
			pT->m_nOldRegionMM = region;
		}
	}*/
	return needUpdate;
}

template<class HandlerT>
bool	CLayeredAxesHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLayeredAxesHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CLayeredAxesHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

Declare_Namespace_End
