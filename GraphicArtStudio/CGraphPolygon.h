#pragma once
#include "CGraphElement.h"
#include <vector>

enum FillAlgorithmType {
	FILL_NONE,
	FILL_SYSTEM,
	FILL_SCANLINE,
	FILL_SEED,
	FILL_SCANLINE_SEED
};

class CGraphPolygon : public CGraphElement
{
public:
	CGraphPolygon();
	CGraphPolygon(COLORREF color, int width, bool filled = false, COLORREF fillColor = RGB(255, 255, 255));
	virtual ~CGraphPolygon();
	virtual void Draw(CDC* pDC);
	virtual CGraphElement* Clone();
	virtual void Transform(double matrix[3][3]);
	virtual bool HitTest(CPoint point);
	virtual CRect GetBoundingBox();
	virtual void Move(int dx, int dy);
	virtual void Serialize(CArchive& ar);
	
	void AddPoint(CPoint pt);
	void Close();
	void SetFillAlgorithm(FillAlgorithmType algorithm);
	void DrawAlgorithmLabel(CDC* pDC);

	struct PointD {
		double x, y;
		PointD(double _x = 0, double _y = 0) : x(_x), y(_y) {}
	};
	std::vector<PointD> m_pointsD;

public:
	std::vector<CPoint> m_points;
	bool m_closed;
	bool m_filled;
	COLORREF m_fillColor;
	FillAlgorithmType m_fillAlgorithm;
	bool m_showAlgorithmLabel;
};
