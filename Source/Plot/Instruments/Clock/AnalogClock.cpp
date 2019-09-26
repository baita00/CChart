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

#include "AnalogClock.h"
#include "../InstruBase/InstruBaseImpl.h"

using namespace NsCChart;

CAnalogClock::CAnalogClock()
{
	SetDefaults();
}

CAnalogClock::~CAnalogClock()
{

}

void	CAnalogClock::SetDefaults()
{
	m_nHourShape = kPointerSingleBar;
	m_nMinuteShape = kPointerSingleBar;
	m_nSecondShape = kPointerSingleSword;
	m_nHourWithdraw = 40;
	m_nMinuteWithdraw = 20;
	m_nSecondWithdraw = 10;
	m_nHourBaseHW = 5;
	m_nMinuteBaseHW = 4;
	m_nSecondBaseHW = 4;
	m_nHourEndHW = 5;
	m_nMinuteEndHW = 4;
	m_nSecondEndHW = 3;
	m_crHourColor = RGB(128, 0, 0);
	m_crMinuteColor = RGB(0, 128, 0);
	m_crSecondColor = RGB(0, 0, 128);
}