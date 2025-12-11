#pragma once
#include "afxdialogex.h"
#include "resource.h"

// CBackgroundColorDlg dialog

class CBackgroundColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBackgroundColorDlg)

public:
	CBackgroundColorDlg(CWnd* pParent = nullptr);
	virtual ~CBackgroundColorDlg();

// Dialog data
	enum { IDD = IDD_BACKGROUND_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

public:
	// Selected background color
	COLORREF m_selectedColor;
	
	// Preset colors (at least 10)
	static const COLORREF m_presetColors[12];
	static const CString m_colorNames[12];
	
	// Message handlers
	virtual BOOL OnInitDialog();
	afx_msg void OnColorButton(UINT nID);
	afx_msg void OnCustomColor();
	afx_msg void OnBnClickedOk();
	
private:
	// Color preview
	void UpdateColorPreview();
	CStatic m_colorPreview;
};
