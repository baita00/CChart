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

#include "CloudPlot.h"
#include <math.h>
#include "../../Basic/PlotUtility.h"
#include "../ContourLinePlot/ListContour.h"

using namespace NsCChart;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace NsCChart{
extern double TestFunction(double x, double y);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation


CCloudPlot::CCloudPlot()
{
	m_hMemImageDC = NULL;

	//SetFieldFcn(TestFunction);
	//SetFieldFcn(0);

	SetDefaults();
}

CCloudPlot::~CCloudPlot()
{
	//if(m_pLegend) delete m_pLegend;
	ReleaseMemDC();
}

void	CCloudPlot::SetDefaults()
{
	//m_nWidth = 320;//GetSystemMetrics(SM_CXSCREEN);//640;
	//m_nHeight = 240;//GetSystemMetrics(SM_CYSCREEN);//480;
	m_nWidth = GetSystemMetrics(SM_CXSCREEN);
	m_nHeight = GetSystemMetrics(SM_CYSCREEN);

	m_bContourChanged = true;

	m_fMaxValue = m_fMinValue = 0.0;

	m_bExponential = false;

	m_hMemImageDC = 0;
	m_hOldBmp = 0;

	m_bRightAxisAsLegend = true;
	m_bTopAxisAsLegend = false;
	m_bLeftAxisAsLegend = false;
	m_bBottomAxisAsLegend = false;

	SetContourXRange(-5, 5);
	SetContourYRange(-5, 5);
}


void	CCloudPlot::CreateMemDC(HWND hWnd)
{
	HDC hDC=GetDC(hWnd);
	HBITMAP hMemBmp;
	int	maxX=GetSystemMetrics(SM_CXSCREEN);
	int	maxY=GetSystemMetrics(SM_CYSCREEN);
	
	hMemBmp=CreateCompatibleBitmap(hDC,maxX,maxY);
	ReleaseMemDC();

	m_hMemImageDC=CreateCompatibleDC(hDC);
	ReleaseDC(hWnd,hDC);
	
	m_hOldBmp = (HBITMAP)SelectObject(m_hMemImageDC,hMemBmp); 
	RECT rect; 
	SetRect(&rect,0,0,maxX,maxY);
	HBRUSH hBrush, hOldBrush;
	hBrush = CreateSolidBrush(RGB(255,255,255)); 
	hOldBrush = (HBRUSH)SelectObject(m_hMemImageDC,hBrush); 
	FillRect(m_hMemImageDC,&rect,hBrush);
	SelectObject(m_hMemImageDC, hOldBrush);
	DeleteObject(hBrush);
}

void	CCloudPlot::ReleaseMemDC()
{
	if(m_hMemImageDC)
	{
		HBITMAP hBmp = (HBITMAP)SelectObject(m_hMemImageDC, m_hOldBmp);
		if(hBmp)DeleteObject(hBmp);

		DeleteDC(m_hMemImageDC);
		m_hMemImageDC = 0;
	}
}

bool	CCloudPlot::CreateImage()
{
	double *pData = NULL;
	pData = new double[m_nWidth*m_nHeight];
	if(!pData)return false;
	if(!GetFieldFcn())
	{
		delete []pData;
		return false;
	}

	double dx,dy,x0,y0;
	double xRange[2], yRange[2];
	GetContourRange(xRange, yRange);
	x0 = xRange[0];
	y0 = yRange[0];
	dx = (xRange[1] - xRange[0])/m_nWidth;
	dy = (yRange[1] - yRange[0])/m_nHeight;


	// Firstly, calculate the value on the array
	double maxval,minval;
	double x, y;
	int i, j;
	for(i=0; i<m_nHeight; i++)
	{
		for(j=0; j<m_nWidth; j++)
		{
			x = x0 + (j+0.5)*dx;
			y = y0 + (i+0.5)*dy;
			
			pData[j + i*m_nWidth ] = GetFieldFcn()(x, y); 
			
			if(i == 0 && j == 0)
			{
				maxval = minval =pData[0];
			}
			else
			{
				if(pData[j + i*m_nWidth ] > maxval)maxval = pData[j + i*m_nWidth ];
				if(pData[j + i*m_nWidth ] < minval)minval = pData[j + i*m_nWidth ];
			}
		}
	}
	
	//m_fMaxValue = maxval;
	//m_fMinValue = minval;
	double inRange[2]={minval, maxval};
	double outRange[2];
	int nTicks, nMinor;
	GetOptimalRange(inRange, outRange, &nTicks, &nMinor, true, false, true, false, false, false);
	m_fMaxValue = outRange[1];
	m_fMinValue = outRange[0];

	bool bExp = m_bExponential;
	if(outRange[0] <= 0.0)bExp = false;
	
	if(bExp)
	{
		for(i=0; i<m_nWidth*m_nHeight; i++)
		{
			pData[i] = log10(pData[i]);
		}
		outRange[1] = log10(outRange[1]);
		outRange[0] = log10(outRange[0]);
	}
	
	// Then set the color at every point
	short r,g,b;
	short c1,c2;
	double val;
	if(outRange[1] == outRange[0])
	{
		r = ( GetRValue(m_crColor1) + GetRValue(m_crColor2) )/2;
		g = ( GetGValue(m_crColor1) + GetGValue(m_crColor2) )/2;
		b = ( GetBValue(m_crColor1) + GetBValue(m_crColor2) )/2;
		for(i=0; i<m_nHeight; i++)
		{
			for(j=0; j<m_nWidth; j++)
			{
				SetPixel(m_hMemImageDC, j, m_nHeight-1-i, RGB(r,g,b));
			}
		}
		delete []pData;
		return true;
	}
	
	for(i=0; i<m_nHeight; i++)
	{
		for(j=0; j<m_nWidth; j++)
		{
			val = pData[j + i*m_nWidth ];
			c1 = GetRValue(m_crColor1); c2 = GetRValue(m_crColor2);
			r = short( c1 + (c2 - c1)/(outRange[1] - outRange[0])*(val - outRange[0]) );
			c1 = GetGValue(m_crColor1); c2 = GetGValue(m_crColor2);
			g = short( c1 + (c2 - c1)/(outRange[1] - outRange[0])*(val - outRange[0]) );
			c1 = GetBValue(m_crColor1); c2 = GetBValue(m_crColor2);
			b = short ( c1 + (c2 - c1)/(outRange[1] - outRange[0])*(val - outRange[0]) );
			SetPixel(m_hMemImageDC, j, m_nHeight-1-i, RGB(r,g,b));
		}
	}

	delete []pData;

	
	m_bContourChanged = false;
	return true;
}

bool	CCloudPlot::UpdateImage(RECT plotRect)
{
	int w = abs(Width(plotRect));
	int h = abs(Height(plotRect));

	if(w == 0 || h == 0)return false;
	if(w<64 && h<64)return false;
	if(w/(double)m_nWidth<1.28 && m_nWidth/(double)w<1.28 && h/(double)m_nHeight<1.28 && m_nHeight/(double)h<1.28)return false;

	m_nWidth = 8*((w-1)/8+1);
	m_nHeight = 8*((h-1)/8+1);

	return CreateImage();
}

void	CCloudPlot::DrawCloud(HDC hDC, RECT plotRect, bool bXOpposite, bool bYOpposite)
{
	//RECT plotRect = GetLastPlotRect();
	
	int l, t, w, h;
	if(!bXOpposite)
	{
		l = plotRect.left;
		w = abs(Width(plotRect));
	}
	else
	{
		l = plotRect.right;
		w = -abs(Width(plotRect));
	}
	if(!bYOpposite)
	{
		t = plotRect.top;
		h = abs(Height(plotRect));
	}
	else
	{
		t = plotRect.bottom;
		h = -abs(Height(plotRect));
	}
	SetStretchBltMode(hDC, HALFTONE);
	SetBrushOrgEx(hDC, 0, 0, NULL);
	StretchBlt(hDC, l, t, w, h, m_hMemImageDC, 0, 0, m_nWidth, m_nHeight, SRCCOPY);
}