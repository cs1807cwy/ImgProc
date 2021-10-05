
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
	ON_COMMAND(ID_IMAGEPROCESSING_SAVETONEWBMPFILE, &CImgProcDoc::OnImageprocessingSavetonewbmpfile)
	ON_COMMAND(ID_IMAGEPROCESSING_DISPPLAYFILEHEADER, &CImgProcDoc::OnImageprocessingDispplayfileheader)
END_MESSAGE_MAP()


// CImgProcDoc 构造/析构

CImgProcDoc::CImgProcDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	memset((char*)&this->bmpHead, 0, sizeof(BITMAPFILEHEADER));
	memset((char*)&this->bmpInfo, 0, sizeof(BITMAPINFOHEADER));
}

CImgProcDoc::~CImgProcDoc()
{
	memset((char*)&this->bmpHead, 0, sizeof(BITMAPFILEHEADER));
	memset((char*)&this->bmpInfo, 0, sizeof(BITMAPINFOHEADER));
}

// 保护方法

// note: 打开BMP文件
bool CImgProcDoc::OpenBMPfile(CString strBmpFile)
{
	CFile hFile;
	if (!hFile.Open(strBmpFile, CFile::modeRead | CFile::typeBinary))
	{
		AfxMessageBox(_T("Failed to open the BMP file"));
		return false;
	}

	long lFileSize = (long)hFile.Seek(0L, CFile::end);
	if (lFileSize < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER))
	{
		AfxMessageBox(_T("This isn't a valid BMP file"));
		return false;
	}

	hFile.Seek(0L, CFile::begin);
	hFile.Read((char*)&this->bmpHead, sizeof(BITMAPFILEHEADER));
	hFile.Read((char*)&this->bmpInfo, sizeof(BITMAPINFOHEADER));
	
	// note: 校验文件头
	if (
		bmpHead.bfType != 0x4D42 ||	//"BM"=0x424D
		bmpInfo.biSize != 0x28 ||		// 位图信息子结构长度(等于40,即0x28,sizeof(BITMAPINFOHEADER))
		bmpInfo.biPlanes != 0x01)		// 此域必须等于1
	{
		AfxMessageBox(_T("This isn't a valid BMP file"));
		return false;
	}
	
	// note: 校验压缩格式（不支持RGB压缩）
	if (this->bmpInfo.biCompression != BI_RGB)
	{
		AfxMessageBox(_T("This is a compressed BMP file"));
		return false;
	}
	
	// note: 校验是否为 8位、24位 颜色格式
	if (bmpInfo.biBitCount != 8 && bmpInfo.biBitCount != 24)
	{
		AfxMessageBox(_T("Only 8-bit and 24-bit BMP files are supported"));
		return false;
	}

	// note: 清除（可能存在的）已有数据
	this->palette.clear();
	this->pixelData.clear();

	if (bmpInfo.biBitCount == 8)
	{
		DWORD colorCnt = (bmpHead.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER)) >> 2;

		for (DWORD i = 0; i < colorCnt; ++i)
		{
			RGBQUAD color32;
			hFile.Read(&color32, 4);
			this->palette.push_back(color32);
		}
	}
	this->pixelData.resize(lFileSize - bmpHead.bfOffBits);
	hFile.Read(this->pixelData.data(), lFileSize - bmpHead.bfOffBits);
	hFile.Close();

	return true;
}

// note: 保存为BMP文件
bool CImgProcDoc::SaveDIB(CString strBmpFile)
{
	CFile hFile;
	if (!hFile.Open(strBmpFile, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		AfxMessageBox(_T("Failed to create the BMP file"));
		return false;
	}
	
	// note: 将BMP写入文件
	hFile.Write((char*)&this->bmpHead, sizeof(BITMAPFILEHEADER));
	hFile.Write((char*)&this->bmpInfo, sizeof(BITMAPINFOHEADER));
	hFile.Write((char*)this->palette.data(), this->palette.size() * sizeof(RGBQUAD));
	hFile.Write((char*)this->pixelData.data(), this->bmpHead.bfSize - this->bmpHead.bfOffBits);
	hFile.Close();

	return true;
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
	if (this->OpenBMPfile(lpszPathName) == false)
	{
		return FALSE;
	}
	fileName = lpszPathName;
	UpdateAllViews(NULL);

	return TRUE;
}

BOOL CImgProcDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	LPCTSTR lpszFilter = _T("BMP Files (*.bmp)|*.bmp||");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT, lpszFilter, NULL);
	if (dlg.DoModal() != IDOK) return FALSE;
	CString tmps = dlg.GetPathName();
	if (tmps.Right(4) != ".bmp") { tmps += ".bmp"; }
	//TRACE1("OnFileSave() invoked -> %s\n", tmps);
	return (int)this->SaveDIB(tmps);
}

void CImgProcDoc::OnCloseDocument()
{
	memset((char*)&this->bmpHead, 0, sizeof(BITMAPFILEHEADER));
	memset((char*)&this->bmpInfo, 0, sizeof(BITMAPINFOHEADER));
	this->palette.clear();
	this->pixelData.clear();
	this->fileName.Empty();
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

//   象素操作
//	 读象素颜色值
//	 返回: >=0 表示象素在位图数据中的偏移值
//				 <0 失败或参数无效
long CImgProcDoc::GetPixel(int x, int y, RGBQUAD rgb[1], bool bGray[1])
{
	int  nColorBits = GetColorBits();
	int  nImageHeight = GetImageHeight();
	int  nBytesPerRow = GetWidthBytes();

	long nOffInImage = (nImageHeight - 1 - y) * nBytesPerRow;

	if (bGray != NULL) *bGray = true;
	if (nColorBits == 8)
	{
		nOffInImage += x;
		rgb[0].rgbReserved = 0;
		rgb[0].rgbRed = this->pixelData[nOffInImage];
		rgb[0].rgbGreen = this->pixelData[nOffInImage];
		rgb[0].rgbBlue = this->pixelData[nOffInImage];
	}
	else if (nColorBits == 24)
	{
		if (bGray != NULL) *bGray = false;
		nOffInImage += 3 * x;
		rgb[0].rgbReserved = 0;
		rgb[0].rgbRed = this->pixelData[nOffInImage + 2];
		rgb[0].rgbGreen = this->pixelData[nOffInImage + 1];
		rgb[0].rgbBlue = this->pixelData[nOffInImage];
	}
	else
	{
		AfxMessageBox(_T("It is not an 8-bit or 24-bit image"));
		return(-1L);
	}

	return (nOffInImage);
}

//  设置像素(x,y)的颜色值
void CImgProcDoc::SetPixel(int x, int y, RGBQUAD rgb, int width, int height)
{
	int  nColorBits = GetColorBits();
	int  nImageHeight = GetImageHeight();
	int  nImageWidth = GetImageWidth();
	int  nBytesPerRow = GetWidthBytes();
	
	if (x < 0 || x >= nImageWidth ||
		y < 0 || y >= nImageHeight)
	{
		return;
	}

	int realHeight = min(height, nImageHeight - y);
	int realWidth = min(width, nImageWidth - x);

	if (nColorBits == 8)
	{
		for (int ty = y; ty < y + realHeight; ++ty)
		{
			for (int tx = x; tx < x + realWidth; ++tx)
			{
				long nOffInImage = (nImageHeight - 1 - ty) * nBytesPerRow;
				nOffInImage += tx;
				this->pixelData[nOffInImage] = rgb.rgbRed;
			}
		}
	}
	else if (nColorBits == 24)
	{
		for (int ty = y; ty < y + realHeight; ++ty)
		{
			for (int tx = x; tx < x + realWidth; ++tx)
			{
				long nOffInImage = (nImageHeight - 1 - ty) * nBytesPerRow;
				nOffInImage += (3 * tx);
				this->pixelData[nOffInImage] = rgb.rgbBlue;
				this->pixelData[nOffInImage + 1] = rgb.rgbGreen;
				this->pixelData[nOffInImage + 2] = rgb.rgbRed;
			}
		}
	}
	else
	{
		AfxMessageBox(_T("It is not an 8-bit or 24-bit image"));
	}

	return;
}



void CImgProcDoc::DisplayHeaderMessage()
{
	const LPCTSTR COMPRESSION[]{ _T("BI_RGB"), _T("BI_RLE8"), _T("BI_RLE4"), _T("BI_BITFIELDS"), _T("BI_JPEG"), _T("BI_PNG"), _T("UNKNOWN") };
	CString promptMsg;
	promptMsg.AppendFormat(_T("bfType (file type) = 0X%4.4X(\"%c%c\")\n"), this->bmpHead.bfType, ((char*)&this->bmpHead.bfType)[0], ((char*)&this->bmpHead.bfType)[1]);
	promptMsg.AppendFormat(_T("bfSize (file length) = 0X%X(%lu)\n"), this->bmpHead.bfSize, this->bmpHead.bfSize);
	promptMsg.AppendFormat(_T("bfOffBits (offset of bitmap data in bytes) = 0X%X(%lu)\n"), this->bmpHead.bfOffBits, this->bmpHead.bfOffBits);
	promptMsg.AppendFormat(_T("biSize (header structure length should be 0X28 or 40) = 0X%X(%lu)\n"), this->bmpInfo.biSize, this->bmpInfo.biSize);
	promptMsg.AppendFormat(_T("biWidth (image width) = 0X%X(%ld)\n"), this->bmpInfo.biWidth, this->bmpInfo.biWidth);
	promptMsg.AppendFormat(_T("biHeight (image height) = 0X%X(%ld)\n"), this->bmpInfo.biHeight, this->bmpInfo.biHeight);
	promptMsg.AppendFormat(_T("biPlanes (must be eaual to 1) = %u\n"), this->bmpInfo.biPlanes);
	promptMsg.AppendFormat(_T("biBitCount (color/pixel bits) = %u\n"), this->bmpInfo.biBitCount);
	promptMsg.AppendFormat(_T("biCompression (compressed?) = %lu(%s)\n"), this->bmpInfo.biCompression, COMPRESSION[min(6UL, this->bmpInfo.biCompression)]);
	promptMsg.AppendFormat(_T("biSizeImage (length of bitmap data in bytes must be the times of 4) = 0X%X(%lu)\n"), this->bmpInfo.biSizeImage, this->bmpInfo.biSizeImage);
	promptMsg.AppendFormat(_T("biXPelsPerMeter (horizontal resolution of target device in pixels/metre) = 0X%X(%ld)\n"), this->bmpInfo.biXPelsPerMeter, this->bmpInfo.biXPelsPerMeter);
	promptMsg.AppendFormat(_T("biYPelsPerMeter (vertical resolution of target device in pixels/metre) = 0X%X(%ld)\n"), this->bmpInfo.biYPelsPerMeter, this->bmpInfo.biYPelsPerMeter);
	promptMsg.AppendFormat(_T("biColorUsed (number of colors used in bitmap,0=2^biBitCount) = %lu\n"), this->bmpInfo.biClrUsed);
	promptMsg.AppendFormat(_T("biColorImportant (number of important colors,0=all colors are important) = %lu\n"), this->bmpInfo.biClrImportant);
	promptMsg.AppendFormat(_T("\nThe following is additional information:\n"));
	promptMsg.AppendFormat(_T("Bytes per row in bitmap (nBytesPerRow) = %ld\n"), this->GetWidthBytes());
	promptMsg.AppendFormat(_T("Total bytes of bitmap (nImageSizeInByte) = %ld\n"), this->GetWidthBytes() * this->GetImageHeight());
	promptMsg.AppendFormat(_T("Actual pixels per row in bitmap (nPixelsPerRow) = %ld\n"), this->GetImageWidth());
	promptMsg.AppendFormat(_T("Total rows of bitmap (nTotalRows) = %ld\n"), this->GetImageHeight());
	promptMsg.AppendFormat(_T("Total colors (2^biBitCount)(nTotalColors) = %d\n"), 1 << this->GetColorBits());
	promptMsg.AppendFormat(_T("Used colors (biColorUsed)(nUsedColors) = %lu"), this->GetUsedColors() ? this->GetUsedColors() : 1 << this->GetColorBits());

	AfxMessageBox(promptMsg);
}

void CImgProcDoc::OnImageprocessingSavetonewbmpfile()
{
	// TODO: 在此添加命令处理程序代码
	this->OnSaveDocument(NULL);
}


void CImgProcDoc::OnImageprocessingDispplayfileheader()
{
	// TODO: 在此添加命令处理程序代码
	this->DisplayHeaderMessage();
}
