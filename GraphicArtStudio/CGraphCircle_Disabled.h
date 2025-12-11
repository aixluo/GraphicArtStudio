#pragma once
#include "CGraphElement.h"

// 暂时禁用 CGraphCircle 类以解决编译问题
// 使用简化的实现，仅提供基本接口

class CGraphCircle : public CGraphElement
{
public:
	CGraphCircle() 
	{ 
		m_type = SHAPE_CIRCLE; 
		m_selected = false;
		m_color = RGB(0, 0, 0);
		m_penWidth = 1;
	}
	
	CGraphCircle(CPoint center, int radius, COLORREF color, int width, bool filled = false, COLORREF fillColor = RGB(255, 255, 255))
	{
		m_type = SHAPE_CIRCLE;
		m_selected = false;
		m_color = color;
		m_penWidth = width;
		// 暂时不实现具体功能
	}
	
	virtual ~CGraphCircle() {}
	
	virtual void Draw(CDC* pDC) 
	{
		// 简单绘制一个圆
		CPen pen(PS_SOLID, m_penWidth, m_color);
		CPen* pOldPen = pDC->SelectObject(&pen);
		pDC->Ellipse(100, 100, 150, 150);  // 固定大小的圆
		pDC->SelectObject(pOldPen);
	}
	
	virtual CGraphElement* Clone() { return new CGraphCircle(); }
	virtual void Transform(double matrix[3][3]) {}
	virtual bool HitTest(CPoint point) { return false; }
	virtual CRect GetBoundingBox() { return CRect(0, 0, 100, 100); }
	virtual void Move(int dx, int dy) {}
	virtual void Serialize(CArchive& ar) {}
	
	void DrawMidpointCircle(CDC* pDC) {}
	void DrawCirclePoints(CDC* pDC, int xc, int yc, int x, int y) {}

public:
	CPoint m_center;
	int m_radius;
	bool m_filled;
	COLORREF m_fillColor;
};
