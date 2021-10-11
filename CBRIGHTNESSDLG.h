#pragma once


// CBRIGHTNESSDLG 对话框

class CBRIGHTNESSDLG : public CDialogEx
{
	DECLARE_DYNAMIC(CBRIGHTNESSDLG)
private:
	LONG brightnessModifier;
	double contrastMultiplier;
public:
	CBRIGHTNESSDLG(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CBRIGHTNESSDLG();
	const LONG BRIGHTNESSMODIFIERMIN = -255;
	const LONG BRIGHTNESSMODIFIERMAX = 255;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BRIGHTNESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CEdit m_edit_bright;
	CEdit m_edit_contrast;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnKillfocusBrightnessEditBrightness();
	afx_msg void OnEnKillfocusBrightnessEditContrast();
};
