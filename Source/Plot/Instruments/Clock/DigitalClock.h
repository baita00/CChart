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

#include "Clock.h"

Declare_Namespace_CChart

class CDigitalClock : public CClock
{
public:
	CDigitalClock();
	virtual	~CDigitalClock();

	void	SetDefaults();
protected:
	bool	m_bIncludeYMD;
	bool	m_bSingleLine;
	bool	m_bShowColon;

public:
	inline	bool	IsIncludeYMD() { return m_bIncludeYMD; }
	inline	void	SetIncludeYMD( bool incl ) { m_bIncludeYMD = incl; }
	inline	bool	IsSingleLine() { return m_bSingleLine; }
	inline	void	SetSingleLine( bool sl ) { m_bSingleLine = sl; }
	inline	bool	IsShowColon() { return m_bShowColon; }
	inline	void	SetShowColon( bool show ) { m_bShowColon = show; }
};

Declare_Namespace_End
