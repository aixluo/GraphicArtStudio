#pragma once
#include "CGraphElement.h"

// Forward declaration
class CGraphicArtStudioDoc;

class CComprehensiveDrawing
{
public:
    // Create a comprehensive drawing using multiple shapes and features
    static void CreateSolarSystem(CGraphicArtStudioDoc* pDoc, CPoint center = CPoint(400, 300), int frame = 0);
    static void CreateHouse(CGraphicArtStudioDoc* pDoc);
    
private:
    // Helper functions
    static void AddSun(CGraphicArtStudioDoc* pDoc, CPoint center, int radius);
    static void AddPlanet(CGraphicArtStudioDoc* pDoc, CPoint center, int radius, COLORREF color);
    static void AddOrbit(CGraphicArtStudioDoc* pDoc, CPoint center, int radius);
    static void AddStar(CGraphicArtStudioDoc* pDoc, CPoint center, int size);
};
