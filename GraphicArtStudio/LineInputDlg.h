#pragma once
#include "afxdialogex.h"

class CGraphLineInputDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CGraphLineInputDlg)

public:
    CGraphLineInputDlg(CWnd* pParent = nullptr);
    virtual ~CGraphLineInputDlg();

    enum { IDD = IDD_LINE_INPUT };

    // Line coordinates
    int m_startX;
    int m_startY;
    int m_endX;
    int m_endY;
    
    // Get line points
    CPoint GetStartPoint() const { return CPoint(m_startX, m_startY); }
    CPoint GetEndPoint() const { return CPoint(m_endX, m_endY); }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    DECLARE_MESSAGE_MAP()
};
