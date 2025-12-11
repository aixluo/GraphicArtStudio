#pragma once
#include "CGraphElement.h"

class CGraphText : public CGraphElement
{
public:
	CGraphText();
	CGraphText(CPoint pos, CString text, CString fontName, int fontSize, COLORREF color);
	virtual ~CGraphText();
	virtual void Draw(CDC* pDC);
	virtual CGraphElement* Clone();
	virtual void Transform(double matrix[3][3]);
	virtual bool HitTest(CPoint point);
	virtual CRect GetBoundingBox();
	virtual void Move(int dx, int dy);
	virtual void Serialize(CArchive& ar);

public:
	CPoint m_position;
	CString m_text;
	CString m_fontName;
	int m_fontSize;
};
