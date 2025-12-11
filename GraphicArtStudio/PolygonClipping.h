#pragma once
#include "CGraphElement.h"
#include "CGraphPolygon.h"
#include <vector>

class CGraphPolygonClipping
{
public:
    // Sutherland-Hodgeman polygon clipping algorithm
    static CGraphPolygon* SutherlandHodgemanClip(CGraphPolygon* polygon, CRect clipWindow);

    // Weiler-Atherton polygon clipping algorithm
    static CGraphPolygon* WeilerAthertonClip(CGraphPolygon* polygon, CRect clipWindow);
    
    // General Clip function
    static CGraphPolygon* ClipPolygon(CGraphPolygon* polygon, CRect clipWindow, int algorithm = 0); // 0: SH, 1: WA
    
    // Clip polygon against a single edge
    static std::vector<CPoint> ClipAgainstEdge(const std::vector<CPoint>& inputVertices, 
                                               CPoint edgeStart, CPoint edgeEnd);
    
    // Helper functions
    static bool IsInside(CPoint point, CPoint edgeStart, CPoint edgeEnd);
    static CPoint ComputeIntersection(CPoint p1, CPoint p2, CPoint edgeStart, CPoint edgeEnd);
    
private:
    // Edge types for clipping window
    enum EdgeType {
        LEFT_EDGE,
        RIGHT_EDGE,
        BOTTOM_EDGE,
        TOP_EDGE
    };
    
    // Get edge points for clipping window
    static void GetEdgePoints(CRect clipWindow, EdgeType edge, CPoint& start, CPoint& end);
};
