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

#include "Grids.h"

using namespace NsCChart;

CGrids::CGrids()
{
	SetDefaults();
}

CGrids::~CGrids()
{

}

void	CGrids::SetDefaults()
{
	m_bMajorHGrids = false;
	m_bMajorVGrids = false;
	m_bMinorHGrids = false;
	m_bMinorVGrids = false;

	m_crMajorGridColor = RGB( 175, 175, 175 );
	m_crMinorGridColor = RGB( 205, 205, 205 );
	
	m_nMajorGridLineSize = 1;
	m_nMinorGridLineSize = 1;
	
	m_nMajorGridLineStyle = PS_DASH;
	m_nMinorGridLineStyle = PS_DOT;
}