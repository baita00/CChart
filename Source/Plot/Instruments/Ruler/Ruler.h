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

class CRuler
{
public:
	CRuler();
	virtual	~CRuler();

	void	SetDefaults();
protected:
	int		m_nRulerLength;
	bool	m_bLowerSide;
	bool	m_bDodgeLabel;

public:
	inline	int		GetRulerLength(){return m_nRulerLength;}
	inline	void	SetRulerLength(int length){m_nRulerLength = length;}
	inline	bool	IsLowerSide(){return m_bLowerSide;}
	inline	void	SetLowerSide(bool lower){m_bLowerSide = lower;}
	inline	bool	IsDodgeLabel(){return m_bDodgeLabel;}
	inline	void	SetDodgeLabel(bool dodge){m_bDodgeLabel = dodge;}
};

Declare_Namespace_End