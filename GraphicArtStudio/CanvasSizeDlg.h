#pragma once
#include "afxdialogex.h"
#include "resource.h"

// CCanvasSizeDlg dialog

class CCanvasSizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCanvasSizeDlg)

public:
	CCanvasSizeDlg(CWnd* pParent = nullptr);   // Standard constructor
	virtual ~CCanvasSizeDlg();

// Dialog data
	enum { IDD = IDD_CANVAS_SIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	// Canvas size
	int m_width;
	int m_height;
	
	// Preset size selection
	int m_presetSize;
	
	// Control variables
	CComboBox m_presetCombo;
	CEdit m_widthEdit;
	CEdit m_heightEdit;
	
	// Message handlers
	afx_msg void OnPresetSelectionChanged();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
