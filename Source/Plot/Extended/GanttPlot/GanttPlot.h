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
#include "../../Basic/MyString.h"
#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

class	CGanttPlot
{
public:
	CGanttPlot();
	virtual ~CGanttPlot();

	void				SetDefaults( );

protected:
	double	m_fRatioOccupy;
	bool	m_bReverseOrder;

	bool	m_bDrawGanttTitle;
	tstring	m_strGanttTitleFormat;
	bool	m_bGanttTitleUpper;
	LOGFONT	m_lfGanttTitleFont;

public:
	inline	double	GetRatioOccupy(){ return m_fRatioOccupy; }
	inline	void	SetRatioOccupy( double ratio ){ m_fRatioOccupy = ratio; }
	inline	bool	IsReverseOrder(){ return m_bReverseOrder; }
	inline	void	SetReverseOrder( bool reverse ){ m_bReverseOrder = reverse; }
	inline	bool	IsDrawGanttTitle(){ return m_bDrawGanttTitle; }
	inline	void	SetDrawGanttTitle( bool draw ){ m_bDrawGanttTitle = draw; }
	inline	tstring	GetGanttTitleFormat(){ return m_strGanttTitleFormat; }
	inline	void	SetGanttTitleFormat( tstring format ){ m_strGanttTitleFormat = format; }
	inline	bool	IsGanttTitleUpper(){ return m_bGanttTitleUpper; }
	inline	void	SetGanttTitleUpper( bool upper ){ m_bGanttTitleUpper = upper; }
	inline	LOGFONT	GetGanttTitleFont(){ return m_lfGanttTitleFont; }
	inline	void	SetGanttTitleFont( LOGFONT logFont ){ m_lfGanttTitleFont = logFont; }
};

Declare_Namespace_End