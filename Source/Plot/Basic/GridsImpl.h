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

#include "Grids.h"
#include <math.h>

Declare_Namespace_CChart

template <class PlotImplT>
class	CGridsImpl : public CGrids
{
public:
	CGridsImpl();
	virtual ~CGridsImpl();
public:
	virtual	void	DrawGrids( HDC hDC );
};

template <class PlotImplT>
CGridsImpl<PlotImplT>::CGridsImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template <class PlotImplT>
CGridsImpl<PlotImplT>::~CGridsImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template <class PlotImplT>
void	CGridsImpl<PlotImplT>::DrawGrids( HDC hDC )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	RECT plotRect = pT->GetLastPlotRect();

	double		scale, range, theRange[2];
	POINT		point, tickPoint;
	int			j, k;
	double		position;
	HPEN		hPen;
	HPEN		hOldPen, hMinorPen;
	int			startIdx, endIdx;

	hPen = CreatePen( m_nMajorGridLineStyle, m_nMajorGridLineSize, m_crMajorGridColor );
	hMinorPen = CreatePen( m_nMinorGridLineStyle, m_nMinorGridLineSize, m_crMinorGridColor );

	hOldPen = (HPEN)SelectObject( hDC, hPen );

	SetBkColor(hDC, pT->GetBkgndColor() );
	SetBkMode(hDC, TRANSPARENT );

	pT->GetYRange( theRange );
	range = theRange[1] - theRange[0];
	
	if(m_bMajorHGrids == true || m_bMinorHGrids == true )
	{
		scale = fabs(double(plotRect.bottom - plotRect.top)) / range;
		if( m_bMajorHGrids == true )
		{
			if(!pT->IsFloatYTicks())
			{
				startIdx = 1;
				endIdx = pT->GetYTickCount();
			}
			else
			{
				if(fabs(pT->GetYTickMin()-theRange[0])<fMinVal)
				{
					startIdx = 1;
				}
				else
				{
					startIdx = 0;
				}
				if(fabs(pT->GetYTickMax()-theRange[1])<fMinVal)
				{
					endIdx = pT->GetYTickCount();
				}
				else
				{
					endIdx = pT->GetYTickCount()+1;
				}
			}
			for( j = startIdx; j <= endIdx; j++ )
			{
				position = pT->GetMajorTickPosValue( 1, j );
				SelectObject( hDC, hPen );
				if(!pT->IsYOpposite())
					point.y = (long)(plotRect.bottom - scale * (position - theRange[0]));
				else
					point.y = (long)(plotRect.top + scale * (position - theRange[0]));
				point.x = plotRect.left;
				MoveToEx(hDC, point.x,point.y, NULL );
				point.x = plotRect.right;
				LineTo( hDC,point.x,point.y );
			}
		}
		if( m_bMinorHGrids == true )
		{
			if(!pT->IsFloatYTicks())
			{
				startIdx = 0;
				endIdx = pT->GetYTickCount();
			}
			else
			{
				
				if(fabs(pT->GetYTickMin()-theRange[0])<fMinVal)
				{
					startIdx = 0;
				}
				else
				{
					startIdx = -1;
				}
				if(fabs(pT->GetYTickMax()-theRange[1])<fMinVal)
				{
					endIdx = pT->GetYTickCount();
				}
				else
				{
					endIdx = pT->GetYTickCount()+1;
				}
			}
			for( j = startIdx; j <= endIdx; j++ )
			{				
				double *minorPos = new double[pT->GetYMinorTickCount()+1];
				pT->GetMinorTickPosValue(1, j, minorPos);
				
				SelectObject(hDC, hMinorPen );
				for( k = 1; k < pT->GetYMinorTickCount(); k++ )
				{
					if(minorPos[k]<theRange[0] || minorPos[k]>theRange[1])continue;
					if(!pT->IsYOpposite())
						tickPoint.y = plotRect.bottom - (long)((minorPos[k] - theRange[0]) * scale);
					else
						tickPoint.y = plotRect.top + (long)((minorPos[k] - theRange[0]) * scale);
					tickPoint.x = plotRect.left;
					MoveToEx( hDC,tickPoint.x,tickPoint.y,NULL );
					tickPoint.x = plotRect.right;
					LineTo( hDC,tickPoint.x,tickPoint.y );
				}
				
				delete []minorPos;
			}
		}
	}

	pT->GetXRange( theRange );
	range = theRange[1] - theRange[0];
	if( m_bMajorVGrids == true || m_bMinorVGrids == true )
	{
		scale = (double)(plotRect.right - plotRect.left) / range;
		if( m_bMajorVGrids == true )
		{
			if(!pT->IsFloatXTicks())
			{
				startIdx = 1;
				endIdx = pT->GetXTickCount();
			}
			else
			{
				if(fabs(pT->GetXTickMin()-theRange[0])<fMinVal)
				{
					startIdx = 1;
				}
				else
				{
					startIdx = 0;
				}
				if(fabs(pT->GetXTickMax()-theRange[1])<fMinVal)
				{
					endIdx = pT->GetXTickCount();
				}
				else
				{
					endIdx = pT->GetXTickCount()+1;
				}
			}
			for( j = startIdx; j <= endIdx; j++ )
			{
				position = pT->GetMajorTickPosValue( 0, j );
				SelectObject( hDC, hPen );
				
				if(!pT->IsXOpposite())
					point.x = (long)(plotRect.left + scale * (position - theRange[0]) );
				else
					point.x = (long)(plotRect.right - scale * (position - theRange[0]) );
				point.y = plotRect.bottom;
				MoveToEx(hDC, point.x, point.y, NULL );
				point.y = plotRect.top;
				LineTo( hDC, point.x, point.y );
			}
		}
		if( m_bMinorVGrids == true )
		{
			if(!pT->IsFloatXTicks())
			{
				startIdx = 0;
				endIdx = pT->GetXTickCount();
			}
			else
			{
				if(fabs(pT->GetXTickMin()-theRange[0])<fMinVal)
				{
					startIdx = 0;
				}
				else
				{
					startIdx = -1;
				}
				if(fabs(pT->GetXTickMax()-theRange[1])<fMinVal)
				{
					endIdx = pT->GetXTickCount();
				}
				else
				{
					endIdx = pT->GetXTickCount()+1;
				}
			}
			for( j = startIdx; j <= endIdx; j++ )
			{
				double *minorPos = new double[pT->GetXMinorTickCount()+1];
				pT->GetMinorTickPosValue(0, j, minorPos);
				
				SelectObject( hDC,hMinorPen );
				for( k = 1; k < pT->GetXMinorTickCount(); k++ )
				{
					if(minorPos[k]<theRange[0] || minorPos[k]>theRange[1])continue;
					
					if(!pT->IsXOpposite())
						tickPoint.x = plotRect.left + (long)((minorPos[k]-theRange[0]) * scale);
					else
						tickPoint.x = plotRect.right - (long)((minorPos[k]-theRange[0]) * scale);
					tickPoint.y = plotRect.bottom;
					MoveToEx( hDC, tickPoint.x,tickPoint.y, NULL );
					tickPoint.y = plotRect.top;
					LineTo( hDC,tickPoint.x,tickPoint.y );
				}
				
				delete []minorPos;
			}
		}
	}

	SelectObject( hDC, hOldPen );
	DeleteObject( hPen );
	DeleteObject( hMinorPen );
}

Declare_Namespace_End