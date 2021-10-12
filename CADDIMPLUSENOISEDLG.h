#pragma once


// CADDIMPLUSENOISEDLG 对话框

class CADDIMPLUSENOISEDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CADDIMPLUSENOISEDLG)
private:
	double duty;
	bool binaryzation;
public:
	CADDIMPLUSENOISEDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CADDIMPLUSENOISEDLG();
	const double DUTYMIN = 0.0;
	const double DUTYMAX = 100.0;
	const CString TYPENAME[2]{ _T("Random impluse noise"),_T("Salt & Pepper noise") };

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDIMPLUSENOISE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnKillfocusAddimpulsenoiseEditDuty();
	afx_msg void OnNoisetypeSaltpepper();
	afx_msg void OnNoisetypeRandomimplusenoise();
	CSplitButton m_split_type;
	CEdit m_edit_duty;
	virtual BOOL OnInitDialog();
};
