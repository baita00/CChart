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

#include "InterConnect.h"

using namespace NsCChart;

CInterConnect::CInterConnect()
{
	SetDefaults();
}

CInterConnect::~CInterConnect()
{

}

void	CInterConnect::SetDefaults()
{
	m_bInterConnect = false;
	m_nInterConnectLineSize = 1;
	m_nInterConnectLineStyle = PS_DASH;
	m_crInterConnectLineColor = RGB(128, 128, 128);
	m_nInterConnectAlpha = 255;
}