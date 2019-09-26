#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include "array.h"

#ifndef _CCELLRANGE_H
#define _CCELLRANGE_H

#define			CellID					POINT

class CCellRange
{
public:
	CCellRange();
	CCellRange(CellID id, int width = 1, int height = 1);
	virtual ~CCellRange();

	CellID&				operator = (const CellID& id);
	CCellRange&			operator = (const CCellRange& e);
	bool				IsCellIn(CellID &id);
	bool				IsCellIn(int x, int y);

	void				SetOrigin(int x = 0, int y = 0);
	void				SetOrigin(CellID& id);
	void				SetRange(int width = 1, int height = 1);

	void				Set(int x, int y, int width = 1, int height = 1);
	void				Set(CellID& id, int width = 1, int height = 1);
	CellID		GetID() const;
	int			GetCX() const;
	int			GetCY() const;
	int			GetRight() const;
	int			GetBottom() const;

protected:
	CellID				m_cellID;
	int					cx, cy;
};

#endif
