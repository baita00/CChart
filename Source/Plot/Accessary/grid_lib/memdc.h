#ifndef _CGridMemDC_H
#define _CGridMemDC_H

#define _WINSOCKAPI_
#include <windows.h>

class CGridMemDC
{
public:
	CGridMemDC(HDC hdc, HBRUSH hBkgnd = NULL);
	virtual ~CGridMemDC();

	void			SetBkgBrush(HBRUSH hBrush);

	HDC				m_hdc;
protected:
	RECT			m_rc;
	HDC				m_hOriginDC;
	HBITMAP			m_bmp;
	HBRUSH			m_hBkgnd;
};

#endif