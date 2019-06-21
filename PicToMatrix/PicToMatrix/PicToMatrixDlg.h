
// PicToMatrixDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afximagepaintarea.h"
#include "EditImage.h"


// CPicToMatrixDlg dialog
class CPicToMatrixDlg : public CDialogEx
{
// Construction
public:
	CPicToMatrixDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PicToMatrix_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CBitmap m_Bitmap;
	CEditImage m_EditImage;

	CMFCColorBar m_wndColorBar;
	CStatic m_wndColorPickerLocation;
	
	CImage m_img;
	CString m_strRGBColorPicker;
	COLORREF m_Color;
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnClickedStaticColorpick();
	afx_msg void OnFileNewEmpty();
	afx_msg void OnDeviceMatrix();
};
