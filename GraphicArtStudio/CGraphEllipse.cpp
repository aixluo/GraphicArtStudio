#include "pch.h"
#include "CGraphEllipse.h"
#include "CMatrixTool.h"

CGraphEllipse::CGraphEllipse()
{
	m_center = CPoint(0, 0);
	m_rx = 10;
	m_ry = 10;
	m_color = RGB(0, 0, 0);
	m_penWidth = 1;
	m_filled = false;
	m_fillColor = RGB(255, 255, 255);
	m_type = SHAPE_ELLIPSE;
}

CGraphEllipse::CGraphEllipse(CPoint center, int rx, int ry, COLORREF color, int width, bool filled, COLORREF fillColor)
{
	m_center = center;
	m_rx = rx;
	m_ry = ry;
	m_color = color;
	m_penWidth = width;
	m_filled = filled;
	m_fillColor = fillColor;
	m_type = SHAPE_ELLIPSE;
}

CGraphEllipse::~CGraphEllipse()
{
}

void CGraphEllipse::Draw(CDC* pDC)
{
	DrawMidpointEllipse(pDC);
}

void CGraphEllipse::DrawMidpointEllipse(CDC* pDC)
{
	if (m_filled) {
		CBrush brush(m_fillColor);
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		CPen pen(PS_SOLID, m_penWidth, m_color);
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->Ellipse(m_center.x - m_rx, m_center.y - m_ry,
			m_center.x + m_rx, m_center.y + m_ry);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
	else {
		int x = 0;
		int y = m_ry;
		double rx2 = m_rx * m_rx;
		double ry2 = m_ry * m_ry;
		double d1 = ry2 - rx2 * m_ry + 0.25 * rx2;
		double dx = 2 * ry2 * x;
		double dy = 2 * rx2 * y;
		
		while (dx < dy) {
			DrawEllipsePoints(pDC, m_center.x, m_center.y, x, y);
			if (d1 < 0) {
				x++;
				dx += 2 * ry2;
				d1 += dx + ry2;
			} else {
				x++;
				y--;
				dx += 2 * ry2;
				dy -= 2 * rx2;
				d1 += dx - dy + ry2;
			}
		}
		
		double d2 = ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
		while (y >= 0) {
			DrawEllipsePoints(pDC, m_center.x, m_center.y, x, y);
			if (d2 > 0) {
				y--;
				dy -= 2 * rx2;
				d2 += rx2 - dy;
			} else {
				x++;
				y--;
				dx += 2 * ry2;
				dy -= 2 * rx2;
				d2 += dx - dy + rx2;
			}
		}
	}
}

void CGraphEllipse::DrawEllipsePoints(CDC* pDC, int xc, int yc, int x, int y)
{
	pDC->SetPixel(xc + x, yc + y, m_color);
	pDC->SetPixel(xc - x, yc + y, m_color);
	pDC->SetPixel(xc + x, yc - y, m_color);
	pDC->SetPixel(xc - x, yc - y, m_color);
}

CGraphElement* CGraphEllipse::Clone()
{
	CGraphEllipse* pEllipse = new CGraphEllipse(m_center, m_rx, m_ry, m_color, m_penWidth, m_filled, m_fillColor);
	pEllipse->m_isTransformed = m_isTransformed;
	pEllipse->m_transformLabel = m_transformLabel;
	return pEllipse;
}

void CGraphEllipse::Transform(double matrix[3][3])
{
	// Transform center point
	CMatrixTool::TransformPoint(m_center, matrix);
	
	// For scaling, we need to transform the radii
	// Calculate scale factors from the matrix
	double scaleX = sqrt(matrix[0][0] * matrix[0][0] + matrix[1][0] * matrix[1][0]);
	double scaleY = sqrt(matrix[0][1] * matrix[0][1] + matrix[1][1] * matrix[1][1]);
	
	// Scale the radii
	if (scaleX > 0.1 && scaleX < 10.0) {  // Reasonable scale limits
		m_rx = (int)(m_rx * scaleX);
		if (m_rx < 1) m_rx = 1;  // Minimum radius
	}
	if (scaleY > 0.1 && scaleY < 10.0) {
		m_ry = (int)(m_ry * scaleY);
		if (m_ry < 1) m_ry = 1;  // Minimum radius
	}
}

bool CGraphEllipse::HitTest(CPoint point)
{
	int dx = point.x - m_center.x;
	int dy = point.y - m_center.y;
	
	// Ellipse equation: (x/rx)^2 + (y/ry)^2 = 1
	double val = (double)(dx * dx) / (m_rx * m_rx) + (double)(dy * dy) / (m_ry * m_ry);
	
	// Check if point is near the ellipse boundary
	return abs(val - 1.0) <= 0.1;
}

CRect CGraphEllipse::GetBoundingBox()
{
	return CRect(m_center.x - m_rx - 5, m_center.y - m_ry - 5,
		m_center.x + m_rx + 5, m_center.y + m_ry + 5);
}

void CGraphEllipse::Move(int dx, int dy)
{
	m_center.x += dx;
	m_center.y += dy;
}

void CGraphEllipse::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_center.x << m_center.y;
		ar << m_rx << m_ry;
		ar << (DWORD)m_color;
		ar << m_penWidth;
		ar << m_filled;
		ar << (DWORD)m_fillColor;
		ar << m_transformLabel;
	}
	else
	{
		ar >> m_center.x >> m_center.y;
		ar >> m_rx >> m_ry;
		DWORD color;
		ar >> color;
		m_color = (COLORREF)color;
		ar >> m_penWidth;
		ar >> m_filled;
		ar >> color;
		m_fillColor = (COLORREF)color;
		ar >> m_transformLabel;
		m_type = SHAPE_ELLIPSE;
		m_selected = false;
	}
}
