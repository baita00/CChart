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

#include "ScrollBar.h"

using namespace NsCChart;

CScrollBar::CScrollBar()
{
	SetDefaults();
}

CScrollBar::~CScrollBar()
{

}

void	CScrollBar::SetDefaults()
{
	m_bEnableScrollBar = true;
	m_pfSliderRange[0]=0.0;
	m_pfSliderRange[1]=1.0;
}