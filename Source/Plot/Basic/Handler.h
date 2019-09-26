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

#include <windows.h>
#include <vector>
#include "MyString.h"
#include "CChartNameSpace.h"

Declare_Namespace_CChart

typedef bool	(FcnAddonMouseAction)(void *plot, HDC hDC, POINT point, UINT ctrlKey, void *para, bool &bContinue);
typedef bool	(FcnAddonKeyAction)(void *plot, HDC hDC, UINT key, void *para, bool &bContinue);

typedef void	(FcnAddPluginPopupMenu)(void *plot, HMENU addMenu, void *pPara );
typedef	int		(FcnInterpretPluginPopupMenu)(void *plot, HWND hWnd, int result, void *pPara );


class	CHandler
{
public:
	CHandler();
	virtual	~CHandler();

protected:
	void	SetDefaults();

protected:
	bool	m_bMsgHandled;

	bool	m_bReact;
public:
	int		m_nOldRegionLBD, m_nOldRegionMM;

	bool	m_bMouseTrack;

protected:
	FcnAddonMouseAction	*m_fcnPreMouseMove;
	void				*m_pParaPreMouseMove;
	FcnAddonMouseAction	*m_fcnPreLButtonDown;
	void				*m_pParaPreLButtonDown;
	FcnAddonMouseAction	*m_fcnPreLButtonUp;
	void				*m_pParaPreLButtonUp;
	FcnAddonMouseAction	*m_fcnPreLButtonDblClk;
	void				*m_pParaPreLButtonDblClk;
	FcnAddonKeyAction	*m_fcnPreKeyDown;
	void				*m_pParaPreKeyDown;
	
	FcnAddonMouseAction	*m_fcnPostMouseMove;
	void				*m_pParaPostMouseMove;
	FcnAddonMouseAction	*m_fcnPostLButtonDown;
	void				*m_pParaPostLButtonDown;
	FcnAddonMouseAction	*m_fcnPostLButtonUp;
	void				*m_pParaPostLButtonUp;
	FcnAddonMouseAction	*m_fcnPostLButtonDblClk;
	void				*m_pParaPostLButtonDblClk;
	FcnAddonKeyAction	*m_fcnPostKeyDown;
	void				*m_pParaPostKeyDown;
public:
	// for the additional context menus
	FcnAddPluginPopupMenu			*m_pFcnAddPluginMenu;
	FcnInterpretPluginPopupMenu		*m_pFcnInterpretPluginMenu;
	void							*m_pParaAddPlugInMenu;
	void							*m_pParaInterpretPlugInMenu;

	bool							m_bContextMenu;
public:
	inline	FcnAddonMouseAction	*GetFcnPreLButtonDown() { return m_fcnPreLButtonDown; }
	inline	void		SetFcnPreLButtonDown( FcnAddonMouseAction *action) {m_fcnPreLButtonDown = action;}
	inline	void		SetFcnPreLButtonDown( FcnAddonMouseAction *action, void *para) {m_fcnPreLButtonDown = action; m_pParaPreLButtonDown = para;}
	inline	void*		GetParaPreLButtonDown() { return m_pParaPreLButtonDown; }
	inline	void		SetParaPreLButtonDown( void *para ){ m_pParaPreLButtonDown = para; }
	
	inline	FcnAddonMouseAction	*GetFcnPreLButtonUp() { return m_fcnPreLButtonUp; }
	inline	void		SetFcnPreLButtonUp( FcnAddonMouseAction *action) {m_fcnPreLButtonUp = action;}
	inline	void		SetFcnPreLButtonUp( FcnAddonMouseAction *action, void *para) {m_fcnPreLButtonUp = action; m_pParaPreLButtonUp = para;}
	inline	void*		GetParaPreLButtonUp() { return m_pParaPreLButtonUp; }
	inline	void		SetParaPreLButtonUp( void *para ){ m_pParaPreLButtonUp = para; }
	
	inline	FcnAddonMouseAction	*GetFcnPreLButtonDblClk() { return m_fcnPreLButtonDblClk; }
	inline	void		SetFcnPreLButtonDblClk( FcnAddonMouseAction *action) {m_fcnPreLButtonDblClk = action;}
	inline	void		SetFcnPreLButtonDblClk( FcnAddonMouseAction *action, void *para) {m_fcnPreLButtonDblClk = action; m_pParaPreLButtonDblClk = para;}
	inline	void*		GetParaPreLButtonDblClk() { return m_pParaPreLButtonDblClk; }
	inline	void		SetParaPreLButtonDblClk( void *para ){ m_pParaPreLButtonDblClk = para; }

	inline	FcnAddonMouseAction	*GetFcnPreMouseMove() { return m_fcnPreMouseMove; }
	inline	void		SetFcnPreMouseMove( FcnAddonMouseAction *action) {m_fcnPreMouseMove = action;}
	inline	void		SetFcnPreMouseMove( FcnAddonMouseAction *action, void *para) {m_fcnPreMouseMove = action;m_pParaPreMouseMove = para;}
	inline	void*		GetParaPreMouseMove() { return m_pParaPreMouseMove; }
	inline	void		SetParaPreMouseMove( void *para ){ m_pParaPreMouseMove = para; }
	
	inline	FcnAddonKeyAction	*GetFcnPreKeyDown() { return m_fcnPreKeyDown; }
	inline	void		SetFcnPreKeyDown( FcnAddonKeyAction *action) {m_fcnPreKeyDown = action;}
	inline	void		SetFcnPreKeyDown( FcnAddonKeyAction *action, void *para) {m_fcnPreKeyDown = action; m_pParaPreKeyDown = para;}
	inline	void*		GetParaPreKeyDown() { return m_pParaPreKeyDown; }
	inline	void		SetParaPreKeyDown( void *para ){ m_pParaPreKeyDown = para; }
	
	
	inline	FcnAddonMouseAction	*GetFcnPostLButtonDown() { return m_fcnPostLButtonDown; }
	inline	void		SetFcnPostLButtonDown( FcnAddonMouseAction *action) {m_fcnPostLButtonDown = action;}
	inline	void		SetFcnPostLButtonDown( FcnAddonMouseAction *action, void *para) {m_fcnPostLButtonDown = action; m_pParaPostLButtonDown = para;}
	inline	void*		GetParaPostLButtonDown() { return m_pParaPostLButtonDown; }
	inline	void		SetParaPostLButtonDown( void *para ){ m_pParaPostLButtonDown = para; }
	
	inline	FcnAddonMouseAction	*GetFcnPostLButtonUp() { return m_fcnPostLButtonUp; }
	inline	void		SetFcnPostLButtonUp( FcnAddonMouseAction *action) {m_fcnPostLButtonUp = action;}
	inline	void		SetFcnPostLButtonUp( FcnAddonMouseAction *action, void *para) {m_fcnPostLButtonUp = action; m_pParaPostLButtonUp = para;}
	inline	void*		GetParaPostLButtonUp() { return m_pParaPostLButtonUp; }
	inline	void		SetParaPostLButtonUp( void *para ){ m_pParaPostLButtonUp = para; }
	
	inline	FcnAddonMouseAction	*GetFcnPostLButtonDblClk() { return m_fcnPostLButtonDblClk; }
	inline	void		SetFcnPostLButtonDblClk( FcnAddonMouseAction *action) {m_fcnPostLButtonDblClk = action;}
	inline	void		SetFcnPostLButtonDblClk( FcnAddonMouseAction *action, void *para) {m_fcnPostLButtonDblClk = action; m_pParaPostLButtonDblClk = para;}
	inline	void*		GetParaPostLButtonDblClk() { return m_pParaPostLButtonDblClk; }
	inline	void		SetParaPostLButtonDblClk( void *para ){ m_pParaPostLButtonDblClk = para; }

	inline	FcnAddonMouseAction	*GetFcnPostMouseMove() { return m_fcnPostMouseMove; }
	inline	void		SetFcnPostMouseMove( FcnAddonMouseAction *action) {m_fcnPostMouseMove = action;}
	inline	void		SetFcnPostMouseMove( FcnAddonMouseAction *action, void *para) {m_fcnPostMouseMove = action; m_pParaPostMouseMove = para;}
	inline	void*		GetParaPostMouseMove() { return m_pParaPostMouseMove; }
	inline	void		SetParaPostMouseMove( void *para ){ m_pParaPostMouseMove = para; }
	
	inline	FcnAddonKeyAction	*GetFcnPostKeyDown() { return m_fcnPostKeyDown; }
	inline	void		SetFcnPostKeyDown( FcnAddonKeyAction *action) {m_fcnPostKeyDown = action;}
	inline	void		SetFcnPostKeyDown( FcnAddonKeyAction *action, void *para) {m_fcnPostKeyDown = action; m_pParaPostKeyDown = para;}
	inline	void*		GetParaPostKeyDown() { return m_pParaPostKeyDown; }
	inline	void		SetParaPostKeyDown( void *para ){ m_pParaPostKeyDown = para; }

public:
	bool	IsMsgHandled(){return m_bMsgHandled;}
	void	SetMsgHandled(bool bHandled){m_bMsgHandled = bHandled;}
	bool	IsContextMenu(){return m_bContextMenu;}
	void	SetContextMenu(bool bSet){m_bContextMenu=bSet;}

	bool	IsReact(){return m_bReact;}
	void	SetReact(bool bReact){m_bReact = true;}

public:
	virtual bool		OnLButtonDown( HDC hDC, POINT point, UINT ctrlKey )=0;
	virtual bool		OnLButtonUp( HDC hDC, POINT point, UINT ctrlKey )=0;
	virtual bool		OnLButtonDblClk( HDC hDC, POINT point, UINT ctrlKey )=0;
	virtual bool		OnMouseMove( HDC hDC, POINT point, UINT ctrlKey )=0;
	virtual bool		OnMouseLeave( HDC hDC, POINT point, UINT ctrlKey )=0;
	virtual bool		OnContextMenu( HMENU hMenu, HDC hDC, POINT point )=0;
	virtual bool		OnKeyDown( HDC hDC, UINT key ) = 0;
public:
	virtual bool		OnLButtonDown( HWND hWnd, POINT point, UINT ctrlKey );
	virtual bool		OnLButtonUp( HWND hWnd, POINT point, UINT ctrlKey );
	virtual bool		OnLButtonDblClk( HWND hWnd, POINT point, UINT ctrlKey );
	virtual bool		OnMouseMove( HWND hWnd, POINT point, UINT ctrlKey );
	virtual bool		OnMouseLeave( HWND hWnd, POINT point, UINT ctrlKey );
	virtual bool		OnContextMenu( HMENU hMenu, HWND hWnd, POINT point );
	virtual bool		OnKeyDown( HWND hWnd, UINT key );
	
	virtual	bool		OnEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual void		Interactive(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)=0;
};

Declare_Namespace_End
