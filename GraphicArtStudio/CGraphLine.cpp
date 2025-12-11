#include "pch.h"
#include "CGraphLine.h"
#include "CMatrixTool.h"
#include <cmath>

CGraphLine::CGraphLine()
{
	m_start = CPoint(0, 0);
	m_end = CPoint(0, 0);
	m_algoType = 0;
	m_color = RGB(0, 0, 0);
	m_penWidth = 1;
	m_type = SHAPE_LINE;
}

CGraphLine::CGraphLine(CPoint start, CPoint end, int algo, COLORREF color, int width)
{
	m_start = start;
	m_end = end;
	m_algoType = algo;
	m_color = color;
	m_penWidth = width;
	m_type = SHAPE_LINE;
}

CGraphLine::~CGraphLine()
{
}

void CGraphLine::Draw(CDC* pDC)
{
	switch (m_algoType) {
	case 0:
		DrawDDA(pDC);
		break;
	case 1:
		DrawMidBresenham(pDC);
		break;
	case 2:
		DrawBresenham(pDC);
		break;
	case 3:
	default:
		CPen pen(PS_SOLID, m_penWidth, m_color);
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(m_start);
		pDC->LineTo(m_end);
		pDC->SelectObject(pOldPen);
		break;
	}
}

void CGraphLine::DrawDDA(CDC* pDC)
{
	int dx = m_end.x - m_start.x;
	int dy = m_end.y - m_start.y;
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	
	float Xinc = dx / (float)steps;
	float Yinc = dy / (float)steps;
	
	float x = (float)m_start.x;
	float y = (float)m_start.y;
	
	for (int i = 0; i <= steps; i++) {
		pDC->SetPixel((int)round(x), (int)round(y), m_color);
		x += Xinc;
		y += Yinc;
	}
}

void CGraphLine::DrawMidBresenham(CDC* pDC)
{
	int x0 = m_start.x, y0 = m_start.y;
	int x1 = m_end.x, y1 = m_end.y;
	int dx = abs(x1 - x0), dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;
	
	while (true) {
		pDC->SetPixel(x0, y0, m_color);
		if (x0 == x1 && y0 == y1) break;
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void CGraphLine::DrawBresenham(CDC* pDC)
{
	int x0 = m_start.x, y0 = m_start.y;
	int x1 = m_end.x, y1 = m_end.y;
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	
	if (dx > dy) {
		int p = 2 * dy - dx;
		while (x0 != x1) {
			pDC->SetPixel(x0, y0, m_color);
			x0 += sx;
			if (p >= 0) {
				y0 += sy;
				p += 2 * (dy - dx);
			} else {
				p += 2 * dy;
			}
		}
	} else {
		int p = 2 * dx - dy;
		while (y0 != y1) {
			pDC->SetPixel(x0, y0, m_color);
			y0 += sy;
			if (p >= 0) {
				x0 += sx;
				p += 2 * (dx - dy);
			} else {
				p += 2 * dx;
			}
		}
	}
	pDC->SetPixel(x1, y1, m_color);
}

CGraphElement* CGraphLine::Clone()
{
	CGraphLine* pLine = new CGraphLine(m_start, m_end, m_algoType, m_color, m_penWidth);
	pLine->m_isTransformed = m_isTransformed;
	pLine->m_transformLabel = m_transformLabel;
	return pLine;
}

void CGraphLine::Transform(double matrix[3][3])
{
	CMatrixTool::TransformPoint(m_start, matrix);
	CMatrixTool::TransformPoint(m_end, matrix);
}

bool CGraphLine::HitTest(CPoint point)
{
	// Calculate distance from point to line segment
	int dx = m_end.x - m_start.x;
	int dy = m_end.y - m_start.y;
	
	if (dx == 0 && dy == 0) {
		// Line is a point
		int dist = (int)sqrt(pow(point.x - m_start.x, 2) + pow(point.y - m_start.y, 2));
		return dist <= 5;
	}
	
	// Calculate parameter t for closest point on line
	float t = ((point.x - m_start.x) * dx + (point.y - m_start.y) * dy) / 
		(float)(dx * dx + dy * dy);
	
	// Clamp t to [0, 1] to stay on segment
	if (t < 0) t = 0;
	if (t > 1) t = 1;
	
	// Find closest point on segment
	int closestX = (int)(m_start.x + t * dx);
	int closestY = (int)(m_start.y + t * dy);
	
	// Calculate distance
	int dist = (int)sqrt(pow(point.x - closestX, 2) + pow(point.y - closestY, 2));
	
	return dist <= 5;  // 5 pixel tolerance
}

CRect CGraphLine::GetBoundingBox()
{
	int left = min(m_start.x, m_end.x) - 5;
	int top = min(m_start.y, m_end.y) - 5;
	int right = max(m_start.x, m_end.x) + 5;
	int bottom = max(m_start.y, m_end.y) + 5;
	
	return CRect(left, top, right, bottom);
}

void CGraphLine::Move(int dx, int dy)
{
	m_start.x += dx;
	m_start.y += dy;
	m_end.x += dx;
	m_end.y += dy;
}

void CGraphLine::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// 保存
		ar << m_start.x << m_start.y;
		ar << m_end.x << m_end.y;
		ar << m_algoType;
		ar << (DWORD)m_color;
		ar << m_penWidth;
		ar << m_transformLabel;
	}
	else
	{
		// 加载
		ar >> m_start.x >> m_start.y;
		ar >> m_end.x >> m_end.y;
		ar >> m_algoType;
		DWORD color;
		ar >> color;
		m_color = (COLORREF)color;
		ar >> m_penWidth;
		ar >> m_transformLabel;
		m_type = SHAPE_LINE;
		m_selected = false;
	}
}
