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

// Contour.cpp: implementation of the CContour class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "stdlib.h"
#include "math.h"
#include "Contour.h"
#include "../../Accessary/Numerical/Powell/Powell.h"
#include "../../Basic/PlotUtility.h"

using namespace NsCChart;
namespace NsCChart{
extern double TestFunction(double x, double y);
}
CContour *CContour::m_pThis = 0;

void CContour::f4min(int *n, double x[2], double *y)
{
	*y = m_pThis->m_pFieldFcn(x[0],x[1]);
}

void CContour::f4max(int *n, double x[2], double *y)
{
	*y = -m_pThis->m_pFieldFcn(x[0],x[1]);
}

/*
void CContour::f4min(double x[2], double &y)
{
	y = pThis->m_pFieldFcn(x[0],x[1]);
}

void CContour::f4max(double x[2], double &y)
{
	y = -pThis->m_pFieldFcn(x[0],x[1]);
}
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContour::CContour()
{
	//
	m_pThis = this;
	//
	m_iColFir = m_iRowFir = 32;
	m_iColSec = m_iRowSec = 256;
	m_dDx = m_dDy = 0;

	m_pLimits[0] = m_pLimits[2] = -5.;
	m_pLimits[1] = m_pLimits[3] = 5.;
	m_ppFnData = NULL;

	// temporary stuff
	m_pFieldFcn = TestFunction;
	m_vPlanes.resize(20);
	for (UINT i = 0; i < m_vPlanes.size(); i++)
	{
		m_vPlanes[i] = (i - m_vPlanes.size() / 2.0) * 0.1;
	}

	m_nContourLines = 30;
}

CContour::~CContour()
{
	CleanMemory();
}

void CContour::InitMemory()
{
	CleanMemory();

	m_ppFnData = new CFnStr * [m_iColSec + 1];
	for (int i = 0; i < m_iColSec + 1; i++)
	{
		m_ppFnData[i] = NULL;
	}
}

void CContour::CleanMemory()
{
	if (m_ppFnData)
	{
		int i;
		for (i = 0; i < m_iColSec + 1; i++)
		{
			if (m_ppFnData[i])
			{
				delete[] (m_ppFnData[i]);
				m_ppFnData[i] = NULL;
			}
		}
		delete[] m_ppFnData;
		m_ppFnData = NULL;
	}
}

void CContour::Generate()
{
	int i, j;
	int x3, x4, y3, y4, x, y, oldx3, xlow;
	const int cols = m_iColSec + 1;
	const int rows = m_iRowSec + 1;
	double xoff, yoff;

	// Initialize memroy if needed
	InitMemory();

	m_dDx = (m_pLimits[1] - m_pLimits[0]) / (double) (m_iColSec);
	xoff = m_pLimits[0];
	m_dDy = (m_pLimits[3] - m_pLimits[2]) / (double) (m_iRowSec);
	yoff = m_pLimits[2];

	xlow = 0;
	oldx3 = 0;
	x3 = (cols - 1) / m_iRowFir;
	x4 = (2 * (cols - 1)) / m_iRowFir;
	for (x = oldx3; x <= x4; x++)
	{
		/* allocate new columns needed
		*/
		if (x >= cols)
			break;
		if (m_ppFnData[x] == NULL)
			m_ppFnData[x] = new CFnStr[rows];

		for (y = 0; y < rows; y++)
			FnctData(x, y)->m_sTopLen = -1;
	}

	y4 = 0;
	for (j = 0; j < m_iColFir; j++)
	{
		y3 = y4;
		y4 = ((j + 1) * (rows - 1)) / m_iColFir;
		Cntr1(oldx3, x3, y3, y4);
	}

	for (i = 1; i < m_iRowFir; i++)
	{
		y4 = 0;
		for (j = 0; j < m_iColFir; j++)
		{
			y3 = y4;
			y4 = ((j + 1) * (rows - 1)) / m_iColFir;
			Cntr1(x3, x4, y3, y4);
		}

		y4 = 0;
		for (j = 0; j < m_iColFir; j++)
		{
			y3 = y4;
			y4 = ((j + 1) * (rows - 1)) / m_iColFir;
			Pass2(oldx3, x3, y3, y4);
		}

		if (i < (m_iRowFir - 1))
		{
			/* re-use columns no longer needed */
			oldx3 = x3;
			x3 = x4;
			x4 = ((i + 2) * (cols - 1)) / m_iRowFir;
			for (x = x3 + 1; x <= x4; x++)
			{
				if (xlow < oldx3)
				{
					if (m_ppFnData[x])
						delete[] m_ppFnData[x];
					m_ppFnData[x] = m_ppFnData[xlow];
					m_ppFnData[xlow++] = NULL;
				}
				else if (m_ppFnData[x] == NULL)
					m_ppFnData[x] = new CFnStr[rows];

				for (y = 0; y < rows; y++)
					FnctData(x, y)->m_sTopLen = -1;
			}
		}
	}

	y4 = 0;
	for (j = 0; j < m_iColFir; j++)
	{
		y3 = y4;
		y4 = ((j + 1) * (rows - 1)) / m_iColFir;
		Pass2(x3, x4, y3, y4);
	}
}

void CContour::Cntr1(int x1, int x2, int y1, int y2)
{
	double f11, f12, f21, f22, f33;
	int x3, y3, i, j;

	if ((x1 == x2) || (y1 == y2))	/* if not a real cell, punt */
		return;
	f11 = Field(x1, y1);
	f12 = Field(x1, y2);
	f21 = Field(x2, y1);
	f22 = Field(x2, y2);
	if ((x2 > x1 + 1) || (y2 > y1 + 1))
	{
		/* is cell divisible? */
		x3 = (x1 + x2) / 2;
		y3 = (y1 + y2) / 2;
		f33 = Field(x3, y3);
		i = j = 0;
		if (f33 < f11)
			i++;
		else if (f33 > f11)
			j++;
		if (f33 < f12)
			i++;
		else if (f33 > f12)
			j++;
		if (f33 < f21)
			i++;
		else if (f33 > f21)
			j++;
		if (f33 < f22)
			i++;
		else if (f33 > f22)
			j++;
		if ((i > 2) || (j > 2)) /* should we divide cell? */
		{
			/* subdivide cell */
			Cntr1(x1, x3, y1, y3);
			Cntr1(x3, x2, y1, y3);
			Cntr1(x1, x3, y3, y2);
			Cntr1(x3, x2, y3, y2);
			return;
		}
	}
	/* install cell in array */
	FnctData(x1, y2)->m_sBotLen = FnctData(x1, y1)->m_sTopLen = x2 - x1;
	FnctData(x2, y1)->m_sLeftLen = FnctData(x1, y1)->m_sRightLen = y2 - y1;
}

void CContour::Pass2(int x1, int x2, int y1, int y2)
{
	int left, right, top, bot, old, iNew, i, j, x3, y3;
	double yy0, yy1, xx0, xx1, xx3, yy3;
	double v, f11, f12, f21, f22, f33, fold, fnew, f;
	double xoff = m_pLimits[0];
	double yoff = m_pLimits[2];

	if ((x1 == x2) || (y1 == y2))	/* if not a real cell, punt */
		return;
	f11 = FnctData(x1, y1)->m_dFnVal;
	f12 = FnctData(x1, y2)->m_dFnVal;
	f21 = FnctData(x2, y1)->m_dFnVal;
	f22 = FnctData(x2, y2)->m_dFnVal;
	if ((x2 > x1 + 1) || (y2 > y1 + 1)) /* is cell divisible? */
	{
		x3 = (x1 + x2) / 2;
		y3 = (y1 + y2) / 2;
		f33 = FnctData(x3, y3)->m_dFnVal;
		i = j = 0;
		if (f33 < f11)
			i++;
		else if (f33 > f11)
			j++;
		if (f33 < f12)
			i++;
		else if (f33 > f12)
			j++;
		if (f33 < f21)
			i++;
		else if (f33 > f21)
			j++;
		if (f33 < f22)
			i++;
		else if (f33 > f22)
			j++;
		if ((i > 2) || (j > 2)) /* should we divide cell? */
		{
			/* subdivide cell */
			Pass2(x1, x3, y1, y3);
			Pass2(x3, x2, y1, y3);
			Pass2(x1, x3, y3, y2);
			Pass2(x3, x2, y3, y2);
			return;
		}
	}

	for (i = 0; i < (int) m_vPlanes.size(); i++)
	{
		v = m_vPlanes[i];
		j = 0;
		if (f21 > v)
			j++;
		if (f11 > v)
			j |= 2;
		if (f22 > v)
			j |= 4;
		if (f12 > v)
			j |= 010;
		if ((f11 > v) ^ (f12 > v))
		{
			if ((FnctData(x1, y1)->m_sLeftLen != 0) &&
				(FnctData(x1, y1)->m_sLeftLen < FnctData(x1, y1)->m_sRightLen))
			{
				old = y1;
				fold = f11;
				while (1)
				{
					iNew = old + FnctData(x1, old)->m_sLeftLen;
					fnew = FnctData(x1, iNew)->m_dFnVal;
					if ((fnew > v) ^ (fold > v))
						break;
					old = iNew;
					fold = fnew;
				}
				yy0 = ((old - y1) + (iNew - old) * (v - fold) / (fnew - fold)) /
					(y2 - y1);
			}
			else
				yy0 = (v - f11) / (f12 - f11);

			left = (int) (y1 + (y2 - y1) * yy0 + 0.5);
		}
		if ((f21 > v) ^ (f22 > v))
		{
			if ((FnctData(x2, y1)->m_sRightLen != 0) &&
				(FnctData(x2, y1)->m_sRightLen < FnctData(x2, y1)->m_sLeftLen))
			{
				old = y1;
				fold = f21;
				while (1)
				{
					iNew = old + FnctData(x2, old)->m_sRightLen;
					fnew = FnctData(x2, iNew)->m_dFnVal;
					if ((fnew > v) ^ (fold > v))
						break;
					old = iNew;
					fold = fnew;
				}
				yy1 = ((old - y1) + (iNew - old) * (v - fold) / (fnew - fold)) /
					(y2 - y1);
			}
			else
				yy1 = (v - f21) / (f22 - f21);

			right = (int) (y1 + (y2 - y1) * yy1 + 0.5);
		}
		if ((f21 > v) ^ (f11 > v))
		{
			if ((FnctData(x1, y1)->m_sBotLen != 0) &&
				(FnctData(x1, y1)->m_sBotLen < FnctData(x1, y1)->m_sTopLen))
			{
				old = x1;
				fold = f11;
				while (1)
				{
					iNew = old + FnctData(old, y1)->m_sBotLen;
					fnew = FnctData(iNew, y1)->m_dFnVal;
					if ((fnew > v) ^ (fold > v))
						break;
					old = iNew;
					fold = fnew;
				}
				xx0 = ((old - x1) + (iNew - old) * (v - fold) / (fnew - fold)) /
					(x2 - x1);
			}
			else
				xx0 = (v - f11) / (f21 - f11);

			bot = (int) (x1 + (x2 - x1) * xx0 + 0.5);
		}
		if ((f22 > v) ^ (f12 > v))
		{
			if ((FnctData(x1, y2)->m_sTopLen != 0) &&
				(FnctData(x1, y2)->m_sTopLen < FnctData(x1, y2)->m_sBotLen))
			{
				old = x1;
				fold = f12;
				while (1)
				{
					iNew = old + FnctData(old, y2)->m_sTopLen;
					fnew = FnctData(iNew, y2)->m_dFnVal;
					if ((fnew > v) ^ (fold > v))
						break;
					old = iNew;
					fold = fnew;
				}
				xx1 = ((old - x1) + (iNew - old) * (v - fold) / (fnew - fold)) /
					(x2 - x1);
			}
			else
				xx1 = (v - f12) / (f22 - f12);

			top = (int) (x1 + (x2 - x1) * xx1 + 0.5);
		}

		switch (j)
		{
		case 7:
		case 010:
			ExportLine(i, x1, left, top, y2);
			break;
		case 5:
		case 012:
			ExportLine(i, bot, y1, top, y2);
			break;
		case 2:
		case 015:
			ExportLine(i, x1, left, bot, y1);
			break;
		case 4:
		case 013:
			ExportLine(i, top, y2, x2, right);
			break;
		case 3:
		case 014:
			ExportLine(i, x1, left, x2, right);
			break;
		case 1:
		case 016:
			ExportLine(i, bot, y1, x2, right);
			break;
		case 0:
		case 017:
			break;
		case 6:
		case 011:
			yy3 = (xx0 * (yy1 - yy0) + yy0) /
				(1.0 - (xx1 - xx0) * (yy1 - yy0));
			xx3 = yy3 * (xx1 - xx0) + xx0;
			xx3 = x1 + xx3 * (x2 - x1);
			yy3 = y1 + yy3 * (y2 - y1);
			xx3 = xoff + xx3 * m_dDx;
			yy3 = yoff + yy3 * m_dDy;
			f = (*m_pFieldFcn) (xx3, yy3);
			if (f == v)
			{
				ExportLine(i, bot, y1, top, y2);
				ExportLine(i, x1, left, x2, right);
			}
			else if (((f > v) && (f22 > v)) || ((f < v) && (f22 < v)))
			{
				ExportLine(i, x1, left, top, y2);
				ExportLine(i, bot, y1, x2, right);
			}
			else
			{
				ExportLine(i, x1, left, bot, y1);
				ExportLine(i, top, y2, x2, right);
			}
		}
	}
}

double CContour::Field(int x, int y)	 /* evaluate funct if we must,	*/
{
	double x1, y1;

	if (FnctData(x, y)->m_sTopLen != -1)  /* is it already in the array */
		return(FnctData(x, y)->m_dFnVal);

	/* not in the array, create new array element */
	x1 = m_pLimits[0] + m_dDx * x;
	y1 = m_pLimits[2] + m_dDy * y;
	FnctData(x, y)->m_sTopLen = 0;
	FnctData(x, y)->m_sBotLen = 0;
	FnctData(x, y)->m_sRightLen = 0;
	FnctData(x, y)->m_sLeftLen = 0;
	return (FnctData(x, y)->m_dFnVal = (*m_pFieldFcn) (x1, y1));
}

void CContour::SetPlanes(const std::vector<double>& vPlanes)
{
	// cleaning memory
	CleanMemory();

	m_vPlanes.clear();
	m_vPlanes = vPlanes;

	m_nContourLines = (int)vPlanes.size();
}

void CContour::SetPlanes(int nPlanes)
{
	SetPlanes(nPlanes, false);
}

void CContour::SetPlanes(int nPlanes, bool bExponential)
{
	//ASSERT(nPlanes > 2);
	if(nPlanes<=0)
	{
		CleanMemory();
		m_vPlanes.clear();
		m_nContourLines = 0;
		return;
	}

	//
	double mn, mx;
	int iflag;
	double x[2],xlb[2],xub[2];
	xlb[0] = m_pLimits[0];
	xlb[1] = m_pLimits[2];
	xub[0] = m_pLimits[1];
	xub[1] = m_pLimits[3];
	BoundedPowell(f4min,2,x,xlb,xub,1.0,1e-6,10000,iflag);
	mn = m_pFieldFcn(x[0],x[1]);
	BoundedPowell(f4max,2,x,xlb,xub,1.0,1e-6,10000,iflag);
	mx = m_pFieldFcn(x[0],x[1]);
	//

	double min, max, delta;

	std::vector<double> vPlanes;
	vPlanes.resize(nPlanes);
	if (!bExponential || mn <= 0.0)
	{
		min = mn + (mx - mn) / (16.0 * nPlanes);
		max = mx - (mx - mn) / (16.0 * nPlanes);

		double inRange[2]={min, max};
		double outRange[2];
		int nTicks, nMinor;
		GetOptimalRange(inRange, outRange, &nTicks, &nMinor, false, true, true, false, false, false);
		max = outRange[1];
		min = outRange[0];
	}
	else
	{
		min = mn * pow(mx / mn, 1.0 / (16.0 * nPlanes));
		max = mx / pow(mx / mn, 1.0 / (16.0 * nPlanes));
	}

/*	if (!bExponential || mn <= 0.0)
	{
		delta = (max - min) / (nPlanes - 1.0);
	}
	else
	{
		delta = pow(max / min, 1.0 / (nPlanes - 1.0));
	}
	for (int i = 0; i < nPlanes; i++)
	{
		if (!bExponential || mn <= 0.0)
		{
			vPlanes[i] = max - i * delta;
		}
		else
		{
			vPlanes[i] = max / pow(delta, i);
		}
	}*/
	if (!bExponential || mn <= 0.0)
	{
		delta = (max - min) / nPlanes;
	}
	else
	{
		delta = pow(max / min, 1.0 / nPlanes);
	}
	for (int i = 0; i < nPlanes; i++)
	{
		if (!bExponential || mn <= 0.0)
		{
			vPlanes[i] = max - (i+0.5) * delta;
		}
		else
		{
			vPlanes[i] = max / pow(delta, i+0.5);
		}
	}

	SetPlanes(vPlanes);
}

void CContour::SetFieldFcn(double (*_pFieldFcn) (double, double))
{
	m_pFieldFcn = _pFieldFcn;
}

double		(*CContour::GetFieldFcn( ))( double, double )
{
	return m_pFieldFcn;
}

void CContour::SetFirstGrid(int iCol, int iRow)
{
	m_iColFir = max(iCol, 2);
	m_iRowFir = max(iRow, 2);
}

void CContour::SetSecondaryGrid(int iCol, int iRow)
{
	// cleaning work matrices if allocated
	CleanMemory();

	m_iColSec = max(iCol, 2);
	m_iRowSec = max(iRow, 2);
}

void CContour::SetLimits(double pLimits[])
{
	//ASSERT(pLimits[0] < pLimits[1]);
	//ASSERT(pLimits[2] < pLimits[3]);
	memcpy(m_pLimits, pLimits, 4*sizeof(double));
}

void CContour::GetLimits(double pLimits[])
{
	memcpy(pLimits, m_pLimits, 4*sizeof(double));
}
