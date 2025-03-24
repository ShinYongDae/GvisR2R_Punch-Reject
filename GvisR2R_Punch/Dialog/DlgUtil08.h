#pragma once

#include "../Process/DataFile.h"

#define TIM_EXTRACT_REELMAP 100

// CDlgUtil08 ��ȭ �����Դϴ�.

class CDlgUtil08 : public CDialog
{
	DECLARE_DYNAMIC(CDlgUtil08)

	BOOL m_bConfirm;
	BOOL m_bTIM_EXTRACT_REELMAP;
	int m_nStep_TIM_EXTRACT_REELMAP;

	BOOL FileBrowse(CString& sPath);
	void DoConfirm();

public:
	CDlgUtil08(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgUtil08();

	BOOL ExtractReelmap(CString sPath);
	BOOL ExtractInfo(CDataFile *pDataFile);
	BOOL LoadCamMstInfo();
	BOOL MakePcrDir(CString sPath);
	BOOL ExtractPcr(CDataFile *pDataFile);

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_UTIL_08 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnReelmap();
	afx_msg void OnBnClickedBtnConfirm();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
