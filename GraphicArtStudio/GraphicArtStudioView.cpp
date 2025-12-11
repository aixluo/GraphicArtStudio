#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "GraphicArtStudio.h"
#endif

#include "GraphicArtStudioDoc.h"
#include "GraphicArtStudioView.h"
#include "MainWindow.h"
#include "Resource.h"
#include "CGraphLine.h"
#include "CGraphCircle.h"
#include "CGraphEllipse.h"
#include "CGraphPolygon.h"
#include "CGraphText.h"
#include "CGraphBezier.h"
#include "CGraphBezierComposite.h"
#include "CGraphBSpline.h"
#include "CircleInputDlg.h"
#include "TextInputDlg.h"
#include "TransformDlg.h"
#include "CMatrixTool.h"
#include "ColorPickerDlg.h"
#include "CanvasSizeDlg.h"  // Using simplified implementation
#include "BackgroundColorDlg.h"
#include <float.h>  // For DBL_MAX
#include "ClippingDlg.h"
#include "LineClipping.h"
#include "PolygonClipping.h"
#include "CFillTool.h"
#include "ComprehensiveDrawing.h"
// 包含头文件
// #include "ChineseCharDlg.h"  // 暂时注释掉，绕过对话框问题
// #include "CChineseChar.h"    // 暂时注释掉，使用CGraphText替代
#include "LineInputDlg.h"
#include "NameCharacter.h"
#include "CChineseChar.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CGraphicArtStudioView, CView)

BEGIN_MESSAGE_MAP(CGraphicArtStudioView, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	
	// Keyboard handlers
	ON_WM_KEYDOWN()
	
	// File menu
	ON_COMMAND(ID_FILE_NEW, &CGraphicArtStudioView::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CGraphicArtStudioView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CGraphicArtStudioView::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CGraphicArtStudioView::OnFileSaveAs)
	
	// Edit menu
	ON_COMMAND(ID_MY_UNDO, &CGraphicArtStudioView::OnEditUndo)
	ON_COMMAND(ID_MY_REDO, &CGraphicArtStudioView::OnEditRedo)
	ON_COMMAND(ID_MY_CLEAR, &CGraphicArtStudioView::OnEditClear)
	ON_COMMAND(ID_MY_SELECT_ALL, &CGraphicArtStudioView::OnEditSelectAll)
	
	ON_UPDATE_COMMAND_UI(ID_MY_UNDO, &CGraphicArtStudioView::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_MY_REDO, &CGraphicArtStudioView::OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_MY_CLEAR, &CGraphicArtStudioView::OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI(ID_MY_SELECT_ALL, &CGraphicArtStudioView::OnUpdateEditSelectAll)
	
	// Draw menu
	ON_COMMAND(ID_DRAW_LINE, &CGraphicArtStudioView::OnDrawLine)
	ON_COMMAND(ID_DRAW_LINE_INPUT, &CGraphicArtStudioView::OnDrawLineInput)
	ON_COMMAND(ID_DRAW_CIRCLE, &CGraphicArtStudioView::OnDrawCircle)
	ON_COMMAND(ID_DRAW_CIRCLE_INPUT, &CGraphicArtStudioView::OnDrawCircleInput)
	ON_COMMAND(ID_DRAW_ELLIPSE, &CGraphicArtStudioView::OnDrawEllipse)
	ON_COMMAND(ID_DRAW_POLYGON, &CGraphicArtStudioView::OnDrawPolygon)
	ON_COMMAND(ID_DRAW_TEXT, &CGraphicArtStudioView::OnDrawText)
	ON_COMMAND(ID_DRAW_BEZIER, &CGraphicArtStudioView::OnDrawBezier)
	ON_COMMAND(ID_DRAW_BSPLINE, &CGraphicArtStudioView::OnDrawBSpline)
	
	// Transform menu
	ON_COMMAND(ID_TRANSFORM_TRANSLATE, &CGraphicArtStudioView::OnTransformTranslate)
	ON_COMMAND(ID_TRANSFORM_SCALE, &CGraphicArtStudioView::OnTransformScale)
	ON_COMMAND(ID_TRANSFORM_ROTATE, &CGraphicArtStudioView::OnTransformRotate)
	ON_COMMAND(ID_TRANSFORM_SHEAR, &CGraphicArtStudioView::OnTransformShear)
	ON_COMMAND(ID_TRANSFORM_SYMMETRY, &CGraphicArtStudioView::OnTransformSymmetry)
	ON_COMMAND(ID_TRANSFORM_COMPOSITE, &CGraphicArtStudioView::OnTransformComposite)
	
	// Clip menu
	ON_COMMAND(ID_CLIP_LINE, &CGraphicArtStudioView::OnClipLine)
	ON_COMMAND(ID_CLIP_POLYGON, &CGraphicArtStudioView::OnClipPolygon)
	
	// Fill menu
	ON_COMMAND(ID_FILL_SCANLINE, &CGraphicArtStudioView::OnFillScanline)
	ON_COMMAND(ID_FILL_SEED, &CGraphicArtStudioView::OnFillSeed)
	ON_COMMAND(ID_FILL_CHARACTER, &CGraphicArtStudioView::OnFillCharacter)
	ON_COMMAND(ID_FILL_SET_COLOR, &CGraphicArtStudioView::OnFillSetColor)
	ON_COMMAND(ID_FILL_TOGGLE, &CGraphicArtStudioView::OnFillToggle)
	ON_UPDATE_COMMAND_UI(ID_FILL_TOGGLE, &CGraphicArtStudioView::OnUpdateFillToggle)
	
	// View menu
	ON_COMMAND(ID_VIEW_COORDINATE, &CGraphicArtStudioView::OnViewCoordinate)
	ON_COMMAND(ID_VIEW_GRID, &CGraphicArtStudioView::OnViewGrid)
	ON_COMMAND(ID_VIEW_PROPERTY, &CGraphicArtStudioView::OnViewProperty)
	ON_COMMAND(ID_VIEW_TOOLBAR, &CGraphicArtStudioView::OnViewToolbar)
	ON_COMMAND(ID_VIEW_CANVAS_SIZE, &CGraphicArtStudioView::OnViewCanvasSize)
	
	// Tool menu
	ON_COMMAND(ID_TOOL_SELECT, &CGraphicArtStudioView::OnToolSelect)
	ON_COMMAND(ID_TOOL_MOVE, &CGraphicArtStudioView::OnToolMove)
	ON_COMMAND(ID_TOOL_SCALE, &CGraphicArtStudioView::OnToolScale)
	ON_COMMAND(ID_TOOL_ROTATE, &CGraphicArtStudioView::OnToolRotate)
	
	// Color menu
	ON_COMMAND(ID_COLOR_LINE, &CGraphicArtStudioView::OnColorLine)
	ON_COMMAND(ID_COLOR_FILL, &CGraphicArtStudioView::OnColorFill)
	ON_COMMAND(ID_COLOR_BACKGROUND, &CGraphicArtStudioView::OnColorBackground)
	
	// Line width
	ON_COMMAND(ID_WIDTH_1, &CGraphicArtStudioView::OnWidth1)
	ON_COMMAND(ID_WIDTH_2, &CGraphicArtStudioView::OnWidth2)
	ON_COMMAND(ID_WIDTH_3, &CGraphicArtStudioView::OnWidth3)
	ON_COMMAND(ID_WIDTH_5, &CGraphicArtStudioView::OnWidth5)
	
	// Line algorithm
	ON_COMMAND(ID_ALGO_DDA, &CGraphicArtStudioView::OnAlgoDDA)
	ON_COMMAND(ID_ALGO_BRESENHAM, &CGraphicArtStudioView::OnAlgoBresenham)
	ON_COMMAND(ID_ALGO_MIDPOINT, &CGraphicArtStudioView::OnAlgoMidpoint)
	ON_COMMAND(ID_ALGO_SYSTEM, &CGraphicArtStudioView::OnAlgoSystem)
	
	// Help menu
	ON_COMMAND(ID_HELP_USAGE, &CGraphicArtStudioView::OnHelpUsage)
	ON_COMMAND(ID_HELP_ABOUT, &CGraphicArtStudioView::OnHelpAbout)
	
	// Comprehensive drawing menu
	ON_COMMAND(ID_DRAW_SOLAR_SYSTEM, &CGraphicArtStudioView::OnDrawSolarSystem)
	ON_COMMAND(ID_DRAW_CHINESE_CHAR, &CGraphicArtStudioView::OnDrawChineseChar)
	ON_WM_TIMER()
END_MESSAGE_MAP()

CGraphicArtStudioView::CGraphicArtStudioView() noexcept
{
	m_drawMode = MODE_SELECT;  // Start in select mode by default
	m_isAnimating = false;
	m_animationFrame = 0;
	m_isDrawing = false;
	m_currentColor = RGB(0, 0, 0);
	m_currentWidth = 1;
	m_lineAlgorithm = 0;
	m_filled = false;
	m_fillColor = RGB(255, 255, 255);
	m_seedFillColor = RGB(255, 0, 0);
	m_showCoordinate = false;  // 默认关闭坐标轴
	m_showGrid = false;
	m_bgColor = RGB(255, 255, 255);
	
	// Initialize selection and interaction
	m_selectedShape = nullptr;
	m_selectedShapes.clear();
	m_isDragging = false;
	m_dragMode = DRAG_NONE;
	m_currentRotationAngle = 0.0;
	
	// Initialize undo/redo stacks
	m_undoStack.clear();
	m_redoStack.clear();
	
	// Initialize canvas size to screen size
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	// Use full screen size as default canvas size
	m_canvasWidth = screenWidth;
	m_canvasHeight = screenHeight;
	
	m_pTempCompositeBezier = nullptr;

	m_operationLabel.Empty();
	m_algorithmLabel.Empty();
}

CGraphicArtStudioView::~CGraphicArtStudioView()
{
	if (m_pTempCompositeBezier) {
		delete m_pTempCompositeBezier;
		m_pTempCompositeBezier = nullptr;
	}
}

BOOL CGraphicArtStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

void CGraphicArtStudioView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	// Save initial empty state for undo functionality
	TRACE(_T("OnInitialUpdate: Saving initial state\n"));
	SaveUndoState();
	TRACE(_T("OnInitialUpdate: Undo stack size after save: %d\n"), (int)m_undoStack.size());
}

BOOL CGraphicArtStudioView::OnEraseBkgnd(CDC* pDC)
{
	// Return TRUE to prevent flickering (we handle background drawing in OnDraw via double buffering)
	return TRUE;
}

void CGraphicArtStudioView::OnDraw(CDC* pDC)
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);
	
	// Create memory DC for double buffering to reduce flicker
	CDC memDC;
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);
	
	// Draw to memory DC instead of screen DC
	memDC.FillSolidRect(&rect, m_bgColor);

	// Draw canvas boundary
	DrawCanvasBoundary(&memDC);

	if (m_showGrid) {
		DrawGrid(&memDC);
	}

	if (m_showCoordinate) {
		DrawCoordinateSystem(&memDC);
	}

	for (auto shape : pDoc->m_shapeList) {
		if (shape != nullptr) {
			shape->Draw(&memDC);
			// Draw selection indicators
			if (shape->m_selected) {
				shape->DrawSelection(&memDC);
			}
		}
	}
	
	// Draw combined bounding box and handles for multiple selection
	if (m_selectedShapes.size() > 1) {
		// Calculate combined bounding box
		CRect combinedBBox;
		bool first = true;
		for (auto shape : m_selectedShapes) {
			if (shape) {
				CRect bbox = shape->GetBoundingBox();
				if (first) {
					combinedBBox = bbox;
					first = false;
				} else {
					combinedBBox.UnionRect(combinedBBox, bbox);
				}
			}
		}
		
		// Draw combined bounding box
		CPen pen(PS_SOLID, 2, RGB(0, 120, 215));
		CPen* pOldPen = memDC.SelectObject(&pen);
		memDC.SelectStockObject(NULL_BRUSH);
		memDC.Rectangle(combinedBBox);
		
		// Draw 8 resize handles
		int handleSize = 6;
		CBrush handleBrush(RGB(255, 255, 255));
		CBrush* pOldBrush = memDC.SelectObject(&handleBrush);
		
		// Corner handles
		memDC.Rectangle(combinedBBox.left - handleSize/2, combinedBBox.top - handleSize/2, 
		               combinedBBox.left + handleSize/2, combinedBBox.top + handleSize/2);
		memDC.Rectangle(combinedBBox.right - handleSize/2, combinedBBox.top - handleSize/2, 
		               combinedBBox.right + handleSize/2, combinedBBox.top + handleSize/2);
		memDC.Rectangle(combinedBBox.left - handleSize/2, combinedBBox.bottom - handleSize/2, 
		               combinedBBox.left + handleSize/2, combinedBBox.bottom + handleSize/2);
		memDC.Rectangle(combinedBBox.right - handleSize/2, combinedBBox.bottom - handleSize/2, 
		               combinedBBox.right + handleSize/2, combinedBBox.bottom + handleSize/2);
		
		// Edge handles
		int centerX = (combinedBBox.left + combinedBBox.right) / 2;
		int centerY = (combinedBBox.top + combinedBBox.bottom) / 2;
		memDC.Rectangle(centerX - handleSize/2, combinedBBox.top - handleSize/2, 
		               centerX + handleSize/2, combinedBBox.top + handleSize/2);
		memDC.Rectangle(centerX - handleSize/2, combinedBBox.bottom - handleSize/2, 
		               centerX + handleSize/2, combinedBBox.bottom + handleSize/2);
		memDC.Rectangle(combinedBBox.left - handleSize/2, centerY - handleSize/2, 
		               combinedBBox.left + handleSize/2, centerY + handleSize/2);
		memDC.Rectangle(combinedBBox.right - handleSize/2, centerY - handleSize/2, 
		               combinedBBox.right + handleSize/2, centerY + handleSize/2);
		
		// Draw rotation handle (circle above top center)
		int rotateY = combinedBBox.top - 20;
		memDC.Ellipse(centerX - handleSize, rotateY - handleSize, 
		             centerX + handleSize, rotateY + handleSize);
		// Draw line connecting to top center
		memDC.MoveTo(centerX, combinedBBox.top);
		memDC.LineTo(centerX, rotateY);
		
		memDC.SelectObject(pOldPen);
		memDC.SelectObject(pOldBrush);
	}

	if (m_isDrawing) {
		DrawRubberBand(&memDC);
	}
	
	// Draw selection box
	if (m_isDragging && m_dragMode == DRAG_SELECT_BOX) {
		CPen pen(PS_DOT, 1, RGB(0, 120, 215));
		CPen* pOldPen = memDC.SelectObject(&pen);
		memDC.SelectStockObject(NULL_BRUSH);
		memDC.Rectangle(m_selectionBox);
		memDC.SelectObject(pOldPen);
	}
	
	DrawOperationLabels(&memDC);
	// Copy memory DC to screen DC (double buffering)
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	
	// Clean up
	memDC.SelectObject(pOldBitmap);
	memBitmap.DeleteObject();
	memDC.DeleteDC();
}

BOOL CGraphicArtStudioView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CGraphicArtStudioView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CGraphicArtStudioView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

#ifdef _DEBUG
void CGraphicArtStudioView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicArtStudioView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicArtStudioDoc* CGraphicArtStudioView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicArtStudioDoc)));
	return (CGraphicArtStudioDoc*)m_pDocument;
}
#endif

void CGraphicArtStudioView::DrawCoordinateSystem(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	int centerX = rect.Width() / 2;
	int centerY = rect.Height() / 2;

	// Draw grid first (so axes are drawn on top)
	DrawGrid(pDC);

	CPen axisPen(PS_SOLID, 2, RGB(128, 128, 128));
	CPen* pOldPen = pDC->SelectObject(&axisPen);

	// Draw X axis (horizontal) and arrow
	pDC->MoveTo(0, centerY);
	pDC->LineTo(rect.Width(), centerY);
	pDC->MoveTo(rect.Width() - 10, centerY - 5);
	pDC->LineTo(rect.Width(), centerY);
	pDC->LineTo(rect.Width() - 10, centerY + 5);

	// Draw Y axis (vertical) and arrow
	pDC->MoveTo(centerX, rect.Height());
	pDC->LineTo(centerX, 0);
	pDC->MoveTo(centerX - 5, 10);
	pDC->LineTo(centerX, 0);
	pDC->LineTo(centerX + 5, 10);

	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(rect.Width() - 20, centerY + 10, _T("X"));
	pDC->TextOut(centerX + 10, 10, _T("Y"));
	pDC->TextOut(centerX + 5, centerY + 5, _T("O"));

	// Draw tick marks and labels on axes to match the grid
	const int scale = 50; // 与 DrawGrid 保持一致的 scale

	// Tick marks and X-axis labels
	for (int x = centerX; x >= 0; x -= scale) {
		// tick on X axis (left side)
		pDC->MoveTo(x, centerY - 3);
		pDC->LineTo(x, centerY + 3);

		int value = (x - centerX) / scale;
		if (value != 0) {
			CString label;
			label.Format(_T("%d"), value);
			pDC->TextOut(x - 8, centerY + 6, label);
		}
	}
	for (int x = centerX + scale; x < rect.Width(); x += scale) {
		// tick on X axis (right side)
		pDC->MoveTo(x, centerY - 3);
		pDC->LineTo(x, centerY + 3);

		int value = (x - centerX) / scale;
		CString label;
		label.Format(_T("%d"), value);
		pDC->TextOut(x - 8, centerY + 6, label);
	}

	// Tick marks and Y-axis labels (注意 Y 方向为屏幕坐标：向下为增大)
	for (int y = centerY; y >= 0; y -= scale) {
		pDC->MoveTo(centerX - 3, y);
		pDC->LineTo(centerX + 3, y);

		int value = (centerY - y) / scale;
		if (value != 0) {
			CString label;
			label.Format(_T("%d"), value);
			pDC->TextOut(centerX + 6, y - 8, label);
		}
	}
	for (int y = centerY + scale; y < rect.Height(); y += scale) {
		pDC->MoveTo(centerX - 3, y);
		pDC->LineTo(centerX + 3, y);

		int value = (centerY - y) / scale;
		CString label;
		label.Format(_T("%d"), value);
		pDC->TextOut(centerX + 6, y - 8, label);
	}

	// Display scale information
	CString scaleInfo;
	scaleInfo.Format(_T("Scale: %d pixels/unit"), scale);
	pDC->TextOut(10, rect.Height() - 20, scaleInfo);

	pDC->SelectObject(pOldPen);
}

void CGraphicArtStudioView::DrawRubberBand(CDC* pDC)
{
	CPen pen(PS_DOT, 1, m_currentColor);
	CPen* pOldPen = pDC->SelectObject(&pen);

	switch (m_drawMode) {
	case MODE_LINE:
		pDC->MoveTo(m_startPoint);
		pDC->LineTo(m_currentPoint);
		break;
	case MODE_CIRCLE:
	{
		int radius = (int)sqrt(pow(m_currentPoint.x - m_startPoint.x, 2) + 
			pow(m_currentPoint.y - m_startPoint.y, 2));
		pDC->Ellipse(m_startPoint.x - radius, m_startPoint.y - radius,
			m_startPoint.x + radius, m_startPoint.y + radius);
		break;
	}
	case MODE_ELLIPSE:
		pDC->Ellipse(m_startPoint.x, m_startPoint.y, 
			m_currentPoint.x, m_currentPoint.y);
		break;
	case MODE_POLYGON:
	case MODE_BSPLINE:
		if (!m_tempPoints.empty()) {
			pDC->MoveTo(m_tempPoints[0]);
			for (size_t i = 1; i < m_tempPoints.size(); i++) {
				pDC->LineTo(m_tempPoints[i]);
			}
			pDC->LineTo(m_currentPoint);
		}
		break;
	case MODE_BEZIER:
		if (m_pTempCompositeBezier) {
			m_pTempCompositeBezier->Draw(pDC);
			// Draw rubber band from last point to current mouse position
			if (m_pTempCompositeBezier->HasUnfinishedSegment()) {
				// Get the last added control point
				// We need to access the current segment's last point. 
				// Since CGraphBezierComposite doesn't expose it directly, we can track it or just use m_currentPoint
				// Actually, CGraphBezierComposite::Draw already draws the control polygon for the current segment.
				// We just need to draw the line to the mouse cursor.
				// But we don't know the last point easily without adding a getter.
				// Let's just rely on the visual feedback of points appearing.
				// Or better, let's add a getter to CGraphBezierComposite or just assume the user sees the points.
				// To make it look like the original rubber band, we can draw a line from the last added point.
				// But we don't have easy access. Let's skip the rubber band line to cursor for now, 
				// or use m_startPoint if it was updated (it's not).
			}
		}
		break;
	}

	pDC->SelectObject(pOldPen);
}

void CGraphicArtStudioView::DrawCanvasBoundary(CDC* pDC)
{
	CRect clientRect;
	GetClientRect(&clientRect);
	
	// For full-screen canvas, don't fill with white - keep the background color from OnDraw
	if (m_canvasWidth >= clientRect.Width() && m_canvasHeight >= clientRect.Height()) {
		// Canvas is larger than or equal to client area - just show info text
		// Don't fill with white! The background color is already set in OnDraw
		
		// Draw canvas size info at top-left
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 255));
		CString sizeInfo;
		sizeInfo.Format(_T("Canvas: %d x %d (Full Screen)"), m_canvasWidth, m_canvasHeight);
		pDC->TextOut(10, 10, sizeInfo);
	}
	else {
		// Canvas is smaller than client area - draw boundary
		CPen boundaryPen(PS_SOLID, 2, RGB(0, 0, 255));  // Blue boundary
		CPen* pOldPen = pDC->SelectObject(&boundaryPen);
		
		// Calculate canvas rectangle (centered in view)
		int canvasLeft = max(10, (clientRect.Width() - m_canvasWidth) / 2);
		int canvasTop = max(10, (clientRect.Height() - m_canvasHeight) / 2);
		int canvasRight = canvasLeft + m_canvasWidth;
		int canvasBottom = canvasTop + m_canvasHeight;
		
		// Draw canvas boundary
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->Rectangle(canvasLeft, canvasTop, canvasRight, canvasBottom);
		
		// Fill canvas area with white background (for non-fullscreen mode)
		// If animating (Solar System), use the background color instead of white
		COLORREF canvasColor = m_isAnimating ? m_bgColor : RGB(255, 255, 255);
		CBrush canvasBrush(canvasColor);
		CBrush* pOldBrush = pDC->SelectObject(&canvasBrush);
		pDC->Rectangle(canvasLeft + 1, canvasTop + 1, canvasRight - 1, canvasBottom - 1);
		pDC->SelectObject(pOldBrush);
		
		// Draw canvas size info
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 255));
		CString sizeInfo;
		sizeInfo.Format(_T("Canvas: %d x %d"), m_canvasWidth, m_canvasHeight);
		pDC->TextOut(canvasLeft, canvasBottom + 5, sizeInfo);
		
		pDC->SelectObject(pOldPen);
	}
}

CRect CGraphicArtStudioView::GetCanvasRect()
{
	CRect clientRect;
	GetClientRect(&clientRect);
	
	// For full-screen canvas, use entire client area
	if (m_canvasWidth >= clientRect.Width() && m_canvasHeight >= clientRect.Height()) {
		return clientRect;
	}
	else {
		// For smaller canvas, center it in the client area
		int canvasLeft = max(10, (clientRect.Width() - m_canvasWidth) / 2);
		int canvasTop = max(10, (clientRect.Height() - m_canvasHeight) / 2);
		int canvasRight = canvasLeft + m_canvasWidth;
		int canvasBottom = canvasTop + m_canvasHeight;
		
		return CRect(canvasLeft, canvasTop, canvasRight, canvasBottom);
	}
}

bool CGraphicArtStudioView::IsPointInCanvas(CPoint point)
{
	CRect canvasRect = GetCanvasRect();
	return canvasRect.PtInRect(point);
}

void CGraphicArtStudioView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	
	// Handle selection mode
	if (m_drawMode == MODE_SELECT) {
		bool ctrlPressed = (nFlags & MK_CONTROL) != 0;
		
		// Check if clicking on selected shape's handles (works for single or multiple selection)
		if (!m_selectedShapes.empty()) {
			// Calculate bounding box for all selected shapes
			CRect combinedBBox;
			bool first = true;
			for (auto shape : m_selectedShapes) {
				if (shape) {
					CRect bbox = shape->GetBoundingBox();
					if (first) {
						combinedBBox = bbox;
						first = false;
					} else {
						combinedBBox.UnionRect(combinedBBox, bbox);
					}
				}
			}
			
			DragMode handleType = GetHandleAtPoint(point, combinedBBox);
			
			if (handleType != DRAG_NONE) {
				// Start handle dragging for all selected shapes
				m_isDragging = true;
				m_dragStartPoint = point;
				m_dragMode = handleType;
				// Reset rotation angle when starting rotation
				if (handleType == DRAG_ROTATE) {
					m_currentRotationAngle = 0.0;
				}
				Invalidate();
				CView::OnLButtonDown(nFlags, point);
				return;
			}
		}
		
		// Find shape at click point
		CGraphElement* clickedShape = FindShapeAtPoint(point);
		
		if (clickedShape) {
			if (ctrlPressed) {
				// Ctrl+Click: Toggle selection
				if (clickedShape->m_selected) {
					// Deselect
					clickedShape->m_selected = false;
					auto it = std::find(m_selectedShapes.begin(), m_selectedShapes.end(), clickedShape);
					if (it != m_selectedShapes.end()) {
						m_selectedShapes.erase(it);
					}
				} else {
					// Add to selection
					clickedShape->m_selected = true;
					m_selectedShapes.push_back(clickedShape);
				}
				// Update single selection pointer
				m_selectedShape = m_selectedShapes.empty() ? nullptr : m_selectedShapes.back();
			} else {
				// Normal click
				if (clickedShape->m_selected && m_selectedShapes.size() > 1) {
					// Clicking on an already selected shape in multi-selection
					// Don't clear selection, just start dragging all selected shapes
					m_isDragging = true;
					m_dragStartPoint = point;
					m_dragMode = DRAG_MOVE;
				} else {
					// Clear previous selection and select this shape
					ClearSelection();
					clickedShape->m_selected = true;
					m_selectedShape = clickedShape;
					m_selectedShapes.clear();
					m_selectedShapes.push_back(clickedShape);
					// Start dragging for move
					m_isDragging = true;
					m_dragStartPoint = point;
					m_dragMode = DRAG_MOVE;
				}
			}
		} else {
			// Clicked on empty space
			if (!ctrlPressed) {
				ClearSelection();
			}
			// Start box selection
			m_isDragging = true;
			m_dragStartPoint = point;
			m_dragMode = DRAG_SELECT_BOX;
			m_selectionBox = CRect(point, point);
		}
		Invalidate();
	} else if (m_drawMode == MODE_SEED_FILL) {
		// Handle seed fill mode - perform region-based flood fill
		CGraphicArtStudioDoc* pDoc = GetDocument();
		if (!pDoc) return;
		
		// Always exit seed fill mode after one click
		bool fillResult = PerformFloodFill(point, m_seedFillColor);
		
		// Force exit seed fill mode regardless of result
		m_drawMode = MODE_SELECT;
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		
		if (fillResult) {
			Invalidate();
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame) {
				pFrame->SetMessageText(_T("Fill completed. Returned to selection mode."));
			}
		}
	} else if (m_drawMode == MODE_CLIP_WINDOW) {
		// Start dragging clipping window
		m_isDragging = true;
		m_dragStartPoint = point;
		m_dragMode = DRAG_SELECT_BOX; // Reuse box selection visual
		m_selectionBox = CRect(point, point);
		Invalidate();
	} else if (m_drawMode == MODE_TEXT) {
		// Handle text input
		CGraphTextInputDlg dlg;
		dlg.m_textColor = m_currentColor;
		
		if (dlg.DoModal() == IDOK && !dlg.m_text.IsEmpty()) {
			CGraphicArtStudioDoc* pDoc = GetDocument();
			if (pDoc) {
				SaveUndoState();
				
				CGraphText* pText = new CGraphText(point, dlg.m_text, dlg.m_fontName, dlg.m_fontSize, dlg.m_textColor);
				pDoc->AddShape(pText);
				
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
				if (pFrame) {
					CString msg;
					msg.Format(_T("已添加文字: %s"), dlg.m_text);
					pFrame->SetMessageText(msg);
				}
				
				// Reset to select mode after adding text
				ResetDrawingMode();
				Invalidate();
			}
		}
	} else {
		// Normal drawing mode
		m_isDrawing = true;
		m_startPoint = point;
		m_currentPoint = point;

		if (m_drawMode == MODE_POLYGON || m_drawMode == MODE_BSPLINE) {
			m_tempPoints.push_back(point);
		}
		else if (m_drawMode == MODE_BEZIER) {
			if (m_pTempCompositeBezier == nullptr) {
				m_pTempCompositeBezier = new CGraphBezierComposite(m_currentColor, m_currentWidth);
				m_pTempCompositeBezier->StartNewSegment(m_currentColor);
			}
			m_pTempCompositeBezier->AddControlPoint(point);
			// 查询当前未完成段的控制点数量并显示对应的 Bezier 次数
			int cnt = m_pTempCompositeBezier->GetCurrentSegmentPointCount();
			int degree = max(0, cnt - 1); // points - 1
			CString algoStr;
			if (degree <= 0) {
				algoStr = _T("Bezier: 尚未形成曲线（控制点不足）");
			}
			else if (degree == 1) {
				algoStr = _T("Bezier: 一次（线性）");
			}
			else if (degree == 2) {
				algoStr = _T("Bezier: 二次（Quadratic）");
			}
			else if (degree == 3) {
				algoStr = _T("Bezier: 三次（Cubic）");
			}
			else {
				algoStr.Format(_T("Bezier: %d 次"), degree);
			}
			SetOperationLabel(_T("操作: Bezier 曲线（添加控制点）"));
			SetAlgorithmLabel(algoStr);
			Invalidate();
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CGraphicArtStudioView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_drawMode == MODE_BEZIER && m_pTempCompositeBezier) {
		// Finish current segment and start a new one
		m_pTempCompositeBezier->FinishCurrentSegment();
		
		// Generate a random color for the next segment to distinguish it
		COLORREF nextColor = RGB(rand() % 200, rand() % 200, rand() % 200);
		m_pTempCompositeBezier->StartNewSegment(nextColor);
		
		Invalidate();
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			pFrame->SetMessageText(_T("Started new Bezier segment. Double-click to finish."));
		}
	}
	
	CView::OnRButtonDown(nFlags, point);
}

void CGraphicArtStudioView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	
	// Handle drag end in select mode
	if (m_isDragging) {
		// Handle clipping window definition
		if (m_drawMode == MODE_CLIP_WINDOW) {
			m_isDragging = false;
			m_dragMode = DRAG_NONE;
			
			// Update selection box one last time
			m_selectionBox = CRect(m_dragStartPoint, point);
			m_selectionBox.NormalizeRect();
			
			CRect clipWindow = m_selectionBox;
			
			// Ensure window has some size
			if (clipWindow.Width() < 5 || clipWindow.Height() < 5) {
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
				if (pFrame) pFrame->SetMessageText(_T("Clipping window too small"));
				ResetDrawingMode();
				Invalidate();
				return;
			}
			
			SaveUndoState();
			
			if (m_clipTarget == 1) { // Line Clipping
				int algo = 0;
				int result = MessageBox(_T("Use Cohen-Sutherland Algorithm? (Yes)\nNo for Midpoint Subdivision\nCancel for Liang-Barsky"), 
					_T("Select Algorithm"), MB_YESNOCANCEL | MB_ICONQUESTION);
					
				if (result == IDYES) algo = 0;
				else if (result == IDNO) algo = 1;
				else if (result == IDCANCEL) algo = 2;
				
				int clippedCount = 0;
				std::vector<CGraphLine*> clippedLines;
				
				for (auto shape : pDoc->m_shapeList) {
					if (shape == nullptr) continue;
					CGraphLine* line = dynamic_cast<CGraphLine*>(shape);
					if (line) {
						CGraphLine* clippedLine = CGraphLineClipping::ClipLine(line, clipWindow, algo);
						if (clippedLine) {
							clippedLines.push_back(clippedLine);
							clippedCount++;
						}
					}
				}
				
				for (auto clippedLine : clippedLines) {
					pDoc->AddShape(clippedLine);
				}
				
				// Draw clipping window
				CGraphPolygon* clipRect = new CGraphPolygon(RGB(0, 255, 0), 2, false, RGB(255, 255, 255));
				clipRect->AddPoint(CPoint(clipWindow.left, clipWindow.top));
				clipRect->AddPoint(CPoint(clipWindow.right, clipWindow.top));
				clipRect->AddPoint(CPoint(clipWindow.right, clipWindow.bottom));
				clipRect->AddPoint(CPoint(clipWindow.left, clipWindow.bottom));
				clipRect->Close();
				clipRect->m_transformLabel = _T("Clipping Window");
				pDoc->AddShape(clipRect);
				
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
				if (pFrame) {
					CString msg;
					msg.Format(_T("Line clipping completed: %d lines clipped"), clippedCount);
					pFrame->SetMessageText(msg);
				}
			}
			else if (m_clipTarget == 2) { // Polygon Clipping
				int algo = 0;
				int result = MessageBox(_T("Use Sutherland-Hodgeman Algorithm? (Yes)\nNo for Weiler-Atherton"), 
					_T("Select Algorithm"), MB_YESNO | MB_ICONQUESTION);
					
				if (result == IDYES) algo = 0;
				else algo = 1;
				
				int clippedCount = 0;
				std::vector<CGraphPolygon*> clippedPolygons;
				
				for (auto shape : pDoc->m_shapeList) {
					if (shape == nullptr) continue;
					CGraphPolygon* polygon = dynamic_cast<CGraphPolygon*>(shape);
					if (polygon && polygon->m_closed && polygon->m_points.size() >= 3) {
						CGraphPolygon* clippedPolygon = CGraphPolygonClipping::ClipPolygon(polygon, clipWindow, algo);
						if (clippedPolygon) {
							clippedPolygons.push_back(clippedPolygon);
							clippedCount++;
						}
					}
				}
				
				for (auto clippedPolygon : clippedPolygons) {
					pDoc->AddShape(clippedPolygon);
				}
				
				// Draw clipping window
				CGraphPolygon* clipRect = new CGraphPolygon(RGB(0, 255, 0), 2, false, RGB(255, 255, 255));
				clipRect->AddPoint(CPoint(clipWindow.left, clipWindow.top));
				clipRect->AddPoint(CPoint(clipWindow.right, clipWindow.top));
				clipRect->AddPoint(CPoint(clipWindow.right, clipWindow.bottom));
				clipRect->AddPoint(CPoint(clipWindow.left, clipWindow.bottom));
				clipRect->Close();
				clipRect->m_transformLabel = _T("Clipping Window");
				pDoc->AddShape(clipRect);
				
				// Add algorithm label
				CGraphText* label = new CGraphText(CPoint(clipWindow.left + 10, clipWindow.top - 20), 
					_T("Sutherland-Hodgeman Algorithm"), _T("Arial"), 10, RGB(0, 128, 0));
				label->m_transformLabel = _T("Algorithm Label");
				pDoc->AddShape(label);
				
				Invalidate();
				
				// Show result in status bar
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
				if (pFrame) {
					CString msg;
					msg.Format(_T("Polygon clipping completed: %d polygons clipped"), clippedCount);
					pFrame->SetMessageText(msg);
				}
			}
			
			ResetDrawingMode();
			Invalidate();
			return;
		}

		if (m_dragMode == DRAG_SELECT_BOX) {
			// Complete box selection
			m_selectionBox = CRect(m_dragStartPoint, point);
			m_selectionBox.NormalizeRect();
			
			// Select all shapes within the box
			bool ctrlPressed = (nFlags & MK_CONTROL) != 0;
			if (!ctrlPressed) {
				ClearSelection();
			}
			
			for (auto shape : pDoc->m_shapeList) {
				if (shape) {
					CRect bbox = shape->GetBoundingBox();
					if (m_selectionBox.IntersectRect(m_selectionBox, bbox)) {
						shape->m_selected = true;
						if (std::find(m_selectedShapes.begin(), m_selectedShapes.end(), shape) == m_selectedShapes.end()) {
							m_selectedShapes.push_back(shape);
						}
					}
				}
			}
			m_selectedShape = m_selectedShapes.empty() ? nullptr : m_selectedShapes.back();
			
			// Show selection count in status bar
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame) {
				CString msg;
				msg.Format(_T("Selected %d shapes"), (int)m_selectedShapes.size());
				pFrame->SetMessageText(msg);
			}
		}
		// Save state after transformation
		else if (m_dragMode == DRAG_SCALE || m_dragMode == DRAG_ROTATE) {
			SaveUndoState();
		}
		m_isDragging = false;
		m_dragMode = DRAG_NONE;
		Invalidate();
		CView::OnLButtonUp(nFlags, point);
		return;
	}
	
	// Handle clipping window definition
	if (m_drawMode == MODE_CLIP_WINDOW && m_isDragging) {
		m_isDragging = false;
		m_dragMode = DRAG_NONE;
		
		CRect clipWindow = m_selectionBox;
		clipWindow.NormalizeRect();
		
		// Ensure window has some size
		if (clipWindow.Width() < 5 || clipWindow.Height() < 5) {
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame) pFrame->SetMessageText(_T("Clipping window too small"));
			ResetDrawingMode();
			Invalidate();
			return;
		}
		
		SaveUndoState();
		
		if (m_clipTarget == 1) { // Line Clipping
			int algo = 0;
			int result = MessageBox(_T("Use Cohen-Sutherland Algorithm? (Yes)\nNo for Midpoint Subdivision\nCancel for Liang-Barsky"), 
				_T("Select Algorithm"), MB_YESNOCANCEL | MB_ICONQUESTION);
				
			if (result == IDYES) algo = 0;
			else if (result == IDNO) algo = 1;
			else if (result == IDCANCEL) algo = 2;
			
			int clippedCount = 0;
			std::vector<CGraphLine*> clippedLines;
			
			for (auto shape : pDoc->m_shapeList) {
				if (shape == nullptr) continue;
				CGraphLine* line = dynamic_cast<CGraphLine*>(shape);
				if (line) {
					CGraphLine* clippedLine = CGraphLineClipping::ClipLine(line, clipWindow, algo);
					if (clippedLine) {
						clippedLines.push_back(clippedLine);
						clippedCount++;
					}
				}
			}
			
			for (auto clippedLine : clippedLines) {
				pDoc->AddShape(clippedLine);
			}
			
			// Draw clipping window
			CGraphPolygon* clipRect = new CGraphPolygon(RGB(0, 255, 0), 2, false, RGB(255, 255, 255));
			clipRect->AddPoint(CPoint(clipWindow.left, clipWindow.top));
			clipRect->AddPoint(CPoint(clipWindow.right, clipWindow.top));
			clipRect->AddPoint(CPoint(clipWindow.right, clipWindow.bottom));
			clipRect->AddPoint(CPoint(clipWindow.left, clipWindow.bottom));
			clipRect->Close();
			clipRect->m_transformLabel = _T("Clipping Window");
			pDoc->AddShape(clipRect);
			
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame) {
				CString msg;
				msg.Format(_T("Line clipping completed: %d lines clipped"), clippedCount);
				pFrame->SetMessageText(msg);
			}
		}
		else if (m_clipTarget == 2) { // Polygon Clipping
			int algo = 0;
			int result = MessageBox(_T("Use Sutherland-Hodgeman Algorithm? (Yes)\nNo for Weiler-Atherton"), 
				_T("Select Algorithm"), MB_YESNO | MB_ICONQUESTION);
				
			if (result == IDYES) algo = 0;
			else algo = 1;
			
			int clippedCount = 0;
			std::vector<CGraphPolygon*> clippedPolygons;
			
			for (auto shape : pDoc->m_shapeList) {
				if (shape == nullptr) continue;
				CGraphPolygon* polygon = dynamic_cast<CGraphPolygon*>(shape);
				if (polygon && polygon->m_closed && polygon->m_points.size() >= 3) {
					CGraphPolygon* clippedPolygon = CGraphPolygonClipping::ClipPolygon(polygon, clipWindow, algo);
					if (clippedPolygon) {
						clippedPolygons.push_back(clippedPolygon);
						clippedCount++;
					}
				}
			}
			
			for (auto clippedPolygon : clippedPolygons) {
				pDoc->AddShape(clippedPolygon);
			}
			
			// Draw clipping window
			CGraphPolygon* clipRect = new CGraphPolygon(RGB(0, 255, 0), 2, false, RGB(255, 255, 255));
			clipRect->AddPoint(CPoint(clipWindow.left, clipWindow.top));
			clipRect->AddPoint(CPoint(clipWindow.right, clipWindow.top));
			clipRect->AddPoint(CPoint(clipWindow.right, clipWindow.bottom));
			clipRect->AddPoint(CPoint(clipWindow.left, clipWindow.bottom));
			clipRect->Close();
			clipRect->m_transformLabel = _T("Clipping Window");
			pDoc->AddShape(clipRect);
			
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame) {
				CString msg;
				msg.Format(_T("Polygon clipping completed: %d polygons clipped"), clippedCount);
				pFrame->SetMessageText(msg);
			}
		}
		
		ResetDrawingMode();
		Invalidate();
		return;
	}

	if (!m_isDrawing) return;

	m_endPoint = point;
	
	// Save state before adding new shape
	SaveUndoState();

	switch (m_drawMode) {
	case MODE_LINE:
	{
		CGraphLine* pLine = new CGraphLine(m_startPoint, m_endPoint, m_lineAlgorithm, 
			m_currentColor, m_currentWidth);
		pDoc->AddShape(pLine);
		m_isDrawing = false;
		break;
	}
	case MODE_CIRCLE:
	{
		int radius = (int)sqrt(pow(m_endPoint.x - m_startPoint.x, 2) + 
			pow(m_endPoint.y - m_startPoint.y, 2));
		CGraphCircle* pCircle = new CGraphCircle(m_startPoint, radius, m_currentColor, 
			m_currentWidth, m_filled, m_fillColor);
		pDoc->AddShape(pCircle);
		m_isDrawing = false;
		break;
	}
	case MODE_ELLIPSE:
	{
		int rx = abs(m_endPoint.x - m_startPoint.x) / 2;
		int ry = abs(m_endPoint.y - m_startPoint.y) / 2;
		CPoint center((m_startPoint.x + m_endPoint.x) / 2, 
			(m_startPoint.y + m_endPoint.y) / 2);
		CGraphEllipse* pEllipse = new CGraphEllipse(center, rx, ry, m_currentColor, 
			m_currentWidth, m_filled, m_fillColor);
		pDoc->AddShape(pEllipse);
		m_isDrawing = false;
		break;
	}
	case MODE_TEXT:
	{
		CGraphTextInputDlg dlg;
		dlg.m_textColor = m_currentColor;
		if (dlg.DoModal() == IDOK && !dlg.m_text.IsEmpty()) {
			CGraphText* pText = new CGraphText(point, dlg.m_text, dlg.m_fontName, dlg.m_fontSize, dlg.m_textColor);
			pDoc->AddShape(pText);
		}
		m_isDrawing = false;
		break;
	}
	}

	Invalidate();
	CView::OnLButtonUp(nFlags, point);
}

void CGraphicArtStudioView::OnMouseMove(UINT nFlags, CPoint point)
{
	// Handle dragging in select mode
	if (m_isDragging) {
		switch (m_dragMode) {
		case DRAG_SELECT_BOX:
		{
			// Update selection box
			m_selectionBox = CRect(m_dragStartPoint, point);
			m_selectionBox.NormalizeRect();
			Invalidate();
			break;
		}
		case DRAG_MOVE:
		{
			int dx = point.x - m_dragStartPoint.x;
			int dy = point.y - m_dragStartPoint.y;
			// Move all selected shapes
			for (auto shape : m_selectedShapes) {
				if (shape) shape->Move(dx, dy);
			}
			m_dragStartPoint = point;
			break;
		}
		case DRAG_SCALE:
		{
			// Calculate combined bounding box for all selected shapes
			CRect combinedBBox;
			bool first = true;
			for (auto shape : m_selectedShapes) {
				if (shape) {
					CRect bbox = shape->GetBoundingBox();
					if (first) {
						combinedBBox = bbox;
						first = false;
					} else {
						combinedBBox.UnionRect(combinedBBox, bbox);
					}
				}
			}
			
			// Scale based on distance from center of combined bounding box
			CPoint center((combinedBBox.left + combinedBBox.right) / 2, 
			              (combinedBBox.top + combinedBBox.bottom) / 2);
			
			double oldDist = sqrt(pow(m_dragStartPoint.x - center.x, 2) + pow(m_dragStartPoint.y - center.y, 2));
			double newDist = sqrt(pow(point.x - center.x, 2) + pow(point.y - center.y, 2));
			
			if (oldDist > 0) {
				double scale = newDist / oldDist;
				double matrix[3][3];
				CMatrixTool::ScaleAboutPoint(scale, scale, center, matrix);
				// Apply scaling to all selected shapes
				for (auto shape : m_selectedShapes) {
					if (shape) shape->Transform(matrix);
				}
				m_dragStartPoint = point;
			}
			break;
		}
		case DRAG_ROTATE:
		{
			// Calculate combined bounding box for all selected shapes
			CRect combinedBBox;
			bool first = true;
			for (auto shape : m_selectedShapes) {
				if (shape) {
					CRect bbox = shape->GetBoundingBox();
					if (first) {
						combinedBBox = bbox;
						first = false;
					} else {
						combinedBBox.UnionRect(combinedBBox, bbox);
					}
				}
			}
			
			// Rotate around center of combined bounding box
			CPoint center((combinedBBox.left + combinedBBox.right) / 2, 
			              (combinedBBox.top + combinedBBox.bottom) / 2);
			
			// Calculate angles
			double oldAngle = atan2(m_dragStartPoint.y - center.y, m_dragStartPoint.x - center.x);
			double newAngle = atan2(point.y - center.y, point.x - center.x);
			double deltaAngle = (newAngle - oldAngle) * 180.0 / M_PI;
			
			// Normalize delta angle to avoid large jumps
			while (deltaAngle > 180.0) deltaAngle -= 360.0;
			while (deltaAngle < -180.0) deltaAngle += 360.0;
			
			// Only apply rotation if delta is reasonable (avoid jitter)
			if (abs(deltaAngle) > 0.5 && abs(deltaAngle) < 45.0) {
				// Update cumulative rotation angle
				m_currentRotationAngle += deltaAngle;
				// Normalize to 0-360 range
				while (m_currentRotationAngle < 0) m_currentRotationAngle += 360.0;
				while (m_currentRotationAngle >= 360.0) m_currentRotationAngle -= 360.0;
				
				// Show angle in status bar
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
				if (pFrame) {
					CString msg;
					msg.Format(_T("Rotation angle: %.1f degrees"), m_currentRotationAngle);
					pFrame->SetMessageText(msg);
				}
				
				// Apply rotation to all selected shapes
				double matrix[3][3];
				CMatrixTool::RotateAboutPoint(deltaAngle, center, matrix);
				for (auto shape : m_selectedShapes) {
					if (shape) shape->Transform(matrix);
				}
				m_dragStartPoint = point;
			}
			break;
		}
		}
		Invalidate();
	}
	// Handle normal drawing
	else if (m_isDrawing) {
		// Constrain drawing within canvas boundaries
		CRect canvasRect = GetCanvasRect();
		CPoint constrainedPoint = point;
		
		// Clamp point to canvas boundaries
		if (constrainedPoint.x < canvasRect.left) constrainedPoint.x = canvasRect.left;
		if (constrainedPoint.x > canvasRect.right) constrainedPoint.x = canvasRect.right;
		if (constrainedPoint.y < canvasRect.top) constrainedPoint.y = canvasRect.top;
		if (constrainedPoint.y > canvasRect.bottom) constrainedPoint.y = canvasRect.bottom;
		
		m_currentPoint = constrainedPoint;
		Invalidate(FALSE);  // FALSE = don't erase background, reduces flicker
	}
	
	// Update cursor when not dragging (for hover effects)
	if (!m_isDragging && m_drawMode == MODE_SELECT) {
		// Force cursor update by sending WM_SETCURSOR message
		SendMessage(WM_SETCURSOR, (WPARAM)m_hWnd, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
	}
	
	CView::OnMouseMove(nFlags, point);
}

void CGraphicArtStudioView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CGraphicArtStudioDoc* pDoc = GetDocument();

	// Check if double-click is within canvas for multi-point shapes
	if (!IsPointInCanvas(point)) {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			pFrame->SetMessageText(_T("Cannot complete shape outside canvas boundary"));
		}
		return;
	}

	if (m_drawMode == MODE_POLYGON && m_tempPoints.size() >= 3) {
		CGraphPolygon* pPolygon = new CGraphPolygon(m_currentColor, m_currentWidth, 
			m_filled, m_fillColor);
		for (auto pt : m_tempPoints) {
			pPolygon->AddPoint(pt);
		}
		pPolygon->Close();
		
		// Set fill algorithm if filling is enabled
		if (m_filled) {
			pPolygon->SetFillAlgorithm(FILL_SCANLINE);
		}
		
		pDoc->AddShape(pPolygon);
		m_tempPoints.clear();
		m_isDrawing = false;
		Invalidate();
	}
	else if (m_drawMode == MODE_BEZIER) {
		if (m_pTempCompositeBezier) {
			// 在完成当前段之前/之后均可查询已完成段的次数 ——在 FinishCurrentSegment 之后最后一个段会存在于 segments 中
			m_pTempCompositeBezier->FinishCurrentSegment();

			// 查询刚完成的段次数
			int lastDegree = m_pTempCompositeBezier->GetLastFinishedSegmentDegree();
			CString algoStr;
			if (lastDegree == 1) algoStr = _T("Bezier: 一次（线性）");
			else if (lastDegree == 2) algoStr = _T("Bezier: 二次（Quadratic）");
			else if (lastDegree == 3) algoStr = _T("Bezier: 三次（Cubic）");
			else if (lastDegree > 0) {
				algoStr.Format(_T("Bezier: %d 次"), lastDegree);
			}
			else {
				algoStr = _T("Bezier: 未知次数");
			}

			// 将复合曲线添加到文档并显示标签
			pDoc->AddShape(m_pTempCompositeBezier);

			SetOperationLabel(_T("操作: Bezier 曲线（段完成）"));
			SetAlgorithmLabel(algoStr);

			m_pTempCompositeBezier = nullptr;
			m_isDrawing = false;
			Invalidate();
		}
	}
	else if (m_drawMode == MODE_BSPLINE && m_tempPoints.size() >= 4) {
		CGraphBSpline* pBSpline = new CGraphBSpline(m_currentColor, m_currentWidth, 3);
		for (auto pt : m_tempPoints) {
			pBSpline->AddControlPoint(pt);
		}
		pDoc->AddShape(pBSpline);
		m_tempPoints.clear();
		m_isDrawing = false;
		Invalidate();
	}

	CView::OnLButtonDblClk(nFlags, point);
}

BOOL CGraphicArtStudioView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT) {
		// Set cursor based on current mode
		if (m_drawMode == MODE_SELECT) {
			if (m_isDragging) {
				switch (m_dragMode) {
				case DRAG_MOVE:
					SetCursor(LoadCursor(NULL, IDC_SIZEALL));  // Four-way arrow
					return TRUE;
				case DRAG_SCALE:
					SetCursor(LoadCursor(NULL, IDC_SIZENWSE));  // Diagonal resize
					return TRUE;
				case DRAG_ROTATE:
					SetCursor(LoadCursor(NULL, IDC_CROSS));  // Cross for rotation
					return TRUE;
				case DRAG_SELECT_BOX:
					SetCursor(LoadCursor(NULL, IDC_CROSS));  // Cross for box selection
					return TRUE;
				}
			}
			else {
				// Check if mouse is over a shape or handle
				CPoint point;
				GetCursorPos(&point);
				ScreenToClient(&point);
				
				// Check if over selected shape's handles
				if (m_selectedShapes.size() == 1 && m_selectedShape && m_selectedShape->m_selected) {
					CRect bbox = m_selectedShape->GetBoundingBox();
					DragMode handleType = GetHandleAtPoint(point, bbox);
					
					if (handleType == DRAG_ROTATE) {
						SetCursor(LoadCursor(NULL, IDC_CROSS));  // Cross cursor for rotation
						return TRUE;
					}
					else if (handleType == DRAG_SCALE) {
						SetCursor(LoadCursor(NULL, IDC_SIZENWSE));  // Resize cursor for scale handles
						return TRUE;
					}
				}
				
				// Check if over any shape (for moving)
				CGraphElement* hoverShape = FindShapeAtPoint(point);
				if (hoverShape) {
					SetCursor(LoadCursor(NULL, IDC_SIZEALL));  // Move cursor when over shape
					return TRUE;
				}
			}
			
			// Default selection cursor
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			return TRUE;
		}
		else {
			// Drawing mode - use crosshair
			SetCursor(LoadCursor(NULL, IDC_CROSS));
			return TRUE;
		}
	}
	
	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void CGraphicArtStudioView::OnDrawLine()
{
	ResetDrawingMode();
	m_drawMode = MODE_LINE;
	m_tempPoints.clear();
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	// 设置可视化标签
	SetOperationLabel(_T("操作: 直线绘制"));
	// 将当前的线算法名称也显示出来
	switch (m_lineAlgorithm) {
	case 0: SetAlgorithmLabel(_T("算法: DDA算法")); break;
	case 1: SetAlgorithmLabel(_T("算法: Midpoint算法")); break;
	case 2: SetAlgorithmLabel(_T("算法: Bresenham算法")); break;
	case 3: SetAlgorithmLabel(_T("算法: 系统绘制")); break;
	default: SetAlgorithmLabel(_T("算法: 未知")); break;
	}
}

void CGraphicArtStudioView::OnDrawLineInput()
{
	CGraphLineInputDlg dlg;
	if (dlg.DoModal() == IDOK) {
		SaveUndoState();
		
		CGraphicArtStudioDoc* pDoc = GetDocument();
		CPoint start = dlg.GetStartPoint();
		CPoint end = dlg.GetEndPoint();
		
		CGraphLine* pLine = new CGraphLine(start, end, m_lineAlgorithm, m_currentColor, m_currentWidth);
		pLine->m_transformLabel.Format(_T("Line Input: (%d,%d) to (%d,%d)"), 
			start.x, start.y, end.x, end.y);
		
		pDoc->AddShape(pLine);
		SetOperationLabel(_T("操作: 直线（坐标输入）"));
		switch (m_lineAlgorithm) {
		case 0: SetAlgorithmLabel(_T("算法: DDA算法")); break;
		case 1: SetAlgorithmLabel(_T("算法: Midpoint算法")); break;
		case 2: SetAlgorithmLabel(_T("算法: Bresenham算法")); break;
		case 3: SetAlgorithmLabel(_T("算法: 系统绘制")); break;
		default: SetAlgorithmLabel(_T("算法: 未知")); break;
		}
		Invalidate();
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			CString msg;
			msg.Format(_T("Line created by coordinate input: (%d,%d) to (%d,%d)"), 
				start.x, start.y, end.x, end.y);
			pFrame->SetMessageText(msg);
		}
	}
}

void CGraphicArtStudioView::OnDrawCircle()
{
	ResetDrawingMode();
	m_drawMode = MODE_CIRCLE;
	m_tempPoints.clear();
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	SetOperationLabel(_T("操作: 圆绘制"));
	SetAlgorithmLabel(_T("算法: 中点算法"));
}

void CGraphicArtStudioView::OnDrawCircleInput()
{
	CGraphCircleInputDlg dlg;
	if (dlg.DoModal() == IDOK) {
		CGraphicArtStudioDoc* pDoc = GetDocument();
		CPoint center(dlg.m_centerX, dlg.m_centerY);
		CGraphCircle* pCircle = new CGraphCircle(center, dlg.m_radius, m_currentColor, 
			m_currentWidth, m_filled, m_fillColor);
		pDoc->AddShape(pCircle);
		Invalidate();
	}
}

void CGraphicArtStudioView::OnDrawEllipse()
{
	ResetDrawingMode();
	m_drawMode = MODE_ELLIPSE;
	m_tempPoints.clear();
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	SetOperationLabel(_T("操作: 椭圆绘制"));
	SetAlgorithmLabel(_T("算法: 中点算法"));
}

void CGraphicArtStudioView::OnDrawPolygon()
{
	// Force reset any special modes first
	ResetDrawingMode();
	
	m_drawMode = MODE_POLYGON;
	m_tempPoints.clear();
	
	// Reset cursor to normal
	SetCursor(LoadCursor(NULL, IDC_ARROW));
	
	// Update status
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		pFrame->SetMessageText(_T("Polygon mode: Click to add points, right-click to finish"));
	}
}

void CGraphicArtStudioView::OnDrawText()
{
	ResetDrawingMode();
	m_drawMode = MODE_TEXT;
	SetCursor(LoadCursor(NULL, IDC_IBEAM)); // Use I-beam cursor for text
	
	SetOperationLabel(_T("操作: 文本输入"));
	SetAlgorithmLabel(_T("算法: 系统文本绘制"));
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		pFrame->SetMessageText(_T("请在画布上点击以输入文字"));
	}
}

void CGraphicArtStudioView::OnDrawBezier()
{
	m_drawMode = MODE_BEZIER;
	m_tempPoints.clear();
	if (m_pTempCompositeBezier) {
		delete m_pTempCompositeBezier;
		m_pTempCompositeBezier = nullptr;
	}
	// 显示操作标签（进入 Bezier 模式）
	SetOperationLabel(_T("操作: Bezier 曲线"));
	SetAlgorithmLabel(_T("一次/二次/三次 (根据控制点数实时显示)"));
}

void CGraphicArtStudioView::OnDrawBSpline()
{
	m_drawMode = MODE_BSPLINE;
	m_tempPoints.clear();
}

void CGraphicArtStudioView::OnEditUndo()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) return;
	
	if (m_undoStack.empty()) {
		// Show message in status bar
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame) pFrame->SetMessageText(_T("Cannot undo: no undoable operations"));
		return;
	}
	
	// Save current state to redo stack
	std::vector<CGraphElement*> currentState;
	for (auto shape : pDoc->m_shapeList) {
		if (shape) currentState.push_back(shape->Clone());
	}
	m_redoStack.push_back(currentState);
	
	// Restore previous state
	for (auto shape : pDoc->m_shapeList) {
		delete shape;
	}
	pDoc->m_shapeList = m_undoStack.back();
	m_undoStack.pop_back();
	
	ClearSelection();
	Invalidate();
	
	// Show success message in status bar
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		CString msg;
		msg.Format(_T("Undo successful (%d steps remaining)"), (int)m_undoStack.size());
		pFrame->SetMessageText(msg);
	}
}

void CGraphicArtStudioView::OnEditRedo()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) return;
	
	if (m_redoStack.empty()) {
		// Show message in status bar
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame) pFrame->SetMessageText(_T("Cannot redo: no redoable operations"));
		return;
	}
	
	// Save current state to undo stack
	std::vector<CGraphElement*> currentState;
	for (auto shape : pDoc->m_shapeList) {
		if (shape) currentState.push_back(shape->Clone());
	}
	m_undoStack.push_back(currentState);
	
	// Restore redo state
	for (auto shape : pDoc->m_shapeList) {
		delete shape;
	}
	pDoc->m_shapeList = m_redoStack.back();
	m_redoStack.pop_back();
	
	ClearSelection();
	Invalidate();
	
	// Show success message in status bar
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		CString msg;
		msg.Format(_T("Redo successful (%d steps remaining)"), (int)m_redoStack.size());
		pFrame->SetMessageText(msg);
	}
}

void CGraphicArtStudioView::OnEditClear()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) return;
	
	// If animating, stop animation first
	if (m_isAnimating) {
		StopAnimation();
		// Reset background color to white
		m_bgColor = RGB(255, 255, 255);
	}

	if (pDoc->m_shapeList.empty()) {
		// Show message in status bar
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame) pFrame->SetMessageText(_T("Canvas is already empty"));
		return;
	}
	
	// Confirm clear operation
		int result = AfxMessageBox(_T("Are you sure you want to clear all shapes?"), MB_YESNO | MB_ICONQUESTION);
	if (result != IDYES) {
		return;
	}
	
	// Save state before clearing
	SaveUndoState();
	
	int shapeCount = pDoc->m_shapeList.size();
	for (auto shape : pDoc->m_shapeList) {
		delete shape;
	}
	pDoc->m_shapeList.clear();
	m_tempPoints.clear();
	m_isDrawing = false;
	ClearSelection();
	
	// Show message in status bar
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		CString msg;
		msg.Format(_T("Cleared %d shapes from canvas"), shapeCount);
		pFrame->SetMessageText(msg);
	}
	Invalidate();
}

void CGraphicArtStudioView::OnToolSelect()
{
	m_drawMode = MODE_SELECT;
	m_tempPoints.clear();
	
	// Show message in status bar
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) pFrame->SetMessageText(_T("Select Tool activated. Click shapes to select, drag to move."));
}

// File menu handlers
void CGraphicArtStudioView::OnFileNew()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	for (auto shape : pDoc->m_shapeList) {
		delete shape;
	}
	pDoc->m_shapeList.clear();
	m_tempPoints.clear();
	m_isDrawing = false;
	Invalidate();
}

void CGraphicArtStudioView::OnFileOpen()
{
	// 使用文件对话框打开文件
	CFileDialog dlg(TRUE, _T("dat"), NULL, 
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("绘图文件 (*.dat)|*.dat|所有文件 (*.*)|*.*||"));
	
	if (dlg.DoModal() == IDOK)
	{
		CGraphicArtStudioDoc* pDoc = GetDocument();
		if (pDoc)
		{
			try
			{
				// 打开文件
				CFile file;
				if (file.Open(dlg.GetPathName(), CFile::modeRead))
				{
					CArchive ar(&file, CArchive::load);
					pDoc->Serialize(ar);
					ar.Close();
					file.Close();
					
					pDoc->SetPathName(dlg.GetPathName());
					pDoc->SetModifiedFlag(FALSE);
					Invalidate(); // 重新绘制视图
					
					AfxMessageBox(_T("文件打开成功！"));
				}
				else
				{
					AfxMessageBox(_T("无法打开文件！"));
				}
			}
			catch (CException* e)
			{
				AfxMessageBox(_T("文件格式错误或文件损坏！"));
				e->Delete();
			}
		}
	}
}

void CGraphicArtStudioView::OnFileSave()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (pDoc)
	{
		if (pDoc->GetPathName().IsEmpty())
		{
			// 如果没有文件名，调用另存为
			OnFileSaveAs();
		}
		else
		{
			try
			{
				// 保存到当前文件
				CFile file;
				if (file.Open(pDoc->GetPathName(), CFile::modeCreate | CFile::modeWrite))
				{
					CArchive ar(&file, CArchive::store);
					pDoc->Serialize(ar);
					ar.Close();
					file.Close();
					pDoc->SetModifiedFlag(FALSE);
					
					AfxMessageBox(_T("文件保存成功！"));
				}
				else
				{
					AfxMessageBox(_T("无法保存文件！"));
				}
			}
			catch (CException* e)
			{
				AfxMessageBox(_T("保存文件时发生错误！"));
				e->Delete();
			}
		}
	}
}

// File save as function
void CGraphicArtStudioView::OnFileSaveAs()
{
	// 使用文件对话框另存为
	CFileDialog dlg(FALSE, _T("dat"), NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("绘图文件 (*.dat)|*.dat|所有文件 (*.*)|*.*||"));
	
	if (dlg.DoModal() == IDOK)
	{
		CGraphicArtStudioDoc* pDoc = GetDocument();
		if (pDoc)
		{
			try
			{
				CFile file;
				if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
				{
					CArchive ar(&file, CArchive::store);
					pDoc->Serialize(ar);
					ar.Close();
					file.Close();
					
					pDoc->SetPathName(dlg.GetPathName());
					pDoc->SetModifiedFlag(FALSE);
					
					AfxMessageBox(_T("文件另存为成功！"));
				}
				else
				{
					AfxMessageBox(_T("无法创建文件！"));
				}
			}
			catch (CException* e)
			{
				AfxMessageBox(_T("保存文件时发生错误！"));
				e->Delete();
			}
		}
	}
}

void CGraphicArtStudioView::OnEditSelectAll()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc || pDoc->m_shapeList.empty()) {
		// Show message in status bar
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) pFrame->SetMessageText(_T("No shapes to select"));
		return;
	}
	
	// Select all shapes
	int selectedCount = 0;
	for (auto shape : pDoc->m_shapeList) {
		if (shape) {
			shape->m_selected = true;
			selectedCount++;
		}
	}
	
	// Set the last shape as the primary selected shape
	if (!pDoc->m_shapeList.empty()) {
		m_selectedShape = pDoc->m_shapeList.back();
	}
	
	// Show message in status bar
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		CString msg;
		msg.Format(_T("Selected %d shapes"), selectedCount);
		pFrame->SetMessageText(msg);
	}
	
	Invalidate();
}

// Transform menu handlers
void CGraphicArtStudioView::OnTransformTranslate()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (pDoc->m_shapeList.empty()) {
		// Show message in status bar instead of MessageBox
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) pFrame->SetMessageText(_T("Please draw a shape first"));
		return;
	}
	
	// Use dialog to get parameters
	CTransformDlg dlg(TRANSFORM_TRANSLATE);
	dlg.m_dx = 100;  // Default values
	dlg.m_dy = 50;
	
	if (dlg.DoModal() == IDOK) {
		// Save state for undo
		SaveUndoState();
		
		CGraphElement* pShape = pDoc->m_shapeList.back();
		CGraphElement* pNewShape = pShape->Clone();
		
		double matrix[3][3];
		CMatrixTool::Translate(dlg.m_dx, dlg.m_dy, matrix);
		pNewShape->Transform(matrix);
		
		pNewShape->m_isTransformed = true;
		pNewShape->m_transformLabel.Format(_T("Translate(%.0f,%.0f)"), dlg.m_dx, dlg.m_dy);
		pNewShape->m_color = RGB(255, 0, 0);  // Red for translated shape
		
		pDoc->AddShape(pNewShape);
		SetOperationLabel(_T("操作: 平移变换"));
		SetAlgorithmLabel(_T("算法: 矩阵变换"));
		Invalidate();
		
		// Show confirmation in status bar
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			CString msg;
			msg.Format(_T("Applied translation: dx=%.0f, dy=%.0f"), dlg.m_dx, dlg.m_dy);
			pFrame->SetMessageText(msg);
		}
	}
}

void CGraphicArtStudioView::OnTransformScale()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (pDoc->m_shapeList.empty()) {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) pFrame->SetMessageText(_T("Please draw a shape first"));
		return;
	}
	
	CTransformDlg dlg(TRANSFORM_SCALE);
	dlg.m_sx = 1.5;
	dlg.m_sy = 1.5;
	dlg.m_scaleCenter = CPoint(200, 200);
	
	if (dlg.DoModal() == IDOK) {
		SaveUndoState();
		
		CGraphElement* pShape = pDoc->m_shapeList.back();
		CGraphElement* pNewShape = pShape->Clone();
		
		double matrix[3][3];
		CMatrixTool::ScaleAboutPoint(dlg.m_sx, dlg.m_sy, dlg.m_scaleCenter, matrix);
		pNewShape->Transform(matrix);
		
		pNewShape->m_isTransformed = true;
		pNewShape->m_transformLabel.Format(_T("Scale(%.2f,%.2f)"), dlg.m_sx, dlg.m_sy);
		pNewShape->m_color = RGB(0, 255, 0);
		
		pDoc->AddShape(pNewShape);

		// 绘制参考点 (Requirement 2.4)
		CGraphCircle* pCenterPoint = new CGraphCircle(dlg.m_scaleCenter, 3, RGB(255, 0, 0), 1, true, RGB(255, 0, 0));
		pDoc->AddShape(pCenterPoint);

		// 标注参考点坐标
		CString strCoord;
		strCoord.Format(_T("Ref(%.0f, %.0f)"), (double)dlg.m_scaleCenter.x, (double)dlg.m_scaleCenter.y);
		CGraphText* pCenterLabel = new CGraphText(CPoint(dlg.m_scaleCenter.x + 10, dlg.m_scaleCenter.y + 10), strCoord, _T("宋体"), 16, RGB(255, 0, 0));
		pDoc->AddShape(pCenterLabel);

		SetOperationLabel(_T("操作: 缩放变换")); 
		SetAlgorithmLabel(_T("算法: 矩阵变换"));
		Invalidate();
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			CString msg;
			msg.Format(_T("Applied scaling: sx=%.2f, sy=%.2f"), dlg.m_sx, dlg.m_sy);
			pFrame->SetMessageText(msg);
		}
	}
}

void CGraphicArtStudioView::OnTransformRotate()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (pDoc->m_shapeList.empty()) {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) pFrame->SetMessageText(_T("Please draw a shape first"));
		return;
	}
	
	CTransformDlg dlg(TRANSFORM_ROTATE);
	dlg.m_angle = 45;  // Default angle
	dlg.m_rotateCenter = CPoint(200, 200);  // Default center
	
	if (dlg.DoModal() == IDOK) {
		SaveUndoState();
		
		CGraphElement* pShape = pDoc->m_shapeList.back();
		CGraphElement* pNewShape = pShape->Clone();
		
		double matrix[3][3];
		CMatrixTool::RotateAboutPoint(dlg.m_angle, dlg.m_rotateCenter, matrix);
		pNewShape->Transform(matrix);
		
		pNewShape->m_isTransformed = true;
		pNewShape->m_transformLabel.Format(_T("Rotate %.0f deg"), dlg.m_angle);
		pNewShape->m_color = RGB(0, 0, 255);  // Blue for rotated shape
		
		pDoc->AddShape(pNewShape);

		// 绘制参考点 (Requirement 2.4)
		CGraphCircle* pCenterPoint = new CGraphCircle(dlg.m_rotateCenter, 3, RGB(255, 0, 0), 1, true, RGB(255, 0, 0));
		pDoc->AddShape(pCenterPoint);

		// 标注参考点坐标
		CString strCoord;
		strCoord.Format(_T("Ref(%.0f, %.0f)"), (double)dlg.m_rotateCenter.x, (double)dlg.m_rotateCenter.y);
		CGraphText* pCenterLabel = new CGraphText(CPoint(dlg.m_rotateCenter.x + 10, dlg.m_rotateCenter.y + 10), strCoord, _T("宋体"), 16, RGB(255, 0, 0));
		pDoc->AddShape(pCenterLabel);

		SetOperationLabel(_T("操作: 旋转变换")); 
		SetAlgorithmLabel(_T("算法: 矩阵变换"));
		Invalidate();
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			CString msg;
			msg.Format(_T("Applied rotation: angle=%.0f degrees"), dlg.m_angle);
			pFrame->SetMessageText(msg);
		}
	}
}

void CGraphicArtStudioView::OnTransformShear()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (pDoc->m_shapeList.empty()) {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) pFrame->SetMessageText(_T("Please draw a shape first"));
		return;
	}
	
	CTransformDlg dlg(TRANSFORM_SHEAR);
	dlg.m_shx = 0.5;  // Default shear in X
	dlg.m_shy = 0.0;  // Default shear in Y
	dlg.m_scaleCenter = CPoint(200, 200); // Default center (reusing m_scaleCenter)
	
	if (dlg.DoModal() == IDOK) {
		SaveUndoState();
		
		CGraphElement* pShape = pDoc->m_shapeList.back();
		CGraphElement* pNewShape = pShape->Clone();
		
		double matrix[3][3];
		// Use ShearAboutPoint
		CMatrixTool::ShearAboutPoint(dlg.m_shx, dlg.m_shy, dlg.m_scaleCenter, matrix);
		pNewShape->Transform(matrix);
		
		pNewShape->m_isTransformed = true;
		pNewShape->m_transformLabel.Format(_T("Shear(%.2f,%.2f)"), dlg.m_shx, dlg.m_shy);
		pNewShape->m_color = RGB(255, 165, 0);  // Orange for sheared shape
		
		pDoc->AddShape(pNewShape);

		// 绘制参考点 (Requirement 2.4)
		CGraphCircle* pCenterPoint = new CGraphCircle(dlg.m_scaleCenter, 3, RGB(255, 0, 0), 1, true, RGB(255, 0, 0));
		pDoc->AddShape(pCenterPoint);

		// 标注参考点坐标
		CString strCoord;
		strCoord.Format(_T("Ref(%.0f, %.0f)"), (double)dlg.m_scaleCenter.x, (double)dlg.m_scaleCenter.y);
		CGraphText* pCenterLabel = new CGraphText(CPoint(dlg.m_scaleCenter.x + 10, dlg.m_scaleCenter.y + 10), strCoord, _T("宋体"), 16, RGB(255, 0, 0));
		pDoc->AddShape(pCenterLabel);

		SetOperationLabel(_T("操作: 错切变换")); 
		SetAlgorithmLabel(_T("算法: 矩阵变换"));
		Invalidate();
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			CString msg;
			msg.Format(_T("Applied shear: shx=%.2f, shy=%.2f"), dlg.m_shx, dlg.m_shy);
			pFrame->SetMessageText(msg);
		}
	}
}

void CGraphicArtStudioView::OnTransformSymmetry()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (pDoc->m_shapeList.empty()) {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) pFrame->SetMessageText(_T("Please draw a shape first"));
		return;
	}
	
	CTransformDlg dlg(TRANSFORM_SYMMETRY);
	dlg.m_symmetryType = 0;  // Default: X-axis symmetry
	
	if (dlg.DoModal() == IDOK) {
		SaveUndoState();
		
		CGraphElement* pShape = pDoc->m_shapeList.back();
		CGraphElement* pNewShape = pShape->Clone();
		
		double matSym[3][3];
		CString label;
		
		switch (dlg.m_symmetryType) {
		case 0: // X-axis symmetry
			CMatrixTool::SymmetryX(matSym);
			label = _T("Symmetry about X-axis");
			break;
		case 1: // Y-axis symmetry
			CMatrixTool::SymmetryY(matSym);
			label = _T("Symmetry about Y-axis");
			break;
		case 2: // Origin symmetry
			CMatrixTool::SymmetryOrigin(matSym);
			label = _T("Symmetry about origin");
			break;
		case 3: // Line symmetry
			CMatrixTool::SymmetryLine(dlg.m_lineA, dlg.m_lineB, dlg.m_lineC, matSym);
			label.Format(_T("Symmetry about line %.1fx+%.1fy+%.1f=0"), dlg.m_lineA, dlg.m_lineB, dlg.m_lineC);
			break;
		}

		// Get canvas center for coordinate transformation
		CRect rect;
		GetClientRect(&rect);
		double cx = rect.Width() / 2.0;
		double cy = rect.Height() / 2.0;

		// Matrix to transform from Device to Visual coordinates
		// x_vis = x_dev - cx
		// y_vis = cy - y_dev
		double matD2V[3][3] = {
			{1, 0, -cx},
			{0, -1, cy},
			{0, 0, 1}
		};

		// Matrix to transform from Visual to Device coordinates
		// x_dev = x_vis + cx
		// y_dev = cy - y_vis
		double matV2D[3][3] = {
			{1, 0, cx},
			{0, -1, cy},
			{0, 0, 1}
		};

		double temp[3][3];
		double finalMatrix[3][3];
		
		// final = V2D * Sym * D2V
		CMatrixTool::Multiply(matSym, matD2V, temp);
		CMatrixTool::Multiply(matV2D, temp, finalMatrix);
		
		pNewShape->Transform(finalMatrix);
		
		pNewShape->m_isTransformed = true;
		pNewShape->m_transformLabel = label;
		pNewShape->m_color = RGB(128, 0, 128);  // Purple for symmetric shape
		
		pDoc->AddShape(pNewShape);

		// 绘制参考直线 (Requirement 2.5)
		if (dlg.m_symmetryType == 3) // Line symmetry
		{
			// Transform line equation from Visual to Device coordinates
			// Visual: Ax + By + C = 0
			// Device: A(x-cx) + B(cy-y) + C = 0
			//         Ax - By + (-A*cx + B*cy + C) = 0
			double a = dlg.m_lineA;
			double b = -dlg.m_lineB;
			double c = -dlg.m_lineA * cx + dlg.m_lineB * cy + dlg.m_lineC;

			// Calculate intersection with screen boundaries to ensure visibility
			// Expand rect slightly to ensure line goes to edge
			int left = -10, top = -10;
			int right = rect.Width() + 10;
			int bottom = rect.Height() + 10;

			std::vector<CPoint> points;
			
			// Intersection with x = left
			if (fabs(b) > 1e-6) {
				int y = (int)((-c - a * left) / b);
				if (y >= top && y <= bottom) points.push_back(CPoint(left, y));
			}
			// Intersection with x = right
			if (fabs(b) > 1e-6) {
				int y = (int)((-c - a * right) / b);
				if (y >= top && y <= bottom) points.push_back(CPoint(right, y));
			}
			// Intersection with y = top
			if (fabs(a) > 1e-6) {
				int x = (int)((-c - b * top) / a);
				if (x >= left && x <= right) points.push_back(CPoint(x, top));
			}
			// Intersection with y = bottom
			if (fabs(a) > 1e-6) {
				int x = (int)((-c - b * bottom) / a);
				if (x >= left && x <= right) points.push_back(CPoint(x, bottom));
			}

			// Remove duplicates and sort/select
			if (points.size() >= 2) {
				// Pick the two points that are furthest apart
				CPoint p1 = points[0];
				CPoint p2 = points[1];
				double maxDist = 0;
				
				for (size_t i = 0; i < points.size(); i++) {
					for (size_t j = i + 1; j < points.size(); j++) {
						double dist = pow(points[i].x - points[j].x, 2) + pow(points[i].y - points[j].y, 2);
						if (dist > maxDist) {
							maxDist = dist;
							p1 = points[i];
							p2 = points[j];
						}
					}
				}

				// Draw reference line
				CGraphLine* pRefLine = new CGraphLine(p1, p2, 3, RGB(255, 0, 0), 2); // Width 2
				pDoc->AddShape(pRefLine);

				// Draw equation label at the midpoint
				CString strEq;
				strEq.Format(_T("%.1fx + %.1fy + %.1f = 0"), a, b, c);
				
				CPoint midPoint((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
				// Offset label slightly
				midPoint.x += 10;
				midPoint.y -= 10;

				CGraphText* pLabel = new CGraphText(midPoint, strEq, _T("宋体"), 16, RGB(255, 0, 0));
				pDoc->AddShape(pLabel);
			}
			else {
				// Fallback for lines outside screen or invalid
				// Just draw a long line through center if possible, or log error
				// For now, let's try to draw a default diagonal if calculation failed but params are valid
				// This helps debugging if the line is just "somewhere else"
			}
		}

		SetOperationLabel(_T("操作: 对称变换")); 
		SetAlgorithmLabel(_T("算法: 矩阵变换"));
		Invalidate();
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			CString msg;
			msg.Format(_T("Applied symmetry: %s"), label);
			pFrame->SetMessageText(msg);
		}
	}
}

void CGraphicArtStudioView::OnTransformComposite()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (pDoc->m_shapeList.empty()) {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) pFrame->SetMessageText(_T("Please draw a shape first"));
		return;
	}
	
	// Composite transform: Allow user to build a sequence of transformations
	double compositeMatrix[3][3];
	CMatrixTool::Identity(compositeMatrix);
	CString compositeLabel = _T("Composite:");
	int stepCount = 0;

	while (true) {
		CMenu menu;
		menu.CreatePopupMenu();
		menu.AppendMenu(MF_STRING, 1, _T("Add Translate"));
		menu.AppendMenu(MF_STRING, 2, _T("Add Scale"));
		menu.AppendMenu(MF_STRING, 3, _T("Add Rotate"));
		menu.AppendMenu(MF_STRING, 4, _T("Add Shear"));
		menu.AppendMenu(MF_STRING, 5, _T("Add Symmetry"));
		menu.AppendMenu(MF_SEPARATOR);
		menu.AppendMenu(MF_STRING, 6, _T("Finish & Apply"));
		menu.AppendMenu(MF_STRING, 7, _T("Cancel"));

		CPoint pt;
		GetCursorPos(&pt);
		
		// TrackPopupMenu returns the command ID
		int cmd = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, pt.x, pt.y, this);

		if (cmd <= 0 || cmd == 7) { // Cancel or error
			if (stepCount > 0) {
				if (MessageBox(_T("Cancel current composite transformation?"), _T("Confirm"), MB_YESNO) == IDYES)
					return;
				else
					continue;
			}
			return;
		}

		if (cmd == 6) { // Finish
			if (stepCount == 0) {
				MessageBox(_T("No transformations added."));
				return;
			}
			break;
		}

		double currentMatrix[3][3];
		CMatrixTool::Identity(currentMatrix);
		CString currentLabel;
		bool ok = false;

		if (cmd == 1) { // Translate
			CTransformDlg dlg(TRANSFORM_TRANSLATE);
			dlg.m_dx = 50; dlg.m_dy = 50;
			if (dlg.DoModal() == IDOK) {
				CMatrixTool::Translate(dlg.m_dx, dlg.m_dy, currentMatrix);
				currentLabel.Format(_T(" T(%.0f,%.0f)"), dlg.m_dx, dlg.m_dy);
				ok = true;
			}
		}
		else if (cmd == 2) { // Scale
			CTransformDlg dlg(TRANSFORM_SCALE);
			dlg.m_sx = 1.5; dlg.m_sy = 1.5; dlg.m_scaleCenter = CPoint(200, 200);
			if (dlg.DoModal() == IDOK) {
				CMatrixTool::ScaleAboutPoint(dlg.m_sx, dlg.m_sy, dlg.m_scaleCenter, currentMatrix);
				currentLabel.Format(_T(" S(%.1f,%.1f)"), dlg.m_sx, dlg.m_sy);
				ok = true;
			}
		}
		else if (cmd == 3) { // Rotate
			CTransformDlg dlg(TRANSFORM_ROTATE);
			dlg.m_angle = 45; dlg.m_rotateCenter = CPoint(200, 200);
			if (dlg.DoModal() == IDOK) {
				CMatrixTool::RotateAboutPoint(dlg.m_angle, dlg.m_rotateCenter, currentMatrix);
				currentLabel.Format(_T(" R(%.0f)"), dlg.m_angle);
				ok = true;
			}
		}
		else if (cmd == 4) { // Shear
			CTransformDlg dlg(TRANSFORM_SHEAR);
			dlg.m_shx = 0.5; dlg.m_shy = 0; dlg.m_scaleCenter = CPoint(200, 200);
			if (dlg.DoModal() == IDOK) {
				CMatrixTool::ShearAboutPoint(dlg.m_shx, dlg.m_shy, dlg.m_scaleCenter, currentMatrix);
				currentLabel.Format(_T(" Sh(%.1f,%.1f)"), dlg.m_shx, dlg.m_shy);
				ok = true;
			}
		}
		else if (cmd == 5) { // Symmetry
			CTransformDlg dlg(TRANSFORM_SYMMETRY);
			dlg.m_symmetryType = 0;
			if (dlg.DoModal() == IDOK) {
				double matSym[3][3];
				switch (dlg.m_symmetryType) {
				case 0: CMatrixTool::SymmetryX(matSym); currentLabel = _T(" SymX"); break;
				case 1: CMatrixTool::SymmetryY(matSym); currentLabel = _T(" SymY"); break;
				case 2: CMatrixTool::SymmetryOrigin(matSym); currentLabel = _T(" SymO"); break;
				case 3: 
					CMatrixTool::SymmetryLine(dlg.m_lineA, dlg.m_lineB, dlg.m_lineC, matSym); 
					currentLabel = _T(" SymL"); 
					break;
				}

				// Handle coordinate system for symmetry
				CRect rect;
				GetClientRect(&rect);
				double cx = rect.Width() / 2.0;
				double cy = rect.Height() / 2.0;
				double matD2V[3][3] = { {1, 0, -cx}, {0, -1, cy}, {0, 0, 1} };
				double matV2D[3][3] = { {1, 0, cx}, {0, -1, cy}, {0, 0, 1} };
				double temp[3][3];
				CMatrixTool::Multiply(matSym, matD2V, temp);
				CMatrixTool::Multiply(matV2D, temp, currentMatrix);
				ok = true;
			}
		}

		if (ok) {
			// Accumulate: NewComposite = Current * OldComposite
			// This applies Current AFTER OldComposite (if we think of transforming points: P' = M2 * (M1 * P))
			// So the sequence is M1, then M2.
			double temp[3][3];
			// Copy compositeMatrix to temp
			for(int i=0; i<3; i++) for(int j=0; j<3; j++) temp[i][j] = compositeMatrix[i][j];
			
			CMatrixTool::Multiply(currentMatrix, temp, compositeMatrix);
			compositeLabel += currentLabel;
			stepCount++;
			
			// Show feedback
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if (pFrame) {
				CString msg;
				msg.Format(_T("Added step %d: %s"), stepCount, currentLabel);
				pFrame->SetMessageText(msg);
			}
		}
	}

	// Apply final composite transformation
	SaveUndoState();
	
	CGraphElement* pShape = pDoc->m_shapeList.back();
	CGraphElement* pNewShape = pShape->Clone();
	
	pNewShape->Transform(compositeMatrix);
	
	pNewShape->m_isTransformed = true;
	pNewShape->m_transformLabel = compositeLabel;
	pNewShape->m_color = RGB(255, 128, 0); // Orange for composite
	
	pDoc->AddShape(pNewShape);
	SetOperationLabel(_T("操作: 复合变换")); 
	SetAlgorithmLabel(_T("算法: 复合矩阵"));
	Invalidate();
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		pFrame->SetMessageText(_T("Applied composite transformation"));
	}
}

// Clip menu handlers
void CGraphicArtStudioView::OnClipLine()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (pDoc->m_shapeList.empty()) {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) pFrame->SetMessageText(_T("Please draw some lines first"));
		return;
	}
	
	// Enter clipping window selection mode
	ResetDrawingMode();
	m_drawMode = MODE_CLIP_WINDOW;
	SetOperationLabel(_T("操作: 直线裁剪"));
	SetAlgorithmLabel(_T("算法: 用户选择（Cohen/Liang/Sutherland 等）"));
	m_clipTarget = 1; // Line
	m_tempPoints.clear();
	SetCursor(LoadCursor(NULL, IDC_CROSS));
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) pFrame->SetMessageText(_T("Drag mouse to define clipping window"));
}

void CGraphicArtStudioView::OnClipPolygon()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (pDoc->m_shapeList.empty()) {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) pFrame->SetMessageText(_T("Please draw some polygons first"));
		return;
	}
	
	// Enter clipping window selection mode
	ResetDrawingMode();
	m_drawMode = MODE_CLIP_WINDOW;
	SetOperationLabel(_T("操作: 多边形裁剪"));
	SetAlgorithmLabel(_T("算法: 用户选择（Cohen/Liang/Sutherland 等）"));
	m_clipTarget = 2; // Polygon
	m_tempPoints.clear();
	SetCursor(LoadCursor(NULL, IDC_CROSS));
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) pFrame->SetMessageText(_T("Drag mouse to define clipping window"));
}

// Fill menu handlers
void CGraphicArtStudioView::OnFillScanline()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (pDoc->m_shapeList.empty()) {
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) pFrame->SetMessageText(_T("Please draw some polygons first"));
		return;
	}
	
	CColorPickerDlg dlg;
	if (dlg.DoModal() == IDOK) {
		SaveUndoState();
		
		COLORREF fillColor = dlg.m_selectedColor;
		int fillCount = 0;
		
		// Apply scanline fill to polygon objects (not just DC)
		for (auto shape : pDoc->m_shapeList) {
			CGraphPolygon* polygon = dynamic_cast<CGraphPolygon*>(shape);
			if (polygon && polygon->m_closed) {
				// Set polygon fill properties
				polygon->m_filled = true;
				polygon->m_fillColor = fillColor;
				polygon->SetFillAlgorithm(FILL_SCANLINE);
				fillCount++;
			}
			
			// Handle Chinese Character fill
			CChineseChar* pChar = dynamic_cast<CChineseChar*>(shape);
			if (pChar) {
				pChar->SetFilled(true);
				pChar->SetFillColor(fillColor);
				pChar->SetFillAlgorithm(FILL_SCANLINE);
				fillCount++;
			}
		}
		
		// Add algorithm label
		CGraphText* label = new CGraphText(CPoint(50, 50), _T("Scanline Fill Algorithm"), _T("Arial"), 12, RGB(0, 0, 0));
		label->m_transformLabel = _T("Algorithm Label");
		pDoc->AddShape(label);
		
		SetOperationLabel(_T("操作: 多边形填充"));
		SetAlgorithmLabel(_T("算法: 扫描线填充"));
		Invalidate();
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			CString msg;
			msg.Format(_T("Scanline fill completed: %d polygons filled"), fillCount);
			pFrame->SetMessageText(msg);
		}
	}
}

void CGraphicArtStudioView::OnFillSeed()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) return;

	// Check if there are any closed polygons
	bool hasPolygons = false;
	for (auto shape : pDoc->m_shapeList)
	{
		if (shape->m_type == SHAPE_POLYGON)
		{
			CGraphPolygon* polygon = dynamic_cast<CGraphPolygon*>(shape);
			if (polygon && polygon->m_closed)
			{
				hasPolygons = true;
				break;
			}
		}
		else if (shape->m_type == SHAPE_TEXT && dynamic_cast<CChineseChar*>(shape))
		{
			hasPolygons = true;
			break;
		}
	}

	if (!hasPolygons)
	{
		AfxMessageBox(_T("No closed polygons found. Please draw a polygon first."));
		return;
	}

	// Choose fill color
	CColorPickerDlg dlg;
	if (dlg.DoModal() != IDOK) return;
	
	// Store fill color and enter seed fill mode
	m_seedFillColor = dlg.m_selectedColor;
	m_drawMode = MODE_SEED_FILL;
	SetOperationLabel(_T("操作: 多边形填充"));
	SetAlgorithmLabel(_T("算法: 种子填充"));
	
	// Update status
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		pFrame->SetMessageText(_T("Seed Fill Mode: Click inside a polygon to fill it"));
	}
	
	// Change cursor to indicate seed fill mode
	SetCursor(LoadCursor(NULL, IDC_CROSS));
}

void CGraphicArtStudioView::OnFillCharacter()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) return;
	
	SaveUndoState();
	
	// Clear existing shapes for demo
	for (auto shape : pDoc->m_shapeList) {
		delete shape;
	}
	pDoc->m_shapeList.clear();
	
	// Add character with fill demonstration
	CNameCharacter::AddCharacterWithFill(pDoc, CPoint(400, 300), 80);
	
	// Get fill color
	CColorPickerDlg dlg;
	if (dlg.DoModal() == IDOK) {
		COLORREF fillColor = dlg.m_selectedColor;
		
		// Find the character polygon and apply fill
		for (auto shape : pDoc->m_shapeList) {
			CGraphPolygon* polygon = dynamic_cast<CGraphPolygon*>(shape);
			if (polygon && polygon->m_transformLabel.Find(_T("Character")) >= 0) {
				polygon->m_filled = true;
				polygon->m_fillColor = fillColor;
				break;
			}
		}
	}
	
	SetOperationLabel(_T("操作: 多边形填充"));
	SetAlgorithmLabel(_T("算法: 字符填充"));
	Invalidate();
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		pFrame->SetMessageText(_T("Character fill demonstration completed with algorithm explanation"));
	}
}

void CGraphicArtStudioView::OnFillSetColor()
{
	CColorPickerDlg dlg;
	dlg.m_selectedColor = m_fillColor; // 显示当前填充颜色
	
	if (dlg.DoModal() == IDOK) {
		m_fillColor = dlg.m_selectedColor;
		m_filled = true; // 启用填充
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			CString msg;
			msg.Format(_T("Fill color set to RGB(%d,%d,%d). New polygons will be filled with this color."), 
				GetRValue(m_fillColor), GetGValue(m_fillColor), GetBValue(m_fillColor));
			pFrame->SetMessageText(msg);
		}
	}
}

void CGraphicArtStudioView::OnFillToggle()
{
	m_filled = !m_filled;
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		if (m_filled) {
			CString msg;
			msg.Format(_T("Fill enabled. New polygons will be filled with RGB(%d,%d,%d)."), 
				GetRValue(m_fillColor), GetGValue(m_fillColor), GetBValue(m_fillColor));
			pFrame->SetMessageText(msg);
		} else {
			pFrame->SetMessageText(_T("Fill disabled. New polygons will not be filled."));
		}
	}
}

void CGraphicArtStudioView::OnUpdateFillToggle(CCmdUI* pCmdUI)
{
	// Show checkmark if fill is enabled
	pCmdUI->SetCheck(m_filled ? 1 : 0);
}

// View menu handlers
void CGraphicArtStudioView::OnViewCoordinate()
{
	m_showCoordinate = !m_showCoordinate;
	Invalidate();
}

void CGraphicArtStudioView::OnViewGrid()
{
	m_showGrid = !m_showGrid;
	Invalidate();
}

void CGraphicArtStudioView::OnViewProperty()
{
	AfxMessageBox(_T("Property Panel - To be implemented"));
}

void CGraphicArtStudioView::OnViewToolbar()
{
	AfxMessageBox(_T("Toolbar - To be implemented"));
}

void CGraphicArtStudioView::OnViewCanvasSize()
{
	CCanvasSizeDlg dlg;
	dlg.m_width = m_canvasWidth;
	dlg.m_height = m_canvasHeight;
	
	if (dlg.DoModal() == IDOK) {
		m_canvasWidth = dlg.m_width;
		m_canvasHeight = dlg.m_height;
		
		// Update view
		Invalidate();
		
		// Show confirmation in status bar
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			CString msg;
			msg.Format(_T("Canvas size changed to %d x %d pixels"), m_canvasWidth, m_canvasHeight);
			pFrame->SetMessageText(msg);
		}
	}
}

// Tool menu handlers
void CGraphicArtStudioView::OnToolMove()
{
	m_drawMode = MODE_SELECT;
	m_tempPoints.clear();
	
	// Show message in status bar
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) pFrame->SetMessageText(_T("Move Tool activated. Click and drag shapes to move them."));
}

void CGraphicArtStudioView::OnToolScale()
{
	m_drawMode = MODE_SELECT;
	m_tempPoints.clear();
	
	// Show message in status bar
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) pFrame->SetMessageText(_T("Scale Tool activated. Select a shape and drag the corner handles to scale."));
}

void CGraphicArtStudioView::OnToolRotate()
{
	m_drawMode = MODE_SELECT;
	m_tempPoints.clear();
	
	// Show message in status bar
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) pFrame->SetMessageText(_T("Rotate Tool activated. Select a shape and drag the rotation handle to rotate."));
}

// Color menu handlers
void CGraphicArtStudioView::OnColorLine()
{
	CColorDialog dlg(m_currentColor);
	if (dlg.DoModal() == IDOK) {
		m_currentColor = dlg.GetColor();
	}
}

void CGraphicArtStudioView::OnColorFill()
{
	CColorDialog dlg(m_fillColor);
	if (dlg.DoModal() == IDOK) {
		m_fillColor = dlg.GetColor();
		m_filled = true;
	}
}

void CGraphicArtStudioView::OnColorBackground()
{
	CBackgroundColorDlg dlg;
	dlg.m_selectedColor = m_bgColor;
	
	if (dlg.DoModal() == IDOK) {
		m_bgColor = dlg.m_selectedColor;
		
		// Force complete redraw
		Invalidate();
		UpdateWindow();
		
		// Show confirmation in status bar with color info
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			CString msg;
			msg.Format(_T("Background color changed to RGB(%d, %d, %d)"), 
				GetRValue(m_bgColor), GetGValue(m_bgColor), GetBValue(m_bgColor));
			pFrame->SetMessageText(msg);
		}
	}
}

// Line width handlers
void CGraphicArtStudioView::OnWidth1()
{
	m_currentWidth = 1;
}

void CGraphicArtStudioView::OnWidth2()
{
	m_currentWidth = 2;
}

void CGraphicArtStudioView::OnWidth3()
{
	m_currentWidth = 3;
}

void CGraphicArtStudioView::OnWidth5()
{
	m_currentWidth = 5;
}

// Line algorithm handlers
void CGraphicArtStudioView::OnAlgoDDA()
{
	m_lineAlgorithm = 0;
	SetAlgorithmLabel(_T("算法: DDA算法"));
}

void CGraphicArtStudioView::OnAlgoBresenham()
{
	m_lineAlgorithm = 2;
	SetAlgorithmLabel(_T("算法: Bresenham算法"));
}

void CGraphicArtStudioView::OnAlgoMidpoint()
{
	m_lineAlgorithm = 1;
	SetAlgorithmLabel(_T("算法: Midpoint算法"));
}

void CGraphicArtStudioView::OnAlgoSystem()
{
	m_lineAlgorithm = 3;
	SetAlgorithmLabel(_T("算法: 系统绘制"));
}

// Help menu handlers
void CGraphicArtStudioView::OnHelpUsage()
{
	CString msg = _T("Interactive Drawing System\n\n");
	msg += _T("Basic Shapes:\n");
	msg += _T("- Line: Click start and end points\n");
	msg += _T("- Circle: Click center and drag for radius\n");
	msg += _T("- Ellipse: Click and drag to define bounds\n");
	msg += _T("- Polygon: Click multiple points, double-click to finish\n");
	msg += _T("- Text: Click position to place text\n\n");
	msg += _T("Curves:\n");
	msg += _T("- Bezier: Click control points, double-click to finish\n");
	msg += _T("- B-Spline: Click control points, double-click to finish\n\n");
	msg += _T("Edit:\n");
	msg += _T("- Undo: Ctrl+Z\n");
	msg += _T("- Redo: Ctrl+Y\n");
	msg += _T("- Clear: Delete all shapes\n");
	AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
}

void CGraphicArtStudioView::OnHelpAbout()
{
	CString msg = _T("Interactive Drawing System v1.0\n\n");
	msg += _T("Computer Graphics Course Project\n");
	msg += _T("2025-2026 Academic Year\n\n");
	msg += _T("Features:\n");
	msg += _T("- Basic shape drawing\n");
	msg += _T("- Curve drawing (Bezier & B-Spline)\n");
	msg += _T("- Multiple drawing algorithms\n");
	msg += _T("- Interactive editing\n");
	AfxMessageBox(msg, MB_OK | MB_ICONINFORMATION);
}

// Comprehensive drawing handlers
void CGraphicArtStudioView::OnDrawSolarSystem()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) return;
	
	SaveUndoState();
	
	// Clear existing shapes
	for (auto shape : pDoc->m_shapeList) {
		delete shape;
	}
	pDoc->m_shapeList.clear();
	
	// Get canvas center
	CRect rect;
	GetClientRect(&rect);
	CPoint center(rect.Width() / 2, rect.Height() / 2);
	
	// Create solar system at canvas center
	// CComprehensiveDrawing::CreateSolarSystem(pDoc, center); // Old static call
	
	StartSolarSystemAnimation();
	
	// Invalidate(); // StartSolarSystemAnimation will trigger invalidation via timer
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) {
		pFrame->SetMessageText(_T("Solar System created - Animation Started"));
	}
}

void CGraphicArtStudioView::StartSolarSystemAnimation()
{
	StopAnimation(); // Stop any existing animation
	
	m_isAnimating = true;
	m_animationFrame = 0;
	
	// Set background to dark blue/black for space effect
	m_bgColor = RGB(5, 5, 20); 
	
	// Start timer (ID=1, 50ms interval = 20fps)
	SetTimer(1, 50, NULL);
}

void CGraphicArtStudioView::StopAnimation()
{
	if (m_isAnimating) {
		KillTimer(1);
		m_isAnimating = false;
	}
}

void CGraphicArtStudioView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1 && m_isAnimating) {
		CGraphicArtStudioDoc* pDoc = GetDocument();
		if (pDoc) {
			CRect rect;
			GetClientRect(&rect);
			CPoint center(rect.Width() / 2, rect.Height() / 2);
			
			// Update solar system with new frame
			CComprehensiveDrawing::CreateSolarSystem(pDoc, center, m_animationFrame);
			
			m_animationFrame++;
			Invalidate(); // Trigger redraw
		}
	}
	
	CView::OnTimer(nIDEvent);
}

void CGraphicArtStudioView::OnDrawChineseChar()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) return;

	// Clear existing shapes
	for (auto shape : pDoc->m_shapeList) {
		delete shape;
	}
	pDoc->m_shapeList.clear();

	// Draw "罗" character
	CRect rect;
	GetClientRect(&rect);
	CPoint center = rect.CenterPoint();
	
	CNameCharacter::AddLuoCharacter(pDoc, center, 300, RGB(0, 0, 0));
	
	Invalidate();
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame) pFrame->SetMessageText(_T("Drawn hollow character '罗' using Songti font."));
}


// Grid drawing helper
void CGraphicArtStudioView::DrawGrid(CDC* pDC)
{
	if (!m_showGrid) return;

	CRect rect;
	GetClientRect(&rect);

	// 使用与坐标系相同的单元尺度（可调整）
	const int scale = 50; // 每个单位的像素数量
	int centerX = rect.Width() / 2;
	int centerY = rect.Height() / 2;

	CPen gridPen(PS_DOT, 1, RGB(220, 220, 220));
	CPen* pOldPen = pDC->SelectObject(&gridPen);

	// 从中心向左绘制垂直线（包含中心线）
	for (int x = centerX; x >= 0; x -= scale) {
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, rect.Height());
	}
	// 从中心向右绘制垂直线（不重复中心）
	for (int x = centerX + scale; x < rect.Width(); x += scale) {
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, rect.Height());
	}

	// 从中心向上绘制水平线（包含中心线）
	for (int y = centerY; y >= 0; y -= scale) {
		pDC->MoveTo(0, y);
		pDC->LineTo(rect.Width(), y);
	}
	// 从中心向下绘制水平线（不重复中心）
	for (int y = centerY + scale; y < rect.Height(); y += scale) {
		pDC->MoveTo(0, y);
		pDC->LineTo(rect.Width(), y);
	}

	pDC->SelectObject(pOldPen);
}

// Helper function implementations
void CGraphicArtStudioView::SaveUndoState()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) {
		TRACE(_T("SaveUndoState: No document!\n"));
		return;
	}
	
	TRACE(_T("SaveUndoState: Current shape count: %d\n"), (int)pDoc->m_shapeList.size());
	
	// Create a copy of current shape list
	std::vector<CGraphElement*> shapeCopy;
	for (auto shape : pDoc->m_shapeList) {
		if (shape) {
			shapeCopy.push_back(shape->Clone());
		}
	}
	
	// Add to undo stack
	m_undoStack.push_back(shapeCopy);
	TRACE(_T("SaveUndoState: Undo stack size now: %d\n"), (int)m_undoStack.size());
	
	// Limit undo stack size
	if (m_undoStack.size() > MAX_UNDO_STEPS) {
		// Delete oldest state
		for (auto shape : m_undoStack.front()) {
			delete shape;
		}
		m_undoStack.erase(m_undoStack.begin());
	}
	
	// Clear redo stack when new action is performed
	for (auto& shapeList : m_redoStack) {
		for (auto shape : shapeList) {
			delete shape;
		}
	}
	m_redoStack.clear();
	
	// Debug info
	CString debugMsg;
	debugMsg.Format(_T("Saved undo state. Undo stack size: %d"), (int)m_undoStack.size());
	// Uncomment for debugging: AfxMessageBox(debugMsg);
}

void CGraphicArtStudioView::SetOperationLabel(LPCTSTR pszLabel)
{
	if (pszLabel && pszLabel[0] != '\0')
		m_operationLabel = pszLabel;
	else
		m_operationLabel.Empty();

	Invalidate(FALSE);
}

void CGraphicArtStudioView::SetAlgorithmLabel(LPCTSTR pszLabel)
{
	if (pszLabel && pszLabel[0] != '\0')
		m_algorithmLabel = pszLabel;
	else
		m_algorithmLabel.Empty();

	Invalidate(FALSE);
}

void CGraphicArtStudioView::ClearOperationLabels()
{
	m_operationLabel.Empty();
	m_algorithmLabel.Empty();
	Invalidate(FALSE);
}

void CGraphicArtStudioView::DrawOperationLabels(CDC* pDC)
{
	if (!pDC) return;
	if (m_operationLabel.IsEmpty() && m_algorithmLabel.IsEmpty()) return;

	// Create a readable font
	CFont font;
	font.CreatePointFont(110, _T("Segoe UI")); // ~11pt
	CFont* pOldFont = pDC->SelectObject(&font);

	// Measure text
	CSize szOp = pDC->GetTextExtent(m_operationLabel);
	CSize szAlg = pDC->GetTextExtent(m_algorithmLabel);
	int padding = 8;
	int boxW = max(szOp.cx, szAlg.cx) + padding * 2;
	int boxH = szOp.cy + szAlg.cy + padding * 3;

	// Position (left-top)
	int x = 10;
	int y = 10;

	// Background rectangle
	CBrush brush(RGB(255, 255, 224)); // light yellow
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	CPen pen(PS_SOLID, 1, RGB(160, 160, 160));
	CPen* pOldPen = pDC->SelectObject(&pen);

	CRect rcBox(x, y, x + boxW, y + boxH);
	pDC->RoundRect(&rcBox, CPoint(6, 6));

	// Draw text
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->TextOut(x + padding, y + padding, m_operationLabel);
	pDC->TextOut(x + padding, y + padding + szOp.cy + 4, m_algorithmLabel);

	// Restore GDI
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldFont);
}

void CGraphicArtStudioView::ClearSelection()
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) return;
	
	for (auto shape : pDoc->m_shapeList) {
		if (shape) {
			shape->m_selected = false;
		}
	}
	m_selectedShape = nullptr;
	m_selectedShapes.clear();
}

CGraphElement* CGraphicArtStudioView::FindShapeAtPoint(CPoint point)
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) return nullptr;
	
	// Search from back to front (top to bottom in z-order)
	for (int i = pDoc->m_shapeList.size() - 1; i >= 0; i--) {
		CGraphElement* shape = pDoc->m_shapeList[i];
		if (shape && shape->HitTest(point)) {
			return shape;
		}
	}
	
	return nullptr;
}

CGraphicArtStudioView::DragMode CGraphicArtStudioView::GetHandleAtPoint(CPoint point, CRect bbox)
{
	int handleSize = 6;
	int tolerance = 8;  // Increased tolerance for easier clicking
	
	// Check rotation handle (above top center)
	int centerX = (bbox.left + bbox.right) / 2;
	int rotateY = bbox.top - 30;
	if (abs(point.x - centerX) <= tolerance && abs(point.y - rotateY) <= tolerance) {
		return DRAG_ROTATE;
	}
	
	// Check corner handles for scaling
	if (abs(point.x - bbox.left) <= tolerance && abs(point.y - bbox.top) <= tolerance) {
		return DRAG_SCALE;  // Top-left
	}
	if (abs(point.x - bbox.right) <= tolerance && abs(point.y - bbox.top) <= tolerance) {
		return DRAG_SCALE;  // Top-right
	}
	if (abs(point.x - bbox.left) <= tolerance && abs(point.y - bbox.bottom) <= tolerance) {
		return DRAG_SCALE;  // Bottom-left
	}
	if (abs(point.x - bbox.right) <= tolerance && abs(point.y - bbox.bottom) <= tolerance) {
		return DRAG_SCALE;  // Bottom-right
	}
	
	// Check edge handles for scaling
	int centerY = (bbox.top + bbox.bottom) / 2;
	if (abs(point.x - centerX) <= tolerance && abs(point.y - bbox.top) <= tolerance) {
		return DRAG_SCALE;  // Top center
	}
	if (abs(point.x - centerX) <= tolerance && abs(point.y - bbox.bottom) <= tolerance) {
		return DRAG_SCALE;  // Bottom center
	}
	if (abs(point.x - bbox.left) <= tolerance && abs(point.y - centerY) <= tolerance) {
		return DRAG_SCALE;  // Left center
	}
	if (abs(point.x - bbox.right) <= tolerance && abs(point.y - centerY) <= tolerance) {
		return DRAG_SCALE;  // Right center
	}
	
	return DRAG_NONE;
}

void CGraphicArtStudioView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE && m_drawMode == MODE_SEED_FILL) {
		// Exit seed fill mode
		m_drawMode = MODE_SELECT;
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			pFrame->SetMessageText(_T("Exited seed fill mode"));
		}
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

bool CGraphicArtStudioView::IsPointInsidePolygon(CPoint point, CGraphPolygon* polygon)
{
	if (!polygon || !polygon->m_closed || polygon->m_points.size() < 3) {
		return false;
	}
	
	// Ray casting algorithm
	int x = point.x;
	int y = point.y;
	bool inside = false;
	
	std::vector<CPoint>& points = polygon->m_points;
	int n = points.size();
	
	for (int i = 0, j = n - 1; i < n; j = i++) {
		int xi = points[i].x, yi = points[i].y;
		int xj = points[j].x, yj = points[j].y;
		
		if (((yi > y) != (yj > y)) && 
			(x < (xj - xi) * (y - yi) / (yj - yi) + xi)) {
			inside = !inside;
		}
	}
	
	return inside;
}

double CGraphicArtStudioView::CalculatePolygonArea(CGraphPolygon* polygon)
{
	if (!polygon || !polygon->m_closed || polygon->m_points.size() < 3) {
		return 0.0;
	}
	
	// Shoelace formula for polygon area
	std::vector<CPoint>& points = polygon->m_points;
	int n = points.size();
	double area = 0.0;
	
	for (int i = 0; i < n; i++) {
		int j = (i + 1) % n;
		area += (double)points[i].x * points[j].y;
		area -= (double)points[j].x * points[i].y;
	}
	
	return abs(area) / 2.0;
}

bool CGraphicArtStudioView::PerformFloodFill(CPoint seedPoint, COLORREF fillColor)
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) return false;
	
	// Find which polygons contain the seed point
	CGraphPolygon* outerPolygon = nullptr;
	CGraphPolygon* innerPolygon = nullptr;
	double maxArea = 0;
	double minArea = DBL_MAX;
	
	for (auto shape : pDoc->m_shapeList) {
		if (shape->m_type == SHAPE_POLYGON) {
			CGraphPolygon* polygon = dynamic_cast<CGraphPolygon*>(shape);
			if (polygon && polygon->m_closed && IsPointInsidePolygon(seedPoint, polygon)) {
				double area = CalculatePolygonArea(polygon);
				
				if (area < minArea) {
					minArea = area;
					innerPolygon = polygon;
				}
			}
		}
		else if (shape->m_type == SHAPE_TEXT) {
			CChineseChar* pChar = dynamic_cast<CChineseChar*>(shape);
			if (pChar) {
				for (auto subShape : pChar->GetSubShapes()) {
					CGraphPolygon* polygon = dynamic_cast<CGraphPolygon*>(subShape);
					if (polygon && polygon->m_closed && IsPointInsidePolygon(seedPoint, polygon)) {
						double area = CalculatePolygonArea(polygon);
						if (area > maxArea) {
							maxArea = area;
							outerPolygon = polygon;
						}
						if (area < minArea) {
							minArea = area;
							innerPolygon = polygon;
						}
					}
				}
			}
		}
	}
	
	// If we're in the ring area between outer and inner polygons
	if (outerPolygon && innerPolygon && outerPolygon != innerPolygon) {
		// Fill the outer polygon but exclude the inner polygon area
		outerPolygon->m_filled = true;
		outerPolygon->m_fillColor = fillColor;
		outerPolygon->SetFillAlgorithm(FILL_SCANLINE_SEED);
		
		// Keep inner polygon unfilled to create hollow effect
		innerPolygon->m_filled = false;
		
		return true;
	}
	// If we're only inside one polygon, fill it
	else if (outerPolygon) {
		outerPolygon->m_filled = true;
		outerPolygon->m_fillColor = fillColor;
		outerPolygon->SetFillAlgorithm(FILL_SCANLINE_SEED);
		
		return true;
	}
	
	// If not inside any polygon, show error
	AfxMessageBox(_T("Please click inside a polygon or between two polygons to fill the area."));
	return false;
}

bool CGraphicArtStudioView::FindFillBoundary(CPoint seedPoint, CRect& boundary)
{
	// Simple implementation: expand from seed point until we hit polygon boundaries
	int left = seedPoint.x, right = seedPoint.x;
	int top = seedPoint.y, bottom = seedPoint.y;
	int maxExpansion = 200; // Maximum expansion distance
	
	CGraphicArtStudioDoc* pDoc = GetDocument();
	if (!pDoc) return false;
	
	// Expand in all directions until we hit a polygon boundary
	for (int expansion = 1; expansion < maxExpansion; expansion++) {
		bool hitBoundary = false;
		
		// Check if we hit any polygon boundary
		for (auto shape : pDoc->m_shapeList) {
			if (shape->m_type == SHAPE_POLYGON) {
				CGraphPolygon* polygon = dynamic_cast<CGraphPolygon*>(shape);
				if (polygon && polygon->m_closed) {
					// Check if expansion rectangle intersects with polygon
					CRect testRect(seedPoint.x - expansion, seedPoint.y - expansion,
								  seedPoint.x + expansion, seedPoint.y + expansion);
					
					if (DoesRectIntersectPolygon(testRect, polygon)) {
						hitBoundary = true;
						break;
					}
				}
			}
		}
		
		if (hitBoundary) {
			// Use the previous expansion as boundary
			boundary = CRect(seedPoint.x - (expansion - 1), seedPoint.y - (expansion - 1),
							seedPoint.x + (expansion - 1), seedPoint.y + (expansion - 1));
			return true;
		}
	}
	
	// Default boundary if no polygon found
	boundary = CRect(seedPoint.x - 50, seedPoint.y - 50,
					seedPoint.x + 50, seedPoint.y + 50);
	return true;
}

bool CGraphicArtStudioView::DoesRectIntersectPolygon(CRect rect, CGraphPolygon* polygon)
{
	if (!polygon || !polygon->m_closed || polygon->m_points.size() < 3) {
		return false;
	}
	
	// Check if any polygon vertex is inside the rectangle
	for (const auto& point : polygon->m_points) {
		if (rect.PtInRect(point)) {
			return true;
		}
	}
	
	// Check if any rectangle corner is inside the polygon
	CPoint corners[4] = {
		CPoint(rect.left, rect.top),
		CPoint(rect.right, rect.top),
		CPoint(rect.right, rect.bottom),
		CPoint(rect.left, rect.bottom)
	};
	
	for (int i = 0; i < 4; i++) {
		if (IsPointInsidePolygon(corners[i], polygon)) {
			return true;
		}
	}
	
	return false;
}

void CGraphicArtStudioView::ResetDrawingMode()
{
	StopAnimation(); // Stop animation when switching modes

	// Force reset any special modes
	if (m_drawMode == MODE_SEED_FILL) {
		m_drawMode = MODE_SELECT;
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (pFrame) {
			pFrame->SetMessageText(_T("Reset to selection mode"));
		}
	}
	ClearOperationLabels();
}

// Update UI handlers for Edit menu
void CGraphicArtStudioView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_undoStack.empty());
}

void CGraphicArtStudioView::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!m_redoStack.empty());
}

void CGraphicArtStudioView::OnUpdateEditClear(CCmdUI* pCmdUI)
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc && !pDoc->m_shapeList.empty());
}

void CGraphicArtStudioView::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	CGraphicArtStudioDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc && !pDoc->m_shapeList.empty());
}

