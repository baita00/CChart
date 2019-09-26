#include "g_editbox.h"

#define			GRID_EDITBOX_CLASSNAME			_TEXT("Grid_EditBox")

WNDPROC CGridEditBox::lpfnWndProc = 0;

CGridEditBox::CGridEditBox()
{
	if (!lpfnWndProc)
	{
		WNDCLASS wc;
		if (!GetClassInfo(NULL, _TEXT("EDIT"), &wc))
			throw 1;
		lpfnWndProc = wc.lpfnWndProc;
		wc.lpszClassName = GRID_EDITBOX_CLASSNAME;
		wc.lpfnWndProc = CGridEditBox::EditWindowProc;

		if (!RegisterClass(&wc))
		{
			DWORD dw = GetLastError();
		}
	}
}

CGridEditBox::~CGridEditBox()
{
}

HWND CGridEditBox::CreateEdit(DWORD dwStyle, int x, int y, int cx, int cy,
	HWND hWndParent, HINSTANCE hInstance)
{
	m_hWnd = CreateWindow(GRID_EDITBOX_CLASSNAME, NULL,
				dwStyle | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_MULTILINE, x,
				y, cx, cy, hWndParent, NULL, hInstance, NULL);
	if (m_hWnd)
	{
#ifndef _WIN64
		::SetWindowLong(m_hWnd, GWL_USERDATA, (long)this);
#else
		::SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
#endif
		return m_hWnd;
	}
	return NULL;
}

LRESULT	CALLBACK CGridEditBox::EditWindowProc(HWND hWnd, UINT msg,
	WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	CGridEditBox* pEditBox = (CGridEditBox*)
#ifndef _WIN64
		GetWindowLong(hWnd, GWL_USERDATA);
#else
		GetWindowLongPtr(hWnd, GWLP_USERDATA);
#endif
	switch (msg)
	{
	case WM_KEYDOWN:
		if (!pEditBox->OnKeyDown((UINT) wParam, (UINT) (LOWORD(lParam)),
						(UINT) (HIWORD(lParam))))
			return 0;
		break;
	case WM_KEYUP:
		if (!pEditBox->OnKeyUp((UINT) wParam, (UINT) (LOWORD(lParam)),
						(UINT) (HIWORD(lParam))))
			return 0;
		break;
	case WM_CHAR:
		if (!pEditBox->OnChar((UINT) wParam, (UINT) (LOWORD(lParam)),
						(UINT) (HIWORD(lParam))))
			return 0;
		break;
	case WM_SYSCOMMAND:
		pt.x = (int) (short) LOWORD(lParam);
		pt.y = (int) (short) HIWORD(lParam);
		pEditBox->OnSysCommand((UINT) wParam, pt);
		break;
	case WM_DESTROY:
		//		delete pEditBox;
#ifndef _WIN64
		::SetWindowLong(hWnd, GWL_USERDATA, 0);
#else
		::SetWindowLongPtr(hWnd, GWLP_USERDATA, 0);
#endif
		break;
	}

	return ::CallWindowProc(lpfnWndProc, hWnd, msg, wParam, lParam);
}

bool CGridEditBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		::SendMessage(::GetParent(m_hWnd), WM_CANCEL_EDIT, 0, 0);
		::SetFocus(::GetParent(m_hWnd));
		return false;
	}
	else if (nChar == VK_LEFT ||
		nChar == VK_UP ||
		nChar == VK_DOWN ||
		nChar == VK_RIGHT)
	{
		::SendMessage(::GetParent(m_hWnd), WM_EDITED, 0, 0);
		::SetFocus(::GetParent(m_hWnd));
		::PostMessage(::GetParent(m_hWnd), WM_KEYDOWN, (WPARAM) nChar,
			MAKELPARAM(nRepCnt, nFlags));
		return false;
	}

	return true;
}

bool CGridEditBox::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	return true;
}

bool CGridEditBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	short nCtrl = GetKeyState(VK_CONTROL) >> 8;
	short nShift = GetKeyState(VK_SHIFT) >> 8;

	if (nChar == VK_RETURN)
	{
		::SendMessage(::GetParent(m_hWnd), WM_EDITED, 0, 0);
		::SetFocus(::GetParent(m_hWnd));
		::PostMessage(::GetParent(m_hWnd), WM_KEYDOWN, (WPARAM) nChar,
			MAKELPARAM(nRepCnt, nFlags));
		return false;
	}
	return true;
}

void CGridEditBox::OnSysCommand(UINT uCmdType, POINT pt)
{
	if (uCmdType == SC_KEYMENU)
	{
		::SendMessage(m_hWnd, EM_REPLACESEL, (WPARAM) TRUE,
			(LPARAM) _TEXT("\r\n"));
	}
}
/*
void CGridEditBox::SetCellInfo(CellID id, void* pCellInfo)
{
	m_cellID = id;
	m_cellInfo = pCellInfo;

	if(pCellInfo) {
		CGridCell *pGridCell = (CGridCell*)pCellInfo;

		TCHAR sz[50];
		switch(pGridCell->type) {
		case MYVT_NONE:
			::SetWindowText(m_hWnd, _TEXT(""));
			break;
		case MYVT_BOOL:
			if(pGridCell->vt.vt_bool)
				::SetWindowText(m_hWnd, _TEXT("Yes"));
			else
				::SetWindowText(m_hWnd, _TEXT("No"));
			break; 
		case MYVT_CHAR:
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_char);
			::SetWindowText(m_hWnd, sz);
			break;
		case MYVT_UCHAR:
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_uchar);
			::SetWindowText(m_hWnd, sz);
			break;
		case MYVT_SHORT:
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_short);
			::SetWindowText(m_hWnd, sz);
			break;
		case MYVT_USHORT:
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_ushort);
			::SetWindowText(m_hWnd, sz);
			break;
		case MYVT_INT:
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_int);
			::SetWindowText(m_hWnd, sz);
			break;
		case MYVT_UINT:
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_uint);
			::SetWindowText(m_hWnd, sz);
			break;
		case MYVT_LONG:
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_long);
			::SetWindowText(m_hWnd, sz);
			break;
		case MYVT_ULONG:
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_ulong);
			::SetWindowText(m_hWnd, sz);
			break;
		case MYVT_FLOAT:
			_stprintf(sz, _TEXT("%.3f"), pGridCell->vt.vt_float);
			::SetWindowText(m_hWnd, sz);
			break;
		case MYVT_DOUBLE:
			_stprintf(sz, _TEXT("%.3f"), pGridCell->vt.vt_double);
			::SetWindowText(m_hWnd, sz);
			break;
		case MYVT_PTCHAR:
			::SetWindowText(m_hWnd, pGridCell->vt.vt_ptchar);
			break;
		case MYVT_LPARAM:
			::SetWindowText(m_hWnd, _TEXT(""));
			break;
		}

		::SendMessage(m_hWnd, EM_SETSEL, (WPARAM)0, (LPARAM)-1);
	}
	else {
		::SetWindowText(m_hWnd, _TEXT(""));
	}
}

void CGridEditBox::OnHide()
{
	if(m_cellInfo) {
		CMyVariant *pGridCell = (CMyVariant*)m_cellInfo;

		int n = ::SendMessage(m_hWnd, WM_GETTEXTLENGTH, 0, 0);
		if(n <= 10) {
			TCHAR szTmp[11];
			::GetWindowText(m_hWnd, szTmp, 10);

			VT_TYPE type = CMyVariant::GetType(szTmp);
			unsigned long ul;
			long l;
			double d;

			switch(type) {
			case MYVT_NONE:
				pGridCell->Clear();
				break;
			case MYVT_ULONG:
				ul = _ttol(szTmp);
				(*pGridCell) << ul;
				break;
			case MYVT_LONG:
				l = _ttol(szTmp);
				(*pGridCell) << l;
				break;
			case MYVT_DOUBLE:
				d = atof(szTmp);
				(*pGridCell) << d;
				break;
			case MYVT_PTCHAR:
				(*pGridCell) << szTmp;
				break;
			}
		}
		else {
			TCHAR *p = (TCHAR*)malloc(sizeof(TCHAR) * (n + 1));
			::GetWindowText(m_hWnd, p, n);
			(*pGridCell) << p;
			free(p);
		}
	}
}

void CGridEditBox::Hide()
{
	::ShowWindow(m_hWnd, SW_HIDE);
	m_cellInfo = (void*)::SendMessage(::GetParent(m_hWnd), WM_EDITED, MAKEWPARAM(m_cellID.x, m_cellID.y), 0);
	OnHide();
	::SetFocus(::GetParent(m_hWnd));
}
*/