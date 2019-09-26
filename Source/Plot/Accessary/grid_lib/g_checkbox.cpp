#include "g_checkbox.h"

HPEN CGridCheckBox::m_hBorder = NULL;
HPEN CGridCheckBox::m_hValue = NULL;

CGridCheckBox::CGridCheckBox()
{
}

CGridCheckBox::~CGridCheckBox()
{
}

void CGridCheckBox::Init()
{
	m_hBorder = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_hValue = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
}

void CGridCheckBox::Uninit()
{
	DeleteObject(m_hBorder);
	DeleteObject(m_hValue);
}

void CGridCheckBox::Draw(HDC hdc, const RECT& rc, bool bCheck)
{
	RECT rc1;
	GetRange(rc, &rc1);

	HPEN hOldPen;
	if (bCheck)
	{
		hOldPen = (HPEN) SelectObject(hdc, m_hValue);
		MoveToEx(hdc, rc1.left, rc1.top, NULL);
		LineTo(hdc, rc1.right, rc1.bottom);
		MoveToEx(hdc, rc1.right, rc1.top, NULL);
		LineTo(hdc, rc1.left, rc1.bottom);
		SelectObject(hdc, m_hBorder);
	}
	else
	{
		hOldPen = (HPEN) SelectObject(hdc, m_hBorder);
	}

	MoveToEx(hdc, rc1.left, rc1.top, NULL);
	LineTo(hdc, rc1.right, rc1.top);
	LineTo(hdc, rc1.right, rc1.bottom);
	LineTo(hdc, rc1.left, rc1.bottom);
	LineTo(hdc, rc1.left, rc1.top);

	SelectObject(hdc, hOldPen);
}

void CGridCheckBox::GetRange(const RECT& rcCell, RECT* prcRange)
{
	int n = min(rcCell.right - rcCell.left, rcCell.bottom - rcCell.top) - 8;
	int l = rcCell.left + (rcCell.right - rcCell.left) / 2 - n / 2;
	int t = rcCell.top + (rcCell.bottom - rcCell.top) / 2 - n / 2;
	SetRect(prcRange, l, t, l + n, t + n);
}