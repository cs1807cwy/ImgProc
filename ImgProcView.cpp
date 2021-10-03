
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


// CImgProcView

IMPLEMENT_DYNCREATE(CImgProcView, CView)

BEGIN_MESSAGE_MAP(CImgProcView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_MOUSELEAVE()
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
	rect.SetRect(0, 0, pDoc->GetWidth(), pDoc->GetHeight());	// 根据BMP文件修正右端、底端偏移
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
	if (pDoc->pFileBuf != NULL)
	{
		Utils::DisplayImage(pDC, pDoc->pFileBuf, 0, 0, 0, 0, 0);
	}
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
	Utils::GetPixel(pDoc->GetFileBuf(), point.x, point.y, &color, &isGrey);

	if (isGrey)
	{
		str.Format("%s : Loc(%d,%d)-->Grey(%#X)", pDoc->GetTitle(), point.x, point.y, color.rgbRed);
	}
	else
	{
		str.Format("%s : Loc(%d,%d)-->RGB(%#X%X%X)", pDoc->GetTitle(), point.x, point.y, color.rgbRed, color.rgbGreen, color.rgbBlue);
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
