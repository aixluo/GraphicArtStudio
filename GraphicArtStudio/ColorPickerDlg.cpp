#include "pch.h"
#include "ColorPickerDlg.h"
#include "Resource.h"

IMPLEMENT_DYNAMIC(CColorPickerDlg, CDialogEx)

CColorPickerDlg::CColorPickerDlg(CWnd* pParent)
	: CDialogEx(IDD_COLOR_PICKER, pParent)
	, m_selectedColor(RGB(255, 255, 0)) // 默认黄色
{
}

CColorPickerDlg::~CColorPickerDlg()
{
}

void CColorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CColorPickerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PICK_COLOR_BTN, &CColorPickerDlg::OnBnClickedPickColor)
END_MESSAGE_MAP()

BOOL CColorPickerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

void CColorPickerDlg::OnBnClickedPickColor()
{
	CColorDialog dlg(m_selectedColor, CC_FULLOPEN | CC_ANYCOLOR);
	if (dlg.DoModal() == IDOK)
	{
		m_selectedColor = dlg.GetColor();
	}
}
