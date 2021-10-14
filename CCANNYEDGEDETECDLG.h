#pragma once


// CCANNYEDGEDETECDLG 对话框

class CCANNYEDGEDETECDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CCANNYEDGEDETECDLG)
private:
	double sigma;
	double thh, thl;
public:
	CCANNYEDGEDETECDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCANNYEDGEDETECDLG();

	const double SIGMAMAX = 40.0;
	const double THRESHMIN = 0.0;
	const double THRESHMAX = 255.0;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CANNYEDGEDETECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnKillfocusCannyedgedetectionEditVariance();
	afx_msg void OnEnKillfocusCannyedgedetectionEditThh();
	afx_msg void OnEnKillfocusCannyedgedetectionEditThl();
	CEdit m_edit_sigma;
	CEdit m_edit_thh;
	CEdit m_edit_thl;
	virtual BOOL OnInitDialog();
};
