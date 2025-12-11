#include "pch.h"
#include "NameCharacter.h"
#include "GraphicArtStudioDoc.h"
#include "CGraphText.h"
#include "CChineseChar.h"
#include <vector>

CGraphPolygon* CNameCharacter::CreateCharacter(CPoint center, int size, COLORREF color)
{
    // Create a simple "王" character as default
    return CreateWangCharacter(center, size, color);
}

CGraphPolygon* CNameCharacter::CreateWangCharacter(CPoint center, int size, COLORREF color)
{
    // Create "王" character using polygon approximation
    CGraphPolygon* character = new CGraphPolygon(color, 3, false, RGB(255, 255, 255));
    
    int halfSize = size / 2;
    
    // Top horizontal line
    character->AddPoint(CPoint(center.x - halfSize, center.y - halfSize));
    character->AddPoint(CPoint(center.x + halfSize, center.y - halfSize));
    character->AddPoint(CPoint(center.x + halfSize, center.y - halfSize + 10));
    character->AddPoint(CPoint(center.x - halfSize, center.y - halfSize + 10));
    
    character->Close();
    character->m_transformLabel = _T("Character '王' for Fill Demo");
    
    return character;
}

CGraphPolygon* CNameCharacter::CreateLiCharacter(CPoint center, int size, COLORREF color)
{
    // Create "李" character using polygon approximation
    CGraphPolygon* character = new CGraphPolygon(color, 3, false, RGB(255, 255, 255));
    
    int halfSize = size / 2;
    
    // Simple rectangular approximation
    character->AddPoint(CPoint(center.x - halfSize, center.y - halfSize));
    character->AddPoint(CPoint(center.x + halfSize, center.y - halfSize));
    character->AddPoint(CPoint(center.x + halfSize, center.y + halfSize));
    character->AddPoint(CPoint(center.x - halfSize, center.y + halfSize));
    
    character->Close();
    character->m_transformLabel = _T("Character '李' for Fill Demo");
    
    return character;
}

CGraphPolygon* CNameCharacter::CreateZhangCharacter(CPoint center, int size, COLORREF color)
{
    // Create "张" character using polygon approximation
    CGraphPolygon* character = new CGraphPolygon(color, 3, false, RGB(255, 255, 255));
    
    int halfSize = size / 2;
    
    // L-shaped approximation
    character->AddPoint(CPoint(center.x - halfSize, center.y - halfSize));
    character->AddPoint(CPoint(center.x, center.y - halfSize));
    character->AddPoint(CPoint(center.x, center.y));
    character->AddPoint(CPoint(center.x + halfSize, center.y));
    character->AddPoint(CPoint(center.x + halfSize, center.y + halfSize));
    character->AddPoint(CPoint(center.x - halfSize, center.y + halfSize));
    
    character->Close();
    character->m_transformLabel = _T("Character '张' for Fill Demo");
    
    return character;
}

void CNameCharacter::AddLuoCharacter(CGraphicArtStudioDoc* pDoc, CPoint center, int size, COLORREF color)
{
    if (!pDoc) return;

    // Create a composite character object
    CChineseChar* pChar = new CChineseChar();
    pChar->m_transformLabel = _T("Character '罗' (Songti)");

    // Create a memory DC to generate the path
    CDC memDC;
    memDC.CreateCompatibleDC(NULL);

    // Create font (Songti - SimSun)
    CFont font;
    // Create a large font for better resolution, scaled to 'size'
    font.CreateFont(size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0,
        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体"));

    CFont* pOldFont = memDC.SelectObject(&font);
    memDC.SetBkMode(TRANSPARENT);

    // Generate path from text
    memDC.BeginPath();
    CString strChar = _T("罗");
    CSize textSize = memDC.GetTextExtent(strChar);
    // Center the text
    memDC.TextOut(center.x - textSize.cx / 2, center.y - textSize.cy / 2, strChar);
    memDC.EndPath();

    // Retrieve path data
    int nNumPts = memDC.GetPath(NULL, NULL, 0);
    if (nNumPts <= 0) {
        memDC.SelectObject(pOldFont);
        delete pChar;
        return;
    }

    std::vector<CPoint> points(nNumPts);
    std::vector<BYTE> types(nNumPts);
    memDC.GetPath(points.data(), types.data(), nNumPts);

    memDC.SelectObject(pOldFont);

    // Convert path to CGraphPolygon shapes
    CGraphPolygon* pCurrentPoly = nullptr;
    CPoint startPt(0, 0);
    CPoint currentPt(0, 0);

    for (int i = 0; i < nNumPts; i++)
    {
        CPoint pt = points[i];
        BYTE type = types[i];
        BYTE cmd = type & ~PT_CLOSEFIGURE;

        if (cmd == PT_MOVETO)
        {
            // Close previous polygon if exists
            if (pCurrentPoly) {
                pCurrentPoly->Close();
                pChar->AddShape(pCurrentPoly);
                pCurrentPoly = nullptr;
            }
            // Start new polygon
            pCurrentPoly = new CGraphPolygon(color, 2, false, RGB(255, 255, 255));
            pCurrentPoly->AddPoint(pt);
            startPt = pt;
            currentPt = pt;
        }
        else if (cmd == PT_LINETO)
        {
            if (pCurrentPoly) {
                pCurrentPoly->AddPoint(pt);
                currentPt = pt;
            }
        }
        else if (cmd == PT_BEZIERTO)
        {
            // Bezier comes in 3 points: control1, control2, end
            if (pCurrentPoly && i + 2 < nNumPts) {
                CPoint p1 = points[i];
                CPoint p2 = points[i + 1];
                CPoint p3 = points[i + 2];

                // Flatten Bezier curve (Cubic Bezier)
                // P0=currentPt, P1=p1, P2=p2, P3=p3
                int steps = 10; // Number of segments for smoothness
                for (int s = 1; s <= steps; s++) {
                    double t = (double)s / steps;
                    double u = 1.0 - t;
                    double tt = t * t;
                    double uu = u * u;
                    double uuu = uu * u;
                    double ttt = tt * t;

                    // B(t) = (1-t)^3 P0 + 3(1-t)^2 t P1 + 3(1-t)t^2 P2 + t^3 P3
                    double x = uuu * currentPt.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
                    double y = uuu * currentPt.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;
                    pCurrentPoly->AddPoint(CPoint((int)x, (int)y));
                }

                currentPt = p3;
                i += 2; // Advance to the end point of Bezier
                
                // Update type to the end point's type to check for CLOSEFIGURE
                type = types[i]; 
            }
        }

        // Check for close figure flag
        if (type & PT_CLOSEFIGURE)
        {
            if (pCurrentPoly) {
                pCurrentPoly->AddPoint(startPt); // Ensure closed visually
                pCurrentPoly->Close();
                pChar->AddShape(pCurrentPoly);
                pCurrentPoly = nullptr;
            }
        }
    }

    // Add any remaining polygon
    if (pCurrentPoly) {
        pCurrentPoly->Close();
        pChar->AddShape(pCurrentPoly);
    }

    // Add the composite character to the document
    pDoc->AddShape(pChar);
}

void CNameCharacter::AddCharacterWithFill(CGraphicArtStudioDoc* pDoc, CPoint center, int size)
{
    // Add title
    CGraphText* title = new CGraphText(CPoint(center.x - 80, center.y - size - 20), 
        _T("Name Character Fill Demonstration"), _T("Arial"), 14, RGB(0, 0, 0));
    title->m_transformLabel = _T("Fill Demo Title");
    pDoc->AddShape(title);
    
    // Add character outline
    CGraphPolygon* character = CreateWangCharacter(center, size, RGB(0, 0, 0));
    pDoc->AddShape(character);
    
    // Add algorithm explanation
    CGraphText* algorithm = new CGraphText(CPoint(center.x - 80, center.y + 80), 
        _T("Scanline Fill Algorithm:"), _T("Arial"), 12, RGB(0, 0, 0));
    algorithm->m_transformLabel = _T("Algorithm Label");
    pDoc->AddShape(algorithm);
    
    CGraphText* principle1 = new CGraphText(CPoint(center.x - 80, center.y + 100), 
        _T("1. Build Edge Table (ET) and Active Edge Table (AET)"), _T("Arial"), 10, RGB(0, 0, 0));
    principle1->m_transformLabel = _T("Algorithm Principle 1");
    pDoc->AddShape(principle1);
    
    CGraphText* principle2 = new CGraphText(CPoint(center.x - 80, center.y + 120), 
        _T("2. Process scanlines from bottom to top"), _T("Arial"), 10, RGB(0, 0, 0));
    principle2->m_transformLabel = _T("Algorithm Principle 2");
    pDoc->AddShape(principle2);
    
    CGraphText* principle3 = new CGraphText(CPoint(center.x - 80, center.y + 140), 
        _T("3. Calculate intersections and fill pixels"), _T("Arial"), 10, RGB(0, 0, 0));
    principle3->m_transformLabel = _T("Algorithm Principle 3");
    pDoc->AddShape(principle3);
}
