#ifndef _CMERGECELL_H
#define _CMERGECELL_H

#include "cellrange.h"
#include "matrix.h"

class CMergeCell
{
public:
	CMergeCell();
	virtual ~CMergeCell();

	CCellRange				m_range;
	DWORD					m_dwDrawTick;
	RECT					m_rect;
};

#endif
