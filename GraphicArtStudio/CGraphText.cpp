#include "pch.h"
#include "CGraphText.h"
#include "CMatrixTool.h"

CGraphText::CGraphText()
{
	m_position = CPoint(0, 0);
	m_text = _T("");
	m_fontName = _T("Arial");
	m_fontSize = 12;
	m_color = RGB(0, 0, 0);
	m_penWidth = 1;
	m_type = SHAPE_TEXT;
}

CGraphText::CGraphText(CPoint pos, CString text, CString fontName, int fontSize, COLORREF color)
{
	m_position = pos;
	m_text = text;
	m_fontName = fontName;
	m_fontSize = fontSize;
	m_color = color;
	m_type = SHAPE_TEXT;
}

CGraphText::~CGraphText()
{
}

void CGraphText::Draw(CDC* pDC)
{
	if (!pDC) return;
	
	// 创建字体
	CFont font;
	font.CreateFontW(
		m_fontSize, 0, 0, 0,
		FW_NORMAL, FALSE, FALSE, 0,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS,
		m_fontName);
	
	CFont* pOldFont = pDC->SelectObject(&font);
	
	// 设置文字颜色和背景模式
	COLORREF oldColor = pDC->SetTextColor(m_color);
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);
	
	// 绘制文字
	pDC->TextOut(m_position.x, m_position.y, m_text);
	
	// 恢复原来的设置
	pDC->SetTextColor(oldColor);
	pDC->SetBkMode(oldBkMode);
	pDC->SelectObject(pOldFont);
}

CGraphElement* CGraphText::Clone()
{
	CGraphText* pText = new CGraphText(m_position, m_text, m_fontName, m_fontSize, m_color);
	pText->m_isTransformed = m_isTransformed;
	pText->m_transformLabel = m_transformLabel;
	return pText;
}

void CGraphText::Transform(double matrix[3][3])
{
	CMatrixTool::TransformPoint(m_position, matrix);
}

bool CGraphText::HitTest(CPoint point)
{
	CRect bbox = GetBoundingBox();
	return bbox.PtInRect(point);
}

CRect CGraphText::GetBoundingBox()
{
	// Estimate width based on character types
	int width = 0;
	for (int i = 0; i < m_text.GetLength(); i++) {
		// Simple check for non-ASCII (likely Chinese/Wide characters)
		// In Unicode build, CString holds wchar_t, so we check value
		if (m_text[i] > 0xFF) { 
			width += m_fontSize;
		} else {
			width += m_fontSize / 2 + 1; // +1 for some spacing
		}
	}
	// Ensure minimum width
	if (width < 10) width = 10;
	
	int height = m_fontSize;
	return CRect(m_position.x - 5, m_position.y - 5,
		m_position.x + width + 5, m_position.y + height + 5);
}

void CGraphText::Move(int dx, int dy)
{
	m_position.x += dx;
	m_position.y += dy;
}

void CGraphText::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_position.x << m_position.y;
		ar << m_text;
		ar << m_fontName;
		ar << m_fontSize;
		ar << (DWORD)m_color;
		ar << m_transformLabel;
	}
	else
	{
		ar >> m_position.x >> m_position.y;
		ar >> m_text;
		ar >> m_fontName;
		ar >> m_fontSize;
		DWORD color;
		ar >> color;
		m_color = (COLORREF)color;
		ar >> m_transformLabel;
		m_type = SHAPE_TEXT;
		m_selected = false;
	}
}
