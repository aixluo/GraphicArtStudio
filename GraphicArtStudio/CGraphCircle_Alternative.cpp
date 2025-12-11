#include "pch.h"
#include "CGraphCircle.h"

// 默认构造函数
CGraphCircle::CGraphCircle()
{
	this->m_center = CPoint(0, 0);
	this->m_radius = 10;
	this->m_color = RGB(0, 0, 0);
	this->m_penWidth = 1;
	this->m_filled = false;
	this->m_fillColor = RGB(255, 255, 255);
	this->m_type = SHAPE_CIRCLE;
	this->m_selected = false;
}

// 参数构造函数
CGraphCircle::CGraphCircle(CPoint center, int radius, COLORREF color, int width, bool filled, COLORREF fillColor)
{
	this->m_center = center;
	this->m_radius = radius;
	this->m_color = color;
	this->m_penWidth = width;
	this->m_filled = filled;
	this->m_fillColor = fillColor;
	this->m_type = SHAPE_CIRCLE;
	this->m_selected = false;
}

// 析构函数
CGraphCircle::~CGraphCircle()
{
}

// 绘制函数
void CGraphCircle::Draw(CDC* pDC)
{
	this->DrawMidpointCircle(pDC);
}

// 中点圆算法
void CGraphCircle::DrawMidpointCircle(CDC* pDC)
{
	CPen pen(PS_SOLID, this->m_penWidth, this->m_color);
	CPen* pOldPen = pDC->SelectObject(&pen);
	
	if (this->m_filled)
	{
		CBrush brush(this->m_fillColor);
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		pDC->Ellipse(this->m_center.x - this->m_radius, this->m_center.y - this->m_radius,
			this->m_center.x + this->m_radius, this->m_center.y + this->m_radius);
		pDC->SelectObject(pOldBrush);
	}
	else
	{
		pDC->Ellipse(this->m_center.x - this->m_radius, this->m_center.y - this->m_radius,
			this->m_center.x + this->m_radius + 1, this->m_center.y + this->m_radius + 1);
	}
	
	pDC->SelectObject(pOldPen);
}

// 绘制圆上的点
void CGraphCircle::DrawCirclePoints(CDC* pDC, int xc, int yc, int x, int y)
{
	pDC->SetPixel(xc + x, yc + y, this->m_color);
	pDC->SetPixel(xc - x, yc + y, this->m_color);
	pDC->SetPixel(xc + x, yc - y, this->m_color);
	pDC->SetPixel(xc - x, yc - y, this->m_color);
	pDC->SetPixel(xc + y, yc + x, this->m_color);
	pDC->SetPixel(xc - y, yc + x, this->m_color);
	pDC->SetPixel(xc + y, yc - x, this->m_color);
	pDC->SetPixel(xc - y, yc - x, this->m_color);
}

// 克隆函数
CGraphElement* CGraphCircle::Clone()
{
	CGraphCircle* pCircle = new CGraphCircle(this->m_center, this->m_radius, this->m_color, this->m_penWidth, this->m_filled, this->m_fillColor);
	pCircle->m_isTransformed = this->m_isTransformed;
	pCircle->m_transformLabel = this->m_transformLabel;
	return pCircle;
}

// 变换函数
void CGraphCircle::Transform(double matrix[3][3])
{
	// 简化的变换，只变换中心点
	double x = this->m_center.x;
	double y = this->m_center.y;
	
	this->m_center.x = (int)(matrix[0][0] * x + matrix[0][1] * y + matrix[0][2]);
	this->m_center.y = (int)(matrix[1][0] * x + matrix[1][1] * y + matrix[1][2]);
	
	this->m_isTransformed = true;
}

// 碰撞检测
bool CGraphCircle::HitTest(CPoint point)
{
	int dx = point.x - this->m_center.x;
	int dy = point.y - this->m_center.y;
	int dist = (int)sqrt((double)(dx * dx + dy * dy));
	return abs(dist - this->m_radius) <= 5;
}

// 获取边界框
CRect CGraphCircle::GetBoundingBox()
{
	return CRect(this->m_center.x - this->m_radius - 5, this->m_center.y - this->m_radius - 5,
		this->m_center.x + this->m_radius + 5, this->m_center.y + this->m_radius + 5);
}

// 移动函数
void CGraphCircle::Move(int dx, int dy)
{
	this->m_center.x += dx;
	this->m_center.y += dy;
}
