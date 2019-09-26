#include "gridcell.h"

CGridCell::CGridCell()
{
}

CGridCell::~CGridCell()
{
}

void CGridCell::Draw(HDC hdc, RECT& rc, int x, int y, DWORD dwTick)
{
	DrawVariant(hdc, rc);
}

void CGridCell::DrawVariant(HDC hdc, RECT& rc)
{
	TCHAR sz[50];
	//tostringstream ostr;
	//tstring sz;
	switch (GetType())
	{
	case MYVT_NONE:
		break;
	case MYVT_BOOL:
		/*		if(vt.vt_bool)
						DrawText(hdc, _TEXT("Yes"), 3, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
					else
						DrawText(hdc, _TEXT("No"), 2, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);*/
		CGridCheckBox::Draw(hdc, rc, vt.vt_bool);
		break; 
	case MYVT_CHAR:
		//ostr<<vt.vt_char;
		//sz = ostr.str();
		//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(sz, 50, _TEXT("%d"), vt.vt_char);
#else
		_stprintf(sz, _TEXT("%d"), vt.vt_char);
#endif
		
		DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_UCHAR:
		//ostr<<vt.vt_uchar;
		//sz = ostr.str();
		//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(sz, 50, _TEXT("%d"), vt.vt_uchar);
#else
		_stprintf(sz, _TEXT("%d"), vt.vt_uchar);
#endif
		
		DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_SHORT:
		//ostr<<vt.vt_short;
		//sz = ostr.str();
		//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(sz, 50, _TEXT("%d"), vt.vt_short);
#else
		_stprintf(sz, _TEXT("%d"), vt.vt_short);
#endif
		
		DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_USHORT:
		//ostr<<vt.vt_ushort;
		//sz = ostr.str();
		//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(sz, 50, _TEXT("%d"), vt.vt_ushort);
#else
		_stprintf(sz, _TEXT("%d"), vt.vt_short);
#endif
		
		DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_INT:
		//ostr<<vt.vt_int;
		//sz = ostr.str();
		//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(sz, 50, _TEXT("%d"), vt.vt_int);
#else
		_stprintf(sz, _TEXT("%d"), vt.vt_int);
#endif
		
		DrawText(hdc, sz, (int)_tcslen(sz), &rc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_UINT:
		//ostr<<vt.vt_uint;
		//sz = ostr.str();
		//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(sz, 50, _TEXT("%d"), vt.vt_uint);
#else
		_stprintf(sz, _TEXT("%d"), vt.vt_uint);
#endif
		
		DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_LONG:
		//ostr<<vt.vt_long;
		//sz = ostr.str();
		//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(sz, 50, _TEXT("%d"), vt.vt_long);
#else
		_stprintf(sz, _TEXT("%d"), vt.vt_long);
#endif
		
		DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_ULONG:
		//ostr<<vt.vt_ulong;
		//sz = ostr.str();
		//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(sz, 50, _TEXT("%d"), vt.vt_ulong);
#else
		_stprintf(sz, _TEXT("%d"), vt.vt_ulong);
#endif
		
		DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_FLOAT:
		//ostr<<vt.vt_float;
		//sz = ostr.str();
		//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(sz, 50, _TEXT("%g"), vt.vt_float);
#else
		_stprintf(sz, _TEXT("%g"), vt.vt_float);
#endif
		
		DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_DOUBLE:
		//ostr<<vt.vt_double;
		//sz = ostr.str();
		//DrawText(hdc, sz.c_str(), sz.length(), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
#if defined(_MSC_VER) && (_MSC_VER>=1300)
		_stprintf_s(sz, 50, _TEXT("%g"), vt.vt_double);
#else
		_stprintf(sz, _TEXT("%g"), vt.vt_double);
#endif
		
		DrawText(hdc, sz, (int)_tcslen(sz), &rc,	DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
		break;
	case MYVT_PTCHAR:
		DrawText(hdc, vt.vt_ptchar, (int)_tcslen(vt.vt_ptchar), &rc,	DT_LEFT | DT_VCENTER | DT_WORD_ELLIPSIS);
		break;
	case MYVT_LPARAM:
		break;
	}
}
