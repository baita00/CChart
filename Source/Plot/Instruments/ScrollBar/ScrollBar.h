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

#include "../../Basic/CChartNameSpace.h"
#include <memory.h>

Declare_Namespace_CChart

class CScrollBar
{
public:
	CScrollBar();
	virtual	~CScrollBar();

	void	SetDefaults();
protected:
	bool	m_bEnableScrollBar;
	double	m_pfSliderRange[2];

public:
	inline	bool	IsEnableScrollBar(){return m_bEnableScrollBar;}
	inline	void	SetEnableScrollBar(bool enable){m_bEnableScrollBar = enable;}
	inline	double	*GetSliderRange(){return m_pfSliderRange;}
	inline	void	SetSliderRange(double *range){memcpy(m_pfSliderRange, range, 2*sizeof(double));}
	inline	void	SetSliderRange(double low, double high){m_pfSliderRange[0] = low; m_pfSliderRange[1] = high;}
};

Declare_Namespace_End