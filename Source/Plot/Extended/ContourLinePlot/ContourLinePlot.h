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

#ifndef __COUNTOURLINEPLOT_H_122333444455555__
#define __COUNTOURLINEPLOT_H_122333444455555__

#include <windows.h>
#include "../../Basic/CChartNameSpace.h"

#pragma warning(disable:4786)
#include "ContourBasicPlot.h"

Declare_Namespace_CChart
//
class CListContour;

class CContourLinePlot : virtual public CContourBasicPlot
{
public:
	CContourLinePlot();
	virtual ~CContourLinePlot();
protected:
	bool		m_bColorContour;
public:
	int		GetNContour();
	void	SetNContour(int nContour);

	void	SetContourPrecision(int n);

	inline	bool		IsColorContour(){return m_bColorContour;}
	inline	void		SetColorContour(bool cr){m_bColorContour = cr;};
};

Declare_Namespace_End

#endif