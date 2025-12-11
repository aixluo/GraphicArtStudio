#include "pch.h"
#include "CGraphBSpline.h"
#include "CMatrixTool.h"

CGraphBSpline::CGraphBSpline()
{
	m_color = RGB(0, 0, 0);
	m_penWidth = 1;
	m_degree = 3;
	m_polyColor = RGB(150, 150, 150);
	m_type = SHAPE_BSPLINE;
}

CGraphBSpline::CGraphBSpline(COLORREF color, int width, int degree)
{
	m_color = color;
	m_penWidth = width;
	m_degree = degree;
	m_polyColor = RGB(150, 150, 150);
	m_type = SHAPE_BSPLINE;
}

CGraphBSpline::~CGraphBSpline()
{
}

void CGraphBSpline::AddControlPoint(CPoint pt)
{
	m_controlPoints.push_back(pt);
	GenerateKnotVector();
}

void CGraphBSpline::GenerateKnotVector()
{
	m_knots.clear();
	int n = (int)m_controlPoints.size();
	int m = n + m_degree + 1;
	
	for (int i = 0; i < m; i++) {
		if (i <= m_degree) {
			m_knots.push_back(0);
		} else if (i >= n) {
			m_knots.push_back(n - m_degree);
		} else {
			m_knots.push_back(i - m_degree);
		}
	}
}

double CGraphBSpline::BasisFunction(int i, int k, double t)
{
	if (k == 0) {
		if (t >= m_knots[i] && t < m_knots[i + 1]) {
			return 1.0;
		} else {
			return 0.0;
		}
	} else {
		double c1 = 0.0, c2 = 0.0;
		
		if (m_knots[i + k] - m_knots[i] != 0) {
			c1 = (t - m_knots[i]) / (m_knots[i + k] - m_knots[i]) * BasisFunction(i, k - 1, t);
		}
		
		if (m_knots[i + k + 1] - m_knots[i + 1] != 0) {
			c2 = (m_knots[i + k + 1] - t) / (m_knots[i + k + 1] - m_knots[i + 1]) * BasisFunction(i + 1, k - 1, t);
		}
		
		return c1 + c2;
	}
}

CPoint CGraphBSpline::CalculateBSplinePoint(double t)
{
	int n = (int)m_controlPoints.size();
	double x = 0, y = 0;
	
	for (int i = 0; i < n; i++) {
		double basis = BasisFunction(i, m_degree, t);
		x += m_controlPoints[i].x * basis;
		y += m_controlPoints[i].y * basis;
	}
	
	return CPoint((int)x, (int)y);
}

void CGraphBSpline::Draw(CDC* pDC)
{
	if (m_controlPoints.size() < m_degree + 1) return;
	
	CPen polyPen(PS_DASH, 1, m_polyColor);
	CPen* pOldPen = pDC->SelectObject(&polyPen);
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
	
	CPen curvePen(PS_SOLID, m_penWidth, m_color);
	pDC->SelectObject(&curvePen);
	
	int n = (int)m_controlPoints.size();
	double tStart = m_knots[m_degree];
	double tEnd = m_knots[n];
	int steps = 100;
	
	CPoint prevPoint = CalculateBSplinePoint(tStart);
	pDC->MoveTo(prevPoint);
	
	for (int i = 1; i <= steps; i++) {
		double t = tStart + (tEnd - tStart) * i / steps;
		if (t >= tEnd) t = tEnd - 0.0001;
		CPoint pt = CalculateBSplinePoint(t);
		pDC->LineTo(pt);
	}
	
	pDC->SelectObject(pOldPen);
}

CGraphElement* CGraphBSpline::Clone()
{
	CGraphBSpline* pBSpline = new CGraphBSpline(m_color, m_penWidth, m_degree);
	pBSpline->m_controlPoints = m_controlPoints;
	pBSpline->m_knots = m_knots;
	pBSpline->m_polyColor = m_polyColor;
	pBSpline->m_isTransformed = m_isTransformed;
	pBSpline->m_transformLabel = m_transformLabel;
	return pBSpline;
}

void CGraphBSpline::Transform(double matrix[3][3])
{
	for (size_t i = 0; i < m_controlPoints.size(); i++) {
		CMatrixTool::TransformPoint(m_controlPoints[i], matrix);
	}
}

bool CGraphBSpline::HitTest(CPoint point)
{
	if (m_controlPoints.size() < m_degree + 1) return false;
	
	// Sample curve and check distance to point
	int n = (int)m_controlPoints.size();
	double tStart = m_knots[m_degree];
	double tEnd = m_knots[n];
	int steps = 100;
	
	for (int i = 0; i <= steps; i++) {
		double t = tStart + (tEnd - tStart) * i / steps;
		if (t >= tEnd) t = tEnd - 0.0001;
		CPoint pt = CalculateBSplinePoint(t);
		int dist = (int)sqrt(pow(point.x - pt.x, 2) + pow(point.y - pt.y, 2));
		if (dist <= 5) return true;
	}
	return false;
}

CRect CGraphBSpline::GetBoundingBox()
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

void CGraphBSpline::Move(int dx, int dy)
{
	for (auto& pt : m_controlPoints) {
		pt.x += dx;
		pt.y += dy;
	}
}

void CGraphBSpline::Serialize(CArchive& ar)
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
		ar << m_degree;
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
		ar >> m_degree;
		ar >> color;
		m_polyColor = (COLORREF)color;
		ar >> m_transformLabel;
		m_type = SHAPE_BSPLINE;
		m_selected = false;
		GenerateKnotVector();
	}
}
