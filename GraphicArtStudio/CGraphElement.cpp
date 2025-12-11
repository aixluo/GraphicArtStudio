#include "pch.h"
#include "CGraphElement.h"

CGraphElement::CGraphElement()
{
	m_color = RGB(0, 0, 0);
	m_penWidth = 1;
	m_isTransformed = false;
	m_transformLabel = _T("");
	m_type = SHAPE_LINE;
	m_selected = false;
}

CGraphElement::~CGraphElement()
{
}

void CGraphElement::DrawSelection(CDC* pDC)
{
	if (!m_selected) return;
	
	CRect bbox = GetBoundingBox();
	
	// Draw bounding box
	CPen pen(PS_DOT, 2, RGB(0, 120, 215));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->SelectStockObject(NULL_BRUSH);
	
	pDC->Rectangle(bbox);
	
	// Draw resize handles (8 points)
	int handleSize = 6;
	CBrush handleBrush(RGB(255, 255, 255));
	CBrush* pOldBrush = pDC->SelectObject(&handleBrush);
	
	// Corner handles
	pDC->Rectangle(bbox.left - handleSize/2, bbox.top - handleSize/2, 
		bbox.left + handleSize/2, bbox.top + handleSize/2);
	pDC->Rectangle(bbox.right - handleSize/2, bbox.top - handleSize/2, 
		bbox.right + handleSize/2, bbox.top + handleSize/2);
	pDC->Rectangle(bbox.left - handleSize/2, bbox.bottom - handleSize/2, 
		bbox.left + handleSize/2, bbox.bottom + handleSize/2);
	pDC->Rectangle(bbox.right - handleSize/2, bbox.bottom - handleSize/2, 
		bbox.right + handleSize/2, bbox.bottom + handleSize/2);
	
	// Edge handles
	int centerX = (bbox.left + bbox.right) / 2;
	int centerY = (bbox.top + bbox.bottom) / 2;
	pDC->Rectangle(centerX - handleSize/2, bbox.top - handleSize/2, 
		centerX + handleSize/2, bbox.top + handleSize/2);
	pDC->Rectangle(centerX - handleSize/2, bbox.bottom - handleSize/2, 
		centerX + handleSize/2, bbox.bottom + handleSize/2);
	pDC->Rectangle(bbox.left - handleSize/2, centerY - handleSize/2, 
		bbox.left + handleSize/2, centerY + handleSize/2);
	pDC->Rectangle(bbox.right - handleSize/2, centerY - handleSize/2, 
		bbox.right + handleSize/2, centerY + handleSize/2);
	
	// Draw rotation handle (above top center)
	int rotateY = bbox.top - 30;
	pDC->Ellipse(centerX - handleSize/2, rotateY - handleSize/2,
		centerX + handleSize/2, rotateY + handleSize/2);
	pDC->MoveTo(centerX, bbox.top);
	pDC->LineTo(centerX, rotateY);
	
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}
