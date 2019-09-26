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

#include "TabbedPlot.h"
#include "TabbedHandler.h"
#include "../../Basic/PlotImpl.h"
#include "../../Basic/ReactStatus.h"

Declare_Namespace_CChart

class CTabbedPlotImpl : public CTabbedPlot, public CPlotImpl
{
public:
	CTabbedPlotImpl();
	virtual ~CTabbedPlotImpl();

protected:
	void		DrawTabBar( HDC hDC, RECT destRect );

public:
	virtual	TCHAR*		GetTypeName(){return _TEXT("TabbedPlot");}
	virtual	void		FinalDraw( HDC hDC, RECT destRect );

public:
	bool		IsEmpty(){ return (int)m_vpPlots.size()<=0; }
	bool		IsNoDataPlot(){return false;}
	void		GetDataRange1D( int whichDim, double *range ){}

public:
	SIZE		GetTabBarSize(HDC hDC);
	RECT		GetTabRect(int nIndex, HDC hDC, RECT destRect);

};

class CTabbedPlotImplI :	public CTabbedPlotImpl,
							public CTabbedHandler<CTabbedPlotImplI>
							//public CReactStatus<CTabbedPlotImplI>,
							//public CDragStatus<CTabbedPlotImplI>
{
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

Declare_Namespace_End