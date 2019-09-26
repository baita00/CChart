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

#include "DigitalClockImpl.h"

using namespace NsCChart;

CDigitalClockImpl::CDigitalClockImpl()
{

}

CDigitalClockImpl::~CDigitalClockImpl()
{

}

void	CDigitalClockImpl::OnDraw(HWND hWnd, RECT destRect)
{
	HDC hDC = GetDC(hWnd);
	OnDraw(hDC, destRect);
	ReleaseDC(hWnd, hDC);
}

void	CDigitalClockImpl::OnDraw(HDC hDC, RECT destRect)
{
	//RECT plotRect = GetPlotRect(hDC, destRect);
	//DrawSSCs(hDC, plotRect);
	RECT plotRect, ymdRect, hmsRect;
	GetYmdAdnHmsRect(destRect, &plotRect, &ymdRect, &hmsRect);
	if(m_bIncludeYMD)DrawYMD(hDC, ymdRect);
	DrawHMS(hDC, hmsRect);
}

void	CDigitalClockImpl::GetYmdAdnHmsRect(RECT destRect, RECT *plotRect, RECT *ymdRect, RECT *hmsRect)
{
	*plotRect = destRect;
	*ymdRect = destRect;
	*hmsRect = destRect;

	SIZE size;
	
	double dx1, dx2, dy;
	size = m_SSC.GetSize(0);
	dx1 = size.cx;
	dy = size.cy;
	size = m_SSC.GetSize(10);
	dx2 = size.cx;
	
	InflateRect(plotRect, -m_nMarginHorizontal, -m_nMarginVertical);
	
	double cx1, cx2, cy;
	cx1 = dx1 * 6 + dx2 * 2;
	cx2 = dx1 * 8 + dx2 * 2;
	cy = dy;
	
	if(!m_bIncludeYMD)
	{
		plotRect->right = long(plotRect->left + cx1);
		plotRect->bottom = long(plotRect->top + cy);

		*ymdRect = *plotRect;
		*hmsRect = *plotRect;
	}
	else
	{
		if(m_bSingleLine)
		{
			plotRect->right = long(plotRect->left + cx1 + cx2 + dx1);
			plotRect->bottom = long(plotRect->top + cy);

			*ymdRect = *plotRect;
			ymdRect->right = long(ymdRect->left + cx2);
			*hmsRect = *plotRect;
			hmsRect->left = long(hmsRect->right - cx1);
		}
		else
		{
			plotRect->right = long(plotRect->left + cx2);
			plotRect->bottom = long(plotRect->top + 2.0*cy);

			*ymdRect = *plotRect;
			ymdRect->bottom = long(ymdRect->top + cy);
			
			*hmsRect = *plotRect;
			hmsRect->left = plotRect->left + (long)dx1;
			hmsRect->right = plotRect->right - (long)dx1;
			hmsRect->top = plotRect->bottom - (long)cy;
		}
	}
}

void	CDigitalClockImpl::DrawHMS(HDC hDC, RECT hmsRect)
{
	SIZE size;
	
	double dx1, dx2, dy;
	size = m_SSC.GetSize(0);
	dx1 = size.cx;
	dy = size.cy;
	size = m_SSC.GetSize(10);
	dx2 = size.cx;

/*	SIZE size1, size2;
	size1 = m_SSC.GetSize(0);
	size2 = m_SSC.GetSize(10);*/
	
	RECT sscRect =hmsRect;
	RECT sscRect2 = hmsRect;
	
	int hour, minute, second;
	hour = m_nHour%12;
	minute = m_nMinute%60;
	second = m_nSecond%60;
	
	sscRect.right = sscRect.left + (long)dx1;
	sscRect2.right = sscRect2.left + (long)dx2;
	
	m_SSC.SetCurrValue(hour/10);
	m_SSC.OnDraw(hDC, sscRect);
	
	OffsetRect(&sscRect, (int)dx1, 0);
	m_SSC.SetCurrValue(hour - (hour/10)*10);
	m_SSC.OnDraw(hDC, sscRect);
	
	OffsetRect(&sscRect, (int)dx1, 0);
	sscRect2 = sscRect;
	sscRect2.right = sscRect2.left + (long)dx2;
	m_SSC.SetCurrValue(m_bShowColon?11:12);
	m_SSC.OnDraw(hDC, sscRect2);
	
	OffsetRect(&sscRect, (int)dx2, 0);
	m_SSC.SetCurrValue(minute/10);
	m_SSC.OnDraw(hDC, sscRect);
	
	OffsetRect(&sscRect, (int)dx1, 0);
	m_SSC.SetCurrValue(minute - (minute/10)*10);
	m_SSC.OnDraw(hDC, sscRect);
	
	OffsetRect(&sscRect, int(dx1 + dx2), 0);
	m_SSC.SetCurrValue(second/10);
	m_SSC.OnDraw(hDC, sscRect);
	
	OffsetRect(&sscRect, (int)dx1, 0);
	m_SSC.SetCurrValue(second - (second/10)*10);
	m_SSC.OnDraw(hDC, sscRect);
}

void	CDigitalClockImpl::DrawYMD(HDC hDC, RECT ymdRect)
{
	SIZE size;
	
	double dx1, dx2, dy;
	size = m_SSC.GetSize(0);
	dx1 = size.cx;
	dy = size.cy;
	size = m_SSC.GetSize(10);
	dx2 = size.cx;

/*	SIZE size1, size2;
	size1 = m_SSC.GetSize(0);
	size2 = m_SSC.GetSize(10);*/
	
	RECT sscRect =ymdRect;
	RECT sscRect2 = ymdRect;
	
	int year, month, day;
	year = m_nYear;
	month = m_nMonth;
	day = m_nDay;
	
	sscRect.right = sscRect.left + (long)dx1;
	sscRect2.right = sscRect2.left + (long)dx2;
	
	//year
	m_SSC.SetCurrValue(year/1000);
	m_SSC.OnDraw(hDC, sscRect);
	year -= (year/1000)*1000;
	
	OffsetRect(&sscRect, (int)dx1, 0);
	m_SSC.SetCurrValue(year/100);
	m_SSC.OnDraw(hDC, sscRect);
	year -= (year/100)*100;

	OffsetRect(&sscRect, (int)dx1, 0);
	m_SSC.SetCurrValue(year/10);
	m_SSC.OnDraw(hDC, sscRect);
	year -= (year/10)*10;

	OffsetRect(&sscRect, (int)dx1, 0);
	m_SSC.SetCurrValue(year);
	m_SSC.OnDraw(hDC, sscRect);

	// dot
	OffsetRect(&sscRect, (int)dx1, 0);
	m_SSC.SetCurrValue(10);
	sscRect2 = sscRect;
	sscRect2.right = sscRect2.left + (long)dx2;
	m_SSC.OnDraw(hDC, sscRect2);

	//month
	OffsetRect(&sscRect, (int)dx2, 0);
	m_SSC.SetCurrValue(month/10);
	m_SSC.OnDraw(hDC, sscRect);
	month -= (month/10)*10;
	
	OffsetRect(&sscRect, (int)dx1, 0);
	m_SSC.SetCurrValue(month);
	m_SSC.OnDraw(hDC, sscRect);
	
	// dot
	OffsetRect(&sscRect, (int)dx1, 0);
	m_SSC.SetCurrValue(10);
	sscRect2 = sscRect;
	sscRect2.right = sscRect2.left + (long)dx2;
	m_SSC.OnDraw(hDC, sscRect2);

	//day
	OffsetRect(&sscRect, (int)dx2, 0);
	m_SSC.SetCurrValue(day/10);
	m_SSC.OnDraw(hDC, sscRect);
	day -= (day/10)*10;
	
	OffsetRect(&sscRect, (int)dx1, 0);
	m_SSC.SetCurrValue(day);
	m_SSC.OnDraw(hDC, sscRect);
	
}
