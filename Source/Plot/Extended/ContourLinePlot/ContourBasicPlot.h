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

#include <windows.h>

#include <vector>
using std::vector;

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

class CListContour;

typedef double	(FcnField)(double x, double y);

typedef struct stContourPoint
{
	double c[3];
}ContourPoint;

class CContourBasicPlot
{
public:
	CContourBasicPlot();
	virtual ~CContourBasicPlot();
protected:
	CListContour* m_pListContour;
	COLORREF	m_crColor1,m_crColor2;
	bool		m_bContourChanged;

public:
	CListContour* GetContour(){return m_pListContour;	}
	virtual	void	SetFieldFcn(double (*_pFieldFcn) (double, double));
	double	(*GetFieldFcn( ))( double, double );
#ifndef _WIN64
	void	SetFieldFcn(double (_stdcall *_pFieldFcn) (double, double));
#endif

	inline	COLORREF	GetColor1(){return m_crColor1;}
	inline	void		SetColor1(COLORREF color){m_crColor1=color;}
	inline	COLORREF	GetColor2(){return m_crColor2;}
	inline	void		SetColor2(COLORREF color){m_crColor2=color;}

	inline	bool	IsContourChanged(){return m_bContourChanged;}
	inline	void	SetContourChanged(bool changed){m_bContourChanged = changed;}

	void	SetContourRange(double pRange[4]);
	void	SetContourRange(double xRange[2], double yRange[2]);
	void	SetContourRange(double xl, double xh, double yl, double yh);
	void	GetContourRange(double pRange[4]);
	void	GetContourRange(double xRange[2], double yRange[2]);
	void	SetContourXRange(double low, double high);
	void	SetContourYRange(double low, double high);

	// the following is for points contour
	void	SetToPointContour(){SetFieldFcn(ContourHeight);}
	void	AddContourPoint(double x, double y, double h);
	void	ClrContourPoints();

//protected:
	static	vector<ContourPoint>	m_vContourPoints;
	static	vector<double> vX, vY;
	static	double		ContourHeight(double x, double y);
	void	GetPtsRng(int which, double &lower, double &upper);
	
};

Declare_Namespace_End
