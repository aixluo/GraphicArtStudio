# GraphicArtStudio 用户手册

版本说明
- 仓库提交：Commit OID: f7801c7ea3ce...
- 生成日期（本说明基于仓库当前源码）

---

目录
1. 开发环境、第三方库与版本  
2. 快速构建与运行  
3. 界面总览（含截图位置）  
4. 功能详解与操作步骤（每项功能都包含操作步骤与截图位置）  
5. 常见问题与调试提示  
6. 附录：主要类与关键函数对应表

---

1. 开发环境、第三方库与版本
- IDE / 编译器
  - Visual Studio 2022（项目文件显示 VCProjectVersion = 17.0，VisualStudioVersion = 17.14）
  - PlatformToolset: v143（MSVC v143 工具集）
- 平台 / SDK / 字符集
  - Windows Target Platform Version: 10.0（项目属性）
  - CharacterSet: Unicode
- 框架 / 运行时
  - MFC（UseOfMfc = Dynamic，动态链接 MFC 库）
  - Windows SDK 10.x（通过 Visual Studio 安装）
- 第三方库
  - 源码中未使用额外第三方库（如 SDL/GLFW/FreeImage/ImGui/Boost 等）。程序仅依赖 Windows SDK、MFC 与 C++ STL。
- 运行环境要求
  - Windows（可运行 MFC 应用的版本，例如 Windows 10/11）
  - 安装有 Visual C++ 运行时 / MFC 运行时（通常通过 Visual Studio 安装或分发的运行时即可得到）

---

2. 快速构建与运行
1. 在有 Visual Studio 2022 的机器上打开 `GraphicArtStudio.sln`（位于仓库根目录）。
2. 选择解决方案平台（Win32 / x64 / ARM64）和配置（Debug / Release）。
3. Build -> Build Solution（或按 F7）。
4. 运行：Debug -> Start Debugging (F5) 或 Start Without Debugging。
5. 如缺少 MFC/Windows SDK，请在 Visual Studio 安装器中启用 “Desktop development with C++” 并勾选 MFC/Windows SDK 组件。

提示：如果需要打包运行时，请确保包含对应的 Visual C++ 可再发行组件以保证目标机器可以运行动态链接的 MFC 程序。

---

3. 界面总览（含截图位置）
程序为单文档 SDI 窗口，主要区域与控件如下（建议的截图位置与保存文件名）：

- 主窗口（Main Window）
  - 包含菜单、工具栏、绘图视图、状态栏。
  - 截图建议：整个主窗口（保存为 screenshots/main_window.png）。
  - 资源参考：工具栏位图 `GraphicArtStudio/res/Toolbar.bmp`、应用图标 `GraphicArtStudio/res/GraphicArtStudio.ico`（在资源脚本 `GraphicArtStudio/GraphicArtStudio_修改后.rc.txt` 中引用）。
- 工具栏（Toolbar）
  - 位于窗口顶部。显示新建、打开、保存、撤销等图标。
  - 截图建议：工具栏特写（保存为 screenshots/toolbar.png）。可使用资源位图 `res/Toolbar.bmp` 做对比。
- 绘图视图（Canvas）
  - 主体绘制区域，位于主窗口中心。
  - 截图建议：仅画布区域（保存为 screenshots/canvas_empty.png 或 screenshots/canvas_with_shapes.png）。
- 对话框截图位置（每个对话框打开时截取）：
  - 画布尺寸对话框（CanvasSizeDlg） — screenshots/dialog_canvas_size.png
  - 背景颜色对话框（BackgroundColorDlg） — screenshots/dialog_background_color.png
  - 颜色选择对话框（ColorPickerDlg） — screenshots/dialog_color_picker.png
  - 文字输入对话（TextInputDlg） — screenshots/dialog_text_input.png
  - 直线输入对话（LineInputDlg） — screenshots/dialog_line_input.png
  - 圆输入对话（CircleInputDlg） — screenshots/dialog_circle_input.png
  - 裁剪窗口对话（ClippingDlg） — screenshots/dialog_clipping.png
  - 变换对话（TransformDlg） — screenshots/dialog_transform.png
  - 汉字对话（ChineseCharDlg） — screenshots/dialog_chinese_char.png

截图注意事项
- 截图时请在对话框或视图处于活动状态并显示实际参数或图形，以便文档与教学使用。
- 推荐图片保存路径：仓库内 `screenshots/` 目录（未在仓库中，需你本地创建并放入截图文件）。

---

4. 功能详解与操作步骤（逐项）
下面按功能分组，逐项说明用途、源码中对应的类/函数（便于开发者定位）、以及用户操作步骤与截图位置。

注：函数/类名均取自源码，便于开发者追踪实现细节。

A. 基本文档与项目操作
- 功能：新建 / 打开 / 保存 文档（序列化所有图形）
- 主要类 / 函数：
  - CGraphicArtStudioApp::InitInstance（程序初始化）
  - CGraphicArtStudioDoc::Serialize（保存/加载实现）
  - CGraphicArtStudioDoc::AddShape（向文档加入图形）
- 用户操作：
  1. 菜单 File -> New：新建空白文档（触发 OnFileNew）。
  2. File -> Open：打开 . (MFC 序列化格式) 文件（触发 OnFileOpen）。
  3. File -> Save / Save As：保存当前文档（触发 OnFileSave / OnFileSaveAs）。
- 截图位置：
  - 保存/打开对话后的画面： screenshots/file_save_dialog.png
  - 文档保存前后画布对比： screenshots/canvas_before_save.png / screenshots/canvas_after_load.png

B. 绘图与交互总览（视图）
- 功能：切换绘图模式、通过鼠标在画布上绘制图形、动态橡皮筋显示、选择与编辑
- 主要类 / 函数：
  - CGraphicArtStudioView（视图类，处理鼠标/键盘与命令）
  - 交互方法：OnLButtonDown, OnMouseMove, OnLButtonUp（事件来源），OnDraw（渲染）
  - 模式枚举：DrawMode（MODE_SELECT、MODE_LINE、MODE_CIRCLE、MODE_POLYGON、MODE_TEXT、MODE_BEZIER、MODE_BSPLINE、MODE_SEED_FILL、MODE_CLIP_WINDOW）
- 用户操作（通用流程）：
  1. 从菜单或工具栏选择绘图工具（例如 Draw -> Line）。
  2. 在画布上单击设定起点（OnLButtonDown），移动鼠标预览（OnMouseMove，视图绘制橡皮筋），释放鼠标设定终点（OnLButtonUp）以创建图元并加入文档。
  3. 选择模式（Tool -> Select）可选中图形，显示选中边框与控制把手（由 CGraphElement::DrawSelection 绘制）。
- 截图位置：
  - 进入绘图模式并在绘制时的橡皮筋预览： screenshots/draw_line_rubberband.png
  - 选中图形并显示控制把手： screenshots/selection_handles.png

C. 直线绘制（Line）
- 功能：在画布上绘制直线；支持多种算法（DDA、MidPoint、Bresenham、系统绘制）
- 主要类 / 函数：
  - CGraphLine（类） — 文件 `CGraphLine.h` / `CGraphLine.cpp`
  - DrawDDA, DrawMidBresenham, DrawBresenham（具体算法实现）
  - 构造函数：CGraphLine(CPoint start, CPoint end, int algo, COLORREF color, int width)
  - 对话：CGraphLineInputDlg（LineInputDlg.h/.cpp）用于直接输入（IDC_* 控件）
  - 视图命令：CGraphicArtStudioView::OnDrawLine、OnDrawLineInput
- 用户操作（鼠标）：
  1. 菜单 Draw -> Line 或工具栏点击“线”图标。
  2. 在画布上左键单击开始，拖动到终点释放完成绘制。
- 用户操作（对话输入）：
  1. 菜单 Draw -> Line Input（或工具栏相应命令）。
  2. 填写起点（Start X/Y）与终点（End X/Y），点击 OK（CGraphLineInputDlg::OnOK 会校验坐标）。
- 截图位置：
  - 对话： screenshots/dialog_line_input.png
  - 不同算法绘制结果对比： screenshots/line_dda_vs_bresenham.png

D. 圆与椭圆（Circle / Ellipse）
- 功能：绘制圆与椭圆，支持中点画圆算法与 GDI 系统绘制；支持填充选项
- 主要类 / 函数：
  - CGraphCircle（CGraphCircle.h / 多个实现文件）
    - Draw(), DrawMidpointCircle(), DrawCirclePoints(), Transform(), HitTest(), Serialize()
    - 多个版本存在（CGraphCircle.cpp, CGraphCircle_Final.cpp, CGraphCircle_Fixed.cpp 等），主实现文件为 `CGraphCircle.cpp`
  - CGraphEllipse（CGraphEllipse.h/.cpp）
    - DrawMidpointEllipse(), DrawEllipsePoints()
  - 对话：CGraphCircleInputDlg（CircleInputDlg.h/.cpp）
  - 视图命令：CGraphicArtStudioView::OnDrawCircle, OnDrawCircleInput, OnDrawEllipse
- 用户操作：
  1. 菜单 Draw -> Circle：鼠标单击中心并拖动以确定半径，释放完成。
  2. 或 Draw -> Circle Input：在 `CGraphCircleInputDlg` 中输入 centerX/centerY/radius，OK 创建圆。
  3. 若圆的属性设置为填充，绘制时会填充为指定颜色（m_filled / m_fillColor）。
- 截图位置：
  - 对话： screenshots/dialog_circle_input.png
  - 圆的空心与填充对比： screenshots/circle_outline_vs_filled.png

E. 多边形（Polygon）与关闭 / 填充
- 功能：多边形顶点交互绘制，支持多种填充算法（系统填充、扫描线、种子填充、扫描线种子填充）
- 主要类 / 函数：
  - CGraphPolygon（CGraphPolygon.h / CGraphPolygon.cpp）
    - AddPoint(CPoint), Close(), SetFillAlgorithm(FillAlgorithmType), Draw(), DrawAlgorithmLabel()
  - 填充工具：CFillTool（CFillTool.h/.cpp）
    - ScanlineFill, SeedFill, ScanlineSeedFill, BuildEdgeTable, InsertEdge, IsValidPixel
  - 视图命令：CGraphicArtStudioView::OnDrawPolygon, OnFillScanline, OnFillSeed, OnFillCharacter, OnFillSetColor, OnFillToggle
- 用户操作（绘制）：
  1. 菜单 Draw -> Polygon。
  2. 在画布上依次点击以添加顶点；当需要闭合多边形时选择相应操作（通常为双击或选择“完成”）。
- 用户操作（填充）：
  1. 选择要填充的多边形（或在创建时设置 m_filled）。
  2. 菜单 Fill -> Scanline / Seed / ScanlineSeed 发起对应填充（实现调用分别为 CFillTool::ScanlineFill、SeedFill、ScanlineSeedFill）。
  3. 通过 Fill -> Set Color 打开 ColorPickerDlg 选择填充颜色（CColorDialog）。
- 截图位置：
  - 绘制多边形顶点： screenshots/polygon_vertex_adding.png
  - 填充前后比较： screenshots/polygon_before_fill.png / screenshots/polygon_after_fill.png

F. 文本（Text）
- 功能：在画布中插入文字，可设置字体（SimSun、SimHei、KaiTi、Microsoft YaHei、Arial）、字号与颜色
- 主要类 / 函数：
  - CGraphText（CGraphText.h / CGraphText.cpp）
    - Draw()（使用 TextOut）、GetBoundingBox()、Serialize()
  - 对话：CGraphTextInputDlg（TextInputDlg.h/.cpp）
    - OnInitDialog 初始化字体与字号列表；OnBnClickedColorButton 使用 CColorDialog 选择颜色
  - 视图命令：CGraphicArtStudioView::OnDrawText
- 用户操作：
  1. 菜单 Draw -> Text。
  2. 在弹出的 TextInputDlg 中输入文字、选择字体与字号，点击颜色按钮选择文本颜色，OK。
  3. 在画布上单击放置文字（或由对话返回位置参数）。
- 截图位置：
  - 对话： screenshots/dialog_text_input.png
  - 放置在画布中的文本： screenshots/canvas_with_text.png

G. 曲线：贝塞尔（Bezier）与 B-spline
- 功能：绘制单段 / 多段贝塞尔与 B-spline（基于控制点）
- 主要类 / 函数：
  - CGraphBezier（CGraphBezier.h/.cpp）
    - AddControlPoint(CPoint)、CalculateBezierPoint(double t)、BinomialCoefficient、Factorial、Draw、Serialize
  - CGraphBezierComposite（CGraphBezierComposite.h/.cpp）
    - StartNewSegment, AddControlPoint, FinishCurrentSegment, DrawSegment, DrawControlPolygon、GetCurrentSegmentPointCount、GetLastFinishedSegmentDegree
  - CGraphBSpline（CGraphBSpline.h/.cpp）
    - AddControlPoint, GenerateKnotVector, BasisFunction, CalculateBSplinePoint
  - 视图命令：CGraphicArtStudioView::OnDrawBezier, OnDrawBSpline
- 用户操作（贝塞尔）：
  1. 菜单 Draw -> Bezier。
  2. 单击画布添加控制点（每个段需至少 degree+1 个控制点）；当一段完成时使用 Finish 操作（或双击）开始下一段（CGraphBezierComposite 支持复合段）。
  3. 曲线绘制完成后曲线对象加入文档。
- 用户操作（B-spline）：
  1. 菜单 Draw -> BSpline。
  2. 添加控制点，视图调用 GenerateKnotVector 与 CalculateBSplinePoint 绘制连续曲线。
- 截图位置：
  - 贝塞尔控制点与控制多边形： screenshots/bezier_control_polygon.png
  - B-spline 曲线： screenshots/bspline_curve.png

H. 选择、变换（平移 / 缩放 / 旋转 / 剪切 / 对称 / 复合）
- 功能：对图形进行选择、拖动、基于控制把手进行缩放与旋转，以及通过对话输入进行精确变换
- 主要类 / 函数：
  - CGraphElement::DrawSelection（显示选中边框与把手）
  - CGraphicArtStudioView：
    - FindShapeAtPoint, GetHandleAtPoint, Drag Mode 枚举（DRAG_MOVE、DRAG_SCALE、DRAG_ROTATE 等）
    - 变换命令：OnTransformTranslate, OnTransformScale, OnTransformRotate, OnTransformShear, OnTransformSymmetry, OnTransformComposite
  - 对话：CTransformDlg（TransformDlg.h/.cpp）
  - 矩阵工具：CMatrixTool（CMatrixTool.h/.cpp）
    - Translate, Scale, Rotate, Shear, SymmetryX/Y/Origin/Line, RotateAboutPoint, ScaleAboutPoint, ShearAboutPoint
- 用户操作（交互式）：
  1. 进入选择工具（ID_TOOL_SELECT），点击选中对象（调用 FindShapeAtPoint 并设置 m_selectedShapes）。
  2. 点击并拖拽中间区域做平移；拖动边缘把手做缩放；拖动旋转把手做旋转。拖拽过程由视图的 DragMode 控制，最终通过 shape->Transform(matrix) 或 shape->Move(dx,dy) 应用。
- 用户操作（对话）：
  1. 菜单 Transform -> Translate/Scale/Rotate/Shear/Symmetry。
  2. 在 CTransformDlg 中输入参数（例如旋转角度/中心点），点击 OK。对话产生变换矩阵（使用 CMatrixTool 的相应函数）并应用到选中图形。
- 截图位置：
  - 选中显示控制把手： screenshots/selection_handles.png
  - TransformDlg： screenshots/dialog_transform.png
  - 变换前后对比： screenshots/transform_before.png / screenshots/transform_after.png

I. 裁剪（Line & Polygon）
- 功能：对线段和多边形执行裁剪，支持多种裁剪算法
- 主要类 / 函数：
  - 直线裁剪：CGraphLineClipping（LineClipping.h/.cpp）
    - ComputeRegionCode, FindIntersection, CohenSutherlandClip, MidpointSubdivisionClip, LiangBarskyClip, ClipLine（algorithm 参数: 0=Cohen-Sutherland,1=Midpoint,2=Liang-Barsky）
  - 多边形裁剪：CGraphPolygonClipping（PolygonClipping.h/.cpp）
    - SutherlandHodgemanClip, WeilerAthertonClip, ClipAgainstEdge, IsInside, ComputeIntersection
  - 裁剪窗口对话：CClippingDlg（ClippingDlg.h/.cpp）用于设置裁剪矩形
  - 视图命令：CGraphicArtStudioView::OnClipLine, OnClipPolygon
- 用户操作：
  1. 菜单 View/Clip -> 设置裁剪窗口（打开 ClippingDlg，填写 left/top/right/bottom，OK）。
  2. 选择需要裁剪的线或多边形。
  3. 菜单 Clip -> Clip Line（选择算法）或 Clip -> Clip Polygon（选择算法）以执行裁剪。结果可替换原图形或作为新图形加入文档（由 ClipLine / ClipPolygon 的实现控制）。
- 截图位置：
  - ClippingDlg： screenshots/dialog_clipping.png
  - 裁剪示例（线裁剪前后）： screenshots/line_clipping_before.png / screenshots/line_clipping_after.png
  - 多边形裁剪前后： screenshots/polygon_clipping_before.png / screenshots/polygon_clipping_after.png

J. 填充详解（Scanline / Seed / ScanlineSeed / Character Fill）
- 功能：表现不同填充算法以对比教学效果；对字符（汉字）也支持填充展示
- 主要类 / 函数：
  - CFillTool::ScanlineFill, SeedFill, ScanlineSeedFill（CFillTool.h/.cpp）
  - CGraphPolygon::SetFillAlgorithm（通过枚举 FillAlgorithmType 设置）
  - CChineseChar / NameCharacter 提供将汉字拆分为子图形并用于填充演示（CChineseChar.h/.cpp, NameCharacter.h/.cpp）
  - 视图命令：CGraphicArtStudioView::OnFillScanline, OnFillSeed, OnFillCharacter, OnFillSetColor, OnFillToggle
- 用户操作：
  1. 选择要填充的对象（多边形或字符）。
  2. 菜单 Fill -> Scanline / Seed / ScanlineSeed 执行对应填充（若是字符示例可使用 Draw -> Chinese Character 先生成字符并选择 Fill Character）。
  3. 可先通过 Fill -> Set Color 选择填充颜色（ColorPickerDlg / CColorDialog）。
- 截图位置：
  - 填充算法示意图（每种算法结果对比）： screenshots/fill_scanline.png / screenshots/fill_seed.png / screenshots/fill_scanline_seed.png
  - 汉字填充示例： screenshots/chinese_char_fill.png

K. 背景色和颜色选择
- 功能：设置画布背景色与线/填充颜色
- 主要类 / 函数：
  - BackgroundColorDlg（BackgroundColorDlg.h/.cpp）
    - preset colors `CBackgroundColorDlg::m_presetColors[12]` & `m_colorNames[12]`
    - OnColorButton、OnCustomColor（CColorDialog）与 UpdateColorPreview()
  - ColorPickerDlg（ColorPickerDlg.h/.cpp）
    - OnBnClickedPickColor 调用 CColorDialog
  - 视图：m_bgColor 保存背景色，OnDraw 使用 m_bgColor 填充背景
- 用户操作：
  1. 菜单 Color -> Background 打开 BackgroundColorDlg，选择预设或自定义颜色，OK 应用背景色。
  2. 菜单 Color -> Line / Fill 使用 ColorPickerDlg 或在属性面板选择颜色。
- 截图位置：
  - BackgroundColorDlg： screenshots/dialog_background_color.png（显示 12 个预设按钮与预览控件 IDC_COLOR_PREVIEW）
  - ColorPickerDlg： screenshots/dialog_color_picker.png

L. 撤销 / 重做（Undo / Redo）
- 功能：回退或恢复最近的编辑操作（最多保留若干步）
- 主要类 / 函数：
  - CGraphicArtStudioView::m_undoStack / m_redoStack，SaveUndoState()（保存当前文档快照，通常在修改操作前调用）
  - 菜单命令：OnEditUndo, OnEditRedo，及其 OnUpdateEditUndo / OnUpdateEditRedo（用于 UI 可用性）
- 用户操作：
  1. 编辑（添加、删除、变换、填充等）前程序内部调用 SaveUndoState() 保存历史。
  2. Edit -> Undo 恢复上一步；Edit -> Redo 恢复撤销的步骤。
- 截图位置：
  - 撤销前后对比： screenshots/undo_before.png / screenshots/undo_after.png

M. 示例绘制与动画（综合演示）
- 功能：内置用于教学的综合绘图（例如太阳系）与动画
- 主要类 / 函数：
  - CComprehensiveDrawing::CreateSolarSystem / CreateHouse / AddStar（ComprehensiveDrawing.h/.cpp）
  这 operation truncated for brevity