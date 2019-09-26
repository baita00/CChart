#include <tchar.h>
#include <stdio.h>
#include <windows.h>

#ifndef _CGRIDCHECKBOX_H
#define _CGRIDCHECKBOX_H

class CGridCheckBox
{
public:
	CGridCheckBox();
	virtual ~CGridCheckBox();

	static void		Init();
	static void		Uninit();

	static void		GetRange(const RECT& rcCell, RECT* prcRange);
	static void		Draw(HDC hdc, const RECT& rc, bool bCheck);

protected:
	static HPEN		m_hBorder;
	static HPEN		m_hValue;
};

#endif