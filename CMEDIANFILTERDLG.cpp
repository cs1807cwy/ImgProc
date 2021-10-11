// CMEDIANFILTERDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CMEDIANFILTERDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"


// CMEDIANFILTERDLG 对话框

IMPLEMENT_DYNAMIC(CMEDIANFILTERDLG, CDialogEx)

CMEDIANFILTERDLG::CMEDIANFILTERDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MEDIANFILTER, pParent)
{
	radius = 1;
	centerWeight = 1.0;
}

CMEDIANFILTERDLG::~CMEDIANFILTERDLG()
{
}

void CMEDIANFILTERDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MEDIANFILTER_EDIT_WNDLEN, m_edit_radius);
	DDX_Control(pDX, IDC_MEDIANFILTER_EDIT_CENTERW, m_edit_weight);
	DDX_Control(pDX, IDC_MEDIANFILTER_STATIC_SIZE, m_title_size);
}


BEGIN_MESSAGE_MAP(CMEDIANFILTERDLG, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMEDIANFILTERDLG::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMEDIANFILTERDLG::OnBnClickedCancel)
	ON_EN_KILLFOCUS(IDC_MEDIANFILTER_EDIT_WNDLEN, &CMEDIANFILTERDLG::OnEnKillfocusMedianfilterEditWndlen)
	ON_EN_KILLFOCUS(IDC_MEDIANFILTER_EDIT_CENTERW, &CMEDIANFILTERDLG::OnEnKillfocusMedianfilterEditCenterw)
END_MESSAGE_MAP()


// CMEDIANFILTERDLG 消息处理程序


void CMEDIANFILTERDLG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	pDoc->ImageMedianFiltering(pApp->GetTransDoc(), this->radius, this->centerWeight);
	pApp->ManualFileNew();

	CDialogEx::OnOK();
}


void CMEDIANFILTERDLG::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CMEDIANFILTERDLG::OnEnKillfocusMedianfilterEditWndlen()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sr;
	this->m_edit_radius.GetWindowText(sr);
	this->radius = _ttoi(sr);
	this->radius = max(CMEDIANFILTERDLG::RADIUSMIN, min(CMEDIANFILTERDLG::RADIUSMAX, this->radius));
	sr.Format(_T("%lu"), this->radius);
	this->m_edit_radius.SetWindowText(sr);
	DWORD size = (this->radius << 1) - 1;
	sr.Format(_T("(%lux%lu)"), size, size);
	this->m_title_size.SetWindowText(sr);
}


void CMEDIANFILTERDLG::OnEnKillfocusMedianfilterEditCenterw()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sw;
	this->m_edit_weight.GetWindowText(sw);
	this->centerWeight = _ttof(sw);
	sw.Format(_T("%0.2lf"), this->centerWeight);
	this->m_edit_weight.SetWindowText(sw);
}


BOOL CMEDIANFILTERDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->m_edit_radius.SetWindowText(_T("2"));
	this->m_edit_weight.SetWindowText(_T("1.0"));
	this->m_title_size.SetWindowText(_T("(3x3)"));
	this->radius = 2;
	this->centerWeight = 1.0;
	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}
