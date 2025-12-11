#include "pch.h"
#include "CChineseChar.h"
#include "CGraphPolygon.h" 
#include <algorithm>

CChineseChar::CChineseChar()
{
m_type = SHAPE_TEXT; 
m_selected = false;
m_isTransformed = false;
m_color = RGB(0, 0, 0);
m_penWidth = 1;
m_filled = false;
m_fillColor = RGB(255, 255, 255);
}

CChineseChar::~CChineseChar()
{
for (CGraphElement* pShape : m_subShapes) {
delete pShape;
}
m_subShapes.clear();
}

void CChineseChar::AddShape(CGraphElement* pShape)
{
if (pShape) {
m_subShapes.push_back(pShape);
}
}

void CChineseChar::Draw(CDC* pDC)
{
	if (m_filled) {
		// Use GDI path to draw filled complex polygon (with holes)
		pDC->BeginPath();
		for (CGraphElement* pShape : m_subShapes) {
			if (pShape->m_type == SHAPE_POLYGON) {
				CGraphPolygon* pPoly = (CGraphPolygon*)pShape;
				const std::vector<CPoint>& points = pPoly->m_points;
				if (points.size() > 0) {
					pDC->MoveTo(points[0]);
					for (size_t i = 1; i < points.size(); i++) {
						pDC->LineTo(points[i]);
					}
					// Close figure for path
					pDC->LineTo(points[0]);
				}
			}
		}
		pDC->EndPath();

		// Set fill mode to ALTERNATE to handle holes correctly
		int nOldPolyFillMode = pDC->SetPolyFillMode(ALTERNATE);
		
		// Create brush and pen
		CBrush brush(m_fillColor);
		CBrush* pOldBrush = pDC->SelectObject(&brush);
		CPen pen(PS_SOLID, m_penWidth, m_color);
		CPen* pOldPen = pDC->SelectObject(&pen);

		// Draw path (Stroke and Fill)
		pDC->StrokeAndFillPath();

		// Restore DC
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		pDC->SetPolyFillMode(nOldPolyFillMode);
	}
	else {
		// Draw normally (outlines only)
		// Use container's properties to ensure transformed color/style is applied
		int nPenStyle = PS_SOLID;
		if (m_isTransformed) {
			nPenStyle = PS_DASH; // Use dashed line for transformed shape
		}

		CPen pen(nPenStyle, m_penWidth, m_color);
		CPen* pOldPen = pDC->SelectObject(&pen);

		for (CGraphElement* pShape : m_subShapes) {
			if (pShape->m_type == SHAPE_POLYGON) {
				CGraphPolygon* pPoly = (CGraphPolygon*)pShape;
				const std::vector<CPoint>& points = pPoly->m_points;
				if (points.size() > 1) {
					pDC->MoveTo(points[0]);
					for (size_t i = 1; i < points.size(); i++) {
						pDC->LineTo(points[i]);
					}
					// Close figure if needed (though Chinese chars usually are strokes)
					// But if it was closed originally, we should close it.
					// CGraphPolygon::Draw checks m_closed? No, it just draws lines.
					// Wait, let's check CGraphPolygon::Draw again.
					if (pPoly->m_closed) pDC->LineTo(points[0]);
				}
			}
			else {
				pShape->Draw(pDC);
			}
		}
		pDC->SelectObject(pOldPen);
	}

	if (m_selected) {
		DrawSelection(pDC);
	}

	// Draw Transformation Label
	if (m_isTransformed && !m_transformLabel.IsEmpty()) {
		CRect rect = GetBoundingBox();
		COLORREF oldTextColor = pDC->SetTextColor(m_color);
		int oldBkMode = pDC->SetBkMode(TRANSPARENT);
		
		pDC->TextOutW(rect.right + 5, rect.top, m_transformLabel);
		
		pDC->SetBkMode(oldBkMode);
		pDC->SetTextColor(oldTextColor);
	}
}

CGraphElement* CChineseChar::Clone()
{
CChineseChar* pNewChar = new CChineseChar();
pNewChar->m_type = m_type;
pNewChar->m_color = m_color;
pNewChar->m_penWidth = m_penWidth;
pNewChar->m_isTransformed = m_isTransformed;
pNewChar->m_transformLabel = m_transformLabel;
pNewChar->m_selected = m_selected;

for (CGraphElement* pShape : m_subShapes) {
pNewChar->AddShape(pShape->Clone());
}

return pNewChar;
}

void CChineseChar::Transform(double matrix[3][3])
{
for (CGraphElement* pShape : m_subShapes) {
pShape->Transform(matrix);
}
m_isTransformed = true;
}

bool CChineseChar::HitTest(CPoint point)
{
for (CGraphElement* pShape : m_subShapes) {
if (pShape->HitTest(point)) {
return true;
}
}
return false;
}

CRect CChineseChar::GetBoundingBox()
{
CRect rect(0, 0, 0, 0);
bool first = true;

for (CGraphElement* pShape : m_subShapes) {
CRect r = pShape->GetBoundingBox();
if (first) {
rect = r;
first = false;
}
else {
// Union rect
int left = min(rect.left, r.left);
int top = min(rect.top, r.top);
int right = max(rect.right, r.right);
int bottom = max(rect.bottom, r.bottom);
rect.SetRect(left, top, right, bottom);
}
}
return rect;
}

void CChineseChar::Move(int dx, int dy)
{
for (CGraphElement* pShape : m_subShapes) {
pShape->Move(dx, dy);
}
}

void CChineseChar::Serialize(CArchive& ar)
{
if (ar.IsStoring()) {
ar << (int)m_type;
ar << m_color;
ar << m_penWidth;
ar << m_isTransformed;
ar << m_transformLabel;
ar << m_selected;

ar << (int)m_subShapes.size();
for (CGraphElement* pShape : m_subShapes) {
ar << (int)pShape->m_type;
pShape->Serialize(ar);
}
}
else {
int type;
ar >> type; m_type = (ShapeType)type;
ar >> m_color;
ar >> m_penWidth;
ar >> m_isTransformed;
ar >> m_transformLabel;
ar >> m_selected;

int count;
ar >> count;
for (int i = 0; i < count; i++) {
int shapeTypeVal;
ar >> shapeTypeVal;
ShapeType st = (ShapeType)shapeTypeVal;

CGraphElement* pShape = nullptr;
if (st == SHAPE_POLYGON) {
pShape = new CGraphPolygon();
}
// Add other types if needed

if (pShape) {
pShape->Serialize(ar);
AddShape(pShape);
}
}
}
}

void CChineseChar::DrawSelection(CDC* pDC)
{
CRect rect = GetBoundingBox();
CPen pen(PS_DOT, 1, RGB(0, 0, 255));
CPen* pOldPen = pDC->SelectObject(&pen);
CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

pDC->Rectangle(rect);

pDC->SelectObject(pOldBrush);
pDC->SelectObject(pOldPen);
}

void CChineseChar::SetFillAlgorithm(int algorithm)
{
	for (CGraphElement* pShape : m_subShapes) {
		if (pShape->m_type == SHAPE_POLYGON) {
			((CGraphPolygon*)pShape)->SetFillAlgorithm((FillAlgorithmType)algorithm);
		}
	}
}

void CChineseChar::SetFilled(bool filled)
{
	m_filled = filled;
	// Do NOT set sub-shapes filled status, we handle filling in Draw()
	// to support holes correctly using ALTERNATE fill mode
	for (CGraphElement* pShape : m_subShapes) {
		if (pShape->m_type == SHAPE_POLYGON) {
			((CGraphPolygon*)pShape)->m_filled = false; // Ensure sub-shapes don't fill themselves
		}
	}
}

void CChineseChar::SetFillColor(COLORREF color)
{
	m_fillColor = color;
	// Still update sub-shapes just in case, but they won't fill if m_filled is false
	for (CGraphElement* pShape : m_subShapes) {
		if (pShape->m_type == SHAPE_POLYGON) {
			((CGraphPolygon*)pShape)->m_fillColor = color;
		}
	}
}
