#ifndef _CMYMATRIX_H
#define _CMYMATRIX_H

#include "array.h"
#include "variant.h"

template <class T>
class CMyMatrix
{
public:
	CMyMatrix()
	{
		m_nColSize = 0;
	}
	virtual ~CMyMatrix()
	{
		for (int i = 0; i < m_lines.GetSize(); ++ i)
			ClearRow(i);
	}

	T* GetElement(int x, int y)
	{
		CMyArray<T>* pLine = GetLine(y);
		if (pLine)
		{
			if (x >= 0 && x < pLine->GetSize())
				return &(*pLine)[x];
			return NULL;
		}
		return NULL;
	}
	T* SetElement(int x, int y, T& vt)
	{
		assert(x >= 0);
		assert(y >= 0);

		if (y >= m_lines.GetSize())
		{
			InsertRow(m_lines.GetSize(), y - m_lines.GetSize() + 1);
		}

		CMyArray<T>* pLine = GetLine(y);
		if (pLine->GetSize() > x)
		{
			(*pLine)[x] = vt;
		}
		else if (pLine->GetSize() == x)
		{
			pLine->Add(vt);
		}
		else
		{
			pLine->InsertAt(x, vt);
		}
		return &(pLine->GetAt(x));
	}
	CMyArray<T>* GetLine(int y)
	{
		if (y >= 0 && y < m_lines.GetSize())
		{
			return (CMyArray<T>*) ((void *) m_lines[y]);
		}
		return NULL;
	}

	void ClearRow(int y)
	{
		if (y < 0)
			y = 0;
		else if (y >= m_lines.GetSize())
			return ;
		else
		{
			CMyVariant& vt = m_lines[y];
			CMyArray<T>* pLine = (CMyArray<T>*) vt.vt.vt_plparam;
			if (pLine)
			{
				pLine->SetSize(0);
				delete vt.vt.vt_plparam;
				vt.Clear();
			}
		}
	}
	void ClearCol(int x, int cx = 1)
	{
		int n;
		if (x < 0)
			x = 0;
		else
		{
			for (int i = 0; i < m_lines.GetSize(); ++ i)
			{
				CMyVariant& vt = m_lines[i];
				CMyArray<T>* pLine = (CMyArray<T>*) vt.vt.vt_plparam;
				if (pLine)
				{
					if (x < pLine->GetSize())
					{
						if (x + cx > pLine->GetSize())
						{
							n = pLine->GetSize() - x;
						}
						else
						{
							n = cx;
						}

						for (int i = 0; i < n; ++ i)
						{
							T& vt1 = (*pLine)[x + i];
							vt1.Clear();
						}
					}
				}
			}
		}
	}

	void InsertRow(int y, int cy = 1)
	{
		T vt1;

		vt1.type = MYVT_LPARAM;
		vt1.vt.vt_plparam = (void *)new CMyArray<T>(m_nColSize);
		m_lines.InsertAt(y, vt1, cy);

		for (int i = 1; i < cy; ++ i)
		{
			vt1.vt.vt_plparam = (void *)new CMyArray<T>(m_nColSize);
			m_lines[y + i] = vt1;
		}
	}
	void InserColInRow(int y, int x, int cx = 1)
	{
		CMyVariant& vt = m_lines[y];
		CMyArray<T>* pLine = (CMyArray<T>*) vt.vt.vt_plparam;
		if (pLine)
		{
			T vt;
			pLine->InsertAt(x, vt, cx);
		}
	}
	void InsertCol(int x, int cx = 1)
	{
		for (int i = 0; i < m_lines.GetSize(); ++ i)
		{
			InserColInRow(i, x, cx);
		}
	}


	void RemoveRow(int y, int cy = 1)
	{
		if (y < 0)
			y = 0;
		if ((y + cy) > m_lines.GetSize())
			cy = m_lines.GetSize() - y;

		for (int i = 0; i < cy; ++ i)
		{
			ClearRow(i + y);
		}

		m_lines.RemoveAt(y, cy);
	}
	void RemoveColInRow(int y, int x, int cx = 1)
	{
		CMyVariant& vt = m_lines[y];
		CMyArray<T>* pLine = (CMyArray<T>*) vt.vt.vt_plparam;
		if (pLine)
		{
			if (x < pLine->GetSize())
			{
				if (x + cx > pLine->GetSize())
				{
					cx = pLine->GetSize() - x;
				}

				for (int i = 0; i < cx; ++ i)
				{
					T& vt1 = (*pLine)[x + i];
					vt1.Clear();
				}

				pLine->RemoveAt(x, cx);
			}
		}
	}
	void RemoveCol(int x, int cx = 1)
	{
		for (int i = 0; i < m_lines.GetSize(); ++ i)
		{
			RemoveColInRow(i, x, cx);
		}
	}


	void SetRows(int y = 1)
	{
		if (y < m_lines.GetSize())
		{
			RemoveRow(y, m_lines.GetSize() - y);
		}
		else if (y > m_lines.GetSize())
		{
			int n = m_lines.GetSize();
			m_lines.SetSize(y);
			//			CMyVariant vt1;
			//			vt1.type = MYVT_LPARAM;
			for (int i = n; i < y; ++ i)
			{
				//				vt1.vt.vt_plparam = (void*)new CMyArray<T>(m_nColSize);
				//				m_lines[i] = vt1;
				m_lines[i].type = MYVT_LPARAM;
				m_lines[i].vt.vt_plparam = (void *)new CMyArray<T>(m_nColSize);
			}
		}
	}
	void SetCols(int x = 1)
	{
		m_nColSize = x;
	}

	int GetRows()
	{
		return m_lines.GetSize();
	}
	int GetCols()
	{
		return m_nColSize;
	}



protected:
	CMyArray<CMyVariant> m_lines;
	int m_nColSize;
};

#endif






































