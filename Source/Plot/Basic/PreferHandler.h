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
#include <windows.h>
#include "CChartNameSpace.h"

Declare_Namespace_CChart

template<class PlotImplT>
class	CPreferHandler
{
public:
	CPreferHandler();
	virtual	~CPreferHandler();

public:
	bool		LButtonDown( HDC hDC, POINT point, UINT ctrlKey );
	bool		LButtonUp( HDC hDC, POINT point, UINT ctrlKey );
	bool		LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey );
	bool		MouseMove( HDC hDC, POINT point, UINT ctrlKey );
	bool		MouseLeave( HDC hDC, POINT point, UINT ctrlKey );
	bool		ContextMenu( HMENU hMenu, HDC hDC, POINT point );
	bool		KeyDown( HDC hDC, UINT key );
};

template<class PlotImplT>
CPreferHandler<PlotImplT>::CPreferHandler()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	RegisterHandler(pT, _TEXT("prefer"));
}

template<class PlotImplT>
CPreferHandler<PlotImplT>::~CPreferHandler()
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);
	UnRegisterHandler(pT, _TEXT("prefer"));
}

template<class PlotImplT>
bool	CPreferHandler<PlotImplT>::LButtonDown( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;

	return needUpdate;
}

template<class PlotImplT>
bool	CPreferHandler<PlotImplT>::LButtonUp( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	

	return needUpdate;
}

template<class PlotImplT>
bool	CPreferHandler<PlotImplT>::LButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(pT->IsMsgHandled())return false;
	
	bool needUpdate = false;
	
	return needUpdate;
}

template<class PlotImplT>
bool	CPreferHandler<PlotImplT>::MouseMove( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	if(pT->IsMsgHandled())return false;

	bool needUpdate = false;
	
	return needUpdate;
}

template<class PlotImplT>
bool	CPreferHandler<PlotImplT>::MouseLeave( HDC hDC, POINT point, UINT ctrlKey )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	return false;
}

template<class PlotImplT>
bool	CPreferHandler<PlotImplT>::ContextMenu( HMENU hMenu, HDC hDC, POINT point )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	return false;
}

template<class PlotImplT>
bool	CPreferHandler<PlotImplT>::KeyDown( HDC hDC, UINT key )
{
	PlotImplT* pT = static_cast<PlotImplT*>(this);

	return false;
}

Declare_Namespace_End