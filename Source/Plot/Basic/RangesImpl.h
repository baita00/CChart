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

#include "Ranges.h"

Declare_Namespace_CChart

template <class PlotImplT>
class	CRangesImpl : public CRanges<2>
{
public:
	CRangesImpl();
	virtual	~CRangesImpl();

protected:
	void				CalcEqualRange(double xRange[2], double yRange[2]);
	bool				NeedDataRange1D(int whichDim);

public:
	virtual	void		GetPlotRange( double *xRange, double *yRange );
	void				GetPlotRanges( );

	bool				IsRangeUpdateNeeded();

	void				ZoomRangeX( double fFact ){m_X1(ZoomRange, fFact);}
	void				ZoomRangeY( double fFact ){m_Y1(ZoomRange, fFact);}

public:
	double				GetMajorTickPosValue( int whichDim, int whichTick );//{return m_X1(GetMajorTickPosValue, whichTick);}
	bool				GetMinorTickPosValue( int whichDim, int whichMajor, double *fPos );//{return m_X2(GetMinorTickPosValue, whichMajor, fPos);}
};

template <class PlotImplT>
CRangesImpl<PlotImplT>::CRangesImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template <class PlotImplT>
CRangesImpl<PlotImplT>::~CRangesImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template <class PlotImplT>
void	CRangesImpl<PlotImplT>::CalcEqualRange(double xRange[2], double yRange[2])
{
	double range[2];
	range[0] = min( xRange[0], yRange[0] );
	range[1] = max( xRange[1], yRange[1] );

	int nTicks, nMinorTicks;
	double tickMin, tickMax;
	
	double optRange[2];
	// need to set the axis range and tickcount
	if(IsFloatXTicks() || IsFloatYTicks())
	{
		memcpy(optRange, range, 2*sizeof(double));
		nTicks = GetXTickCount();
		tickMin = GetXTickMin();
		tickMax = GetXTickMax();
		RegulateTicks(optRange[0], optRange[1], nTicks, nMinorTicks, tickMin, tickMax);
		SetXTickCount(nTicks);
		SetYTickCount(nTicks);
		SetXTickMin(tickMin); SetXTickMax(tickMax);
		SetYTickMin(tickMin); SetYTickMax(tickMax);
	}
	else
	{
		nTicks = GetXTickCount();
		nMinorTicks = GetXMinorTickCount();
		GetOptimalRange( range, optRange, &nTicks, &nMinorTicks, false, true, true, true, false, false );
		memcpy(range, optRange,2*sizeof(double));
		GetOptimalRange( range, optRange, &nTicks, &nMinorTicks, false, true, true, true, false, false );
		SetXTickCount(nTicks);
		SetXMinorTickCount(nMinorTicks);
		SetYTickCount(nTicks);
		SetYMinorTickCount(nMinorTicks);
	}
	
	//memcpy(m_pfXPlotRange, optRange, 2*sizeof(double));
	//memcpy(m_pfYPlotRange, optRange, 2*sizeof(double));

	SetXRange(optRange);
	SetYRange(optRange);
	
	xRange[0] = yRange[0] = optRange[0];
	xRange[1] = yRange[1] = optRange[1];
}

template <class PlotImplT>
void	CRangesImpl<PlotImplT>::GetPlotRange( double *xRange, double *yRange )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	bool bXStatic = IsStaticXRange();
	bool bYStatic = IsStaticYRange();
	if(bXStatic)
	{
		GetXRange(xRange);
	}
	if(bYStatic)
	{
		GetYRange(yRange);
	}

	if(bXStatic && bYStatic)goto myrngret;

	bool bRangeN;
	bRangeN = IsRangeUpdateNeeded();
	if(!bRangeN)
	{
		if(!bXStatic)GetXRange(xRange);
		if(!bYStatic)GetYRange(yRange);
		goto myrngret;
	}

/*	if(GetDataSetCount() <= 0)
	{
		SetXRange(0.0, 1.0);
		SetYRange(0.0, 1.0);
		memcpy(xRange, m_pfXRange, 2*sizeof(double));
		memcpy(yRange, m_pfYRange, 2*sizeof(double));
		return;
	}*/
	
	// new conditions
	//if(GetClsRange(0)->IsRangeChanged() && !IsXRangeSet() && IsXAutoRange())
	if(!bXStatic)
	{
		if(NeedDataRange1D(0) || pT->IsFirstDraw() || pT->IsNewDataComming())
		{
			GetXDataRange(xRange);
		}
		else
		{
			//memcpy(xRange, m_pfXPlotRange, 2*sizeof(double));
			GetXRange(xRange);
		}
	}
	
	//if(GetClsRange(1)->IsRangeChanged() && !IsYRangeSet() && IsYAutoRange())
	if(!bYStatic)
	{
		if(NeedDataRange1D(1) || pT->IsFirstDraw() || pT->IsNewDataComming())
		{
			GetYDataRange(yRange);
		}
		else
		{
			//memcpy(yRange, m_pfYPlotRange, 2*sizeof(double));
			GetYRange(yRange);
		}
	}
	
	//force
//	if(pT->IsNewDataComming())
//	{
//		GetXDataRange(xRange);
//		GetYDataRange(yRange);
//	}
	// Set equally range
	if( m_bEqualXYRange )
	{
		double lxr[2], lyr[2];
		GetLastPlotRange(lxr, lyr);
		if(bRangeN || memcmp(lxr, lyr, 2*sizeof(double))!=0)
		{
			CalcEqualRange(xRange, yRange);
		}
	}
	else
	{
		double xdr[2], ydr[2];
		GetDataRanges(xdr, ydr);
		//CalcRange(true, xRange);
		//CalcRange(false, yRange);
		if(!bXStatic)
		{
			if(NeedDataRange1D(0) || pT->IsFirstDraw() || pT->IsNewDataComming())
				GetClsRange(0)->CalcRange(xRange, xdr, pT->IsNewDataComming());
		}
		if(!bYStatic)
		{
			if(NeedDataRange1D(1) || pT->IsFirstDraw() || pT->IsNewDataComming())
				GetClsRange(1)->CalcRange(yRange, ydr, pT->IsNewDataComming());
		}
	}

myrngret:
	pT->SetNewDataComming(false);
	pT->SetFirstDraw(false);

	SetLastXPlotRange(GetXRange());
	SetLastYPlotRange(GetYRange());

}

template <class PlotImplT>
bool	CRangesImpl<PlotImplT>::NeedDataRange1D(int whichDim)
{
	switch(whichDim)
	{
	case 0:
		//return GetClsRange(0)->IsRangeChanged() && !IsXRangeSet() && IsXAutoRange();
		return !IsXRangeSet() && IsXAutoRange();
	case 1:
		//return GetClsRange(1)->IsRangeChanged() && !IsYRangeSet() && IsYAutoRange();
		return !IsYRangeSet() && IsYAutoRange();
	default:
		return false;
	}
}

template <class PlotImplT>
void	CRangesImpl<PlotImplT>::GetPlotRanges(  )
{
	double xRange[2], yRange[2];
	GetPlotRange(xRange, yRange);
}

template <class PlotImplT>
bool	CRangesImpl<PlotImplT>::IsRangeUpdateNeeded()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!IsXRangeSet() || !IsYRangeSet())return true;
	if(pT->IsFirstDraw())return true;
	if(pT->IsNewDataComming())return true;
	
	if(GetClsRange(0)->IsRangeChanged())return true;
	if(GetClsRange(1)->IsRangeChanged())return true;
	
	return false;
}

template <class PlotImplT>
double		CRangesImpl<PlotImplT>::GetMajorTickPosValue( int whichDim, int whichTick )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	bool bX =( (whichDim == 0)?true:false );

	double		range,pos,epsl=1.0e-12;

	double xr[2],yr[2];
	GetRange(xr, yr);
	
	int nTicks = (bX?GetXTickCount():GetYTickCount());
	
	// Assume user has checked the tick values first
	if(!(bX?pT->IsFloatXTicks():pT->IsFloatYTicks()))
	{
		range = (bX?xr[1]:yr[1]) - (bX?xr[0]:yr[0]);
	}
	else
	{
		range = (bX?GetXTickMax():GetYTickMax()) - (bX?GetXTickMin():GetYTickMin());
	}
	
	double startVal;
	if(!(bX?pT->IsFloatXTicks():pT->IsFloatYTicks()))
	{
		startVal = (bX?xr[0]:yr[0]);
	}
	else
	{
		startVal = (bX?GetXTickMin():GetYTickMin());
	}
	
	if( nTicks >= 0 )
	{
		// User has specified number of ticks
		pos = startVal + (double)whichTick * range/((double)nTicks+1.0);
		if(range!=0.0)
		{
			if(fabs(pos/range)<epsl)pos=0.0;
		}
	}
	else
	{
		// For now, just use four regions
		pos = startVal + range * (double)whichTick/((double)5.0);
		if(range!=0.0)
		{
			if(fabs(pos/range)<epsl)pos=0.0;
		}
	}
	
	return pos;
}

// Returns the distances along the axis in pointers, in DATA UNITS
// where the requested minortick mark lies
template <class PlotImplT>
bool		CRangesImpl<PlotImplT>::GetMinorTickPosValue( int whichDim, int whichMajor, double *fPos )
{	
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
	bool bX =( (whichDim == 0)?true:false );

	// Assume user has checked the tick values first
	if( (bX?GetXMinorTickCount():GetYMinorTickCount()) <=0 )
		return false;
	
	double pos1, pos2;
	pos1 = GetMajorTickPosValue(whichDim, whichMajor);
	pos2 = GetMajorTickPosValue(whichDim, whichMajor+1);
	
	int i;
	if(!(bX?pT->IsXLogarithm():pT->IsYLogarithm()))
	{
		for(i=0; i<=(bX?GetXMinorTickCount():GetYMinorTickCount()); i++)
		{
			fPos[i] = pos1 + i * (pos2 - pos1)/(bX?GetXMinorTickCount():GetYMinorTickCount());
		}
	}
	else
	{
		for(i=0; i<=(bX?GetXMinorTickCount():GetYMinorTickCount()); i++)
		{
			fPos[i] = log10( pow(10, pos1) + i * ( pow(10, pos2)-pow(10, pos1) )/(bX?GetXMinorTickCount():GetYMinorTickCount() ) );
		}
	}
	
	return true;
}

Declare_Namespace_End

