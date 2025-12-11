#pragma once
#include "afxdialogex.h"

class CGraphCircleInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphCircleInputDlg)

public:
	CGraphCircleInputDlg(CWnd* pParent = nullptr);
	virtual ~CGraphCircleInputDlg();

	enum { IDD = IDD_CIRCLE_INPUT };

	int m_centerX;
	int m_centerY;
	int m_radius;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};
