#pragma once
#include "afxdialogex.h"

// CChineseCharDlg dialog

class CChineseCharDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChineseCharDlg)

public:
	CChineseCharDlg(CWnd* pParent = nullptr);
	virtual ~CChineseCharDlg();

	enum { IDD = IDD_CHINESE_CHAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CString m_chineseChar;      // 输入的汉字
	CString m_fontName;         // 字体名称
	int m_fontSize;             // 字号
	COLORREF m_outlineColor;    // 轮廓颜色
	int m_outlineWidth;         // 轮廓宽度
	int m_lineStyle;            // 线形样式
	bool m_hollow;              // 是否镂空

	CComboBox m_fontCombo;
	CComboBox m_fontSizeCombo;
	CComboBox m_lineWidthCombo;
	CComboBox m_lineStyleCombo;

	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedColorBtn();
};
