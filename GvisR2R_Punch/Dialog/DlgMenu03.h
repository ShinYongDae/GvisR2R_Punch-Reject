#if !defined(AFX_DLGMENU03_H__F62C5D03_F918_4F60_A383_5D97401627D4__INCLUDED_)
#define AFX_DLGMENU03_H__F62C5D03_F918_4F60_A383_5D97401627D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgMenu03.h : header file
//

#include "MyBtn.h"
#include "MyLabel.h"
#include "MyGroup.h"

#define MAX_MENU03_STC				138
#define MAX_MENU03_BTN				119
#define MAX_MENU03_GRP				11
#define MAX_MENU03_LABEL			5

#define TIM_MENU03_DISP				300
//#define TIM_CHK_DONE_MK				301
//#define TIM_CHK_DONE_AOI			302
//#define TIM_CHK_DONE_MKAOI			303
#define TIM_CHK_MREG				304
#define TIM_SET_RC_RELATION			306
#define TIM_SW_RC_RELATION			307
#define TIM_SW_UC_RELATION			308
#define TIM_AOI_RESET_OFF			309
#define TIM_AOI_LOTEND_OFF			310
#define TIM_CHK_DONE_READY			311
#define TIM_CHK_DONE_BUF_HOME		312
#define TIM_CHK_DONE_BUF_INIT		313
// #define TIM_CHK_SOL_MK			312
// #define TIM_CHK_SOL_AOI			313
//#define TIM_CHK_DONE_ENGRAVE		314

//#define TIM_CHK_DONE_ENG_BUF_HOME	315
//#define TIM_CHK_DONE_ENG_BUF_INIT	316

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu03 dialog

class CDlgMenu03 : public CDialog
{
	CMyBtn myBtn[MAX_MENU03_BTN];
	CMyLabel myStcTitle[MAX_MENU03_STC];
	CMyGroup myGrp[MAX_MENU03_GRP];
	CMyLabel myLabel[MAX_MENU03_LABEL];

	BOOL m_bLoadImg;
	BOOL m_bTIM_MENU03_DISP;
	//BOOL m_bTIM_CHK_DONE_MK;
	//BOOL m_bTIM_CHK_DONE_AOI;
	//BOOL m_bTIM_CHK_DONE_MKAOI;
	BOOL m_bTIM_CHK_DONE_BUF_HOME;
	BOOL m_bTIM_CHK_DONE_BUF_INIT;
	BOOL m_bTIM_CHK_DONE_READY;
	BOOL m_bTIM_SET_RC_RELATION;
	BOOL m_bTIM_SW_RC_RELATION;
	BOOL m_bTIM_SW_UC_RELATION;

	double m_dPrevEnc[MAX_AXIS];
	int m_nDir[MAX_AXIS];
	BOOL m_bBufInitPosMove;

	//BOOL m_bTIM_CHK_DONE_ENGRAVE;
	//BOOL m_bTIM_CHK_DONE_ENG_BUF_HOME;
	//BOOL m_bTIM_CHK_DONE_ENG_BUF_INIT;


	void AtDlgShow();
	void AtDlgHide();

	void LoadImg();
	void DelImg();

	void InitLabel();
	void InitStatic();
	void InitBtn();
	void InitGroup();

	void Disp();
	void DispMain();
	void DispRecoiler();
	void DispPunch();
	void DispAoiDn();
	void DispAoiUp();
	void DispEngrave();
	void DispUncoiler();

	//void ChkDoneMkAoi();
	void DoReady();
	BOOL DoReset();

	
// Construction
public:
	CDlgMenu03(CWnd* pParent = NULL);   // standard constructor
	~CDlgMenu03();

	CRect* m_pRect;
	int m_nFdSpd;
	BOOL m_bVMoveMkFdVac, m_bVMoveAoiFdVac;

	BOOL Create();

	BOOL GetRun();	
	BOOL GetReset();
	BOOL GetReady();
	BOOL GetRelation();
	BOOL GetCw();
	void SetCw(BOOL bOn);
	BOOL GetCcw();
	void SetCcw(BOOL bOn);
	void SetMkOnePnl(BOOL bOn);
	void SetAoiOnePnl(BOOL bOn);
	void MoveMk(int nDir=M_CW);
	void MoveMk(double dOffset);
	void MoveAoi(int nDir=M_CW);
	void MoveAoi(double dOffset);
	BOOL GetMkTq();
	BOOL GetAoiTq();
	void SetMkTq(BOOL bOn);
	void SetAoiTq(BOOL bOn);
	//void ChkDoneMk();
	//void ChkDoneAoi();
	//void ChkBufHomeDone();
	void ChkBufInitDone();
	void ChkReadyDone();
	void DoManual();
	void DoAuto();

	//void ChkEngBufHomeDone();
	//void ChkEngBufInitDone();

	afx_msg LRESULT OnMyBtnDown(WPARAM wPara, LPARAM lPara);
	afx_msg LRESULT OnMyBtnUp(WPARAM wPara, LPARAM lPara);

 	void SwMyBtnDown(int nCtrlID);
 	void SwMyBtnUp(int nCtrlID);
	void SwMpeBtn(int nCtrlID, long lData);

	BOOL IsStop();

	BOOL IsAoiLdRun();
	BOOL IsAoiFdVac();
	BOOL IsBufRolSol();


	// Torque Motor
	void SwEngraveTq(BOOL bOn);

	// AOI
	void SwAoiReset(BOOL bOn);

	void SwReset();
	void SwReady();
	void SwRun();
	void SwStop();
	void SwAoiRelation();

	void SetCore150mmRecoiler(BOOL bOn);
	void SetCore150mmUncoiler(BOOL bOn);

	void SetDualTest(BOOL bOn=TRUE);


	BOOL GetEngraveTq();
	void SetEngraveTq(BOOL bOn);
	void SetEngraveOnePnl(BOOL bOn);
	void MoveEngrave(int nDir = M_CW);
	void MoveEngrave(double dOffset);
	//void ChkDoneEngrave();

	BOOL IsEngraveFdVac();
	BOOL IsEngraveTqVac();
	BOOL IsEngraveTblVac();
	BOOL IsEngraveTblBlw();

	void SetEngraveCleanner(BOOL bOn);
	void SetAoiDnCleanner(BOOL bOn);

	void UpdateSignal();

	void SetLed(int nIdx, BOOL bOn = TRUE);
	void DispAlignRangeL(int* pAlignX, int* pAlignY);
	void DispAlignRangeR(int* pAlignX, int* pAlignY);

// Dialog Data
	//{{AFX_DATA(CDlgMenu03)
	enum { IDD = IDD_DLG_MENU_03 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMenu03)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgMenu03)
	afx_msg void OnChk24();
	afx_msg void OnChk62();
	afx_msg void OnChk16();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);// (UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedChk79();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMENU03_H__F62C5D03_F918_4F60_A383_5D97401627D4__INCLUDED_)
