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
#include <vector>
#include "../CChartNameSpace.h"

typedef struct stAxisRanges
{
	bool bXFloat, bYFloat;
	double fxLow, fxHigh;
	int nXTicks, nXMinorTicks;
	double fXTickMin, fXTickMax;
	bool bxAutoR;
	bool bxSet;
	double fyLow, fyHigh;
	int nYTicks, nYMinorTicks;
	double fYTickMin, fYTickMax;
	bool byAutoR;
	bool bySet;
}AxisRanges;

Declare_Namespace_CChart

template<class HandlerT>
class	CRangeManageHandler
{
public:
	CRangeManageHandler();
	virtual	~CRangeManageHandler();

protected:
	std::vector<AxisRanges> vARs;
	AxisRanges ars;
public:
	void		ResetRanges();

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
CRangeManageHandler<HandlerT>::CRangeManageHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
CRangeManageHandler<HandlerT>::~CRangeManageHandler()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
}

template<class HandlerT>
void	CRangeManageHandler<HandlerT>::ResetRanges()
{
	HandlerT* pT = static_cast<HandlerT*>(this);
	if(pT->GetPlot()->IsRangeDrag())
	{
		pT->GetPlot()->SetRangeSet(false);
	}

	else if(pT->GetPlot()->IsRangeZoomMode())
	{
		if(vARs.size()>0)
		{
			ars = vARs[0];
			pT->GetPlot()->SetFloatXTicks(ars.bXFloat);
			pT->GetPlot()->SetFloatYTicks(ars.bYFloat);

			pT->GetPlot()->SetXRange(ars.fxLow, ars.fxHigh);
			pT->GetPlot()->SetXTickCount(ars.nXTicks);//GetXMainAxis()->SetTickCount(ars.nXTicks);
			pT->GetPlot()->SetXMinorTickCount(ars.nXMinorTicks);//GetXMainAxis()->SetMinorTickCount(ars.nXMinorTicks);
			pT->GetPlot()->SetXTickMin(ars.fXTickMin);
			pT->GetPlot()->SetXTickMax(ars.fXTickMax);
			pT->GetPlot()->SetXAutoRange(ars.bxAutoR);
			pT->GetPlot()->SetXRangeSet(ars.bxSet);//GetXMainAxis()->SetRangeSet(ars.bxSet);
			pT->GetPlot()->SetYRange(ars.fyLow, ars.fyHigh);
			pT->GetPlot()->SetYTickCount(ars.nYTicks);//GetYMainAxis()->SetTickCount(ars.nYTicks);
			pT->GetPlot()->SetYMinorTickCount(ars.nYMinorTicks);//GetYMainAxis()->SetMinorTickCount(ars.nYMinorTicks);
			pT->GetPlot()->SetYTickMin(ars.fYTickMin);
			pT->GetPlot()->SetYTickMax(ars.fYTickMax);
			pT->GetPlot()->SetYAutoRange(ars.byAutoR);
			pT->GetPlot()->SetYRangeSet(ars.bySet);//GetYMainAxis()->SetRangeSet(ars.bySet);
			vARs.clear();
		}
	}
}

template<class HandlerT>
bool	CRangeManageHandler<HandlerT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;

	int region = pT->GetPlot()->RegionIdentify(hDC, point);
	int legend = pT->GetPlot()->LegendRegionIdentify(hDC, point);

	RECT plotRect = pT->GetPlot()->GetLastPlotRect();

	if(region == kXYRegionData && legend < 0)
	{
		if(pT->GetPlot()->IsRangeDrag())
		{
			ClientToScreen(WindowFromDC(hDC), (LPPOINT)&plotRect);
			ClientToScreen(WindowFromDC(hDC), (LPPOINT)&plotRect+1);
			ClipCursor(&plotRect);
			pT->GetPlot()->SetRangeDragging(true);
			pT->GetPlot()->m_ptOrigin = point;
			pT->GetPlot()->m_ptCurr = point;
			//pT->GetPlot()->GetLastPlotRange(m_fXRange, m_fYRange);
			pT->GetPlot()->GetLastPlotRange(pT->GetPlot()->m_fXRange, pT->GetPlot()->m_fYRange);
			//pT->SetMsgHandled(true);
		}
		else if(pT->GetPlot()->IsRangeZoomMode())
		{
			ClientToScreen(WindowFromDC(hDC), (LPPOINT)&plotRect);
			ClientToScreen(WindowFromDC(hDC), (LPPOINT)&plotRect+1);
			ClipCursor(&plotRect);
			pT->GetPlot()->SetRangeZoomming(true);
			pT->GetPlot()->m_ptOrigin = point;
			pT->GetPlot()->m_ptCurr = point;
			//pT->SetMsgHandled(true);
		}
	}

	return needUpdate;
}

template<class HandlerT>
bool	CRangeManageHandler<HandlerT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	
	// Deal with range drag
	if(pT->GetPlot()->IsRangeDrag() && pT->GetPlot()->IsRangeDragging())
	{
		pT->GetPlot()->SetRangeDragging(false);
		pT->GetPlot()->m_ptCurr = point;

		double xRange[2], yRange[2];
		RECT plotRect;
		memcpy(xRange,pT->GetPlot()->m_fXRange, 2*sizeof(double));
		memcpy(yRange,pT->GetPlot()->m_fYRange, 2*sizeof(double));
		plotRect = pT->GetPlot()->GetLastPlotRect();

		double xrat, yrat;
		xrat = (xRange[1] - xRange[0])/(plotRect.right!=plotRect.left?plotRect.right - plotRect.left:1);
		yrat = (yRange[1] - yRange[0])/(plotRect.bottom!=plotRect.top?plotRect.bottom - plotRect.top:1);

		switch(pT->GetPlot()->GetRangeDragType())
		{
		case kRangeDragX:
			xRange[0] -= (pT->GetPlot()->m_ptCurr.x - pT->GetPlot()->m_ptOrigin.x)*xrat;
			xRange[1] -= (pT->GetPlot()->m_ptCurr.x - pT->GetPlot()->m_ptOrigin.x)*xrat;
			pT->GetPlot()->SetXAutoRange(false);
			pT->GetPlot()->SetXRange(xRange[0], xRange[1]);
			break;
		case kRangeDragY:
			yRange[0] += (pT->GetPlot()->m_ptCurr.y - pT->GetPlot()->m_ptOrigin.y)*yrat;
			yRange[1] += (pT->GetPlot()->m_ptCurr.y - pT->GetPlot()->m_ptOrigin.y)*yrat;
			pT->GetPlot()->SetYAutoRange(false);
			pT->GetPlot()->SetYRange(yRange[0], yRange[1]);
			break;
		default:
			xRange[0] -= (pT->GetPlot()->m_ptCurr.x - pT->GetPlot()->m_ptOrigin.x)*xrat;
			xRange[1] -= (pT->GetPlot()->m_ptCurr.x - pT->GetPlot()->m_ptOrigin.x)*xrat;
			pT->GetPlot()->SetXAutoRange(false);
			pT->GetPlot()->SetXRange(xRange[0], xRange[1]);
			yRange[0] += (pT->GetPlot()->m_ptCurr.y - pT->GetPlot()->m_ptOrigin.y)*yrat;
			yRange[1] += (pT->GetPlot()->m_ptCurr.y - pT->GetPlot()->m_ptOrigin.y)*yrat;
			pT->GetPlot()->SetYAutoRange(false);
			pT->GetPlot()->SetYRange(yRange[0], yRange[1]);
		}

		needUpdate = true;

		pT->SetMsgHandled(true);
	}

	// Deal with range zoom
	else if(pT->GetPlot()->IsRangeZoomMode() && pT->GetPlot()->IsRangeZoomming())
	{
		pT->GetPlot()->SetRangeZoomming(false);
		pT->GetPlot()->m_ptCurr = point;

		double dt0[2], dtn[2];
		pT->GetPlot()->LPToData(&pT->GetPlot()->m_ptOrigin, dt0);
		pT->GetPlot()->LPToData(&pT->GetPlot()->m_ptCurr, dtn);

		if(dt0[0] < dtn[0] && dt0[1] != dtn[1])
		{
			ars.bXFloat = pT->GetPlot()->IsFloatXTicks();
			ars.bYFloat = pT->GetPlot()->IsFloatYTicks();

			double xRange[2], yRange[2];
			pT->GetPlot()->GetLastPlotRange(xRange, yRange);
			ars.fxLow = xRange[0];
			ars.fxHigh = xRange[1];
			ars.nXTicks = pT->GetPlot()->GetXTickCount();//pT->GetPlot()->GetXMainAxis()->GetTickCount();
			ars.nXMinorTicks = pT->GetPlot()->GetXMinorTickCount();//GetXMainAxis()->GetMinorTickCount();
			ars.fXTickMin = pT->GetPlot()->GetXTickMin();
			ars.fXTickMax = pT->GetPlot()->GetXTickMax();
			ars.bxAutoR = pT->GetPlot()->IsXAutoRange();
			ars.bxSet = pT->GetPlot()->IsXRangeSet();
			ars.fyLow = yRange[0];
			ars.fyHigh = yRange[1];
			ars.nYTicks = pT->GetPlot()->GetYTickCount();//GetYMainAxis()->GetTickCount();
			ars.nYMinorTicks = pT->GetPlot()->GetYMinorTickCount();//GetYMainAxis()->GetMinorTickCount();
			ars.fYTickMin = pT->GetPlot()->GetYTickMin();
			ars.fYTickMax = pT->GetPlot()->GetYTickMax();
			ars.byAutoR = pT->GetPlot()->IsYAutoRange();
			ars.bySet = pT->GetPlot()->IsYRangeSet();
			vARs.push_back(ars);

			pT->GetPlot()->SetFloatXTicks(true);
			pT->GetPlot()->SetFloatYTicks(true);
			
			pT->GetPlot()->SetXAutoRange(false);
			pT->GetPlot()->SetYAutoRange(false);

			pT->GetPlot()->SetXRange(dt0[0], dtn[0]);
			pT->GetPlot()->SetYRange(min(dt0[1], dtn[1]), max(dt0[1], dtn[1]));

			
		}
		else if(dt0[0] > dtn[0])
		{
			if(vARs.size()>0)
			{
				ars = vARs[vARs.size()-1];
				pT->GetPlot()->SetFloatXTicks(ars.bXFloat);
				pT->GetPlot()->SetFloatYTicks(ars.bYFloat);

				pT->GetPlot()->SetXRange(ars.fxLow, ars.fxHigh);
				pT->GetPlot()->SetXTickCount(ars.nXTicks);//GetXMainAxis()->SetTickCount(ars.nXTicks);
				pT->GetPlot()->SetXMinorTickCount(ars.nXMinorTicks);//GetXMainAxis()->SetMinorTickCount(ars.nXMinorTicks);
				pT->GetPlot()->SetXTickMin(ars.fXTickMin);
				pT->GetPlot()->SetXTickMax(ars.fXTickMax);
				pT->GetPlot()->SetXAutoRange(ars.bxAutoR);
				pT->GetPlot()->SetXRangeSet(ars.bxSet);//GetXMainAxis()->SetRangeSet(ars.bxSet);
				pT->GetPlot()->SetYRange(ars.fyLow, ars.fyHigh);
				pT->GetPlot()->SetYTickCount(ars.nYTicks);//GetYMainAxis()->SetTickCount(ars.nYTicks);
				pT->GetPlot()->SetYMinorTickCount(ars.nYMinorTicks);//GetYMainAxis()->SetMinorTickCount(ars.nYMinorTicks);
				pT->GetPlot()->SetYTickMin(ars.fYTickMin);
				pT->GetPlot()->SetYTickMax(ars.fYTickMax);
				pT->GetPlot()->SetYAutoRange(ars.byAutoR);
				pT->GetPlot()->SetYRangeSet(ars.bySet);//GetYMainAxis()->SetRangeSet(ars.bySet);
				vARs.pop_back();
			}
		}
		pT->GetPlot()->SetRangeZoomming(false);
		
		needUpdate = true;

		pT->SetMsgHandled(true);
	}

	ClipCursor(NULL);

	return needUpdate;
}

template<class HandlerT>
bool	CRangeManageHandler<HandlerT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	
	return needUpdate;
}

template<class HandlerT>
bool	CRangeManageHandler<HandlerT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;
	
	if(pT->GetPlot()->IsRangeDrag() && pT->GetPlot()->IsRangeDragging())
	{
		pT->GetPlot()->m_ptCurr = point;
		
		double xRange[2], yRange[2];
		RECT plotRect;
		memcpy(xRange,pT->GetPlot()->m_fXRange, 2*sizeof(double));
		memcpy(yRange,pT->GetPlot()->m_fYRange, 2*sizeof(double));
		plotRect = pT->GetPlot()->GetLastPlotRect();
		
		double xrat, yrat;
		xrat = (xRange[1] - xRange[0])/(plotRect.right!=plotRect.left?plotRect.right - plotRect.left:1);
		yrat = (yRange[1] - yRange[0])/(plotRect.bottom!=plotRect.top?plotRect.bottom - plotRect.top:1);
		
		switch(pT->GetPlot()->GetRangeDragType())
		{
		case kRangeDragX:
			xRange[0] -= (pT->GetPlot()->m_ptCurr.x - pT->GetPlot()->m_ptOrigin.x)*xrat;
			xRange[1] -= (pT->GetPlot()->m_ptCurr.x - pT->GetPlot()->m_ptOrigin.x)*xrat;
			pT->GetPlot()->SetXAutoRange(false);
			pT->GetPlot()->SetXRange(xRange[0], xRange[1]);
			break;
		case kRangeDragY:
			yRange[0] += (pT->GetPlot()->m_ptCurr.y - pT->GetPlot()->m_ptOrigin.y)*yrat;
			yRange[1] += (pT->GetPlot()->m_ptCurr.y - pT->GetPlot()->m_ptOrigin.y)*yrat;
			pT->GetPlot()->SetYAutoRange(false);
			pT->GetPlot()->SetYRange(yRange[0], yRange[1]);
			break;
		default:
			xRange[0] -= (pT->GetPlot()->m_ptCurr.x - pT->GetPlot()->m_ptOrigin.x)*xrat;
			xRange[1] -= (pT->GetPlot()->m_ptCurr.x - pT->GetPlot()->m_ptOrigin.x)*xrat;
			pT->GetPlot()->SetXAutoRange(false);
			pT->GetPlot()->SetXRange(xRange[0], xRange[1]);
			yRange[0] += (pT->GetPlot()->m_ptCurr.y - pT->GetPlot()->m_ptOrigin.y)*yrat;
			yRange[1] += (pT->GetPlot()->m_ptCurr.y - pT->GetPlot()->m_ptOrigin.y)*yrat;
			pT->GetPlot()->SetYAutoRange(false);
			pT->GetPlot()->SetYRange(yRange[0], yRange[1]);
		}
		
		needUpdate = true;
		pT->SetMsgHandled(true);
	}
	else if(pT->GetPlot()->IsRangeZoomMode() && pT->GetPlot()->IsRangeZoomming())
	{
		int nOldRop = SetROP2(hDC, R2_NOTXORPEN);
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(128, 128, 0));
		HPEN hOldPen = (HPEN) SelectObject(hDC, hPen);
		HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		
		Rectangle(hDC, pT->GetPlot()->m_ptOrigin.x, pT->GetPlot()->m_ptOrigin.y, pT->GetPlot()->m_ptCurr.x, pT->GetPlot()->m_ptCurr.y);
		pT->GetPlot()->m_ptCurr = point;
		Rectangle(hDC, pT->GetPlot()->m_ptOrigin.x, pT->GetPlot()->m_ptOrigin.y, pT->GetPlot()->m_ptCurr.x, pT->GetPlot()->m_ptCurr.y);
		
		SelectObject(hDC, hOldPen);
		DeleteObject(hPen);
		SelectObject(hDC, hOldBrush);
		DeleteObject(hBrush);
		
		SetROP2(hDC, nOldRop);
		
		needUpdate = false;
		pT->SetMsgHandled(true);
	}
	return needUpdate;
}

template<class HandlerT>
bool	CRangeManageHandler<HandlerT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CRangeManageHandler<HandlerT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

template<class HandlerT>
bool	CRangeManageHandler<HandlerT>::KeyDown( HDC hDC, UINT key )
{
	HandlerT* pT = static_cast<HandlerT*>(this);

	return false;
}

Declare_Namespace_End