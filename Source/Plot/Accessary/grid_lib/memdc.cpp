#include "memdc.h"

CGridMemDC::CGridMemDC(HDC hdc, HBRUSH hBkgnd)
{
	::GetClipBox(hdc, &m_rc);

	HDC h = ::GetDC(NULL);
	m_hdc = ::CreateCompatibleDC(h);
	m_bmp = ::CreateCompatibleBitmap(h, m_rc.right - m_rc.left,
				m_rc.bottom - m_rc.top);
	::SelectObject(m_hdc, m_bmp);
	::SetWindowOrgEx(m_hdc, m_rc.left, m_rc.top, NULL);
	::SetBkMode(m_hdc, TRANSPARENT);
	m_hOriginDC = hdc;

	SetWindowOrgEx(m_hdc, m_rc.left, m_rc.top, NULL);

	//	HBRUSH b = CreateSolidBrush(RGB(255, 0, 0));
	if (!hBkgnd)
	{
		HBRUSH b = (HBRUSH) GetStockObject(WHITE_BRUSH);
		::FillRect(m_hdc, &m_rc, b);
	}
	else
	{
		::FillRect(m_hdc, &m_rc, hBkgnd);
	}
	//	DeleteObject(b);

	::ReleaseDC(NULL, h);
}

CGridMemDC::~CGridMemDC()
{
	::BitBlt(m_hOriginDC, m_rc.left, m_rc.top, m_rc.right - m_rc.left,
		m_rc.bottom - m_rc.top, m_hdc, m_rc.left, m_rc.top, SRCCOPY);
	DeleteObject(m_bmp);
	DeleteDC(m_hdc);
}

void CGridMemDC::SetBkgBrush(HBRUSH hBrush)
{
	m_hBkgnd = hBrush;
}
