#include "mergecella.h"

CMergeCell* CMergeCellA::GetMergeCell(int x, int y)
{
	CMyVariant* pMergeCell = m_mergeCells.GetElement(x, y);
	if (!pMergeCell || pMergeCell->GetType() == MYVT_NONE)
	{
		return NULL;
	}
	else
	{
		return (CMergeCell *) ((void *) (*pMergeCell));
	}
}

CMergeCell* CMergeCellA::GetMergeCell(CellID id)
{
	return GetMergeCell(id.x, id.y);
}

bool CMergeCellA::SplitCell(int x, int y)
{
	CMyVariant* pMergeVar = m_mergeCells.GetElement(x, y);
	if (!pMergeVar)
	{
		return false;
	}
	else
	{
		CMergeCell* pMergeCell = (CMergeCell*) ((void*) (*pMergeVar));
		if (pMergeCell)
		{
			CMyVariant* pvt;
			CCellRange rcRange = pMergeCell->m_range;
			int i, j;

			for (i = 0; i < m_matrixCellID.GetSize(); ++ i)
			{
				if (m_matrixCellID[i].x == rcRange.GetID().x &&
					m_matrixCellID[i].y == rcRange.GetID().y)
				{
					m_matrixCellID.RemoveAt(i);
					break;
				}
			}


			for (j = 0; j < rcRange.GetCY(); ++ j)
			{
				for (i = 0; i < rcRange.GetCX(); ++ i)
				{
					pvt = m_mergeCells.GetElement(rcRange.GetID().x + i,
										rcRange.GetID().y + j);
					if (pvt)
					{
						pvt->Clear();
					}
				}
			}
		}
		return true;
	}
}

CMergeCell* CMergeCellA::MergeCells(int x, int y, int cx, int cy,
	RECT* pRectPos)
{
	if (cx == 1 && cy == 1)
		return NULL;


	CMergeCell* pMerge = new CMergeCell();
	pMerge->m_range.Set(x, y, cx, cy);
	if (pRectPos)
		pMerge->m_rect = *pRectPos;

	CMyVariant vt;
	vt = pMerge;

	if (m_mergeCells.GetRows() < (y + cy))
	{
		m_mergeCells.SetRows(y + cy);
	}

	CMyVariant* pvt;
	for (int i = 0; i < cy; ++ i)
	{
		CMyArray<CMyVariant>* pLine = m_mergeCells.GetLine(i + y);
		if (pLine->GetSize() < (x + cx))
		{
			pLine->SetSize(x + cx);
		}

		for (int j = 0; j < cx; ++ j)
		{
			pvt = pLine->SetAt(j + x, vt);
			if (j == 0 && i == 0)
			{
				pvt->bRelease = true;
			}
		}
	}
	CellID id =
	{
		x, y
	};
	m_matrixCellID.Add(id);
	return pMerge;
}

int CMergeCellA::GetMergeCellCount()
{
	return m_matrixCellID.GetSize();
}

CMergeCell* CMergeCellA::GetMergeCell(int index)
{
	int n = m_matrixCellID.GetSize();
	if (index < n)
	{
		CellID id = m_matrixCellID[index];
		CMyVariant* pMergeCell = m_mergeCells.GetElement(id.x, id.y);
		if (!pMergeCell)
			return NULL;
		return (CMergeCell *) ((void *) (*pMergeCell));
	}
	else
		return NULL;
}

void CMergeCellA::InsertRow(int y, int cy)
{
	m_mergeCells.InsertRow(y, cy);

	CellID id;
	int n = m_matrixCellID.GetSize();
	for (int i = 0; i < n; ++ i)
	{
		id = m_matrixCellID[i];

		if (id.y >= y)
			id.y += cy;

		CMergeCell* pMerge = GetMergeCell(id);
		int nTopIndex, nBottomIndex;

		nTopIndex = pMerge->m_range.GetID().y;
		nBottomIndex = pMerge->m_range.GetBottom();

		if (nTopIndex >= y)
			nTopIndex += cy;
		if (nBottomIndex > y)
			nBottomIndex += cy;


		if (nBottomIndex > y)
		{
			if (nTopIndex < y)
			{
				pMerge->m_range.Set(pMerge->m_range.GetID().x, nTopIndex,
									pMerge->m_range.GetCX(),
									pMerge->m_range.GetCY() + cy);

				CMyVariant vt;
				vt = pMerge;

				for (int i = 0; i < cy; ++ i)
				{
					CMyArray<CMyVariant>* pLine = m_mergeCells.GetLine(i + y);
					if (pLine->GetSize() < pMerge->m_range.GetRight())
					{
						pLine->SetSize(pMerge->m_range.GetRight());
					}

					for (int j = pMerge->m_range.GetID().x;
						j < pMerge->m_range.GetRight();
						++ j)
					{
						pLine->SetAt(j, vt);
					}
				}
			}
			else
			{
				pMerge->m_range.Set(pMerge->m_range.GetID().x, nTopIndex,
									pMerge->m_range.GetCX(),
									pMerge->m_range.GetCY());

				m_matrixCellID[i].y += cy;
			}
		}
	}
}

void CMergeCellA::RemoveRow(int y, int cy)
{
	int n = m_matrixCellID.GetSize();
	for (int i = 0; i < n; ++ i)
	{
		CMergeCell* pMerge = GetMergeCell(i);
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
				m_mergeCells.GetElement(pMerge->m_range.GetID().x, pMerge->m_range.GetID().y)->bRelease = false;
				m_mergeCells.GetElement(pMerge->m_range.GetID().x, y + cy)->bRelease = true;

				pMerge->m_range.Set(pMerge->m_range.GetID().x, y,
									pMerge->m_range.GetCX(),
									pMerge->m_range.GetBottom() - y - cy);

				m_matrixCellID[i].y = y;
			}
			else if (nTopIndex >= y + cy)
			{
				pMerge->m_range.Set(pMerge->m_range.GetID().x,
									pMerge->m_range.GetID().y - cy,
									pMerge->m_range.GetCX(),
									pMerge->m_range.GetCY());
				m_matrixCellID[i].y -= cy;
			}
			else
			{
				m_matrixCellID.RemoveAt(i);
				continue;
			}
		}
	}

	if (y + cy < m_mergeCells.GetRows())
	{
		m_mergeCells.RemoveRow(y, cy);
	}
	else if (y < m_mergeCells.GetRows() && (y + cy) >= m_mergeCells.GetRows())
	{
		m_mergeCells.RemoveRow(y, m_mergeCells.GetRows() - y);
	}
}

void CMergeCellA::InsertCol(int x, int cx)
{
	m_mergeCells.InsertCol(x, cx);

	CellID id;
	int n = m_matrixCellID.GetSize();
	for (int i = 0; i < n; ++ i)
	{
		id = m_matrixCellID[i];

		if (id.x >= x)
			id.x += cx;

		CMergeCell* pMerge = GetMergeCell(id);
		int nLeftIndex, nRightIndex;

		nLeftIndex = pMerge->m_range.GetID().x;
		nRightIndex = pMerge->m_range.GetRight();

		if (nLeftIndex >= x)
			nLeftIndex += cx;
		if (nRightIndex > x)
			nRightIndex += cx;


		if (nRightIndex > x)
		{
			if (nLeftIndex < x)
			{
				pMerge->m_range.Set(nLeftIndex, pMerge->m_range.GetID().y,
									pMerge->m_range.GetCX() + cx,
									pMerge->m_range.GetCY());

				CMyVariant vt;
				vt = pMerge;

				for (int i = pMerge->m_range.GetID().y;
					i < pMerge->m_range.GetBottom();
					++ i)
				{
					CMyArray<CMyVariant>* pLine = m_mergeCells.GetLine(i);
					for (int j = 0; j < cx; ++ j)
					{
						pLine->SetAt(x + j, vt);
					}
				}
			}
			else
			{
				pMerge->m_range.Set(nLeftIndex, pMerge->m_range.GetID().y,
									pMerge->m_range.GetCX(),
									pMerge->m_range.GetCY());

				m_matrixCellID[i].x += cx;
			}
		}
	}
}

void CMergeCellA::RemoveCol(int x, int cx)
{
	int n = m_matrixCellID.GetSize();
	for (int i = 0; i < n; ++ i)
	{
		CMergeCell* pMerge = GetMergeCell(i);
		int nLeftIndex, nRightIndex;
		nLeftIndex = pMerge->m_range.GetID().x;
		nRightIndex = pMerge->m_range.GetRight();


		if (nRightIndex > x)
		{
			if (nLeftIndex < x)
			{
				pMerge->m_range.Set(pMerge->m_range.GetID().x,
									pMerge->m_range.GetID().y,
									(pMerge->m_range.GetRight() >= x + cx) ?
									(pMerge->m_range.GetCX() - cx) :
									(x - pMerge->m_range.GetID().x),
									pMerge->m_range.GetCY());
			}
			else if (nLeftIndex >= x &&
				nLeftIndex <x + cx &&
				nRightIndex> x + cx)
			{
				m_mergeCells.GetElement(pMerge->m_range.GetID().x, pMerge->m_range.GetID().y)->bRelease = false;
				m_mergeCells.GetElement(x + cx, pMerge->m_range.GetID().y)->bRelease = true;

				pMerge->m_range.Set(x, pMerge->m_range.GetID().y,
									pMerge->m_range.GetRight() - x - cx,
									pMerge->m_range.GetCY());

				m_matrixCellID[i].x = x;
			}
			else if (nLeftIndex >= x + cx)
			{
				pMerge->m_range.Set(pMerge->m_range.GetID().x - cx,
									pMerge->m_range.GetID().y,
									pMerge->m_range.GetCX(),
									pMerge->m_range.GetCY());
				m_matrixCellID[i].x -= cx;
			}
			else
			{
				m_matrixCellID.RemoveAt(i);
				continue;
			}
		}
	}

	m_mergeCells.RemoveCol(x, cx);
}
