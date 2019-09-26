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
//#include "Handlers/DefOperation.h"
#include "CChartNameSpace.h"

Declare_Namespace_CChart

template<class HandlerT>
class	CTitleHandler
{
public:
	CTitleHandler();
	virtual	~CTitleHandler();

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
CTitleHandler<HandlerT>::CTitleHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
CTitleHandler<HandlerT>::~CTitleHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
bool	CTitleHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactTitle())return false;

	bool needUpdate = false;
	
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if(region == kXYRegionTitle)
	{
		pT->GetPlot()->UnselectAll(false, true, true);
		pT->GetPlot()->SetSelectTitle(!pT->GetPlot()->IsSelectTitle());
		pT->SetMsgHandled(true);
		needUpdate = true;

		pT->m_nOldRegionLBD = region;
	}
	return needUpdate;
}

template<class HandlerT>
bool	CTitleHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CTitleHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	
	return needUpdate;
}

template<class HandlerT>
bool	CTitleHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	if(!pT->GetPlot()->IsReactTitle())return false;

	bool needUpdate = false;
	
	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	if(region == kXYRegionTitle)
	{
		if(!pT->GetPlot()->IsLightTitle())needUpdate = true;
		pT->GetPlot()->UnlightAll(false, true, true);
		pT->GetPlot()->SetLightTitle(true);
		pT->GetPlot()->SetLightDataRegion(false);
		pT->SetMsgHandled(true);

		pT->m_nOldRegionMM = region;
	}

	return needUpdate;
}

template<class HandlerT>
bool	CTitleHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CTitleHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CTitleHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

Declare_Namespace_End
