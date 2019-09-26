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

//#include "StdAfx.h"
#include "PlotLayerImpl.h"
#include "LayeredPlotImpl.h"

using namespace NsCChart;

CPlotLayerImpl::CPlotLayerImpl(CLayeredPlotImpl *pParent):CPlotLayer(pParent)
{
/*	DeleteAllAxes();
//	AddAxis(kLocationLeft);

	SetEraseBkgnd(false);
	//m_bRangeZoomMode = true;
	SetXAutoRange(false);
	SetYAutoRange(false);*/
	AddAxis(kLocationLeft);
	AddAxis(kLocationBottom);
}

CPlotLayerImpl::~CPlotLayerImpl()
{
	
}

void	CPlotLayerImpl::SetXRange( double low, double high)
{
	bool bRangeSet;

	bRangeSet = m_pParent->IsXRangeSet();//GetXMainAxis()->IsRangeSet();
	((CLayeredPlotImpl *)m_pParent)->SetXRange(low, high, bRangeSet);

	int i;
	CPlotLayerImpl *pLayerImpl;
	for(i=0; i<m_pParent->GetSubPlotCount(); i++)
	{
		pLayerImpl = m_pParent->GetSubPlot(i);
		bRangeSet = pLayerImpl->IsXRangeSet();//GetXMainAxis()->IsRangeSet();
		((CRanges<2>*)pLayerImpl)->SetXRange(low, high, bRangeSet);
		
		if(!bRangeSet)m_pParent->SetXRangeSet(false);//GetXMainAxis()->SetRangeSet(false);
	}
}

tstring	CPlotLayerImplI::GetDescriptionByMousePos(HDC hDC, POINT point)
{
	return _TEXT("");
}
