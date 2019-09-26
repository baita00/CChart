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

#pragma warning(disable:4786)

#include <vector>
using namespace std;

#include "../../Basic/CChartNameSpace.h"

Declare_Namespace_CChart

typedef struct stBendingSect
{
	double z1;//
	double psi1;//normal direction along positive y
	double z2;//
	double psi2;//normal direction along positive y
}BendingSect, *pBendingSect;

typedef vector<BendingSect> vBendingSect;

class CBendingPlot
{
public:
	CBendingPlot();
	virtual ~CBendingPlot();
protected:
	double m_fDeltaAngle;

protected:
	vBendingSect m_vBendingSect;
	void ConvertData(double x, double y, double &xn, double &yn);
	bool GetCoodOfSect(int idx, double &x1, double &y1, double &x2, double &y2);
	void GetCoodOfZAxis(double z, double &x, double &y);
public:
	bool AddBendingSect(double z1, double psi1, double z2, double psi2);
};

Declare_Namespace_End