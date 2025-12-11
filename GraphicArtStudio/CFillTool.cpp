#include "pch.h"
#include "CFillTool.h"
#include <algorithm>

namespace FillAlgorithmText {
	const wchar_t SCANLINE_FILL[] = L"Scanline Fill Algorithm";
	const wchar_t SEED_FILL[] = L"Seed Fill Algorithm";
	const wchar_t SCANLINE_SEED_FILL[] = L"Scanline Seed Fill Algorithm";
}

CFillTool::CFillTool()
{
}

CFillTool::~CFillTool()
{
}

void CFillTool::ScanlineFill(CDC* pDC, const std::vector<CPoint>& points,
								   COLORREF fillColor, COLORREF boundaryColor)
{
	if (points.size() < 3) return;

	int yMin, yMax;
	std::vector<std::vector<Edge>> edgeTable;
	BuildEdgeTable(points, edgeTable, yMin, yMax);

	std::vector<Edge> activeEdgeTable;

	for (int y = yMin; y <= yMax; y++)
	{
		int index = y - yMin;
		if (index >= 0 && index < (int)edgeTable.size())
		{
			for (const auto& edge : edgeTable[index])
			{
				InsertEdge(activeEdgeTable, edge);
			}
		}

		activeEdgeTable.erase(
			std::remove_if(activeEdgeTable.begin(), activeEdgeTable.end(),
				[y](const Edge& e) { return e.yMax == y; }),
			activeEdgeTable.end()
		);

		std::sort(activeEdgeTable.begin(), activeEdgeTable.end(),
			[](const Edge& a, const Edge& b) { return a.x < b.x; });

		for (size_t i = 0; i + 1 < activeEdgeTable.size(); i += 2)
		{
			int xStart = (int)(activeEdgeTable[i].x + 0.5);
			int xEnd = (int)(activeEdgeTable[i + 1].x + 0.5);

			for (int x = xStart; x <= xEnd; x++)
			{
				pDC->SetPixel(x, y, fillColor);
			}
		}

		for (auto& edge : activeEdgeTable)
		{
			edge.x += edge.dx;
		}
	}
}

void CFillTool::BuildEdgeTable(const std::vector<CPoint>& points,
									std::vector<std::vector<Edge>>& edgeTable,
									int& yMin, int& yMax)
{
	yMin = INT_MAX;
	yMax = INT_MIN;

	for (const auto& pt : points)
	{
		if (pt.y < yMin) yMin = pt.y;
		if (pt.y > yMax) yMax = pt.y;
	}

	edgeTable.resize(yMax - yMin + 1);

	size_t n = points.size();
	for (size_t i = 0; i < n; i++)
	{
		CPoint p1 = points[i];
		CPoint p2 = points[(i + 1) % n];

		if (p1.y == p2.y) continue;

		if (p1.y > p2.y)
			std::swap(p1, p2);

		double dx = (double)(p2.x - p1.x) / (p2.y - p1.y);

		Edge edge(p2.y, p1.x, dx);
		int index = p1.y - yMin;
		if (index >= 0 && index < (int)edgeTable.size())
		{
			edgeTable[index].push_back(edge);
		}
	}
}

void CFillTool::InsertEdge(std::vector<Edge>& list, const Edge& edge)
{
	list.push_back(edge);
}

void CFillTool::SeedFill(CDC* pDC, CPoint seed, COLORREF fillColor,
							   COLORREF boundaryColor, CRect clipRect)
{
	COLORREF oldColor = pDC->GetPixel(seed.x, seed.y);
	
	if (oldColor == fillColor || oldColor == boundaryColor)
		return;

	std::stack<CPoint> pointStack;
	pointStack.push(seed);

	while (!pointStack.empty())
	{
		CPoint pt = pointStack.top();
		pointStack.pop();

		if (!IsValidPixel(pt.x, pt.y, clipRect))
			continue;

		COLORREF currentColor = pDC->GetPixel(pt.x, pt.y);
		
		if (currentColor == boundaryColor || currentColor == fillColor)
			continue;

		if (currentColor != oldColor)
			continue;

		pDC->SetPixel(pt.x, pt.y, fillColor);

		pointStack.push(CPoint(pt.x + 1, pt.y));
		pointStack.push(CPoint(pt.x - 1, pt.y));
		pointStack.push(CPoint(pt.x, pt.y + 1));
		pointStack.push(CPoint(pt.x, pt.y - 1));
	}
}

void CFillTool::ScanlineSeedFill(CDC* pDC, CPoint seed, COLORREF fillColor,
									   COLORREF boundaryColor, CRect clipRect)
{
	COLORREF oldColor = pDC->GetPixel(seed.x, seed.y);
	
	if (oldColor == fillColor || oldColor == boundaryColor)
		return;

	std::stack<CPoint> seedStack;
	seedStack.push(seed);

	while (!seedStack.empty())
	{
		CPoint pt = seedStack.top();
		seedStack.pop();

		if (!IsValidPixel(pt.x, pt.y, clipRect))
			continue;

		COLORREF currentColor = pDC->GetPixel(pt.x, pt.y);
		if (currentColor == boundaryColor || currentColor == fillColor || currentColor != oldColor)
			continue;

		int xLeft = pt.x;
		while (xLeft >= clipRect.left)
		{
			COLORREF c = pDC->GetPixel(xLeft, pt.y);
			if (c == boundaryColor || c == fillColor || c != oldColor)
				break;
			pDC->SetPixel(xLeft, pt.y, fillColor);
			xLeft--;
		}
		xLeft++;

		int xRight = pt.x + 1;
		while (xRight <= clipRect.right)
		{
			COLORREF c = pDC->GetPixel(xRight, pt.y);
			if (c == boundaryColor || c == fillColor || c != oldColor)
				break;
			pDC->SetPixel(xRight, pt.y, fillColor);
			xRight++;
		}
		xRight--;

		for (int dy = -1; dy <= 1; dy += 2)
		{
			int newY = pt.y + dy;
			if (newY < clipRect.top || newY > clipRect.bottom)
				continue;

			bool inSpan = false;
			for (int x = xLeft; x <= xRight; x++)
			{
				COLORREF c = pDC->GetPixel(x, newY);
				bool needFill = (c != boundaryColor && c != fillColor && c == oldColor);

				if (!inSpan && needFill)
				{
					seedStack.push(CPoint(x, newY));
					inSpan = true;
				}
				else if (inSpan && !needFill)
				{
					inSpan = false;
				}
			}
		}
	}
}

bool CFillTool::IsValidPixel(int x, int y, const CRect& rect)
{
	return x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom;
}
