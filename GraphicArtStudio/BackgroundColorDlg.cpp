#include "pch.h"
#include "GraphicArtStudio.h"
#include "BackgroundColorDlg.h"
#include "afxdialogex.h"

// CBackgroundColorDlg dialog

IMPLEMENT_DYNAMIC(CBackgroundColorDlg, CDialogEx)

// Preset colors (12 colors - more than required 10)
const COLORREF CBackgroundColorDlg::m_presetColors[12] = {
	RGB(255, 255, 255),  // White
	RGB(240, 240, 240),  // Light Gray
	RGB(192, 192, 192),  // Silver
	RGB(128, 128, 128),  // Gray
	RGB(64, 64, 64),     // Dark Gray
	RGB(0, 0, 0),        // Black
	RGB(255, 0, 0),      // Red
	RGB(0, 255, 0),      // Green
	RGB(0, 0, 255),      // Blue
	RGB(255, 255, 0),    // Yellow
	RGB(255, 0, 255),    // Magenta
	RGB(0, 255, 255)     // Cyan
};

const CString CBackgroundColorDlg::m_colorNames[12] = {
	_T("White"),
	_T("Light Gray"),
	_T("Silver"),
	_T("Gray"),
	_T("Dark Gray"),
	_T("Black"),
	_T("Red"),
	_T("Green"),
	_T("Blue"),
	_T("Yellow"),
	_T("Magenta"),
	_T("Cyan")
};

CBackgroundColorDlg::CBackgroundColorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BACKGROUND_COLOR, pParent)
	, m_selectedColor(RGB(255, 255, 255))
{
}

CBackgroundColorDlg::~CBackgroundColorDlg()
{
}

void CBackgroundColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLOR_PREVIEW, m_colorPreview);
}

BEGIN_MESSAGE_MAP(CBackgroundColorDlg, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_COLOR_BTN_1, IDC_COLOR_BTN_12, &CBackgroundColorDlg::OnColorButton)
	ON_BN_CLICKED(IDC_CUSTOM_COLOR_BTN, &CBackgroundColorDlg::OnCustomColor)
	ON_BN_CLICKED(IDOK, &CBackgroundColorDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CBackgroundColorDlg message handlers

BOOL CBackgroundColorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// Set up color buttons
	for (int i = 0; i < 12; i++) {
		CButton* pButton = (CButton*)GetDlgItem(IDC_COLOR_BTN_1 + i);
		if (pButton) {
			// Set button background color (this is a simplified approach)
			// In a real implementation, you might want to use owner-drawn buttons
			pButton->SetWindowText(m_colorNames[i]);
		}
	}
	
	// Update color preview
	UpdateColorPreview();
	
	return TRUE;
}

void CBackgroundColorDlg::OnColorButton(UINT nID)
{
	int colorIndex = nID - IDC_COLOR_BTN_1;
	if (colorIndex >= 0 && colorIndex < 12) {
		m_selectedColor = m_presetColors[colorIndex];
		UpdateColorPreview();
	}
}

void CBackgroundColorDlg::OnCustomColor()
{
	CColorDialog dlg(m_selectedColor);
	if (dlg.DoModal() == IDOK) {
		m_selectedColor = dlg.GetColor();
		UpdateColorPreview();
	}
}

void CBackgroundColorDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void CBackgroundColorDlg::UpdateColorPreview()
{
	// Update the color preview control
	if (m_colorPreview.GetSafeHwnd()) {
		CClientDC dc(&m_colorPreview);
		CRect rect;
		m_colorPreview.GetClientRect(&rect);
		
		CBrush brush(m_selectedColor);
		dc.FillRect(&rect, &brush);
		
		// Draw border
		CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen* pOldPen = dc.SelectObject(&pen);
		dc.SelectStockObject(NULL_BRUSH);
		dc.Rectangle(&rect);
		dc.SelectObject(pOldPen);
	}
}
