// CINTERPOLATIONDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CINTERPOLATIONDLG.h"
#include "afxdialogex.h"


// CINTERPOLATIONDLG 对话框

IMPLEMENT_DYNAMIC(CINTERPOLATIONDLG, CDialogEx)

CINTERPOLATIONDLG::CINTERPOLATIONDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INTERPOLATION, pParent)
{

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
//	ON_BN_CLICKED(IDCANCEL, &CINTERPOLATIONDLG::OnBnClickedCancel)
ON_BN_CLICKED(IDCANCEL, &CINTERPOLATIONDLG::OnBnClickedCancel)
ON_BN_CLICKED(IDOK, &CINTERPOLATIONDLG::OnBnClickedOk)
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


	CDialogEx::OnOK();
}
