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

#ifndef __PLOTLAYERBASICIMPL_H_122333444455555__
#define __PLOTLAYERBASICIMPL_H_122333444455555__

#include "../../Basic/XYPlotBasicImpl.h"
//#include "../../Basic/XYHandler.h"
Declare_Namespace_CChart

class CPlotLayerBasicImpl :	public CXYPlotBasicImpl
{
public:
	CPlotLayerBasicImpl();
	virtual ~CPlotLayerBasicImpl();

	TCHAR* GetTypeName(){return _TEXT("PlotLayer");}

	bool		NeedDrawing(){return !IsEmpty();}

	SIZE		GetAxisSize( HDC hDC, int location );

	void		FinalDraw( HDC hDC, RECT destRect );

	void		DataToLP( double *data, LPPOINT point );
	void		LPToData( LPPOINT point, double *data );
};

Declare_Namespace_End

#endif