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
#include "ShareXLayerImpl.h"
#include "ShareXPlotImpl.h"

using namespace NsCChart;

CShareXLayerImpl::CShareXLayerImpl(CShareXPlotImpl *pParent):CShareXLayer(pParent)
{
/*	DeleteAllAxes();
//	AddAxis(kLocationLeft);

	SetEraseBkgnd(false);
	//m_bRangeZoomMode = true;*/
//	SetXAutoRange(false);
//	SetYAutoRange(true);
	SetMargin(6, 6, 6, 6);
}

CShareXLayerImpl::~CShareXLayerImpl()
{
	
}
/*
SIZE	CShareXLayerImpl::GetAxisSize( HDC hDC, int location )
{
	SIZE size={0,0};
	CAxisImpl		*axis=GetAxisByLocation(location);
	if(axis==NULL)return size;
	
	SIZE		axisDims, paddingDims;
	axisDims = axis->GetMinDisplaySize( hDC );
	paddingDims = axis->GetPaddingSize( hDC );
	size=axisDims;
	int			gap;
	gap = axis->GetAxisGap();
	
	switch(location)
	{
	case kLocationBottom:		
	case kLocationTop:
		size.cx+=(paddingDims.cx+paddingDims.cy);
		size.cy=axisDims.cy + gap;
		break;
	case kLocationLeft:
	case kLocationRight:
		size.cy+=(paddingDims.cx+paddingDims.cy);
		size.cx=axisDims.cx + gap;
		break;
	default: break;
	}
	
	return size;
}
*/
void	CShareXLayerImpl::SetXRange( double low, double high)
{
	bool bRangeSet;

	bRangeSet = m_pParent->IsXRangeSet();//GetXMainAxis()->IsRangeSet();
	((CShareXPlotImpl *)m_pParent)->SetXRange(low, high, bRangeSet);

	int i;
	CShareXLayerImpl *pLayerImpl;
	for(i=0; i<m_pParent->GetSubPlotCount(); i++)
	{
		pLayerImpl = m_pParent->GetSubPlot(i);
		bRangeSet = pLayerImpl->IsXRangeSet();//GetXMainAxis()->IsRangeSet();
		((CRanges<2>*)pLayerImpl)->SetXRange(low, high, bRangeSet);
		
		if(!bRangeSet)m_pParent->SetXRangeSet(false);//GetXMainAxis()->SetRangeSet(false);
	}
}

bool	CShareXLayerImpl::IsToCalcXRange()
{
	return !IsXAutoRange() && IsXRangeSet();
}

tstring	CShareXLayerImplI::GetDescriptionByMousePos(HDC hDC, POINT point)
{
	return _TEXT("");
}