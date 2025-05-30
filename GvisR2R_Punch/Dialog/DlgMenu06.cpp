// DlgMenu06.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgMenu06.h"
#include "DlgKeyNum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../MainFrm.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu06 dialog


CDlgMenu06::CDlgMenu06(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMenu06::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMenu06)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	m_bLoadImg = FALSE;
	m_pMyGL = NULL;
	m_nSerial = 0;
	m_nSerialDispMkInfo = 0;

	m_nIdxMkInfo[0] = 0;
	m_nIdxMkInfo[1] = 0;

	m_nIdxDef[0] = 0;
	m_nIdxDef[1] = 0;
	m_bTIM_DISP_DEF_IMG = FALSE;
	m_bTIM_DISP_MK_CNT = FALSE;

	m_bLastProc = FALSE;
	m_bLastProcFromUp = TRUE;
	m_bLastProcFromEng = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
	m_bLotEnd = FALSE;

	m_nWinkMk[0] = 0;
	m_nWinkMk[1] = 0;
	m_bWinkMk[0] = FALSE;
	m_bWinkMk[1] = FALSE;

	m_bTIM_WINK_MK_L = FALSE;
	m_bTIM_WINK_MK_R = FALSE;

	m_bLowPartialSpd = FALSE;
	m_bEnableMkStrip = FALSE;
}

CDlgMenu06::~CDlgMenu06()
{
	m_bTIM_DISP_DEF_IMG = FALSE;
	m_bTIM_DISP_MK_CNT = FALSE;
	
	DelImg();

	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}

	if(m_pMyGL)
	{
		delete m_pMyGL;
		m_pMyGL = NULL;
	}
}

BOOL CDlgMenu06::Create()
{
	return CDialog::Create(CDlgMenu06::IDD);
}


void CDlgMenu06::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMenu06)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMenu06, CDialog)
	//{{AFX_MSG_MAP(CDlgMenu06)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHK_USER_INFO, OnChkUserInfo)
	ON_WM_PAINT()
	//ON_BN_CLICKED(IDC_CHK_TP_STOP, OnChkTpStop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHK_EJECT_BUFFER, OnChkEjectBuffer)
	ON_BN_CLICKED(IDC_CHK_REVIEW, OnChkReview)
	ON_BN_CLICKED(IDC_STC_REVIEW_LEN, OnStcReviewLen)
	ON_BN_CLICKED(IDC_CHK_REVIEW_MOVE, OnChkReviewMove)
	ON_BN_CLICKED(IDC_CHK_LOT_END, OnChkLotEnd)
	ON_BN_CLICKED(IDC_CHK_DEF_UP, OnChkDefUp)
	ON_BN_CLICKED(IDC_CHK_DEF_DN, OnChkDefDn)
	ON_BN_CLICKED(IDC_CHK_DEF_ALL, OnChkDefAll)
	ON_BN_CLICKED(IDC_STC_UP, OnStcUp)
	ON_BN_CLICKED(IDC_STC_DN, OnStcDn)
	ON_BN_CLICKED(IDC_BTN_MK_ALL, OnBtnMkAll)
	ON_BN_CLICKED(IDC_CHK_REMARKING, OnChkRemarking)
	ON_BN_CLICKED(IDC_CHK_ELEC, OnChkElec)
	ON_BN_CLICKED(IDC_CHK_ELEC_STOP, OnChkElecStop)
	ON_BN_CLICKED(IDC_CHK_2LAYER, OnChk2layer)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DRAW_REELMAP_INNER, OnDrawReelMap)
	ON_MESSAGE(WM_MYSTATIC_REDRAW, OnMyStaticRedraw)
	//ON_BN_CLICKED(IDC_CHK_MK_1, &CDlgMenu06::OnBnClickedChkMk1)
	//ON_BN_CLICKED(IDC_CHK_MK_2, &CDlgMenu06::OnBnClickedChkMk2)
	//ON_BN_CLICKED(IDC_CHK_MK_3, &CDlgMenu06::OnBnClickedChkMk3)
	//ON_BN_CLICKED(IDC_CHK_MK_4, &CDlgMenu06::OnBnClickedChkMk4)
	//ON_BN_DOUBLECLICKED(IDC_CHK_MK_1, &CDlgMenu06::OnDoubleclickedChkMk1)
	//ON_BN_DOUBLECLICKED(IDC_CHK_MK_2, &CDlgMenu06::OnDoubleclickedChkMk2)
	//ON_BN_DOUBLECLICKED(IDC_CHK_MK_3, &CDlgMenu06::OnDoubleclickedChkMk3)
	//ON_BN_DOUBLECLICKED(IDC_CHK_MK_4, &CDlgMenu06::OnDoubleclickedChkMk4)
	ON_MESSAGE(WM_MYBTN_DBLCLK, OnMyBtnDblClk)
	//ON_BN_CLICKED(IDC_CHK_TP_STOP, &CDlgMenu06::OnBnClickedChkTpStop)
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_STN_CLICKED(IDC_STC_LOT_SRL, &CDlgMenu06::OnStnClickedStcLotSrl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu06 message handlers

void CDlgMenu06::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(!m_pRect)
	{
		m_pRect = new CRect;
		
		this->GetClientRect(m_pRect);
		m_pRect->top = 75 + 2;
		m_pRect->bottom += 75 + 2;
		m_pRect->left = 3;
		m_pRect->right += 3;
		this->MoveWindow(m_pRect, TRUE);
	}

	if(bShow)
	{
		//this->MoveWindow(m_pRect, TRUE);
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgMenu06::AtDlgShow()
{
	LoadImg();
	UpdateData();

	if(pDoc->WorkingInfo.LastJob.nMergingLayer==0) 	// [0]:AOI-Up , [1]:AOI-Dn
	{
		myStcTitle[61].SetTextColor(RGB_WHITE);
		myStcTitle[61].SetBkColor(RGB_RED);
		myStcTitle[62].SetTextColor(RGB_WHITE);
		myStcTitle[62].SetBkColor(RGB_DARKBLUE);
	}
	else if(pDoc->WorkingInfo.LastJob.nMergingLayer==1) 	// [0]:AOI-Up , [1]:AOI-Dn
	{
		myStcTitle[61].SetTextColor(RGB_WHITE);
		myStcTitle[61].SetBkColor(RGB_DARKBLUE);
		myStcTitle[62].SetTextColor(RGB_WHITE);
		myStcTitle[62].SetBkColor(RGB_RED);
	}

	myBtn[21].SetCheck(pDoc->WorkingInfo.Probing[0].bUse);
	myBtn[22].SetCheck(pDoc->WorkingInfo.Probing[0].bStopOnOpen);

	//if (pDoc->GetTestMode() == MODE_OUTER)
	//{
	//	if (pDoc->m_bEngDualTest)
	//		SelMap(ALL);
	//	else
	//		SelMap(UP);
	//}
}

void CDlgMenu06::AtDlgHide()
{
	DelImg();
}

void CDlgMenu06::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for(i=0; i<MAX_MENU06_BTN; i++)
	{
		if(i==7 || i==8 || i==9 || i==10 || i==11)
		{
			myBtn[i].LoadBkImage(NI_BTN_UP_DlgMenu06, BTN_IMG_UP);
			myBtn[i].LoadBkImage(NI_BTN_DN_DlgMenu06, BTN_IMG_DN);
		}
		else if(i==15 || i==20)
		{
			myBtn[i].LoadBkImage(IMG_BTN_UP_PURPLE_S, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_PURPLE_S, BTN_IMG_DN);
		}
		else if(i==16 || i==17 || i==18 || i==19)
		{
			myBtn[i].LoadBkImage(IMG_BTN_UP_GRAY_S, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_BLUE_S, BTN_IMG_DN);
		}
		else
		{
			myBtn[i].LoadBkImage(IMG_BTN_UP_DlgMenu06, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_BTN_DN_DlgMenu06, BTN_IMG_DN);
		}
	}
}

void CDlgMenu06::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;
	for(i=0; i<MAX_MENU06_BTN; i++)
		myBtn[i].DelImgList();
}


LRESULT CDlgMenu06::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	SwMyBtnDown(nCtrlID);

	return 0L;
}

void CDlgMenu06::SwMyBtnDown(int nCtrlID)
{
	switch (nCtrlID)
	{
	case IDC_CHK_TP_STOP:
		ChkTpStop();
		break;
	}
}

LRESULT CDlgMenu06::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	SwMyBtnUp(nCtrlID);
	return 0L;
}

void CDlgMenu06::SwMyBtnUp(int nCtrlID)
{
	switch (nCtrlID)
	{
	case IDC_CHK_TP_STOP:
		ChkTpStop();
		break;
	}
}

BOOL CDlgMenu06::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
#ifdef USE_VISION

	if (pView->m_pVisionInner[1])
	{
		delete pView->m_pVisionInner[1];
		pView->m_pVisionInner[1] = NULL;
	}
	if (pView->m_pVisionInner[0])
	{
		delete pView->m_pVisionInner[0];
		pView->m_pVisionInner[0] = NULL;
	}

	HWND hCtrlV0[4] = { 0 };
#ifdef USE_MIL
	if(pView && pView->m_pDlgMenu02)
		pView->m_pVisionInner[0] = new CVision(2, pView->m_pDlgMenu02->m_MilSys, hCtrlV0, this);

	HWND hCtrlV1[4] = { 0 };
	if (pView && pView->m_pDlgMenu02)
		pView->m_pVisionInner[1] = new CVision(3, pView->m_pDlgMenu02->m_MilSys, hCtrlV1, this);
#endif
#endif



	InitStatic();
	InitBtn();

	SetRgbStcDef();
	SetTitleStcDef();

	InitGL();
	//SetPnlNum();
	//SetPnlDefNum();

	//InitMkInfo();
	
	UpdateData();
	//pView->DispStsBar(_T("정지-1"), 0);
	pView->DispMain(_T("정 지"), RGB_RED);
	EnableBtn(TRUE);

	GetDlgItem(IDC_CHK_REMARKING)->ShowWindow(SW_HIDE);

// 	GetDlgItem(IDC_STC_ST)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_GD_RA_1)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_GD_RA_2)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_GD_RA_3)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_GD_RA_4)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_GD_RA_ALL)->ShowWindow(SW_HIDE);
	
	GetDlgItem(IDC_BTN_ST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_GD_RA_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_GD_RA_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_GD_RA_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_GD_RA_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_GD_RA_ALL)->ShowWindow(SW_HIDE);

	//InitCadImg();
	GetCtrlPos();

	m_bTIM_DISP_MK_CNT = TRUE;
	SetTimer(TIM_DISP_MK_CNT, 300, NULL);



	GetDlgItem(IDC_STC_WORK_INFO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_TOT_REEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_TOTAL_REEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_TP_STOP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_TP_STOP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_TEMP_STOP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_WK_RA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_WORK_RATIO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LT_SEP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT_SEP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LT_RA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT_RATIO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LT_LEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT_LEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_TOT_SPD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_TOTAL_SPEED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MKAOI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_AOI_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_AOI_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_ENG)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_CY_SPD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_CYCLE_SPEED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LT_SRL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT_SRL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MK_CNT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MK_L)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MK_CNT_L)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MK_R)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MK_CNT_R)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_USER_INFO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_REVIEW_LEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_REVIEW_MOVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_REVIEW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_EJECT_BUFFER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_LOT_END)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_REMARKING)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_2LAYER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_MK_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_MK_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_MK_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_MK_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_MK_ALL)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STC_WK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_WK_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_WK_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_WK_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_NUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEFECT_NUM_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEFECT_NUM_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEFECT_NUM_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_RT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEFECT_RATIO_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEFECT_RATIO_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEFECT_RATIO_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_NUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GOOD_NUM_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GOOD_NUM_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GOOD_NUM_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GOOD_RATIO_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GOOD_RATIO_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GOOD_RATIO_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_TOT_NUM)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_TOTAL_NUM_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_TOTAL_NUM_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_TOTAL_NUM_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_ST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_ST_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_ST_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_ST_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_1_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_1_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_1_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_2_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_2_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_2_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_3_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_3_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_3_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_4_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_4_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_4_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_ALL_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_ALL_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_GD_RA_ALL_ALL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_OP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_OPEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_SH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_SHORT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_USH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_U_SHORT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_SHORT_TOT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_SP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_SPACE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_EX)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_EXTRA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_PR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_PROT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_SPACE_TOT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_ENK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_E_NICK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_UDD1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_UDEF1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_PHO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_P_HOLE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_PD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_PAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_P_HOLE_TOT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_HOP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_H_OPEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_HMS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_H_MISS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_HPS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_H_POS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_HDF)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_H_DEF)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_H_MISS_TOT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_EPR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_E_PROT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_NRW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_NARROW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_LTD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_LIGHT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_NK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_NICK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_PO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_POI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_VHO)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_VH_OPEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_VHMS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_VH_MISS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_VHP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_VH_POS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_VHD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_VH_DEF)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_VH_MISS_TOT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_ESP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_E_SPACE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_WD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_DEF_WIDE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_UNIT_REVIEW)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STC_OP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_OPERATOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MDL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_MODEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LAY)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LAYER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LAY2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LAYER2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT_ST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT_START)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT_RN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT_RUN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT_ED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_LOT_END)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_CHK_DEF_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_DEF_DN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_DEF_ALL)->ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgMenu06::SelMap(int nSel)
{
	return;

	CString str;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(bDualTest)
	{
		switch(nSel)
		{
		case UP:
			myBtn[12].SetCheck(TRUE);
			myBtn[13].SetCheck(FALSE);
			myBtn[14].SetCheck(FALSE);
			pView->m_nSelRmapInner = RMAP_INNER_UP;
			break;
		case DN:
			myBtn[12].SetCheck(FALSE);
			myBtn[13].SetCheck(TRUE);
			myBtn[14].SetCheck(FALSE);
			pView->m_nSelRmapInner = RMAP_INNER_DN;
			break;
		case ALL:
			myBtn[12].SetCheck(FALSE);
			myBtn[13].SetCheck(FALSE);
			myBtn[14].SetCheck(TRUE);
			if(pDoc->WorkingInfo.LastJob.nMergingLayer==0)
				pView->m_nSelRmapInner = RMAP_INNER_ALLUP;
			else if(pDoc->WorkingInfo.LastJob.nMergingLayer==1)
				pView->m_nSelRmapInner = RMAP_INNER_ALLDN;
			else
				pView->m_nSelRmapInner = RMAP_INNER_ALLUP;
			break;
		}
	}
	else
	{
			myBtn[12].SetCheck(TRUE);
			myBtn[13].SetCheck(FALSE);
			myBtn[14].SetCheck(FALSE);
			pView->m_nSelRmapInner = RMAP_INNER_UP;
	}

	// 20130315
	//if (pDoc->m_pReelMapInner)
	//{
	//	OpenReelmap(pView->m_nSelRmapInner);
	//	DispReelmap(m_nSerial);
	//}
}

void CDlgMenu06::OpenReelmap(int nSelRmap)
{
	CString sPath;
	if(pDoc->m_pReelMapInner)
	{
#ifdef TEST_MODE
		switch(nSelRmap)
		{
		case RMAP_UP:
			sPath = PATH_REELMAP_UP;
			break;
		case RMAP_DN:
			sPath = PATH_REELMAP_DN;
			break;
		case RMAP_ALLUP:
			sPath = PATH_REELMAP_ALL_UP;
			break;
		case RMAP_ALLDN:
			sPath = PATH_REELMAP_ALL_DN;
			break;
		}
		pDoc->m_pReelMapInner->m_nLayer = nSelRmap;
		pDoc->m_pReelMapInner->Open(sPath);
#else
		if(nSelRmap < 0)
			pDoc->m_pReelMapInner->m_nLayer = pView->m_nSelRmapInner;
		else
			pDoc->m_pReelMapInner->m_nLayer = nSelRmap;

		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			//myBtn[12].SetCheck(FALSE);	// IDC_CHK_DEF_UP
			//myBtn[13].SetCheck(FALSE);	// IDC_CHK_DEF_DN
			//myBtn[14].SetCheck(TRUE);	// IDC_CHK_DEF_ALL
			//pDoc->GetCurrentInfoEng();
			if (pDoc->WorkingInfo.LastJob.bDualTestInner)
			{
				pDoc->m_pReelMapInner->Open();
				pDoc->m_pReelMapInner->SetPathAtBuf();
			}
			else
			{
				pDoc->m_pReelMapInner->Open();
				pDoc->m_pReelMapInner->SetPathAtBuf();
			}
		}
		//pDoc->m_pReelMapInner->Open(pView->GetRmapPath(nSelRmap));
		//pDoc->m_pReelMapInner->SetPathAtBuf(pView->GetRmapPath(nSelRmap));
#endif	
	}
}

BOOL CDlgMenu06::DispReelmap(int nSerial, BOOL bDumy)
{
	if(nSerial <= 0)
		return FALSE;

	if (pDoc->WorkingInfo.System.bSaveLog)
	{
		CString strData;
		strData.Format(_T("DispReelmap: Serial(%d), Dumy(%d)"), nSerial, bDumy);
		SaveLog(strData);
	}

	if (pDoc->m_pReelMapInner)
	{
		if (!pDoc->m_pReelMapInner->Disp(nSerial, bDumy))
			return FALSE;
	}
 	SetPnlNum();
 	SetPnlDefNum();
	myStcReelmap.Refresh();
	this->MoveWindow(m_pRect, TRUE);

	return TRUE;
}

void CDlgMenu06::SetPnlNum()
{
	if(m_pMyGL)
		m_pMyGL->SetPnlNum();
}

void CDlgMenu06::SetPnlDefNum()
{
	if(m_pMyGL)
		m_pMyGL->SetPnlDefNum();
}

void CDlgMenu06::DispMkInfo()	// m_bTIM_DISP_DEF_IMG == FALSE 일때까지 계속 호출함.
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if(bDualTest)
	{
		DispMkInfoUp();
		DispMkInfoDn();

		if(m_nDef[0] > 0 || m_nDef[1] > 0)
		{
			if(!m_bTIM_DISP_DEF_IMG)
			{
				m_nSerialDispMkInfo = m_nSerial;
				m_bTIM_DISP_DEF_IMG = TRUE;
				SetTimer(TIM_DISP_DEF_IMG, 10, NULL);
			}
			else
				SetTimer(TIM_DISP_MK_INFO, 10, NULL);
		}
	}
	else
	{
		DispMkInfoUp();

		if(m_nDef[0] > 0)
		{
			if(!m_bTIM_DISP_DEF_IMG)
			{
				m_nSerialDispMkInfo = m_nSerial;
				m_bTIM_DISP_DEF_IMG = TRUE;
				SetTimer(TIM_DISP_DEF_IMG, 10, NULL);
			}
			else
				SetTimer(TIM_DISP_MK_INFO, 10, NULL);
		}
	}
}

void CDlgMenu06::DispMkInfoUp()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	int nIdx = pDoc->GetPcrIdx0(m_nSerial);
	m_nDef[0] = 0;	

	int nMaxMaxDispDefImg = _tstoi(pDoc->WorkingInfo.System.sMaxDispDefImg);
	if(pDoc->WorkingInfo.System.sMaxDispDefImg.IsEmpty())
		nMaxMaxDispDefImg = 0;
	else
		nMaxMaxDispDefImg = _tstoi(pDoc->WorkingInfo.System.sMaxDispDefImg);

	if(nMaxMaxDispDefImg > 0 && pDoc->m_pPcrInner[0][nIdx]->m_nTotDef - nMaxMaxDispDefImg > 0)
		m_nIdxDef[0] = pDoc->m_pPcrInner[0][nIdx]->m_nTotDef - nMaxMaxDispDefImg; // 불량이미지 인덱스.
	else
		m_nIdxDef[0] = 0; // 불량이미지 인덱스.

	if(bDualTest)
	{
		if(m_nIdxMkInfo[0] > MENU01_STC_DEFINFO_HARF)
			m_nIdxMkInfo[0] = MENU01_STC_DEFINFO_HARF;
	}
	else
	{
		if(m_nIdxMkInfo[0] > MENU01_STC_DEFINFO_HARF*2)
			m_nIdxMkInfo[0] = MENU01_STC_DEFINFO_HARF*2;
	}

#ifdef TEST_MODE
	m_nDef[0] = m_nIdxMkInfo[0] + 15; // DefNum : 15
#else
	if(pDoc->m_pPcrInner[0])
	{
		if(pDoc->m_pPcrInner[0][nIdx])
		{
			m_nDef[0] = pDoc->m_pPcrInner[0][nIdx]->m_nTotDef; // m_nDef : m_nIdxMkInfo + Display Def Num.
			//pDoc->m_pPcrInner[0][nIdx]->m_nTotRealDef = 0;
		}
	}
#endif
}

void CDlgMenu06::DispMkInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

	int nIdx = pDoc->GetPcrIdx1(m_nSerial);
	m_nDef[1] = 0;

	int nMaxMaxDispDefImg = _tstoi(pDoc->WorkingInfo.System.sMaxDispDefImg);
	if(pDoc->WorkingInfo.System.sMaxDispDefImg.IsEmpty())
		nMaxMaxDispDefImg = 0;
	else
		nMaxMaxDispDefImg = _tstoi(pDoc->WorkingInfo.System.sMaxDispDefImg);

	if(nMaxMaxDispDefImg > 0 && pDoc->m_pPcrInner[1][nIdx]->m_nTotDef - nMaxMaxDispDefImg > 0)
		m_nIdxDef[1] = pDoc->m_pPcrInner[1][nIdx]->m_nTotDef - nMaxMaxDispDefImg; // 불량이미지 인덱스.
	else
		m_nIdxDef[1] = 0; // 불량이미지 인덱스.

	if(m_nIdxMkInfo[1] > MENU01_STC_DEFINFO_HARF)
		m_nIdxMkInfo[1] = MENU01_STC_DEFINFO_HARF;
#ifdef TEST_MODE
	m_nDef[1] = m_nIdxMkInfo[1] + 15; // DefNum : 15
#else
	if(pDoc->m_pPcrInner[1])
	{
		if(pDoc->m_pPcrInner[1][nIdx])
		{
			m_nDef[1] = pDoc->m_pPcrInner[1][nIdx]->m_nTotDef; // m_nDef : m_nIdxMkInfo + Display Def Num.
			//pDoc->m_pPcrInner[1][nIdx]->m_nTotRealDef = 0;
		}
	}
#endif
}

void CDlgMenu06::InitMkInfo()
{
	InitMkInfoUp();

	if(pDoc->WorkingInfo.LastJob.bDualTest) // 20220519
		InitMkInfoDn();
}

void CDlgMenu06::SelDisp()
{
#ifdef USE_VISION
	if(!pView->m_pVisionInner[0] || !pView->m_pVisionInner[1])
		return;
#endif

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	HWND hW;
	CRect rt;
	int nCtrlIdCad, nCtrlIdDef;

	if(bDualTest)
	{
		for(int nIdxMkInfo=0; nIdxMkInfo<DEF_VIEW_IMG_NUMBER/2; nIdxMkInfo++)
		{
			switch(nIdxMkInfo)
			{
			case 0:
				nCtrlIdCad = IDC_PIC_CAD_001;
				nCtrlIdDef = IDC_PIC_DEF_001;
				break;
			case 1:
				nCtrlIdCad = IDC_PIC_CAD_002;
				nCtrlIdDef = IDC_PIC_DEF_002;
				break;
			case 2:
				nCtrlIdCad = IDC_PIC_CAD_003;
				nCtrlIdDef = IDC_PIC_DEF_003;
				break;
			case 3:
				nCtrlIdCad = IDC_PIC_CAD_004;
				nCtrlIdDef = IDC_PIC_DEF_004;
				break;
			case 4:
				nCtrlIdCad = IDC_PIC_CAD_005;
				nCtrlIdDef = IDC_PIC_DEF_005;
				break;
			case 5:
				nCtrlIdCad = IDC_PIC_CAD_006;
				nCtrlIdDef = IDC_PIC_DEF_006;
				break;
			}

			hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
			GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
#ifdef USE_VISION
			pView->m_pVisionInner[0]->DisplaySelect(CAD_IMG, hW, rt, nIdxMkInfo);
#endif
			hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
			GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
#ifdef USE_VISION
			pView->m_pVisionInner[0]->DisplaySelect(DEF_IMG, hW, rt, nIdxMkInfo);
#endif

			switch(nIdxMkInfo)
			{
			case 0:
				nCtrlIdCad = IDC_PIC_CAD_007;
				nCtrlIdDef = IDC_PIC_DEF_007;
				break;
			case 1:
				nCtrlIdCad = IDC_PIC_CAD_008;
				nCtrlIdDef = IDC_PIC_DEF_008;
				break;
			case 2:
				nCtrlIdCad = IDC_PIC_CAD_009;
				nCtrlIdDef = IDC_PIC_DEF_009;
				break;
			case 3:
				nCtrlIdCad = IDC_PIC_CAD_010;
				nCtrlIdDef = IDC_PIC_DEF_010;
				break;
			case 4:
				nCtrlIdCad = IDC_PIC_CAD_011;
				nCtrlIdDef = IDC_PIC_DEF_011;
				break;
			case 5:
				nCtrlIdCad = IDC_PIC_CAD_012;
				nCtrlIdDef = IDC_PIC_DEF_012;
				break;
			}

// 			hW = NULL;
// 			GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
// 			pView->m_pVisionInner[0]->DisplaySelect(CAD_IMG, hW, rt, nIdxMkInfo+DEF_VIEW_IMG_NUMBER/2);
// 
// 			hW = NULL;
// 			GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
// 			pView->m_pVisionInner[0]->DisplaySelect(DEF_IMG, hW, rt, nIdxMkInfo+DEF_VIEW_IMG_NUMBER/2);


			hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
			GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
#ifdef USE_VISION
			pView->m_pVisionInner[1]->DisplaySelect(CAD_IMG, hW, rt, nIdxMkInfo);
#endif

			hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
			GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
#ifdef USE_VISION
			pView->m_pVisionInner[1]->DisplaySelect(DEF_IMG, hW, rt, nIdxMkInfo);
#endif
		}
	}
	else
	{
		for(int nIdxMkInfo=0; nIdxMkInfo<DEF_VIEW_IMG_NUMBER; nIdxMkInfo++)
		{
			switch(nIdxMkInfo)
			{
			case 0:
				nCtrlIdCad = IDC_PIC_CAD_001;
				nCtrlIdDef = IDC_PIC_DEF_001;
				break;
			case 1:
				nCtrlIdCad = IDC_PIC_CAD_002;
				nCtrlIdDef = IDC_PIC_DEF_002;
				break;
			case 2:
				nCtrlIdCad = IDC_PIC_CAD_003;
				nCtrlIdDef = IDC_PIC_DEF_003;
				break;
			case 3:
				nCtrlIdCad = IDC_PIC_CAD_004;
				nCtrlIdDef = IDC_PIC_DEF_004;
				break;
			case 4:
				nCtrlIdCad = IDC_PIC_CAD_005;
				nCtrlIdDef = IDC_PIC_DEF_005;
				break;
			case 5:
				nCtrlIdCad = IDC_PIC_CAD_006;
				nCtrlIdDef = IDC_PIC_DEF_006;
				break;
			case 6:
				nCtrlIdCad = IDC_PIC_CAD_007;
				nCtrlIdDef = IDC_PIC_DEF_007;
				break;
			case 7:
				nCtrlIdCad = IDC_PIC_CAD_008;
				nCtrlIdDef = IDC_PIC_DEF_008;
				break;
			case 8:
				nCtrlIdCad = IDC_PIC_CAD_009;
				nCtrlIdDef = IDC_PIC_DEF_009;
				break;
			case 9:
				nCtrlIdCad = IDC_PIC_CAD_010;
				nCtrlIdDef = IDC_PIC_DEF_010;
				break;
			case 10:
				nCtrlIdCad = IDC_PIC_CAD_011;
				nCtrlIdDef = IDC_PIC_DEF_011;
				break;
			case 11:
				nCtrlIdCad = IDC_PIC_CAD_012;
				nCtrlIdDef = IDC_PIC_DEF_012;
				break;
			}

// 			if(nIdxMkInfo >= DEF_VIEW_IMG_NUMBER/2)
// 			{
// 				hW = NULL;
// 				GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
// 				pView->m_pVisionInner[1]->DisplaySelect(CAD_IMG, hW, rt, nIdxMkInfo-DEF_VIEW_IMG_NUMBER/2);
// 
// 				hW = NULL;
// 				GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
// 				pView->m_pVisionInner[1]->DisplaySelect(DEF_IMG, hW, rt, nIdxMkInfo-DEF_VIEW_IMG_NUMBER/2);
// 			}

			hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
			GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
#ifdef USE_VISION
			pView->m_pVisionInner[0]->DisplaySelect(CAD_IMG, hW, rt, nIdxMkInfo);
#endif
			hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
			GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
#ifdef USE_VISION
			pView->m_pVisionInner[0]->DisplaySelect(DEF_IMG, hW, rt, nIdxMkInfo);
#endif
		}

	}

}

void CDlgMenu06::InitMkInfoUp()
{
#ifdef USE_VISION
	if(!pView->m_pVisionInner[0])
		return;
#else
	return;
#endif
	//pDoc->GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	HWND hW;
	CRect rt;
	int nCtrlIdCad, nCtrlIdDef;

	if(bDualTest)
	{
		for(int nIdxMkInfo=0; nIdxMkInfo<DEF_VIEW_IMG_NUMBER/2; nIdxMkInfo++)
		{
			switch(nIdxMkInfo)
			{
			case 0:
				nCtrlIdCad = IDC_PIC_CAD_001;
				nCtrlIdDef = IDC_PIC_DEF_001;
				break;
			case 1:
				nCtrlIdCad = IDC_PIC_CAD_002;
				nCtrlIdDef = IDC_PIC_DEF_002;
				break;
			case 2:
				nCtrlIdCad = IDC_PIC_CAD_003;
				nCtrlIdDef = IDC_PIC_DEF_003;
				break;
			case 3:
				nCtrlIdCad = IDC_PIC_CAD_004;
				nCtrlIdDef = IDC_PIC_DEF_004;
				break;
			case 4:
				nCtrlIdCad = IDC_PIC_CAD_005;
				nCtrlIdDef = IDC_PIC_DEF_005;
				break;
			case 5:
				nCtrlIdCad = IDC_PIC_CAD_006;
				nCtrlIdDef = IDC_PIC_DEF_006;
				break;
			}

			hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
			GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
#ifdef USE_VISION
			pView->m_pVisionInner[0]->SelDispCad(hW, rt, nIdxMkInfo);
			pView->m_pVisionInner[0]->SetOvrCadFontSz(nIdxMkInfo);

			hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
			GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
			pView->m_pVisionInner[0]->SelDispDef(hW, rt, nIdxMkInfo);
#endif
		}
	}
	else
	{
		FreeMkInfoDn();
		for(int nIdxMkInfo=0; nIdxMkInfo<DEF_VIEW_IMG_NUMBER; nIdxMkInfo++)
		{
			switch(nIdxMkInfo)
			{
			case 0:
				nCtrlIdCad = IDC_PIC_CAD_001;
				nCtrlIdDef = IDC_PIC_DEF_001;
				break;
			case 1:
				nCtrlIdCad = IDC_PIC_CAD_002;
				nCtrlIdDef = IDC_PIC_DEF_002;
				break;
			case 2:
				nCtrlIdCad = IDC_PIC_CAD_003;
				nCtrlIdDef = IDC_PIC_DEF_003;
				break;
			case 3:
				nCtrlIdCad = IDC_PIC_CAD_004;
				nCtrlIdDef = IDC_PIC_DEF_004;
				break;
			case 4:
				nCtrlIdCad = IDC_PIC_CAD_005;
				nCtrlIdDef = IDC_PIC_DEF_005;
				break;
			case 5:
				nCtrlIdCad = IDC_PIC_CAD_006;
				nCtrlIdDef = IDC_PIC_DEF_006;
				break;
			case 6:
				nCtrlIdCad = IDC_PIC_CAD_007;
				nCtrlIdDef = IDC_PIC_DEF_007;
				break;
			case 7:
				nCtrlIdCad = IDC_PIC_CAD_008;
				nCtrlIdDef = IDC_PIC_DEF_008;
				break;
			case 8:
				nCtrlIdCad = IDC_PIC_CAD_009;
				nCtrlIdDef = IDC_PIC_DEF_009;
				break;
			case 9:
				nCtrlIdCad = IDC_PIC_CAD_010;
				nCtrlIdDef = IDC_PIC_DEF_010;
				break;
			case 10:
				nCtrlIdCad = IDC_PIC_CAD_011;
				nCtrlIdDef = IDC_PIC_DEF_011;
				break;
			case 11:
				nCtrlIdCad = IDC_PIC_CAD_012;
				nCtrlIdDef = IDC_PIC_DEF_012;
				break;
			}

#ifdef USE_VISION
			hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
			GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
			pView->m_pVisionInner[0]->SelDispCad(hW, rt, nIdxMkInfo);
			pView->m_pVisionInner[0]->SetOvrCadFontSz(nIdxMkInfo);

			hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
			GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
			pView->m_pVisionInner[0]->SelDispDef(hW, rt, nIdxMkInfo);
#endif
 		}
	}

}

void CDlgMenu06::InitMkInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

#ifdef USE_VISION
	if(!pView->m_pVisionInner[1])
		return;
#else
	return;
#endif

	HWND hW;
	CRect rt;
	int nCtrlIdCad, nCtrlIdDef;

	FreeMkInfoUp();
	for(int nIdxMkInfo=0; nIdxMkInfo<DEF_VIEW_IMG_NUMBER/2; nIdxMkInfo++)
	{
		switch(nIdxMkInfo)
		{
		case 0:
			nCtrlIdCad = IDC_PIC_CAD_007;
			nCtrlIdDef = IDC_PIC_DEF_007;
			break;
		case 1:
			nCtrlIdCad = IDC_PIC_CAD_008;
			nCtrlIdDef = IDC_PIC_DEF_008;
			break;
		case 2:
			nCtrlIdCad = IDC_PIC_CAD_009;
			nCtrlIdDef = IDC_PIC_DEF_009;
			break;
		case 3:
			nCtrlIdCad = IDC_PIC_CAD_010;
			nCtrlIdDef = IDC_PIC_DEF_010;
			break;
		case 4:
			nCtrlIdCad = IDC_PIC_CAD_011;
			nCtrlIdDef = IDC_PIC_DEF_011;
			break;
		case 5:
			nCtrlIdCad = IDC_PIC_CAD_012;
			nCtrlIdDef = IDC_PIC_DEF_012;
			break;
		}

		hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
		GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
#ifdef USE_VISION
		pView->m_pVisionInner[1]->SelDispCad(hW, rt, nIdxMkInfo);
		pView->m_pVisionInner[1]->SetOvrCadFontSz(nIdxMkInfo);

		hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
		GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
		pView->m_pVisionInner[1]->SelDispDef(hW, rt, nIdxMkInfo);
#endif
	}

}

void CDlgMenu06::FreeMkInfoUp()
{
#ifdef USE_VISION
	if (!pView->m_pVisionInner[0])
		return;
#else
	return;
#endif

	HWND hW;
	CRect rt;
	int nCtrlIdCad, nCtrlIdDef;
	const int nSt = DEF_VIEW_IMG_NUMBER / 2;
	for (int nIdxMkInfo = nSt; nIdxMkInfo < DEF_VIEW_IMG_NUMBER; nIdxMkInfo++)
	{
		switch (nIdxMkInfo)
		{
		case nSt:
			nCtrlIdCad = IDC_PIC_CAD_007;
			nCtrlIdDef = IDC_PIC_DEF_007;
			break;
		case nSt+1:
			nCtrlIdCad = IDC_PIC_CAD_008;
			nCtrlIdDef = IDC_PIC_DEF_008;
			break;
		case nSt+2:
			nCtrlIdCad = IDC_PIC_CAD_009;
			nCtrlIdDef = IDC_PIC_DEF_009;
			break;
		case nSt+3:
			nCtrlIdCad = IDC_PIC_CAD_010;
			nCtrlIdDef = IDC_PIC_DEF_010;
			break;
		case nSt+4:
			nCtrlIdCad = IDC_PIC_CAD_011;
			nCtrlIdDef = IDC_PIC_DEF_011;
			break;
		case nSt+5:
			nCtrlIdCad = IDC_PIC_CAD_012;
			nCtrlIdDef = IDC_PIC_DEF_012;
			break;
		}

#ifdef USE_VISION
		hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
		GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
		pView->m_pVisionInner[0]->FreeDispCad(hW, rt, nIdxMkInfo);

		hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
		GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
		pView->m_pVisionInner[0]->FreeDispDef(hW, rt, nIdxMkInfo);
#endif
	}

}

void CDlgMenu06::FreeMkInfoDn()
{
#ifdef USE_VISION
	if (!pView->m_pVisionInner[1])
		return;
#else
	return;
#endif

	HWND hW;
	CRect rt;
	int nCtrlIdCad, nCtrlIdDef;

	for (int nIdxMkInfo = 0; nIdxMkInfo < DEF_VIEW_IMG_NUMBER / 2; nIdxMkInfo++)
	{
		switch (nIdxMkInfo)
		{
		case 0:
			nCtrlIdCad = IDC_PIC_CAD_007;
			nCtrlIdDef = IDC_PIC_DEF_007;
			break;
		case 1:
			nCtrlIdCad = IDC_PIC_CAD_008;
			nCtrlIdDef = IDC_PIC_DEF_008;
			break;
		case 2:
			nCtrlIdCad = IDC_PIC_CAD_009;
			nCtrlIdDef = IDC_PIC_DEF_009;
			break;
		case 3:
			nCtrlIdCad = IDC_PIC_CAD_010;
			nCtrlIdDef = IDC_PIC_DEF_010;
			break;
		case 4:
			nCtrlIdCad = IDC_PIC_CAD_011;
			nCtrlIdDef = IDC_PIC_DEF_011;
			break;
		case 5:
			nCtrlIdCad = IDC_PIC_CAD_012;
			nCtrlIdDef = IDC_PIC_DEF_012;
			break;
		}

#ifdef USE_VISION
		hW = GetDlgItem(nCtrlIdCad)->GetSafeHwnd();
		GetDlgItem(nCtrlIdCad)->GetWindowRect(&rt);
		pView->m_pVisionInner[1]->FreeDispCad(hW, rt, nIdxMkInfo);

		hW = GetDlgItem(nCtrlIdDef)->GetSafeHwnd();
		GetDlgItem(nCtrlIdDef)->GetWindowRect(&rt);
		pView->m_pVisionInner[1]->FreeDispDef(hW, rt, nIdxMkInfo);
#endif
	}

}

void CDlgMenu06::ResetMkInfo()
{
	m_nIdxMkInfo[0] = 0;
	m_nIdxMkInfo[1] = 0;
	ClrCadImg();
	ClrDefImg();
	InitDefInfo();
	ClrInfo();


	int nCtrlIdCad, nCtrlIdDef;
	for(int nIdxMkInfo=0; nIdxMkInfo<DEF_VIEW_IMG_NUMBER; nIdxMkInfo++)
	{
		switch(nIdxMkInfo)
		{
		case 0:
			nCtrlIdCad = IDC_PIC_CAD_001;
			nCtrlIdDef = IDC_PIC_DEF_001;
			break;
		case 1:
			nCtrlIdCad = IDC_PIC_CAD_002;
			nCtrlIdDef = IDC_PIC_DEF_002;
			break;
		case 2:
			nCtrlIdCad = IDC_PIC_CAD_003;
			nCtrlIdDef = IDC_PIC_DEF_003;
			break;
		case 3:
			nCtrlIdCad = IDC_PIC_CAD_004;
			nCtrlIdDef = IDC_PIC_DEF_004;
			break;
		case 4:
			nCtrlIdCad = IDC_PIC_CAD_005;
			nCtrlIdDef = IDC_PIC_DEF_005;
			break;
		case 5:
			nCtrlIdCad = IDC_PIC_CAD_006;
			nCtrlIdDef = IDC_PIC_DEF_006;
			break;
		case 6:
			nCtrlIdCad = IDC_PIC_CAD_007;
			nCtrlIdDef = IDC_PIC_DEF_007;
			break;
		case 7:
			nCtrlIdCad = IDC_PIC_CAD_008;
			nCtrlIdDef = IDC_PIC_DEF_008;
			break;
		case 8:
			nCtrlIdCad = IDC_PIC_CAD_009;
			nCtrlIdDef = IDC_PIC_DEF_009;
			break;
		case 9:
			nCtrlIdCad = IDC_PIC_CAD_010;
			nCtrlIdDef = IDC_PIC_DEF_010;
			break;
		case 10:
			nCtrlIdCad = IDC_PIC_CAD_011;
			nCtrlIdDef = IDC_PIC_DEF_011;
			break;
		case 11:
			nCtrlIdCad = IDC_PIC_CAD_012;
			nCtrlIdDef = IDC_PIC_DEF_012;
			break;
		}

		GetDlgItem(nCtrlIdCad)->RedrawWindow();
		GetDlgItem(nCtrlIdDef)->RedrawWindow();
	}

}

void CDlgMenu06::ResetMkInfoUp()
{
	m_nIdxMkInfo[0] = 0;
	ClrCadImgUp();
	ClrDefImgUp();
	InitDefInfo();
	ClrInfo();
}

void CDlgMenu06::ResetMkInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

	m_nIdxMkInfo[1] = 0;
	ClrCadImgDn();
	ClrDefImgDn();
	InitDefInfo();
	ClrInfo();
}

void CDlgMenu06::InitCadImg()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
#ifdef USE_VISION
	if(pView->m_pVisionInner[0])
	{
		pView->m_pVisionInner[0]->InitCADBuf(0); // Top Side
	}

	if(bDualTest)
	{
		if(pView->m_pVisionInner[1])
		{
			pView->m_pVisionInner[1]->InitCADBuf(1); // Bottom Side
		}
	}
#endif
}

void CDlgMenu06::InitCadImgUp()
{
#ifdef USE_VISION
	if(pView->m_pVisionInner[0])
	{
		pView->m_pVisionInner[0]->InitCADBuf(0);
	}
#endif
}

void CDlgMenu06::InitCadImgDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

#ifdef USE_VISION
	if(pView->m_pVisionInner[1])
	{
		pView->m_pVisionInner[1]->InitCADBuf(1);
	}
#endif
}

void CDlgMenu06::InitDefImg()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

#ifdef USE_VISION
	if(pView->m_pVisionInner[0])
		pView->m_pVisionInner[0]->InitDispDef();

	if(bDualTest)
	{
		if(pView->m_pVisionInner[1])
			pView->m_pVisionInner[1]->InitDispDef();
	}
#endif
}

void CDlgMenu06::InitDefImgUp()
{
#ifdef USE_VISION
	if(pView->m_pVisionInner[0])
		pView->m_pVisionInner[0]->InitDispDef();
#endif
}

void CDlgMenu06::InitDefImgDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

#ifdef USE_VISION
	if(pView->m_pVisionInner[1])
		pView->m_pVisionInner[1]->InitDispDef();
#endif
}

void CDlgMenu06::InitDefInfo()
{
	for(int nIdx=0; nIdx<DEF_VIEW_IMG_NUMBER; nIdx++)
	{
		myStcDefInfo[nIdx].SetText(_T(""));
		myStcDefInfo[nIdx].SetBkColor(RGB_LTDKORANGE);
	}
}

void CDlgMenu06::InitDefInfoUp()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if(bDualTest)
	{
		for(int nIdx=0; nIdx<DEF_VIEW_IMG_NUMBER/2; nIdx++)
		{
			myStcDefInfo[nIdx].SetText(_T(""));
			myStcDefInfo[nIdx].SetBkColor(RGB_LTDKORANGE);
		}
	}
	else
	{
		for(int nIdx=0; nIdx<DEF_VIEW_IMG_NUMBER; nIdx++)
		{
			myStcDefInfo[nIdx].SetText(_T(""));
			myStcDefInfo[nIdx].SetBkColor(RGB_LTDKORANGE);
		}
	}
}

void CDlgMenu06::InitDefInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

	for(int nIdx=DEF_VIEW_IMG_NUMBER/2; nIdx<DEF_VIEW_IMG_NUMBER; nIdx++)
	{
		myStcDefInfo[nIdx].SetText(_T(""));
		myStcDefInfo[nIdx].SetBkColor(RGB_LTDKORANGE);
	}
}

void CDlgMenu06::ClrCadImg()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

#ifdef USE_VISION
	if(pView->m_pVisionInner[0])
		pView->m_pVisionInner[0]->ClrDispCad();

	if(bDualTest)
	{
		if(pView->m_pVisionInner[1])
			pView->m_pVisionInner[1]->ClrDispCad();
	}
#endif
}

void CDlgMenu06::ClrCadImgUp()
{
#ifdef USE_VISION
	if(pView->m_pVisionInner[0])
		pView->m_pVisionInner[0]->ClrDispCad();
#endif
}

void CDlgMenu06::ClrCadImgDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

#ifdef USE_VISION
	if(pView->m_pVisionInner[1])
		pView->m_pVisionInner[1]->ClrDispCad();
#endif
}

void CDlgMenu06::ClrDefImg()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

#ifdef USE_VISION
	if(pView->m_pVisionInner[0])
		pView->m_pVisionInner[0]->ClrDispDef();

	if(bDualTest)
	{
		if(pView->m_pVisionInner[1])
			pView->m_pVisionInner[1]->ClrDispDef();
	}
#endif
}

void CDlgMenu06::ClrDefImgUp()
{
#ifdef USE_VISION
	if(pView->m_pVisionInner[0])
		pView->m_pVisionInner[0]->ClrDispDef();
#endif
}

void CDlgMenu06::ClrDefImgDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

#ifdef USE_VISION
	if(pView->m_pVisionInner[1])
		pView->m_pVisionInner[1]->ClrDispDef();
#endif
}

// void CDlgMenu06::ClrRmap()
// {
// 
// }

void CDlgMenu06::DispMkInfo(int nSerial)
{
	if(nSerial <= 0)
	{
		pView->MsgBox(_T("Serial Error.2"));
		//AfxMessageBox(_T("Serial Error.2"));
		return;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	DispMkInfoUp(nSerial);
	if(bDualTest)
		DispMkInfoDn(nSerial);
}

void CDlgMenu06::DispMkInfoUp(int nSerial)
{
	if(nSerial <= 0)
	{
		pView->MsgBox(_T("Serial Error.3"));
		//AfxMessageBox(_T("Serial Error.3"));
		return;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	
#ifdef USE_VISION
	if(bDualTest)
	{
		if(pView->m_pVisionInner[0])
		{
 			int nIdxMkInfo = (m_nIdxMkInfo[0] < MENU01_STC_DEFINFO_HARF) ? m_nIdxMkInfo[0] : (MENU01_STC_DEFINFO_HARF-1);	

			int nIdx = pDoc->GetPcrIdx0(nSerial);
			int nDefImg;
			if(pDoc->m_pPcrInner[0])
			{
				if(pDoc->m_pPcrInner[0][nIdx])
				{
					if(pDoc->m_pPcrInner[0][nIdx]->m_pImg)
					{
						if(pDoc->m_pPcrInner[0][nIdx]->m_pMk[m_nIdxDef[0]] != -2) // -2 (NoMarking)
						{	
							if(m_nIdxDef[0] < pDoc->m_pPcrInner[0][nIdx]->m_nTotDef)
							{
								if(m_nIdxMkInfo[0] >= MENU01_STC_DEFINFO_HARF)
									ShiftDefInfoUp();
 								pView->m_pVisionInner[0]->ShowDispCad(nIdxMkInfo, nSerial, 0, m_nIdxDef[0]);
								pView->m_pVisionInner[0]->ShowOvrCad(nIdxMkInfo, nSerial);
								nDefImg = pDoc->m_pPcrInner[0][nIdx]->m_pImg[m_nIdxDef[0]];
								pView->m_pVisionInner[0]->ShowDispDef(nIdxMkInfo, nSerial, 0, nDefImg);
								ShowDefInfoUp(nIdxMkInfo);
								m_nIdxMkInfo[0]++;
								m_nIdxDef[0]++;
								//(pDoc->m_pPcrInner[0][nIdx]->m_nTotRealDef)++;
							}
							else
								m_nIdxMkInfo[0]++;
						}
						else
							m_nIdxDef[0]++;
					}
				}
			}
		}
	}
	else
	{
		if(pView->m_pVisionInner[0])
		{
 			int nIdxMkInfo = (m_nIdxMkInfo[0] < MENU01_STC_DEFINFO_HARF*2) ? m_nIdxMkInfo[0] : (MENU01_STC_DEFINFO_HARF*2-1);	

			int nIdx = pDoc->GetPcrIdx0(nSerial);
			int nDefImg;
			if(pDoc->m_pPcrInner[0])
			{
				if(pDoc->m_pPcrInner[0][nIdx])
				{
					if(pDoc->m_pPcrInner[0][nIdx]->m_pImg)
					{
						if(pDoc->m_pPcrInner[0][nIdx]->m_pMk[m_nIdxDef[0]] != -2) // -2 (NoMarking)
						{	
							if(m_nIdxDef[0] < pDoc->m_pPcrInner[0][nIdx]->m_nTotDef)
							{
								if(m_nIdxMkInfo[0] >= MENU01_STC_DEFINFO_HARF*2)
									ShiftDefInfoUp();
 								pView->m_pVisionInner[0]->ShowDispCad(nIdxMkInfo, nSerial, 0, m_nIdxDef[0]);
								pView->m_pVisionInner[0]->ShowOvrCad(nIdxMkInfo, nSerial);
								nDefImg = pDoc->m_pPcrInner[0][nIdx]->m_pImg[m_nIdxDef[0]];
								pView->m_pVisionInner[0]->ShowDispDef(nIdxMkInfo, nSerial, 0, nDefImg);
								ShowDefInfoUp(nIdxMkInfo);
								m_nIdxMkInfo[0]++;
								m_nIdxDef[0]++;
								//(pDoc->m_pPcrInner[0][nIdx]->m_nTotRealDef)++;
							}
							else
								m_nIdxMkInfo[0]++;
						}
						else
							m_nIdxDef[0]++;
					}
				}
			}
		}
	}
#endif
}

void CDlgMenu06::DispMkInfoDn(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

	if(nSerial <= 0)
	{
		pView->MsgBox(_T("Serial Error.4"));
		//AfxMessageBox(_T("Serial Error.4"));
		return;
	}

#ifdef USE_VISION
	if(pView->m_pVisionInner[1])
	{
 		int nIdxMkInfo = (m_nIdxMkInfo[1] < MENU01_STC_DEFINFO_HARF) ? m_nIdxMkInfo[1] : (MENU01_STC_DEFINFO_HARF-1);	

		int nIdx = pDoc->GetPcrIdx1(nSerial);
		int nDefImg;
		if(pDoc->m_pPcrInner[1])
		{
			if(pDoc->m_pPcrInner[1][nIdx])
			{
				if(pDoc->m_pPcrInner[1][nIdx]->m_pImg)
				{
					if(pDoc->m_pPcrInner[1][nIdx]->m_pMk[m_nIdxDef[1]] != -2) // -2 (NoMarking)
					{	
						if(m_nIdxDef[1] < pDoc->m_pPcrInner[1][nIdx]->m_nTotDef)
						{
 							if(m_nIdxMkInfo[1] >= MENU01_STC_DEFINFO_HARF)
								ShiftDefInfoDn();
 							pView->m_pVisionInner[1]->ShowDispCad(nIdxMkInfo, nSerial, 1, m_nIdxDef[1]);
							pView->m_pVisionInner[1]->ShowOvrCad(nIdxMkInfo, nSerial);
							nDefImg = pDoc->m_pPcrInner[1][nIdx]->m_pImg[m_nIdxDef[1]];
							pView->m_pVisionInner[1]->ShowDispDef(nIdxMkInfo, nSerial, 1, nDefImg);
							ShowDefInfoDn(nIdxMkInfo);
							m_nIdxMkInfo[1]++;
							m_nIdxDef[1]++;
							//(pDoc->m_pPcrInner[1][nIdx]->m_nTotRealDef)++;
						}
						else
							m_nIdxMkInfo[1]++;
					}
					else
						m_nIdxDef[1]++;
				}
			}
		}
	}
#endif
}

void CDlgMenu06::ShiftDefInfoUp()
{
#ifdef USE_VISION
	if(!pView->m_pVisionInner[0])
		return;

	pView->m_pVisionInner[0]->ShiftDisp();

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	CString str;
	COLORREF rgbDef;

	if(bDualTest)
	{
		for(int i=0; i<MENU01_STC_DEFINFO_HARF-1; i++)
		{
			str = myStcDefInfo[i+1].GetText();
			rgbDef = myStcDefInfo[i+1].GetBkColor();
			myStcDefInfo[i].SetText(str);
			myStcDefInfo[i].SetBkColor(rgbDef);
		}
	}
	else
	{
		for(int i=0; i<MENU01_STC_DEFINFO_HARF*2-1; i++)
		{
			str = myStcDefInfo[i+1].GetText();
			rgbDef = myStcDefInfo[i+1].GetBkColor();
			myStcDefInfo[i].SetText(str);
			myStcDefInfo[i].SetBkColor(rgbDef);
		}
	}
#endif
}

void CDlgMenu06::ShiftDefInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

#ifdef USE_VISION
	if(!pView->m_pVisionInner[1])
		return;

	pView->m_pVisionInner[1]->ShiftDisp();

	CString str;
	COLORREF rgbDef;
	for(int i=0; i<MENU01_STC_DEFINFO_HARF-1; i++)
	{
		str = myStcDefInfo[i+1+MENU01_STC_DEFINFO_HARF].GetText();
		rgbDef = myStcDefInfo[i+1+MENU01_STC_DEFINFO_HARF].GetBkColor();
		myStcDefInfo[i+MENU01_STC_DEFINFO_HARF].SetText(str);
		myStcDefInfo[i+MENU01_STC_DEFINFO_HARF].SetBkColor(rgbDef);
	}
#endif
}

void CDlgMenu06::ShowDefInfoUp(int nIdx) // nIdx : 0 ~ 11 (12ea)
{
	if(!pView->m_bDrawGL_Menu06)
		return;
	if(!pDoc->m_pReelMapInner)
	{
		pView->MsgBox(_T("Not created Reelmap."));
		//AfxMessageBox(_T("Not created Reelmap."));
		return;
	}

	CString str;
	int nDefCode, nStrip, nCol, nRow;
	COLORREF rgbDef;

#ifdef TEST_MODE 
 	int nPcrIdx = pDoc->GetPcrIdx0(m_nSerial);
	if(pDoc->m_pPcrInner[0][nPcrIdx]->m_pMk[m_nIdxDef[0]] == -2) // -2 (NoMarking)
		return;
	int nPcsIdx = pDoc->m_pPcrInner[0][nPcrIdx]->m_pDefPcs[m_nIdxDef[0]]; //m_pImg[m_nIdxDef];
	nDefCode = pDoc->m_pPcrInner[0][nPcrIdx]->m_pDefType[m_nIdxDef[0]];
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[nDefCode];
	if(pDoc->m_Master[0].m_pPcsRgn)
		pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[0].MasterInfo.nActionCode, nPcsIdx, nStrip, nCol, nRow);
	str.Format(_T("%s\r\n%c - %d, %d"), pDoc->m_pReelMapInner->m_sKorDef[nDefCode], nStrip+'A', nCol+1, nRow+1);
#else
 	int nPcrIdx = pDoc->GetPcrIdx0(m_nSerial);
	if (MODE_OUTER != pDoc->GetTestMode())
	{
		if (pDoc->m_pPcrInner[0][nPcrIdx]->m_pMk[m_nIdxDef[0]] == -2) // -2 (NoMarking)
			return;
	}
	else
	{
		if (pDoc->m_pPcrInner[0][nPcrIdx]->m_pMk[m_nIdxDef[0]] == -2) // -2 (NoMarking)
			return;
	}
	int nPcsIdx = pDoc->m_pPcrInner[0][nPcrIdx]->m_pDefPcs[m_nIdxDef[0]]; //m_pImg[m_nIdxDef];
	nDefCode = pDoc->m_pPcrInner[0][nPcrIdx]->m_pDefType[m_nIdxDef[0]];
 	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[nDefCode];	
	if(pDoc->m_Master[0].m_pPcsRgn)
		pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[0].MasterInfo.nActionCode, nPcsIdx, nStrip, nCol, nRow);
	str.Format(_T("%s\r\n%c - %d, %d"), pDoc->m_pReelMapInner->m_sKorDef[nDefCode], nStrip+'A', nCol+1, nRow+1);
#endif
	myStcDefInfo[nIdx].SetText(str);
	myStcDefInfo[nIdx].SetBkColor(rgbDef);
}

void CDlgMenu06::ShowDefInfoDn(int nIdx) // nIdx : 0 ~ 11 (12ea)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

	if(!pView->m_bDrawGL_Menu06)
		return;
	if(!pDoc->m_pReelMapInner)
	{
		pView->MsgBox(_T("Not created Reelmap."));
		return;
	}

	CString str;
	int nDefCode, nStrip, nCol, nRow;
	COLORREF rgbDef;

#ifdef TEST_MODE 
 	int nPcrIdx = pDoc->GetPcrIdx1(m_nSerial);
	if(pDoc->m_pPcrInner[1][nPcrIdx]->m_pMk[m_nIdxDef[1]] == -2) // -2 (NoMarking)
		return;
	int nPcsIdx = pDoc->m_pPcrInner[1][nPcrIdx]->m_pDefPcs[m_nIdxDef[1]]; //m_pImg[m_nIdxDef];
	nDefCode = pDoc->m_pPcrInner[1][nPcrIdx]->m_pDefType[m_nIdxDef[1]];
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[nDefCode];
	if(pDoc->m_Master[0].m_pPcsRgn)
		pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[1].MasterInfo.nActionCode, nPcsIdx, nStrip, nCol, nRow);
	str.Format(_T("%s\r\n%c - %d, %d"), pDoc->m_pReelMapInner->m_sKorDef[nDefCode], nStrip+'A', nCol+1, nRow+1);
#else
 	int nPcrIdx = pDoc->GetPcrIdx1(m_nSerial);
	if (MODE_OUTER != pDoc->GetTestMode())
	{
		if (pDoc->m_pPcrInner[1][nPcrIdx]->m_pMk[m_nIdxDef[1]] == -2) // -2 (NoMarking)
			return;
	}
	else
	{
		if (pDoc->m_pPcrInner[1][nPcrIdx]->m_pMk[m_nIdxDef[1]] == -2) // -2 (NoMarking)
			return;
	}
	int nPcsIdx = pDoc->m_pPcrInner[1][nPcrIdx]->m_pDefPcs[m_nIdxDef[1]]; //m_pImg[m_nIdxDef];
	nDefCode = pDoc->m_pPcrInner[1][nPcrIdx]->m_pDefType[m_nIdxDef[1]];
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[nDefCode];	
	if(pDoc->m_Master[0].m_pPcsRgn)
		pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[1].MasterInfo.nActionCode, nPcsIdx, nStrip, nCol, nRow);
	str.Format(_T("%s\r\n%c - %d, %d"), pDoc->m_pReelMapInner->m_sKorDef[nDefCode], nStrip+'A', nCol+1, nRow+1);
#endif
	myStcDefInfo[MENU01_STC_DEFINFO_HARF+nIdx].SetText(str);
	myStcDefInfo[MENU01_STC_DEFINFO_HARF+nIdx].SetBkColor(rgbDef);
}

void CDlgMenu06::InitGL()
{
	if (!pDoc->m_pReelMapInner)
		return;

	if (m_pMyGL)
	{
		delete m_pMyGL;
		m_pMyGL = NULL;
	}

	if(!m_pMyGL)
	{
		m_pMyGL = new CMyGL(this);
		m_pMyGL->Init(IDC_STC_REELMAP_INNER, pDoc->m_pReelMapInner);
	}

	m_pMyGL->SetRgn();
	m_pMyGL->SetPcsDef();
}

void CDlgMenu06::ResetGL()
{
	if(m_pMyGL)
		m_pMyGL->ResetRgn();
}

void CDlgMenu06::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_CHK_TP_STOP, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_TP_STOP);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);
	myBtn[0].SetText(_T("On"), BTN_DN);
	myBtn[0].SetText(_T("Off"), BTN_UP);

	myBtn[1].SubclassDlgItem(IDC_CHK_USER_INFO, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USER_INFO);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	myBtn[2].SubclassDlgItem(IDC_CHK_REVIEW, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_REVIEW);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	myBtn[3].SubclassDlgItem(IDC_CHK_EJECT_BUFFER, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_EJECT_BUFFER);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	myBtn[4].SubclassDlgItem(IDC_CHK_REMARKING, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_REMARKING);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_CHK_REVIEW_MOVE, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_REVIEW_MOVE);
	myBtn[5].SetBtnType(BTN_TYPE_CHECK);
	
	myBtn[6].SubclassDlgItem(IDC_CHK_LOT_END, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_LOT_END);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);
	
	myBtn[7].SubclassDlgItem(IDC_CHK_MK_1, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_1);
	myBtn[7].SetBtnType(BTN_TYPE_CHECK);
	
	myBtn[8].SubclassDlgItem(IDC_CHK_MK_2, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_2);
	myBtn[8].SetBtnType(BTN_TYPE_CHECK);
	
	myBtn[9].SubclassDlgItem(IDC_CHK_MK_3, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_3);
	myBtn[9].SetBtnType(BTN_TYPE_CHECK);
	
	myBtn[10].SubclassDlgItem(IDC_CHK_MK_4, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_MK_4);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);
	
	myBtn[11].SubclassDlgItem(IDC_BTN_MK_ALL, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_BTN_MK_ALL);
	
	myBtn[12].SubclassDlgItem(IDC_CHK_DEF_UP, this);
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_DEF_UP);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);
	
	myBtn[13].SubclassDlgItem(IDC_CHK_DEF_DN, this);
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_DEF_DN);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);
	
	myBtn[14].SubclassDlgItem(IDC_CHK_DEF_ALL, this);
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_DEF_ALL);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);
	
	myBtn[15].SubclassDlgItem(IDC_BTN_ST, this);
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_BTN_ST);

	myBtn[16].SubclassDlgItem(IDC_BTN_GD_RA_1, this);
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_1);

	myBtn[17].SubclassDlgItem(IDC_BTN_GD_RA_2, this);
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_2);

	myBtn[18].SubclassDlgItem(IDC_BTN_GD_RA_3, this);
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_3);

	myBtn[19].SubclassDlgItem(IDC_BTN_GD_RA_4, this);
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_4);

	myBtn[20].SubclassDlgItem(IDC_BTN_GD_RA_ALL, this);
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_BTN_GD_RA_ALL);

	myBtn[21].SubclassDlgItem(IDC_CHK_ELEC, this);
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_ELEC);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);
	myBtn[21].ShowWindow(SW_HIDE);

	myBtn[22].SubclassDlgItem(IDC_CHK_ELEC_STOP, this);
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_CHK_ELEC_STOP);
	myBtn[22].SetBtnType(BTN_TYPE_CHECK);
	myBtn[22].ShowWindow(SW_HIDE);

	int i;
	for(i=0; i<MAX_MENU06_BTN; i++)
	{
		if(i==1 || i==2)
			myBtn[i].SetFont(_T("굴림체"),12,TRUE);
		else if(i==7 || i==8 || i==9 || i==10 || i==11)
			myBtn[i].SetFont(_T("굴림체"),11,TRUE);
		else if(i==12 || i==13 || i==14)
			myBtn[i].SetFont(_T("굴림체"),12,TRUE);
		else if(i==15 || i==16 || i==17 || i==18 || i==19 || i==20 || i==21)
		{
			myBtn[i].SetFont(_T("굴림체"),11,TRUE);
		}
		else
			myBtn[i].SetFont(_T("굴림체"),12,TRUE);

		if(i==15 || i==20)
			myBtn[i].SetTextColor(RGB_WHITE);
		else
			myBtn[i].SetTextColor(RGB_BLACK);
		
		myBtn[i].SetBtnType(BTN_TYPE_CHECK);
	}
}

void CDlgMenu06::InitStatic()
{
	myStcReelmap.SubclassDlgItem(IDC_STC_REELMAP_INNER, this);
	myStcReelmap.SetHwnd(this->GetSafeHwnd(), IDC_STC_REELMAP_INNER);

	InitStcDefInf();
	InitStcTitle();
	InitStcData();
}

void CDlgMenu06::InitStcDefInf()
{
	myStcDefInfo[0].SubclassDlgItem(IDC_EDIT_DEF_INFO_001, this);
	myStcDefInfo[1].SubclassDlgItem(IDC_EDIT_DEF_INFO_002, this);
	myStcDefInfo[2].SubclassDlgItem(IDC_EDIT_DEF_INFO_003, this);
	myStcDefInfo[3].SubclassDlgItem(IDC_EDIT_DEF_INFO_004, this);
	myStcDefInfo[4].SubclassDlgItem(IDC_EDIT_DEF_INFO_005, this);
	myStcDefInfo[5].SubclassDlgItem(IDC_EDIT_DEF_INFO_006, this);
	myStcDefInfo[6].SubclassDlgItem(IDC_EDIT_DEF_INFO_007, this);
	myStcDefInfo[7].SubclassDlgItem(IDC_EDIT_DEF_INFO_008, this);
	myStcDefInfo[8].SubclassDlgItem(IDC_EDIT_DEF_INFO_009, this);
	myStcDefInfo[9].SubclassDlgItem(IDC_EDIT_DEF_INFO_010, this);
	myStcDefInfo[10].SubclassDlgItem(IDC_EDIT_DEF_INFO_011, this);
	myStcDefInfo[11].SubclassDlgItem(IDC_EDIT_DEF_INFO_012, this);

	for(int i=0; i<MAX_MENU06_STC_DEFINFO; i++)
	{
		myStcDefInfo[i].SetFontName(_T("Arial"));
		myStcDefInfo[i].SetFontSize(12);
		myStcDefInfo[i].SetFontBold(FALSE);
		myStcDefInfo[i].SetTextColor(RGB_BLACK);
		myStcDefInfo[i].SetBkColor(RGB_LTDKORANGE);
	}
}

void CDlgMenu06::InitStcData()
{
	myStcData[0].SubclassDlgItem(IDC_STC_OPERATOR, this);
	myStcData[1].SubclassDlgItem(IDC_STC_MODEL, this);
	myStcData[2].SubclassDlgItem(IDC_STC_LAYER, this);
	myStcData[3].SubclassDlgItem(IDC_STC_LOT, this);
	myStcData[4].SubclassDlgItem(IDC_STC_TOTAL_REEL, this);
	myStcData[5].SubclassDlgItem(IDC_STC_WORK_RATIO, this);
	myStcData[6].SubclassDlgItem(IDC_STC_LOT_RATIO, this);
	myStcData[7].SubclassDlgItem(IDC_STC_TOTAL_SPEED, this);
	myStcData[8].SubclassDlgItem(IDC_STC_CYCLE_SPEED, this);
	myStcData[9].SubclassDlgItem(IDC_STC_TEMP_STOP, this);
	myStcData[10].SubclassDlgItem(IDC_STC_LOT_SEP, this);
	myStcData[11].SubclassDlgItem(IDC_STC_LOT_LEN, this);
	myStcData[12].SubclassDlgItem(IDC_STC_MK, this);
	myStcData[13].SubclassDlgItem(IDC_STC_AOI_DN, this);
	myStcData[14].SubclassDlgItem(IDC_STC_LOT_SRL, this);
	myStcData[15].SubclassDlgItem(IDC_STC_DEFECT_NUM_UP, this);
	myStcData[16].SubclassDlgItem(IDC_STC_DEFECT_RATIO_UP, this);
	myStcData[17].SubclassDlgItem(IDC_STC_GOOD_NUM_UP, this);
	myStcData[18].SubclassDlgItem(IDC_STC_GOOD_RATIO_UP, this);
	myStcData[19].SubclassDlgItem(IDC_STC_TOTAL_NUM_UP, this);
	myStcData[20].SubclassDlgItem(IDC_STC_REVIEW_LEN, this);
	myStcData[21].SubclassDlgItem(IDC_STC_LOT_START, this);
	myStcData[22].SubclassDlgItem(IDC_STC_LOT_RUN, this);
	myStcData[23].SubclassDlgItem(IDC_STC_LOT_END, this);
	myStcData[24].SubclassDlgItem(IDC_STC_DEF_OPEN, this);
	myStcData[25].SubclassDlgItem(IDC_STC_DEF_SHORT, this);
	myStcData[26].SubclassDlgItem(IDC_STC_DEF_U_SHORT, this);
	myStcData[27].SubclassDlgItem(IDC_STC_DEF_SPACE, this);
	myStcData[28].SubclassDlgItem(IDC_STC_DEF_EXTRA, this);
	myStcData[29].SubclassDlgItem(IDC_STC_DEF_PROT, this);
	myStcData[30].SubclassDlgItem(IDC_STC_DEF_P_HOLE, this);
	myStcData[31].SubclassDlgItem(IDC_STC_DEF_PAD, this);
	myStcData[32].SubclassDlgItem(IDC_STC_DEF_H_OPEN, this);
	myStcData[33].SubclassDlgItem(IDC_STC_DEF_H_MISS, this);
	myStcData[34].SubclassDlgItem(IDC_STC_DEF_H_POS, this);
	myStcData[35].SubclassDlgItem(IDC_STC_DEF_H_DEF, this);
	myStcData[36].SubclassDlgItem(IDC_STC_DEF_NICK, this);
	myStcData[37].SubclassDlgItem(IDC_STC_DEF_POI, this);
	myStcData[38].SubclassDlgItem(IDC_STC_DEF_VH_OPEN, this);
	myStcData[39].SubclassDlgItem(IDC_STC_DEF_VH_MISS, this);
	myStcData[40].SubclassDlgItem(IDC_STC_DEF_VH_POS, this);
	myStcData[41].SubclassDlgItem(IDC_STC_DEF_VH_DEF, this);
	myStcData[42].SubclassDlgItem(IDC_STC_DEF_LIGHT, this);
	myStcData[43].SubclassDlgItem(IDC_STC_DEF_E_NICK, this);
	myStcData[44].SubclassDlgItem(IDC_STC_DEF_E_PROT, this);
	myStcData[45].SubclassDlgItem(IDC_STC_DEF_E_SPACE, this);
	myStcData[46].SubclassDlgItem(IDC_STC_DEF_UDEF1, this);
	myStcData[47].SubclassDlgItem(IDC_STC_DEF_NARROW, this);
	myStcData[48].SubclassDlgItem(IDC_STC_DEF_WIDE, this);

	myStcData[49].SubclassDlgItem(IDC_STC_DEFECT_NUM_DN, this);
	myStcData[50].SubclassDlgItem(IDC_STC_DEFECT_RATIO_DN, this);
	myStcData[51].SubclassDlgItem(IDC_STC_GOOD_NUM_DN, this);
	myStcData[52].SubclassDlgItem(IDC_STC_GOOD_RATIO_DN, this);
	myStcData[53].SubclassDlgItem(IDC_STC_TOTAL_NUM_DN, this);

	myStcData[54].SubclassDlgItem(IDC_STC_DEFECT_NUM_ALL, this);
	myStcData[55].SubclassDlgItem(IDC_STC_DEFECT_RATIO_ALL, this);
	myStcData[56].SubclassDlgItem(IDC_STC_GOOD_NUM_ALL, this);
	myStcData[57].SubclassDlgItem(IDC_STC_GOOD_RATIO_ALL, this);
	myStcData[58].SubclassDlgItem(IDC_STC_TOTAL_NUM_ALL, this);

	myStcData[59].SubclassDlgItem(IDC_STC_GD_RA_1_UP, this);
	myStcData[60].SubclassDlgItem(IDC_STC_GD_RA_2_UP, this);
	myStcData[61].SubclassDlgItem(IDC_STC_GD_RA_3_UP, this);
	myStcData[62].SubclassDlgItem(IDC_STC_GD_RA_4_UP, this);
	myStcData[63].SubclassDlgItem(IDC_STC_GD_RA_ALL_UP, this);

	myStcData[64].SubclassDlgItem(IDC_STC_GD_RA_1_DN, this);
	myStcData[65].SubclassDlgItem(IDC_STC_GD_RA_2_DN, this);
	myStcData[66].SubclassDlgItem(IDC_STC_GD_RA_3_DN, this);
	myStcData[67].SubclassDlgItem(IDC_STC_GD_RA_4_DN, this);
	myStcData[68].SubclassDlgItem(IDC_STC_GD_RA_ALL_DN, this);

	myStcData[69].SubclassDlgItem(IDC_STC_GD_RA_1_ALL, this);
	myStcData[70].SubclassDlgItem(IDC_STC_GD_RA_2_ALL, this);
	myStcData[71].SubclassDlgItem(IDC_STC_GD_RA_3_ALL, this);
	myStcData[72].SubclassDlgItem(IDC_STC_GD_RA_4_ALL, this);
	myStcData[73].SubclassDlgItem(IDC_STC_GD_RA_ALL_ALL, this);

	myStcData[74].SubclassDlgItem(IDC_STC_AOI_UP, this);
	myStcData[75].SubclassDlgItem(IDC_STC_LAYER2, this);

	myStcData[76].SubclassDlgItem(IDC_STC_DEF_SHORT_TOT, this);
	myStcData[77].SubclassDlgItem(IDC_STC_DEF_SPACE_TOT, this);
	myStcData[78].SubclassDlgItem(IDC_STC_DEF_P_HOLE_TOT, this);
	myStcData[79].SubclassDlgItem(IDC_STC_DEF_H_MISS_TOT, this);
	myStcData[80].SubclassDlgItem(IDC_STC_DEF_VH_MISS_TOT, this);

	myStcData[81].SubclassDlgItem(IDC_STC_MK_CNT_L, this);
	myStcData[82].SubclassDlgItem(IDC_STC_MK_CNT_R, this);

	myStcData[83].SubclassDlgItem(IDC_STC_ENG, this);

	for(int i=0; i<MAX_MENU06_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
		myStcData[i].SetFontSize(12);
		myStcData[i].SetFontBold(FALSE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}
}

void CDlgMenu06::InitStcTitle()
{
	int i;

	myStcTitle[0].SubclassDlgItem(IDC_STC_OP, this);
	myStcTitle[1].SubclassDlgItem(IDC_STC_MDL, this);
	myStcTitle[2].SubclassDlgItem(IDC_STC_LAY, this);
	myStcTitle[3].SubclassDlgItem(IDC_STC_LT, this);
	for(i=0; i<=3; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(14);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_WHITE);
		myStcTitle[i].SetBkColor(RGB_DARKBLUE);
	}

	myStcTitle[4].SubclassDlgItem(IDC_STC_TOT_REEL, this);
	myStcTitle[5].SubclassDlgItem(IDC_STC_WK_RA, this);
	myStcTitle[6].SubclassDlgItem(IDC_STC_LT_RA, this);
	myStcTitle[7].SubclassDlgItem(IDC_STC_TOT_SPD, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_CY_SPD, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_TP_STOP, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_LT_SEP, this);
	myStcTitle[11].SubclassDlgItem(IDC_STC_LT_LEN, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_MKAOI, this);
	myStcTitle[13].SubclassDlgItem(IDC_STC_LT_SRL, this);
	for(i=4; i<=13; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_BLACK);
		myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
	}

	myStcTitle[14].SubclassDlgItem(IDC_STC_WORK_INFO, this);
	myStcTitle[14].SetFontName(_T("Arial"));
	myStcTitle[14].SetFontSize(60);
	myStcTitle[14].SetFontBold(TRUE);
	myStcTitle[14].SetTextColor(RGB_RED);
	myStcTitle[14].SetBkColor(RGB_BLACK);

	myStcTitle[15].SubclassDlgItem(IDC_STC_DEF_NUM, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_DEF_RT, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_GD_NUM, this);
	myStcTitle[18].SubclassDlgItem(IDC_STC_GD_RT, this);
	for(i=15; i<=18; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_BLACK);
		myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
	}

	myStcTitle[19].SubclassDlgItem(IDC_STC_TOT_NUM, this);
	myStcTitle[19].SetFontName(_T("Arial"));
	myStcTitle[19].SetFontSize(12);
	myStcTitle[19].SetFontBold(TRUE);
	myStcTitle[19].SetTextColor(RGB_WHITE);
	myStcTitle[19].SetBkColor(RGB_DARKBLUE);

	myStcTitle[20].SubclassDlgItem(IDC_STC_LOT_ST, this);
	myStcTitle[21].SubclassDlgItem(IDC_STC_LOT_RN, this);
	myStcTitle[22].SubclassDlgItem(IDC_STC_LOT_ED, this);
	for(i=20; i<=22; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_WHITE);
		myStcTitle[i].SetBkColor(RGB_DARKGREEN);
	}

	myStcTitle[23].SubclassDlgItem(IDC_STC_DEF_OP, this);
	myStcTitle[23].SetBkColor(RGB_LTDKORANGE);
	myStcTitle[24].SubclassDlgItem(IDC_STC_DEF_SH, this);
	myStcTitle[24].SetBkColor(RGB_RED);
	myStcTitle[25].SubclassDlgItem(IDC_STC_DEF_USH, this);
	myStcTitle[25].SetBkColor(RGB_SKYBLUE);
	myStcTitle[26].SubclassDlgItem(IDC_STC_DEF_SP, this);
	myStcTitle[26].SetBkColor(RGB_PURPLE);
	myStcTitle[27].SubclassDlgItem(IDC_STC_DEF_EX, this);
	myStcTitle[27].SetBkColor(RGB_DARKGREEN);
	myStcTitle[28].SubclassDlgItem(IDC_STC_DEF_PR, this);
	myStcTitle[28].SetBkColor(RGB_BLUE);
	myStcTitle[29].SubclassDlgItem(IDC_STC_DEF_PHO, this);
	myStcTitle[29].SetBkColor(RGB_LTBLUE);
	myStcTitle[30].SubclassDlgItem(IDC_STC_DEF_PD, this);
	myStcTitle[30].SetBkColor(RGB_LTGREEN);
	myStcTitle[31].SubclassDlgItem(IDC_STC_DEF_HOP, this);
	myStcTitle[31].SetBkColor(RGB_PINK);
	myStcTitle[32].SubclassDlgItem(IDC_STC_DEF_HMS, this);
	myStcTitle[32].SetBkColor(RGB_GREEN);
	myStcTitle[33].SubclassDlgItem(IDC_STC_DEF_HPS, this);
	myStcTitle[33].SetBkColor(RGB_DKSKYBLUE);
	myStcTitle[34].SubclassDlgItem(IDC_STC_DEF_HDF, this);
	myStcTitle[34].SetBkColor(RGB_LTPINK);
	myStcTitle[35].SubclassDlgItem(IDC_STC_DEF_NK, this);
	myStcTitle[35].SetBkColor(RGB_CYAN);
	myStcTitle[36].SubclassDlgItem(IDC_STC_DEF_PO, this);
	myStcTitle[36].SetBkColor(RGB_DARKRED);
	myStcTitle[37].SubclassDlgItem(IDC_STC_DEF_VHO, this);
	myStcTitle[37].SetBkColor(RGB_DARKCYAN);
	myStcTitle[38].SubclassDlgItem(IDC_STC_DEF_VHMS, this);
	myStcTitle[38].SetBkColor(RGB_YELLOW);
	myStcTitle[39].SubclassDlgItem(IDC_STC_DEF_VHP, this);
	myStcTitle[39].SetBkColor(RGB_ORANGE);
	myStcTitle[40].SubclassDlgItem(IDC_STC_DEF_VHD, this);
	myStcTitle[40].SetBkColor(RGB_YELLOWGREEN);
	myStcTitle[41].SubclassDlgItem(IDC_STC_DEF_LTD, this);
	myStcTitle[41].SetBkColor(RGB_LTYELLOW);

	myStcTitle[42].SubclassDlgItem(IDC_STC_DEF_ENK, this);
	myStcTitle[42].SetBkColor(RGB_LTYELLOW);
	myStcTitle[43].SubclassDlgItem(IDC_STC_DEF_EPR, this);
	myStcTitle[43].SetBkColor(RGB_LTYELLOW);
	myStcTitle[44].SubclassDlgItem(IDC_STC_DEF_ESP, this);
	myStcTitle[44].SetBkColor(RGB_LTYELLOW);
	myStcTitle[45].SubclassDlgItem(IDC_STC_DEF_UDD1, this);
	myStcTitle[45].SetBkColor(RGB_LTYELLOW);
	myStcTitle[46].SubclassDlgItem(IDC_STC_DEF_NRW, this);
	myStcTitle[46].SetBkColor(RGB_LTYELLOW);
	myStcTitle[47].SubclassDlgItem(IDC_STC_DEF_WD, this);
	myStcTitle[47].SetBkColor(RGB_LTYELLOW);
	
	for(i=23; i<48; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(FALSE);
		myStcTitle[i].SetTextColor(RGB_BLACK);
	}

	myStcTitle[48].SubclassDlgItem(IDC_STC_WK, this);
	myStcTitle[49].SubclassDlgItem(IDC_STC_WK_UP, this);
	myStcTitle[50].SubclassDlgItem(IDC_STC_WK_DN, this);
	myStcTitle[51].SubclassDlgItem(IDC_STC_WK_ALL, this);
	myStcTitle[52].SubclassDlgItem(IDC_STC_ST, this);
	myStcTitle[53].SubclassDlgItem(IDC_STC_ST_UP, this);
	myStcTitle[54].SubclassDlgItem(IDC_STC_ST_DN, this);
	myStcTitle[55].SubclassDlgItem(IDC_STC_ST_ALL, this);
	for(i=48; i<56; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_WHITE);
		myStcTitle[i].SetBkColor(RGB_DARKBLUE);
	}

	myStcTitle[56].SubclassDlgItem(IDC_STC_GD_RA_1, this);
	myStcTitle[57].SubclassDlgItem(IDC_STC_GD_RA_2, this);
	myStcTitle[58].SubclassDlgItem(IDC_STC_GD_RA_3, this);
	myStcTitle[59].SubclassDlgItem(IDC_STC_GD_RA_4, this);
	myStcTitle[60].SubclassDlgItem(IDC_STC_GD_RA_ALL, this);
	for(i=56; i<61; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_BLACK);
		myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
	}
	myStcTitle[61].SubclassDlgItem(IDC_STC_UP, this);
	myStcTitle[62].SubclassDlgItem(IDC_STC_DN, this);
	myStcTitle[63].SubclassDlgItem(IDC_STC_LAY2, this);
	for(i=61; i<64; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_WHITE);
		myStcTitle[i].SetBkColor(RGB_DARKBLUE);
	}

	myStcTitle[64].SubclassDlgItem(IDC_STC_MK_CNT, this);
	myStcTitle[64].SetFontName(_T("Arial"));
	myStcTitle[64].SetFontSize(12);
	myStcTitle[64].SetFontBold(FALSE);
	myStcTitle[64].SetTextColor(RGB_WHITE);
	myStcTitle[64].SetBkColor(RGB_DARKBLUE);

	myStcTitle[65].SubclassDlgItem(IDC_STC_MK_L, this);
	myStcTitle[66].SubclassDlgItem(IDC_STC_MK_R, this);
	for(i=65; i<MAX_MENU06_STC_TITLE; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(FALSE);
		myStcTitle[i].SetTextColor(RGB_BLACK);
		myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
	}
}

void CDlgMenu06::SetRgbStcDef()
{
	if(!pDoc->m_pReelMapInner)
		return;

	COLORREF rgbDef;

	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_OPEN];
	myStcTitle[23].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_SHORT];
	myStcTitle[24].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_USHORT];
	myStcTitle[25].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_SPACE];
	myStcTitle[26].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_EXTRA];
	myStcTitle[27].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_PROTRUSION];
	myStcTitle[28].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_PINHOLE];
	myStcTitle[29].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_PAD];
	myStcTitle[30].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_HOLE_OPEN];
	myStcTitle[31].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_HOLE_MISS];
	myStcTitle[32].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_HOLE_POSITION];
	myStcTitle[33].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_HOLE_DEFECT];
	myStcTitle[34].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_NICK];
	myStcTitle[35].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_POI];
	myStcTitle[36].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_VH_OPEN];
	myStcTitle[37].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_VH_MISS];
	myStcTitle[38].SetBkColor(RGB_YELLOW);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_VH_POSITION];
	myStcTitle[39].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_VH_DEF];
	myStcTitle[40].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_LIGHT];
	myStcTitle[41].SetBkColor(rgbDef);
	
	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_EDGE_NICK];
	myStcTitle[42].SetBkColor(rgbDef);

	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_EDGE_PROT];
	myStcTitle[43].SetBkColor(rgbDef);

	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_EDGE_SPACE];
	myStcTitle[44].SetBkColor(rgbDef);

	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_USER_DEFINE_1];
	myStcTitle[45].SetBkColor(rgbDef);

	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_NARROW];
	myStcTitle[46].SetBkColor(rgbDef);

	rgbDef = pDoc->m_pReelMapInner->m_rgbDef[DEF_WIDE];
	myStcTitle[47].SetBkColor(rgbDef);
}

void CDlgMenu06::SetTitleStcDef()
{
	if(!pDoc->m_pReelMapInner)
		return;

	myStcTitle[23].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_OPEN]);	
	myStcTitle[24].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_SHORT]);	
	myStcTitle[25].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_USHORT]);	
	myStcTitle[26].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_SPACE]);	
	myStcTitle[27].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_EXTRA]);	
	myStcTitle[28].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_PROTRUSION]);	
	myStcTitle[29].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_PINHOLE]);	
	myStcTitle[30].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_PAD]);	
	myStcTitle[31].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_HOLE_OPEN]);	
	myStcTitle[32].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_HOLE_MISS]);	
	myStcTitle[33].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_HOLE_POSITION]);	
	myStcTitle[34].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_HOLE_DEFECT]);	
	myStcTitle[35].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_NICK]);	
	myStcTitle[36].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_POI]);	
	myStcTitle[37].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_VH_OPEN]);	
	myStcTitle[38].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_VH_MISS]);	
	myStcTitle[39].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_VH_POSITION]);	
	myStcTitle[40].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_VH_DEF]);	
	myStcTitle[41].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_LIGHT]);	

	myStcTitle[42].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_EDGE_NICK]);	
	myStcTitle[43].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_EDGE_PROT]);	
	myStcTitle[44].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_EDGE_SPACE]);	
	myStcTitle[45].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_USER_DEFINE_1]);	
	myStcTitle[46].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_NARROW]);	
	myStcTitle[47].SetText(pDoc->m_pReelMapInner->m_sKorDef[DEF_WIDE]);	
}

void CDlgMenu06::OnChkUserInfo() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[1].GetCheck();
	ChkUserInfo(bOn);
	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::ChkUserInfo(BOOL bOn)
{
	if(bOn)
	{
		pView->PostMessage(WM_DLG_INFO, 0, 0);
	}
	else
	{
		UpdateData();
	}

	myBtn[1].SetCheck(bOn);
}

void CDlgMenu06::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
}


LRESULT CDlgMenu06::OnMyStaticRedraw(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	
	switch(nCtrlID)
	{
	case IDC_STC_REELMAP_INNER:
		this->PostMessage(WM_DRAW_REELMAP_INNER, 0, 0);
		break;
	}

	return 0L;
}

LRESULT CDlgMenu06::OnDrawReelMap(WPARAM wPara, LPARAM lPara)
{
	if(m_pMyGL)
	{
		m_pMyGL->Draw();
		//this->MoveWindow(m_pRect, TRUE);
	}

	return 0L;
}


BOOL CDlgMenu06::IsDoneDispMkInfo()
{
	return (!m_bTIM_DISP_DEF_IMG);
}

void CDlgMenu06::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIM_DISP_DEF_IMG) // SetSerial() ---> DispMkInfo() ---> DispMkInfo(m_nSerial);
	{
		KillTimer(TIM_DISP_DEF_IMG);
		if (m_nSerialDispMkInfo > 0) // if (m_nSerial > 0)
		{			
			DispMkInfo(m_nSerialDispMkInfo);	// DispMkInfo(m_nSerial);

			if(m_nIdxDef[0] < m_nDef[0] || m_nIdxDef[1] < m_nDef[1]) // m_nIdxDef(불량이미지 인덱스) , m_nDef[0] = pDoc->m_pPcrInner[0][nIdx]->m_nTotDef; // m_nDef[up] : 릴맵 화면 표시 인덱스의 Display Def Num.
			{
				if(m_bTIM_DISP_DEF_IMG)
					SetTimer(TIM_DISP_DEF_IMG, 100, NULL);
			}
			else
				m_bTIM_DISP_DEF_IMG = FALSE;
		}
		else
			m_bTIM_DISP_DEF_IMG = FALSE;
	}
	if(nIDEvent == TIM_DISP_MK_INFO) // SetSerial() ---> if m_bTIM_DISP_DEF_IMG == FALSE then Wait to call SetTimer(TIM_DISP_MK_INFO)
	{
		KillTimer(TIM_DISP_MK_INFO);
		if(m_bTIM_DISP_DEF_IMG)
			SetTimer(TIM_DISP_MK_INFO, 100, NULL);
		else
			DispMkInfo();
	}
	if(nIDEvent == TIM_DISP_MK_CNT)
	{
		KillTimer(TIM_DISP_MK_CNT);
		DispMkCnt();
		ChkMkLimit();
		ChkPartialSpd();
		if(m_bTIM_DISP_MK_CNT)
			SetTimer(TIM_DISP_MK_CNT, 300, NULL);
	}
	if(nIDEvent == TIM_WINK_MK_L)
	{
		KillTimer(TIM_WINK_MK_L);
		m_nWinkMk[0]++;
		if(m_nWinkMk[0]%2)
		{
			myStcData[81].SetBkColor(RGB_RED);
			myStcData[81].SetTextColor(RGB_WHITE);
		}
		else
		{
			m_nWinkMk[0] = 0;
			myStcData[81].SetBkColor(RGB_WHITE);
			myStcData[81].SetTextColor(RGB_BLACK);
		}
		if(m_bTIM_WINK_MK_L)
			SetTimer(TIM_WINK_MK_L, 500, NULL);
		else
		{
			m_nWinkMk[0] = 0;
			myStcData[81].SetBkColor(RGB_WHITE);
			myStcData[81].SetTextColor(RGB_BLACK);
		}
	}
	if(nIDEvent == TIM_WINK_MK_R)
	{
		KillTimer(TIM_WINK_MK_R);
		m_nWinkMk[1]++;
		if(m_nWinkMk[1]%2)
		{
			myStcData[82].SetBkColor(RGB_RED);
			myStcData[82].SetTextColor(RGB_WHITE);
		}
		else
		{
			m_nWinkMk[1] = 0;
			myStcData[82].SetBkColor(RGB_WHITE);
			myStcData[82].SetTextColor(RGB_BLACK);
		}
		if(m_bTIM_WINK_MK_R)
			SetTimer(TIM_WINK_MK_R, 500, NULL);
		else
		{
			m_nWinkMk[1] = 0;
			myStcData[82].SetBkColor(RGB_WHITE);
			myStcData[82].SetTextColor(RGB_BLACK);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CDlgMenu06::DispMain(CString sMsg, COLORREF rgb)
{
	if(myStcTitle[14].GetText() != sMsg)
	{
		myStcTitle[14].SetText(sMsg);
		myStcTitle[14].SetTextColor(rgb);
	}
}

CString CDlgMenu06::GetDispMain()
{
	CString sMain;// = myStcTitle[14].GetText();
	GetDlgItem(IDC_STC_WORK_INFO)->GetWindowText(sMain);
	return (sMain);
}

void CDlgMenu06::RefreshRmap()
{
	myStcReelmap.Refresh();	
	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::ResetLotTime()
{
	pDoc->WorkingInfo.Lot.StTime.nYear = 0; 
	pDoc->WorkingInfo.Lot.StTime.nMonth = 0; 
	pDoc->WorkingInfo.Lot.StTime.nDay = 0; 
	pDoc->WorkingInfo.Lot.StTime.nHour = 0; 
	pDoc->WorkingInfo.Lot.StTime.nMin = 0; 
	pDoc->WorkingInfo.Lot.StTime.nSec = 0; 

	pDoc->WorkingInfo.Lot.CurTime.nYear = 0;  
	pDoc->WorkingInfo.Lot.CurTime.nMonth = 0;  
	pDoc->WorkingInfo.Lot.CurTime.nDay = 0;  
	pDoc->WorkingInfo.Lot.CurTime.nHour = 0;  
	pDoc->WorkingInfo.Lot.CurTime.nMin = 0;  
	pDoc->WorkingInfo.Lot.CurTime.nSec = 0; 
	
	pDoc->WorkingInfo.Lot.EdTime.nYear = 0;  
	pDoc->WorkingInfo.Lot.EdTime.nMonth = 0;  
	pDoc->WorkingInfo.Lot.EdTime.nDay = 0;  
	pDoc->WorkingInfo.Lot.EdTime.nHour = 0;  
	pDoc->WorkingInfo.Lot.EdTime.nMin = 0;  
	pDoc->WorkingInfo.Lot.EdTime.nSec = 0; 

	myStcData[21].SetText(_T(""));
	myStcData[22].SetText(_T(""));
	myStcData[23].SetText(_T(""));
}

void CDlgMenu06::DispLotTime()
{
	DispStTime();
	DispRunTime();
	DispEdTime();
}

void CDlgMenu06::DispStTime()
{
	CString str, sPrev;
	int nYear, nMonth, nDay, nHour, nMin, nSec;
	
	nYear = pDoc->WorkingInfo.Lot.StTime.nYear; 
	nMonth = pDoc->WorkingInfo.Lot.StTime.nMonth; 
	nDay = pDoc->WorkingInfo.Lot.StTime.nDay; 
	nHour = pDoc->WorkingInfo.Lot.StTime.nHour; 
	nMin = pDoc->WorkingInfo.Lot.StTime.nMin; 
	nSec = pDoc->WorkingInfo.Lot.StTime.nSec;

	if(!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		str = _T("");
	else
		str.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);

// 	sPrev = myStcData[21].GetText();
	GetDlgItem(IDC_STC_LOT_START)->GetWindowText(sPrev);
	if (sPrev != str)
	{
		myStcData[21].SetText(str);
		pDoc->SetMkMenu01(_T("LotTime"), _T("Start"), str);
	}

//#ifdef USE_ENGRAVE
//	if (pView)
//	{
//		if (pView->m_pEngrave)
//		{
//			pView->m_pEngrave->SetStTime();
//		}
//	}
//#endif
}

CString CDlgMenu06::GetStTime()
{
	CString str;
	GetDlgItem(IDC_STC_LOT_START)->GetWindowText(str);
	return str;
}

void CDlgMenu06::DispRunTime()
{
// 	if(!pView->IsRun() || !pView->IsAuto() || pView->GetAutoStep() < 22) // 새로운 로트의 시작시간을 설정함.
// 		return;

	CString str, sPrev;
	int nDiff;
	int nHour, nMin, nSec;
	int nStYear, nStMonth, nStDay, nStHour, nStMin, nStSec;
	int nCurYear, nCurMonth, nCurDay, nCurHour, nCurMin, nCurSec;
	int nEdYear, nEdMonth, nEdDay, nEdHour, nEdMin, nEdSec;
	
	nStYear = pDoc->WorkingInfo.Lot.StTime.nYear; 
	nStMonth = pDoc->WorkingInfo.Lot.StTime.nMonth; 
	nStDay = pDoc->WorkingInfo.Lot.StTime.nDay; 
	nStHour = pDoc->WorkingInfo.Lot.StTime.nHour; 
	nStMin = pDoc->WorkingInfo.Lot.StTime.nMin; 
	nStSec = pDoc->WorkingInfo.Lot.StTime.nSec;
	
	nCurYear = pDoc->WorkingInfo.Lot.CurTime.nYear; 
	nCurMonth = pDoc->WorkingInfo.Lot.CurTime.nMonth; 
	nCurDay = pDoc->WorkingInfo.Lot.CurTime.nDay; 
	nCurHour = pDoc->WorkingInfo.Lot.CurTime.nHour; 
	nCurMin = pDoc->WorkingInfo.Lot.CurTime.nMin; 
	nCurSec = pDoc->WorkingInfo.Lot.CurTime.nSec;
	
	nEdYear = pDoc->WorkingInfo.Lot.EdTime.nYear; 
	nEdMonth = pDoc->WorkingInfo.Lot.EdTime.nMonth; 
	nEdDay = pDoc->WorkingInfo.Lot.EdTime.nDay; 
	nEdHour = pDoc->WorkingInfo.Lot.EdTime.nHour; 
	nEdMin = pDoc->WorkingInfo.Lot.EdTime.nMin; 
	nEdSec = pDoc->WorkingInfo.Lot.EdTime.nSec;

	if(!nStYear && !nStMonth && !nStDay && !nStHour && !nStMin && !nStSec)
	{
		str = _T("");
	}
	else if(!nEdYear && !nEdMonth && !nEdDay && !nEdHour && !nEdMin && !nEdSec)
	{
		nDiff = (GetTickCount()-pView->m_dwLotSt)/1000;
		nHour = int(nDiff/3600);
		nMin = int((nDiff-3600*nHour)/60);
		nSec = nDiff % 60;
		str.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);
	}
	else
	{
		if(pView->m_dwLotEd > 0)
		{
			nDiff = (pView->m_dwLotEd-pView->m_dwLotSt)/1000;
			nHour = int(nDiff/3600);
			nMin = int((nDiff-3600*nHour)/60);
			nSec = nDiff % 60;
			str.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);
		}
		else
		{
			nHour = nEdHour - nStHour;
			if(nHour < 0)
				nHour += 24;

			nMin = nEdMin - nStMin;
			if(nMin < 0)
				nMin += 60;

			nSec = nEdSec - nStSec;
			if(nSec < 0)
				nSec += 60;

			str.Format(_T("%02d:%02d:%02d"), nHour, nMin, nSec);
		}
	}

// 	sPrev = myStcData[22].GetText();
	GetDlgItem(IDC_STC_LOT_START)->GetWindowText(sPrev);
	if(!sPrev.IsEmpty())
	{
		GetDlgItem(IDC_STC_LOT_RUN)->GetWindowText(sPrev);
		if (sPrev != str)
		{
			myStcData[22].SetText(str);
			pDoc->SetMkMenu01(_T("LotTime"), _T("Run"), str);

//#ifdef USE_ENGRAVE
//			if (pView)
//			{
//				if (pView->m_pEngrave)
//				{
//					pView->m_pEngrave->SetRunTime();
//				}
//			}
//#endif
		}
	}
}

CString CDlgMenu06::GetRunTime()
{
	CString str;
	GetDlgItem(IDC_STC_LOT_RUN)->GetWindowText(str);
	return str;
}

void CDlgMenu06::DispEdTime()
{
	CString str, sPrev;
	int nYear, nMonth, nDay, nHour, nMin, nSec;
	
	nYear = pDoc->WorkingInfo.Lot.EdTime.nYear; 
	nMonth = pDoc->WorkingInfo.Lot.EdTime.nMonth; 
	nDay = pDoc->WorkingInfo.Lot.EdTime.nDay; 
	nHour = pDoc->WorkingInfo.Lot.EdTime.nHour; 
	nMin = pDoc->WorkingInfo.Lot.EdTime.nMin; 
	nSec = pDoc->WorkingInfo.Lot.EdTime.nSec;

	if(!nYear && !nMonth && !nDay && !nHour && !nMin && !nSec)
		str = _T("");
	else
		str.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMin, nSec);

// 	sPrev = myStcData[23].GetText();
	GetDlgItem(IDC_STC_LOT_END)->GetWindowText(sPrev);
	if (sPrev != str)
	{
		myStcData[23].SetText(str);
		pDoc->SetMkMenu01(_T("LotTime"), _T("End"), str);
	}

//#ifdef USE_ENGRAVE
//	if (pView)
//	{
//		if (pView->m_pEngrave)
//		{
//			pView->m_pEngrave->SetEdTime();
//		}
//	}
//#endif
}

CString CDlgMenu06::GetEdTime()
{
	CString str;
	GetDlgItem(IDC_STC_LOT_END)->GetWindowText(str);
	return str;
}

BOOL CDlgMenu06::SetSerial(int nSerial, BOOL bDumy)
{
	if(nSerial <= 0)
	{
		pView->MsgBox(_T("Serial Error.5"));
		//AfxMessageBox(_T("Serial Error.5"));
		return 0;
	}

	CString sPath;
	if(pDoc->MakeMkDir())
	{
		if(m_nSerial != nSerial)
		{
			m_nSerial = nSerial;
			DispReelmap(nSerial, bDumy);

			if(nSerial == pView->m_nLotEndSerial)
				int nBreak = 1;
			
			if (pView->m_bSerialDecrese)
			{
				if (nSerial >= pView->m_nLotEndSerial || !pView->m_nLotEndSerial)
					DispMkInfo();
			}
			else
			{
				if (nSerial <= pView->m_nLotEndSerial || !pView->m_nLotEndSerial)
					DispMkInfo();
			}
		}
		return TRUE;
	}

	return FALSE;
}

BOOL CDlgMenu06::SetSerialReelmap(int nSerial, BOOL bDumy)
{
	if(nSerial <= 0)
	{
		pView->MsgBox(_T("Serial Error.6"));
		return FALSE;
	}

	return DispReelmap(nSerial, bDumy);
}

BOOL CDlgMenu06::SetSerialMkInfo(int nSerial, BOOL bDumy)
{
	if(nSerial <= 0)
	{
		pView->MsgBox(_T("Serial Error.7"));
		return 0;
	}

	CString sPath;
	if(pDoc->MakeMkDir())
	{
		if(m_nSerial != nSerial)
		{
			m_nSerial = nSerial;

			if(nSerial == pView->m_nLotEndSerial)
				int nBreak = 1;

			if (pView->m_bSerialDecrese)
			{
				if (nSerial >= pView->m_nLotEndSerial || !pView->m_nLotEndSerial)
					DispMkInfo();
			}
			else
			{
				if (nSerial <= pView->m_nLotEndSerial || !pView->m_nLotEndSerial)
					DispMkInfo();
			}
		}
		return TRUE;
	}

	return FALSE;
}


int CDlgMenu06::GetSerial()
{
	return m_nSerial;
}

void CDlgMenu06::ResetSerial()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	m_nSerial = 0;

	if(myBtn[3].GetCheck())
	{
		myBtn[3].SetCheck(FALSE);
		if (MODE_INNER != pDoc->GetTestMode())
		{
			if (!pDoc->WorkingInfo.System.bUseEngrave)
			{
				m_bLastProcFromUp = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromUp"), m_bLastProcFromUp);
				pView->m_bWaitPcr[0] = FALSE;
			}
			else
			{
				m_bLastProcFromEng = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
			}
		}
		else
		{
			m_bLastProcFromEng = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
		}
		m_bLastProc = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
	}

	if(myBtn[6].GetCheck())
	{
		myBtn[6].SetCheck(FALSE);
		m_bLotEnd = FALSE; 
			pView->MpeWrite(pView->Plc.DlgMenu01.JobEnd, 0);			// 작업종료(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
	}

	if(myBtn[4].GetCheck())
	{
		pView->m_bReMk = FALSE;
		myBtn[4].SetCheck(FALSE);
		//if(pView->m_pMpe)
		//	pView->MpeWrite(_T("MB440182"), 0);			// 재마킹(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
	}
}

void CDlgMenu06::UpdateData()
{
	CString sVal;

	myStcData[0].SetText(pDoc->WorkingInfo.LastJob.sSelUserName);	// 운용자
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(bDualTest)
	{
		myStcData[75].SetText(pDoc->WorkingInfo.LastJob.sLayerDn);		// 하면레이어
	}

	myStcData[1].SetText(pDoc->WorkingInfo.LastJob.sModel);		// 모델
	myStcData[3].SetText(pDoc->WorkingInfo.LastJob.sLot);			// 로트
	myStcData[2].SetText(pDoc->WorkingInfo.LastJob.sLayerUp);		// 상면레이어

	double dTotLen = _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 1000.0;
	double dLotLen = _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen) * 1000.0;

	sVal.Format(_T("%.1f"), dTotLen / 1000.0);
	myStcData[4].SetText(sVal);			// 전체릴길이

	double dFdTotLen = pView->GetMkFdLen();

	sVal.Format(_T("%d"), (int)(dFdTotLen / dTotLen * 100.0));
	myStcData[5].SetText(sVal);			// 전체진행율

	sVal.Format(_T("%d"), (int)(dFdTotLen / dTotLen * 100.0));
	myStcData[6].SetText(sVal);			// 로트진행율

	myStcData[6].SetText(_T(""));		// 로트진행율

	myStcData[10].SetText(pDoc->WorkingInfo.LastJob.sStripOutRatio);	// 스트립 양폐율[%]
	myStcData[10].SetBkColor(RGB_WHITE);

	sVal.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nVerifyPeriod);
	myStcData[14].SetText(sVal);

	myStcData[11].SetText(pDoc->WorkingInfo.LastJob.sCustomNeedRatio);	// 고객출하수율
	myStcData[11].SetBkColor(RGB_WHITE);

	if(pDoc->WorkingInfo.LastJob.bTempPause)
	{
		myBtn[0].SetCheck(TRUE);
		myStcData[9].SetText(pDoc->WorkingInfo.LastJob.sTempPauseLen);	// 일시정지길이
	}
	else
	{
		myBtn[0].SetCheck(FALSE);
		myStcData[9].SetText(pDoc->WorkingInfo.LastJob.sTempPauseLen);	// 일시정지길이
	}

	myBtn[2].SetCheck(pDoc->WorkingInfo.LastJob.bVerify);
	GetDlgItem(IDC_STC_REVIEW_LEN)->SetWindowText(pDoc->WorkingInfo.LastJob.sVerifyLen);
	pDoc->SetMkMenu01(_T("Data"), _T("VerifyLen"), pDoc->WorkingInfo.LastJob.sVerifyLen);

	((CButton*)GetDlgItem(IDC_CHK_2LAYER))->SetCheck(pDoc->WorkingInfo.LastJob.bUse2Layer);

}

void CDlgMenu06::UpdateWorking()
{
	if(!pDoc->m_pReelMapInner)
		return;

	CString sVal;
	double dFdTotLen = pView->GetMkFdLen();

	double dTotLen = _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 1000.0;
	double dLotLen = _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen) * 1000.0;

	sVal.Format(_T("%d"), (int)(dFdTotLen / dTotLen * 100.0));
	myStcData[5].SetText(sVal);			// 전체진행율
	pDoc->SetMkMenu01(_T("Info"), _T("Total Work Ratio"), sVal);

	if(pDoc->m_pReelMapInner->m_bUseLotSep)
	{
		sVal.Format(_T("%d"), (int)(dFdTotLen / dLotLen * 100.0));
		myStcData[6].SetText(sVal);		// 로트진행율
		pDoc->SetMkMenu01(_T("Info"), _T("Lot Work Ratio"), sVal);
	}
	else
	{
		myStcData[6].SetText(_T(""));		// 로트진행율
		pDoc->SetMkMenu01(_T("Info"), _T("Lot Work Ratio"), _T(""));
	}

	sVal.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nVerifyPeriod);
	myStcData[14].SetText(sVal);
	pDoc->SetMkMenu01(_T("Info"), _T("VerifyImageNum"), sVal);


	sVal.Format(_T("%.1f"), pView->GetTotVel());
	myStcData[7].SetText(sVal);			// 전체속도
	pDoc->SetMkMenu01(_T("Info"), _T("Total Speed"), sVal);

	if(pDoc->GetLastShotMk() > 0)
		UpdateTotVel(sVal);

	sVal.Format(_T("%.1f"), pView->GetPartVel());
	myStcData[8].SetText(sVal);			// 구간속도
	pDoc->SetMkMenu01(_T("Info"), _T("Patial Speed"), sVal);

	// 마킹부/검사부 : Distance (FdDone)
	sVal.Format(_T("%.2f"), dFdTotLen / 1000.0);	// [M]
	myStcData[12].SetText(sVal);			// 마킹부 : Distance (FdDone)
	pDoc->SetMkMenu01(_T("Info"), _T("DoneLengthMk"), sVal);

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(bDualTest)
	{
		sVal.Format(_T("%.2f"), pView->GetAoiDnFdLen() / 1000.0);	// [M]
		myStcData[13].SetText(sVal);			// 검사부(하) : Distance (FdDone)
		pDoc->SetMkMenu01(_T("Info"), _T("DoneLengthAoiDn"), sVal);
	}
	sVal.Format(_T("%.2f"), pView->GetAoiUpFdLen() / 1000.0);	// [M]
	myStcData[74].SetText(sVal);			// 검사부(상) : Distance (FdDone)
	pDoc->SetMkMenu01(_T("Info"), _T("DoneLengthAoiUp"), sVal);

	sVal.Format(_T("%.2f"), pView->GetEngraveFdLen() / 1000.0);	// [M]
	myStcData[83].SetText(sVal);			// 각인부 : Distance (FdDone)
	pDoc->SetMkMenu01(_T("Info"), _T("DoneLengthEngrave"), sVal);

		pView->MpeWrite(pView->Plc.DlgMenu01.FeedTotalLengthPunch, (long)dFdTotLen);	// 마킹부 진행량(mm단위로 피딩 후에 PC가 기록함) - 20141104
		if(bDualTest)
			pView->MpeWrite(pView->Plc.DlgMenu01.FeedTotalLengthAoiDn, (long)(pView->GetAoiDnFdLen()));	// AOI(하) 진행량(mm단위로 피딩 후에 PC가 기록함) - 20141104
		pView->MpeWrite(pView->Plc.DlgMenu01.FeedTotalLengthAoiUp, (long)(pView->GetAoiUpFdLen()));	// AOI(상) 진행량(mm단위로 피딩 후에 PC가 기록함) - 20141104
		pView->MpeWrite(pView->Plc.DlgMenu01.FeedTotalLengthEngrave, (long)(pView->GetEngraveFdLen()));	// 각인부 진행량(mm단위로 피딩 후에 PC가 기록함)
}

void CDlgMenu06::UpdateTotVel(CString sVel)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(pDoc->m_pReelMapInnerUp)
		pDoc->m_pReelMapInnerUp->UpdateTotVel(sVel, 0); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	if(bDualTest)
	{
		if(pDoc->m_pReelMapInnerDn)
			pDoc->m_pReelMapInnerDn->UpdateTotVel(sVel, 1); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		if(pDoc->m_pReelMapInnerAllUp)
			pDoc->m_pReelMapInnerAllUp->UpdateTotVel(sVel, 2); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		if(pDoc->m_pReelMapInnerAllDn)
			pDoc->m_pReelMapInnerAllDn->UpdateTotVel(sVel, 3); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CDlgMenu06::UpdateRst() // Menu01 화면에서의 수율정보를 업데이트함.
{
	if(!pDoc->m_pReelMapInner)
		return;

	DispTotRatio();
	DispStripRatio();
	DispDef();
}

void CDlgMenu06::DispTotRatio()
{
	CString str;
	int nGood=0, nBad=0, nTot=0, nVal;
	int nPnl = m_nSerial - 1;
	double dRatio=0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (!pDoc->m_pReelMapInnerUp)
		return;

	// < 전체 수율 >
	// 상면
	pDoc->m_pReelMapInnerUp->GetPcsNum(nGood, nBad);
	nTot = nGood + nBad;

	str.Format(_T("%d"), nBad);
	myStcData[15].SetText(str); // IDC_STC_DEFECT_NUM_UP
	pDoc->SetMkMenu01(_T("Bed"), _T("Up"), str);

	if(nTot > 0)
		dRatio = ((double)nBad/(double)nTot) * 100.0;
	else
		dRatio = 0.0;
	str.Format(_T("%.1f"), dRatio);
	myStcData[16].SetText(str); // IDC_STC_DEFECT_RATIO_UP
	pDoc->SetMkMenu01(_T("Bed Ratio"), _T("Up"), str);

	str.Format(_T("%d"), nGood);
	myStcData[17].SetText(str); // IDC_STC_GOOD_NUM_UP
	pDoc->SetMkMenu01(_T("Good"), _T("Up"), str);

	if(nTot > 0)
		dRatio = ((double)nGood/(double)nTot) * 100.0;
	else
		dRatio = 0.0;
	str.Format(_T("%.1f"), dRatio);
	myStcData[18].SetText(str); // IDC_STC_GOOD_RATIO
	pDoc->SetMkMenu01(_T("Good Ratio"), _T("Up"), str);

	str.Format(_T("%d"), nTot);
	myStcData[19].SetText(str); // IDC_STC_TOTAL_NUM_UP
	pDoc->SetMkMenu01(_T("Total Test"), _T("Up"), str);

	if(bDualTest)
	{
		if (!pDoc->m_pReelMapInnerDn)
			return;

		// 하면
		pDoc->m_pReelMapInnerDn->GetPcsNum(nGood, nBad);

		nTot = nGood + nBad;

		str.Format(_T("%d"), nBad);
		myStcData[49].SetText(str); // IDC_STC_DEFECT_NUM_DN
		pDoc->SetMkMenu01(_T("Bed"), _T("Dn"), str);

		if(nTot > 0)
			dRatio = ((double)nBad/(double)nTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[50].SetText(str); // IDC_STC_DEFECT_RATIO_DN
		pDoc->SetMkMenu01(_T("Bed Ratio"), _T("Dn"), str);

		str.Format(_T("%d"), nGood);
		myStcData[51].SetText(str); // IDC_STC_GOOD_NUM_DN
		pDoc->SetMkMenu01(_T("Good"), _T("Dn"), str);

		if(nTot > 0)
			dRatio = ((double)nGood/(double)nTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[52].SetText(str); // IDC_STC_GOOD_RATIO_DN
		pDoc->SetMkMenu01(_T("Good Ratio"), _T("Dn"), str);

		str.Format(_T("%d"), nTot);
		myStcData[53].SetText(str); // IDC_STC_TOTAL_NUM_DN
		pDoc->SetMkMenu01(_T("Total Test"), _T("Dn"), str);

		if (!pDoc->m_pReelMapInnerAllDn)
			return;

		// 전체
		pDoc->m_pReelMapInnerAllDn->GetPcsNum(nGood, nBad);
		nTot = nGood + nBad;

		str.Format(_T("%d"), nBad);
		myStcData[54].SetText(str); // IDC_STC_DEFECT_NUM_ALL
		pDoc->SetMkMenu01(_T("Bed"), _T("Total"), str);

		if(nTot > 0)
			dRatio = ((double)nBad/(double)nTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[55].SetText(str); // IDC_STC_DEFECT_RATIO_ALL
		pDoc->SetMkMenu01(_T("Bed Ratio"), _T("Total"), str);

		str.Format(_T("%d"), nGood);
		myStcData[56].SetText(str); // IDC_STC_GOOD_NUM_ALL
		pDoc->SetMkMenu01(_T("Good"), _T("Total"), str);

		if(nTot > 0)
			dRatio = ((double)nGood/(double)nTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[57].SetText(str); // IDC_STC_GOOD_RATIO_ALL
		pDoc->SetMkMenu01(_T("Good Ratio"), _T("Total"), str);

		str.Format(_T("%d"), nTot);
		myStcData[58].SetText(str); // IDC_STC_TOTAL_NUM_ALL
		pDoc->SetMkMenu01(_T("Total Test"), _T("Total"), str);
	}
}

void CDlgMenu06::DispStripRatio()
{
	CString str;
	int nGood=0, nBad=0, nTot=0, nStTot=0, nSum=0, nVal[2][4];
	int nMer[4];
	int nPnl = m_nSerial - 1;
	double dRatio=0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	for(int i=0; i<2; i++)
	{
		for(int k=0; k<4; k++)
			nVal[i][k] = 0;
	}

	if (!pDoc->m_pReelMapInnerUp)
		return;

	// < 스트립 별 수율 >
	pDoc->m_pReelMapInnerUp->GetPcsNum(nGood, nBad);
	nTot = nGood + nBad;
	nStTot = nTot / MAX_STRIP;

	// 상면
	nVal[0][0] = pDoc->m_pReelMapInnerUp->GetDefStrip(0);
	nSum += nVal[0][0];
	if(nTot > 0)
		dRatio = ((double)(nStTot-nVal[0][0])/(double)nStTot) * 100.0;
	else
		dRatio = 0.0;
	str.Format(_T("%.1f"), dRatio);
	myStcData[59].SetText(str); // IDC_STC_GD_RA_1_UP
	pDoc->SetMkMenu01(_T("Yield Strip0"), _T("Up"), str);

	nVal[0][1] = pDoc->m_pReelMapInnerUp->GetDefStrip(1);
	nSum += nVal[0][1];
	if(nTot > 0)
		dRatio = ((double)(nStTot-nVal[0][1])/(double)nStTot) * 100.0;
	else
		dRatio = 0.0;
	str.Format(_T("%.1f"), dRatio);
	myStcData[60].SetText(str); // IDC_STC_GD_RA_2_UP
	pDoc->SetMkMenu01(_T("Yield Strip1"), _T("Up"), str);

	nVal[0][2] = pDoc->m_pReelMapInnerUp->GetDefStrip(2);
	nSum += nVal[0][2];
	if(nTot > 0)
		dRatio = ((double)(nStTot-nVal[0][2])/(double)nStTot) * 100.0;
	else
		dRatio = 0.0;
	str.Format(_T("%.1f"), dRatio);
	myStcData[61].SetText(str); // IDC_STC_GD_RA_3_UP
	pDoc->SetMkMenu01(_T("Yield Strip2"), _T("Up"), str);

	nVal[0][3] = pDoc->m_pReelMapInnerUp->GetDefStrip(3);
	nSum += nVal[0][3];
	if(nTot > 0)
		dRatio = ((double)(nStTot-nVal[0][3])/(double)nStTot) * 100.0;
	else
		dRatio = 0.0;
	str.Format(_T("%.1f"), dRatio);
	myStcData[62].SetText(str); // IDC_STC_GD_RA_4_UP
	pDoc->SetMkMenu01(_T("Yield Strip3"), _T("Up"), str);

	if(nTot > 0)
		dRatio = ((double)(nTot-nSum)/(double)nTot) * 100.0;
	else
		dRatio = 0.0;
	str.Format(_T("%.1f"), dRatio);
	myStcData[63].SetText(str); // IDC_STC_GD_RA_ALL_UP
	pDoc->SetMkMenu01(_T("Yield Total"), _T("Up"), str);

	nSum = 0;

	if(bDualTest)
	{
		if (!pDoc->m_pReelMapInnerDn)
			return;

		// 하면
		nVal[1][0] = pDoc->m_pReelMapInnerDn->GetDefStrip(0);
		nSum += nVal[1][0];
		if(nTot > 0)
			dRatio = ((double)(nStTot-nVal[1][0])/(double)nStTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[64].SetText(str); // IDC_STC_GD_RA_1_DN
		pDoc->SetMkMenu01(_T("Yield Strip0"), _T("Dn"), str);

		nVal[1][1] = pDoc->m_pReelMapInnerDn->GetDefStrip(1);
		nSum += nVal[1][1];
		if(nTot > 0)
			dRatio = ((double)(nStTot-nVal[1][1])/(double)nStTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[65].SetText(str); // IDC_STC_GD_RA_2_DN
		pDoc->SetMkMenu01(_T("Yield Strip1"), _T("Dn"), str);

		nVal[1][2] = pDoc->m_pReelMapInnerDn->GetDefStrip(2);
		nSum += nVal[1][2];
		if(nTot > 0)
			dRatio = ((double)(nStTot-nVal[1][2])/(double)nStTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[66].SetText(str); // IDC_STC_GD_RA_3_DN
		pDoc->SetMkMenu01(_T("Yield Strip2"), _T("Dn"), str);

		nVal[1][3] = pDoc->m_pReelMapInnerDn->GetDefStrip(3);
		nSum += nVal[1][3];
		if(nTot > 0)
			dRatio = ((double)(nStTot-nVal[1][3])/(double)nStTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[67].SetText(str); // IDC_STC_GD_RA_4_DN
		pDoc->SetMkMenu01(_T("Yield Strip3"), _T("Dn"), str);

		if(nTot > 0)
			dRatio = ((double)(nTot-nSum)/(double)nTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[68].SetText(str); // IDC_STC_GD_RA_ALL_DN
		pDoc->SetMkMenu01(_T("Yield Total"), _T("Dn"), str);

		nSum = 0;

		if (!pDoc->m_pReelMapInnerAllUp)
			return;

		// 상면 + 하면
		nMer[0] = pDoc->m_pReelMapInnerAllUp->GetDefStrip(0);
		nSum += nMer[0];
		if(nTot > 0)
			dRatio = ((double)(nStTot-nMer[0])/(double)nStTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[69].SetText(str); // IDC_STC_GD_RA_1_ALL
		pDoc->SetMkMenu01(_T("Yield Strip0"), _T("Total"), str);

		nMer[1] = pDoc->m_pReelMapInnerAllUp->GetDefStrip(1);
		nSum += nMer[1];
		if(nTot > 0)
			dRatio = ((double)(nStTot-nMer[1])/(double)nStTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[70].SetText(str); // IDC_STC_GD_RA_2_ALL
		pDoc->SetMkMenu01(_T("Yield Strip1"), _T("Total"), str);

		nMer[2] = pDoc->m_pReelMapInnerAllUp->GetDefStrip(2);
		nSum += nMer[2];
		if(nTot > 0)
			dRatio = ((double)(nStTot-nMer[2])/(double)nStTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[71].SetText(str); // IDC_STC_GD_RA_3_ALL
		pDoc->SetMkMenu01(_T("Yield Strip2"), _T("Total"), str);

		nMer[3] = pDoc->m_pReelMapInnerAllUp->GetDefStrip(3);
		nSum += nMer[3];
		if(nTot > 0)
			dRatio = ((double)(nStTot-nMer[3])/(double)nStTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[72].SetText(str); // IDC_STC_GD_RA_4_ALL
		pDoc->SetMkMenu01(_T("Yield Strip3"), _T("Total"), str);

		if(nTot > 0)
			dRatio = ((double)(nTot-nSum)/(double)nTot) * 100.0;
		else
			dRatio = 0.0;
		str.Format(_T("%.1f"), dRatio);
		myStcData[73].SetText(str); // IDC_STC_GD_RA_ALL_ALL
		pDoc->SetMkMenu01(_T("Yield Total"), _T("Total"), str);
	}
}

void CDlgMenu06::ClrInfo()
{
	myStcData[5].SetText(_T(""));			// 전체진행율
	pDoc->SetMkMenu01(_T("Info"), _T("Total Work Ratio"), _T(""));
	myStcData[6].SetText(_T(""));			// 로트진행율
	pDoc->SetMkMenu01(_T("Info"), _T("Lot Work Ratio"), _T(""));
	myStcData[14].SetText(_T(""));			// 진행Lot시리얼
	pDoc->SetMkMenu01(_T("Info"), _T("VerifyImageNum"), _T(""));
	myStcData[7].SetText(_T(""));			// 전체속도
	pDoc->SetMkMenu01(_T("Info"), _T("Total Speed"), _T(""));
	myStcData[8].SetText(_T(""));			// 구간속도
	pDoc->SetMkMenu01(_T("Info"), _T("Patial Speed"), _T(""));
	myStcData[12].SetText(_T(""));			// 마킹부 : Distance (FdDone)
	pDoc->SetMkMenu01(_T("Info"), _T("DoneLengthMk"), _T(""));
	myStcData[13].SetText(_T(""));			// 검사부 Dn: Distance (FdDone)
	pDoc->SetMkMenu01(_T("Info"), _T("DoneLengthAoiDn"), _T(""));
	myStcData[74].SetText(_T(""));			// 검사부 Up: Distance (FdDone)
	pDoc->SetMkMenu01(_T("Info"), _T("DoneLengthAoiUp"), _T(""));
	myStcData[83].SetText(_T(""));			// 각인부: Distance (FdDone)
	pDoc->SetMkMenu01(_T("Info"), _T("DoneLengthEngrave"), _T(""));

	// < 전체 수율 >
	// 상면
	myStcData[15].SetText(_T("")); // IDC_STC_DEFECT_NUM_UP
	pDoc->SetMkMenu01(_T("Bed"), _T("Up"), _T(""));
	myStcData[16].SetText(_T("")); // IDC_STC_DEFECT_RATIO_UP
	pDoc->SetMkMenu01(_T("Bed Ratio"), _T("Up"), _T(""));
	myStcData[17].SetText(_T("")); // IDC_STC_GOOD_NUM_UP
	pDoc->SetMkMenu01(_T("Good"), _T("Up"), _T(""));
	myStcData[18].SetText(_T("")); // IDC_STC_GOOD_RATIO_UP
	pDoc->SetMkMenu01(_T("Good Ratio"), _T("Up"), _T(""));
	myStcData[19].SetText(_T("")); // IDC_STC_TOTAL_NUM_UP
	pDoc->SetMkMenu01(_T("Total Test"), _T("Up"), _T(""));
	
	// 하면
	myStcData[49].SetText(_T("")); // IDC_STC_DEFECT_NUM_DN
	pDoc->SetMkMenu01(_T("Bed"), _T("Dn"), _T(""));
	myStcData[50].SetText(_T("")); // IDC_STC_DEFECT_RATIO_DN
	pDoc->SetMkMenu01(_T("Bed Ratio"), _T("Dn"), _T(""));
	myStcData[51].SetText(_T("")); // IDC_STC_GOOD_NUM_DN
	pDoc->SetMkMenu01(_T("Good"), _T("Dn"), _T(""));
	myStcData[52].SetText(_T("")); // IDC_STC_GOOD_RATIO_DN
	pDoc->SetMkMenu01(_T("Good Ratio"), _T("Dn"), _T(""));
	myStcData[53].SetText(_T("")); // IDC_STC_TOTAL_NUM_DN
	pDoc->SetMkMenu01(_T("Total Test"), _T("Dn"), _T(""));

	// 전체
	myStcData[54].SetText(_T("")); // IDC_STC_DEFECT_NUM_ALL
	pDoc->SetMkMenu01(_T("Bed"), _T("Total"), _T(""));
	myStcData[55].SetText(_T("")); // IDC_STC_DEFECT_RATIO_ALL
	pDoc->SetMkMenu01(_T("Bed Ratio"), _T("Total"), _T(""));
	myStcData[56].SetText(_T("")); // IDC_STC_GOOD_NUM_ALL
	pDoc->SetMkMenu01(_T("Good"), _T("Total"), _T(""));
	myStcData[57].SetText(_T("")); // IDC_STC_GOOD_RATIO_ALL
	pDoc->SetMkMenu01(_T("Good Ratio"), _T("Total"), _T(""));
	myStcData[58].SetText(_T("")); // IDC_STC_TOTAL_NUM_ALL
	pDoc->SetMkMenu01(_T("Total Test"), _T("Total"), _T(""));

	// < 스트립 별 수율 >
	// 상면
	myStcData[59].SetText(_T("")); // DC_STC_GD_RA_1_UP
	pDoc->SetMkMenu01(_T("Yield Strip0"), _T("Up"), _T(""));
	myStcData[60].SetText(_T("")); // DC_STC_GD_RA_2_UP
	pDoc->SetMkMenu01(_T("Yield Strip1"), _T("Up"), _T(""));
	myStcData[61].SetText(_T("")); // DC_STC_GD_RA_3_UP
	pDoc->SetMkMenu01(_T("Yield Strip2"), _T("Up"), _T(""));
	myStcData[62].SetText(_T("")); // DC_STC_GD_RA_4_UP
	pDoc->SetMkMenu01(_T("Yield Strip3"), _T("Up"), _T(""));
	myStcData[63].SetText(_T("")); // IDC_STC_GD_RA_ALL_UP
	pDoc->SetMkMenu01(_T("Yield Total"), _T("Up"), _T(""));
	
	// 하면
	myStcData[64].SetText(_T("")); // IDC_STC_GD_RA_1_DN
	pDoc->SetMkMenu01(_T("Yield Strip0"), _T("Dn"), _T(""));
	myStcData[65].SetText(_T("")); // IDC_STC_GD_RA_1_DN
	pDoc->SetMkMenu01(_T("Yield Strip1"), _T("Dn"), _T(""));
	myStcData[66].SetText(_T("")); // IDC_STC_GD_RA_1_DN
	pDoc->SetMkMenu01(_T("Yield Strip2"), _T("Dn"), _T(""));
	myStcData[67].SetText(_T("")); // IDC_STC_GD_RA_1_DN
	pDoc->SetMkMenu01(_T("Yield Strip3"), _T("Dn"), _T(""));
	myStcData[68].SetText(_T("")); // IDC_STC_GD_RA_ALL_DN
	pDoc->SetMkMenu01(_T("Yield Total"), _T("Dn"), _T(""));

	// 상면 + 하면
	myStcData[69].SetText(_T("")); // IDC_STC_GD_RA_1_ALL
	pDoc->SetMkMenu01(_T("Yield Strip0"), _T("Total"), _T(""));
	myStcData[70].SetText(_T("")); // IDC_STC_GD_RA_2_ALL
	pDoc->SetMkMenu01(_T("Yield Strip0"), _T("Total"), _T(""));
	myStcData[71].SetText(_T("")); // IDC_STC_GD_RA_3_ALL
	pDoc->SetMkMenu01(_T("Yield Strip0"), _T("Total"), _T(""));
	myStcData[72].SetText(_T("")); // IDC_STC_GD_RA_4_ALL
	pDoc->SetMkMenu01(_T("Yield Strip0"), _T("Total"), _T(""));
	myStcData[73].SetText(_T("")); // IDC_STC_GD_RA_ALL_ALL
	pDoc->SetMkMenu01(_T("Yield Total"), _T("Total"), _T(""));

	// 불량 내역
	myStcData[24].SetText(_T("")); // IDC_STC_DEF_OPEN
	myStcData[25].SetText(_T("")); // IDC_STC_DEF_SHORT
	myStcData[26].SetText(_T("")); // IDC_STC_DEF_U_SHORT
	myStcData[27].SetText(_T("")); // IDC_STC_DEF_SPACE
	myStcData[28].SetText(_T("")); // IDC_STC_DEF_EXTRA
	myStcData[29].SetText(_T("")); // IDC_STC_DEF_PROT
	myStcData[30].SetText(_T("")); // IDC_STC_DEF_P_HOLE
	myStcData[31].SetText(_T("")); // IDC_STC_DEF_PAD
	myStcData[32].SetText(_T("")); // IDC_STC_DEF_H_OPEN
	myStcData[33].SetText(_T("")); // IDC_STC_DEF_H_MISS
	myStcData[34].SetText(_T("")); // IDC_STC_DEF_H_POS
	myStcData[35].SetText(_T("")); // IDC_STC_DEF_H_DEF
	myStcData[36].SetText(_T("")); // IDC_STC_DEF_NICK
	myStcData[37].SetText(_T("")); // IDC_STC_DEF_POI
	myStcData[38].SetText(_T("")); // IDC_STC_DEF_VH_OPEN
	myStcData[39].SetText(_T("")); // IDC_STC_DEF_VH_MISS
	myStcData[40].SetText(_T("")); // IDC_STC_DEF_VH_POS
	myStcData[41].SetText(_T("")); // IDC_STC_DEF_VH_DEF
	myStcData[42].SetText(_T("")); // IDC_STC_DEF_LIGHT
	myStcData[43].SetText(_T("")); // DEF_EDGE_NICK
	myStcData[44].SetText(_T("")); // DEF_EDGE_PROT
	myStcData[45].SetText(_T("")); // DEF_EDGE_SPACE
	myStcData[46].SetText(_T("")); // DEF_USER_DEFINE_1
	myStcData[47].SetText(_T("")); // DEF_NARROW
	myStcData[48].SetText(_T("")); // DEF_WIDE

	int i;
	CString sItem;
	for (int i = 1; i < MAX_DEF; i++)
	{
		sItem.Format(_T("%d"), i);
		pDoc->SetMkMenu01(_T("Defect"), sItem, _T(""));
	}

}

void CDlgMenu06::DispDef()
{
	CString str, sItem;
	int nNum=0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	CReelMap* pReelMap;
	pReelMap = NULL;

	if(bDualTest)
	{
		switch(pView->m_nSelRmapInner)
		{
		case RMAP_INNER_UP:
			pReelMap = pDoc->m_pReelMapInnerUp;
			break;
		case RMAP_INNER_DN:
			pReelMap = pDoc->m_pReelMapInnerDn;
			break;
		case RMAP_INNER_ALLUP:
			pReelMap = pDoc->m_pReelMapInnerAllUp;
			break;
		case RMAP_INNER_ALLDN:
			pReelMap = pDoc->m_pReelMapInnerAllDn;
			break;
		}
	}
	else
		pReelMap = pDoc->m_pReelMapInnerUp;

	if (!pReelMap)
		return;

	nNum = pReelMap->GetDefNum(DEF_OPEN);
	str.Format(_T("%d"), nNum);
	myStcData[24].SetText(str); // IDC_STC_DEF_OPEN
	sItem.Format(_T("%d"), DEF_OPEN);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_SHORT);
	str.Format(_T("%d"), nNum);
	myStcData[25].SetText(str); // IDC_STC_DEF_SHORT
	sItem.Format(_T("%d"), DEF_SHORT);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_USHORT);
	str.Format(_T("%d"), nNum);
	myStcData[26].SetText(str); // IDC_STC_DEF_U_SHORT
	sItem.Format(_T("%d"), DEF_USHORT);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_SPACE);
	str.Format(_T("%d"), nNum);
	myStcData[27].SetText(str); // IDC_STC_DEF_SPACE
	sItem.Format(_T("%d"), DEF_SPACE);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_EXTRA);
	str.Format(_T("%d"), nNum);
	myStcData[28].SetText(str); // IDC_STC_DEF_EXTRA
	sItem.Format(_T("%d"), DEF_EXTRA);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_PROTRUSION);
	str.Format(_T("%d"), nNum);
	myStcData[29].SetText(str); // IDC_STC_DEF_PROT
	sItem.Format(_T("%d"), DEF_PROTRUSION);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_PINHOLE);
	str.Format(_T("%d"), nNum);
	myStcData[30].SetText(str); // IDC_STC_DEF_P_HOLE
	sItem.Format(_T("%d"), DEF_PINHOLE);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_PAD);
	str.Format(_T("%d"), nNum);
	myStcData[31].SetText(str); // IDC_STC_DEF_PAD
	sItem.Format(_T("%d"), DEF_PAD);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_HOLE_OPEN);
	str.Format(_T("%d"), nNum);
	myStcData[32].SetText(str); // IDC_STC_DEF_H_OPEN
	sItem.Format(_T("%d"), DEF_HOLE_OPEN);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_HOLE_MISS);
	str.Format(_T("%d"), nNum);
	myStcData[33].SetText(str); // IDC_STC_DEF_H_MISS
	sItem.Format(_T("%d"), DEF_HOLE_MISS);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_HOLE_POSITION);
	str.Format(_T("%d"), nNum);
	myStcData[34].SetText(str); // IDC_STC_DEF_H_POS
	sItem.Format(_T("%d"), DEF_HOLE_POSITION);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_HOLE_DEFECT);
	str.Format(_T("%d"), nNum);
	myStcData[35].SetText(str); // IDC_STC_DEF_H_DEF
	sItem.Format(_T("%d"), DEF_HOLE_DEFECT);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_NICK);
	str.Format(_T("%d"), nNum);
	myStcData[36].SetText(str); // IDC_STC_DEF_NICK
	sItem.Format(_T("%d"), DEF_NICK);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_POI);
	str.Format(_T("%d"), nNum);
	myStcData[37].SetText(str); // IDC_STC_DEF_POI
	sItem.Format(_T("%d"), DEF_POI);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_VH_OPEN);
	str.Format(_T("%d"), nNum);
	myStcData[38].SetText(str); // IDC_STC_DEF_VH_OPEN
	sItem.Format(_T("%d"), DEF_VH_OPEN);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_VH_MISS);
	str.Format(_T("%d"), nNum);
	myStcData[39].SetText(str); // IDC_STC_DEF_VH_MISS
	sItem.Format(_T("%d"), DEF_VH_MISS);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_VH_POSITION);
	str.Format(_T("%d"), nNum);
	myStcData[40].SetText(str); // IDC_STC_DEF_VH_POS
	sItem.Format(_T("%d"), DEF_VH_POSITION);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_VH_DEF);
	str.Format(_T("%d"), nNum);
	myStcData[41].SetText(str); // IDC_STC_DEF_VH_DEF
	sItem.Format(_T("%d"), DEF_VH_DEF);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_LIGHT);
	str.Format(_T("%d"), nNum);
	myStcData[42].SetText(str); // IDC_STC_DEF_LIGHT
	sItem.Format(_T("%d"), DEF_LIGHT);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_EDGE_NICK);
	str.Format(_T("%d"), nNum);
	myStcData[43].SetText(str);
	sItem.Format(_T("%d"), DEF_EDGE_NICK);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_EDGE_PROT);
	str.Format(_T("%d"), nNum);
	myStcData[44].SetText(str);
	sItem.Format(_T("%d"), DEF_EDGE_PROT);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_EDGE_SPACE);
	str.Format(_T("%d"), nNum);
	myStcData[45].SetText(str);
	sItem.Format(_T("%d"), DEF_EDGE_SPACE);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_USER_DEFINE_1);
	str.Format(_T("%d"), nNum);
	myStcData[46].SetText(str);
	sItem.Format(_T("%d"), DEF_USER_DEFINE_1);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_NARROW);
	str.Format(_T("%d"), nNum);
	myStcData[47].SetText(str);
	sItem.Format(_T("%d"), DEF_NARROW);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);

	nNum = pReelMap->GetDefNum(DEF_WIDE);
	str.Format(_T("%d"), nNum);
	myStcData[48].SetText(str);
	sItem.Format(_T("%d"), DEF_WIDE);
	pDoc->SetMkMenu01(_T("Defect"), sItem, str);


	nNum = pReelMap->GetDefNum(DEF_SHORT) + pReelMap->GetDefNum(DEF_USHORT);
	str.Format(_T("%d"), nNum);
	myStcData[76].SetText(str); // IDC_STC_DEF_SHORT_TOT

	nNum = pReelMap->GetDefNum(DEF_SPACE) + pReelMap->GetDefNum(DEF_EXTRA) + pReelMap->GetDefNum(DEF_PROTRUSION);
	str.Format(_T("%d"), nNum);
	myStcData[77].SetText(str); // IDC_STC_DEF_SPACE_TOT

	nNum = pReelMap->GetDefNum(DEF_PINHOLE) + pReelMap->GetDefNum(DEF_PAD);
	str.Format(_T("%d"), nNum);
	myStcData[78].SetText(str); // IDC_STC_DEF_P_HOLE_TOT

	nNum = pReelMap->GetDefNum(DEF_HOLE_MISS) + pReelMap->GetDefNum(DEF_HOLE_POSITION) + pReelMap->GetDefNum(DEF_HOLE_DEFECT);
	str.Format(_T("%d"), nNum);
	myStcData[79].SetText(str); // IDC_STC_DEF_H_MISS_TOT

	nNum = pReelMap->GetDefNum(DEF_VH_MISS) + pReelMap->GetDefNum(DEF_VH_POSITION) + pReelMap->GetDefNum(DEF_VH_DEF);
	str.Format(_T("%d"), nNum);
	myStcData[80].SetText(str); // IDC_STC_DEF_VH_MISS_TOT

}

void CDlgMenu06::SetRmapRgn()
{
	if(m_pMyGL)
	{
// 		m_pMyGL->ResetRgn();
		m_pMyGL->SetRgn();
	}
}

BOOL CDlgMenu06::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message != WM_KEYDOWN)
		return CDialog::PreTranslateMessage(pMsg);

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		case 'S':
		case 's':
			if(GetKeyState(VK_CONTROL) < 0) // Ctrl 키가 눌려진 상태
			{
				WINDOWPLACEMENT wndPlace;
				AfxGetMainWnd()->GetWindowPlacement(&wndPlace);
				wndPlace.showCmd |= SW_MAXIMIZE; 
				AfxGetMainWnd()->SetWindowPlacement(&wndPlace);	
			}
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgMenu06::LotEnd() 
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if(m_bLastProc)
	{
		if (MODE_INNER != pDoc->GetTestMode())
		{
			if (!pDoc->WorkingInfo.System.bUseEngrave)
			{
				m_bLastProcFromUp = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromUp"), m_bLastProcFromUp);
				pView->m_bWaitPcr[0] = FALSE;
			}
			else
			{
				m_bLastProcFromEng = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
			}
		}
		else
		{
			m_bLastProcFromEng = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
		}
		m_bLastProc = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
		myBtn[3].SetCheck(FALSE);
	}

	if(m_bLotEnd)
	{
		m_bLotEnd = FALSE;
		myBtn[6].SetCheck(FALSE);
			pView->MpeWrite(pView->Plc.DlgMenu01.JobEnd, 0);			// 작업종료(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
	}

	if(myBtn[4].GetCheck())
	{
		pView->m_bReMk = FALSE;
		myBtn[4].SetCheck(FALSE);
		//pView->MpeWrite(_T("MB440182"), 0);			// 재마킹(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
	}
}

void CDlgMenu06::SetLastProc()
{
	if(!m_bLastProc)
	{
		if (MODE_INNER != pDoc->GetTestMode())
		{
			if (!pDoc->WorkingInfo.System.bUseEngrave)
			{
				m_bLastProcFromUp = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromUp"), m_bLastProcFromUp);
				pView->m_bWaitPcr[0] = FALSE;
			}
			else
			{
				m_bLastProcFromEng = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
			}
		}
		else
		{
			m_bLastProcFromEng = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
		}
		m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
		myBtn[3].SetCheck(TRUE);
	}
}

BOOL CDlgMenu06::IsLastProc()
{
	return m_bLastProc;
}

void CDlgMenu06::OnChkEjectBuffer() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[3].GetCheck();
	if(bOn && !m_bLastProc && pView->IsBuffer())
	{
		if(IDNO == pView->MsgBox(_T("잔량처리를 하시겠습니까?"), 0, MB_YESNO))
			myBtn[3].SetCheck(FALSE);
		else
		{
			m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
		}
	}
	else
	{
		if(m_bLastProc && pView->IsBuffer())
		{
			if(IDYES == pView->MsgBox(_T("잔량처리를 해제 하시겠습니까?"), 0, MB_YESNO))
			{
				ResetLastProc();
			}
			else
				myBtn[3].SetCheck(TRUE);
		}
	}
	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::ResetLastProc()
{
	m_bLastProc = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
	m_bLastProcFromUp = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProcFromUp"), m_bLastProcFromUp);
	m_bLastProcFromEng = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
	//pView->MpeWrite(pView->Plc.DlgMenu01.LastJobFromEngrave, 0);			// 잔량처리 각인부 부터(PC가 On시키고, PLC가 확인하고 Off시킴)
	//pView->MpeWrite(pView->Plc.DlgMenu01.LastJobFromAoiUp, 0);			// 잔량처리 AOI(상) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
	//pView->MpeWrite(pView->Plc.DlgMenu01.LastJobFromAoiDn, 0);			// 잔량처리 AOI(상) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
	pView->MpeWrite(pView->Plc.DlgMenu01.DoLastJob, 0);			// 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
}

void CDlgMenu06::OnChkReview() 
{
	// TODO: Add your control notification handler code here
	pDoc->WorkingInfo.LastJob.bVerify = myBtn[2].GetCheck();

	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bVerify?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Verify"), sData, sPath);
	this->MoveWindow(m_pRect, TRUE);
}

BOOL CDlgMenu06::ShowKeypad(int nCtlID, CPoint ptSt, int nDir)
{
	BOOL bAdj = TRUE;
	CString strData, strPrev;
	GetDlgItemText(nCtlID, strData);
	strPrev = strData;

	CString strMin, strMax;
	CRect rect(0,0,0,0);
	CDlgKeyNum *pDlg = new CDlgKeyNum(&strData, &rect, ptSt, nDir);
	pDlg->m_strCurr = strData;
	pDlg->m_strMin = strMin = pDoc->GetMin(IDD_DLG_MENU_01, nCtlID);
	pDlg->m_strMax = strMax = pDoc->GetMax(IDD_DLG_MENU_01, nCtlID);
	pDlg->DoModal();
	delete pDlg;

	if(!strMin.IsEmpty() && !strMax.IsEmpty())
	{
		if(_tstof(strData) < _tstof(strMin) || 
			_tstof(strData) > _tstof(strMax))
		{
			SetDlgItemText(nCtlID, strPrev);
			//pView->DispMsg(_T("입력 범위를 벗어났습니다."), _T("주의"), RGB_YELLOW);
			pView->MsgBox(_T("입력 범위를 벗어났습니다."));
		}
		else
			SetDlgItemText(nCtlID, strData);
	}
	else
		SetDlgItemText(nCtlID, strData);
	
	return bAdj;
}

void CDlgMenu06::OnStcReviewLen() 
{
	// TODO: Add your control notification handler code here
	myStcData[20].SetBkColor(RGB_RED);
	myStcData[20].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_REVIEW_LEN)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_REVIEW_LEN, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[20].SetBkColor(RGB_WHITE);
	myStcData[20].RedrawWindow();
	
	CString sData, sPath=PATH_WORKING_INFO;
	GetDlgItem(IDC_STC_REVIEW_LEN)->GetWindowText(sData);
	
	pDoc->WorkingInfo.LastJob.sVerifyLen = sData; // [M]
	::WritePrivateProfileString(_T("Last Job"), _T("Verify Length"), sData, sPath);
}

void CDlgMenu06::OnChkReviewMove() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[5].GetCheck();
	if(bOn && !pDoc->WorkingInfo.LastJob.bReview)
	{
		;
	}
	else if(!bOn && pDoc->WorkingInfo.LastJob.bReview)
	{
//		if(IDNO == pView->DoMyMsgBox(_T("마킹위치 확인을 종료하시겠습니까?"), MB_YESNO))
		if(IDNO == pView->MsgBox(_T("마킹위치 확인을 종료하시겠습니까?"), 0, MB_YESNO))
		{
			myBtn[5].SetCheck(TRUE);
			return;
		}
	}

	pDoc->WorkingInfo.LastJob.bReview = bOn;

	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bReview?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Review"), sData, sPath);
	this->MoveWindow(m_pRect, TRUE);
}

int CDlgMenu06::GetCurSerial()
{
	return m_nSerial;
}

void CDlgMenu06::UpdateLotTime()
{
	CString sData;
	if (pDoc->m_pReelMapInner)
	{
		sData = pDoc->m_pReelMapInner->GetLotSt();
		myStcData[21].SetText(sData);
		sData = pDoc->m_pReelMapInner->GetLotEd();
		myStcData[23].SetText(sData);
	}
}

void CDlgMenu06::OnChkLotEnd() 
{
	// TODO: Add your control notification handler code here
	// IDC_CHK_LOT_END
	BOOL bOn = myBtn[6].GetCheck();
	if(bOn && !m_bLotEnd)
	{
//		if(IDNO == pView->DoMyMsgBox(_T("작업종료를 하시겠습니까?"), MB_YESNO))
		if(IDNO == pView->MsgBox(_T("작업종료를 하시겠습니까?"), 0, MB_YESNO))
			myBtn[6].SetCheck(FALSE);
		else
		{
			m_bLotEnd = TRUE;
			pView->MpeWrite(pView->Plc.DlgMenu01.JobEnd, 1);			// 작업종료(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
		}
	}
	else
	{
		if(m_bLotEnd)
		{
			pView->MsgBox(_T("현재 작업종료를 하고 있는 중입니다."));
			myBtn[6].SetCheck(TRUE);
		}
	}
	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::OnChkDefUp() 
{
	// TODO: Add your control notification handler code here
	int nOn = myBtn[12].GetCheck();
// 	int nOn = ((CButton*)GetDlgItem(IDC_CHK_DEF_UP))->GetCheck();

	//if(nOn)
	//	SelMap(UP);
	//else
	//	myBtn[12].SetCheck(TRUE);

// 		((CButton*)GetDlgItem(IDC_CHK_DEF_UP))->SetCheck(TRUE);

	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::OnChkDefDn() 
{
	// TODO: Add your control notification handler code here
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

	int nOn = myBtn[13].GetCheck();
// 	int nOn = ((CButton*)GetDlgItem(IDC_CHK_DEF_DN))->GetCheck();

	//if(nOn)
	//	SelMap(DN);
	//else
	//	myBtn[13].SetCheck(TRUE);

// 		((CButton*)GetDlgItem(IDC_CHK_DEF_DN))->SetCheck(TRUE);	

	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::OnChkDefAll() 
{
	// TODO: Add your control notification handler code here
	int nOn = myBtn[14].GetCheck();
// 	int nOn = ((CButton*)GetDlgItem(IDC_CHK_DEF_ALL))->GetCheck();

	//if(nOn)
	//	SelMap(ALL);
	//else
	//	myBtn[14].SetCheck(TRUE);

// 		((CButton*)GetDlgItem(IDC_CHK_DEF_ALL))->SetCheck(TRUE);	

	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::OnStcUp() 
{
	// TODO: Add your control notification handler code here
	myStcTitle[61].SetTextColor(RGB_WHITE);
	myStcTitle[61].SetBkColor(RGB_RED);
	myStcTitle[62].SetTextColor(RGB_WHITE);
	myStcTitle[62].SetBkColor(RGB_DARKBLUE);

	BOOL bShow=FALSE;
	if(pDoc->WorkingInfo.LastJob.nMergingLayer!=0)
		bShow=TRUE;

	pDoc->WorkingInfo.LastJob.nMergingLayer=0; 	// [0]:AOI-Up , [1]:AOI-Dn
	::WritePrivateProfileString(_T("Last Job"), _T("Merging Layer"), _T("0"), PATH_WORKING_INFO);
	//if(bShow)
	//	SelMap(ALL);
}

void CDlgMenu06::OnStcDn() 
{
	// TODO: Add your control notification handler code here
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if(!bDualTest)
		return;

	myStcTitle[61].SetTextColor(RGB_WHITE);
	myStcTitle[61].SetBkColor(RGB_DARKBLUE);
	myStcTitle[62].SetTextColor(RGB_WHITE);
	myStcTitle[62].SetBkColor(RGB_RED);

	BOOL bShow=FALSE;
	if(pDoc->WorkingInfo.LastJob.nMergingLayer!=1)
		bShow=TRUE;

	pDoc->WorkingInfo.LastJob.nMergingLayer=1; 	// [0]:AOI-Up , [1]:AOI-Dn
	::WritePrivateProfileString(_T("Last Job"), _T("Merging Layer"), _T("1"), PATH_WORKING_INFO);	
	//if(bShow)
	//	SelMap(ALL);
}

void CDlgMenu06::OnBtnMkAll() 
{
	// TODO: Add your control notification handler code here
	BOOL bEnable[4];
	bEnable[0] = myBtn[7].IsWindowEnabled();
	bEnable[1] = myBtn[8].IsWindowEnabled();
	bEnable[2] = myBtn[9].IsWindowEnabled();
	bEnable[3] = myBtn[10].IsWindowEnabled();

	if(!bEnable[0])
		myBtn[7].EnableWindow(TRUE);
	if(!bEnable[1])
		myBtn[8].EnableWindow(TRUE);
	if(!bEnable[2])
		myBtn[9].EnableWindow(TRUE);
	if(!bEnable[3])
		myBtn[10].EnableWindow(TRUE);

	SetStripAllMk();

	if(!bEnable[0])
		myBtn[7].EnableWindow(FALSE);
	if(!bEnable[1])
		myBtn[8].EnableWindow(FALSE);
	if(!bEnable[2])
		myBtn[9].EnableWindow(FALSE);
	if(!bEnable[3])
		myBtn[10].EnableWindow(FALSE);

	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::SetStripAllMk()
{
	myBtn[7].SetCheck(TRUE); // IDC_CHK_MK_1
	myBtn[8].SetCheck(TRUE); // IDC_CHK_MK_2
	myBtn[9].SetCheck(TRUE); // IDC_CHK_MK_3
	myBtn[10].SetCheck(TRUE); // IDC_CHK_MK_4

	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::EnableBtn(BOOL bEnable)
{
	m_bEnableMkStrip = bEnable;
	myBtn[7].EnableWindow(bEnable); // IDC_CHK_MK_1
	myBtn[8].EnableWindow(bEnable); // IDC_CHK_MK_2
	myBtn[9].EnableWindow(bEnable); // IDC_CHK_MK_3
	myBtn[10].EnableWindow(bEnable); // IDC_CHK_MK_4
}

BOOL CDlgMenu06::IsEnableBtn()
{
	return m_bEnableMkStrip;
}

void CDlgMenu06::SetTempStop(BOOL bOn)
{
	pDoc->WorkingInfo.LastJob.bTempPause = bOn;
	if(bOn)
	{
		myBtn[0].SetCheck(TRUE);
		pView->MpeWrite(pView->Plc.DlgInfo.UseStopLength, 1);
	}
	else
	{
		myBtn[0].SetCheck(FALSE);
		pView->MpeWrite(pView->Plc.DlgInfo.UseStopLength, 0);
	}
}

BOOL CDlgMenu06::GetChkStrip(int nStrip) // 0, 1, 2, 3
{
	BOOL bOn = TRUE;
	switch(nStrip)
	{
	case 0:
		if(!myBtn[7].GetCheck()) // IDC_CHK_MK_1
			bOn = FALSE;
		break;
	case 1:
		if(!myBtn[8].GetCheck()) // IDC_CHK_MK_2
			bOn = FALSE;
		break;
	case 2:
		if(!myBtn[9].GetCheck()) // IDC_CHK_MK_3
			bOn = FALSE;
		break;
	case 3:
		if(!myBtn[10].GetCheck()) // IDC_CHK_MK_4
			bOn = FALSE;
		break;
	}

	return bOn;
}

void CDlgMenu06::DispMkCnt()
{
	CString str;

	str.Format(_T("%d"), pDoc->GetMkCntL());
	myStcData[81].SetText(str);
	pDoc->SetMkMenu01(_T("Data"), _T("MkNumLf"), str);

	str.Format(_T("%d"), pDoc->GetMkCntR());
	myStcData[82].SetText(str);
	pDoc->SetMkMenu01(_T("Data"), _T("MkNumRt"), str);

}

void CDlgMenu06::ChkMkLimit()
{
	int nCurrL = pDoc->GetMkCntL();
	int nLmtL = pDoc->GetMkLimitL();
	int nCurrR = pDoc->GetMkCntR();
	int nLmtR = pDoc->GetMkLimitR();

	if(nCurrL > nLmtL)
	{
		if(!IsWinkMkL())
			WinkMkL(TRUE);
	}
	else
	{
		if(IsWinkMkL())
			WinkMkL(FALSE);
	}

	if(nCurrR > nLmtR)
	{
		if(!IsWinkMkR())
			WinkMkR(TRUE);
	}
	else
	{
		if(IsWinkMkR())
			WinkMkR(FALSE);
	}
}

void CDlgMenu06::WriteMkCnt()
{
	pDoc->SaveMkCntL();
	pDoc->SaveMkCntR();
}

BOOL CDlgMenu06::IsWinkMkL()
{
	return m_bWinkMk[0];
}

void CDlgMenu06::WinkMkL(BOOL bOn)
{
	m_bWinkMk[0] = bOn;
	if(m_bWinkMk[0] && !m_bTIM_WINK_MK_L)
	{
		m_nWinkMk[0] = 0;
		m_bTIM_WINK_MK_L = TRUE;
		SetTimer(TIM_WINK_MK_L, 500, NULL);
	}
	else if(!m_bWinkMk[0] && m_bTIM_WINK_MK_L)
		m_bTIM_WINK_MK_L = FALSE;
}

BOOL CDlgMenu06::IsWinkMkR()
{
	return m_bWinkMk[1];
}

void CDlgMenu06::WinkMkR(BOOL bOn)
{
	m_bWinkMk[1] = bOn;
	if(m_bWinkMk[1] && !m_bTIM_WINK_MK_R)
	{
		m_nWinkMk[1] = 0;
		m_bTIM_WINK_MK_R = TRUE;
		SetTimer(TIM_WINK_MK_R, 500, NULL);
	}
	else if(!m_bWinkMk[1] && m_bTIM_WINK_MK_R)
		m_bTIM_WINK_MK_R = FALSE;
}

void CDlgMenu06::OnChkRemarking() 
{
	// TODO: Add your control notification handler code here

	BOOL bOn = myBtn[4].GetCheck();
	if(bOn)
	{
		if(IDNO == pView->MsgBox(_T("마킹 작업을 하시겠습니까?"), 0, MB_YESNO))
			myBtn[4].SetCheck(FALSE);
		else
		{
			myBtn[4].SetCheck(FALSE);
			pView->m_bMkStSw[0] = TRUE;
			pView->m_bMkStSw[1] = TRUE;
		}
	}
	else
	{
		;
	}

	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::ChkPartialSpd()
{
	if(pDoc->Status.bAuto)
	{
		if(pView->IsRun())
		{
			if(pView->m_nStepAuto >= AT_LP)
			{
				double dVel = pView->GetPartVel();
				if(dVel < _tstof(pDoc->WorkingInfo.LastJob.sPartialSpd) && !m_bLowPartialSpd)
				{
					m_bLowPartialSpd = TRUE;
					myStcData[8].SetFontSize(13);
					myStcData[8].SetFontBold(TRUE);
					myStcData[8].SetTextColor(RGB_WHITE);
					myStcData[8].SetBkColor(RGB_RED);
					return;
				}
				else if(dVel >= _tstof(pDoc->WorkingInfo.LastJob.sPartialSpd) && m_bLowPartialSpd)
				{
					m_bLowPartialSpd = FALSE;
					myStcData[8].SetFontSize(12);
					myStcData[8].SetFontBold(FALSE);
					myStcData[8].SetTextColor(RGB_BLACK);
					myStcData[8].SetBkColor(RGB_WHITE);
				}
			}
		}
	}
}

void CDlgMenu06::OnChkElec() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[21].GetCheck();
	if(bOn)
	{
		myBtn[21].SetCheck(TRUE);
		pDoc->WorkingInfo.Probing[0].bUse = pDoc->WorkingInfo.Probing[1].bUse = TRUE;
	}
	else
	{
		if(IDYES == pView->MsgBox(_T("쇼트 검사를 정말 하지 않겠습니까?"), 0, MB_YESNO))
		{
			myBtn[21].SetCheck(FALSE);
			pDoc->WorkingInfo.Probing[0].bUse = pDoc->WorkingInfo.Probing[1].bUse = FALSE;
		}
		else
		{
			myBtn[21].SetCheck(TRUE);
			pDoc->WorkingInfo.Probing[0].bUse = pDoc->WorkingInfo.Probing[1].bUse = TRUE;
		}
	}
	
	if(pDoc->WorkingInfo.Probing[0].bUse)
		::WritePrivateProfileString(_T("Elec Check"), _T("Use"), _T("1"), PATH_WORKING_INFO);
	else
		::WritePrivateProfileString(_T("Elec Check"), _T("Use"), _T("0"), PATH_WORKING_INFO);
	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::OnChkElecStop() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHK_ELEC_STOP))->GetCheck();	
	if(bOn)
	{
		pDoc->WorkingInfo.Probing[0].bStopOnOpen = pDoc->WorkingInfo.Probing[1].bStopOnOpen = TRUE;
	}
	else
	{
		if(IDYES == pView->MsgBox(_T("Open시에도 그냥 작업을 진행하시겠습니까?"), 0, MB_YESNO))
		{
			myBtn[22].SetCheck(FALSE);
			pDoc->WorkingInfo.Probing[0].bStopOnOpen = pDoc->WorkingInfo.Probing[1].bStopOnOpen = FALSE;
		}
		else
		{
			myBtn[22].SetCheck(TRUE);
			pDoc->WorkingInfo.Probing[0].bStopOnOpen = pDoc->WorkingInfo.Probing[1].bStopOnOpen = TRUE;
		}

	}
	
	if(pDoc->WorkingInfo.Probing[0].bStopOnOpen)
		::WritePrivateProfileString(_T("Fluck"), _T("StopOnOpen"), _T("1"), PATH_WORKING_INFO);
	else
		::WritePrivateProfileString(_T("Fluck"), _T("StopOnOpen"), _T("0"), PATH_WORKING_INFO);
	this->MoveWindow(m_pRect, TRUE);
}

void CDlgMenu06::SetDualTest(BOOL bOn)
{
	bOn = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if(bOn)
	{
		ShiftCtrlPos(bOn);

		if(!myStcTitle[61].IsWindowVisible())
			myStcTitle[61].ShowWindow(SW_SHOW);
		if(!myStcTitle[62].IsWindowVisible())
			myStcTitle[62].ShowWindow(SW_SHOW);

		if(!myStcTitle[63].IsWindowEnabled())
			myStcTitle[63].EnableWindow(TRUE);
		if(!myStcData[75].IsWindowEnabled())
			myStcData[75].EnableWindow(TRUE);

		if(!myStcData[13].IsWindowEnabled())
			myStcData[13].EnableWindow(TRUE);

		if(!myStcTitle[50].IsWindowEnabled())
			myStcTitle[50].EnableWindow(TRUE);
		if(!myStcData[49].IsWindowEnabled())
			myStcData[49].EnableWindow(TRUE);
		if(!myStcData[50].IsWindowEnabled())
			myStcData[50].EnableWindow(TRUE);
		if(!myStcData[51].IsWindowEnabled())
			myStcData[51].EnableWindow(TRUE);
		if(!myStcData[52].IsWindowEnabled())
			myStcData[52].EnableWindow(TRUE);
		if(!myStcData[53].IsWindowEnabled())
			myStcData[53].EnableWindow(TRUE);

		if(!myStcTitle[51].IsWindowEnabled())
			myStcTitle[51].EnableWindow(TRUE);
		if(!myStcData[54].IsWindowEnabled())
			myStcData[54].EnableWindow(TRUE);
		if(!myStcData[55].IsWindowEnabled())
			myStcData[55].EnableWindow(TRUE);
		if(!myStcData[56].IsWindowEnabled())
			myStcData[56].EnableWindow(TRUE);
		if(!myStcData[57].IsWindowEnabled())
			myStcData[57].EnableWindow(TRUE);
		if(!myStcData[58].IsWindowEnabled())
			myStcData[58].EnableWindow(TRUE);

		if(!myStcTitle[54].IsWindowEnabled())
			myStcTitle[54].EnableWindow(TRUE);
		if(!myStcData[64].IsWindowEnabled())
			myStcData[64].EnableWindow(TRUE);
		if(!myStcData[65].IsWindowEnabled())
			myStcData[65].EnableWindow(TRUE);
		if(!myStcData[66].IsWindowEnabled())
			myStcData[66].EnableWindow(TRUE);
		if(!myStcData[67].IsWindowEnabled())
			myStcData[67].EnableWindow(TRUE);
		if(!myStcData[68].IsWindowEnabled())
			myStcData[68].EnableWindow(TRUE);

		if(!myStcTitle[55].IsWindowEnabled())
			myStcTitle[55].EnableWindow(TRUE);
		if(!myStcData[69].IsWindowEnabled())
			myStcData[69].EnableWindow(TRUE);
		if(!myStcData[70].IsWindowEnabled())
			myStcData[70].EnableWindow(TRUE);
		if(!myStcData[71].IsWindowEnabled())
			myStcData[71].EnableWindow(TRUE);
		if(!myStcData[72].IsWindowEnabled())
			myStcData[72].EnableWindow(TRUE);
		if(!myStcData[73].IsWindowEnabled())
			myStcData[73].EnableWindow(TRUE);
		
		if(!myBtn[13].IsWindowEnabled())
			myBtn[13].EnableWindow(TRUE);
	}
	else
	{
		if(myStcTitle[61].IsWindowVisible())
			myStcTitle[61].ShowWindow(SW_HIDE);
		if(myStcTitle[62].IsWindowVisible())
			myStcTitle[62].ShowWindow(SW_HIDE);

		//if(myBtn[12].IsWindowVisible())
		//	myBtn[12].ShowWindow(SW_HIDE);
		//if(myBtn[13].IsWindowVisible())
		//	myBtn[13].ShowWindow(SW_HIDE);
		//if(myBtn[14].IsWindowVisible())
		//	myBtn[14].ShowWindow(SW_HIDE);
		
		ShiftCtrlPos(bOn);

		if(myStcTitle[63].IsWindowEnabled())
			myStcTitle[63].EnableWindow(FALSE);
		if(myStcData[75].IsWindowEnabled())
			myStcData[75].EnableWindow(FALSE);

		if(myStcData[13].IsWindowEnabled())
			myStcData[13].EnableWindow(FALSE);

		if(myStcTitle[50].IsWindowEnabled())
			myStcTitle[50].EnableWindow(FALSE);
		if(myStcData[49].IsWindowEnabled())
			myStcData[49].EnableWindow(FALSE);
		if(myStcData[50].IsWindowEnabled())
			myStcData[50].EnableWindow(FALSE);
		if(myStcData[51].IsWindowEnabled())
			myStcData[51].EnableWindow(FALSE);
		if(myStcData[52].IsWindowEnabled())
			myStcData[52].EnableWindow(FALSE);
		if(myStcData[53].IsWindowEnabled())
			myStcData[53].EnableWindow(FALSE);

		if(myStcTitle[51].IsWindowEnabled())
			myStcTitle[51].EnableWindow(FALSE);
		if(myStcData[54].IsWindowEnabled())
			myStcData[54].EnableWindow(FALSE);
		if(myStcData[55].IsWindowEnabled())
			myStcData[55].EnableWindow(FALSE);
		if(myStcData[56].IsWindowEnabled())
			myStcData[56].EnableWindow(FALSE);
		if(myStcData[57].IsWindowEnabled())
			myStcData[57].EnableWindow(FALSE);
		if(myStcData[58].IsWindowEnabled())
			myStcData[58].EnableWindow(FALSE);

		if(myStcTitle[54].IsWindowEnabled())
			myStcTitle[54].EnableWindow(FALSE);
		if(myStcData[64].IsWindowEnabled())
			myStcData[64].EnableWindow(FALSE);
		if(myStcData[65].IsWindowEnabled())
			myStcData[65].EnableWindow(FALSE);
		if(myStcData[66].IsWindowEnabled())
			myStcData[66].EnableWindow(FALSE);
		if(myStcData[67].IsWindowEnabled())
			myStcData[67].EnableWindow(FALSE);
		if(myStcData[68].IsWindowEnabled())
			myStcData[68].EnableWindow(FALSE);

		if(myStcTitle[55].IsWindowEnabled())
			myStcTitle[55].EnableWindow(FALSE);
		if(myStcData[69].IsWindowEnabled())
			myStcData[69].EnableWindow(FALSE);
		if(myStcData[70].IsWindowEnabled())
			myStcData[70].EnableWindow(FALSE);
		if(myStcData[71].IsWindowEnabled())
			myStcData[71].EnableWindow(FALSE);
		if(myStcData[72].IsWindowEnabled())
			myStcData[72].EnableWindow(FALSE);
		if(myStcData[73].IsWindowEnabled())
			myStcData[73].EnableWindow(FALSE);
		
		if(myBtn[13].IsWindowEnabled())
			myBtn[13].EnableWindow(FALSE);
	}

	//SelDisp();
	InitMkInfo();
}

void CDlgMenu06::GetCtrlPos()
{
	GetDlgItem(IDC_STC_UP)->GetWindowRect(&m_RectTitle[0]);
	GetDlgItem(IDC_STC_DN)->GetWindowRect(&m_RectTitle[1]);
	GetDlgItem(IDC_PIC_CAD_001)->GetWindowRect(&m_RectCAD[0]);
	GetDlgItem(IDC_PIC_CAD_002)->GetWindowRect(&m_RectCAD[1]);
	GetDlgItem(IDC_PIC_CAD_003)->GetWindowRect(&m_RectCAD[2]);
	GetDlgItem(IDC_PIC_CAD_004)->GetWindowRect(&m_RectCAD[3]);
	GetDlgItem(IDC_PIC_CAD_005)->GetWindowRect(&m_RectCAD[4]);
	GetDlgItem(IDC_PIC_CAD_006)->GetWindowRect(&m_RectCAD[5]);
	GetDlgItem(IDC_PIC_CAD_007)->GetWindowRect(&m_RectCAD[6]);
	GetDlgItem(IDC_PIC_CAD_008)->GetWindowRect(&m_RectCAD[7]);
	GetDlgItem(IDC_PIC_CAD_009)->GetWindowRect(&m_RectCAD[8]);
	GetDlgItem(IDC_PIC_CAD_010)->GetWindowRect(&m_RectCAD[9]);
	GetDlgItem(IDC_PIC_CAD_011)->GetWindowRect(&m_RectCAD[10]);
	GetDlgItem(IDC_PIC_CAD_012)->GetWindowRect(&m_RectCAD[11]);
	GetDlgItem(IDC_PIC_DEF_001)->GetWindowRect(&m_RectDef[0]);
	GetDlgItem(IDC_PIC_DEF_002)->GetWindowRect(&m_RectDef[1]);
	GetDlgItem(IDC_PIC_DEF_003)->GetWindowRect(&m_RectDef[2]);
	GetDlgItem(IDC_PIC_DEF_004)->GetWindowRect(&m_RectDef[3]);
	GetDlgItem(IDC_PIC_DEF_005)->GetWindowRect(&m_RectDef[4]);
	GetDlgItem(IDC_PIC_DEF_006)->GetWindowRect(&m_RectDef[5]);
	GetDlgItem(IDC_PIC_DEF_007)->GetWindowRect(&m_RectDef[6]);
	GetDlgItem(IDC_PIC_DEF_008)->GetWindowRect(&m_RectDef[7]);
	GetDlgItem(IDC_PIC_DEF_009)->GetWindowRect(&m_RectDef[8]);
	GetDlgItem(IDC_PIC_DEF_010)->GetWindowRect(&m_RectDef[9]);
	GetDlgItem(IDC_PIC_DEF_011)->GetWindowRect(&m_RectDef[10]);
	GetDlgItem(IDC_PIC_DEF_012)->GetWindowRect(&m_RectDef[11]);
	GetDlgItem(IDC_EDIT_DEF_INFO_001)->GetWindowRect(&m_RectInfo[0]);
	GetDlgItem(IDC_EDIT_DEF_INFO_002)->GetWindowRect(&m_RectInfo[1]);
	GetDlgItem(IDC_EDIT_DEF_INFO_003)->GetWindowRect(&m_RectInfo[2]);
	GetDlgItem(IDC_EDIT_DEF_INFO_004)->GetWindowRect(&m_RectInfo[3]);
	GetDlgItem(IDC_EDIT_DEF_INFO_005)->GetWindowRect(&m_RectInfo[4]);
	GetDlgItem(IDC_EDIT_DEF_INFO_006)->GetWindowRect(&m_RectInfo[5]);
	GetDlgItem(IDC_EDIT_DEF_INFO_007)->GetWindowRect(&m_RectInfo[6]);
	GetDlgItem(IDC_EDIT_DEF_INFO_008)->GetWindowRect(&m_RectInfo[7]);
	GetDlgItem(IDC_EDIT_DEF_INFO_009)->GetWindowRect(&m_RectInfo[8]);
	GetDlgItem(IDC_EDIT_DEF_INFO_010)->GetWindowRect(&m_RectInfo[9]);
	GetDlgItem(IDC_EDIT_DEF_INFO_011)->GetWindowRect(&m_RectInfo[10]);
	GetDlgItem(IDC_EDIT_DEF_INFO_012)->GetWindowRect(&m_RectInfo[11]);
}

void CDlgMenu06::ShiftCtrlPos(BOOL bOrg)
{
	CRect rt;

	int nSyX = GetSystemMetrics(SM_CXEDGE);
	int nSyY = GetSystemMetrics(SM_CYCAPTION);

	if(bOrg)
	{
		rt.left = m_RectCAD[0].left - nSyX; rt.top = m_RectCAD[0].top - nSyY;
		rt.right = m_RectCAD[0].right - nSyX; rt.bottom = m_RectCAD[0].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_001)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[1].left - nSyX; rt.top = m_RectCAD[1].top - nSyY;
		rt.right = m_RectCAD[1].right - nSyX; rt.bottom = m_RectCAD[1].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_002)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[2].left - nSyX; rt.top = m_RectCAD[2].top - nSyY;
		rt.right = m_RectCAD[2].right - nSyX; rt.bottom = m_RectCAD[2].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_003)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[3].left - nSyX; rt.top = m_RectCAD[3].top - nSyY;
		rt.right = m_RectCAD[3].right - nSyX; rt.bottom = m_RectCAD[3].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_004)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[4].left - nSyX; rt.top = m_RectCAD[4].top - nSyY;
		rt.right = m_RectCAD[4].right - nSyX; rt.bottom = m_RectCAD[4].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_005)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[5].left - nSyX; rt.top = m_RectCAD[5].top - nSyY;
		rt.right = m_RectCAD[5].right - nSyX; rt.bottom = m_RectCAD[5].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_006)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[6].left - nSyX; rt.top = m_RectCAD[6].top - nSyY;
		rt.right = m_RectCAD[6].right - nSyX; rt.bottom = m_RectCAD[6].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_007)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[7].left - nSyX; rt.top = m_RectCAD[7].top - nSyY;
		rt.right = m_RectCAD[7].right - nSyX; rt.bottom = m_RectCAD[7].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_008)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[8].left - nSyX; rt.top = m_RectCAD[8].top - nSyY;
		rt.right = m_RectCAD[8].right - nSyX; rt.bottom = m_RectCAD[8].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_009)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[9].left - nSyX; rt.top = m_RectCAD[9].top - nSyY;
		rt.right = m_RectCAD[9].right - nSyX; rt.bottom = m_RectCAD[9].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_010)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[10].left - nSyX; rt.top = m_RectCAD[10].top - nSyY;
		rt.right = m_RectCAD[10].right - nSyX; rt.bottom = m_RectCAD[10].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_011)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[11].left - nSyX; rt.top = m_RectCAD[11].top - nSyY;
		rt.right = m_RectCAD[11].right - nSyX; rt.bottom = m_RectCAD[11].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_012)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[0].left - nSyX; rt.top = m_RectDef[0].top - nSyY;
		rt.right = m_RectDef[0].right - nSyX; rt.bottom = m_RectDef[0].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_001)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[1].left - nSyX; rt.top = m_RectDef[1].top - nSyY;
		rt.right = m_RectDef[1].right - nSyX; rt.bottom = m_RectDef[1].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_002)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[2].left - nSyX; rt.top = m_RectDef[2].top - nSyY;
		rt.right = m_RectDef[2].right - nSyX; rt.bottom = m_RectDef[2].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_003)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[3].left - nSyX; rt.top = m_RectDef[3].top - nSyY;
		rt.right = m_RectDef[3].right - nSyX; rt.bottom = m_RectDef[3].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_004)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[4].left - nSyX; rt.top = m_RectDef[4].top - nSyY;
		rt.right = m_RectDef[4].right - nSyX; rt.bottom = m_RectDef[4].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_005)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[5].left - nSyX; rt.top = m_RectDef[5].top - nSyY;
		rt.right = m_RectDef[5].right - nSyX; rt.bottom = m_RectDef[5].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_006)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[6].left - nSyX; rt.top = m_RectDef[6].top - nSyY;
		rt.right = m_RectDef[6].right - nSyX; rt.bottom = m_RectDef[6].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_007)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[7].left - nSyX; rt.top = m_RectDef[7].top - nSyY;
		rt.right = m_RectDef[7].right - nSyX; rt.bottom = m_RectDef[7].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_008)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[8].left - nSyX; rt.top = m_RectDef[8].top - nSyY;
		rt.right = m_RectDef[8].right - nSyX; rt.bottom = m_RectDef[8].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_009)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[9].left - nSyX; rt.top = m_RectDef[9].top - nSyY;
		rt.right = m_RectDef[9].right - nSyX; rt.bottom = m_RectDef[9].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_010)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[10].left - nSyX; rt.top = m_RectDef[10].top - nSyY;
		rt.right = m_RectDef[10].right - nSyX; rt.bottom = m_RectDef[10].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_011)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[11].left - nSyX; rt.top = m_RectDef[11].top - nSyY;
		rt.right = m_RectDef[11].right - nSyX; rt.bottom = m_RectDef[11].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_012)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[0].left - nSyX; rt.top = m_RectInfo[0].top - nSyY;
		rt.right = m_RectInfo[0].right - nSyX; rt.bottom = m_RectInfo[0].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_001)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[1].left - nSyX; rt.top = m_RectInfo[1].top - nSyY;
		rt.right = m_RectInfo[1].right - nSyX; rt.bottom = m_RectInfo[1].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_002)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[2].left - nSyX; rt.top = m_RectInfo[2].top - nSyY;
		rt.right = m_RectInfo[2].right - nSyX; rt.bottom = m_RectInfo[2].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_003)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[3].left - nSyX; rt.top = m_RectInfo[3].top - nSyY;
		rt.right = m_RectInfo[3].right - nSyX; rt.bottom = m_RectInfo[3].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_004)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[4].left - nSyX; rt.top = m_RectInfo[4].top - nSyY;
		rt.right = m_RectInfo[4].right - nSyX; rt.bottom = m_RectInfo[4].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_005)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[5].left - nSyX; rt.top = m_RectInfo[5].top - nSyY;
		rt.right = m_RectInfo[5].right - nSyX; rt.bottom = m_RectInfo[5].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_006)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[6].left - nSyX; rt.top = m_RectInfo[6].top - nSyY;
		rt.right = m_RectInfo[6].right - nSyX; rt.bottom = m_RectInfo[6].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_007)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[7].left - nSyX; rt.top = m_RectInfo[7].top - nSyY;
		rt.right = m_RectInfo[7].right - nSyX; rt.bottom = m_RectInfo[7].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_008)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[8].left - nSyX; rt.top = m_RectInfo[8].top - nSyY;
		rt.right = m_RectInfo[8].right - nSyX; rt.bottom = m_RectInfo[8].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_009)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[9].left - nSyX; rt.top = m_RectInfo[9].top - nSyY;
		rt.right = m_RectInfo[9].right - nSyX; rt.bottom = m_RectInfo[9].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_010)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[10].left - nSyX; rt.top = m_RectInfo[10].top - nSyY;
		rt.right = m_RectInfo[10].right - nSyX; rt.bottom = m_RectInfo[10].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_011)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[11].left - nSyX; rt.top = m_RectInfo[11].top - nSyY;
		rt.right = m_RectInfo[11].right - nSyX; rt.bottom = m_RectInfo[11].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_012)->MoveWindow(&rt, TRUE);
	}
	else
	{
		rt.left = m_RectCAD[0].left - nSyX - 11; rt.top = m_RectCAD[0].top - nSyY;
		rt.right = m_RectCAD[0].right - nSyX - 11; rt.bottom = m_RectCAD[0].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_001)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[1].left - nSyX - 9; rt.top = m_RectCAD[1].top - nSyY;
		rt.right = m_RectCAD[1].right - nSyX - 9; rt.bottom = m_RectCAD[1].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_002)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[2].left - nSyX - 7; rt.top = m_RectCAD[2].top - nSyY;
		rt.right = m_RectCAD[2].right - nSyX - 7; rt.bottom = m_RectCAD[2].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_003)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[3].left - nSyX - 5; rt.top = m_RectCAD[3].top - nSyY;
		rt.right = m_RectCAD[3].right - nSyX - 5; rt.bottom = m_RectCAD[3].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_004)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[4].left - nSyX - 3; rt.top = m_RectCAD[4].top - nSyY;
		rt.right = m_RectCAD[4].right - nSyX - 3; rt.bottom = m_RectCAD[4].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_005)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[5].left - nSyX- 1; rt.top = m_RectCAD[5].top - nSyY;
		rt.right = m_RectCAD[5].right - nSyX - 1; rt.bottom = m_RectCAD[5].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_006)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[6].left - nSyX - 23; rt.top = m_RectCAD[6].top - nSyY;
		rt.right = m_RectCAD[6].right - nSyX - 23; rt.bottom = m_RectCAD[6].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_007)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[7].left - nSyX - 21; rt.top = m_RectCAD[7].top - nSyY;
		rt.right = m_RectCAD[7].right - nSyX - 21; rt.bottom = m_RectCAD[7].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_008)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[8].left - nSyX - 19; rt.top = m_RectCAD[8].top - nSyY;
		rt.right = m_RectCAD[8].right - nSyX - 19; rt.bottom = m_RectCAD[8].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_009)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[9].left - nSyX - 17; rt.top = m_RectCAD[9].top - nSyY;
		rt.right = m_RectCAD[9].right - nSyX - 17; rt.bottom = m_RectCAD[9].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_010)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[10].left - nSyX - 15; rt.top = m_RectCAD[10].top - nSyY;
		rt.right = m_RectCAD[10].right - nSyX - 15; rt.bottom = m_RectCAD[10].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_011)->MoveWindow(&rt, TRUE);

		rt.left = m_RectCAD[11].left - nSyX - 13; rt.top = m_RectCAD[11].top - nSyY;
		rt.right = m_RectCAD[11].right - nSyX - 13; rt.bottom = m_RectCAD[11].bottom - nSyY;
		GetDlgItem(IDC_PIC_CAD_012)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[0].left - nSyX - 11; rt.top = m_RectDef[0].top - nSyY;
		rt.right = m_RectDef[0].right - nSyX - 11; rt.bottom = m_RectDef[0].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_001)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[1].left - nSyX - 9; rt.top = m_RectDef[1].top - nSyY;
		rt.right = m_RectDef[1].right - nSyX - 9; rt.bottom = m_RectDef[1].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_002)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[2].left - nSyX - 7; rt.top = m_RectDef[2].top - nSyY;
		rt.right = m_RectDef[2].right - nSyX - 7; rt.bottom = m_RectDef[2].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_003)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[3].left - nSyX - 5; rt.top = m_RectDef[3].top - nSyY;
		rt.right = m_RectDef[3].right - nSyX - 5; rt.bottom = m_RectDef[3].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_004)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[4].left - nSyX - 3; rt.top = m_RectDef[4].top - nSyY;
		rt.right = m_RectDef[4].right - nSyX - 3; rt.bottom = m_RectDef[4].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_005)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[5].left - nSyX - 1; rt.top = m_RectDef[5].top - nSyY;
		rt.right = m_RectDef[5].right - nSyX - 1; rt.bottom = m_RectDef[5].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_006)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[6].left - nSyX - 23; rt.top = m_RectDef[6].top - nSyY;
		rt.right = m_RectDef[6].right - nSyX - 23; rt.bottom = m_RectDef[6].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_007)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[7].left - nSyX - 21; rt.top = m_RectDef[7].top - nSyY;
		rt.right = m_RectDef[7].right - nSyX - 21; rt.bottom = m_RectDef[7].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_008)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[8].left - nSyX - 19; rt.top = m_RectDef[8].top - nSyY;
		rt.right = m_RectDef[8].right - nSyX - 19; rt.bottom = m_RectDef[8].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_009)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[9].left - nSyX - 17; rt.top = m_RectDef[9].top - nSyY;
		rt.right = m_RectDef[9].right - nSyX - 17; rt.bottom = m_RectDef[9].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_010)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[10].left - nSyX - 15; rt.top = m_RectDef[10].top - nSyY;
		rt.right = m_RectDef[10].right - nSyX - 15; rt.bottom = m_RectDef[10].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_011)->MoveWindow(&rt, TRUE);

		rt.left = m_RectDef[11].left - nSyX - 13; rt.top = m_RectDef[11].top - nSyY;
		rt.right = m_RectDef[11].right - nSyX - 13; rt.bottom = m_RectDef[11].bottom - nSyY;
		GetDlgItem(IDC_PIC_DEF_012)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[0].left - nSyX - 11; rt.top = m_RectInfo[0].top - nSyY;
		rt.right = m_RectInfo[0].right - nSyX - 11; rt.bottom = m_RectInfo[0].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_001)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[1].left - nSyX - 9; rt.top = m_RectInfo[1].top - nSyY;
		rt.right = m_RectInfo[1].right - nSyX - 9; rt.bottom = m_RectInfo[1].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_002)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[2].left - nSyX - 7; rt.top = m_RectInfo[2].top - nSyY;
		rt.right = m_RectInfo[2].right - nSyX - 7; rt.bottom = m_RectInfo[2].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_003)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[3].left - nSyX - 5; rt.top = m_RectInfo[3].top - nSyY;
		rt.right = m_RectInfo[3].right - nSyX - 5; rt.bottom = m_RectInfo[3].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_004)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[4].left - nSyX - 3; rt.top = m_RectInfo[4].top - nSyY;
		rt.right = m_RectInfo[4].right - nSyX - 3; rt.bottom = m_RectInfo[4].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_005)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[5].left - nSyX - 1; rt.top = m_RectInfo[5].top - nSyY;
		rt.right = m_RectInfo[5].right - nSyX - 1; rt.bottom = m_RectInfo[5].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_006)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[6].left - nSyX - 23; rt.top = m_RectInfo[6].top - nSyY;
		rt.right = m_RectInfo[6].right - nSyX - 23; rt.bottom = m_RectInfo[6].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_007)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[7].left - nSyX - 21; rt.top = m_RectInfo[7].top - nSyY;
		rt.right = m_RectInfo[7].right - nSyX - 21; rt.bottom = m_RectInfo[7].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_008)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[8].left - nSyX - 19; rt.top = m_RectInfo[8].top - nSyY;
		rt.right = m_RectInfo[8].right - nSyX - 19; rt.bottom = m_RectInfo[8].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_009)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[9].left - nSyX - 17; rt.top = m_RectInfo[9].top - nSyY;
		rt.right = m_RectInfo[9].right - nSyX - 17; rt.bottom = m_RectInfo[9].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_010)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[10].left - nSyX - 15; rt.top = m_RectInfo[10].top - nSyY;
		rt.right = m_RectInfo[10].right - nSyX - 15; rt.bottom = m_RectInfo[10].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_011)->MoveWindow(&rt, TRUE);

		rt.left = m_RectInfo[11].left - nSyX - 13; rt.top = m_RectInfo[11].top - nSyY;
		rt.right = m_RectInfo[11].right - nSyX - 13; rt.bottom = m_RectInfo[11].bottom - nSyY;
		GetDlgItem(IDC_EDIT_DEF_INFO_012)->MoveWindow(&rt, TRUE);
	}

	this->RedrawWindow();
}

void CDlgMenu06::OnChk2layer() 
{
	// TODO: Add your control notification handler code here
	BOOL bUse = ((CButton*)GetDlgItem(IDC_CHK_2LAYER))->GetCheck();

	pDoc->WorkingInfo.LastJob.bUse2Layer = bUse;

	CString sData = bUse ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use 2Layer"), sData, PATH_WORKING_INFO);

	this->MoveWindow(m_pRect, TRUE);
}


LRESULT CDlgMenu06::OnMyBtnDblClk(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;
	BOOL bOn;

	switch (nCtrlID)
	{
	case IDC_CHK_MK_1:
		bOn = myBtn[7].GetCheck();
		if (!bOn)
			myBtn[7].SetCheck(TRUE); // IDC_CHK_MK_1
		else
			myBtn[7].SetCheck(FALSE); // IDC_CHK_MK_1
		break;
	case IDC_CHK_MK_2:
		bOn = myBtn[8].GetCheck();
		if (!bOn)
			myBtn[8].SetCheck(TRUE); // IDC_CHK_MK_2
		else
			myBtn[8].SetCheck(FALSE); // IDC_CHK_MK_2
		break;
	case IDC_CHK_MK_3:
		bOn = myBtn[9].GetCheck();
		if (!bOn)
			myBtn[9].SetCheck(TRUE); // IDC_CHK_MK_3
		else
			myBtn[9].SetCheck(FALSE); // IDC_CHK_MK_3
		break;
	case IDC_CHK_MK_4:
		bOn = myBtn[10].GetCheck();
		if (!bOn)
			myBtn[10].SetCheck(TRUE); // IDC_CHK_MK_4
		else
			myBtn[10].SetCheck(FALSE); // IDC_CHK_MK_4
		break;
	}

	this->MoveWindow(m_pRect, TRUE);

	return 0L;
}

void CDlgMenu06::ChkTpStop()
{
	BOOL bUse = !pDoc->WorkingInfo.LastJob.bTempPause;
	Sleep(100);

	if (bUse)
	{
		pView->MpeWrite(pView->Plc.DlgInfo.UseStopLength, 1);
		pView->ChkTempStop(TRUE);
		if (!myBtn[0].GetCheck())
		{
			myBtn[0].SetCheck(TRUE);
		}
	}
	else
	{
		pView->MpeWrite(pView->Plc.DlgInfo.UseStopLength, 0);
		pView->ChkTempStop(FALSE);

		if (myBtn[0].GetCheck())
		{
			myBtn[0].SetCheck(FALSE);
		}
	}

	pDoc->WorkingInfo.LastJob.bTempPause = bUse;
	if (pDoc->m_pReelMapInner)
		pDoc->m_pReelMapInner->m_bUseTempPause = bUse;

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetTempPause();	//_stSigInx::_TempPause
#endif

	CString sData = bUse ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), sData, PATH_WORKING_INFO);
	this->MoveWindow(m_pRect, TRUE);
}


void CDlgMenu06::OnStnClickedStcLotSrl()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	myStcData[14].SetBkColor(RGB_RED);
	myStcData[14].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_LOT_SRL)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_LOT_SRL, pt, TO_BOTTOM | TO_RIGHT);

	myStcData[14].SetBkColor(RGB_WHITE);
	myStcData[14].RedrawWindow();

	CString sVal;
	GetDlgItem(IDC_STC_LOT_SRL)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.nVerifyPeriod = _ttoi(sVal);

	::WritePrivateProfileString(_T("Last Job"), _T("Verify Period"), sVal, PATH_WORKING_INFO);
}
