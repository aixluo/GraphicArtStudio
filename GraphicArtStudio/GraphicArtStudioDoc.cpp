
// GraphicArtStudioDoc.cpp: CGraphicArtStudioDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "GraphicArtStudio.h"
#endif

#include "GraphicArtStudioDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGraphicArtStudioDoc

IMPLEMENT_DYNCREATE(CGraphicArtStudioDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphicArtStudioDoc, CDocument)
END_MESSAGE_MAP()


// CGraphicArtStudioDoc 构造/析构

CGraphicArtStudioDoc::CGraphicArtStudioDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CGraphicArtStudioDoc::~CGraphicArtStudioDoc()
{
}

BOOL CGraphicArtStudioDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CGraphicArtStudioDoc 序列化

#include "CGraphLine.h"
#include "CGraphCircle.h"
#include "CGraphEllipse.h"
#include "CGraphPolygon.h"
#include "CGraphText.h"
#include "CGraphBezier.h"
#include "CGraphBSpline.h"

void CGraphicArtStudioDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// 保存文件
		// 保存图形数量
		int count = (int)m_shapeList.size();
		ar << count;
		
		// 保存每个图形
		for (auto shape : m_shapeList)
		{
			// 保存图形类型
			ar << (int)shape->m_type;
			
			// 调用图形的序列化方法
			shape->Serialize(ar);
		}
	}
	else
	{
		// 加载文件
		// 清空现有图形
		for (auto shape : m_shapeList)
		{
			delete shape;
		}
		m_shapeList.clear();
		
		// 读取图形数量
		int count;
		ar >> count;
		
		// 读取每个图形
		for (int i = 0; i < count; i++)
		{
			// 读取图形类型
			int type;
			ar >> type;
			
			// 根据类型创建图形对象
			CGraphElement* pShape = nullptr;
			switch ((ShapeType)type)
			{
			case SHAPE_LINE:
				pShape = new CGraphLine();
				break;
			case SHAPE_CIRCLE:
				pShape = new CGraphCircle();
				break;
			case SHAPE_ELLIPSE:
				pShape = new CGraphEllipse();
				break;
			case SHAPE_POLYGON:
				pShape = new CGraphPolygon();
				break;
			case SHAPE_TEXT:
				pShape = new CGraphText();
				break;
			case SHAPE_BEZIER:
				pShape = new CGraphBezier();
				break;
			case SHAPE_BSPLINE:
				pShape = new CGraphBSpline();
				break;
			default:
				continue;
			}
			
			if (pShape)
			{
				// 调用图形的序列化方法加载数据
				pShape->Serialize(ar);
				m_shapeList.push_back(pShape);
			}
		}
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CGraphicArtStudioDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CGraphicArtStudioDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CGraphicArtStudioDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGraphicArtStudioDoc 诊断

#ifdef _DEBUG
void CGraphicArtStudioDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphicArtStudioDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGraphicArtStudioDoc 命令
// 【添加实现】：在文件末尾添加 AddShape 函数
void CGraphicArtStudioDoc::AddShape(CGraphElement* pShape)
{
	if (pShape != nullptr)
	{
		m_shapeList.push_back(pShape);
		SetModifiedFlag(); // 标记文档已修改，关闭时提示保存
	}
}