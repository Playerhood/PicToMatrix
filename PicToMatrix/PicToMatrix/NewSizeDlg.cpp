// NewSize.cpp : implementation file
//

#include "stdafx.h"
#include "PicToMatrix.h"
#include "NewSizeDlg.h"
#include "afxdialogex.h"


// CNewSizeDlg dialog

IMPLEMENT_DYNAMIC(CNewSizeDlg, CDialogEx)

CNewSizeDlg::CNewSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SIZE, pParent)
{
	m_nWidth = 0;
	m_nHeight = 0;
}

CNewSizeDlg::~CNewSizeDlg()
{
}

void CNewSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_EditWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_EditHeight);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
}


BEGIN_MESSAGE_MAP(CNewSizeDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNewSizeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CNewSizeDlg message handlers


void CNewSizeDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CDialogEx::OnOK();
}
