
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
#include "SrColorQuant.h"

// CImgProcDoc

IMPLEMENT_DYNCREATE(CImgProcDoc, CDocument)

BEGIN_MESSAGE_MAP(CImgProcDoc, CDocument)
	ON_COMMAND(ID_IMAGEPROCESSING_SAVETONEWBMPFILE, &CImgProcDoc::OnImageprocessingSavetonewbmpfile)
	ON_COMMAND(ID_FILE_SAVE, &CImgProcDoc::OnImageprocessingSavetonewbmpfile)
	ON_COMMAND(ID_IMAGEPROCESSING_DISPPLAYFILEHEADER, &CImgProcDoc::OnImageprocessingDispplayfileheader)
	ON_UPDATE_COMMAND_UI(ID_TRANSFER_INDEXCOLOR, &CImgProcDoc::OnUpdateTransferIndexcolor)
	ON_UPDATE_COMMAND_UI(ID_TRANSFER_RGB24, &CImgProcDoc::OnUpdateTransferRgb24)
	ON_COMMAND(ID_TRANSFER_RGB24, &CImgProcDoc::OnTransferRgb24)
	ON_COMMAND(ID_TRANSFER_INDEXCOLOR, &CImgProcDoc::OnTransferIndexcolor)
	ON_COMMAND(ID_TRANSFER_GREY, &CImgProcDoc::OnTransferGrey)
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

	DWORD lFileSize = (DWORD)hFile.Seek(0L, CFile::end);
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
	*this = ((CImgProcApp*)AfxGetApp())->GetTransDoc();

	return TRUE;
}

CImgProcDoc::CImgProcDoc(const CImgProcDoc& doc):
	bmpHead(doc.bmpHead), bmpInfo(doc.bmpInfo),
	palette(doc.palette), pixelData(doc.pixelData), fileName(doc.fileName)
{
}

CImgProcDoc& CImgProcDoc::operator=(const CImgProcDoc& doc)
{
	// TODO: 在此处插入 return 语句
	this->bmpHead = doc.bmpHead;
	this->bmpInfo = doc.bmpInfo;
	this->palette = doc.palette;
	this->pixelData = doc.pixelData;
	this->fileName = doc.fileName;
	return (*this);
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
	//TRACE0("OnFileSave() invoked <-\n");
	LPCTSTR lpszFilter = _T("BMP Files (*.bmp)|*.bmp||");
	CFileDialog dlg(FALSE, NULL, NULL, OFN_NOCHANGEDIR | OFN_OVERWRITEPROMPT, lpszFilter, NULL);
	if (dlg.DoModal() != IDOK) return TRUE;
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

void CImgProcDoc::Clear()
{
	memset((char*)&this->bmpHead, 0, sizeof(BITMAPFILEHEADER));
	memset((char*)&this->bmpInfo, 0, sizeof(BITMAPINFOHEADER));
	this->palette.clear();
	this->pixelData.clear();
	this->fileName.Empty();
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

bool CImgProcDoc::IsDefaultPalette()
{
	if (this->palette.size() == 0) { return true; }
	if (this->palette.size() != 256) { return false; }
	for (size_t i = 0; i < this->palette.size(); ++i)
	{
		if (this->palette[i].rgbRed != i
			|| this->palette[i].rgbGreen != i
			|| this->palette[i].rgbBlue != i)
		{
			return false;
		}
	}
	return true;
}

void CImgProcDoc::DisplayPalette(std::vector<RGBQUAD>& plt)
{
	CString s_plt;
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

RGBQUAD CImgProcDoc::MapColor(const RGBQUAD& rgb)
{
	if (this->GetColorBits() == 8 && this->palette.size() > 0)
	{
		return this->palette[rgb.rgbRed];
	}
	return rgb;
}

RGBQUAD CImgProcDoc::MapRGB24ToGrey(const RGBQUAD& rgb)
{
	RGBQUAD ans;
	// note: 心理学经验公式 Gray = R * 0.299 + G * 0.587 + B * 0.114
	ans.rgbRed = ans.rgbGreen = ans.rgbBlue =
		(rgb.rgbRed * 38 + rgb.rgbGreen * 75 + rgb.rgbBlue * 15) >> 7;
	ans.rgbReserved = 0;
	return ans;
}

int CImgProcDoc::MapRGB24ToPalette(const RGBQUAD& rgb, std::vector<RGBQUAD>& plt)
{
	// note: 遍历调色板，取最邻近颜色值
	DWORD minDist = ~0UL;
	int val = 0;
	for (size_t i = 0; i < plt.size(); ++i)
	{
		DWORD dist = ((long)rgb.rgbRed - plt[i].rgbRed) * ((long)rgb.rgbRed - plt[i].rgbRed) +
			((long)rgb.rgbGreen - plt[i].rgbGreen) * ((long)rgb.rgbGreen - plt[i].rgbGreen) +
			((long)rgb.rgbBlue - plt[i].rgbBlue) * ((long)rgb.rgbBlue - plt[i].rgbBlue);
		if (dist < minDist)
		{
			minDist = dist;
			val = (int)i;
		}
	}
	return val;
}


//   象素操作
//	 读象素颜色值： 灰度值（R=G=B) 或 RGB值
//   对 灰度值 不进行颜色索引
//	 返回: >=0 表示象素在位图数据中的偏移值
//				 <0 失败或参数无效
long CImgProcDoc::GetPixel(LONG x, LONG y, RGBQUAD rgb[1], bool bGray[1])
{
	WORD  nColorBits = GetColorBits();
	LONG  nImageHeight = GetImageHeight();
	LONG  nBytesPerRow = GetWidthBytes();

	LONG nOffInImage = (nImageHeight - 1 - y) * nBytesPerRow;

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
		nOffInImage += 3L * x;
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
void CImgProcDoc::SetPixel(LONG x, LONG y, RGBQUAD rgb, int width, int height)
{
	WORD  nColorBits = GetColorBits();
	LONG  nImageHeight = GetImageHeight();
	LONG  nImageWidth = GetImageWidth();
	LONG  nBytesPerRow = GetWidthBytes();
	
	if (x < 0 || x >= nImageWidth ||
		y < 0 || y >= nImageHeight)
	{
		return;
	}

	LONG realHeight = min(height, nImageHeight - y);
	LONG realWidth = min(width, nImageWidth - x);

	if (nColorBits == 8)
	{
		for (LONG ty = y; ty < y + realHeight; ++ty)
		{
			for (LONG tx = x; tx < x + realWidth; ++tx)
			{
				LONG nOffInImage = (nImageHeight - 1 - ty) * nBytesPerRow;
				nOffInImage += tx;
				this->pixelData[nOffInImage] = rgb.rgbRed;
			}
		}
	}
	else if (nColorBits == 24)
	{
		for (LONG ty = y; ty < y + realHeight; ++ty)
		{
			for (LONG tx = x; tx < x + realWidth; ++tx)
			{
				LONG nOffInImage = (nImageHeight - 1 - ty) * nBytesPerRow;
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

// note: 仅对灰度图像生效
std::vector<DWORD> CImgProcDoc::GetHistogram()
{
	std::vector<DWORD> gram;
	if (this->GetColorBits() != 8L) { return gram; }

	DWORD clrCnt = this->palette.size();
	if (clrCnt == 0) { clrCnt = this->GetUsedColors(); }
	if (clrCnt == 0) { clrCnt = 256UL; }
	gram.resize(clrCnt, 0);
	for (auto c : this->pixelData) { ++gram[c]; }
	return gram;
}

// note: 编码转换 ( 灰度/RGB24/索引颜色 )
//
// note: 灰度（8-bits）
bool CImgProcDoc::CodingGrey(CImgProcDoc& newDoc)
{
	ASSERT(&newDoc != this);
	newDoc.Clear();
	// note: 在本软件中只处理 8位索引颜色/RGB24 到 灰度 的转换
	WORD clrBits = this->GetColorBits();
	if (clrBits != 8 && clrBits != 24)
	{
		AfxMessageBox(_T("BMP format not supported"));
		newDoc = *this;
		return false;
	}

	// note: newDoc使用默认256阶灰度调色板
	newDoc.palette.resize(256);
	for (int c = 0; c < 256; c++)
	{
		newDoc.palette[c] = RGBQUAD{ (unsigned char)c,(unsigned char)c,(unsigned char)c,0 };
	}

	// note: 基本规格
	LONG orgWidth = this->bmpInfo.biWidth;
	LONG orgHeight = this->bmpInfo.biHeight;
	LONG bytesPerRow = 4 * ((orgWidth * 8 + 31) / 32);
	LONG newImageSize = bytesPerRow * orgHeight;

	// BITMAPFILEHEADER
	newDoc.bmpHead.bfType = this->bmpHead.bfType;
	newDoc.bmpHead.bfSize = newImageSize + newDoc.palette.size() * sizeof(RGBQUAD)
		+ sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	newDoc.bmpHead.bfOffBits = newDoc.bmpHead.bfSize - newImageSize;
	// BITMAPINFOHEADER
	newDoc.bmpInfo = this->bmpInfo;
	newDoc.bmpInfo.biBitCount = 8;
	newDoc.bmpInfo.biSizeImage = newImageSize;
	newDoc.bmpInfo.biClrUsed = 256;
	newDoc.bmpInfo.biClrImportant = 256;
	// fill pixel
	newDoc.pixelData.resize(newDoc.bmpInfo.biSizeImage);
	for (LONG y = 0; y < this->bmpInfo.biHeight; ++y)
	{
		for (LONG x = 0; x < this->bmpInfo.biWidth; ++x)
		{
			RGBQUAD rgb;
			this->GetPixel(x, y, &rgb);
			// note: 8位索引颜色从调色板映射，RGB 24位色直接转换
			rgb = CImgProcDoc::MapRGB24ToGrey(clrBits == 8 ? this->MapColor(rgb) : rgb);
			newDoc.SetPixel(x, y, rgb);
		}
	}
	return true;
}

// note: RGB24编码
bool CImgProcDoc::CodingRGB24(CImgProcDoc& newDoc)
{
	ASSERT(&newDoc != this);
	newDoc.Clear();
	if (this->GetColorBits() == 24)
	{
		newDoc = *this;
		return true;
	}

	// note: 在本软件中只处理 索引颜色（包括灰度） 到 RGB24 的转换
	if (this->GetColorBits() != 8)
	{
		AfxMessageBox(_T("BMP format not supported"));
		newDoc = *this;
		return false;
	}
	// note: 处理 索引颜色 到 RGB24 的转换
	LONG orgWidth = this->bmpInfo.biWidth;
	LONG orgHeight = this->bmpInfo.biHeight;

	LONG bytesPerRow = 4 * ((orgWidth * 24 + 31) / 32);
	LONG newImageSize = bytesPerRow * orgHeight;

	newDoc.bmpHead = this->bmpHead;
	newDoc.bmpHead.bfSize = newImageSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	newDoc.bmpHead.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	newDoc.bmpInfo = this->bmpInfo;
	newDoc.bmpInfo.biSizeImage = newImageSize;
	newDoc.bmpInfo.biBitCount = 24;
	newDoc.bmpInfo.biClrUsed = 0;
	newDoc.bmpInfo.biClrImportant = 0;

	//newDoc.DisplayHeaderMessage();

	newDoc.pixelData.resize(newImageSize, 0);

	for (LONG y = 0; y < orgHeight; ++y)
	{
		for (LONG x = 0; x < orgWidth; ++x)
		{
			RGBQUAD rgb;
			this->GetPixel(x, y, &rgb);
			rgb = this->MapColor(rgb);
			newDoc.SetPixel(x, y, rgb);
		}
	}

	return true;
}

bool CImgProcDoc::CodingIndexColor(CImgProcDoc& newDoc)
{
	ASSERT(&newDoc != this);
	newDoc.Clear();
	if (this->GetColorBits() == 8)
	{
		newDoc = *this;
		return true;
	}

	// note: 在本软件中只处理 索引颜色（包括灰度） 到 RGB24 的转换
	if (this->GetColorBits() != 24)
	{
		AfxMessageBox(_T("BMP format not supported"));
		newDoc = *this;
		return false;
	}
	// note: 基本规格
	LONG orgWidth = this->bmpInfo.biWidth;
	LONG orgHeight = this->bmpInfo.biHeight;
	LONG bytesPerRow = 4 * ((orgWidth * 8 + 31) / 32);
	LONG newImageSize = bytesPerRow * orgHeight;

	// BITMAPFILEHEADER
	newDoc.bmpHead.bfType = this->bmpHead.bfType;

	// BITMAPINFOHEADER
	newDoc.bmpInfo = this->bmpInfo;
	newDoc.bmpInfo.biBitCount = 8;
	newDoc.bmpInfo.biSizeImage = newImageSize;

	// 八叉树颜色量化（生成调色板）
	SrColorQuant octree;
	LONG pixelCnt = orgWidth * orgHeight * 3;
	int clrCnt = 0;
	unsigned char* pBuf = new unsigned char[pixelCnt * 3];
	for (LONG y = 0; y < orgHeight; ++y)
	{
		for (LONG x = 0; x < orgWidth; ++x)
		{
			RGBQUAD rgb;
			this->GetPixel(x, y, &rgb);
			pBuf[3 * (y * orgWidth + x)] = rgb.rgbRed;
			pBuf[3 * (y * orgWidth + x) + 1] = rgb.rgbGreen;
			pBuf[3 * (y * orgWidth + x) + 2] = rgb.rgbBlue;
		}
	}
	clrCnt = octree.buildOctree(pBuf, pixelCnt, 256);
	ASSERT(clrCnt == octree.getLeafNodeCount());
	delete[] pBuf; pBuf = nullptr;
	size_t pltLen = static_cast<size_t>(octree.getLeafNodeCount()) * 3;
	unsigned char* plt = new unsigned char[pltLen];
	octree.getColorPallette(plt);
	for (size_t i = 0; i < pltLen; i += 3)
	{
		newDoc.palette.push_back(RGBQUAD{ plt[i + 2],plt[i + 1],plt[i],0 });
	}
	delete[] plt; plt = nullptr;
	// note: 量化结束

#ifdef _DEBUG
	CImgProcDoc::DisplayPalette(newDoc.palette);
	newDoc.DisplayHeaderMessage();
#endif // END _DEBUG

	// BITMAPFILEHEADER 体积偏移量参数
	newDoc.bmpHead.bfSize = newImageSize + newDoc.palette.size() * sizeof(RGBQUAD)
		+ sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	newDoc.bmpHead.bfOffBits = newDoc.bmpHead.bfSize - newImageSize;

	// BITMAPINFOHEADER 调色板参数
	newDoc.bmpInfo.biClrUsed = clrCnt;
	newDoc.bmpInfo.biClrImportant = clrCnt;

	// fill pixel
	newDoc.pixelData.resize(newDoc.bmpInfo.biSizeImage);
	for (LONG y = 0; y < this->bmpInfo.biHeight; ++y)
	{
		for (LONG x = 0; x < this->bmpInfo.biWidth; ++x)
		{
			RGBQUAD rgb;
			this->GetPixel(x, y, &rgb);
			unsigned char greyVal = (unsigned char)CImgProcDoc::MapRGB24ToPalette(rgb, newDoc.palette);
			newDoc.SetPixel(x, y, RGBQUAD{ greyVal,greyVal,greyVal,0 });
		}
	}

	return true;
}



// 图像插值
/**
	 功能: 图像插值
			 nMethod  插值算法
					0 = 最临近插值法
					1 = (双)线性插值法
	 返回: 新图像的BMP文件缓冲区首地址
					 NULL 表示失败（内存不足）
**/
void CImgProcDoc::ImageInterpolation(CImgProcDoc& newDoc, double factor_w, double factor_h, INTERPOLATION_MODE nMethod)
{
	switch (nMethod)
	{
	//最临近插值法
	case INTERPOLATION_MODE::DEFAULT: case INTERPOLATION_MODE::NEAREST:
	{
		LONG orgWidth = this->bmpInfo.biWidth;
		LONG orgHeight = this->bmpInfo.biHeight;
		LONG colorBits = this->bmpInfo.biBitCount;
		LONG newWidth = (LONG)((double)orgWidth * factor_w + 0.5);
		LONG newHeight = (LONG)((double)orgHeight * factor_h + 0.5);
		LONG bytesPerRow = 4 * ((newWidth * colorBits + 31) / 32);
		LONG newImageSize = bytesPerRow * newHeight;

		// note: BITMAPFILEHEADER
		newDoc.bmpHead = this->bmpHead;
		newDoc.bmpHead.bfSize = newImageSize + this->bmpHead.bfOffBits;

		// note: BITMAPINFOHEADER
		newDoc.bmpInfo = this->bmpInfo;
		newDoc.bmpInfo.biWidth = newWidth;
		newDoc.bmpInfo.biHeight = newHeight;
		newDoc.bmpInfo.biSizeImage = bytesPerRow * newHeight;
		newDoc.palette = this->palette;
		newDoc.pixelData.resize(newImageSize);

		// note: fill pixel
		for (LONG y = 0; y < newHeight; ++y)
		{
			double fy = (double)y / factor_h;
			for (LONG x = 0; x < newWidth; ++x)
			{
				RGBQUAD rgb{ 0,0,0,0 };
				double fx = (double)x / factor_w;

				LONG xx = min((LONG)(fx + 0.5), orgWidth - 1);
				LONG yy = min((LONG)(fy + 0.5), orgHeight - 1);
				this->GetPixel(xx, yy, &rgb);
				newDoc.SetPixel(x, y, rgb);
			}
		}
		break;
	}
	//(双)线性插值法
	case INTERPOLATION_MODE::BILINEAR:
	{
		CImgProcDoc tmpDoc24, tmpNewDoc;
		CImgProcDoc* srcDoc = this, * dstDoc = &newDoc;
		bool transit = false; // 标记是否使用中转文档
		if (this->GetColorBits() == 8 && this->GetPalette().size() > 0 && !this->IsDefaultPalette())
		{
			// note: 如果是索引颜色，先转换为RGB24编码进行双线性插值
			this->CodingRGB24(tmpDoc24);
			srcDoc = &tmpDoc24;
			dstDoc = &tmpNewDoc;
			transit = true;
		}
		LONG orgWidth = srcDoc->bmpInfo.biWidth;
		LONG orgHeight = srcDoc->bmpInfo.biHeight;
		LONG colorBits = srcDoc->bmpInfo.biBitCount;
		LONG newWidth = (LONG)((double)orgWidth * factor_w + 0.5);
		LONG newHeight = (LONG)((double)orgHeight * factor_h + 0.5);
		LONG bytesPerRow = 4 * ((newWidth * colorBits + 31) / 32);
		LONG newImageSize = bytesPerRow * newHeight;

		// note: BITMAPFILEHEADER
		dstDoc->bmpHead = srcDoc->bmpHead;
		dstDoc->bmpHead.bfSize = newImageSize + srcDoc->bmpHead.bfOffBits;

		// note: BITMAPINFOHEADER
		dstDoc->bmpInfo = srcDoc->bmpInfo;
		dstDoc->bmpInfo.biWidth = newWidth;
		dstDoc->bmpInfo.biHeight = newHeight;
		dstDoc->bmpInfo.biSizeImage = bytesPerRow * newHeight;
		dstDoc->palette = srcDoc->palette;
		dstDoc->pixelData.resize(newImageSize);

		// note: fill pixel
		for (LONG y = 0; y < newHeight; ++y)
		{
			double fy = (double)y / factor_h;
			for (LONG x = 0; x < newWidth; ++x)
			{
				RGBQUAD rgb{ 0,0,0,0 };
				double fx = (double)x / factor_w;

				RGBQUAD rgbLT, rgbRT, rgbLB, rgbRB;
				LONG   x1 = (LONG)fx;
				LONG   x2 = min(x1 + 1L, orgWidth - 1L);
				double dx = fx - (double)x1;
				LONG   y1 = (LONG)fy;
				LONG   y2 = min(y1 + 1L, orgHeight - 1L);
				double dy = (double)fy - (double)y1;
				srcDoc->GetPixel(x1, y1, &rgbLT);
				srcDoc->GetPixel(x2, y1, &rgbRT);
				srcDoc->GetPixel(x1, y2, &rgbLB);
				srcDoc->GetPixel(x2, y2, &rgbRB);
				for (int N = 0; N < 4; N++)
				{
					double v1 = ((BYTE*)&rgbLT)[N] + dy * (((BYTE*)&rgbLB)[N] - ((BYTE*)&rgbLT)[N]);
					double v2 = ((BYTE*)&rgbRT)[N] + dy * (((BYTE*)&rgbRB)[N] - ((BYTE*)&rgbRT)[N]);
					((BYTE*)&rgb)[N] = (int)(v1 + dx * (v2 - v1) + 0.5);
				}
				dstDoc->SetPixel(x, y, rgb);
			}
		}

		// ntoe: 如果使用中转文档处理，则重新转换为索引颜色图像
		if (transit) { dstDoc->CodingIndexColor(newDoc); }
		break;
	}
	default: break;
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


void CImgProcDoc::OnUpdateTransferIndexcolor(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(GetColorBits() != 8);
}


void CImgProcDoc::OnUpdateTransferRgb24(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->Enable(GetColorBits() != 24);
}


void CImgProcDoc::OnTransferRgb24()
{
	// TODO: 在此添加命令处理程序代码
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	this->CodingRGB24(pApp->GetTransDoc());
	pApp->ManualFileNew();
}


void CImgProcDoc::OnTransferIndexcolor()
{
	// TODO: 在此添加命令处理程序代码
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	this->CodingIndexColor(pApp->GetTransDoc());
	pApp->ManualFileNew();
}


void CImgProcDoc::OnTransferGrey()
{
	// TODO: 在此添加命令处理程序代码
	CImgProcApp* pApp = (CImgProcApp*)AfxGetApp();
	this->CodingGrey(pApp->GetTransDoc());
	pApp->ManualFileNew();
}
