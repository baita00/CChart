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

#include <vector>
#include <algorithm>

#include <windows.h>

Declare_Namespace_CChart

class	CPlotDataSpan
{
public:
	CPlotDataSpan();
	virtual	~CPlotDataSpan();

protected:
	void	SetDefaults();

protected:
	bool		m_bSpanMode;
	bool		m_bSpanHorizontal;
	double		m_fSpan[2];
	COLORREF	m_crSpan[2];
	bool		m_bSpanBkgnd;

public:
	inline	bool		IsSpanMode() {return m_bSpanMode;}
	inline	void		SetSpanMode(bool span){m_bSpanMode = span;}

	inline	bool		IsSpanHorizontal() {return m_bSpanHorizontal;}
	inline	void		SetSpanHorizontal(bool horiz){m_bSpanHorizontal = horiz;}

	inline	double*		GetSpan(){return m_fSpan;}
	inline	void		SetSpan(double val){m_fSpan[0] = m_fSpan[1] = val; }

	inline	double		GetSpanLower(){return m_fSpan[0];}
	inline	void		SetSpanLower(double val){m_fSpan[0] = val;}
	inline	double		GetSpanUpper(){return m_fSpan[1];}
	inline	void		SetSpanUpper(double val){m_fSpan[1] = val;}

	inline	void		SetSpanColor(COLORREF cr){m_crSpan[0] = m_crSpan[1] = cr;}
	inline	COLORREF	GetSpanColorLower(){return m_crSpan[0];}
	inline	void		SetSpanColorLower(COLORREF cr){m_crSpan[0] = cr;}
	inline	COLORREF	GetSpanColorUpper(){return m_crSpan[1];}
	inline	void		SetSpanColorUpper(COLORREF cr){m_crSpan[1] = cr;}

	inline	bool		IsSpanBkgnd(){return m_bSpanBkgnd;}
	inline	void		SetSpanBkgnd(bool span){m_bSpanBkgnd = span;}
};

Declare_Namespace_End
