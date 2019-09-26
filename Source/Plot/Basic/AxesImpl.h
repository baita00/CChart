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

#include "Axes.h"
#include "AxisImpl.h"

Declare_Namespace_CChart

template<class PlotImplT>
class CAxesImpl : public CAxes
{
public:
	CAxesImpl();
	virtual ~CAxesImpl();

protected:
	CAxisImpl	*NewAxis( );
	RECT		AdjustByAxes(HDC hDC, RECT destRect, RECT plotRect);
	RECT		AdjustByPadding(HDC hDC, RECT destRect, RECT plotRect);

public:
	virtual	void	DrawAxes(HDC hDC);

public:
	SIZE		GetAxisSize( HDC hDC, int location );
	bool		IsMapAlternateMode();
	void		ApplyMapAlternated();
};

template<class PlotImplT>
CAxesImpl<PlotImplT>::CAxesImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CAxesImpl<PlotImplT>::~CAxesImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template<class PlotImplT>
CAxisImpl *CAxesImpl<PlotImplT>::NewAxis()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	CAxisImpl *axis = new CAxisImpl(pT);
	return axis;
}

template<class PlotImplT>
RECT	CAxesImpl<PlotImplT>::AdjustByAxes(HDC hDC, RECT destRect, RECT plotRect)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	CAxisImpl *axis;

	
	SIZE minSize;
	unsigned int i;
	for(i=0; i<m_vpAxis.size(); i++)
	{
		axis = (m_vpAxis[i]);
		if(!axis->IsVisible())continue;
		
		minSize = axis->GetMinDisplaySize(hDC);
		switch(axis->GetLocation())
		{
		case kLocationLeft:
			plotRect.left += minSize.cx + axis->GetAxisGap();
			break;
		case kLocationRight:
			plotRect.right -= minSize.cx + axis->GetAxisGap();
			break;
		case kLocationTop:
			plotRect.top += minSize.cy + axis->GetAxisGap();
			break;
		case kLocationBottom:
			plotRect.bottom -= minSize.cy + axis->GetAxisGap();
			break;
		}
	}

	plotRect = AdjustByPadding(hDC, destRect, plotRect);
	
	return plotRect;
}

template<class PlotImplT>
RECT	CAxesImpl<PlotImplT>::AdjustByPadding(HDC hDC, RECT destRect, RECT plotRect)
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	CAxisImpl *axis;
	SIZE paddingDims;

	bool bX;
	int nL, nR, nT, nB;
	nL = nR = nT = nB = 0;
	int i;
	for( i = 0; i < GetAxesCount(); i++ )
	{
		axis = m_vpAxis[i];
		if( !axis->IsVisible() )continue;
		bX = axis->IsXAxis();

		if( !( bX?( pT->IsFloatXTicks() ):( pT->IsFloatYTicks() ) ) )
			paddingDims = axis->GetPaddingSize( hDC, plotRect );
		else
			paddingDims = axis->GetPaddingSizeForFloatTicks( hDC, plotRect );
		if( (bX && pT->IsXOpposite()) || (!bX && pT->IsYOpposite()) )
		{
			std::swap(paddingDims.cx, paddingDims.cy);
		}
		
		if(bX)
		{
			if(paddingDims.cx > nL && !axis->IsDespiteLow())nL = paddingDims.cx;
			if(paddingDims.cy > nR && !axis->IsDespiteHigh())nR = paddingDims.cy;
		}
		else
		{
			if(paddingDims.cx > nB  && !axis->IsDespiteLow())nB = paddingDims.cx;
			if(paddingDims.cy > nT  && !axis->IsDespiteHigh())nT = paddingDims.cy;
		}
	}
	int ml, mr, mt, mb;
	pT->GetMargin(ml, mr, mt, mb);
	if(plotRect.left - nL < destRect.left + ml) plotRect.left += (nL + destRect.left + ml - plotRect.left);
	if(plotRect.right + nR > destRect.right - mr) plotRect.right -= (nR - destRect.right + mr + plotRect.right);
	if(plotRect.top - nT < destRect.top + mt) plotRect.top += (nT + destRect.top + mt - plotRect.top);
	if(plotRect.bottom + nB > destRect.bottom - mb) plotRect.bottom -= (nB - destRect.bottom + mb + plotRect.bottom);

	return plotRect;
}

template<class PlotImplT>
void	CAxesImpl<PlotImplT>::DrawAxes(HDC hDC)
{
	unsigned int i;
	for(i=0; i<m_vpAxis.size(); i++)
	{
		m_vpAxis[i]->OnDraw(hDC);
	}
}

template<class PlotImplT>
SIZE	CAxesImpl<PlotImplT>::GetAxisSize(HDC hDC, int location)
{
	SIZE axisSize = {0, 0};
	CAxisImpl *axis = GetAxisByLocation(location);
	if(axis)
	{
		axisSize = axis->GetSize(hDC);
	}
	return axisSize;
}

template<class PlotImplT>
bool	CAxesImpl<PlotImplT>::IsMapAlternateMode()
{
	unsigned int i;
	for(i=0; i<m_vpAxis.size(); i++)
	{
		if(m_vpAxis[i]->IsMapAlternateMode())return true;
	}
	return false;
}

template<class PlotImplT>
void	CAxesImpl<PlotImplT>::ApplyMapAlternated()
{
	unsigned int i;
	for(i=0; i<m_vpAxis.size(); i++)
	{
		if(m_vpAxis[i]->IsMapAlternateMode())m_vpAxis[i]->ApplyMapAlternated();
	}
}

Declare_Namespace_End