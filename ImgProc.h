
// ImgProc.h: ImgProc 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号
#include "ImgProcDoc.h"

// CImgProcApp:
// 有关此类的实现，请参阅 ImgProc.cpp
//

class CImgProcApp : public CWinApp
{
	
public:
	CImgProcApp() noexcept;
	CImgProcDoc transDoc;

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void ManualFileNew();
	CImgProcDoc& GetTransDoc() { return this->transDoc; }

// 实现
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnImageprocessingOpenbmpfile();
};

extern CImgProcApp theApp;
