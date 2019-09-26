#include "grid.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

#define					DEF_ROW_HEIGHT				20
#define					DEF_COL_WIDTH				70

#define					SELECT_TIMER_ID				1
#define					SELECT_TIMER_INTERVAL		50

typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;
typedef basic_ostringstream<TCHAR> tostringstream;

CGrid::CGrid()
{
	m_cellIndex.x = m_cellIndex.y = 0;
	m_nFixRowCount = 1;
	m_nFixColCount = 1;

	m_nLastRow = m_nLastCol = 0;
	m_nRows = 320;
	m_nCols = 240;

	m_nLastRowPos = m_nLastColPos = -1;

	m_hCellBorder = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	m_hFixCellBorder = CreatePen(PS_SOLID, 1, RGB(192, 192, 192));
	m_hFixBkgnd = CreateSolidBrush(RGB(236, 233, 216));

	m_hSheetBorder = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	m_hOutSheetBkgnd = CreateSolidBrush(RGB(192, 192, 192));
	m_hSheetBkgnd = (HBRUSH) GetStockObject(WHITE_BRUSH);
	m_hSelectBkgnd = CreateSolidBrush(RGB(182, 202, 234));//CreateSolidBrush(RGB(182, 202, 234));
	m_hSelectBorder = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	m_hFocusInSideBorder = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));

	m_hHorzSize = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE));
	m_hVertSize = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));
	m_hRight = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	m_hDown = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	m_hCross = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));

	m_resizeType = none;
	m_bResizing = false;
	m_hResizePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	m_allowRisizeType = ALL;

	m_idClickDown.x = m_idClickDown.y = -1;
	m_idLastSelect.x = m_idLastSelect.y = -1;
	m_idFocus.x = m_idFocus.y = -1;
	m_bLButtonDown = false;

	m_nXExcursion = m_nYExcursion = 0;

	m_bTimerOn = false;

	m_bEditting = false;

	m_bPreferColumn = true;

	CGridCheckBox::Init();

	m_hFont = NULL;
}

CGrid::~CGrid()
{
	DeleteObject(m_hCellBorder);
	DeleteObject(m_hFixCellBorder);
	DeleteObject(m_hFixBkgnd);

	DeleteObject(m_hSheetBorder);
	DeleteObject(m_hOutSheetBkgnd);
	DeleteObject(m_hSelectBkgnd);
	DeleteObject(m_hSelectBorder);
	DeleteObject(m_hFocusInSideBorder);

	DestroyCursor(m_hHorzSize);
	DestroyCursor(m_hVertSize);
	DestroyCursor(m_hRight);
	DestroyCursor(m_hDown);
	DestroyCursor(m_hCross);

	DeleteObject(m_hResizePen);

	CGridCheckBox::Uninit();

	if (m_hFont)
		DeleteObject(m_hFont);
}

void CGrid::SetFont(HFONT hFont)
{
	LOGFONT font;
	if (sizeof(LOGFONT) != ::GetObject(hFont, sizeof(LOGFONT), &font))
		return ;
	if (m_hFont)
		DeleteObject(m_hFont);
	m_hFont = CreateFontIndirect(&font);

	m_columnInfo.SetFont(m_hFont);
	m_rowInfo.SetFont(m_hFont);
}

HFONT CGrid::GetFont()
{
	return m_hFont;
}

RECT CGrid::GetCellRect(int x, int y)
{
	RECT cellRect;
//	GetCellsRect(x, y, x+1, y+1, &cellRect);
	
	cellRect.left = 0;
	cellRect.top = 0;

	int i;
	for(i=0; i<x; i++)
	{
		cellRect.left += GetColWidth(i);
	}
	cellRect.right = cellRect.left + GetColWidth(i);
	for(i=0; i<y; i++)
	{
		cellRect.top += GetRowHeight(i);
	}
	cellRect.bottom = cellRect.top + GetRowHeight(i);

	return cellRect;
}

RECT CGrid::GetWindowRect()
{
	RECT windowRect;
	
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = 0;
	windowRect.bottom = 0;
	int i;
	for(i=0; i<GetCols(); i++)
	{
		windowRect.right += GetColWidth(i);
	}
	for(i=0; i<GetRows(); i++)
	{
		windowRect.bottom += GetRowHeight(i);
	}
	
	return windowRect;
}

int CGrid::GetRowHeight(int nRow)
{
	if (nRow < m_aryHeight.GetSize())
		return m_aryHeight[nRow];
	else
		return DEF_ROW_HEIGHT;
}

int CGrid::GetColWidth(int nCol)
{
	if (nCol < m_aryWidth.GetSize())
		return m_aryWidth[nCol];
	else
		return DEF_COL_WIDTH;
}

int CGrid::GetFixRowCount()
{
	return m_nFixRowCount;
}

int CGrid::GetFixColCount()
{
	return m_nFixColCount;
}


void CGrid::SetRowHeight(int nRow, int nHeight, int nCount)
{
	int nOldSize = m_aryHeight.GetSize();
	if (nRow > nOldSize)
	{
		m_aryHeight.InsertAt(nOldSize, DEF_ROW_HEIGHT, nRow - nOldSize);
		m_aryHeight.InsertAt(nRow, nHeight, nCount);
	}
	else if ((nRow + nCount) <= m_aryHeight.GetSize())
	{
		for (int i = 0; i < nCount; ++ i)
			m_aryHeight[nRow + i] = nHeight;
	}
	else
	{
		//		m_aryHeight.InsertAt(nRow, nHeight, nCount);
		m_aryHeight.SetSize(nRow + nCount);
		for (int i = nRow; i < nRow + nCount; ++ i)
		{
			m_aryHeight[i] = nHeight;
		}
	}

	//
	int n = m_merge.GetMergeCellCount();
	for (int i = 0; i < n; ++ i)
	{
		CMergeCell* pMerge = m_merge.GetMergeCell(i);
		if (pMerge->m_range.GetID().y > nRow ||
			pMerge->m_range.GetBottom() > nRow)
		{
			GetCellsRect(pMerge->m_range.GetID().x, pMerge->m_range.GetID().y,
				pMerge->m_range.GetID().x + pMerge->m_range.GetCX(),
				pMerge->m_range.GetID().y + pMerge->m_range.GetCY(),
				&pMerge->m_rect);
		}
	}

	//
	if (nRow <m_nFixRowCount || nRow + nCount> m_nLastRow)
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		POINT pt = GetOriginPos();
		CalcVInfo(rc.bottom - pt.y);
	}
}

void CGrid::SetColWidth(int nCol, int nWidth, int nCount)
{
	int nOldSize = m_aryWidth.GetSize();
	if (nCol > nOldSize)
	{
		m_aryWidth.InsertAt(nOldSize, DEF_COL_WIDTH, nCol - nOldSize);
		m_aryWidth.InsertAt(nCol, nWidth, nCount);
	}
	else if ((nCol + nCount) <= m_aryWidth.GetSize())
	{
		for (int i = 0; i < nCount; ++ i)
			m_aryWidth[nCol + i] = nWidth;
	}
	else
	{
		//		m_aryWidth.InsertAt(nCol, nWidth, nCount);
		m_aryWidth.SetSize(nCol + nCount);
		for (int i = nCol; i < nCol + nCount; ++ i)
		{
			m_aryWidth[i] = nWidth;
		}
	}

	//
	int n = m_merge.GetMergeCellCount();
	for (int i = 0; i < n; ++ i)
	{
		CMergeCell* pMerge = m_merge.GetMergeCell(i);
		if (pMerge->m_range.GetID().x > nCol ||
			pMerge->m_range.GetRight() > nCol)
		{
			GetCellsRect(pMerge->m_range.GetID().x, pMerge->m_range.GetID().y,
				pMerge->m_range.GetID().x + pMerge->m_range.GetCX(),
				pMerge->m_range.GetID().y + pMerge->m_range.GetCY(),
				&pMerge->m_rect);
		}
	}

	//
	if (nCol <m_nFixColCount || nCol + nCount> m_nLastCol)
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		POINT pt = GetOriginPos();
		CalcHInfo(rc.right - pt.x);
	}
}

void CGrid::SetFixRowCount(int nRow)
{
	m_nFixRowCount = nRow;
}

void CGrid::SetFixColCount(int nCol)
{
	m_nFixColCount = nCol;
}

void CGrid::OnDraw(HDC hdc, DWORD dwTick)
{
	RECT rc;
	GetClipBox(hdc, &rc);

	POINT ptOrigin = GetOriginPos();

	RECT rcTopHeader, rcLeftHeader, rcTopLeftHeader, rcBody;
	RECT rcTopHeaderPos, rcLeftHeaderPos, rcTopLeftHeaderPos, rcBodyPos;


	m_nDrawedBodyRows = m_nDrawedBodyCols = 0;
	GetDrawDetail(&rc, &rcTopHeader, &rcLeftHeader, &rcTopLeftHeader, &rcBody,
		&rcTopHeaderPos, &rcLeftHeaderPos, &rcTopLeftHeaderPos, &rcBodyPos);

	DrawBodyCells(hdc, ptOrigin, &rcBody, &rcBodyPos, dwTick);
	DrawTopHeaderCells(hdc, ptOrigin, &rcTopHeader, &rcTopHeaderPos, dwTick);
	DrawLeftHeaderCells(hdc, ptOrigin, &rcLeftHeader, &rcLeftHeaderPos, dwTick);
	DrawTopLeftHeaderCells(hdc, ptOrigin, &rcTopLeftHeader,
		&rcTopLeftHeaderPos, dwTick);

	m_nDrawedBodyRows = rcBody.bottom -
		rcBody.top +
		rcTopHeader.bottom -
		rcTopHeader.top;
	m_nDrawedBodyCols = rcBody.right -
		rcBody.left +
		rcLeftHeader.right -
		rcLeftHeader.left;

	// draw sheet border
	if (m_nLastRowPos != -1 || m_nLastColPos != -1)
		DrawSheetBorder(hdc, rc);
}

void CGrid::DrawTopHeaderCells(HDC hDC, POINT ptOrigin, RECT* pCells,
	RECT* pPos, DWORD dwTick)
{
	if (pCells->right < pCells->left || pCells->bottom < pCells->top)
		return ;

	int i, j, n;
	// background
	FillRect(hDC, pPos, m_hFixBkgnd);
	// lines
	SelectObject(hDC, m_hFixCellBorder);
	n = pPos->top;
	for (j = pCells->top; j < pCells->bottom; ++ j)
	{
		n += GetRowHeight(j);
		MoveToEx(hDC, pPos->left, n, NULL);
		LineTo(hDC, pPos->right, n);
	}
	n = pPos->left;
	for (i = pCells->left; i < pCells->right; ++ i)
	{
		n += GetColWidth(i);
		MoveToEx(hDC, n, pPos->top, NULL);
		LineTo(hDC, n, pPos->bottom);
	}

	// cells
	RECT rcCell;
	rcCell.top = pPos->top;
	for (j = pCells->top; j < pCells->bottom; ++ j)
	{
		rcCell.bottom = rcCell.top + GetRowHeight(j);
		rcCell.left = pPos->left;
		for (i = pCells->left; i < pCells->right; ++ i)
		{
			rcCell.right = rcCell.left + GetColWidth(i);



			CMergeCell* pMerge = m_merge.GetMergeCell(i, j);
			if (pMerge)
			{
				if (pMerge->m_dwDrawTick < dwTick)
				{
					RECT rcMerge = pMerge->m_rect;
					rcMerge.left -= (m_nXExcursion - ptOrigin.x - 1);
					rcMerge.right -= (m_nXExcursion - ptOrigin.x);
					rcMerge.top -= (m_nYExcursion - ptOrigin.y - 1);
					rcMerge.bottom -= (m_nYExcursion - ptOrigin.y);

					FillRect(hDC, &rcMerge, m_hSheetBkgnd);

					DrawCell(hDC, rcMerge, pMerge->m_range.GetID().x,
						pMerge->m_range.GetID().y, dwTick);
					pMerge->m_dwDrawTick = dwTick;
					DrawSelectedBorder(hDC, rcMerge, i, j, pMerge);
				}
			}
			else
			{
				DrawFixCell(hDC, rcCell, i, j, dwTick);
			}


			rcCell.left = rcCell.right;
		}
		rcCell.top = rcCell.bottom;
	}
}

void CGrid::DrawLeftHeaderCells(HDC hDC, POINT ptOrigin, RECT* pCells,
	RECT* pPos, DWORD dwTick)
{
	if (pCells->right < pCells->left || pCells->bottom < pCells->top)
		return ;

	int i, j, n;
	// background
	FillRect(hDC, pPos, m_hFixBkgnd);
	// lines
	SelectObject(hDC, m_hFixCellBorder);
	n = pPos->top;
	for (j = pCells->top; j < pCells->bottom; ++ j)
	{
		n += GetRowHeight(j);
		MoveToEx(hDC, pPos->left, n, NULL);
		LineTo(hDC, pPos->right, n);
	}
	n = pPos->left;
	for (i = pCells->left; i < pCells->right; ++ i)
	{
		n += GetColWidth(i);
		MoveToEx(hDC, n, pPos->top, NULL);
		LineTo(hDC, n, pPos->bottom);
	}

	// cells
	RECT rcCell;
	rcCell.top = pPos->top;
	for (j = pCells->top; j < pCells->bottom; ++ j)
	{
		rcCell.bottom = rcCell.top + GetRowHeight(j);
		rcCell.left = pPos->left;
		for (i = pCells->left; i < pCells->right; ++ i)
		{
			rcCell.right = rcCell.left + GetColWidth(i);



			CMergeCell* pMerge = m_merge.GetMergeCell(i, j);
			if (pMerge)
			{
				if (pMerge->m_dwDrawTick < dwTick)
				{
					RECT rcMerge = pMerge->m_rect;
					rcMerge.left -= (m_nXExcursion - ptOrigin.x - 1);
					rcMerge.right -= (m_nXExcursion - ptOrigin.x);
					rcMerge.top -= (m_nYExcursion - ptOrigin.y - 1);
					rcMerge.bottom -= (m_nYExcursion - ptOrigin.y);

					FillRect(hDC, &rcMerge, m_hSheetBkgnd);

					DrawCell(hDC, rcMerge, pMerge->m_range.GetID().x,
						pMerge->m_range.GetID().y, dwTick);
					pMerge->m_dwDrawTick = dwTick;
					DrawSelectedBorder(hDC, rcMerge, i, j, pMerge);
				}
			}
			else
			{
				DrawFixCell(hDC, rcCell, i, j, dwTick);
			}


			rcCell.left = rcCell.right;
		}
		rcCell.top = rcCell.bottom;
	}
}

void CGrid::DrawTopLeftHeaderCells(HDC hDC, POINT ptOrigin, RECT* pCells,
	RECT* pPos, DWORD dwTick)
{
	if (pCells->right < pCells->left || pCells->bottom < pCells->top)
		return ;

	int i, j, n;
	// background
	FillRect(hDC, pPos, m_hFixBkgnd);
	// lines
	SelectObject(hDC, m_hFixCellBorder);
	n = pPos->top;
	for (j = pCells->top; j < pCells->bottom; ++ j)
	{
		n += GetRowHeight(j);
		MoveToEx(hDC, pPos->left, n, NULL);
		LineTo(hDC, pPos->right, n);
	}
	n = pPos->left;
	for (i = pCells->left; i < pCells->right; ++ i)
	{
		n += GetColWidth(i);
		MoveToEx(hDC, n, pPos->top, NULL);
		LineTo(hDC, n, pPos->bottom);
	}

	// cells
	RECT rcCell;
	rcCell.top = pPos->top;
	for (j = pCells->top; j < pCells->bottom; ++ j)
	{
		rcCell.bottom = rcCell.top + GetRowHeight(j);
		rcCell.left = pPos->left;
		for (i = pCells->left; i < pCells->right; ++ i)
		{
			rcCell.right = rcCell.left + GetColWidth(i);



			CMergeCell* pMerge = m_merge.GetMergeCell(i, j);
			if (pMerge)
			{
				if (pMerge->m_dwDrawTick < dwTick)
				{
					RECT rcMerge = pMerge->m_rect;
					rcMerge.left -= (m_nXExcursion - ptOrigin.x - 1);
					rcMerge.right -= (m_nXExcursion - ptOrigin.x);
					rcMerge.top -= (m_nYExcursion - ptOrigin.y - 1);
					rcMerge.bottom -= (m_nYExcursion - ptOrigin.y);

					FillRect(hDC, &rcMerge, m_hSheetBkgnd);

					DrawCell(hDC, rcMerge, pMerge->m_range.GetID().x,
						pMerge->m_range.GetID().y, dwTick);
					pMerge->m_dwDrawTick = dwTick;
					DrawSelectedBorder(hDC, rcMerge, i, j, pMerge);
				}
			}
			else
			{
				DrawFixCell(hDC, rcCell, i, j, dwTick);
			}


			rcCell.left = rcCell.right;
		}
		rcCell.top = rcCell.bottom;
	}
}

void CGrid::DrawBodyCells(HDC hDC, POINT ptOrigin, RECT* pCells, RECT* pPos,
	DWORD dwTick)
{
	if (pCells->right < pCells->left || pCells->bottom < pCells->top)
		return ;

	int i, j, n;
	// background
	//	FillRect(hDC, pPos, m_hSheetBkgnd);
	// lines
	SelectObject(hDC, m_hCellBorder);
	n = pPos->top;
	for (j = pCells->top; j <= pCells->bottom; ++ j)
	{
		n += GetRowHeight(j);
		MoveToEx(hDC, pPos->left, n, NULL);
		LineTo(hDC, pPos->right, n);
	}
	n = pPos->left;
	for (i = pCells->left; i <= pCells->right; ++ i)
	{
		n += GetColWidth(i);
		MoveToEx(hDC, n, pPos->top, NULL);
		LineTo(hDC, n, pPos->bottom);
	}

	// cells
	RECT rcCell, rcTmp;
	rcCell.top = pPos->top;
	for (j = pCells->top; j <= pCells->bottom; ++ j)
	{
		rcCell.bottom = rcCell.top + GetRowHeight(j);
		rcCell.left = pPos->left;
		for (i = pCells->left; i <= pCells->right; ++ i)
		{
			rcCell.right = rcCell.left + GetColWidth(i);


			CMergeCell* pMerge = m_merge.GetMergeCell(i, j);
			if (pMerge)
			{
				if (pMerge->m_dwDrawTick < dwTick)
				{
					RECT rcMerge = pMerge->m_rect;
					rcMerge.left -= (m_nXExcursion - ptOrigin.x - 1);
					rcMerge.right -= (m_nXExcursion - ptOrigin.x);
					rcMerge.top -= (m_nYExcursion - ptOrigin.y - 1);
					rcMerge.bottom -= (m_nYExcursion - ptOrigin.y);

					if (IsSelected(pMerge->m_range.GetID().x,
							pMerge->m_range.GetID().y))
					{
						FillRect(hDC, &rcMerge, m_hSelectBkgnd);
					}
					else
					{
						FillRect(hDC, &rcMerge, m_hSheetBkgnd);
					}

					DrawCell(hDC, rcMerge, pMerge->m_range.GetID().x,
						pMerge->m_range.GetID().y, dwTick);
					pMerge->m_dwDrawTick = dwTick;
					DrawSelectedBorder(hDC, rcMerge, i, j, pMerge);
				}
			}
			else
			{
				if (IsSelected(i, j))
				{
					rcTmp = rcCell;
					++ rcTmp.left;
					++ rcTmp.top;
					FillRect(hDC, &rcTmp, m_hSelectBkgnd);

					DrawSelectedBorder(hDC, rcCell, i, j);
				}
				DrawCell(hDC, rcCell, i, j, dwTick);
			}











			rcCell.left = rcCell.right;
		}
		rcCell.top = rcCell.bottom;
	}
}

void CGrid::GetDrawDetail(RECT* prcDraw, RECT* pTopHeader, RECT* pLeftHeader,
	RECT* pTopLeftHeader, RECT* pBody, RECT* pTopHeaderPos,
	RECT* pLeftHeaderPos, RECT* pTopLeftHeaderPos, RECT* pBodyPos)
{
	POINT ptHeaderPos;
	POINT ptBodyPos;
	POINT ptEnd;
	int nLeft, nRight, nTop, nBottom;
	nLeft = nRight = nTop = nBottom = -1;

	int i, n;

	n = 0;
	for (i = 0; i < m_nFixColCount; ++ i)
	{
		n += GetColWidth(i);
		if (n > prcDraw->left && nLeft == -1)
		{
			nLeft = i;
			ptHeaderPos.x = n - GetColWidth(i);
		}
		if (n > prcDraw->right && nRight == -1)
		{
			nRight = i;
			ptEnd.x = n;
			break;
		}
	}

	ptBodyPos.x = n;
	if (nRight == -1)
	{
		for (i = m_nFixColCount + m_cellIndex.x; i < m_nCols; ++ i)
		{
			n += GetColWidth(i);
			if (n > prcDraw->left && nLeft == -1)
			{
				nLeft = i;
			}
			if (n > prcDraw->right)
			{
				nRight = i;
				ptEnd.x = n;
				break;
			}
		}
	}
	if (nRight == -1)
	{
		nRight = i;
		ptEnd.x = n;
	}

	if (i == m_nCols)
	{
		m_nLastColPos = n;
	}
	else
	{
		m_nLastColPos = -1;
	}



	n = 0;
	for (i = 0; i < m_nFixRowCount; ++ i)
	{
		n += GetRowHeight(i);
		if (n > prcDraw->top && nTop == -1)
		{
			nTop = i;
			ptHeaderPos.y = n - GetRowHeight(i);
		}
		if (n > prcDraw->bottom && nBottom == -1)
		{
			nBottom = i;
			ptEnd.y = n;
			break;
		}
	}

	ptBodyPos.y = n;
	if (nBottom == -1)
	{
		for (i = m_nFixRowCount + m_cellIndex.y; i < m_nRows; ++ i)
		{
			n += GetRowHeight(i);
			if (n > prcDraw->top && nTop == -1)
			{
				nTop = i;
			}
			if (n > prcDraw->bottom)
			{
				nBottom = i;
				ptEnd.y = n;
				break;
			}
		}
	}
	if (nBottom == -1)
	{
		nBottom = i;
		ptEnd.y = n;
	}

	if (i == m_nRows)
	{
		m_nLastRowPos = n;
	}
	else
	{
		m_nLastRowPos = -1;
	}

	SetRect(pTopLeftHeaderPos, ptHeaderPos.x, ptHeaderPos.y, ptBodyPos.x,
		ptBodyPos.y);
	SetRect(pTopHeaderPos, ptBodyPos.x, ptHeaderPos.y, ptEnd.x, ptBodyPos.y);
	SetRect(pLeftHeaderPos, ptHeaderPos.x, ptBodyPos.y, ptBodyPos.x, ptEnd.y);
	SetRect(pBodyPos, ptBodyPos.x, ptBodyPos.y, ptEnd.x, ptEnd.y);


	if (nTop >= m_nFixRowCount)
		SetRect(pTopHeader, 0, 0, 0, 0);
	else
		SetRect(pTopHeader,
			nLeft <
			m_nFixColCount +
			m_cellIndex.x ?
			m_nFixColCount +
			m_cellIndex.x :
			nLeft,
			nTop, nRight, m_nFixRowCount);

	if (nLeft >= m_nFixColCount)
		SetRect(pLeftHeader, 0, 0, 0, 0);
	else
		SetRect(pLeftHeader, nLeft,
			nTop <
			m_nFixRowCount +
			m_cellIndex.y ?
			m_nFixRowCount +
			m_cellIndex.y :
			nTop,
			m_nFixColCount, nBottom);


	SetRect(pTopLeftHeader, nLeft, nTop,
		nRight < m_nFixColCount ? nRight : m_nFixColCount, 
		nBottom < m_nFixRowCount ? nBottom : m_nFixRowCount);

	SetRect(pBody,
		nLeft <
		m_nFixColCount +
		m_cellIndex.x ?
		m_nFixColCount +
		m_cellIndex.x :
		nLeft,
		nTop <
		m_nFixRowCount +
		m_cellIndex.y ?
		m_nFixRowCount +
		m_cellIndex.y :
		nTop,
		nRight, nBottom);
}

void CGrid::DrawSelectedBorder(HDC hdc, RECT& rc, int x, int y,
	CMergeCell* pMerge)
{
	return ;

	if (m_selectRgn.GetSubRectCount() == 1)
	{
		HPEN hOldPen = (HPEN) SelectObject(hdc, m_hSelectBorder);
		if (x == m_selectRgn.GetSubRect(0)->Left())
		{
			MoveToEx(hdc, rc.left, rc.top, NULL);
			LineTo(hdc, rc.left, rc.bottom);
		}

		if (pMerge)
		{
			if (x +
				pMerge->m_range.GetCX() ==
				m_selectRgn.GetSubRect(0)->Right())
			{
				MoveToEx(hdc, rc.right, rc.top, NULL);
				LineTo(hdc, rc.right, rc.bottom);
			}
		}
		else
		{
			if (x == m_selectRgn.GetSubRect(0)->Right() - 1)
			{
				MoveToEx(hdc, rc.right, rc.top, NULL);
				LineTo(hdc, rc.right, rc.bottom);
			}
		}

		if (y == m_selectRgn.GetSubRect(0)->Top())
		{
			MoveToEx(hdc, rc.left, rc.top, NULL);
			LineTo(hdc, rc.right, rc.top);
		}

		if (y == m_selectRgn.GetSubRect(0)->Bottom() - 1)
		{
			MoveToEx(hdc, rc.left, rc.bottom, NULL);
			LineTo(hdc, rc.right, rc.bottom);
		}

		SelectObject(hdc, hOldPen);
	}
}

POINT CGrid::GetOriginPos()
{
	int i;
	POINT pt =
	{
		0, 0
	};
	for (i = 0; i < m_nFixRowCount; ++ i)
		pt.y += GetRowHeight(i);
	for (i = 0; i < m_nFixColCount; ++ i)
		pt.x += GetColWidth(i);

	return pt;
}

void CGrid::DrawCell(HDC hdc, RECT& rc, int x, int y, DWORD dwTick)
{
	if (m_idFocus.x == x && m_idFocus.y == y)
	{
		RECT rcTmp = rc;
		InflateRect(&rcTmp, -2, -2);
		HPEN hOldPen = (HPEN) SelectObject(hdc, m_hFocusInSideBorder);

		MoveToEx(hdc, rcTmp.left, rcTmp.top, NULL);
		LineTo(hdc, rcTmp.right, rcTmp.top);
		LineTo(hdc, rcTmp.right, rcTmp.bottom);
		LineTo(hdc, rcTmp.left, rcTmp.bottom);
		LineTo(hdc, rcTmp.left, rcTmp.top);
		SelectObject(hdc, hOldPen);

//		POINT pt=GetOriginPos();
//		RECT rt=GetCellRect(x, y);
	}
	//
	/*
		if(m_idLastSelect.x == x && m_idLastSelect.y == y) {
			RECT rcTmp = rc;
			InflateRect(&rcTmp, -2, -2);
			HPEN hOldPen = (HPEN)SelectObject(hdc, m_hFocusInSideBorder);
			MoveToEx(hdc, rcTmp.left, rcTmp.top, NULL);
			LineTo(hdc, rcTmp.right, rcTmp.bottom);
			MoveToEx(hdc, rcTmp.right, rcTmp.top, NULL);
			LineTo(hdc, rcTmp.left, rcTmp.bottom);
			SelectObject(hdc, hOldPen);
		}
		if(m_idClickDown.x == x && m_idClickDown.y == y) {
			RECT rcTmp = rc;
			InflateRect(&rcTmp, -2, -2);
			HPEN hOldPen = (HPEN)SelectObject(hdc, m_hFocusInSideBorder);
			MoveToEx(hdc, rcTmp.left, rcTmp.top + (rcTmp.bottom - rcTmp.top) / 2, NULL);
			LineTo(hdc, rcTmp.right, rcTmp.top + (rcTmp.bottom - rcTmp.top) / 2);
			MoveToEx(hdc, rcTmp.left + (rcTmp.right - rcTmp.left) / 2, rcTmp.top, NULL);
			LineTo(hdc, rcTmp.left + (rcTmp.right - rcTmp.left) / 2, rcTmp.bottom);
			SelectObject(hdc, hOldPen);
		}
	*/
	//
	/*
	
		_stprintf(sz, _TEXT("%s"), GetCellText(id));
	*/

	if (x < m_nFixColCount || y < m_nFixRowCount)
	{
		TCHAR sz[32];
		//tostringstream ostr;
		//tstring sz;
		CellID id={x,y};
		if(GetCellText(id))
		{
			//ostr<<GetCellText(id);
			//sz = ostr.str();
			//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 32, _TEXT("%s"), GetCellText(id));
#else
			_stprintf(sz, _TEXT("%s"), GetCellText(id));
#endif
			
			DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			return ;
		}
		else if(x < m_nFixColCount && y < m_nFixRowCount)
		{
			if(m_bPreferColumn)
			{
				//ostr<<y;
				//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
				_stprintf_s(sz, 32, _TEXT("%d"), y);
#else
				_stprintf(sz, _TEXT("%d"), y);
#endif
				
			}
			else
			{
				//ostr<<x;
				//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
				_stprintf_s(sz, 32, _TEXT("%d"), x);
#else
				_stprintf(sz, _TEXT("%d"), x);
#endif
				
			}
			DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			return ;
		}
		else if (x < m_nFixColCount)
		{
			//ostr<<y;
			//sz = ostr.str();
			//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 32, _TEXT("%d"), y);
#else
			_stprintf(sz, _TEXT("%d"), y);
#endif
			
			DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			return ;
		}
		else if (y < m_nFixRowCount)
		{
			//ostr<<x;
			//sz = ostr.str();
			//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 32, _TEXT("%d"), x);
#else
			_stprintf(sz, _TEXT("%d"), x);
#endif
			
			DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			return ;
		}
	}


	CGridCell* pVariant = OnGetElement(x, y);
	//VT_TYPE type = m_bPreferColumn?GetColumnType(x):GetRowType(y);
	VT_TYPE type = GetCellType(x, y);
	if (pVariant)
	{
		if (MYVT_BOOL == type)
		{
			CGridCheckBox::Draw(hdc, rc, (bool) (*(CMyVariant *) pVariant));
		}
		else
			pVariant->Draw(hdc, rc, x, y, dwTick);
	}
	else
	{
		if (MYVT_BOOL == type)
		{
			CGridCheckBox::Draw(hdc, rc, false);
		}
	}
}

void CGrid::DrawFixCell(HDC hdc, RECT& rc, int x, int y, DWORD dwTick)
{
	HPEN hPen = (HPEN) GetStockObject(WHITE_PEN);
	HPEN hOldPen = (HPEN) SelectObject(hdc, hPen);

	if (x < m_nFixColCount)
	{
		MoveToEx(hdc, rc.left + 1, rc.top + 1, NULL);
		LineTo(hdc, rc.right - 1, rc.top + 1);
	}
	if (y < m_nFixRowCount)
	{
		MoveToEx(hdc, rc.left + 1, rc.top + 1, NULL);
		LineTo(hdc, rc.left + 1, rc.bottom - 1);
	}

	DrawCell(hdc, rc, x, y, dwTick);

	SelectObject(hdc, hOldPen);
}

void CGrid::DrawSheetBorder(HDC hdc, RECT& rc)
{
	RECT rcBlank;
	if (m_nLastRowPos != -1 && m_nLastColPos == -1)
	{
		SetRect(&rcBlank, 0, m_nLastRowPos + 1, rc.right, rc.bottom);
		FillRect(hdc, &rcBlank, m_hOutSheetBkgnd);

		SelectObject(hdc, m_hSheetBorder);
		MoveToEx(hdc, 0, m_nLastRowPos + 1, NULL);
		LineTo(hdc, rc.right, m_nLastRowPos + 1);
	}
	else if (m_nLastRowPos == -1 && m_nLastColPos != -1)
	{
		SetRect(&rcBlank, m_nLastColPos + 1, 0, rc.right, rc.bottom);
		FillRect(hdc, &rcBlank, m_hOutSheetBkgnd);

		SelectObject(hdc, m_hSheetBorder);
		MoveToEx(hdc, m_nLastColPos + 1, 0, NULL);
		LineTo(hdc, m_nLastColPos + 1, rc.bottom);
	}
	else if (m_nLastRowPos != -1 && m_nLastColPos != -1)
	{
		SetRect(&rcBlank, 0, m_nLastRowPos + 1, rc.right, rc.bottom);
		FillRect(hdc, &rcBlank, m_hOutSheetBkgnd);

		SetRect(&rcBlank, m_nLastColPos + 1, 0, rc.right, m_nLastRowPos + 1);
		FillRect(hdc, &rcBlank, m_hOutSheetBkgnd);


		SelectObject(hdc, m_hSheetBorder);

		MoveToEx(hdc, 0, m_nLastRowPos + 1, NULL);
		LineTo(hdc, m_nLastColPos, m_nLastRowPos + 1);

		MoveToEx(hdc, m_nLastColPos + 1, 0, NULL);
		LineTo(hdc, m_nLastColPos + 1, m_nLastRowPos);
	}
}

void CGrid::CalcVInfo(int nHeight)
{
	int nTmp = 0;
	int i;
	for (i = m_nRows - 1; i >= m_nFixRowCount; -- i)
	{
		nTmp += GetRowHeight(i);
		if (nTmp >= nHeight)
		{
			++ i;
			break;
		}
	}

	m_nLastRow = i - m_nFixRowCount;
	if (m_nLastRow < 0)
		m_nLastRow = 0;
}

void CGrid::CalcHInfo(int nWidth)
{
	int nTmp = 0;
	int i;
	for (i = m_nCols - 1; i >= m_nFixColCount; -- i)
	{
		nTmp += GetColWidth(i);
		if (nTmp >= nWidth)
		{
			++ i;
			break;
		}
	}

	m_nLastCol = i - m_nFixColCount;
	if (m_nLastCol < 0)
		m_nLastCol = 0;
}

void CGrid::SplitCell(int x, int y)
{
	m_merge.SplitCell(x, y);
}

CMergeCell* CGrid::GetMergeInfo(int x, int y)
{
	return m_merge.GetMergeCell(x, y);
}

void CGrid::MergeCells(int x, int y, int cx, int cy)
{
	if (cx == 1 && cy == 1)
		return ;
	RECT rcPos;
	GetCellsRect(x, y, x + cx, y + cy, &rcPos);
	m_merge.MergeCells(x, y, cx, cy, &rcPos);
}

CellID CGrid::CellFromPoint(POINT pt)
{
	CellID id;
	POINT origin = GetOriginPos();

	int n;
	if (pt.x < origin.x)
	{
		n = 0;
		for (int i = 0; i < m_nFixColCount; ++ i)
		{
			n += GetColWidth(i);
			if (n > pt.x)
			{
				id.x = i;
				break;
			}
		}
	}
	else
	{
		n = origin.x;
		for (int i = m_cellIndex.x + m_nFixColCount; ; ++ i)
		{
			if (i >= m_nCols)
			{
				id.x = i - 1;
				break;
			}
			else
			{
				n += GetColWidth(i);
				if (n > pt.x)
				{
					id.x = i;
					break;
				}
			}
		}
	}

	if (pt.y < origin.y)
	{
		n = 0;
		for (int i = 0; i < m_nFixRowCount; ++ i)
		{
			n += GetRowHeight(i);
			if (n > pt.y)
			{
				id.y = i;
				break;
			}
		}
	}
	else
	{
		n = origin.y;
		for (int i = m_cellIndex.y + m_nFixRowCount; ; ++ i)
		{
			if (i >= m_nRows)
			{
				id.y = i - 1;
				break;
			}
			else
			{
				n += GetRowHeight(i);
				if (n > pt.y)
				{
					id.y = i;
					break;
				}
			}
		}
	}

	return id;
}

int CGrid::GetBodyCellLeft(int x, int nStart)
{
	int n = 0;
	for (int i = (nStart == 0 ? m_nFixColCount : nStart); i < x; ++ i)
	{
		n += GetColWidth(i);
	}
	return n;
}

int CGrid::GetBodyCellTop(int y, int nStart)
{
	int n = 0;
	for (int i = (nStart == 0 ? m_nFixRowCount : nStart); i < y; ++ i)
	{
		n += GetRowHeight(i);
	}
	return n;
}

DWORD CGrid::GetNowTick()
{
	static DWORD dwOld = 0;
	DWORD dwTick = GetTickCount();
	if (dwOld < dwTick)
	{
		dwOld = dwTick;
	}
	else
	{
		++ dwOld;
	}
	return dwOld;
}

CRectRgn* CGrid::GetSelect()
{
	return &m_selectRgn;
}

bool CGrid::IsSelected(int x, int y)
{
	//	return true;
	return m_selectRgn.PtInRgn(x, y);
}

void CGrid::GetCellsRect(int l, int t, int r, int b, RECT* pRect)
{
	int left, top, right, bottom;
	left = GetBodyCellLeft(l);
	top = GetBodyCellTop(t);
	right = left + GetBodyCellLeft(r, l);
	bottom = top + GetBodyCellTop(b, t);
	SetRect(pRect, left, top, right, bottom);
}

void CGrid::DrawResizeLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	int nOldRop = SetROP2(hdc, R2_MERGEPENNOT);
	HPEN hOldPen = (HPEN) SelectObject(hdc, m_hResizePen);

	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);

	SelectObject(hdc, hOldPen);
	SetROP2(hdc, nOldRop);
}

void CGrid::SetResizeType(AllowResizeType type)
{
	m_allowRisizeType = type;
}

void CGrid::SetTopIndex(int y)
{
	if (y >= m_nFixRowCount &&
		y <= m_nLastRow + m_nFixRowCount &&
		m_cellIndex.y != y - m_nFixRowCount)
	{
		y -= m_nFixRowCount;
		m_cellIndex.y = y;
		::SetScrollPos(m_hWnd, SB_VERT, y, TRUE);
		m_nYExcursion = GetBodyCellTop(y + m_nFixRowCount);
	}
}

void CGrid::SetLeftIndex(int x)
{
	if (x >= m_nFixColCount &&
		x <= m_nLastCol + m_nFixColCount &&
		m_cellIndex.x != x - m_nFixColCount)
	{
		x -= m_nFixColCount;
		m_cellIndex.x = x;
		::SetScrollPos(m_hWnd, SB_HORZ, x, TRUE);
		m_nXExcursion = GetBodyCellLeft(x + m_nFixColCount);
	}
}

void CGrid::OnUpKey(int nCtrl, int nShift)
{
	CMyRect rcSelect;
	CellID id;
	CCellRange range;
	if (nCtrl && !nShift)
	{
		m_idFocus.y = m_nFixRowCount;
		m_cellIndex.y = 0;
		m_idLastSelect = m_idClickDown = m_idFocus;
		m_selectRgn.Init(m_idFocus.x, m_idFocus.y, 1, 1);

		range.Set(m_idFocus.x, m_idFocus.y, 1, 1);
		OnSelectChanged(range);

		m_nYExcursion = 0;

		InvalidateRect(m_hWnd, NULL, TRUE);
		::SetScrollPos(m_hWnd, SB_VERT, 0, TRUE);
	}
	else if (!nCtrl && nShift)
	{
		if (m_idClickDown.x < m_nFixColCount &&
			m_idClickDown.y >= m_nFixRowCount)
		{
			// click in left header
			if (m_idLastSelect.y > m_nFixRowCount)
			{
				-- m_idLastSelect.y;
				id = m_idLastSelect;
				RECT_TYPE t, h;
				t = min(id.y, m_idFocus.y);
				h = max(id.y, m_idFocus.y) - t + 1;
				m_selectRgn.Init(m_nFixColCount, t, m_nCols - m_nFixColCount,
								h);

				range.Set(m_nFixColCount, t, m_nCols - m_nFixColCount, h);
				OnSelectChanged(range);

				if (t < m_cellIndex.y + m_nFixRowCount)
				{
					::SetScrollPos(m_hWnd, SB_VERT, -- m_cellIndex.y, TRUE);
					m_nYExcursion -= GetRowHeight(m_cellIndex.y +
										m_nFixRowCount);
				}
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
		else if (m_idClickDown.x >= m_nFixColCount &&
			m_idClickDown.y < m_nFixRowCount)
		{
			// click in top header
		}
		else if (m_idClickDown.x < m_nFixColCount &&
			m_idClickDown.y < m_nFixRowCount)
		{
			// click in top left header
		}
		else
		{
			// click in body
			if (m_idLastSelect.y > m_nFixRowCount)
			{
				-- m_idLastSelect.y;
				id = m_idLastSelect;

				/*				CMergeCell* pMerge = m_merge.GetMergeCell(id.x, id.y);
										if(pMerge) {
											id = pMerge->m_range.GetID();
											CellID idLeftTop, idRightBottom;
											idLeftTop.x = min(id.x, m_idClickDown.x);
											idLeftTop.y = id.y;
											idRightBottom.x = max(pMerge->m_range.GetRight() - 1, m_idLastSelect.x);
											idRightBottom.y = m_idClickDown.y;
											m_idClickDown.x = idRightBottom.x;
											m_idClickDown.y = idRightBottom.y;
											m_idLastSelect.x = idLeftTop.x;
											m_idLastSelect.y = idLeftTop.y;
										}*/

				RECT_TYPE l, t, w, h;
				l = min(m_idClickDown.x, m_idLastSelect.x);
				w = max(m_idClickDown.x, m_idLastSelect.x) - l + 1;
				t = min(m_idClickDown.y, m_idLastSelect.y);
				h = max(m_idClickDown.y, m_idLastSelect.y) - t + 1;

				rcSelect.Set(l, t, w, h);
				m_selectRgn.Init(&rcSelect);

				range.Set(l, t, w, h);
				OnSelectChanged(range);

				if (m_idLastSelect.y <m_cellIndex.y + m_nFixRowCount &&
					m_cellIndex.y> 0)
				{
					::SetScrollPos(m_hWnd, SB_VERT, -- m_cellIndex.y, TRUE);
					m_nYExcursion -= GetRowHeight(m_cellIndex.y +
										m_nFixRowCount);
				}

				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
	}
	else if (nCtrl && nShift)
	{
	}
	else
	{
		id = m_idFocus;
		-- id.y;
		if (id.y < m_nFixRowCount)
			id.y = m_nFixRowCount;

		CMergeCell* pMerge = m_merge.GetMergeCell(id.x, id.y);
		if (pMerge)
		{
			id = pMerge->m_range.GetID();
		}

		m_idLastSelect = m_idFocus = m_idClickDown = id;
		m_selectRgn.Init(id.x, id.y, 1, 1);

		range.Set(id.x, id.y, 1, 1);
		OnSelectChanged(range);

		if (id.y <m_nFixRowCount + m_cellIndex.y - 1 ||
			id.y> m_nFixRowCount + m_cellIndex.y + m_nDrawedBodyRows)
		{
			if (id.y > m_nLastRow + m_nFixRowCount)
				id.y = m_nLastRow + m_nFixRowCount;
			SetTopIndex(id.y);
		}
		else
		{
			if (m_cellIndex.y > 0 && m_cellIndex.y + m_nFixRowCount > id.y)
			{
				::SetScrollPos(m_hWnd, SB_VERT, -- m_cellIndex.y, TRUE);
				m_nYExcursion -= GetRowHeight(m_cellIndex.y + m_nFixRowCount);
			}
		}
		InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void CGrid::OnLeftKey(int nCtrl, int nShift)
{
	CMyRect rcSelect;
	CellID id;
	CCellRange range;
	if (nCtrl && !nShift)
	{
		m_idFocus.x = m_nFixColCount;
		m_cellIndex.x = 0;
		m_idLastSelect = m_idClickDown = m_idFocus;
		m_selectRgn.Init(m_idFocus.x, m_idFocus.y, 1, 1);

		range.Set(m_idFocus.x, m_idFocus.y, 1, 1);
		OnSelectChanged(range);

		m_nXExcursion = 0;
		InvalidateRect(m_hWnd, NULL, TRUE);
		::SetScrollPos(m_hWnd, SB_HORZ, 0, TRUE);
	}
	else if (!nCtrl && nShift)
	{
		if (m_idClickDown.x < m_nFixColCount &&
			m_idClickDown.y >= m_nFixRowCount)
		{
			// click in left header
		}
		else if (m_idClickDown.x >= m_nFixColCount &&
			m_idClickDown.y < m_nFixRowCount)
		{
			// click in top header
			if (m_idLastSelect.x > m_nFixColCount)
			{
				-- m_idLastSelect.x;
				id = m_idLastSelect;
				RECT_TYPE l, w;
				l = min(id.x, m_idFocus.x);
				w = max(id.x, m_idFocus.x) - l + 1;
				m_selectRgn.Init(l, m_nFixRowCount, w,
								m_nRows - m_nFixRowCount);

				range.Set(l, m_nFixRowCount, w, m_nRows - m_nFixRowCount);
				OnSelectChanged(range);

				if (l < m_cellIndex.x + m_nFixColCount)
				{
					::SetScrollPos(m_hWnd, SB_HORZ, -- m_cellIndex.x, TRUE);
					m_nXExcursion -= GetColWidth(m_cellIndex.x +
										m_nFixColCount);
				}
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
		else if (m_idClickDown.x < m_nFixColCount &&
			m_idClickDown.y < m_nFixRowCount)
		{
			// click in top left header
		}
		else
		{
			// click in body
			if (m_idLastSelect.x > m_nFixColCount)
			{
				-- m_idLastSelect.x;
				id = m_idLastSelect;

				/*				CMergeCell* pMerge = m_merge.GetMergeCell(id.x, id.y);
										if(pMerge) {
											id = pMerge->m_range.GetID();
											CellID idLeftTop, idRightBottom;
											idLeftTop.x = id.x;
											idLeftTop.y = min(id.y, m_idClickDown.y);
											idRightBottom.x = m_idClickDown.x;
											idRightBottom.y = max(pMerge->m_range.GetBottom() - 1, m_idLastSelect.y);
											m_idClickDown.x = idRightBottom.x;
											m_idClickDown.y = idRightBottom.y;
											m_idLastSelect.x = idLeftTop.x;
											m_idLastSelect.y = idLeftTop.y;
										}*/


				RECT_TYPE l, t, w, h;
				l = min(m_idClickDown.x, m_idLastSelect.x);
				w = max(m_idClickDown.x, m_idLastSelect.x) - l + 1;
				t = min(m_idClickDown.y, m_idLastSelect.y);
				h = max(m_idClickDown.y, m_idLastSelect.y) - t + 1;

				rcSelect.Set(l, t, w, h);
				m_selectRgn.Init(&rcSelect);

				range.Set(l, t, w, h);
				OnSelectChanged(range);

				if (m_idLastSelect.x <m_cellIndex.x + m_nFixColCount &&
					m_cellIndex.x> 0)
				{
					::SetScrollPos(m_hWnd, SB_HORZ, -- m_cellIndex.x, TRUE);
					m_nXExcursion -= GetColWidth(m_cellIndex.x +
										m_nFixColCount);
				}

				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
	}
	else if (nCtrl && nShift)
	{
	}
	else
	{
		id = m_idFocus;
		-- id.x;
		if (id.x < m_nFixColCount)
			id.x = m_nFixColCount;

		CMergeCell* pMerge = m_merge.GetMergeCell(id.x, id.y);
		if (pMerge)
		{
			id = pMerge->m_range.GetID();
		}

		m_idLastSelect = m_idFocus = m_idClickDown = id;
		m_selectRgn.Init(id.x, id.y, 1, 1);

		range.Set(id.x, id.y, 1, 1);
		OnSelectChanged(range);

		if (id.x <m_nFixColCount + m_cellIndex.x - 1 ||
			id.x> m_nFixColCount + m_cellIndex.x + m_nDrawedBodyCols)
		{
			if (id.x > m_nLastCol + m_nFixColCount)
				id.x = m_nLastCol + m_nFixColCount;
			SetLeftIndex(id.x);
		}
		else
		{
			if (m_cellIndex.x > 0 && m_cellIndex.x + m_nFixColCount > id.x)
			{
				::SetScrollPos(m_hWnd, SB_HORZ, -- m_cellIndex.x, TRUE);
				m_nXExcursion -= GetColWidth(m_cellIndex.x + m_nFixColCount);
			}
		}

		InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void CGrid::OnRightKey(int nCtrl, int nShift)
{
	CMyRect rcSelect;
	CellID id;
	CCellRange range;
	if (nCtrl && !nShift)
	{
		m_nXExcursion += GetBodyCellLeft(m_nLastCol + m_nFixColCount,
							m_nFixColCount + m_cellIndex.x);

		m_idFocus.x = m_nCols - 1;
		m_cellIndex.x = m_nLastCol;
		m_idLastSelect = m_idClickDown = m_idFocus;
		m_selectRgn.Init(m_idFocus.x, m_idFocus.y, 1, 1);

		range.Set(m_idFocus.x, m_idFocus.y, 1, 1);
		OnSelectChanged(range);

		InvalidateRect(m_hWnd, NULL, TRUE);
		::SetScrollPos(m_hWnd, SB_HORZ, m_nLastCol, TRUE);
	}
	else if (!nCtrl && nShift)
	{
		if (m_idClickDown.x < m_nFixColCount &&
			m_idClickDown.y >= m_nFixRowCount)
		{
			// click in left header
		}
		else if (m_idClickDown.x >= m_nFixColCount &&
			m_idClickDown.y < m_nFixRowCount)
		{
			// click in top header
			if (m_idLastSelect.x < m_nCols - 1)
			{
				++ m_idLastSelect.x;
				id = m_idLastSelect;
				RECT_TYPE l, w;
				l = min(id.x, m_idFocus.x);
				w = max(id.x, m_idFocus.x) - l + 1;
				m_selectRgn.Init(l, m_nFixRowCount, w,
								m_nRows - m_nFixRowCount);

				range.Set(l, m_nFixRowCount, w, m_nRows - m_nFixRowCount);
				OnSelectChanged(range);

				if (m_idLastSelect.x >
					m_nFixColCount +
					m_cellIndex.x +
					m_nDrawedBodyCols -
					1)
				{
					m_nXExcursion += GetColWidth(m_cellIndex.x +
										m_nFixColCount);
					::SetScrollPos(m_hWnd, SB_HORZ, ++ m_cellIndex.x, TRUE);
				}
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
		else if (m_idClickDown.x < m_nFixColCount &&
			m_idClickDown.y < m_nFixRowCount)
		{
			// click in top left header
		}
		else
		{
			// click in body
			if (m_idLastSelect.x < m_nCols - 1)
			{
				CMergeCell* pMerge = m_merge.GetMergeCell(m_idLastSelect.x,
												m_idLastSelect.y);
				if (pMerge)
				{
					m_idLastSelect.x = pMerge->m_range.GetRight();
				}
				else
				{
					++ m_idLastSelect.x;
				}
				id = m_idLastSelect;

				/*				pMerge = m_merge.GetMergeCell(id.x, id.y);
										if(pMerge) {
											CellID idLeftTop, idRightBottom;
											idLeftTop.x = m_idClickDown.x;
											idLeftTop.y = min(pMerge->m_range.GetID().y, m_idClickDown.y);
											idRightBottom.x = pMerge->m_range.GetRight() - 1;
											idRightBottom.y = max(pMerge->m_range.GetBottom() - 1, m_idLastSelect.y);
											
											m_idClickDown.x = idLeftTop.x;
											m_idClickDown.y = idRightBottom.y;
											m_idLastSelect = pMerge->m_range.GetID();
										}*/


				RECT_TYPE l, t, w, h;
				l = min(m_idClickDown.x, m_idLastSelect.x);
				w = max(m_idClickDown.x, m_idLastSelect.x) - l + 1;
				t = min(m_idClickDown.y, m_idLastSelect.y);
				h = max(m_idClickDown.y, m_idLastSelect.y) - t + 1;

				rcSelect.Set(l, t, w, h);
				m_selectRgn.Init(&rcSelect);

				range.Set(l, t, w, h);
				OnSelectChanged(range);

				if (m_idLastSelect.x >
					m_nFixColCount +
					m_cellIndex.x +
					m_nDrawedBodyCols -
					1)
				{
					m_nXExcursion += GetColWidth(m_cellIndex.x +
										m_nFixColCount);
					::SetScrollPos(m_hWnd, SB_HORZ, ++ m_cellIndex.x, TRUE);
				}

				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
	}
	else if (nCtrl && nShift)
	{
	}
	else
	{
		id = m_idFocus;

		CMergeCell* pMerge = m_merge.GetMergeCell(id.x, id.y);
		if (pMerge)
		{
			id.x = pMerge->m_range.GetRight();
		}
		else
		{
			++ id.x;
		}
		pMerge = m_merge.GetMergeCell(id.x, id.y);
		if (pMerge)
		{
			id = pMerge->m_range.GetID();
		}

		if (id.x >= m_nCols)
			id.x = m_nCols - 1;
		m_idLastSelect = m_idFocus = m_idClickDown = id;
		m_selectRgn.Init(id.x, id.y, 1, 1);

		range.Set(id.x, id.y, 1, 1);
		OnSelectChanged(range);

		if (id.x <m_nFixColCount + m_cellIndex.x + 1 ||
			id.x> m_nFixColCount + m_cellIndex.x + m_nDrawedBodyCols)
		{
			if (id.x > m_nLastCol + m_nFixColCount)
				id.x = m_nLastCol + m_nFixColCount;
			SetLeftIndex(id.x);
		}
		else
		{
			if (m_cellIndex.x < m_nLastCol)
			{
				if (m_idLastSelect.x >
					m_nFixColCount +
					m_cellIndex.x +
					m_nDrawedBodyCols -
					1)
				{
					m_nXExcursion += GetColWidth(m_cellIndex.x +
										m_nFixColCount);
					::SetScrollPos(m_hWnd, SB_HORZ, ++ m_cellIndex.x, TRUE);
				}
			}
		}

		InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void CGrid::OnDownKey(int nCtrl, int nShift)
{
	CMyRect rcSelect;
	CellID id;
	CCellRange range;
	if (nCtrl && !nShift)
	{
		m_nYExcursion += GetBodyCellTop(m_nLastRow + m_nFixRowCount,
							m_nFixRowCount + m_cellIndex.y);

		m_idFocus.y = m_nRows - 1;
		m_cellIndex.y = m_nLastRow;
		m_idLastSelect = m_idClickDown = m_idFocus;
		m_selectRgn.Init(m_idFocus.x, m_idFocus.y, 1, 1);

		range.Set(m_idFocus.x, m_idFocus.y, 1, 1);
		OnSelectChanged(range);

		InvalidateRect(m_hWnd, NULL, TRUE);
		::SetScrollPos(m_hWnd, SB_VERT, m_nLastRow, TRUE);
	}
	else if (!nCtrl && nShift)
	{
		if (m_idClickDown.x < m_nFixColCount &&
			m_idClickDown.y >= m_nFixRowCount)
		{
			// click in left header
			if (m_idLastSelect.y < m_nRows - 1)
			{
				++ m_idLastSelect.y;
				id = m_idLastSelect;
				RECT_TYPE t, h;
				t = min(id.y, m_idFocus.y);
				h = max(id.y, m_idFocus.y) - t + 1;
				m_selectRgn.Init(m_nFixColCount, t, m_nCols - m_nFixColCount,
								h);

				range.Set(m_nFixColCount, t, m_nCols - m_nFixColCount, h);
				OnSelectChanged(range);

				if (m_idLastSelect.y >
					m_nFixRowCount +
					m_cellIndex.y +
					m_nDrawedBodyRows -
					1)
				{
					m_nYExcursion += GetRowHeight(m_cellIndex.y +
										m_nFixRowCount);
					::SetScrollPos(m_hWnd, SB_VERT, ++ m_cellIndex.y, TRUE);
				}
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
		else if (m_idClickDown.x >= m_nFixColCount &&
			m_idClickDown.y < m_nFixRowCount)
		{
			// click in top header
		}
		else if (m_idClickDown.x < m_nFixColCount &&
			m_idClickDown.y < m_nFixRowCount)
		{
			// click in top left header
		}
		else
		{
			// click in body
			if (m_idLastSelect.y < m_nRows - 1)
			{
				CMergeCell* pMerge = m_merge.GetMergeCell(m_idLastSelect.x,
												m_idLastSelect.y);
				if (pMerge)
				{
					m_idLastSelect.y = pMerge->m_range.GetBottom();
				}
				else
				{
					++ m_idLastSelect.y;
				}
				id = m_idLastSelect;

				/*				pMerge = m_merge.GetMergeCell(id.x, id.y);
										if(pMerge) {
											CellID idLeftTop, idRightBottom;
											idLeftTop.x = min(pMerge->m_range.GetID().x, m_idClickDown.x);
											idLeftTop.y = m_idClickDown.y;
											idRightBottom.x = max(pMerge->m_range.GetRight() - 1, m_idLastSelect.x);
											idRightBottom.y = pMerge->m_range.GetBottom() - 1;
											
											m_idClickDown = idLeftTop;
											m_idLastSelect = idRightBottom;
										}*/

				RECT_TYPE l, t, w, h;
				l = min(m_idClickDown.x, m_idLastSelect.x);
				w = max(m_idClickDown.x, m_idLastSelect.x) - l + 1;
				t = min(m_idClickDown.y, m_idLastSelect.y);
				h = max(m_idClickDown.y, m_idLastSelect.y) - t + 1;

				rcSelect.Set(l, t, w, h);
				m_selectRgn.Init(&rcSelect);

				range.Set(l, t, w, h);
				OnSelectChanged(range);

				if (m_idLastSelect.y >
					m_nFixRowCount +
					m_cellIndex.y +
					m_nDrawedBodyRows -
					1)
				{
					m_nYExcursion += GetRowHeight(m_cellIndex.y +
										m_nFixRowCount);
					::SetScrollPos(m_hWnd, SB_VERT, ++ m_cellIndex.y, TRUE);
				}

				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
	}
	else if (nCtrl && nShift)
	{
	}
	else
	{
		id = m_idFocus;

		CMergeCell* pMerge = m_merge.GetMergeCell(id);
		if (pMerge)
		{
			id.y = pMerge->m_range.GetBottom();
		}
		else
		{
			++ id.y;
		}
		pMerge = m_merge.GetMergeCell(id.x, id.y);
		if (pMerge)
		{
			id = pMerge->m_range.GetID();
		}

		if (id.y >= m_nRows)
			id.y = m_nRows - 1;
		m_idLastSelect = m_idFocus = m_idClickDown = id;
		m_selectRgn.Init(id.x, id.y, 1, 1);

		range.Set(id.x, id.y, 1, 1);
		OnSelectChanged(range);

		if (id.y <m_nFixRowCount + m_cellIndex.y + 1 ||
			id.y> m_nFixRowCount + m_cellIndex.y + m_nDrawedBodyRows)
		{
			if (id.y > m_nLastRow + m_nFixRowCount)
				id.y = m_nLastRow + m_nFixRowCount;
			SetTopIndex(id.y);
		}
		else
		{
			if (m_cellIndex.y < m_nLastRow)
			{
				if (m_idLastSelect.y >
					m_nFixRowCount +
					m_cellIndex.y +
					m_nDrawedBodyRows -
					1)
				{
					m_nYExcursion += GetRowHeight(m_cellIndex.y +
										m_nFixRowCount);
					::SetScrollPos(m_hWnd, SB_VERT, ++ m_cellIndex.y, TRUE);
				}
			}
		}

		InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void CGrid::OnDelKey(int nCtrl, int nShift)
{
	int n = m_selectRgn.GetSubRectCount();
	int nDelBottom = m_cellMatrix.GetRows();
	CMyRect rcTmp;
	for (int i = 0; i < n; ++ i)
	{
		rcTmp = *m_selectRgn.GetSubRect(i);
		if (rcTmp.Bottom() > nDelBottom)
			rcTmp.SetHeight(nDelBottom - rcTmp.Top());

		for (int y = rcTmp.Top(); y < rcTmp.Bottom(); ++ y)
		{
			for (int x = rcTmp.Left(); x < rcTmp.Right(); ++ x)
			{
				CGridCell* pCell = OnGetElement(x, y);
				if (pCell)
				{
					pCell->Clear();
				}
			}
		}
	}
	::InvalidateRect(m_hWnd, NULL, TRUE);
}

void CGrid::OnPriorKey(int nCtrl, int nShift)
{
	if (m_idLastSelect.y == m_nFixRowCount)
		return ;

	RECT rc;
	CCellRange range;

	::GetClientRect(m_hWnd, &rc);
	POINT pt = GetOriginPos();
	int i = m_cellIndex.y - 1;
	if (i < 0)
	{
		m_idLastSelect.y = m_nFixRowCount;
	}
	else
	{
		int n = 0;
		do
		{
			n += GetRowHeight(i + m_nFixRowCount);
			-- i;
		}
		while (n <rc.bottom - pt.y && i> 0);

		m_idLastSelect.y -= (m_cellIndex.y - i);
		m_cellIndex.y = i;		
		::SetScrollPos(m_hWnd, SB_VERT, m_cellIndex.y, TRUE);
	}

	if (nShift)
	{
		CMyRect rcSelect;
		int x, y, cx, cy;
		x = min(m_idFocus.x, m_idLastSelect.x);
		y = min(m_idFocus.y, m_idLastSelect.y);
		cx = max(m_idFocus.x, m_idLastSelect.x) - x + 1;
		cy = max(m_idFocus.y, m_idLastSelect.y) - y + 1;
		rcSelect.Set(x, y, cx, cy);
		m_selectRgn.Init(&rcSelect);

		range.Set(x, y, cx, cy);
		OnSelectChanged(range);
	}
	else
	{
		m_idClickDown = m_idFocus = m_idLastSelect;
		m_selectRgn.Init(m_idFocus.x, m_idFocus.y, 1, 1);

		range.Set(m_idFocus.x, m_idFocus.y, 1, 1);
		OnSelectChanged(range);
	}

	InvalidateRect(m_hWnd, NULL, TRUE);
}
void CGrid::OnNextKey(int nCtrl, int nShift)
{
	if (m_idLastSelect.y == m_nRows - 1)
		return ;

	CCellRange range;
	int nOldY = m_cellIndex.y;
	int n = m_nDrawedBodyRows - m_nFixRowCount;
	m_cellIndex.y += n;
	if (m_cellIndex.y > m_nLastRow)
		m_cellIndex.y = m_nLastRow;
	::SetScrollPos(m_hWnd, SB_VERT, m_cellIndex.y, TRUE);

	m_idLastSelect.y += n;
	if (m_idLastSelect.y >= m_nRows)
		m_idLastSelect.y = m_nRows - 1;

	if (nShift)
	{
		CMyRect rcSelect;
		int x, y, cx, cy;
		x = min(m_idFocus.x, m_idLastSelect.x);
		y = min(m_idFocus.y, m_idLastSelect.y);
		cx = max(m_idFocus.x, m_idLastSelect.x) - x + 1;
		cy = max(m_idFocus.y, m_idLastSelect.y) - y + 1;
		rcSelect.Set(x, y, cx, cy);
		m_selectRgn.Init(&rcSelect);

		range.Set(x, y, cx, cy);
		OnSelectChanged(range);
	}
	else
	{
		m_idClickDown = m_idFocus = m_idLastSelect;
		m_selectRgn.Init(m_idFocus.x, m_idFocus.y, 1, 1);

		range.Set(m_idFocus.x, m_idFocus.y, 1, 1);
		OnSelectChanged(range);
	}

	InvalidateRect(m_hWnd, NULL, TRUE);
}

CGridCell* CGrid::OnGetElement(int x, int y)
{
	return m_cellMatrix.GetElement(x, y);
}

void CGrid::InsertRow(int y, int cy)
{
	m_cellMatrix.InsertRow(y, cy);

	if (m_aryHeight.GetSize() < y)
	{
		int n = m_aryHeight.GetSize();
		m_aryHeight.SetSize(y);
		for (int i = n; i < y; ++ i)
			m_aryHeight[i] = DEF_ROW_HEIGHT;
	}
	m_aryHeight.InsertAt(y, DEF_ROW_HEIGHT, cy);



	m_merge.InsertRow(y, cy);
	int n = m_merge.GetMergeCellCount();
	for (int i = 0; i < n; ++ i)
	{
		CMergeCell* pMerge = m_merge.GetMergeCell(i);

		if (pMerge->m_range.GetBottom() > y)
		{
			GetCellsRect(pMerge->m_range.GetID().x, pMerge->m_range.GetID().y,
				pMerge->m_range.GetID().x + pMerge->m_range.GetCX(),
				pMerge->m_range.GetID().y + pMerge->m_range.GetCY(),
				&pMerge->m_rect);
		}
	}

	//
	if (y + cy > m_nLastRow)
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		POINT pt = GetOriginPos();
		CalcVInfo(rc.bottom - pt.y);
	}
}

void CGrid::InsertCol(int x, int cx)
{
	m_cellMatrix.InsertCol(x, cx);

	if (m_aryWidth.GetSize() < x)
	{
		int n = m_aryWidth.GetSize();
		m_aryWidth.SetSize(x);
		for (int i = n; i < x; ++ i)
			m_aryWidth[i] = DEF_COL_WIDTH;
	}
	m_aryWidth.InsertAt(x, DEF_COL_WIDTH, cx);

	m_merge.InsertCol(x, cx);
	int n = m_merge.GetMergeCellCount();
	for (int i = 0; i < n; ++ i)
	{
		CMergeCell* pMerge = m_merge.GetMergeCell(i);

		if (pMerge->m_range.GetRight() > x)
		{
			GetCellsRect(pMerge->m_range.GetID().x, pMerge->m_range.GetID().y,
				pMerge->m_range.GetID().x + pMerge->m_range.GetCX(),
				pMerge->m_range.GetID().y + pMerge->m_range.GetCY(),
				&pMerge->m_rect);
		}
	}

	//
	if (x + cx > m_nLastCol)
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		POINT pt = GetOriginPos();
		CalcHInfo(rc.right - pt.x);
	}
}

void CGrid::RemoveRow(int y, int cy)
{
	if (y < m_aryHeight.GetSize() && y + cy >= m_aryHeight.GetSize())
	{
		m_aryHeight.SetSize(y);
	}
	else if (y + cy < m_aryHeight.GetSize())
	{
		m_aryHeight.RemoveAt(y, cy);
	}


	m_cellMatrix.RemoveRow(y, cy);
	m_merge.RemoveRow(y, cy);
	int n = m_merge.GetMergeCellCount();
	for (int i = 0; i < n; ++ i)
	{
		CMergeCell* pMerge = m_merge.GetMergeCell(i);

		if (pMerge->m_range.GetBottom() >= y)
		{
			GetCellsRect(pMerge->m_range.GetID().x, pMerge->m_range.GetID().y,
				pMerge->m_range.GetID().x + pMerge->m_range.GetCX(),
				pMerge->m_range.GetID().y + pMerge->m_range.GetCY(),
				&pMerge->m_rect);
		}
	}

	//
	if (y + cy > m_nLastRow)
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		POINT pt = GetOriginPos();
		CalcVInfo(rc.bottom - pt.y);
	}
}

void CGrid::RemoveCol(int x, int cx)
{
	if (x < m_aryWidth.GetSize() && x + cx >= m_aryWidth.GetSize())
	{
		m_aryWidth.SetSize(x);
	}
	else if (x + cx < m_aryWidth.GetSize())
	{
		m_aryWidth.RemoveAt(x, cx);
	}


	m_cellMatrix.RemoveCol(x, cx);
	m_merge.RemoveCol(x, cx);
	int n = m_merge.GetMergeCellCount();
	for (int i = 0; i < n; ++ i)
	{
		CMergeCell* pMerge = m_merge.GetMergeCell(i);

		if (pMerge->m_range.GetRight() >= x)
		{
			GetCellsRect(pMerge->m_range.GetID().x, pMerge->m_range.GetID().y,
				pMerge->m_range.GetID().x + pMerge->m_range.GetCX(),
				pMerge->m_range.GetID().y + pMerge->m_range.GetCY(),
				&pMerge->m_rect);
		}
	}

	//
	if (x + cx > m_nLastCol)
	{
		RECT rc;
		::GetClientRect(m_hWnd, &rc);
		POINT pt = GetOriginPos();
		CalcHInfo(rc.right - pt.x);
	}
}

void CGrid::ClearCell(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
		return ;
	else
	{
		pCell->Clear();
	}
}

void CGrid::SetCellText(CellID id, TCHAR* e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellChar(CellID id, const char e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellUChar(CellID id, const unsigned char e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellInt(CellID id, const int e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellUInt(CellID id, const unsigned int e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellShort(CellID id, const short e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellUShort(CellID id, const unsigned short e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellLong(CellID id, const long e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellULong(CellID id, const unsigned long e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellFloat(CellID id, const float e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellDouble(CellID id, const double e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}

void CGrid::SetCellBool(CellID id, const bool e)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (!pCell)
	{
		CGridCell v;
		pCell = m_cellMatrix.SetElement(id.x, id.y, v);
	}
	(*pCell) << e;
}


TCHAR* CGrid::GetCellText(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (TCHAR *) (*pCell);
	else
		return NULL;
}

char CGrid::GetCellChar(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (char) (*pCell);
	else
		return 0;
}

unsigned char CGrid::GetCellUChar(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (unsigned char) (*pCell);
	else
		return 0;
}

short CGrid::GetCellShort(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (short) (*pCell);
	else
		return 0;
}

unsigned short CGrid::GetCellUShort(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (unsigned short) (*pCell);
	else
		return 0;
}

int CGrid::GetCellInt(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (int) (*pCell);
	else
		return 0;
}

unsigned int CGrid::GetCellUInt(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (unsigned int) (*pCell);
	else
		return 0;
}

long CGrid::GetCellLong(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (long) (*pCell);
	else
		return 0;
}

unsigned long CGrid::GetCellULong(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (unsigned long) (*pCell);
	else
		return 0;
}

float CGrid::GetCellFloat(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (float) (*pCell);
	else
		return 0.0f;
}

double CGrid::GetCellDouble(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (double) (*pCell);
	else
		return 0.0;
}

bool CGrid::GetCellBool(CellID id)
{
	CMyVariant* pCell = m_cellMatrix.GetElement(id.x, id.y);
	if (pCell)
		return (bool) (*pCell);
	else
		return false;
}

int CGrid::SetRows(int nRows)
{
	int n = m_nRows;
	m_nRows = nRows < m_nFixRowCount ? m_nFixRowCount : nRows;

	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	POINT pt = GetOriginPos();
	CalcVInfo(rc.bottom - pt.y);

	return n;
}

int CGrid::SetCols(int nCols)
{
	int n = m_nCols;
	m_nCols = nCols < m_nFixColCount ? m_nFixColCount : nCols;

	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	POINT pt = GetOriginPos();
	CalcHInfo(rc.right - pt.x);

	return n;
}

int CGrid::GetRows()
{
	return m_nRows;
}

int CGrid::GetCols()
{
	return m_nCols;
}

void CGrid::OnEdited(int x, int y)
{
	UnEditCell();
	m_bEditting = false;
	m_cellEditting.x = m_cellEditting.y = -1;
}

void CGrid::OnCancelEdit(int x, int y)
{
	UnEditCell(false);
	m_bEditting = false;
	m_cellEditting.x = m_cellEditting.y = -1;
}

void CGrid::OnSelectChanged(const CCellRange& range)
{
	/*	TCHAR sz[100];
		_stprintf(sz, _TEXT("%d,%d - %d,%d"), range.GetID().x, range.GetID().y, range.GetCX(), range.GetCY());
		::SetWindowText(::GetParent(m_hWnd), sz);*/
	::SendMessage(m_hWnd, WM_GRID_SELECTCHANGED,
		MAKEWPARAM(range.GetID().x, range.GetID().y),
		MAKELPARAM(range.GetCX(), range.GetCY()));
}

CGridCellSeries* CGrid::GetColumnInfo()
{
	return &m_columnInfo;
}

VT_TYPE CGrid::GetColumnType(int nColumn)
{
	return m_columnInfo.GetType(nColumn - m_nFixColCount);
}

void CGrid::SetColumnType(int nColumn, VT_TYPE type, int nCount)
{
	m_columnInfo.SetType(nColumn - m_nFixColCount, type, nCount);
}

CGridCellSeries* CGrid::GetRowInfo()
{
	return &m_rowInfo;
}

VT_TYPE CGrid::GetRowType(int nRow)
{
	return m_rowInfo.GetType(nRow - m_nFixRowCount);
}

void CGrid::SetRowType(int nRow, VT_TYPE type, int nCount)
{
	m_rowInfo.SetType(nRow - m_nFixRowCount, type, nCount);
}

VT_TYPE CGrid::GetCellType(int x, int y)
{
	VT_TYPE type = m_bPreferColumn ? GetColumnType(x) : GetRowType(y);
	VT_TYPE t1 = !m_bPreferColumn ? GetColumnType(x) : GetRowType(y);

	if (type == MYVT_NONE && t1 != MYVT_NONE)
		type = t1;
	return type;
}

void CGrid::UnEditCell(bool bSave)
{
	if (!m_bEditting)
		return ;
	HWND hWnd;
	if (m_bPreferColumn)
		hWnd = this->m_columnInfo.GetEditHandle(m_cellEditting.x -
									m_nFixColCount);
	else
		hWnd = this->m_rowInfo.GetEditHandle(m_cellEditting.y - m_nFixRowCount);
	if (!hWnd)
		return ;

	if (bSave)
	{
#ifndef _WIN64
		int n = ::SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
#else
		LRESULT n = ::SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0);
#endif
		if (n <= 10)
		{
			TCHAR szTmp[11];
			::GetWindowText(hWnd, szTmp, 10);

			VT_TYPE type = CMyVariant::GetType(szTmp);
			unsigned long ul;
			long l;
			double d;

			switch (type)
			{
			case MYVT_NONE:
				ClearCell(m_cellEditting);
				break;
			case MYVT_ULONG:
				ul = _ttol(szTmp);
				SetCellULong(m_cellEditting, ul);
				break;
			case MYVT_LONG:
				l = _ttol(szTmp);
				SetCellLong(m_cellEditting, l);
				break;
			case MYVT_DOUBLE:
				d = _tcstod(szTmp, 0);
				SetCellDouble(m_cellEditting, d);
				break;
			case MYVT_PTCHAR:
				SetCellText(m_cellEditting, szTmp);
				break;
			}
		}
		else
		{
			TCHAR* p = (TCHAR*) malloc(sizeof(TCHAR) * (n + 1));
			::GetWindowText(hWnd, p, (int)n);
			SetCellText(m_cellEditting, p);
			free(p);
		}
	}

	::ShowWindow(hWnd, SW_HIDE);
	::SetFocus(m_hWnd);
}

HWND CGrid::EditCell(CellID id, UINT nChar)
{
	int cx, cy;
	cx = cy = 1;
	CMergeCell* pMerge = m_merge.GetMergeCell(id.x, id.y);
	if (pMerge)
	{
		id.x = pMerge->m_range.GetID().x;
		id.y = pMerge->m_range.GetID().y;
		cx = pMerge->m_range.GetCX();
		cy = pMerge->m_range.GetCY();
	}

	CGridCell* pCell = OnGetElement(id.x, id.y);

	//VT_TYPE type = m_bPreferColumn?GetColumnType(id.x):GetRowType(id.y);
	VT_TYPE type = GetCellType(id.x, id.y);
	if (type == MYVT_NONE)
	{
		if (pCell && pCell->type == MYVT_BOOL)
		{
			return NULL;
		}
	}
	else if (type == MYVT_BOOL || type == MYVT_LPARAM)
	{
		return NULL;
	}

	m_bEditting = true;
	m_cellEditting = id;

	// show the left and top of the cell
	bool bRedraw = false;
	if (id.x < m_cellIndex.x + m_nFixColCount)
	{
		SetLeftIndex(id.x);
		bRedraw = true;
	}
	if (id.y < m_cellIndex.y + m_nFixRowCount)
	{
		SetTopIndex(id.y);
		bRedraw = true;
	}
	if (bRedraw)
		::InvalidateRect(m_hWnd, NULL, TRUE);

	RECT rc;
	POINT pt = GetOriginPos();
	GetCellsRect(id.x, id.y, id.x + cx, id.y + cy, &rc);
	rc.left += (pt.x + 1 - m_nXExcursion);
	rc.right += (pt.x - m_nXExcursion);
	rc.top += (pt.y + 1 - m_nYExcursion);
	rc.bottom += (pt.y - m_nYExcursion);

	if (m_bPreferColumn)
		return m_columnInfo.EditCell(id.x - m_nFixColCount, rc, pCell, nChar);
	else
		return m_rowInfo.EditCell(id.y - m_nFixRowCount, rc, pCell, nChar);
}







COLORREF CGrid::GetFixCellBorderColor()
{
	LOGPEN logPen;
	GetObject(m_hFixCellBorder, sizeof(LOGPEN), &logPen);
	return logPen.lopnColor;
}

void CGrid::SetFixCellBorderColor(COLORREF color)
{
	DeleteObject(m_hFixCellBorder);
	m_hFixCellBorder = CreatePen(PS_SOLID, 1, color);
}

COLORREF CGrid::GetCellBorderColor()
{
	LOGPEN logPen;
	GetObject(m_hCellBorder, sizeof(LOGPEN), &logPen);
	return logPen.lopnColor;
}

void CGrid::SetCellBorderColor(COLORREF color)
{
	DeleteObject(m_hCellBorder);
	m_hCellBorder = CreatePen(PS_SOLID, 1, color);
}

COLORREF CGrid::GetFixCellBkgColor()
{
	LOGBRUSH logBrush;
	GetObject(m_hFixBkgnd, sizeof(LOGBRUSH), &logBrush);
	return logBrush.lbColor;
}

void CGrid::SetFixCellBkgColor(COLORREF color)
{
	DeleteObject(m_hFixBkgnd);
	m_hFixBkgnd = CreateSolidBrush(color);
}

COLORREF CGrid::GetSheetBorderColor()
{
	LOGPEN logPen;
	GetObject(m_hSheetBorder, sizeof(LOGPEN), &logPen);
	return logPen.lopnColor;
}

void CGrid::SetSheetBorderColor(COLORREF color)
{
	DeleteObject(m_hSheetBorder);
	m_hSheetBorder = CreatePen(PS_SOLID, 2, color);
}

COLORREF CGrid::GetBkgColor()
{
	LOGBRUSH logBrush;
	GetObject(m_hSheetBkgnd, sizeof(LOGBRUSH), &logBrush);
	return logBrush.lbColor;
}

void CGrid::SetBkgColor(COLORREF color)
{
	DeleteObject(m_hSheetBkgnd);
	m_hSheetBkgnd = CreateSolidBrush(color);
}

HBRUSH CGrid::GetBkgBrush()
{
	return m_hSheetBkgnd;
}

COLORREF CGrid::GetFocusBorderColor()
{
	LOGPEN logPen;
	GetObject(m_hFocusInSideBorder, sizeof(LOGPEN), &logPen);
	return logPen.lopnColor;
}

void CGrid::SetFocusBorderColor(COLORREF color)
{
	DeleteObject(m_hFocusInSideBorder);
	m_hFocusInSideBorder = CreatePen(PS_SOLID, 1, color);
}

COLORREF CGrid::GetSelectedBkgColor()
{
	LOGBRUSH logBrush;
	GetObject(m_hSelectBkgnd, sizeof(LOGBRUSH), &logBrush);
	return logBrush.lbColor;
}

void CGrid::SetSelectedBkgColor(COLORREF color)
{
	DeleteObject(m_hSelectBkgnd);
	m_hSelectBkgnd = CreateSolidBrush(color);
}

////////////////////////////////////////////////////////////////////////////////////////////////





































////////////////////////////////////////////////////////////////////////////////////////////////
void CGrid::OnClose()
{
	//	PostQuitMessage(0);
}

void DrawGrid(CGrid* pGrid, HDC hdc)
{
	CGridMemDC dc(hdc, pGrid->GetBkgBrush());
	::SelectObject(dc.m_hdc, pGrid->GetFont());
	pGrid->OnDraw(dc.m_hdc, pGrid->GetNowTick());
}

void CGrid::OnPaint()
{
	HDC hdc = ::GetDC(m_hWnd);
	DrawGrid(this, hdc);
	::ReleaseDC(m_hWnd, hdc);
}

BOOL CGrid::OnEraseBkgnd(HDC hDC)
{
	return TRUE;//CWidget::OnEraseBkgnd(hDC);
}

void CGrid::OnSize(UINT nType, int cx, int cy)
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);

	POINT pt = GetOriginPos();
	CalcVInfo(rc.bottom - pt.y);
	if (m_nLastRow == 0)
		m_cellIndex.y = 0;
	::SetScrollRange(m_hWnd, SB_VERT, 0, m_nLastRow, TRUE);

	CalcHInfo(rc.right - pt.x);
	if (m_nLastCol == 0)
		m_cellIndex.x = 0;
	::SetScrollRange(m_hWnd, SB_HORZ, 0, m_nLastCol, TRUE);
}

void CGrid::OnVScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
{
	UINT i, j;
	SCROLLINFO si;
	switch (nSBCode)
	{
	case SB_BOTTOM:
		break;
	case SB_ENDSCROLL:
		break;
	case SB_LINEDOWN:
		if (m_cellIndex.y < m_nLastRow)
		{
			m_nYExcursion += GetRowHeight(m_cellIndex.y + m_nFixRowCount);
			++ m_cellIndex.y;
			::SetScrollPos(m_hWnd, SB_VERT, m_cellIndex.y, TRUE);
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
		break;
	case SB_LINEUP:
		if (m_cellIndex.y > 0)
		{
			-- m_cellIndex.y;
			m_nYExcursion -= GetRowHeight(m_cellIndex.y + m_nFixRowCount);
			::SetScrollPos(m_hWnd, SB_VERT, m_cellIndex.y, TRUE);
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
		break;
	case SB_PAGEDOWN:
		if (m_cellIndex.y < m_nLastRow)
		{
			j = m_cellIndex.y;
			m_cellIndex.y += 10;
			if (m_cellIndex.y > m_nLastRow)
				m_cellIndex.y = m_nLastRow;

			for (i = j; i < (UINT) m_cellIndex.y; ++ i)
			{
				m_nYExcursion += GetRowHeight(j + m_nFixRowCount + i);
			}

			::SetScrollPos(m_hWnd, SB_VERT, m_cellIndex.y, TRUE);
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
		break;
	case SB_PAGEUP:
		if (m_cellIndex.y > 0)
		{
			j = m_cellIndex.y;

			m_cellIndex.y -= 10;
			if (m_cellIndex.y < 0)
				m_cellIndex.y = 0;

			for (i = m_cellIndex.y; i < j; ++ i)
			{
				m_nYExcursion -= GetRowHeight(m_nFixRowCount + i);
			}

			::SetScrollPos(m_hWnd, SB_VERT, m_cellIndex.y, TRUE);
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
		break;
	case SB_THUMBPOSITION:
		ZeroMemory(&si, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_TRACKPOS;

		if (!::GetScrollInfo(m_hWnd, SB_VERT, &si))
			return ; // GetScrollInfo failed
		m_cellIndex.y = si.nTrackPos;
		::SetScrollPos(m_hWnd, SB_VERT, m_cellIndex.y, TRUE);
		m_nYExcursion = GetBodyCellTop(nPos + m_nFixRowCount);
		InvalidateRect(m_hWnd, NULL, FALSE);
		break;
	case SB_THUMBTRACK:
		break;
	case SB_TOP:
		break;
	}
}

void CGrid::OnHScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
{
	UINT i, j;
	SCROLLINFO si;
	switch (nSBCode)
	{
	case SB_ENDSCROLL:
		break;
	case SB_LEFT:
		break;
	case SB_RIGHT:
		break;
	case SB_LINELEFT:
		if (m_cellIndex.x > 0)
		{
			-- m_cellIndex.x;
			m_nXExcursion -= GetColWidth(m_cellIndex.x + m_nFixColCount);
			::SetScrollPos(m_hWnd, SB_HORZ, m_cellIndex.x, TRUE);
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
		break;
	case SB_LINERIGHT:
		if (m_cellIndex.x < m_nLastCol)
		{
			m_nXExcursion += GetColWidth(m_cellIndex.x + m_nFixColCount);
			++ m_cellIndex.x;
			::SetScrollPos(m_hWnd, SB_HORZ, m_cellIndex.x, TRUE);
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
		break;
	case SB_PAGELEFT:
		if (m_cellIndex.x > 0)
		{
			j = m_cellIndex.x;

			m_cellIndex.x -= 10;
			if (m_cellIndex.x < 0)
				m_cellIndex.x = 0;

			for (i = m_cellIndex.x; i < j; ++ i)
			{
				m_nXExcursion -= GetColWidth(m_nFixColCount + i);
			}

			::SetScrollPos(m_hWnd, SB_HORZ, m_cellIndex.x, TRUE);
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
		break;
	case SB_PAGERIGHT:
		if (m_cellIndex.x < m_nLastCol)
		{
			j = m_cellIndex.x;
			m_cellIndex.x += 10;
			if (m_cellIndex.x > m_nLastCol)
				m_cellIndex.x = m_nLastCol;

			for (i = j; i < (UINT) m_cellIndex.x; ++ i)
			{
				m_nXExcursion += GetColWidth(j + m_nFixColCount + i);
			}

			::SetScrollPos(m_hWnd, SB_HORZ, m_cellIndex.x, TRUE);
			InvalidateRect(m_hWnd, NULL, FALSE);
		}
		break;
	case SB_THUMBPOSITION:
		ZeroMemory(&si, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_TRACKPOS;

		if (!::GetScrollInfo(m_hWnd, SB_HORZ, &si))
			return ; // GetScrollInfo failed
		m_cellIndex.x = si.nTrackPos;

		::SetScrollPos(m_hWnd, SB_HORZ, m_cellIndex.x, TRUE);
		m_nXExcursion = GetBodyCellLeft(nPos + m_nFixColCount);
		InvalidateRect(m_hWnd, NULL, FALSE);
		break;
	case SB_THUMBTRACK:
		break;
	}
}

void CGrid::OnLButtonDown(UINT nFlags, POINT point)
{
	::SetCapture(m_hWnd);
	if (m_resizeType == vert)
	{
		m_bResizing = true;

		HDC hdc = ::GetDC(m_hWnd);
		//POINT ptOrigin = GetOriginPos();
		RECT rcClient = GetWindowRect();
		DrawResizeLine(hdc, 0, m_nResizePos, rcClient.right, m_nResizePos);
		::ReleaseDC(m_hWnd, hdc);

		return ;
	}
	else if (m_resizeType == horz)
	{
		m_bResizing = true;

		HDC hdc = ::GetDC(m_hWnd);
		//POINT ptOrigin = GetOriginPos();
		RECT rcClient = GetWindowRect();
		DrawResizeLine(hdc, m_nResizePos, 0, m_nResizePos, rcClient.bottom);
		::ReleaseDC(m_hWnd, hdc);

		return ;
	}

	UnEditCell();

	CellID id = CellFromPoint(point);
	CCellRange range;

	CMergeCell* pMerge = m_merge.GetMergeCell(id.x, id.y);
	if (pMerge)
	{
		id.x = pMerge->m_range.GetID().x;
		id.y = pMerge->m_range.GetID().y;
	}
	m_idClickDown = m_idLastSelect = id;

	CMyRect rcSelect;
	short nCtrl = GetKeyState(VK_CONTROL) >> 8;
	short nShift = GetKeyState(VK_SHIFT) >> 8;

	if (id.x < m_nFixColCount && id.y >= m_nFixRowCount)
	{
		// click in left header

		m_idLastSelect.x = m_nCols - 1;

		if (!nCtrl && nShift)
		{
			RECT_TYPE t, h;
			t = min(id.y, m_idFocus.y);
			h = max(id.y, m_idFocus.y) - t + 1;
			rcSelect.Set(m_nFixColCount, t, m_nCols - m_nFixColCount, h);
			m_selectRgn.Init(rcSelect.Left(), rcSelect.Top(),
							rcSelect.Width(), rcSelect.Height());

			range.Set(rcSelect.Left(), rcSelect.Top(), rcSelect.Width(),
					rcSelect.Height());
			OnSelectChanged(range);
		}
		else if (nCtrl && !nShift)
		{
			rcSelect.Set(m_nFixColCount, id.y, m_nCols - m_nFixColCount, 1);
			m_selectRgn.Merge(&rcSelect);

			m_idFocus.x = m_nFixColCount;
			m_idFocus.y = id.y;
		}
		else
		{
			rcSelect.Set(m_nFixColCount, id.y, m_nCols - m_nFixColCount, 1);
			m_selectRgn.Init(rcSelect.Left(), rcSelect.Top(),
							rcSelect.Width(), rcSelect.Height());

			range.Set(rcSelect.Left(), rcSelect.Top(), rcSelect.Width(),
					rcSelect.Height());
			OnSelectChanged(range);

			m_idFocus.x = m_nFixColCount;
			m_idFocus.y = id.y;
		}
	}
	else if (id.x >= m_nFixColCount && id.y < m_nFixRowCount)
	{
		// click in top header

		m_idLastSelect.y = m_nRows - 1;

		if (!nCtrl && nShift)
		{
			RECT_TYPE l, w;
			l = min(id.x, m_idFocus.x);
			w = max(id.x, m_idFocus.x) - l + 1;
			rcSelect.Set(l, m_nFixRowCount, w, m_nRows - m_nFixRowCount);
			m_selectRgn.Init(rcSelect.Left(), rcSelect.Top(),
							rcSelect.Width(), rcSelect.Height());

			range.Set(rcSelect.Left(), rcSelect.Top(), rcSelect.Width(),
					rcSelect.Height());
			OnSelectChanged(range);
		}
		else if (nCtrl && !nShift)
		{
			rcSelect.Set(id.x, m_nFixRowCount, 1, m_nRows - m_nFixRowCount);
			m_selectRgn.Merge(&rcSelect);

			m_idFocus.x = id.x;
			m_idFocus.y = m_nFixRowCount;
		}
		else
		{
			rcSelect.Set(id.x, m_nFixRowCount, 1, m_nRows - m_nFixRowCount);
			m_selectRgn.Init(rcSelect.Left(), rcSelect.Top(),
							rcSelect.Width(), rcSelect.Height());

			range.Set(rcSelect.Left(), rcSelect.Top(), rcSelect.Width(),
					rcSelect.Height());
			OnSelectChanged(range);

			m_idFocus.x = id.x;
			m_idFocus.y = m_nFixRowCount;
		}
	}
	else if (id.x < m_nFixColCount && id.y < m_nFixRowCount)
	{
		// click in top left header
		rcSelect.Set(m_nFixColCount, m_nFixRowCount, m_nCols - m_nFixColCount,
					m_nRows - m_nFixRowCount);
		m_selectRgn.Init(rcSelect.Left(), rcSelect.Top(), rcSelect.Width(),
						rcSelect.Height());

		range.Set(rcSelect.Left(), rcSelect.Top(), rcSelect.Width(),
				rcSelect.Height());
		OnSelectChanged(range);

		m_idLastSelect.x = m_nCols - 1;
		m_idLastSelect.y = m_nRows - 1;

		m_idFocus.x = m_nFixColCount;
		m_idFocus.y = m_nFixRowCount;
	}
	else
	{
		// click in body
		if (nCtrl && !nShift)
		{
			CMyRect rc;
			rc.Set(id.x, id.y, 1, 1);
			if (!m_selectRgn.PtInRgn(id.x, id.y))
			{
				m_selectRgn.Merge(&rc);
			}
			m_idFocus = id;
		}
		else if (!nCtrl && nShift)
		{
			int l, t, w, h;
			l = min(m_idFocus.x, id.x);
			w = max(m_idFocus.x, id.x) - l + 1;
			t = min(m_idFocus.y, id.y);
			h = max(m_idFocus.y, id.y) - t + 1;

			if (w == 1 && h == 1)
				return ;
			//			rcSelect.Set(l, t, w, h);
			m_selectRgn.Init(l, t, w, h);

			range.Set(l, t, w, h);
			OnSelectChanged(range);
		}
		else if (nCtrl && nShift)
		{
		}
		else
		{
			m_idFocus = id;
			m_selectRgn.Init(id.x, id.y, 1, 1);

			range.Set(id.x, id.y, 1, 1);
			OnSelectChanged(range);
		}
	}

	InvalidateRect(m_hWnd, NULL, TRUE);

	m_bLButtonDown = true;
}

void CGrid::OnLButtonUp(UINT nFlags, POINT point)
{
	if (m_bResizing)
	{
		m_bResizing = false;

		if (m_resizeType == vert)
		{
			HDC hdc = ::GetDC(m_hWnd);
			//POINT ptOrigin = GetOriginPos();
			RECT rcClient = GetWindowRect();
			DrawResizeLine(hdc, 0, m_nResizePos, rcClient.right, m_nResizePos);
			::ReleaseDC(m_hWnd, hdc);


			m_nResizeOldPos -= GetRowHeight(m_nResizeIndex);
			if (m_nResizePos - m_nResizeOldPos > 0)
			{
				SetRowHeight(m_nResizeIndex, m_nResizePos - m_nResizeOldPos);
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
		else if (m_resizeType == horz)
		{
			HDC hdc = ::GetDC(m_hWnd);
			//POINT ptOrigin = GetOriginPos();
			RECT rcClient = GetWindowRect();
			DrawResizeLine(hdc, m_nResizePos, 0, m_nResizePos, rcClient.bottom);
			::ReleaseDC(m_hWnd, hdc);

			m_nResizeOldPos -= GetColWidth(m_nResizeIndex);
			if (m_nResizePos - m_nResizeOldPos > 0)
			{
				SetColWidth(m_nResizeIndex, m_nResizePos - m_nResizeOldPos);
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
	}

	m_bLButtonDown = false;

	ReleaseCapture();

	if (m_bTimerOn)
	{
		m_bTimerOn = false;
		::KillTimer(m_hWnd, SELECT_TIMER_ID);
	}

	CellID id = CellFromPoint(point);
	if (id.x < m_nFixColCount)
		return ;
	if (id.y < m_nFixRowCount)
		return ;

	int r, b;
	r = id.x + 1;
	b = id.y + 1;
	CMergeCell* pMerge = m_merge.GetMergeCell(id.x, id.y);
	if (pMerge)
	{
		id.x = pMerge->m_range.GetID().x;
		id.y = pMerge->m_range.GetID().y;
		r = pMerge->m_range.GetRight();
		b = pMerge->m_range.GetBottom();
	}

	//VT_TYPE type = m_bPreferColumn?this->GetColumnType(id.x):this->GetRowType(id.y);
	VT_TYPE type = GetCellType(id.x, id.y);
	CMyVariant* pCell = (CMyVariant*) OnGetElement(id.x, id.y);
	if (type == MYVT_BOOL || (pCell && pCell->type == MYVT_BOOL))
	{
		POINT pt = GetOriginPos();
		RECT rcCell, rcCheckRange;
		GetCellsRect(id.x, id.y, r, b, &rcCell);
		rcCell.left += (pt.x - m_nXExcursion);
		rcCell.right += (pt.x - m_nXExcursion);
		rcCell.top += (pt.y - m_nYExcursion);
		rcCell.bottom += (pt.y - m_nYExcursion);

		CGridCheckBox::GetRange(rcCell, &rcCheckRange);
		if (PtInRect(&rcCheckRange, point))
		{
			SetCellBool(id, !GetCellBool(id));
			::InvalidateRect(m_hWnd, NULL, TRUE);
		}
	}
}

void CGrid::OnLButtonDblClk(UINT nFlags, POINT point)
{
	POINT pt = GetOriginPos();
	if (point.x <= pt.x || point.y <= pt.y)
		return ;

	CellID id = CellFromPoint(point);
	EditCell(id);
}

void CGrid::OnMouseMove(UINT nFlags, POINT point)
{
#define			INTERVAL			3

	POINT pt = GetOriginPos();
	if (!m_bLButtonDown)
	{
		if (!m_bResizing)
		{
			int i;
			bool bChgCursor;
			int n;

			if (point.x < pt.x)
			{
				if (m_allowRisizeType == ALL || m_allowRisizeType == VERT)
				{
					bChgCursor = false;
					n = 0;
					i = 0;
					while (n - 10 < point.y)
					{
						n += GetRowHeight(i ++);
						if (i == m_nFixRowCount + 1)
							i += m_cellIndex.y;

						if (point.y > n - INTERVAL && point.y < n + INTERVAL)
						{
							bChgCursor = true;
							m_nResizeOldPos = m_nResizePos = n;
							break;
						}
					}

					if (bChgCursor)
					{
						m_nResizeIndex = i - 1;
						m_resizeType = vert;
						SetCursor(m_hVertSize);
					}
					else
					{
						m_resizeType = none;
						if (point.y < pt.y)
						{
							goto TRY_Y;
						}
						else
							SetCursor(m_hRight);
					}
				}
				else //if(m_allowRisizeType == ALL || m_allowRisizeType == VERT)
				{
					m_resizeType = none;
					if (point.y < pt.y)
						goto TRY_Y;
					else
						SetCursor(m_hRight);
				}
			}
			else if (point.y < pt.y)
			{
				TRY_Y:
				if (m_allowRisizeType == ALL || m_allowRisizeType == HORZ)
				{
					bChgCursor = false;
					n = 0;
					i = 0;
					while (n - 10 < point.x)
					{
						n += GetColWidth(i ++);
						if (i == m_nFixColCount + 1)
							i += m_cellIndex.x;

						if (point.x > n - INTERVAL && point.x < n + INTERVAL)
						{
							bChgCursor = true;
							m_nResizeOldPos = m_nResizePos = n;
							break;
						}
					}
					if (bChgCursor)
					{
						m_nResizeIndex = i - 1;
						m_resizeType = horz;
						SetCursor(m_hHorzSize);
					}
					else
					{
						m_resizeType = none;

						if (point.x < pt.x)
							SetCursor(m_hCross);
						else
							SetCursor(m_hDown);
					}
				}
				else
				{
					m_resizeType = none;
					if (point.x < pt.x)
						SetCursor(m_hCross);
					else
						SetCursor(m_hDown);
				}
			}
			else
			{
				m_resizeType = none;
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
		}
		else //if (!m_bResizing)
		{
			if (m_resizeType == vert)
			{
				HDC hdc = ::GetDC(m_hWnd);
				RECT rcClient  = GetWindowRect();
				DrawResizeLine(hdc, rcClient.left, m_nResizePos, rcClient.right, m_nResizePos);
				if(point.y >rcClient.bottom)
					DrawResizeLine(hdc, rcClient.right, rcClient.bottom, rcClient.right, m_nResizePos);

				m_nResizePos = point.y;
				DrawResizeLine(hdc, rcClient.left, m_nResizePos, rcClient.right, m_nResizePos);
				if(point.y >rcClient.bottom)
					DrawResizeLine(hdc, rcClient.right, rcClient.bottom, rcClient.right, m_nResizePos);
				
				::ReleaseDC(m_hWnd, hdc);
			}
			else if (m_resizeType == horz)
			{
				HDC hdc = ::GetDC(m_hWnd);
				RECT rcClient = GetWindowRect();
				DrawResizeLine(hdc, m_nResizePos, rcClient.top, m_nResizePos, rcClient.bottom);
				if(point.x > rcClient.right)
					DrawResizeLine(hdc, rcClient.right, rcClient.bottom, m_nResizePos, rcClient.bottom);

				m_nResizePos = point.x;
				DrawResizeLine(hdc, m_nResizePos, rcClient.top, m_nResizePos, rcClient.bottom);
				if(point.x > rcClient.right)
					DrawResizeLine(hdc, rcClient.right, rcClient.bottom, m_nResizePos, rcClient.bottom);
				
				::ReleaseDC(m_hWnd, hdc);
			}
		}
	}

	if (m_bLButtonDown)
	{
		if (m_idLastSelect.x < m_nFixColCount &&
			m_idLastSelect.y < m_nFixRowCount)
			return ;


		RECT rc;
		CCellRange range;
		::GetClientRect(m_hWnd, &rc);
		rc.left = pt.x;
		rc.top = pt.y;
		if (PtInRect(&rc, point))
		{
			if (m_bTimerOn)
			{
				m_bTimerOn = false;
			}

			CellID id = CellFromPoint(point);
			if (id.x != m_idLastSelect.x || id.y != m_idLastSelect.y)
			{
				m_idLastSelect = id;

				CMyRect rcSelect;

				int l, t, w, h;

				if (m_idClickDown.x < m_nFixColCount)
				{
					// move in left head
					l = m_nFixColCount;
					t = min(m_idFocus.y, m_idLastSelect.y);
					w = m_nCols - l;
					h = max(m_idFocus.y, m_idLastSelect.y) - t + 1;
				}
				else if (m_idClickDown.y < m_nFixRowCount)
				{
					// move in top head
					l = min(m_idFocus.x, m_idLastSelect.x);
					t = m_nFixRowCount;
					w = max(m_idFocus.x, m_idLastSelect.x) - l + 1;
					h = m_nRows - t;
				}
				else
				{
					l = min(m_idFocus.x, m_idLastSelect.x);
					t = min(m_idFocus.y, m_idLastSelect.y);
					w = max(m_idFocus.x, m_idLastSelect.x) - l + 1;
					h = max(m_idFocus.y, m_idLastSelect.y) - t + 1;
				}

				rcSelect.Set(l, t, w, h);
				m_selectRgn.Init(rcSelect.Left(), rcSelect.Top(),
								rcSelect.Width(), rcSelect.Height());

				range.Set(rcSelect.Left(), rcSelect.Top(), rcSelect.Width(),
						rcSelect.Height());
				OnSelectChanged(range);

				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}
		else
		{
			if (!m_bTimerOn)
			{
				m_bTimerOn = true;
				::SetTimer(m_hWnd, SELECT_TIMER_ID, SELECT_TIMER_INTERVAL,
					NULL);
			}
		}
	}
}

void CGrid::OnMouseWheel(UINT nFlags, short zDelta, POINT pt)
{
	if (zDelta < 0)
	{
		// go down
		::SendMessage(m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
	}
	else if (zDelta > 0)
	{
		// go up
		::SendMessage(m_hWnd, WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
	}
}

void CGrid::OnRButtonDown(UINT nFlags, POINT point)
{
	const int MenuID = 1000;
	int result;

	//GetCells
	RECT_TYPE l = m_selectRgn.GetBoundRect()->Left();
	RECT_TYPE t = m_selectRgn.GetBoundRect()->Top();
	RECT_TYPE r = m_selectRgn.GetBoundRect()->Right();
	RECT_TYPE b = m_selectRgn.GetBoundRect()->Bottom();

	HMENU hMenu = CreatePopupMenu();
	if(r>l && b>t)AppendMenu(hMenu, MF_STRING, MenuID, _TEXT("&Copy data") );
	AppendMenu(hMenu, MF_STRING, MenuID+1, _TEXT("&About...") );
	ClientToScreen(m_hWnd, &point);
	result = TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_RETURNCMD, point.x, point.y, 0, this->m_hWnd, NULL);
	DestroyMenu(hMenu);

	if(result == MenuID)
	{		
		double val;
		CellID cid;

		tstring tstr;
		tostringstream tostr;

		RECT_TYPE i, j;

		// Get the max width
		unsigned int mwid=0;
		for(j=t; j<b; j++)
		{
			for(i=l; i<r; i++)
			{
				cid.x = i;
				cid.y = j;
				val = GetCellDouble(cid);
				tostr.str(_T(""));
				tostr<<val;
				tstr = tostr.str();
				if(tstr.length()>mwid)mwid=(unsigned int)tstr.length();
			}
		}

		//Get the buffer
		tostr.str(_T(""));
		for(j=t; j<b; j++)
		{
			for(i=l; i<r; i++)
			{
				cid.x = i;
				cid.y = j;
				val = GetCellDouble(cid);
				tostr<<setw(mwid)<<val;
				if(i!=r-1)tostr<<_T("\t");
			}
			tostr<<_T("\r\n");
		}
		tstr = tostr.str();
		
		TCHAR *source = (TCHAR *)tstr.c_str();

		// Copy to clipboard
		if( OpenClipboard(NULL) )
		{
			HGLOBAL clipbuffer;
			EmptyClipboard();
			
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, _tcslen(source)+1);
			char *buffer = (char*)GlobalLock(clipbuffer);
#if defined(_UNICODE) || defined(UNICODE)
			WideCharToMultiByte(CP_OEMCP, 0, source, (int)_tcslen(source), buffer, (int)_tcslen(source), NULL, FALSE );
#else
#	if defined(_MSC_VER) && (_MSC_VER>=1300)
			strcpy_s(buffer, strlen(source)+1, source);
#	else
			strcpy(buffer, source);
#	endif
#endif
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT,clipbuffer);
			CloseClipboard();
		}
	}
	else if(result == MenuID+1)
	{
		MessageBox(m_hWnd, _T("CGrid, a grid code based on Win32API."), _T("About"), MB_OK);
	}
	/*	short nCtrl = GetKeyState(VK_CONTROL) >> 8;
		short nShift = GetKeyState(VK_SHIFT) >> 8;
		if(nCtrl) {
			CellID id = CellFromPoint(point);
			SplitCell(id.x, id.y);
			InvalidateRect(m_hWnd, NULL, TRUE);
		}
		else {
			if(nShift) {
	//			CellID id = CellFromPoint(point);
	//			InsertRow(id.y, 2);
	//			RemoveRow(id.y, 2);
	//			InsertCol(id.x, 2);
	//			RemoveCol(id.x, 2);
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
			else {
				MergeCells(m_selectRgn.GetBoundRect()->Left(), 
					m_selectRgn.GetBoundRect()->Top(), 
					m_selectRgn.GetBoundRect()->Width(), 
					m_selectRgn.GetBoundRect()->Height());
				InvalidateRect(m_hWnd, NULL, TRUE);
			}
		}*/
}

void CGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	short nCtrl = GetKeyState(VK_CONTROL) >> 8;
	short nShift = GetKeyState(VK_SHIFT) >> 8;

	if (nChar == VK_UP)
	{
		OnUpKey(nCtrl, nShift);
	}
	else if (nChar == VK_LEFT)
	{
		OnLeftKey(nCtrl, nShift);
	}
	else if (nChar == VK_RIGHT)
	{
		OnRightKey(nCtrl, nShift);
	}
	else if (nChar == VK_DOWN)
	{
		OnDownKey(nCtrl, nShift);
	}
	else if (nChar == VK_PRIOR)
	{
		OnPriorKey(nCtrl, nShift);
	}
	else if (nChar == VK_NEXT)
	{
		OnNextKey(nCtrl, nShift);
	}
	else if (nChar == VK_RETURN)
	{
		if (!nCtrl && !nShift)
			OnDownKey(0, 0);
		else if(!nCtrl && nShift)
			OnUpKey(0, 0);
	}
	else if (nChar == VK_DELETE)
	{
		OnDelKey(nCtrl, nShift);
	}
}

void CGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar != VK_ESCAPE && nChar != VK_RETURN)
	{
		if (nChar == VK_SPACE)
		{
			//VT_TYPE type = m_bPreferColumn?GetColumnType(m_idFocus.x):GetRowType(m_idFocus.y);
			VT_TYPE type = GetCellType(m_idFocus.x, m_idFocus.y);

			CGridCell* pCell = OnGetElement(m_idFocus.x, m_idFocus.y);
			if (type == MYVT_BOOL || (pCell && pCell->type == MYVT_BOOL))
			{
				SetCellBool(m_idFocus, !GetCellBool(m_idFocus));
				::InvalidateRect(m_hWnd, NULL, FALSE);
			}
			else
				EditCell(m_idFocus, nChar);
		}
		else
			EditCell(m_idFocus, nChar);
	}
}

void CGrid::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == SELECT_TIMER_ID)
	{
		RECT rc;
		POINT pt;
		::GetClientRect(m_hWnd, &rc);
		GetCursorPos(&pt);
		::ScreenToClient(m_hWnd, &pt);
		POINT ptOrigin = GetOriginPos();
		rc.left = ptOrigin.x;
		rc.top = ptOrigin.y;


		if (PtInRect(&rc, pt))
		{
			m_bTimerOn = false;
			::KillTimer(m_hWnd, SELECT_TIMER_ID);
		}
		else
		{
			if (pt.x < rc.left)
			{
				if (m_cellIndex.x > 0)
				{
					-- m_cellIndex.x;
					m_nXExcursion -= GetColWidth(m_cellIndex.x +
										m_nFixColCount);
					::SetScrollPos(m_hWnd, SB_HORZ, m_cellIndex.x, TRUE);
				}
			}
			else if (pt.x > rc.right)
			{
				if (m_cellIndex.x < m_nLastCol)
				{
					m_nXExcursion += GetColWidth(m_cellIndex.x +
										m_nFixColCount);
					++ m_cellIndex.x;
					::SetScrollPos(m_hWnd, SB_HORZ, m_cellIndex.x, TRUE);
				}
			}

			if (pt.y < rc.top)
			{
				if (m_cellIndex.y > 0)
				{
					-- m_cellIndex.y;
					m_nYExcursion -= GetRowHeight(m_cellIndex.y +
										m_nFixRowCount);
					::SetScrollPos(m_hWnd, SB_VERT, m_cellIndex.y, TRUE);
				}
			}
			else if (pt.y > rc.bottom)
			{
				if (m_cellIndex.y < m_nLastRow)
				{
					m_nYExcursion += GetRowHeight(m_cellIndex.y +
										m_nFixRowCount);
					++ m_cellIndex.y;
					::SetScrollPos(m_hWnd, SB_VERT, m_cellIndex.y, TRUE);
				}
			}





			CCellRange range;
			CellID id = CellFromPoint(pt);
			if (id.x >= m_nCols)
				id.x = m_nCols - 1;
			if (id.y >= m_nRows)
				id.y = m_nRows - 1;

			CMyRect rcSelect;

			int l, t, w, h;

			if (m_idClickDown.x < m_nFixColCount)
			{
				// move in left head
				m_idLastSelect = id;
				if (m_idLastSelect.y < m_nFixRowCount)
					m_idLastSelect.y = m_nFixRowCount;
				l = m_nFixColCount;
				t = min(m_idFocus.y, m_idLastSelect.y);
				w = m_nCols - l;
				h = max(m_idFocus.y, m_idLastSelect.y) - t + 1;
			}
			else if (m_idClickDown.y < m_nFixRowCount)
			{
				// move in top head
				m_idLastSelect = id;
				if (m_idLastSelect.x < m_nFixColCount)
					m_idLastSelect.x = m_nFixColCount;
				l = min(m_idFocus.x, m_idLastSelect.x);
				t = m_nFixRowCount;
				w = max(m_idFocus.x, m_idLastSelect.x) - l + 1;
				h = m_nRows - t;
			}
			else
			{
				if (id.x < m_nFixColCount)
					id.x = m_nFixColCount;
				if (id.y < m_nFixRowCount)
					id.y = m_nFixRowCount;
				m_idLastSelect = id;
				l = min(m_idFocus.x, m_idLastSelect.x);
				t = min(m_idFocus.y, m_idLastSelect.y);
				w = max(m_idFocus.x, m_idLastSelect.x) - l + 1;
				h = max(m_idFocus.y, m_idLastSelect.y) - t + 1;
			}

			rcSelect.Set(l, t, w, h);
			m_selectRgn.Init(rcSelect.Left(), rcSelect.Top(),
							rcSelect.Width(), rcSelect.Height());

			range.Set(rcSelect.Left(), rcSelect.Top(), rcSelect.Width(),
					rcSelect.Height());
			OnSelectChanged(range);

			InvalidateRect(m_hWnd, NULL, TRUE);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////

















////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK GridWindowProc(HWND hWnd, UINT msg, WPARAM wParam,
	LPARAM lParam)
{
#ifndef _WIN64
	CGrid* pGrid = (CGrid*) GetWindowLong(hWnd, GWL_USERDATA);
#else
	CGrid* pGrid = (CGrid*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
#endif
	POINT pt;

	PAINTSTRUCT ps;
	HDC hdc;
	if (pGrid)
	{
		switch (msg)
		{
		case WM_SETFOCUS:
			SetFocus(pGrid->m_hWnd);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			pGrid->OnPaint();
			EndPaint(hWnd, &ps);
			break;
		case WM_ERASEBKGND:
			return pGrid->OnEraseBkgnd((HDC) wParam);
			break;
		case WM_SIZE:
			pGrid->OnSize((UINT)wParam, LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_VSCROLL:
			pGrid->OnVScroll((int) LOWORD(wParam), (short int) HIWORD(wParam),
					(HWND) lParam);
			break;
		case WM_HSCROLL:
			pGrid->OnHScroll((int) LOWORD(wParam), (short int) HIWORD(wParam),
					(HWND) lParam);
			break;
		case WM_LBUTTONDOWN:
			SetFocus(pGrid->m_hWnd);
			pt.x = (int) (short) LOWORD(lParam);
			pt.y = (int) (short) HIWORD(lParam);
			pGrid->OnLButtonDown((UINT)wParam, pt);
			break;
		case WM_LBUTTONUP:
			pt.x = (int) (short) LOWORD(lParam);
			pt.y = (int) (short) HIWORD(lParam);
			pGrid->OnLButtonUp((UINT)wParam, pt);
			break;
		case WM_LBUTTONDBLCLK:
			pt.x = (int) (short) LOWORD(lParam);
			pt.y = (int) (short) HIWORD(lParam);
			pGrid->OnLButtonDblClk((UINT)wParam, pt);
			break;
		case WM_RBUTTONDOWN:
			pt.x = (int) (short) LOWORD(lParam);
			pt.y = (int) (short) HIWORD(lParam);
			pGrid->OnRButtonDown((UINT)wParam, pt);
			break;
		case WM_KEYDOWN:
			pGrid->OnKeyDown((UINT) wParam, (UINT) (LOWORD(lParam)),
					(UINT) (HIWORD(lParam)));
			break;
		case WM_CHAR:
			pGrid->OnChar((UINT) wParam, (UINT) (LOWORD(lParam)),
					(UINT) (HIWORD(lParam)));
			break;
		case WM_MOUSEMOVE:
			pt.x = (int) (short) LOWORD(lParam);
			pt.y = (int) (short) HIWORD(lParam);
			pGrid->OnMouseMove((UINT) wParam, pt);
			break;
		case 0x020A:
			//WM_MOUSEWHEEL:
			pt.x = (int) (short) LOWORD(lParam);
			pt.y = (int) (short) HIWORD(lParam);
			pGrid->OnMouseWheel(LOWORD(wParam), (short) HIWORD(wParam), pt);
			break;
		case WM_TIMER:
			pGrid->OnTimer((UINT) wParam);
			break;
		case WM_DESTROY:
			delete pGrid;
#ifndef _WIN64
			::SetWindowLong(hWnd, GWL_USERDATA, 0);
#else
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
#endif
			break;
		case WM_EDITED:
			pGrid->OnEdited(LOWORD(wParam), HIWORD(wParam));
			break;
		case WM_CANCEL_EDIT:
			pGrid->OnCancelEdit(LOWORD(wParam), HIWORD(wParam));
			break;
		}
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
