#pragma once
#include <vector>
#include "CGraphElement.h"
#include "CGraphBezierComposite.h"

// Forward declarations
class CGraphicArtStudioDoc;
class CGraphPolygon;

enum DrawMode {
	MODE_SELECT,
	MODE_LINE,
	MODE_CIRCLE,
	MODE_ELLIPSE,
	MODE_POLYGON,
	MODE_TEXT,
	MODE_BEZIER,
	MODE_BSPLINE,
	MODE_SEED_FILL,
	MODE_CLIP_WINDOW, // New mode for defining clipping window
	MODE_NONE
};

class CGraphicArtStudioView : public CView
{
protected:
	CGraphicArtStudioView() noexcept;
	DECLARE_DYNCREATE(CGraphicArtStudioView)

public:
	CGraphicArtStudioDoc* GetDocument() const;

	// Drawing state
	DrawMode m_drawMode;
	// Target for clipping operation (0: None, 1: Line, 2: Polygon)
	int m_clipTarget;
	CPoint m_startPoint;
	CPoint m_endPoint;
	CPoint m_currentPoint;
	bool m_isDrawing;

	// Polygon and curve control points
	std::vector<CPoint> m_tempPoints;
	CGraphBezierComposite* m_pTempCompositeBezier;

	// Current drawing properties
	COLORREF m_currentColor;
	int m_currentWidth;
	int m_lineAlgorithm;
	bool m_filled;
	COLORREF m_fillColor;
	COLORREF m_seedFillColor;

	// View settings
	bool m_showCoordinate;
	bool m_showGrid;
	COLORREF m_bgColor;

	// Canvas size
	int m_canvasWidth;
	int m_canvasHeight;

	// Undo/Redo
	std::vector<std::vector<CGraphElement*>> m_undoStack;
	std::vector<std::vector<CGraphElement*>> m_redoStack;
	const int MAX_UNDO_STEPS = 5;

	// Selection and interaction
	CGraphElement* m_selectedShape;  // For single selection compatibility
	std::vector<CGraphElement*> m_selectedShapes;  // For multiple selection
	bool m_isDragging;
	CPoint m_dragStartPoint;
	enum DragMode { DRAG_NONE, DRAG_MOVE, DRAG_SCALE, DRAG_ROTATE, DRAG_SELECT_BOX };
	DragMode m_dragMode;
	double m_currentRotationAngle;  // Current rotation angle for display
	CRect m_selectionBox;  // Selection rectangle for box selection

	// Helper functions
	void SaveUndoState();
	void ClearSelection();
	CGraphElement* FindShapeAtPoint(CPoint point);
	DragMode GetHandleAtPoint(CPoint point, CRect bbox);
	void DrawCoordinateSystem(CDC* pDC);
	void DrawRubberBand(CDC* pDC);
	void DrawGrid(CDC* pDC);
	void DrawCanvasBoundary(CDC* pDC);
	bool IsPointInCanvas(CPoint point);
	CRect GetCanvasRect();
	bool IsPointInsidePolygon(CPoint point, CGraphPolygon* polygon);
	double CalculatePolygonArea(CGraphPolygon* polygon);
	bool PerformFloodFill(CPoint seedPoint, COLORREF fillColor);
	bool FindFillBoundary(CPoint seedPoint, CRect& boundary);
	bool DoesRectIntersectPolygon(CRect rect, CGraphPolygon* polygon);
	void ResetDrawingMode();

	// Operation / algorithm labels (visualization)
public:
	// Set or clear labels shown in view
	void SetOperationLabel(LPCTSTR pszLabel);
	void SetAlgorithmLabel(LPCTSTR pszLabel);
	void ClearOperationLabels();

protected:
	// Internal label storage (drawn in OnDraw)
	CString m_operationLabel;   // e.g. "操作: 直线绘制"
	CString m_algorithmLabel;   // e.g. "算法: DDA算法"

	// Draw labels on the passed DC (call from OnDraw's memDC before BitBlt)
	void DrawOperationLabels(CDC* pDC);

	// Animation support
	bool m_isAnimating;
	int m_animationFrame;
	void StartSolarSystemAnimation();
	void StopAnimation();

public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

public:
	virtual ~CGraphicArtStudioView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	// Mouse handlers
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// Keyboard handlers
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	// File menu
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();

	// Edit menu
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnEditClear();
	afx_msg void OnEditSelectAll();

	// Update UI handlers for Edit menu commands
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSelectAll(CCmdUI* pCmdUI);

	// Draw menu - Basic shapes
	afx_msg void OnDrawLine();
	afx_msg void OnDrawLineInput();
	afx_msg void OnDrawCircle();
	afx_msg void OnDrawCircleInput();
	afx_msg void OnDrawEllipse();
	afx_msg void OnDrawPolygon();
	afx_msg void OnDrawText();

	// Draw menu - Curves
	afx_msg void OnDrawBezier();
	afx_msg void OnDrawBSpline();

	// Transform menu
	afx_msg void OnTransformTranslate();
	afx_msg void OnTransformScale();
	afx_msg void OnTransformRotate();
	afx_msg void OnTransformShear();
	afx_msg void OnTransformSymmetry();
	afx_msg void OnTransformComposite();

	// Clip menu
	afx_msg void OnClipLine();
	afx_msg void OnClipPolygon();

	// Fill menu
	afx_msg void OnFillScanline();
	afx_msg void OnFillSeed();
	afx_msg void OnFillCharacter();
	afx_msg void OnFillSetColor();
	afx_msg void OnFillToggle();
	afx_msg void OnUpdateFillToggle(CCmdUI* pCmdUI);

	// View menu
	afx_msg void OnViewCoordinate();
	afx_msg void OnViewGrid();
	afx_msg void OnViewProperty();
	afx_msg void OnViewToolbar();
	afx_msg void OnViewCanvasSize();

	// Tool menu
	afx_msg void OnToolSelect();
	afx_msg void OnToolMove();
	afx_msg void OnToolScale();
	afx_msg void OnToolRotate();

	// Color menu
	afx_msg void OnColorLine();
	afx_msg void OnColorFill();
	afx_msg void OnColorBackground();

	// Line width
	afx_msg void OnWidth1();
	afx_msg void OnWidth2();
	afx_msg void OnWidth3();
	afx_msg void OnWidth5();

	// Line algorithm
	afx_msg void OnAlgoDDA();
	afx_msg void OnAlgoBresenham();
	afx_msg void OnAlgoMidpoint();
	afx_msg void OnAlgoSystem();

	// Help menu
	afx_msg void OnHelpUsage();
	afx_msg void OnHelpAbout();

	// Comprehensive drawing menu
	afx_msg void OnDrawSolarSystem();
	afx_msg void OnDrawChineseChar();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG
inline CGraphicArtStudioDoc* CGraphicArtStudioView::GetDocument() const
{
	return reinterpret_cast<CGraphicArtStudioDoc*>(m_pDocument);
}
#endif