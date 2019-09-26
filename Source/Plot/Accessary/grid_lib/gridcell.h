#ifndef _CGRIDCELL_H
#define _CGRIDCELL_H

#include <tchar.h>
#include <stdio.h>
#include <windows.h>

#include "variant.h"
#include "g_checkbox.h"


class CGridCell : public CMyVariant
{
public:
	CGridCell();
	virtual ~CGridCell();
	void					Draw(HDC hdc, RECT& rc, int x, int y, DWORD dwTick);
protected:
	void					DrawVariant(HDC hdc, RECT &rc);
};

#endif