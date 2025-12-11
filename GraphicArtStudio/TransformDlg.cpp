#include "pch.h"
#include "GraphicArtStudio.h"
#include "TransformDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CTransformDlg, CDialogEx)

CTransformDlg::CTransformDlg(TransformType type, CWnd* pParent)
    : CDialogEx(IDD_TRANSFORM, pParent)
    , m_transformType(type)
    , m_dx(0)
    , m_dy(0)
    , m_sx(1.0)
    , m_sy(1.0)
    , m_scaleCenter(0, 0)
    , m_angle(0)
    , m_rotateCenter(0, 0)
    , m_shx(0)
    , m_shy(0)
    , m_symmetryType(0)
    , m_lineA(1)
    , m_lineB(0)
    , m_lineC(0)
{
}

CTransformDlg::~CTransformDlg()
{
}

void CTransformDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    
    switch (m_transformType) {
    case TRANSFORM_TRANSLATE:
        DDX_Text(pDX, IDC_TRANS_PARAM1, m_dx);
        DDX_Text(pDX, IDC_TRANS_PARAM2, m_dy);
        break;
    case TRANSFORM_SCALE:
        DDX_Text(pDX, IDC_TRANS_PARAM1, m_sx);
        DDX_Text(pDX, IDC_TRANS_PARAM2, m_sy);
        DDX_Text(pDX, IDC_TRANS_PARAM3, m_scaleCenter.x);
        DDX_Text(pDX, IDC_TRANS_PARAM4, m_scaleCenter.y);
        break;
    case TRANSFORM_ROTATE:
        DDX_Text(pDX, IDC_TRANS_PARAM1, m_angle);
        DDX_Text(pDX, IDC_TRANS_PARAM3, m_rotateCenter.x);
        DDX_Text(pDX, IDC_TRANS_PARAM4, m_rotateCenter.y);
        break;
    case TRANSFORM_SHEAR:
        DDX_Text(pDX, IDC_TRANS_PARAM1, m_shx);
        DDX_Text(pDX, IDC_TRANS_PARAM2, m_shy);
        DDX_Text(pDX, IDC_TRANS_PARAM3, m_scaleCenter.x); // Reuse m_scaleCenter for shear center
        DDX_Text(pDX, IDC_TRANS_PARAM4, m_scaleCenter.y);
        break;
    case TRANSFORM_SYMMETRY:
        DDX_CBIndex(pDX, IDC_SYMMETRY_TYPE, m_symmetryType);
        DDX_Text(pDX, IDC_TRANS_PARAM1, m_lineA);
        DDX_Text(pDX, IDC_TRANS_PARAM2, m_lineB);
        DDX_Text(pDX, IDC_TRANS_PARAM3, m_lineC);
        break;
    }
}

BEGIN_MESSAGE_MAP(CTransformDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CTransformDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    
    // Helper lambda to show/hide controls
    auto ShowControlPair = [&](int labelID, int editID, BOOL show) {
        GetDlgItem(labelID)->ShowWindow(show ? SW_SHOW : SW_HIDE);
        GetDlgItem(editID)->ShowWindow(show ? SW_SHOW : SW_HIDE);
    };

    // Hide all initially
    for (int i = 0; i < 6; i++) {
        ShowControlPair(IDC_TRANS_LABEL1 + i, IDC_TRANS_PARAM1 + i, FALSE);
    }
    GetDlgItem(IDC_SYMMETRY_TYPE)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_SYMMETRY_LABEL)->ShowWindow(SW_HIDE);

    CString title;
    switch (m_transformType) {
    case TRANSFORM_TRANSLATE:
        title = _T("平移变换");
        SetDlgItemText(IDC_TRANS_LABEL1, _T("水平平移量 (dx):"));
        SetDlgItemText(IDC_TRANS_LABEL2, _T("垂直平移量 (dy):"));
        ShowControlPair(IDC_TRANS_LABEL1, IDC_TRANS_PARAM1, TRUE);
        ShowControlPair(IDC_TRANS_LABEL2, IDC_TRANS_PARAM2, TRUE);
        break;

    case TRANSFORM_SCALE:
        title = _T("缩放变换");
        SetDlgItemText(IDC_TRANS_LABEL1, _T("水平缩放比例 (sx):"));
        SetDlgItemText(IDC_TRANS_LABEL2, _T("垂直缩放比例 (sy):"));
        SetDlgItemText(IDC_TRANS_LABEL3, _T("缩放中心 X:"));
        SetDlgItemText(IDC_TRANS_LABEL4, _T("缩放中心 Y:"));
        ShowControlPair(IDC_TRANS_LABEL1, IDC_TRANS_PARAM1, TRUE);
        ShowControlPair(IDC_TRANS_LABEL2, IDC_TRANS_PARAM2, TRUE);
        ShowControlPair(IDC_TRANS_LABEL3, IDC_TRANS_PARAM3, TRUE);
        ShowControlPair(IDC_TRANS_LABEL4, IDC_TRANS_PARAM4, TRUE);
        break;

    case TRANSFORM_ROTATE:
        title = _T("旋转变换");
        SetDlgItemText(IDC_TRANS_LABEL1, _T("旋转角度 (度):"));
        SetDlgItemText(IDC_TRANS_LABEL3, _T("旋转中心 X:"));
        SetDlgItemText(IDC_TRANS_LABEL4, _T("旋转中心 Y:"));
        ShowControlPair(IDC_TRANS_LABEL1, IDC_TRANS_PARAM1, TRUE);
        ShowControlPair(IDC_TRANS_LABEL3, IDC_TRANS_PARAM3, TRUE);
        ShowControlPair(IDC_TRANS_LABEL4, IDC_TRANS_PARAM4, TRUE);
        break;

    case TRANSFORM_SHEAR:
        title = _T("错切变换");
        SetDlgItemText(IDC_TRANS_LABEL1, _T("水平错切 (shx):"));
        SetDlgItemText(IDC_TRANS_LABEL2, _T("垂直错切 (shy):"));
        SetDlgItemText(IDC_TRANS_LABEL3, _T("错切中心 X:"));
        SetDlgItemText(IDC_TRANS_LABEL4, _T("错切中心 Y:"));
        ShowControlPair(IDC_TRANS_LABEL1, IDC_TRANS_PARAM1, TRUE);
        ShowControlPair(IDC_TRANS_LABEL2, IDC_TRANS_PARAM2, TRUE);
        ShowControlPair(IDC_TRANS_LABEL3, IDC_TRANS_PARAM3, TRUE);
        ShowControlPair(IDC_TRANS_LABEL4, IDC_TRANS_PARAM4, TRUE);
        break;

    case TRANSFORM_SYMMETRY:
        title = _T("对称变换");
        SetDlgItemText(IDC_TRANS_LABEL1, _T("直线参数 A:"));
        SetDlgItemText(IDC_TRANS_LABEL2, _T("直线参数 B:"));
        SetDlgItemText(IDC_TRANS_LABEL3, _T("直线参数 C:"));
        ShowControlPair(IDC_TRANS_LABEL1, IDC_TRANS_PARAM1, TRUE);
        ShowControlPair(IDC_TRANS_LABEL2, IDC_TRANS_PARAM2, TRUE);
        ShowControlPair(IDC_TRANS_LABEL3, IDC_TRANS_PARAM3, TRUE);
        
        GetDlgItem(IDC_SYMMETRY_TYPE)->ShowWindow(SW_SHOW);
        GetDlgItem(IDC_SYMMETRY_LABEL)->ShowWindow(SW_SHOW);
        
        // Initialize symmetry type combo box
        CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_SYMMETRY_TYPE);
        if (pCombo) {
            pCombo->AddString(_T("X轴对称"));
            pCombo->AddString(_T("Y轴对称"));
            pCombo->AddString(_T("原点对称"));
            pCombo->AddString(_T("直线对称 (Ax+By+C=0)"));
            pCombo->SetCurSel(m_symmetryType);
        }
        break;
    }
    SetWindowText(title);
    
    return TRUE;
}

void CTransformDlg::OnOK()
{
    UpdateData(TRUE);
    CDialogEx::OnOK();
}
