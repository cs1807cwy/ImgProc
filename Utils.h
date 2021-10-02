//utils.h

#pragma once
#include <afxstr.h>
#include <afxwin.h>

class Utils
{
public:
	static char*   OpenBMPfile(CString strBmpFile);
	static BITMAPFILEHEADER* GetDIBHEADER(char* pFileBuffer);
	static BITMAPINFOHEADER* GetDIBINFO(char* pFileBuffer);
	static char*   GetDIBImageData(char* pFileBuffer);
	static RGBQUAD* GetDIBPaletteData(char* pFileBuffer, int nEntryNumber[1]);
	static int     GetImageWidth(char* pFileBuffer);
	static int     GetImageHeight(char* pFileBuffer);
	static int     GetColorBits(char* pFileBuffer);
	static long    GetUsedColors(char* pFileBuffer);
	static long    GetWidthBytes(char* pFileBuffer);
	static void    DisplayHeaderMessage(char* pBmpFileBuf);
	static void		 DisplayImage(	CDC* pDC, char* pBmpFileBuf,
															int disp_xL = 0, int disp_yL = 0,
															int disp_Width = 0, int disp_Height = 0,
															int mode = 0);
	static long    GetPixel(char* pFileBuffer, int x, int y, RGBQUAD rgb[1], bool bGray[1] = NULL);
	static void    SetPixel(char* pFileBuffer, int x, int y, RGBQUAD rgb);
	static BOOL    SaveDIB(char* pFileBuffer, CString strBmpFile);
	static char*   ImageInterpolation(char* pBmpFileBuf, int newWidth, int newHeight, int nMethod = 0);

};
