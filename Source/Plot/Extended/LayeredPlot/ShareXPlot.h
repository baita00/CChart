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

#pragma warning(disable:4786)

#include <windows.h>
#include <vector>
using namespace std;

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart


class CShareXPlot
{
public:
	CShareXPlot();
	virtual ~CShareXPlot();

	void	SetDefaults();

protected:
	bool	m_bShareExact;
	bool	m_bOriginRange;

public:
	inline	bool	IsShareExact(){return m_bShareExact;}
	inline	void	SetShareExact(bool exact){m_bShareExact = exact;}
	inline	bool	IsOriginRange(){return m_bOriginRange;}
	inline	void	SetOriginRange(bool origin){m_bOriginRange = origin;}

};

Declare_Namespace_End