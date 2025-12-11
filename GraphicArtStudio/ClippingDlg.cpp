#include "pch.h"
#include "GraphicArtStudio.h"
#include "ClippingDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CClippingDlg, CDialogEx)

CClippingDlg::CClippingDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_CLIPPING_WINDOW, pParent)
    , m_left(100)
    , m_top(100)
    , m_right(300)
    , m_bottom(250)
{
}

CClippingDlg::~CClippingDlg()
{
}

void CClippingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_CLIP_LEFT, m_left);
    DDX_Text(pDX, IDC_CLIP_TOP, m_top);
    DDX_Text(pDX, IDC_CLIP_RIGHT, m_right);
    DDX_Text(pDX, IDC_CLIP_BOTTOM, m_bottom);
    
    DDV_MinMaxInt(pDX, m_left, 0, 1000);
    DDV_MinMaxInt(pDX, m_top, 0, 800);
    DDV_MinMaxInt(pDX, m_right, 0, 1000);
    DDV_MinMaxInt(pDX, m_bottom, 0, 800);
}

BEGIN_MESSAGE_MAP(CClippingDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CClippingDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    
    SetWindowText(_T("设置裁剪窗口"));
    
    return TRUE;
}

void CClippingDlg::OnOK()
{
    UpdateData(TRUE);
    
    // Validate clipping window
    if (m_left >= m_right || m_top >= m_bottom) {
        AfxMessageBox(_T("Invalid clipping window! Please ensure: left < right, top < bottom"));
        return;
    }
    
    CDialogEx::OnOK();
}
