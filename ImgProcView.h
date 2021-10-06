
// ImgProcView.h: CImgProcView 类的接口
//

#pragma once


class CImgProcView : public CView
{
protected: // 仅从序列化创建
	CImgProcView() noexcept;
	DECLARE_DYNCREATE(CImgProcView)

	BOOL m_bTracked = FALSE;	// 鼠标窗口跟踪判据

	void DisplayImage(CDC* pDC, int disp_xL, int disp_yL, int disp_Width, int disp_Height, int mode);
	void DisplayPalette();

// 特性
public:
	CImgProcDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImgProcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnMouseLeave();
	afx_msg void OnUpdateInspectionPalette(CCmdUI* pCmdUI);
	afx_msg void OnInspectionPalette();
	afx_msg void OnImageprocessingGetpixelvalue();
	afx_msg void OnImageprocessingSetpixelvalue();
	afx_msg void OnImageprocessingImageinterpolation();
	afx_msg void OnInspectionHistogram();
	afx_msg void OnUpdateInspectionHistogram(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // ImgProcView.cpp 中的调试版本
inline CImgProcDoc* CImgProcView::GetDocument() const
   { return reinterpret_cast<CImgProcDoc*>(m_pDocument); }
#endif

