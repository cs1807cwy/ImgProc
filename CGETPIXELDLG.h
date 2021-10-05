#pragma once
#include <vector>

// CGETPIXELDLG 对话框

class CGETPIXELDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CGETPIXELDLG)

private:
	LONG ctr_x, ctr_y;
	CStatic& Pic(WORD x, WORD y);

public:
	CGETPIXELDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGETPIXELDLG();
	void FillInspectWnd(const std::vector<RGBQUAD>& clrMat5x5,
		RGBQUAD clrFrame, RGBQUAD clrFocusFrame, WORD focusWidth);
	void SetEditColorRGB(RGBQUAD color);
	void SetEditColorNA();
	void ProcCapture();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GET_PIXEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCapture();
	CButton m_btn_capture;
	CButton m_btn_return;
	CEdit m_edit_x;
	CEdit m_edit_y;
	CEdit m_edit_r;
	CEdit m_edit_g;
	CEdit m_edit_b;
	CStatic m_pic_00;
	CStatic m_pic_01;
	CStatic m_pic_02;
	CStatic m_pic_03;
	CStatic m_pic_04;
	CStatic m_pic_10;
	CStatic m_pic_11;
	CStatic m_pic_12;
	CStatic m_pic_13;
	CStatic m_pic_14;
	CStatic m_pic_20;
	CStatic m_pic_21;
	CStatic m_pic_22;
	CStatic m_pic_23;
	CStatic m_pic_24;
	CStatic m_pic_30;
	CStatic m_pic_31;
	CStatic m_pic_32;
	CStatic m_pic_33;
	CStatic m_pic_34;
	CStatic m_pic_40;
	CStatic m_pic_41;
	CStatic m_pic_42;
	CStatic m_pic_43;
	CStatic m_pic_44;
	afx_msg void OnBnClickedReturn();
	CStatic m_coordx;
	CStatic m_coordy;
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedGetpixelPicture1();
	afx_msg void OnStnClickedGetpixelPicture2();
	afx_msg void OnStnClickedGetpixelPicture3();
	afx_msg void OnStnClickedGetpixelPicture4();
	afx_msg void OnStnClickedGetpixelPicture5();
	afx_msg void OnStnClickedGetpixelPicture6();
	afx_msg void OnStnClickedGetpixelPicture7();
	afx_msg void OnStnClickedGetpixelPicture8();
	afx_msg void OnStnClickedGetpixelPicture9();
	afx_msg void OnStnClickedGetpixelPicture10();
	afx_msg void OnStnClickedGetpixelPicture11();
	afx_msg void OnStnClickedGetpixelPicture12();
	afx_msg void OnStnClickedGetpixelPicture14();
	afx_msg void OnStnClickedGetpixelPicture13();
	afx_msg void OnStnClickedGetpixelPicture15();
	afx_msg void OnStnClickedGetpixelPicture16();
	afx_msg void OnStnClickedGetpixelPicture17();
	afx_msg void OnStnClickedGetpixelPicture18();
	afx_msg void OnStnClickedGetpixelPicture19();
	afx_msg void OnStnClickedGetpixelPicture20();
	afx_msg void OnStnClickedGetpixelPicture21();
	afx_msg void OnStnClickedGetpixelPicture22();
	afx_msg void OnStnClickedGetpixelPicture23();
	afx_msg void OnStnClickedGetpixelPicture24();
	afx_msg void OnStnClickedGetpixelPicture25();
};
