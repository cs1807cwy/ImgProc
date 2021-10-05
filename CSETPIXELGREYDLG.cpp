// CSETPIXELGREYDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CSETPIXELGREYDLG.h"
#include "afxdialogex.h"
#include "ImgProcDoc.h"
#include "MainFrm.h"


// CSETPIXELGREYDLG 对话框

IMPLEMENT_DYNAMIC(CSETPIXELGREYDLG, CDialogEx)

void CSETPIXELGREYDLG::SychronizeColorToInspect()
{
	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	
	std::vector<RGBQUAD>& plt = pDoc->GetPalette();

	// note: 从Grey文本框获取Grey值
	CString strClr;
	m_edit_g.GetWindowText(strClr);
	selColor.rgbRed = selColor.rgbGreen = selColor.rgbBlue = selColor.rgbReserved = _ttoi(strClr);
	
	// note: 使用附属调色板
	if (plt.size() > 0)
	{
		size_t index = selColor.rgbReserved %= plt.size();
		selColor.rgbRed = plt[index].rgbRed;
		selColor.rgbGreen = plt[index].rgbGreen;
		selColor.rgbBlue = plt[index].rgbBlue;
	}

	// note: 至此处selColor.rgbReserved存储实际灰度值
	//
	// 设置ColorButton的值
	m_clr_btn.SetColor(RGB(selColor.rgbRed, selColor.rgbGreen, selColor.rgbBlue));

	// 重新设置Grey文本框（规范化）
	strClr.Format(_T("%hhu"), selColor.rgbReserved);
	m_edit_g.SetWindowText(strClr);

	// selColor重设为灰度值
	selColor.rgbRed = selColor.rgbGreen = selColor.rgbBlue = selColor.rgbReserved;
	selColor.rgbReserved = 255;
}

void CSETPIXELGREYDLG::SychronizeColorToEditor()
{
	// note: 读取ColorButton的值
	COLORREF clr = m_clr_btn.GetColor();
	selColor.rgbRed = GetRValue(clr);
	selColor.rgbGreen = GetGValue(clr);
	selColor.rgbBlue = GetBValue(clr);

	// note: 心理学经验公式 Gray = R * 0.299 + G * 0.587 + B * 0.114
	selColor.rgbRed = selColor.rgbGreen = selColor.rgbBlue = 
		(selColor.rgbRed * 38 + selColor.rgbGreen * 75 + selColor.rgbBlue * 15) >> 7;

	// 设置Grey文本框
	CString strClr;
	strClr.Format(_T("%hhu"), selColor.rgbRed);
	m_edit_g.SetWindowText(strClr);

}

CSETPIXELGREYDLG::CSETPIXELGREYDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SET_PIXEL_GREY, pParent)
{
	selColor.rgbRed = 0;
	selColor.rgbGreen = 0;
	selColor.rgbBlue = 0;
	selColor.rgbReserved = 0;
}

CSETPIXELGREYDLG::~CSETPIXELGREYDLG()
{
}

void CSETPIXELGREYDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETPIXELGREY_EDIT_X, m_edit_x);
	DDX_Control(pDX, IDC_SETPIXELGREY_EDIT_Y, m_edit_y);
	DDX_Control(pDX, IDC_SETPIXELGREY_EDIT_WIDTH, m_edit_w);
	DDX_Control(pDX, IDC_SETPIXELGREY_EDIT_HEIGHT, m_edit_h);
	DDX_Control(pDX, IDC_SETPIXELGREY_EDIT_G, m_edit_g);
	DDX_Control(pDX, IDC_MFCCOLORINSPECT, m_clr_btn);
	DDX_Control(pDX, IDC_RANGE_STATIC, m_title_range);
}


BEGIN_MESSAGE_MAP(CSETPIXELGREYDLG, CDialogEx)
	ON_BN_CLICKED(IDRETURN, &CSETPIXELGREYDLG::OnBnClickedReturn)
	ON_BN_CLICKED(IDAPPLY, &CSETPIXELGREYDLG::OnBnClickedApply)
	ON_EN_KILLFOCUS(IDC_SETPIXELGREY_EDIT_G, &CSETPIXELGREYDLG::OnEnKillfocusGetpixelEditG)
	ON_BN_CLICKED(IDC_MFCCOLORINSPECT, &CSETPIXELGREYDLG::OnBnClickedMfccolorinspect)
END_MESSAGE_MAP()


// CSETPIXELGREYDLG 消息处理程序


void CSETPIXELGREYDLG::OnBnClickedReturn()
{
	// TODO: 在此添加控件通知处理程序代码
	this->EndDialog(0);
}


void CSETPIXELGREYDLG::OnBnClickedApply()
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


void CSETPIXELGREYDLG::OnEnKillfocusGetpixelEditG()
{
	// TODO: 在此添加控件通知处理程序代码
	this->SychronizeColorToInspect();
}


BOOL CSETPIXELGREYDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_edit_g.SetWindowText(_T("0"));
	m_edit_w.SetWindowText(_T("1"));
	m_edit_h.SetWindowText(_T("1"));
	m_edit_x.SetWindowText(_T("0"));
	m_edit_y.SetWindowText(_T("0"));
	m_clr_btn.SetColor(RGB(0, 0, 0));

	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	CString range;
	DWORD cnt = pDoc->GetUsedColors(); 
	if (cnt == 0) { cnt = 256; }
	range.Format(_T("(0-%lu)"), --cnt);
	m_title_range.SetWindowText(range);

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}


void CSETPIXELGREYDLG::OnBnClickedMfccolorinspect()
{
	// TODO: 在此添加控件通知处理程序代码
	this->SychronizeColorToEditor();
	this->SychronizeColorToInspect();
}
