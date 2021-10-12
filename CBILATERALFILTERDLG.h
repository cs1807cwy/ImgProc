#pragma once


// CBILATERALFILTERDLG 对话框

class CBILATERALFILTERDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CBILATERALFILTERDLG)
private:
	double sigma_d, sigma_r;
public:
	CBILATERALFILTERDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBILATERALFILTERDLG();

	// note:限制 sigma_d 不超过 SIGMADMAX，避免窗口过大
	const double SIGMADMAX = 20.0;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BILATERALFILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CEdit m_edit_sigmad;
	CEdit m_edit_sigmar;
	afx_msg void OnEnKillfocusBifilterEditSigmad();
	afx_msg void OnEnKillfocusBifilterEditSigmar();
	virtual BOOL OnInitDialog();
};
