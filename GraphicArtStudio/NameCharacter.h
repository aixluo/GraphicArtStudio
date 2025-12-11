#pragma once
#include "CGraphElement.h"
#include "CGraphPolygon.h"

// Forward declaration
class CGraphicArtStudioDoc;

class CNameCharacter
{
public:
    // Create a character shape for filling demonstration
    static CGraphPolygon* CreateCharacter(CPoint center, int size, COLORREF color);
    
    // Create a simple "王" character as an example
    static CGraphPolygon* CreateWangCharacter(CPoint center, int size, COLORREF color);
    
    // Create a simple "李" character as an example  
    static CGraphPolygon* CreateLiCharacter(CPoint center, int size, COLORREF color);
    
    // Create a simple "张" character as an example
    static CGraphPolygon* CreateZhangCharacter(CPoint center, int size, COLORREF color);

    // Add "罗" character (Hollow, Multiple Strokes)
    static void AddLuoCharacter(CGraphicArtStudioDoc* pDoc, CPoint center, int size, COLORREF color);
    
    // Add character with fill demonstration
    static void AddCharacterWithFill(CGraphicArtStudioDoc* pDoc, CPoint center, int size);
};
