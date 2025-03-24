
// DlgCammasterinfoPch.h : ��� ����
//

#pragma once

#include "../Process/CamMasterDefine.h"
#include "../Global/GlobalDefine.h"

#define TIM_DISP_STS				450

// CDlgCammasterinfoPch ��ȭ ����
class CDlgCammasterinfoPch : public CDialogEx
{
	BOOL m_bTIM_DISP_STS;

	BOOL FileBrowse(CString& sPath);
	BOOL LoadPchFromCam(CString sPath, CString& sData);
	BOOL LoadCadMk2PntAlign(CString sPath, CString& sData);
	BOOL LoadCadMk4PntAlign(CString sPath, CString& sData);

// �����Դϴ�.
public:
	CDlgCammasterinfoPch(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	~CDlgCammasterinfoPch();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_DLG_CAMMASTERINFO_PCH};
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	//afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
