#pragma once


// CGAUSSIANSMOOTHDLG 对话框

class CGAUSSIANSMOOTHDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CGAUSSIANSMOOTHDLG)
private:
	double sigma;
	DWORD radius;
public:
	CGAUSSIANSMOOTHDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGAUSSIANSMOOTHDLG();
	const double SIGMAMAX = 50.0;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAUSSIANSMOOTH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_edit_sigma;
	CStatic m_title_size;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusGaussiansmoothEditSigma();
};
