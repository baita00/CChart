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

class	CFixAxisPlot
{
public:
	CFixAxisPlot();
	virtual ~CFixAxisPlot();

	void				SetDefaults( );

protected:
	bool				m_bDualX;
	bool				m_bDualY;

	int					m_nMajorDivsX;
	int					m_nMajorDivsY;
	int					m_nMinorDivsX;
	int					m_nMinorDivsY;

	bool				m_bShowYScaleOnly;

public:
	inline	bool		IsDualX() { return m_bDualX; }
	inline	void		SetDualX( bool set ) { m_bDualX = set; }
	inline	bool		IsDualY() { return m_bDualY; }
	inline	void		SetDualY( bool set ) { m_bDualY = set; }

	inline	bool		IsShowYScaleOnly() { return m_bShowYScaleOnly; }
	inline	void		SetShowYScaleOnly( bool only ) { m_bShowYScaleOnly = only; }

};

Declare_Namespace_End
