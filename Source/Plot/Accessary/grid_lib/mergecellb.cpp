#include "mergecellb.h"

CMergeCell* CMergeCellB::GetMergeCell(int x, int y)
{
	for (int i = 0; i < m_aryMerge.GetSize(); ++ i)
	{
		if (m_aryMerge[i].m_range.IsCellIn(x, y))
		{
			return &m_aryMerge[i];
		}
	}
	return NULL;
}

CMergeCell* CMergeCellB::GetMergeCell(CellID id)
{
	return GetMergeCell(id.x, id.y);
}

bool CMergeCellB::SplitCell(int x, int y)
{
	for (int i = 0; i < m_aryMerge.GetSize(); ++ i)
	{
		if (m_aryMerge[i].m_range.IsCellIn(x, y))
		{
			m_aryMerge.RemoveAt(i);
			return true;;
		}
	}
	return false;
}

CMergeCell* CMergeCellB::MergeCells(int x, int y, int cx, int cy,
	RECT* pRectPos)
{
	if (cx == 1 && cy == 1)
		return NULL;

	CMergeCell merge;
	merge.m_range.Set(x, y, cx, cy);
	if (pRectPos)
		merge.m_rect = *pRectPos;

	int n = m_aryMerge.Add(merge);
	return &m_aryMerge[n];
}

int CMergeCellB::GetMergeCellCount()
{
	return m_aryMerge.GetSize();
}

CMergeCell* CMergeCellB::GetMergeCell(int index)
{
	int n = m_aryMerge.GetSize();
	if (index >= 0 && index < n)
	{
		return &m_aryMerge[index];
	}
	else
		return NULL;
}

void CMergeCellB::InsertRow(int y, int cy)
{
	int n = m_aryMerge.GetSize();
	CCellRange* pRange;
	int nTop, nBottom;
	for (int i = 0; i < n; ++ i)
	{
		pRange = &m_aryMerge[i].m_range;
		nTop = pRange->GetID().y;
		nBottom = pRange->GetBottom();

		if (nBottom > y)
		{
			if (nTop >= y)
				nTop += cy;
			nBottom += cy;

			pRange->Set(pRange->GetID().x, nTop, pRange->GetCX(),
						nBottom - nTop);
		}
	}
}

void CMergeCellB::RemoveRow(int y, int cy)
{
	int n = m_aryMerge.GetSize();
	for (int i = 0; i < n; ++ i)
	{
		CMergeCell* pMerge = &m_aryMerge[i];
		int nTopIndex, nBottomIndex;
		nTopIndex = pMerge->m_range.GetID().y;
		nBottomIndex = pMerge->m_range.GetBottom();


		if (nBottomIndex > y)
		{
			if (nTopIndex < y)
			{
				pMerge->m_range.Set(pMerge->m_range.GetID().x,
									pMerge->m_range.GetID().y,
									pMerge->m_range.GetCX(),
									(pMerge->m_range.GetBottom() >= y + cy) ?
									(pMerge->m_range.GetCY() - cy) :
									(y - pMerge->m_range.GetID().y));
			}
			else if (nTopIndex >= y &&
				nTopIndex <y + cy &&
				nBottomIndex> y + cy)
			{
				pMerge->m_range.Set(pMerge->m_range.GetID().x, y,
									pMerge->m_range.GetCX(),
									pMerge->m_range.GetBottom() - y - cy);
			}
			else if (nTopIndex >= y + cy)
			{
				pMerge->m_range.Set(pMerge->m_range.GetID().x,
									pMerge->m_range.GetID().y - cy,
									pMerge->m_range.GetCX(),
									pMerge->m_range.GetCY());
			}
			else
			{
				m_aryMerge.RemoveAt(i);
				continue;
			}
		}
	}
}

void CMergeCellB::InsertCol(int x, int cx)
{
	int n = m_aryMerge.GetSize();
	CCellRange* pRange;
	int nLeft, nRight;
	for (int i = 0; i < n; ++ i)
	{
		pRange = &m_aryMerge[i].m_range;
		nLeft = pRange->GetID().x;
		nRight = pRange->GetRight();

		if (nRight > x)
		{
			if (nLeft >= x)
				nLeft += cx;
			nRight += cx;

			pRange->Set(nLeft, pRange->GetID().y, nRight - nLeft,
						pRange->GetCY());
		}
	}
}

void CMergeCellB::RemoveCol(int x, int cx)
{
	int n = m_aryMerge.GetSize();
	for (int i = 0; i < n; ++ i)
	{
		CMergeCell* pMerge = &m_aryMerge[i];
		int nLeftIndex, nRightIndex;
		nLeftIndex = pMerge->m_range.GetID().x;
		nRightIndex = pMerge->m_range.GetRight();


		if (nRightIndex > x)
		{
			if (nLeftIndex < x)
			{
				pMerge->m_range.Set(pMerge->m_range.GetID().x,
									pMerge->m_range.GetID().y,
									(nRightIndex >= x + cx) ?
									(pMerge->m_range.GetCX() - cx) :
									(x - pMerge->m_range.GetID().x),
									pMerge->m_range.GetCY());
			}
			else if (nLeftIndex >= x &&
				nLeftIndex <x + cx &&
				nRightIndex> x + cx)
			{
				pMerge->m_range.Set(x, pMerge->m_range.GetID().y,
									pMerge->m_range.GetRight() - x - cx,
									pMerge->m_range.GetCY());
			}
			else if (nLeftIndex >= x + cx)
			{
				pMerge->m_range.Set(pMerge->m_range.GetID().x - cx,
									pMerge->m_range.GetID().y,
									pMerge->m_range.GetCX(),
									pMerge->m_range.GetCY());
			}
			else
			{
				m_aryMerge.RemoveAt(i);
				continue;
			}
		}
	}
}
