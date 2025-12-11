#include "pch.h"
#include "CGraphBezier.h"
#include "CMatrixTool.h"
#include <cmath>

CGraphBezier::CGraphBezier()
{
	m_color = RGB(0, 0, 0);
	m_penWidth = 1;
	m_polyColor = RGB(150, 150, 150);
	m_type = SHAPE_BEZIER;
	m_showControlPoints = true;
}

CGraphBezier::CGraphBezier(COLORREF color, int width)
{
	m_color = color;
	m_penWidth = width;
	m_polyColor = RGB(150, 150, 150);
	m_type = SHAPE_BEZIER;
	m_showControlPoints = true;
}

CGraphBezier::~CGraphBezier()
{
}

void CGraphBezier::AddControlPoint(CPoint pt)
{
	m_controlPoints.push_back(pt);
}

double CGraphBezier::Factorial(int n)
{
	if (n <= 1) return 1;
	double result = 1;
	for (int i = 2; i <= n; i++) {
		result *= i;
	}
	return result;
}

double CGraphBezier::BinomialCoefficient(int n, int i)
{
	return Factorial(n) / (Factorial(i) * Factorial(n - i));
}

CPoint CGraphBezier::CalculateBezierPoint(double t)
{
	int n = (int)m_controlPoints.size() - 1;
	double x = 0, y = 0;
	
	for (int i = 0; i <= n; i++) {
		double blend = BinomialCoefficient(n, i) * pow(t, i) * pow(1 - t, n - i);
		x += m_controlPoints[i].x * blend;
		y += m_controlPoints[i].y * blend;
	}
	
	return CPoint((int)x, (int)y);
}

void CGraphBezier::Draw(CDC* pDC)
{
	if (m_controlPoints.size() < 2) return;
	
	CPen* pOldPen = nullptr;

	if (m_showControlPoints) {
		// Draw control polygon with dashed line
		CPen polyPen(PS_DASH, 1, m_polyColor);
		pOldPen = pDC->SelectObject(&polyPen);
		pDC->MoveTo(m_controlPoints[0]);
		for (size_t i = 1; i < m_controlPoints.size(); i++) {
			pDC->LineTo(m_controlPoints[i]);
		}
		
		for (size_t i = 0; i < m_controlPoints.size(); i++) {
			CRect rect(m_controlPoints[i].x - 3, m_controlPoints[i].y - 3,
				m_controlPoints[i].x + 3, m_controlPoints[i].y + 3);
			pDC->Ellipse(&rect);
			
			CString label;
			label.Format(_T("P%d"), (int)i);
			pDC->SetBkMode(TRANSPARENT);
			pDC->TextOut(m_controlPoints[i].x + 5, m_controlPoints[i].y + 5, label);
		}
	}
	
	CPen curvePen(PS_SOLID, m_penWidth, m_color);
	if (pOldPen) 
		pDC->SelectObject(&curvePen);
	else 
		pOldPen = pDC->SelectObject(&curvePen);
	
	int steps = 100;
	CPoint prevPoint = CalculateBezierPoint(0);
	pDC->MoveTo(prevPoint);
	
	for (int i = 1; i <= steps; i++) {
		double t = (double)i / steps;
		CPoint pt = CalculateBezierPoint(t);
		pDC->LineTo(pt);
	}
	
	if (pOldPen) pDC->SelectObject(pOldPen);
}

CGraphElement* CGraphBezier::Clone()
{
	CGraphBezier* pBezier = new CGraphBezier(m_color, m_penWidth);
	pBezier->m_controlPoints = m_controlPoints;
	pBezier->m_polyColor = m_polyColor;
	pBezier->m_isTransformed = m_isTransformed;
	pBezier->m_transformLabel = m_transformLabel;
	pBezier->m_showControlPoints = m_showControlPoints;
	return pBezier;
}

void CGraphBezier::Transform(double matrix[3][3])
{
	for (size_t i = 0; i < m_controlPoints.size(); i++) {
		CMatrixTool::TransformPoint(m_controlPoints[i], matrix);
	}
}

bool CGraphBezier::HitTest(CPoint point)
{
	// Sample curve and check distance to point
	int steps = 100;
	for (int i = 0; i <= steps; i++) {
		double t = (double)i / steps;
		CPoint pt = CalculateBezierPoint(t);
		int dist = (int)sqrt(pow(point.x - pt.x, 2) + pow(point.y - pt.y, 2));
		if (dist <= 5) return true;
	}
	return false;
}

CRect CGraphBezier::GetBoundingBox()
{
	if (m_controlPoints.empty()) return CRect(0, 0, 0, 0);
	
	int minX = m_controlPoints[0].x, maxX = m_controlPoints[0].x;
	int minY = m_controlPoints[0].y, maxY = m_controlPoints[0].y;
	
	for (const auto& pt : m_controlPoints) {
		if (pt.x < minX) minX = pt.x;
		if (pt.x > maxX) maxX = pt.x;
		if (pt.y < minY) minY = pt.y;
		if (pt.y > maxY) maxY = pt.y;
	}
	
	return CRect(minX - 5, minY - 5, maxX + 5, maxY + 5);
}

void CGraphBezier::Move(int dx, int dy)
{
	for (auto& pt : m_controlPoints) {
		pt.x += dx;
		pt.y += dy;
	}
}

void CGraphBezier::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		int count = (int)m_controlPoints.size();
		ar << count;
		for (auto& pt : m_controlPoints)
		{
			ar << pt.x << pt.y;
		}
		ar << (DWORD)m_color;
		ar << m_penWidth;
		ar << (DWORD)m_polyColor;
		ar << m_transformLabel;
	}
	else
	{
		int count;
		ar >> count;
		m_controlPoints.clear();
		for (int i = 0; i < count; i++)
		{
			CPoint pt;
			ar >> pt.x >> pt.y;
			m_controlPoints.push_back(pt);
		}
		DWORD color;
		ar >> color;
		m_color = (COLORREF)color;
		ar >> m_penWidth;
		ar >> color;
		m_polyColor = (COLORREF)color;
		ar >> m_transformLabel;
		m_type = SHAPE_BEZIER;
		m_selected = false;
	}
}
