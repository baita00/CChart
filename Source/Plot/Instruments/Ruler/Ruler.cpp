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

#include "Ruler.h"

using namespace NsCChart;

CRuler::CRuler()
{
	SetDefaults();
}

CRuler::~CRuler()
{

}

void	CRuler::SetDefaults()
{
	m_nRulerLength = 10;
	m_bLowerSide = true;
	m_bDodgeLabel = true;
}