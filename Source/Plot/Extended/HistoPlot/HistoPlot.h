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

#include <vector>
using namespace std;

Declare_Namespace_CChart

class CHistoPlot
{
public:
	CHistoPlot();
	virtual	~CHistoPlot();

	void				SetDefaults( );

protected:
	vector<double>		m_vfHistoData;
	int					m_nDivides;
	double				m_fRange[2];
	bool				m_bAutoRange;

	virtual	bool		Statistic() = 0;


public:
	void				InsertData(double data) { m_vfHistoData.push_back(data); }
	vector<double>		&GetHistoData(){ return m_vfHistoData; }
	void				AddHistoData(double *pVal, int nLen);
};

Declare_Namespace_End