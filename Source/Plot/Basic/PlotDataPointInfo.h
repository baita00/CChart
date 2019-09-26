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

#include "CChartNameSpace.h"

Declare_Namespace_CChart

// how to show datapoint infomation
enum
{
	kShowDataPointInfoX = 0,
	kShowDataPointInfoY = 1,
	kShowDataPointInfoXY = 2,
	
	kShowDataPointInfoTypeCount
};

class CPlotDataPointInfo
{
public:
	CPlotDataPointInfo();
	virtual	~CPlotDataPointInfo();
protected:
	void	SetDefaults();

protected:
	bool		m_bDataPointInfoShow;
	int			m_nDataPointInfoType;
	int			m_nDataPointInfoFrequency;
	int			m_nDataPointInfoStartIndex;

public:
	inline	bool		InDataPointInfoShow( ) { return m_bDataPointInfoShow; }
	inline	void		SetDataPointInfoShow( bool show ) { m_bDataPointInfoShow = show; }
	inline	int			GetDataPointInfoType( ) { return m_nDataPointInfoType; }
	inline	void		SetDataPointInfoType( int type ) { m_nDataPointInfoType = type; }
	inline	int			GetDataPointInfoFrequency( ) { return m_nDataPointInfoFrequency; }
	inline	void		SetDataPointInfoFrequency( int freq ) { m_nDataPointInfoFrequency = freq; }
	inline	int			GetDataPointInfoStartIndex( ) { return m_nDataPointInfoStartIndex; }
	inline	void		SetDataPointInfoStartIndex( int index ) { m_nDataPointInfoStartIndex = index; }

};

Declare_Namespace_End