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

class	CXY3DPlot
{
public:
	CXY3DPlot();
	virtual ~CXY3DPlot();

	void				SetDefaults( );

protected:
	double	m_fLenZAxis;
	double	m_fAngleZAxis;

	bool	m_bYZMajorHGrids;
	bool	m_bYZMajorVGrids;
	bool	m_bYZMinorHGrids;
	bool	m_bYZMinorVGrids;

	bool	m_bZXMajorHGrids;
	bool	m_bZXMajorVGrids;
	bool	m_bZXMinorHGrids;
	bool	m_bZXMinorVGrids;

public:
	inline	double	GetLenZAxis() { return m_fLenZAxis; }
	inline	void	SetLenZAxis( int len ) { m_fLenZAxis = len; }
	inline	double	GetAngleZAxis() { return m_fAngleZAxis; }
	inline	void	SetAngleZAxis( int angle ) { m_fAngleZAxis = angle; }

	inline	bool	IsYZMajorHGrids() { return m_bYZMajorHGrids; }
	inline	void	SetYZMajorHGrids( bool use ) { m_bYZMajorHGrids = use; }
	inline	bool	IsYZMajorVGrids() { return m_bYZMajorVGrids; }
	inline	void	SetYZMajorVGrids( bool use ) { m_bYZMajorVGrids = use; }
	inline	bool	IsYZMinorHGrids() { return m_bYZMinorHGrids; }
	inline	void	SetYZMinorHGrids( bool use ) { m_bYZMinorHGrids = use; }
	inline	bool	IsYZMinorVGrids() { return m_bYZMinorVGrids; }
	inline	void	SetYZMinorVGrids( bool use ) { m_bYZMinorVGrids = use; }

	inline	bool	IsZXMajorHGrids() { return m_bZXMajorHGrids; }
	inline	void	SetZXMajorHGrids( bool use ) { m_bZXMajorHGrids = use; }
	inline	bool	IsZXMajorVGrids() { return m_bZXMajorVGrids; }
	inline	void	SetZXMajorVGrids( bool use ) { m_bZXMajorVGrids = use; }
	inline	bool	IsZXMinorHGrids() { return m_bZXMinorHGrids; }
	inline	void	SetZXMinorHGrids( bool use ) { m_bZXMinorHGrids = use; }
	inline	bool	IsZXMinorVGrids() { return m_bZXMinorVGrids; }
	inline	void	SetZXMinorVGrids( bool use ) { m_bZXMinorVGrids = use; }

};

Declare_Namespace_End