#include "pch.h"
#include "GraphicArtStudio.h"
#include "CircleInputDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CGraphCircleInputDlg, CDialogEx)

CGraphCircleInputDlg::CGraphCircleInputDlg(CWnd* pParent)
	: CDialogEx(IDD_CIRCLE_INPUT, pParent)
	, m_centerX(200)
	, m_centerY(200)
	, m_radius(50)
{
}

CGraphCircleInputDlg::~CGraphCircleInputDlg()
{
}

void CGraphCircleInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CIRCLE_CENTER_X, m_centerX);
	DDX_Text(pDX, IDC_CIRCLE_CENTER_Y, m_centerY);
	DDX_Text(pDX, IDC_CIRCLE_RADIUS, m_radius);
	DDV_MinMaxInt(pDX, m_radius, 1, 1000);
}

BEGIN_MESSAGE_MAP(CGraphCircleInputDlg, CDialogEx)
END_MESSAGE_MAP()
