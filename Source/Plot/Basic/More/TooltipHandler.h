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
#include "../Handlers/DefOperation.h"

Declare_Namespace_CChart

template<class HandlerT>
class	CTooltipHandler
{
public:
	CTooltipHandler();
	virtual	~CTooltipHandler();

public:


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
CTooltipHandler<HandlerT>::CTooltipHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);

}

template<class HandlerT>
CTooltipHandler<HandlerT>::~CTooltipHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
bool	CTooltipHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);
	
	return false;
}

template<class HandlerT>
bool	CTooltipHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CTooltipHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);
	
	return false;
}

template<class HandlerT>
bool	CTooltipHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

//	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;
	
	if(CGlobal::m_bEnableToolTip)
	{
		pT->GetPlot()->m_bShowTooltip = true;

		needUpdate = false;

		static POINT ptOld;
		
		if(point.x != ptOld.x || point.y != ptOld.y)
		{
			ptOld = point;

			static	tstring strOld = _TEXT("");
			tstring str = pT->GetPlot()->GetDescriptionByMousePos(hDC, point);
			
			pT->GetPlot()->m_strTooltip = str;
			pT->GetPlot()->m_pointTooltip = point;

			if(str!=strOld)needUpdate = true;
			strOld = str;
		}
	}

	return needUpdate;
}

template<class HandlerT>
bool	CTooltipHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

//	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;

	if(CGlobal::m_bEnableToolTip)
	{
		//CGlobal::m_toolTip.DeactiveToolTip();
		pT->GetPlot()->m_bShowTooltip = false;
		pT->GetPlot()->m_strTooltip = _TEXT("");
//		pT->SetMsgHandled(true);
		needUpdate = true;
	}

	return needUpdate;
}

template<class HandlerT>
bool	CTooltipHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CTooltipHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

Declare_Namespace_End