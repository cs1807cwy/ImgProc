
// ImgProcDoc.h: CImgProcDoc 类的接口
//


#pragma once
#include "Utils.h"
#include <vector>

class CImgProcDoc : public CDocument
{
//protected: 
public:
	// 仅从序列化创建
	CImgProcDoc() noexcept;
	DECLARE_DYNCREATE(CImgProcDoc)

// 特性
public:
	BITMAPFILEHEADER bmpHead;
	BITMAPINFOHEADER bmpInfo;
	std::vector<RGBQUAD> palette; // note: B G R A
	std::vector<BYTE> pixelData;
	CString fileName;
// 操作
public:
	CImgProcDoc(const CImgProcDoc& doc);
	CImgProcDoc& operator=(const CImgProcDoc& doc);

protected:
	bool OpenBMPfile(CString strBmpFile);
	bool SaveDIB(CString strBmpFile);
	void DisplayHeaderMessage();


// 重写
public:
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	virtual void Serialize(CArchive& ar);

	void Clear();

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

	BITMAPFILEHEADER* GetFileHeader() { return &this->bmpHead; }
	BITMAPINFOHEADER* GetInfoHeader() { return &this->bmpInfo; }
	std::vector<RGBQUAD>& GetPalette() { return this->palette; }
	bool IsDefaultPalette();
	static void DisplayPalette(std::vector<RGBQUAD>& plt);

	std::vector<BYTE>& GetImageBuf() { return this->pixelData; }
	LONG GetImageWidth() { return this->bmpInfo.biWidth; }
	LONG GetImageHeight() { return this->bmpInfo.biHeight; }
	LONG GetWidthBytes()
	{
		LONG nBytesPerRow = 4L * ((this->bmpInfo.biWidth * this->bmpInfo.biBitCount + 31L) / 32L);
		return (nBytesPerRow);
	}
	WORD GetColorBits() { return this->bmpInfo.biBitCount; }
	DWORD GetUsedColors() { return this->bmpInfo.biClrUsed; }

	CString GetFilePath() { return this->fileName; }

	RGBQUAD MapColor(const RGBQUAD& rgb);
	static RGBQUAD MapRGB24ToGrey(const RGBQUAD& rgb);
	static int MapRGB24ToPalette(const RGBQUAD& rgb, std::vector<RGBQUAD>& plt);
	LONG GetPixel(LONG x, LONG y, RGBQUAD rgb[1], bool bGray[1] = nullptr);
	void SetPixel(LONG x, LONG y, RGBQUAD rgb, int width = 1, int height = 1);
	std::vector<DWORD> GetHistogram();

	bool CodingGrey(CImgProcDoc& newDoc);
	bool CodingRGB24(CImgProcDoc& newDoc);
	bool CodingIndexColor(CImgProcDoc& newDoc);
	void BrightnessContrast(CImgProcDoc& newDoc, LONG brightnessModifier, double contrastMultiplier);

	enum INTERPOLATION_MODE { DEFAULT, NEAREST, BILINEAR };
	void ImageInterpolation(CImgProcDoc& newDoc, double factor_w, double factor_h, INTERPOLATION_MODE nMethod);
	void ImageMedianFiltering(CImgProcDoc& newDoc, DWORD radius, double cenw);
	void ImageGaussianSmoothing(CImgProcDoc& newDoc, double sigma);
	void ImageHistogramEqualization(CImgProcDoc& newDoc);
	void ImageSharpening(CImgProcDoc& newDoc, double reserve, double enhance,
		const std::vector<std::vector<double>>& optx, const std::vector<std::vector<double>>& opty);
	void ImageBilateralFiltering(CImgProcDoc& newDoc, double sigma_d, double sigma_r);
	void ImageAddImpluseNoise(CImgProcDoc& newDoc, double dutyPercentage, bool binaryzation);

	afx_msg void OnImageprocessingSavetonewbmpfile();
	afx_msg void OnImageprocessingDispplayfileheader();
	afx_msg void OnUpdateTransferIndexcolor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTransferRgb24(CCmdUI* pCmdUI);
	afx_msg void OnTransferRgb24();
	afx_msg void OnTransferIndexcolor();
	afx_msg void OnTransferGrey();
};
