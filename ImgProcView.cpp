
// ImgProcView.cpp: CImgProcView 类的实现
//

#include "pch.h"
#include "framework.h"
#include "Utils.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImgProc.h"
#endif

#include "ImgProcDoc.h"
#include "ImgProcView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "ChildFrm.h"
#include "MainFrm.h"
#include "CGETPIXELDLG.h"
#include "CSETPIXELDLG.h"
#include "CSETPIXELGREYDLG.h"
#include "CINTERPOLATIONDLG.h"
#include "CMEDIANFILTERDLG.h"
#include "CGAUSSIANSMOOTHDLG.h"


// CImgProcView

IMPLEMENT_DYNCREATE(CImgProcView, CView)

BEGIN_MESSAGE_MAP(CImgProcView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	//ON_UPDATE_COMMAND_UI(ID_INSPECTION_PALETTE， &)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_MOUSELEAVE()
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_PALETTE, &CImgProcView::OnUpdateInspectionPalette)
	ON_COMMAND(ID_INSPECTION_PALETTE, &CImgProcView::OnInspectionPalette)
	ON_COMMAND(ID_IMAGEPROCESSING_GETPIXELVALUE, &CImgProcView::OnImageprocessingGetpixelvalue)
	ON_COMMAND(ID_IMAGEPROCESSING_SETPIXELVALUE, &CImgProcView::OnImageprocessingSetpixelvalue)
	ON_COMMAND(ID_IMAGEPROCESSING_IMAGEINTERPOLATION, &CImgProcView::OnImageprocessingImageinterpolation)
	ON_COMMAND(ID_INSPECTION_HISTOGRAM, &CImgProcView::OnInspectionHistogram)
	ON_UPDATE_COMMAND_UI(ID_INSPECTION_HISTOGRAM, &CImgProcView::OnUpdateInspectionHistogram)
	ON_UPDATE_COMMAND_UI(ID_IMAGEPROCESSING_MEDIANFILTERING, &CImgProcView::OnUpdateImageprocessingMedianfiltering)
	ON_UPDATE_COMMAND_UI(ID_IMAGEPROCESSING_GAUSSIANSMOOTHING, &CImgProcView::OnUpdateImageprocessingGaussiansmoothing)
	ON_COMMAND(ID_IMAGEPROCESSING_MEDIANFILTERING, &CImgProcView::OnImageprocessingMedianfiltering)
	ON_COMMAND(ID_IMAGEPROCESSING_GAUSSIANSMOOTHING, &CImgProcView::OnImageprocessingGaussiansmoothing)
	ON_UPDATE_COMMAND_UI(ID_IMAGEPROCESSING_HISTOGRAMEQUALIZATION, &CImgProcView::OnUpdateImageprocessingHistogramequalization)
	ON_COMMAND(ID_IMAGEPROCESSING_HISTOGRAMEQUALIZATION, &CImgProcView::OnImageprocessingHistogramequalization)
END_MESSAGE_MAP()

// CImgProcView 构造/析构

CImgProcView::CImgProcView() noexcept
{
	// TODO: 在此处添加构造代码

}

CImgProcView::~CImgProcView()
{
}

BOOL CImgProcView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}


void CImgProcView::DisplayImage(CDC* pDC, int disp_xL, int disp_yL, int disp_Width, int disp_Height, int mode)
{
	ASSERT(pDC != NULL);
	HDC hDC = pDC->GetSafeHdc();
	ASSERT(hDC != 0);

	CImgProcDoc* pDoc = GetDocument();
	//ASSERT(pDoc->pixelData.size() != 0);

	int nImageWidth = pDoc->GetImageWidth();
	int nImageHeight = pDoc->GetImageHeight();
	if (disp_Width <= 0 || disp_Height <= 0)
	{
		disp_Width = nImageWidth;
		disp_Height = nImageHeight;
	}

	CRect rect;
	CWnd* pWnd = pDC->GetWindow();
	pWnd->GetClientRect(&rect);
	disp_Width = min(disp_Width, rect.right - disp_xL);
	disp_Height = min(disp_Height, rect.bottom - disp_yL);

	BITMAPINFOHEADER* pBitmapInfo = pDoc->GetInfoHeader();
	std::vector<BYTE>& pDIBImageData = pDoc->GetImageBuf();

	// note: storage of BITMAPINFOHEADER & PALETTE
	char buf[40 + 256 * 4];	// note: 一定要连续填满40 + 256 * 4，确保bmiHeader和bmiColors连续
	BITMAPINFO* pBitsInfo = (BITMAPINFO*)buf;
	memcpy(&pBitsInfo->bmiHeader, pBitmapInfo, sizeof(BITMAPINFOHEADER));

	//TRACE2("size:%d, biclrused:%d\n", pDoc->GetPalette().size(), pBitmapInfo->biClrUsed);
	ASSERT(!pBitmapInfo->biClrUsed || pDoc->GetPalette().size() == pBitmapInfo->biClrUsed);

	if (pBitmapInfo->biClrUsed == 0)
	{
		// note: 使用默认256阶灰度调色板
		for (int c = 0; c < 256; c++)
		{
			(pBitsInfo->bmiColors[c]).rgbRed = (mode == 0 || mode == 1 ? c : 0);
			(pBitsInfo->bmiColors[c]).rgbGreen = (mode == 0 || mode == 2 ? c : 0);
			(pBitsInfo->bmiColors[c]).rgbBlue = (mode == 0 || mode == 3 ? c : 0);
			(pBitsInfo->bmiColors[c]).rgbReserved = 0;
		}
	}
	else
	{
		// note: 使用附属调色板
		std::vector<RGBQUAD>& plt = pDoc->GetPalette();
		for (size_t c = 0; c < plt.size(); c++)
		{
			(pBitsInfo->bmiColors[c]).rgbRed = (mode == 0 || mode == 1 ? plt[c].rgbRed : 0);
			(pBitsInfo->bmiColors[c]).rgbGreen = (mode == 0 || mode == 2 ? plt[c].rgbGreen : 0);
			(pBitsInfo->bmiColors[c]).rgbBlue = (mode == 0 || mode == 3 ? plt[c].rgbBlue : 0);
			(pBitsInfo->bmiColors[c]).rgbReserved = 0;
		}
	}

	SetStretchBltMode(hDC, COLORONCOLOR);
	StretchDIBits(hDC, disp_xL, disp_yL, disp_Width, disp_Height,
		0, 0, nImageWidth, nImageHeight,
		(char*)pDIBImageData.data(), pBitsInfo,
		DIB_RGB_COLORS, SRCCOPY);

// ! Bug
//BITMAPINFO bitsInfo;
//bitsInfo.bmiHeader = *pBitmapInfo;

//ASSERT(pDoc->GetPalette().size() == pBitmapInfo->biClrUsed);

//if (pBitmapInfo->biClrUsed == 0)
//{
//	// note: 使用默认256阶灰度调色板
//	RGBQUAD defaultPalette[256];
//	for (int c = 0; c < 256; c++)
//	{
//		(defaultPalette[c]).rgbRed = (mode == 0 || mode == 1 ? c : 0);
//		(defaultPalette[c]).rgbGreen = (mode == 0 || mode == 2 ? c : 0);
//		(defaultPalette[c]).rgbBlue = (mode == 0 || mode == 3 ? c : 0);
//		(defaultPalette[c]).rgbReserved = 0;
//	}

//	*(RGBQUAD**)&bitsInfo.bmiColors = defaultPalette;
//}
//else
//{
//	// note: 使用附属调色板
//	*(RGBQUAD**)&bitsInfo.bmiColors = pDoc->GetPalette().data();
//}

	return;
}

void CImgProcView::DisplayPalette()
{
	CString s_plt;
  std::vector<RGBQUAD>& plt = GetDocument()->GetPalette();
	if (plt.size() == 0)
	{
		AfxMessageBox(_T("Palette not found!"));
	}
	else
	{
		for (size_t i = 0; i < plt.size(); ++i)
		{
			s_plt.AppendFormat(_T("[%3llu]0X%06X\t"), i, *(unsigned int*)&plt[i]);
			if (i % 8 == 7) { s_plt.AppendChar('\n'); }
		}
		AfxMessageBox(s_plt);
	}
}

// 当视图建立后的第一帧更新
// 设置窗口绘图区大小与图片大小一致
void CImgProcView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	CImgProcDoc* pDoc = GetDocument();

	// note: 修正窗口位置、大小
	// 从ChildFrame获取窗口创建时使用的风格描述
	CREATESTRUCT createStyle = ((CChildFrame*)GetParentFrame())->GetWndStyle();
	CRect rect;
	rect.SetRect(0, 0, pDoc->GetImageWidth(), pDoc->GetImageHeight());	// 根据BMP文件修正右端、底端偏移
	// 根据窗体风格、用户区修正矩形，修正窗体
	AdjustWindowRectEx(&rect, createStyle.style, FALSE, createStyle.dwExStyle);
	// 窗体左上顶点修正到(0,0)
	rect.MoveToXY(0, 0);
	GetParentFrame()->MoveWindow(&rect);
}

// CImgProcView 绘图

void CImgProcView::OnDraw(CDC* pDC)
{
	CImgProcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	this->DisplayImage(pDC, 0, 0, 0, 0, 0);
}


// CImgProcView 打印

BOOL CImgProcView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImgProcView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImgProcView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CImgProcView 诊断

#ifdef _DEBUG
void CImgProcView::AssertValid() const
{
	CView::AssertValid();
}

void CImgProcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImgProcDoc* CImgProcView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImgProcDoc)));
	return (CImgProcDoc*)m_pDocument;
}
#endif //_DEBUG


// CImgProcView 消息处理程序


void CImgProcView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bTracked)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.dwHoverTime = 0;
		tme.hwndTrack = m_hWnd;
		TrackMouseEvent(&tme);	// 调用鼠标事件跟踪函数

		m_bTracked = TRUE;		// 标记鼠标进入窗口
	}

	// note: 跟踪鼠标位置像素信息
	CString str, title; CImgProcDoc* pDoc;
	RGBQUAD color; bool isGrey;
	pDoc = GetDocument();
	pDoc->GetPixel(point.x, point.y, &color, &isGrey);

	if (isGrey)
	{
		std::vector<RGBQUAD>& plt = pDoc->GetPalette();
		str.Format(_T("%s : Loc(%d,%d)-->Grey(0X%02X) map to RGB(0X%02X%02X%02X)"), pDoc->GetTitle(), point.x, point.y, color.rgbRed,
			plt[color.rgbRed].rgbRed, plt[color.rgbRed].rgbGreen, plt[color.rgbRed].rgbBlue);
	}
	else
	{
		str.Format(_T("%s : Loc(%d,%d)-->RGB(0X%02X%02X%02X)"), pDoc->GetTitle(), point.x, point.y, color.rgbRed, color.rgbGreen, color.rgbBlue);
	}
	
	CStatusBar* pStatus;
	pStatus = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);
	pStatus->SetPaneText(0, str);

	CView::OnMouseMove(nFlags, point);
}



void CImgProcView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	// 重设状态栏提示
	CStatusBar* pStatus; CString str;
	pStatus = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);
	str.LoadString(AFX_IDS_IDLEMESSAGE);
	pStatus->SetPaneText(0, str);
}


void CImgProcView::OnMouseLeave()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_bTracked = FALSE;		// 标记鼠标离开窗口

	// 重设状态栏提示
	CStatusBar* pStatus; CString str;
	pStatus = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);
	str.LoadString(AFX_IDS_IDLEMESSAGE);
	pStatus->SetPaneText(0, str);

	CView::OnMouseLeave();
}


void CImgProcView::OnUpdateInspectionPalette(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(GetDocument()->GetColorBits() == 8);
}


void CImgProcView::OnInspectionPalette()
{
	// TODO: 在此添加命令处理程序代码
	this->DisplayPalette();
}


void CImgProcView::OnImageprocessingGetpixelvalue()
{
	// TODO: 在此添加命令处理程序代码
	CGETPIXELDLG getPixelDlg;
	getPixelDlg.DoModal();
}


void CImgProcView::OnImageprocessingSetpixelvalue()
{
	// TODO: 在此添加命令处理程序代码
	CImgProcDoc* pDoc = GetDocument();
	if (pDoc->GetColorBits() == 24)
	{
		CSETPIXELDLG setPixelDlg;
		setPixelDlg.DoModal();
	}
	else if (pDoc->GetColorBits() == 8)
	{
		CSETPIXELGREYDLG setPixelGreyDlg;
		setPixelGreyDlg.DoModal();
	}
}



void CImgProcView::OnImageprocessingImageinterpolation()
{
	// TODO: 在此添加命令处理程序代码
	CINTERPOLATIONDLG interpolationDlg;
	interpolationDlg.DoModal();
}


void CImgProcView::OnInspectionHistogram()
{
	// TODO: 在此添加命令处理程序代码
	CImgProcDoc* pDoc = GetDocument();
	std::vector<DWORD> histogram = pDoc->GetHistogram();
	CString hinfo;
	for (size_t i = 0; i < histogram.size(); ++i)
	{
		hinfo.AppendFormat(_T("Val[%3llu] %-8lu"), i, histogram[i]);
		if (i % 8 == 7) { hinfo.AppendChar(_T('\n')); }
		else { hinfo.AppendChar(_T('\t')); }
	}
	AfxMessageBox(hinfo);
}


void CImgProcView::OnUpdateInspectionHistogram(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(GetDocument()->GetColorBits() == 8);
}


void CImgProcView::OnUpdateImageprocessingMedianfiltering(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(GetDocument()->GetColorBits() == 8);
}



void CImgProcView::OnImageprocessingMedianfiltering()
{
	// TODO: 在此添加命令处理程序代码
	CMEDIANFILTERDLG medianFilterDlg;
	medianFilterDlg.DoModal();
}


void CImgProcView::OnUpdateImageprocessingGaussiansmoothing(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(GetDocument()->GetColorBits() == 8);
}

void CImgProcView::OnImageprocessingGaussiansmoothing()
{
	// TODO: 在此添加命令处理程序代码
	CGAUSSIANSMOOTHDLG gaussianSmoothDlg;
	gaussianSmoothDlg.DoModal();
}


void CImgProcView::OnUpdateImageprocessingHistogramequalization(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(GetDocument()->GetColorBits() == 8);
}


void CImgProcView::OnImageprocessingHistogramequalization()
{
	// TODO: 在此添加命令处理程序代码
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	CImgProcDoc* pDoc = (CImgProcDoc*)this->GetDocument();
	pDoc->HistogramEqualization(pApp->GetTransDoc());
	pApp->ManualFileNew();
}
