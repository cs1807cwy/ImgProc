// CGAUSSIANSMOOTHDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CGAUSSIANSMOOTHDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CGAUSSIANSMOOTHDLG 对话框

IMPLEMENT_DYNAMIC(CGAUSSIANSMOOTHDLG, CDialogEx)

CGAUSSIANSMOOTHDLG::CGAUSSIANSMOOTHDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAUSSIANSMOOTH, pParent)
{
	sigma = 0.0;
	radius = 1;
}

CGAUSSIANSMOOTHDLG::~CGAUSSIANSMOOTHDLG()
{
}

void CGAUSSIANSMOOTHDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAUSSIANSMOOTH_EDIT_SIGMA, m_edit_sigma);
	DDX_Control(pDX, IDC_GAUSSIANSMOOTH_STATIC_SIZE, m_title_size);
}


BEGIN_MESSAGE_MAP(CGAUSSIANSMOOTHDLG, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGAUSSIANSMOOTHDLG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGAUSSIANSMOOTHDLG::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_GAUSSIANSMOOTH_EDIT_SIGMA, &CGAUSSIANSMOOTHDLG::OnEnKillfocusGaussiansmoothEditSigma)
END_MESSAGE_MAP()


// CGAUSSIANSMOOTHDLG 消息处理程序


void CGAUSSIANSMOOTHDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// note: 处理可能的未写入更新
	this->OnEnKillfocusGaussiansmoothEditSigma();

	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	pDoc->ImageGaussianSmoothing(pApp->GetTransDoc(), this->sigma);
	pApp->ManualFileNew();

	CDialogEx::OnOK();
}


void CGAUSSIANSMOOTHDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CGAUSSIANSMOOTHDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->sigma = 1.0;
	this->radius = (DWORD)(3.0 * this->sigma) + 1;
	this->m_edit_sigma.SetWindowText(_T("1.0"));
	// note: 显示窗口大小
	this->m_title_size.SetWindowText(_T("7x7"));

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}


void CGAUSSIANSMOOTHDLG::OnEnKillfocusGaussiansmoothEditSigma()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_sigma.GetWindowText(sr);
	this->sigma = min(CGAUSSIANSMOOTHDLG::SIGMAMAX, abs(_ttof(sr)));
	this->radius = (DWORD)(3.0 * this->sigma) + 1;
	sr.Format(_T("%.2lf"), this->sigma);
	this->m_edit_sigma.SetWindowText(sr);
	// note: 显示窗口大小
	DWORD size = (this->radius << 1) - 1;
	sr.Format(_T("%lux%lu"), size, size);
	this->m_title_size.SetWindowText(sr);
}
