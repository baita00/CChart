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

#include "Global.h"

#include "PlotBasicImpl.h"

#include "PlotData.h"

//#include "MyGdiPlus.h"
using namespace NsCChart;

CPlotBasicImpl::CPlotBasicImpl()
{	
	if(!CGlobal::m_bInDll)
	{
		if(!CGlobal::IsGdiPlusInitialized())
			CGlobal::InitGdiPlus();
	}

	m_hWnd = 0;
	m_bRectConfined = false;
	RECT rct={0, 0, 0, 0};
	m_rctConfined = rct;
	m_nPlotID = -1;

	//
	Reset();
}

CPlotBasicImpl::~CPlotBasicImpl()
{
	if(!CGlobal::m_bInDll)
	{
		if(CGlobal::IsGdiPlusInitialized())
			CGlobal::FreeGdiPlus();
	}
}

HWND				CPlotBasicImpl::GetWnd()
{
	return m_hWnd;
}

void				CPlotBasicImpl::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}

bool				CPlotBasicImpl::IsRectConfined()
{
	return m_bRectConfined;
}

void				CPlotBasicImpl::SetRectConfined(bool confined)
{
	m_bRectConfined = confined;
}

RECT				CPlotBasicImpl::GetConfineRect()
{
	RECT rect={0, 0, 0, 0};
	if(memcmp(&m_rctConfined, &rect, sizeof(RECT))==0 && m_bRectConfined && IsWindow(m_hWnd))
	{
		GetClientRect(m_hWnd, &rect);
		return rect;
	}
	return m_rctConfined;
}

void				CPlotBasicImpl::SetConfineRect(RECT rect)
{
	m_rctConfined = rect;
}

int					CPlotBasicImpl::GetPlotID()
{
	return m_nPlotID;
}

void				CPlotBasicImpl::SetPlotID(int id)
{
	m_nPlotID = id;
}
