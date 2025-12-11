#pragma once
#include "afxdialogex.h"

class CGraphTextInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphTextInputDlg)

public:
	CGraphTextInputDlg(CWnd* pParent = nullptr);
	virtual ~CGraphTextInputDlg();

	enum { IDD = IDD_TEXT_INPUT };

	CString m_text;
	CString m_fontName;
	int m_fontSize;
	COLORREF m_textColor;
	
	int m_selectedFont;
	int m_selectedSize;
	
	CComboBox m_fontCombo;
	CComboBox m_sizeCombo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedColorButton();
};
