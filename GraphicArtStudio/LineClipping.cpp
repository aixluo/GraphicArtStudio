#include "pch.h"
#include "LineClipping.h"
#include "CGraphLine.h"
#include <algorithm>

int CGraphLineClipping::ComputeRegionCode(CPoint point, CRect clipWindow)
{
    int code = INSIDE;
    
    if (point.x < clipWindow.left)        // to the left of clip window
        code |= LEFT;
    else if (point.x > clipWindow.right)  // to the right of clip window
        code |= RIGHT;
        
    if (point.y < clipWindow.top)         // above the clip window
        code |= TOP;
    else if (point.y > clipWindow.bottom) // below the clip window
        code |= BOTTOM;
        
    return code;
}

CPoint CGraphLineClipping::FindIntersection(CPoint p1, CPoint p2, int regionCode, CRect clipWindow)
{
    CPoint intersection;
    
    if (regionCode & TOP) {
        // Point is above the clip window
        intersection.x = p1.x + (p2.x - p1.x) * (clipWindow.top - p1.y) / (p2.y - p1.y);
        intersection.y = clipWindow.top;
    }
    else if (regionCode & BOTTOM) {
        // Point is below the clip window
        intersection.x = p1.x + (p2.x - p1.x) * (clipWindow.bottom - p1.y) / (p2.y - p1.y);
        intersection.y = clipWindow.bottom;
    }
    else if (regionCode & RIGHT) {
        // Point is to the right of clip window
        intersection.y = p1.y + (p2.y - p1.y) * (clipWindow.right - p1.x) / (p2.x - p1.x);
        intersection.x = clipWindow.right;
    }
    else if (regionCode & LEFT) {
        // Point is to the left of clip window
        intersection.y = p1.y + (p2.y - p1.y) * (clipWindow.left - p1.x) / (p2.x - p1.x);
        intersection.x = clipWindow.left;
    }
    
    return intersection;
}

bool CGraphLineClipping::CohenSutherlandClip(CPoint& p1, CPoint& p2, CRect clipWindow)
{
    int code1 = ComputeRegionCode(p1, clipWindow);
    int code2 = ComputeRegionCode(p2, clipWindow);
    bool accept = false;
    
    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            // Both endpoints lie within rectangle
            accept = true;
            break;
        }
        else if (code1 & code2) {
            // Both endpoints are outside rectangle, in same region
            break;
        }
        else {
            // Some segment of line lies within the rectangle
            int codeOut;
            CPoint intersection;
            
            // At least one endpoint is outside the rectangle, pick it
            if (code1 != 0)
                codeOut = code1;
            else
                codeOut = code2;
                
            // Find intersection point
            intersection = FindIntersection(p1, p2, codeOut, clipWindow);
            
            // Replace point outside rectangle by intersection point
            if (codeOut == code1) {
                p1 = intersection;
                code1 = ComputeRegionCode(p1, clipWindow);
            }
            else {
                p2 = intersection;
                code2 = ComputeRegionCode(p2, clipWindow);
            }
        }
    }
    
    return accept;
}

bool CGraphLineClipping::MidpointSubdivisionClip(CPoint& p1, CPoint& p2, CRect clipWindow)
{
    int code1 = ComputeRegionCode(p1, clipWindow);
    int code2 = ComputeRegionCode(p2, clipWindow);

    // Trivial accept
    if ((code1 == 0) && (code2 == 0)) return true;
    // Trivial reject
    if (code1 & code2) return false;

    // If p1 is outside, find the intersection point closest to p1
    if (code1 != 0) {
        CPoint tempP1 = p1;
        CPoint tempP2 = p2;
        
        // Binary search for intersection
        while (abs(tempP1.x - tempP2.x) > 1 || abs(tempP1.y - tempP2.y) > 1) {
            CPoint mid((tempP1.x + tempP2.x) / 2, (tempP1.y + tempP2.y) / 2);
            int codeMid = ComputeRegionCode(mid, clipWindow);
            
            // If mid and p1 are on same side outside, move p1 to mid
            // Note: This logic is simplified. Correct midpoint subdivision checks if segment p1-mid is trivially rejected.
            // If p1 and mid are both outside and in same region (code1 & codeMid != 0), then intersection is in mid-p2.
            // But if they are just both outside (code1 != 0, codeMid != 0) but not same region, intersection might be in p1-mid.
            
            // Better approach:
            // We want to find the point where the line enters the window.
            // Since we know p1 is outside and p2 is inside (or we treat it as such for this step),
            // we move p1 towards p2 until it hits the boundary.
            
            if ((code1 & codeMid) != 0) {
                // p1 and mid are in same outside region, intersection is in mid-p2
                tempP1 = mid;
                code1 = codeMid; // Update code1
            } else {
                // Intersection is in p1-mid
                tempP2 = mid;
            }
        }
        p1 = tempP1;
    }

    // If p2 is outside, find the intersection point closest to p2
    code2 = ComputeRegionCode(p2, clipWindow);
    if (code2 != 0) {
        CPoint tempP1 = p1; // p1 is now inside or on boundary
        CPoint tempP2 = p2;
        
        while (abs(tempP1.x - tempP2.x) > 1 || abs(tempP1.y - tempP2.y) > 1) {
            CPoint mid((tempP1.x + tempP2.x) / 2, (tempP1.y + tempP2.y) / 2);
            int codeMid = ComputeRegionCode(mid, clipWindow);
            
            if ((code2 & codeMid) != 0) {
                // p2 and mid are in same outside region, intersection is in p1-mid
                tempP2 = mid;
                code2 = codeMid;
            } else {
                // Intersection is in mid-p2
                tempP1 = mid;
            }
        }
        p2 = tempP2;
    }

    // Final check
    return (ComputeRegionCode(p1, clipWindow) == 0 && ComputeRegionCode(p2, clipWindow) == 0);
}

bool CGraphLineClipping::ClipTest(double p, double q, double& u1, double& u2)
{
    double r;
    if (p < 0.0) {
        r = q / p;
        if (r > u2) return false;
        if (r > u1) u1 = r;
    }
    else if (p > 0.0) {
        r = q / p;
        if (r < u1) return false;
        if (r < u2) u2 = r;
    }
    else {
        if (q < 0.0) return false;
    }
    return true;
}

bool CGraphLineClipping::LiangBarskyClip(CPoint& p1, CPoint& p2, CRect clipWindow)
{
    double u1 = 0.0, u2 = 1.0;
    double dx = (double)(p2.x - p1.x);
    double dy = (double)(p2.y - p1.y);

    if (ClipTest(-dx, p1.x - clipWindow.left, u1, u2)) {
        if (ClipTest(dx, clipWindow.right - p1.x, u1, u2)) {
            if (ClipTest(-dy, p1.y - clipWindow.top, u1, u2)) {
                if (ClipTest(dy, clipWindow.bottom - p1.y, u1, u2)) {
                    if (u2 < 1.0) {
                        p2.x = (long)(p1.x + u2 * dx);
                        p2.y = (long)(p1.y + u2 * dy);
                    }
                    if (u1 > 0.0) {
                        p1.x = (long)(p1.x + u1 * dx);
                        p1.y = (long)(p1.y + u1 * dy);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

CGraphLine* CGraphLineClipping::ClipLine(CGraphLine* line, CRect clipWindow, int algorithm)
{
    if (!line) return nullptr;
    
    CPoint p1 = line->m_start;
    CPoint p2 = line->m_end;
    bool visible = false;
    CString algoName;

    switch (algorithm) {
    case 1: // Midpoint Subdivision
        visible = MidpointSubdivisionClip(p1, p2, clipWindow);
        algoName = _T("Clipped by Midpoint Subdivision");
        break;
    case 2: // Liang-Barsky
        visible = LiangBarskyClip(p1, p2, clipWindow);
        algoName = _T("Clipped by Liang-Barsky");
        break;
    case 0: // Cohen-Sutherland
    default:
        visible = CohenSutherlandClip(p1, p2, clipWindow);
        algoName = _T("Clipped by Cohen-Sutherland");
        break;
    }
    
    if (visible) {
        // Line is visible after clipping
        // Use safe default values in case line properties are invalid
        int algoType = 3; // Default to system drawing
        int penWidth = 3; // Default to 3 pixels for clipped lines
        
        // Safely access line properties
        try {
            algoType = line->m_algoType;
            penWidth = line->m_penWidth + 1;
        }
        catch (...) {
            // Use defaults if access fails
        }
        
        CGraphLine* clippedLine = new CGraphLine(p1, p2, algoType, RGB(255, 0, 0), penWidth);
        clippedLine->m_transformLabel = algoName;
        return clippedLine;
    }
    
    return nullptr; // Line is completely outside
}
