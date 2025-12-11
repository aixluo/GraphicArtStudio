#pragma once
#include <afxdialogex.h>
#include "Resource.h"

// 颜色选择对话框
class CColorPickerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorPickerDlg)

public:
	CColorPickerDlg(CWnd* pParent = nullptr);
	virtual ~CColorPickerDlg();

	enum { IDD = IDD_COLOR_PICKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	COLORREF m_selectedColor;
	afx_msg void OnBnClickedPickColor();
	virtual BOOL OnInitDialog();
};
