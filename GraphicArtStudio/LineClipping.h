#pragma once
#include "CGraphElement.h"

// Forward declarations
class CGraphLine;

// Cohen-Sutherland region codes
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

class CGraphLineClipping
{
public:
    // Cohen-Sutherland line clipping algorithm
    static bool CohenSutherlandClip(CPoint& p1, CPoint& p2, CRect clipWindow);
    
    // Compute region code for a point
    static int ComputeRegionCode(CPoint point, CRect clipWindow);
    
    // Clip a line shape and return the clipped result
    static CGraphLine* ClipLine(CGraphLine* line, CRect clipWindow, int algorithm = 0); // 0: Cohen-Sutherland, 1: Midpoint, 2: Liang-Barsky

    // Midpoint Subdivision algorithm
    static bool MidpointSubdivisionClip(CPoint& p1, CPoint& p2, CRect clipWindow);

    // Liang-Barsky algorithm
    static bool LiangBarskyClip(CPoint& p1, CPoint& p2, CRect clipWindow);
    
private:
    // Helper function to find intersection point
    static CPoint FindIntersection(CPoint p1, CPoint p2, int regionCode, CRect clipWindow);
    
    // Helper for Liang-Barsky
    static bool ClipTest(double p, double q, double& u1, double& u2);
};
