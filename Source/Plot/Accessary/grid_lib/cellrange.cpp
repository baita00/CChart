#include "cellrange.h"

CCellRange::CCellRange()
{
	m_cellID.x = m_cellID.y = -1;
	cx = cy = 0;
}

CCellRange::~CCellRange()
{
}

CCellRange::CCellRange(CellID id, int width, int height)
{
	m_cellID = id;
	cx = width;
	cy = height;
}

CCellRange& CCellRange::operator =(const CCellRange& e)
{
	m_cellID = e.m_cellID;
	cx = e.cx;
	cy = e.cy;
	return *this;
}

bool CCellRange::IsCellIn(CellID& id)
{
	if (id.x >= m_cellID.x &&
		id.y >= m_cellID.y &&
		id.x < (m_cellID.x + cx) &&
		id.y < (m_cellID.y + cy))
		return true;
	return false;
}

bool CCellRange::IsCellIn(int x, int y)
{
	if (x >= m_cellID.x &&
		y >= m_cellID.y &&
		x < (m_cellID.x + cx) &&
		y < (m_cellID.y + cy))
		return true;
	return false;
}

CellID& CCellRange::operator =(const CellID& id)
{
	m_cellID = id;
	return m_cellID;
}

void CCellRange::SetRange(int width, int height)
{
	cx = width;
	cy = height;
}

void CCellRange::SetOrigin(int x, int y)
{
	m_cellID.x = x;
	m_cellID.y = y;
}

void CCellRange::SetOrigin(CellID& id)
{
	m_cellID = id;
}

void CCellRange::Set(int x, int y, int width, int height)
{
	m_cellID.x = x;
	m_cellID.y = y;
	cx = width;
	cy = height;
}

void CCellRange::Set(CellID& id, int width, int height)
{
	m_cellID = id;
	cx = width;
	cy = height;
}

CellID CCellRange::GetID() const
{
	return m_cellID;
}

int CCellRange::GetCX() const
{
	return cx;
}

int CCellRange::GetCY() const
{
	return cy;
}

int CCellRange::GetRight() const
{
	return m_cellID.x + cx;
}

int CCellRange::GetBottom() const
{
	return m_cellID.y + cy;
}
