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

//#include "stdafx.h"

#pragma warning(disable:4786)

#include "ContourBasicPlot.h"
#include "ListContour.h"
#include <math.h>
#include <algorithm>

using namespace NsCChart;

vector<double> CContourBasicPlot::vX;
vector<double> CContourBasicPlot::vY;
vector<ContourPoint> CContourBasicPlot::m_vContourPoints;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CContourBasicPlot::CContourBasicPlot()
{
	m_crColor1 = RGB(0, 128, 255);
	m_crColor2 = RGB(255, 0, 0);

	m_pListContour = new CListContour;
}

CContourBasicPlot::~CContourBasicPlot()
{
	if(m_pListContour)delete m_pListContour;
}

void	CContourBasicPlot::SetFieldFcn(double (*_pFieldFcn)(double, double))
{
	m_pListContour->SetFieldFcn(_pFieldFcn);
	m_bContourChanged = true;
}

#ifndef _WIN64
double (_stdcall *__pFieldFcnStdCall) (double, double);
double	FieldFcnStdCall(double x, double y)
{
	return __pFieldFcnStdCall(x, y);
}

void	CContourBasicPlot::SetFieldFcn(double (_stdcall *_pFieldFcn) (double, double))
{
	__pFieldFcnStdCall = _pFieldFcn;
	m_pListContour->SetFieldFcn(&FieldFcnStdCall);
	m_bContourChanged = true;
}
#endif

double		(*CContourBasicPlot::GetFieldFcn( ))( double, double )
{
	return m_pListContour->GetFieldFcn();
}

void	CContourBasicPlot::SetContourRange(double pRange[4])
{
	m_pListContour->SetLimits(pRange);

	m_bContourChanged = true;
}

void	CContourBasicPlot::SetContourRange(double xRange[2], double yRange[2])
{
	double pLimits[4];
	memcpy(pLimits, xRange, 2*sizeof(double));
	memcpy(pLimits+2, yRange, 2*sizeof(double));
	m_pListContour->SetLimits(pLimits);

	m_bContourChanged = true;
}

void	CContourBasicPlot::SetContourRange(double xl, double xh, double yl, double yh)
{
	double pLimits[4] = {xl, xh, yl, yh};

	m_pListContour->SetLimits(pLimits);

	m_bContourChanged = true;
}

void	CContourBasicPlot::GetContourRange(double pRange[4])
{
	m_pListContour->GetLimits(pRange);
}

void	CContourBasicPlot::GetContourRange(double xRange[2], double yRange[2])
{
	double pLimits[4];
	m_pListContour->GetLimits(pLimits);
	memcpy(xRange, pLimits, 2*sizeof(double));
	memcpy(yRange, pLimits+2, 2*sizeof(double));
}

void	CContourBasicPlot::SetContourXRange(double low, double high)
{
	double pLimits[4];
	m_pListContour->GetLimits(pLimits);
	pLimits[0] = low;
	pLimits[1] = high;
	m_pListContour->SetLimits(pLimits);

	m_bContourChanged = true;
}

void	CContourBasicPlot::SetContourYRange(double low, double high)
{
	double pLimits[4];
	m_pListContour->GetLimits(pLimits);
	pLimits[2] = low;
	pLimits[3] = high;
	m_pListContour->SetLimits(pLimits);

	m_bContourChanged = true;
}

void	CContourBasicPlot::AddContourPoint(double x, double y, double h)
{
	ContourPoint cp={x, y, h};
	int i;
	for(i=0; i<(int)m_vContourPoints.size(); i++)
	{
		if(x==m_vContourPoints[i].c[0] && y==m_vContourPoints[i].c[1] && h==m_vContourPoints[i].c[2])
		{
			m_vContourPoints[i] = cp;
			return;
		}
	}
	m_vContourPoints.push_back(cp);

	bool bFndX, bFndY;
	bFndX = false;
	for(i=0; i<(int)vX.size(); i++)
	{
		if(x==vX[i])
		{
			bFndX = true;
			break;
		}
	}
	if(!bFndX)vX.push_back(x);
	
	bFndY = false;
	for(i=0; i<(int)vY.size(); i++)
	{
		if(y==vY[i])
		{
			bFndY = true;
			break;
		}
	}
	if(!bFndY)vY.push_back(y);

	std::sort(vX.begin(), vX.end());
	std::sort(vY.begin(), vY.end());
}

void	CContourBasicPlot::ClrContourPoints()
{
	m_vContourPoints.clear();
	vX.clear();
	vY.clear();
}

double	CContourBasicPlot::ContourHeight(double x, double y)
{
	if(m_vContourPoints.size()==0)return 0.0;
	if(m_vContourPoints.size()==1)return m_vContourPoints[0].c[2];

	int i;

	int xl, xu, yl, yu;

	//Get the rect region
	if(vX.size()==1)
	{
		xl = 0;
		xu = 0;
	}
	else if(x<vX[0])
	{
		xl=0;
		xu=1;
	}
	else if(x>=vX[vX.size()-1])
	{
		xu=(int)vX.size()-1;
		xl=(int)vX.size()-2;
	}
	else
	{
		for(i=0; i<(int)vX.size()-1; i++)
		{
			if(x>=vX[i] && x<vX[i+1])
			{
				xl = i;
				xu = i+1;
				break;
			}
		}
	}
	
	if(vY.size()==1)
	{
		yl = 0;
		yu = 0;
	}
	else if(y<vY[0])
	{
		yl=0;
		yu=1;
	}
	else if(y>=vY[vY.size()-1])
	{
		yu=(int)vY.size()-1;
		yl=(int)vY.size()-2;
	}
	else
	{
		for(i=0; i<(int)vY.size()-1; i++)
		{
			if(y>=vY[i] && y<vY[i+1])
			{
				yl = i;
				yu = i+1;
				break;
			}
		}
	}

	// Get the rect region value
	double	ll, lu, ul, uu;
	bool	bll=false, blu=false, bul=false, buu=false;

	for(i=0; i<(int)m_vContourPoints.size(); i++)
	{
		if(!bll && m_vContourPoints[i].c[0]==vX[xl] && m_vContourPoints[i].c[1]==vY[yl])
		{
			ll=m_vContourPoints[i].c[2];
			bll=true;
		}
		if(!blu && m_vContourPoints[i].c[0]==vX[xl] && m_vContourPoints[i].c[1]==vY[yu])
		{
			lu=m_vContourPoints[i].c[2];
			blu=true;
		}
		if(!bul && m_vContourPoints[i].c[0]==vX[xu] && m_vContourPoints[i].c[1]==vY[yl])
		{
			ul=m_vContourPoints[i].c[2];
			bul=true;
		}
		if(!buu && m_vContourPoints[i].c[0]==vX[xu] && m_vContourPoints[i].c[1]==vY[yu])
		{
			uu=m_vContourPoints[i].c[2];
			buu=true;
		}

		if(bll && blu && bul && buu)break;
	}

	// Get the height
	double hx1, hx2;

	if(yu==yl)
	{
		hx1 = ll;
		hx2 = ul;
	}
	else
	{
		hx1 = ll + (y-vY[yl])*(lu-ll)/(vY[yu]-vY[yl]);
		hx2 = ul + (y-vY[yl])*(uu-ul)/(vY[yu]-vY[yl]);
	}
	

	double h;
	if(xu==xl)h=hx1;
	else h=hx1+(x-vX[xl])*(hx2-hx1)/(vX[xu]-vX[xl]);
	
	return h;
}

void	CContourBasicPlot::GetPtsRng(int which, double &lower, double &upper)
{
	if(which<0 || which>=3)return;

	if(m_vContourPoints.size()==0)lower = upper = 0.0;
	else
	{
		int i;
		lower = m_vContourPoints[0].c[which];
		upper = m_vContourPoints[0].c[which];

		for(i=1; i<(int)m_vContourPoints.size(); i++)
		{
			if(m_vContourPoints[i].c[which]<lower)lower=m_vContourPoints[i].c[which];
			if(m_vContourPoints[i].c[which]>upper)upper=m_vContourPoints[i].c[which];
		}
	}
}