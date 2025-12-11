#pragma once
#include "CGraphElement.h"
#include <vector>

class CChineseChar : public CGraphElement
{
public:
	CChineseChar();
	virtual ~CChineseChar();

	void AddShape(CGraphElement* pShape);

	virtual void Draw(CDC* pDC) override;
	virtual CGraphElement* Clone() override;
	virtual void Transform(double matrix[3][3]) override;
	virtual bool HitTest(CPoint point) override;
	virtual CRect GetBoundingBox() override;
	virtual void Move(int dx, int dy) override;
	virtual void Serialize(CArchive& ar) override;
	virtual void DrawSelection(CDC* pDC) override;

	// Fill support
	void SetFillAlgorithm(int algorithm); // Using int to avoid dependency issues, cast to FillAlgorithmType internally
	void SetFilled(bool filled);
	void SetFillColor(COLORREF color);

	std::vector<CGraphElement*>& GetSubShapes() { return m_subShapes; }

private:
	std::vector<CGraphElement*> m_subShapes;
	bool m_filled;
	COLORREF m_fillColor;
};
