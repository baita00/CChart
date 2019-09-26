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

#ifndef __SHAREXLAYER_H_122333444455555__
#define __SHAREXLAYER_H_122333444455555__

#include <windows.h>
#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

class CShareXPlotImpl;

class CShareXLayer
{
protected:
	CShareXPlotImpl	*m_pParent;
public:
	CShareXLayer(CShareXPlotImpl *pParent);
	virtual ~CShareXLayer();

	void		SetDefaults();
};

Declare_Namespace_End

#endif