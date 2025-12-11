#pragma once
#include "CGraphElement.h"

class CGraphLine : public CGraphElement
{
public:
	CGraphLine();  // 默认构造函数（用于序列化）
	CGraphLine(CPoint start, CPoint end, int algo, COLORREF color, int width);
	virtual ~CGraphLine();
	virtual void Draw(CDC* pDC);
	virtual CGraphElement* Clone();
	virtual void Transform(double matrix[3][3]);
	virtual bool HitTest(CPoint point);
	virtual CRect GetBoundingBox();
	virtual void Move(int dx, int dy);
	virtual void Serialize(CArchive& ar);

	void DrawDDA(CDC* pDC);
	void DrawMidBresenham(CDC* pDC);
	void DrawBresenham(CDC* pDC);

public:
	CPoint m_start;
	CPoint m_end;
	int m_algoType;  // 0=DDA, 1=MidPoint, 2=Bresenham, 3=System
};
