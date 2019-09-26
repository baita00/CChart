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

#ifndef __RINGPLOT_H_122333444455555__
#define __RINGPLOT_H_122333444455555__

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

class	CRingPlot
{
public:
	CRingPlot();
	virtual ~CRingPlot();

	void		SetDefaults();
	void		CopySettings(CRingPlot *plot);

protected:
	double		m_fInnerRatio;

public:
	double		GetInnerRatio() {return m_fInnerRatio;}
	void		SetInnerRatio(double ratio){m_fInnerRatio = ratio;}
	
};

Declare_Namespace_End

#endif