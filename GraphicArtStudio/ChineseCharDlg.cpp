#include "pch.h"
#include "GraphicArtStudio.h"
#include "ChineseCharDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CChineseCharDlg, CDialogEx)

CChineseCharDlg::CChineseCharDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHINESE_CHAR, pParent)
	, m_chineseChar(_T(""))
	, m_fontName(_T("宋体"))
	, m_fontSize(200)
	, m_outlineColor(RGB(0, 0, 0))
	, m_outlineWidth(2)
	, m_lineStyle(PS_SOLID)
	, m_hollow(true)
{
}

CChineseCharDlg::~CChineseCharDlg()
{
}

void CChineseCharDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CHAR_EDIT, m_chineseChar);
	DDX_Control(pDX, IDC_FONT_COMBO, m_fontCombo);
	DDX_Control(pDX, IDC_FONTSIZE_COMBO, m_fontSizeCombo);
	DDX_Control(pDX, IDC_LINEWIDTH_COMBO, m_lineWidthCombo);
	DDX_Control(pDX, IDC_LINESTYLE_COMBO, m_lineStyleCombo);
	DDX_Check(pDX, IDC_HOLLOW_CHECK, m_hollow);
}

BEGIN_MESSAGE_MAP(CChineseCharDlg, CDialogEx)
	ON_BN_CLICKED(IDC_PREVIEW_BTN, &CChineseCharDlg::OnBnClickedPreview)
	ON_BN_CLICKED(IDOK, &CChineseCharDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_COLOR_BTN, &CChineseCharDlg::OnBnClickedColorBtn)
END_MESSAGE_MAP()

BOOL CChineseCharDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 初始化字体列表
	m_fontCombo.AddString(_T("宋体"));
	m_fontCombo.AddString(_T("黑体"));
	m_fontCombo.AddString(_T("楷体"));
	m_fontCombo.AddString(_T("微软雅黑"));
	m_fontCombo.AddString(_T("仿宋"));
	m_fontCombo.AddString(_T("隶书"));
	m_fontCombo.AddString(_T("幼圆"));
	m_fontCombo.SetCurSel(0);

	// 初始化字号列表
	m_fontSizeCombo.AddString(_T("100"));
	m_fontSizeCombo.AddString(_T("150"));
	m_fontSizeCombo.AddString(_T("200"));
	m_fontSizeCombo.AddString(_T("250"));
	m_fontSizeCombo.AddString(_T("300"));
	m_fontSizeCombo.AddString(_T("400"));
	m_fontSizeCombo.SetCurSel(2); // 默认200

	// 初始化线宽列表
	m_lineWidthCombo.AddString(_T("1"));
	m_lineWidthCombo.AddString(_T("2"));
	m_lineWidthCombo.AddString(_T("3"));
	m_lineWidthCombo.AddString(_T("4"));
	m_lineWidthCombo.AddString(_T("5"));
	m_lineWidthCombo.AddString(_T("8"));
	m_lineWidthCombo.AddString(_T("10"));
	m_lineWidthCombo.SetCurSel(1); // 默认2

	// 初始化线形列表
	m_lineStyleCombo.AddString(_T("实线"));
	m_lineStyleCombo.AddString(_T("虚线"));
	m_lineStyleCombo.AddString(_T("点线"));
	m_lineStyleCombo.AddString(_T("点划线"));
	m_lineStyleCombo.AddString(_T("双点划线"));
	m_lineStyleCombo.SetCurSel(0); // 默认实线

	// 默认镂空
	CheckDlgButton(IDC_HOLLOW_CHECK, BST_CHECKED);

	return TRUE;
}

void CChineseCharDlg::OnBnClickedPreview()
{
	UpdateData(TRUE);

	// 获取字体和字号
	int fontSel = m_fontCombo.GetCurSel();
	if (fontSel != CB_ERR) {
		m_fontCombo.GetLBText(fontSel, m_fontName);
	}

	int sizeSel = m_fontSizeCombo.GetCurSel();
	if (sizeSel != CB_ERR) {
		CString sizeStr;
		m_fontSizeCombo.GetLBText(sizeSel, sizeStr);
		m_fontSize = _ttoi(sizeStr);
	}

	// 验证输入
	if (m_chineseChar.IsEmpty()) {
		AfxMessageBox(_T("请输入一个汉字！"));
		return;
	}

	if (m_chineseChar.GetLength() > 1) {
		AfxMessageBox(_T("请只输入一个汉字！"));
		return;
	}

	// 显示预览信息
	CString msg;
	CString lineStyleName;
	switch (m_lineStyle) {
	case PS_SOLID: lineStyleName = _T("实线"); break;
	case PS_DASH: lineStyleName = _T("虚线"); break;
	case PS_DOT: lineStyleName = _T("点线"); break;
	case PS_DASHDOT: lineStyleName = _T("点划线"); break;
	case PS_DASHDOTDOT: lineStyleName = _T("双点划线"); break;
	default: lineStyleName = _T("实线"); break;
	}
	msg.Format(_T("汉字: %s\n字体: %s\n字号: %d\n线宽: %d\n线形: %s\n颜色: RGB(%d,%d,%d)\n镂空: %s"),
		m_chineseChar,
		m_fontName,
		m_fontSize,
		m_outlineWidth,
		lineStyleName,
		GetRValue(m_outlineColor), GetGValue(m_outlineColor), GetBValue(m_outlineColor),
		m_hollow ? _T("是") : _T("否"));
	AfxMessageBox(msg);
}

void CChineseCharDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	// 获取字体和字号
	int fontSel = m_fontCombo.GetCurSel();
	if (fontSel != CB_ERR) {
		m_fontCombo.GetLBText(fontSel, m_fontName);
	}

	int sizeSel = m_fontSizeCombo.GetCurSel();
	if (sizeSel != CB_ERR) {
		CString sizeStr;
		m_fontSizeCombo.GetLBText(sizeSel, sizeStr);
		m_fontSize = _ttoi(sizeStr);
	}

	// 获取线宽
	int widthSel = m_lineWidthCombo.GetCurSel();
	if (widthSel != CB_ERR) {
		CString widthStr;
		m_lineWidthCombo.GetLBText(widthSel, widthStr);
		m_outlineWidth = _ttoi(widthStr);
	}

	// 获取线形
	int styleSel = m_lineStyleCombo.GetCurSel();
	if (styleSel != CB_ERR) {
		switch (styleSel) {
		case 0: m_lineStyle = PS_SOLID; break;
		case 1: m_lineStyle = PS_DASH; break;
		case 2: m_lineStyle = PS_DOT; break;
		case 3: m_lineStyle = PS_DASHDOT; break;
		case 4: m_lineStyle = PS_DASHDOTDOT; break;
		default: m_lineStyle = PS_SOLID; break;
		}
	}

	// 验证输入
	if (m_chineseChar.IsEmpty()) {
		AfxMessageBox(_T("请输入一个汉字！"));
		return;
	}

	if (m_chineseChar.GetLength() > 1) {
		AfxMessageBox(_T("请只输入一个汉字！"));
		return;
	}

	CDialogEx::OnOK();
}

void CChineseCharDlg::OnBnClickedColorBtn()
{
	CColorDialog dlg(m_outlineColor);
	if (dlg.DoModal() == IDOK) {
		m_outlineColor = dlg.GetColor();
		// 可以在这里更新颜色按钮的显示
	}
}
