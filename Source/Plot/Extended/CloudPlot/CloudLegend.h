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

#ifndef __CLOUDLEGEND_H_122333444455555__
#define __CLOUDLEGEND_H_122333444455555__

//#include "../../Basics/Origin.h"
#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

class CCloudLegend
{
public:
	CCloudLegend();
	virtual ~CCloudLegend();

	void		SetDefaults();
protected:
	//int			m_nLegends;
public:
	//int			GetLegends(){return m_nLegends;}
	//void		SetLegends(int num){if(num<2)return; m_nLegends = num;}
};

Declare_Namespace_End

#endif