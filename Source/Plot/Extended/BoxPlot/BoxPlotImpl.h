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
#include "BoxPlot.h"
#include "BoxHandler.h"
#include "../../Basic/XYPlotImpl.h"

Declare_Namespace_CChart

class CBoxPlotImpl :	public CBoxPlot,
						public CXYPlotImpl
{
public:
	CBoxPlotImpl();
	virtual	~CBoxPlotImpl();

public:
	void			GetPlotRange( double *xRange, double *yRange );
	
	bool			IsNoDataPlot(){return false;}
	TCHAR*			GetTypeName(){return _TEXT("BixPlot");}
	bool			IsEmpty(){return m_vpBoxNodes.size()==0;}
	
	void			GetDataRange1D( int whichDim, double *range );

public:
	int		AddBoxNode(BoxNode node);
	int		AddBoxNode(double fXVal, double fQ1, double fQ3, double fLower, double fUpper);

protected:
	long			GetBoxNodeXCoord( int nIndex, RECT plotRect, double *xRange);
	long			GetBoxNodeYCoord( double fYVal, RECT plotRect, double *yRange);
	RECT			GetBoxNodeRect( int nIndex, RECT plotRect, double *xRange, double *yRange, bool bFull );
	void			DrawPlotDataEach(HDC hDC, double *xRange, double *yRange);
	void			DrawSingleBoxNode(int nIndex, HDC hDC, RECT plotRect, double *xRange, double *yRange);

public:
	int				GetBoxNodeIndexByPoint(POINT point);
};

class	CBoxPlotImplI :		public CBoxPlotImpl,
							public CBoxHandler<CBoxPlotImplI>
{
public:
	CBoxPlotImplI();
	virtual ~CBoxPlotImplI();
};

Declare_Namespace_End