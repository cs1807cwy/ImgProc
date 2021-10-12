// CBILATERALFILTERDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CBILATERALFILTERDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CBILATERALFILTERDLG 对话框

IMPLEMENT_DYNAMIC(CBILATERALFILTERDLG, CDialogEx)

CBILATERALFILTERDLG::CBILATERALFILTERDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BILATERALFILTER, pParent)
{
	sigma_d = sigma_r = 0.0;
}

CBILATERALFILTERDLG::~CBILATERALFILTERDLG()
{
}

void CBILATERALFILTERDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BIFILTER_EDIT_SIGMAD, m_edit_sigmad);
	DDX_Control(pDX, IDC_BIFILTER_EDIT_SIGMAR, m_edit_sigmar);
}


BEGIN_MESSAGE_MAP(CBILATERALFILTERDLG, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CBILATERALFILTERDLG::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CBILATERALFILTERDLG::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_BIFILTER_EDIT_SIGMAD, &CBILATERALFILTERDLG::OnEnKillfocusBifilterEditSigmad)
	ON_EN_KILLFOCUS(IDC_BIFILTER_EDIT_SIGMAR, &CBILATERALFILTERDLG::OnEnKillfocusBifilterEditSigmar)
END_MESSAGE_MAP()


// CBILATERALFILTERDLG 消息处理程序


void CBILATERALFILTERDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CBILATERALFILTERDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// note: 处理可能的未写入更新
	this->OnEnKillfocusBifilterEditSigmad();
	this->OnEnKillfocusBifilterEditSigmar();

	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	pDoc->ImageBilateralFiltering(pApp->GetTransDoc(), this->sigma_d, this->sigma_r);
	pApp->ManualFileNew();

	CDialogEx::OnOK();
}


void CBILATERALFILTERDLG::OnEnKillfocusBifilterEditSigmad()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_sigmad.GetWindowText(sr);
	this->sigma_d = min(CBILATERALFILTERDLG::SIGMADMAX, abs(_ttof(sr)));
	sr.Format(_T("%.2lf"), this->sigma_d);
	this->m_edit_sigmad.SetWindowText(sr);
}


void CBILATERALFILTERDLG::OnEnKillfocusBifilterEditSigmar()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_sigmar.GetWindowText(sr);
	this->sigma_r = _ttof(sr);
	sr.Format(_T("%.2lf"), this->sigma_r);
	this->m_edit_sigmar.SetWindowText(sr);
}


BOOL CBILATERALFILTERDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	sigma_d = sigma_r = 1.0;
	CString sr;
	sr.Format(_T("%.2lf"), this->sigma_d);
	this->m_edit_sigmad.SetWindowText(sr);
	sr.Format(_T("%.2lf"), this->sigma_r);
	this->m_edit_sigmar.SetWindowText(sr);

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}
