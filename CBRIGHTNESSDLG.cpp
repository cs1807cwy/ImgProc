// CBRIGHTNESSDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CBRIGHTNESSDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CBRIGHTNESSDLG 对话框

IMPLEMENT_DYNAMIC(CBRIGHTNESSDLG, CDialogEx)

CBRIGHTNESSDLG::CBRIGHTNESSDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BRIGHTNESS, pParent)
{
	brightnessModifier = 0;
	contrastMultiplier = 1.0;
}

CBRIGHTNESSDLG::~CBRIGHTNESSDLG()
{
}

void CBRIGHTNESSDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BRIGHTNESS_EDIT_BRIGHTNESS, m_edit_bright);
	DDX_Control(pDX, IDC_BRIGHTNESS_EDIT_CONTRAST, m_edit_contrast);
}


BEGIN_MESSAGE_MAP(CBRIGHTNESSDLG, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CBRIGHTNESSDLG::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CBRIGHTNESSDLG::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_BRIGHTNESS_EDIT_BRIGHTNESS, &CBRIGHTNESSDLG::OnEnKillfocusBrightnessEditBrightness)
	ON_EN_KILLFOCUS(IDC_BRIGHTNESS_EDIT_CONTRAST, &CBRIGHTNESSDLG::OnEnKillfocusBrightnessEditContrast)
END_MESSAGE_MAP()


// CBRIGHTNESSDLG 消息处理程序


void CBRIGHTNESSDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CBRIGHTNESSDLG::OnBnClickedOk()
{
	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	pDoc->BrightnessContrast(pApp->GetTransDoc(), this->brightnessModifier, this->contrastMultiplier);
	pApp->ManualFileNew();
}


BOOL CBRIGHTNESSDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->brightnessModifier = 0;
	this->contrastMultiplier = 1.0;
	CString sr;
	sr.Format(_T("%ld"), this->brightnessModifier);
	this->m_edit_bright.SetWindowText(sr);
	sr.Format(_T("%.2lf"), this->contrastMultiplier);
	this->m_edit_contrast.SetWindowText(sr);

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}


void CBRIGHTNESSDLG::OnEnKillfocusBrightnessEditBrightness()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_bright.GetWindowText(sr);
	this->brightnessModifier = min(CBRIGHTNESSDLG::BRIGHTNESSMODIFIERMAX,
		max(CBRIGHTNESSDLG::BRIGHTNESSMODIFIERMIN, (LONG)_ttoi(sr)));
	sr.Format(_T("%ld"), this->brightnessModifier);
	this->m_edit_bright.SetWindowText(sr);
}


void CBRIGHTNESSDLG::OnEnKillfocusBrightnessEditContrast()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_contrast.GetWindowText(sr);
	this->contrastMultiplier =  _ttof(sr);
	sr.Format(_T("%.2lf"), this->contrastMultiplier);
	this->m_edit_contrast.SetWindowText(sr);
}
