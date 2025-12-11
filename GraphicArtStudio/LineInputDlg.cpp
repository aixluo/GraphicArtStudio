#include "pch.h"
#include "GraphicArtStudio.h"
#include "LineInputDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CGraphLineInputDlg, CDialogEx)

CGraphLineInputDlg::CGraphLineInputDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_LINE_INPUT, pParent)
    , m_startX(100)
    , m_startY(100)
    , m_endX(200)
    , m_endY(200)
{
}

CGraphLineInputDlg::~CGraphLineInputDlg()
{
}

void CGraphLineInputDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_START_X, m_startX);
    DDX_Text(pDX, IDC_START_Y, m_startY);
    DDX_Text(pDX, IDC_END_X, m_endX);
    DDX_Text(pDX, IDC_END_Y, m_endY);
    
    DDV_MinMaxInt(pDX, m_startX, 0, 1000);
    DDV_MinMaxInt(pDX, m_startY, 0, 800);
    DDV_MinMaxInt(pDX, m_endX, 0, 1000);
    DDV_MinMaxInt(pDX, m_endY, 0, 800);
}

BEGIN_MESSAGE_MAP(CGraphLineInputDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CGraphLineInputDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    
    SetWindowText(_T("输入直线坐标"));
    
    return TRUE;
}

void CGraphLineInputDlg::OnOK()
{
    UpdateData(TRUE);
    
    // Validate coordinates
    if (m_startX == m_endX && m_startY == m_endY) {
        AfxMessageBox(_T("起点和终点不能相同！"));
        return;
    }
    
    CDialogEx::OnOK();
}
