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

#include "../../Basic/PlotBasicImpl.h"
#include "../../Basic/Handler.h"

#include <gl/gl.h>

Declare_Namespace_CChart

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Declaration of CTDHandler

template <typename TDPlotImplT>
class CTDHandler : public CHandler
{
public:
	CTDHandler();
	virtual	~CTDHandler();
public:
	virtual	bool		OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey );
	virtual	bool		OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey );
	virtual	bool		OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey );
	virtual	bool		OnMouseMove( HDC hDC, POINT point, UINT ctrlKey );
	virtual	bool		OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey );
	virtual	bool		OnContextMenu( HMENU hMenu, HDC hDC, POINT point );
	virtual bool		OnKeyDown( HDC hDC, UINT key );
	
	virtual	void		Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
protected:
	bool				m_bDragMode;
	POINT				m_ptOrigin,m_ptCurr;

	GLfloat				m_pfOffsetOrigin[3];
	
	double				m_fRotSpeed;
	double				m_fPanSpeed;
public:
	double				GetRotSpeed(){return m_fRotSpeed;}
	void				SetRotSpeed(double speed){m_fRotSpeed = speed;}
	double				GetPanSpeed(){return m_fPanSpeed;}
	void				SetPanSpeed(double speed){m_fPanSpeed = speed;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation of CTDHandler

template <typename TDPlotImplT>
CTDHandler<TDPlotImplT>::CTDHandler()
{
	m_bDragMode = false;

	m_fRotSpeed = 2;
	m_fPanSpeed = 8;
}

template <typename TDPlotImplT>
CTDHandler<TDPlotImplT>::~CTDHandler()
{
}

template <typename TDPlotImplT>
bool	CTDHandler<TDPlotImplT>::OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	TDPlotImplT *pT=static_cast<TDPlotImplT*>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;
	
	HWND hWnd = WindowFromDC(hDC);
	if(hWnd)SetCapture(hWnd);
	m_bDragMode = true;
	m_ptOrigin = point;
	if(GetKeyState(VK_CONTROL)<0)
	{
		memcpy(m_pfOffsetOrigin, pT->m_pfOffset, 3*sizeof(GLfloat));
	}
	return false;
}

template <typename TDPlotImplT>
bool	CTDHandler<TDPlotImplT>::OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	TDPlotImplT *pT=static_cast<TDPlotImplT*>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	if(GetCapture())ReleaseCapture();
	m_bDragMode = false;
	return false;
}

template <typename TDPlotImplT>
bool	CTDHandler<TDPlotImplT>::OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	return false;
}

template <typename TDPlotImplT>
bool	CTDHandler<TDPlotImplT>::OnMouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	TDPlotImplT *pT=static_cast<TDPlotImplT*>(this);
	if(!PtInRect(&pT->GetLastClientRect(), point))return false;

	if(!m_bDragMode)
		return false;

	m_ptCurr = point;

	int dx, dy;
	dx = m_ptCurr.x - m_ptOrigin.x;
	dy = m_ptCurr.y - m_ptOrigin.y;

	RECT lastRt = pT->GetLastClientRect();

	int lastWidth = lastRt.right - lastRt.left;
	int lastHeight = lastRt.bottom - lastRt.top;
	double r1 = dx / double(lastWidth==0?1:lastWidth);
	double r2 = dy / double(lastHeight==0?1:lastHeight);

	if(GetKeyState(VK_CONTROL)>=0)
	{
		pT->m_pfAngle[2] += GLfloat(r1*m_fRotSpeed);
		pT->m_pfAngle[0] += GLfloat(r2*m_fRotSpeed);
	}
	else
	{
		pT->m_pfOffset[0]=m_pfOffsetOrigin[0]+GLfloat(r1*m_fPanSpeed);
		pT->m_pfOffset[1]=m_pfOffsetOrigin[1]-GLfloat(r2*m_fPanSpeed);
	}
	

	return true;
}

template <typename TDPlotImplT>
bool	CTDHandler<TDPlotImplT>::OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	return false;
}

template <typename TDPlotImplT>
bool	CTDHandler<TDPlotImplT>::OnContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	return false;
}

template <typename TDPlotImplT>
bool	CTDHandler<TDPlotImplT>::OnKeyDown( HDC hDC, UINT key )
{
	TDPlotImplT *pT=static_cast<TDPlotImplT*>(this);

	switch(key)
	{
	case VK_RETURN:
		pT->ResetTransform();
		return true;
	}
	return false;
}

template <typename TDPlotImplT>
void	CTDHandler<TDPlotImplT>::Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	TDPlotImplT *pT=static_cast<TDPlotImplT*>(this);
	if(OnEvent(hWnd, message, wParam, lParam))pT->OnDraw(hWnd);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

Declare_Namespace_End