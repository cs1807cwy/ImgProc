#pragma once


// CSHARPENINGDLG 对话框

class CSHARPENINGDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CSHARPENINGDLG)
public:
	enum OPERATOR { UNKNOWN, PIXELDIFF, SOBEL, ROBERTS, PREWITT };
	const CString OPTNAME[5]{ _T("Unknown"), _T("Pixel differential"), _T("Sobel"), _T("Roberts"), _T("Prewitt") };
	const std::vector<std::vector<double>> OPTMATX[5]{
		{{0,0,0},{0,0,0},{0,0,0}},
		{{0,0,0},{0,-1,1},{0,0,0}},
		{{-1.0/4.0,0,1.0/4.0},{-2.0/4.0,0,2.0/4.0},{-1.0/4.0,0,1.0/4.0}},
		{{0,0,-1},{0,1,0},{0,0,0}},
		{{-1.0/3.0,0,1.0/3.0},{-1.0/3.0,0,1.0/3.0},{-1.0/3.0,0,1.0/3.0}}
	};
	const std::vector<std::vector<double>> OPTMATY[5]{
	{{0,0,0},{0,0,0},{0,0,0}},
	{{0,0,0},{0,-1,0},{0,1,0}},
	{{-1.0/4.0,-2.0/4.0,-1.0/4.0},{0,0,0},{1.0/4.0,2.0/4.0,1.0/4.0}},
	{{-1,0,0},{0,1,0},{0,0,0}},
	{{-1.0/3.0,-1.0/3.0,-1.0/3.0},{0,0,0},{1.0/3.0,1.0/3.0,1.0/3.0}}
	};

private:
	double reserve, enhance;
	CSHARPENINGDLG::OPERATOR opt;
public:
	CSHARPENINGDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSHARPENINGDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHARPENING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnKillfocusSharpeningEditRev();
	afx_msg void OnEnKillfocusSharpeningEditEnh();
	CEdit m_edit_rev;
	CEdit m_edit_enh;
	virtual BOOL OnInitDialog();
	CSplitButton m_split_opt;
	afx_msg void OnOperatorDifferential();
	afx_msg void OnOperatorSobel();
	afx_msg void OnOperatorRoberts();
	afx_msg void OnOperatorPrewitt();
};
