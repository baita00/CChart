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

#include "../../Basic/Ranges.h"

Declare_Namespace_CChart

template <class PlotImplT>
class	CRanges3DImpl : public CRanges<3>
{
public:
	CRanges3DImpl();
	virtual	~CRanges3DImpl();

public:
	virtual	void		GetPlotRange( double *xRange, double *yRange, double *zRange );

	bool				IsRangeUpdateNeeded();

	//virtual	void		GetDataRange1D( int whichDim, double *range ){}

public:
	double				GetMajorTickPosValue( int whichDim, int whichTick );//{return m_X1(GetMajorTickPosValue, whichTick);}
	bool				GetMinorTickPosValue( int whichDim, int whichMajor, double *fPos );//{return m_X2(GetMinorTickPosValue, whichMajor, fPos);}
};

template <class PlotImplT>
CRanges3DImpl<PlotImplT>::CRanges3DImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template <class PlotImplT>
CRanges3DImpl<PlotImplT>::~CRanges3DImpl()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
}

template <class PlotImplT>
void	CRanges3DImpl<PlotImplT>::GetPlotRange( double *xRange, double *yRange, double *zRange )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	
	// new conditions
	double *pRange;
	for(int i=0; i<3; i++)
	{
		if(i==0)pRange = xRange;
		else if(i==1)pRange = yRange;
		else pRange =zRange;
		if(GetClsRange(i)->IsRangeChanged() && !GetClsRange(i)->IsRangeSet() && GetClsRange(i)->IsAutoRange())
		{
			GetDataRange1D(i, pRange);
		}
	}

	double xdr[2], ydr[2], zdr[2];
	GetDataRange1D(0, xdr);
	GetDataRange1D(1, ydr);
	GetDataRange1D(2, zdr);
	//CalcRange(true, xRange);
	//CalcRange(false, yRange);
	GetClsRange(0)->CalcRange(xRange, xdr, true);
	GetClsRange(1)->CalcRange(yRange, ydr, true);
	GetClsRange(2)->CalcRange(zRange, zdr, true);
	

	//memcpy(m_pfLastXPlotRange, m_pfXPlotRange, 2*sizeof(double));
	//memcpy(m_pfLastYPlotRange, m_pfYPlotRange, 2*sizeof(double));
	SetLastXPlotRange(GetXRange());
	SetLastYPlotRange(GetYRange());
	GetClsRange(2)->SetLastPlotRange(GetClsRange(2)->GetRange());
}

template <class PlotImplT>
bool	CRanges3DImpl<PlotImplT>::IsRangeUpdateNeeded()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	if(!IsXRangeSet() || !IsYRangeSet() || !GetClsRange(2)->IsRangeSet())return true;
	if(pT->IsFirstDraw())return true;
	if(pT->IsNewDataComming())return true;
	
	if(GetClsRange(0)->IsRangeChanged())return true;
	if(GetClsRange(1)->IsRangeChanged())return true;
	if(GetClsRange(2)->IsRangeChanged())return true;
	
	return false;
}

template <class PlotImplT>
double		CRanges3DImpl<PlotImplT>::GetMajorTickPosValue( int whichDim, int whichTick )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	double		range,pos,epsl=1.0e-12;

	double val[2];
	GetClsRange(whichDim)->GetRange(val);
	
	int nTicks = (bX?GetXTickCount():GetYTickCount());
	
	// Assume user has checked the tick values first
	if(!GetClsRange(whichDim)->IsFloatTicks())
	{
		range = val[1] - val[0];
	}
	else
	{
		range = GetClsRange(whichDim)->GetTickMax() - GetClsRange(whichDim)->GetTickMin(); 
	}
	
	double startVal;
	if(!GetClsRange(whichDim)->IsFloatTicks())
	{
		startVal = val[0];
	}
	else
	{
		startVal = GetClsRange(whichDim)->GetTickMin();
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
bool		CRanges3DImpl<PlotImplT>::GetMinorTickPosValue( int whichDim, int whichMajor, double *fPos )
{	
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	// Assume user has checked the tick values first
	if( GetClsRange(whichDim)->GetMinorTickCount() <=0 )
		return false;
	
	double pos1, pos2;
	pos1 = GetMajorTickPosValue(whichDim, whichMajor);
	pos2 = GetMajorTickPosValue(whichDim, whichMajor+1);
	
	int i;
	if(!GetClsRange(whichDim)->IsLogarithm())
	{
		for(i=0; i<=GetClsRange(whichDim)->GetMinorTickCount(); i++)
		{
			fPos[i] = pos1 + i * (pos2 - pos1)/GetClsRange(whichDim)->GetMinorTickCount();
		}
	}
	else
	{
		for(i=0; i<=GetClsRange(whichDim)->GetMinorTickCount(); i++)
		{
			fPos[i] = log10( pow(10, pos1) + i * ( pow(10, pos2)-pow(10, pos1) )/GetClsRange(whichDim)->GetMinorTickCount() );
		}
	}
	
	return true;
}

Declare_Namespace_End

