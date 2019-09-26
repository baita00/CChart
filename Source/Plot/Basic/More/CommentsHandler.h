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
class	CCommentsHandler
{
public:
	CCommentsHandler();
	virtual	~CCommentsHandler();

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
CCommentsHandler<HandlerT>::CCommentsHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
CCommentsHandler<HandlerT>::~CCommentsHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
bool	CCommentsHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;
	
	int commentid = pT->GetPlot()->GetCommentIdByPoint(hDC, point);
	if(commentid >= 0)
	{
		if(!pT->GetPlot()->GetComment(commentid)->IsDragEnable())return needUpdate;
		RECT plotRect = pT->GetPlot()->GetLastPlotRect();
		ClientToScreen(WindowFromDC(hDC), (LPPOINT)&plotRect);
		ClientToScreen(WindowFromDC(hDC), (LPPOINT)&plotRect+1);
		ClipCursor(&plotRect);
		
		pT->GetPlot()->SetCommentDragMode(commentid, true);
		pT->GetPlot()->BeginDragging(point, pT->GetPlot()->GetCommentOffset(commentid));
		pT->SetMsgHandled(true);
		needUpdate = true;
	}
	return needUpdate;
}

template<class HandlerT>
bool	CCommentsHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	
	int dragid = pT->GetPlot()->GetCommentDragModeID();
	if(dragid>=0)
	{
		pT->GetPlot()->UnsetCommentsDragMode();
		pT->SetMsgHandled(true);
		needUpdate = true;
		ClipCursor(NULL);
	}

	return needUpdate;
}

template<class HandlerT>
bool	CCommentsHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;

	return false;
}

template<class HandlerT>
bool	CCommentsHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;
	
	int dragid = pT->GetPlot()->GetCommentDragModeID();
	if(dragid>=0)
	{
		pT->GetPlot()->SetDraggingPoint(point, pT->GetPlot()->GetCommentOffset(dragid));
		needUpdate = true;
		pT->SetMsgHandled(true);
	}

	return needUpdate;
}

template<class HandlerT>
bool	CCommentsHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CCommentsHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CCommentsHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

Declare_Namespace_End
