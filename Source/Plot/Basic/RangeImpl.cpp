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

#include "RangeImpl.h"
#include "PlotUtility.h"
#include <math.h>

using namespace NsCChart;

CRangeImpl::CRangeImpl()
{

}

CRangeImpl::~CRangeImpl()
{

}

void	CRangeImpl::SetOptimalRange( double low, double high )
{
	double range[2], optRange[2];
	range[0] = low; range[1] = high;
	int optMajorTicks, optMinorTicks;
	GetOptimalRange( range, optRange, &optMajorTicks, &optMinorTicks, false, 
		m_bOptimalExactRange,
		m_bOptimalZeroRepair,
		m_bOptimalSymetryRepair,
		m_bOptimalLExtend,
		m_bOptimalUExtend);
	
	memcpy(m_pfPlotRange, optRange, 2*sizeof(double));
	m_nMajorTicks = optMajorTicks;
	m_nMinorTicks = optMinorTicks;
}

void	CRangeImpl::SetRange( double low, double high, bool bRangeSet )
{
	m_pfPlotRange[0] = low;
	m_pfPlotRange[1] = high;
	SetRangeSet(bRangeSet);
	
	double outRange[2];

	if(!IsFloatTicks())
	{
		double range[2];
		memcpy(range, m_pfPlotRange, 2*sizeof(double));
		if(fabs(GetAtom())!=0.0 && fabs(GetAtom())!=1.0)
		{
			range[0]=(range[0]-GetAtomZero())/(GetAtom());
			range[1]=(range[1]-GetAtomZero())/(GetAtom());
		}
		GetOptimalRangeExact(range, outRange, &m_nMajorTicks, &m_nMinorTicks);
		if(fabs(GetAtom())!=0.0 && fabs(GetAtom())!=1.0)
		{
			outRange[0]=outRange[0]*(GetAtom())+(GetAtomZero());
			outRange[1]=outRange[1]*(GetAtom())+(GetAtomZero());
		}
	}
	else
	{
		double v1, v2, realGap;
		if(fabs(GetAtom())!=0.0 && fabs(GetAtom())!=1.0)
		{
			v1 = (m_pfPlotRange[0]-GetAtomZero())/GetAtom();
			v2 = (m_pfPlotRange[1]-GetAtomZero())/GetAtom();
			realGap = m_fTickGap / GetAtom();
		}

		if(!m_bGapDetermined)RegulateTicks(m_pfPlotRange[0], m_pfPlotRange[1], m_nMajorTicks, m_nMinorTicks, m_fTickMin, m_fTickMax);
		else
		{
			if(fabs(GetAtom())!=0.0 && fabs(GetAtom())!=1.0)
			{
				ComputeTicksWithGap(v1, v2, realGap, m_nMajorTicks, m_nMinorTicks, m_fTickMin, m_fTickMax);
				m_fTickMin = m_fTickMin*GetAtom() + GetAtomZero();
				m_fTickMax = m_fTickMax*GetAtom() + GetAtomZero();
			}
			else
			{
				ComputeTicksWithGap(m_pfPlotRange[0]-GetAtomZero(), m_pfPlotRange[1]-GetAtomZero(), m_fTickGap, m_nMajorTicks, m_nMinorTicks, m_fTickMin, m_fTickMax);
				m_fTickMin += GetAtomZero();
				m_fTickMax += GetAtomZero();
			}
		}
	}
}

void	CRangeImpl::SetRange( double low, double high, int nTicks, int nMinorTicks, double fTickMin, double fTickMax, bool bRangeSet)
{
	m_pfPlotRange[0] = low;
	m_pfPlotRange[1] = high;
	SetTickCount(nTicks);
	SetMinorTickCount(nMinorTicks);
	SetTickMin(fTickMin);
	SetTickMax(fTickMax);
	SetRangeSet(bRangeSet);
	//m_bXAutoRange = false;
}

bool	CRangeImpl::IsRangeChanged()
{
	//if(!IsRangeSet())return true;
	if(memcmp(m_pfPlotRange, m_pfLastPlotRange, 2*sizeof(double)) != 0)return true;
	return false;
}

void	CRangeImpl::CalcRange(double range[2], double dataRange[2], bool bNewDataComming)
{
	//double *pRange = (bX?m_pfXPlotRange:m_pfYPlotRange);
	double pRange[2];
	GetRange(pRange);

	int nMajorT, nMinorT;
	double realGap;
	
	memcpy(range, pRange, 2*sizeof(double));
	
	double optRange[2];
	if(IsExactRange())
	{
		if(!IsRangeSet())memcpy(range, dataRange, 2*sizeof(double));

		if(fabs(GetAtom())!=0.0 && fabs(GetAtom())!=1.0)
		{
			range[0]=(range[0]-GetAtomZero())/(GetAtom());
			range[1]=(range[1]-GetAtomZero())/(GetAtom());
			realGap = m_fTickGap/GetAtom();
		}
		
		if(IsFloatTicks())
		{
			if(!m_bGapDetermined)RegulateTicks(range[0], range[1], m_nMajorTicks, m_nMinorTicks, m_fTickMin, m_fTickMax);
			else
			{
				if(fabs(GetAtom())!=0.0 && fabs(GetAtom())!=1.0)
				{
					ComputeTicksWithGap(range[0], range[1], realGap, m_nMajorTicks, m_nMinorTicks, m_fTickMin, m_fTickMax);
				}
				else
				{
					ComputeTicksWithGap(range[0]-GetAtomZero(), range[1]-GetAtomZero(), m_fTickGap, m_nMajorTicks, m_nMinorTicks, m_fTickMin, m_fTickMax);
					m_fTickMin += GetAtomZero();
					m_fTickMax += GetAtomZero();

					m_pfPlotRange[0] = m_fTickMin - m_fTickGap/2.0;
					m_pfPlotRange[1] = m_fTickMax + m_fTickGap/2.0;
				}
			}
		}
		else
		{
			GetOptimalRangeExact( range, optRange, &nMajorT, &nMinorT, GetSegsPrefer() ); 
			if(IsAutoMajorTick())(SetTickCount(nMajorT));
			if(IsAutoMinorTick())(SetMinorTickCount(nMinorT));
		}
		if(fabs(GetAtom())!=0.0 && fabs(GetAtom())!=1.0)
		{
			range[0]=range[0]*(GetAtom())+(GetAtomZero());
			range[1]=range[1]*(GetAtom())+(GetAtomZero());
		}

		//memcpy(bX?m_pfXPlotRange:m_pfYPlotRange, range, 2*sizeof(double));
		SetRange(range);
		SetRangeSet(true);
		return;
	}
	
	if(bNewDataComming)
	{
		if(IsAutoRange())SetRangeSet(false);
		//SetRangeSet(false);
	}
	if(IsAutoRange()) (SetRangeSet(false));
	double Range[2];
	if(!IsRangeSet())
	{
		//GetDataRange(Range);
		memcpy(Range, dataRange, 2*sizeof(double));
		
		// divide
		realGap = m_fTickGap;
		if(fabs(GetAtom())!=0.0 && fabs(GetAtom())!=1.0)
		{
			Range[0]=(Range[0]-(GetAtomZero()))/(GetAtom());
			Range[1]=(Range[1]-(GetAtomZero()))/(GetAtom());
			realGap /= GetAtom();
		}
		
		if(IsFloatTicks())
		{
			memcpy(range, Range, 2*sizeof(double));

			if(!m_bGapDetermined)RegulateTicks(Range[0], Range[1], m_nMajorTicks, m_nMinorTicks, m_fTickMin, m_fTickMax);
			else
			{
				if(fabs(GetAtom())!=0.0 && fabs(GetAtom())!=1.0)
				{
					ComputeTicksWithGap(Range[0], Range[1], realGap, m_nMajorTicks, m_nMinorTicks, m_fTickMin, m_fTickMax);
				}
				else
				{
					ComputeTicksWithGap(Range[0]-GetAtomZero(), Range[1]-GetAtomZero(), m_fTickGap, m_nMajorTicks, m_nMinorTicks, m_fTickMin, m_fTickMax);
					m_fTickMin += GetAtomZero();
					m_fTickMax += GetAtomZero();
				}
				
			}
		}
		else
		{
			if(!(IsLogarithm()))
			{
				GetOptimalRange( Range, range, &nMajorT, &nMinorT, true,
					IsOptimalExactRange(),
					IsOptimalZeroRepair(),
					IsOptimalSymetryRepair(),
					IsOptimalLExtend(),
					IsOptimalUExtend() );
				if(IsAutoMajorTick())(SetTickCount(nMajorT));
				if(IsAutoMinorTick())(SetMinorTickCount(nMinorT));
			}
			else
			{
				range[0] = floor(Range[0]);
				range[1] = ceil(Range[1]);
				//GetOptimalRangeExact( Range, range, bX?&m_nMajorTicksX:&m_nMajorTicksY, bX?&m_nMinorTicksX:&m_nMinorTicksY );
				if(IsAutoMajorTick())(SetTickCount(int(range[1] - range[0] - 1)));
				if(IsAutoMinorTick())(SetMinorTickCount(9));
				
				(SetTickMin(range[0]));
				(SetTickMax(range[1]));
			}
		}
		SetRangeSet(true);
		// reverse
		if(fabs(GetAtom())!=0.0 && fabs(GetAtom())!=1.0)
		{
			//Range[0] = floor(range[0]);
			//Range[1] = ceil(range[1]);
			range[0]=range[0]*fabs(GetAtom())+(GetAtomZero());
			range[1]=range[1]*fabs(GetAtom())+(GetAtomZero());
			
			//GetOptimalRangeExact( Range, range, &nMajorT, &nMinorT, bX?m_nSegsPreferX:m_nSegsPreferY );
			//if(bX?m_bAutoMajorTickX:m_bAutoMajorTickY)(bX?m_nMajorTicksX:m_nMajorTicksY)=nMajorT;
			//if(bX?m_bAutoMinorTickX:m_bAutoMinorTickY)(bX?m_nMinorTicksX:m_nMinorTicksY)=nMinorT;
		}

		
/*		if(m_bGapDetermined && m_bDefExpandWhileGapDet)
		{
			range[0] = min(m_pfPlotRange[0], m_fTickMin - m_fTickGap/2.0);
			range[1] = max(m_pfPlotRange[1], m_fTickMax + m_fTickGap/2.0);
		}
*/
		memcpy(pRange, range, 2*sizeof(double));
		SetRange(pRange);
	}
	else
	{
		GetRange(range);
	}
}

void		CRangeImpl::ZoomRange( double fFact )
{
	if(fFact<=0)return;

	double ct;
	switch(m_nZoomCenter)
	{
	case kRangeZoomCenterL:
		ct = m_pfPlotRange[0];
		break;
	case kRangeZoomCenterH:
		ct = m_pfPlotRange[1];
		break;
	case kRangeZoomCenterM:
		ct = (m_pfPlotRange[0] + m_pfPlotRange[1])/2.0;
		break;
	default:
		return;
	}

	m_pfPlotRange[0] = ct + fFact*(m_pfPlotRange[0] - ct);
	m_pfPlotRange[1] = ct + fFact*(m_pfPlotRange[1] - ct);
}

/*
double		CRangeImpl::GetMajorTickPosValue( int whichTick )
{
	double		range,pos,epsl=1.0e-12;
	
	
	int nTicks = (GetTickCount());
	
	// Assume user has checked the tick values first
	if(!(IsFloatTicks()))
	{
		range = m_pfPlotRange[1]-m_pfPlotRange[0];
	}
	else
	{
		range = (GetTickMax()) - (GetTickMin());
	}
	
	double startVal;
	if(!(IsFloatTicks()))
	{
		startVal = m_pfPlotRange[0];
	}
	else
	{
		startVal = (GetTickMin());
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

bool		CRangeImpl::GetMinorTickPosValue( int whichMajor, double *fPos )
{	
	// Assume user has checked the tick values first
	if( (GetMinorTickCount()) <=0 )
		return false;
	
	double pos1, pos2;
	pos1 = GetMajorTickPosValue(whichMajor);
	pos2 = GetMajorTickPosValue(whichMajor+1);
	
	int i;
	if(!(IsLogarithm()))
	{
		for(i=0; i<=(GetMinorTickCount()); i++)
		{
			fPos[i] = pos1 + i * (pos2 - pos1)/(GetMinorTickCount());
		}
	}
	else
	{
		for(i=0; i<=(GetMinorTickCount()); i++)
		{
			fPos[i] = log10( pow(10, pos1) + i * ( pow(10, pos2)-pow(10, pos1) )/(GetMinorTickCount() ) );
		}
	}
	
	return true;
}
*/