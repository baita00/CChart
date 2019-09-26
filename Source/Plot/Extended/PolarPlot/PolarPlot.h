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

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

class	CPolarPlot
{
public:
	CPolarPlot();
	virtual ~CPolarPlot();

	void		SetDefaults();

protected:
	bool		m_bUseMajorRGrids;
	bool		m_bUseMajorPGrids;
	bool		m_bUseMinorRGrids;
	bool		m_bUseMinorPGrids;

public:
	inline	bool		IsUseMajorRGrids(){return m_bUseMajorRGrids;}
	inline	void		SetUseMajorRGrids(bool bUse){m_bUseMajorRGrids=bUse;}
	inline	bool		IsUseMajorPGrids(){return m_bUseMajorPGrids;}
	inline	void		SetUseMajorPGrids(bool bUse){m_bUseMajorPGrids=bUse;}
	inline	bool		IsUseMinorRGrids(){return m_bUseMinorRGrids;}
	inline	void		SetUseMinorRGrids(bool bUse){m_bUseMinorRGrids=bUse;}
	inline	bool		IsUseMinorPGrids(){return m_bUseMinorPGrids;}
	inline	void		SetUseMinorPGrids(bool bUse){m_bUseMinorPGrids=bUse;}
};

Declare_Namespace_End