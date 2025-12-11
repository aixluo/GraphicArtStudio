#include "pch.h"
#include "CGraphBezierComposite.h"
#include "CMatrixTool.h"
#include <cmath>

CGraphBezierComposite::CGraphBezierComposite(COLORREF color, int width)
{
    m_color = color;
    m_penWidth = width;
    m_polyColor = RGB(150, 150, 150);
    m_type = SHAPE_BEZIER;
}

CGraphBezierComposite::~CGraphBezierComposite()
{
}

void CGraphBezierComposite::AddSegment(const std::vector<CPoint>& controlPoints, COLORREF color)
{
    if (controlPoints.size() < 2) return;

    BezierSegment segment;
    segment.controlPoints = controlPoints;
    segment.color = color;
    segment.degree = (int)controlPoints.size() - 1;

    m_segments.push_back(segment);
}

void CGraphBezierComposite::StartNewSegment(COLORREF color)
{
    // Finish current segment if exists
    if (!m_currentSegment.controlPoints.empty()) {
        FinishCurrentSegment();
    }

    m_currentSegment.controlPoints.clear();
    m_currentSegment.color = color;
}

void CGraphBezierComposite::AddControlPoint(CPoint pt)
{
    m_currentSegment.controlPoints.push_back(pt);
}

void CGraphBezierComposite::FinishCurrentSegment()
{
    if (m_currentSegment.controlPoints.size() >= 2) {
        m_currentSegment.degree = (int)m_currentSegment.controlPoints.size() - 1;
        m_segments.push_back(m_currentSegment);
    }
    m_currentSegment.controlPoints.clear();
}

// ---------- 新增：返回当前未完成段的控制点数量 ----------
int CGraphBezierComposite::GetCurrentSegmentPointCount() const
{
    return (int)m_currentSegment.controlPoints.size();
}

// ---------- 新增：返回最后已完成段的 degree（如果没有已完成段返回 -1） ----------
int CGraphBezierComposite::GetLastFinishedSegmentDegree() const
{
    if (m_segments.empty()) return -1;
    return m_segments.back().degree;
}
// ------------------------------------------------------------------------------

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
    int n = (int)points.size() - 1;
    double x = 0, y = 0;

    for (int i = 0; i <= n; i++) {
        double blend = BinomialCoefficient(n, i) * pow(t, i) * pow(1 - t, n - i);
        x += points[i].x * blend;
        y += points[i].y * blend;
    }

    return CPoint((int)x, (int)y);
}

void CGraphBezierComposite::DrawControlPolygon(CDC* pDC, const BezierSegment& segment)
{
    if (segment.controlPoints.size() < 2) return;

    // Draw control polygon (dashed line)
    CPen polyPen(PS_DASH, 1, m_polyColor);
    CPen* pOldPen = pDC->SelectObject(&polyPen);

    pDC->MoveTo(segment.controlPoints[0]);
    for (size_t i = 1; i < segment.controlPoints.size(); i++) {
        pDC->LineTo(segment.controlPoints[i]);
    }

    // Draw control points (small circles)
    for (size_t i = 0; i < segment.controlPoints.size(); i++) {
        CRect rect(segment.controlPoints[i].x - 3, segment.controlPoints[i].y - 3,
            segment.controlPoints[i].x + 3, segment.controlPoints[i].y + 3);
        pDC->Ellipse(&rect);

        // Label control points
        CString label;
        label.Format(_T("P%d"), (int)i);
        pDC->SetBkMode(TRANSPARENT);
        pDC->TextOut(segment.controlPoints[i].x + 5, segment.controlPoints[i].y + 5, label);
    }

    pDC->SelectObject(pOldPen);
}

void CGraphBezierComposite::DrawSegment(CDC* pDC, const BezierSegment& segment)
{
    if (segment.controlPoints.size() < 2) return;

    // Draw Bezier curve
    CPen curvePen(PS_SOLID, m_penWidth, segment.color);
    CPen* pOldPen = pDC->SelectObject(&curvePen);

    int steps = 100;
    CPoint prevPoint = CalculateBezierPoint(segment.controlPoints, 0);
    pDC->MoveTo(prevPoint);

    for (int i = 1; i <= steps; i++) {
        double t = (double)i / steps;
        CPoint pt = CalculateBezierPoint(segment.controlPoints, t);
        pDC->LineTo(pt);
    }

    pDC->SelectObject(pOldPen);
}

void CGraphBezierComposite::Draw(CDC* pDC)
{
    // Draw all control polygons first
    for (const auto& segment : m_segments) {
        DrawControlPolygon(pDC, segment);
    }

    // Draw all curve segments
    for (const auto& segment : m_segments) {
        DrawSegment(pDC, segment);
    }

    // Draw unfinished segment if exists
    if (!m_currentSegment.controlPoints.empty()) {
        DrawControlPolygon(pDC, m_currentSegment);
        if (m_currentSegment.controlPoints.size() >= 2) {
            DrawSegment(pDC, m_currentSegment);
        }
    }

    // Display transform label
    if (m_isTransformed && !m_transformLabel.IsEmpty()) {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(m_color);
        if (!m_segments.empty() && !m_segments[0].controlPoints.empty()) {
            CPoint labelPos = m_segments[0].controlPoints[0];
            pDC->TextOut(labelPos.x, labelPos.y - 20, m_transformLabel);
        }
    }
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
    for (auto& segment : m_segments) {
        for (auto& pt : segment.controlPoints) {
            CMatrixTool::TransformPoint(pt, matrix);
        }
    }

    // Transform current segment control points
    for (auto& pt : m_currentSegment.controlPoints) {
        CMatrixTool::TransformPoint(pt, matrix);
    }
}

bool CGraphBezierComposite::HitTest(CPoint point)
{
    // Check all segments
    for (const auto& segment : m_segments) {
        // Sample curve and check distance
        int steps = 100;
        for (int i = 0; i <= steps; i++) {
            double t = (double)i / steps;
            CPoint pt = CalculateBezierPoint(segment.controlPoints, t);
            int dist = (int)sqrt(pow(point.x - pt.x, 2) + pow(point.y - pt.y, 2));
            if (dist <= 5) return true;
        }
    }
    return false;
}

CRect CGraphBezierComposite::GetBoundingBox()
{
    if (m_segments.empty()) return CRect(0, 0, 0, 0);

    int minX = INT_MAX, maxX = INT_MIN;
    int minY = INT_MAX, maxY = INT_MIN;

    for (const auto& segment : m_segments) {
        for (const auto& pt : segment.controlPoints) {
            if (pt.x < minX) minX = pt.x;
            if (pt.x > maxX) maxX = pt.x;
            if (pt.y < minY) minY = pt.y;
            if (pt.y > maxY) maxY = pt.y;
        }
    }

    return CRect(minX - 5, minY - 5, maxX + 5, maxY + 5);
}

void CGraphBezierComposite::Move(int dx, int dy)
{
    // Move all segment control points
    for (auto& segment : m_segments) {
        for (auto& pt : segment.controlPoints) {
            pt.x += dx;
            pt.y += dy;
        }
    }

    // Move current segment control points
    for (auto& pt : m_currentSegment.controlPoints) {
        pt.x += dx;
        pt.y += dy;
    }
}

void CGraphBezierComposite::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // 保存段数量
        int segmentCount = (int)m_segments.size();
        ar << segmentCount;

        // 保存每个段
        for (size_t i = 0; i < m_segments.size(); i++)
        {
            const BezierSegment& segment = m_segments[i];
            int pointCount = (int)segment.controlPoints.size();
            ar << pointCount;
            for (size_t j = 0; j < segment.controlPoints.size(); j++)
            {
                const CPoint& pt = segment.controlPoints[j];
                ar << pt.x << pt.y;
            }
            ar << (DWORD)segment.color;
            ar << segment.degree;
        }

        // 保存当前段
        int currentPointCount = (int)m_currentSegment.controlPoints.size();
        ar << currentPointCount;
        for (size_t i = 0; i < m_currentSegment.controlPoints.size(); i++)
        {
            const CPoint& pt = m_currentSegment.controlPoints[i];
            ar << pt.x << pt.y;
        }
        ar << (DWORD)m_currentSegment.color;
        ar << m_currentSegment.degree;

        // 保存基本属性
        ar << (DWORD)m_color;
        ar << m_penWidth;
        ar << (DWORD)m_polyColor;
        ar << m_transformLabel;
    }
    else
    {
        // 加载段数量
        int segmentCount;
        ar >> segmentCount;

        m_segments.clear();

        // 加载每个段
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

        // 加载当前段
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

        // 加载基本属性
        ar >> color;
        m_color = (COLORREF)color;
        ar >> m_penWidth;
        ar >> color;
        m_polyColor = (COLORREF)color;
        ar >> m_transformLabel;

        m_type = SHAPE_BEZIER;  // 或者定义新的类型
        m_selected = false;
    }
}