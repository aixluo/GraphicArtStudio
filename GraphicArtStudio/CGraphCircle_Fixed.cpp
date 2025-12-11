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
	int x = 0;
	int y = m_radius;
	int d = 1 - m_radius;
	
	CPen pen(PS_SOLID, m_penWidth, m_color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	
	CBrush brush;
	CBrush* pOldBrush = nullptr;
	
	if (m_filled) {
		brush.CreateSolidBrush(m_fillColor);
		pOldBrush = pDC->SelectObject(&brush);
	}
	
	DrawCirclePoints(pDC, m_center.x, m_center.y, x, y);
	
	while (x < y) {
		if (d < 0) {
			d += 2 * x + 3;
		} else {
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
		DrawCirclePoints(pDC, m_center.x, m_center.y, x, y);
	}
	
	pDC->SelectObject(pOldPen);
	if (pOldBrush) {
		pDC->SelectObject(pOldBrush);
	}
}

void CGraphCircle::DrawCirclePoints(CDC* pDC, int xc, int yc, int x, int y)
{
	if (m_filled) {
		// Fill the circle using horizontal lines
		pDC->MoveTo(xc - x, yc + y);
		pDC->LineTo(xc + x, yc + y);
		pDC->MoveTo(xc - x, yc - y);
		pDC->LineTo(xc + x, yc - y);
		pDC->MoveTo(xc - y, yc + x);
		pDC->LineTo(xc + y, yc + x);
		pDC->MoveTo(xc - y, yc - x);
		pDC->LineTo(xc + y, yc - x);
	} else {
		// Draw circle outline
		pDC->SetPixel(xc + x, yc + y, m_color);
		pDC->SetPixel(xc - x, yc + y, m_color);
		pDC->SetPixel(xc + x, yc - y, m_color);
		pDC->SetPixel(xc - x, yc - y, m_color);
		pDC->SetPixel(xc + y, yc + x, m_color);
		pDC->SetPixel(xc - y, yc + x, m_color);
		pDC->SetPixel(xc + y, yc - x, m_color);
		pDC->SetPixel(xc - y, yc - x, m_color);
	}
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
	double dist = sqrt(dx * dx + dy * dy);
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

void CGraphCircle::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_center.x << m_center.y;
		ar << m_radius;
		ar << (DWORD)m_color;
		ar << m_penWidth;
		ar << m_filled;
		ar << (DWORD)m_fillColor;
		ar << m_transformLabel;
	}
	else
	{
		ar >> m_center.x >> m_center.y;
		ar >> m_radius;
		DWORD color;
		ar >> color;
		m_color = (COLORREF)color;
		ar >> m_penWidth;
		ar >> m_filled;
		ar >> color;
		m_fillColor = (COLORREF)color;
		ar >> m_transformLabel;
		m_type = SHAPE_CIRCLE;
		m_selected = false;
	}
}
