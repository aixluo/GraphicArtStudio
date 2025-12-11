#pragma once
#include "CGraphElement.h"

class CGraphCircle : public CGraphElement
{
public:
	CGraphCircle();
	CGraphCircle(CPoint center, int radius, COLORREF color, int width, bool filled = false, COLORREF fillColor = RGB(255, 255, 255));
	virtual ~CGraphCircle();
	virtual void Draw(CDC* pDC);
	virtual CGraphElement* Clone();
	virtual void Transform(double matrix[3][3]);
	virtual bool HitTest(CPoint point);
	virtual CRect GetBoundingBox();
	virtual void Move(int dx, int dy);
	virtual void Serialize(CArchive& ar) { /* 暂时禁用 */ }
	
	void DrawMidpointCircle(CDC* pDC);
	void DrawCirclePoints(CDC* pDC, int xc, int yc, int x, int y);

public:
	CPoint m_center;
	int m_radius;
	bool m_filled;
	COLORREF m_fillColor;
};
