#include "pch.h"
#include "CGraphBezierComposite.h"
#include "CMatrixTool.h"

CGraphBezierComposite::CGraphBezierComposite(COLORREF color, int width)
{
    m_color = color;
    m_penWidth = width;
    m_polyColor = RGB(150, 150, 150);
    m_type = SHAPE_BEZIER;
    m_selected = false;
}

CGraphBezierComposite::~CGraphBezierComposite()
{
}

void CGraphBezierComposite::AddSegment(const std::vector<CPoint>& controlPoints, COLORREF color)
{
    BezierSegment segment;
    segment.controlPoints = controlPoints;
    segment.color = color;
    segment.degree = (int)controlPoints.size() - 1;
    m_segments.push_back(segment);
}

void CGraphBezierComposite::StartNewSegment(COLORREF color)
{
    m_currentSegment.controlPoints.clear();
    m_currentSegment.color = color;
    m_currentSegment.degree = 0;
}

void CGraphBezierComposite::AddControlPoint(CPoint pt)
{
    m_currentSegment.controlPoints.push_back(pt);
    m_currentSegment.degree = (int)m_currentSegment.controlPoints.size() - 1;
}

void CGraphBezierComposite::FinishCurrentSegment()
{
    if (!m_currentSegment.controlPoints.empty()) {
        m_segments.push_back(m_currentSegment);
        m_currentSegment.controlPoints.clear();
    }
}

void CGraphBezierComposite::Draw(CDC* pDC)
{
    // Draw all completed segments
    for (size_t i = 0; i < m_segments.size(); i++) {
        DrawSegment(pDC, m_segments[i]);
    }
    
    // Draw current segment if it has points
    if (!m_currentSegment.controlPoints.empty()) {
        DrawSegment(pDC, m_currentSegment);
    }
}

void CGraphBezierComposite::DrawSegment(CDC* pDC, const BezierSegment& segment)
{
    if (segment.controlPoints.size() < 2) return;
    
    CPen pen(PS_SOLID, m_penWidth, segment.color);
    CPen* pOldPen = pDC->SelectObject(&pen);
    
    // Draw Bezier curve
    if (segment.controlPoints.size() >= 4) {
        for (double t = 0.0; t <= 1.0; t += 0.01) {
            CPoint pt = CalculateBezierPoint(segment.controlPoints, t);
            if (t == 0.0) {
                pDC->MoveTo(pt);
            } else {
                pDC->LineTo(pt);
            }
        }
    } else {
        // Draw simple line for segments with less than 4 points
        pDC->MoveTo(segment.controlPoints[0]);
        for (size_t i = 1; i < segment.controlPoints.size(); i++) {
            pDC->LineTo(segment.controlPoints[i]);
        }
    }
    
    // Draw control polygon
    DrawControlPolygon(pDC, segment);
    
    pDC->SelectObject(pOldPen);
}

void CGraphBezierComposite::DrawControlPolygon(CDC* pDC, const BezierSegment& segment)
{
    if (segment.controlPoints.size() < 2) return;
    
    CPen polyPen(PS_DOT, 1, m_polyColor);
    CPen* pOldPen = pDC->SelectObject(&polyPen);
    
    pDC->MoveTo(segment.controlPoints[0]);
    for (size_t i = 1; i < segment.controlPoints.size(); i++) {
        pDC->LineTo(segment.controlPoints[i]);
    }
    
    // Draw control points
    for (size_t i = 0; i < segment.controlPoints.size(); i++) {
        CRect rect(segment.controlPoints[i].x - 3, segment.controlPoints[i].y - 3,
                   segment.controlPoints[i].x + 3, segment.controlPoints[i].y + 3);
        pDC->Rectangle(&rect);
    }
    
    pDC->SelectObject(pOldPen);
}

double CGraphBezierComposite::Factorial(int n)
{
    if (n <= 1) return 1;
    double result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

double CGraphBezierComposite::BinomialCoefficient(int n, int i)
{
    return Factorial(n) / (Factorial(i) * Factorial(n - i));
}

CPoint CGraphBezierComposite::CalculateBezierPoint(const std::vector<CPoint>& points, double t)
{
    if (points.empty()) return CPoint(0, 0);
    if (points.size() == 1) return points[0];
    
    int n = (int)points.size() - 1;
    double x = 0, y = 0;
    
    for (int i = 0; i <= n; i++) {
        double coeff = BinomialCoefficient(n, i) * pow(1 - t, n - i) * pow(t, i);
        x += coeff * points[i].x;
        y += coeff * points[i].y;
    }
    
    return CPoint((int)x, (int)y);
}

CGraphElement* CGraphBezierComposite::Clone()
{
    CGraphBezierComposite* pComposite = new CGraphBezierComposite(m_color, m_penWidth);
    pComposite->m_segments = m_segments;
    pComposite->m_currentSegment = m_currentSegment;
    pComposite->m_polyColor = m_polyColor;
    pComposite->m_isTransformed = m_isTransformed;
    pComposite->m_transformLabel = m_transformLabel;
    return pComposite;
}

void CGraphBezierComposite::Transform(double matrix[3][3])
{
    // Transform all segment control points
    for (size_t i = 0; i < m_segments.size(); i++) {
        for (size_t j = 0; j < m_segments[i].controlPoints.size(); j++) {
            CMatrixTool::TransformPoint(m_segments[i].controlPoints[j], matrix);
        }
    }
    
    // Transform current segment control points
    for (size_t i = 0; i < m_currentSegment.controlPoints.size(); i++) {
        CMatrixTool::TransformPoint(m_currentSegment.controlPoints[i], matrix);
    }
    
    m_isTransformed = true;
}

bool CGraphBezierComposite::HitTest(CPoint point)
{
    // Simple hit test - check if point is near any control point
    for (size_t i = 0; i < m_segments.size(); i++) {
        for (size_t j = 0; j < m_segments[i].controlPoints.size(); j++) {
            CPoint cp = m_segments[i].controlPoints[j];
            if (abs(point.x - cp.x) <= 5 && abs(point.y - cp.y) <= 5) {
                return true;
            }
        }
    }
    
    for (size_t i = 0; i < m_currentSegment.controlPoints.size(); i++) {
        CPoint cp = m_currentSegment.controlPoints[i];
        if (abs(point.x - cp.x) <= 5 && abs(point.y - cp.y) <= 5) {
            return true;
        }
    }
    
    return false;
}

CRect CGraphBezierComposite::GetBoundingBox()
{
    if (m_segments.empty() && m_currentSegment.controlPoints.empty()) {
        return CRect(0, 0, 0, 0);
    }
    
    int minX = INT_MAX, minY = INT_MAX;
    int maxX = INT_MIN, maxY = INT_MIN;
    
    // Check all segments
    for (size_t i = 0; i < m_segments.size(); i++) {
        for (size_t j = 0; j < m_segments[i].controlPoints.size(); j++) {
            CPoint pt = m_segments[i].controlPoints[j];
            if (pt.x < minX) minX = pt.x;
            if (pt.x > maxX) maxX = pt.x;
            if (pt.y < minY) minY = pt.y;
            if (pt.y > maxY) maxY = pt.y;
        }
    }
    
    // Check current segment
    for (size_t i = 0; i < m_currentSegment.controlPoints.size(); i++) {
        CPoint pt = m_currentSegment.controlPoints[i];
        if (pt.x < minX) minX = pt.x;
        if (pt.x > maxX) maxX = pt.x;
        if (pt.y < minY) minY = pt.y;
        if (pt.y > maxY) maxY = pt.y;
    }
    
    return CRect(minX - 5, minY - 5, maxX + 5, maxY + 5);
}

void CGraphBezierComposite::Move(int dx, int dy)
{
    // Move all segment control points
    for (size_t i = 0; i < m_segments.size(); i++) {
        for (size_t j = 0; j < m_segments[i].controlPoints.size(); j++) {
            m_segments[i].controlPoints[j].x += dx;
            m_segments[i].controlPoints[j].y += dy;
        }
    }
    
    // Move current segment control points
    for (size_t i = 0; i < m_currentSegment.controlPoints.size(); i++) {
        m_currentSegment.controlPoints[i].x += dx;
        m_currentSegment.controlPoints[i].y += dy;
    }
}

void CGraphBezierComposite::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // Save number of segments
        int segmentCount = (int)m_segments.size();
        ar << segmentCount;
        
        // Save each segment
        for (int i = 0; i < segmentCount; i++)
        {
            const BezierSegment& segment = m_segments[i];
            int pointCount = (int)segment.controlPoints.size();
            ar << pointCount;
            for (int j = 0; j < pointCount; j++)
            {
                const CPoint& pt = segment.controlPoints[j];
                ar << pt.x << pt.y;
            }
            ar << (DWORD)segment.color;
            ar << segment.degree;
        }
        
        // Save current segment
        int currentPointCount = (int)m_currentSegment.controlPoints.size();
        ar << currentPointCount;
        for (int i = 0; i < currentPointCount; i++)
        {
            const CPoint& pt = m_currentSegment.controlPoints[i];
            ar << pt.x << pt.y;
        }
        ar << (DWORD)m_currentSegment.color;
        ar << m_currentSegment.degree;
        
        // Save basic properties
        ar << (DWORD)m_color;
        ar << m_penWidth;
        ar << (DWORD)m_polyColor;
        ar << m_transformLabel;
    }
    else
    {
        // Load number of segments
        int segmentCount;
        ar >> segmentCount;
        
        m_segments.clear();
        
        // Load each segment
        for (int i = 0; i < segmentCount; i++)
        {
            BezierSegment segment;
            int pointCount;
            ar >> pointCount;
            
            segment.controlPoints.clear();
            for (int j = 0; j < pointCount; j++)
            {
                CPoint pt;
                ar >> pt.x >> pt.y;
                segment.controlPoints.push_back(pt);
            }
            
            DWORD color;
            ar >> color;
            segment.color = (COLORREF)color;
            ar >> segment.degree;
            
            m_segments.push_back(segment);
        }
        
        // Load current segment
        int currentPointCount;
        ar >> currentPointCount;
        
        m_currentSegment.controlPoints.clear();
        for (int i = 0; i < currentPointCount; i++)
        {
            CPoint pt;
            ar >> pt.x >> pt.y;
            m_currentSegment.controlPoints.push_back(pt);
        }
        
        DWORD color;
        ar >> color;
        m_currentSegment.color = (COLORREF)color;
        ar >> m_currentSegment.degree;
        
        // Load basic properties
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
