
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


// CImgProcView

IMPLEMENT_DYNCREATE(CImgProcView, CView)

BEGIN_MESSAGE_MAP(CImgProcView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
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
