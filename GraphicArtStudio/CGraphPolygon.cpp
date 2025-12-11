#include "pch.h"
#include "CGraphPolygon.h"
#include "CMatrixTool.h"
#include "CFillTool.h"

CGraphPolygon::CGraphPolygon()
{
	m_color = RGB(0, 0, 0);
	m_penWidth = 1;
	m_closed = false;
	m_filled = false;
	m_fillColor = RGB(255, 255, 255);
	m_type = SHAPE_POLYGON;
	m_fillAlgorithm = FILL_NONE;
	m_showAlgorithmLabel = false;
}

CGraphPolygon::CGraphPolygon(COLORREF color, int width, bool filled, COLORREF fillColor)
{
	m_color = color;
	m_penWidth = width;
	m_closed = false;
	m_filled = filled;
	m_fillColor = fillColor;
	m_type = SHAPE_POLYGON;
	m_fillAlgorithm = FILL_NONE;
	m_showAlgorithmLabel = false;
}

CGraphPolygon::~CGraphPolygon()
{
}

void CGraphPolygon::AddPoint(CPoint pt)
{
	m_points.push_back(pt);
	m_pointsD.push_back(PointD((double)pt.x, (double)pt.y));
}

void CGraphPolygon::Close()
{
	m_closed = true;
}

void CGraphPolygon::Draw(CDC* pDC)
{
	if (m_points.size() < 2) return;
	
	CPen pen(PS_SOLID, m_penWidth, m_color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	
	pDC->MoveTo(m_points[0]);
	for (size_t i = 1; i < m_points.size(); i++) {
		pDC->LineTo(m_points[i]);
	}
	if (m_closed && m_points.size() >= 3) {
		pDC->LineTo(m_points[0]);
	}
	
	if (m_filled && m_closed && m_points.size() >= 3) {
		switch (m_fillAlgorithm) {
		case FILL_SCANLINE:
			CFillTool::ScanlineFill(pDC, m_points, m_fillColor, m_color);
			break;
			
		case FILL_SEED:
		case FILL_SCANLINE_SEED:
		{
			int centerX = 0, centerY = 0;
			for (const auto& pt : m_points) {
				centerX += pt.x;
				centerY += pt.y;
			}
			centerX /= (int)m_points.size();
			centerY /= (int)m_points.size();
			
			CRect rect(m_points[0], m_points[0]);
			for (const auto& pt : m_points) {
				if (pt.x < rect.left) rect.left = pt.x;
				if (pt.x > rect.right) rect.right = pt.x;
				if (pt.y < rect.top) rect.top = pt.y;
				if (pt.y > rect.bottom) rect.bottom = pt.y;
			}
			rect.InflateRect(10, 10);
			
			if (m_fillAlgorithm == FILL_SEED) {
				CFillTool::SeedFill(pDC, CPoint(centerX, centerY), 
										 m_fillColor, m_color, rect);
			}
			else {
				CFillTool::ScanlineSeedFill(pDC, CPoint(centerX, centerY),
												 m_fillColor, m_color, rect);
			}
			break;
		}
		
		case FILL_SYSTEM:
		{
			CBrush brush(m_fillColor);
			CBrush* pOldBrush = pDC->SelectObject(&brush);
			POINT* pts = new POINT[m_points.size()];
			for (size_t i = 0; i < m_points.size(); i++) {
				pts[i].x = m_points[i].x;
				pts[i].y = m_points[i].y;
			}
			pDC->Polygon(pts, (int)m_points.size());
			delete[] pts;
			pDC->SelectObject(pOldBrush);
			break;
		}
		
		default:
			break;
		}
	}
	
	pDC->SelectObject(pOldPen);
	
	if (m_showAlgorithmLabel) {
		DrawAlgorithmLabel(pDC);
	}
}

CGraphElement* CGraphPolygon::Clone()
{
	CGraphPolygon* pPolygon = new CGraphPolygon(m_color, m_penWidth, m_filled, m_fillColor);
	pPolygon->m_points = m_points;
	pPolygon->m_pointsD = m_pointsD;
	pPolygon->m_closed = m_closed;
	pPolygon->m_isTransformed = m_isTransformed;
	pPolygon->m_transformLabel = m_transformLabel;
	pPolygon->m_fillAlgorithm = m_fillAlgorithm;
	pPolygon->m_showAlgorithmLabel = m_showAlgorithmLabel;
	return pPolygon;
}

void CGraphPolygon::Transform(double matrix[3][3])
{
	// Ensure m_pointsD is synchronized if it was empty (e.g. loaded from file)
	if (m_pointsD.empty() && !m_points.empty()) {
		for (const auto& pt : m_points) {
			m_pointsD.push_back(PointD((double)pt.x, (double)pt.y));
		}
	}

	for (size_t i = 0; i < m_pointsD.size(); i++) {
		CMatrixTool::TransformPoint(m_pointsD[i].x, m_pointsD[i].y, matrix);
		// Update integer points for drawing
		m_points[i].x = (int)(m_pointsD[i].x + 0.5);
		m_points[i].y = (int)(m_pointsD[i].y + 0.5);
	}
}

void CGraphPolygon::SetFillAlgorithm(FillAlgorithmType algorithm)
{
	m_fillAlgorithm = algorithm;
	// Disable automatic label display to avoid clutter, especially for composite shapes like Chinese characters
	m_showAlgorithmLabel = false; 
}

void CGraphPolygon::DrawAlgorithmLabel(CDC* pDC)
{
	if (m_points.empty()) return;
	
	int maxX = m_points[0].x;
	int sumY = 0;
	for (const auto& pt : m_points) {
		if (pt.x > maxX) maxX = pt.x;
		sumY += pt.y;
	}
	int labelX = maxX + 20;
	int labelY = sumY / (int)m_points.size();
	
	CFont font;
	font.CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
	
	CFont* pOldFont = pDC->SelectObject(&font);
	COLORREF oldColor = pDC->SetTextColor(RGB(0, 0, 255));
	int oldMode = pDC->SetBkMode(TRANSPARENT);
	
	CString labelText;
	switch (m_fillAlgorithm) {
	case FILL_SCANLINE:
		labelText = FillAlgorithmText::SCANLINE_FILL;
		break;
	case FILL_SEED:
		labelText = FillAlgorithmText::SEED_FILL;
		break;
	case FILL_SCANLINE_SEED:
		labelText = FillAlgorithmText::SCANLINE_SEED_FILL;
		break;
	default:
		break;
	}
	
	if (!labelText.IsEmpty()) {
		CRect textRect(labelX, labelY - 50, labelX + 300, labelY + 100);
		pDC->DrawText(labelText, &textRect, DT_LEFT | DT_WORDBREAK);
	}
	
	pDC->SetBkMode(oldMode);
	pDC->SetTextColor(oldColor);
	pDC->SelectObject(pOldFont);
}

bool CGraphPolygon::HitTest(CPoint point)
{
	if (m_points.size() < 2) return false;
	
	// Check if point is near any edge
	for (size_t i = 0; i < m_points.size(); i++) {
		CPoint p1 = m_points[i];
		CPoint p2 = m_points[(i + 1) % m_points.size()];
		
		int dx = p2.x - p1.x;
		int dy = p2.y - p1.y;
		
		if (dx == 0 && dy == 0) continue;
		
		float t = ((point.x - p1.x) * dx + (point.y - p1.y) * dy) / 
			(float)(dx * dx + dy * dy);
		
		if (t < 0) t = 0;
		if (t > 1) t = 1;
		
		int closestX = (int)(p1.x + t * dx);
		int closestY = (int)(p1.y + t * dy);
		
		int dist = (int)sqrt(pow(point.x - closestX, 2) + pow(point.y - closestY, 2));
		
		if (dist <= 5) return true;
	}
	
	return false;
}

CRect CGraphPolygon::GetBoundingBox()
{
	if (m_points.empty()) return CRect(0, 0, 0, 0);
	
	int minX = m_points[0].x, maxX = m_points[0].x;
	int minY = m_points[0].y, maxY = m_points[0].y;
	
	for (const auto& pt : m_points) {
		if (pt.x < minX) minX = pt.x;
		if (pt.x > maxX) maxX = pt.x;
		if (pt.y < minY) minY = pt.y;
		if (pt.y > maxY) maxY = pt.y;
	}
	
	return CRect(minX - 5, minY - 5, maxX + 5, maxY + 5);
}

void CGraphPolygon::Move(int dx, int dy)
{
	for (auto& pt : m_points) {
		pt.x += dx;
		pt.y += dy;
	}
	for (auto& pt : m_pointsD) {
		pt.x += dx;
		pt.y += dy;
	}
}

void CGraphPolygon::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		int count = (int)m_points.size();
		ar << count;
		for (auto& pt : m_points)
		{
			ar << pt.x << pt.y;
		}
		ar << (DWORD)m_color;
		ar << m_penWidth;
		ar << m_closed;
		ar << m_filled;
		ar << (DWORD)m_fillColor;
		ar << (int)m_fillAlgorithm;
		ar << m_showAlgorithmLabel;
		ar << m_transformLabel;
	}
	else
	{
		int count;
		ar >> count;
		m_points.clear();
		for (int i = 0; i < count; i++)
		{
			CPoint pt;
			ar >> pt.x >> pt.y;
			m_points.push_back(pt);
		}
		DWORD color;
		ar >> color;
		m_color = (COLORREF)color;
		ar >> m_penWidth;
		ar >> m_closed;
		ar >> m_filled;
		ar >> color;
		m_fillColor = (COLORREF)color;
		int fillAlgo;
		ar >> fillAlgo;
		m_fillAlgorithm = (FillAlgorithmType)fillAlgo;
		ar >> m_showAlgorithmLabel;
		ar >> m_transformLabel;
		m_type = SHAPE_POLYGON;
		m_selected = false;
	}
}
