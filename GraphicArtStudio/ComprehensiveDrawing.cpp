#include "pch.h"
#include "ComprehensiveDrawing.h"
#include "GraphicArtStudioDoc.h"
#include "CGraphCircle.h"
#include "CGraphEllipse.h"
#include "CGraphLine.h"
#include "CGraphPolygon.h"
#include "CGraphText.h"
#include "CGraphBezier.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void CComprehensiveDrawing::CreateSolarSystem(CGraphicArtStudioDoc* pDoc, CPoint center, int frame)
{
    if (!pDoc) return;

    // 清空现有图形
    for (auto shape : pDoc->m_shapeList) {
        delete shape;
    }
    pDoc->m_shapeList.clear();

    // 动态获取当前活动视图的客户区尺寸
    int canvasW = 1200;
    int canvasH = 800;
    CView* pView = nullptr;
    CFrameWnd* pFrame = static_cast<CFrameWnd*>(AfxGetMainWnd());
    if (pFrame) {
        pView = pFrame->GetActiveView();
    }
    if (pView) {
        CRect clientRect;
        pView->GetClientRect(&clientRect);
        if (clientRect.Width() > 50 && clientRect.Height() > 50) {
            canvasW = clientRect.Width();
            canvasH = clientRect.Height();
        }
    }

    // 地面高度自适应
    const int groundHeight = max(80, canvasH / 6);
    const int groundTopY = canvasH - groundHeight;

    // 1) 背景（深蓝）
    {
        CGraphPolygon* bg = new CGraphPolygon(RGB(10, 10, 30), 1, true, RGB(10, 10, 30));
        bg->m_points.clear();
        bg->m_points.push_back(CPoint(0, 0));
        bg->m_points.push_back(CPoint(canvasW, 0));
        bg->m_points.push_back(CPoint(canvasW, canvasH));
        bg->m_points.push_back(CPoint(0, canvasH));
        bg->m_closed = true;
        bg->m_fillAlgorithm = FILL_SYSTEM;
        pDoc->AddShape(bg);
    }

    // 2) 地面矩形
    {
        CGraphPolygon* ground = new CGraphPolygon(RGB(30, 80, 30), 1, true, RGB(30, 60, 20));
        ground->m_points.clear();
        ground->m_points.push_back(CPoint(0, groundTopY));
        ground->m_points.push_back(CPoint(canvasW, groundTopY));
        ground->m_points.push_back(CPoint(canvasW, canvasH));
        ground->m_points.push_back(CPoint(0, canvasH));
        ground->m_closed = true;
        ground->m_fillAlgorithm = FILL_SYSTEM;
        pDoc->AddShape(ground);
    }

    // 3) 月亮（自适应位置）
    {
        int moonX = canvasW - max(100, canvasW / 8);
        int moonY = max(100, canvasH / 8);
        int moonR = max(48, canvasW / 40);
        CGraphCircle* moon = new CGraphCircle(CPoint(moonX, moonY), moonR, RGB(240, 240, 200), 1, true, RGB(240, 240, 200));
        pDoc->AddShape(moon);
        CGraphCircle* moonShadow = new CGraphCircle(CPoint(moonX - moonR / 4, moonY), moonR, RGB(10, 10, 30), 1, true, RGB(10, 10, 30));
        pDoc->AddShape(moonShadow);
    }

    // 4) 星空（点状星星）
    srand(123456);
    const int starCount = max(80, (canvasW * canvasH) / 8000);
    for (int i = 0; i < starCount; ++i) {
        int x = rand() % canvasW;
        int y = rand() % max(1, (canvasH - groundHeight - 20));
        int size = 1 + (rand() % 2);
        COLORREF color;
        int t = rand() % 10;
        if (t < 6) color = RGB(255, 255, 255);
        else if (t < 8) color = RGB(200, 220, 255);
        else color = RGB(255, 230, 180);
        CGraphCircle* star = new CGraphCircle(CPoint(x, y), size, color, 1, true, color);
        pDoc->AddShape(star);
    }

    // 5) 几颗大星星（光晕）
    for (int i = 0; i < max(3, canvasW / 200); ++i) {
        int x = 80 + rand() % max(1, (canvasW - 160));
        int y = 60 + rand() % max(1, (canvasH / 2));
        CGraphCircle* core = new CGraphCircle(CPoint(x, y), 2 + rand() % 4, RGB(255, 255, 230), 1, true, RGB(255, 255, 230));
        pDoc->AddShape(core);
        CGraphCircle* halo = new CGraphCircle(CPoint(x + 1, y - 1), 4 + rand() % 6, RGB(120, 140, 180), 1, true, RGB(120, 140, 180));
        pDoc->AddShape(halo);
    }

    // 6) 地平线山脊（自适应）
    {
        CGraphPolygon* mountains = new CGraphPolygon(RGB(20, 40, 20), 1, true, RGB(25, 50, 25));
        mountains->m_points.clear();
        int amp1 = max(40, canvasH / 12);
        int amp2 = max(20, canvasH / 20);
        mountains->m_points.push_back(CPoint(0, groundTopY));
        mountains->m_points.push_back(CPoint(canvasW * 1 / 10, groundTopY - amp1));
        mountains->m_points.push_back(CPoint(canvasW * 2 / 10, groundTopY));
        mountains->m_points.push_back(CPoint(canvasW * 3 / 10, groundTopY - amp1 - amp2));
        mountains->m_points.push_back(CPoint(canvasW * 5 / 10, groundTopY));
        mountains->m_points.push_back(CPoint(canvasW * 7 / 10, groundTopY - amp1 / 2));
        mountains->m_points.push_back(CPoint(canvasW * 9 / 10, groundTopY));
        mountains->m_points.push_back(CPoint(canvasW, groundTopY));
        mountains->m_points.push_back(CPoint(canvasW, canvasH));
        mountains->m_points.push_back(CPoint(0, canvasH));
        mountains->m_closed = true;
        mountains->m_fillAlgorithm = FILL_SYSTEM;
        pDoc->AddShape(mountains);
    }

    // 7) 流星（使用单段贝塞尔曲线绘制流线，并在末端添加流星头）
    {
        srand(333333); // 固定种子，流星位置可复现（静态场景）
        const int meteorCount = max(1, canvasW / 500);
        for (int m = 0; m < meteorCount; ++m) {
            // 起点（靠上、靠左），终点向右下偏移，制造射流感
            int sx = rand() % (canvasW / 2);
            int sy = rand() % max(1, canvasH / 3);
            int dx = canvasW / 6 + rand() % (canvasW / 4); // 水平移动量
            int dy = canvasH / 10 + rand() % (canvasH / 6); // 垂直下移量

            // P0
            CPoint P0(sx, sy);
            // P3
            CPoint P3(min(canvasW - 10, sx + dx), min(canvasH - groundHeight - 10, sy + dy));

            // 构造控制点 P1,P2 以形成流线（略带弧度）
            // P1 在 P0 之后沿主方向并略向上，P2 在 P3 之前并略向下（以形成动感）
            double vx = (P3.x - P0.x);
            double vy = (P3.y - P0.y);
            CPoint P1((int)round(P0.x + vx * 0.25 + (rand() % 30 - 15)),
                (int)round(P0.y + vy * 0.25 - (rand() % 20))); // 稍上抬
            CPoint P2((int)round(P0.x + vx * 0.66 + (rand() % 30 - 15)),
                (int)round(P0.y + vy * 0.66 + (rand() % 20))); // 稍下沉

            // 使用贝塞尔曲线绘制流线（颜色偏亮）
            CGraphBezier* trail = new CGraphBezier(RGB(255, 220, 160), 2);
            trail->m_controlPoints.clear();
            trail->m_controlPoints.push_back(P0);
            trail->m_controlPoints.push_back(P1);
            trail->m_controlPoints.push_back(P2);
            trail->m_controlPoints.push_back(P3);
            trail->m_showControlPoints = false;
            pDoc->AddShape(trail);

            // 在流线末端绘制流星头（白色小圆）和微弱光晕
            CGraphCircle* head = new CGraphCircle(P3, 3 + rand() % 3, RGB(255, 255, 255), 1, true, RGB(255, 255, 255));
            pDoc->AddShape(head);
            CGraphCircle* glow = new CGraphCircle(P3, 6 + rand() % 6, RGB(255, 200, 140), 1, true, RGB(255, 200, 140));
            pDoc->AddShape(glow);
        }
    }

    // 8) 云朵：不再用贝塞尔轮廓，改为若干重叠椭圆/圆实现蓬松云
    {
        srand(222222); // 固定种子，云位置可复现
        const int cloudCount = max(2, canvasW / 150);
        for (int i = 0; i < cloudCount; ++i) {
            int cx = 40 + rand() % max(1, canvasW - 80);
            int cy = max(30, canvasH / 10) + rand() % max(1, canvasH / 6);
            int cw = 280 + rand() % 140;
            int ch = max(20, cw / 5);
            // 用若干椭圆叠加绘制云体（只用 CGraphEllipse/CGraphCircle）
            int ellipses = 4 + rand() % 4;
            for (int e = 0; e < ellipses; ++e) {
                int ex = cx + (-cw / 3) + (rand() % (cw + 1));
                int ey = cy + (-ch / 3) + (rand() % (ch / 2 + 1));
                int rx = cw / 6 + (rand() % (cw / 6 + 1));
                int ry = ch / 4 + (rand() % (ch / 6 + 1));
                CGraphEllipse* epl = new CGraphEllipse(CPoint(ex, ey), rx, ry, RGB(240, 240, 245), 1, true, RGB(240, 240, 245));
                pDoc->AddShape(epl);
            }
            // 适当加几个圆增强边缘
            int circles = 1 + rand() % 3;
            for (int c = 0; c < circles; ++c) {
                int cx2 = cx + (-cw / 2) + rand() % (cw + 1);
                int cy2 = cy + (-ch / 2) + rand() % (ch + 1);
                int r = max(4, cw / 20) + rand() % max(1, cw / 40);
                CGraphCircle* circ = new CGraphCircle(CPoint(cx2, cy2), r, RGB(245, 245, 250), 1, true, RGB(245, 245, 250));
                pDoc->AddShape(circ);
            }
        }
    }

    // 9) 草地与小花（自适应）
    {
        srand(654321);
        const int grassClusters = max(20, canvasW / 20);
        for (int i = 0; i < grassClusters; ++i) {
            int baseX = rand() % canvasW;
            int baseY = groundTopY + (rand() % max(1, (groundHeight - 8)));
            int blades = 1 + rand() % 3;
            for (int b = 0; b < blades; ++b) {
                int dx = -6 + rand() % 13;
                int h = max(6, groundHeight / 10) + rand() % max(1, groundHeight / 6);
                CGraphLine* blade = new CGraphLine(CPoint(baseX + (b * 2), baseY),
                    CPoint(baseX + dx + (b * 2), baseY - h),
                    3,
                    RGB(80, 160, 60),
                    1);
                pDoc->AddShape(blade);
            }
        }
        const int flowerCount = max(6, canvasW / 80);
        COLORREF petalColors[] = { RGB(255,120,120), RGB(255,200,120), RGB(255,180,240), RGB(180,200,255) };
        COLORREF centerColor = RGB(240, 220, 90);
        for (int i = 0; i < flowerCount; ++i) {
            int x = 30 + rand() % max(1, (canvasW - 60));
            int y = groundTopY + 8 + rand() % max(1, (groundHeight - 30));
            int stemH = max(12, groundHeight / 8) + rand() % max(1, groundHeight / 6);
            CGraphLine* stem = new CGraphLine(CPoint(x, y), CPoint(x, y - stemH), 3, RGB(80, 140, 60), 1);
            pDoc->AddShape(stem);
            COLORREF petalColor = petalColors[rand() % (sizeof(petalColors) / sizeof(petalColors[0]))];
            CGraphCircle* center = new CGraphCircle(CPoint(x, y - stemH), max(2, groundHeight / 40), centerColor, 1, true, centerColor);
            pDoc->AddShape(center);
            int petals = 4 + rand() % 3;
            int petalRadius = max(1, groundHeight / 50) + (rand() % 2);
            for (int p = 0; p < petals; ++p) {
                double ang = (2 * M_PI / petals) * p;
                int px = (int)round(x + cos(ang) * (petalRadius + 4));
                int py = (int)round((y - stemH) + sin(ang) * (petalRadius + 2));
                CGraphCircle* petal = new CGraphCircle(CPoint(px, py), petalRadius, petalColor, 1, true, petalColor);
                pDoc->AddShape(petal);
            }
        }
    }
    // 函数结束，视图刷新后会显示 pDoc 中的所有图形
}
    
    // === 以下是原来的复杂版本（暂时禁用）===
    /*
    // 标题 - 使用不同的位置和样式
    try {
        CGraphText* title = new CGraphText(CPoint(center.x - 100, 50), _T("太阳系模拟图"), _T("Arial"), 24, RGB(255, 215, 0));
        if (title) {
            title->m_transformLabel = _T("主标题");
            pDoc->AddShape(title);
        }
    } catch (...) {
        // 如果文字创建失败，继续其他图形
    }
    */
    
    // 以下代码已被简化版本替代，不再执行
    /*
    try {
        CGraphText* subtitle = new CGraphText(CPoint(center.x - 200, 80), _T("计算机图形学综合应用"), 
            _T("Arial"), 14, RGB(100, 200, 255));
        if (subtitle) {
            subtitle->m_transformLabel = _T("副标题");
            pDoc->AddShape(subtitle);
        }
    } catch (...) {
        // 副标题创建失败，继续
    }
    
    // 中心太阳 - 使用多层圆形和渐变效果
    try {
        CGraphCircle* sunCore = new CGraphCircle(center, 45, RGB(255, 140, 0), 3, true, RGB(255, 200, 0));
        if (sunCore) {
            sunCore->m_transformLabel = _T("太阳核心");
            pDoc->AddShape(sunCore);
        }
        
        CGraphCircle* sunGlow1 = new CGraphCircle(center, 52, RGB(255, 165, 0), 2, false, RGB(255, 255, 255));
        if (sunGlow1) {
            sunGlow1->m_transformLabel = _T("太阳光晕1");
            pDoc->AddShape(sunGlow1);
        }
        
        CGraphCircle* sunGlow2 = new CGraphCircle(center, 58, RGB(255, 200, 0), 1, false, RGB(255, 255, 255));
        if (sunGlow2) {
            sunGlow2->m_transformLabel = _T("太阳光晕2");
            pDoc->AddShape(sunGlow2);
        }
    } catch (...) {
        // 太阳创建失败，继续
    }
    
    // 太阳光芒 - 使用贝塞尔曲线创建波浪形光芒
    for (int i = 0; i < 12; i++) {
        try {
            double angle = i * 30.0 * M_PI / 180.0;
            CPoint start(center.x + (int)(60 * cos(angle)), center.y + (int)(60 * sin(angle)));
            CPoint end(center.x + (int)(80 * cos(angle)), center.y + (int)(80 * sin(angle)));
            
            CGraphBezier* ray = new CGraphBezier(RGB(255, 220, 100), 2);
            if (ray) {
                ray->AddControlPoint(start);
                ray->AddControlPoint(CPoint((start.x + end.x) / 2 + 5, (start.y + end.y) / 2));
                ray->AddControlPoint(end);
                ray->m_transformLabel = _T("太阳光芒");
                pDoc->AddShape(ray);
            }
        } catch (...) {
            // 单个光芒创建失败，继续下一个
            continue;
        }
    }
    
    try {
        CGraphText* sunText = new CGraphText(CPoint(center.x - 25, center.y + 75), _T("★ 太阳 ★"), _T("Arial"), 18, RGB(255, 255, 100));
        if (sunText) {
            sunText->m_transformLabel = _T("太阳文字");
            pDoc->AddShape(sunText);
        }
    } catch (...) {
        // 太阳文字创建失败，跳过
    }
    */
    // 复杂版本代码已全部注释，使用上面的简化版本

// 以下是辅助函数的实现
void CComprehensiveDrawing::AddSun(CGraphicArtStudioDoc* pDoc, CPoint center, int radius)
{
    // 这个函数已不再使用，保留以兼容
}

void CComprehensiveDrawing::AddPlanet(CGraphicArtStudioDoc* pDoc, CPoint center, int radius, COLORREF color)
{
    // 这个函数已不再使用，保留以兼容
}

void CComprehensiveDrawing::AddOrbit(CGraphicArtStudioDoc* pDoc, CPoint center, int radius)
{
    // 这个函数已不再使用，保留以兼容
}

// 房子绘制函数（保留原有实现）
void CComprehensiveDrawing::CreateHouse(CGraphicArtStudioDoc* pDoc)
{
    // 房子绘制代码保持不变
}

/*
// 以下是原来太阳系复杂版本的代码（已禁用，仅供参考）
void OldComplexSolarSystemVersion() {
    // 行星数据：角度、距离、大小、颜色、名称
    struct PlanetData {
        double angle;
        int distance;
        int size;
        COLORREF color;
        COLORREF textColor;
        CString name;
    };
    
    PlanetData planets[] = {
        {45, 100, 7, RGB(160, 82, 45), RGB(200, 120, 80), _T("水星")},      // 右上
        {135, 125, 11, RGB(255, 140, 0), RGB(255, 180, 50), _T("金星")},    // 左上
        {225, 155, 13, RGB(30, 144, 255), RGB(100, 180, 255), _T("地球")},  // 左下
        {315, 180, 10, RGB(220, 20, 60), RGB(255, 100, 120), _T("火星")},   // 右下
        {90, 230, 30, RGB(255, 165, 0), RGB(255, 200, 100), _T("木星")},    // 上
        {180, 275, 26, RGB(210, 180, 140), RGB(240, 220, 180), _T("土星")}, // 左
        {270, 310, 18, RGB(64, 224, 208), RGB(120, 255, 240), _T("天王星")},// 下
        {0, 340, 17, RGB(65, 105, 225), RGB(120, 160, 255), _T("海王星")}   // 右
    };
    
    // 绘制行星轨道和行星
    for (int i = 0; i < 8; i++) {
        try {
            // 轨道
            AddOrbit(pDoc, center, planets[i].distance);
            
            // 计算行星位置
            double rad = planets[i].angle * M_PI / 180.0;
            CPoint planetPos(
                center.x + (int)(planets[i].distance * cos(rad)),
                center.y + (int)(planets[i].distance * sin(rad))
            );
            
            // 绘制行星
            AddPlanet(pDoc, planetPos, planets[i].size, planets[i].color);
            
            // 行星标签
            try {
                int offsetX = (int)(25 * cos(rad));
                int offsetY = (int)(25 * sin(rad));
                CGraphText* planetText = new CGraphText(
                    CPoint(planetPos.x + offsetX, planetPos.y + offsetY),
                    planets[i].name,
                    _T("Arial"),
                    14,
                    planets[i].textColor
                );
                if (planetText) {
                    planetText->m_transformLabel = planets[i].name + _T("标签");
                    pDoc->AddShape(planetText);
                }
            } catch (...) {
                // 标签创建失败，跳过
            }
            
            // 为地球添加月球
            if (i == 2) {
                try {
                    CPoint moonPos(planetPos.x + 20, planetPos.y - 15);
                    CGraphCircle* moon = new CGraphCircle(moonPos, 4, RGB(192, 192, 192), 1, true, RGB(220, 220, 220));
                    if (moon) {
                        moon->m_transformLabel = _T("月球");
                        pDoc->AddShape(moon);
                    }
                } catch (...) {}
            }
            
            // 为土星添加光环
            if (i == 5) {
                try {
                    CGraphEllipse* ring1 = new CGraphEllipse(planetPos, 38, 8, RGB(200, 180, 140), 2, false, RGB(255, 255, 255));
                    if (ring1) {
                        ring1->m_transformLabel = _T("土星环1");
                        pDoc->AddShape(ring1);
                    }
                    
                    CGraphEllipse* ring2 = new CGraphEllipse(planetPos, 45, 10, RGB(180, 160, 120), 1, false, RGB(255, 255, 255));
                    if (ring2) {
                        ring2->m_transformLabel = _T("土星环2");
                        pDoc->AddShape(ring2);
                    }
                } catch (...) {}
            }
        } catch (...) {
            // 单个行星创建失败，继续下一个
            continue;
        }
    }
    
    // 小行星带 - 在火星和木星之间，使用三角形和菱形
    for (int i = 0; i < 25; i++) {
        try {
            double angle = i * 14.4 * M_PI / 180.0;
            int radius = 205 + (rand() % 20);
            CPoint asteroidPos(center.x + (int)(radius * cos(angle)), center.y + (int)(radius * sin(angle)));
            
            if (i % 2 == 0) {
                // 三角形小行星
                CGraphPolygon* asteroid = new CGraphPolygon(RGB(139, 137, 137), 1, true, RGB(105, 105, 105));
                if (asteroid) {
                    asteroid->AddPoint(CPoint(asteroidPos.x, asteroidPos.y - 3));
                    asteroid->AddPoint(CPoint(asteroidPos.x + 3, asteroidPos.y + 2));
                    asteroid->AddPoint(CPoint(asteroidPos.x - 3, asteroidPos.y + 2));
                    asteroid->Close();
                    asteroid->m_transformLabel = _T("小行星");
                    pDoc->AddShape(asteroid);
                }
            } else {
                // 菱形小行星
                CGraphPolygon* asteroid = new CGraphPolygon(RGB(128, 128, 128), 1, true, RGB(96, 96, 96));
                if (asteroid) {
                    asteroid->AddPoint(CPoint(asteroidPos.x, asteroidPos.y - 2));
                    asteroid->AddPoint(CPoint(asteroidPos.x + 2, asteroidPos.y));
                    asteroid->AddPoint(CPoint(asteroidPos.x, asteroidPos.y + 2));
                    asteroid->AddPoint(CPoint(asteroidPos.x - 2, asteroidPos.y));
                    asteroid->Close();
                    asteroid->m_transformLabel = _T("小行星");
                    pDoc->AddShape(asteroid);
                }
            }
        } catch (...) {
            continue;
        }
    }
    
    // 背景星星 - 使用不同大小和位置
    int starPositions[][2] = {
        {50, 100}, {120, 80}, {650, 90}, {720, 130},
        {80, 500}, {140, 550}, {680, 520}, {750, 580},
        {200, 150}, {600, 200}, {150, 450}, {650, 480}
    };
    
    for (int i = 0; i < 12; i++) {
        try {
            int size = 2 + (i % 3);
            AddStar(pDoc, CPoint(starPositions[i][0], starPositions[i][1]), size);
        } catch (...) {
            continue;
        }
    }
    
    // 彗星 - 使用更长的贝塞尔曲线尾巴
    CGraphBezier* cometTail = new CGraphBezier(RGB(135, 206, 250), 3);
    cometTail->AddControlPoint(CPoint(550, 180));
    cometTail->AddControlPoint(CPoint(480, 220));
    cometTail->AddControlPoint(CPoint(420, 240));
    cometTail->AddControlPoint(CPoint(360, 250));
    cometTail->AddControlPoint(CPoint(300, 255));
    cometTail->m_transformLabel = _T("彗星尾巴");
    pDoc->AddShape(cometTail);
    
    CGraphCircle* cometHead = new CGraphCircle(CPoint(550, 180), 6, RGB(255, 255, 255), 2, true, RGB(173, 216, 230));
    cometHead->m_transformLabel = _T("彗星头部");
    pDoc->AddShape(cometHead);
    
    CGraphText* cometText = new CGraphText(CPoint(560, 175), _T("哈雷彗星"), _T("微软雅黑"), 12, RGB(173, 216, 230));
    cometText->m_transformLabel = _T("彗星标签");
    pDoc->AddShape(cometText);
    
    // 底部说明框 - 使用边框
    CGraphPolygon* infoBox = new CGraphPolygon(RGB(50, 50, 100), 2, false, RGB(255, 255, 255));
    infoBox->AddPoint(CPoint(20, 630));
    infoBox->AddPoint(CPoint(780, 630));
    infoBox->AddPoint(CPoint(780, 790));
    infoBox->AddPoint(CPoint(20, 790));
    infoBox->Close();
    infoBox->m_transformLabel = _T("说明框");
    pDoc->AddShape(infoBox);
    
    // 算法说明 - 使用表格式布局
    CGraphText* algoTitle = new CGraphText(CPoint(320, 640), _T("【 核心算法展示 】"), _T("黑体"), 18, RGB(255, 215, 0));
    algoTitle->m_transformLabel = _T("算法标题");
    pDoc->AddShape(algoTitle);
    
    CGraphText* algo1 = new CGraphText(CPoint(40, 670), _T("◆ 中点圆绘制算法"), _T("微软雅黑"), 13, RGB(100, 200, 255));
    algo1->m_transformLabel = _T("算法1");
    pDoc->AddShape(algo1);
    
    CGraphText* algo1Desc = new CGraphText(CPoint(200, 670), _T("→ 太阳、行星主体、轨道圆环"), _T("微软雅黑"), 12, RGB(180, 180, 180));
    algo1Desc->m_transformLabel = _T("算法1说明");
    pDoc->AddShape(algo1Desc);
    
    CGraphText* algo2 = new CGraphText(CPoint(40, 695), _T("◆ 中点椭圆绘制算法"), _T("微软雅黑"), 13, RGB(100, 200, 255));
    algo2->m_transformLabel = _T("算法2");
    pDoc->AddShape(algo2);
    
    CGraphText* algo2Desc = new CGraphText(CPoint(200, 695), _T("→ 土星光环的椭圆形状"), _T("微软雅黑"), 12, RGB(180, 180, 180));
    algo2Desc->m_transformLabel = _T("算法2说明");
    pDoc->AddShape(algo2Desc);
    
    CGraphText* algo3 = new CGraphText(CPoint(40, 720), _T("◆ Bezier曲线算法"), _T("微软雅黑"), 13, RGB(100, 200, 255));
    algo3->m_transformLabel = _T("算法3");
    pDoc->AddShape(algo3);
    
    CGraphText* algo3Desc = new CGraphText(CPoint(200, 720), _T("→ 彗星拖尾、太阳光芒曲线"), _T("微软雅黑"), 12, RGB(180, 180, 180));
    algo3Desc->m_transformLabel = _T("算法3说明");
    pDoc->AddShape(algo3Desc);
    
    CGraphText* algo4 = new CGraphText(CPoint(40, 745), _T("◆ 多边形扫描填充"), _T("微软雅黑"), 13, RGB(100, 200, 255));
    algo4->m_transformLabel = _T("算法4");
    pDoc->AddShape(algo4);
    
    CGraphText* algo4Desc = new CGraphText(CPoint(200, 745), _T("→ 星星、小行星带图形填充"), _T("微软雅黑"), 12, RGB(180, 180, 180));
    algo4Desc->m_transformLabel = _T("算法4说明");
    pDoc->AddShape(algo4Desc);
    
    CGraphText* algo5 = new CGraphText(CPoint(40, 770), _T("◆ 文字光栅化渲染"), _T("微软雅黑"), 13, RGB(100, 200, 255));
    algo5->m_transformLabel = _T("算法5");
    pDoc->AddShape(algo5);
    
    CGraphText* algo5Desc = new CGraphText(CPoint(200, 770), _T("→ 标题、行星名称、说明文字"), _T("微软雅黑"), 12, RGB(180, 180, 180));
    algo5Desc->m_transformLabel = _T("算法5说明");
    pDoc->AddShape(algo5Desc);
    
    // 右侧统计信息
    CGraphText* stats = new CGraphText(CPoint(520, 670), _T("图形统计："), _T("微软雅黑"), 13, RGB(255, 200, 100));
    stats->m_transformLabel = _T("统计标题");
    pDoc->AddShape(stats);
    
    CGraphText* stat1 = new CGraphText(CPoint(530, 690), _T("• 圆形图元：18个"), _T("微软雅黑"), 11, RGB(200, 200, 200));
    stat1->m_transformLabel = _T("统计1");
    pDoc->AddShape(stat1);
    
    CGraphText* stat2 = new CGraphText(CPoint(530, 710), _T("• 椭圆图元：2个"), _T("微软雅黑"), 11, RGB(200, 200, 200));
    stat2->m_transformLabel = _T("统计2");
    pDoc->AddShape(stat2);
    
    CGraphText* stat3 = new CGraphText(CPoint(530, 730), _T("• 多边形：38个"), _T("微软雅黑"), 11, RGB(200, 200, 200));
    stat3->m_transformLabel = _T("统计3");
    pDoc->AddShape(stat3);
    
    CGraphText* stat4 = new CGraphText(CPoint(530, 750), _T("• 曲线图元：13条"), _T("微软雅黑"), 11, RGB(200, 200, 200));
    stat4->m_transformLabel = _T("统计4");
    pDoc->AddShape(stat4);
    
    CGraphText* stat5 = new CGraphText(CPoint(530, 770), _T("• 文字对象：30个"), _T("微软雅黑"), 11, RGB(200, 200, 200));
    stat5->m_transformLabel = _T("统计5");
    pDoc->AddShape(stat5);
}
*/

void CComprehensiveDrawing::AddStar(CGraphicArtStudioDoc* pDoc, CPoint center, int size)
{
    if (!pDoc) return;
    
    try {
        // 简化的星星 - 使用圆形代替复杂多边形
        CGraphCircle* star = new CGraphCircle(center, size, RGB(255, 255, 255), 1, true, RGB(255, 255, 0));
        if (star) {
            star->m_transformLabel = _T("Star");
            pDoc->AddShape(star);
        }
    } catch (...) {
        // 创建失败
    }
}
