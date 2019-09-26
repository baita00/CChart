#include <tchar.h>
#include <stdio.h>
#include <windows.h>

#ifndef _CGRIDEDITBOX_H
#define _CGRIDEDITBOX_H

#define			CellID					POINT

#define			WM_CANCEL_EDIT			WM_USER + 202
#define			WM_EDITED				WM_USER + 203


class CGridEditBox
{
public:
	CGridEditBox();
	virtual ~CGridEditBox();

	HWND	CreateEdit(DWORD dwStyle, int x, int y, int cx, int cy, HWND hWndParent, HINSTANCE hInstance);

protected:
	virtual bool		OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool		OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual bool		OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void		OnSysCommand(UINT uCmdType, POINT pt);

protected:
	static	LRESULT		CALLBACK EditWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static WNDPROC		lpfnWndProc;

public:
	HWND				m_hWnd;
};

#endif
