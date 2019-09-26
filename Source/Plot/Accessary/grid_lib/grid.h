#ifndef _CGRID_H
#define _CGRID_H

#define _WINSOCKAPI_
#include <windows.h>
#include "matrix.h"
#include "cellrange.h"
#include "mergemanager.h"
#include "memdc.h"
#include "matrix.h"
#include "rectrgn.h"
#include "gridcell.h"

#include "g_cellseries.h"

#define					GRID_CLASS_NAME				_TEXT("Grid")

#define					WM_GRID_SELECTCHANGED		WM_USER + 250

enum AllowResizeType
{
	ALL, 
	VERT, 
	HORZ, 
	NONE, 
};

class CGrid
{
	friend class CGridCell;
	friend HWND			CreateGrid(DWORD dwStyle, int x, int y, int cx, int cy, HWND hWndParent, HINSTANCE hInstance);
public:
	CGrid();
	virtual ~CGrid();

	RECT					GetCellRect(int x, int y);
	RECT					GetWindowRect();

	int						GetRowHeight(int nRow);
	int						GetColWidth(int nCol);
	int						GetFixRowCount();
	int						GetFixColCount();

	void					SetRowHeight(int nRow, int nHeight, int nCount = 1);
	void					SetColWidth(int nCol, int nWidth, int nCount = 1);
	void					SetFixRowCount(int nRow = 1);
	void					SetFixColCount(int nCol = 1);

	void					OnDraw(HDC hdc, DWORD dwTick);

	CMergeCell*				GetMergeInfo(int x, int y);
	void					MergeCells(int x, int y, int cx, int cy);
	void					SplitCell(int x, int y);

	CellID					CellFromPoint(POINT pt);

	void					SetTopIndex(int y);
	void					SetLeftIndex(int x);

	void					InsertRow(int y, int cy = 1);
	void					InsertCol(int x, int cx = 1);
	void					RemoveRow(int y, int cy = 1);
	void					RemoveCol(int x, int cx = 1);

	void					ClearCell(CellID id);
	void					SetCellText(CellID id, TCHAR *e);
	void					SetCellChar(CellID id, const char e);
	void					SetCellUChar(CellID id, const unsigned char e);
	void					SetCellInt(CellID id, const int e);
	void					SetCellUInt(CellID id, const unsigned int e);
	void					SetCellShort(CellID id, const short e);
	void					SetCellUShort(CellID id, const unsigned short e);
	void					SetCellLong(CellID id, const long e);
	void					SetCellULong(CellID id, const unsigned long e);
	void					SetCellFloat(CellID id, const float e);
	void					SetCellDouble(CellID id, const double e);
	void					SetCellBool(CellID id, const bool e);

	TCHAR*					GetCellText(CellID id);
	char					GetCellChar(CellID id);
	unsigned char			GetCellUChar(CellID id);
	short					GetCellShort(CellID id);
	unsigned short			GetCellUShort(CellID id);
	int						GetCellInt(CellID id);
	unsigned int			GetCellUInt(CellID id);
	long					GetCellLong(CellID id);
	unsigned long			GetCellULong(CellID id);
	float					GetCellFloat(CellID id);
	double					GetCellDouble(CellID id);
	bool					GetCellBool(CellID id);

	int						SetRows(int nRows);
	int						SetCols(int nCols);
	int						GetRows();
	int						GetCols();

	COLORREF				GetFixCellBorderColor();
	void					SetFixCellBorderColor(COLORREF color);
	COLORREF				GetCellBorderColor();
	void					SetCellBorderColor(COLORREF color);
	COLORREF				GetFixCellBkgColor();
	void					SetFixCellBkgColor(COLORREF color);
	COLORREF				GetSheetBorderColor();
	void					SetSheetBorderColor(COLORREF color);
	COLORREF				GetBkgColor();
	void					SetBkgColor(COLORREF color);
	HBRUSH					GetBkgBrush();
	COLORREF				GetFocusBorderColor();
	void					SetFocusBorderColor(COLORREF color);
	COLORREF				GetSelectedBkgColor();
	void					SetSelectedBkgColor(COLORREF color);

//protected:
	static	DWORD			GetNowTick();

protected:
	POINT					GetOriginPos();
	void					DrawCell(HDC hdc, RECT &rc, int x, int y, DWORD dwTick);
	void					DrawFixCell(HDC hdc, RECT &rc, int x, int y, DWORD dwTick);
	void					DrawSheetBorder(HDC hdc, RECT &rc);

	void					DrawSelectedBorder(HDC hdc, RECT &rc, int x, int y, CMergeCell* pMerge = NULL);

	void					CalcVInfo(int nHeight);
	void					CalcHInfo(int nWidth);

	void					GetDrawDetail(RECT* prcDraw, 
								RECT* pTopHeader, RECT* pLeftHeader, RECT* pTopLeftHeader, RECT* pBody, 
								RECT* pTopHeaderPos, RECT* pLeftHeaderPos, RECT* pTopLeftHeaderPos, RECT* pBodyPos);

	void					DrawTopHeaderCells(HDC hDC, POINT ptOrigin, RECT* pCells, RECT* pPos, DWORD dwTick);
	void					DrawLeftHeaderCells(HDC hDC, POINT ptOrigin, RECT* pCells, RECT* pPos, DWORD dwTick);
	void					DrawTopLeftHeaderCells(HDC hDC, POINT ptOrigin, RECT* pCells, RECT* pPos, DWORD dwTick);
	void					DrawBodyCells(HDC hDC, POINT ptOrigin, RECT* pCells, RECT* pPos, DWORD dwTick);

public:
	bool					GetPreferColumn(){return m_bPreferColumn;}
	void					SetPreferColumn(bool prefer){m_bPreferColumn = prefer;}
	CGridCellSeries*		GetColumnInfo();
	virtual VT_TYPE			GetColumnType(int nColumn);
	virtual void			SetColumnType(int nColumn, VT_TYPE type, int nCount = 1);
	CGridCellSeries*		GetRowInfo();
	virtual VT_TYPE			GetRowType(int nRow);
	virtual void			SetRowType(int nRow, VT_TYPE type, int nCount = 1);
	virtual	VT_TYPE			GetCellType(int x, int y);
	virtual HWND			EditCell(CellID id, UINT nChar = 0);
	virtual void			UnEditCell(bool bSave = true);
protected:
//	CGridEditBox			m_editBox;
	bool					m_bPreferColumn;
	CGridCellSeries			m_columnInfo;
	CGridCellSeries			m_rowInfo;
	bool					m_bEditting;
	CellID					m_cellEditting;

protected:
	virtual void			OnEdited(int x, int y);
	virtual void			OnCancelEdit(int x, int y);

protected:
	virtual void			OnSelectChanged(const CCellRange& range);

public:
	void					SetFont(HFONT hFont);
	HFONT					GetFont();
protected:
	HFONT					m_hFont;

protected:
	CMyArray<int>			m_aryWidth, m_aryHeight;
	int						m_nFixRowCount, m_nFixColCount;
	CellID					m_cellIndex;

	HPEN					m_hCellBorder;
	HPEN					m_hFixCellBorder;
	HBRUSH					m_hFixBkgnd;
	int						m_nLastRow, m_nLastCol;
	int						m_nRows, m_nCols;
	int						m_nLastRowPos, m_nLastColPos;
	int						m_nDrawedBodyRows, m_nDrawedBodyCols;
	HPEN					m_hSheetBorder;
	HBRUSH					m_hOutSheetBkgnd;
	HBRUSH					m_hSheetBkgnd;
	HBRUSH					m_hSelectBkgnd;
	HPEN					m_hSelectBorder;
	HPEN					m_hFocusInSideBorder;

	HCURSOR					m_hHorzSize;
	HCURSOR					m_hVertSize;
	HCURSOR					m_hRight;
	HCURSOR					m_hDown;
	HCURSOR					m_hCross;

protected:
	int						GetBodyCellLeft(int x, int nStart = 0);
	int						GetBodyCellTop(int y, int nStart = 0);
	void					GetCellsRect(int l, int t, int r, int b, RECT *pRect);
	int						m_nXExcursion, m_nYExcursion;
	CGridMerger				m_merge;

public:
	CRectRgn*				GetSelect();
protected:
	bool					IsSelected(int x, int y);
	CRectRgn				m_selectRgn;
	CellID					m_idFocus;

	bool					m_bLButtonDown;
	CellID					m_idLastSelect;
	CellID					m_idClickDown;
	bool					m_bTimerOn;

protected:
	enum ResizeType
	{
		none, 
		vert, 
		horz, 
	};

	ResizeType				m_resizeType;
	int						m_nResizeIndex;
	int						m_nResizeOldPos;
	int						m_nResizePos;
	bool					m_bResizing;
	HPEN					m_hResizePen;
	AllowResizeType			m_allowRisizeType;
	void					DrawResizeLine(HDC hdc, int x1, int y1, int x2, int y2);
public:
	void					SetResizeType(AllowResizeType type);

protected:
	void					OnUpKey(int nCtrl, int nShift);
	void					OnLeftKey(int nCtrl, int nShift);
	void					OnRightKey(int nCtrl, int nShift);
	void					OnDownKey(int nCtrl, int nShift);
	void					OnPriorKey(int nCtrl, int nShift);
	void					OnNextKey(int nCtrl, int nShift);
	void					OnDelKey(int nCtrl, int nShift);
	CGridCell*				OnGetElement(int x, int y);

public:
	virtual void			OnClose();
	virtual void			OnPaint();
	virtual BOOL			OnEraseBkgnd(HDC hDC);
	virtual void			OnSize(UINT nType, int cx, int cy);
	virtual void			OnVScroll(UINT nSBCode, UINT nPos, HWND hScrollBar);
	virtual void			OnHScroll(UINT nSBCode, UINT nPos, HWND hScrollBar);
	virtual void			OnLButtonDown(UINT nFlags, POINT point);
	virtual void			OnLButtonUp(UINT nFlags, POINT point);
	virtual void			OnLButtonDblClk(UINT nFlags, POINT point);
	virtual void			OnRButtonDown(UINT nFlags, POINT point);
	virtual void			OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void			OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void			OnMouseMove(UINT nFlags, POINT point);
	virtual void			OnMouseWheel(UINT nFlags, short zDelta, POINT pt);
	virtual void			OnTimer(UINT nIDEvent);

protected:
	CMyMatrix<CGridCell>	m_cellMatrix;

public:
	HWND					m_hWnd;
	friend LRESULT	CALLBACK GridWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

LRESULT	CALLBACK GridWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif