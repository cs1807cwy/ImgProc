#pragma once


// CSETPIXELDLG 对话框

class CSETPIXELDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CSETPIXELDLG)
private:
	RGBQUAD selColor;

	void SychronizeColorToInspect();
	void SychronizeColorToEditor();
public:
	CSETPIXELDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSETPIXELDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_PIXEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReturn();
	afx_msg void OnBnClickedApply();
	CEdit m_edit_x;
	CEdit m_edit_y;
	CEdit m_edit_w;
	CEdit m_edit_h;
	CEdit m_edit_r;
	CEdit m_edit_g;
	CEdit m_edit_b;
	CMFCColorButton m_clr_btn;
	afx_msg void OnEnKillfocusGetpixelEditR();
	afx_msg void OnEnKillfocusGetpixelEditG();
	afx_msg void OnEnKillfocusGetpixelEditB();
	afx_msg void OnBnClickedMfccolorinspect();
	virtual BOOL OnInitDialog();
};
