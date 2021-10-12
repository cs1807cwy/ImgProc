// CSHARPENINGDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CSHARPENINGDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CSHARPENINGDLG 对话框

IMPLEMENT_DYNAMIC(CSHARPENINGDLG, CDialogEx)

CSHARPENINGDLG::CSHARPENINGDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHARPENING, pParent)
{
	this->reserve = 1.0;
	this->enhance = 0.0;
	this->opt = OPERATOR::PIXELDIFF;
	m_split_opt.SetDropDownMenu(IDR_SHARPENING_OPERATOR_MENU, 0);
}

CSHARPENINGDLG::~CSHARPENINGDLG()
{
}

void CSHARPENINGDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHARPENING_EDIT_REV, m_edit_rev);
	DDX_Control(pDX, IDC_SHARPENING_EDIT_ENH, m_edit_enh);
	DDX_Control(pDX, IDC_SHARPENING_SPLIT_OPERATOR, m_split_opt);
}


BEGIN_MESSAGE_MAP(CSHARPENINGDLG, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CSHARPENINGDLG::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CSHARPENINGDLG::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_SHARPENING_EDIT_REV, &CSHARPENINGDLG::OnEnKillfocusSharpeningEditRev)
	ON_EN_KILLFOCUS(IDC_SHARPENING_EDIT_ENH, &CSHARPENINGDLG::OnEnKillfocusSharpeningEditEnh)
	ON_COMMAND(ID_OPERATOR_DIFFERENTIAL, &CSHARPENINGDLG::OnOperatorDifferential)
	ON_COMMAND(ID_OPERATOR_SOBEL, &CSHARPENINGDLG::OnOperatorSobel)
	ON_COMMAND(ID_OPERATOR_ROBERTS, &CSHARPENINGDLG::OnOperatorRoberts)
	ON_COMMAND(ID_OPERATOR_PREWITT, &CSHARPENINGDLG::OnOperatorPrewitt)
END_MESSAGE_MAP()


// CSHARPENINGDLG 消息处理程序


void CSHARPENINGDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CSHARPENINGDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// note: 处理可能的未写入更新
	this->OnEnKillfocusSharpeningEditEnh();
	this->OnEnKillfocusSharpeningEditRev();

	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	pDoc->ImageSharpening(pApp->GetTransDoc(),this->reserve,  this->enhance, this->OPTMATX[this->opt], this->OPTMATY[this->opt]);
	pApp->ManualFileNew();

	CDialogEx::OnOK();
}


void CSHARPENINGDLG::OnEnKillfocusSharpeningEditRev()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_rev.GetWindowText(sr);
	this->reserve = _ttof(sr);
	sr.Format(_T("%.2lf"), this->reserve);
	this->m_edit_rev.SetWindowText(sr);
}


void CSHARPENINGDLG::OnEnKillfocusSharpeningEditEnh()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_enh.GetWindowText(sr);
	this->enhance = _ttof(sr);
	sr.Format(_T("%.2lf"), this->enhance);
	this->m_edit_enh.SetWindowText(sr);
}


BOOL CSHARPENINGDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->reserve = 1.0;
	this->enhance = 0.0;
	this->opt = OPERATOR::PIXELDIFF;
	CString sr;
	sr.Format(_T("%.2lf"), this->reserve);
	this->m_edit_rev.SetWindowText(sr);
	sr.Format(_T("%.2lf"), this->enhance);
	this->m_edit_enh.SetWindowText(sr);
	this->m_split_opt.SetWindowText(OPTNAME[this->opt]);

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}


void CSHARPENINGDLG::OnOperatorDifferential()
{
	// TODO: 在此添加命令处理程序代码
	this->opt = OPERATOR::PIXELDIFF;
	this->m_split_opt.SetWindowText(OPTNAME[this->opt]);
}


void CSHARPENINGDLG::OnOperatorSobel()
{
	// TODO: 在此添加命令处理程序代码
	this->opt = OPERATOR::SOBEL;
	this->m_split_opt.SetWindowText(OPTNAME[this->opt]);
}


void CSHARPENINGDLG::OnOperatorRoberts()
{
	// TODO: 在此添加命令处理程序代码
	this->opt = OPERATOR::ROBERTS;
	this->m_split_opt.SetWindowText(OPTNAME[this->opt]);
}


void CSHARPENINGDLG::OnOperatorPrewitt()
{
	// TODO: 在此添加命令处理程序代码
	this->opt = OPERATOR::PREWITT;
	this->m_split_opt.SetWindowText(OPTNAME[this->opt]);
}
