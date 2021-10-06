#pragma once


// CINTERPOLATIONDLG 对话框

class CINTERPOLATIONDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CINTERPOLATIONDLG)

public:
	CINTERPOLATIONDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CINTERPOLATIONDLG();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTERPOLATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();

	enum MODE { UNKNOWN, NEAREST, BILINEAR };
	const CString MODENAME[3]{ _T("Unknown"), _T("Nearest"), _T("Bi-Linear") };


	MODE modeSel;
	CSplitButton m_split_sel;
	CEdit m_edit_fx;
	CEdit m_edit_fy;
	afx_msg void OnModeNearest();
	afx_msg void OnModeBi();
	virtual BOOL OnInitDialog();
};
