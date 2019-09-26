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
#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

class CGradienter
{
public:
	CGradienter();
	virtual	~CGradienter();

	void	SetDefaults();

protected:
	COLORREF		m_crWaterColor;
	COLORREF		m_crAirColor;
	COLORREF		m_crSurfaceColor;
	int				m_nSurfaceThick;

};

Declare_Namespace_End
