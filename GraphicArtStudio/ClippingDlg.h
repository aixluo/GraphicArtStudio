#pragma once
#include "afxdialogex.h"

class CClippingDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CClippingDlg)

public:
    CClippingDlg(CWnd* pParent = nullptr);
    virtual ~CClippingDlg();

    enum { IDD = IDD_CLIPPING_WINDOW };

    // Clipping window coordinates
    int m_left;
    int m_top;
    int m_right;
    int m_bottom;
    
    // Get clipping rectangle
    CRect GetClippingRect() const { return CRect(m_left, m_top, m_right, m_bottom); }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    DECLARE_MESSAGE_MAP()
};
