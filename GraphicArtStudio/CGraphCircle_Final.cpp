#include "pch.h"
#include "CGraphCircle.h"
#include "CMatrixTool.h"

CGraphCircle::CGraphCircle()
{
	m_center = CPoint(0, 0);
	m_radius = 10;
	m_color = RGB(0, 0, 0);
	m_penWidth = 1;
	m_filled = false;
	m_fillColor = RGB(255, 255, 255);
	m_type = SHAPE_CIRCLE;
	m_selected = false;
}

CGraphCircle::CGraphCircle(CPoint center, int radius, COLORREF color, int width, bool filled, COLORREF fillColor)
{
	m_center = center;
	m_radius = radius;
	m_color = color;
	m_penWidth = width;
	m_filled = filled;
	m_fillColor = fillColor;
	m_type = SHAPE_CIRCLE;
	m_selected = false;
}

CGraphCircle::~CGraphCircle()
{
}

void CGraphCircle::Draw(CDC* pDC)
{
	DrawMidpointCircle(pDC);
}

void CGraphCircle::DrawMidpointCircle(CDC* pDC)
{
	CPen pen(PS_SOLID, m_penWidth, m_color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	
	if (m_filled)
	{
		CBrush brush(m_fillColor);
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		pDC->Ellipse(m_center.x - m_radius, m_center.y - m_radius,
			m_center.x + m_radius, m_center.y + m_radius);
		pDC->SelectObject(pOldBrush);
	}
	else
	{
		pDC->Arc(m_center.x - m_radius, m_center.y - m_radius,
			m_center.x + m_radius, m_center.y + m_radius,
			m_center.x + m_radius, m_center.y,
			m_center.x + m_radius, m_center.y);
		pDC->Ellipse(m_center.x - m_radius, m_center.y - m_radius,
			m_center.x + m_radius + 1, m_center.y + m_radius + 1);
	}
	
	pDC->SelectObject(pOldPen);
}

void CGraphCircle::DrawCirclePoints(CDC* pDC, int xc, int yc, int x, int y)
{
	pDC->SetPixel(xc + x, yc + y, m_color);
	pDC->SetPixel(xc - x, yc + y, m_color);
	pDC->SetPixel(xc + x, yc - y, m_color);
	pDC->SetPixel(xc - x, yc - y, m_color);
	pDC->SetPixel(xc + y, yc + x, m_color);
	pDC->SetPixel(xc - y, yc + x, m_color);
	pDC->SetPixel(xc + y, yc - x, m_color);
	pDC->SetPixel(xc - y, yc - x, m_color);
}

CGraphElement* CGraphCircle::Clone()
{
	CGraphCircle* pCircle = new CGraphCircle(m_center, m_radius, m_color, m_penWidth, m_filled, m_fillColor);
	pCircle->m_isTransformed = m_isTransformed;
	pCircle->m_transformLabel = m_transformLabel;
	return pCircle;
}

void CGraphCircle::Transform(double matrix[3][3])
{
	CMatrixTool::TransformPoint(m_center, matrix);
	m_isTransformed = true;
}

bool CGraphCircle::HitTest(CPoint point)
{
	int dx = point.x - m_center.x;
	int dy = point.y - m_center.y;
	int dist = (int)sqrt((double)(dx * dx + dy * dy));
	return abs(dist - m_radius) <= 5;
}

CRect CGraphCircle::GetBoundingBox()
{
	return CRect(m_center.x - m_radius - 5, m_center.y - m_radius - 5,
		m_center.x + m_radius + 5, m_center.y + m_radius + 5);
}

void CGraphCircle::Move(int dx, int dy)
{
	m_center.x += dx;
	m_center.y += dy;
}
