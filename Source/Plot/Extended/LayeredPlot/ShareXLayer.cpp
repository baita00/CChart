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

//#include "StdAfx.h"
#include "ShareXLayer.h"
using namespace NsCChart;

CShareXLayer::CShareXLayer(CShareXPlotImpl *pParent):m_pParent(pParent)
{
	SetDefaults();
}

CShareXLayer::~CShareXLayer()
{
	
}
/*
CShareXLayer& CShareXLayer::operator=(const CShareXLayer& rhs)
{
	CopySettings(&rhs);
	return *this;
}
*/
void	CShareXLayer::SetDefaults()
{
}
/*
void	CShareXLayer::CopySettings(const CShareXLayer *plot)
{
}
*/
