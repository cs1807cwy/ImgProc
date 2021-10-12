// CADDIMPLUSENOISEDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CADDIMPLUSENOISEDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CADDIMPLUSENOISEDLG 对话框

IMPLEMENT_DYNAMIC(CADDIMPLUSENOISEDLG, CDialogEx)

CADDIMPLUSENOISEDLG::CADDIMPLUSENOISEDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADDIMPLUSENOISE, pParent)
{
	duty = 0.0;
	binaryzation = true;
	m_split_type.SetDropDownMenu(IDR_ADDIMPLUSENOISE_TYPE_MENU, 0);
}

CADDIMPLUSENOISEDLG::~CADDIMPLUSENOISEDLG()
{
}

void CADDIMPLUSENOISEDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDIMPULSENOISE_SPLIT_TYPE, m_split_type);
	DDX_Control(pDX, IDC_ADDIMPULSENOISE_EDIT_DUTY, m_edit_duty);
}


BEGIN_MESSAGE_MAP(CADDIMPLUSENOISEDLG, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CADDIMPLUSENOISEDLG::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CADDIMPLUSENOISEDLG::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_ADDIMPULSENOISE_EDIT_DUTY, &CADDIMPLUSENOISEDLG::OnEnKillfocusAddimpulsenoiseEditDuty)
	ON_COMMAND(ID_NOISETYPE_SALTPEPPER, &CADDIMPLUSENOISEDLG::OnNoisetypeSaltpepper)
	ON_COMMAND(ID_NOISETYPE_RANDOMIMPLUSENOISE, &CADDIMPLUSENOISEDLG::OnNoisetypeRandomimplusenoise)
END_MESSAGE_MAP()


// CADDIMPLUSENOISEDLG 消息处理程序


void CADDIMPLUSENOISEDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CADDIMPLUSENOISEDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	// note: 处理可能的未写入更新
	this->OnEnKillfocusAddimpulsenoiseEditDuty();

	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	pDoc->ImageAddImpluseNoise(pApp->GetTransDoc(), this->duty, this->binaryzation);
	pApp->ManualFileNew();

	CDialogEx::OnOK();
}


void CADDIMPLUSENOISEDLG::OnEnKillfocusAddimpulsenoiseEditDuty()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_duty.GetWindowText(sr);
	this->duty = min(CADDIMPLUSENOISEDLG::DUTYMAX, abs(_ttof(sr)));
	sr.Format(_T("%.1lf"), this->duty);
	this->m_edit_duty.SetWindowText(sr);
}


BOOL CADDIMPLUSENOISEDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->duty = 50.0;
	this->binaryzation = true;
	CString sr;
	sr.Format(_T("%.1lf"), this->duty);
	this->m_edit_duty.SetWindowText(sr);
	this->m_split_type.SetWindowText(CADDIMPLUSENOISEDLG::TYPENAME[(size_t)this->binaryzation]);

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}


void CADDIMPLUSENOISEDLG::OnNoisetypeSaltpepper()
{
	// TODO: 在此添加命令处理程序代码
	this->binaryzation = true;
	this->m_split_type.SetWindowText(CADDIMPLUSENOISEDLG::TYPENAME[(size_t)this->binaryzation]);
}


void CADDIMPLUSENOISEDLG::OnNoisetypeRandomimplusenoise()
{
	// TODO: 在此添加命令处理程序代码
	this->binaryzation = false;
	this->m_split_type.SetWindowText(CADDIMPLUSENOISEDLG::TYPENAME[(size_t)this->binaryzation]);
}
