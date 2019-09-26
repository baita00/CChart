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

#include "PlotImpl.h"
#include "Global.h"
#include "../Resources/PlotResources.h"

using namespace NsCChart;

CPlotImpl::CPlotImpl()
{
	if(CGlobal::m_bLoadDefPatterns)
	{
		HBITMAP hb = (HBITMAP)LoadImage(CGlobal::MyGetModuleHandle(), MAKEINTRESOURCE(IDB_BACKGROUND_IMAGE), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
		if(hb)
		{
			SetBkgndImage(hb);
			SetBkgndPattern(hb);
			SetDataRegionPattern(hb);
			SetTitlePattern(hb);
			DeleteObject(hb);
		}
	}
	//SetBLAxis();
	if(CGlobal::m_bAddDefAxes)
	{
		AddBLAxis();
	}
	memset(&m_rctClient, 0, sizeof(RECT));
	memset(&m_rctPlot, 0, sizeof(RECT));
}

CPlotImpl::~CPlotImpl()
{

}


RECT	CPlotImpl::GetPlotRect( HDC hDC, RECT clientRect )
{
	RECT plotRect = clientRect;

	if(m_bNothingElse)
	{
		if(m_bEqualXYAxis)
		{
			SquareRect(plotRect);
		}
		else if(m_bEqualXYScale )
		{
			double xRange[2], yRange[2];
			GetRange(xRange, yRange);
			ScaleRect(plotRect, xRange[0], xRange[1], yRange[0], yRange[1]);
		}
		return plotRect;
	}

	if(IsAxesVisible())
	{
		plotRect.left += m_nMarginLeft;
		plotRect.right -= m_nMarginRight;
		plotRect.top += m_nMarginTop;
		plotRect.bottom -= m_nMarginBottom;
	}
	else
	{
		plotRect.left += max(m_nMarginLeft, m_nMinMargin);
		plotRect.right -= max(m_nMarginRight, m_nMinMargin);
		plotRect.top += max(m_nMarginTop, m_nMinMargin);
		plotRect.bottom -= max(m_nMarginBottom, m_nMinMargin);
	}
	
	plotRect = AdjustByTitle(hDC, plotRect);
	plotRect = AdjustByAxes(hDC, clientRect, plotRect);
	plotRect = AdjustPlotSizeForHeadernote(hDC, plotRect);
	plotRect = AdjustPlotSizeForFootnote(hDC, plotRect);
	plotRect = AddtionalPlotRectAdjust(hDC, plotRect);

	// Equal XY Axis
	if( m_bEqualXYAxis )
	{
		SquareRect(plotRect);
	}
	else if(m_bEqualXYScale)
	{
		double xRange[2], yRange[2];
		GetRange(xRange, yRange);
		ScaleRect(plotRect, xRange[0], xRange[1], yRange[0], yRange[1]);
	}

	NormalizeRect(plotRect);
	return plotRect;
}

RECT	CPlotImpl::AdjustTitleRect(HDC hDC, RECT titleRect)
{
	if(GetTitlePosition()==kPositionT)
	{
		if( m_strHeadernote != _TEXT("") && m_bShowHeadernote )
		{
			SIZE		tmSize={0,0};
			MyGetTextExtentPoint(hDC, &m_lfHeadernoteFont, m_strHeadernote, DT_LEFT, &tmSize);
			titleRect.bottom += tmSize.cy;
			titleRect.top += tmSize.cy;
		}
	}
	if(GetTitlePosition()==kPositionB)
	{
		if( m_strFootnote != _TEXT("") && m_bShowFootnote )
		{
			SIZE		tmSize={0,0};
			MyGetTextExtentPoint(hDC, &m_lfFootnoteFont, m_strFootnote, DT_LEFT, &tmSize);
			titleRect.bottom -= tmSize.cy;
			titleRect.top -= tmSize.cy;
		}
	}
	return titleRect;
}

void	CPlotImpl::OnDraw()
{
	if(m_hWnd == 0)return;
	m_bRectConfined?OnDraw(m_hWnd, m_rctConfined):OnDraw(m_hWnd);
}

void	CPlotImpl::OnDraw( HWND hWnd )
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	HDC hDC = GetDC(hWnd);
	
	FinalDraw(hDC, rect);

	ReleaseDC(hWnd, hDC);
}

void	CPlotImpl::OnDraw( HWND hWnd, RECT destRect )
{
	if(!IsWindow(hWnd))return;
	HDC hDC = GetDC(hWnd);
	
	FinalDraw(hDC, destRect);
	
	ReleaseDC(hWnd, hDC);
}

void	CPlotImpl::OnDraw(HDC hDC)
{
	HWND hWnd = WindowFromDC(hDC);
	if(!IsWindow(hWnd))return;
	
	RECT destRect;
	GetClientRect(hWnd,&destRect);
	
	FinalDraw(hDC, destRect);
}

void	CPlotImpl::OnDraw(HDC hDC, RECT destRect)
{
	FinalDraw(hDC, destRect);
}

void	CPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	// Save the used rectangles
	m_rctClient = destRect;
	m_rctPlot = GetPlotRect(hDC, destRect);

	// For user action in CPlotBaseImpl, especially for CPlotWnd.
	// In fact, these line is useless internally.
	HWND theHWnd = WindowFromDC(hDC);
	if(theHWnd && !m_hWnd)m_hWnd = theHWnd;
	m_rctConfined = destRect;

	if(m_bNothingElse)
	{
		m_rctPlot = destRect;
		return;
	}
//	int nOldMapmode = SetMapMode(hDC, MM_TEXT);

	DrawCanvas(hDC);
	DrawTitle(hDC);
	DrawGrids(hDC);
	DrawAxes(hDC);
	DrawPageNotes(hDC);
	DrawComments(hDC);

	// User drawing actions
	if(m_fcnUserDrawing)
	{
		(*m_fcnUserDrawing)(this, hDC, m_rctClient, m_rctPlot, m_pParaUserDrawing);
	}
	
	// User update actions
	if(m_fcnUserUpdate)
	{
		(*m_fcnUserUpdate)(this, m_pParaUserUpdate);
	}
	for(int i=0; i<(int)m_vFcnUserUpdate.size(); i++)
	{
		if(m_vFcnUserUpdate[i].fcn)
		{
			(*m_vFcnUserUpdate[i].fcn)(this, m_vFcnUserUpdate[i].pPara);
		}
	}

	m_bFirstDraw = false;

//	SetMapMode(hDC, nOldMapmode);
}

void	CPlotImpl::Update(HDC hDC, RECT destRect)
{
	OnDraw(hDC, destRect);
}

// The default functions do nothing to the passed things
void	CPlotImpl::DataToLP( double *data, LPPOINT point )
{
	RECT		plotRect;
	double		xRange[2], yRange[2];
	double		scaleX, scaleY;

	if( point == NULL ) return;

	plotRect = GetLastPlotRect();

	GetRange( xRange, yRange );

	scaleX = (xRange[1] - xRange[0] ) / abs(plotRect.right - plotRect.left);
	scaleY = (yRange[1] - yRange[0] ) / abs(plotRect.bottom - plotRect.top);

	if( scaleX == 0. || scaleY == 0. ) return;

	if(!m_bXOpposite)
		point->x = plotRect.left + (long)((data[0]-xRange[0]) / scaleX) ;
	else
		point->x = plotRect.right - (long)((data[0]-xRange[0]) / scaleX) ;
	if(!m_bYOpposite)
		point->y = plotRect.bottom - (long)((data[1]-yRange[0]) / scaleY) ;
	else
		point->y = plotRect.top + (long)((data[1]-yRange[0]) / scaleY) ;
}

void	CPlotImpl::LPToData( LPPOINT point, double *data )
{
	RECT		plotRect;
	double		xRange[2], yRange[2];
	double		scaleX, scaleY;
	
	if( point == NULL ) return;

	plotRect = GetLastPlotRect( );

	GetRange( xRange, yRange );

	scaleX = (xRange[1] - xRange[0] ) / abs(plotRect.right - plotRect.left);
	scaleY = (yRange[1] - yRange[0] ) / abs(plotRect.bottom - plotRect.top);

	if( scaleX == 0. || scaleY == 0. ) return;

	if(!m_bXOpposite)
		data[0] = (point->x-plotRect.left) * scaleX + xRange[0];
	else
		data[0] = (plotRect.right-point->x) * scaleX + xRange[0];
	if(!m_bYOpposite)
		data[1] = (plotRect.bottom-point->y) * scaleY + yRange[0];
	else
		data[1] = (point->y-plotRect.top) * scaleY + yRange[0];
}

void	CPlotImpl::DataToLPX( double *pX, long *pPtx, int nCnt)
{
	RECT		plotRect;
	double		xRange[2];
	double		scaleX;
	
	if( pX == NULL ) return;
	
	plotRect = GetLastPlotRect();
	
	GetXRange( xRange );
	
	scaleX = (xRange[1] - xRange[0] ) / abs(plotRect.right - plotRect.left);
	
	if( scaleX == 0.) return;
	
	for(int i=0; i<nCnt; i++)
	{
		if(!m_bXOpposite)
			pPtx[i] = plotRect.left + (long)((pX[i]-xRange[0]) / scaleX) ;
		else
			pPtx[i] = plotRect.right - (long)((pX[i]-xRange[0]) / scaleX) ;
	}	
}

void	CPlotImpl::LPToDataX( long *pPtx, double *pX, int nCnt)
{
	RECT		plotRect;
	double		xRange[2];
	double		scaleX;
	
	if( pPtx == NULL ) return;
	
	plotRect = GetLastPlotRect( );
	
	GetXRange( xRange );
	
	scaleX = (xRange[1] - xRange[0] ) / abs(plotRect.right - plotRect.left);
	
	if( scaleX == 0. ) return;
	
	for(int i=0; i<nCnt; i++)
	{
		if(!m_bXOpposite)
			pX[i] = (pPtx[i]-plotRect.left) * scaleX + xRange[0];
		else
			pX[i] = (plotRect.right-pPtx[i]) * scaleX + xRange[0];
	}
	
}

void	CPlotImpl::DataToLPY( double *pY, long *pPty, int nCnt)
{
	RECT		plotRect;
	double		yRange[2];
	double		scaleY;
	
	if( pY == NULL ) return;
	
	plotRect = GetLastPlotRect();
	
	GetYRange( yRange );
	
	scaleY = (yRange[1] - yRange[0] ) / abs(plotRect.bottom - plotRect.top);
	
	if( scaleY == 0. ) return;
	
	for(int i=0; i<nCnt; i++)
	{
		if(!m_bYOpposite)
			pPty[i] = plotRect.bottom - (long)((pY[i]-yRange[0]) / scaleY) ;
		else
			pPty[i] = plotRect.top + (long)((pY[i]-yRange[0]) / scaleY) ;
	}
	
}

void	CPlotImpl::LPToDataY( long *pPty, double *pY, int nCnt)
{
	RECT		plotRect;
	double		yRange[2];
	double		scaleY;
	
	if( pPty == NULL ) return;
	
	plotRect = GetLastPlotRect( );
	
	GetYRange( yRange );
	
	scaleY = (yRange[1] - yRange[0] ) / abs(plotRect.bottom - plotRect.top);
	
	if( scaleY == 0. ) return;
	
	for(int i=0; i<nCnt; i++)
	{
		if(!m_bYOpposite)
			pY[i] = (plotRect.bottom-pPty[i]) * scaleY + yRange[0];
		else
			pY[i] = (pPty[i]-plotRect.top) * scaleY + yRange[0];
	}
	
}

int		CPlotImpl::RegionIdentify( HDC hDC, POINT point )
{
	CAxis *pAxis=0;
	RECT theRect;
	
	theRect=GetLastPlotRect( );
	if( PtInRect(&theRect,point) )
		return kXYRegionData;
	theRect=GetTitleRect(hDC);
	if( PtInRect(&theRect,point) )
		return kXYRegionTitle;
	CAxisImpl *axis;
	for(int i=0;i<GetAxesCount();i++)
	{
		axis=GetAxis(i);
		theRect=axis->GetPlotRect(hDC);//, GetLastClientRect(), GetLastPlotRect( ));
		if( PtInRect(&theRect,point) )
		{
			if(axis->GetLocation()==kLocationLeft)return kXYRegionLeft;
			else if(axis->GetLocation()==kLocationRight)return kXYRegionRight;
			else if(axis->GetLocation()==kLocationBottom)return kXYRegionBottom;
			else if(axis->GetLocation()==kLocationTop)return kXYRegionTop;
		}
	}
	//outer axis region
	
	return kXYRegionOut;
}

bool	CPlotImpl::PtInDataRegion(HDC hDC, POINT point)
{
	RECT plotRect = GetLastPlotRect( );
	
	return PtInRect(&plotRect, point)==TRUE;
}

void	CPlotImpl::SetApperanceMode(int mode)
{
	m_nApperanceMode = mode;

	int i;
	for(i=0; i<(int)GetAxes().size(); i++)
	{
		GetAxes()[i]->SetApperanceMode(mode);
	}
	
	switch(mode)
	{
	case kExtraSmall:
		
		m_lfTitleFont.lfHeight = 12;
		m_lfTitleFont.lfWeight = FW_THIN;
	
		m_lfFootnoteFont.lfHeight = 8;
		m_lfHeadernoteFont.lfHeight = 8;
		m_lfFootnoteFont.lfWeight = FW_THIN;
		m_lfHeadernoteFont.lfWeight = FW_THIN;

		break;
	case kSmall:
		m_lfTitleFont.lfHeight = 18;
		m_lfTitleFont.lfWeight = FW_LIGHT;

		m_lfFootnoteFont.lfHeight = 12;
		m_lfHeadernoteFont.lfHeight = 12;
		m_lfFootnoteFont.lfWeight = FW_LIGHT;
		m_lfHeadernoteFont.lfWeight = FW_LIGHT;
		break;
	case kNormal:
		m_lfTitleFont.lfHeight = 24;
		m_lfTitleFont.lfWeight = FW_NORMAL;

		m_lfFootnoteFont.lfHeight = 16;
		m_lfHeadernoteFont.lfHeight = 16;
		m_lfFootnoteFont.lfWeight = FW_NORMAL;
		m_lfHeadernoteFont.lfWeight = FW_NORMAL;
		break;
	case kLarge:
		m_lfTitleFont.lfHeight = 36;
		m_lfTitleFont.lfWeight = FW_BOLD;

		m_lfFootnoteFont.lfHeight = 20;
		m_lfHeadernoteFont.lfHeight = 20;
		m_lfFootnoteFont.lfWeight = FW_BOLD;
		m_lfHeadernoteFont.lfWeight = FW_BOLD;
		break;
	case kExtraLarge:
		m_lfTitleFont.lfHeight = 48;
		m_lfTitleFont.lfWeight = FW_ULTRABOLD;

		m_lfFootnoteFont.lfHeight = 24;
		m_lfHeadernoteFont.lfHeight = 24;
		m_lfFootnoteFont.lfWeight = FW_ULTRABOLD;
		m_lfHeadernoteFont.lfWeight = FW_ULTRABOLD;
		break;
	default:
		break;
	}
}

void		CPlotImpl::ResetApperance()
{
	CPlot::SetApperanceMode(GetApperanceMode());
	for(int i=0; i<GetAxesCount(); i++)
	{
		GetAxis(i)->SetApperanceMode(GetApperanceMode());
	}
}

