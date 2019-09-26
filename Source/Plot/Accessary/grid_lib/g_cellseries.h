#include <tchar.h>
#include <stdio.h>
#include <windows.h>

#include "array.h"
#include "gridcell.h"

#include "g_editbox.h"
#include "g_checkbox.h"

#ifndef _CGRIDCELLSERIES_H
#define _CGRIDCELLSERIES_H

class CGridCellSeries
{
public:
	CGridCellSeries();
	virtual ~CGridCellSeries();
	
	bool						Init(HWND hWnd, HINSTANCE hInst);
	void						Uninit();
	VT_TYPE						GetType(int nIndex);
	void						SetType(int nColumn, VT_TYPE type, int nCount = 1);
	HWND						GetEditHandle(int nColumn);
	void						SetFont(HFONT hFont);


	virtual HWND				EditCell(int nColumn, RECT& rcCell, void* pCellInfo, UINT nChar = 0);

protected:
	CMyArray<VT_TYPE>			m_cellseries;
	CGridEditBox				*m_editBox;
	bool						m_bEditBoxCreated;
	HWND						m_hParent;

public:
	CGridEditBox				*GetEditBox(){return m_editBox;}
	void						SetEditBox(CGridEditBox *pBox){delete m_editBox; m_bEditBoxCreated = false; m_editBox = pBox;}
	bool						GetEditBoxCreated(){return m_bEditBoxCreated;}
	void						SetEditBoxCreated(bool bCreated){m_bEditBoxCreated =bCreated;}
};

#endif