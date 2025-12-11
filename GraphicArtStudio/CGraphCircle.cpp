#include "pch.h"
#include "CGraphCircle.h"
#include "CMatrixTool.h"
#include <cmath>

CGraphCircle::CGraphCircle()
{
	m_center = CPoint(0, 0);
	m_radius = 10;
	m_color = RGB(0, 0, 0);
	m_penWidth = 1;
	m_filled = false;
	m_fillColor = RGB(255, 255, 255);
	m_type = SHAPE_CIRCLE;
	m_selected = false;
	m_penStyle = PS_SOLID;
}

CGraphCircle::CGraphCircle(CPoint center, int radius, COLORREF color, int width, bool filled, COLORREF fillColor)
{
	m_center = center;
	m_radius = radius;
	m_color = color;
	m_penWidth = width;
	m_filled = filled;
	m_fillColor = fillColor;
	m_type = SHAPE_CIRCLE;
	m_selected = false;
	m_penStyle = PS_SOLID;
}

CGraphCircle::~CGraphCircle()
{
}

void CGraphCircle::Draw(CDC* pDC)
{
	if (!pDC) return;
	
	// 创建画笔
	CPen pen(m_penStyle, m_penWidth, m_color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	
	// 创建画刷
	CBrush brush;
	CBrush* pOldBrush = nullptr;
	
	if (m_filled) {
		// 填充圆形
		brush.CreateSolidBrush(m_fillColor);
		pOldBrush = pDC->SelectObject(&brush);
	} else {
		// 空心圆形
		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
	}
	
	// 绘制圆形
	pDC->Ellipse(m_center.x - m_radius, m_center.y - m_radius, 
	             m_center.x + m_radius, m_center.y + m_radius);
	
	// 恢复原来的画笔和画刷
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CGraphCircle::DrawMidpointCircle(CDC* pDC)
{
	Draw(pDC);
}

void CGraphCircle::DrawCirclePoints(CDC* pDC, int xc, int yc, int x, int y)
{
}

CGraphElement* CGraphCircle::Clone()
{
	CGraphCircle* pCircle = new CGraphCircle(m_center, m_radius, m_color, m_penWidth, m_filled, m_fillColor);
	pCircle->m_isTransformed = m_isTransformed;
	pCircle->m_transformLabel = m_transformLabel;
	pCircle->m_selected = m_selected;
	pCircle->m_penStyle = m_penStyle;
	return pCircle;
}

void CGraphCircle::Transform(double matrix[3][3])
{
	// Transform center point
	CMatrixTool::TransformPoint(m_center, matrix);
	
	// For scaling, we need to transform the radius
	// Calculate scale factor from the matrix (use average of X and Y scale)
	double scaleX = sqrt(matrix[0][0] * matrix[0][0] + matrix[1][0] * matrix[1][0]);
	double scaleY = sqrt(matrix[0][1] * matrix[0][1] + matrix[1][1] * matrix[1][1]);
	double scale = (scaleX + scaleY) / 2.0;
	
	// Scale the radius
	if (scale > 0.1 && scale < 10.0) {  // Reasonable scale limits
		m_radius = (int)(m_radius * scale);
		if (m_radius < 1) m_radius = 1;  // Minimum radius
	}
}

bool CGraphCircle::HitTest(CPoint point)
{
	return false;
}

CRect CGraphCircle::GetBoundingBox()
{
	return CRect(m_center.x - m_radius, m_center.y - m_radius, m_center.x + m_radius, m_center.y + m_radius);
}

void CGraphCircle::Move(int dx, int dy)
{
	m_center.x += dx;
	m_center.y += dy;
}

void CGraphCircle::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// 保存圆形数据
		ar << m_center.x << m_center.y;
		ar << m_radius;
		ar << (DWORD)m_color;
		ar << m_penWidth;
		ar << m_filled;
		ar << (DWORD)m_fillColor;
	}
	else
	{
		// 加载圆形数据
		ar >> m_center.x >> m_center.y;
		ar >> m_radius;
		DWORD color;
		ar >> color;
		m_color = (COLORREF)color;
		ar >> m_penWidth;
		ar >> m_filled;
		ar >> color;
		m_fillColor = (COLORREF)color;
		m_type = SHAPE_CIRCLE;
		m_selected = false;
	}
}
