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

#include "CChartNameSpace.h"
#include "Range.h"

Declare_Namespace_CChart

class CRangeImpl : public CRange
{
public:
	CRangeImpl();
	virtual ~CRangeImpl();

public:
	void				SetOptimalRange( double low, double high );

	void				SetRange( double *range ){memcpy(m_pfPlotRange, range, 2*sizeof(double));}
	void				SetRange( double low, double high, bool bRangeSet=true );
	void				SetRange( double low, double high, int nTicks, int nMinorTicks, double fTickMin, double fTickMax, bool bRangeSet);

	bool				IsRangeChanged();

	void				CalcRange(double range[2], double dataRange[2], bool bNewDataComming);

	void				ZoomRange( double fFact );

public:
	//double				GetMajorTickPosValue( int whichTick );
	//bool				GetMinorTickPosValue( int whichMajor, double *fPos );
};

Declare_Namespace_End