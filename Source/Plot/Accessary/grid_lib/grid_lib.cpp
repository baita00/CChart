#include "grid_lib.h"

bool InitGrid(HINSTANCE hInstance)
{
	WNDCLASS wc;
	if (::GetClassInfo(hInstance, GRID_CLASS_NAME, &wc))
		return true;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;//(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = NULL;//LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = GridWindowProc;
	wc.lpszClassName = GRID_CLASS_NAME;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	if (!RegisterClass(&wc))
	{
		return false;
	}
	return true;
}

HWND CreateGrid(HWND hWndParent, HINSTANCE hInst, DWORD dwStyle, int x, int y, int cx, int cy)
{
	HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, GRID_CLASS_NAME, NULL,
					dwStyle |
					WS_TABSTOP |
					WS_HSCROLL |
					WS_VSCROLL |
					WS_CLIPCHILDREN |
					WS_CLIPSIBLINGS,
					x, y, cx, cy, hWndParent, NULL, hInst, NULL);
	if (hWnd)
	{
		UpdateWindow(hWnd);
		SetFocus(hWnd);
		CGrid* pGrid = new CGrid();
		pGrid->m_hWnd = hWnd;
		pGrid->OnSize(0, 0, 0);
#ifndef _WIN64
		::SetWindowLong(hWnd, GWL_USERDATA, (long) pGrid);
#else
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) pGrid);
#endif
		if (pGrid->GetPreferColumn())
		{
			pGrid->GetColumnInfo()->Init(hWnd, hInst);
			pGrid->GetRowInfo()->SetEditBox(pGrid->GetColumnInfo()->GetEditBox());
			int a = pGrid->GetColumnInfo()->GetType(0);
		}
		else
		{
			pGrid->GetRowInfo()->Init(hWnd, hInst);
			pGrid->GetColumnInfo()->SetEditBox(pGrid->GetRowInfo()->GetEditBox());
		}
		pGrid->SetFont((HFONT) ::GetStockObject(ANSI_FIXED_FONT));
	}
	return hWnd;
}

CGrid* GetGrid(HWND hWnd)
{
#ifndef _WIN64
	CGrid* pGrid = (CGrid*) ::GetWindowLong(hWnd, GWL_USERDATA);
#else
	CGrid* pGrid = (CGrid*) ::GetWindowLongPtr(hWnd, GWLP_USERDATA);
#endif
	CGrid* pRet = static_cast<CGrid*>(pGrid);
	if (pRet)
		return pRet;
	else
		return NULL;
}
