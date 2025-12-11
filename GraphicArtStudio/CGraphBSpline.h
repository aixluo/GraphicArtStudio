#pragma once
#include "CGraphElement.h"
#include <vector>

class CGraphBSpline : public CGraphElement
{
public:
	CGraphBSpline();
	CGraphBSpline(COLORREF color, int width, int degree = 3);
	virtual ~CGraphBSpline();
	virtual void Draw(CDC* pDC);
	virtual CGraphElement* Clone();
	virtual void Transform(double matrix[3][3]);
	virtual bool HitTest(CPoint point);
	virtual CRect GetBoundingBox();
	virtual void Move(int dx, int dy);
	virtual void Serialize(CArchive& ar);
	void AddControlPoint(CPoint pt);
	CPoint CalculateBSplinePoint(double t);
	double BasisFunction(int i, int k, double t);
	void GenerateKnotVector();

public:
	std::vector<CPoint> m_controlPoints;
	std::vector<double> m_knots;
	COLORREF m_polyColor;
	int m_degree;
};
