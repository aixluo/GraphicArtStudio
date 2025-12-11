#pragma once
#include "afxdialogex.h"

// CanvasSizeDlg 对话框

class CanvasSizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CanvasSizeDlg)

public:
	CanvasSizeDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CanvasSizeDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CANVAS_SIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	// 画布尺寸
	int m_width;
	int m_height;
	
	// 预设尺寸选择
	int m_presetSize;
	
	// 控件变量
	CComboBox m_presetCombo;
	CEdit m_widthEdit;
	CEdit m_heightEdit;
	
	// 消息处理
	afx_msg void OnPresetSelectionChanged();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
