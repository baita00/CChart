#include <tchar.h>
#include <stdio.h>
#include <assert.h>
#include <new.h>
#include <malloc.h>

#include "Windows.h"

#ifndef _CMYARRAY_H
#define _CMYARRAY_H

#define					GROWBY_SIZE				5


template <class T>
void ConstructElement(T* pElement, int nCount)
{
	memset(pElement, 0, sizeof(T) * (nCount));
	for (int i = 0; i < nCount; ++ i)
	{
		::new((void *) &pElement[i]) T;
	}
}

template <class T>
void DestructElement(T* pElement, int nCount)
{
	for (int i = 0; i < nCount; ++ i)
	{
		pElement[i].~T();
	}
}

template <class T>
class CMyArray
{
public:
	CMyArray()
	{
		m_nGrowCount = GROWBY_SIZE;
		m_nSize = 0;
		m_nCount = 0;
		m_pBuf = NULL;
	}
	CMyArray(int nNewSize, int nGrowBy = -1)
	{
		m_nGrowCount = nGrowBy;
		m_nSize = 0;
		m_nCount = 0;
		m_pBuf = NULL;
		SetSize(nNewSize, nGrowBy);
	}
	virtual ~CMyArray()
	{
		for (int i = 0; i < m_nCount; ++ i)
			m_pBuf[i].~T();
		if (m_pBuf)
			delete[] (unsigned char *) m_pBuf;
	}

	CMyArray<T>& operator =(const CMyArray<T>& e)
	{
		SetSize(0, e.m_nGrowCount);
		Append(e);
		return *this;
	}

	int Append(const CMyArray<T>& e)
	{
		if (e.m_nCount == 0)
			return m_nCount;
		int nOldIndex = m_nCount;
		T* p = (T*) new unsigned char[sizeof(T) * (m_nCount + e.m_nCount)];
		if (m_nCount > 0)
		{
			memcpy(p, m_pBuf, sizeof(T) * m_nCount);
			delete[] (unsigned char *) m_pBuf;
		}
		m_pBuf = p;
		ConstructElement<T>(&m_pBuf[m_nCount], e.m_nCount);
		memcpy(&m_pBuf[m_nCount], e.m_pBuf, e.m_nCount * sizeof(T));
		m_nCount += e.m_nCount;
		return nOldIndex;
	}

	void SetSize(int nNewSize, int nGrowBy = -1)
	{
		assert(nNewSize >= 0);
		if (nNewSize == 0)
		{
			for (int i = 0; i < m_nCount; ++ i)
				m_pBuf[i].~T();
			if (m_pBuf)
				delete[] (unsigned char *) m_pBuf;

			m_pBuf = NULL;
			m_nCount = m_nSize = nNewSize;
		}
		else if (!m_pBuf)
		{
			m_pBuf = (T *) new unsigned char[sizeof(T) * nNewSize];
			ConstructElement<T>(m_pBuf, nNewSize);
			m_nCount = m_nSize = nNewSize;
		}
		else if (nNewSize < m_nCount)
		{
			DestructElement<T>(&m_pBuf[nNewSize], m_nCount - nNewSize);
			m_nCount = m_nSize = nNewSize;
		}
		else if (nNewSize > m_nSize)
		{
			T* p = (T*) new unsigned char[sizeof(T) * nNewSize];
			memcpy(p, m_pBuf, sizeof(T) * m_nSize);
			ConstructElement<T>(&p[m_nSize], nNewSize - m_nSize);
			delete[] (unsigned char *) m_pBuf;
			m_pBuf = p;
			m_nCount = m_nSize = nNewSize;
		}
		else
		{
			ConstructElement<T>(&m_pBuf[m_nCount], nNewSize - m_nCount);
			m_nCount = nNewSize;
		}
		m_nGrowCount = (nGrowBy == -1) ? GROWBY_SIZE : nGrowBy;
	}

	T* SetAt(int nIndex, const T& element)
	{
		assert(nIndex >= 0 && nIndex < m_nCount);
		m_pBuf[nIndex] = element;
		return &m_pBuf[nIndex];
	}
	T& GetAt(int nIndex)
	{
		assert(m_pBuf);
		assert(nIndex >= 0 && nIndex < m_nCount);

		return m_pBuf[nIndex];
	}

	int GetSize()
	{
		return m_nCount;
	}

	int GetUpperBound()
	{
		return m_nSize;
	}

	int Add(const T& element)
	{
		if (m_nSize == m_nCount)
		{
			T* p = (T*) new unsigned char[sizeof(T) * (m_nCount + m_nGrowCount)];
			if (m_nCount > 0)
			{
				memcpy(p, m_pBuf, sizeof(T) * m_nCount);
				delete[] (unsigned char *) m_pBuf;
			}
			m_pBuf = p;
			m_nSize += m_nGrowCount;
		}

		ConstructElement<T>(&m_pBuf[m_nCount], 1);
		m_pBuf[m_nCount ++] = element;
		return m_nCount - 1;
	}

	void FreeExtra()
	{
		if (!m_pBuf || m_nSize == m_nCount)
			return ;
		T* p = (T*) new unsigned char[sizeof(T) * m_nCount];
		memcpy(p, m_pBuf, sizeof(T) * m_nCount);

		delete[] (unsigned char *) m_pBuf;
		m_pBuf = p;
		m_nSize = m_nCount;
	}
	T& operator [](int nIndex)
	{
		return GetAt(nIndex);
	}

	void InsertAt(int nIndex, const T& element, int nCount = 1)
	{
		assert(nIndex >= 0);
		assert(nCount > 0);

		if (nIndex >= m_nSize)
		{
			//			SetSize(nIndex + nCount);

			T* p = (T*) new unsigned char[sizeof(T) * (nIndex + nCount)];
			memcpy(p, m_pBuf, sizeof(T) * m_nCount);
			ConstructElement<T>(&p[m_nCount], nIndex + nCount - m_nCount);
			if (m_nCount)
			{
				delete[] (unsigned char *) m_pBuf;
			}
			m_pBuf = p;
			m_nCount = m_nSize = nIndex + nCount;
		}
		else if (nIndex < m_nCount)
		{
			if ((m_nCount + nCount) > m_nSize)
			{
				T* p = (T*) new unsigned char[sizeof(T) * (m_nCount + nCount)];
				memcpy(p, m_pBuf, sizeof(T) * nIndex);
				ConstructElement<T>(&p[nIndex], nCount);
				memcpy(&p[nIndex + nCount], &m_pBuf[nIndex],
					sizeof(T) * (m_nSize - nIndex));
				delete[] (unsigned char *) m_pBuf;
				m_pBuf = p;
				m_nSize = m_nCount + nCount;
			}
			else
			{
				memmove(&m_pBuf[nIndex + nCount], &m_pBuf[nIndex],
					(m_nCount - nIndex) * sizeof(T));
				ConstructElement<T>(&m_pBuf[nIndex], nCount);
			}
			m_nCount += nCount;
		}
		else
		{
			if ((m_nCount + nCount) > m_nSize)
			{
				SetSize(m_nCount + nCount);
			}
			m_nCount = nIndex + nCount;
		}

		while (nCount --)
			m_pBuf[nIndex ++] = element;
	}
	void RemoveAt(int nIndex, int nCount = 1)
	{
		assert(m_pBuf);
		assert(nCount >= 0);
		assert(nIndex + nCount <= m_nCount);

		for (int i = 0; i < nCount; ++ i)
		{
			((T *) m_pBuf)[i + nIndex].~T();
		}

		int nMoveCount = m_nCount - (nIndex + nCount);
		if (nMoveCount)
		{
			memmove(&((T *) m_pBuf)[nIndex], &((T *) m_pBuf)[nIndex + nCount],
				nMoveCount * sizeof(T));
		}
		m_nCount -= nCount;
	}

	void RemoveAll()
	{
		SetSize(0);
	}

protected:
	int m_nGrowCount;
	int m_nSize;
	int m_nCount;
	T* m_pBuf;
};

#endif