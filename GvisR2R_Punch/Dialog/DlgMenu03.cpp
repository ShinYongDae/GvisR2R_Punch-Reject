// DlgMenu03.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgMenu03.h"

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
// CDlgMenu03 dialog


CDlgMenu03::CDlgMenu03(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMenu03::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMenu03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRect = NULL;
	m_bLoadImg = FALSE;
	m_bTIM_MENU03_DISP = FALSE;
	//m_bTIM_CHK_DONE_MK = FALSE;
	//m_bTIM_CHK_DONE_AOI = FALSE;
	//m_bTIM_CHK_DONE_MKAOI = FALSE;
	//m_bTIM_CHK_DONE_BUF_HOME = FALSE;
	//m_bTIM_CHK_DONE_BUF_INIT = FALSE;
	m_bTIM_CHK_DONE_READY = FALSE;
	m_bTIM_SET_RC_RELATION = FALSE;
	m_bTIM_SW_RC_RELATION = FALSE;
	m_bTIM_SW_UC_RELATION = FALSE;

	m_nFdSpd = 2; // Normal
	for(int i=0; i<MAX_AXIS; i++)
	{
		m_dPrevEnc[i] = 0.0;
		m_nDir[i] = 0; // -1 : CCW , 0 : NO Dir , 1 : CW
	}

	m_bBufInitPosMove = FALSE;
	m_bVMoveMkFdVac = FALSE;
	m_bVMoveAoiFdVac = FALSE;

	//m_bTIM_CHK_DONE_ENGRAVE = FALSE;

	//m_bTIM_CHK_DONE_ENG_BUF_HOME = FALSE;
	//m_bTIM_CHK_DONE_ENG_BUF_INIT = FALSE;
}

CDlgMenu03::~CDlgMenu03()
{
	m_bTIM_MENU03_DISP = FALSE;
	//m_bTIM_CHK_DONE_MK = FALSE;
	//m_bTIM_CHK_DONE_AOI = FALSE;
	//m_bTIM_CHK_DONE_MKAOI = FALSE;
	//m_bTIM_CHK_DONE_BUF_HOME = FALSE;
	//m_bTIM_CHK_DONE_BUF_INIT = FALSE;
	m_bTIM_CHK_DONE_READY = FALSE;
	m_bTIM_SET_RC_RELATION = FALSE;
	m_bTIM_SW_RC_RELATION = FALSE;
	m_bTIM_SW_UC_RELATION = FALSE;

	//m_bTIM_CHK_DONE_ENGRAVE = FALSE;

	//m_bTIM_CHK_DONE_ENG_BUF_HOME = FALSE;
	//m_bTIM_CHK_DONE_ENG_BUF_INIT = FALSE;

	DelImg();
	if(m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgMenu03::Create()
{
	return CDialog::Create(CDlgMenu03::IDD);
}

void CDlgMenu03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMenu03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMenu03, CDialog)
	//{{AFX_MSG_MAP(CDlgMenu03)
	ON_BN_CLICKED(IDC_CHK_24, OnChk24)
	ON_BN_CLICKED(IDC_CHK_62, OnChk62)
	ON_BN_CLICKED(IDC_CHK_16, OnChk16)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MYBTN_DOWN, OnMyBtnDown)
	ON_MESSAGE(WM_MYBTN_UP, OnMyBtnUp)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CHK_79, &CDlgMenu03::OnBnClickedChk79)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMenu03 message handlers

void CDlgMenu03::OnShowWindow(BOOL bShow, UINT nStatus) 
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
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}	
}

void CDlgMenu03::AtDlgShow()
{
	LoadImg();
	SetTimer(TIM_CHK_MREG, 300, NULL);

	UpdateSignal();
}

void CDlgMenu03::AtDlgHide()
{
	DelImg();
}

void CDlgMenu03::LoadImg()
{
	if (m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for (i = 0; i < MAX_MENU03_LABEL; i++)
	{
		myLabel[i].LoadImage(ICO_LED_GRY_DlgFrameHigh, LBL_IMG_UP, CSize(20, 20), LBL_POS_CENTER);
		myLabel[i].LoadImage(ICO_LED_BLU_DlgFrameHigh, LBL_IMG_DN, CSize(20, 20), LBL_POS_CENTER);
	}

	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		switch (i)
		{
		case 0:		// Run-Main
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			//myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 33:	// Stop-Main
		case 2:		// 340mm-레이저
		case 3:		// 346mm-레이저
		case 83:	// 380mm-레이저
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			//myBtn[i].LoadBkImage(IMG_CIR_RED_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;

		case 34:	// 운전준비-Main
		case 6:		// 제품샤프트 지지대-Recoiler
		case 27:	// 제품샤프트 지지대-Uncoiler
		case 51:	// 테이블 진공-Punching
		case 52:	// 테이블 블로어-Punching
		case 62:	// 테이블 진공-AOI하
		case 16:	// 테이블 실린더-Punching
		case 63:	// 테이블 블로어-AOI하
		case 64:	// 테이블 실린더-AOI하
		case 53:	// 테이블 진공-AOI상
		case 54:	// 테이블 블로어-AOI상
		case 24:	// 테이블 실린더-AOI상	
		case 81:	// 테이블 실린터-Engraving
		case 79:	// 테이블 진공-Engraving
		case 80:	// 테이블 블로어-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			//myBtn[i].LoadBkImage(IMG_CIR_BLUE_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_BLUE_ON_DlgMenu03, BTN_IMG_DN);
			break;

		case 1:		// Reset-Main
		//case 12:	// 피딩 HOME-Punching
		case 13:	// 피딩 진공-Punching
		//case 20:	// 피딩 HOME-AOI상
		case 21:	// 피딩 진공-AOI상
		//case 58:	// 피딩 HOME-AOI하
		case 59:	// 피딩 진공-AOI하
		//case 75:	// 피딩 HOME-Engraving
		case 76:	// 피딩 진공-Engraving
		case 48:	// 마킹시작-Punching
		case 98:	// 검사시작-AOI하
		case 104:	// 검사시작-AOI상
		case 111:	// 정렬시작-Engraving
		case 112:	// 각인시작-Engraving
		case 113:	// 리딩시작-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			//myBtn[i].LoadBkImage(IMG_CIR_YLO_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_YLO_ON_DlgMenu03, BTN_IMG_DN);
			break;
		case 47:	// 연동선택-운전모드
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			//myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRY_ON_DlgMenu03, BTN_IMG_DN);
			break;

		case 9:		// 정회전-Punching
		case 94:	// 역회전-Punching
		case 55:	// 정회전-AOI하
		case 96:	// 역회전-AOI하
		case 17:	// 정회전-AOI상
		case 101:	// 역회전-AOI상
		case 72:	// 정회전-Engraving
		case 84:	// 역회전-Engraving

		case 10:	// 텐션 정방향-Punching
		case 11:	// 텐션 역방향-Punching
		case 56:	// 텐션 정방향-AOI하
		case 57:	// 텐션 역방향-AOI하
		case 18:	// 텐션 정방향-AOI상
		case 19:	// 텐션 역방향-AOI상
		case 73:	// 텐션 정방향-Engraving
		case 74:	// 텐션 역방향-Engraving

		case 32:	// 제품인버터 정방향-Uncoiler
		case 35:	// 제품인버터 역방향-Uncoiler
		case 36:	// 간지인버터 정방향-Uncoiler
		case 31:	// 간지인버터 역방향-Uncoiler
		case 8:		// 제품인버터 정방향-Recoiler
		case 44:	// 제품인버터 역방향-Recoiler
		case 45:	// 간지인버터 정방향-Recoiler
		case 46:	// 간지인버터 역방향-Recoiler

		case 70:	// 전체 상승/하강-댄서롤/버퍼롤
		case 71:	// 전체 고정 On/Off-댄서롤/버퍼롤

		//case 39:	// 댄서롤 고정-Uncoiler
		//case 38:	// 댄서롤 상승-Uncoiler
		//case 66:	// 댄서롤 상승-Recoiler
		//case 67:	// 댄서롤 고정-Recoiler

		case 99:	// 버퍼롤러 상승-AOI하
		case 100:	// 버퍼롤러 고정-AOI하
		//case 109:	// 버퍼롤 상승-Engraving
		//case 110:	// 버퍼롤 고정-Engraving

		case 65:	// 클린롤러 상부-AOI하
		case 86:	// 클린롤러 하부-AOI하
		case 97:	// 클린롤러 누름-AOI하
		//case 50:	// 클린롤러 상부-AOI상
		case 102:	// 클린롤러 하부-AOI상
		case 103:	// 클린롤러 누름-AOI상

			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			//myBtn[i].LoadBkImage(IMG_CIR_GRN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_GRN_ON_DlgMenu03, BTN_IMG_DN);
			break;

		case 26:	// 이음매 클램프-Uncoiler
		case 4:		// 제품척 클램프-Recoiler
		case 88:	// 간지척 클램프-Recoiler
		case 5:		// 이음매 클램프-Recoiler
		case 14:	// 피딩 클램프-Punching
		case 15:	// 텐션 클램프-Punching
		//case 95:	// 테이블 클램프 전진-Punching
		case 49:	// 테이블 클램프 하강-Punching
		case 60:	// 피딩 클램프-AOI하
		case 61:	// 텐션 클램프-AOI하
		case 22:	// 피딩 클램프-AOI상
		case 23:	// 텐션 클램프-AOI상
		case 77:	// 피딩 클램프-Engraving
		case 78:	// 텐션 클램프-Engraving
		case 25:	// 제품척 클램프-Uncoiler
		case 40:	// 간지척 클램프-Uncoiler
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			//myBtn[i].LoadBkImage(IMG_CIR_DGN_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_DGN_ON_DlgMenu03, BTN_IMG_DN);
			break;

		case 41:	// EPC HOME-Recoiler
		case 42:	// EPC #1-Recoiler
		case 43:	// EPC #2-Recoiler
		case 7:		// EPC #3-Recoiler
		case 89:	// EPC 자동-Recoiler
		case 90:	// EPC 수동-Recoiler
		case 91:	// EPC In-Recoiler
		case 92:	// EPC Out-Recoiler
		case 93:	// EPC 원점-Recoiler
		case 28:	// EPC HOME-Uncoiler
		case 29:	// EPC #1-Uncoiler
		case 30:	// EPC #2-Uncoiler
		case 37:	// EPC #3-Uncoiler
		case 114:	// EPC 자동-Uncoiler
		case 115:	// EPC 수동-Uncoiler
		case 116:	// EPC In-Uncoiler
		case 117:	// EPC Out-Uncoiler
		case 118:	// EPC 원점-Uncoiler

		case 105:	// 초음파 세정기 하강-AOI상
		case 106:	// 초음파 세정기 운전-AOI상
		case 107:	// 초음파 세정기 속도-AOI상
		case 82:	// 초음파 세정기 하강-Engraving
		case 85:	// 초음파 세정기 운전-Engrave
		case 108:	// 초음파 세정기 속도-Engraving
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			//myBtn[i].LoadBkImage(IMG_CIR_PNK_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_PNK_ON_DlgMenu03, BTN_IMG_DN);
			break;

		case 87:	// 한판넬이송-운전모드
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			//myBtn[i].LoadBkImage(IMG_CIR_ORG_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;

		case 68:	// Recoiler-정방향-회전방향
		case 69:	// Uncoiler-정방향-회전방향	
			myBtn[i].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, BTN_IMG_UP);
			//myBtn[i].LoadBkImage(IMG_CIR_BLUE_OFF_DlgMenu03, BTN_IMG_UP);
			myBtn[i].LoadBkImage(IMG_CIR_RED_ON_DlgMenu03, BTN_IMG_DN);
			break;
		}
	}

	myStcTitle[33].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, LBL_IMG_UP);
	//myStcTitle[33].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[33].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
	myStcTitle[35].LoadBkImage(IMG_CIR_GRY_OFF_DlgMenu03, LBL_IMG_UP);
	//myStcTitle[35].LoadBkImage(IMG_CIR_BLU_OFF_DlgMenu03, LBL_IMG_UP);
	myStcTitle[35].LoadBkImage(IMG_CIR_BLU_ON_DlgMenu03, LBL_IMG_DN);
}

void CDlgMenu03::DelImg()
{
	if (!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;

	for (i = 0; i < MAX_MENU03_LABEL; i++)
		myLabel[i].DelImgList();

	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		myBtn[i].DelImgList();
	}

	myStcTitle[33].DelImgList();
	myStcTitle[35].DelImgList();
}

BOOL CDlgMenu03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStatic();
	InitBtn();
	InitGroup();
	InitLabel();

	//GetDlgItem(IDC_STC_12_55)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_66)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_STC_12_56)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_67)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_STC_12_8)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_12)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_STC_000_15)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_118)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_STC_12_48)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_58)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_STC_12_52)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_63)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_50)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_20)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_STC_12_60)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_75)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_STC_12_74)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_98)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_STC_12_75)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_99)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_STC_12_21)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_29)->ShowWindow(SW_HIDE);

	//GetDlgItem(IDC_STC_12_27)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_36)->ShowWindow(SW_HIDE);

#ifndef TEST_MODE

	if (pDoc->m_pMpeSignal)
	{
		if (pDoc->m_pMpeSignal[0] & (0x01 << 3))	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
		{
			m_bTIM_CHK_DONE_READY = FALSE;
			pView->m_bReadyDone = TRUE; pDoc->SetStatus(_T("General"), _T("bReadyDone"), pView->m_bReadyDone);
			//pView->MpeWrite(pView->Plc.DlgMenu01.PlcReadyDone, 0);	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)MB440100
		}
	}

#endif

	m_bTIM_MENU03_DISP = TRUE;
	SetTimer(TIM_MENU03_DISP, 500, NULL);	// Disp();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgMenu03::InitGroup()
{
	myGrp[0].SubclassDlgItem(IDC_GRP_00, this);
	myGrp[0].SetHwnd(this->GetSafeHwnd(), IDC_GRP_00);	// Main

	myGrp[1].SubclassDlgItem(IDC_GRP_1, this);
	myGrp[1].SetHwnd(this->GetSafeHwnd(), IDC_GRP_1);	// Recoiler

	myGrp[2].SubclassDlgItem(IDC_GRP_02, this);
	myGrp[2].SetHwnd(this->GetSafeHwnd(), IDC_GRP_02);	// Punching

	myGrp[3].SubclassDlgItem(IDC_GRP_4, this);
	myGrp[3].SetHwnd(this->GetSafeHwnd(), IDC_GRP_4);	// AOI하

	myGrp[4].SubclassDlgItem(IDC_GRP_03, this);
	myGrp[4].SetHwnd(this->GetSafeHwnd(), IDC_GRP_03);	// AOI상

	myGrp[5].SubclassDlgItem(IDC_GRP_04, this);
	myGrp[5].SetHwnd(this->GetSafeHwnd(), IDC_GRP_04);	// Uncoiler

	myGrp[6].SubclassDlgItem(IDC_GRP_05, this);
	myGrp[6].SetHwnd(this->GetSafeHwnd(), IDC_GRP_05);	// TorqueMotor

	myGrp[7].SubclassDlgItem(IDC_GRP_16, this);
	myGrp[7].SetHwnd(this->GetSafeHwnd(), IDC_GRP_16);	// InductionMotor

	myGrp[8].SubclassDlgItem(IDC_GRP_17, this);
	myGrp[8].SetHwnd(this->GetSafeHwnd(), IDC_GRP_17);	// Core150mm

	myGrp[9].SubclassDlgItem(IDC_GRP_3, this);
	myGrp[9].SetHwnd(this->GetSafeHwnd(), IDC_GRP_3);	// Engraving

	myGrp[10].SubclassDlgItem(IDC_GRP_18, this);
	myGrp[10].SetHwnd(this->GetSafeHwnd(), IDC_GRP_18);	// 운전모드

	for (int i = 0; i < MAX_MENU03_GRP; i++)
	{
		myGrp[i].SetFontName(_T("Arial"));
		myGrp[i].SetFontSize(14);
		myGrp[i].SetFontBold(TRUE);
		myGrp[i].SetTextColor(RGB_NAVY);
		myGrp[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgMenu03::InitBtn()
{
	// Main
	myBtn[0].SubclassDlgItem(IDC_CHK_0, this);			// Run
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_0);
	myBtn[0].SetBoarder(FALSE);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[34].SubclassDlgItem(IDC_CHK_34, this);		// 운전준비
	myBtn[34].SetHwnd(this->GetSafeHwnd(), IDC_CHK_34);
	myBtn[34].SetBoarder(FALSE);
	myBtn[34].SetBtnType(BTN_TYPE_CHECK);

	myBtn[33].SubclassDlgItem(IDC_CHK_33, this);		// Stop
	myBtn[33].SetHwnd(this->GetSafeHwnd(), IDC_CHK_33);
	myBtn[33].SetBoarder(FALSE);
	myBtn[33].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_CHK_1, this);			// Reset
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1);
	myBtn[1].SetBoarder(FALSE);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	// 운전모드
	myBtn[47].SubclassDlgItem(IDC_CHK_95, this);		// 연동선택
	myBtn[47].SetHwnd(this->GetSafeHwnd(), IDC_CHK_95);
	myBtn[47].SetBoarder(FALSE);
	myBtn[47].SetBtnType(BTN_TYPE_CHECK);

	myBtn[87].SubclassDlgItem(IDC_CHK_96, this);		// 한판넬이송
	myBtn[87].SetHwnd(this->GetSafeHwnd(), IDC_CHK_96);
	myBtn[87].SetBoarder(FALSE);
	myBtn[87].SetBtnType(BTN_TYPE_CHECK);

	// 레이저
	myBtn[2].SubclassDlgItem(IDC_CHK_2, this);			// 340mm
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_2);
	myBtn[2].SetBoarder(FALSE);
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	myBtn[3].SubclassDlgItem(IDC_CHK_3, this);			// 346mm
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_3);
	myBtn[3].SetBoarder(FALSE);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	myBtn[83].SubclassDlgItem(IDC_CHK_84, this);		// 380mm
	myBtn[83].SetHwnd(this->GetSafeHwnd(), IDC_CHK_84);
	myBtn[83].SetBoarder(FALSE);
	myBtn[83].SetBtnType(BTN_TYPE_CHECK);


	// 회전방향
	myBtn[68].SubclassDlgItem(IDC_CHK_68, this);		// Recoiler-정방향
	myBtn[68].SetHwnd(this->GetSafeHwnd(), IDC_CHK_68);
	myBtn[68].SetBoarder(FALSE);
	myBtn[68].SetBtnType(BTN_TYPE_CHECK);

	myBtn[69].SubclassDlgItem(IDC_CHK_69, this);		// Uncoiler-정방향
	myBtn[69].SetHwnd(this->GetSafeHwnd(), IDC_CHK_69);
	myBtn[69].SetBoarder(FALSE);
	myBtn[69].SetBtnType(BTN_TYPE_CHECK);

	// 댄서롤/버퍼롤
	myBtn[70].SubclassDlgItem(IDC_CHK_70, this);		// 전체 상승/하강
	myBtn[70].SetHwnd(this->GetSafeHwnd(), IDC_CHK_70);
	myBtn[70].SetBoarder(FALSE);
	myBtn[70].SetBtnType(BTN_TYPE_CHECK);

	myBtn[71].SubclassDlgItem(IDC_CHK_71, this);		// 전체 고정 On/Off
	myBtn[71].SetHwnd(this->GetSafeHwnd(), IDC_CHK_71);
	myBtn[71].SetBoarder(FALSE);
	myBtn[71].SetBtnType(BTN_TYPE_CHECK);

	// Recoiler
	myBtn[4].SubclassDlgItem(IDC_CHK_4, this);			// 제품척 클램프
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_4);
	myBtn[4].SetBoarder(FALSE);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[88].SubclassDlgItem(IDC_CHK_120, this);		// 간지척 클램프
	myBtn[88].SetHwnd(this->GetSafeHwnd(), IDC_CHK_120);
	myBtn[88].SetBoarder(FALSE);
	myBtn[88].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_CHK_5, this);			// 이음매 클램프
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_5);
	myBtn[5].SetBoarder(FALSE);
	myBtn[5].SetBtnType(BTN_TYPE_CHECK);

	myBtn[6].SubclassDlgItem(IDC_CHK_6, this);			// 제품샤프트 지지대
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_6);
	myBtn[6].SetBoarder(FALSE);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);

	myBtn[41].SubclassDlgItem(IDC_CHK_41, this);		// EPC HOME
	myBtn[41].SetHwnd(this->GetSafeHwnd(), IDC_CHK_41);
	myBtn[41].SetBoarder(FALSE);
	myBtn[41].SetBtnType(BTN_TYPE_CHECK);

	myBtn[42].SubclassDlgItem(IDC_CHK_42, this);		// EPC #1
	myBtn[42].SetHwnd(this->GetSafeHwnd(), IDC_CHK_42);
	myBtn[42].SetBoarder(FALSE);
	myBtn[42].SetBtnType(BTN_TYPE_CHECK);

	myBtn[43].SubclassDlgItem(IDC_CHK_43, this);		// EPC #2
	myBtn[43].SetHwnd(this->GetSafeHwnd(), IDC_CHK_43);
	myBtn[43].SetBoarder(FALSE);
	myBtn[43].SetBtnType(BTN_TYPE_CHECK);

	myBtn[7].SubclassDlgItem(IDC_CHK_7, this);			// EPC #3
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_7);
	myBtn[7].SetBoarder(FALSE);
	myBtn[7].SetBtnType(BTN_TYPE_CHECK);

	myBtn[8].SubclassDlgItem(IDC_CHK_8, this);			// 제품인버터 정방향
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_8);
	myBtn[8].SetBoarder(FALSE);
	myBtn[8].SetBtnType(BTN_TYPE_CHECK);

	myBtn[44].SubclassDlgItem(IDC_CHK_44, this);		// 제품인버터 역방향
	myBtn[44].SetHwnd(this->GetSafeHwnd(), IDC_CHK_44);
	myBtn[44].SetBoarder(FALSE);
	myBtn[44].SetBtnType(BTN_TYPE_CHECK);

	myBtn[45].SubclassDlgItem(IDC_CHK_45, this);		// 간지인버터 정방향
	myBtn[45].SetHwnd(this->GetSafeHwnd(), IDC_CHK_45);
	myBtn[45].SetBoarder(FALSE);
	myBtn[45].SetBtnType(BTN_TYPE_CHECK);

	myBtn[46].SubclassDlgItem(IDC_CHK_46, this);		// 간지인버터 역방향
	myBtn[46].SetHwnd(this->GetSafeHwnd(), IDC_CHK_46);
	myBtn[46].SetBoarder(FALSE);
	myBtn[46].SetBtnType(BTN_TYPE_DEFAULT);

	//myBtn[66].SubclassDlgItem(IDC_CHK_66, this);		// 댄서롤 상승
	//myBtn[66].SetHwnd(this->GetSafeHwnd(), IDC_CHK_66);
	//myBtn[66].SetBoarder(FALSE);
	//myBtn[66].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[67].SubclassDlgItem(IDC_CHK_67, this);		// 댄서롤 고정
	//myBtn[67].SetHwnd(this->GetSafeHwnd(), IDC_CHK_67);
	//myBtn[67].SetBoarder(FALSE);
	//myBtn[67].SetBtnType(BTN_TYPE_CHECK);

	myBtn[89].SubclassDlgItem(IDC_CHK_121, this);		// EPC 자동
	myBtn[89].SetHwnd(this->GetSafeHwnd(), IDC_CHK_121);
	myBtn[89].SetBoarder(FALSE);
	myBtn[89].SetBtnType(BTN_TYPE_CHECK);

	myBtn[90].SubclassDlgItem(IDC_CHK_122, this);		// EPC 수동
	myBtn[90].SetHwnd(this->GetSafeHwnd(), IDC_CHK_122);
	myBtn[90].SetBoarder(FALSE);
	myBtn[90].SetBtnType(BTN_TYPE_CHECK);

	myBtn[91].SubclassDlgItem(IDC_CHK_123, this);		// EPC In
	myBtn[91].SetHwnd(this->GetSafeHwnd(), IDC_CHK_123);
	myBtn[91].SetBoarder(FALSE);
	myBtn[91].SetBtnType(BTN_TYPE_CHECK);

	myBtn[92].SubclassDlgItem(IDC_CHK_124, this);		// EPC Out
	myBtn[92].SetHwnd(this->GetSafeHwnd(), IDC_CHK_124);
	myBtn[92].SetBoarder(FALSE);
	myBtn[92].SetBtnType(BTN_TYPE_CHECK);

	myBtn[93].SubclassDlgItem(IDC_CHK_125, this);		// EPC 원점
	myBtn[93].SetHwnd(this->GetSafeHwnd(), IDC_CHK_125);
	myBtn[93].SetBoarder(FALSE);
	myBtn[93].SetBtnType(BTN_TYPE_CHECK);


	// Punching
	myBtn[9].SubclassDlgItem(IDC_CHK_9, this);			// ◀정회전
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_9);
	myBtn[9].SetBoarder(FALSE);
	myBtn[9].SetBtnType(BTN_TYPE_CHECK);

	myBtn[94].SubclassDlgItem(IDC_CHK_116, this);		// 역회전▶
	myBtn[94].SetHwnd(this->GetSafeHwnd(), IDC_CHK_116);
	myBtn[94].SetBoarder(FALSE);
	myBtn[94].SetBtnType(BTN_TYPE_CHECK);

	myBtn[10].SubclassDlgItem(IDC_CHK_10, this);		// 텐션 정방향
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_10);
	myBtn[10].SetBoarder(FALSE);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	myBtn[11].SubclassDlgItem(IDC_CHK_11, this);		// 텐션 역방향
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_11);
	myBtn[11].SetBoarder(FALSE);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[12].SubclassDlgItem(IDC_CHK_12, this);		// 피딩 HOME
	//myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_12);
	//myBtn[12].SetBoarder(FALSE);
	//myBtn[12].SetBtnType(BTN_TYPE_CHECK);

	myBtn[13].SubclassDlgItem(IDC_CHK_13, this);		// 피딩 진공
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_13);
	myBtn[13].SetBoarder(FALSE);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);

	myBtn[14].SubclassDlgItem(IDC_CHK_14, this);		// 피딩 클램프
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_14);
	myBtn[14].SetBoarder(FALSE);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);

	myBtn[15].SubclassDlgItem(IDC_CHK_15, this);		// 텐션 클램프
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_CHK_15);
	myBtn[15].SetBoarder(FALSE);
	myBtn[15].SetBtnType(BTN_TYPE_CHECK);

	myBtn[51].SubclassDlgItem(IDC_CHK_51, this);		// 테이블 진공
	myBtn[51].SetHwnd(this->GetSafeHwnd(), IDC_CHK_51);
	myBtn[51].SetBoarder(FALSE);
	myBtn[51].SetBtnType(BTN_TYPE_CHECK);

	myBtn[52].SubclassDlgItem(IDC_CHK_52, this);		// 테이블 블로어
	myBtn[52].SetHwnd(this->GetSafeHwnd(), IDC_CHK_52);
	myBtn[52].SetBoarder(FALSE);
	myBtn[52].SetBtnType(BTN_TYPE_CHECK);

	myBtn[16].SubclassDlgItem(IDC_CHK_16, this);		// 테이블 실린더
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_16);
	myBtn[16].SetBoarder(FALSE);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);

	myBtn[49].SubclassDlgItem(IDC_CHK_49, this);		// 테이블 클램프 하강
	myBtn[49].SetHwnd(this->GetSafeHwnd(), IDC_CHK_49);
	myBtn[49].SetBoarder(FALSE);
	myBtn[49].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[95].SubclassDlgItem(IDC_CHK_118, this);		// 테이블 클램프 전진
	//myBtn[95].SetHwnd(this->GetSafeHwnd(), IDC_CHK_118);
	//myBtn[95].SetBoarder(FALSE);
	//myBtn[95].SetBtnType(BTN_TYPE_CHECK);

	myBtn[48].SubclassDlgItem(IDC_CHK_48, this);		// 마킹 시작
	myBtn[48].SetHwnd(this->GetSafeHwnd(), IDC_CHK_48);
	myBtn[48].SetBoarder(FALSE);
	myBtn[48].SetBtnType(BTN_TYPE_CHECK);

	// AOI 하면.
	myBtn[55].SubclassDlgItem(IDC_CHK_55, this);		// ◀정회전
	myBtn[55].SetHwnd(this->GetSafeHwnd(), IDC_CHK_55);
	myBtn[55].SetBoarder(FALSE);
	myBtn[55].SetBtnType(BTN_TYPE_CHECK);

	myBtn[96].SubclassDlgItem(IDC_CHK_110, this);		// 역회전▶
	myBtn[96].SetHwnd(this->GetSafeHwnd(), IDC_CHK_110);
	myBtn[96].SetBoarder(FALSE);
	myBtn[96].SetBtnType(BTN_TYPE_CHECK);

	myBtn[56].SubclassDlgItem(IDC_CHK_56, this);		// 텐션 정방향
	myBtn[56].SetHwnd(this->GetSafeHwnd(), IDC_CHK_56);
	myBtn[56].SetBoarder(FALSE);
	myBtn[56].SetBtnType(BTN_TYPE_CHECK);

	myBtn[57].SubclassDlgItem(IDC_CHK_57, this);		// 텐션 역방향
	myBtn[57].SetHwnd(this->GetSafeHwnd(), IDC_CHK_57);
	myBtn[57].SetBoarder(FALSE);
	myBtn[57].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[58].SubclassDlgItem(IDC_CHK_58, this);		// 피딩 HOME
	//myBtn[58].SetHwnd(this->GetSafeHwnd(), IDC_CHK_58);
	//myBtn[58].SetBoarder(FALSE);
	//myBtn[58].SetBtnType(BTN_TYPE_CHECK);

	myBtn[59].SubclassDlgItem(IDC_CHK_59, this);		// 피딩 진공
	myBtn[59].SetHwnd(this->GetSafeHwnd(), IDC_CHK_59);
	myBtn[59].SetBoarder(FALSE);
	myBtn[59].SetBtnType(BTN_TYPE_CHECK);

	myBtn[60].SubclassDlgItem(IDC_CHK_60, this);		// 피딩 클램프
	myBtn[60].SetHwnd(this->GetSafeHwnd(), IDC_CHK_60);
	myBtn[60].SetBoarder(FALSE);
	myBtn[60].SetBtnType(BTN_TYPE_CHECK);

	myBtn[61].SubclassDlgItem(IDC_CHK_61, this);		// 텐션 클램프
	myBtn[61].SetHwnd(this->GetSafeHwnd(), IDC_CHK_61);
	myBtn[61].SetBoarder(FALSE);
	myBtn[61].SetBtnType(BTN_TYPE_CHECK);

	myBtn[62].SubclassDlgItem(IDC_CHK_64, this);		// 테이블 진공
	myBtn[62].SetHwnd(this->GetSafeHwnd(), IDC_CHK_64);
	myBtn[62].SetBoarder(FALSE);
	myBtn[62].SetBtnType(BTN_TYPE_CHECK);

	myBtn[63].SubclassDlgItem(IDC_CHK_65, this);		// 테이블 블로어
	myBtn[63].SetHwnd(this->GetSafeHwnd(), IDC_CHK_65);
	myBtn[63].SetBoarder(FALSE);
	myBtn[63].SetBtnType(BTN_TYPE_CHECK);


	myBtn[64].SubclassDlgItem(IDC_CHK_62, this);		// 테이블 실린더
	myBtn[64].SetHwnd(this->GetSafeHwnd(), IDC_CHK_62);
	myBtn[64].SetBoarder(FALSE);
	myBtn[64].SetBtnType(BTN_TYPE_CHECK);

	myBtn[65].SubclassDlgItem(IDC_CHK_63, this);		//클린롤러 상부
	myBtn[65].SetHwnd(this->GetSafeHwnd(), IDC_CHK_63);
	myBtn[65].SetBoarder(FALSE);
	myBtn[65].SetBtnType(BTN_TYPE_CHECK);

	myBtn[86].SubclassDlgItem(IDC_CHK_88, this);		// 클린롤러 하부
	myBtn[86].SetHwnd(this->GetSafeHwnd(), IDC_CHK_88);
	myBtn[86].SetBoarder(FALSE);
	myBtn[86].SetBtnType(BTN_TYPE_CHECK);

	myBtn[97].SubclassDlgItem(IDC_CHK_111, this);		// 클린롤러 누름
	myBtn[97].SetHwnd(this->GetSafeHwnd(), IDC_CHK_111);
	myBtn[97].SetBoarder(FALSE);
	myBtn[97].SetBtnType(BTN_TYPE_CHECK);

	myBtn[98].SubclassDlgItem(IDC_CHK_112, this);		// 검사시작
	myBtn[98].SetHwnd(this->GetSafeHwnd(), IDC_CHK_112);
	myBtn[98].SetBoarder(FALSE);
	myBtn[98].SetBtnType(BTN_TYPE_CHECK);

	myBtn[99].SubclassDlgItem(IDC_CHK_114, this);		// 버퍼롤러 상승
	myBtn[99].SetHwnd(this->GetSafeHwnd(), IDC_CHK_114);
	myBtn[99].SetBoarder(FALSE);
	myBtn[99].SetBtnType(BTN_TYPE_CHECK);

	myBtn[100].SubclassDlgItem(IDC_CHK_113, this);		// 버퍼롤러 고정
	myBtn[100].SetHwnd(this->GetSafeHwnd(), IDC_CHK_113);
	myBtn[100].SetBoarder(FALSE);
	myBtn[100].SetBtnType(BTN_TYPE_CHECK);

	// AOI 상면.	
	myBtn[17].SubclassDlgItem(IDC_CHK_17, this);		// ◀정회전
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_17);
	myBtn[17].SetBoarder(FALSE);
	myBtn[17].SetBtnType(BTN_TYPE_CHECK);

	myBtn[101].SubclassDlgItem(IDC_CHK_103, this);		// 역회전▶
	myBtn[101].SetHwnd(this->GetSafeHwnd(), IDC_CHK_103);
	myBtn[101].SetBoarder(FALSE);
	myBtn[101].SetBtnType(BTN_TYPE_CHECK);

	myBtn[18].SubclassDlgItem(IDC_CHK_18, this);		// 텐션 정방향
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_CHK_18);
	myBtn[18].SetBoarder(FALSE);
	myBtn[18].SetBtnType(BTN_TYPE_CHECK);

	myBtn[19].SubclassDlgItem(IDC_CHK_19, this);		// 텐션 역방향
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_19);
	myBtn[19].SetBoarder(FALSE);
	myBtn[19].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[20].SubclassDlgItem(IDC_CHK_20, this);		// 피딩 HOME
	//myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_20);
	//myBtn[20].SetBoarder(FALSE);
	//myBtn[20].SetBtnType(BTN_TYPE_CHECK);

	myBtn[21].SubclassDlgItem(IDC_CHK_21, this);		// 피딩 진공
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_21);
	myBtn[21].SetBoarder(FALSE);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);

	myBtn[22].SubclassDlgItem(IDC_CHK_22, this);		// 피딩 클램프
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_CHK_22);
	myBtn[22].SetBoarder(FALSE);
	myBtn[22].SetBtnType(BTN_TYPE_CHECK);

	myBtn[23].SubclassDlgItem(IDC_CHK_23, this);		// 텐션 클램프
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_CHK_23);
	myBtn[23].SetBoarder(FALSE);
	myBtn[23].SetBtnType(BTN_TYPE_CHECK);

	myBtn[53].SubclassDlgItem(IDC_CHK_53, this);		// 테이블 진공
	myBtn[53].SetHwnd(this->GetSafeHwnd(), IDC_CHK_53);
	myBtn[53].SetBoarder(FALSE);
	myBtn[53].SetBtnType(BTN_TYPE_CHECK);

	myBtn[54].SubclassDlgItem(IDC_CHK_54, this);		// 테이블 블로어
	myBtn[54].SetHwnd(this->GetSafeHwnd(), IDC_CHK_54);
	myBtn[54].SetBoarder(FALSE);
	myBtn[54].SetBtnType(BTN_TYPE_CHECK);

	myBtn[24].SubclassDlgItem(IDC_CHK_24, this);		// 테이블 실린더
	myBtn[24].SetHwnd(this->GetSafeHwnd(), IDC_CHK_24);
	myBtn[24].SetBoarder(FALSE);
	myBtn[24].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[50].SubclassDlgItem(IDC_CHK_50, this);		// 클린롤러 상부
	//myBtn[50].SetHwnd(this->GetSafeHwnd(), IDC_CHK_50);
	//myBtn[50].SetBoarder(FALSE);
	//myBtn[50].SetBtnType(BTN_TYPE_CHECK);

	myBtn[102].SubclassDlgItem(IDC_CHK_104, this);		// 클린롤러 하부
	myBtn[102].SetHwnd(this->GetSafeHwnd(), IDC_CHK_104);
	myBtn[102].SetBoarder(FALSE);
	myBtn[102].SetBtnType(BTN_TYPE_CHECK);

	myBtn[103].SubclassDlgItem(IDC_CHK_105, this);		// 클린롤러 누름
	myBtn[103].SetHwnd(this->GetSafeHwnd(), IDC_CHK_105);
	myBtn[103].SetBoarder(FALSE);
	myBtn[103].SetBtnType(BTN_TYPE_CHECK);

	myBtn[104].SubclassDlgItem(IDC_CHK_106, this);		// 검사시작
	myBtn[104].SetHwnd(this->GetSafeHwnd(), IDC_CHK_106);
	myBtn[104].SetBoarder(FALSE);
	myBtn[104].SetBtnType(BTN_TYPE_CHECK);

	myBtn[105].SubclassDlgItem(IDC_CHK_107, this);		// 초음파 세정기 하강
	myBtn[105].SetHwnd(this->GetSafeHwnd(), IDC_CHK_107);
	myBtn[105].SetBoarder(FALSE);
	myBtn[105].SetBtnType(BTN_TYPE_CHECK);

	myBtn[106].SubclassDlgItem(IDC_CHK_115, this);		// 초음파 세정기 운전
	myBtn[106].SetHwnd(this->GetSafeHwnd(), IDC_CHK_115);
	myBtn[106].SetBoarder(FALSE);
	myBtn[106].SetBtnType(BTN_TYPE_CHECK);

	myBtn[107].SubclassDlgItem(IDC_CHK_109, this);		// 초음파 세정기 속도 
	myBtn[107].SetHwnd(this->GetSafeHwnd(), IDC_CHK_109);
	myBtn[107].SetBoarder(FALSE);
	myBtn[107].SetBtnType(BTN_TYPE_CHECK);

	// 각인부
	myBtn[72].SubclassDlgItem(IDC_CHK_72, this);		// ◀정회전
	myBtn[72].SetHwnd(this->GetSafeHwnd(), IDC_CHK_72);
	myBtn[72].SetBoarder(FALSE);
	myBtn[72].SetBtnType(BTN_TYPE_CHECK);

	myBtn[84].SubclassDlgItem(IDC_CHK_80, this);		// 역회전▶
	myBtn[84].SetHwnd(this->GetSafeHwnd(), IDC_CHK_80);
	myBtn[84].SetBoarder(FALSE);
	myBtn[84].SetBtnType(BTN_TYPE_CHECK);

	myBtn[73].SubclassDlgItem(IDC_CHK_73, this);		// 텐션 정방향
	myBtn[73].SetHwnd(this->GetSafeHwnd(), IDC_CHK_73);
	myBtn[73].SetBoarder(FALSE);
	myBtn[73].SetBtnType(BTN_TYPE_CHECK);

	myBtn[74].SubclassDlgItem(IDC_CHK_74, this);		// 텐션 역방향
	myBtn[74].SetHwnd(this->GetSafeHwnd(), IDC_CHK_74);
	myBtn[74].SetBoarder(FALSE);
	myBtn[74].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[75].SubclassDlgItem(IDC_CHK_75, this);		// 피딩 HOME
	//myBtn[75].SetHwnd(this->GetSafeHwnd(), IDC_CHK_75);
	//myBtn[75].SetBoarder(FALSE);
	//myBtn[75].SetBtnType(BTN_TYPE_CHECK);

	myBtn[76].SubclassDlgItem(IDC_CHK_76, this);		// 피딩 진공
	myBtn[76].SetHwnd(this->GetSafeHwnd(), IDC_CHK_76);
	myBtn[76].SetBoarder(FALSE);
	myBtn[76].SetBtnType(BTN_TYPE_CHECK);

	myBtn[77].SubclassDlgItem(IDC_CHK_77, this);		// 피딩 클램프
	myBtn[77].SetHwnd(this->GetSafeHwnd(), IDC_CHK_77);
	myBtn[77].SetBoarder(FALSE);
	myBtn[77].SetBtnType(BTN_TYPE_CHECK);

	myBtn[78].SubclassDlgItem(IDC_CHK_78, this);		// 텐션 클램프
	myBtn[78].SetHwnd(this->GetSafeHwnd(), IDC_CHK_78);
	myBtn[78].SetBoarder(FALSE);
	myBtn[78].SetBtnType(BTN_TYPE_CHECK);

	myBtn[79].SubclassDlgItem(IDC_CHK_82, this);		// 테이블 진공
	myBtn[79].SetHwnd(this->GetSafeHwnd(), IDC_CHK_82);
	myBtn[79].SetBoarder(FALSE);
	myBtn[79].SetBtnType(BTN_TYPE_CHECK);

	myBtn[80].SubclassDlgItem(IDC_CHK_83, this);		// 테이블 블로어
	myBtn[80].SetHwnd(this->GetSafeHwnd(), IDC_CHK_83);
	myBtn[80].SetBoarder(FALSE);
	myBtn[80].SetBtnType(BTN_TYPE_CHECK);

	myBtn[81].SubclassDlgItem(IDC_CHK_79, this);		// 테이블 실린더
	myBtn[81].SetHwnd(this->GetSafeHwnd(), IDC_CHK_79);
	myBtn[81].SetBoarder(FALSE);
	myBtn[81].SetBtnType(BTN_TYPE_CHECK);

	myBtn[82].SubclassDlgItem(IDC_CHK_81, this);		// 초음파 세정기 하강
	myBtn[82].SetHwnd(this->GetSafeHwnd(), IDC_CHK_81);
	myBtn[82].SetBoarder(FALSE);
	myBtn[82].SetBtnType(BTN_TYPE_CHECK);

	myBtn[85].SubclassDlgItem(IDC_CHK_87, this);		// 초음파 세정기 운전 - 각인부
	myBtn[85].SetHwnd(this->GetSafeHwnd(), IDC_CHK_87);
	myBtn[85].SetBoarder(FALSE);
	myBtn[85].SetBtnType(BTN_TYPE_CHECK);

	myBtn[108].SubclassDlgItem(IDC_CHK_97, this);		// 초음파 세정기 속도 - 각인부
	myBtn[108].SetHwnd(this->GetSafeHwnd(), IDC_CHK_97);
	myBtn[108].SetBoarder(FALSE);
	myBtn[108].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[109].SubclassDlgItem(IDC_CHK_98, this);		// 버퍼롤러 상승 - 각인부
	//myBtn[109].SetHwnd(this->GetSafeHwnd(), IDC_CHK_98);
	//myBtn[109].SetBoarder(FALSE);
	//myBtn[109].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[110].SubclassDlgItem(IDC_CHK_99, this);		// 버퍼롤러 고정 - 각인부
	//myBtn[110].SetHwnd(this->GetSafeHwnd(), IDC_CHK_99);
	//myBtn[110].SetBoarder(FALSE);
	//myBtn[110].SetBtnType(BTN_TYPE_CHECK);

	myBtn[111].SubclassDlgItem(IDC_CHK_100, this);		// 정렬시작 - 각인부
	myBtn[111].SetHwnd(this->GetSafeHwnd(), IDC_CHK_100);
	myBtn[111].SetBoarder(FALSE);
	myBtn[111].SetBtnType(BTN_TYPE_CHECK);

	myBtn[112].SubclassDlgItem(IDC_CHK_101, this);		// 각인시작 - 각인부
	myBtn[112].SetHwnd(this->GetSafeHwnd(), IDC_CHK_101);
	myBtn[112].SetBoarder(FALSE);
	myBtn[112].SetBtnType(BTN_TYPE_CHECK);

	myBtn[113].SubclassDlgItem(IDC_CHK_102, this);		// 리딩시작 - 각인부
	myBtn[113].SetHwnd(this->GetSafeHwnd(), IDC_CHK_102);
	myBtn[113].SetBoarder(FALSE);
	myBtn[113].SetBtnType(BTN_TYPE_CHECK);

	// Uncoiler
	myBtn[25].SubclassDlgItem(IDC_CHK_25, this);		// 제품척 클램프
	myBtn[25].SetHwnd(this->GetSafeHwnd(), IDC_CHK_25);
	myBtn[25].SetBoarder(FALSE);
	myBtn[25].SetBtnType(BTN_TYPE_CHECK);

	myBtn[40].SubclassDlgItem(IDC_CHK_94, this);		// 간지척 클램프
	myBtn[40].SetHwnd(this->GetSafeHwnd(), IDC_CHK_94);
	myBtn[40].SetBoarder(FALSE);
	myBtn[40].SetBtnType(BTN_TYPE_CHECK);

	myBtn[26].SubclassDlgItem(IDC_CHK_26, this);		// 이음매 클램프
	myBtn[26].SetHwnd(this->GetSafeHwnd(), IDC_CHK_26);
	myBtn[26].SetBoarder(FALSE);
	myBtn[26].SetBtnType(BTN_TYPE_CHECK);

	myBtn[27].SubclassDlgItem(IDC_CHK_27, this);		// 제품샤프트 지지대
	myBtn[27].SetHwnd(this->GetSafeHwnd(), IDC_CHK_27);
	myBtn[27].SetBoarder(FALSE);
	myBtn[27].SetBtnType(BTN_TYPE_CHECK);

	myBtn[28].SubclassDlgItem(IDC_CHK_35, this);		// EPC HOME
	myBtn[28].SetHwnd(this->GetSafeHwnd(), IDC_CHK_35);
	myBtn[28].SetBoarder(FALSE);
	myBtn[28].SetBtnType(BTN_TYPE_CHECK);

	myBtn[29].SubclassDlgItem(IDC_CHK_28, this);		// EPC #1
	myBtn[29].SetHwnd(this->GetSafeHwnd(), IDC_CHK_28);
	myBtn[29].SetBoarder(FALSE);
	myBtn[29].SetBtnType(BTN_TYPE_CHECK);

	myBtn[30].SubclassDlgItem(IDC_CHK_30, this);		// EPC #2
	myBtn[30].SetHwnd(this->GetSafeHwnd(), IDC_CHK_30);
	myBtn[30].SetBoarder(FALSE);
	myBtn[30].SetBtnType(BTN_TYPE_CHECK);

	myBtn[37].SubclassDlgItem(IDC_CHK_37, this);		// // EPC #3
	myBtn[37].SetHwnd(this->GetSafeHwnd(), IDC_CHK_37);
	myBtn[37].SetBoarder(FALSE);
	myBtn[37].SetBtnType(BTN_TYPE_CHECK);

	myBtn[32].SubclassDlgItem(IDC_CHK_38, this);		// 제품인버터 정방향
	myBtn[32].SetHwnd(this->GetSafeHwnd(), IDC_CHK_38);
	myBtn[32].SetBoarder(FALSE);
	myBtn[32].SetBtnType(BTN_TYPE_CHECK);

	myBtn[31].SubclassDlgItem(IDC_CHK_31, this);		// 제품인버터 역방향-Uncoiler
	myBtn[31].SetHwnd(this->GetSafeHwnd(), IDC_CHK_31);
	myBtn[31].SetBoarder(FALSE);
	myBtn[31].SetBtnType(BTN_TYPE_CHECK);

	myBtn[35].SubclassDlgItem(IDC_CHK_32, this);		// 간지인버터 역방향
	myBtn[35].SetHwnd(this->GetSafeHwnd(), IDC_CHK_32);
	myBtn[35].SetBoarder(FALSE);
	myBtn[35].SetBtnType(BTN_TYPE_CHECK);

	myBtn[36].SubclassDlgItem(IDC_CHK_39, this);		// 간지인버터 정방향
	myBtn[36].SetHwnd(this->GetSafeHwnd(), IDC_CHK_39);
	myBtn[36].SetBoarder(FALSE);
	myBtn[36].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[38].SubclassDlgItem(IDC_CHK_29, this);		// 댄서롤 상승
	//myBtn[38].SetHwnd(this->GetSafeHwnd(), IDC_CHK_29);
	//myBtn[38].SetBoarder(FALSE);
	//myBtn[38].SetBtnType(BTN_TYPE_CHECK);

	//myBtn[39].SubclassDlgItem(IDC_CHK_36, this);		// 댄서롤 고정
	//myBtn[39].SetHwnd(this->GetSafeHwnd(), IDC_CHK_36);
	//myBtn[39].SetBoarder(FALSE);
	//myBtn[39].SetBtnType(BTN_TYPE_CHECK);

	myBtn[114].SubclassDlgItem(IDC_CHK_89, this);		// EPC 자동
	myBtn[114].SetHwnd(this->GetSafeHwnd(), IDC_CHK_89);
	myBtn[114].SetBoarder(FALSE);
	myBtn[114].SetBtnType(BTN_TYPE_CHECK);

	myBtn[115].SubclassDlgItem(IDC_CHK_90, this);		// EPC 수동
	myBtn[115].SetHwnd(this->GetSafeHwnd(), IDC_CHK_90);
	myBtn[115].SetBoarder(FALSE);
	myBtn[115].SetBtnType(BTN_TYPE_CHECK);

	myBtn[116].SubclassDlgItem(IDC_CHK_91, this);		// EPC In
	myBtn[116].SetHwnd(this->GetSafeHwnd(), IDC_CHK_91);
	myBtn[116].SetBoarder(FALSE);
	myBtn[116].SetBtnType(BTN_TYPE_CHECK);

	myBtn[117].SubclassDlgItem(IDC_CHK_92, this);		// EPC Out
	myBtn[117].SetHwnd(this->GetSafeHwnd(), IDC_CHK_92);
	myBtn[117].SetBoarder(FALSE);
	myBtn[117].SetBtnType(BTN_TYPE_CHECK);

	myBtn[118].SubclassDlgItem(IDC_CHK_93, this);		// EPC 원점
	myBtn[118].SetHwnd(this->GetSafeHwnd(), IDC_CHK_93);
	myBtn[118].SetBoarder(FALSE);
	myBtn[118].SetBtnType(BTN_TYPE_CHECK);

	int i;
	for (i = 0; i < MAX_MENU03_BTN; i++)
	{
		myBtn[i].SetFont(_T("굴림체"), 12, TRUE);
		myBtn[i].SetTextColor(RGB_BLACK);
	}
}

void CDlgMenu03::InitLabel()
{
	myLabel[0].SubclassDlgItem(IDC_STC_SIG1, this);
	myLabel[1].SubclassDlgItem(IDC_STC_SIG2, this);
	myLabel[2].SubclassDlgItem(IDC_STC_SIG3, this);
	myLabel[3].SubclassDlgItem(IDC_STC_SIG4, this);
	myLabel[4].SubclassDlgItem(IDC_STC_SIG5, this);

	for (int i = 0; i < MAX_MENU03_LABEL; i++)
	{
		myLabel[i].SetFontName(_T("Arial"));
		myLabel[i].SetFontSize(18);
		myLabel[i].SetFontBold(TRUE);
		myLabel[i].SetTextColor(RGB_DARKRED);
		myLabel[i].SetImageBk(LBL_IMG_UP);
	}
}

void CDlgMenu03::InitStatic()
{
	// Main
	myStcTitle[0].SubclassDlgItem(IDC_STC_000_, this);		// Run
	myStcTitle[1].SubclassDlgItem(IDC_STC_01_, this);		// Reset
	myStcTitle[39].SubclassDlgItem(IDC_STC_000_3, this);	// Stop
	myStcTitle[40].SubclassDlgItem(IDC_STC_000_4, this);	// 운전준비

	myStcTitle[33].SubclassDlgItem(IDC_STC_02, this);		// LED-자동 
	myStcTitle[35].SubclassDlgItem(IDC_STC_04, this);		// LED-수동 

	myStcTitle[36].SubclassDlgItem(IDC_STC_02_, this);		// 자동
	myStcTitle[38].SubclassDlgItem(IDC_STC_04_, this);		// 수동

															// 운전모드
	myStcTitle[34].SubclassDlgItem(IDC_STC_000_12, this);	// 연동선택
	myStcTitle[37].SubclassDlgItem(IDC_STC_01_5, this);		// 한판넬이송

															// 레이저
	myStcTitle[2].SubclassDlgItem(IDC_STC_000_2, this);		// 340mm
	myStcTitle[3].SubclassDlgItem(IDC_STC_01_2, this);		// 346mm
	myStcTitle[78].SubclassDlgItem(IDC_STC_000_9, this);	// 380mm

															// 회전방향
	myStcTitle[74].SubclassDlgItem(IDC_STC_000_6, this);	// Recoiler역방향
	myStcTitle[75].SubclassDlgItem(IDC_STC_01_3, this);		// Uncoiler역방향

															// 댄서롤/버퍼롤
	myStcTitle[76].SubclassDlgItem(IDC_STC_000_7, this);	// 전체 상승/하강
	myStcTitle[77].SubclassDlgItem(IDC_STC_01_4, this);		// 전체 고정 On/Off

															// Recoiler
	myStcTitle[4].SubclassDlgItem(IDC_STC_10_, this);		// 제품척 클램프
	myStcTitle[46].SubclassDlgItem(IDC_STC_12_92, this);	// 간지척 클램프
	myStcTitle[5].SubclassDlgItem(IDC_STC_12_2, this);		// 이음매 클램프
	myStcTitle[6].SubclassDlgItem(IDC_STC_12_, this);		// 제품샤프트 지지대
	myStcTitle[7].SubclassDlgItem(IDC_STC_12_4, this);		// EPC HOME
	myStcTitle[8].SubclassDlgItem(IDC_STC_12_3, this);		// EPC #1
	myStcTitle[47].SubclassDlgItem(IDC_STC_12_32, this);	// EPC #2
	myStcTitle[48].SubclassDlgItem(IDC_STC_12_33, this);	// EPC #3
	myStcTitle[49].SubclassDlgItem(IDC_STC_12_34, this);	// 제품인버터 정방향
	myStcTitle[50].SubclassDlgItem(IDC_STC_12_36, this);	// 제품인버터 역방향
	myStcTitle[51].SubclassDlgItem(IDC_STC_12_35, this);	// 간지인버터 정방향
	myStcTitle[52].SubclassDlgItem(IDC_STC_12_37, this);	// 간지인버터 역방향
	//myStcTitle[72].SubclassDlgItem(IDC_STC_12_55, this);	// 댄서롤 상승
	//myStcTitle[73].SubclassDlgItem(IDC_STC_12_56, this);	// 댄서롤 고정
	myStcTitle[53].SubclassDlgItem(IDC_STC_12_93, this);	// EPC 자동
	myStcTitle[93].SubclassDlgItem(IDC_STC_12_94, this);	// EPC 수동
	myStcTitle[94].SubclassDlgItem(IDC_STC_12_95, this);	// EPC In
	myStcTitle[95].SubclassDlgItem(IDC_STC_12_96, this);	// EPC Out
	myStcTitle[96].SubclassDlgItem(IDC_STC_12_97, this);	// EPC 원점

															// Punching
	myStcTitle[9].SubclassDlgItem(IDC_STC_20_, this);		// 피딩 정회전
	myStcTitle[97].SubclassDlgItem(IDC_STC_12_89, this);	// 피딩 역회전
	myStcTitle[10].SubclassDlgItem(IDC_STC_12_6, this);		// 텐션 정방향
	myStcTitle[11].SubclassDlgItem(IDC_STC_12_5, this);		// 텐션 역방향
	//myStcTitle[12].SubclassDlgItem(IDC_STC_12_8, this);		// 피딩 HOME
	myStcTitle[13].SubclassDlgItem(IDC_STC_12_7, this);		// 피딩 진공
	myStcTitle[14].SubclassDlgItem(IDC_STC_12_10, this);	// 피딩 클램프
	myStcTitle[15].SubclassDlgItem(IDC_STC_12_9, this);		// 텐션 클램프
	myStcTitle[57].SubclassDlgItem(IDC_STC_12_42, this);	// 테이블 진공
	myStcTitle[58].SubclassDlgItem(IDC_STC_12_41, this);	// 테이블 블로어
	myStcTitle[16].SubclassDlgItem(IDC_STC_12_12, this);	// 테이블 실린더
	myStcTitle[55].SubclassDlgItem(IDC_STC_12_39, this);	// 테이블 클램프 하강
	//myStcTitle[98].SubclassDlgItem(IDC_STC_000_15, this);	// 테이블 클램프 전진
	myStcTitle[54].SubclassDlgItem(IDC_STC_000_5, this);	// 마킹 시작

															// AOI하
	myStcTitle[61].SubclassDlgItem(IDC_STC_30_2, this);		// 피딩 정회전
	myStcTitle[99].SubclassDlgItem(IDC_STC_12_85, this);	// 피딩 역회전
	myStcTitle[62].SubclassDlgItem(IDC_STC_12_46, this);	// 텐션 정방향
	myStcTitle[63].SubclassDlgItem(IDC_STC_12_45, this);	// 텐션 역방향
	//myStcTitle[64].SubclassDlgItem(IDC_STC_12_48, this);	// 피딩 HOME
	myStcTitle[65].SubclassDlgItem(IDC_STC_12_47, this);	// 피딩 진공
	myStcTitle[66].SubclassDlgItem(IDC_STC_12_50, this);	// 피딩 클램프
	myStcTitle[67].SubclassDlgItem(IDC_STC_12_49, this);	// 텐션 클램프
	myStcTitle[68].SubclassDlgItem(IDC_STC_12_54, this);	// 테이블 진공
	myStcTitle[69].SubclassDlgItem(IDC_STC_12_53, this);	// 테이블 블로어
	myStcTitle[70].SubclassDlgItem(IDC_STC_12_51, this);	// 테이블 실린더
	myStcTitle[71].SubclassDlgItem(IDC_STC_12_52, this);	// 클린롤러 상부
	myStcTitle[92].SubclassDlgItem(IDC_STC_000_11, this);	// 클린롤러 하부
	myStcTitle[100].SubclassDlgItem(IDC_STC_12_86, this);	// 클린롤러 누름
	myStcTitle[101].SubclassDlgItem(IDC_STC_000_14, this);	// 검사시작
	myStcTitle[102].SubclassDlgItem(IDC_STC_12_88, this);	// 버퍼롤러 상승
	myStcTitle[103].SubclassDlgItem(IDC_STC_12_87, this);	// 버퍼롤러 고정

															// AOI상
	myStcTitle[17].SubclassDlgItem(IDC_STC_30_, this);		// 피딩 정회전
	myStcTitle[104].SubclassDlgItem(IDC_STC_12_79, this);	// 피딩 역회전
	myStcTitle[18].SubclassDlgItem(IDC_STC_12_13, this);	// 텐션 정방향
	myStcTitle[19].SubclassDlgItem(IDC_STC_12_11, this);	// 텐션 역방향
	//myStcTitle[20].SubclassDlgItem(IDC_STC_12_15, this);	// 피딩 HOME
	myStcTitle[21].SubclassDlgItem(IDC_STC_12_14, this);	// 피딩 진공
	myStcTitle[22].SubclassDlgItem(IDC_STC_12_17, this);	// 피딩 클램프
	myStcTitle[23].SubclassDlgItem(IDC_STC_12_16, this);	// 텐션 클램프
	myStcTitle[59].SubclassDlgItem(IDC_STC_12_44, this);	// 테이블 진공
	myStcTitle[60].SubclassDlgItem(IDC_STC_12_43, this);	// 테이블 블로어
	myStcTitle[24].SubclassDlgItem(IDC_STC_12_19, this);	// 테이블 실린더
	//myStcTitle[56].SubclassDlgItem(IDC_STC_12_40, this);	// 클린롤러 상부	
	myStcTitle[105].SubclassDlgItem(IDC_STC_000_13, this);	// 클린롤러 하부
	myStcTitle[106].SubclassDlgItem(IDC_STC_12_80, this);	// 클린롤러 누름
	myStcTitle[107].SubclassDlgItem(IDC_STC_12_81, this);	// 검사시작
	myStcTitle[108].SubclassDlgItem(IDC_STC_12_82, this);	// 초음파 세정기 하강
	myStcTitle[109].SubclassDlgItem(IDC_STC_12_83, this);	// 초음파 세정기 운전
	myStcTitle[110].SubclassDlgItem(IDC_STC_12_84, this);	// 초음파 세정기 속도

															// 각인기
	myStcTitle[79].SubclassDlgItem(IDC_STC_20_2, this);		// 피딩 정회전
	myStcTitle[87].SubclassDlgItem(IDC_STC_000_8, this);	// 피딩 역회전
	myStcTitle[80].SubclassDlgItem(IDC_STC_12_58, this);	// 텐션 정방향
	myStcTitle[81].SubclassDlgItem(IDC_STC_12_57, this);	// 텐션 역방향
	//myStcTitle[82].SubclassDlgItem(IDC_STC_12_60, this);	// 피딩 HOME
	myStcTitle[83].SubclassDlgItem(IDC_STC_12_59, this);	// 피딩 진공
	myStcTitle[84].SubclassDlgItem(IDC_STC_12_62, this);	// 피딩 클램프
	myStcTitle[85].SubclassDlgItem(IDC_STC_12_61, this);	// 텐션 클램프
	myStcTitle[89].SubclassDlgItem(IDC_STC_12_66, this);	// 테이블 진공
	myStcTitle[90].SubclassDlgItem(IDC_STC_12_65, this);	// 테이블 블로어
	myStcTitle[86].SubclassDlgItem(IDC_STC_12_63, this);	// 테이블 실린더
	myStcTitle[88].SubclassDlgItem(IDC_STC_12_64, this);	// 초음파 세정기 하강
	myStcTitle[91].SubclassDlgItem(IDC_STC_000_10, this);	// 초음파 세정기 운전
	myStcTitle[111].SubclassDlgItem(IDC_STC_12_73, this);	// 초음파 세정기 속도
	//myStcTitle[112].SubclassDlgItem(IDC_STC_12_74, this);	// 버퍼롤 상승
	//myStcTitle[113].SubclassDlgItem(IDC_STC_12_75, this);	// 버퍼롤 고정
	myStcTitle[114].SubclassDlgItem(IDC_STC_12_76, this);	// 정렬시작
	myStcTitle[115].SubclassDlgItem(IDC_STC_12_77, this);	// 각인시작
	myStcTitle[116].SubclassDlgItem(IDC_STC_12_78, this);	// 리딩시작

															// Uncoiler
	myStcTitle[25].SubclassDlgItem(IDC_STC_40_, this);		// 제품척 클램프
	myStcTitle[117].SubclassDlgItem(IDC_STC_12_72, this);	// 간지척 클램프
	myStcTitle[26].SubclassDlgItem(IDC_STC_12_20, this);	// 이음매 클램프
	myStcTitle[27].SubclassDlgItem(IDC_STC_12_18, this);	// 제품샤프트 지지대
	myStcTitle[41].SubclassDlgItem(IDC_STC_12_26, this);	// EPC HOME
	myStcTitle[28].SubclassDlgItem(IDC_STC_12_22, this);	// EPC #1
	myStcTitle[30].SubclassDlgItem(IDC_STC_12_24, this);	// EPC #2
	myStcTitle[43].SubclassDlgItem(IDC_STC_12_28, this);	// EPC #3
	myStcTitle[44].SubclassDlgItem(IDC_STC_12_29, this);	// 제품인버터 정방향
	myStcTitle[31].SubclassDlgItem(IDC_STC_12_25, this);	// 제품인버터 역방향
	myStcTitle[32].SubclassDlgItem(IDC_STC_12_23, this);	// 간지인버터 정방향
	myStcTitle[45].SubclassDlgItem(IDC_STC_12_30, this);	// 간지인버터 역방향
	//myStcTitle[29].SubclassDlgItem(IDC_STC_12_21, this);	// 댄서롤 상승
	//myStcTitle[42].SubclassDlgItem(IDC_STC_12_27, this);	// 댄서롤 고정
	myStcTitle[118].SubclassDlgItem(IDC_STC_12_67, this);	// EPC 자동
	myStcTitle[119].SubclassDlgItem(IDC_STC_12_68, this);	// EPC 수동
	myStcTitle[120].SubclassDlgItem(IDC_STC_12_69, this);	// EPC In
	myStcTitle[121].SubclassDlgItem(IDC_STC_12_70, this);	// EPC Out
	myStcTitle[122].SubclassDlgItem(IDC_STC_12_71, this);	// EPC 원점

															// Status
	myStcTitle[123].SubclassDlgItem(IDC_STATIC_2D_DONE_READ, this);		// 2D 리딩완료
	myStcTitle[124].SubclassDlgItem(IDC_STATIC_2D_DONE_ENGRAVE, this);	// 2D 각인완료
	myStcTitle[125].SubclassDlgItem(IDC_STATIC_UP_DONE_TEST, this);		// 상면 검사완료
	myStcTitle[126].SubclassDlgItem(IDC_STATIC_DN_DONE_TEST, this);		// 하면 검사완료
	myStcTitle[127].SubclassDlgItem(IDC_STATIC_PUNCH_DONE, this);		// 마킹 작업완료

	myStcTitle[128].SubclassDlgItem(IDC_STATIC_L, this);
	myStcTitle[129].SubclassDlgItem(IDC_STATIC_L_LT, this);
	myStcTitle[130].SubclassDlgItem(IDC_STATIC_L_LB, this);
	myStcTitle[131].SubclassDlgItem(IDC_STATIC_L_RB, this);
	myStcTitle[132].SubclassDlgItem(IDC_STATIC_L_RT, this);

	myStcTitle[133].SubclassDlgItem(IDC_STATIC_R, this);
	myStcTitle[134].SubclassDlgItem(IDC_STATIC_R_LT, this);
	myStcTitle[135].SubclassDlgItem(IDC_STATIC_R_LB, this);
	myStcTitle[136].SubclassDlgItem(IDC_STATIC_R_RB, this);
	myStcTitle[137].SubclassDlgItem(IDC_STATIC_R_RT, this);

	for (int i = 0; i < MAX_MENU03_STC; i++)
	{
		if(i == 12 || i == 20 || i == 29 || i == 42 || i == 56 || i == 64 || i == 72 || i == 73 || i == 82 || i == 98 || i == 112 || i == 113)
			continue;
		myStcTitle[i].SetFontName(_T("Arial"));
		myStcTitle[i].SetFontSize(12);
		myStcTitle[i].SetFontBold(TRUE);
		myStcTitle[i].SetTextColor(RGB_NAVY);
		myStcTitle[i].SetBkColor(RGB_DLG_FRM);
	}
}

void CDlgMenu03::Disp()
{
	DispMain();
	DispRecoiler();
	DispPunch();
	DispAoiDn();
	DispAoiUp();
	DispEngrave();
	DispUncoiler();
}

void CDlgMenu03::DispMain()
{
	BOOL bOn;

	// [Main]
	bOn = pDoc->BtnStatus.Main.Ready;	// 마킹부 운전준비 스위치 램프
	if (myBtn[34].GetCheck() != bOn)
		myBtn[34].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Main.Run;		// 마킹부 운전 스위치 램프
	if (myBtn[0].GetCheck() != bOn)
		myBtn[0].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Main.Reset;	// 마킹부 리셋 스위치 램프
	if (myBtn[1].GetCheck() != bOn)
		myBtn[1].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Main.Stop;	// 마킹부 정지 스위치 램프
	if (myBtn[33].GetCheck() != bOn)
		myBtn[33].SetCheck(bOn);

	pDoc->Status.bAuto = pDoc->BtnStatus.Main.Auto;
	pDoc->Status.bManual = pDoc->BtnStatus.Main.Manual;
	if (pDoc->Status.bAuto)	// Auto
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_DN)
			myStcTitle[33].SetImageBk(LBL_IMG_DN);	// 자 동
		if (myStcTitle[35].GetImageBk() != LBL_IMG_UP)
			myStcTitle[35].SetImageBk(LBL_IMG_UP);	// 수 동 
	}
	else if (pDoc->Status.bManual)	// Manual
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_UP)
			myStcTitle[33].SetImageBk(LBL_IMG_UP);	// 자 동
		if (myStcTitle[35].GetImageBk() != LBL_IMG_DN)
			myStcTitle[35].SetImageBk(LBL_IMG_DN);	// 수 동 
	}
	else
	{
		if (myStcTitle[33].GetImageBk() != LBL_IMG_UP)
			myStcTitle[33].SetImageBk(LBL_IMG_UP);	// 자 동
		if (myStcTitle[35].GetImageBk() != LBL_IMG_UP)
			myStcTitle[35].SetImageBk(LBL_IMG_UP);	// 수 동 
	}

	// [Torque Motor]
	//bOn = pDoc->WorkingInfo.Motion.bMkTq = pDoc->BtnStatus.Tq.Mk;
	//if (myBtn[2].GetImageBk() != bOn)
	//	myBtn[2].SetCheck(bOn);

	//bOn = pDoc->WorkingInfo.Motion.bAoiTq = pDoc->BtnStatus.Tq.Aoi;
	//if (myBtn[3].GetImageBk() != bOn)
	//	myBtn[3].SetCheck(bOn);

	//bOn = pDoc->WorkingInfo.Motion.bEngraveTq = pDoc->BtnStatus.Tq.Eng;
	//if (myBtn[83].GetImageBk() != bOn)
	//	myBtn[83].SetCheck(bOn);

	// [운전 모드]
	bOn = pDoc->BtnStatus.RunMode.ConnectModule;
	if (myBtn[47].GetImageBk() != bOn)
		myBtn[47].SetCheck(bOn);

	bOn = pDoc->BtnStatus.RunMode.FeedOnePanel;
	if (myBtn[87].GetImageBk() != bOn)
		myBtn[87].SetCheck(bOn);

	// [레이저]
	bOn = pDoc->BtnStatus.Lsr.Laser340mm;
	if (myBtn[2].GetImageBk() != bOn)
		myBtn[2].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Lsr.Laser346mm;
	if (myBtn[3].GetImageBk() != bOn)
		myBtn[3].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Lsr.Laser380mm;
	if (myBtn[83].GetImageBk() != bOn)
		myBtn[83].SetCheck(bOn);

	// [InductionMotor]
	CString sCap;
	bOn = pDoc->BtnStatus.FeedDir.FeedCcwRecoiler; //pDoc->BtnStatus.Induct.Rc;	// [One Metal]
	if (bOn)
	{
		sCap = myStcTitle[74].GetText();
		if (sCap != _T("Recoiler\r정방향"))
		{
			myStcTitle[74].ShowWindow(SW_HIDE);
			myStcTitle[74].SetTextColor(RGB_RED);
			myStcTitle[74].SetText(_T("Recoiler\r정방향"));
			myStcTitle[74].ShowWindow(SW_SHOW);
		}
	}
	else
	{
		sCap = myStcTitle[74].GetText();
		if (sCap != _T("Recoiler\r정방향"))
		{
			myStcTitle[74].ShowWindow(SW_HIDE);
			myStcTitle[74].SetTextColor(RGB_NAVY);
			myStcTitle[74].SetText(_T("Recoiler\r정방향"));
			myStcTitle[74].ShowWindow(SW_SHOW);
		}
	}

	if (myBtn[68].GetImageBk() != bOn) // Induction Motor Recoiler\r역방향 ON
	{
		myBtn[68].SetCheck(bOn);
	}

	bOn = pDoc->BtnStatus.FeedDir.FeedCcwUncoiler; //pDoc->BtnStatus.Induct.Uc;	// [Two Metal]
	if (bOn)
	{
		sCap = myStcTitle[75].GetText();
		if (sCap != _T("Uncoiler\r역방향"))
		{
			myStcTitle[75].ShowWindow(SW_HIDE);
			myStcTitle[75].SetTextColor(RGB_RED);
			myStcTitle[75].SetText(_T("Uncoiler\r역방향"));
			myStcTitle[75].ShowWindow(SW_SHOW);
		}
	}
	else
	{
		sCap = myStcTitle[75].GetText();
		if (sCap != _T("Uncoiler\r정방향"))
		{
			myStcTitle[75].ShowWindow(SW_HIDE);
			myStcTitle[75].SetTextColor(RGB_NAVY);
			myStcTitle[75].SetText(_T("Uncoiler\r정방향"));
			myStcTitle[75].ShowWindow(SW_SHOW);
		}
	}

	if (myBtn[69].GetImageBk() != bOn) // Induction Motor Uncoiler\r역방향 ON
	{
		myBtn[69].SetCheck(bOn);
	}

	// [댄서롤/버퍼롤]
	bOn = pDoc->BtnStatus.Dancer.AllDancerUpDn;
	if (myBtn[70].GetImageBk() != bOn)
		myBtn[70].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Dancer.AllDancerFixOnOff;
	if (myBtn[71].GetImageBk() != bOn)
		myBtn[71].SetCheck(bOn);

	// [Core 150mm]
	//bOn = pDoc->WorkingInfo.LastJob.bCore150Recoiler = pDoc->BtnStatus.Core150.Rc;
	//if (myBtn[70].GetImageBk() != bOn)
	//	myBtn[70].SetCheck(bOn);

	//bOn = pDoc->WorkingInfo.LastJob.bCore150Uncoiler = pDoc->BtnStatus.Core150.Uc;
	//if (myBtn[71].GetImageBk() != bOn)
	//	myBtn[71].SetCheck(bOn);
}

void CDlgMenu03::DispRecoiler()
{
	BOOL bOn;

	// [Recoiler]
	bOn = pDoc->BtnStatus.Rc.ChuckPcb;			// 리코일러 제품척 클램프 스위치 램프
	if (myBtn[4].GetImageBk() != bOn)
		myBtn[4].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.ChuckPaper;		// 리코일러 간지척 클램프 스위치 램프
	if (myBtn[88].GetImageBk() != bOn)
		myBtn[88].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.JoinClamp;			// 리코일러 이음매 클램프 스위치 램프
	if (myBtn[5].GetImageBk() != bOn)
		myBtn[5].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PcbShaftSupport;	// 리코일러 제품샤프트 지지대 스위치 램프
	if (myBtn[6].GetImageBk() != bOn)
		myBtn[6].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.EpcActHome;		// 리코일러 EPC 액추에이터 HOME 스위치 램프
	if (myBtn[41].GetImageBk() != bOn)
		myBtn[41].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.EpcActFirst;		// 리코일러 EPC 액추에이터 #1 스위치 램프
	if (myBtn[42].GetImageBk() != bOn)
		myBtn[42].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.EpcActSecond;		// 리코일러 EPC 액추에이터 #2 스위치 램프
	if (myBtn[43].GetImageBk() != bOn)
		myBtn[43].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.EpcActThird;		// 리코일러 EPC 액추에이터 #3 스위치 램프
	if (myBtn[7].GetImageBk() != bOn)
		myBtn[7].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.EpcAuto;			// 리코일러 EPC 자동 스위치 램프
	if (myBtn[89].GetImageBk() != bOn)
		myBtn[89].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.EpcManual;			// 리코일러 EPC 수동 스위치 램프
	if (myBtn[90].GetImageBk() != bOn)
		myBtn[90].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.EpcHome;			// 리코일러 EPC 원점 스위치 램프
	if (myBtn[93].GetImageBk() != bOn)
		myBtn[93].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.EpcIn;				// 리코일러 EPC In 스위치 램프
	if (myBtn[91].GetImageBk() != bOn)
		myBtn[91].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.EpcOut;			// 리코일러 EPC Out 스위치 램프
	if (myBtn[92].GetImageBk() != bOn)
		myBtn[92].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PcbInverterCw;		// 리코일러 제품 인버터 정방향 스위치 램프
	if (myBtn[8].GetImageBk() != bOn)
		myBtn[8].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PcbInverterCcw;	// 리코일러 제품 인버터 역방향 스위치 램프
	if (myBtn[44].GetImageBk() != bOn)
		myBtn[44].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PaperInverterCw;	// 리코일러 간지 인버터 정방향 스위치 램프
	if (myBtn[45].GetImageBk() != bOn)
		myBtn[45].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Rc.PaperInverterCcw;	// 리코일러 간지 인버터 역방향 스위치 램프
	if (myBtn[46].GetImageBk() != bOn)
		myBtn[46].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.Rc.DancerUpper;		// 리코일러 댄서롤 상승 스위치 램프
	//if (myBtn[66].GetImageBk() != bOn)
	//	myBtn[66].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.Rc.DancerFixer;		// 리코일러 댄서롤 고정 스위치 램프
	//if (myBtn[67].GetImageBk() != bOn)
	//	myBtn[67].SetCheck(bOn);
}

void CDlgMenu03::DispPunch()
{
	BOOL bOn;

	// [Marking]
	bOn = pDoc->BtnStatus.Mk.FeedCw;			// 마킹부 피딩 정방향 스위치 램프
	if (myBtn[9].GetImageBk() != bOn)
		myBtn[9].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FeedCcw;			// 마킹부 피딩 역방향 스위치 램프
	if (myBtn[94].GetImageBk() != bOn)
		myBtn[94].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TensionCw;			// 마킹부 텐션 정방향 스위치 램프
	if (myBtn[10].GetImageBk() != bOn)
		myBtn[10].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TensionCcw;		// 마킹부 텐션 역방향 스위치 램프
	if (myBtn[11].GetImageBk() != bOn)
		myBtn[11].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.Mk.FeedHome;			// 마킹부 피딩 HOME 스위치 램프
	//if (myBtn[12].GetImageBk() != bOn)
	//	myBtn[12].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FeedVacuum;		//  마킹부 피딩 진공 스위치 램프
	if (myBtn[13].GetImageBk() != bOn)
		myBtn[13].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.FeedClamp;			// 마킹부 피딩 클램프 스위치 램프
	if (myBtn[14].GetImageBk() != bOn)
		myBtn[14].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TensionClamp;		// 마킹부 텐션 클램프 스위치 램프  
	if (myBtn[15].GetImageBk() != bOn)
		myBtn[15].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TableVacuum;		// 마킹부 테이블 진공 스위치 램프
	if (myBtn[51].GetImageBk() != bOn)
		myBtn[51].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TableBlower;		// 마킹부 테이블 에어블로어 스위치 
	if (myBtn[52].GetImageBk() != bOn)
		myBtn[52].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TableCylinder;		// 마킹부 테이블 실린더 스위치 램프
	if (myBtn[16].GetImageBk() != bOn)
		myBtn[16].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.TableClampDn;		// 마킹부 테이블 클램프 하강 스위치 램프
	if (myBtn[49].GetCheck() != bOn)
		myBtn[49].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.Mk.TableClampForward;	// 마킹부 테이블 클램프 전진 스위치 램프
	//if (myBtn[95].GetImageBk() != bOn)
	//	myBtn[95].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Mk.PunchStart;		// 마킹부 마킹 시작 스위치 램프
	if (myBtn[48].GetImageBk() != bOn)
		myBtn[48].SetCheck(bOn);
}

void CDlgMenu03::DispAoiDn()
{
	BOOL bOn;

	// [하면 AOI]
	bOn = pDoc->BtnStatus.AoiDn.FeedCw;				// 검사부 하 피딩 정방향 스위치 램프
	if (myBtn[55].GetImageBk() != bOn)
		myBtn[55].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FeedCcw;			// 검사부 하 피딩 역방향 스위치 램프
	if (myBtn[96].GetImageBk() != bOn)
		myBtn[96].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TensionCw;			// 검사부 하 텐션 정방향 스위치 램프
	if (myBtn[56].GetImageBk() != bOn)
		myBtn[56].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TensionCcw;			// 검사부 하 텐션 역방향 스위치 램프
	if (myBtn[57].GetImageBk() != bOn)
		myBtn[57].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.AoiDn.FeedHome;			// 검사부 하 피딩 HOME 스위치 램프
	//if (myBtn[58].GetImageBk() != bOn)
	//	myBtn[58].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FeedVacuum;			// 검사부 하 피딩 진공 스위치 램프
	if (myBtn[59].GetImageBk() != bOn)
		myBtn[59].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.FeedClamp;			// 검사부 하 피딩 클램프 스위치 램프
	if (myBtn[60].GetImageBk() != bOn)
		myBtn[60].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TensionClamp;		// 검사부 하 텐션 클램프 스위치 램프
	if (myBtn[61].GetImageBk() != bOn)
		myBtn[61].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TableVacuum;		// 검사부 하 테이블 진공 스위치 램프
	if (myBtn[62].GetImageBk() != bOn)
		myBtn[62].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TableBlower;		// 검사부 하 테이블 에어블로어 스위치 램프
	if (myBtn[63].GetImageBk() != bOn)
		myBtn[63].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TableCylinder;		// 검사부 하 테이블 실린더 스위치 램프
	if (myBtn[64].GetImageBk() != bOn)
		myBtn[64].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.CleanRollerUp;		// 검사부 하 클린롤러 상부 스위치 램프
	if (myBtn[65].GetImageBk() != bOn)
		myBtn[65].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.CleanRollerDn;		// 검사부 하 클린롤러 하부 스위치 램프
	if (myBtn[86].GetImageBk() != bOn)
		myBtn[86].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.CleanRollerPush;	// 검사부 하 클린롤러 누름 스위치 램프
	if (myBtn[97].GetImageBk() != bOn)
		myBtn[97].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.TestStart;			// 검사부 하 AOI 상면 검사 시작 스위치 램프
	if (myBtn[98].GetImageBk() != bOn)
		myBtn[98].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.BufferRollerUp;		// 검사부 하 버퍼롤 상승 스위치 램프
	if (myBtn[99].GetImageBk() != bOn)
		myBtn[99].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiDn.BufferRollerFix;	// 검사부 하 버퍼롤 고정 스위치 램프
	if (myBtn[100].GetImageBk() != bOn)
		myBtn[100].SetCheck(bOn);
}

void CDlgMenu03::DispAoiUp()
{
	BOOL bOn;

	// [상면 AOI]
	bOn = pDoc->BtnStatus.AoiUp.FeedCw;	// 검사부 상 피딩 정방향 스위치 램프
	if (myBtn[17].GetImageBk() != bOn)
		myBtn[17].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FeedCcw;	// 검사부 상 피딩 역방향 스위치 램프
	if (myBtn[101].GetImageBk() != bOn)
		myBtn[101].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TensionCw;	// 검사부 상 텐션 정방향 스위치 램프
	if (myBtn[18].GetImageBk() != bOn)
		myBtn[18].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TensionCcw;	// 검사부 상 텐션 역방향 스위치 램프
	if (myBtn[19].GetImageBk() != bOn)
		myBtn[19].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.AoiUp.FeedHome;	// 검사부 상 피딩 HOME 스위치 램프
	//if (myBtn[20].GetImageBk() != bOn)
	//	myBtn[20].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FeedVacuum;	// 검사부 상 피딩 진공 스위치 램프
	if (myBtn[21].GetImageBk() != bOn)
		myBtn[21].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.FeedClamp;	// 검사부 상 피딩 클램프 스위치 램프
	if (myBtn[22].GetImageBk() != bOn)
		myBtn[22].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TensionClamp;	// 검사부 상 텐션 클램프 스위치 램프
	if (myBtn[23].GetImageBk() != bOn)
		myBtn[23].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TableVacuum;	// 검사부 상 테이블 진공 스위치 램프
	if (myBtn[53].GetImageBk() != bOn)
		myBtn[53].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TableBlower; // 검사부 상 테이블 에어블로어 스위치 램프
	if (myBtn[54].GetImageBk() != bOn)
		myBtn[54].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TableCylinder;	// 검사부 상 테이블 실린더 스위치 램프
	if (myBtn[24].GetImageBk() != bOn)
		myBtn[24].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.AoiUp.CleanRollerUp; // 검사부 상 클린롤러 상부 스위치 램프
	//if (myBtn[50].GetImageBk() != bOn)
	//	myBtn[50].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.CleanRollerDn;	// 검사부 상 클린롤러 하부 스위치 램프
	if (myBtn[102].GetImageBk() != bOn)
		myBtn[102].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.CleanRollerPush; // 검사부 상 클린롤러 누름 스위치 램프
	if (myBtn[103].GetImageBk() != bOn)
		myBtn[103].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.TestStart;	// 검사부 상 검사 시작 스위치 램프
	if (myBtn[104].GetImageBk() != bOn)
		myBtn[104].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.UltrasonicDn; // 검사부 상 초음파 세정기 하강 스위치 램프
	if (myBtn[105].GetImageBk() != bOn)
		myBtn[105].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.UltrasonicRun;	// 검사부 상 초음파 세정기 운전 스위치 램프
	if (myBtn[106].GetImageBk() != bOn)
		myBtn[106].SetCheck(bOn);

	bOn = pDoc->BtnStatus.AoiUp.UltrasonicSpeed;	// 검사부 상 초음파 세정기 속도 스위치 램프
	if (myBtn[107].GetImageBk() != bOn)
		myBtn[107].SetCheck(bOn);
}

void CDlgMenu03::DispEngrave()
{
	BOOL bOn;

	bOn = pDoc->BtnStatus.Eng.FeedCw;			// 각인부 피딩 정방향 스위치 램프
	if (myBtn[72].GetImageBk() != bOn)
		myBtn[72].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FeedCcw;			// 각인부 피딩 역방향 스위치 램프
	if (myBtn[84].GetImageBk() != bOn)
		myBtn[84].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TensionCw;		// 각인부 텐션 정방향 스위치 램프
	if (myBtn[73].GetImageBk() != bOn)
		myBtn[73].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TensionCcw;		// 각인부 텐션 역방향 스위치 램프
	if (myBtn[74].GetImageBk() != bOn)
		myBtn[74].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.Eng.FeedHome;			// 각인부 피딩 HOME 스위치 램프
	//if (myBtn[75].GetImageBk() != bOn)
	//	myBtn[75].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FeedVacuum;		// 각인부 피딩 진공 스위치 램프
	if (myBtn[76].GetImageBk() != bOn)
		myBtn[76].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.FeedClamp;		// 각인부 피딩 클램프 스위치 램프
	if (myBtn[77].GetImageBk() != bOn)
		myBtn[77].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TensionClamp;		// 각인부 텐션 클램프 스위치 램프
	if (myBtn[78].GetImageBk() != bOn)
		myBtn[78].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TableVacuum;		// 각인부 테이블 진공 스위치 램프
	if (myBtn[79].GetImageBk() != bOn)
		myBtn[79].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TableBlower;		// 각인부 테이블 에어블로어 스위치 램프
	if (myBtn[80].GetImageBk() != bOn)
		myBtn[80].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.TableCylinder;	// 각인부 테이블 실린더 스위치 램프
	if (myBtn[81].GetImageBk() != bOn)
		myBtn[81].SetCheck(bOn);


	bOn = pDoc->BtnStatus.Eng.UltrasonicDown;	// 각인부 초음파 세정기 하강 스위치 램프
	if (myBtn[82].GetImageBk() != bOn)
		myBtn[82].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.UltrasonicRun;	// 각인부 초음파 세정기 운전 스위치 램프
	if (myBtn[85].GetImageBk() != bOn)
		myBtn[85].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.UltrasonicSpeed;	// 각인부 초음파 세정기 속도 스위치 램프
	if (myBtn[108].GetImageBk() != bOn)
		myBtn[108].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.Eng.DancerUpper;		// 각인부 버퍼롤 상승 스위치 램프
	//if (myBtn[109].GetImageBk() != bOn)
	//	myBtn[109].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.Eng.DancerFixer;		// 각인부 버퍼롤 고정 스위치 램프
	//if (myBtn[110].GetImageBk() != bOn)
	//	myBtn[110].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.AlignStart;		// 각인부 정렬 시작 스위치 램프
	if (myBtn[111].GetImageBk() != bOn)
		myBtn[111].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.LaserStart;		// 각인부 각인 시작 스위치 램프
	if (myBtn[112].GetImageBk() != bOn)
		myBtn[112].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Eng.ReadStart;		// 각인부 리딩 시작 스위치 램프
	if (myBtn[113].GetImageBk() != bOn)
		myBtn[113].SetCheck(bOn);
}

void CDlgMenu03::DispUncoiler()
{
	BOOL bOn;

	// [Uncoiler]
	bOn = pDoc->BtnStatus.Uc.ChuckPcb;			// 언코일러 제품척 클램프 스위치 램프
	if (myBtn[25].GetImageBk() != bOn)
		myBtn[25].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.ChuckPaper;		// 언코일러 간지척 클램프 스위치 램프
	if (myBtn[40].GetImageBk() != bOn)
		myBtn[40].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.JoinClamp;			// 언코일러 이음매 클램프 스위치 램프
	if (myBtn[26].GetImageBk() != bOn)
		myBtn[26].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PcbShaftSupport;	// 언코일러 제품샤프트 지지대 스위치 램프
	if (myBtn[27].GetImageBk() != bOn)
		myBtn[27].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.EpcActHome;		// 언코일러 EPC 액추에이터 HOME 스위치 램프
	if (myBtn[28].GetImageBk() != bOn)
		myBtn[28].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.EpcActFirst;		// 언코일러 EPC 액추에이터 #1 스위치 램프
	if (myBtn[29].GetImageBk() != bOn)
		myBtn[29].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.EpcActSecond;		// 언코일러 EPC 액추에이터 #2 스위치 램프
	if (myBtn[30].GetImageBk() != bOn)
		myBtn[30].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.EpcActThird;		// 언코일러 EPC 액추에이터 #3 스위치 램프 
	if (myBtn[37].GetImageBk() != bOn)
		myBtn[37].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.EpcAuto;			// 언코일러 EPC 자동 스위치 램프
	if (myBtn[114].GetImageBk() != bOn)
		myBtn[114].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.EpcManual;			// 언코일러 EPC 수동 스위치 램프
	if (myBtn[115].GetImageBk() != bOn)
		myBtn[115].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.EpcHome;			// 언코일러 EPC 원점 스위치 램프 
	if (myBtn[118].GetImageBk() != bOn)
		myBtn[118].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.EpcIn;				// 언코일러 EPC In 스위치 램프
	if (myBtn[116].GetImageBk() != bOn)
		myBtn[116].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.EpcOut;			// 언코일러 EPC Out 스위치 램프
	if (myBtn[117].GetImageBk() != bOn)
		myBtn[117].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PcbInverterCw;		// 언코일러 제품 인버터 정방향 스위치 램프
	if (myBtn[32].GetImageBk() != bOn)
		myBtn[32].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PcbInverterCcw;	// 언코일러 제품 인버터 역방향 스위치 램프 
	if (myBtn[31].GetImageBk() != bOn)
		myBtn[31].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PaperInverterCw;	// 언코일러 간지 인버터 정방향 스위치 램프 
	if (myBtn[35].GetImageBk() != bOn)
		myBtn[35].SetCheck(bOn);

	bOn = pDoc->BtnStatus.Uc.PaperInverterCcw;	// 언코일러 간지 인버터 역방향 스위치 램프
	if (myBtn[36].GetImageBk() != bOn)
		myBtn[36].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.Uc.DancerUpper;		// 언코일러 댄서롤 상승 스위치 램프
	//if (myBtn[38].GetImageBk() != bOn)
	//	myBtn[38].SetCheck(bOn);

	//bOn = pDoc->BtnStatus.Uc.DancerFixer;		// 언코일러 댄서롤 고정 스위치 램프 
	//if (myBtn[39].GetImageBk() != bOn)
	//	myBtn[39].SetCheck(bOn);
}

void CDlgMenu03::OnTimer(UINT_PTR nIDEvent)//(UINT nIDEvent)
{
	CString strMsg, strTitle;

	if (nIDEvent == TIM_MENU03_DISP)
	{
		KillTimer(TIM_MENU03_DISP);
		UpdateSignal();
		if (m_bTIM_MENU03_DISP)
			SetTimer(TIM_MENU03_DISP, 300, NULL);
	}

	//if(nIDEvent == TIM_CHK_DONE_MKAOI)
	//{
	//	KillTimer(TIM_CHK_DONE_MKAOI);
	//	if(!(pDoc->m_pMpeSignal[5] & (0x01<<1)) && !(pDoc->m_pMpeSignal[5] & (0x01<<0)))	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF) && 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
	//	{
	//		m_bTIM_CHK_DONE_MKAOI = FALSE;
	//	}
	//	if(m_bTIM_CHK_DONE_MKAOI)
	//		SetTimer(TIM_CHK_DONE_MKAOI, 100, NULL);
	//}
	
	//if(nIDEvent == TIM_CHK_DONE_MK)
	//{
	//	KillTimer(TIM_CHK_DONE_MK);
	//	if(!(pDoc->m_pMpeSignal[5] & (0x01<<1)))	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF)
	//		m_bTIM_CHK_DONE_MK = FALSE;
	//	if(m_bTIM_CHK_DONE_MK)
	//		SetTimer(TIM_CHK_DONE_MK, 100, NULL);
	//}
	
	//if(nIDEvent == TIM_CHK_DONE_AOI)
	//{
	//	KillTimer(TIM_CHK_DONE_AOI);
	//	if(!(pDoc->m_pMpeSignal[5] & (0x01<<0)))	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
	//		m_bTIM_CHK_DONE_AOI = FALSE;
	//	if(m_bTIM_CHK_DONE_AOI)
	//		SetTimer(TIM_CHK_DONE_AOI, 100, NULL);
	//}

	if(nIDEvent == TIM_CHK_MREG)
	{
		KillTimer(TIM_CHK_MREG);
		pView->ChkMRegOut();
	}
	
	//if(nIDEvent == TIM_CHK_DONE_BUF_HOME)
	//{
	//	KillTimer(TIM_CHK_DONE_BUF_HOME);
	//	if(pDoc->m_pMpeSignal[4] & (0x01<<2))	// 마킹부 버퍼롤러 홈동작 ON (PLC가 홈동작 완료 후 OFF)
	//	{
	//		pView->GetDispMsg(strMsg, strTitle);
	//		if(strMsg != _T("Searching Buffer Home Position...") || strTitle != _T("Homming"))
	//			pView->DispMsg(_T("Searching Buffer Home Position..."), _T("Homming"), RGB_GREEN, DELAY_TIME_MSG);
	//	}
	//	else
	//	{
	//		m_bTIM_CHK_DONE_BUF_HOME = FALSE;
	//		pView->m_bBufHomeDone = TRUE;
	//		pView->ClrDispMsg();
	//	}
	//	if(m_bTIM_CHK_DONE_BUF_HOME)
	//		SetTimer(TIM_CHK_DONE_BUF_HOME, 100, NULL);
	//}
	
	//if(nIDEvent == TIM_CHK_DONE_BUF_INIT)
	//{
	//	KillTimer(TIM_CHK_DONE_BUF_INIT);
	//	if(pDoc->m_pMpeSignal[4] & (0x01<<10))	// 마킹부 버퍼 초기위치 이동(PC가 ON, PLC가 OFF)
	//	{
	//		pView->GetDispMsg(strMsg, strTitle);
	//		if(strMsg != _T("Searching Buffer Initial Position...") || strTitle != _T("Moving"))
	//			pView->DispMsg(_T("Searching Buffer Initial Position..."), _T("Moving"), RGB_GREEN, DELAY_TIME_MSG);
	//	}
	//	else
	//	{
	//		m_bTIM_CHK_DONE_BUF_INIT = FALSE;
	//		pView->ClrDispMsg();
	//	}
	//	if(m_bTIM_CHK_DONE_BUF_INIT)
	//		SetTimer(TIM_CHK_DONE_BUF_INIT, 100, NULL);
	//}
	
	if(nIDEvent == TIM_CHK_DONE_READY)
	{
		KillTimer(TIM_CHK_DONE_READY);

		if(pDoc->m_pMpeSignal[0] & (0x01<<3))	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
		{
			m_bTIM_CHK_DONE_READY = FALSE;
			pView->m_bReadyDone = TRUE; pDoc->SetStatus(_T("General"), _T("bReadyDone"), pView->m_bReadyDone);
			//pView->MpeWrite(pView->Plc.DlgMenu01.PlcReadyDone, 0);	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)MB440100
		}
		else
		{
			pView->GetDispMsg(strMsg, strTitle);
			if(strMsg != _T("Searching Buffer Home Position...") || strTitle != _T("Homming"))
			{
				pView->m_bReadyDone = FALSE; pDoc->SetStatus(_T("General"), _T("bReadyDone"), pView->m_bReadyDone);
			}
		}
		if(m_bTIM_CHK_DONE_READY)
			SetTimer(TIM_CHK_DONE_READY, 100, NULL);
	}

	if(nIDEvent == TIM_AOI_RESET_OFF)
	{
		KillTimer(TIM_AOI_RESET_OFF);
		SwAoiReset(FALSE);
	}

	if(nIDEvent == TIM_AOI_LOTEND_OFF)
	{
		KillTimer(TIM_AOI_LOTEND_OFF);
	}

	//if (nIDEvent == TIM_CHK_DONE_ENGRAVE)
	//{
	//	KillTimer(TIM_CHK_DONE_ENGRAVE);
	//	if (!(pDoc->m_pMpeSignal[6] & (0x01 << 12)))	// 각인부 피딩 CW ON (PLC가 피딩완료 후 OFF) - MB44017C
	//		m_bTIM_CHK_DONE_ENGRAVE = FALSE;
	//	if (m_bTIM_CHK_DONE_ENGRAVE)
	//		SetTimer(TIM_CHK_DONE_ENGRAVE, 100, NULL);
	//}
	
//	if (nIDEvent == TIM_CHK_DONE_ENG_BUF_HOME)
//	{
//		KillTimer(TIM_CHK_DONE_ENG_BUF_HOME);
//
//		if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[6] & (0x01 << 2))	// 각인부 버퍼롤러 홈동작 ON (PLC가 홈동작 완료 후 OFF)
//		{
//			pView->GetDispMsg(strMsg, strTitle);
//			if (strMsg != _T("Searching Engrave Buffer Home Position...") || strTitle != _T("Homming"))
//				pView->DispMsg(_T("Searching Engrave Buffer Home Position..."), _T("Homming"), RGB_GREEN, DELAY_TIME_MSG);
//		}
//		else
//		{
//			m_bTIM_CHK_DONE_ENG_BUF_HOME = FALSE;
//			pView->m_bEngBufHomeDone = TRUE;
//			pView->ClrDispMsg();
//
//#ifdef USE_ENGRAVE
//			if (pView && pView->m_pEngrave)
//			{
//				pDoc->WorkingInfo.Motion.bEngBuffHommingDone = TRUE;
//				pView->m_pEngrave->SetEngBuffHomeDone();	//_SigInx::_EngBuffOrgMvDone // 각인부 버퍼롤러 홈동작 ON (PLC가 홈동작 완료 후 OFF)
//			}
//#endif
//		}
//		if (m_bTIM_CHK_DONE_ENG_BUF_HOME)
//			SetTimer(TIM_CHK_DONE_ENG_BUF_HOME, 100, NULL);
//	}

//	if (nIDEvent == TIM_CHK_DONE_ENG_BUF_INIT)
//	{
//		KillTimer(TIM_CHK_DONE_ENG_BUF_INIT);
//
//		if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[6] & (0x01 << 10))	// 각인부 버퍼 초기위치 이동(PC가 ON, PLC가 OFF)
//		{
//			pView->GetDispMsg(strMsg, strTitle);
//			if (strMsg != _T("Searching Engrave Buffer Initial Position...") || strTitle != _T("Moving"))
//				pView->DispMsg(_T("Searching Engrave Buffer Initial Position..."), _T("Moving"), RGB_GREEN, DELAY_TIME_MSG);
//		}
//		else
//		{
//			m_bTIM_CHK_DONE_ENG_BUF_INIT = FALSE;
//			pView->ClrDispMsg();
//
//#ifdef USE_ENGRAVE
//			if (pView && pView->m_pEngrave)
//			{
//				pDoc->WorkingInfo.Motion.bEngBuffInitMvDone = TRUE;
//				pView->m_pEngrave->SetEngBuffInitMoveDone();	//_SigInx::_EngBuffInitPosMvDone // 각인부 버퍼 초기위치 이동(PC가 ON, PLC가 OFF)
//			}
//#endif
//		}
//		if (m_bTIM_CHK_DONE_ENG_BUF_INIT)
//			SetTimer(TIM_CHK_DONE_ENG_BUF_INIT, 100, NULL);
//	}

	//if (nIDEvent == TIM_CHK_DONE_AOI)
	//{
	//	KillTimer(TIM_CHK_DONE_AOI);
	//	if (!(pDoc->m_pMpeSignal[5] & (0x01 << 0)))	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
	//		m_bTIM_CHK_DONE_AOI = FALSE;
	//	if (m_bTIM_CHK_DONE_AOI)
	//		SetTimer(TIM_CHK_DONE_AOI, 100, NULL);
	//}

	CDialog::OnTimer(nIDEvent);
}

LRESULT CDlgMenu03::OnMyBtnDown(WPARAM wPara, LPARAM lPara)
{

	if(pView->m_bProbDn[0])
	{
		if(pView->m_pVoiceCoil[0])
		{
			pView->m_pVoiceCoil[0]->SearchHomeSmac(0);
			pView->m_pVoiceCoil[0]->MoveSmacShiftPos(0);
			pView->m_bProbDn[0] = FALSE;
		}
	}

	if(pView->m_bProbDn[1])
	{
		if(pView->m_pVoiceCoil[1])
		{
			pView->m_pVoiceCoil[1]->SearchHomeSmac(1);
			pView->m_pVoiceCoil[1]->MoveSmacShiftPos(1);
			pView->m_bProbDn[1] = FALSE;
		}
	}
	
	int nCtrlID = (int)lPara;

 	SwMyBtnDown(nCtrlID);
	return 0L;
}

LRESULT CDlgMenu03::OnMyBtnUp(WPARAM wPara, LPARAM lPara)
{
	int nCtrlID = (int)lPara;

	SwMyBtnUp(nCtrlID);
	return 0L;
}

void CDlgMenu03::SwMyBtnDown(int nCtrlID)
{
	if (!pView)
		return;

	long lData = 1; BOOL bData = TRUE;
#ifndef TEST_MODE
	if (nCtrlID != IDC_CHK_33 && nCtrlID != IDC_CHK_0)		// 마킹부 정지 스위치 , 마킹부 운전 스위치
	{
		if (!pDoc->Status.bManual)
		{
			//pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의"), RGB_YELLOW);
			pView->MsgBox(_T("수동 모드가 아닙니다."));
			return;
		}
	}
#endif
#ifdef USE_ENGRAVE
	if (!pView->m_pEngrave)
		return;
#endif

	if (!pView->m_pMpe)
		return;

	SwMpeBtn(nCtrlID, lData);
}

void CDlgMenu03::SwMyBtnUp(int nCtrlID)
{
	if (!pView)
		return;

	long lData = 0; BOOL bData = FALSE;

	if (nCtrlID != IDC_CHK_33 && nCtrlID != IDC_CHK_0)		// 마킹부 정지 스위치 , 마킹부 운전 스위치
	{
		if (!pDoc->Status.bManual)
		{
			//pView->DispMsg(_T("수동 모드가 아닙니다."), _T("주의"), RGB_YELLOW);
			pView->MsgBox(_T("수동 모드가 아닙니다."));
			return;
		}
	}

#ifdef USE_ENGRAVE
	if (!pView->m_pEngrave)
		return;
#endif

	if (!pView->m_pMpe)
		return;

	SwMpeBtn(nCtrlID, lData);
}

void CDlgMenu03::SwMpeBtn(int nCtrlID, long lData)
{
	switch (nCtrlID)
	{
	// Main
	case IDC_CHK_34:		// 마킹부 운전준비 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.Ready, lData);
		break;
	case IDC_CHK_0:			// 마킹부 운전 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.Run, lData);
		break;
	case IDC_CHK_1:			// 마킹부 리셋 스위치	
		pView->MpeWrite(pView->Plc.DlgMenu03.Reset, lData);
		break;
	case IDC_CHK_33:		// 마킹부 정지 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.Stop, lData);
		break;

	// Run Mode
	case IDC_CHK_95:			// 연동선택 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModule, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ConnectModule, 0);
		break;
	case IDC_CHK_96:			// 한판넬이송 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, 0);
		break;

	// Laser
	case IDC_CHK_2:			// 340mm 스위치
		pView->MpeWrite(pView->Plc.DlgInfo.Laser340mm, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgInfo.Laser340mm, 0);
		break;
	case IDC_CHK_3:			// 346mm 스위치
		pView->MpeWrite(pView->Plc.DlgInfo.Laser346mm, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgInfo.Laser346mm, 0);
		break;
	case IDC_CHK_84:			// 380mm 스위치
		pView->MpeWrite(pView->Plc.DlgInfo.Laser380mm, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgInfo.Laser380mm, 0);
		break;

	// Feed Direction
	case IDC_CHK_68:			// 리코일러 역방향 스위치
		pView->MpeWrite(pView->Plc.DlgInfo.FeedCcwRecoiler, lData);
		//pView->MpeWrite(pView->Plc.DlgInfo.FeedCcwRecoiler, 1);
		//Sleep(300);
		//pView->MpeWrite(pView->Plc.DlgInfo.FeedCcwRecoiler, 0);
		break;
	case IDC_CHK_69:			// 언코일러 역방향 스위치
		pView->MpeWrite(pView->Plc.DlgInfo.FeedCcwUncoiler, lData);
		//pView->MpeWrite(pView->Plc.DlgInfo.FeedCcwUncoiler, 1);
		//Sleep(300);
		//pView->MpeWrite(pView->Plc.DlgInfo.FeedCcwUncoiler, 0);
		break;

	// 댄서롤/버퍼롤
	case IDC_CHK_70:			// 전체 상승/하강
		pView->MpeWrite(pView->Plc.DlgMenu03.AllDancerUpDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.AllDancerUpDn, 0);
		break;
	case IDC_CHK_71:			// 전체고정 On/Off
		pView->MpeWrite(pView->Plc.DlgMenu03.AllDancerFixOnOff, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.AllDancerFixOnOff, 0);
		break;

	// Recoiler
	case IDC_CHK_4:			// 리코일러 제품척 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPcbRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPcbRecoiler, 0);
		break;
	case IDC_CHK_120:			// 리코일러 간지척 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPaperRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPaperRecoiler, 0);
		break;
	case IDC_CHK_5:		// 리코일러 이음매 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinClampRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinClampRecoiler, 0);
		break;
	case IDC_CHK_6:		// 리코일러 제품샤프트 지지대 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbShaftSupportRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbShaftSupportRecoiler, 0);
		break;
	case IDC_CHK_41:		// 리코일러 EPC HOME 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActHomeRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActHomeRecoiler, 0);
		break;
	case IDC_CHK_42:			// 리코일러 EPC #1 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActFirstRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActFirstRecoiler, 0);
		break;
	case IDC_CHK_43:			// 리코일러 EPC #2 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActSecondRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActSecondRecoiler, 0);
		break;
	case IDC_CHK_7:				// 리코일러 EPC #3 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActThirdRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActThirdRecoiler, 0);
		break;
	case IDC_CHK_8:		// 리코일러 제품인버터 정방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbInverterCwRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbInverterCwRecoiler, 0);
		break;
	case IDC_CHK_44:		// 리코일러 제품인버터 역방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbInverterCcwRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbInverterCcwRecoiler, 0);
		break;
	case IDC_CHK_45:		// 리코일러 간지인버터 정방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperInverterCwRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperInverterCwRecoiler, 0);
		break;
	case IDC_CHK_46:		// 리코일러 간지인버터 역방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperInverterCcwRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperInverterCcwRecoiler, 0);
		break;

	//case IDC_CHK_66:		// 리코일러 댄서롤 상승 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerUpperRecoiler, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerUpperRecoiler, 0);
	//	break;
	//case IDC_CHK_67:		// 리코일러 댄서롤 고정 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerFixerRecoiler, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerFixerRecoiler, 0);
	//	break;
	case IDC_CHK_121:		// 리코일러 EPC 자동 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcAutoRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcAutoRecoiler, 0);
		break;
	case IDC_CHK_122:		// 리코일러 EPC 수동 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcManualRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcManualRecoiler, 0);
		break;
	case IDC_CHK_123:		// 리코일러 EPC In 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcInRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcInRecoiler, 0);
		break;
	case IDC_CHK_124:		// 리코일러 EPC Out 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcOutRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcOutRecoiler, 0);
		break;
	case IDC_CHK_125:		// 리코일러 EPC 원점 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcHomeRecoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcHomeRecoiler, 0);
		break;

		// Punch
	case IDC_CHK_9:			// 마킹부 ◀정회전 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwPunch, 0);
		break;
	case IDC_CHK_116:		// 마킹부 역회전▶ 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwPunch, 0);
		break;
	case IDC_CHK_10:		// 마킹부 텐션 정방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwPunch, 0);
		break;
	case IDC_CHK_11:		// 마킹부 텐션 역방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCcwPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCcwPunch, 0);
		break;
	//case IDC_CHK_12:		// 마킹부 피딩 HOME 스위치 
	//	pView->MpeWrite(pView->Plc.DlgMenu03.FeedHomePunch, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.FeedHomePunch, 0);
	//	break;
	case IDC_CHK_13:		// 마킹부 피딩 진공 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumPunch, 0);
		break;
	case IDC_CHK_14:		// 마킹부 피딩 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampPunch, 0);
		break;
	case IDC_CHK_15:		// 마킹부 텐션 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampPunch, 0);
		break;
	case IDC_CHK_51:		// 마킹부 테이블 진공 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumPunch, 0);
		break;
	case IDC_CHK_52:		// 마킹부 테이블 블로어 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerPunch, 0);
		break;
	case IDC_CHK_16:		// 마킹부 테이블 실린더 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableCylinderPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableCylinderPunch, 0);
		break;
	case IDC_CHK_49:		// 마킹부 테이블 클램프하강 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableClampDnPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableClampDnPunch, 0);
		break;
	//case IDC_CHK_118:		// 마킹부 테이블 클램프전진 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.TableClampForwardPunch, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.TableClampForwardPunch, 0);
	//	break;
	case IDC_CHK_48:		// 마킹부 마킹 시작 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PunchStartPunch, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PunchStartPunch, 0);
		break;

		// AOI 하면 검사	
	case IDC_CHK_55:		// 검사부 하 ◀정회전 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwAoiDn, 1);		// 검사부 하 연동 온/오프 스위치
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwAoiDn, 0);		// 검사부 하 연동 온/오프 스위치
		break;
	case IDC_CHK_110:		// 검사부 하 역회전▶ 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwAoiDn, 0);
		break;
	case IDC_CHK_56:		// 검사부 하 텐션 정방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwAoiDn, 0);
		break;
	case IDC_CHK_57:		// 검사부 하 텐션 역방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCcwAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCcwAoiDn, 0);
		break;
	//case IDC_CHK_58:		// 검사부 하 피딩 HOME 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.FeedHomeAoiDn, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.FeedHomeAoiDn, 0);
	//	break;
	case IDC_CHK_59:		// 검사부 하 피딩 진공 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumAoiDn, 0);
		break;
	case IDC_CHK_60:		// 검사부 하 피딩 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampAoiDn, 0);
		break;
	case IDC_CHK_61:		// 검사부 하 텐션 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampAoiDn, 0);
		break;
	case IDC_CHK_64:		// 검사부 하 테이블 진공 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumAoiDn, 0);
		break;
	case IDC_CHK_65:		// 검사부 하 테이블 블로어 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerAoiDn, 0);
		break;
	case IDC_CHK_62:		// 검사부 하 테이블 실린더 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableCylinderAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableCylinderAoiDn, 0);
		break;
	case IDC_CHK_63:		// 검사부 하 클린롤러 상부 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerUpAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerUpAoiDn, 0);
		break;
	case IDC_CHK_88:		// 검사부 하 클린롤러 하부 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerDnAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerDnAoiDn, 0);
		break;
	case IDC_CHK_111:		// 검사부 하 클린롤러 누름 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerPushAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerPushAoiDn, 0);
		break;
	case IDC_CHK_112:		// 검사부 하 검사시작 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TestStartAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TestStartAoiDn, 0);
		break;
	case IDC_CHK_114:		// 검사부 하 버퍼롤러 상승 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.BufferRollerUpAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.BufferRollerUpAoiDn, 0);
		break;
	case IDC_CHK_113:		// 검사부 하 버퍼롤러 고정 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.BufferRollerFixAoiDn, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.BufferRollerFixAoiDn, 0);
		break;

		// AOI 상면 검사
	case IDC_CHK_17:		// 검사부 상 ◀정회전 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwAoiUp, 0);
		break;
	case IDC_CHK_103:		// 검사부 상 역회전▶ 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwAoiUp, 0);
		break;
	case IDC_CHK_18:		// 검사부 상 텐션 정방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwAoiUp, 0);
		break;
	case IDC_CHK_19:		// 검사부 상 텐션 역방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCcwAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCcwAoiUp, 0);
		break;
	//case IDC_CHK_20:		// 검사부 상 피딩 HOME 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.FeedHomeAoiUp, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.FeedHomeAoiUp, 0);
	//	break;
	case IDC_CHK_21:		// 검사부 상 피딩 진공 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumAoiUp, 0);
		break;
	case IDC_CHK_22:		// 검사부 상 피딩 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampAoiUp, 0);
		break;
	case IDC_CHK_23:		// 검사부 상 텐션 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampAoiUp, 0);
		break;
	case IDC_CHK_53:		// 검사부 상 테이블 진공 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumAoiUp, 0);
		break;
	case IDC_CHK_54:		// 검사부 상 테이블 블로어 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerAoiUp, 0);
		break;
	case IDC_CHK_24:		// 검사부 상 테이블 실린더 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableCylinderAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableCylinderAoiUp, 0);
		break;
	//case IDC_CHK_50:		// 검사부 상 클린롤러 상부 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerUpAoiUp, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerUpAoiUp, 0);
	//	break;
	case IDC_CHK_104:		// 검사부 상 클린롤러 하부 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerDnAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerDnAoiUp, 0);
		break;
	case IDC_CHK_105:		// 검사부 상 클린롤러 누름 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerPushAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.CleanRollerPushAoiUp, 0);
		break;
	case IDC_CHK_106:		// 검사부 상 검사시작 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TestStartAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TestStartAoiUp, 0);
		break;
	case IDC_CHK_107:		// 검사부 상 초음파 세정기하강 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicDnAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicDnAoiUp, 0);
		break;
	case IDC_CHK_115:		// 검사부 상 초음파 세정기운전 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicRunAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicRunAoiUp, 0);
		break;
	case IDC_CHK_109:		// 검사부 상 초음파 세정기속도 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedAoiUp, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedAoiUp, 0);
		break;

		// 각인기
	case IDC_CHK_72:		// 각인기 ◀정회전 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCwEngrave, 0);
		break;
	case IDC_CHK_80:		// 각인기 역회전▶ 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedCcwEngrave, 0);
		break;
	case IDC_CHK_73:		// 각인기 텐션 정방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwEngrave, 0);
		break;
	case IDC_CHK_74:		// 각인기 텐션 역방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCcwEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionCcwEngrave, 0);
		break;
	//case IDC_CHK_75:		// 각인기 피딩 HOME 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.FeedHomeEngrave, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.FeedHomeEngrave, 0);
	//	break;
	case IDC_CHK_76:		// 각인기 피딩 진공 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedVacuumEngrave, 0);
		break;
	case IDC_CHK_77:		// 각인기 피딩 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.FeedClampEngrave, 0);
		break;
	case IDC_CHK_78:		// 각인기 텐션 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TensionClampEngrave, 0);
		break;
	case IDC_CHK_82:		// 각인기 테이블 진공 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableVacuumEngrave, 0);
		break;
	case IDC_CHK_83:		// 각인기 테이블 블로어 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableBlowerEngrave, 0);
		break;
	case IDC_CHK_79:		// 각인기 테이블 실린더 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.TableCylinderEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.TableCylinderEngrave, 0);
		break;
	case IDC_CHK_81:		// 각인기 초음파 세정기하강 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicDownEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicDownEngrave, 0);
		break;
	case IDC_CHK_87:		// 각인기 초음파 세정기운전 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicRunEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicRunEngrave, 0);
		break;
	case IDC_CHK_97:		// 각인기 초음파 세정기속도 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedEngrave, 0);
		break;
	//case IDC_CHK_98:		// 각인기 버퍼롤 상승 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerUpperEngrave, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerUpperEngrave, 0);
	//	break;
	//case IDC_CHK_99:		// 각인기 버퍼롤 고정 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerFixerEngrave, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerFixerEngrave, 0);
	//	break;
	case IDC_CHK_100:		// 각인기 정렬시작 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.AlignStartEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.AlignStartEngrave, 0);
		break;
	case IDC_CHK_101:		// 각인기 각인시작 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.LaserStartEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.LaserStartEngrave, 0);
		break;
	case IDC_CHK_102:		// 각인기 리딩시작 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.ReadStartEngrave, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ReadStartEngrave, 0);
		break;

		// Uncoiler
	case IDC_CHK_25:		// 언코일러 제품척 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPcbUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPcbUncoiler, 0);
		break;
	case IDC_CHK_94:		// 언코일러 간지척 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPaperUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.ChuckPaperUncoiler, 0);
		break;
	case IDC_CHK_26:		// 언코일러 이음매 클램프 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinClampUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.JoinClampUncoiler, 0);
		break;
	case IDC_CHK_27:		// 언코일러 제품샤프트 지지대 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbShaftSupportUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbShaftSupportUncoiler, 0);
		break;
	case IDC_CHK_35:		// 언코일러 EPC HOME 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActHomeUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActHomeUncoiler, 0);
		break;
	case IDC_CHK_28:		// 언코일러 EPC #1 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActFirstUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActFirstUncoiler, 0);
		break;
	case IDC_CHK_30:		// 언코일러 EPC #2 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActSecondUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActSecondUncoiler, 0);
		break;
	case IDC_CHK_37:		// 언코일러 EPC #3 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActThirdUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcActThirdUncoiler, 0);
		break;
	case IDC_CHK_38:		// 언코일러 제품인버터 정방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbInverterCwUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbInverterCwUncoiler, 0);
		break;
	case IDC_CHK_31:		// 언코일러 제품인버터 역방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbInverterCcwUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PcbInverterCcwUncoiler, 0);
		break;
	case IDC_CHK_32:		// 언코일러 간지인버터 정방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperInverterCwUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperInverterCwUncoiler, 0);
		break;
	case IDC_CHK_39:		// 언코일러 간지인버터 역방향 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperInverterCcwUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.PaperInverterCcwUncoiler, 0);
		break;
	//case IDC_CHK_29:		// 언코일러 댄서롤 상승 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerUpperUncoiler, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerUpperUncoiler, 0);
	//	break;
	//case IDC_CHK_36:		// 언코일러 댄서롤 고정 스위치
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerFixerUncoiler, 1);
	//	Sleep(300);
	//	pView->MpeWrite(pView->Plc.DlgMenu03.DancerFixerUncoiler, 0);
	//	break;
	case IDC_CHK_89:		// 언코일러 EPC 자동 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcAutoUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcAutoUncoiler, 0);
		break;
	case IDC_CHK_90:		// 언코일러 EPC 수동 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcManualUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcManualUncoiler, 0);
		break;
	case IDC_CHK_91:		// 언코일러 EPC In 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcInUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcInUncoiler, 0);
		break;
	case IDC_CHK_92:		// 언코일러 EPC Out 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcOutUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcOutUncoiler, 0);
		break;
	case IDC_CHK_93:		// 언코일러 EPC 원점 스위치
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcHomeUncoiler, 1);
		Sleep(300);
		pView->MpeWrite(pView->Plc.DlgMenu03.EpcHomeUncoiler, 0);
		break;
	}
}

BOOL CDlgMenu03::GetCw() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	if(pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<3))		//[29] 마킹부 피딩 정회전 스위치 램프
		return TRUE;
	if(pDoc->m_pMpeIo[nInSeg + 8] & (0x01<<3))		//[32] 검사부 피딩 정회전 스위치 램프
		return TRUE;
	return FALSE;
}

void CDlgMenu03::SetCw(BOOL bOn) 
{
	long lData = 1;
	pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwPunch, lData);	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF)
	pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwAoi, lData);	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
}

BOOL CDlgMenu03::GetCcw() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	if(pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<4))		//[29] 마킹부 피딩 역회전 스위치 램프 
		return TRUE;
	if(pDoc->m_pMpeIo[nInSeg + 8] & (0x01<<4))		//[32] 검사부 피딩 역회전 스위치 램프
		return TRUE;
	return FALSE;
}

void CDlgMenu03::SetCcw(BOOL bOn) 
{
	long lData = 1;
	pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwPunch, lData);	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF)
	pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwAoi, lData);	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
}

void CDlgMenu03::MoveMk(double dOffset)
{
	double fLen, fVel, fAcc, fTarget;

	fLen = pDoc->GetOnePnlLen();
	fVel = pDoc->GetOnePnlVel();
	fAcc = pDoc->GetOnePnlAcc();
	fTarget = dOffset;// fLen + dOffset;

	long lData = (long)(fTarget * 1000.0);
	pView->MpeWrite(pView->Plc.DlgFrameHigh.FeedOffsetPunch, lData);	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)

	MoveMk();
} 

void CDlgMenu03::MoveMk(int nDir) 
{
	if(pDoc->WorkingInfo.LastJob.bMkOnePnl)
	{
		pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwPunch, 1);	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF)
	}
}

void CDlgMenu03::MoveAoi(double dOffset)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	double fLen, fVel, fAcc, fTarget;

	fLen = pDoc->GetOnePnlLen();
	fVel = pDoc->GetOnePnlVel();
	fAcc = pDoc->GetOnePnlAcc();
	fTarget = dOffset;// fLen - dOffset;

 	long lData = (long)(fTarget * 1000.0);
	if (bDualTest)
		pView->MpeWrite(pView->Plc.DlgFrameHigh.FeedOffsetAoiDn, lData);	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
	else
		pView->MpeWrite(pView->Plc.DlgFrameHigh.FeedOffsetAoiUp, lData);	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)

	MoveAoi();
} 

void CDlgMenu03::MoveAoi(int nDir) 
{
	if(pDoc->WorkingInfo.LastJob.bAoiOnePnl)
	{
		pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwAoi, 1);	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
	}
}

//void CDlgMenu03::ChkDoneMk() 
//{
//	if(pDoc->WorkingInfo.LastJob.bMkOnePnl)
//	{
//		if(!m_bTIM_CHK_DONE_MK)
//		{
//			m_bTIM_CHK_DONE_MK = TRUE;
//			SetTimer(TIM_CHK_DONE_MK, 100, NULL);
//		}
//	}
//}

//void CDlgMenu03::ChkDoneAoi() 
//{
//	if(pDoc->WorkingInfo.LastJob.bAoiOnePnl)
//	{
//		if(!m_bTIM_CHK_DONE_AOI)
//		{
//			m_bTIM_CHK_DONE_AOI = TRUE;
//			SetTimer(TIM_CHK_DONE_AOI, 100, NULL);
//		}
//	}
//}

//void CDlgMenu03::ChkDoneMkAoi() 
//{
//	if(pDoc->WorkingInfo.LastJob.bMkOnePnl && pDoc->WorkingInfo.LastJob.bAoiOnePnl)
//	{
//		if(!m_bTIM_CHK_DONE_MKAOI)
//		{
//			m_bTIM_CHK_DONE_MKAOI = TRUE;
//			SetTimer(TIM_CHK_DONE_MKAOI, 100, NULL);
//		}
//	}
//}

BOOL CDlgMenu03::GetRelation() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	if(!(pDoc->m_pMpeIo[nInSeg + 16] & (0x01<<1)))	//[40] 리코일러 연동 온/오프 스위치 램프
		return FALSE;
	if(!(pDoc->m_pMpeIo[nInSeg] & (0x01<<1)))		//[24] 언코일러 연동 온/오프 스위치 램프
		return FALSE;

	if(!(pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<1)))	//[29] 마킹부 연동 온/오프 스위치 램프
		return FALSE;
	if(!(pDoc->m_pMpeIo[nInSeg + 8] & (0x01<<1)))	//[32] 검사부 상 연동 온/오프 스위치 램프
		return FALSE;
	if(!(pDoc->m_pMpeIo[nInSeg + 12] & (0x01<<1)))	//[36] 검사부 하 연동 온/오프 스위치 램프
		return FALSE;

	return TRUE;
}

BOOL CDlgMenu03::GetRun() 
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	if(pView->m_bSwRun) // 초기운전시 램프 On/Off
		return TRUE;

	return FALSE;
}

BOOL CDlgMenu03::GetReady()
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	if(pView->m_bReadyDone)
		return TRUE;

	return FALSE;
}

BOOL CDlgMenu03::GetReset()
{
	if(!pDoc->m_pMpeIo)
		return FALSE;

	if(pView->m_bSwReset)
		return TRUE;

	return FALSE;
}

BOOL CDlgMenu03::GetMkTq()
{
	return (pDoc->WorkingInfo.Motion.bMkTq);
}

BOOL CDlgMenu03::GetAoiTq()
{
	return (pDoc->WorkingInfo.Motion.bAoiTq);
}

BOOL CDlgMenu03::GetEngraveTq()
{
	return (pDoc->WorkingInfo.Motion.bEngraveTq);
}

void CDlgMenu03::SetMkTq(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwPunch, bOn?1:0);	// 마킹부 텐션 ON (PC가 ON/OFF시킴)
	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.Motion.bMkTq?1:0);
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE_ON"), sData, sPath);
}

void CDlgMenu03::SetAoiTq(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwAoiUp, bOn?1:0);	// 검사부 텐션 ON (PC가 ON/OFF시킴)
	pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwAoiDn, bOn?1:0);	// 검사부 텐션 ON (PC가 ON/OFF시킴)
	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.Motion.bAoiTq?1:0);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE_ON"), sData, sPath);	
}

void CDlgMenu03::SetEngraveTq(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.TensionCwEngrave, bOn?1:0);	// 각인부 텐션 ON (PC가 ON/OFF시킴)
	CString sData, sPath=PATH_WORKING_INFO;
	sData.Format(_T("%d"), pDoc->WorkingInfo.Motion.bEngraveTq?1:0);
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE_ON"), sData, sPath);	
}

void CDlgMenu03::SetMkOnePnl(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, bOn?1:0);	// 한판넬 이송상태 ON (PC가 ON, OFF)
	CString sData, sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bMkOnePnl = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bMkOnePnl?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), sData, sPath);
}

void CDlgMenu03::SetEngraveCleanner(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedEngrave, bOn ? 1 : 0); // [각인부 세정기]
	CString sData, sPath = PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bVelEngraveUltrasonic = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bVelEngraveUltrasonic ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Cleanner Vel"), sData, sPath);
}

void CDlgMenu03::SetAoiDnCleanner(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.UltrasonicSpeedAoiUp, bOn ? 1 : 0); // [AOI(상) 세정기]
	CString sData, sPath = PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bVelAoiDnUltrasonic = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bVelAoiDnUltrasonic ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("AoiDn Cleanner Vel"), sData, sPath);
}

void CDlgMenu03::SetAoiOnePnl(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, bOn?1:0);	// 한판넬 이송상태 ON (PC가 ON, OFF)
	CString sData, sPath=PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bAoiOnePnl = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bAoiOnePnl?1:0);
	::WritePrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), sData, sPath);
}

BOOL CDlgMenu03::DoReset()
{
	pView->DispThreadTick();

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	// 자신의 로컬파일에 설정
	pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgYes, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgNo, FALSE);
	pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgOk, FALSE);


	if(pDoc->Status.bManual)
	{
		if (pDoc->GetTestMode() == MODE_INNER || pDoc->WorkingInfo.System.bUseEngrave)
		{
			if (!pView->IsConnectedEng())
			{
				pView->MsgBox(_T("각인부 프로그램이 연결되지 않았습니다."));
				return FALSE;
			}
		}

		BOOL bInit=TRUE;

		if(m_bTIM_CHK_DONE_READY)
		{
			m_bTIM_CHK_DONE_READY = FALSE;
			pView->m_bReadyDone = FALSE; pDoc->SetStatus(_T("General"), _T("bReadyDone"), pView->m_bReadyDone);
			pView->MpeWrite(pView->Plc.DlgMenu01.PlcReadyDone, 0);	// PLC 운전준비 완료(PC가 확인하고 Reset시킴.)MB440100
		}
		pView->ClrDispMsg();

		if (pView->m_pEngrave)
		{
			pDoc->BtnStatus.EngAuto.Init = TRUE;
			pDoc->BtnStatus.EngAuto.IsInit = FALSE;
			pView->m_pEngrave->SwEngAutoInit(TRUE);
		}
		
		if(IDNO == pView->MsgBox(_T("초기화를 하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, TRUE))
			bInit = FALSE;
		else
		{
			pDoc->m_bDoneChgLot = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneChgLot"), pDoc->m_bDoneChgLot);
			pView->m_nNewLot = 0;

			if (pView->m_pDlgMenu01)
				pView->m_pDlgMenu01->ClrInfo();

			pView->ResetYield();
		}

		if(!bInit)
		{
			if(IDNO == pView->MsgBox(_T("이어가기를 하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, TRUE))
			{
				pView->m_bCont = FALSE; pDoc->SetStatus(_T("General"), _T("bCont"), pView->m_bCont);
				return FALSE;
			}
			pView->m_bCont = TRUE; pDoc->SetStatus(_T("General"), _T("bCont"), pView->m_bCont);
		}

		pView->InitAuto(bInit);
		pView->SetPathAtBuf();

		pView->m_bAoiFdWrite[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWrite[0]"), pView->m_bAoiFdWrite[0]);
		pView->m_bAoiFdWrite[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWrite[1]"), pView->m_bAoiFdWrite[1]);
		pView->m_bAoiFdWriteF[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[0]"), pView->m_bAoiFdWriteF[0]);
		pView->m_bAoiFdWriteF[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[1]"), pView->m_bAoiFdWriteF[1]);
		pView->m_bCycleStop = FALSE; pDoc->SetStatus(_T("General"), _T("bCycleStop"), pView->m_bCycleStop);
		pView->m_bContDiffLot = FALSE; pDoc->SetStatus(_T("General"), _T("bContDiffLot"), pView->m_bContDiffLot);

		pView->m_bInit = bInit;

		pView->ClearBuffer();

		pView->TowerLamp(RGB_RED, TRUE, FALSE);
		pView->DispMain(_T("정 지"), RGB_RED);	
		SwAoiReset(TRUE);
		pView->OpenReelmap();
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			pView->OpenReelmapInner();
		}

		if(bInit)
		{
			// 초기화
			pDoc->m_ListBuf[0].Clear();
			pDoc->m_ListBuf[1].Clear();
		}
		else
		{
			// 이어가기
			pView->SetListBuf();
		}
		
		return TRUE;
	}

	return FALSE;		
}

void CDlgMenu03::DoReady()
{
	if(m_bTIM_CHK_DONE_READY)
	{
		m_bTIM_CHK_DONE_READY = FALSE;
		KillTimer(TIM_CHK_DONE_READY);
	}
	ChkReadyDone();
	pDoc->DelPcrAll();
}

//void CDlgMenu03::ChkBufHomeDone()
//{
//	if(!m_bTIM_CHK_DONE_BUF_HOME)
//	{
//		m_bTIM_CHK_DONE_BUF_HOME = TRUE;
//		SetTimer(TIM_CHK_DONE_BUF_HOME, 100, NULL);
//	}
//}

void CDlgMenu03::ChkBufInitDone()
{
	if(!m_bTIM_CHK_DONE_BUF_INIT)
	{
		m_bTIM_CHK_DONE_BUF_INIT = TRUE;
		SetTimer(TIM_CHK_DONE_BUF_INIT, 100, NULL);
	}
}

void CDlgMenu03::ChkReadyDone()
{
	if(!m_bTIM_CHK_DONE_READY)
	{
		m_bTIM_CHK_DONE_READY = TRUE;
		SetTimer(TIM_CHK_DONE_READY, 100, NULL);
	}
}

// [Main]
void CDlgMenu03::SwRun() 
{
	if(pDoc->Status.bAuto)
	{
		pView->m_bSwRun = TRUE;
		pView->m_nStop = 0;
		pView->m_bSwStop = FALSE;
		pView->m_bSwReady = FALSE;
		pView->m_bSwReset = FALSE;
		pView->m_bCycleStop = FALSE; pDoc->SetStatus(_T("General"), _T("bCycleStop"), pView->m_bCycleStop);

		// 한판넬 이송 On
		SetMkOnePnl(TRUE);
		SetAoiOnePnl(TRUE);
		SetEngraveOnePnl(TRUE);

		pDoc->m_sAlmMsg = _T("");
		pDoc->m_sIsAlmMsg = _T("");
		pDoc->m_sPrevAlmMsg = _T("");
	}

	pView->ClrDispMsg();
}

BOOL CDlgMenu03::IsStop() 
{
	return pView->m_bSwStop;
}

void CDlgMenu03::SwStop() 
{
	pView->m_bSwRun = FALSE;
	pView->m_bSwStop = TRUE;
	pView->m_bSwReady = FALSE;
	pView->m_bSwReset = FALSE;

	pView->MpeWrite(_T("MB40003B"), 1);					// 마킹부 즉시 정지 스위치(PC가 On / PLC Off시킴)
	pView->PlcAlm(0, TRUE);
	Sleep(30);
	pView->TowerLamp(RGB_RED, TRUE, FALSE);
	pView->MpeWrite(pView->Plc.DlgMenu03.LampStop, 1);	// 마킹부 정지 스위치 램프 ON(PC가 On/Off시킴)
	pView->ClrDispMsg();
	pView->DispMain(_T("정 지"), RGB_RED);
}

void CDlgMenu03::SwReady()
{
 	if(!pView->m_bSwRun)
	{
		pView->m_bSwReady = TRUE;
 		DoReady();
	}
}

void CDlgMenu03::SwReset()
{
	pView->ClrDispMsg();

	if(!DoReset())
		return;

	pView->m_bSwRun = FALSE;
	pView->m_bSwStop = FALSE;
	pView->m_bSwReady = FALSE;
	pView->m_bSwReset = TRUE;
}

void CDlgMenu03::SwEngraveTq(BOOL bOn)
{
	SetEngraveTq(bOn);
}

// [Marking]

BOOL CDlgMenu03::IsBufRolSol() 
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = FALSE;
	bOn = pDoc->m_pMpeIo[nInSeg + 5] & (0x01<<11);		//[29] MB00371B,	Y425B	,	마킹부 댄서롤 상승/하강 스위치 램프
	return bOn;
}

// [AOI]
void CDlgMenu03::SwAoiRelation() 
{
	if(pDoc->WorkingInfo.LastJob.bMkOnePnl != pDoc->WorkingInfo.LastJob.bAoiOnePnl) 
	{
		SetMkOnePnl(FALSE);
		SetAoiOnePnl(FALSE);
		SetEngraveOnePnl(FALSE);
	}
}

BOOL CDlgMenu03::IsAoiFdVac()
{
	BOOL bOn = FALSE;
	bOn = pDoc->m_pMpeIb[9] & (0x01<<4);
	return bOn;
}

BOOL CDlgMenu03::IsAoiLdRun()
{
	BOOL bRtn = TRUE;

	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 11)) ? TRUE : FALSE;		// 검사부 상 자동 운전 <-> X432B I/F
 	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01<<11)) ? TRUE : FALSE;		// 검사부 하 자동 운전 <-> X442B I/F

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if(bDualTest)
	{
		if((pView->m_AoiLdRun & 0x03) == 0x03)
		{
			if(!bOn0 || !bOn1)
				bRtn = FALSE;
		}

		if(bOn0)
			pView->m_AoiLdRun |= (0x01<<0);
		else 
			pView->m_AoiLdRun &= ~(0x01<<0);

		if(bOn1)
			pView->m_AoiLdRun |= (0x01<<1);
		else 
			pView->m_AoiLdRun &= ~(0x01<<1);


		if(bOn0 && bOn1)
			return TRUE;
		else if(bOn0 && !bOn1)
			return FALSE;
		else if(!bOn0 && !bOn1)
			return FALSE;
	}
	else
	{
		if((pView->m_AoiLdRun & 0x03) == 0x03)
		{
			if(!bOn0)
				bRtn = FALSE;
		}

		if(bOn0)
			pView->m_AoiLdRun |= (0x01<<0);
		else 
			pView->m_AoiLdRun &= ~(0x01<<0);


		if(bOn0)
			return TRUE;

		return FALSE;
	}
	return bRtn;
}

void CDlgMenu03::SwAoiReset(BOOL bOn)
{
	if(bOn)
	{
		pView->MpeWrite(pView->Plc.DlgMenu01.ResetAoiUp, 1); // 검사부 상 Reset <-> Y436A I/F
		pView->MpeWrite(pView->Plc.DlgMenu01.ResetAoiDn, 1); // 검사부 하 Reset <-> Y446A I/F
		SetTimer(TIM_AOI_RESET_OFF, 500, NULL);
	}
	else
	{
		pView->MpeWrite(pView->Plc.DlgMenu01.ResetAoiUp, 0); // 검사부 상 Reset <-> Y436A I/F
		pView->MpeWrite(pView->Plc.DlgMenu01.ResetAoiDn, 0); // 검사부 하 Reset <-> Y446A I/F
	}
}

void CDlgMenu03::OnChk16() 
{
	BOOL bOn = pDoc->WorkingInfo.LastJob.bMkOnePnl;				// 마킹부 한판넬 이송
	SetMkOnePnl(!bOn);

	SetAoiOnePnl(!bOn);
	SetEngraveOnePnl(!bOn); 
}

void CDlgMenu03::OnChk24() 
{
	BOOL bOn = pDoc->WorkingInfo.LastJob.bAoiOnePnl;			// 검사부 한판넬 이송
	SetAoiOnePnl(!bOn);

	SetMkOnePnl(!bOn);
	SetEngraveOnePnl(!bOn);
}

BOOL CDlgMenu03::PreTranslateMessage(MSG* pMsg) 
{
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

void CDlgMenu03::DoManual()
{
	SetMkOnePnl(FALSE);
	SetAoiOnePnl(FALSE);
	SetEngraveOnePnl(FALSE);

	SwStop();

	pView->ClrDispMsg();
}

void CDlgMenu03::DoAuto()
{
	SetMkOnePnl(TRUE);
	SetAoiOnePnl(TRUE);
	SetEngraveOnePnl(TRUE);
}

void CDlgMenu03::OnChk62() 
{
	BOOL bOn = pDoc->WorkingInfo.LastJob.bAoiOnePnl;			// 검사부 한판넬 이송
	SetAoiOnePnl(!bOn);

	BOOL bRelation = GetRelation();
	if (bRelation)
	{
		SetMkOnePnl(!bOn);
		SetEngraveOnePnl(!bOn);
	}
}

void CDlgMenu03::SetCore150mmRecoiler(BOOL bOn)
{
	//pView->MpeWrite(pView->Plc.DlgMenu03.UseCore150mmRecoiler, bOn?1:0);
	//CString sData, sPath=PATH_WORKING_INFO;
	//pDoc->WorkingInfo.LastJob.bCore150Recoiler = bOn;
	//sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bCore150Recoiler?1:0);
	//::WritePrivateProfileString(_T("Last Job"), _T("Recoiler Core 150mm"), sData, sPath);
}

void CDlgMenu03::SetCore150mmUncoiler(BOOL bOn)
{
	//pView->MpeWrite(pView->Plc.DlgMenu03.UseCore150mmUncoiler, bOn?1:0);
	//CString sData, sPath=PATH_WORKING_INFO;
	//pDoc->WorkingInfo.LastJob.bCore150Uncoiler = bOn;
	//sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bCore150Uncoiler?1:0);
	//::WritePrivateProfileString(_T("Last Job"), _T("Uncoiler Core 150mm"), sData, sPath);
}

void CDlgMenu03::SetDualTest(BOOL bOn)
{
	//if(bOn)
	//{
	//	if(!myStcTitle[61].IsWindowVisible())
	//		myStcTitle[61].ShowWindow(SW_SHOW);
	//	if(!myBtn[55].IsWindowVisible())
	//		myBtn[55].ShowWindow(SW_SHOW);
	//	
	//	if(!myStcTitle[62].IsWindowVisible())
	//		myStcTitle[62].ShowWindow(SW_SHOW);
	//	if(!myBtn[56].IsWindowVisible())
	//		myBtn[56].ShowWindow(SW_SHOW);
	//	
	//	if(!myStcTitle[63].IsWindowVisible())
	//		myStcTitle[63].ShowWindow(SW_SHOW);
	//	if(!myBtn[57].IsWindowVisible())
	//		myBtn[57].ShowWindow(SW_SHOW);
	//	
	//	if(!myStcTitle[64].IsWindowVisible())
	//		myStcTitle[64].ShowWindow(SW_SHOW);
	//	if(!myBtn[58].IsWindowVisible())
	//		myBtn[58].ShowWindow(SW_SHOW);
	//	
	//	if(!myStcTitle[65].IsWindowVisible())
	//		myStcTitle[65].ShowWindow(SW_SHOW);
	//	if(!myBtn[59].IsWindowVisible())
	//		myBtn[59].ShowWindow(SW_SHOW);
	//	
	//	if(!myStcTitle[66].IsWindowVisible())
	//		myStcTitle[66].ShowWindow(SW_SHOW);
	//	if(!myBtn[60].IsWindowVisible())
	//		myBtn[60].ShowWindow(SW_SHOW);
	//	
	//	if(!myStcTitle[67].IsWindowVisible())
	//		myStcTitle[67].ShowWindow(SW_SHOW);
	//	if(!myBtn[61].IsWindowVisible())
	//		myBtn[61].ShowWindow(SW_SHOW);
	//	
	//	if(!myStcTitle[68].IsWindowVisible())
	//		myStcTitle[68].ShowWindow(SW_SHOW);
	//	if(!myBtn[62].IsWindowVisible())
	//		myBtn[62].ShowWindow(SW_SHOW);
	//	
	//	if(!myStcTitle[69].IsWindowVisible())
	//		myStcTitle[69].ShowWindow(SW_SHOW);
	//	if(!myBtn[63].IsWindowVisible())
	//		myBtn[63].ShowWindow(SW_SHOW);
	//	
	//	if(!myStcTitle[70].IsWindowVisible())
	//		myStcTitle[70].ShowWindow(SW_SHOW);
	//	if(!myBtn[64].IsWindowVisible())
	//		myBtn[64].ShowWindow(SW_SHOW);
	//	
	//	if(!myStcTitle[71].IsWindowVisible())
	//		myStcTitle[71].ShowWindow(SW_SHOW);
	//	if(!myBtn[65].IsWindowVisible())
	//		myBtn[65].ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	if(myStcTitle[61].IsWindowVisible())
	//		myStcTitle[61].ShowWindow(SW_HIDE);
	//	if(myBtn[55].IsWindowVisible())
	//		myBtn[55].ShowWindow(SW_HIDE);
	//	
	//	if(myStcTitle[62].IsWindowVisible())
	//		myStcTitle[62].ShowWindow(SW_HIDE);
	//	if(myBtn[56].IsWindowVisible())
	//		myBtn[56].ShowWindow(SW_HIDE);
	//	
	//	if(myStcTitle[63].IsWindowVisible())
	//		myStcTitle[63].ShowWindow(SW_HIDE);
	//	if(myBtn[57].IsWindowVisible())
	//		myBtn[57].ShowWindow(SW_HIDE);
	//	
	//	if(myStcTitle[64].IsWindowVisible())
	//		myStcTitle[64].ShowWindow(SW_HIDE);
	//	if(myBtn[58].IsWindowVisible())
	//		myBtn[58].ShowWindow(SW_HIDE);
	//	
	//	if(myStcTitle[65].IsWindowVisible())
	//		myStcTitle[65].ShowWindow(SW_HIDE);
	//	if(myBtn[59].IsWindowVisible())
	//		myBtn[59].ShowWindow(SW_HIDE);
	//	
	//	if(myStcTitle[66].IsWindowVisible())
	//		myStcTitle[66].ShowWindow(SW_HIDE);
	//	if(myBtn[60].IsWindowVisible())
	//		myBtn[60].ShowWindow(SW_HIDE);
	//	
	//	if(myStcTitle[67].IsWindowVisible())
	//		myStcTitle[67].ShowWindow(SW_HIDE);
	//	if(myBtn[61].IsWindowVisible())
	//		myBtn[61].ShowWindow(SW_HIDE);
	//	
	//	if(myStcTitle[68].IsWindowVisible())
	//		myStcTitle[68].ShowWindow(SW_HIDE);
	//	if(myBtn[62].IsWindowVisible())
	//		myBtn[62].ShowWindow(SW_HIDE);
	//	
	//	if(myStcTitle[69].IsWindowVisible())
	//		myStcTitle[69].ShowWindow(SW_HIDE);
	//	if(myBtn[63].IsWindowVisible())
	//		myBtn[63].ShowWindow(SW_HIDE);
	//	
	//	if(myStcTitle[70].IsWindowVisible())
	//		myStcTitle[70].ShowWindow(SW_HIDE);
	//	if(myBtn[64].IsWindowVisible())
	//		myBtn[64].ShowWindow(SW_HIDE);
	//	
	//	if(myStcTitle[71].IsWindowVisible())
	//		myStcTitle[71].ShowWindow(SW_HIDE);
	//	if(myBtn[65].IsWindowVisible())
	//		myBtn[65].ShowWindow(SW_HIDE);
	//}
}

BOOL CDlgMenu03::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB_DLG_FRM);
	return TRUE;
}

void CDlgMenu03::SetEngraveOnePnl(BOOL bOn)
{
	pView->MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, bOn ? 1 : 0);	// 한판넬 이송상태 ON (PC가 ON, OFF)
	CString sData, sPath = PATH_WORKING_INFO;
	pDoc->WorkingInfo.LastJob.bEngraveOnePnl = bOn;
	sData.Format(_T("%d"), pDoc->WorkingInfo.LastJob.bEngraveOnePnl ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave One Pannel Move On"), sData, sPath);

}

void CDlgMenu03::OnBnClickedChk79()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = pDoc->WorkingInfo.LastJob.bEngraveOnePnl;				// 각인부 한판넬 이송

	SetMkOnePnl(!bOn);
	SetAoiOnePnl(!bOn);
	SetEngraveOnePnl(!bOn);
}

void CDlgMenu03::MoveEngrave(double dOffset)
{
	double fLen, fVel, fAcc, fTarget;

	fLen = pDoc->GetOnePnlLen();
	fVel = pDoc->GetOnePnlVel();
	fAcc = pDoc->GetOnePnlAcc();
	fTarget = dOffset;// fLen + dOffset;

	long lData = (long)(fTarget * 1000.0);
	pView->MpeWrite(pView->Plc.DlgFrameHigh.FeedOffsetEngrave, lData);	// 각인부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)
	MoveEngrave();
}

void CDlgMenu03::MoveEngrave(int nDir)
{
	if (pDoc->WorkingInfo.LastJob.bEngraveOnePnl)
	{
		pView->MpeWrite(pView->Plc.DlgMenu01.FeedCwEngrave, 1);	// 각인부 피딩 CW ON (PLC가 피딩완료 후 OFF)
	}
}

//void CDlgMenu03::ChkDoneEngrave()
//{
//	if (pDoc->WorkingInfo.LastJob.bEngraveOnePnl)
//	{
//		if (!m_bTIM_CHK_DONE_ENGRAVE)
//		{
//			m_bTIM_CHK_DONE_ENGRAVE = TRUE;
//			SetTimer(TIM_CHK_DONE_ENGRAVE, 100, NULL);
//		}
//	}
//}

BOOL CDlgMenu03::IsEngraveFdVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 5);		// 각인부 피딩 진공 스위치 램프
	return bOn;
}

BOOL CDlgMenu03::IsEngraveTqVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 6);		// 각인부 토크 진공 스위치 램프
	return bOn;
}

BOOL CDlgMenu03::IsEngraveTblVac()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 7);		// 각인부 테이블 진공 스위치 램프
	return bOn;
}

BOOL CDlgMenu03::IsEngraveTblBlw()
{
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn = pDoc->m_pMpeIo[nInSeg + 21] & (0x01 << 2);		// 각인부 테이블 브로워 스위치 램프
	return bOn;
}


void CDlgMenu03::UpdateSignal()
{
	if(pView)
		pView->GetPlcParam();

	Disp();
}


//void CDlgMenu03::ChkEngBufHomeDone()
//{
//	if (!m_bTIM_CHK_DONE_ENG_BUF_HOME)
//	{
//		m_bTIM_CHK_DONE_ENG_BUF_HOME = TRUE;
//		SetTimer(TIM_CHK_DONE_ENG_BUF_HOME, 100, NULL);
//	}
//}

//void CDlgMenu03::ChkEngBufInitDone()
//{
//	if (!m_bTIM_CHK_DONE_ENG_BUF_INIT)
//	{
//		m_bTIM_CHK_DONE_ENG_BUF_INIT = TRUE;
//		SetTimer(TIM_CHK_DONE_ENG_BUF_INIT, 100, NULL);
//	}
//}

void CDlgMenu03::SetLed(int nIdx, BOOL bOn)
{
	if (bOn && myLabel[nIdx].GetImageBk() != LBL_IMG_DN)
		myLabel[nIdx].SetImageBk(LBL_IMG_DN);
	else if (!bOn && myLabel[nIdx].GetImageBk() != LBL_IMG_UP)
		myLabel[nIdx].SetImageBk(LBL_IMG_UP);
}

void CDlgMenu03::DispAlignRangeL(int* pAlignX, int* pAlignY)
{
	CString sVal;

	sVal.Format(_T("%d , %d"), pAlignX[0], pAlignY[0]);
	GetDlgItem(IDC_STATIC_ALIGN_L_LT)->SetWindowText(sVal);
	sVal.Format(_T("%d , %d"), pAlignX[1], pAlignY[1]);
	GetDlgItem(IDC_STATIC_ALIGN_L_LB)->SetWindowText(sVal);
	sVal.Format(_T("%d , %d"), pAlignX[2], pAlignY[2]);
	GetDlgItem(IDC_STATIC_ALIGN_L_RB)->SetWindowText(sVal);
	sVal.Format(_T("%d , %d"), pAlignX[3], pAlignY[3]);
	GetDlgItem(IDC_STATIC_ALIGN_L_RT)->SetWindowText(sVal);
}

void CDlgMenu03::DispAlignRangeR(int* pAlignX, int* pAlignY)
{
	CString sVal;

	sVal.Format(_T("%d , %d"), pAlignX[0], pAlignY[0]);
	GetDlgItem(IDC_STATIC_ALIGN_R_LT)->SetWindowText(sVal);
	sVal.Format(_T("%d , %d"), pAlignX[1], pAlignY[1]);
	GetDlgItem(IDC_STATIC_ALIGN_R_LB)->SetWindowText(sVal);
	sVal.Format(_T("%d , %d"), pAlignX[2], pAlignY[2]);
	GetDlgItem(IDC_STATIC_ALIGN_R_RB)->SetWindowText(sVal);
	sVal.Format(_T("%d , %d"), pAlignX[3], pAlignY[3]);
	GetDlgItem(IDC_STATIC_ALIGN_R_RT)->SetWindowText(sVal);
}
