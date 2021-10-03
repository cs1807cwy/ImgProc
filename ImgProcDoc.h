
// ImgProcDoc.h: CImgProcDoc 类的接口
//


#pragma once
#include "Utils.h"

class CImgProcDoc : public CDocument
{
protected: 
	// 仅从序列化创建
	CImgProcDoc() noexcept;
	DECLARE_DYNCREATE(CImgProcDoc)

// 特性
public:
	char* pFileBuf;
	CString fileName;
// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CImgProcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS

public:

	char* GetFileBuf() { return this->pFileBuf; }
	int GetWidth() { return Utils::GetImageWidth(this->pFileBuf); }
	int GetHeight() { return Utils::GetImageHeight(this->pFileBuf); }
	int GetNBytesPerRow() { return Utils::GetWidthBytes(this->pFileBuf); }
	int GetColorBits() { return Utils::GetColorBits(this->pFileBuf); }

	CString GetFilePath();
};
