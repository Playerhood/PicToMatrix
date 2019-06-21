
// PicToMatrixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PicToMatrix.h"
#include "PicToMatrixDlg.h"
#include "afxdialogex.h"

#include "NewSizeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPicToMatrixDlg dialog

CPicToMatrixDlg::CPicToMatrixDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PicToMatrix_DIALOG, pParent)
	, m_strRGBColorPicker(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strRGBColorPicker = _T("255, 0, 0");
	m_Color = RGB(255, 0, 0);
}

void CPicToMatrixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_IMG, m_EditImage);
	DDX_Control(pDX, IDC_STATIC_COLORPICKER, m_wndColorPickerLocation);
	DDX_Text(pDX, IDC_STATIC_COLOR, m_strRGBColorPicker);
}

BEGIN_MESSAGE_MAP(CPicToMatrixDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, &CPicToMatrixDlg::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CPicToMatrixDlg::OnFileSave)
	ON_STN_CLICKED(IDC_STATIC_COLORPICKER, &CPicToMatrixDlg::OnClickedStaticColorpick)
	ON_COMMAND(ID_FILE_NEW_EMPTY, &CPicToMatrixDlg::OnFileNewEmpty)
	ON_COMMAND(ID_DEVICE_MATRIX, &CPicToMatrixDlg::OnDeviceMatrix)
END_MESSAGE_MAP()


// CPicToMatrixDlg message handlers

BOOL CPicToMatrixDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_Bitmap.LoadBitmap(IDB_BITMAP_BLANK);
	m_EditImage.SetBitmap(&m_Bitmap);
	m_EditImage.SetColor(m_Color);
	m_EditImage.SetMode(CEditImage::IMAGE_EDIT_MODE_PEN);

	// color bar
	CRect rectColorBar;
	m_wndColorPickerLocation.GetClientRect(&rectColorBar);
	m_wndColorPickerLocation.MapWindowPoints(this, &rectColorBar);
	rectColorBar.DeflateRect(1, 1);
	
	int nColumns = 10;
	m_wndColorBar.EnableOtherButton(_T("Other"));
	m_wndColorBar.EnableAutomaticButton(_T("auto"),RGB(255,0,0));
	m_wndColorBar.SetVertMargin(1);
	m_wndColorBar.SetHorzMargin(1);

	m_wndColorBar.CreateControl(this, rectColorBar, IDC_STATIC_COLORPICKER, nColumns,/* &pal*/NULL);
	m_wndColorBar.SetColor(RGB(0, 0, 0));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPicToMatrixDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPicToMatrixDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPicToMatrixDlg::OnFileOpen()
{
	// TODO: Add your command handler code here
	HRESULT hResult;
	CSimpleArray<GUID> aguidFileTypes;
	
	CString strFilter = _T("Bitmap image|*.bmp||");
	// un-mark below to enable all types CImage supports.
	//CString strFilter;
	//hResult = m_img.GetExporterFilterString(strFilter, aguidFileTypes);
	//if (FAILED(hResult)) {
	//	CString fmt;
	//	fmt.Format(L"GetExporterFilter failed:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
	//	::AfxMessageBox(fmt);
	//	return;
	//}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	hResult = (int)dlg.DoModal();
	if (FAILED(hResult)) {
		return;
	}

	if (hResult == IDCANCEL)
		return;

	m_img.Destroy();
	CString strPathName = dlg.GetPathName();
	hResult = m_img.Load(strPathName);
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format(L"Load image failed:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		return;
	}

	// avoid picture exceeding draw area.
	CRect rect;
	m_EditImage.GetClientRect(&rect);
	if (m_img.GetWidth() > rect.Width()  || m_img.GetHeight() > rect.Height())
	{
		AfxMessageBox(_T("file size is exceeding : %dx%d"), rect.Width(), rect.Height());
		m_img.Destroy();
		return;
	}

	// update m_Bitmap.
	HGDIOBJ htmp;
	htmp = m_Bitmap.Detach();
	m_Bitmap.Attach(m_img);
	m_EditImage.SetBitmap(&m_Bitmap);
	m_EditImage.Invalidate();
	DeleteObject(htmp);
}

void CPicToMatrixDlg::OnFileSave()
{
	// TODO: Add your command handler code here
	HRESULT hResult;
	CString strFilter = _T("Bitmap image|*.bmp||");

	CFileDialog dlg(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER, strFilter);
	hResult = (int)dlg.DoModal();
	if (FAILED(hResult)) {
		return;
	}

	if (hResult == IDCANCEL)
	{
		return;
	}

	// Add the appropriate extension if the user didn't type one
	CString strFileName;
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;
	switch (dlg.m_ofn.nFilterIndex)
	{
	case 1:
		strExtension = "bmp";
		break;
	case 2:
		strExtension = "jpg";
		break;
	case 3:
		strExtension = "gif";
		break;
	case 4:
		strExtension = "png";
		break;
	default:
		break;
	}
	strFileName = strFileName + '.' + strExtension;
	
	if(m_img.IsNull())
		m_img.Attach(m_Bitmap);

	// the extension on the file name will determine the file type that is saved
	hResult = m_img.Save(strFileName);
	if (FAILED(hResult)) {
		CString fmt;
		fmt.Format(L"Save image failed:\n%x - %s", hResult, _com_error(hResult).ErrorMessage());
		::AfxMessageBox(fmt);
		return;
	}
}


void CPicToMatrixDlg::OnClickedStaticColorpick()
{
	// TODO: Add your control notification handler code here
	m_Color = m_wndColorBar.GetColor();
	if (m_Color == RGB(192, 192, 192))
	{
		m_Color = GetGlobalData()->clrBtnFace;
	}

	m_strRGBColorPicker.Format(_T("(%d,%d,%d)"), GetRValue(m_Color), GetGValue(m_Color), GetBValue(m_Color));
	
	m_EditImage.SetColor(m_Color);

	UpdateData(FALSE);
}


void CPicToMatrixDlg::OnFileNewEmpty()
{
	// TODO: Add your command handler code here
	BITMAP bmp;
	m_Bitmap.GetBitmap(&bmp);

	CNewSizeDlg nsDlg;
	nsDlg.m_nWidth = bmp.bmWidth;
	nsDlg.m_nHeight = bmp.bmHeight;
	if (nsDlg.DoModal() != IDOK)
	{
		return;
	}

	CWindowDC dc(this);
	CDC 		memDC;
	memDC.CreateCompatibleDC(&dc);

	// create bitmap.
	BITMAPINFO bi;
	ZeroMemory(&bi, sizeof(BITMAPINFO));
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = nsDlg.m_nWidth;
	bi.bmiHeader.biHeight = nsDlg.m_nHeight;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	HBITMAP bitmap = ::CreateDIBSection(memDC, &bi, DIB_RGB_COLORS, NULL, NULL, 0);

	// update m_Bitmap.
	m_Bitmap.Detach();
	m_Bitmap.Attach(bitmap);

	// draw the background to easier viewing.
	CBitmap* pOldBitmap = memDC.SelectObject(&m_Bitmap);

	CRect rect(0, 0, nsDlg.m_nWidth, nsDlg.m_nHeight);
	memDC.FillRect(&rect, &(GetGlobalData()->brBtnFace));
	memDC.SelectObject(pOldBitmap);

	// update EditImage.
	m_EditImage.SetBitmap(&m_Bitmap);
	m_EditImage.Invalidate();
}

void CPicToMatrixDlg::OnDeviceMatrix()
{
	// TODO: Add your command handler code here
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	CBitmap* pOldBitmap = memDC.SelectObject(&m_Bitmap);

	BITMAP bm;
	m_Bitmap.GetBitmap(&bm);

	int nSize = bm.bmWidth*bm.bmHeight;

	CStdioFile matrixfile;
	TCHAR* pszFileName = _T("Data.txt");
	// do stuff that may cause exceptions
	CFileException ex;
	if (!matrixfile.Open(pszFileName, CFile::modeWrite | CFile::modeCreate, &ex))
	{
		ex.ReportError();
		matrixfile.Abort();   // close file safely and quietly
	}

	CString strData;
	BYTE bOne;
	for (int y = 0; y < bm.bmHeight; y++)
	{
		for (int x = 0; x < bm.bmWidth; x++)
		{
			COLORREF rgbPixel = memDC.GetPixel(x, y);

			// filter background color.
			// get Red value only here.
			if (rgbPixel == GetSysColor(COLOR_BTNFACE))
				rgbPixel = 0;

			if (rgbPixel != (COLORREF)-1)
			{
				bOne = GetRValue(rgbPixel);
			}

			strData.Format(_T("0x%02X,"), bOne);
			matrixfile.WriteString(strData);
		}
		matrixfile.WriteString(_T("\n"));
	}

	memDC.SelectObject(pOldBitmap);
}
