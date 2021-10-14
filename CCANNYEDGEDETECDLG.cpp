// CCANNYEDGEDETECDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CCANNYEDGEDETECDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CCANNYEDGEDETECDLG 对话框

IMPLEMENT_DYNAMIC(CCANNYEDGEDETECDLG, CDialogEx)

CCANNYEDGEDETECDLG::CCANNYEDGEDETECDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CANNYEDGEDETECTION, pParent)
{
	sigma = 1.0;
	thh = 100.0;
	thl = 40.0;
}

CCANNYEDGEDETECDLG::~CCANNYEDGEDETECDLG()
{
}

void CCANNYEDGEDETECDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CANNYEDGEDETECTION_EDIT_VARIANCE, m_edit_sigma);
	DDX_Control(pDX, IDC_CANNYEDGEDETECTION_EDIT_THH, m_edit_thh);
	DDX_Control(pDX, IDC_CANNYEDGEDETECTION_EDIT_THL, m_edit_thl);
}


BEGIN_MESSAGE_MAP(CCANNYEDGEDETECDLG, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CCANNYEDGEDETECDLG::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CCANNYEDGEDETECDLG::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_CANNYEDGEDETECTION_EDIT_VARIANCE, &CCANNYEDGEDETECDLG::OnEnKillfocusCannyedgedetectionEditVariance)
	ON_EN_KILLFOCUS(IDC_CANNYEDGEDETECTION_EDIT_THH, &CCANNYEDGEDETECDLG::OnEnKillfocusCannyedgedetectionEditThh)
	ON_EN_KILLFOCUS(IDC_CANNYEDGEDETECTION_EDIT_THL, &CCANNYEDGEDETECDLG::OnEnKillfocusCannyedgedetectionEditThl)
END_MESSAGE_MAP()


// CCANNYEDGEDETECDLG 消息处理程序


void CCANNYEDGEDETECDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CCANNYEDGEDETECDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	this->OnEnKillfocusCannyedgedetectionEditThh();
	this->OnEnKillfocusCannyedgedetectionEditThl();
	this->OnEnKillfocusCannyedgedetectionEditVariance();

	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	pDoc->ImageCannyEdgeDetection(pApp->GetTransDoc(), this->sigma, this->thl, this->thh);
	pApp->ManualFileNew();

	CDialogEx::OnOK();
}


void CCANNYEDGEDETECDLG::OnEnKillfocusCannyedgedetectionEditVariance()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_sigma.GetWindowText(sr);
	this->sigma = min(CCANNYEDGEDETECDLG::SIGMAMAX, abs(_ttof(sr)));
	sr.Format(_T("%.2lf"), this->sigma);
	this->m_edit_sigma.SetWindowText(sr);
}


void CCANNYEDGEDETECDLG::OnEnKillfocusCannyedgedetectionEditThh()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_thh.GetWindowText(sr);
	this->thh = min(CCANNYEDGEDETECDLG::THRESHMAX, max(this->thl, _ttof(sr)));
	sr.Format(_T("%.2lf"), this->thh);
	this->m_edit_thh.SetWindowText(sr);
}


void CCANNYEDGEDETECDLG::OnEnKillfocusCannyedgedetectionEditThl()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_thl.GetWindowText(sr);
	this->thl = min(this->thh, max(CCANNYEDGEDETECDLG::THRESHMIN, _ttof(sr)));
	sr.Format(_T("%.2lf"), this->thl);
	this->m_edit_thl.SetWindowText(sr);
}


BOOL CCANNYEDGEDETECDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString sr;
	this->sigma = 1.0;
	this->thh = 100.0;
	this->thl = 40.0;
	sr.Format(_T("%.2lf"), this->sigma);
	this->m_edit_sigma.SetWindowText(sr);
	sr.Format(_T("%.2lf"), this->thh);
	this->m_edit_thh.SetWindowText(sr);
	sr.Format(_T("%.2lf"), this->thl);
	this->m_edit_thl.SetWindowText(sr);

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}
