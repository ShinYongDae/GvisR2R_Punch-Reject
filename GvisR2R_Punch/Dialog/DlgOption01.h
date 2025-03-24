#pragma once
#include <afxwin.h>

#define TIM_DISP_STS				450

// CDlgOption01 대화 상자입니다.

class CDlgOption01 : public CDialog
{
	//DECLARE_DYNAMIC(CDlgOption01)

	BOOL m_bTIM_DISP_STS;

public:
	CDlgOption01(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgOption01();

	CRect* m_pRect;

	BOOL Create();
	void AtDlgShow();
	void AtDlgHide();
	void DispStatus();

// 대화 상자 데이터입니다.
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OPTION_01 };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck10();
};
