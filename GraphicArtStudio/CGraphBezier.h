#pragma once
#include "CGraphElement.h"
#include <vector>

class CGraphBezier : public CGraphElement
{
public:
	CGraphBezier();
	CGraphBezier(COLORREF color, int width);
	virtual ~CGraphBezier();
	virtual void Draw(CDC* pDC);
	virtual CGraphElement* Clone();
	virtual void Transform(double matrix[3][3]);
	virtual bool HitTest(CPoint point);
	virtual CRect GetBoundingBox();
	virtual void Move(int dx, int dy);
	virtual void Serialize(CArchive& ar);
	void AddControlPoint(CPoint pt);
	CPoint CalculateBezierPoint(double t);
	double BinomialCoefficient(int n, int i);
	double Factorial(int n);

public:
	std::vector<CPoint> m_controlPoints;
	COLORREF m_polyColor;
	bool m_showControlPoints; // New flag to control visibility of control points
};
