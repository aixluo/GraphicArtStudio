#include "pch.h"
#include "GraphicArtStudio.h"
#include "CanvasSizeDlg.h"
#include "afxdialogex.h"

// CCanvasSizeDlg dialog

IMPLEMENT_DYNAMIC(CCanvasSizeDlg, CDialogEx)

CCanvasSizeDlg::CCanvasSizeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CANVAS_SIZE, pParent)
	, m_width(800)
	, m_height(600)
	, m_presetSize(0)
{

}

CCanvasSizeDlg::~CCanvasSizeDlg()
{
}

void CCanvasSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PRESET_COMBO, m_presetCombo);
	DDX_Control(pDX, IDC_WIDTH_EDIT, m_widthEdit);
	DDX_Control(pDX, IDC_HEIGHT_EDIT, m_heightEdit);
	DDX_Text(pDX, IDC_WIDTH_EDIT, m_width);
	DDX_Text(pDX, IDC_HEIGHT_EDIT, m_height);
	DDV_MinMaxInt(pDX, m_width, 400, 2000);
	DDV_MinMaxInt(pDX, m_height, 300, 1500);
}

BEGIN_MESSAGE_MAP(CCanvasSizeDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_PRESET_COMBO, &CCanvasSizeDlg::OnPresetSelectionChanged)
	ON_BN_CLICKED(IDOK, &CCanvasSizeDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CCanvasSizeDlg message handlers

BOOL CCanvasSizeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// Add preset size options
	m_presetCombo.AddString(_T("Custom"));
	m_presetCombo.AddString(_T("800 x 600 (Standard)"));
	m_presetCombo.AddString(_T("1024 x 768 (Medium)"));
	m_presetCombo.AddString(_T("1280 x 720 (HD)"));
	m_presetCombo.AddString(_T("1366 x 768 (Laptop)"));
	m_presetCombo.AddString(_T("1920 x 1080 (Full HD)"));
	
	// Set default selection
	m_presetCombo.SetCurSel(1); // Default to 800x600
	
	// Update edit controls
	UpdateData(FALSE);
	
	return TRUE;
}

void CCanvasSizeDlg::OnPresetSelectionChanged()
{
	int sel = m_presetCombo.GetCurSel();
	
	switch (sel) {
	case 0: // Custom
		m_widthEdit.EnableWindow(TRUE);
		m_heightEdit.EnableWindow(TRUE);
		break;
	case 1: // 800x600 (Standard)
		m_width = 800;
		m_height = 600;
		m_widthEdit.EnableWindow(FALSE);
		m_heightEdit.EnableWindow(FALSE);
		break;
	case 2: // 1024x768 (Medium)
		m_width = 1024;
		m_height = 768;
		m_widthEdit.EnableWindow(FALSE);
		m_heightEdit.EnableWindow(FALSE);
		break;
	case 3: // 1280x720 (HD)
		m_width = 1280;
		m_height = 720;
		m_widthEdit.EnableWindow(FALSE);
		m_heightEdit.EnableWindow(FALSE);
		break;
	case 4: // 1366x768 (Laptop)
		m_width = 1366;
		m_height = 768;
		m_widthEdit.EnableWindow(FALSE);
		m_heightEdit.EnableWindow(FALSE);
		break;
	case 5: // 1920x1080 (Full HD)
		m_width = 1920;
		m_height = 1080;
		m_widthEdit.EnableWindow(FALSE);
		m_heightEdit.EnableWindow(FALSE);
		break;
	}
	
	UpdateData(FALSE);
}

void CCanvasSizeDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	// Validate size range
	if (m_width < 400 || m_width > 2000 || m_height < 300 || m_height > 1500) {
		AfxMessageBox(_T("Canvas size out of range!\nWidth: 400-2000 pixels\nHeight: 300-1500 pixels"));
		return;
	}
	
	CDialogEx::OnOK();
}
