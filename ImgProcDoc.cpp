
// ImgProcDoc.cpp: CImgProcDoc 类的实现
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

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "ImgProcView.h"
#include "MainFrm.h"
#include "ChildFrm.h"

// CImgProcDoc

IMPLEMENT_DYNCREATE(CImgProcDoc, CDocument)

BEGIN_MESSAGE_MAP(CImgProcDoc, CDocument)
END_MESSAGE_MAP()


// CImgProcDoc 构造/析构

CImgProcDoc::CImgProcDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	pFileBuf = nullptr;
}

CImgProcDoc::~CImgProcDoc()
{
	if (pFileBuf)
	{
		delete[] pFileBuf;
		pFileBuf = nullptr;
	}
}

BOOL CImgProcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	
	return TRUE;
}

BOOL CImgProcDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	pFileBuf = Utils::OpenBMPfile(lpszPathName);
	fileName = lpszPathName;
	UpdateAllViews(NULL);

	return TRUE;
}

BOOL CImgProcDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	LPCTSTR lpszFilter = "BMP Files (*.bmp)|*.bmp||";
	CFileDialog dlg(FALSE, NULL, NULL, OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT, lpszFilter, NULL);
	if (dlg.DoModal() != IDOK) return FALSE;
	CString tmps = dlg.GetPathName();
	if (tmps.Right(4) != ".bmp") { tmps += ".bmp"; }
	//TRACE1("OnFileSave() invoked -> %s\n", tmps);
	Utils::SaveDIB(this->pFileBuf, tmps);
	return TRUE;
}

void CImgProcDoc::OnCloseDocument()
{
	if (pFileBuf) 
	{
		delete[] pFileBuf; 
		pFileBuf = nullptr;
	}
	CDocument::OnCloseDocument();
}

// CImgProcDoc 序列化

void CImgProcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CImgProcDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CImgProcDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CImgProcDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImgProcDoc 诊断

#ifdef _DEBUG
void CImgProcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImgProcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CImgProcDoc 命令





CString CImgProcDoc::GetFilePath()
{
	return this->fileName;
}
