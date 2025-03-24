#pragma once

#include "../Process/DataFile.h"

#define TIM_EXTRACT_REELMAP 100

// CDlgUtil08 대화 상자입니다.

class CDlgUtil08 : public CDialog
{
	DECLARE_DYNAMIC(CDlgUtil08)

	BOOL m_bConfirm;
	BOOL m_bTIM_EXTRACT_REELMAP;
	int m_nStep_TIM_EXTRACT_REELMAP;

	BOOL FileBrowse(CString& sPath);
	void DoConfirm();

public:
	CDlgUtil08(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgUtil08();

	BOOL ExtractReelmap(CString sPath);
	BOOL ExtractInfo(CDataFile *pDataFile);
	BOOL LoadCamMstInfo();
	BOOL MakePcrDir(CString sPath);
	BOOL ExtractPcr(CDataFile *pDataFile);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_UTIL_08 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnReelmap();
	afx_msg void OnBnClickedBtnConfirm();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
