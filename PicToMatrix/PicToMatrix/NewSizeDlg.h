#pragma once
#include "afxwin.h"


// CNewSizeDlg dialog

class CNewSizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewSizeDlg)

public:
	CNewSizeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNewSizeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nWidth;
	int m_nHeight;
	CEdit m_EditWidth;
	CEdit m_EditHeight;
	afx_msg void OnBnClickedOk();
	
};
