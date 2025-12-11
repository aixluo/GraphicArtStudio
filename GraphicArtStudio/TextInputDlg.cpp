#include "pch.h"
#include "GraphicArtStudio.h"
#include "TextInputDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CGraphTextInputDlg, CDialogEx)

CGraphTextInputDlg::CGraphTextInputDlg(CWnd* pParent)
	: CDialogEx(IDD_TEXT_INPUT, pParent)
	, m_text(_T(""))
	, m_fontName(_T("SimSun"))
	, m_fontSize(16)
	, m_textColor(RGB(0, 0, 0))
	, m_selectedFont(0)
	, m_selectedSize(2)
{
}

CGraphTextInputDlg::~CGraphTextInputDlg()
{
}

void CGraphTextInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT_CONTENT, m_text);
	DDX_Control(pDX, IDC_FONT_COMBO, m_fontCombo);
	DDX_Control(pDX, IDC_FONTSIZE_COMBO, m_sizeCombo);
}

BEGIN_MESSAGE_MAP(CGraphTextInputDlg, CDialogEx)
	ON_BN_CLICKED(IDC_TEXT_COLOR_BTN, &CGraphTextInputDlg::OnBnClickedColorButton)
END_MESSAGE_MAP()

BOOL CGraphTextInputDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Initialize font combo box with English names to avoid encoding issues
	m_fontCombo.AddString(_T("SimSun"));      // 宋体
	m_fontCombo.AddString(_T("SimHei"));      // 黑体
	m_fontCombo.AddString(_T("KaiTi"));       // 楷体
	m_fontCombo.AddString(_T("Microsoft YaHei"));  // 微软雅黑
	m_fontCombo.AddString(_T("Arial"));    // Arial
	m_fontCombo.SetCurSel(0);

	// Initialize font size combo box
	m_sizeCombo.AddString(_T("12"));
	m_sizeCombo.AddString(_T("14"));
	m_sizeCombo.AddString(_T("16"));
	m_sizeCombo.AddString(_T("18"));
	m_sizeCombo.AddString(_T("20"));
	m_sizeCombo.AddString(_T("24"));
	m_sizeCombo.AddString(_T("28"));
	m_sizeCombo.SetCurSel(2); // Default to 16

	return TRUE;
}

void CGraphTextInputDlg::OnBnClickedColorButton()
{
	CColorDialog dlg(m_textColor);
	if (dlg.DoModal() == IDOK) {
		m_textColor = dlg.GetColor();
	}
}

void CGraphTextInputDlg::OnOK()
{
	UpdateData(TRUE);
	
	m_selectedFont = m_fontCombo.GetCurSel();
	m_selectedSize = m_sizeCombo.GetCurSel();
	
	if (m_selectedFont != CB_ERR) {
		m_fontCombo.GetLBText(m_selectedFont, m_fontName);
	}
	
	if (m_selectedSize != CB_ERR) {
		CString sizeStr;
		m_sizeCombo.GetLBText(m_selectedSize, sizeStr);
		m_fontSize = _ttoi(sizeStr);
	}
	
	CDialogEx::OnOK();
}
