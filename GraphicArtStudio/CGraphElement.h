#pragma once
#include <afxwin.h>

enum ShapeType { 
	SHAPE_LINE, 
	SHAPE_CIRCLE, 
	SHAPE_ELLIPSE, 
	SHAPE_POLYGON, 
	SHAPE_TEXT, 
	SHAPE_CURVE,
	SHAPE_BEZIER,
	SHAPE_BSPLINE,
	SHAPE_TRANSFORM
};

class CGraphElement
{
public:
	CGraphElement();
	virtual ~CGraphElement();
	virtual void Draw(CDC* pDC) = 0;
	virtual CGraphElement* Clone() = 0;
	virtual void Transform(double matrix[3][3]) = 0;
	
	// Selection and interaction
	virtual bool HitTest(CPoint point) = 0;  // Test if point is on/near shape
	virtual CRect GetBoundingBox() = 0;      // Get bounding rectangle
	virtual void Move(int dx, int dy) = 0;   // Move shape by offset
	virtual void DrawSelection(CDC* pDC);    // Draw selection indicators
	
	// Serialization
	virtual void Serialize(CArchive& ar) = 0;  // 序列化方法

public:
	COLORREF m_color;
	int m_penWidth;
	ShapeType m_type;
	bool m_isTransformed;
	CString m_transformLabel;
	
	// Selection state
	bool m_selected;
};
