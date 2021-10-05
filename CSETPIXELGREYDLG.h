#pragma once


// CSETPIXELGREYDLG 对话框

class CSETPIXELGREYDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CSETPIXELGREYDLG)
private:
	RGBQUAD selColor;

	void SychronizeColorToInspect();
	void SychronizeColorToEditor();
public:
	CSETPIXELGREYDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSETPIXELGREYDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_PIXEL_GREY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReturn();
	afx_msg void OnBnClickedApply();
	afx_msg void OnEnKillfocusGetpixelEditG();
	CEdit m_edit_x;
	CEdit m_edit_y;
	CEdit m_edit_w;
	CEdit m_edit_h;
	CEdit m_edit_g;
	CMFCColorButton m_clr_btn;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedMfccolorinspect();
	CStatic m_title_range;
};
