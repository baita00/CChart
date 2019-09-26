#ifndef _CGRIDMERGE_H
#define _CGRIDMERGE_H

#include "mergecella.h"
#include "mergecellb.h"

class CGridMerger
{
public:
	CGridMerger();
	virtual ~CGridMerger();
	
	int						GetMergeCellCount();
	CMergeCell*				GetMergeCell(int index);
	CMergeCell*				GetMergeCell(CellID id);
	CMergeCell*				GetMergeCell(int x, int y);

	CMergeCell*				MergeCells(int x, int y, int cx, int cy, RECT* pRectPos);
	void					SplitCell(int x, int y);

	void					InsertRow(int y, int cy = 1);
	void					RemoveRow(int y, int cy = 1);
	void					InsertCol(int x, int cx = 1);
	void					RemoveCol(int x, int cx = 1);

protected:
	CMergeCellA				m_mergeA;
	CMergeCellB				m_mergeB;
};

#endif
