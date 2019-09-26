#include "rectrgn.h"


CMyRect::CMyRect()
{
	x = y = 0;
	cx = cy = 0;
}

CMyRect::~CMyRect()
{
}

void CMyRect::Set(RECT_TYPE left, RECT_TYPE top, RECT_TYPE width,
	RECT_TYPE height)
{
	x = left;
	y = top;
	cx = width;
	cy = height;
}

const RECT_TYPE CMyRect::Left()
{
	return x;
}

const RECT_TYPE CMyRect::Top()
{
	return y;
}

const RECT_TYPE CMyRect::Right()
{
	return x + cx;
}

const RECT_TYPE CMyRect::Bottom()
{
	return y + cy;
}

const RECT_TYPE CMyRect::Width()
{
	return cx;
}

const RECT_TYPE CMyRect::Height()
{
	return cy;
}

bool CMyRect::operator ==(const CMyRect& e)
{
	if (x == e.x && y == e.y && cx == e.cx && cy == e.cy)
		return true;
	return false;
}

void CMyRect::SetWidth(RECT_TYPE width)
{
	cx = width;
}

void CMyRect::SetHeight(RECT_TYPE height)
{
	cy = height;
}

bool CMyRect::IsCellIn(int xPos, int yPos)
{
	if (xPos < x || yPos < y || xPos >= Right() || yPos >= Bottom())
		return false;
	else
		return true;
}




void DivideRect(CMyRect* prc1, CMyRect* prc2, CMyRect* pBound,
	CMyArray<CMyRect>* pRectArray)
{
	CMyRect tmp;
	if ((*prc1 == (*prc2)))
	{
		if (pRectArray)
			pRectArray->RemoveAll();
		if (pBound)
			pBound->Set(0, 0, 0, 0);
		return ;
	}

	RECT_TYPE l = min(prc1->Left(), prc2->Left());
	RECT_TYPE t = min(prc1->Top(), prc2->Top());
	RECT_TYPE r = max(prc1->Right(), prc2->Right());
	RECT_TYPE b = max(prc1->Bottom(), prc2->Bottom());

	if ((r - l >= prc1->Width() + prc2->Width()) ||
		(b - t >= prc1->Height() + prc2->Height()))
	{
		if (pRectArray)
		{
			pRectArray->SetSize(1);
			pRectArray->SetAt(0, *prc1);
		}
		if (pBound)
		{
			*pBound = *prc1;
		}
		return ;
	}
	else if ((r - l <= prc1->Width() + prc2->Width()) &&
		(b - t <= prc1->Height() + prc2->Height()))
	{
		if ((prc1->Left() <= prc2->Left()) &&
			(prc1->Top() <= prc2->Top()) &&
			(prc1->Right() >= prc2->Right()) &&
			(prc1->Bottom() >= prc2->Bottom()))
		{
			// prc2 is in prc1    cover
			if (pRectArray)
			{
				pRectArray->SetSize(4);
				tmp.Set(prc1->Left(), prc1->Top(), prc1->Width(),
						prc2->Top() - prc1->Top());
				if (tmp.Width() != 0 && tmp.Height() != 0)
					pRectArray->SetAt(0, tmp);
				tmp.Set(prc1->Left(), prc2->Top(),
						prc2->Left() - prc1->Left(), prc2->Height());
				if (tmp.Width() != 0 && tmp.Height() != 0)
					pRectArray->SetAt(1, tmp);
				tmp.Set(prc2->Right(), prc2->Top(),
						prc1->Right() - prc2->Right(), prc2->Height());
				if (tmp.Width() != 0 && tmp.Height() != 0)
					pRectArray->SetAt(2, tmp);
				tmp.Set(prc1->Left(), prc2->Bottom(), prc1->Width(),
						prc1->Bottom() - prc2->Bottom());
				if (tmp.Width() != 0 && tmp.Height() != 0)
					pRectArray->SetAt(3, tmp);
			}
			if (pBound)
			{
				*pBound = *prc1;
			}
			return ;
		}
		else if ((prc1->Left() >= prc2->Left()) &&
			(prc1->Top() >= prc2->Top()) &&
			(prc1->Right() <= prc2->Right()) &&
			(prc1->Bottom() <= prc2->Bottom()))
		{
			// prc1 is in prc2    cover
			if (pRectArray)
				pRectArray->RemoveAll();
			if (pBound)
				pBound->Set(0, 0, 0, 0);
			return ;
		}
		else
		{
			if (prc1->Left() < prc2->Left() &&
				prc1->Right() > prc2->Right() &&
				prc1->Top() >= prc2->Top() &&
				prc1->Bottom() <= prc2->Bottom())
			{
				// prc1 is through prc2 in x direction   through
				if (pRectArray)
				{
					pRectArray->SetSize(2);
					tmp.Set(prc1->Left(), prc1->Top(),
							prc2->Left() - prc1->Left(), prc1->Height());
					pRectArray->SetAt(0, tmp);
					tmp.Set(prc2->Right(), prc1->Top(),
							prc1->Right() - prc2->Right(), prc1->Height());
					pRectArray->SetAt(1, tmp);
				}
				if (pBound)
				{
					*pBound = *prc1;
				}
				return ;
			}
			else if (prc1->Left() >= prc2->Left() &&
				prc1->Right() <= prc2->Right() &&
				prc1->Top() < prc2->Top() &&
				prc1->Bottom() > prc2->Bottom())
			{
				// prc2 is through prc1 in x direction   through
				if (pRectArray)
				{
					pRectArray->SetSize(2);
					tmp.Set(prc1->Left(), prc1->Top(), prc1->Width(),
							prc2->Top() - prc1->Top());
					pRectArray->SetAt(0, tmp);
					tmp.Set(prc1->Left(), prc2->Bottom(), prc1->Width(),
							prc1->Bottom() - prc2->Bottom());
					pRectArray->SetAt(1, tmp);
				}
				if (pBound)
				{
					*pBound = *prc1;
				}
				return ;
			}
			else
			{
				if (prc1->Left() < prc2->Left() &&
					prc1->Right() > prc2->Right() &&
					prc1->Top() < prc2->Bottom() &&
					prc1->Bottom() < prc2->Bottom())
				{
					// prc1 cut prc2 in prc2's top
					if (pRectArray)
					{
						pRectArray->SetSize(3);
						tmp.Set(prc1->Left(), prc1->Top(),
								prc2->Left() - prc1->Left(), prc1->Height());
						pRectArray->SetAt(0, tmp);
						tmp.Set(prc2->Left(), prc1->Top(), prc2->Width(),
								prc2->Top() - prc1->Top());
						pRectArray->SetAt(1, tmp);
						tmp.Set(prc2->Right(), prc1->Top(),
								prc1->Right() - prc2->Right(), prc1->Height());
						pRectArray->SetAt(2, tmp);
					}
					if (pBound)
					{
						*pBound = *prc1;
					}
					return ;
				}
				else if (prc1->Left() < prc2->Right() &&
					prc1->Right() < prc2->Right() &&
					prc1->Top() < prc2->Top() &&
					prc1->Bottom() > prc2->Bottom())
				{
					// prc1 cut prc2 in prc2's left
					if (pRectArray)
					{
						pRectArray->SetSize(3);
						tmp.Set(prc1->Left(), prc1->Top(), prc1->Width(),
								prc2->Top() - prc1->Top());
						pRectArray->SetAt(0, tmp);
						tmp.Set(prc1->Left(), prc2->Top(),
								prc2->Left() - prc1->Left(), prc2->Height());
						pRectArray->SetAt(1, tmp);
						tmp.Set(prc1->Left(), prc2->Bottom(), prc1->Width(),
								prc1->Bottom() - prc2->Bottom());
						pRectArray->SetAt(2, tmp);
					}
					if (pBound)
					{
						*pBound = *prc1;
					}
					return ;
				}
				else if (prc1->Left() < prc2->Left() &&
					prc1->Right() > prc2->Right() &&
					prc1->Top() > prc2->Top() &&
					prc1->Bottom() > prc2->Top())
				{
					// prc1 cut prc2 in prc2's bottom
					if (pRectArray)
					{
						pRectArray->SetSize(3);
						tmp.Set(prc1->Left(), prc1->Top(),
								prc2->Left() - prc1->Left(), prc1->Height());
						pRectArray->SetAt(0, tmp);
						tmp.Set(prc2->Left(), prc2->Bottom(), prc2->Width(),
								prc1->Bottom() - prc2->Bottom());
						pRectArray->SetAt(1, tmp);
						tmp.Set(prc2->Right(), prc1->Top(),
								prc1->Right() - prc2->Right(), prc1->Height());
						pRectArray->SetAt(2, tmp);
					}
					if (pBound)
					{
						*pBound = *prc1;
					}
					return ;
				}
				else if (prc1->Left() > prc2->Left() &&
					prc1->Right() > prc2->Left() &&
					prc1->Top() < prc2->Top() &&
					prc1->Bottom() > prc2->Bottom())
				{
					// prc1 cut prc2 in prc2's right
					if (pRectArray)
					{
						pRectArray->SetSize(3);
						tmp.Set(prc1->Left(), prc1->Top(), prc1->Width(),
								prc2->Top() - prc1->Top());
						pRectArray->SetAt(0, tmp);
						tmp.Set(prc2->Right(), prc2->Top(),
								prc1->Right() - prc2->Right(), prc2->Height());
						pRectArray->SetAt(1, tmp);
						tmp.Set(prc1->Left(), prc2->Bottom(), prc1->Height(),
								prc1->Bottom() - prc2->Bottom());
						pRectArray->SetAt(2, tmp);
					}
					if (pBound)
					{
						*pBound = *prc1;
					}
					return ;
				}
				if (prc1->Left() >= prc2->Left() &&
					prc1->Right() <= prc2->Right() &&
					prc1->Top() >= prc2->Top() &&
					prc2->Bottom() > prc1->Top())
				{
					// prc2 cut prc1 in prc1's top
					tmp.Set(prc1->Left(), prc2->Bottom(), prc1->Width(),
							prc1->Bottom() - prc2->Bottom());
					if (pRectArray)
					{
						pRectArray->SetSize(1);
						pRectArray->SetAt(0, tmp);
					}
					if (pBound)
					{
						*pBound = tmp;
					}
					return ;
				}
				else if (prc1->Left() >= prc2->Left() &&
					prc2->Right() > prc1->Left() &&
					prc1->Top() >= prc2->Top() &&
					prc1->Bottom() <= prc2->Bottom())
				{
					// prc2 cut prc1 in prc1's left
					tmp.Set(prc2->Right(), prc1->Top(),
							prc1->Right() - prc2->Right(), prc1->Height());
					if (pRectArray)
					{
						pRectArray->SetSize(1);
						pRectArray->SetAt(0, tmp);
					}
					if (pBound)
					{
						*pBound = tmp;
					}
					return ;
				}
				else if (prc1->Left() >= prc2->Left() &&
					prc1->Right() <= prc2->Right() &&
					prc2->Top() < prc1->Bottom() &&
					prc1->Bottom() <= prc2->Bottom())
				{
					// prc2 cut prc1 in prc1's bottom
					tmp.Set(prc1->Left(), prc1->Top(), prc1->Width(),
							prc2->Top() - prc1->Top());
					if (pRectArray)
					{
						pRectArray->SetSize(1);
						pRectArray->SetAt(0, tmp);
					}
					if (pBound)
					{
						*pBound = tmp;
					}
					return ;
				}
				else if (prc2->Left() < prc1->Right() &&
					prc1->Right() <= prc2->Right() &&
					prc1->Top() >= prc2->Top() &&
					prc1->Bottom() <= prc2->Bottom())
				{
					// prc2 cut prc1 in prc1's right
					tmp.Set(prc1->Left(), prc1->Top(),
							prc2->Left() - prc1->Left(), prc1->Height());
					if (pRectArray)
					{
						pRectArray->SetSize(1);
						pRectArray->SetAt(0, tmp);
					}
					if (pBound)
					{
						*pBound = tmp;
					}
					return ;
				}
				else
				{
					if (prc1->Left() < prc2->Left() &&
						prc1->Right() < prc2->Right() &&
						prc1->Top() < prc2->Top() &&
						prc1->Bottom() < prc2->Bottom())
					{
						// prc1 cut prc2 in prc2's top left
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp.Set(prc1->Left(), prc1->Top(), prc1->Width(),
									prc2->Top() - prc1->Top());
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc1->Left(), prc2->Top(),
									prc2->Left() - prc1->Left(),
									prc1->Bottom() - prc2->Top());
							pRectArray->SetAt(1, tmp);
						}
						if (pBound)
						{
							*pBound = *prc1;
						}
						return ;
					}
					else if (prc1->Left() < prc2->Left() &&
						prc1->Right() < prc2->Right() &&
						prc1->Top() > prc2->Top() &&
						prc1->Bottom() > prc2->Bottom())
					{
						// prc1 cut prc2 in prc2's bottom left
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp.Set(prc1->Left(), prc1->Top(),
									prc2->Left() - prc1->Left(),
									prc2->Bottom() - prc1->Top());
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc1->Left(), prc2->Bottom(),
									prc1->Width(),
									prc1->Bottom() - prc2->Bottom());
							pRectArray->SetAt(1, tmp);
						}
						if (pBound)
						{
							*pBound = *prc1;
						}
						return ;
					}
					else if (prc1->Left() > prc2->Left() &&
						prc1->Left() < prc2->Right() &&
						prc1->Top() > prc2->Top() &&
						prc1->Top() < prc2->Bottom())
					{
						// prc1 cut prc2 in prc2's bottom right
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp.Set(prc2->Right(), prc1->Top(),
									prc1->Right() - prc2->Right(),
									prc2->Bottom() - prc1->Top());
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc1->Left(), prc2->Bottom(),
									prc1->Width(),
									prc1->Bottom() - prc2->Bottom());
							pRectArray->SetAt(1, tmp);
						}
						if (pBound)
						{
							*pBound = *prc1;
						}
						return ;
					}
					else if (prc1->Left() > prc2->Left() &&
						prc1->Right() > prc2->Right() &&
						prc1->Top() < prc2->Top() &&
						prc1->Bottom() < prc2->Bottom())
					{
						// prc1 cut prc2 in prc2's top right
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp.Set(prc1->Left(), prc1->Top(), prc1->Width(),
									prc2->Top() - prc1->Top());
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc2->Right(), prc2->Top(),
									prc1->Right() - prc2->Right(),
									prc1->Bottom() - prc2->Top());
							pRectArray->SetAt(1, tmp);
						}
						if (pBound)
						{
							*pBound = *prc1;
						}
						return ;
					}
					else if (prc1->Left() >= prc2->Left() &&
						prc1->Left() < prc2->Right() &&
						prc1->Top() >= prc2->Top() &&
						prc1->Top() < prc2->Bottom())
					{
						// prc2 cut prc1 in prc1's top left
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp.Set(prc2->Right(), prc1->Top(),
									prc1->Right() - prc2->Right(),
									prc2->Bottom() - prc1->Top());
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc1->Left(), prc2->Bottom(),
									prc1->Width(),
									prc1->Bottom() - prc2->Bottom());
							pRectArray->SetAt(1, tmp);
						}
						if (pBound)
						{
							*pBound = *prc1;
						}
						return ;
					}
					else if (prc1->Left() >= prc2->Left() &&
						prc1->Left() < prc2->Right() &&
						prc1->Top() < prc2->Top() &&
						prc1->Bottom() <= prc2->Bottom())
					{
						// prc2 cut prc1 in prc1's bottom left
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp.Set(prc1->Left(), prc1->Top(), prc1->Width(),
									prc2->Top() - prc1->Top());
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc2->Right(), prc2->Top(),
									prc1->Right() - prc2->Right(),
									prc1->Bottom() - prc2->Top());
							pRectArray->SetAt(1, tmp);
						}
						if (pBound)
						{
							*pBound = *prc1;
						}
						return ;
					}
					else if (prc1->Left() < prc2->Left() &&
						prc1->Right() <= prc2->Right() &&
						prc1->Top() < prc2->Top() &&
						prc1->Bottom() <= prc2->Bottom())
					{
						// prc2 cut prc1 in prc1's bottom right
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp.Set(prc1->Left(), prc1->Top(), prc1->Width(),
									prc2->Top() - prc1->Top());
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc1->Left(), prc2->Top(),
									prc2->Left() - prc1->Left(),
									prc1->Bottom() - prc2->Top());
							pRectArray->SetAt(1, tmp);
						}
						if (pBound)
						{
							*pBound = *prc1;
						}
						return ;
					}
					else if (prc1->Left() < prc2->Left() &&
						prc1->Right() <= prc2->Right() &&
						prc1->Top() >= prc2->Top() &&
						prc1->Bottom() > prc2->Bottom())
					{
						// prc2 cut prc1 in prc1's top right
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp.Set(prc1->Left(), prc1->Top(),
									prc2->Left() - prc1->Left(),
									prc2->Bottom() - prc1->Top());
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc1->Left(), prc2->Bottom(),
									prc1->Width(),
									prc1->Bottom() - prc2->Bottom());
							pRectArray->SetAt(1, tmp);
						}
						if (pBound)
						{
							*pBound = *prc1;
						}
						return ;
					}
				}
			}
		}
	}
}

// return cut or not
bool MergeRect(CMyRect* prc1, CMyRect* prc2, CMyRect* pBound,
	CMyArray<CMyRect>* pRectArray)
{
	if ((*prc1 == (*prc2)))
		return true;

	CMyRect tmp;
	RECT_TYPE l = min(prc1->Left(), prc2->Left());
	RECT_TYPE t = min(prc1->Top(), prc2->Top());
	RECT_TYPE r = max(prc1->Right(), prc2->Right());
	RECT_TYPE b = max(prc1->Bottom(), prc2->Bottom());

	if ((r - l > prc1->Width() + prc2->Width()) ||
		(b - t > prc1->Height() + prc2->Height()))
	{
		// away
		if (pRectArray)
		{
			pRectArray->SetSize(2);
			tmp = *prc1;
			pRectArray->SetAt(0, tmp);
			tmp = *prc2;
			pRectArray->SetAt(1, tmp);
		}

		if (pBound)
			pBound->Set(l, t, r - l, b - t);
		return false;
	}
	else if ((r - l == prc1->Width() + prc2->Width()) &&
		!(prc1->Height() == prc2->Height() && prc1->Top() == prc2->Top()))
	{
		// away
		if (pRectArray)
		{
			pRectArray->SetSize(2);
			tmp = *prc1;
			pRectArray->SetAt(0, tmp);
			tmp = *prc2;
			pRectArray->SetAt(1, tmp);
		}

		if (pBound)
			pBound->Set(l, t, r - l, b - t);
		return false;
	}
	else if ((b - t == prc1->Height() + prc2->Height()) &&
		!(prc1->Width() == prc2->Width() && prc1->Left() == prc2->Left()))
	{
		// away
		if (pRectArray)
		{
			pRectArray->SetSize(2);
			tmp = *prc1;
			pRectArray->SetAt(0, tmp);
			tmp = *prc2;
			pRectArray->SetAt(1, tmp);
		}

		if (pBound)
			pBound->Set(l, t, r - l, b - t);
		return false;
	}
	else
	{
		if ((prc1->Width() == prc2->Width()) &&
			(prc1->Left() == prc2->Left()) &&
			((b - t) <= (prc1->Height() + prc2->Height())))
		{
			// join in vert
			tmp.Set(l, t, r - l, b - t);
			if (pRectArray)
			{
				pRectArray->SetSize(1);
				pRectArray->SetAt(0, tmp);
			}
			if (pBound)
				*pBound = tmp;
			return true;
		}
		else if ((prc1->Height() == prc2->Height()) &&
			(prc1->Top() == prc2->Top()) &&
			((r - l) <= (prc1->Width() + prc2->Width())))
		{
			// join in horz
			tmp.Set(l, t, r - l, b - t);
			if (pRectArray)
			{
				pRectArray->SetSize(1);
				pRectArray->SetAt(0, tmp);
			}
			if (pBound)
				*pBound = tmp;
			return true;
		}
		else if ((r - l < prc1->Width() + prc2->Width()) &&
			(b - t < prc1->Height() + prc2->Height()))
		{
			// cut
			if ((prc1->Left() <= prc2->Left()) &&
				(prc1->Top() <= prc2->Top()) &&
				(prc1->Right() >= prc2->Right()) &&
				(prc1->Bottom() >= prc2->Bottom()))
			{
				// prc2 is in prc1    cover
				if (pRectArray)
				{
					pRectArray->SetSize(1);
					pRectArray->SetAt(0, *prc1);
				}
				if (pBound)
					*pBound = *prc1;
				return true;
			}
			else if ((prc1->Left() >= prc2->Left()) &&
				(prc1->Top() >= prc2->Top()) &&
				(prc1->Right() <= prc2->Right()) &&
				(prc1->Bottom() <= prc2->Bottom()))
			{
				// prc1 is in prc2    cover
				if (pRectArray)
				{
					pRectArray->SetSize(1);
					pRectArray->SetAt(0, *prc2);
				}
				if (pBound)
					*pBound = *prc2;
				return true;
			}
			else
			{
				if (prc1->Left() <= prc2->Left() &&
					prc1->Right() >= prc2->Right() &&
					prc1->Top() >= prc2->Top() &&
					prc1->Bottom() <= prc2->Bottom())
				{
					// prc1 is through prc2 in x direction   through
					if (pRectArray)
					{
						pRectArray->SetSize(3);
						tmp.Set(prc1->Left(), prc1->Top(),
								prc2->Left() - prc1->Left(), prc1->Height());
						pRectArray->SetAt(0, tmp);
						tmp = *prc2;
						pRectArray->SetAt(1, tmp);
						tmp.Set(prc2->Right(), prc1->Top(),
								prc1->Right() - prc2->Right(), prc1->Height());
						pRectArray->SetAt(2, tmp);
					}

					if (pBound)
						pBound->Set(prc1->Left(), prc2->Top(), prc1->Width(),
									prc2->Height());
					return true;
				}
				else if (prc1->Left() >= prc2->Left() &&
					prc1->Right() <= prc2->Right() &&
					prc1->Top() <= prc2->Top() &&
					prc1->Bottom() >= prc2->Bottom())
				{
					// prc2 is through prc1 in x direction   through
					if (pRectArray)
					{
						pRectArray->SetSize(3);
						tmp.Set(prc2->Left(), prc2->Top(),
								prc1->Left() - prc2->Left(), prc2->Height());
						pRectArray->SetAt(0, tmp);
						tmp = *prc1;
						pRectArray->SetAt(1, tmp);
						tmp.Set(prc1->Right(), prc2->Top(),
								prc2->Right() - prc1->Right(), prc2->Height());
						pRectArray->SetAt(2, tmp);
					}

					if (pBound)
						pBound->Set(prc2->Left(), prc1->Top(), prc2->Width(),
									prc1->Height());

					return true;
				}
				else
				{
					if (prc1->Left() <= prc2->Left() &&
						prc1->Right() >= prc2->Right() &&
						prc1->Top() <= prc2->Top() &&
						prc1->Bottom() <= prc2->Bottom())
					{
						// prc1 cut prc2 in prc2's top
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp = *prc1;
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc2->Left(), prc1->Bottom(),
									prc2->Width(),
									prc2->Bottom() - prc1->Bottom());
							pRectArray->SetAt(1, tmp);
						}

						if (pBound)
							pBound->Set(prc1->Left(), prc1->Top(),
										prc1->Width(),
										prc2->Bottom() - prc1->Top());
						return true;
					}
					else if (prc1->Left() <= prc2->Left() &&
						prc1->Right() <= prc2->Right() &&
						prc1->Top() <= prc2->Top() &&
						prc1->Bottom() >= prc2->Bottom())
					{
						// prc1 cut prc2 in prc2's left
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp = *prc1;
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc1->Right(), prc2->Top(),
									prc2->Right() - prc1->Right(),
									prc2->Height());
							pRectArray->SetAt(1, tmp);
						}

						if (pBound)
							pBound->Set(prc1->Left(), prc1->Top(),
										prc2->Right() - prc1->Left(),
										prc1->Height());
						return true;
					}
					else if (prc1->Left() <= prc2->Left() &&
						prc1->Right() >= prc2->Right() &&
						prc1->Top() >= prc2->Top() &&
						prc1->Bottom() >= prc2->Bottom())
					{
						// prc1 cut prc2 in prc2's bottom
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp = *prc1;
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc2->Left(), prc2->Top(), prc2->Width(),
									prc1->Top() - prc2->Top());
							pRectArray->SetAt(1, tmp);
						}

						if (pBound)
							pBound->Set(prc1->Left(), prc2->Top(),
										prc1->Width(),
										prc1->Bottom() - prc2->Top());
						return true;
					}
					else if (prc1->Left() >= prc2->Left() &&
						prc1->Right() >= prc2->Right() &&
						prc1->Top() <= prc2->Top() &&
						prc1->Bottom() >= prc2->Bottom())
					{
						// prc1 cut prc2 in prc2's right
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp = *prc1;
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc2->Left(), prc2->Top(),
									prc1->Left() - prc2->Left(),
									prc2->Height());
							pRectArray->SetAt(1, tmp);
						}

						if (pBound)
							pBound->Set(prc2->Left(), prc1->Top(),
										prc1->Right() - prc2->Left(),
										prc1->Height());
						return true;
					}
					if (prc1->Left() >= prc2->Left() &&
						prc1->Right() <= prc2->Right() &&
						prc1->Top() >= prc2->Top() &&
						prc1->Bottom() >= prc2->Bottom())
					{
						// prc2 cut prc1 in prc1's top
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp = *prc2;
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc1->Left(), prc2->Bottom(),
									prc1->Width(),
									prc1->Bottom() - prc2->Bottom());
							pRectArray->SetAt(1, tmp);
						}

						if (pBound)
							pBound->Set(prc2->Left(), prc2->Top(),
										prc2->Width(),
										prc1->Bottom() - prc2->Top());
						return true;
					}
					else if (prc1->Left() >= prc2->Left() &&
						prc1->Right() >= prc2->Right() &&
						prc1->Top() >= prc2->Top() &&
						prc1->Bottom() <= prc2->Bottom())
					{
						// prc2 cut prc1 in prc1's left
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp = *prc2;
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc2->Right(), prc1->Top(),
									prc1->Right() - prc2->Right(),
									prc1->Height());
							pRectArray->SetAt(1, tmp);
						}

						if (pBound)
							pBound->Set(prc2->Left(), prc2->Top(),
										prc1->Right() - prc2->Left(),
										prc2->Height());
						return true;
					}
					else if (prc1->Left() >= prc2->Left() &&
						prc1->Right() <= prc2->Right() &&
						prc1->Top() <= prc2->Top() &&
						prc1->Bottom() <= prc2->Bottom())
					{
						// prc2 cut prc1 in prc1's bottom
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp = *prc2;
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc1->Left(), prc1->Top(), prc1->Width(),
									prc2->Top() - prc1->Top());
							pRectArray->SetAt(1, tmp);
						}

						if (pBound)
							pBound->Set(prc2->Left(), prc1->Top(),
										prc2->Width(),
										prc2->Bottom() - prc1->Top());
						return true;
					}
					else if (prc1->Left() <= prc2->Left() &&
						prc1->Right() <= prc2->Right() &&
						prc1->Top() >= prc2->Top() &&
						prc1->Bottom() <= prc2->Bottom())
					{
						// prc2 cut prc1 in prc1's right
						if (pRectArray)
						{
							pRectArray->SetSize(2);
							tmp = *prc2;
							pRectArray->SetAt(0, tmp);
							tmp.Set(prc1->Left(), prc1->Top(),
									prc2->Left() - prc1->Left(),
									prc1->Height());
							pRectArray->SetAt(1, tmp);
						}

						if (pBound)
							pBound->Set(prc1->Left(), prc2->Top(),
										prc2->Right() - prc1->Left(),
										prc2->Height());
						return true;
					}
					else
					{
						if (prc1->Left() <= prc2->Left() &&
							prc1->Right() <= prc2->Right() &&
							prc1->Top() <= prc2->Top() &&
							prc1->Bottom() <= prc2->Bottom())
						{
							// prc1 cut prc2 in prc2's top left
							if (pRectArray)
							{
								pRectArray->SetSize(3);
								tmp = *prc2;
								pRectArray->SetAt(0, tmp);
								tmp.Set(prc1->Left(), prc1->Top(),
										prc2->Left() - prc1->Left(),
										prc1->Height());
								pRectArray->SetAt(1, tmp);
								tmp.Set(prc2->Left(), prc1->Top(),
										prc1->Right() - prc2->Left(),
										prc2->Top() - prc1->Top());
								pRectArray->SetAt(2, tmp);
							}

							if (pBound)
								pBound->Set(prc1->Left(), prc1->Top(),
											prc2->Right() - prc1->Left(),
											prc2->Bottom() - prc1->Top());
							return true;
						}
						else if (prc1->Left() <= prc2->Left() &&
							prc1->Right() <= prc2->Right() &&
							prc1->Top() >= prc2->Top() &&
							prc1->Bottom() >= prc2->Bottom())
						{
							// prc1 cut prc2 in prc2's bottom left
							if (pRectArray)
							{
								pRectArray->SetSize(3);
								tmp = *prc2;
								pRectArray->SetAt(0, tmp);
								tmp.Set(prc1->Left(), prc1->Top(),
										prc2->Left() - prc1->Left(),
										prc1->Height());
								pRectArray->SetAt(1, tmp);
								tmp.Set(prc2->Left(), prc2->Bottom(),
										prc1->Right() - prc2->Left(),
										prc1->Bottom() - prc2->Bottom());
								pRectArray->SetAt(2, tmp);
							}

							if (pBound)
								pBound->Set(prc1->Left(), prc2->Top(),
											prc2->Right() - prc1->Left(),
											prc1->Bottom() - prc2->Top());
							return true;
						}
						else if (prc1->Left() >= prc2->Left() &&
							prc1->Left() <= prc2->Right() &&
							prc1->Top() >= prc2->Top() &&
							prc1->Top() <= prc2->Bottom())
						{
							// prc1 cut prc2 in prc2's bottom right
							if (pRectArray)
							{
								pRectArray->SetSize(3);
								tmp = *prc2;
								pRectArray->SetAt(0, tmp);
								tmp.Set(prc1->Left(), prc2->Bottom(),
										prc1->Width(),
										prc1->Bottom() - prc2->Bottom());
								pRectArray->SetAt(1, tmp);
								tmp.Set(prc2->Right(), prc1->Top(),
										prc1->Right() - prc2->Right(),
										prc2->Bottom() - prc1->Top());
								pRectArray->SetAt(2, tmp);
							}

							if (pBound)
								pBound->Set(prc2->Left(), prc2->Top(),
											prc1->Right() - prc2->Left(),
											prc1->Bottom() - prc2->Top());
							return true;
						}
						else if (prc1->Left() >= prc2->Left() &&
							prc1->Right() >= prc2->Right() &&
							prc1->Top() <= prc2->Top() &&
							prc1->Bottom() <= prc2->Bottom())
						{
							// prc1 cut prc2 in prc2's top right
							if (pRectArray)
							{
								pRectArray->SetSize(3);
								tmp = *prc2;
								pRectArray->SetAt(0, tmp);
								tmp.Set(prc1->Left(), prc1->Top(),
										prc1->Width(),
										prc2->Top() - prc1->Top());
								pRectArray->SetAt(1, tmp);
								tmp.Set(prc2->Right(), prc2->Top(),
										prc1->Right() - prc2->Right(),
										prc1->Bottom() - prc2->Top());
								pRectArray->SetAt(2, tmp);
							}

							if (pBound)
								pBound->Set(prc2->Left(), prc1->Top(),
											prc1->Right() - prc2->Left(),
											prc2->Bottom() - prc1->Top());
							return true;
						}
						else if (prc1->Left() >= prc2->Left() &&
							prc1->Left() <= prc2->Right() &&
							prc1->Top() >= prc2->Top() &&
							prc1->Top() <= prc2->Bottom())
						{
							// prc2 cut prc1 in prc1's top left
							if (pRectArray)
							{
								pRectArray->SetSize(3);
								tmp = *prc1;
								pRectArray->SetAt(0, tmp);
								tmp.Set(prc2->Left(), prc2->Top(),
										prc1->Left() - prc2->Left(),
										prc2->Height());
								pRectArray->SetAt(1, tmp);
								tmp.Set(prc1->Left(), prc2->Top(),
										prc2->Right() - prc1->Left(),
										prc1->Top() - prc2->Top());
								pRectArray->SetAt(2, tmp);
							}

							if (pBound)
								pBound->Set(prc2->Left(), prc2->Top(),
											prc1->Right() - prc2->Left(),
											prc1->Bottom() - prc2->Top());
							return true;
						}
						else if (prc1->Left() >= prc2->Left() &&
							prc1->Right() >= prc2->Right() &&
							prc1->Top() <= prc2->Top() &&
							prc1->Bottom() <= prc2->Bottom())
						{
							// prc2 cut prc1 in prc1's bottom left
							if (pRectArray)
							{
								pRectArray->SetSize(3);
								tmp = *prc1;
								pRectArray->SetAt(0, tmp);
								tmp.Set(prc2->Left(), prc2->Top(),
										prc1->Left() - prc2->Left(),
										prc2->Height());
								pRectArray->SetAt(1, tmp);
								tmp.Set(prc1->Left(), prc1->Bottom(),
										prc2->Right() - prc1->Left(),
										prc2->Bottom() - prc1->Bottom());
								pRectArray->SetAt(2, tmp);
							}

							if (pBound)
								pBound->Set(prc2->Left(), prc1->Top(),
											prc1->Right() - prc2->Left(),
											prc2->Bottom() - prc1->Top());
							return true;
						}
						else if (prc1->Left() <= prc2->Left() &&
							prc1->Right() <= prc2->Right() &&
							prc1->Top() <= prc2->Top() &&
							prc1->Bottom() <= prc2->Bottom())
						{
							// prc2 cut prc1 in prc1's bottom right
							if (pRectArray)
							{
								pRectArray->SetSize(3);
								tmp = *prc1;
								pRectArray->SetAt(0, tmp);
								tmp.Set(prc2->Left(), prc1->Bottom(),
										prc2->Width(),
										prc2->Bottom() - prc1->Bottom());
								pRectArray->SetAt(1, tmp);
								tmp.Set(prc1->Right(), prc2->Top(),
										prc2->Right() - prc1->Right(),
										prc1->Bottom() - prc2->Top());
								pRectArray->SetAt(2, tmp);
							}

							if (pBound)
								pBound->Set(prc1->Left(), prc1->Top(),
											prc2->Right() - prc1->Left(),
											prc2->Bottom() - prc1->Top());
							return true;
						}
						else if (prc1->Left() <= prc2->Left() &&
							prc1->Right() <= prc2->Right() &&
							prc1->Top() >= prc2->Top() &&
							prc1->Bottom() >= prc2->Bottom())
						{
							// prc2 cut prc1 in prc1's top right
							if (pRectArray)
							{
								pRectArray->SetSize(3);
								tmp = *prc1;
								pRectArray->SetAt(0, tmp);
								tmp.Set(prc2->Left(), prc2->Top(),
										prc2->Width(),
										prc1->Top() - prc2->Top());
								pRectArray->SetAt(1, tmp);
								tmp.Set(prc1->Right(), prc1->Top(),
										prc2->Right() - prc1->Right(),
										prc2->Bottom() - prc1->Top());
								pRectArray->SetAt(2, tmp);
							}

							if (pBound)
								pBound->Set(prc1->Left(), prc2->Top(),
											prc2->Right() - prc1->Left(),
											prc1->Bottom() - prc2->Top());
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

























CRectRgn::CRectRgn()
{
}

CRectRgn::~CRectRgn()
{
}

void CRectRgn::Merge(CMyRect* pRect)
{
	int i;
	CMyRect rcBound;
	CMyArray<CMyRect> rcMerge, rcCut, tmp;
	bool bMerged = false;
	for (i = 0; i < m_array.GetSize(); ++ i)
	{
		if (MergeRect(&m_array[i], pRect, NULL, &tmp))
		{
			rcMerge.SetSize(i);
			for (int j = 0; j < i; ++ j)
				rcMerge[j] = m_array[j];
			rcMerge.Append(tmp);
			++ i;
			bMerged = true;
			break;
		}
	}

	if (!bMerged)
	{
		m_array.Add(*pRect);
		m_array.FreeExtra();
	}
	else
	{
		for (; i < m_array.GetSize(); ++ i)
		{
			DivideRect(&m_array[i], pRect, NULL, &tmp);
			rcCut.Append(tmp);
		}
		rcMerge.Append(rcCut);
		m_array = rcMerge;
	}

	MergeRect(&m_rect, pRect, &rcBound, NULL);
	m_rect = rcBound;
}

void CRectRgn::Divide(CMyRect* pRect)
{
	int i;
	CMyArray<CMyRect> rects, tmp;
	for (i = 0; i < m_array.GetSize(); ++ i)
	{
		DivideRect(&m_array[i], pRect, NULL, &tmp);
		rects.Append(tmp);
	}
	m_array = rects;

	RECT_TYPE l, t, r, b;
	l = t = 0xfffffff;
	r = b = 0;
	for (i = 0; i < m_array.GetSize(); ++ i)
	{
		if (m_array[i].Left() < l)
			l = m_array[i].Left();
		if (m_array[i].Top() < t)
			t = m_array[i].Top();

		if (m_array[i].Right() > r)
			r = m_array[i].Right();
		if (m_array[i].Bottom() > b)
			b = m_array[i].Bottom();
	}
	m_rect.Set(l, t, r - l, b - t);
}

CMyRect* CRectRgn::GetBoundRect()
{
	return &m_rect;
}

int CRectRgn::GetSubRectCount()
{
	return m_array.GetSize();
}

CMyRect* CRectRgn::GetSubRect(int nIndex)
{
	return &m_array[nIndex];
}

void CRectRgn::Clear()
{
	m_array.RemoveAll();
	m_rect.Set(0, 0, 0, 0);
}

void CRectRgn::Init(CMyRect* pRect)
{
	m_rect = *pRect;
	m_array.SetSize(1);
	m_array.SetAt(0, m_rect);
}

void CRectRgn::Init(RECT_TYPE left, RECT_TYPE top, RECT_TYPE width,
	RECT_TYPE height)
{
	CMyRect rect;
	rect.Set(left, top, width, height);
	Init(&rect);
}

bool CRectRgn::PtInRgn(int x, int y)
{
	if (m_rect.IsCellIn(x, y))
	{
		for (int i = 0; i < m_array.GetSize(); ++ i)
		{
			if (m_array[i].IsCellIn(x, y))
				return true;
		}
		return false;
	}
	else
		return false;
}