#pragma once
#include "CGraphElement.h"

class CGraphEllipse : public CGraphElement
{
public:
	CGraphEllipse();
	CGraphEllipse(CPoint center, int rx, int ry, COLORREF color, int width, bool filled = false, COLORREF fillColor = RGB(255, 255, 255));
	virtual ~CGraphEllipse();
	virtual void Draw(CDC* pDC);
	virtual CGraphElement* Clone();
	virtual void Transform(double matrix[3][3]);
	virtual bool HitTest(CPoint point);
	virtual CRect GetBoundingBox();
	virtual void Move(int dx, int dy);
	virtual void Serialize(CArchive& ar);
	
	void DrawMidpointEllipse(CDC* pDC);
	void DrawEllipsePoints(CDC* pDC, int xc, int yc, int x, int y);

public:
	CPoint m_center;
	int m_rx;
	int m_ry;
	bool m_filled;
	COLORREF m_fillColor;
};
