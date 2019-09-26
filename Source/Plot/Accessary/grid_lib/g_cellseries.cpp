#include "g_cellseries.h"

CGridCellSeries::CGridCellSeries()
{
	m_editBox = new CGridEditBox;
	m_bEditBoxCreated = true;
}

CGridCellSeries::~CGridCellSeries()
{
	if (m_editBox && m_bEditBoxCreated)
	{
		delete m_editBox;
		m_editBox = 0;
	}
}

VT_TYPE CGridCellSeries::GetType(int nIndex)
{
	if (nIndex >= m_cellseries.GetSize())
		return MYVT_NONE;
	else
		return m_cellseries.GetAt(nIndex);
}

void CGridCellSeries::SetType(int nIndex, VT_TYPE type, int nCount)
{
	int nOldSize = m_cellseries.GetSize();
	if (nIndex > nOldSize)
	{
		m_cellseries.InsertAt(nOldSize, MYVT_NONE, nIndex - nOldSize);
		m_cellseries.InsertAt(nIndex, type, nCount);
	}
	else if ((nIndex + nCount) <= m_cellseries.GetSize())
	{
		for (int i = 0; i < nCount; ++ i)
			m_cellseries[nIndex + i] = type;
	}
	else
	{
		m_cellseries.SetSize(nIndex + nCount);
		for (int i = nIndex; i < nIndex + nCount; ++ i)
		{
			m_cellseries[i] = type;
		}
	}
}

HWND CGridCellSeries::GetEditHandle(int nIndex)
{
	VT_TYPE type = GetType(nIndex);
	if (type == MYVT_NONE ||
		type == MYVT_CHAR ||
		type == MYVT_UCHAR ||
		type == MYVT_SHORT ||
		type == MYVT_USHORT ||
		type == MYVT_INT ||
		type == MYVT_UINT ||
		type == MYVT_LONG ||
		type == MYVT_ULONG ||
		type == MYVT_FLOAT ||
		type == MYVT_DOUBLE ||
		type == MYVT_PTCHAR ||
		type == MYVT_LPARAM)
	{
		return m_editBox->m_hWnd;
	}
	else
	{
		return NULL;
	}
}

bool CGridCellSeries::Init(HWND hWnd, HINSTANCE hInst)
{
	m_hParent = hWnd;
	HWND h = m_editBox->CreateEdit(WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, hWnd,
							hInst);
	return h != NULL;
}

void CGridCellSeries::Uninit()
{
}

HWND CGridCellSeries::EditCell(int nIndex, RECT& rcCell, void* pCellInfo,
	UINT nChar)
{
	CGridCell* pGridCell = (CGridCell*) pCellInfo;
	VT_TYPE type;
	if (pGridCell && pGridCell->type != MYVT_NONE)
	{
		type = pGridCell->type;
	}
	else
	{
		type = GetType(nIndex);
	}

	HWND hWnd = GetEditHandle(nIndex);
	if (!hWnd)
		return NULL;

	TCHAR sz[50];
	//tostringstream ostr;
	//tstring sz;
	switch (type)
	{
	case MYVT_NONE:
		::SetWindowText(hWnd, _TEXT(""));
		break;
	case MYVT_BOOL:
		return NULL;
		break; 
	case MYVT_CHAR:
		if (pCellInfo)
		{
			//ostr<<pGridCell->vt.vt_char;
			//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 50, _TEXT("%d"), pGridCell->vt.vt_char);
#else
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_char);
#endif
			
		}
		else
		{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_tcscpy_s(sz, 50, _TEXT(""));
#else
			_tcscpy(sz, _TEXT(""));
#endif
			
			//sz = _TEXT("");
		}
		::SetWindowText(hWnd, sz);
		break;
	case MYVT_UCHAR:
		if (pCellInfo)
		{
			//ostr<<pGridCell->vt.vt_uchar;
			//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 50, _TEXT("%d"), pGridCell->vt.vt_uchar);
#else
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_uchar);
#endif
			
		}
		else
		{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_tcscpy_s(sz, 50, _TEXT(""));
#else
			_tcscpy(sz, _TEXT(""));
#endif
			//sz = _TEXT("");
		}
		::SetWindowText(hWnd, sz);
		break;
	case MYVT_SHORT:
		if (pCellInfo)
		{
			//ostr<<pGridCell->vt.vt_short;
			//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 50, _TEXT("%d"), pGridCell->vt.vt_short);
#else
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_short);
#endif
			
		}
		else
		{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_tcscpy_s(sz, 50, _TEXT(""));
#else
			_tcscpy(sz, _TEXT(""));
#endif
			//sz = _TEXT("");
		}
		::SetWindowText(hWnd, sz);
		break;
	case MYVT_USHORT:
		if (pCellInfo)
		{
			//ostr<<pGridCell->vt.vt_ushort;
			//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 50, _TEXT("%d"), pGridCell->vt.vt_ushort);
#else
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_ushort);
#endif
			
		}
		else
		{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_tcscpy_s(sz, 50, _TEXT(""));
#else
			_tcscpy(sz, _TEXT(""));
#endif
			//sz = _TEXT("");
		}
		::SetWindowText(hWnd, sz);
		break;
	case MYVT_INT:
		if (pCellInfo)
		{
			//ostr<<pGridCell->vt.vt_int;
			//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 50, _TEXT("%d"), pGridCell->vt.vt_int);
#else
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_int);
#endif
			
		}
		else
		{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_tcscpy_s(sz, 50, _TEXT(""));
#else
			_tcscpy(sz, _TEXT(""));
#endif
			//sz = _TEXT("");
		}
		::SetWindowText(hWnd, sz);
		break;
	case MYVT_UINT:
		if (pCellInfo)
		{
			//ostr<<pGridCell->vt.vt_uint;
			//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 50, _TEXT("%d"), pGridCell->vt.vt_uint);
#else
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_uint);
#endif
			
		}
		else
		{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_tcscpy_s(sz, 50, _TEXT(""));
#else
			_tcscpy(sz, _TEXT(""));
#endif
			//sz = _TEXT("");
		}
		::SetWindowText(hWnd, sz);
		break;
	case MYVT_LONG:
		if (pCellInfo)
		{
			//ostr<<pGridCell->vt.vt_long;
			//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 50, _TEXT("%d"), pGridCell->vt.vt_long);
#else
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_long);
#endif
			
		}
		else
		{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_tcscpy_s(sz, 50, _TEXT(""));
#else
			_tcscpy(sz, _TEXT(""));
#endif
			//sz = _TEXT("");
		}
		::SetWindowText(hWnd, sz);
		break;
	case MYVT_ULONG:
		if (pCellInfo)
		{
			//ostr<<pGridCell->vt.vt_ulong;
			//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 50, _TEXT("%d"), pGridCell->vt.vt_ulong);
#else
			_stprintf(sz, _TEXT("%d"), pGridCell->vt.vt_ulong);
#endif
			
		}
		else
		{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_tcscpy_s(sz, 50, _TEXT(""));
#else
			_tcscpy(sz, _TEXT(""));
#endif
			//sz = _TEXT("");
		}
		::SetWindowText(hWnd, sz);
		break;
	case MYVT_FLOAT:
		if (pCellInfo)
		{
			//ostr<<pGridCell->vt.vt_float;
			//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 50, _TEXT("%.3f"), pGridCell->vt.vt_float);
#else
			_stprintf(sz, _TEXT("%.3f"), pGridCell->vt.vt_float);
#endif
			
		}
		else
		{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_tcscpy_s(sz, 50, _TEXT(""));
#else
			_tcscpy(sz, _TEXT(""));
#endif
			//sz = _TEXT("");
		}
		::SetWindowText(hWnd, sz);
		break;
	case MYVT_DOUBLE:
		if (pCellInfo)
		{
			//ostr<<pGridCell->vt.vt_double;
			//sz = ostr.str();
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_stprintf_s(sz, 50, _TEXT("%.3f"), pGridCell->vt.vt_double);
#else
			_stprintf(sz, _TEXT("%.3f"), pGridCell->vt.vt_double);
#endif
			
		}
		else
		{
#if defined(_MSC_VER) && (_MSC_VER>=1300)
			_tcscpy_s(sz, 50, _TEXT(""));
#else
			_tcscpy(sz, _TEXT(""));
#endif
			//sz = _TEXT("");
		}
		::SetWindowText(hWnd, sz);
		break;
	case MYVT_PTCHAR:
		if (pCellInfo)
			::SetWindowText(hWnd, pGridCell->vt.vt_ptchar);
		else
			::SetWindowText(hWnd, _TEXT(""));
		break;
	case MYVT_LPARAM:
		return NULL;
		break;
	}

	::MoveWindow(hWnd, rcCell.left, rcCell.top, rcCell.right - rcCell.left,
		rcCell.bottom - rcCell.top, false);
	::SendMessage(hWnd, EM_SETSEL, (WPARAM) 0, (LPARAM) - 1);
	if (nChar != 0)
		::SendMessage(hWnd, WM_CHAR, (WPARAM) nChar, 0);
	::ShowWindow(hWnd, SW_SHOW);
	::SetFocus(hWnd);
	return hWnd;
}

void CGridCellSeries::SetFont(HFONT hFont)
{
	::SendMessage(m_editBox->m_hWnd, WM_SETFONT, (WPARAM) hFont, 0);
}
/*
void CGridCellSeries::OnHide()
{
	HWND hWnd = GetEditHandle(m_cellID.x);
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
*/