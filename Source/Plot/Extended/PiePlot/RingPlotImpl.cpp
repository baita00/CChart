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

#include "RingPlotImpl.h"

using namespace NsCChart;

CRingPlotImpl::CRingPlotImpl()
{
	m_bShadowing = false;
	m_b3DStyle = false;
}

CRingPlotImpl::~CRingPlotImpl()
{

}

void	CRingPlotImpl::DrawPieAreas( HDC hDC, RECT plotRect, double ctx, double cty, double a, double b, double total )
{
	int saveDC = SaveDC(hDC);
	
	HRGN hRgn, hR1, hR2;
	hRgn = CreateEllipticRgn(0, 0, 1, 1);
	hR1 = CreateRectRgn( plotRect.left, plotRect.top, plotRect.right, plotRect.bottom );
	hR2 = CreateEllipticRgn(int(ctx-a*m_fInnerRatio), int(cty-b*m_fInnerRatio), int(ctx+a*m_fInnerRatio), int(cty+b*m_fInnerRatio));
	CombineRgn(hRgn, hR1, hR2, RGN_DIFF);
	
	SelectObject(hDC,hRgn);
	
	CPiePlotImpl::DrawPieAreas(hDC, plotRect, ctx, cty, a, b, total);

	RestoreDC(hDC, saveDC );
	
	DeleteObject(hRgn);
	DeleteObject(hR1);
	DeleteObject(hR2);
}
/*
bool	CRingPlotImplI::ShowDlgBySelection(HDC hDC)
{
	bool needUpdate = false;
	if(IsSelectTitle())
	{
		if(DefPlotSettings(this, hDC))
		{
			needUpdate = true;
		}
	}
	else if(IsAxisSelected())
	{
		int index, location;
		index = GetIndexOfSelectedAxis();
		location = GetAxis(index)->GetLocation();
		if(DefAxisSettings(this,hDC,location))
		{
			needUpdate = true;
		}
	}
	else
	{
		if(DefPlotSettings(this,hDC))
		{
			needUpdate = true;
		}
	}
	
	return needUpdate;
}
*/