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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Three Dimension plot use the OpenGL API to draw on the screen

#pragma once

#include <windows.h>
#include "gl/gl.h"
#include "../../Basic/CChartNameSpace.h"
#include "../../Basic/XYPlot.h"


Declare_Namespace_CChart

enum
{
	kTDPlotScatter,
	kTDPlotConnected,
	kTDPlotBezier,

	kTDPlotCount
};

class CTDLinePlot : public CXYPlot
{
public:
	CTDLinePlot();
	~CTDLinePlot();

	void		SetDefaults();


//	bool		m_bAutoDataColor;
//	int			m_nModeOfSetColors;
	GLfloat		m_fvLineColor1[4];
	GLfloat		m_fvLineColor2[4];

public:
//	bool		IsAutoDataColorGeneral(){return m_bAutoDataColor;}
//	void		SetAutoDataColorGerneral(bool bAuto){m_bAutoDataColor = bAuto;}
//	int			GetModeOfSetColors(){return m_nModeOfSetColors;}
//	void		SetModeOnSetColors(int nMode){m_nModeOfSetColors = nMode;}

};

Declare_Namespace_End