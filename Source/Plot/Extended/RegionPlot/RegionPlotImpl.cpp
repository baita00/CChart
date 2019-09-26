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

#include "RegionPlotImpl.h"
#include "../../Basic/MyMemDC.h"
using namespace NsCChart;

CRegionPlotImpl::CRegionPlotImpl()
{
	m_pHScroll->SetFillBar(false);
	m_pVScroll->SetFillBar(false);
}

CRegionPlotImpl::~CRegionPlotImpl()
{
	
}

RECT	CRegionPlotImpl::AddtionalPlotRectAdjust(HDC hDC, RECT plotRect)
{
	if(m_bRegionHorizontal)plotRect.bottom -= m_nHScrollWidth;
	if(m_bRegionVerticle)plotRect.right -= m_nVScrollWidth;
	return plotRect;
}

RECT	CRegionPlotImpl::GetHScrollRect(HDC hDC, RECT destRect, RECT plotRect)
{
	RECT regionRect = plotRect;
	regionRect.bottom = destRect.bottom - GetMarginBottom();
	regionRect.top = regionRect.bottom - m_nHScrollWidth;

	if(IsShowFootnote())
	{
		RECT footNoteRect = GetFootnoteRect(hDC);
		OffsetRect(&regionRect, 0, -Height(footNoteRect));
	}
	return regionRect;
}

RECT	CRegionPlotImpl::GetVScrollRect(HDC hDC, RECT destRect, RECT plotRect)
{
	RECT regionRect = plotRect;
	regionRect.right = destRect.right - GetMarginRight();
	regionRect.left = regionRect.right - m_nVScrollWidth;
	
	return regionRect;
}

void	CRegionPlotImpl::GetPlotRange( double *xRange, double *yRange )
{
	CXYPlotImpl::GetPlotRange(xRange, yRange);
	//memcpy(m_fXRangeSave, xRange, 2*sizeof(double));
	//memcpy(m_fYRangeSave, yRange, 2*sizeof(double));

	int nTicks, nMinorTicks;
	double tickMin, tickMax;
	do 
	{
		if(m_bRegionHorizontal)
		{
			if(m_fXRegion[0]>m_fXRegion[1])swap(m_fXRegion[0], m_fXRegion[1]);
			SetFloatXTicks(true);
			if(m_fXRegion[0]>=m_fXRegion[1])break;

			double v1, v2, realGap;
			v1 = m_fXRegion[0]; v2 = m_fXRegion[1];
			realGap = GetXTickGap();
			if(GetAtomX()!=0.0 && GetAtomX()!=1.0)
			{
				v1 = (v1 - GetAtomZeroX())/GetAtomX();
				v2 = (v2 - GetAtomZeroX())/GetAtomX();
				realGap /= GetAtomX();
			}
			else
			{
				v1 -= GetAtomZeroX();
				v2 -= GetAtomZeroX();
			}
			
			if(!IsXGapDetermined())RegulateTicks(v1, v2,nTicks, nMinorTicks, tickMin, tickMax);
			else
			{
				ComputeTicksWithGap(v1, v2, realGap, nTicks, nMinorTicks, tickMin, tickMax);
			}

			if(GetAtomX()!=0.0 && GetAtomX()!=1.0)
			{
				tickMin = tickMin*GetAtomX() + GetAtomZeroX();
				tickMax = tickMax*GetAtomX() + GetAtomZeroX();
			}
			else
			{
				tickMin += GetAtomZeroX();
				tickMax += GetAtomZeroX();
			}

			if(IsXGapDetermined() && IsXDefExpandWhileGapDet())
			{
				double rgn[2];
				GetXDataRange(rgn);

				memcpy(xRange, rgn, 2*sizeof(double));
				double tmin, tmax;
				tmin = tickMin;
				while(tmin>rgn[0]+GetXTickGap())
				{
					tmin -= GetXTickGap();
				}
				tmax = tickMax;
				while(tmax<rgn[1]-GetXTickGap())
				{
					tmax += GetXTickGap();
				}

				if(tmin - xRange[0]<GetXTickGap()/2.0)
				{
					xRange[0] = tmin - GetXTickGap()/2.0;
				}
				if(xRange[1] - tmax<GetXTickGap()/2.0)
				{
					xRange[1] = tmax + GetXTickGap()/2.0;
				}
			}

			m_pHScroll->SetRangeOnly(xRange);
			SetXRange(m_fXRegion);
			m_pHScroll->SetSliderRange(m_fXRegion);

			memcpy(m_fXRangeSave, xRange, 2*sizeof(double));

			memcpy(xRange, m_fXRegion, 2*sizeof(double));

			SetXTickCount(nTicks);
			SetXMinorTickCount(nMinorTicks);
			SetXTickMin(tickMin);
			SetXTickMax(tickMax);
			
		}
		if(m_bRegionVerticle)
		{
			if(m_fYRegion[0]>m_fYRegion[1])swap(m_fYRegion[0], m_fYRegion[1]);
			SetFloatYTicks(true);
			if(m_fYRegion[0]>=m_fYRegion[1])break;

			double v1, v2, realGap;
			v1 = m_fYRegion[0]; v2 = m_fYRegion[1];
			realGap = GetYTickGap();
			if(GetAtomY()!=0.0 && GetAtomY()!=1.0)
			{
				v1 = (v1 - GetAtomZeroY())/GetAtomY();
				v2 = (v2 - GetAtomZeroY())/GetAtomY();
				realGap /= GetAtomY();
			}
			else
			{
				v1 -= GetAtomZeroY();
				v2 -= GetAtomZeroY();
			}

			if(!IsYGapDetermined())RegulateTicks(v1, v2,nTicks, nMinorTicks, tickMin, tickMax);
			else
			{
				ComputeTicksWithGap(v1, v2, realGap, nTicks, nMinorTicks, tickMin, tickMax);
			}

			if(GetAtomY()!=0.0 && GetAtomY()!=1.0)
			{
				tickMin = tickMin*GetAtomY() + GetAtomZeroY();
				tickMax = tickMax*GetAtomY() + GetAtomZeroY();
			}
			else
			{
				tickMin += GetAtomZeroY();
				tickMax += GetAtomZeroY();
			}
			
			if(IsYGapDetermined() && IsYDefExpandWhileGapDet())
			{
				double rgn[2];
				GetYDataRange(rgn);
				
				memcpy(yRange, rgn, 2*sizeof(double));
				double tmin, tmax;
				tmin = tickMin;
				while(tmin>rgn[0]+GetYTickGap())
				{
					tmin -= GetYTickGap();
				}
				tmax = tickMax;
				while(tmax<rgn[1]-GetYTickGap())
				{
					tmax += GetYTickGap();
				}
				
				if(tmin - yRange[0]<GetYTickGap()/2.0)
				{
					yRange[0] = tmin - GetYTickGap()/2.0;
				}
				if(yRange[1] - tmax<GetYTickGap()/2.0)
				{
					yRange[1] = tmax + GetYTickGap()/2.0;
				}
			}

			m_pVScroll->SetRangeOnly(yRange);
			SetYRange(m_fYRegion);
			m_pVScroll->SetSliderRange(m_fYRegion);

			memcpy(m_fYRangeSave, yRange, 2*sizeof(double));

			memcpy(yRange, m_fYRegion, 2*sizeof(double));
			
			SetYTickCount(nTicks);
			SetYMinorTickCount(nMinorTicks);
			SetYTickMin(tickMin);
			SetYTickMax(tickMax);
		}
//		memcpy(m_fXRangeSave, xRange, 2*sizeof(double));
//		memcpy(m_fYRangeSave, yRange, 2*sizeof(double));
		// Apply map
		if(IsMapAlternateMode())ApplyMapAlternated();

	} while (false);
	
}

void	CRegionPlotImpl::FinalDraw( HDC hDC, RECT destRect )
{
	HDC hdc;
	CMyMemDC memdc;
	if(!m_bDoubleBuffer)
	{
		hdc = hDC;
	}
	else
	{
		memdc->CreateMemDC(hDC, NULL, &destRect);
		hdc = memdc;
	}
	bool bDB = IsDoubleBuffer();
	SetDoubleBuffer(false);
	CXYPlotImpl::FinalDraw(hdc, destRect);
	SetDoubleBuffer(bDB);

	RECT theRect;
	if(m_bRegionHorizontal)
	{
		theRect = GetHScrollRect(hdc, destRect, GetLastPlotRect());
		m_pHScroll->OnDraw(hdc, theRect);
	}
	if(m_bRegionVerticle)
	{
		theRect = GetVScrollRect(hdc, destRect, GetLastPlotRect());
		m_pVScroll->OnDraw(hdc, theRect);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//

CRegionPlotImplI::CRegionPlotImplI()
{
	m_nDragWhich = -1;
}

CRegionPlotImplI::~CRegionPlotImplI()
{
	
}

bool		CRegionPlotImplI::OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HWND hWnd = WindowFromDC(hDC);
	if(!IsWindow(hWnd))return false;

	bool needUpdate = false;
	RECT rectH = GetHScrollRect(hDC, GetLastClientRect(), GetLastPlotRect());
	RECT rectV = GetVScrollRect(hDC, GetLastClientRect(), GetLastPlotRect());
	RECT rectLP, rectUP, rectSlider;
	RECT rectL, rectU;
	if( ( m_bRegionHorizontal && PtInRect(&rectH, point) ) || ( m_bRegionVerticle && PtInRect(&rectV, point) ) )
	{
		m_sizeOffset.cx = m_sizeOffset.cy = 0;
		
		if(m_bRegionHorizontal && PtInRect(&rectH, point))
		{
			memcpy(m_fRegionOld, m_fXRegion, 2*sizeof(double));
			rectSlider = m_pHScroll->GetSliderRect(hDC, rectH);
			rectL = m_pHScroll->GetLowerRect(hDC, rectH);
			rectU = m_pHScroll->GetUpperRect(hDC, rectH);
			
			if(m_pHScroll->HitLower(point, rectH))
			{
				rectLP = m_pHScroll->GetLowerPRect(hDC, rectH);
				MyClientToScreen(hWnd, &rectLP);
				ClipCursor(&rectLP);
				m_nDragWhich = 1;
				BeginDragging(point, m_sizeOffset);

				::SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			}
			else if(m_pHScroll->HitUpper(point, rectH))
			{
				rectUP = m_pHScroll->GetUpperPRect(hDC, rectH);
				MyClientToScreen(hWnd, &rectUP);
				ClipCursor(&rectUP);
				m_nDragWhich = 2;
				BeginDragging(point, m_sizeOffset);

				::SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			}
			else if(PtInRect(&rectSlider, point))
			{
				MyClientToScreen(hWnd, &rectH);
				ClipCursor(&rectH);
				m_nDragWhich = 0;
				BeginDragging(point, m_sizeOffset);
			}
			else if(PtInRect(&rectL, point))
			{
				MoveRange(m_fXRegion, -MyRange(m_fXRegion)/2.0);
				if(m_fXRegion[0] < m_fXRangeSave[0])
				{
					MoveRange(m_fXRegion, m_fXRangeSave[0]-m_fXRegion[0]);
				}
			}
			else if(PtInRect(&rectU, point))
			{
				MoveRange(m_fXRegion, MyRange(m_fXRegion)/2.0);
				if(m_fXRegion[1] > m_fXRangeSave[1])
				{
					MoveRange(m_fXRegion, m_fXRangeSave[1]-m_fXRegion[1]);
				}
			}
		}
		else if(m_bRegionVerticle && PtInRect(&rectV, point))
		{
			memcpy(m_fRegionOld, m_fYRegion, 2*sizeof(double));
			rectSlider = m_pVScroll->GetSliderRect(hDC, rectV);
			rectL = m_pVScroll->GetLowerRect(hDC, rectV);
			rectU = m_pVScroll->GetUpperRect(hDC, rectV);
			
			if(m_pVScroll->HitLower(point, rectV))
			{
				rectLP = m_pVScroll->GetLowerPRect(hDC, rectV);
				MyClientToScreen(hWnd, &rectLP);
				ClipCursor(&rectLP);
				m_nDragWhich = 1;
				BeginDragging(point, m_sizeOffset);

				::SetCursor(LoadCursor(NULL, IDC_SIZENS));
			}
			else if(m_pVScroll->HitUpper(point, rectV))
			{
				rectUP = m_pVScroll->GetUpperPRect(hDC, rectV);
				MyClientToScreen(hWnd, &rectUP);
				ClipCursor(&rectUP);
				m_nDragWhich = 2;
				BeginDragging(point, m_sizeOffset);

				::SetCursor(LoadCursor(NULL, IDC_SIZENS));
			}
			else if(PtInRect(&rectSlider, point))
			{
				MyClientToScreen(hWnd, &rectV);
				ClipCursor(&rectV);
				m_nDragWhich = 0;
				BeginDragging(point, m_sizeOffset);
			}
			else if(PtInRect(&rectL, point))
			{
				MoveRange(m_fYRegion, -MyRange(m_fYRegion)/2.0);
				if(m_fYRegion[0] < m_fYRangeSave[0])
				{
					MoveRange(m_fYRegion, m_fYRangeSave[0]-m_fYRegion[0]);
				}
			}
			else if(PtInRect(&rectU, point))
			{
				MoveRange(m_fYRegion, MyRange(m_fYRegion)/2.0);
				if(m_fYRegion[1] > m_fYRangeSave[1])
				{
					MoveRange(m_fYRegion, m_fYRangeSave[1]-m_fYRegion[1]);
				}
			}
		}
		return true;
	}
	
	needUpdate = CXYPlotImplI::OnLButtonDown(hDC, point, ctrlKey) | needUpdate;
	return needUpdate;
}

bool		CRegionPlotImplI::OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	bool needUpdate = false;
	needUpdate = CXYPlotImplI::OnLButtonUp(hDC, point, ctrlKey) | needUpdate;
	m_nDragWhich = -1;
	return needUpdate;
}

bool		CRegionPlotImplI::OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HWND hWnd = WindowFromDC(hDC);
	if(!IsWindow(hWnd))return false;
	
	bool needUpdate = false;
	RECT rectH = GetHScrollRect(hDC, GetLastClientRect(), GetLastPlotRect());
	RECT rectV = GetVScrollRect(hDC, GetLastClientRect(), GetLastPlotRect());

	if( ( m_bRegionHorizontal && PtInRect(&rectH, point) ) || ( m_bRegionVerticle && PtInRect(&rectV, point) ) )
	{
		return false;
	}

	needUpdate = CXYPlotImplI::OnLButtonDblClk(hDC, point, ctrlKey) | needUpdate;
	return needUpdate;
}

bool		CRegionPlotImplI::OnMouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	bool needUpdate = false;

	RECT rectH = GetHScrollRect(hDC, GetLastClientRect(), GetLastPlotRect());
	RECT rectV = GetVScrollRect(hDC, GetLastClientRect(), GetLastPlotRect());

	if( ( m_bRegionHorizontal && PtInRect(&rectH, point) ) || ( m_bRegionVerticle && PtInRect(&rectV, point) ) )
	{
		if(m_bRegionHorizontal && PtInRect(&rectH, point))
		{
			if(m_nDragWhich == 0 || m_nDragWhich == 1 || m_nDragWhich == 2)
			{
				SetDraggingPoint(point, m_sizeOffset);
				if(m_nDragWhich == 0)
				{
					m_fXRegion[0] = m_fRegionOld[0] + m_sizeOffset.cx*m_pHScroll->GetRatio(rectH);
					m_fXRegion[1] = m_fRegionOld[1] + m_sizeOffset.cx*m_pHScroll->GetRatio(rectH);
					if(m_fXRegion[0] < m_fXRangeSave[0])
					{
						MoveRange(m_fXRegion, m_fXRangeSave[0]-m_fXRegion[0]);
					}
					if(m_fXRegion[1] > m_fXRangeSave[1])
					{
						MoveRange(m_fXRegion, m_fXRangeSave[1]-m_fXRegion[1]);
					}
					return true;
				}
				if(m_nDragWhich == 1)
				{
					::SetCursor(LoadCursor(NULL, IDC_SIZEWE));
					m_fXRegion[0] = m_fRegionOld[0] + m_sizeOffset.cx*m_pHScroll->GetRatio(rectH);
					return true;
				}
				if(m_nDragWhich == 2)
				{
					::SetCursor(LoadCursor(NULL, IDC_SIZEWE));
					m_fXRegion[1] = m_fRegionOld[1] + m_sizeOffset.cx*m_pHScroll->GetRatio(rectH);
					return true;
				}
			}
			else if(m_pHScroll->HitLower(point, rectH) || m_pHScroll->HitUpper(point, rectH))
			{
				::SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			}
		}
		if(m_bRegionVerticle && PtInRect(&rectV, point))
		{
			if(m_nDragWhich == 0 || m_nDragWhich == 1 || m_nDragWhich == 2)
			{
				SetDraggingPoint(point, m_sizeOffset);
				if(m_nDragWhich == 0)
				{
					m_fYRegion[0] = m_fRegionOld[0] - m_sizeOffset.cy*m_pVScroll->GetRatio(rectV);
					m_fYRegion[1] = m_fRegionOld[1] - m_sizeOffset.cy*m_pVScroll->GetRatio(rectV);
					if(m_fYRegion[0] < m_fYRangeSave[0])
					{
						MoveRange(m_fYRegion, m_fYRangeSave[0]-m_fYRegion[0]);
					}
					if(m_fYRegion[1] > m_fYRangeSave[1])
					{
						MoveRange(m_fYRegion, m_fYRangeSave[1]-m_fYRegion[1]);
					}
					return true;
				}
				if(m_nDragWhich == 1)
				{
					::SetCursor(LoadCursor(NULL, IDC_SIZENS));
					m_fYRegion[0] = m_fRegionOld[0] - m_sizeOffset.cy*m_pVScroll->GetRatio(rectV);
					return true;
				}
				if(m_nDragWhich == 2)
				{
					::SetCursor(LoadCursor(NULL, IDC_SIZENS));
					m_fYRegion[1] = m_fRegionOld[1] - m_sizeOffset.cy*m_pVScroll->GetRatio(rectV);
					return true;
				}
			}
			else if(m_pVScroll->HitLower(point, rectV) || m_pVScroll->HitUpper(point, rectV))
			{
				::SetCursor(LoadCursor(NULL, IDC_SIZENS));
			}
		}
	}
	needUpdate = CXYPlotImplI::OnMouseMove(hDC, point, ctrlKey) | needUpdate;
	return needUpdate;
}

