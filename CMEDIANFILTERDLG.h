#pragma once


// CMEDIANFILTERDLG 对话框

class CMEDIANFILTERDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CMEDIANFILTERDLG)

private:
	DWORD radius;
	double centerWeight;
public:
	CMEDIANFILTERDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMEDIANFILTERDLG();
	const DWORD RADIUSMIN = 1;
	const DWORD RADIUSMAX = 10;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEDIANFILTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnKillfocusMedianfilterEditWndlen();
	afx_msg void OnEnKillfocusMedianfilterEditCenterw();
	CEdit m_edit_radius;
	CEdit m_edit_weight;
	CStatic m_title_size;
	virtual BOOL OnInitDialog();
};
