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

#include "PolarPlotImpl.h"

#include "../../Basic/MyConst.h"
#include "../../Basic/MyMemDC.h"

using namespace NsCChart;

CPolarPlotImpl::CPolarPlotImpl()
{
	m_pPhiAxis = new CAxisImpl(this);
	m_pRAxis = new CAxisImpl(this);

	m_pRAxis->SetIndependent(true);
	m_pPhiAxis->SetIndependent(true);

	m_pPhiAxis->GetPlotIndep()->SetXRange(0, 2.0*myPi);
	m_pPhiAxis->GetPlotIndep()->SetXTickCount(7);
	m_pPhiAxis->GetPlotIndep()->SetXMinorTickCount(5);
	m_pPhiAxis->SetTitle(_TEXT("Theta"));

	//AddAxis(kLocationBottom)->SetVisible(false);
	//AddAxis(kLocationLeft)->SetVisible(false);

	SetCircleMode(true);
	SetClipEllipse(true);
}

CPolarPlotImpl::~CPolarPlotImpl()
{
	delete m_pPhiAxis;
	m_pPhiAxis = 0;
	delete m_pRAxis;
	m_pRAxis = 0;
}

void	CPolarPlotImpl::GetPlotRange( double *xRange, double *yRange )
{
	CXYPlotImpl::GetPlotRange(xRange, yRange);
	
	double a, b;
	a = max(fabs(xRange[0]), fabs(xRange[1]));
	b = max(fabs(yRange[0]), fabs(yRange[1]));

	SetXRange(-a, a);
	SetYRange(-b, b);
	GetRAxis()->GetPlotIndep()->SetOptimalXRange(0, max(a, b));

	xRange[0] = -a;
	xRange[1] = a;
	yRange[0] = -b;
	yRange[1] = b;

	//memcpy(m_pfXPlotRange, xRange, 2*sizeof(double));
	//memcpy(m_pfYPlotRange, yRange, 2*sizeof(double));
	SetXRange(xRange);
	SetYRange(yRange);
	//SetXRange(xRange[0], xRange[1]);
	//SetYRange(yRange[0], yRange[1]);
	//memcpy(m_pfLastXPlotRange, xRange, 2*sizeof(double));
	//memcpy(m_pfLastYPlotRange, yRange, 2*sizeof(double));
	SetLastPlotRange(xRange, yRange);
}

double	CPolarPlotImpl::GetPolarXRange()
{
	double range[2];
	GetXRange(range);
	return max(fabs(range[0]), fabs(range[1]));
}

double	CPolarPlotImpl::GetPolarYRange()
{
	double range[2];
	GetYRange(range);
	return max(fabs(range[0]), fabs(range[1]));
}

void	CPolarPlotImpl::SetPolarXRange(double rng)
{
	if(rng<=0.0)return;
	SetXRange(-rng, rng);
}

void	CPolarPlotImpl::SetPolarYRange(double rng)
{
	if(rng<=0.0)return;
	SetYRange(-rng, rng);
}

void	CPolarPlotImpl::SetPolarRange(double xrng, double yrng)
{
	if(xrng<=0.0 || yrng<=0.0)return;
	SetXRange(-xrng, xrng);
	SetYRange(-yrng, yrng);

	m_bNewDataComming = false;
}

int		CPolarPlotImpl::AddPolarCurve( double *pRadius, double *pTheta, int nLen )
{
	return AddPolarCurve(0.0, 0.0, pRadius, pTheta, nLen);
}

int		CPolarPlotImpl::AddPolarCurve(double x0, double y0, double *pRadius, double *pTheta, int nLen)
{
	if(!pTheta || !pRadius || nLen<=0)return -1;
	double *pX, *pY;
	pX = new double[nLen];
	pY = new double[nLen];
	for(int i=0; i<nLen; i++)
	{
		pX[i] = x0 + pRadius[i] * cos(pTheta[i]);
		pY[i] = y0 + pRadius[i] * sin(pTheta[i]);
	}
	int dataID = AddCurve(pX, pY, nLen);
	SetPlotType(dataID, kXYPlotBezier);
	SetMarkerFrequency(dataID, 0);

	delete []pY;
	delete []pX;
	
	return dataID;
}

int		CPolarPlotImpl::AddCircle(double x0, double y0, double radius, int segs)
{
	if(segs<=0)return -1;

	double *pRad, *pTht;
	pRad = new double[segs+1];
	pTht = new double[segs+1];
	for(int i=0; i<segs; i++)
	{
		pRad[i] = radius;
		pTht[i] = i*2.0*myPi/segs;
	}
	pRad[segs] = pRad[0];
	pTht[segs] = pTht[0];
	int dataID = AddPolarCurve(x0, y0, pRad, pTht, segs);

	delete []pTht;
	delete []pRad;

	return dataID;
}

void	CPolarPlotImpl::FinalDraw(HDC hDC, RECT destRect)
{
	if(IsEmpty())
	{
		SetLastClientRect(destRect);
		//DrawBkgnd(hDC);
		DrawDefScreen(hDC, destRect);
		return;
	}
	
	int nOldMapmode = SetMapMode(hDC, MM_TEXT);

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

	CXYPlotImpl::FinalDraw(hdc, destRect);

	if(m_bNothingElse)
	{
		SetMapMode(hdc, nOldMapmode);
		return;
	}

	RECT plotRect = GetLastPlotRect();//GetPlotRect(hDC, destRect);
//	double xRange[2], yRange[2];
//	GetPlotRange(xRange, yRange);
	DrawPhiAxis(hdc, plotRect);	
	DrawPolarGrids(hdc, plotRect);

	SetMapMode(hDC, nOldMapmode);
}

void	CPolarPlotImpl::DrawPhiAxis( HDC hDC, RECT plotRect )
{
	RECT ellipseRect = plotRect;//GetEllipseRect(plotRect);

	int i, num;

	CAxisImpl *axis;
	axis = GetPhiAxis();
	if(!axis)return;

	HPEN hPen, hOldPen;
	hPen = CreatePen(axis->GetLineStyle(), axis->GetLineSize(), axis->GetAxisColor());
	hOldPen = (HPEN)SelectObject(hDC, hPen);

	HBRUSH hBrush,hOldBrush;
	hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
	//Axis line
	Ellipse(hDC, ellipseRect.left, ellipseRect.top, ellipseRect.right, ellipseRect.bottom);
	SelectObject(hDC, hOldBrush);
	DeleteObject(hBrush);

	int w, h, ctx, cty;
	w = ellipseRect.right - ellipseRect.left;
	h = ellipseRect.bottom - ellipseRect.top;
	ctx = (ellipseRect.left + ellipseRect.right)/2;
	cty = (ellipseRect.bottom + ellipseRect.top)/2;
	//Major tick
	if(axis->IsShowMajorTicks())
	{
		num = axis->GetPlotIndep()->GetXTickCount();
		if(num>=1)
		{
			double a, b;
			a = w/2.0;
			b = h/2.0;
			double dtht,tht;
			dtht = 2.0 * myPi / (num+1);

			int tickL = axis->GetTickLength();
			
			int x1, y1, x2, y2;
			for(i=0; i<=num; i++)
			{
				tht = i*dtht;
				x1 = int(ctx + a * cos(tht));
				y1 = int(cty + b * sin(tht));
				x2 = int(ctx + (a-tickL)*cos(tht));
				y2 = int(cty + (b-tickL*b/(double)a)*sin(tht));
				MoveToEx(hDC, x1, y1, NULL);
				LineTo(hDC, x2, y2);
			}
		}
	}
	//Minor tick
	if(axis->IsShowMinorTicks())
	{
		num = axis->GetPlotIndep()->GetXTickCount();
		if(num>=1)
		{
			int num1, j;
			num1 = axis->GetPlotIndep()->GetXMinorTickCount();
			if(num1>=1)
			{
				double a, b;
				a = w/2.0;
				b = h/2.0;
				double dtht,tht;
				dtht = 2.0 * myPi / (num+1);

				double dtht1;
				dtht1 = dtht/num1;
				
				int tickL = axis->GetMinorTickLength();
				
				int x1, y1, x2, y2;
				for(i=0; i<=num; i++)
				{
					for(j=1; j<num1; j++)
					{
						tht = i*dtht + j*dtht1;
						x1 = int(ctx + a * cos(tht));
						y1 = int(cty + b * sin(tht));
						x2 = int(ctx + (a-tickL)*cos(tht));
						y2 = int(cty + (b-tickL*b/(double)a)*sin(tht));
						MoveToEx(hDC, x1, y1, NULL);
						LineTo(hDC, x2, y2);
					}
				}
			}
			
		}
	}
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}

void	CPolarPlotImpl::DrawPolarGrids( HDC hDC, RECT plotRect )
{
	RECT ellipseRect = plotRect; //GetEllipseRect(plotRect);

	HPEN hPen, hOldPen;
	hPen = CreatePen(GetMajorGridLineStyle(), GetMajorGridLineSize(), GetMajorGridColor());
	hOldPen = (HPEN)SelectObject(hDC, hPen);

	int w, h, ctx, cty;
	w = ellipseRect.right - ellipseRect.left;
	h = ellipseRect.bottom - ellipseRect.top;
	ctx = (ellipseRect.left + ellipseRect.right)/2;
	cty = (ellipseRect.bottom + ellipseRect.top)/2;

	int i, num;
	CAxisImpl *axis;

	if(m_bUseMajorPGrids)
	{
		axis = GetPhiAxis();
		num = axis->GetPlotIndep()->GetXTickCount();
		
		if(num>=1)
		{
			double a, b;
			a = w/2.0;
			b = h/2.0;
			double dtht, tht;
			dtht = 2.0 * myPi / (num+1);
			
			int x, y;
			for(i=0; i<=num; i++)
			{
				tht = i*dtht;
				x = int(ctx + a * cos(tht));
				y = int(cty + b * sin(tht));
				MoveToEx(hDC, ctx, cty, NULL);
				LineTo(hDC, x, y);
			}
		}
	}
	if(m_bUseMajorRGrids)
	{
		axis = GetRAxis();
		num = axis->GetPlotIndep()->GetXTickCount();
		
		if(num>=1)
		{
			HBRUSH hBrush, hOldBrush;
			
			hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
			hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
			
			double dx, dy;
			
			dx = w / 2.0 / (num+1);
			dy = h / 2.0 / (num+1);
			for( i=1; i<=num; i++)
			{
				Ellipse(hDC, int(ctx - i*dx), int(cty - i*dy), int(ctx + i*dx), int(cty + i*dy));
			}
			
			SelectObject(hDC, hOldBrush);
			DeleteObject(hBrush);
		}
	}

	DeleteObject(hPen);
	hPen = CreatePen(GetMinorGridLineStyle(), GetMinorGridLineSize(), GetMinorGridColor());
	SelectObject(hDC, hPen);
	if(m_bUseMinorPGrids)
	{
		axis = GetPhiAxis();
		num = axis->GetPlotIndep()->GetXTickCount();

		int num1, j;
		
		if(num>=1)
		{
			double a, b;
			a = w/2.0;
			b = h/2.0;
			double dtht, tht;
			dtht = 2.0 * myPi / (num+1);

			num1 = axis->GetPlotIndep()->GetXMinorTickCount();
			if(num1>=1)
			{
				double dtht1;
				dtht1 = dtht/num1;
				
				int x, y;
				for(i=0; i<=num; i++)
				{
					for(j=1; j<num1; j++)
					{
						tht = i*dtht + j*dtht1;
						x = int(ctx + a * cos(tht));
						y = int(cty + b * sin(tht));
						MoveToEx(hDC, ctx, cty, NULL);
						LineTo(hDC, x, y);
					}
				}
			}
		}
	}
	if(m_bUseMinorRGrids)
	{
		axis = GetRAxis();
		num = axis->GetPlotIndep()->GetXTickCount();
		
		int num1, j;

		if(num>=1)
		{
			num1 = axis->GetPlotIndep()->GetXMinorTickCount();
			if(num1>=1)
			{
				HBRUSH hBrush, hOldBrush;
				
				hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
				
				double dx, dy;
				
				dx = w / 2.0 / (num+1);
				dy = h / 2.0 / (num+1);
				
				double dx1, dy1;
				dx1 = dx / num1;
				dy1 = dy / num1;
				
				for( i=1; i<=num; i++)
				{
					for( j=1; j<num1; j++)
					{
						Ellipse(hDC, int(ctx - i*dx - j*dx1), int(cty - i*dy - j*dy1), int(ctx + i*dx + j*dx1), int(cty + i*dy + j*dy1));
					}
				}
				
				SelectObject(hDC, hOldBrush);
				DeleteObject(hBrush);
			}
		}
	}
	
	SelectObject(hDC, hOldPen);
	DeleteObject(hPen);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class CSmithChart

CSmithChart::CSmithChart()
{
	SetTitle(_TEXT("Smith Chart"));
	
	m_nCircleSegments = 360;
	m_nCenterPos = kSmithCenterRight;
}

CSmithChart::~CSmithChart()
{

}

void CSmithChart::InitSmith()
{
	
	SetBorderShow(false);

	SetUseMajorRGrids(false);
	SetUseMajorPGrids(false);

	CAxis *axis;
	SetPolarRange(1.0, 1.0);
	axis = AddAxis(kLocationBottom);
	SetXTickCount(GetRAxis()->GetPlotIndep()->GetXTickCount()*2);
	axis = AddAxis(kLocationLeft);
	SetYTickCount(GetRAxis()->GetPlotIndep()->GetXTickCount()*2);


	AddAxis(kLocationBottom)->SetVisible(false);
	AddAxis(kLocationLeft)->SetVisible(false);

	int nSegs = m_nCircleSegments;
	int nNum;
	double center, sign;

	switch(m_nCenterPos)
	{
	case kSmithCenterLeft:
	case kSmithCenterBottom:
		center = -1.0;
		sign = 1;
		break;
	case kSmithCenterRight:
	case kSmithCenterTop:
		center = 1.0;
		sign = -1;
		break;
	default:
		return;
	}

	SetAutoDataColorGeneral(false);
	int i, dataID;
	double radius;
	DeleteAllData();
	
	double pX[2], pY[2];
	pX[0] = -1.0; pX[1] = 1.0;
	pY[0] = 0.0; pY[1] = 0.0;
	
	if(m_nCenterPos == kSmithCenterLeft || m_nCenterPos == kSmithCenterRight)
	{
		nNum = 2 + 2*GetRAxis()->GetPlotIndep()->GetXTickCount();
		for(i=1; i<=nNum; i++)
		{
			radius = i*1.0/nNum;
			dataID = AddCircle(center + sign * radius, 0, radius, nSegs);
			
		}
		nNum = (GetPhiAxis()->GetPlotIndep()->GetXTickCount() + 1) * GetPhiAxis()->GetPlotIndep()->GetXMinorTickCount();
		for(i=1; i<=nNum; i++)
		{
			double tht;
			tht = i*2.0*myPi/nNum;
			radius = tan(tht/2.0);
			dataID = AddCircle(center, radius, radius, nSegs);
			dataID = AddCircle(center, -radius, radius, nSegs);
		}
		//Add Center line
		dataID = AddCurve(pX, pY, 2);
		dataID = AddCurve(pY, pX, 2);
		SetDataLineStyle(dataID, PS_DASH);
	}
	else if(m_nCenterPos == kSmithCenterBottom || m_nCenterPos == kSmithCenterTop)
	{
		nNum = 2 + 2*GetRAxis()->GetPlotIndep()->GetXTickCount();
		for(i=1; i<=nNum; i++)
		{
			radius = i*1.0/nNum;
			dataID = AddCircle(0, center + sign * radius, radius, nSegs);
		}
		nNum = (GetPhiAxis()->GetPlotIndep()->GetXTickCount() + 1) * GetPhiAxis()->GetPlotIndep()->GetXMinorTickCount();
		for(i=1; i<=nNum; i++)
		{
			double tht;
			tht = i*2.0*myPi/nNum;
			radius = tan(tht/2.0);
			dataID = AddCircle(radius, center, radius, nSegs);
			dataID = AddCircle(-radius, center, radius, nSegs);
		}
		//Add Center line
		dataID = AddCurve(pY, pX, 2);
		dataID = AddCurve(pX, pY, 2);
		SetDataLineStyle(dataID, PS_DASH);
	}
	
	
	for(i=0; i<GetPlotDataCount(); i++)
	{
		//SetReact(GetDataID(i), false);
		GetAbsPlotData(i)->SetReact(false);
	}

	m_bNewDataComming = false;
}