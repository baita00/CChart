#include "mergemanager.h"

#define					MERGE_CELLS_MAX_SIZE		16

CGridMerger::CGridMerger()
{
}

CGridMerger::~CGridMerger()
{
}
//m_mergeB
CMergeCell* CGridMerger::GetMergeCell(int x, int y)
{
	CMergeCell* pMerge = m_mergeA.GetMergeCell(x, y);
	if (pMerge)
		return pMerge;
	else
		return m_mergeB.GetMergeCell(x, y);
}

CMergeCell* CGridMerger::GetMergeCell(CellID id)
{
	return GetMergeCell(id.x, id.y);
}

void CGridMerger::SplitCell(int x, int y)
{
	if (m_mergeA.SplitCell(x, y))
		return ;
	else
		m_mergeB.SplitCell(x, y);
}

CMergeCell* CGridMerger::MergeCells(int x, int y, int cx, int cy,
	RECT* pRectPos)
{
	if (cx * cy >= MERGE_CELLS_MAX_SIZE)
	{
		return m_mergeB.MergeCells(x, y, cx, cy, pRectPos);
	}
	else
	{
		return m_mergeA.MergeCells(x, y, cx, cy, pRectPos);
	}
}

int CGridMerger::GetMergeCellCount()
{
	return m_mergeA.GetMergeCellCount() + m_mergeB.GetMergeCellCount();
}

CMergeCell* CGridMerger::GetMergeCell(int index)
{
	int n = m_mergeA.GetMergeCellCount();
	if (index < n)
	{
		return m_mergeA.GetMergeCell(index);
	}
	else
	{
		return m_mergeB.GetMergeCell(index - n);
	}
}

void CGridMerger::InsertRow(int y, int cy)
{
	m_mergeA.InsertRow(y, cy);
	m_mergeB.InsertRow(y, cy);
}

void CGridMerger::RemoveRow(int y, int cy)
{
	m_mergeA.RemoveRow(y, cy);
	m_mergeB.RemoveRow(y, cy);
}

void CGridMerger::InsertCol(int x, int cx)
{
	m_mergeA.InsertCol(x, cx);
	m_mergeB.InsertCol(x, cx);
}

void CGridMerger::RemoveCol(int x, int cx)
{
	m_mergeA.RemoveCol(x, cx);
	m_mergeB.RemoveCol(x, cx);
}
