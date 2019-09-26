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

#include "RadarPlotImpl.h"
#include "../../Basic/MyConst.h"
#include "../../Basic/MyMemDC.h"

using namespace NsCChart;

CRadarPlotImpl::CRadarPlotImpl()
{
	SetEqualXYAxis(true);
	SetEqualXYRange(true);

	CAxis *axis;
	axis = AddAxis(kLocationBottom);
	axis->SetVisible(false);
	axis = AddAxis(kLocationLeft);
	axis->SetVisible(false);

	SetLegendShow(false);
}

CRadarPlotImpl::~CRadarPlotImpl()
{

}

void	CRadarPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	if(GetPlotDataCount() < 1)
		return;
	else if(GetPlotDataCount() == 1)
		InitRadarData();
	
	int nOldMapmode = SetMapMode(hDC, MM_TEXT);

	double range[2], maxval, edge;
	GetRadarRange(range, maxval, edge);


	RECT plotRect = GetPlotRect(hDC, destRect);
	SetLastClientRect(destRect);
	SetLastPlotRect(plotRect);

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
	
	DrawBkgnd(hdc);	

	bool bErase = IsEraseBkgnd();
	SetEraseBkgnd(false);
	CXYPlotImpl::FinalDraw(hdc, destRect);
	SetEraseBkgnd(bErase);

	DrawRadarFrame(hdc, plotRect, maxval, edge);
	DrawRadarGrids(hdc, plotRect, maxval, edge);
	DrawRadarTitle(hdc, plotRect, maxval, edge);

	SetMapMode(hDC, nOldMapmode);
}

int		CRadarPlotImpl::AddRadarLine(double val, tstring title)
{
	if(val < 0.0)return -1;

	int dataID;
	if(GetPlotDataCount() <= 0)
	{
		double pY[1];
		pY[0] = val;
		dataID = AddCurve(pY, 1);
		SetVisible(dataID, false);
		SetDataTitle(dataID, _T("Initial radar data"));

		m_vstrTitles.push_back(title);
		return dataID;
	}
	else
	{
		DataPoint2D dp;
		dp.val[0] = 0;
		dp.val[1] = val;
		GetPlotData(GetDataID(0))->GetData().push_back(dp);
		m_vstrTitles.push_back(title);
		return GetDataID(0);
	}
}

void	CRadarPlotImpl::GetRadarRange(double range[2], double &maxval, double &edge)
{
	range[0] = fMaxVal;
	range[1] = - fMaxVal;

	int nRadarLines = (int)GetAbsData(0).size(); 
	for( int i=0; i<nRadarLines; i++)
	{
		if(GetAbsData(0)[i].val[1] < range[0])range[0] = GetAbsData(0)[i].val[1];
		if(GetAbsData(0)[i].val[1] > range[1])range[1] = GetAbsData(0)[i].val[1];
	}

	maxval = max(fabs(range[0]), fabs(range[1]));
	if(maxval <= 0)return;
	
	int epsl;
	double res, resn;
	double sig = maxval>0?1.0:-1.0;
	
	epsl=(int)floor( log10( fabs(maxval) ) );
	res = fabs(maxval) / pow(10.0, epsl);	
	resn = ceil(res);
	if(resn == res)resn++;
	maxval = sig * resn * pow(10.0, epsl);
	edge = 1.0 * pow(10.0, epsl);

	SetXRange(-maxval-edge, maxval+edge);
	SetYRange(-maxval-edge, maxval+edge);

	m_bNewDataComming = false;
}

double	CRadarPlotImpl::GetStartAngle()
{
	if(GetPlotDataCount()<=0)return 0.0;
	
	if(GetAbsData(0).size() <= 3)return 0.0;
		
	int nRadarLines = (int)GetAbsData(0).size();
	
	double sa = 0;
	if(nRadarLines%2 == 1)
	{
		sa = myPi/nRadarLines -myPi/2.0;
	}
	return sa;
}

void	CRadarPlotImpl::InitRadarData()
{
	if(GetPlotDataCount()<=0)return;

	if(GetAbsData(0).size() <= 3)return;

	while(GetPlotDataCount() >= 2)
	{
		DeleteData(GetDataID(GetPlotDataCount()-1));
	}

	int nRadarLines = (int)GetAbsData(0).size();

	double *pX, *pY;
	pX = new double[nRadarLines];
	pY = new double[nRadarLines];

	double tht0 = GetStartAngle();

	double radius, tht;
	for(int i=0; i<nRadarLines; i++)
	{
		radius = GetAbsData(0)[i].val[1];
		tht = i*2.0*myPi/nRadarLines + tht0;
		pX[i] = radius * cos(tht);
		pY[i] = radius * sin(tht);
	}
	int dataID = AddCurve(pX, pY, nRadarLines);
	SetDataTitle(dataID, _TEXT("Radar data for plotting"));
	SetDataFill(dataID, true);
	SetDataFillMode(dataID, kDataFillClosed);

	delete []pY;
	delete []pX;
}

void	CRadarPlotImpl::DrawRadarGrids(HDC hDC, RECT plotRect, double maxval, double edge)
{
	int ctX, ctY;
	ctX = (plotRect.left + plotRect.right)/2;
	ctY = (plotRect.bottom + plotRect.top)/2;

	double scaleX, scaleY;
	scaleX = (plotRect.right - plotRect.left)/(maxval+edge)/2.0;
	scaleY = (plotRect.bottom - plotRect.top)/(maxval+edge)/2.0;

	int nRadarLines = (int)GetAbsData(0).size();

	HPEN hPen, hOldPen;
	hPen = CreatePen(m_nRadarGridStyle, m_nRadarGridWidth, m_crRadarGridColor);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	double tht0 = GetStartAngle();

	int i, j;

	int x, y;
	double tht, dtht = -2.0*myPi/nRadarLines;
	//
	//Draw radius
	for(i=0; i<nRadarLines; i++)
	{
		tht = i*dtht + tht0;
		x = int(ctX + maxval * scaleX * cos(tht));
		y = int(ctY - maxval * scaleY * sin(tht));
		MoveToEx(hDC, ctX, ctY, NULL);
		LineTo(hDC, x, y);
	}
	//

	int nRGrids;
	nRGrids =int( maxval / edge);
	if(nRGrids <= 3)nRGrids = 4;
	for(j=1; j<nRGrids; j++)
	{
		for(i=0; i<nRadarLines; i++)
		{
			tht = i*dtht + tht0;
			x = int(ctX + j * (maxval/nRGrids) * scaleX * cos(tht));
			y = int(ctY - j * (maxval/nRGrids) * scaleY * sin(tht));
			MoveToEx(hDC, x, y, NULL);
			tht += dtht;
			x = int(ctX + j * (maxval/nRGrids) * scaleX * cos(tht));
			y = int(ctY - j * (maxval/nRGrids) * scaleY * sin(tht));
			LineTo(hDC, x, y);
		}
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	CRadarPlotImpl::DrawRadarFrame(HDC hDC, RECT plotRect, double maxval, double edge)
{
	int ctX, ctY;
	ctX = (plotRect.left + plotRect.right)/2;
	ctY = (plotRect.bottom + plotRect.top)/2;
	
	double scaleX, scaleY;
	scaleX = (plotRect.right - plotRect.left)/(maxval+edge)/2.0;
	scaleY = (plotRect.bottom - plotRect.top)/(maxval+edge)/2.0;
	
	int nRadarLines = (int)GetAbsData(0).size();
	
	HPEN hPen, hOldPen;
	hPen = CreatePen(m_nRadarFrameStyle, m_nRadarFrameWidth, m_crRadarFrameColor);
	hOldPen = (HPEN)SelectObject(hDC, hPen);
	
	double tht0 = GetStartAngle();
	
	int i;
	
	int x, y;
	double tht, dtht = 2.0*myPi/nRadarLines;
	//
	for(i=0; i<nRadarLines; i++)
	{
		tht = i*dtht + tht0;
		x = int(ctX + maxval * scaleX * cos(tht));
		y = int(ctY - maxval * scaleY * sin(tht));
		MoveToEx(hDC, x, y, NULL);
		tht += dtht;
		x = int(ctX + maxval * scaleX * cos(tht));
		y = int(ctY - maxval * scaleY * sin(tht));
		LineTo(hDC, x, y);
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	CRadarPlotImpl::DrawRadarTitle(HDC hDC, RECT plotRect, double maxval, double edge)
{
	int nRadarLines = (int)GetAbsData(0).size(); 
	if(nRadarLines != (int)m_vstrTitles.size())return;

	for(int i = 0; i < nRadarLines; i++)
	{
		DrawSingleRadarTitle(hDC, plotRect, maxval, edge, i);
	}
}

void	CRadarPlotImpl::DrawSingleRadarTitle(HDC hDC, RECT plotRect, double maxval, double edge, int nIndex)
{
	int nRadarLines = (int)GetAbsData(0).size(); 
	if(nRadarLines != (int)m_vstrTitles.size())return;

	if(nIndex < 0 || nIndex >= (int)m_vstrTitles.size())return;
	
	double scaleX, scaleY;
	//scaleX = (plotRect.right - plotRect.left)/(maxval+edge)/2.0;
	//scaleY = (plotRect.bottom - plotRect.top)/(maxval+edge)/2.0;
	scaleX = maxval/(maxval + edge);
	scaleY = maxval/(maxval + edge);

			
	double tht0 = GetStartAngle();
	double tht, dtht = 2.0*myPi/nRadarLines;
	tht = nIndex*dtht + tht0;
	
	LOGFONT logFont;
	logFont = m_lfTitleFont;
	logFont.lfHeight = 24;
	RECT titleRect = GetTextRectAroundEllipse(hDC, plotRect, &plotRect, tht, scaleX, scaleY, m_vstrTitles[nIndex], logFont, 8);

	HFONT hFont, hOldFont;
	hFont = CreateFontIndirect(&logFont);
	hOldFont = (HFONT)SelectObject(hDC, hFont);
	DrawText(hDC, m_vstrTitles[nIndex].c_str(), (int)m_vstrTitles[nIndex].length(), &titleRect, DT_LEFT);
	SelectObject(hDC, hOldFont);
	DeleteObject(hFont);
}
