#pragma once
#include <afxwin.h>
#include <vector>
#include <stack>
#include <queue>

// 边表结构
struct Edge {
	int yMax;      // 边的最大y坐标
	double x;      // 当前扫描线与边的交点x坐标
	double dx;     // 斜率的倒数 (1/k)
	
	Edge(int ymax, double xi, double slope) 
		: yMax(ymax), x(xi), dx(slope) {}
};

// 填充算法类
class CFillTool
{
public:
	CFillTool();
	~CFillTool();

	// 扫描线填充算法（用于多边形）
	static void ScanlineFill(CDC* pDC, const std::vector<CPoint>& points, 
							 COLORREF fillColor, COLORREF boundaryColor);
	
	// 种子填充算法（泛洪填充）
	static void SeedFill(CDC* pDC, CPoint seed, COLORREF fillColor, 
						 COLORREF boundaryColor, CRect clipRect);
	
	// 扫描线种子填充（更高效的种子填充）
	static void ScanlineSeedFill(CDC* pDC, CPoint seed, COLORREF fillColor,
								  COLORREF boundaryColor, CRect clipRect);

private:
	// 辅助函数
	static void BuildEdgeTable(const std::vector<CPoint>& points,
							   std::vector<std::vector<Edge>>& edgeTable,
							   int& yMin, int& yMax);
	static void InsertEdge(std::vector<Edge>& list, const Edge& edge);
	static bool IsValidPixel(int x, int y, const CRect& rect);
};

// 填充算法说明文字
namespace FillAlgorithmText {
	extern const wchar_t SCANLINE_FILL[];
	extern const wchar_t SEED_FILL[];
	extern const wchar_t SCANLINE_SEED_FILL[];
}
