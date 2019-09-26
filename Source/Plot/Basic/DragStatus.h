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

#include "CChartNameSpace.h"
#include <windows.h>

Declare_Namespace_CChart

template<class PlotImplT>
class	CDragStatus
{
public:
	CDragStatus();
	virtual	~CDragStatus();

public:
	POINT				m_ptOrigin,m_ptCurr;
	SIZE				m_sizeOffsetOrigin;
	double				m_fOffsetXOrigin, m_fOffsetYOrigin;
	int					m_nPositionOrigin;
	
public:
	void				BeginDragging(POINT pointStart, SIZE &offset);
	void				SetDraggingPoint(POINT point, SIZE &offset);
};

template<class PlotImplT>
CDragStatus<PlotImplT>::CDragStatus()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CDragStatus<PlotImplT>::~CDragStatus()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
void	CDragStatus<PlotImplT>::BeginDragging(POINT pointStart, SIZE &offset)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	m_ptOrigin= pointStart;
	m_ptCurr = pointStart;
	m_sizeOffsetOrigin = offset;
}

template<class PlotImplT>
void	CDragStatus<PlotImplT>::SetDraggingPoint(POINT point, SIZE &offset)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
	pT->m_ptCurr = point;
	
	offset = pT->m_sizeOffsetOrigin;
	offset.cx+=(pT->m_ptCurr.x-pT->m_ptOrigin.x);
	offset.cy+=(pT->m_ptCurr.y-pT->m_ptOrigin.y);	
}

Declare_Namespace_End