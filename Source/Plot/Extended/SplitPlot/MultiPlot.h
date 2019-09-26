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

#ifndef __MULTIPLOT_H_122333444455555__
#define __MULTIPLOT_H_122333444455555__

#include "../../Basic/CChartNameSpace.h"
#include "../../Basic/PlotImpl.h"
#include "SplitPlot.h"

Declare_Namespace_CChart

class CMultiPlot : public CSplitPlotBase
{
public:
	CMultiPlot();
	virtual ~CMultiPlot();

	void	SetDefaults();
	
public:
	vector<CPlotImpl *>	m_vpPlots;
	vector<CHandler *>	m_vpHandlers;
	vector<CReactStatus *> m_vpReactStatus;

public:
	CPlotImpl		*GetSubPlot(int nIndex);
	CHandler		*GetHandler(int nIndex);
	CReactStatus	*GetReactStatus(int nIndex);

public:
	bool			IsSubPlotSelected();
	int				GetIndexOfSelectedSubPlot();
	
public:
	void			DeleteAll();
	void			ResizePlots(int mode, int nRows, int nCols);
	bool			SetSubPlot(CPlotImpl *pSubPlot, CHandler *pHandler, CReactStatus *pReactStatus, int nIndex);
};

Declare_Namespace_End

#endif
