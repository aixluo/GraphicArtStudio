#pragma once
#include "afxdialogex.h"

// Transform types
enum TransformType {
    TRANSFORM_TRANSLATE,
    TRANSFORM_SCALE,
    TRANSFORM_ROTATE,
    TRANSFORM_SHEAR,
    TRANSFORM_SYMMETRY
};

class CTransformDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CTransformDlg)

public:
    CTransformDlg(TransformType type, CWnd* pParent = nullptr);
    virtual ~CTransformDlg();

    enum { IDD = IDD_TRANSFORM };

    TransformType m_transformType;
    
    // Translate parameters
    double m_dx;
    double m_dy;
    
    // Scale parameters
    double m_sx;
    double m_sy;
    CPoint m_scaleCenter;
    
    // Rotate parameters
    double m_angle;
    CPoint m_rotateCenter;
    
    // Shear parameters
    double m_shx;
    double m_shy;
    
    // Symmetry parameters
    int m_symmetryType; // 0=X-axis, 1=Y-axis, 2=Origin, 3=Line
    double m_lineA;
    double m_lineB;
    double m_lineC;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    DECLARE_MESSAGE_MAP()
};
