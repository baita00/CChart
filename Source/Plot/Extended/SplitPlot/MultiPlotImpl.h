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

#ifndef __MULTIPLOTIMPL_H_122333444455555__
#define __MULTIPLOTIMPL_H_122333444455555__

#include "MultiPlot.h"
#include "SplitPlotImpl.h"
#include "../../Basic/XYPlotImpl.h"

Declare_Namespace_CChart

class CMultiPlotImpl :	public CMultiPlot,
						public CPlotImpl
{
public:
	CMultiPlotImpl();
	virtual ~CMultiPlotImpl();

public:
	virtual	TCHAR*		GetTypeName(){return _TEXT("MultiPlot");}

	virtual	void		FinalDraw( HDC hDC, RECT destRect );
	
public:
	RECT	GetSubPlotRect(RECT clientRect,int nIndex);
	
	// CPlotImpl is a pure virtual class, so the following functions should be defined
	CAxisImpl	*NewAxis(){ return 0; }
	bool	IsEmpty(){ return (int)m_vpPlots.size()<=0; }
	void	GetDataRange1D( int whichDim, double *range ){}
	bool	IsNoDataPlot(){return false;}

	//add the following function since m_rctLastClientRect have inherited twice
	RECT			GetLastClientRect(){return CPlotImpl::GetLastClientRect();}
	void			SetLastClientRect(RECT rt){CPlotImpl::SetLastClientRect(rt);}
	RECT			GetLastPlotRect(){return CPlotImpl::GetLastPlotRect();}
	void			SetLastPlotRect(RECT rt){CPlotImpl::SetLastPlotRect(rt);}
};

class CMultiPlotImplI : public CMultiPlotImpl,
						public CSplitHandler<CMultiPlotImplI>,
						public CSplitReactStatus<CMultiPlotImplI>,
						public CDragStatus<CMultiPlotImplI>
{
public:
	virtual	tstring	GetDescriptionByMousePos(HDC hDC, POINT point);
	
public:
	int		m_nSpliterIndex;	
};

HMENU	CreateContextMenu(CMultiPlotImplI *plot);
bool DefPlotSettings( CMultiPlotImplI *plot, HDC hDC );
bool DefAxisSettings( CMultiPlotImplI *plot, HDC hDC, int location );
bool DefDataSettings( CMultiPlotImplI *plot, HDC hDC, int whichDataSet );

Declare_Namespace_End

#endif
