// CINTERPOLATIONDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CINTERPOLATIONDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CINTERPOLATIONDLG 对话框

IMPLEMENT_DYNAMIC(CINTERPOLATIONDLG, CDialogEx)

CINTERPOLATIONDLG::CINTERPOLATIONDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INTERPOLATION, pParent)
{
	modeSel = CINTERPOLATIONDLG::MODE::NEAREST;
	m_split_sel.SetDropDownMenu(IDR_INTERPOLATION_MODE_MENU, 0);
}

CINTERPOLATIONDLG::~CINTERPOLATIONDLG()
{
}

void CINTERPOLATIONDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INTERPOLATION_SPLIT_MODESEL, m_split_sel);
	DDX_Control(pDX, IDC_INTERPOLATION_EDIT_FX, m_edit_fx);
	DDX_Control(pDX, IDC_INTERPOLATION_EDIT_FY, m_edit_fy);
}


BEGIN_MESSAGE_MAP(CINTERPOLATIONDLG, CDialogEx)
ON_BN_CLICKED(IDCANCEL, &CINTERPOLATIONDLG::OnBnClickedCancel)
ON_BN_CLICKED(IDOK, &CINTERPOLATIONDLG::OnBnClickedOk)
ON_COMMAND(ID_MODE_NEAREST, &CINTERPOLATIONDLG::OnModeNearest)
ON_COMMAND(ID_MODE_BI, &CINTERPOLATIONDLG::OnModeBi)
END_MESSAGE_MAP()


// CINTERPOLATIONDLG 消息处理程序



void CINTERPOLATIONDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CINTERPOLATIONDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFx, strFy;
	m_edit_fx.GetWindowText(strFx);
	m_edit_fy.GetWindowText(strFy);
	// note: 缩放因子转浮点值
	double fx = _ttof(strFx);
	double fy = _ttof(strFy);

	AfxMessageBox(_T("DLG"));

	if (fx <= 0.0 || isnan(fx) || isinf(fx)
		|| fy <= 0.0 || isnan(fy) || isinf(fy))
	{
		AfxMessageBox(_T("Invalid Factor(s)"));
		m_edit_fx.SetWindowText(_T("1.0"));
		m_edit_fy.SetWindowText(_T("1.0"));
	}
	else
	{
		CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
		CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
		pApp->GetTransDoc() = *pDoc;



		pApp->ManualFileNew();


		CDialogEx::OnOK();
	}


}


void CINTERPOLATIONDLG::OnModeNearest()
{
	// TODO: 在此添加命令处理程序代码
	this->modeSel = CINTERPOLATIONDLG::MODE::NEAREST;
	this->m_split_sel.SetWindowText(CINTERPOLATIONDLG::MODENAME[MODE::NEAREST]);
}


void CINTERPOLATIONDLG::OnModeBi()
{
	// TODO: 在此添加命令处理程序代码
	this->modeSel = CINTERPOLATIONDLG::MODE::BILINEAR;
	this->m_split_sel.SetWindowText(CINTERPOLATIONDLG::MODENAME[MODE::BILINEAR]);
}


BOOL CINTERPOLATIONDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_edit_fx.SetWindowText(_T("1.0"));
	m_edit_fy.SetWindowText(_T("1.0"));
	this->m_split_sel.SetWindowText(CINTERPOLATIONDLG::MODENAME[MODE::NEAREST]);
	this->modeSel = CINTERPOLATIONDLG::MODE::NEAREST;

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}
