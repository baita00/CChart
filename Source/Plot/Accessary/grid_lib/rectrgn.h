#ifndef _CRECTRGN_H
#define _CRECTRGN_H

#include "array.h"

typedef					int					RECT_TYPE;

#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))

enum RECT_RELATION
{
	RECT_AWAY, 
	RECT_EQUAL, 
	RECT_COVER, 
	RECT_JOIN, 
	RECT_CUT, 
	RECT_THROUGH, 
	RECT_CORNER_CUT, 
};

class CMyRect
{
public:
	CMyRect();
	~CMyRect();

	void				Set(RECT_TYPE left, RECT_TYPE top, RECT_TYPE width, RECT_TYPE height);
	void				SetWidth(RECT_TYPE width);
	void				SetHeight(RECT_TYPE height);
	const RECT_TYPE		Left();
	const RECT_TYPE		Top();
	const RECT_TYPE		Right();
	const RECT_TYPE		Bottom();
	const RECT_TYPE		Width();
	const RECT_TYPE		Height();

	bool				IsCellIn(int xPos, int yPos);

	bool		operator == (const CMyRect& e);

protected:
	RECT_TYPE			x, y;
	RECT_TYPE			cx, cy;
};

bool MergeRect(CMyRect *prc1, CMyRect *prc2, CMyRect *pBound, CMyArray<CMyRect>* pRectArray);
void DivideRect(CMyRect *prc1, CMyRect *prc2, CMyRect *pBound, CMyArray<CMyRect>* pRectArray);

class CRectRgn
{
public:
	CRectRgn();
	virtual ~CRectRgn();

	void			Divide(CMyRect* pRect);
	void			Merge(CMyRect* pRect);
	CMyRect*		GetBoundRect();
	int				GetSubRectCount();
	CMyRect*		GetSubRect(int nIndex);
	void			Clear();
	void			Init(CMyRect* pRect);
	void			Init(RECT_TYPE left, RECT_TYPE top, RECT_TYPE width, RECT_TYPE height);
	bool			PtInRgn(int x, int y);

protected:
	CMyRect					m_rect;
	CMyArray<CMyRect>		m_array;
};

#endif
