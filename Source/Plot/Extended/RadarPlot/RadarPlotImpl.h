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

#include "RadarPlot.h"
#include "../../Basic/XYPlotImpl.h"

Declare_Namespace_CChart

class	CRadarPlotImpl : public CRadarPlot, public CXYPlotImplI
{
public:
	CRadarPlotImpl();
	virtual ~CRadarPlotImpl();

protected:
	vector<tstring>	m_vstrTitles;

public:
	virtual	TCHAR*		GetTypeName(){return _TEXT("RadarPlot");}

	virtual	void		FinalDraw( HDC hDC, RECT destRect );

public:
	int		AddRadarLine(double val, tstring title);

protected:
	void	InitRadarData();
	void	GetRadarRange(double range[2], double &maxval, double &edge);

	void	DrawRadarGrids(HDC hDC, RECT plotRect, double maxval, double edge);
	void	DrawRadarFrame(HDC hDC, RECT plotRect, double maxval, double edge);

	void	DrawRadarTitle(HDC hDC, RECT plotRect, double maxval, double edge);
	void	DrawSingleRadarTitle(HDC hDC, RECT plotRect, double maxval, double edge, int nIndex);

	double	GetStartAngle();

};

typedef CRadarPlotImpl CRadarPlotImplI;

Declare_Namespace_End