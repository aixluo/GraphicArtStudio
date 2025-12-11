#pragma once
#include "afxdialogex.h"

class CanvasSizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CanvasSizeDlg)

public:
	CanvasSizeDlg(CWnd* pParent = nullptr);
	virtual ~CanvasSizeDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CANVAS_SIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

public:
	int m_width;
	int m_height;
	int m_presetSize;
	
	CComboBox m_presetCombo;
	CEdit m_widthEdit;
	CEdit m_heightEdit;
	
	afx_msg void OnPresetSelectionChanged();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
