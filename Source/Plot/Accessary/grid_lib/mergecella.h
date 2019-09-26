#ifndef _CMERGECELLA_H
#define _CMERGECELLA_H

#include "mergecell.h"

class CMergeCellA
{
public:
	int						GetMergeCellCount();
	CMergeCell*				GetMergeCell(int index);
	CMergeCell*				GetMergeCell(CellID id);
	CMergeCell*				GetMergeCell(int x, int y);

	CMergeCell*				MergeCells(int x, int y, int cx, int cy, RECT* pRectPos);
	bool					SplitCell(int x, int y);

	void					InsertRow(int y, int cy = 1);
	void					RemoveRow(int y, int cy = 1);
	void					InsertCol(int x, int cx = 1);
	void					RemoveCol(int x, int cx = 1);
protected:
	CMyMatrix<CMyVariant>	m_mergeCells;
	CMyArray<CellID>		m_matrixCellID;
};

#endif