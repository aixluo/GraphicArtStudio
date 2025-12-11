#pragma once
#include <vector>
#include "CGraphElement.h" // 引用图形基类

class CGraphicArtStudioDoc : public CDocument
{
protected: // 仅从序列化创建
	CGraphicArtStudioDoc() noexcept;
	DECLARE_DYNCREATE(CGraphicArtStudioDoc)

	// 特性
public:
	// 【核心数据结构】：存储所有图形对象的列表
	std::vector<CGraphElement*> m_shapeList;

	// 操作
public:
	// 【接口】：添加图形到列表
	void AddShape(CGraphElement* pShape);

	// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// 实现
public:
	virtual ~CGraphicArtStudioDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};