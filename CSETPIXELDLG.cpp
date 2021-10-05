// CSETPIXELDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CSETPIXELDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ImgProcDoc.h"


// CSETPIXELDLG 对话框

IMPLEMENT_DYNAMIC(CSETPIXELDLG, CDialogEx)

CSETPIXELDLG::CSETPIXELDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SET_PIXEL, pParent)
{
	selColor.rgbRed = 0;
	selColor.rgbGreen = 0;
	selColor.rgbBlue = 0;
	selColor.rgbReserved = 0;
}

CSETPIXELDLG::~CSETPIXELDLG()
{
}

void CSETPIXELDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETPIXEL_EDIT_X, m_edit_x);
	DDX_Control(pDX, IDC_SETPIXEL_EDIT_Y, m_edit_y);
	DDX_Control(pDX, IDC_SETPIXEL_EDIT_WIDTH, m_edit_w);
	DDX_Control(pDX, IDC_SETPIXEL_EDIT_HEIGHT, m_edit_h);
	DDX_Control(pDX, IDC_SETPIXEL_EDIT_R, m_edit_r);
	DDX_Control(pDX, IDC_SETPIXEL_EDIT_G, m_edit_g);
	DDX_Control(pDX, IDC_SETPIXEL_EDIT_B, m_edit_b);
	DDX_Control(pDX, IDC_MFCCOLORINSPECT, m_clr_btn);
}


BEGIN_MESSAGE_MAP(CSETPIXELDLG, CDialogEx)
	ON_BN_CLICKED(IDRETURN, &CSETPIXELDLG::OnBnClickedReturn)
	ON_BN_CLICKED(IDAPPLY, &CSETPIXELDLG::OnBnClickedApply)
	ON_EN_KILLFOCUS(IDC_SETPIXEL_EDIT_R, &CSETPIXELDLG::OnEnKillfocusGetpixelEditR)
	ON_EN_KILLFOCUS(IDC_SETPIXEL_EDIT_G, &CSETPIXELDLG::OnEnKillfocusGetpixelEditG)
	ON_EN_KILLFOCUS(IDC_SETPIXEL_EDIT_B, &CSETPIXELDLG::OnEnKillfocusGetpixelEditB)
	ON_BN_CLICKED(IDC_MFCCOLORINSPECT, &CSETPIXELDLG::OnBnClickedMfccolorinspect)
END_MESSAGE_MAP()


// CSETPIXELDLG 消息处理程序


void CSETPIXELDLG::OnBnClickedReturn()
{
	// TODO: 在此添加控件通知处理程序代码
	this->EndDialog(0);
}


void CSETPIXELDLG::OnBnClickedApply()
{
	// TODO: 在此添加控件通知处理程序代码
	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();

	CString str_x, str_y, str_w, str_h;
	m_edit_x.GetWindowText(str_x);
	m_edit_y.GetWindowText(str_y);
	m_edit_w.GetWindowText(str_w);
	m_edit_h.GetWindowText(str_h);
	int beg_x = _ttoi(str_x),
		beg_y = _ttoi(str_y),
		width = min(_ttoi(str_w), pDoc->GetImageWidth() - beg_x),
		height = min(_ttoi(str_h), pDoc->GetImageHeight() - beg_y);

	pDoc->SetPixel(beg_x, beg_y, selColor, width, height);

	pDoc->UpdateAllViews(NULL);
}

void CSETPIXELDLG::SychronizeColorToInspect()
{
	// note: 从RGB文本框获取RGB值
	CString strClr;
	m_edit_r.GetWindowText(strClr);
	selColor.rgbRed = _ttoi(strClr);
	m_edit_g.GetWindowText(strClr);
	selColor.rgbGreen = _ttoi(strClr);
	m_edit_b.GetWindowText(strClr);
	selColor.rgbBlue = _ttoi(strClr);
	// 设置ColorButton的值
	m_clr_btn.SetColor(RGB(selColor.rgbRed, selColor.rgbGreen, selColor.rgbBlue));
}

void CSETPIXELDLG::SychronizeColorToEditor()
{
	// note: 读取ColorButton的值
	COLORREF clr = m_clr_btn.GetColor();
	selColor.rgbRed = GetRValue(clr);
	selColor.rgbGreen = GetGValue(clr);
	selColor.rgbBlue = GetBValue(clr);

	// 设置RGB文本框
	CString strClr;
	strClr.Format(_T("%hhu"), selColor.rgbRed);
	m_edit_r.SetWindowText(strClr);
	strClr.Format(_T("%hhu"), selColor.rgbGreen);
	m_edit_g.SetWindowText(strClr);
	strClr.Format(_T("%hhu"), selColor.rgbBlue);
	m_edit_b.SetWindowText(strClr);
}

void CSETPIXELDLG::OnEnKillfocusGetpixelEditR()
{
	// TODO: 在此添加控件通知处理程序代码
	this->SychronizeColorToInspect();
	this->SychronizeColorToEditor();
}


void CSETPIXELDLG::OnEnKillfocusGetpixelEditG()
{
	// TODO: 在此添加控件通知处理程序代码
	this->SychronizeColorToInspect();
	this->SychronizeColorToEditor();
}


void CSETPIXELDLG::OnEnKillfocusGetpixelEditB()
{
	// TODO: 在此添加控件通知处理程序代码
	// note: 从RGB文本框获取RGB值
	this->SychronizeColorToInspect();
	this->SychronizeColorToEditor();
}


void CSETPIXELDLG::OnBnClickedMfccolorinspect()
{
	// TODO: 在此添加控件通知处理程序代码
	this->SychronizeColorToEditor();
}


BOOL CSETPIXELDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_edit_b.SetWindowText(_T("0"));
	m_edit_g.SetWindowText(_T("0"));
	m_edit_r.SetWindowText(_T("0"));
	m_edit_w.SetWindowText(_T("1"));
	m_edit_h.SetWindowText(_T("1"));
	m_edit_x.SetWindowText(_T("0"));
	m_edit_y.SetWindowText(_T("0"));
	m_clr_btn.SetColor(RGB(0, 0, 0));

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}
