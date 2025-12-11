#pragma once
#include "CGraphElement.h"
#include <vector>

// Bezier curve segment structure
struct BezierSegment
{
    std::vector<CPoint> controlPoints;
    COLORREF color;
    int degree;
};

// Bezier composite curve class
class CGraphBezierComposite : public CGraphElement
{
public:
    CGraphBezierComposite(COLORREF color = RGB(0, 0, 0), int width = 1);
    virtual ~CGraphBezierComposite();

    void AddSegment(const std::vector<CPoint>& controlPoints, COLORREF color);
    void StartNewSegment(COLORREF color);
    void AddControlPoint(CPoint pt);
    void FinishCurrentSegment();

    virtual void Draw(CDC* pDC) override;
    virtual CGraphElement* Clone() override;
    virtual void Transform(double matrix[3][3]) override;
    virtual bool HitTest(CPoint point) override;
    virtual CRect GetBoundingBox() override;
    virtual void Move(int dx, int dy) override;
    virtual void Serialize(CArchive& ar) override;

    int GetSegmentCount() const { return (int)m_segments.size(); }
    bool HasUnfinishedSegment() const { return !m_currentSegment.controlPoints.empty(); }

    // 新增：便于视图查询当前未完成段的控制点数量
    int GetCurrentSegmentPointCount() const;
    // 新增：便于视图查询最后已完成段的次数（degree），若无已完成段返回 -1
    int GetLastFinishedSegmentDegree() const;

private:
    std::vector<BezierSegment> m_segments;
    BezierSegment m_currentSegment;
    COLORREF m_polyColor;

    double Factorial(int n);
    double BinomialCoefficient(int n, int i);
    CPoint CalculateBezierPoint(const std::vector<CPoint>& points, double t);
    void DrawSegment(CDC* pDC, const BezierSegment& segment);
    void DrawControlPolygon(CDC* pDC, const BezierSegment& segment);
};