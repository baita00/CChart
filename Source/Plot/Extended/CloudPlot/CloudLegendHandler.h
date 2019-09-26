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
class	CCloudLegendHandler
{
public:
	CCloudLegendHandler();
	virtual	~CCloudLegendHandler();

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
CCloudLegendHandler<HandlerT>::CCloudLegendHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
CCloudLegendHandler<HandlerT>::~CCloudLegendHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
bool	CCloudLegendHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactLegend())return false;

	bool needUpdate = false;
	
	int region = pT->GetPlot()->LegendRegionIdentify(hDC, point);
	if(region == 0)
	{
		pT->GetPlot()->SetLegendDragMode(true);
		pT->GetPlot()->BeginDragging(point, pT->GetPlot()->GetLegendOffset());
		pT->SetMsgHandled(true);
	}
	return needUpdate;
}

template<class HandlerT>
bool	CCloudLegendHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactLegend())return false;
	
	bool needUpdate = false;
	
	if(pT->GetPlot()->IsLegendDragMode())
	{
		pT->GetPlot()->SetLegendDragMode(false);
		pT->SetMsgHandled(true);
	}

	return needUpdate;
}

template<class HandlerT>
bool	CCloudLegendHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;

	return needUpdate;
}

template<class HandlerT>
bool	CCloudLegendHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactLegend())return false;

	bool needUpdate = false;
	
	if(pT->GetPlot()->IsLegendDragMode())
	{
		//pT->GetPlot()->SetLegendDraggingPoint(point);
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
bool	CCloudLegendHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CCloudLegendHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CCloudLegendHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

Declare_Namespace_End
