// CGETPIXELDLG.cpp: 实现文件
//

#include "pch.h"
#include "ImgProc.h"
#include "CGETPIXELDLG.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ImgProcDoc.h"


// CGETPIXELDLG 对话框

IMPLEMENT_DYNAMIC(CGETPIXELDLG, CDialogEx)

CStatic& CGETPIXELDLG::Pic(WORD x, WORD y)
{
	// TODO: 在此处插入 return 语句
	ASSERT(x >= 0 && x < 5);
	ASSERT(y >= 0 && y < 5);
	switch (y)
	{
	case 0: { switch (x) { case 0:return m_pic_00; case 1:return m_pic_01; case 2:return m_pic_02; case 3:return m_pic_03; case 4:return m_pic_04; default:break; }}
	case 1: { switch (x) { case 0:return m_pic_10; case 1:return m_pic_11; case 2:return m_pic_12; case 3:return m_pic_13; case 4:return m_pic_14; default:break; }}
	case 2: { switch (x) { case 0:return m_pic_20; case 1:return m_pic_21; case 2:return m_pic_22; case 3:return m_pic_23; case 4:return m_pic_24; default:break; }}
	case 3: { switch (x) { case 0:return m_pic_30; case 1:return m_pic_31; case 2:return m_pic_32; case 3:return m_pic_33; case 4:return m_pic_34; default:break; }}
	case 4: { switch (x) { case 0:return m_pic_40; case 1:return m_pic_41; case 2:return m_pic_42; case 3:return m_pic_43; case 4:return m_pic_44; default:break; }}
	}
	return m_pic_00;
}

CGETPIXELDLG::CGETPIXELDLG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GET_PIXEL, pParent), ctr_x(0), ctr_y(0)
{
}

CGETPIXELDLG::~CGETPIXELDLG()
{
}

void CGETPIXELDLG::FillInspectWnd(const std::vector<RGBQUAD>& clrMat5x5, RGBQUAD clrFrame, RGBQUAD clrFocusFrame, WORD focusWidth)
{
	for (int y = 0; y < 5; ++y)
	{
		for (int x = 0; x < 5; ++x)
		{
			CRect prect;
			Pic(x, y).GetClientRect(&prect);

			// note: 绘制像素色块
			if (clrMat5x5[y * 5 + x].rgbReserved == 0)
			{
				// 无效色块（超出图片边界）
				RGBQUAD CLR_CHG{ clrMat5x5[y * 5 + x].rgbBlue, clrMat5x5[y * 5 + x].rgbGreen, clrMat5x5[y * 5 + x].rgbRed,(BYTE)0 };	// 变换色
				int rectWidth = prect.Width();
				int rectHeight = prect.Height();

				// 16x16交替颜色填充
				for (int ty = 0; ty < 2; ++ty)
				{
					for (int tx = 0; tx < 2; ++tx)
					{
						CRect splitRect;
						splitRect.left = prect.left + rectWidth * tx / 2; splitRect.right = prect.left + rectWidth * (tx + 1) / 2;
						splitRect.top = prect.top + rectHeight * ty / 2; splitRect.bottom = prect.top + rectHeight * (ty + 1) / 2;

						FillRect(Pic(x, y).GetDC()->GetSafeHdc(), &splitRect,
							CBrush(RGB(CLR_CHG.rgbRed, CLR_CHG.rgbGreen, CLR_CHG.rgbBlue)));

						// note: 反色
						*(DWORD*)&CLR_CHG = ~*(DWORD*)&CLR_CHG;
						//CLR_CHG.rgbBlue = ~CLR_CHG.rgbBlue;
						//CLR_CHG.rgbGreen = ~CLR_CHG.rgbGreen;
						//CLR_CHG.rgbRed = ~CLR_CHG.rgbRed;
					}
					// note: 每行附加一次反色
					*(DWORD*)&CLR_CHG = ~*(DWORD*)&CLR_CHG;
				}
			}
			else
			{
				// 有效色块
				FillRect(Pic(x, y).GetDC()->GetSafeHdc(), &prect,
					CBrush(RGB(clrMat5x5[y * 5 + x].rgbRed, clrMat5x5[y * 5 + x].rgbGreen, clrMat5x5[y * 5 + x].rgbBlue)));
			}

			// note: 绘制分隔框
			if (x == 2 && y == 2)
			{
				// 中心框（厚线条框）
				CRect splitRect = prect;
				for (int i = 0; i < focusWidth; ++i)
				{
					FrameRect(Pic(x, y).GetDC()->GetSafeHdc(), &splitRect,
						CBrush(RGB(clrFocusFrame.rgbRed, clrFocusFrame.rgbGreen, clrFocusFrame.rgbBlue)));
					++splitRect.left; --splitRect.right;
					++splitRect.top; --splitRect.bottom;
				}
			}
			else
			{
				// 其他边框
				FrameRect(Pic(x, y).GetDC()->GetSafeHdc(), &prect,
					CBrush(RGB(clrFrame.rgbRed, clrFrame.rgbGreen, clrFrame.rgbBlue)));
			}
		}
	}
}

void CGETPIXELDLG::SetEditColorRGB(RGBQUAD color)
{
	CString strClr;
	strClr.Format(_T("%hu"), color.rgbRed);
	m_edit_r.SetWindowText(strClr);
	strClr.Format(_T("%hu"), color.rgbGreen);
	m_edit_g.SetWindowText(strClr);
	strClr.Format(_T("%hu"), color.rgbBlue);
	m_edit_b.SetWindowText(strClr);
}

void CGETPIXELDLG::SetEditColorNA()
{
	m_edit_r.SetWindowText(_T("NA"));
	m_edit_g.SetWindowText(_T("NA"));
	m_edit_b.SetWindowText(_T("NA"));
}

void CGETPIXELDLG::ProcCapture()
{
	CString str_x, str_y;
	LONG pos_x, pos_y;
	m_edit_x.GetWindowText(str_x);
	m_edit_y.GetWindowText(str_y);
	pos_x = _ttoi(str_x);
	pos_y = _ttoi(str_y);
	ctr_x = pos_x; ctr_y = pos_y;

	// note: 规格化坐标数字
	str_x.Format(_T("%ld"), pos_x);
	str_y.Format(_T("%ld"), pos_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	m_coordx.SetWindowText(str_x);
	m_coordy.SetWindowText(str_y);

	CImgProcDoc* pDoc = (CImgProcDoc*)((CMainFrame*)AfxGetMainWnd())->MDIGetActive()->GetActiveView()->GetDocument();
	LONG imgWidth = pDoc->GetImageWidth(), imgHeight = pDoc->GetImageHeight();

	// note: 设置5x5监视窗口
	std::vector<RGBQUAD> clrMat5x5;
	RGBQUAD CLR_TRANSPARENT{ (BYTE)255,(BYTE)255,(BYTE)255,(BYTE)0 };	// 透明色
	RGBQUAD CLR_GREY{ (BYTE)192,(BYTE)192,(BYTE)192,(BYTE)255 };	// 灰色分隔框
	RGBQUAD CLR_RED{ (BYTE)32,(BYTE)32,(BYTE)255,(BYTE)255 };	// 红色中心框
	for (int y = -2; y <= 2; ++y)
	{
		for (int x = -2; x <= 2; ++x)
		{
			if (pos_y + y >= 0 && pos_y + y < imgHeight && pos_x + x >= 0 && pos_x + x < imgWidth)
			{
				RGBQUAD tClr; pDoc->GetPixel(pos_x + x, pos_y + y, &tClr, nullptr);
				tClr.rgbReserved = (BYTE)255;
				clrMat5x5.push_back(tClr);
			}
			else
			{
				clrMat5x5.push_back(CLR_TRANSPARENT);
			}
		}
	}
	this->FillInspectWnd(clrMat5x5, CLR_GREY, CLR_RED, 3);

	// note: 设置RGB值文本显示
	if (pos_y >= 0 && pos_y < imgHeight && pos_x >= 0 && pos_x < imgWidth)
	{
		RGBQUAD tClr; pDoc->GetPixel(pos_x, pos_y, &tClr, nullptr);
		this->SetEditColorRGB(tClr);
	}
	else
	{
		this->SetEditColorNA();
	}
	//this->UpdateWindow();
}


void CGETPIXELDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCAPTURE, m_btn_capture);
	DDX_Control(pDX, IDRETURN, m_btn_return);
	DDX_Control(pDX, IDC_GETPIXEL_EDIT_X, m_edit_x);
	DDX_Control(pDX, IDC_GETPIXEL_EDIT_Y, m_edit_y);
	DDX_Control(pDX, IDC_GETPIXEL_EDIT_R, m_edit_r);
	DDX_Control(pDX, IDC_GETPIXEL_EDIT_G, m_edit_g);
	DDX_Control(pDX, IDC_GETPIXEL_EDIT_B, m_edit_b);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE1, m_pic_00);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE2, m_pic_01);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE3, m_pic_02);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE4, m_pic_03);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE5, m_pic_04);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE6, m_pic_10);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE7, m_pic_11);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE8, m_pic_12);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE9, m_pic_13);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE10, m_pic_14);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE11, m_pic_20);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE12, m_pic_21);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE13, m_pic_22);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE14, m_pic_23);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE15, m_pic_24);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE16, m_pic_30);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE17, m_pic_31);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE18, m_pic_32);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE19, m_pic_33);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE20, m_pic_34);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE21, m_pic_40);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE22, m_pic_41);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE23, m_pic_42);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE24, m_pic_43);
	DDX_Control(pDX, IDC_GETPIXEL_PICTURE25, m_pic_44);
	DDX_Control(pDX, IDC_GETPIXEL_STATIC_COORDX, m_coordx);
	DDX_Control(pDX, IDC_GETPIXEL_STATIC_COORDY, m_coordy);
}


BEGIN_MESSAGE_MAP(CGETPIXELDLG, CDialogEx)
	ON_BN_CLICKED(IDCAPTURE, &CGETPIXELDLG::OnBnClickedCapture)
	ON_BN_CLICKED(IDRETURN, &CGETPIXELDLG::OnBnClickedReturn)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE1, &CGETPIXELDLG::OnStnClickedGetpixelPicture1)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE2, &CGETPIXELDLG::OnStnClickedGetpixelPicture2)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE3, &CGETPIXELDLG::OnStnClickedGetpixelPicture3)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE4, &CGETPIXELDLG::OnStnClickedGetpixelPicture4)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE5, &CGETPIXELDLG::OnStnClickedGetpixelPicture5)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE6, &CGETPIXELDLG::OnStnClickedGetpixelPicture6)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE7, &CGETPIXELDLG::OnStnClickedGetpixelPicture7)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE8, &CGETPIXELDLG::OnStnClickedGetpixelPicture8)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE9, &CGETPIXELDLG::OnStnClickedGetpixelPicture9)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE10, &CGETPIXELDLG::OnStnClickedGetpixelPicture10)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE11, &CGETPIXELDLG::OnStnClickedGetpixelPicture11)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE12, &CGETPIXELDLG::OnStnClickedGetpixelPicture12)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE14, &CGETPIXELDLG::OnStnClickedGetpixelPicture14)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE13, &CGETPIXELDLG::OnStnClickedGetpixelPicture13)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE15, &CGETPIXELDLG::OnStnClickedGetpixelPicture15)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE16, &CGETPIXELDLG::OnStnClickedGetpixelPicture16)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE17, &CGETPIXELDLG::OnStnClickedGetpixelPicture17)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE18, &CGETPIXELDLG::OnStnClickedGetpixelPicture18)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE19, &CGETPIXELDLG::OnStnClickedGetpixelPicture19)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE20, &CGETPIXELDLG::OnStnClickedGetpixelPicture20)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE21, &CGETPIXELDLG::OnStnClickedGetpixelPicture21)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE22, &CGETPIXELDLG::OnStnClickedGetpixelPicture22)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE23, &CGETPIXELDLG::OnStnClickedGetpixelPicture23)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE24, &CGETPIXELDLG::OnStnClickedGetpixelPicture24)
	ON_STN_CLICKED(IDC_GETPIXEL_PICTURE25, &CGETPIXELDLG::OnStnClickedGetpixelPicture25)
END_MESSAGE_MAP()


// CGETPIXELDLG 消息处理程序


void CGETPIXELDLG::OnBnClickedCapture()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ProcCapture();
}


void CGETPIXELDLG::OnBnClickedReturn()
{
	// TODO: 在此添加控件通知处理程序代码
	this->EndDialog(0);
}



BOOL CGETPIXELDLG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ctr_x = ctr_y = 0;
	m_edit_x.SetWindowText(_T("0"));
	m_edit_y.SetWindowText(_T("0"));

	return TRUE;  // return TRUE unless you set the focus to a control
								// 异常: OCX 属性页应返回 FALSE
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture1()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x -= 2;
	this->ctr_y -= 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture2()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x -= 1;
	this->ctr_y -= 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture3()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_y -= 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture4()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x += 1;
	this->ctr_y -= 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture5()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x += 2;
	this->ctr_y -= 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture6()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x -= 2;
	this->ctr_y -= 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture7()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x -= 1;
	this->ctr_y -= 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture8()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_y -= 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture9()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x += 1;
	this->ctr_y -= 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture10()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x += 2;
	this->ctr_y -= 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture11()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x -= 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture12()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x -= 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture13()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture14()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x += 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture15()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x += 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture16()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x -= 2;
	this->ctr_y += 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture17()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x -= 1;
	this->ctr_y += 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture18()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_y += 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture19()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x += 1;
	this->ctr_y += 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture20()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x += 2;
	this->ctr_y += 1;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture21()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x -= 2;
	this->ctr_y += 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture22()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x -= 1;
	this->ctr_y += 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture23()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_y += 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture24()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x += 1;
	this->ctr_y += 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}


void CGETPIXELDLG::OnStnClickedGetpixelPicture25()
{
	// TODO: 在此添加控件通知处理程序代码
	this->ctr_x += 2;
	this->ctr_y += 2;
	CString str_x, str_y;
	str_x.Format(_T("%ld"), ctr_x);
	str_y.Format(_T("%ld"), ctr_y);
	m_edit_x.SetWindowText(str_x);
	m_edit_y.SetWindowText(str_y);
	this->ProcCapture();
}
