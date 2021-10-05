
// ChildFrm.cpp: CChildFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "ImgProc.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CChildFrame 构造/析构

CChildFrame::CChildFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.x = 0;	// 窗体左上角对齐
	cs.y = 0;
	cs.style &= ~WS_MINIMIZEBOX;	// 禁止窗口最小化
	cs.style &= ~WS_MAXIMIZEBOX;	// 禁止窗口最大化
	cs.style &= ~WS_THICKFRAME;		// 禁止修改窗体大小
	this->m_wndStyle = cs;
	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 消息处理程序


LRESULT CChildFrame::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect movableRect, wndRect;
	GetWindowRect(&wndRect);
	GetClientRect(&movableRect);
	ClientToScreen(&movableRect);
	movableRect.left = wndRect.left;
	movableRect.right = wndRect.right;
	movableRect.bottom = wndRect.bottom;

	return movableRect.PtInRect(point) ? HTCAPTION : CMDIChildWnd::OnNcHitTest(point);
}
