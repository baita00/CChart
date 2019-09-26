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

#include "../../Basic/MyString.h"
#include "../../Basic/CChartNameSpace.h"
#include <windows.h>

Declare_Namespace_CChart

enum
{
	kTrafficRed,
	kTrafficYellow,
	kTrafficGreen,
	kTrafficOff,
	kTrafficLeftRed,
	kTrafficLeftGreen,
	kTrafficRightRed,
	kTrafficRightGreen,
	kTrafficTurnLeftRed,
	kTrafficTurnLeftGreen,
	kTrafficTurnRightRed,
	kTrafficTurnRightGreen,
	kTrafficNoLeft,
	kTrafficNoRight,
	kTrafficForwardRed,
	kTrafficForwardGreen,
	kTrafficBackwardRed,
	kTrafficBackwardGreen,
	kTrafficNoReturn,
	kTrafficStop,
	kTrafficError,

	kTrafficStatusCount
};

class CTrafficLight
{
public:
	CTrafficLight();
	virtual	~CTrafficLight();

	void	SetDefaults();

protected:
	int			m_nRadius;
	int			m_nStatus;
	COLORREF	m_crOffColor;

public:
	inline	int			GetRadius(){return m_nRadius;}
	inline	void		SetRadius(int radius){m_nRadius = radius;}
	inline	int			GetStatus(){return m_nStatus;}
	inline	void		SetStatus(int status){m_nStatus = status;}
	inline	COLORREF	GetOffColor(){return m_crOffColor;}
	inline	void		SetOffColor(COLORREF color){m_crOffColor = color;}
};

Declare_Namespace_End
