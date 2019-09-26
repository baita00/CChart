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

#include "Clock.h"

Declare_Namespace_CChart

class CAnalogClock : public CClock
{
public:
	CAnalogClock();
	virtual	~CAnalogClock();

	void	SetDefaults();

public:
	int			m_nHourShape, m_nMinuteShape, m_nSecondShape;
	int			m_nHourWithdraw, m_nMinuteWithdraw, m_nSecondWithdraw;
	int			m_nHourBaseHW, m_nMinuteBaseHW, m_nSecondBaseHW;
	int			m_nHourEndHW, m_nMinuteEndHW, m_nSecondEndHW;
	COLORREF	m_crHourColor, m_crMinuteColor, m_crSecondColor;
};

Declare_Namespace_End
