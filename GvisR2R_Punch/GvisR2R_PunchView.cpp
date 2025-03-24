
// GvisR2R_PunchView.cpp : CGvisR2R_PunchView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GvisR2R_Punch.h"
#endif

#include "GvisR2R_PunchDoc.h"
#include "GvisR2R_PunchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "MainFrm.h"

#include "Dialog/DlgOption01.h"
#include "Dialog/DlgUtil04.h"
#include "Dialog/DlgUtil07.h"
#include "Dialog/DlgMyPassword.h"
#include "Dialog/DlgProgress.h"
#include "Dialog/DlgMyMsgSub00.h"
#include "Dialog/DlgMyMsgSub01.h"
#include "Dialog/DlgMyMsgSub02.h"

#include "Device/SR1000WDEF.h"
#include "Process/DataFile.h"

#include "Dialog/DlgCammasterinfoMst.h"
#include "Dialog/DlgCammasterinfoPch.h"
#include "Dialog/DlgCammasterinfoStrpcs.h"

extern CMainFrame* pFrm;
extern CGvisR2R_PunchDoc* pDoc;
CGvisR2R_PunchView* pView;


// CGvisR2R_PunchView

IMPLEMENT_DYNCREATE(CGvisR2R_PunchView, CFormView)

BEGIN_MESSAGE_MAP(CGvisR2R_PunchView, CFormView)
	ON_WM_TIMER()
	ON_MESSAGE(WM_DLG_INFO, OnDlgInfo)
	ON_MESSAGE(WM_BUF_THREAD_DONE, OnBufThreadDone)
	ON_MESSAGE(WM_MYMSG_EXIT, OnMyMsgExit)
	ON_MESSAGE(WM_CLIENT_RECEIVED_SR, wmClientReceivedSr)
	ON_MESSAGE(WM_CLIENT_RECEIVED, wmClientReceived)
END_MESSAGE_MAP()

// CGvisR2R_PunchView 생성/소멸

CGvisR2R_PunchView::CGvisR2R_PunchView()
	: CFormView(IDD_GVISR2R_PUNCH_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.
	pView = this;

	m_sMsg = _T("");
	m_nAoiUpAutoSerial = 0;
	m_nAoiUpAutoSerialPrev = 0;
	m_nAoiDnAutoSerial = 0;
	m_nAoiDnAutoSerialPrev = 0;

	m_dMkTorqList[0] = 0.0;
	m_dMkTorqList[1] = 0.0;

	int nMsgID = 0, i;
	for (nMsgID = 0; nMsgID < _SigInx::_EndIdx; nMsgID++)
	{
		m_bRcvSig[nMsgID] = FALSE;
	}

	m_sAoiUpAlarmReStartMsg = _T(""); m_sAoiDnAlarmReStartMsg = _T("");
	m_sAoiUpAlarmReTestMsg = _T(""); m_sAoiDnAlarmReTestMsg = _T("");

	m_bShift2Mk = FALSE;

	m_bBufEmpty[0] = FALSE;
	m_bBufEmpty[1] = FALSE;

	m_nDebugStep = 0;
	m_nWatiDispMain = 0;
	m_nNewLot = 0;

	m_bStopFromThread = FALSE;
	m_bBuzzerFromThread = FALSE;

	m_nRtnMyMsgBoxIdx = -1;
	m_bDispMyMsgBox = FALSE;
	m_bDispMain = FALSE;
	m_bProbDn[0] = m_bProbDn[1] = FALSE;

	m_nSelRmap = RMAP_UP;
	m_nSelRmapInner = RMAP_INNER_UP;

	m_bTIM_INIT_VIEW = FALSE;
	m_nStepInitView = 0;
	m_nLotEndSerial = 0;

	m_nDummy[0] = 0;
	m_nDummy[1] = 0;
	m_nAoiLastSerial[0] = 0;
	m_nAoiLastSerial[1] = 0;
	m_bChkLastProcVs = FALSE;

	m_dTempPauseLen = 0.0;
	m_dElecChkVal = 0.0;

	m_bInit = FALSE;
	m_bDispMsg = FALSE;
	for (i = 0; i < 10; i++)
	{
		m_bDispMsgDoAuto[i] = FALSE;
		m_nStepDispMsg[i] = 0;
	}
	m_sFixMsg[0] = _T("");
	m_sFixMsg[1] = _T("");

	m_bWaitClrDispMsg = FALSE;

	m_bStopFeeding = FALSE;

	m_pDlgMyMsg = NULL;
	m_pDlgMsgBox = NULL;

	m_pDlgInfo = NULL;
	m_pDlgFrameHigh = NULL;
	m_pDlgMenu01 = NULL;
	m_pDlgMenu02 = NULL;
	m_pDlgMenu03 = NULL;
	m_pDlgMenu04 = NULL;
	m_pDlgMenu05 = NULL;
	m_pDlgMenu06 = NULL;
	//m_pDlgUtil01 = NULL;
	//m_pDlgUtil02 = NULL;
	//m_pDlgUtil03 = NULL;

	m_nCntTowerWinker = 0;
	m_bTimTowerWinker = FALSE;
	m_bTimBtnWinker = FALSE;
	m_bTimBuzzerWarn = FALSE;
	m_bTowerWinker[0] = FALSE; // R
	m_bTowerWinker[1] = FALSE; // G
	m_bTowerWinker[2] = FALSE; // Y
	m_bBtnWinker[0] = FALSE; // Ready
	m_bBtnWinker[1] = FALSE; // Reset
	m_bBtnWinker[2] = FALSE; // Run
	m_bBtnWinker[3] = FALSE; // Stop
	for (i = 0; i < 4; i++)
	{
		m_nCntBtnWinker[i] = 0;
		m_nDlyWinker[i] = 5;
	}

	m_bAoiFdWriteF[0] = FALSE;
	m_bAoiFdWriteF[1] = FALSE;
	m_bAoiTest[0] = FALSE;
	m_bAoiTest[1] = FALSE;

	m_bEngFdWrite = FALSE; 
	m_bEngFdWriteF = FALSE;
	m_bEngTest = FALSE;
	m_bEngTestF = FALSE;
	m_bMkFdWrite = FALSE;
	m_bMkFdWriteF = FALSE;


	// H/W Device 초기화.....
	m_pMpe = NULL;
	m_pMotion = NULL;
	m_pVoiceCoil[0] = NULL;
	m_pVoiceCoil[1] = NULL;
	m_pLight = NULL;
#ifdef USE_FLUCK
	m_pFluck = NULL;
#endif

#ifdef USE_VISION
	m_pVision[0] = NULL;
	m_pVision[1] = NULL;
	m_pVisionInner[0] = NULL;
	m_pVisionInner[1] = NULL;
#endif

	m_bTIM_DISP_STATUS = FALSE;

	m_bThread[0] = FALSE;
	m_dwThreadTick[0] = 0;
	m_bThread[1] = FALSE;
	m_dwThreadTick[1] = 0;
	m_bThread[2] = FALSE;
	m_dwThreadTick[2] = 0;
	m_bThread[3] = FALSE;
	m_dwThreadTick[3] = 0;
	m_bThread[4] = FALSE;
	m_dwThreadTick[4] = 0;
	m_bThread[5] = FALSE;
	m_dwThreadTick[5] = 0;
	m_bThread[6] = FALSE;
	m_dwThreadTick[6] = 0;
	m_bThread[7] = FALSE;
	m_dwThreadTick[7] = 0;
	m_bThread[8] = FALSE;
	m_dwThreadTick[8] = 0;
	m_bThread[9] = FALSE;
	m_dwThreadTick[9] = 0;
	m_bThread[10] = FALSE;
	m_dwThreadTick[10] = 0;
	m_bThread[11] = FALSE;
	m_dwThreadTick[11] = 0;
	m_bThread[12] = FALSE;
	m_dwThreadTick[12] = 0;
	m_bThread[13] = FALSE;
	m_dwThreadTick[13] = 0;
	m_bThread[14] = FALSE;
	m_dwThreadTick[14] = 0;
	m_bThread[15] = FALSE;
	m_dwThreadTick[15] = 0;
	m_bThread[16] = FALSE;
	m_dwThreadTick[16] = 0;
	m_bThread[17] = FALSE;
	m_dwThreadTick[17] = 0;
	m_bThread[18] = FALSE;
	m_dwThreadTick[18] = 0;
	m_bThread[19] = FALSE;
	m_dwThreadTick[19] = 0;
	m_bThread[20] = FALSE;
	m_dwThreadTick[20] = 0;
	m_bThread[21] = FALSE;
	m_dwThreadTick[21] = 0;
	m_bThread[22] = FALSE;
	m_dwThreadTick[22] = 0;
	m_bThread[23] = FALSE;
	m_dwThreadTick[23] = 0;
	m_bThread[24] = FALSE;
	m_dwThreadTick[24] = 0;
	m_bThread[25] = FALSE;
	m_dwThreadTick[25] = 0;
	m_bThread[26] = FALSE;
	m_dwThreadTick[26] = 0;
	m_bThread[27] = FALSE;
	m_dwThreadTick[27] = 0;
	m_bThread[28] = FALSE;
	m_dwThreadTick[28] = 0;
	m_bThread[29] = FALSE;
	m_dwThreadTick[29] = 0;
	m_bThread[30] = FALSE;
	m_dwThreadTick[30] = 0;
	m_bThread[31] = FALSE;
	m_dwThreadTick[31] = 0;
	m_bThread[32] = FALSE;
	m_dwThreadTick[32] = 0;
	m_bThread[33] = FALSE;
	m_dwThreadTick[33] = 0;
	m_bThread[34] = FALSE;
	m_dwThreadTick[34] = 0;
	m_bThread[35] = FALSE;
	m_dwThreadTick[35] = 0;
	m_bThread[36] = FALSE;
	m_dwThreadTick[36] = 0;
	m_bThread[37] = FALSE;
	m_dwThreadTick[37] = 0;
	m_bThread[38] = FALSE;
	m_dwThreadTick[38] = 0;
	m_bThread[39] = FALSE;
	m_dwThreadTick[39] = 0;
	m_bThread[40] = FALSE;
	m_dwThreadTick[40] = 0;
	m_bThread[41] = FALSE;
	m_dwThreadTick[41] = 0;
	m_bThread[42] = FALSE;
	m_dwThreadTick[42] = 0;
	m_bThread[43] = FALSE;
	m_dwThreadTick[43] = 0;
	m_bThread[44] = FALSE;
	m_dwThreadTick[44] = 0;
	m_bThread[45] = FALSE;
	m_dwThreadTick[45] = 0;
	m_bThread[46] = FALSE;
	m_dwThreadTick[46] = 0;
	m_bThread[47] = FALSE;
	m_dwThreadTick[47] = 0;
	m_bThread[48] = FALSE;
	m_dwThreadTick[48] = 0;

	m_bTHREAD_UPDATE_REELMAP_UP_OFFLINE = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ALLUP_OFFLINE = FALSE;
	m_bTHREAD_UPDATE_REELMAP_DN_OFFLINE = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ALLDN_OFFLINE = FALSE;

	m_bTHREAD_LOAD_PCR_UP_OFFLINE = FALSE;
	m_bTHREAD_LOAD_PCR_ALLUP_OFFLINE = FALSE;
	m_bTHREAD_LOAD_PCR_DN_OFFLINE = FALSE;
	m_bTHREAD_LOAD_PCR_ALLDN_OFFLINE = FALSE;

	m_bTHREAD_WRITE_LAST_INFO_REELMAP_UP_OFFLINE = FALSE;
	m_bTHREAD_WRITE_LAST_INFO_REELMAP_DN_OFFLINE = FALSE;
	m_bTHREAD_WRITE_LAST_INFO_REELMAP_ALLUP_OFFLINE = FALSE;
	m_bTHREAD_WRITE_LAST_INFO_REELMAP_ALLDN_OFFLINE = FALSE;
	

	m_bTIM_MPE_IO = FALSE;


	m_nStepAuto = 0;
	m_nPrevStepAuto = 0;
	m_nPrevMkStAuto = 0;
	m_nStepMk[0] = 0;
	m_nStepMk[1] = 0;

	m_sShare[0] = _T("");
	m_sBuf[0] = _T("");
	m_sShare[1] = _T("");
	m_sBuf[1] = _T("");

	m_sTick = _T("");
	m_sDispTime = _T("");

	m_bChkMpeIoOut = FALSE;

	m_bMkTmpStop = FALSE;
	m_bAoiLdRun = TRUE;
	m_bAoiLdRunF = FALSE;

	m_dwCycSt = 0;
	m_dwCycTim = 0;

	m_bTHREAD_DISP_DEF = FALSE;
	m_nStepTHREAD_DISP_DEF = 0;
	m_bTHREAD_DISP_DEF_INNER = FALSE;
	m_nStepTHREAD_DISP_DEF_INNER = 0;

	m_bTHREAD_UPDATAE_YIELD[0] = FALSE;
	m_bTHREAD_UPDATAE_YIELD[1] = FALSE;
	m_nSerialTHREAD_UPDATAE_YIELD[0] = 0;
	m_nSerialTHREAD_UPDATAE_YIELD[1] = 0;
	m_bTHREAD_SHIFT2MK = FALSE;
	m_bTHREAD_UPDATE_REELMAP_UP = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ALLUP = FALSE;
	m_bTHREAD_UPDATE_REELMAP_DN = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ALLDN = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ITS = FALSE;
	m_bTHREAD_MAKE_ITS_FILE_UP = FALSE;
	m_bTHREAD_MAKE_ITS_FILE_DN = FALSE;
	m_bTHREAD_UPDATE_REELMAP_INNER_UP = FALSE;
	m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP = FALSE;
	m_bTHREAD_UPDATE_REELMAP_INNER_DN = FALSE;
	m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN = FALSE;


	m_nSnTHREAD_UPDATAE_YIELD = 0;
	m_nSnTHREAD_UPDATAE_YIELD_ITS = 0;

	m_bTHREAD_REELMAP_YIELD_UP = FALSE;
	m_bTHREAD_REELMAP_YIELD_ALLUP = FALSE;
	m_bTHREAD_REELMAP_YIELD_DN = FALSE;
	m_bTHREAD_REELMAP_YIELD_ALLDN = FALSE;
	m_bTHREAD_REELMAP_YIELD_ITS = FALSE;

	m_bTHREAD_UPDATE_YIELD_UP = FALSE;
	m_bTHREAD_UPDATE_YIELD_ALLUP = FALSE;
	m_bTHREAD_UPDATE_YIELD_DN = FALSE;
	m_bTHREAD_UPDATE_YIELD_ALLDN = FALSE;
	m_bTHREAD_UPDATE_YIELD_ITS = FALSE;

	m_bTHREAD_UPDATE_YIELD_INNER_UP = FALSE;
	m_bTHREAD_UPDATE_YIELD_INNER_ALLUP = FALSE;
	m_bTHREAD_UPDATE_YIELD_INNER_DN = FALSE;
	m_bTHREAD_UPDATE_YIELD_INNER_ALLDN = FALSE;

	m_bTHREAD_RELOAD_RST_UP = FALSE;
	m_bTHREAD_RELOAD_RST_ALLUP = FALSE;
	m_bTHREAD_RELOAD_RST_DN = FALSE;
	m_bTHREAD_RELOAD_RST_ALLDN = FALSE;
	m_bTHREAD_RELOAD_RST_ITS = FALSE;
	m_bTHREAD_RELOAD_RST_UP_INNER = FALSE;
	m_bTHREAD_RELOAD_RST_ALLUP_INNER = FALSE;
	m_bTHREAD_RELOAD_RST_DN_INNER = FALSE;
	m_bTHREAD_RELOAD_RST_ALLDN_INNER = FALSE;

	m_bTHREAD_MK[0] = FALSE;
	m_bTHREAD_MK[1] = FALSE;
	m_bTHREAD_MK[2] = FALSE;
	m_bTHREAD_MK[3] = FALSE;
	m_nMkPcs[0] = 0;
	m_nMkPcs[1] = 0;
	m_nMkPcs[2] = 0;
	m_nMkPcs[3] = 0;
	m_nMarkingOrder[0] = 0;
	m_nMarkingOrder[1] = 0;

	m_nErrCnt = 0;

	m_bAuto = FALSE;
	m_bManual = FALSE;
	//m_bOneCycle = FALSE;

	m_bSwRun = FALSE; m_bSwRunF = FALSE;
	m_bSwStop = FALSE; m_bSwStopF = FALSE;
	m_bSwReset = FALSE; m_bSwResetF = FALSE;
	m_bSwReady = FALSE; m_bSwReadyF = FALSE;

	m_bSwStopNow = FALSE;

	for (int nAxis = 0; nAxis < MAX_AXIS; nAxis++)
		m_dEnc[nAxis] = 0.0;

	for (int i = 0; i < 10; i++)
		m_sDispMsg[i] = _T("");

	m_bNewModel = FALSE;
	m_dTotVel = 0.0; m_dPartVel = 0.0;
	m_bTIM_CHK_TEMP_STOP = FALSE;
	m_bTIM_SAFTY_STOP = FALSE;
	m_bTIM_CAMMASTER_UPDATE = FALSE;
	m_bTIM_CHK_RCV_CURR_INFO_SIG = FALSE;
	m_bTIM_CHK_RCV_MON_DISP_MAIN_SIG = FALSE;
	m_sMyMsg = _T("");
	m_nTypeMyMsg = IDOK;

	m_dwLotSt = 0; m_dwLotEd = 0;

	m_lFuncId = 0;

	m_bDrawGL = TRUE;
	m_bDrawGL_Menu01 = TRUE;
	m_bDrawGL_Menu06 = TRUE;
	m_bCont = FALSE;
	m_bCam = FALSE;
	m_bReview = FALSE;

	m_bChkBufIdx[0] = TRUE;
	m_nChkBufIdx[0] = 0;
	m_bChkBufIdx[1] = TRUE;
	m_nChkBufIdx[1] = 0;

	m_dwStMkDn[0] = 0;
	m_dwStMkDn[1] = 0;
	m_nVsBufLastSerial[0] = 0;
	m_nVsBufLastSerial[1] = 0;
	m_bShowModalMyPassword = FALSE;

	m_nRstNum = 0;
	m_bBufHomeDone = FALSE;
	m_bReadyDone = FALSE;

	ResetPriority();
	m_Flag = 0L;
	m_AoiLdRun = 0L;

	m_bCollision[0] = FALSE;
	m_bCollision[1] = FALSE;

	m_bDoneDispMkInfo[0][0] = FALSE; // Cam0, Up
	m_bDoneDispMkInfo[0][1] = FALSE; // Cam0, Dn
	m_bDoneDispMkInfo[1][0] = FALSE; // Cam1, Up
	m_bDoneDispMkInfo[1][1] = FALSE; // Cam1, Dn

	m_nShareUpS = 0; m_nShareUpSprev = 0;
	m_nShareUpSerial[0] = 0;
	m_nShareUpSerial[1] = 0;
	m_nShareUpCnt = 0;

	m_nShareDnS = 0; m_nShareDnSprev = 0;
	m_nShareDnSerial[0] = 0;
	m_nShareDnSerial[1] = 0;
	m_nShareDnCnt = 0;

	m_nBufSerial[0][0] = 0; // Up-Cam0
	m_nBufSerial[0][1] = 0; // Up-Cam1
	m_nBufSerial[1][0] = 0; // Dn-Cam0
	m_nBufSerial[1][1] = 0; // Dn-Cam0

	m_bReAlign[0][0] = FALSE; // [nCam][nPos] 
	m_bReAlign[0][1] = FALSE; // [nCam][nPos] 
	m_bReAlign[0][2] = FALSE; // [nCam][nPos] 
	m_bReAlign[0][3] = FALSE; // [nCam][nPos] 
	m_bReAlign[1][0] = FALSE; // [nCam][nPos] 
	m_bReAlign[1][1] = FALSE; // [nCam][nPos] 
	m_bReAlign[1][2] = FALSE; // [nCam][nPos] 
	m_bReAlign[1][3] = FALSE; // [nCam][nPos] 

	m_bSkipAlign[0][0] = FALSE; // [nCam][nPos] 
	m_bSkipAlign[0][1] = FALSE; // [nCam][nPos] 
	m_bSkipAlign[0][2] = FALSE; // [nCam][nPos] 
	m_bSkipAlign[0][3] = FALSE; // [nCam][nPos] 
	m_bSkipAlign[1][0] = FALSE; // [nCam][nPos] 
	m_bSkipAlign[1][1] = FALSE; // [nCam][nPos] 
	m_bSkipAlign[1][2] = FALSE; // [nCam][nPos] 
	m_bSkipAlign[1][3] = FALSE; // [nCam][nPos] 

	m_bFailAlign[0][0] = FALSE; // [nCam][nPos] 
	m_bFailAlign[0][1] = FALSE; // [nCam][nPos] 
	m_bFailAlign[0][2] = FALSE; // [nCam][nPos] 
	m_bFailAlign[0][3] = FALSE; // [nCam][nPos] 
	m_bFailAlign[1][0] = FALSE; // [nCam][nPos] 
	m_bFailAlign[1][1] = FALSE; // [nCam][nPos] 
	m_bFailAlign[1][2] = FALSE; // [nCam][nPos] 
	m_bFailAlign[1][3] = FALSE; // [nCam][nPos] 

	m_bDoMk[0] = TRUE;
	m_bDoMk[1] = TRUE;
	m_bDoneMk[0] = FALSE;
	m_bDoneMk[1] = FALSE;
	m_bReMark[0] = FALSE;
	m_bReMark[1] = FALSE;

	m_nMonAlmF = 0;
	m_nClrAlmF = 0;

	m_bMkSt[0] = FALSE;
	m_bMkSt[0] = FALSE;
	m_bMkStSw[1] = FALSE;
	m_bMkStSw[1] = FALSE;
	m_nMkStAuto = 0;

	m_bEngSt = FALSE;
	m_bEngStSw = FALSE;
	m_nEngStAuto = 0;

	m_bEng2dSt = FALSE;
	m_bEng2dStSw = FALSE;
	m_nEng2dStAuto = 0;

	m_bJoinContinue = FALSE;
	m_bLotEnd = FALSE;
	m_bLotEndF = FALSE;
	m_nLotEndAuto = 0;

	m_bLastProc = FALSE;
	m_bLastProcFromUp = TRUE;
	m_bLastProcFromEng = TRUE;
	m_nLastProcAuto = 0;

	m_bLoadShare[0] = FALSE;
	m_bLoadShare[1] = FALSE;

	m_sNewLotUp = _T("");
	m_sNewLotDn = _T("");

	m_bAoiFdWrite[0] = FALSE;
	m_bAoiFdWrite[1] = FALSE;
	m_bAoiFdWriteF[0] = FALSE;
	m_bAoiFdWriteF[1] = FALSE;

	m_bCycleStop = FALSE;

	for (int a = 0; a < 2; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			m_nMkStrip[a][b] = 0;
			m_bRejectDone[a][b] = FALSE;
		}
	}

	m_sDispMain = _T("");

	m_bReMk = FALSE;

	m_bWaitPcr[0] = FALSE;
	m_bWaitPcr[1] = FALSE;

	m_bShowMyMsg = FALSE;
	m_pMyMsgForeground = NULL;
	m_bContDiffLot = FALSE;

	m_nStepElecChk = 0;

	for (int nAns = 0; nAns < 10; nAns++)
		m_bAnswer[nAns] = FALSE;

	m_bChkLightErr = FALSE;

	// client for SR-1000W
	m_pSr1000w = NULL;

	// client for engrave
	m_pEngrave = NULL;
	m_pDts = NULL;

	m_bDestroyedView = FALSE;
	m_bContEngraveF = FALSE;

	m_nSaveMk0Img = 0;
	m_nSaveMk1Img = 0;

	m_bStopF_Verify = FALSE;
	m_bInitAuto = TRUE;
	m_bInitAutoLoadMstInfo = FALSE;

	m_bLoadMstInfo = FALSE;
	m_bLoadMstInfoF = FALSE;

	m_sGet2dCodeLot = _T("");
	m_nGet2dCodeSerial = 0;
	m_nReloadReelmapSerial = 0;

	m_bSetSig = FALSE;
	m_bSetSigF = FALSE;
	m_bSetData = FALSE;
	m_bSetDataF = FALSE;

	m_bTIM_START_UPDATE = FALSE;
	m_bEscape = FALSE;
	m_bSerialDecrese = FALSE;

	pView->m_bIsBuf[0] = FALSE;
	pView->m_bIsBuf[1] = FALSE;

	m_bUpdateYield = FALSE;
	m_bUpdateYieldOnRmap = FALSE;

	m_bPcrInShare[0] = FALSE;
	m_bPcrInShare[1] = FALSE;
	m_bPcrInShareVs[0] = FALSE;
	m_bPcrInShareVs[1] = FALSE;
}

CGvisR2R_PunchView::~CGvisR2R_PunchView()
{
	//DestroyView();

#ifdef USE_VISION
	if (m_pVisionInner[1])
	{
		delete m_pVisionInner[1];
		m_pVisionInner[1] = NULL;
	}

	if (m_pVisionInner[0])
	{
		delete m_pVisionInner[0];
		m_pVisionInner[0] = NULL;
	}
	if (m_pVision[1])
	{
		delete m_pVision[1];
		m_pVision[1] = NULL;
	}

	if (m_pVision[0])
	{
		delete m_pVision[0];
		m_pVision[0] = NULL;
	}
#endif

	m_bTIM_MPE_IO = FALSE;
	m_bTIM_DISP_STATUS = FALSE;
	m_bTIM_INIT_VIEW = FALSE;
	Sleep(100);


	InitIoWrite();
	SetMainMc(FALSE);

	// H/W Device 소멸.....
	HwKill();

	CloseMyMsg();

	if (m_ArrayMyMsgBox.GetSize() > 0)
	{
		m_ArrayMyMsgBox.RemoveAll();
	}

}

void CGvisR2R_PunchView::DestroyView()
{
	CString sData;
	sData.Format(_T("%d"), m_nMkStAuto);
	::WritePrivateProfileString(_T("Last Job"), _T("MkStAuto"), sData, PATH_WORKING_INFO);

	if (!m_bDestroyedView)
	{
		m_bDestroyedView = TRUE;

		ThreadKill();
		Sleep(30);

		DelAllDlg();
		Sleep(100);

		Buzzer(FALSE, 0);
		Buzzer(FALSE, 1);
	}
}

void CGvisR2R_PunchView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CGvisR2R_PunchView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CGvisR2R_PunchView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	InitMyMsg();

	pDoc->LoadWorkingInfo();
	pDoc->LoadIoInfo();
	pDoc->LoadSignalInfo();
	pDoc->LoadDataInfo();
	if (!LoadMySpec())
		LoadMySpec();
	pDoc->GetMkInfo();

#ifdef USE_CAM_MASTER
	CFileFind finder;
	CString sDir, sMsg;
	sDir = pDoc->WorkingInfo.System.sPathCamSpecDir;
	sDir.Delete(sDir.GetLength() - 1, 1);
	sDir.ReleaseBuffer();

	if(!pDoc->DirectoryExists(sDir))
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("캠마스터에 스펙폴더가 없습니다. : \n 1.SpecFolder : %s"), sDir);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg, MB_ICONSTOP | MB_OK);
		ExitProgram();
		return;
	}

	if (!pDoc->CheckPathCamMstSpec())
	{
		ExitProgram();
		return;
	}
	sDir = pDoc->WorkingInfo.System.sPathTorqSpecDir;
	sDir.Delete(sDir.GetLength() - 1, 1);
	sDir.ReleaseBuffer();

	if (!pDoc->DirectoryExists(sDir))
	{
		sMsg.Format(_T("캠마스터에 토크리스트 스펙폴더가 없습니다. : \n 1.Torque SpecFolder : %s"), sDir);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg, MB_ICONSTOP | MB_OK);
	}

#endif

	if (!m_bTIM_INIT_VIEW)
	{
		m_nStepInitView = 0;
		m_bTIM_INIT_VIEW = TRUE;
		SetTimer(TIM_INIT_VIEW, 300, NULL);
	}

	Plc.Load();
	pDoc->SetMonDispMain(_T(""));
}


// CGvisR2R_PunchView 진단

#ifdef _DEBUG
void CGvisR2R_PunchView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGvisR2R_PunchView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGvisR2R_PunchDoc* CGvisR2R_PunchView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGvisR2R_PunchDoc)));
	return (CGvisR2R_PunchDoc*)m_pDocument;
}
#endif //_DEBUG


// CGvisR2R_PunchView 메시지 처리기


void CGvisR2R_PunchView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nInc = 0, nSrl = 0;
	CString str, sMsg, sPath, sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner;
	BOOL bExist = FALSE; 
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bRtn = FALSE;
	BOOL bTempStop = FALSE;
	int nLastShot;

	if (nIDEvent == TIM_INIT_VIEW)	
	{
		KillTimer(TIM_INIT_VIEW);

		switch (m_nStepInitView)
		{
		case 0:
			m_nStepInitView++;
			DispMsg(_T("프로그램을 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			DtsInit();

			// H/W Device 초기화.....
			HwInit();

			break;
		case 1:
			m_nStepInitView++;
			InitIO();
			break;
		case 2:
			m_nStepInitView++;
			InitIoWrite();
			SetMainMc(TRUE);
			m_nMonAlmF = 0;
			m_nClrAlmF = 0;
			break;
		case 3:
			m_nStepInitView++;
			ThreadInit();
			break;
		case 4:
			pDoc->GetCamPxlRes();
			m_nStepInitView++;
			break;
		case 5:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- MENU_02"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_02);
 			//ShowDlg(IDD_DLG_UTIL_01);
			break;
		case 6:
			m_nStepInitView++;
 			//ShowDlg(IDD_DLG_UTIL_02);
			break;
		case 7:
			m_nStepInitView++;
			//DispMsg(_T("마킹위치데이터 화면구성을 생성합니다.- 2"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			//ShowDlg(IDD_DLG_UTIL_03);
			break;
		case 8:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.-MENU_01"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_01);
				if (bDualTest)
					pView->m_pDlgMenu01->SelMap(ALL);
				else
					pView->m_pDlgMenu01->SelMap(UP);
			break;
		case 9:
			m_nStepInitView++;
			//DispMsg(_T("화면구성을 생성합니다.- 3"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
 			//ShowDlg(IDD_DLG_MENU_02);
			break;
		case 10:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- MENU_03"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_03);
			break;
		case 11:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- MENU_04"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_04);
			break;
		case 12:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- MENU_05"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_05);
			break;
		case 13:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- MENU_06"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_MENU_06);
			break;
		case 14:
			m_nStepInitView++;
			DispMsg(_T("화면구성을 생성합니다.- FRAME_HIGH"), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			ShowDlg(IDD_DLG_FRAME_HIGH);
			if (m_pDlgFrameHigh)
				m_pDlgFrameHigh->ChkMenu01();
			SetDualTest(pDoc->WorkingInfo.LastJob.bDualTest);

			if (pDoc->GetCurrentInfoEng())
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
					{
						if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->m_sEngModel, sLayerUp))
						{
							if (m_pDlgMenu06)
								m_pDlgMenu06->RedrawWindow();

							pDoc->MakeDirRmap(RMAP_INNER_UP);
							pDoc->MakeDirRmap(RMAP_INNER_DN);
							pDoc->CopyReelmapInner(m_nShareUpS);
						}
					}
				}
			}
			Init();
			Sleep(300);
			break;
		case 15:
			m_nStepInitView++;
			m_bLoadMstInfo = TRUE; pDoc->SetStatus(_T("General"), _T("bLoadMstInfo"), m_bLoadMstInfo);
			DispMsg(_T("H/W를 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
			InitAct();
			m_bStopFeeding = TRUE;
			MpeWrite(Plc.DlgMenu01.ProhibitFeedingPunch, 1); // 마킹부Feeding금지
			Sleep(100);
			ClampOn(); // 마킹부 테이블 클램프 하강 스위치
			break;
		case 16:
			if (IsClampOff())
			{
				break;
			}

			if (m_pMotion)
			{
				MpeWrite(_T("MB40015F"), 1); // Motion Busy
				DispMsg(_T("Homming"), _T("Searching Home Position..."), RGB_GREEN, 2000, TRUE);
				m_pMotion->SearchHome();
				m_nStepInitView++;
			}
			else
			{
				m_bTIM_INIT_VIEW = FALSE;
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Motion is failed."));
				PostMessage(WM_CLOSE);
			}
			break;
		case 17:
			if (m_pMotion)
			{
				if (m_pMotion->IsHomeDone())// && m_pMotion->IsHomeDone(MS_MKFD))
				{
					MpeWrite(_T("MB40015F"), 0); // Motion Busy
					m_nStepInitView++;
				}

				sMsg.Format(_T("X0(%s) , Y0(%s)\r\nX1(%s) , Y1(%s)"), m_pMotion->IsHomeDone(MS_X0) ? _T("Done") : _T("Doing"),
					m_pMotion->IsHomeDone(MS_Y0) ? _T("Done") : _T("Doing"),
					m_pMotion->IsHomeDone(MS_X1) ? _T("Done") : _T("Doing"),
					m_pMotion->IsHomeDone(MS_Y1) ? _T("Done") : _T("Doing"));

				if (m_sMsg != sMsg)
				{
					m_sMsg = sMsg;
					DispMsg(sMsg, _T("Homming"), RGB_GREEN, 2000, TRUE);
					Sleep(300);
				}
			}
			else
			{
				m_bTIM_INIT_VIEW = FALSE;
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Motion is failed."));
				PostMessage(WM_CLOSE);
			}
			break;
		case 18:
			m_sMsg = _T("");
			m_nStepInitView++;
			break;
		case 19:
			m_nStepInitView++;
			DispMsg(_T("Completed Searching Home Pos..."), _T("Homming"), RGB_GREEN, 2000, TRUE);
			m_pMotion->ObjectMapping();
			m_bEscape = FALSE;

			if (m_pVoiceCoil[0])
			{
				m_pVoiceCoil[0]->SetMarkFinalData(0);
				//m_pVoiceCoil[0]->SetProbFinalData(0);
			}

			if (m_pVoiceCoil[1])
			{
				m_pVoiceCoil[1]->SetMarkFinalData(1);
				//m_pVoiceCoil[1]->SetProbFinalData(1);
			}

			break;
		case 20:
			m_bStopFeeding = FALSE;
			MpeWrite(Plc.DlgMenu01.ProhibitFeedingPunch, 0); // 마킹부Feeding금지
			m_nStepInitView++;
			if(m_pDlgMenu02)
				m_pDlgMenu02->SetJogSpd(_tstoi(pDoc->WorkingInfo.LastJob.sJogSpd));
			m_pMotion->SetR2RConf();
			TowerLamp(RGB_YELLOW, TRUE);

			if (!SetCollision(-1.0*_tstof(pDoc->WorkingInfo.Motion.sCollisionLength)))
			{
				DispMsg(_T("Collision"), _T("Failed to Set Collision ..."), RGB_GREEN, 2000, TRUE);
			}

			break;
		case 21:
			m_nStepInitView++;
			//MoveMkInitPos();
			InitPLC();
			SetPlcParam();
			GetPlcParam();
			TcpIpInit();
			DtsInit();

			InitVoiceCoil(0);

			m_bTIM_DISP_STATUS = TRUE;
			SetTimer(TIM_DISP_STATUS, 100, NULL);

			m_bTIM_START_UPDATE = TRUE;
			SetTimer(TIM_START_UPDATE, 500, NULL);
			break;
		case 22:
			if (m_bLoadMstInfo || m_bLoadMstInfoF) // CamMaster의 정보 Loading 완료.
				break;

			m_nStepInitView++;
			ClrDispMsg();

			bRtn = LoadPcrFromBuf();

			if (m_pDlgMenu01)
			{
				m_pDlgMenu01->SetStripAllMk();
				m_pDlgMenu01->RefreshRmap();
				m_pDlgMenu01->ResetLastProc();
			}

			StartThread();

			if (!MemChk())
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Memory Error - Cam Spec Data : PCR[0] or PCR[1] or Reelmap"));
			}
			else
			{
				if (pDoc->GetCurrentInfoEng())
				{
					if (pDoc->GetTestMode() == MODE_OUTER)
					{
						if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 3))
						{
							if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->m_sEngModel, sLayerUp))
							{
								if (m_pDlgMenu06)
								{
									m_pDlgMenu06->SetStripAllMk();
									m_pDlgMenu06->RefreshRmap();
									m_pDlgMenu06->ResetLastProc();
								}

								if (pDoc->m_pReelMapInner)
								{
									ReloadReelmapInner();
									UpdateRstInner();
								}
							}
						}
					}
				}

			}

			MoveInitPos1();
			Sleep(30);
			MoveInitPos0();

			SetLotLastShot();
			StartLive();

			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (m_pDlgMenu01)
					m_pDlgMenu01->EnableItsMode();
			}

			m_bTIM_MPE_IO = TRUE;
			SetTimer(TIM_MPE_IO, 50, NULL);

			if(m_bMkSt[0] || m_bMkSt[1])
				LoadSerial();

			pDoc->LoadStatus();

			SetMkReStart();
			InitVoiceCoil(1);

			if (pDoc->m_pReelMap) //&& bRtn
			{
				//ReloadYield();
				nLastShot = _tstoi(pDoc->WorkingInfo.LastJob.sSerialUp);
				ReloadReelmap(nLastShot);
				UpdateYield(nLastShot);
			}
			break;
		case 23:
			if (!IsDoneReloadReelmap() || !IsDoneReloadYield())
				break;

			m_nStepInitView++;

			if (pDoc->m_pReelMap)
			{
				UpdateRst();
				UpdateLotTime();
			}

			m_bTIM_INIT_VIEW = FALSE;
			break;
		}

		if (m_bTIM_INIT_VIEW)
			SetTimer(TIM_INIT_VIEW, 100, NULL);
	}

	if (nIDEvent == TIM_MPE_IO)
	{
		KillTimer(TIM_MPE_IO);

		CntMk();
		GetMpeIO();
		GetMpeSignal();
		DoIO();
		ChkMyMsg();
		if (m_bTIM_MPE_IO)
			SetTimer(TIM_MPE_IO, 300, NULL);
	}

	if (nIDEvent == TIM_TOWER_WINKER)
	{
		KillTimer(TIM_TOWER_WINKER);
		DispTowerWinker();
		if (m_bTimTowerWinker)
			SetTimer(TIM_TOWER_WINKER, 100, NULL);
	}

	if (nIDEvent == TIM_BUZZER_WARN)
	{
		KillTimer(TIM_BUZZER_WARN);
		m_nCntBz++;
		if (m_nCntBz > BUZZER_DELAY)
		{
			m_bTimBuzzerWarn = FALSE;
			Buzzer(FALSE);
		}
		if (m_bTimBuzzerWarn)
			SetTimer(TIM_BUZZER_WARN, 100, NULL);
	}

	if (nIDEvent == TIM_DISP_STATUS)
	{
		KillTimer(TIM_DISP_STATUS);

		DispStsBar();
		DoDispMain();
		//DispMyMsgBox();

		if (m_bStopFromThread)
		{
			m_bStopFromThread = FALSE;
			Stop();
		}
		if (m_bBuzzerFromThread)
		{
			m_bBuzzerFromThread = FALSE;
			Buzzer(TRUE, 0);
		}


		//		SetMpeIO();

		ChkEmg();
		ChkSaftySen();
		ChkDoor();
		ChkRcvSig();

		if (m_bTIM_DISP_STATUS)
			SetTimer(TIM_DISP_STATUS, 100, NULL);
	}


	if (nIDEvent == TIM_SHOW_MENU01)
	{
		KillTimer(TIM_SHOW_MENU01);
		if (m_pDlgFrameHigh)
			m_pDlgFrameHigh->ChkMenu01();
	}

	if (nIDEvent == TIM_SHOW_MENU02)
	{
		KillTimer(TIM_SHOW_MENU02);
		if (m_pDlgFrameHigh)
			m_pDlgFrameHigh->ChkMenu02();
	}

	if (nIDEvent == TIM_CHK_TEMP_STOP)
	{
		KillTimer(TIM_CHK_TEMP_STOP);
#ifdef USE_MPE
		bTempStop = MpeRead(_T("MB40000B")); // 일시정지사용

		//if (!(pDoc->m_pMpeSignal[20] & (0x01 << 11)))	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
		if (!bTempStop)	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
		{
			pDoc->LogAuto(_T("PC: 일시정지 OFF"));
			m_bTIM_CHK_TEMP_STOP = FALSE;
			m_pDlgMenu01->SetTempStop(FALSE);
		}
#endif
		if (m_bTIM_CHK_TEMP_STOP)
			SetTimer(TIM_CHK_TEMP_STOP, 500, NULL);
	}

	if (nIDEvent == TIM_SAFTY_STOP)
	{
		KillTimer(TIM_SAFTY_STOP);
		MsgBox(_T("일시정지 - 마킹부 안전센서가 감지되었습니다."));
		m_bTIM_SAFTY_STOP = FALSE;
	}

	if (nIDEvent == TIM_START_UPDATE)
	{
		KillTimer(TIM_START_UPDATE);

		if (m_bLoadMstInfo && !m_bLoadMstInfoF)
		{
			if (!pDoc->WorkingInfo.LastJob.sModel.IsEmpty() && !pDoc->WorkingInfo.LastJob.sLayerUp.IsEmpty())
			{
				m_bLoadMstInfoF = TRUE;
				SetTimer(TIM_CAMMASTER_UPDATE, 500, NULL);
			}
			else
			{
				m_bLoadMstInfo = FALSE; pDoc->SetStatus(_T("General"), _T("bLoadMstInfo"), m_bLoadMstInfo);
			}
		}

		if (m_bSetSig && !m_bSetSigF)
		{
			m_bSetSigF = TRUE;

			if (m_pEngrave->m_bGetOpInfo || m_pEngrave->m_bGetInfo)
			{
				if (m_pDlgInfo)
					m_pDlgInfo->UpdateData();

				if (m_pDlgMenu01)
					m_pDlgMenu01->UpdateData();

				m_pEngrave->m_bGetOpInfo = FALSE;
				m_pEngrave->m_bGetInfo = FALSE;
			}
			else
			{
				if (m_pDlgMenu03)
					m_pDlgMenu03->UpdateSignal();
			}

			m_bSetSig = FALSE;
		}
		else if (/*!m_bSetSig && */m_bSetSigF)
		{
			m_bSetSigF = FALSE;
		}

		if (m_bSetData && !m_bSetDataF)
		{
			m_bSetDataF = TRUE;

			if (m_pEngrave->m_bGetOpInfo || m_pEngrave->m_bGetInfo)
			{
				if (m_pDlgInfo)
					m_pDlgInfo->UpdateData();

				if (m_pDlgMenu01)
					m_pDlgMenu01->UpdateData();

				m_pEngrave->m_bGetOpInfo = FALSE;
				m_pEngrave->m_bGetInfo = FALSE;
			}

			if (m_pDlgMenu02)
				m_pDlgMenu02->UpdateData();

			if (m_pDlgMenu03)
				m_pDlgMenu03->UpdateData();

			if (m_pDlgMenu04)
				m_pDlgMenu04->UpdateData();

			m_bSetData = FALSE;
		}
		else if (!m_bSetData && m_bSetDataF)
		{
			m_bSetDataF = FALSE;
		}

		if (m_bTIM_START_UPDATE)
			SetTimer(TIM_START_UPDATE, 100, NULL);
	}

	if (nIDEvent == TIM_CAMMASTER_UPDATE)
	{
		KillTimer(TIM_CAMMASTER_UPDATE);
		if (LoadMstInfo())
		{
			if (m_pDlgMenu01)
				m_pDlgMenu01->UpdateData();
		}
		m_bLoadMstInfoF = FALSE;
		m_bLoadMstInfo = FALSE; pDoc->SetStatus(_T("General"), _T("bLoadMstInfo"), m_bLoadMstInfo);
	}

	if (nIDEvent == TIM_CHK_RCV_CURR_INFO_SIG)
	{
		KillTimer(TIM_CHK_RCV_CURR_INFO_SIG);

		//if (m_bTIM_CHK_RCV_CURR_INFO_SIG)
		//{
		//	if (m_pEngrave)
		//		m_pEngrave->SetCurrentInfoSignal();

		//	SetTimer(TIM_CHK_RCV_CURR_INFO_SIG, 500, NULL);
		//}
	}

	//if (nIDEvent == TIM_CHK_RCV_MON_DISP_MAIN_SIG)
	//{
	//	KillTimer(TIM_CHK_RCV_MON_DISP_MAIN_SIG);

	//	if (m_bTIM_CHK_RCV_MON_DISP_MAIN_SIG)
	//	{
	//		if (m_pEngrave)
	//			m_pEngrave->SetMonDispMainSignal();

	//		SetTimer(TIM_CHK_RCV_MON_DISP_MAIN_SIG, 500, NULL);
	//	}
	//}

	CFormView::OnTimer(nIDEvent);
}


void CGvisR2R_PunchView::InitMyMsg()
{
	if (m_pDlgMyMsg)
		CloseMyMsg();

	m_pDlgMyMsg = new CDlgMyMsg(this);
	m_pDlgMyMsg->Create();
}

void CGvisR2R_PunchView::CloseMyMsg()
{
	if (m_pDlgMyMsg)
	{
		delete m_pDlgMyMsg;
		m_pDlgMyMsg = NULL;
	}
}

LRESULT CGvisR2R_PunchView::OnMyMsgExit(WPARAM wPara, LPARAM lPara)
{
	Buzzer(FALSE, 0);
	ClrAlarm();
	return 0L;
}

int CGvisR2R_PunchView::MsgBox(CString sMsg, int nThreadIdx, int nType, int nTimOut, BOOL bEngave)
{
	int nRtnVal = -1; // Reply(-1) is None.
	pView->ClrDispMsg();
	if (bEngave)
	{
		if (m_pEngrave)
		{
			pDoc->m_sMsgBox = sMsg;
			//if (m_pEngrave)
			//	m_pEngrave->SetMsgBox(pDoc->m_sMsgBox, nType);
		}
	}

	if (m_pDlgMyMsg)
		nRtnVal = m_pDlgMyMsg->SyncMsgBox(sMsg, nThreadIdx, nType, nTimOut);

	return nRtnVal;
}

int CGvisR2R_PunchView::AsyncMsgBox(CString sMsg, int nThreadIdx, int nType, int nTimOut)
{
	int nRtn = -1;
	if (m_pDlgMyMsg)
		m_pDlgMyMsg->AsyncMsgBox(sMsg, nThreadIdx, nType, nTimOut);
	return nRtn;
}

int CGvisR2R_PunchView::WaitRtnVal(int nThreadIdx)
{
	int nRtn = -1;
	if (m_pDlgMyMsg)
		nRtn = m_pDlgMyMsg->WaitRtnVal(nThreadIdx);
	return nRtn;
}



void CGvisR2R_PunchView::ChkMyMsg()
{
	return;

	//HWND hwndForeground;
	CWnd *pWndForeground;

	if (m_bShowMyMsg && m_pMyMsgForeground)
	{
		pWndForeground = pFrm->GetForegroundWindow();
		if (pWndForeground != m_pMyMsgForeground)
			m_pMyMsgForeground->SetForegroundWindow();
	}
}

void CGvisR2R_PunchView::UpdateLotTime()
{
	m_dwLotSt = (DWORD)pDoc->WorkingInfo.Lot.dwStTick;

	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateLotTime();
}

void CGvisR2R_PunchView::DispStsBar(CString sMsg, int nIdx)
{
	if (m_sDispMsg[nIdx] != sMsg)
		m_sDispMsg[nIdx] = sMsg;
	sMsg.Empty();
}

void CGvisR2R_PunchView::DispStsBar()
{
	DispStsMainMsg(); // 0
	//DispStsMainMsg(6); // 6
	//DispStsMainMsg(5); // 5
	//DispThreadTick(); // 5, 6
	DispTime(); // 7
	ChkShare(); // 2, 4
	if (!m_bShift2Mk && !m_bTHREAD_SHIFT2MK)// && (m_nMkStAuto < MK_ST + (Mk2PtIdx::DoneMk) || m_nMkStAuto > MK_ST + (Mk2PtIdx::DoneMk) + 5 || m_bTIM_INIT_VIEW))
	{
		ChkBuf(); // 1, 3
		if (m_bTIM_INIT_VIEW)
			SetListBuf();
	}
}

BOOL CGvisR2R_PunchView::MemChk() // 릴맵화면에 표시할 데이터를 담을 구조체 할당을 확인함.
{
	if (!pDoc->m_pPcr[0] || !pDoc->m_pPcr[1])// || !pDoc->m_pReelMap)
		return FALSE;
	return TRUE;
}

void CGvisR2R_PunchView::SetMainMc(BOOL bOn)
{
	if (bOn)
	{
		MpeWrite(Plc.DlgMenu01.MainMc, 1); // 마킹부 MC ON
		//MpeWrite(Plc.DlgMenu01.MainMc, 1); // 마킹부 Door Interlock ON
	}
	else
	{
		MpeWrite(Plc.DlgMenu01.MainMc, 0); // 마킹부 MC OFF
		//MpeWrite(Plc.DlgMenu01.MainMc, 0); // 마킹부 Door Interlock OFF
	}
}

void CGvisR2R_PunchView::ExitProgram()
{
	long lParam = 0;
	long lData = 1;
	lParam = lParam | lData;
	lData = 0x00 << 16;
	lParam = lParam | lData;
	lData = 1 << 29;
	lParam = lParam | lData;
	AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT);
}

void CGvisR2R_PunchView::Init()
{
	int nAxis;
	if (m_pMotion)
	{
		for (nAxis = 0; nAxis<m_pMotion->m_ParamCtrl.nTotAxis; nAxis++)
		{
			m_pMotion->AmpReset(nAxis);
			Sleep(30);
		}
	}
}

void CGvisR2R_PunchView::InitIO()
{
	int i, k;

#ifdef USE_MPE
	pDoc->m_nMpeIo = pDoc->MkIo.MpeIo.nMaxSeg;
	pDoc->m_nMpeIb = pDoc->MkIo.MpeIo.nMaxSeg;

	if (!pDoc->m_pMpeIb)
	{
		if (pDoc->m_nMpeIb > 0)
		{
			pDoc->m_pMpeIb = new unsigned short[pDoc->m_nMpeIb];
			for (i = 0; i < pDoc->m_nMpeIb; i++)
				pDoc->m_pMpeIb[i] = 0;
		}
	}

	if (!pDoc->m_pMpeIF)
	{
		if (pDoc->m_nMpeIb>0)
		{
			pDoc->m_pMpeIF = new unsigned short[pDoc->m_nMpeIb];
			for (i = 0; i<pDoc->m_nMpeIb; i++)
				pDoc->m_pMpeIF[i] = 0;
		}
	}

	if (!pDoc->m_pMpeIo)
	{
		if (pDoc->m_nMpeIo>0)
		{
			pDoc->m_pMpeIo = new unsigned short[pDoc->m_nMpeIo];
			for (i = 0; i<pDoc->m_nMpeIo; i++)
				pDoc->m_pMpeIo[i] = 0;
		}
	}

	if (!pDoc->m_pMpeIoF)
	{
		if (pDoc->m_nMpeIo>0)
		{
			pDoc->m_pMpeIoF = new unsigned short[pDoc->m_nMpeIo];
			for (i = 0; i<pDoc->m_nMpeIo; i++)
				pDoc->m_pMpeIoF[i] = 0;
		}
	}


	pDoc->m_nMpeSignal = pDoc->MkIo.MpeSignal.nMaxSeg;
	if (!pDoc->m_pMpeSignal)
	{
		if (pDoc->m_nMpeSignal>0)
		{
			pDoc->m_pMpeSignal = new unsigned short[pDoc->m_nMpeSignal];
			for (i = 0; i<pDoc->m_nMpeSignal; i++)
				pDoc->m_pMpeSignal[i] = 0;
		}
	}


	pDoc->m_nMpeData = pDoc->MkIo.MpeData.nMaxSeg;
	if (!pDoc->m_pMpeData)
	{
		if (pDoc->m_nMpeData>0)
		{
			pDoc->m_pMpeData = new long*[pDoc->m_nMpeData];
			for (i = 0; i<pDoc->m_nMpeData; i++)
			{
				pDoc->m_pMpeData[i] = new long[16];
				for (k = 0; k<16; k++)
					pDoc->m_pMpeData[i][k] = 0;
			}
		}
	}
#endif
}

BOOL CGvisR2R_PunchView::InitAct()
{
#ifdef USE_XMP
	if (!m_pMotion)
		return FALSE;
#endif

#ifdef USE_LIGHT
	if (!m_pLight)
		return FALSE;
#endif

#ifdef USE_FLUCK
	if (!m_pFluck)
		return FALSE;
#endif

	int nAxis;

	if (m_pMotion)
	{
		// Motor On
		for (nAxis = 0; nAxis<m_pMotion->m_ParamCtrl.nTotAxis; nAxis++)
		{
			m_pMotion->ServoOnOff(nAxis, TRUE);
			Sleep(100);
		}

		double dX[2], dY[2];

		if (pDoc->m_pSpecLocal && IsPinData())
		{
			dX[0] = pDoc->m_pSpecLocal->m_dPinPosX[0];
			dY[0] = pDoc->m_pSpecLocal->m_dPinPosY[0];
			dX[1] = pDoc->m_pSpecLocal->m_dPinPosX[1];
			dY[1] = pDoc->m_pSpecLocal->m_dPinPosY[1];
		}
		else
		{
			dX[0] = _tstof(pDoc->WorkingInfo.Motion.sPinPosX[0]);
			dY[0] = _tstof(pDoc->WorkingInfo.Motion.sPinPosY[0]);
			dX[1] = _tstof(pDoc->WorkingInfo.Motion.sPinPosX[1]);
			dY[1] = _tstof(pDoc->WorkingInfo.Motion.sPinPosY[1]);
		}
		m_pMotion->SetPinPos(0, dX[0], dY[0]);
		m_pMotion->SetPinPos(1, dX[1], dY[1]);
		m_pMotion->m_dStBufPos = _tstof(pDoc->WorkingInfo.Motion.sStBufPos);


		CfPoint ptPnt0(dX[0], dY[0]);
		if (pDoc->m_Master[0].m_pPcsRgn)
			pDoc->m_Master[0].m_pPcsRgn->SetPinPos(0, ptPnt0);

		CfPoint ptPnt1(dX[1], dY[1]);
		if (pDoc->m_Master[0].m_pPcsRgn)
			pDoc->m_Master[0].m_pPcsRgn->SetPinPos(1, ptPnt1);

		if (pDoc->m_pSpecLocal)
		{
			pDoc->SetMkPnt(CAM_BOTH);
		}

		double dPos = _tstof(pDoc->WorkingInfo.Motion.sStBufPos);
		SetBufInitPos(dPos);
		double dVel = _tstof(pDoc->WorkingInfo.Motion.sBufHomeSpd);
		double dAcc = _tstof(pDoc->WorkingInfo.Motion.sBufHomeAcc);
		SetBufHomeParam(dVel, dAcc);
	}

	// Light On
	if (m_pDlgMenu02)
	{
		m_pDlgMenu02->SetLight(_tstoi(pDoc->WorkingInfo.Light.sVal[0]));
		m_pDlgMenu02->SetLight2(_tstoi(pDoc->WorkingInfo.Light.sVal[1]));
	}

	// Homming
	if (m_pVoiceCoil[0])
		m_pVoiceCoil[0]->SearchHomeSmac(0);
	if (m_pVoiceCoil[1])
		m_pVoiceCoil[1]->SearchHomeSmac(1);

	return TRUE;
}

BOOL CGvisR2R_PunchView::TcpIpInit()
{
#ifdef USE_SR1000W
	if (!m_pSr1000w)
	{
		m_pSr1000w = new CSr1000w(pDoc->WorkingInfo.System.sIpClient[ID_SR1000W], pDoc->WorkingInfo.System.sIpServer[ID_SR1000W], pDoc->WorkingInfo.System.sPort[ID_SR1000W], this);
	}
#endif	

#ifdef USE_TCPIP
	if (!m_pEngrave)
	{
		m_pEngrave = new CEngrave(pDoc->WorkingInfo.System.sIpClient[ID_PUNCH], pDoc->WorkingInfo.System.sIpServer[ID_ENGRAVE], pDoc->WorkingInfo.System.sPort[ID_ENGRAVE], this);
		m_pEngrave->SetHwnd(this->GetSafeHwnd());
	}
#endif

	return TRUE;
}

void CGvisR2R_PunchView::DtsInit()
{
#ifdef USE_DTS
	if (pDoc->m_bUseDts)
	{
		if (!m_pDts)
		{
			m_pDts = new CDts(this);
		}
	}
#endif
}

BOOL CGvisR2R_PunchView::HwInit()
{
	if (m_pLight)
	{
		delete m_pLight;
		m_pLight = NULL;
	}
	m_pLight = new CLight(this);
	m_pLight->Init();
#ifdef USE_FLUCK
	if (pDoc->WorkingInfo.Fluck.bUse)
	{
		if (m_pFluck)
		{
			delete m_pFluck;
			m_pFluck = NULL;
		}
		m_pFluck = new CFluck(this);
		m_pFluck->Init();
	}
#endif
	if (m_pVoiceCoil[0])
	{
		delete m_pVoiceCoil[0];
		m_pVoiceCoil[0] = NULL;
	}

	m_pVoiceCoil[0] = new CSmac(this);
	m_pVoiceCoil[0]->SetCh(VOICE_COIL_FIRST_CAM);
	m_pVoiceCoil[0]->Init();

	if (m_pVoiceCoil[1])
	{
		delete m_pVoiceCoil[1];
		m_pVoiceCoil[1] = NULL;
	}

	m_pVoiceCoil[1] = new CSmac(this);
	m_pVoiceCoil[1]->SetCh(VOICE_COIL_SECOND_CAM);
	m_pVoiceCoil[1]->Init();

	if (m_pMotion)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}
	m_pMotion = new CMotion(this);
	if (!m_pMotion->InitBoard())
	{
		//		DoMyMsgBox(_T("XMP 보드 초기화 실패, 다시 시작하세요.!!!"));
		MsgBox(_T("XMP 보드 초기화 실패, 다시 시작하세요.!!!"));
		PostMessage(WM_CLOSE);
		return FALSE;
	}

#ifdef USE_MPE
	if (!m_pMpe)
		m_pMpe = new CMpDevice(this);
	if (!m_pMpe->Init(1, 1))
	{
		//		DoMyMsgBox(_T("메카트로링크(MC0)의 통신프로그램을 실행 후, 다시 시작하세요.!!!"));
		MsgBox(_T("메카트로링크(MC0)의 통신프로그램을 실행 후, 다시 시작하세요.!!!"));
		PostMessage(WM_CLOSE);
		return FALSE;
	}
#endif

	return TRUE;
}

void CGvisR2R_PunchView::HwKill()
{
	if (m_pMotion)
	{
		delete m_pMotion;
		m_pMotion = NULL;
	}

	if (m_pMpe)
	{
		delete m_pMpe;
		m_pMpe = NULL;
	}

	if (m_pVoiceCoil[0])
	{
		delete m_pVoiceCoil[0];
		m_pVoiceCoil[0] = NULL;
	}

	if (m_pVoiceCoil[1])
	{
		delete m_pVoiceCoil[1];
		m_pVoiceCoil[1] = NULL;
	}

	if (m_pLight)
	{
		m_pLight->Close();
		delete m_pLight;
		m_pLight = NULL;
	}

	if (m_pEngrave)
	{
		m_pEngrave->Close();
		delete m_pEngrave;
		m_pEngrave = NULL;
	}

	if (m_pSr1000w)
	{
		m_pSr1000w->Close();
		delete m_pSr1000w;
		m_pSr1000w = NULL;
	}

	if (m_pDts)
	{
		delete m_pDts;
		m_pDts = NULL;
	}

#ifdef USE_FLUCK
	if (m_pFluck)
	{
		m_pFluck->Close();
		delete m_pFluck;
		m_pFluck = NULL;
	}
#endif
}

void CGvisR2R_PunchView::GetDispMsg(CString &strMsg, CString &strTitle)
{
	if (m_pDlgMsgBox)
		m_pDlgMsgBox->GetDispMsg(strMsg, strTitle);
}

void CGvisR2R_PunchView::DispMsg(CString strMsg, CString strTitle, COLORREF color, DWORD dwDispTime, BOOL bOverWrite)
{
	if (m_bDispMsg)
		return;

	if (m_bAuto)
	{
		return;
	}

	m_bDispMsg = TRUE;

	if (dwDispTime == 0)
	{
		dwDispTime = 24 * 3600 * 1000;
	}

	if (m_pDlgMsgBox != NULL)
	{
		if (bOverWrite)
		{
			if(m_pDlgMsgBox)
				m_pDlgMsgBox->SetDispMsg(strMsg, strTitle, dwDispTime, color);
		}
		if (m_pDlgMsgBox)
			m_pDlgMsgBox->ShowWindow(SW_SHOW);
		if (m_pDlgMsgBox)
			m_pDlgMsgBox->SetFocus();
		if (m_pDlgMsgBox)
			((CButton*)m_pDlgMsgBox->GetDlgItem(IDOK))->SetCheck(TRUE);
	}
	else
	{
		m_pDlgMsgBox = new CDlgMsgBox(this, strTitle, strMsg, dwDispTime, color);
		if (m_pDlgMsgBox->GetSafeHwnd() == 0)
		{
			m_pDlgMsgBox->Create();
			m_pDlgMsgBox->ShowWindow(SW_SHOW);
			m_pDlgMsgBox->SetDispMsg(strMsg, strTitle, dwDispTime, color);
			m_pDlgMsgBox->SetFocus();
			((CButton*)m_pDlgMsgBox->GetDlgItem(IDOK))->SetCheck(TRUE);
		}
	}

	m_bDispMsg = FALSE;
}

void CGvisR2R_PunchView::ClrDispMsg()
{
	OnQuitDispMsg(NULL, NULL);
}

BOOL CGvisR2R_PunchView::WaitClrDispMsg()
{
	m_bWaitClrDispMsg = TRUE;
	MSG message;

	DWORD dwMilliseconds = 0; // 100ms sec sleep
	while (WAIT_OBJECT_0 != ::WaitForSingleObject(m_evtWaitClrDispMsg, dwMilliseconds) && m_pDlgMsgBox != NULL)
	{
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	};
	Sleep(10);
	m_bWaitClrDispMsg = FALSE;
	return TRUE;
}

LONG CGvisR2R_PunchView::OnQuitDispMsg(UINT wParam, LONG lParam)
{
	if (m_pDlgMsgBox)
	{
		if (m_pDlgMsgBox->GetSafeHwnd())
			m_pDlgMsgBox->DestroyWindow();
		delete m_pDlgMsgBox;
		m_pDlgMsgBox = NULL;
	}

	return 0L;
}

void CGvisR2R_PunchView::ShowDlg(int nID)
{
	HideAllDlg();

	switch (nID)
	{
	//case IDD_DLG_INFO:
	//	if (!m_pDlgInfo)
	//	{
	//		m_pDlgInfo = new CDlgInfo(this);
	//		if (m_pDlgInfo->GetSafeHwnd() == 0)
	//		{
	//			m_pDlgInfo->Create();
	//			m_pDlgInfo->ShowWindow(SW_SHOW);
	//		}
	//	}
	//	else
	//	{
	//		m_pDlgInfo->ShowWindow(SW_SHOW);
	//	}
	//	break;

	case IDD_DLG_FRAME_HIGH:
		if (!m_pDlgFrameHigh)
		{
			m_pDlgFrameHigh = new CDlgFrameHigh(this);
			if (m_pDlgFrameHigh->GetSafeHwnd() == 0)
			{
				m_pDlgFrameHigh->Create();
				m_pDlgFrameHigh->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgFrameHigh->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_01:
		if (!m_pDlgMenu01)
		{
			m_pDlgMenu01 = new CDlgMenu01(this);
			if (m_pDlgMenu01->GetSafeHwnd() == 0)
			{
				m_pDlgMenu01->Create();
				m_pDlgMenu01->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu01->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_02:
		if (!m_pDlgMenu02)
		{
			m_pDlgMenu02 = new CDlgMenu02(this);
			if (m_pDlgMenu02->GetSafeHwnd() == 0)
			{
				m_pDlgMenu02->Create();
				m_pDlgMenu02->ShowDebugEngSig();
				m_pDlgMenu02->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu02->ShowDebugEngSig();
			m_pDlgMenu02->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_03:
		if (!m_pDlgMenu03)
		{
			m_pDlgMenu03 = new CDlgMenu03(this);
			if (m_pDlgMenu03->GetSafeHwnd() == 0)
			{
				m_pDlgMenu03->Create();
				m_pDlgMenu03->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu03->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_04:
		if (!m_pDlgMenu04)
		{
			m_pDlgMenu04 = new CDlgMenu04(this);
			if (m_pDlgMenu04->GetSafeHwnd() == 0)
			{
				m_pDlgMenu04->Create();
				m_pDlgMenu04->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu04->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_05:
		if (!m_pDlgMenu05)
		{
			m_pDlgMenu05 = new CDlgMenu05(this);
			if (m_pDlgMenu05->GetSafeHwnd() == 0)
			{
				m_pDlgMenu05->Create();
				m_pDlgMenu05->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu05->ShowWindow(SW_SHOW);
		}
		break;

	case IDD_DLG_MENU_06:
		if (!m_pDlgMenu06)
		{
			m_pDlgMenu06 = new CDlgMenu06(this);
			if (m_pDlgMenu06->GetSafeHwnd() == 0)
			{
				m_pDlgMenu06->Create();
				m_pDlgMenu06->ShowWindow(SW_SHOW);
			}
		}
		else
		{
			m_pDlgMenu06->ShowWindow(SW_SHOW);
		}
		break;

	//case IDD_DLG_UTIL_01:
	//	if (!m_pDlgUtil01)
	//	{
	//		m_pDlgUtil01 = new CDlgUtil01(this);
	//		if (m_pDlgUtil01->GetSafeHwnd() == 0)
	//		{
	//			m_pDlgUtil01->Create();
	//			m_pDlgUtil01->ShowWindow(SW_SHOW);
	//		}
	//	}
	//	else
	//	{
	//		m_pDlgUtil01->ShowWindow(SW_SHOW);
	//	}
	//	break;

	//case IDD_DLG_UTIL_02:
	//	if (!m_pDlgUtil02)
	//	{
	//		m_pDlgUtil02 = new CDlgUtil02(this);
	//		if (m_pDlgUtil02->GetSafeHwnd() == 0)
	//		{
	//			m_pDlgUtil02->Create();
	//			m_pDlgUtil02->ShowWindow(SW_SHOW);
	//		}
	//	}
	//	else
	//	{
	//		m_pDlgUtil02->ShowWindow(SW_SHOW);
	//	}
	//	break;

		// 	case IDD_DLG_UTIL_03:
		// 		if(!m_pDlgUtil03)
		// 		{
		// 			m_pDlgUtil03 = new CDlgUtil03(this);
		// 			if(m_pDlgUtil03->GetSafeHwnd() == 0)
		// 			{
		// 				m_pDlgUtil03->Create();
		// 				m_pDlgUtil03->ShowWindow(SW_SHOW);
		// 			}
		// 		}
		// 		else
		// 		{
		// 			m_pDlgUtil03->ShowWindow(SW_SHOW);
		// 		}
		// 		break;		
	}
}

void CGvisR2R_PunchView::HideAllDlg()
{
	//if (m_pDlgInfo && m_pDlgInfo->GetSafeHwnd())
	//{
	//	if (m_pDlgInfo->IsWindowVisible())
	//		m_pDlgInfo->ShowWindow(SW_HIDE);
	//}
	if (m_pDlgMenu01 && m_pDlgMenu01->GetSafeHwnd())
	{
		if (m_pDlgMenu01->IsWindowVisible())
			m_pDlgMenu01->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu02 && m_pDlgMenu02->GetSafeHwnd())
	{
		if (m_pDlgMenu02->m_pDlgUtil03 && m_pDlgMenu02->m_pDlgUtil03->GetSafeHwnd())
		{
			if (m_pDlgMenu02->m_pDlgUtil03->IsWindowVisible())
			{
				m_pDlgMenu02->m_pDlgUtil03->ShowWindow(SW_HIDE);
				m_pDlgMenu02->ResetBtn(IDC_CHK_MARKING_TEST2);
			}
		}

		if (m_pDlgMenu02->IsWindowVisible())
			m_pDlgMenu02->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu03 && m_pDlgMenu03->GetSafeHwnd())
	{
		if (m_pDlgMenu03->IsWindowVisible())
			m_pDlgMenu03->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu04 && m_pDlgMenu04->GetSafeHwnd())
	{
		if (m_pDlgMenu04->IsWindowVisible())
			m_pDlgMenu04->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu05 && m_pDlgMenu05->GetSafeHwnd())
	{
		if (m_pDlgMenu05->IsWindowVisible())
			m_pDlgMenu05->ShowWindow(SW_HIDE);
	}
	if (m_pDlgMenu06 && m_pDlgMenu06->GetSafeHwnd())
	{
		if (m_pDlgMenu06->IsWindowVisible())
			m_pDlgMenu06->ShowWindow(SW_HIDE);
	}

	//if (m_pDlgUtil01 && m_pDlgUtil01->GetSafeHwnd())
	//{
	//	if (m_pDlgUtil01->IsWindowVisible())
	//		m_pDlgUtil01->ShowWindow(SW_HIDE);
	//}
	//if (m_pDlgUtil02 && m_pDlgUtil02->GetSafeHwnd())
	//{
	//	if (m_pDlgUtil02->IsWindowVisible())
	//		m_pDlgUtil02->ShowWindow(SW_HIDE);
	//}
}

void CGvisR2R_PunchView::DelAllDlg()
{
	//if (m_pDlgInfo != NULL)
	//{
	//	delete m_pDlgInfo;
	//	m_pDlgInfo = NULL;
	//}
	// 	if(m_pDlgUtil03 != NULL) 
	// 	{
	// 		delete m_pDlgUtil03;
	// 		m_pDlgUtil03 = NULL;
	// 	}
	//if (m_pDlgUtil02 != NULL)
	//{
	//	delete m_pDlgUtil02;
	//	m_pDlgUtil02 = NULL;
	//}
	//if (m_pDlgUtil01 != NULL)
	//{
	//	delete m_pDlgUtil01;
	//	m_pDlgUtil01 = NULL;
	//}

	if (m_pDlgMenu06 != NULL)
	{
		delete m_pDlgMenu06;
		m_pDlgMenu06 = NULL;
	}
	if (m_pDlgMenu05 != NULL)
	{
		delete m_pDlgMenu05;
		m_pDlgMenu05 = NULL;
	}
	if (m_pDlgMenu04 != NULL)
	{
		delete m_pDlgMenu04;
		m_pDlgMenu04 = NULL;
	}
	if (m_pDlgMenu03 != NULL)
	{
		delete m_pDlgMenu03;
		m_pDlgMenu03 = NULL;
	}
	if (m_pDlgMenu02 != NULL)
	{
		delete m_pDlgMenu02;
		m_pDlgMenu02 = NULL;
	}
	if (m_pDlgMenu01 != NULL)
	{
		delete m_pDlgMenu01;
		m_pDlgMenu01 = NULL;
	}
	if (m_pDlgFrameHigh != NULL)
	{
		delete m_pDlgFrameHigh;
		m_pDlgFrameHigh = NULL;
	}

	if (m_pDlgMsgBox != NULL)
	{
		if (m_pDlgMsgBox->GetSafeHwnd())
			m_pDlgMsgBox->DestroyWindow();
		delete m_pDlgMsgBox;
		m_pDlgMsgBox = NULL;
	}
}

LRESULT CGvisR2R_PunchView::OnDlgInfo(WPARAM wParam, LPARAM lParam)
{
	ClrDispMsg();
	CDlgInfo Dlg;
	m_pDlgInfo = &Dlg;
	Dlg.DoModal();
	m_pDlgInfo = NULL;

	if (m_pDlgMenu01)
		m_pDlgMenu01->ChkUserInfo(FALSE);

	return 0L;
}

void CGvisR2R_PunchView::DispIo()
{
	ClrDispMsg();
	CDlgUtil04 Dlg;
	Dlg.DoModal();
}

void CGvisR2R_PunchView::DispDatabaseConnection()
{
	//ClrDispMsg();
	//CDlgUtil07 Dlg;
	//Dlg.DoModal();
}

void CGvisR2R_PunchView::Option01()
{
	ClrDispMsg();
	CDlgOption01 Dlg;
	Dlg.DoModal();
}

void CGvisR2R_PunchView::CamMasterInfoMst()
{
	ClrDispMsg();
	CDlgCammasterinfoMst Dlg;
	Dlg.DoModal();
}

void CGvisR2R_PunchView::CamMasterInfoPch()
{
	ClrDispMsg();
	CDlgCammasterinfoPch Dlg;
	Dlg.DoModal();
}

void CGvisR2R_PunchView::CamMasterInfoStrPcs()
{
	ClrDispMsg();
	CDlgCammasterinfoStrpcs Dlg;
	Dlg.DoModal();
}

void CGvisR2R_PunchView::TowerLamp(COLORREF color, BOOL bOn, BOOL bWink)
{
	return;

	if (bWink)
	{
		if (color == RGB_RED)
		{
			m_bTowerWinker[0] = bOn;
			if (bOn)
			{
				m_bTowerWinker[1] = FALSE;
				m_bTowerWinker[2] = FALSE;
			}
		}
		if (color == RGB_GREEN)
		{
			m_bTowerWinker[1] = bOn;
			if (bOn)
			{
				m_bTowerWinker[0] = FALSE;
				m_bTowerWinker[2] = FALSE;
			}
		}
		if (color == RGB_YELLOW)
		{
			m_bTowerWinker[2] = bOn;
			if (bOn)
			{
				m_bTowerWinker[1] = FALSE;
				m_bTowerWinker[0] = FALSE;
			}
		}

		if (!m_bTowerWinker[0] && !m_bTowerWinker[1] && !m_bTowerWinker[2])
			m_bTimTowerWinker = FALSE;
		else
		{
			if (!m_bTimTowerWinker)
			{
				m_bTimTowerWinker = TRUE;
				SetTimer(TIM_TOWER_WINKER, 100, NULL);
			}
		}
	}
	else
	{
		m_bTimTowerWinker = FALSE;

#ifdef USE_MPE
		if (!pDoc->m_pMpeIo)
			return;
#endif
	}
}

void CGvisR2R_PunchView::DispTowerWinker()
{
	m_nCntTowerWinker++;
	if (m_nCntTowerWinker > LAMP_DELAY)
	{
		m_nCntTowerWinker = 0;
	}

}

void CGvisR2R_PunchView::BuzzerFromThread(BOOL bOn, int nCh)
{
	m_bBuzzerFromThread = TRUE;
}

void CGvisR2R_PunchView::Buzzer(BOOL bOn, int nCh)
{
	return; // PLC제어

	if (!m_pMpe)
		return;

	if (!bOn)
	{
		switch (nCh)
		{
		case 0:
			MpeWrite(Plc.DlgMenu01.Buzzer1, 0); // 부저1 On  (PC가 ON, OFF)
			break;
		case 1:
			MpeWrite(Plc.DlgMenu01.Buzzer2, 0); // 부저2 On  (PC가 ON, OFF)
			break;
		}
	}
	else
	{
		switch (nCh)
		{
		case 0:
			//IoWrite(Plc.DlgMenu01.Buzzer1, 1); // 부저1 On  (PC가 ON, OFF) - 20141020
			MpeWrite(Plc.DlgMenu01.Buzzer1, 0);
			Sleep(300);
			MpeWrite(Plc.DlgMenu01.Buzzer1, 1);
			break;
		case 1:
			//IoWrite(Plc.DlgMenu01.Buzzer2, 1); // 부저2 On  (PC가 ON, OFF) - 20141020
			MpeWrite(Plc.DlgMenu01.Buzzer1, 0);
			Sleep(300);
			MpeWrite(Plc.DlgMenu01.Buzzer2, 1);
			break;
		}
	}
}



void CGvisR2R_PunchView::ThreadInit()
{
	if (!m_bThread[0])
		m_Thread[0].Start(GetSafeHwnd(), this, ThreadProc0);
}

void CGvisR2R_PunchView::ThreadKill()
{
	if (m_bThread[0])
	{
		m_Thread[0].Stop();
		Sleep(20);
		while (m_bThread[0])
		{
			Sleep(20);
		}
	}
	if (m_bThread[1])
	{
		m_Thread[1].Stop();
		Sleep(20);
		while (m_bThread[1])
		{
			Sleep(20);
		}
	}
	if (m_bThread[2])
	{
		m_Thread[2].Stop();
		Sleep(20);
		while (m_bThread[2])
		{
			Sleep(20);
		}
	}
	if (m_bThread[3])
	{
		m_Thread[3].Stop();
		Sleep(20);
		while (m_bThread[3])
		{
			Sleep(20);
		}
	}
	if (m_bThread[4])
	{
		m_Thread[4].Stop();
		Sleep(20);
		while (m_bThread[4])
		{
			Sleep(20);
		}
	}
	if (m_bThread[5])
	{
		CWinThread* pPointer = m_Thread[5].GetPointer();
		m_Thread[5].Stop();
		Sleep(20);
		//while (m_bThread[5])
		//{
		//	Sleep(20);
		//}
	}
	if (m_bThread[6])
	{
		m_Thread[6].Stop();
		Sleep(20);
		while (m_bThread[6])
		{
			Sleep(20);
		}
	}
	if (m_bThread[7])
	{
		m_Thread[7].Stop();
		Sleep(20);
		while (m_bThread[7])
		{
			Sleep(20);
		}
	}
	if (m_bThread[8])
	{
		m_Thread[8].Stop();
		Sleep(20);
		while (m_bThread[8])
		{
			Sleep(20);
		}
	}
	if (m_bThread[9])
	{
		m_Thread[9].Stop();
		Sleep(20);
		while (m_bThread[9])
		{
			Sleep(20);
		}
	}
	if (m_bThread[10])
	{
		m_Thread[10].Stop();
		Sleep(20);
		while (m_bThread[10])
		{
			Sleep(20);
		}
	}
	if (m_bThread[11])
	{
		m_Thread[11].Stop();
		Sleep(20);
		while (m_bThread[11])
		{
			Sleep(20);
		}
	}
	if (m_bThread[12])
	{
		m_Thread[12].Stop();
		Sleep(20);
		while (m_bThread[12])
		{
			Sleep(20);
		}
	}
	if (m_bThread[13])
	{
		m_Thread[13].Stop();
		Sleep(20);
		while (m_bThread[13])
		{
			Sleep(20);
		}
	}
	if (m_bThread[14])
	{
		m_Thread[14].Stop();
		Sleep(20);
		while (m_bThread[14])
		{
			Sleep(20);
		}
	}
	if (m_bThread[15])
	{
		m_Thread[15].Stop();
		Sleep(20);
		while (m_bThread[15])
		{
			Sleep(20);
		}
	}
	if (m_bThread[16])
	{
		m_Thread[16].Stop();
		Sleep(20);
		while (m_bThread[16])
		{
			Sleep(20);
		}
	}
	if (m_bThread[17])
	{
		m_Thread[17].Stop();
		Sleep(20);
		while (m_bThread[17])
		{
			Sleep(20);
		}
	}
	if (m_bThread[18])
	{
		m_Thread[18].Stop();
		Sleep(20);
		while (m_bThread[18])
		{
			Sleep(20);
		}
	}
	if (m_bThread[19])
	{
		m_Thread[19].Stop();
		Sleep(20);
		while (m_bThread[19])
		{
			Sleep(20);
		}
	}
	if (m_bThread[20])
	{
		m_Thread[20].Stop();
		Sleep(20);
		while (m_bThread[20])
		{
			Sleep(20);
		}
	}
	if (m_bThread[21])
	{
		m_Thread[21].Stop();
		Sleep(20);
		while (m_bThread[21])
		{
			Sleep(20);
		}
	}
	if (m_bThread[22])
	{
		m_Thread[22].Stop();
		Sleep(20);
		while (m_bThread[22])
		{
			Sleep(20);
		}
	}
	if (m_bThread[23])
	{
		m_Thread[23].Stop();
		Sleep(20);
		while (m_bThread[23])
		{
			Sleep(20);
		}
	}
	if (m_bThread[24])
	{
		m_Thread[24].Stop();
		Sleep(20);
		while (m_bThread[24])
		{
			Sleep(20);
		}
	}
	if (m_bThread[25])
	{
		m_Thread[25].Stop();
		Sleep(20);
		while (m_bThread[25])
		{
			Sleep(20);
		}
	}
	if (m_bThread[26])
	{
		m_Thread[26].Stop();
		Sleep(20);
		while (m_bThread[26])
		{
			Sleep(20);
		}
	}
	if (m_bThread[27])
	{
		m_Thread[27].Stop();
		Sleep(20);
		while (m_bThread[27])
		{
			Sleep(20);
		}
	}
	if (m_bThread[28])
	{
		m_Thread[28].Stop();
		Sleep(20);
		while (m_bThread[28])
		{
			Sleep(20);
		}
	}
	if (m_bThread[29])
	{
		m_Thread[29].Stop();
		Sleep(20);
		while (m_bThread[29])
		{
			Sleep(20);
		}
	}
	if (m_bThread[30])
	{
		m_Thread[30].Stop();
		Sleep(20);
		while (m_bThread[30])
		{
			Sleep(20);
		}
	}
	if (m_bThread[31])
	{
		m_Thread[31].Stop();
		Sleep(20);
		while (m_bThread[31])
		{
			Sleep(20);
		}
	}
	if (m_bThread[32])
	{
		m_Thread[32].Stop();
		Sleep(20);
		while (m_bThread[32])
		{
			Sleep(20);
		}
	}
	if (m_bThread[33])
	{
		m_Thread[33].Stop();
		Sleep(20);
		while (m_bThread[33])
		{
			Sleep(20);
		}
	}
	if (m_bThread[34])
	{
		m_Thread[34].Stop();
		Sleep(20);
		while (m_bThread[34])
		{
			Sleep(20);
		}
	}
	if (m_bThread[35])
	{
		m_Thread[35].Stop();
		Sleep(20);
		while (m_bThread[35])
		{
			Sleep(20);
		}
	}
	if (m_bThread[36])
	{
		m_Thread[36].Stop();
		Sleep(20);
		while (m_bThread[36])
		{
			Sleep(20);
		}
	}
	if (m_bThread[37])
	{
		m_Thread[37].Stop();
		Sleep(20);
		while (m_bThread[37])
		{
			Sleep(20);
		}
	}
	if (m_bThread[38])
	{
		m_Thread[38].Stop();
		Sleep(20);
		while (m_bThread[38])
		{
			Sleep(20);
		}
	}
	if (m_bThread[39])
	{
		m_Thread[39].Stop();
		Sleep(20);
		while (m_bThread[39])
		{
			Sleep(20);
		}
	}
	if (m_bThread[40])
	{
		m_Thread[40].Stop();
		Sleep(20);
		while (m_bThread[40])
		{
			Sleep(20);
		}
	}
}

UINT CGvisR2R_PunchView::ThreadProc0(LPVOID lpContext)	// DoMark0(), DoMark1()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast< CGvisR2R_PunchView* >(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[0] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[0].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[0] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;
#ifndef TEST_MODE
			if (pDoc->m_Master[0].MasterInfo.nActionCode == 1 || pDoc->m_Master[0].MasterInfo.nActionCode == 3)	// 0 : Rotation / Mirror 적용 없음(CAM Data 원본), 1 : 좌우 미러, 2 : 상하 미러, 3 : 180 회전, 4 : 270 회전(CCW), 5 : 90 회전(CW)
			{
				if (pThread->m_bTHREAD_MK[0])
				{
					if (pThread->m_nBufUpSerial[0] > 0)
					{
						if (pDoc->GetTestMode() == MODE_OUTER)
							pThread->DoMark0Its();
						else
							pThread->DoMark0();
					}
					else
					{
						pThread->m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), pThread->m_bDoneMk[0]);
						pThread->m_bTHREAD_MK[0] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[0]"), pThread->m_bTHREAD_MK[0]);
					}
				}
				if (pThread->m_bTHREAD_MK[1])
				{
					if (pThread->m_nBufUpSerial[1] > 0)
					{
						if (pDoc->GetTestMode() == MODE_OUTER)
							pThread->DoMark1Its();
						else
							pThread->DoMark1();
					}
					else
					{
						pThread->m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), pThread->m_bDoneMk[1]);
						pThread->m_bTHREAD_MK[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), pThread->m_bTHREAD_MK[1]);
					}
				}
			}
			else
			{
				if (pThread->m_bTHREAD_MK[1])
				{
					if (pThread->m_nBufUpSerial[1] > 0)
					{
						if (pDoc->GetTestMode() == MODE_OUTER)
							pThread->DoMark1Its();
						else
							pThread->DoMark1();
					}
					else
					{
						pThread->m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), pThread->m_bDoneMk[1]);
						pThread->m_bTHREAD_MK[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), pThread->m_bTHREAD_MK[1]);
					}
				}
				if (pThread->m_bTHREAD_MK[0])
				{
					if (pThread->m_nBufUpSerial[0] > 0)
					{
						if (pDoc->GetTestMode() == MODE_OUTER)
							pThread->DoMark0Its();
						else
							pThread->DoMark0();
					}
					else
					{
						pThread->m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), pThread->m_bDoneMk[0]);
						pThread->m_bTHREAD_MK[0] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[0]"), pThread->m_bTHREAD_MK[0]);
					}
				}
			}
#else
			pThread->DoMark1();
			pThread->DoMark0();
			Sleep(100);
#endif
			if (pThread->m_bTHREAD_MK[3])
				pThread->DoReject1();
			if (pThread->m_bTHREAD_MK[2])
				pThread->DoReject0();

			if (!pThread->m_bTHREAD_MK[0] && !pThread->m_bTHREAD_MK[1] &&
				!pThread->m_bTHREAD_MK[2] && !pThread->m_bTHREAD_MK[3])
				Sleep(50);
			bLock = FALSE;
		}
		Sleep(10);
	}
	pThread->m_bThread[0] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc1(LPVOID lpContext)	// ChkCollision()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast< CGvisR2R_PunchView* >(lpContext);

	BOOL bLock = FALSE, bEStop = FALSE, bCollision = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[1] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[1].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[1] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;

			pThread->GetEnc();

			if (!pThread->m_bTHREAD_MK[0] && !pThread->m_bTHREAD_MK[1] &&
				!pThread->m_bTHREAD_MK[2] && !pThread->m_bTHREAD_MK[3])
			{
				if (pThread->ChkCollision() && !bEStop)
				{
					if (pThread->IsRunAxisX())
					{
						bEStop = TRUE;
						pThread->EStop();
					}
				}
				else if (!pThread->ChkCollision() && bEStop)
				{
					bEStop = FALSE;
				}
			}
			else
			{
				if (pThread->m_bCollision[0] && pThread->m_bCollision[1])
				{
					if (!bCollision)
					{
						bCollision = TRUE;
						pThread->SetPriority();
					}
				}
				else
					bCollision = FALSE;
			}

			bLock = FALSE;
		}
		Sleep(10);
	}
	pThread->m_bThread[1] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc2(LPVOID lpContext)	// DispDefImg()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast< CGvisR2R_PunchView* >(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[2] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[2].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[2] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;

			if (pThread->m_bTHREAD_DISP_DEF)
			{
				pThread->DispDefImg();
				Sleep(0);
			}
			else
				Sleep(30);
			

			bLock = FALSE;
		}
	}
	pThread->m_bThread[2] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc3(LPVOID lpContext)	// UpdateYield()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[3] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[3].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[3] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATAE_YIELD[0]) // Left Punching Yield
		{
			pThread->UpdateYield(pThread->m_nSerialTHREAD_UPDATAE_YIELD[0]);
			pThread->m_bTHREAD_UPDATAE_YIELD[0] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATAE_YIELD[0]"), pThread->m_bTHREAD_UPDATAE_YIELD[0]);
			Sleep(0);
		}
		else if(pThread->m_bTHREAD_UPDATAE_YIELD[1] && !pThread->m_bTHREAD_UPDATE_YIELD_UP) // Right Punching Yield
		{
			if (pDoc->WorkingInfo.LastJob.bDualTest)
			{
				if (!pThread->m_bTHREAD_UPDATE_YIELD_DN && !pThread->m_bTHREAD_UPDATE_YIELD_ALLUP && !pThread->m_bTHREAD_UPDATE_YIELD_ALLDN)
				{
					pThread->UpdateYield(pThread->m_nSerialTHREAD_UPDATAE_YIELD[1]);
					pThread->m_bTHREAD_UPDATAE_YIELD[1] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATAE_YIELD[1]"), pThread->m_bTHREAD_UPDATAE_YIELD[1]);
					Sleep(0);
				}
			}
			else
			{
				pThread->m_bTHREAD_UPDATAE_YIELD[1] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATAE_YIELD[1]"), pThread->m_bTHREAD_UPDATAE_YIELD[1]);
				pThread->UpdateYield(pThread->m_nSerialTHREAD_UPDATAE_YIELD[1]);
				Sleep(0);
			}
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[3] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc4(LPVOID lpContext)	// RunShift2Mk()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[4] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[4].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[4] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_SHIFT2MK)
		{
			if (!pThread->m_bTHREAD_UPDATAE_YIELD[0] && !pThread->m_bTHREAD_UPDATAE_YIELD[1])
			{
				pThread->RunShift2Mk();
				pThread->m_bTHREAD_SHIFT2MK = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_SHIFT2MK"), pThread->m_bTHREAD_SHIFT2MK);
				Sleep(0);
			}
			else
				Sleep(30);

		}
		else
			Sleep(30);
	}

	pThread->m_bThread[4] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc5(LPVOID lpContext)	// GetCurrentInfoSignal()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[5] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[5].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[5] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!pThread->m_bDestroyedView)
		{
			if (!bLock)
			{
				bLock = TRUE;
				//pThread->GetCurrentInfoSignal();
				bLock = FALSE;
			}
		}
		Sleep(500);
	}
	pThread->m_bThread[5] = FALSE;

	return 0;
}

void CGvisR2R_PunchView::DispStsMainMsg(int nIdx)
{
	CString str;
	str = m_sDispMsg[nIdx];
	pFrm->DispStatusBar(str, nIdx);
}

void CGvisR2R_PunchView::DispThreadTick()
{
	if (m_bDestroyedView)
		return;

	CString str;
		str.Format(_T("%d%d%d%d:%d%d"),
			m_bTHREAD_UPDATE_REELMAP_UP ? 1 : 0, m_bTHREAD_UPDATE_REELMAP_DN ? 1 : 0,
			m_bTHREAD_UPDATE_REELMAP_ALLUP ? 1 : 0, m_bTHREAD_UPDATE_REELMAP_ALLDN ? 1 : 0,
			m_bTHREAD_REELMAP_YIELD_UP ? 1 : 0, m_bTHREAD_REELMAP_YIELD_DN ? 1 : 0);
		pFrm->DispStatusBar(str, 5);
#ifdef USE_IDS
		double dFPS[2];
		if (m_pVision[0])
			m_pVision[0]->GetFramesPerSecond(&dFPS[0]);
		if (m_pVision[1])
			m_pVision[1]->GetFramesPerSecond(&dFPS[1]);
		str.Format(_T("%.1f,%.1f"), dFPS[0], dFPS[1]);
		pFrm->DispStatusBar(str, 6);
#else
		//str.Format(_T("%d,%d,%d,%d"), m_nStepAuto, m_nSerialRmapUpdate, m_nShareUpS, m_nShareDnS);//pView->m_nLotEndAuto
		str.Format(_T("%d,%d,%d,%d"), m_nStepAuto, m_nMkStAuto, m_nStepMk[0], m_nStepMk[1]);//pView->m_nLotEndAuto
		pFrm->DispStatusBar(str, 6);
#endif
}

void CGvisR2R_PunchView::SwJog(int nAxisID, int nDir, BOOL bOn)
{
	if (!pView->m_pMotion)// || pDoc->Status.bAuto)
		return;

	if (IsClampOff())
	{
		return;
	}

	double fVel, fAcc, fJerk;
	double dStep;
	if (pDoc->Status.bSwJogFast)
		dStep = 0.5;
	else
		dStep = 0.1;

	if (pDoc->Status.bSwJogStep)
	{
		if (bOn)
		{
			double dPos = pView->m_dEnc[nAxisID]; // m_pMotion->GetActualPosition(nAxisID);
			if (nDir == M_CW)
				dPos += dStep;
			else if (nDir == M_CCW)
				dPos -= dStep;

			if (nAxisID == AXIS_X0)
			{
				if (m_bAuto && m_bTHREAD_MK[0] && m_bTHREAD_MK[1] && IsReview())
				{
					if (nDir == M_CW) // ▶ Jog 버튼.
						return;
				}

				if (m_pMotion->IsLimit(MS_X0, nDir))
					return;
				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, dStep, fVel, fAcc, fJerk);
				m_pMotion->Move(MS_X0, dPos, fVel, fAcc, fAcc);
			}
			else if (nAxisID == AXIS_Y0)
			{
				if (m_pMotion->IsLimit(MS_Y0, nDir))
					return;
				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_Y0, dStep, fVel, fAcc, fJerk);
				m_pMotion->Move(MS_Y0, dPos, fVel, fAcc, fAcc);
			}
			else if (nAxisID == AXIS_X1)
			{
				if (m_bAuto && m_bTHREAD_MK[0] && m_bTHREAD_MK[1] && IsReview())
				{
					if (nDir == M_CW) // ▶ Jog 버튼.
						return;
				}

				if (m_pMotion->IsLimit(MS_X1, nDir))
					return;
				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, dStep, fVel, fAcc, fJerk);
				m_pMotion->Move(MS_X1, dPos, fVel, fAcc, fAcc);
			}
			else if (nAxisID == AXIS_Y1)
			{
				if (m_pMotion->IsLimit(MS_Y1, nDir))
					return;
				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_Y1, dStep, fVel, fAcc, fJerk);
				m_pMotion->Move(MS_Y1, dPos, fVel, fAcc, fAcc);
			}
		}
	}
	else	// Jog Mode
	{
		if (!m_pDlgMenu02)
			return;

		if (nAxisID == AXIS_Y0)
		{
			if (nDir == M_CCW)		// Up
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_UP);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_UP);
			}
			else if (nDir == M_CW)	// Dn
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_DN);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_DN);
			}
		}
		else if (nAxisID == AXIS_X0)
		{
			if (m_bAuto && m_bTHREAD_MK[0] && m_bTHREAD_MK[1] && IsReview())
			{
				if (nDir == M_CW) // ▶ Jog 버튼.
					return;
			}

			if (nDir == M_CW)		// Rt
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_RT);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_RT);
			}
			else if (nDir == M_CCW)	// Lf
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_LF);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_LF);
			}
		}
		else if (nAxisID == AXIS_Y1)
		{
			if (nDir == M_CCW)		// Up
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_UP2);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_UP2);
			}
			else if (nDir == M_CW)	// Dn
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_DN2);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_DN2);
			}
		}
		else if (nAxisID == AXIS_X1)
		{
			if (m_bAuto && m_bTHREAD_MK[0] && m_bTHREAD_MK[1] && IsReview())
			{
				if (nDir == M_CW) // ▶ Jog 버튼.
					return;
			}

			if (nDir == M_CW)		// Rt
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_RT2);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_RT2);
			}
			else if (nDir == M_CCW)	// Lf
			{
				if (bOn)
					m_pDlgMenu02->SwMyBtnDown(IDC_BTN_JOG_LF2);
				else
					m_pDlgMenu02->SwMyBtnUp(IDC_BTN_JOG_LF2);
			}
		}
	}
}


BOOL CGvisR2R_PunchView::ChkShareIdx(int *pBufSerial, int nBufTot, int nShareSerial)
{
	if (nBufTot<1)
		return TRUE;
	for (int i = 0; i<nBufTot; i++)
	{
		if (pBufSerial[i] == nShareSerial)
			return FALSE;
	}
	return TRUE;
}


void CGvisR2R_PunchView::ChkShareVs()
{
	ChkShareVsUp();
	ChkShareVsDn();
}

void CGvisR2R_PunchView::ChkShareVsUp()
{
	CString str, str2;
	int nSerial, nAoiUpAutoSerial;

	if (ChkShareVsUp(nSerial))
	{
		str.Format(_T("US: %d"), nSerial);
		pDoc->Status.PcrShareVs[0].bExist = TRUE;
		pDoc->Status.PcrShareVs[0].nSerial = nSerial;
		str2.Format(_T("PCR파일 Received from VS - US: %d"), nSerial);
		pDoc->LogAuto(str2);
	}
	else
	{
		pDoc->Status.PcrShareVs[0].bExist = FALSE;
		pDoc->Status.PcrShareVs[0].nSerial = -1;
		str.Format(_T("US: "));
	}

	if (pFrm)
	{
		if (m_sShare[0] != str)
		{
			m_sShare[0] = str;
			pFrm->DispStatusBar(str, 4);
		}
	}
}

void CGvisR2R_PunchView::ChkShareVsDn()
{
	CString str, str2;
	int nSerial, nAoiDnAutoSerial;

	if (ChkShareVsDn(nSerial))
	{
		str.Format(_T("DS: %d"), nSerial);
		pDoc->Status.PcrShareVs[1].bExist = TRUE;
		pDoc->Status.PcrShareVs[1].nSerial = nSerial;
		str2.Format(_T("PCR파일 Received from VS - DS: %d"), nSerial);
		pDoc->LogAuto(str2);
	}
	else
	{
		pDoc->Status.PcrShareVs[1].bExist = FALSE;
		pDoc->Status.PcrShareVs[1].nSerial = -1;
		str.Format(_T("DS: "));
	}
	if (pFrm)
	{
		if (m_sShare[1] != str)
		{
			m_sShare[1] = str;
			pFrm->DispStatusBar(str, 2);
		}
	}
}

BOOL CGvisR2R_PunchView::ChkShareVs(int &nSerial)
{
	int nS0, nS1;
	BOOL b0 = ChkShareVsUp(nS0);
	BOOL b1 = ChkShareVsDn(nS1);

	if (!b0 || !b1)
	{
		nSerial = -1;
		return FALSE;
	}
	else if (nS0 != nS1)
	{
		nSerial = -1;
		return FALSE;
	}

	nSerial = nS0;
	return TRUE;
}

BOOL CGvisR2R_PunchView::ChkShareVsUp(int &nSerial)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsShareUp + _T("*.pcr"));
	if (!bExist)
		return FALSE; // pcr파일이 존재하지 않음.

	int nPos;
	CString sFileName, sSerial;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFileName = cFile.GetFileName();
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
				sSerial = sFileName.Left(nPos);

			nSerial = _tstoi(sSerial);
			if (nSerial > 0)
				return TRUE;
			else
			{
				nSerial = 0;
				return FALSE;
			}
		}
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::ChkShareVsDn(int &nSerial)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsShareDn + _T("*.pcr"));
	if (!bExist)
		return FALSE; // pcr파일이 존재하지 않음.

	int nPos;
	CString sFileName, sSerial;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFileName = cFile.GetFileName();
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
				sSerial = sFileName.Left(nPos);

			nSerial = _tstoi(sSerial);
			if (nSerial > 0)
				return TRUE;
			else
			{
				nSerial = 0;
				return FALSE;
			}
		}
	}

	return FALSE;
}


void CGvisR2R_PunchView::ChkShare()
{
	ChkShareUp();
	ChkShareDn();
	if (pDoc->m_bVsStatusUp)
		ChkShareVs();
}

void CGvisR2R_PunchView::ChkShareUp()
{
	CString str, str2;
	int nSerial, nAoiUpAutoSerial;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (ChkShareUp(nSerial))
	{
		str.Format(_T("US: %d"), nSerial);
		pDoc->Status.PcrShare[0].bExist = TRUE;
		pDoc->Status.PcrShare[0].nSerial = nSerial;
		if (m_pMpe)
		{
			str2.Format(_T("PCR파일 Received - US: %d"), nSerial);
			pDoc->LogAuto(str2);
		}
	}
	else
	{
		pDoc->Status.PcrShare[0].bExist = FALSE;
		pDoc->Status.PcrShare[0].nSerial = -1;
		str.Format(_T("US: "));
	}

	if (pDoc->Status.PcrShare[0].bExist && !pDoc->Status.PcrShare[0].bExistF)
		pDoc->Status.PcrShare[0].bExistF = TRUE;
	else if (!pDoc->Status.PcrShare[0].bExist && pDoc->Status.PcrShare[0].bExistF)
		pDoc->Status.PcrShare[0].bExistF = FALSE;

	if (bDualTest)
	{
		if (!pDoc->m_bVsStatusUp && !pDoc->m_bVsStatusDn)
		{
			if (pFrm)
			{
				if (m_sShare[0] != str)
				{
					m_sShare[0] = str;
					pFrm->DispStatusBar(str, 4);
				}
			}
		}
	}
	else
	{
		if (!pDoc->m_bVsStatusUp)
		{
			if (pFrm)
			{
				if (m_sShare[0] != str)
				{
					m_sShare[0] = str;
					pFrm->DispStatusBar(str, 4);
				}
			}
		}
	}
}

void CGvisR2R_PunchView::ChkShareDn()
{
	CString str, str2;
	int nSerial, nAoiDnAutoSerial;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (ChkShareDn(nSerial))
	{
		str.Format(_T("DS: %d"), nSerial);
		pDoc->Status.PcrShare[1].bExist = TRUE;
		pDoc->Status.PcrShare[1].nSerial = nSerial;
		if (m_pMpe)
		{
			str2.Format(_T("PCR파일 Received - DS: %d"), nSerial);
			pDoc->LogAuto(str2);
		}
	}
	else
	{
		pDoc->Status.PcrShare[1].bExist = FALSE;
		pDoc->Status.PcrShare[1].nSerial = -1;
		str.Format(_T("DS: "));
	}

	if (pDoc->Status.PcrShare[1].bExist && !pDoc->Status.PcrShare[1].bExistF)
		pDoc->Status.PcrShare[1].bExistF = TRUE;
	else if (!pDoc->Status.PcrShare[1].bExist && pDoc->Status.PcrShare[1].bExistF)
		pDoc->Status.PcrShare[1].bExistF = FALSE;

	if (bDualTest)
	{
		if (!pDoc->m_bVsStatusUp && !pDoc->m_bVsStatusDn)
		{
			if (pFrm)
			{
				if (m_sShare[1] != str)
				{
					m_sShare[1] = str;
					pFrm->DispStatusBar(str, 2);
				}
			}
		}
	}
}

BOOL CGvisR2R_PunchView::ChkShare(int &nSerial)
{
	int nS0, nS1;
	BOOL b0 = ChkShareUp(nS0);
	BOOL b1 = ChkShareDn(nS1);

	if (!b0 || !b1)
	{
		nSerial = -1;
		return FALSE;
	}
	else if (nS0 != nS1)
	{
		nSerial = -1;
		return FALSE;
	}

	nSerial = nS0;
	return TRUE;
}

BOOL CGvisR2R_PunchView::ChkShareUp(int &nSerial)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareUp + _T("*.pcr"));
	if (!bExist)
		return FALSE; // pcr파일이 존재하지 않음.

	int nPos;
	CString sFileName, sSerial;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFileName = cFile.GetFileName();
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
				sSerial = sFileName.Left(nPos);

			nSerial = _tstoi(sSerial);
			if (nSerial > 0)
				return TRUE;
			else
			{
				nSerial = 0;
				return FALSE;
			}
		}
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::ChkShareDn(int &nSerial)
{
	CFileFind cFile;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareDn + _T("*.pcr"));
	if (!bExist)
		return FALSE; // pcr파일이 존재하지 않음.

	int nPos;
	CString sFileName, sSerial;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			// 파일명을 얻음.
			sFileName = cFile.GetFileName();
			nPos = sFileName.ReverseFind('.');
			if (nPos > 0)
				sSerial = sFileName.Left(nPos);

			nSerial = _tstoi(sSerial);
			if (nSerial > 0)
				return TRUE;
			else
			{
				nSerial = 0;
				return FALSE;
			}
		}
	}

	return FALSE;
}


BOOL CGvisR2R_PunchView::ChkBufIdx(int* pSerial, int nTot)
{
	if (nTot < 2)
		return TRUE;

	for (int i = 0; i<(nTot - 1); i++)
	{
		if (pSerial[i + 1] != pSerial[i] + 1)
			return FALSE;
	}
	return TRUE;
}


void CGvisR2R_PunchView::SwapUp(__int64 *num1, __int64 *num2) 	// 위치 바꾸는 함수
{
	__int64 temp;

	temp = *num2;
	*num2 = *num1;
	*num1 = temp;
}

BOOL CGvisR2R_PunchView::SortingInUp(CString sPath, int nIndex)
{
	struct _stat buf;
	struct tm *t;

	CString sMsg, sFileName, sSerial;
	int nPos, nSerial;
	char filename[MAX_PATH];
	StringToChar(sPath, filename);

	if (_stat(filename, &buf) != 0)
	{
		sMsg.Format(_T("일시정지 - Failed getting information."));
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}
	else
	{
		sFileName = sPath;
		nPos = sFileName.ReverseFind('.');
		if (nPos > 0)
		{
			sSerial = sFileName.Left(nPos);
			sSerial = sSerial.Right(4);
		}

		nSerial = _tstoi(sSerial);

		t = localtime(&buf.st_ctime);

		CString sYear, sMonth, sDay, sHour, sMin, sSec;
		sYear.Format(_T("%04d"), t->tm_year + 1900);
		sMonth.Format(_T("%02d"), t->tm_mon + 1);
		sDay.Format(_T("%02d"), t->tm_mday);
		sHour.Format(_T("%02d"), t->tm_hour);
		sMin.Format(_T("%02d"), t->tm_min);
		sSec.Format(_T("%02d"), t->tm_sec);

		__int64 nYear = _tstoi(sYear);
		__int64 nMonth = _tstoi(sMonth);
		__int64 nDay = _tstoi(sDay);
		__int64 nHour = _tstoi(sHour);
		__int64 nMin = _tstoi(sMin);
		__int64 nSec = _tstoi(sSec);

		m_nBufSerialSorting[0][nIndex] = nYear * 100000000000000 + nMonth * 1000000000000 + nDay * 10000000000 +
			nHour * 100000000 + nMin * 1000000 + nSec * 10000 + nSerial;
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::SortingOutUp(int* pSerial, int nTot)
{
	int i, k;

	for (k = 0; k < nTot; k++) 			// 버블 정렬 소스 시작
	{
		for (i = 0; i < (nTot - 1) - k; i++)
		{

			if (m_nBufSerialSorting[0][i] > m_nBufSerialSorting[0][i + 1])
			{
				SwapUp(&m_nBufSerialSorting[0][i + 1], &m_nBufSerialSorting[0][i]);
			}
		}
	}									// 버블 정렬 소스 끝

	for (i = 0; i < nTot; i++)
	{
		pSerial[i] = (int)(m_nBufSerialSorting[0][i] % 10000);
	}
	return TRUE;
}

BOOL CGvisR2R_PunchView::ChkBufUp(int* pSerial, int &nTot)
{
	CFileFind cFile, cFile2;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufUp + _T("*.pcr"));
	BOOL bExist2 = cFile2.FindFile(pDoc->WorkingInfo.System.sPathVsDummyBufUp + _T("*.pcr"));
	if (!bExist && !bExist2)
	{
		pDoc->m_bBufEmpty[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bBufEmpty[0]"), pDoc->m_bBufEmpty[0]);
		if (!pDoc->m_bBufEmptyF[0])
		{
			pDoc->m_bBufEmptyF[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bBufEmptyF[0]"), pDoc->m_bBufEmptyF[0]);		// 최초 한번 버퍼가 비어있으면(초기화를 하고 난 이후) TRUE.
		}
		return FALSE; // pcr파일이 존재하지 않음.
	}

	int nPos, nSerial;

	CString sFileName, sSerial;
	CString sNewName;

	nTot = 0;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			sFileName = cFile.GetFileName();

			if (!SortingInUp(pDoc->WorkingInfo.System.sPathVrsBufUp + sFileName, nTot))
				return FALSE;

			nTot++;
		}
	}

	BOOL bRtn = SortingOutUp(pSerial, nTot);

	if (nTot == 0)
	{
		pDoc->m_bBufEmpty[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bBufEmpty[0]"), pDoc->m_bBufEmpty[0]);
	}
	else
	{
		pDoc->m_bBufEmpty[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bBufEmpty[0]"), pDoc->m_bBufEmpty[0]);
		m_bIsBuf[0] = TRUE;
	}

	return (bRtn);
}

BOOL CGvisR2R_PunchView::ChkBufDn(int* pSerial, int &nTot)
{
	CFileFind cFile, cFile2;
	BOOL bExist = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufDn + _T("*.pcr"));
	BOOL bExist2 = cFile2.FindFile(pDoc->WorkingInfo.System.sPathVsDummyBufDn + _T("*.pcr"));
	if (!bExist && !bExist2)
	{
		pDoc->m_bBufEmpty[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bBufEmpty[1]"), pDoc->m_bBufEmpty[1]);
		if (!pDoc->m_bBufEmptyF[1])
		{
			pDoc->m_bBufEmptyF[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bBufEmptyF[1]"), pDoc->m_bBufEmptyF[1]);
		}
		return FALSE; // pcr파일이 존재하지 않음.
	}

	int nPos, nSerial;

	CString sFileName, sSerial;
	nTot = 0;
	while (bExist)
	{
		bExist = cFile.FindNextFile();
		if (cFile.IsDots()) continue;
		if (!cFile.IsDirectory())
		{
			sFileName = cFile.GetFileName();

			if (!SortingInDn(pDoc->WorkingInfo.System.sPathVrsBufDn + sFileName, nTot))
				return FALSE;

			nTot++;
		}
	}


	BOOL bRtn = SortingOutDn(pSerial, nTot);

	if (nTot == 0)
	{
		pDoc->m_bBufEmpty[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bBufEmpty[1]"), pDoc->m_bBufEmpty[1]);
	}
	else
	{
		pDoc->m_bBufEmpty[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bBufEmpty[1]"), pDoc->m_bBufEmpty[1]);
		m_bIsBuf[1] = TRUE;
	}

	return (bRtn);
}

void CGvisR2R_PunchView::SwapDn(__int64 *num1, __int64 *num2) 	// 위치 바꾸는 함수
{
	__int64 temp;

	temp = *num2;
	*num2 = *num1;
	*num1 = temp;
}

BOOL CGvisR2R_PunchView::SortingInDn(CString sPath, int nIndex)
{
	struct _stat buf;
	struct tm *t;

	CString sMsg, sFileName, sSerial;
	int nPos, nSerial;
	char filename[MAX_PATH];
	StringToChar(sPath, filename);

	if (_stat(filename, &buf) != 0)
	{
		sMsg.Format(_T("일시정지 - Failed getting information."));
		//MsgBox(sMsg);
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}
	else
	{
		sFileName = sPath;
		nPos = sFileName.ReverseFind('.');
		if (nPos > 0)
		{
			sSerial = sFileName.Left(nPos);
			sSerial = sSerial.Right(4);
		}

		nSerial = _tstoi(sSerial);

		t = localtime(&buf.st_ctime);

		CString sYear, sMonth, sDay, sHour, sMin, sSec;
		sYear.Format(_T("%04d"), t->tm_year + 1900);
		sMonth.Format(_T("%02d"), t->tm_mon + 1);
		sDay.Format(_T("%02d"), t->tm_mday);
		sHour.Format(_T("%02d"), t->tm_hour);
		sMin.Format(_T("%02d"), t->tm_min);
		sSec.Format(_T("%02d"), t->tm_sec);

		__int64 nYear = _tstoi(sYear);
		__int64 nMonth = _tstoi(sMonth);
		__int64 nDay = _tstoi(sDay);
		__int64 nHour = _tstoi(sHour);
		__int64 nMin = _tstoi(sMin);
		__int64 nSec = _tstoi(sSec);

		m_nBufSerialSorting[1][nIndex] = nYear * 100000000000000 + nMonth * 1000000000000 + nDay * 10000000000 +
			nHour * 100000000 + nMin * 1000000 + nSec * 10000 + nSerial;

	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::SortingOutDn(int* pSerial, int nTot)
{
	int i, k;

	for (k = 0; k < nTot; k++) 			// 버블 정렬 소스 시작
	{
		for (i = 0; i < (nTot - 1) - k; i++)
		{

			if (m_nBufSerialSorting[1][i] > m_nBufSerialSorting[1][i + 1])
			{
				SwapUp(&m_nBufSerialSorting[1][i + 1], &m_nBufSerialSorting[1][i]);
			}
		}
	}									// 버블 정렬 소스 끝

	for (i = 0; i < nTot; i++)
	{
		pSerial[i] = (int)(m_nBufSerialSorting[1][i] % 10000);
	}
	return TRUE;
}

BOOL CGvisR2R_PunchView::ChkYield() // 수율 양호 : TRUE , 수율 불량 : FALSE
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nGood = 0, nBad = 0, nTot;
	double dRatio;
	CString sMsg;

	if (bDualTest)
	{
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad);
		else
		{
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			sMsg.Format(_T("일시정지 - Failed ChkYield()."));
			MsgBox(sMsg);
			return FALSE;
		}
	}
	else
	{
		if (pDoc->m_pReelMapUp)
			pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad);
		else
		{
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			sMsg.Format(_T("일시정지 - Failed ChkYield()."));
			MsgBox(sMsg);
			return FALSE;
		}
	}

	nTot = (nGood + nBad);
	double dTotLmt = _tstof(pDoc->WorkingInfo.LastJob.sLmtTotYld);
	if (dTotLmt > 0.0)
	{
		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		if (dRatio < dTotLmt)
		{
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			sMsg.Format(_T("일시정지 - 전체 수율 제한범위 : %.1f 미달 ( %.1f )"), dTotLmt, dRatio);
			MsgBox(sMsg);
			return FALSE;
		}
	}
	double dPrtLmt = _tstof(pDoc->WorkingInfo.LastJob.sLmtPatlYld);
	if (dPrtLmt > 0.0)
	{
		if (bDualTest)
		{
			if (pDoc->m_Yield[2].IsValid())
			{
				dRatio = pDoc->m_Yield[2].GetYield();

				if (dRatio < dPrtLmt)
				{
					Buzzer(TRUE, 0);
					TowerLamp(RGB_RED, TRUE);
					Stop();
					sMsg.Format(_T("일시정지 - 구간 수율 제한범위 : %.1f 미달 ( %.1f )"), dPrtLmt, dRatio);
					MsgBox(sMsg);
					return FALSE;
				}
			}
		}
		else
		{
			if (pDoc->m_Yield[0].IsValid())
			{
				dRatio = pDoc->m_Yield[0].GetYield();

				if (dRatio < dPrtLmt)
				{
					Buzzer(TRUE, 0);
					TowerLamp(RGB_RED, TRUE);
					Stop();
					sMsg.Format(_T("일시정지 - 구간 수율 제한범위 : %.1f 미달 ( %.1f )"), dPrtLmt, dRatio);
					MsgBox(sMsg);
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::ChkSaftySen() // 감지 : TRUE , 비감지 : FALSE
{
	if (pDoc->WorkingInfo.LastJob.bMkSftySen)
	{
		if (pDoc->Status.bSensSaftyMk && !pDoc->Status.bSensSaftyMkF)
		{
			pDoc->Status.bSensSaftyMkF = TRUE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);			
			m_bTIM_SAFTY_STOP = TRUE;//MsgBox(_T("일시정지 - 마킹부 안전센서가 감지되었습니다."));
			SetTimer(TIM_SAFTY_STOP, 100, NULL);
		}
		else if (!pDoc->Status.bSensSaftyMk && pDoc->Status.bSensSaftyMkF)
		{
			pDoc->Status.bSensSaftyMkF = FALSE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
	}

	return (pDoc->Status.bSensSaftyMk);
}

void CGvisR2R_PunchView::ResetMotion()
{
	for (int i = 0; i<MAX_MS; i++)
	{
		ResetMotion(i);

		if (i<MAX_AXIS)
		{
			while (!m_pMotion->IsServoOn(i))
			{
				if (i == MS_X0 || i == MS_Y0)
					pView->m_pMotion->Clear(MS_X0Y0);
				else if (i == MS_X1 || i == MS_Y1)
					pView->m_pMotion->Clear(MS_X1Y1);
				else
					pView->m_pMotion->Clear(i);
				Sleep(30);
				m_pMotion->ServoOnOff(i, TRUE);
				Sleep(30);
			}
		}
	}
}

void CGvisR2R_PunchView::ResetMotion(int nMsId)
{
	if (m_pDlgMenu02)
		m_pDlgMenu02->ResetMotion(nMsId);
}

unsigned long CGvisR2R_PunchView::ChkDoor() // 0: All Closed , Open Door Index : Doesn't all closed. (Bit3: F, Bit2: L, Bit1: R, Bit0; B)
{
	unsigned long ulOpenDoor = 0;

	if (pDoc->WorkingInfo.LastJob.bAoiUpDrSen)
	{
		if (pDoc->Status.bDoorAoi[DOOR_FM_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_FM_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 0);
			pDoc->Status.bDoorAoiF[DOOR_FM_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FM_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_FM_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 0);
			pDoc->Status.bDoorAoiF[DOOR_FM_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			//pView->DispStsBar(_T("정지-5"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 전면 중앙 도어 Open"));
		}

		if (pDoc->Status.bDoorAoi[DOOR_FL_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_FL_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 1);
			pDoc->Status.bDoorAoiF[DOOR_FL_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FL_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_FL_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 1);
			pDoc->Status.bDoorAoiF[DOOR_FL_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 전면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorAoi[DOOR_FR_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_FR_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 2);
			pDoc->Status.bDoorAoiF[DOOR_FR_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FR_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_FR_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 2);
			pDoc->Status.bDoorAoiF[DOOR_FR_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 전면 우측 도어 Open"));
		}

		if (pDoc->Status.bDoorAoi[DOOR_BM_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_BM_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 3);
			pDoc->Status.bDoorAoiF[DOOR_BM_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BM_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_BM_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 3);
			pDoc->Status.bDoorAoiF[DOOR_BM_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 후면 중앙 도어 Open"));
		}

		if (pDoc->Status.bDoorAoi[DOOR_BL_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_BL_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 4);
			pDoc->Status.bDoorAoiF[DOOR_BL_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BL_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_BL_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 4);
			pDoc->Status.bDoorAoiF[DOOR_BL_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 후면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorAoi[DOOR_BR_AOI_UP] && !pDoc->Status.bDoorAoiF[DOOR_BR_AOI_UP])
		{
			ulOpenDoor |= (0x01 << 5);
			pDoc->Status.bDoorAoiF[DOOR_BR_AOI_UP] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BR_AOI_UP] && pDoc->Status.bDoorAoiF[DOOR_BR_AOI_UP])
		{
			ulOpenDoor &= ~(0x01 << 5);
			pDoc->Status.bDoorAoiF[DOOR_BR_AOI_UP] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 상 후면 우측 도어 Open"));
		}
	}

	if (pDoc->WorkingInfo.LastJob.bAoiDnDrSen)
	{
		if (pDoc->Status.bDoorAoi[DOOR_FM_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_FM_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 0);
			pDoc->Status.bDoorAoiF[DOOR_FM_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FM_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_FM_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 0);
			pDoc->Status.bDoorAoiF[DOOR_FM_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 전면 중앙 도어 Open"));
		}

		if (pDoc->Status.bDoorAoi[DOOR_FL_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_FL_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 1);
			pDoc->Status.bDoorAoiF[DOOR_FL_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FL_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_FL_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 1);
			pDoc->Status.bDoorAoiF[DOOR_FL_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 전면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorAoi[DOOR_FR_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_FR_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 2);
			pDoc->Status.bDoorAoiF[DOOR_FR_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_FR_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_FR_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 2);
			pDoc->Status.bDoorAoiF[DOOR_FR_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 전면 우측 도어 Open"));
		}

		if (pDoc->Status.bDoorAoi[DOOR_BM_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_BM_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 3);
			pDoc->Status.bDoorAoiF[DOOR_BM_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BM_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_BM_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 3);
			pDoc->Status.bDoorAoiF[DOOR_BM_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 후면 중앙 도어 Open"));
		}

		if (pDoc->Status.bDoorAoi[DOOR_BL_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_BL_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 4);
			pDoc->Status.bDoorAoiF[DOOR_BL_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BL_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_BL_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 4);
			pDoc->Status.bDoorAoiF[DOOR_BL_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 후면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorAoi[DOOR_BR_AOI_DN] && !pDoc->Status.bDoorAoiF[DOOR_BR_AOI_DN])
		{
			ulOpenDoor |= (0x01 << 5);
			pDoc->Status.bDoorAoiF[DOOR_BR_AOI_DN] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorAoi[DOOR_BR_AOI_DN] && pDoc->Status.bDoorAoiF[DOOR_BR_AOI_DN])
		{
			ulOpenDoor &= ~(0x01 << 5);
			pDoc->Status.bDoorAoiF[DOOR_BR_AOI_DN] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 검사부 하 후면 우측 도어 Open"));
		}
	}

	if (pDoc->WorkingInfo.LastJob.bMkDrSen)
	{
		if (pDoc->Status.bDoorMk[DOOR_FL_MK] && !pDoc->Status.bDoorMkF[DOOR_FL_MK])
		{
			ulOpenDoor |= (0x01 << 6);
			pDoc->Status.bDoorMkF[DOOR_FL_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorMk[DOOR_FL_MK] && pDoc->Status.bDoorMkF[DOOR_FL_MK])
		{
			ulOpenDoor &= ~(0x01 << 6);
			pDoc->Status.bDoorMkF[DOOR_FL_MK] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 마킹부 전면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorMk[DOOR_FR_MK] && !pDoc->Status.bDoorMkF[DOOR_FR_MK])
		{
			ulOpenDoor |= (0x01 << 7);
			pDoc->Status.bDoorMkF[DOOR_FR_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorMk[DOOR_FR_MK] && pDoc->Status.bDoorMkF[DOOR_FR_MK])
		{
			ulOpenDoor &= ~(0x01 << 7);
			pDoc->Status.bDoorMkF[DOOR_FR_MK] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 마킹부 전면 우측 도어 Open"));
		}

		if (pDoc->Status.bDoorMk[DOOR_BL_MK] && !pDoc->Status.bDoorMkF[DOOR_BL_MK])
		{
			ulOpenDoor |= (0x01 << 8);
			pDoc->Status.bDoorMkF[DOOR_BL_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorMk[DOOR_BL_MK] && pDoc->Status.bDoorMkF[DOOR_BL_MK])
		{
			ulOpenDoor &= ~(0x01 << 8);
			pDoc->Status.bDoorMkF[DOOR_BL_MK] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 마킹부 후면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorMk[DOOR_BR_MK] && !pDoc->Status.bDoorMkF[DOOR_BR_MK])
		{
			ulOpenDoor |= (0x01 << 9);
			pDoc->Status.bDoorMkF[DOOR_BR_MK] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorMk[DOOR_BR_MK] && pDoc->Status.bDoorMkF[DOOR_BR_MK])
		{
			ulOpenDoor &= ~(0x01 << 9);
			pDoc->Status.bDoorMkF[DOOR_BR_MK] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 마킹부 후면 우측 도어 Open"));
		}
	}

	if (pDoc->WorkingInfo.LastJob.bEngvDrSen)
	{
		if (pDoc->Status.bDoorEngv[DOOR_FL_ENGV] && !pDoc->Status.bDoorEngvF[DOOR_FL_ENGV])
		{
			ulOpenDoor |= (0x01 << 6);
			pDoc->Status.bDoorEngvF[DOOR_FL_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorEngv[DOOR_FL_ENGV] && pDoc->Status.bDoorEngvF[DOOR_FL_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 6);
			pDoc->Status.bDoorEngvF[DOOR_FL_ENGV] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 각인부 전면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorEngv[DOOR_FR_ENGV] && !pDoc->Status.bDoorEngvF[DOOR_FR_ENGV])
		{
			ulOpenDoor |= (0x01 << 7);
			pDoc->Status.bDoorEngvF[DOOR_FR_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorEngv[DOOR_FR_ENGV] && pDoc->Status.bDoorEngvF[DOOR_FR_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 7);
			pDoc->Status.bDoorEngvF[DOOR_FR_ENGV] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 각인부 전면 우측 도어 Open"));
		}

		if (pDoc->Status.bDoorEngv[DOOR_BL_ENGV] && !pDoc->Status.bDoorEngvF[DOOR_BL_ENGV])
		{
			ulOpenDoor |= (0x01 << 8);
			pDoc->Status.bDoorEngvF[DOOR_BL_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorEngv[DOOR_BL_ENGV] && pDoc->Status.bDoorEngvF[DOOR_BL_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 8);
			pDoc->Status.bDoorEngvF[DOOR_BL_ENGV] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 각인부 후면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorEngv[DOOR_BR_ENGV] && !pDoc->Status.bDoorEngvF[DOOR_BR_ENGV])
		{
			ulOpenDoor |= (0x01 << 9);
			pDoc->Status.bDoorEngvF[DOOR_BR_ENGV] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorEngv[DOOR_BR_ENGV] && pDoc->Status.bDoorEngvF[DOOR_BR_ENGV])
		{
			ulOpenDoor &= ~(0x01 << 9);
			pDoc->Status.bDoorEngvF[DOOR_BR_ENGV] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 각인부 후면 우측 도어 Open"));
		}
	}

	if (pDoc->WorkingInfo.LastJob.bUclDrSen)
	{
		if (pDoc->Status.bDoorUc[DOOR_FL_UC] && !pDoc->Status.bDoorUcF[DOOR_FL_UC])
		{
			ulOpenDoor |= (0x01 << 10);
			pDoc->Status.bDoorUcF[DOOR_FL_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorUc[DOOR_FL_UC] && pDoc->Status.bDoorUcF[DOOR_FL_UC])
		{
			ulOpenDoor &= ~(0x01 << 10);
			pDoc->Status.bDoorUcF[DOOR_FL_UC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 언코일러부 전면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorUc[DOOR_FR_UC] && !pDoc->Status.bDoorUcF[DOOR_FR_UC])
		{
			ulOpenDoor |= (0x01 << 11);
			pDoc->Status.bDoorUcF[DOOR_FR_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorUc[DOOR_FR_UC] && pDoc->Status.bDoorUcF[DOOR_FR_UC])
		{
			ulOpenDoor &= ~(0x01 << 11);
			pDoc->Status.bDoorUcF[DOOR_FR_UC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 언코일러부 측면 도어 Open"));
		}

		if (pDoc->Status.bDoorUc[DOOR_BL_UC] && !pDoc->Status.bDoorUcF[DOOR_BL_UC])
		{
			ulOpenDoor |= (0x01 << 12);
			pDoc->Status.bDoorUcF[DOOR_BL_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorUc[DOOR_BL_UC] && pDoc->Status.bDoorUcF[DOOR_BL_UC])
		{
			ulOpenDoor &= ~(0x01 << 12);
			pDoc->Status.bDoorUcF[DOOR_BL_UC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 언코일러부 후면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorUc[DOOR_BR_UC] && !pDoc->Status.bDoorUcF[DOOR_BR_UC])
		{
			ulOpenDoor |= (0x01 << 13);
			pDoc->Status.bDoorUcF[DOOR_BR_UC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorUc[DOOR_BR_UC] && pDoc->Status.bDoorUcF[DOOR_BR_UC])
		{
			ulOpenDoor &= ~(0x01 << 13);
			pDoc->Status.bDoorUcF[DOOR_BR_UC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 언코일러부 후면 우측 도어 Open"));
		}
	}

	if (pDoc->WorkingInfo.LastJob.bRclDrSen)
	{
		if (pDoc->Status.bDoorRe[DOOR_FR_RC] && !pDoc->Status.bDoorReF[DOOR_FR_RC])
		{
			ulOpenDoor |= (0x01 << 15);
			pDoc->Status.bDoorReF[DOOR_FR_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorRe[DOOR_FR_RC] && pDoc->Status.bDoorReF[DOOR_FR_RC])
		{
			ulOpenDoor &= ~(0x01 << 15);
			pDoc->Status.bDoorReF[DOOR_FR_RC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			pView->DispStsBar(_T("정지-19"), 0);
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 리코일러부 전면 우측 도어 Open"));
		}

		if (pDoc->Status.bDoorRe[DOOR_S_RC] && !pDoc->Status.bDoorReF[DOOR_S_RC])
		{
			ulOpenDoor |= (0x01 << 16);
			pDoc->Status.bDoorReF[DOOR_S_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorRe[DOOR_S_RC] && pDoc->Status.bDoorReF[DOOR_S_RC])
		{
			ulOpenDoor &= ~(0x01 << 16);
			pDoc->Status.bDoorReF[DOOR_S_RC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 리코일러부 측면 도어 Open"));
		}

		if (pDoc->Status.bDoorRe[DOOR_BL_RC] && !pDoc->Status.bDoorReF[DOOR_BL_RC])
		{
			ulOpenDoor |= (0x01 << 17);
			pDoc->Status.bDoorReF[DOOR_BL_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorRe[DOOR_BL_RC] && pDoc->Status.bDoorReF[DOOR_BL_RC])
		{
			ulOpenDoor &= ~(0x01 << 17);
			pDoc->Status.bDoorReF[DOOR_BL_RC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 리코일러부 후면 좌측 도어 Open"));
		}

		if (pDoc->Status.bDoorRe[DOOR_BR_RC] && !pDoc->Status.bDoorReF[DOOR_BR_RC])
		{
			ulOpenDoor |= (0x01 << 18);
			pDoc->Status.bDoorReF[DOOR_BR_RC] = TRUE;
			Buzzer(FALSE, 0);
			Sleep(300);
		}
		else if (!pDoc->Status.bDoorRe[DOOR_BR_RC] && pDoc->Status.bDoorReF[DOOR_BR_RC])
		{
			ulOpenDoor &= ~(0x01 << 18);
			pDoc->Status.bDoorReF[DOOR_BR_RC] = FALSE;
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(_T("일시정지 - 리코일러부 후면 우측 도어 Open"));
		}
	}

	return ulOpenDoor;
}

void CGvisR2R_PunchView::ChkEmg()
{
	if (pDoc->Status.bEmgAoi[EMG_F_AOI_UP] && !pDoc->Status.bEmgAoiF[EMG_F_AOI_UP])
	{
		pDoc->Status.bEmgAoiF[EMG_F_AOI_UP] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 검사부 상 전면 스위치"));
	}
	else if (!pDoc->Status.bEmgAoi[EMG_F_AOI_UP] && pDoc->Status.bEmgAoiF[EMG_F_AOI_UP])
	{
		pDoc->Status.bEmgAoiF[EMG_F_AOI_UP] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgAoi[EMG_B_AOI_UP] && !pDoc->Status.bEmgAoiF[EMG_B_AOI_UP])
	{
		pDoc->Status.bEmgAoiF[EMG_B_AOI_UP] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 검사부 상 후면 스위치"));
	}
	else if (!pDoc->Status.bEmgAoi[EMG_B_AOI_UP] && pDoc->Status.bEmgAoiF[EMG_B_AOI_UP])
	{
		pDoc->Status.bEmgAoiF[EMG_B_AOI_UP] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgAoi[EMG_F_AOI_DN] && !pDoc->Status.bEmgAoiF[EMG_F_AOI_DN])
	{
		pDoc->Status.bEmgAoiF[EMG_F_AOI_DN] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 검사부 하 전면 스위치"));
	}
	else if (!pDoc->Status.bEmgAoi[EMG_F_AOI_DN] && pDoc->Status.bEmgAoiF[EMG_F_AOI_DN])
	{
		pDoc->Status.bEmgAoiF[EMG_F_AOI_DN] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgAoi[EMG_B_AOI_DN] && !pDoc->Status.bEmgAoiF[EMG_B_AOI_DN])
	{
		pDoc->Status.bEmgAoiF[EMG_B_AOI_DN] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 검사부 하 후면 스위치"));
	}
	else if (!pDoc->Status.bEmgAoi[EMG_B_AOI_DN] && pDoc->Status.bEmgAoiF[EMG_B_AOI_DN])
	{
		pDoc->Status.bEmgAoiF[EMG_B_AOI_DN] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgMk[EMG_M_MK] && !pDoc->Status.bEmgMkF[EMG_M_MK])
	{
		pDoc->Status.bEmgMkF[EMG_M_MK] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 마킹부 메인 스위치"));
	}
	else if (!pDoc->Status.bEmgMk[EMG_M_MK] && pDoc->Status.bEmgMkF[EMG_M_MK])
	{
		pDoc->Status.bEmgMkF[EMG_M_MK] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgMk[EMG_B_MK] && !pDoc->Status.bEmgMkF[EMG_B_MK])
	{
		pDoc->Status.bEmgMkF[EMG_B_MK] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 마킹부 스위치"));
	}
	else if (!pDoc->Status.bEmgMk[EMG_B_MK] && pDoc->Status.bEmgMkF[EMG_B_MK])
	{
		pDoc->Status.bEmgMkF[EMG_B_MK] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgUc && !pDoc->Status.bEmgUcF)
	{
		pDoc->Status.bEmgUcF = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 언코일러부 스위치"));
	}
	else if (!pDoc->Status.bEmgUc && pDoc->Status.bEmgUcF)
	{
		pDoc->Status.bEmgUcF = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgRc && !pDoc->Status.bEmgRcF)
	{
		pDoc->Status.bEmgRcF = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 리코일러부 스위치"));
	}
	else if (!pDoc->Status.bEmgRc && pDoc->Status.bEmgRcF)
	{
		pDoc->Status.bEmgRcF = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgEngv[0] && !pDoc->Status.bEmgEngvF[0])
	{
		pDoc->Status.bEmgEngvF[0] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 각인부 모니터"));
	}
	else if (!pDoc->Status.bEmgEngv[0] && pDoc->Status.bEmgEngvF[0])
	{
		pDoc->Status.bEmgEngvF[0] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}

	if (pDoc->Status.bEmgEngv[1] && !pDoc->Status.bEmgEngvF[1])
	{
		pDoc->Status.bEmgEngvF[1] = TRUE;
		m_bSwStopNow = TRUE;
		m_bSwRunF = FALSE;
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
		MsgBox(_T("비상정지 - 각인부 스위치"));
	}
	else if (!pDoc->Status.bEmgEngv[1] && pDoc->Status.bEmgEngvF[1])
	{
		pDoc->Status.bEmgEngvF[1] = FALSE;
		Buzzer(FALSE, 0);
		Sleep(300);
		ResetMotion();
	}
}

int CGvisR2R_PunchView::ChkSerial() // // 0: Continue, -: Previous, +: Discontinue --> 0: Same Serial, -: Decrese Serial, +: Increase Serial
{
	int nSerial0 = GetBuffer0(); // 첫번째 버퍼 시리얼 : 상하 시리얼이 다르면 0
	int nSerial1 = GetBuffer1(); // 두번째 버퍼 시리얼 : 상하 시리얼이 다르면 0
	int nLastShot = pDoc->GetLastShotMk();
	return (nSerial0 - nLastShot);
}

void CGvisR2R_PunchView::ChkBuf()
{
	if (!m_bShift2Mk)
		ChkBufUp();
	if (!m_bShift2Mk)
		ChkBufDn();
}

void CGvisR2R_PunchView::ChkBufUp()
{
	CString str, sTemp;

	str = _T("UB: ");
	if (ChkBufUp(m_pBufSerial[0], m_nBufTot[0]))
	{
		for (int i = 0; i<m_nBufTot[0]; i++)
		{
			if (m_bShift2Mk)
				return;

			DelOverLotEndSerialUp(m_pBufSerial[0][i]);

			if (i == m_nBufTot[0] - 1)
				sTemp.Format(_T("%d"), m_pBufSerial[0][i]);
			else
				sTemp.Format(_T("%d,"), m_pBufSerial[0][i]);
			str += sTemp;
		}
	}
	else
	{
		m_nBufTot[0] = 0;
	}

	if (pFrm)
	{
		if (m_sBuf[0] != str)
		{
			m_sBuf[0] = str;
			pFrm->DispStatusBar(str, 3);

			pDoc->SetCurrentInfoBufUpTot(m_nBufTot[0]);
			for (int k = 0; k<m_nBufTot[0]; k++)
				pDoc->SetCurrentInfoBufUp(k, m_pBufSerial[0][k]);

			//if (m_nBufTot[0] == 1)
			//{
			//	pDoc->m_nAoiCamInfoStrPcs[0] = GetAoiUpCamMstInfo(); pDoc->SetStatusInt(_T("General"), _T("nAoiCamInfoStrPcs[0]"), pDoc->m_nAoiCamInfoStrPcs[0]);
			//}
		}
	}
}

void CGvisR2R_PunchView::ChkBufDn()
{
	CString str, sTemp;

	str = _T("DB: ");
	if (ChkBufDn(m_pBufSerial[1], m_nBufTot[1]))
	{
		for (int i = 0; i<m_nBufTot[1]; i++)
		{
			if (m_bShift2Mk)
				return;

			DelOverLotEndSerialDn(m_pBufSerial[1][i]);

			if (i == m_nBufTot[1] - 1)
				sTemp.Format(_T("%d"), m_pBufSerial[1][i]);
			else
				sTemp.Format(_T("%d,"), m_pBufSerial[1][i]);
			str += sTemp;
		}
	}
	else
	{
		m_nBufTot[1] = 0;
	}

	if (pFrm)
	{
		if (m_sBuf[1] != str)
		{
			m_sBuf[1] = str;
			pFrm->DispStatusBar(str, 1);

			pDoc->SetCurrentInfoBufDnTot(m_nBufTot[1]);
			for (int k = 0; k < m_nBufTot[1]; k++)
				pDoc->SetCurrentInfoBufDn(k, m_pBufSerial[1][k]);

			//if (m_nBufTot[1] == 1)
			//{
			//	pDoc->m_nAoiCamInfoStrPcs[1] = GetAoiDnCamMstInfo(); pDoc->SetStatusInt(_T("General"), _T("nAoiCamInfoStrPcs[1]"), pDoc->m_nAoiCamInfoStrPcs[1]);
			//}
		}
	}
}

int CGvisR2R_PunchView::GetAoiUpAutoSerial()
{
	int nSerial = 0;
	CString sPath = pDoc->WorkingInfo.System.sPathAoiUpStatusInfo;
	TCHAR szData[512];
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Auto"), _T("nSerial"), NULL, szData, sizeof(szData), sPath))
			nSerial = _ttoi(szData);
		else
			nSerial = -1;
	}
	else
	{
		sMsg.Format(_T("%s파일의 Auto에 nSerial 정보가 없습니다."), sPath);
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}

	return nSerial;
}

void CGvisR2R_PunchView::SetAoiUpAutoSerial(int nSerial)
{
	CString sPath = pDoc->WorkingInfo.System.sPathAoiUpStatusInfo;
	CString str, sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		str.Format(_T("%d"), nSerial);
		::WritePrivateProfileString(_T("Auto"), _T("nSerial"), str, sPath);
	}
	else
	{
		sMsg.Format(_T("%s파일의 Auto에 nSerial 정보가 없습니다."), sPath);
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
}

int CGvisR2R_PunchView::GetAoiUpAutoStep()
{
	TCHAR szData[512];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiUpStatusInfo;
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Auto"), _T("nStep"), NULL, szData, sizeof(szData), sPath))
			return _ttoi(szData);
	}
	else
	{
		sMsg.Format(_T("%s파일의 Auto에 nStep 정보가 없습니다.\r\n상면 AOI가 꺼진 것 같습니다."), sPath);
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}

	return (-1);
}

void CGvisR2R_PunchView::SetAoiUpAutoStep(int nStep)
{
	CString sPath = pDoc->WorkingInfo.System.sPathAoiUpStatusInfo;
	CString str, sMsg;
	str.Format(_T("%d"), nStep);

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		::WritePrivateProfileString(_T("Auto"), _T("nStep"), str, sPath);
	}
	else
	{
		sMsg.Format(_T("%s파일의 Auto에 nStep 정보가 없습니다.\r\n상면 AOI가 꺼진 것 같습니다."), sPath);
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
}

int CGvisR2R_PunchView::GetAoiDnAutoSerial()
{
	int nSerial = 0;
	CString sPath = pDoc->WorkingInfo.System.sPathAoiDnStatusInfo;
	TCHAR szData[512];
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Auto"), _T("nSerial"), NULL, szData, sizeof(szData), sPath))
			nSerial = _ttoi(szData);
		else
			nSerial = -1;
	}
	else
	{
		sMsg.Format(_T("%s파일의 Auto에 nSerial 정보가 없습니다."), sPath);
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}

	return nSerial;
}

void CGvisR2R_PunchView::SetAoiDnAutoSerial(int nSerial)
{
	CString sPath = pDoc->WorkingInfo.System.sPathAoiDnStatusInfo;
	CString str;
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		str.Format(_T("%d"), nSerial);
		::WritePrivateProfileString(_T("Auto"), _T("nSerial"), str, sPath);
	}
	else
	{
		sMsg.Format(_T("%s파일의 Auto에 nSerial 정보가 없습니다."), sPath);
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
}

int CGvisR2R_PunchView::GetAoiDnAutoStep()
{
	TCHAR szData[512];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiDnStatusInfo;
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Auto"), _T("nStep"), NULL, szData, sizeof(szData), sPath))
			return _ttoi(szData);
	}
	else
	{
		sMsg.Format(_T("%s파일의 Auto에 nStep 정보가 없습니다.\r\n하면 AOI가 꺼진 것 같습니다."), sPath);
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}

	return (-1);
}

void CGvisR2R_PunchView::SetAoiDnAutoStep(int nStep)
{
	CString sPath = pDoc->WorkingInfo.System.sPathAoiDnStatusInfo;
	CString str;
	str.Format(_T("%d"), nStep);
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		::WritePrivateProfileString(_T("Auto"), _T("nStep"), str, sPath);
	}
	else
	{
		sMsg.Format(_T("%s파일의 Auto에 nStep 정보가 없습니다.\r\n하면 AOI가 꺼진 것 같습니다."), sPath);
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
}

CString CGvisR2R_PunchView::GetAoiUpAlarmRestartMsg()
{
	CString sMsg = _T("Not Find Message.");
	TCHAR szData[512];
	CString sPath = PATH_ALARM;
	if (0 < ::GetPrivateProfileString(_T("11"), _T("27"), NULL, szData, sizeof(szData), sPath))
		sMsg = CString(szData);

	return sMsg;
}

CString CGvisR2R_PunchView::GetAoiUpAlarmReTestMsg()
{
	CString sMsg = _T("Not Find Message.");
	TCHAR szData[512];
	CString sPath = PATH_ALARM;
	if (0 < ::GetPrivateProfileString(_T("20"), _T("9"), NULL, szData, sizeof(szData), sPath))
		sMsg = CString(szData);

	return sMsg;
}

CString CGvisR2R_PunchView::GetAoiDnAlarmRestartMsg()
{
	CString sMsg = _T("Not Find Message.");
	TCHAR szData[512];
	CString sPath = PATH_ALARM;
	if (0 < ::GetPrivateProfileString(_T("11"), _T("28"), NULL, szData, sizeof(szData), sPath))
		sMsg = CString(szData);

	return sMsg;
}

CString CGvisR2R_PunchView::GetAoiDnAlarmReTestMsg()
{
	CString sMsg = _T("Not Find Message.");
	TCHAR szData[512];
	CString sPath = PATH_ALARM;
	if (0 < ::GetPrivateProfileString(_T("20"), _T("10"), NULL, szData, sizeof(szData), sPath))
		sMsg = CString(szData);

	return sMsg;
}

void CGvisR2R_PunchView::DoIO()
{
	//	ChkEmg();

	DoEmgSens();	//20220607
	DoSaftySens();	//20220603
	DoDoorSens();	//20220607

	DoModeSel();
	DoMainSw();
	DoEngraveSens();

	DoInterlock();

	MonPlcAlm();
	MonDispMain();
	MonPlcSignal();

	if (m_bCycleStop)
	{
		m_bCycleStop = FALSE; pDoc->SetStatus(_T("General"), _T("bCycleStop"), m_bCycleStop);
		Buzzer(TRUE);
		TowerLamp(RGB_YELLOW, TRUE);
		//Stop();
		//MyMsgBox(pDoc->m_sAlmMsg);
		if (!pDoc->m_sAlmMsg.IsEmpty())
		{
			pDoc->LogAuto(pDoc->m_sAlmMsg);
			MsgBox(pDoc->m_sAlmMsg, 0, MB_OK, DEFAULT_TIME_OUT, FALSE);
			MpeWrite(_T("MB400183"), 1); // 알람메시지 확인

			if (pDoc->m_sAlmMsg == m_sAoiUpAlarmReStartMsg || pDoc->m_sAlmMsg == m_sAoiUpAlarmReTestMsg)
			{
				ChkReTestAlarmOnAoiUp();
				//SetAoiUpAutoStep(2); // Wait for AOI 검사시작 신호.
				//Sleep(300);
				//if(m_pMpe)
				//	MpeWrite(Plc.DlgMenu01.ReTestAlarmAoiUp, 1); // 검사부 상부 재작업 (시작신호) : PC가 On시키고 PLC가 Off
			}
			else if(pDoc->m_sAlmMsg == m_sAoiDnAlarmReStartMsg || pDoc->m_sAlmMsg == m_sAoiDnAlarmReTestMsg)
			{
				ChkReTestAlarmOnAoiDn();
				//SetAoiDnAutoStep(2); // Wait for AOI 검사시작 신호.
				//Sleep(300);
				//if(m_pMpe)
				//	MpeWrite(Plc.DlgMenu01.ReTestAlarmAoiDn, 1); // 검사부 하부 재작업 (시작신호) : PC가 On시키고 PLC가 Off
			}
		}
		pDoc->m_sAlmMsg = _T("");
		pDoc->m_sIsAlmMsg = _T("");
		pDoc->m_sPrevAlmMsg = _T("");
	}

	if (pDoc->Status.bManual)
	{
		DoBoxSw();
	}
	else if (pDoc->Status.bAuto)
	{
		DoAutoEng();
		DoAuto();
	}

	// 	DoSignal();
	// 	DoSens();
	// 	DoEmgSw();

	if (IsRun())
	{
		if (m_pDlgMenu01)
		{
			if (m_pDlgMenu01->IsEnableBtn())
				m_pDlgMenu01->EnableBtn(FALSE);
		}
	}
	else
	{
		if (m_pDlgMenu01)
		{
			if (!m_pDlgMenu01->IsEnableBtn())
				m_pDlgMenu01->EnableBtn(TRUE);
		}
	}
}

void CGvisR2R_PunchView::DoSens()
{
	//DoEmgSens();		// SliceIo[4]
	//DoMkSens();			// SliceIo[2]
	// 	DoUcSens1();		// MpeIo[1]
	// 	DoUcSens2();		// MpeIo[2]
	// 	DoUcSens3();		// MpeIo[3]
	// 	DoRcSens1();		// MpeIo[5]
	// 	DoRcSens2();		// MpeIo[6]
	// 	DoRcSens3();		// MpeIo[7]
}

void CGvisR2R_PunchView::DoBoxSw()
{
	// 	DoRcBoxSw();		// MpeIo[4]
	// 	DoAoiBoxSw();		// SliceIo[3]
	// 	DoUcBoxSw();		// MpeIo[0]
}

void CGvisR2R_PunchView::DoEmgSw()
{
	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;
}

void CGvisR2R_PunchView::GetInput()
{
	GetEnc();
	GetMpeSignal();
	DoMainSw();
}

void CGvisR2R_PunchView::GetEnc()
{
	if (!m_pMotion)
		return;

	m_dEnc[AXIS_X0] = m_pMotion->GetActualPosition(AXIS_X0);
	m_dEnc[AXIS_Y0] = m_pMotion->GetActualPosition(AXIS_Y0);
	m_dEnc[AXIS_X1] = m_pMotion->GetActualPosition(AXIS_X1);
	m_dEnc[AXIS_Y1] = m_pMotion->GetActualPosition(AXIS_Y1);
}

void CGvisR2R_PunchView::ChkMRegOut()
{
	m_bChkMpeIoOut = TRUE;
}

void CGvisR2R_PunchView::GetMpeIO()
{
#ifdef USE_MPE
	if (!m_pMpe || !pDoc->m_pMpeIo)
		return;

	m_pMpe->GetMpeIO();
#endif
}

void CGvisR2R_PunchView::GetMpeSignal()
{
#ifdef USE_MPE
	if (!m_pMpe || !pDoc->m_pMpeSignal)
		return;
	m_pMpe->GetMpeSignal();
#endif
}


void CGvisR2R_PunchView::GetMpeData()
{
#ifdef USE_MPE
	if (!m_pMpe || !pDoc->m_pMpeData)
		return;

	// MpeData
	int nSize, nIdx, nLoop, nSt, k;
	int nInSeg = pDoc->MkIo.MpeData.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeData.nOutSeg;

	// MpeData - [In]
	int *pSeg0 = new int[nInSeg];
	long **pData0 = new long*[nInSeg];
	for (nIdx = 0; nIdx<nInSeg; nIdx++)
	{
		pData0[nIdx] = new long[16];
		memset(pData0[nIdx], 0, sizeof(long) * 16);
	}

	// Group Read...
	int nGrpStep = pDoc->MkIo.MpeData.nGrpStep;
	nSt = pDoc->MkIo.MpeData.nGrpInSt;
	for (nLoop = 0; nLoop<pDoc->MkIo.MpeData.nGrpIn; nLoop++)
	{
		for (nIdx = 0; nIdx<nGrpStep; nIdx++)
			pSeg0[nIdx] = nIdx + nSt;
		nSize = m_pMpe->ReadData(pSeg0, nGrpStep, pData0);

		for (nIdx = 0; nIdx<nGrpStep; nIdx++)
		{
			for (k = 0; k<16; k++)
				pDoc->m_pMpeData[nIdx + nSt][k] = pData0[nIdx][k];
		}

		nSt += nGrpStep;
	}
	for (nIdx = 0; nIdx<nInSeg; nIdx++)
		delete[] pData0[nIdx];
	delete[] pData0;
	delete[] pSeg0;

	m_bChkMpeIoOut = FALSE;

	// MpeData - [Out]
	int *pSeg2 = new int[nOutSeg];
	long **pData2 = new long*[nOutSeg];
	for (nIdx = 0; nIdx<nOutSeg; nIdx++)
	{
		pData2[nIdx] = new long[16];
		memset(pData2[nIdx], 0, sizeof(unsigned long) * 16);
	}

	// Group Read...
	nSt = pDoc->MkIo.MpeData.nGrpOutSt;
	for (nLoop = 0; nLoop<pDoc->MkIo.MpeData.nGrpOut; nLoop++)
	{
		for (nIdx = 0; nIdx<nGrpStep; nIdx++)
			pSeg2[nIdx] = nIdx + nSt;
		nSize = m_pMpe->ReadData(pSeg2, nGrpStep, pData2);

		for (nIdx = 0; nIdx<nGrpStep; nIdx++)
		{
			for (k = 0; k<16; k++)
				pDoc->m_pMpeData[nIdx + nSt][k] = pData2[nIdx][k];
		}

		nSt += nGrpStep;
	}
	for (nIdx = 0; nIdx<nOutSeg; nIdx++)
		delete[] pData2[nIdx];
	delete[] pData2;
	delete[] pSeg2;
#endif
}

int CGvisR2R_PunchView::GetTime(int nSel)
{

	time_t osBinTime;			// C run-time time (defined in <time.h>)
	time(&osBinTime);		// Get the current time from the 
							// operating system.
	CTime Tim(osBinTime);

	switch (nSel)
	{
	case YEAR:
		return Tim.GetYear();
	case MONTH:
		return Tim.GetMonth();
	case DAY:
		return Tim.GetDay();
	case HOUR:
		return Tim.GetHour();
	case MINUTE:
		return Tim.GetMinute();
	case SEC:
		return Tim.GetSecond();
	}

	return 0;
}

CString CGvisR2R_PunchView::GetTime()
{
	CString strVal;
	time_t osBinTime;			// C run-time time (defined in <time.h>)
	time(&osBinTime);		// Get the current time from the 
							// operating system.
	CTime Tim(osBinTime);

	int nYear = Tim.GetYear();
	int nMonth = Tim.GetMonth();
	int nDay = Tim.GetDay();
	int nHour = Tim.GetHour();
	int nMinute = Tim.GetMinute();
	int nSec = Tim.GetSecond();

	strVal.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMinute, nSec);
	return strVal;
}

CString CGvisR2R_PunchView::GetTime(stLotTime &LotTime)
{
	CString strVal;
	time_t osBinTime;			// C run-time time (defined in <time.h>)
	time(&osBinTime);		// Get the current time from the 
							// operating system.
	CTime Tim(osBinTime);

	LotTime.nYear = Tim.GetYear();
	LotTime.nMonth = Tim.GetMonth();
	LotTime.nDay = Tim.GetDay();
	LotTime.nHour = Tim.GetHour();
	LotTime.nMin = Tim.GetMinute();
	LotTime.nSec = Tim.GetSecond();

	strVal.Format(_T("%04d-%02d-%02d,%02d:%02d:%02d"), LotTime.nYear, LotTime.nMonth, LotTime.nDay,
		LotTime.nHour, LotTime.nMin, LotTime.nSec);
	return strVal;
}


CString CGvisR2R_PunchView::GetTime(int &nHour, int &nMinute, int &nSec)
{
	CString strVal;
	time_t osBinTime;			// C run-time time (defined in <time.h>)
	time(&osBinTime);		// Get the current time from the 
							// operating system.
	CTime Tim(osBinTime);

	int nYear = Tim.GetYear();
	int nMonth = Tim.GetMonth();
	int nDay = Tim.GetDay();
	nHour = Tim.GetHour();
	nMinute = Tim.GetMinute();
	nSec = Tim.GetSecond();

	strVal.Format(_T("%04d-%02d-%02d, %02d:%02d:%02d"), nYear, nMonth, nDay, nHour, nMinute, nSec);
	return strVal;
}

void CGvisR2R_PunchView::DispTime()
{
	stLotTime LotTime;
	CString str;
	str = GetTime(LotTime);
	if (m_sDispTime != str)
	{
		m_sDispTime = str;
		pFrm->DispStatusBar(str, 7);

		pDoc->WorkingInfo.Lot.CurTime.nYear = LotTime.nYear;
		pDoc->WorkingInfo.Lot.CurTime.nMonth = LotTime.nMonth;
		pDoc->WorkingInfo.Lot.CurTime.nDay = LotTime.nDay;
		pDoc->WorkingInfo.Lot.CurTime.nHour = LotTime.nHour;
		pDoc->WorkingInfo.Lot.CurTime.nMin = LotTime.nMin;
		pDoc->WorkingInfo.Lot.CurTime.nSec = LotTime.nSec;

		if (m_pDlgMenu01)
			m_pDlgMenu01->DispRunTime();
	}
}

// System Input IO .......................................................................
void CGvisR2R_PunchView::DoSaftySens()
{
#ifdef USE_MPE
	if (!pDoc->m_pMpeIb || !pDoc->m_pMpeIF)
		return;

	if (!IsRun())
		return;
	//BOOL bOn = pDoc->m_pMpeIb[7] & (0x01 << 8) ? TRUE : FALSE;	// 마킹부 안전 센서 1
	//BOOL bOnF = pDoc->m_pMpeIF[7] & (0x01 << 8) ? TRUE : FALSE;	// 마킹부 안전 센서 1

	unsigned short usIn = pDoc->m_pMpeIb[7];
	unsigned short *usInF = &pDoc->m_pMpeIF[7];


	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))		// 마킹부 안전 센서
	{
		*usInF |= (0x01 << 8);								
		pDoc->Status.bSensSaftyMk = TRUE;
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))	// 마킹부 안전 센서
	{
		*usInF &= ~(0x01 << 8);
		pDoc->Status.bSensSaftyMk = FALSE;
	}
#endif
}

void CGvisR2R_PunchView::DoDoorSens()
{
#ifdef USE_MPE
	unsigned short usIn;
	unsigned short *usInF;

	if (!pDoc->m_pMpeIb || !pDoc->m_pMpeIF)
		return;

	if (!IsRun())
		return;

	usIn = pDoc->m_pMpeIb[1];
	usInF = &pDoc->m_pMpeIF[1];

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 언코일러 전면 도어 센서
	{
		*usInF |= (0x01 << 12);
		pDoc->Status.bDoorUc[DOOR_FL_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 언코일러 전면 도어 센서
	{
		*usInF &= ~(0x01 << 12);
		pDoc->Status.bDoorUc[DOOR_FL_UC] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 언코일러 측면 도어 센서
	{
		*usInF |= (0x01 << 13);
		pDoc->Status.bDoorUc[DOOR_FR_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 언코일러 측면 도어 센서
	{
		*usInF &= ~(0x01 << 13);
		pDoc->Status.bDoorUc[DOOR_FR_UC] = FALSE;
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))		// 언코일러 후면 도어 센서(좌)
	{
		*usInF |= (0x01 << 14);
		pDoc->Status.bDoorUc[DOOR_BL_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))	// 언코일러 후면 도어 센서(좌)
	{
		*usInF &= ~(0x01 << 14);
		pDoc->Status.bDoorUc[DOOR_BL_UC] = FALSE;
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))		// 언코일러 후면 도어 센서(우)
	{
		*usInF |= (0x01 << 15);
		pDoc->Status.bDoorUc[DOOR_BR_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))	// 언코일러 후면 도어 센서(우)
	{
		*usInF &= ~(0x01 << 15);
		pDoc->Status.bDoorUc[DOOR_BR_UC] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[7];
	usInF = &pDoc->m_pMpeIF[7];

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 마킹부 도어 센서 1 
	{
		*usInF |= (0x01 << 10);
		pDoc->Status.bDoorMk[DOOR_FL_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))	// 마킹부 도어 센서 1 
	{
		*usInF &= ~(0x01 << 10);
		pDoc->Status.bDoorMk[DOOR_FL_MK] = FALSE;
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))		// 마킹부 도어 센서 2 
	{
		*usInF |= (0x01 << 11);
		pDoc->Status.bDoorMk[DOOR_FR_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 마킹부 도어 센서 2
	{
		*usInF &= ~(0x01 << 11);
		pDoc->Status.bDoorMk[DOOR_FR_MK] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 마킹부 도어 센서 3 
	{
		*usInF |= (0x01 << 12);
		pDoc->Status.bDoorMk[DOOR_BL_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 마킹부 도어 센서 3
	{
		*usInF &= ~(0x01 << 12);
		pDoc->Status.bDoorMk[DOOR_BL_MK] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 마킹부 도어 센서 4 
	{
		*usInF |= (0x01 << 13);
		pDoc->Status.bDoorMk[DOOR_BR_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 마킹부 도어 센서 4
	{
		*usInF &= ~(0x01 << 13);
		pDoc->Status.bDoorMk[DOOR_BR_MK] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[11];
	usInF = &pDoc->m_pMpeIF[11];

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 검사부 상 도어 센서 1 
	{
		*usInF |= (0x01 << 10);
		pDoc->Status.bDoorMk[DOOR_FL_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))	// 검사부 상 도어 센서 1
	{
		*usInF &= ~(0x01 << 10);
		pDoc->Status.bDoorMk[DOOR_FL_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))		// 검사부 상 도어 센서 2 
	{
		*usInF |= (0x01 << 11);
		pDoc->Status.bDoorMk[DOOR_FR_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 검사부 상 도어 센서 2
	{
		*usInF &= ~(0x01 << 11);
		pDoc->Status.bDoorMk[DOOR_FR_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 검사부 상 도어 센서 3 
	{
		*usInF |= (0x01 << 12);
		pDoc->Status.bDoorMk[DOOR_BL_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 검사부 상 도어 센서 3
	{
		*usInF &= ~(0x01 << 12);
		pDoc->Status.bDoorMk[DOOR_BL_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 검사부 상 도어 센서 4 
	{
		*usInF |= (0x01 << 13);
		pDoc->Status.bDoorMk[DOOR_BR_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 검사부 상 도어 센서 4
	{
		*usInF &= ~(0x01 << 13);
		pDoc->Status.bDoorMk[DOOR_BR_AOI_UP] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[15];
	usInF = &pDoc->m_pMpeIF[15];

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 검사부 상 도어 센서 1 
	{
		*usInF |= (0x01 << 10);
		pDoc->Status.bDoorAoi[DOOR_FL_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))	// 검사부 상 도어 센서 1
	{
		*usInF &= ~(0x01 << 11);
		pDoc->Status.bDoorAoi[DOOR_FL_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))		// 검사부 상 도어 센서 2 
	{
		*usInF |= (0x01 << 11);
		pDoc->Status.bDoorAoi[DOOR_FR_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 검사부 상 도어 센서 2
	{
		*usInF &= ~(0x01 << 11);
		pDoc->Status.bDoorAoi[DOOR_FR_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 검사부 상 도어 센서 3 
	{
		*usInF |= (0x01 << 12);
		pDoc->Status.bDoorAoi[DOOR_BL_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 검사부 상 도어 센서 3
	{
		*usInF &= ~(0x01 << 12);
		pDoc->Status.bDoorAoi[DOOR_BL_AOI_UP] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 검사부 상 도어 센서 4 
	{
		*usInF |= (0x01 << 13);
		pDoc->Status.bDoorAoi[DOOR_BR_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 검사부 상 도어 센서 4
	{
		*usInF &= ~(0x01 << 13);
		pDoc->Status.bDoorAoi[DOOR_BR_AOI_UP] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[15];
	usInF = &pDoc->m_pMpeIF[15];

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 검사부 하 도어 센서 1 
	{
		*usInF |= (0x01 << 10);
		pDoc->Status.bDoorAoi[DOOR_FL_AOI_DN] = TRUE;
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))	// 검사부 하 도어 센서 1
	{
		*usInF &= ~(0x01 << 10);
		pDoc->Status.bDoorAoi[DOOR_FL_AOI_DN] = FALSE;
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))		// 검사부 하 도어 센서 2 
	{
		*usInF |= (0x01 << 11);
		pDoc->Status.bDoorAoi[DOOR_FR_AOI_DN] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 검사부 하 도어 센서 2
	{
		*usInF &= ~(0x01 << 11);
		pDoc->Status.bDoorAoi[DOOR_FR_AOI_DN] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 검사부 하 도어 센서 3 
	{
		*usInF |= (0x01 << 12);
		pDoc->Status.bDoorAoi[DOOR_BL_AOI_DN] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 검사부 하 도어 센서 3
	{
		*usInF &= ~(0x01 << 12);
		pDoc->Status.bDoorAoi[DOOR_BL_AOI_DN] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 검사부 하 도어 센서 4 
	{
		*usInF |= (0x01 << 13);
		pDoc->Status.bDoorAoi[DOOR_BR_AOI_DN] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 검사부 하 도어 센서 4
	{
		*usInF &= ~(0x01 << 13);
		pDoc->Status.bDoorAoi[DOOR_BR_AOI_DN] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[17];
	usInF = &pDoc->m_pMpeIF[17];

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 리코일러 전면 도어 센서
	{
		*usInF |= (0x01 << 12);
		pDoc->Status.bDoorRe[DOOR_FL_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 리코일러 전면 도어 센서
	{
		*usInF &= ~(0x01 << 12);
		pDoc->Status.bDoorRe[DOOR_FL_RC] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 리코일러 측면 도어 센서
	{
		*usInF |= (0x01 << 13);
		pDoc->Status.bDoorRe[DOOR_FR_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 리코일러 측면 도어 센서
	{
		*usInF &= ~(0x01 << 13);
		pDoc->Status.bDoorRe[DOOR_FR_RC] = FALSE;
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))		// 리코일러 후면 도어 센서
	{
		*usInF |= (0x01 << 14);
		pDoc->Status.bDoorRe[DOOR_BL_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))	// 리코일러 후면 도어 센서(좌)
	{
		*usInF &= ~(0x01 << 14);
		pDoc->Status.bDoorRe[DOOR_BL_RC] = FALSE;
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))		// 리코일러 후면 도어 센서(우)
	{
		*usInF |= (0x01 << 15);
		pDoc->Status.bDoorRe[DOOR_BR_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))	// 리코일러 후면 도어 센서(우)
	{
		*usInF &= ~(0x01 << 15);
		pDoc->Status.bDoorRe[DOOR_BR_RC] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[27];
	usInF = &pDoc->m_pMpeIF[27];

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))		// 각인부 도어 센서 1
	{
		*usInF |= (0x01 << 10);
		pDoc->Status.bDoorEngv[DOOR_FL_ENGV] = TRUE;
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))	// 각인부 도어 센서 1
	{
		*usInF &= ~(0x01 << 10);
		pDoc->Status.bDoorEngv[DOOR_FL_ENGV] = FALSE;
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))		// 각인부 도어 센서 2
	{
		*usInF |= (0x01 << 11);
		pDoc->Status.bDoorEngv[DOOR_FR_ENGV] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))	// 각인부 도어 센서 2
	{
		*usInF &= ~(0x01 << 11);
		pDoc->Status.bDoorEngv[DOOR_FR_ENGV] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))		// 각인부 도어 센서 3
	{
		*usInF |= (0x01 << 12);
		pDoc->Status.bDoorEngv[DOOR_BL_ENGV] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))	// 각인부 도어 센서 3
	{
		*usInF &= ~(0x01 << 12);
		pDoc->Status.bDoorEngv[DOOR_BL_ENGV] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))		// 각인부 도어 센서 4
	{
		*usInF |= (0x01 << 13);
		pDoc->Status.bDoorEngv[DOOR_BR_ENGV] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))	// 각인부 도어 센서 4
	{
		*usInF &= ~(0x01 << 13);
		pDoc->Status.bDoorEngv[DOOR_BR_ENGV] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[28];
	usInF = &pDoc->m_pMpeIF[28];


	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 2D 리셋 스위치
		m_bSwStopNow = FALSE;
		if (m_pDlgMenu03)
			m_pDlgMenu03->SwReset();
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

#endif
}

void CGvisR2R_PunchView::DoEngraveSens()
{
#ifdef USE_MPE
	unsigned short usIn;
	unsigned short *usInF;

	if (!pDoc->m_pMpeIb || !pDoc->m_pMpeIF)
		return;

	usIn = pDoc->m_pMpeIb[28];
	usInF = &pDoc->m_pMpeIF[28];


	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 2D 리셋 스위치
		m_bSwStopNow = FALSE;
		if (m_pDlgMenu03)
			m_pDlgMenu03->SwReset();
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}
#endif
}

void CGvisR2R_PunchView::DoModeSel()
{
#ifdef USE_MPE
	if (!pDoc->m_pMpeIo)
		return;

	BOOL bMode;// [2];
	bMode = pDoc->m_pMpeIo[36] & (0x01 << 5) ? TRUE : FALSE;	// 마킹부 자동 상태 스위치 램프
	if (bMode)
	{
		pDoc->Status.bAuto = TRUE;
		pDoc->Status.bManual = FALSE;

		m_bAuto = TRUE;
		m_bManual = FALSE;
		if(pView->m_pEngrave)
			pView->m_pEngrave->SwAuto(TRUE);
	}
	else
	{
		pDoc->Status.bManual = TRUE;
		pDoc->Status.bAuto = FALSE;

		m_bManual = TRUE;
		m_bAuto = FALSE;
		if (pView->m_pEngrave)
			pView->m_pEngrave->SwManual(TRUE);
	}
#else
	pDoc->Status.bAuto = FALSE;
	pDoc->Status.bManual = TRUE;
#endif

}

void CGvisR2R_PunchView::DoMainSw()
{
	if (!pDoc->m_pMpeIb || !pDoc->m_pMpeIF)
		return;

	unsigned short usIn = pDoc->m_pMpeIb[4];
	unsigned short *usInF = &pDoc->m_pMpeIF[4];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 마킹부 비상정지 스위치(모니터부)
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 마킹부 운전 스위치
		m_bSwStopNow = FALSE;
		if (m_pDlgMenu03)
			m_pDlgMenu03->SwRun();
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 마킹부 정지 스위치
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 마킹부 운전준비 스위치
		m_bSwStopNow = FALSE;
		if (m_pDlgMenu03)
			m_pDlgMenu03->SwReady();
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 마킹부 리셋 스위치
		m_bSwStopNow = FALSE;
		if (m_pDlgMenu03)
			m_pDlgMenu03->SwReset();
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 마킹부 JOG 버튼(상)
		if (pDoc->Status.bSwJogLeft)
			SwJog(AXIS_Y0, M_CCW, TRUE);
		else
			SwJog(AXIS_Y1, M_CCW, TRUE);
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
		if (pDoc->Status.bSwJogLeft)
			SwJog(AXIS_Y0, M_CCW, FALSE);
		else
			SwJog(AXIS_Y1, M_CCW, FALSE);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 마킹부 JOG 버튼(하)
		if (pDoc->Status.bSwJogLeft)
			SwJog(AXIS_Y0, M_CW, TRUE);
		else
			SwJog(AXIS_Y1, M_CW, TRUE);
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
		if (pDoc->Status.bSwJogLeft)
			SwJog(AXIS_Y0, M_CW, FALSE);
		else
			SwJog(AXIS_Y1, M_CW, FALSE);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 마킹부 JOG 버튼(좌)
		if (pDoc->Status.bSwJogLeft)
			SwJog(AXIS_X0, M_CCW, TRUE);
		else
			SwJog(AXIS_X1, M_CCW, TRUE);
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
		if (pDoc->Status.bSwJogLeft)
			SwJog(AXIS_X0, M_CCW, FALSE);
		else
			SwJog(AXIS_X1, M_CCW, FALSE);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 마킹부 JOG 버튼(우)
		if (pDoc->Status.bSwJogLeft)
			SwJog(AXIS_X0, M_CW, TRUE);
		else
			SwJog(AXIS_X1, M_CW, TRUE);
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
		if (pDoc->Status.bSwJogLeft)
			SwJog(AXIS_X0, M_CW, FALSE);
		else
			SwJog(AXIS_X1, M_CW, FALSE);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 마킹부 모션 선택(LEFT)
		pDoc->Status.bSwJogLeft = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
		pDoc->Status.bSwJogLeft = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 마킹부 속도 선택
		pDoc->Status.bSwJogFast = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
		pDoc->Status.bSwJogFast = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// 마킹부 운전 선택(INDEX)
		pDoc->Status.bSwJogStep = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
		pDoc->Status.bSwJogStep = FALSE;
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	{
		*usInF |= (0x01 << 14);								// SPARE	
															// No Use....
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	{
		*usInF &= ~(0x01 << 14);
		// No Use....
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// SPARE	
															// No Use....
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
		// No Use....
	}

	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;
}


void CGvisR2R_PunchView::DoMkSens()
{
	unsigned short usIn = 0;
	unsigned short *usInF = NULL;

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 마킹부 테이블 진공 센서
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 마킹부 테이블 압력 스위치
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 마킹부 피딩 진공 센서
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 마킹부 피딩 클램프 상승 센서
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 마킹부 피딩 클램프 하강 센서
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// 마킹부 토크 진공 센서
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// 마킹부 토크 클램프 상승 센서
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 마킹부 토크 클램프 하강 센서
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 마킹부 댄서롤 상승 센서
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 마킹부 댄서롤 하강 센서
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 검사부 피딩 진공 센서
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 검사부 피딩 클램프 상승 센서
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 검사부 피딩 클램프 하강 센서
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// 검사부 토크 진공 센서
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	{
		*usInF |= (0x01 << 14);								// 검사부 토크 클램프 상승 센서
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	{
		*usInF &= ~(0x01 << 14);
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// 검사부 토크 클램프 하강 센서
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
	}
}

void CGvisR2R_PunchView::DoAoiBoxSw()
{
	unsigned short usIn = 0;
	unsigned short *usInF = NULL;

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 검사부 연동 온/오프 스위치
		if (m_pDlgMenu03)
			m_pDlgMenu03->SwAoiRelation();
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 검사부 테이블 브로워 스위치
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 검사부 피딩 정회전 스위치
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 검사부 피딩 역회전 스위치
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 검사부 피딩 진공 스위치
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// 검사부 토크 진공 스위치
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// 검사부 테이블 진공 스위치
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 검사부 레이져 포인터 스위치
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 검사부 피딩 클램프 스위치
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 검사부 토크 클램프 스위치
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
	}
}

void CGvisR2R_PunchView::DoEmgSens()
{
	unsigned short usIn, *usInF;

	if (!pDoc->m_pMpeIb || !pDoc->m_pMpeIF)
		return;

	usIn = pDoc->m_pMpeIb[0];
	usInF = &pDoc->m_pMpeIF[0];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 언코일러 비상정지 스위치
	{
		*usInF |= (0x01 << 0);						
		pDoc->Status.bEmgUc = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		pDoc->Status.bEmgUc = FALSE;
	}

	usIn = pDoc->m_pMpeIb[4];
	usInF = &pDoc->m_pMpeIF[4];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 마킹부 비상정지 스위치(모니터부)
	{
		*usInF |= (0x01 << 0);						
		pDoc->Status.bEmgMk[EMG_M_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		pDoc->Status.bEmgMk[EMG_M_MK] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[5];
	usInF = &pDoc->m_pMpeIF[5];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 마킹부 비상정지 스위치(스위치부)	
	{
		*usInF |= (0x01 << 0);						
		pDoc->Status.bEmgMk[EMG_B_MK] = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		pDoc->Status.bEmgMk[EMG_B_MK] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[8];
	usInF = &pDoc->m_pMpeIF[8];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 검사부 상 비상정지 스위치(후면) 
	{
		*usInF |= (0x01 << 0);						
		pDoc->Status.bEmgAoi[EMG_B_AOI_UP] = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		pDoc->Status.bEmgAoi[EMG_B_AOI_UP] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[12];
	usInF = &pDoc->m_pMpeIF[12];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 검사부 하 비상정지 스위치(후면) 
	{
		*usInF |= (0x01 << 0);						
		pDoc->Status.bEmgAoi[EMG_B_AOI_DN] = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		pDoc->Status.bEmgAoi[EMG_B_AOI_DN] = FALSE;
	}

	usIn = pDoc->m_pMpeIb[16];
	usInF = &pDoc->m_pMpeIF[16];

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))		// 리코일러 비상정지 스위치
	{
		*usInF |= (0x01 << 0);
		pDoc->Status.bEmgRc = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		pDoc->Status.bEmgRc = FALSE;
	}
}

void CGvisR2R_PunchView::DoSignal()
{
	unsigned short usIn = 0;
	unsigned short *usInF = NULL;

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 검사부 검사 완료
		pDoc->Status.bSigTestDoneAoi = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		pDoc->Status.bSigTestDoneAoi = FALSE;
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 검사부 테이블 진공 완료
		pDoc->Status.bSigTblAirAoi = TRUE;
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
		pDoc->Status.bSigTblAirAoi = FALSE;
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
		// No Use....
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
		// No Use....
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
		// No Use....
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
		// No Use....
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
		// No Use....
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
		// No Use....
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 검사부 시그날 타워-적색
		TowerLamp(RGB_RED, TRUE, FALSE);
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
		TowerLamp(RGB_RED, FALSE, FALSE);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 검사부 시그날 타워-황색
		TowerLamp(RGB_YELLOW, TRUE, FALSE);
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
		TowerLamp(RGB_YELLOW, FALSE, FALSE);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 검사부 시그날 타워-녹색
		TowerLamp(RGB_GREEN, TRUE, FALSE);
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
		TowerLamp(RGB_GREEN, FALSE, FALSE);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 검사부 부져 1
		Buzzer(TRUE, 0);
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
		Buzzer(FALSE, 0);
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 검사부 부져 2
		Buzzer(TRUE, 1);
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
		Buzzer(FALSE, 1);
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
		// No Use....
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	{
		*usInF |= (0x01 << 14);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	{
		*usInF &= ~(0x01 << 14);
		// No Use....
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
		// No Use....
	}
}

void CGvisR2R_PunchView::DoUcBoxSw()
{
	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;

	unsigned short usIn = 0;
	unsigned short *usInF = NULL;

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 언코일러 연동 온/오프 스위치
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 언코일러 댄서롤 상승/하강 스위치
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 언코일러 클린롤러 상승/하강 스위치
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 언코일러 클린롤러누름 상승/하강 스위치
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// 언코일러 제품 이음매(좌) 스위치
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// 언코일러 제품 이음매(우) 스위치	
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 언코일러 제품휠 지지 스위치
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 언코일러 간지척 클램프 스위치
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 언코일러 간지휠 정회전 스위치
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 언코일러 간지휠 역회전 스위치
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 언코일러 제품척 클램프 스위치
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 언코일러 제품휠 정회전 스위치
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// 언코일러 제품휠 역회전 스위치
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
		// No Use....
	}
}

void CGvisR2R_PunchView::DoUcSens1()
{
	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;

	unsigned short usIn = 0;
	unsigned short *usInF = NULL;

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 언코일러 간지모터조절 1	
															// Late....
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 언코일러 간지모터조절 2	
															// Late....
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 언코일러 간지모터조절 3	
															// Late....
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 언코일러 간지모터조절 4	
															// Late....
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 언코일러 제품모터조절 1	
															// Late....
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// 언코일러 제품모터조절 2
															// Late....
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// 언코일러 제품모터조절 3	
															// Late....
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 언코일러 제품모터조절 4	
															// Late....
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 언코일러 제품 EPC POS 리미트 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 언코일러 제품 EPC NEG 리미트 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 언코일러 제품 EPC 원점 센서 
															// Late....
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 언코일러 전면 도어 센서(좌)
		pDoc->Status.bDoorUc[DOOR_FL_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
		pDoc->Status.bDoorUc[DOOR_FL_UC] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 언코일러 전면 도어 센서(우)
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// 언코일러 측면 도어 센서
		pDoc->Status.bDoorUc[DOOR_FR_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
		pDoc->Status.bDoorUc[DOOR_FR_UC] = FALSE;
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	{
		*usInF |= (0x01 << 14);								// 언코일러 후면 도어 센서(좌)
		pDoc->Status.bDoorUc[DOOR_BL_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	{
		*usInF &= ~(0x01 << 14);
		pDoc->Status.bDoorUc[DOOR_BL_UC] = FALSE;
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// 언코일러 후면 도어 센서(우)
		pDoc->Status.bDoorUc[DOOR_BR_UC] = TRUE;
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
		pDoc->Status.bDoorUc[DOOR_BR_UC] = FALSE;
	}

}

void CGvisR2R_PunchView::DoUcSens2()
{

	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;

	unsigned short usIn = 0;
	unsigned short *usInF = NULL;
	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 언코일러 간지텐션 POS 리미트 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 언코일러 간지텐션 상한 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 언코일러 간지텐션 2/3 지점 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 언코일러 간지텐션 1/3 지점 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 언코일러 간지텐션 하한 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// 언코일러 간지텐션 NEG 리미트 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// 언코일러 간지척 진공 스위치
															// Late....
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 언코일러 간지버퍼 압력 스위치	
															// Late....
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 언코일러 클린롤러 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 언코일러 클린롤러 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 언코일러 클린롤러 누름(전면) 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 언코일러 클린롤러 누름(전면) 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 언코일러 클린롤러 누름(후면) 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// 언코일러 클린롤러 누름(후면) 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	{
		*usInF |= (0x01 << 14);								// 언코일러 댄서롤 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	{
		*usInF &= ~(0x01 << 14);
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// 언코일러 댄서롤 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
	}

}

void CGvisR2R_PunchView::DoUcSens3()
{

	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;

	unsigned short usIn = 0;
	unsigned short *usInF = NULL;

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 언코일러 댄서롤 POS 리미트 센서
		pDoc->Status.bSensLmtBufUc[LMT_POS] = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		pDoc->Status.bSensLmtBufUc[LMT_POS] = FALSE;
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 언코일러 댄서롤 상한 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 언코일러 댄서롤 2/3 지점 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 언코일러 댄서롤 1/3 지점 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 언코일러 댄서롤 하한 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// 언코일러 댄서롤 NEG 리미트 센서
		pDoc->Status.bSensLmtBufUc[LMT_NEG] = TRUE;
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
		pDoc->Status.bSensLmtBufUc[LMT_NEG] = FALSE;
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// 언코일러 제품척 진공 스위치
															// Late....
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 언코일러 메인 에어 
															// Late....
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 언코일러 제품휠 지지 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 언코일러 제품휠 지지 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 언코일러 제품 이음매(좌) 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 언코일러 제품 이음매(좌) 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 언코일러 제품 이음매(우) 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// 언코일러 제품 이음매(우) 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	{
		*usInF |= (0x01 << 14);								// SPARE	
															// No Use....
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	{
		*usInF &= ~(0x01 << 14);
		// No Use....
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// 언코일러 제품휠 지지 정위치 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
	}

}

void CGvisR2R_PunchView::DoRcBoxSw()
{

	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;
	unsigned short usIn = 0;
	unsigned short *usInF = NULL;
	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 리코일러 연동 온/오프 스위치
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 리코일러 댄서롤 상승/하강 스위치
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
		// No Use....
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
		// No Use....
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// 리코일러 제품 이음매(좌) 스위치
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// 리코일러 제품 이음매(우) 스위치
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 리코일러 제품휠 지지 스위치
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 리코일러 간지척 클램프 스위치
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 리코일러 간지휠 정회전 스위치
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 리코일러 간지휠 역회전 스위치
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 리코일러 제품척 클램프 스위치
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 리코일러 제품휠 정회전 스위치
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// 리코일러 제품휠 역회전 스위치
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
		// No Use....
	}

}

void CGvisR2R_PunchView::DoRcSens1()
{
	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;

	unsigned short usIn = 0;
	unsigned short *usInF = NULL;

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 리코일러 간지모터조절 1
															// Late....
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 리코일러 간지모터조절 2
															// Late....
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 리코일러 간지모터조절 3
															// Late....
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 리코일러 간지모터조절 4
															// Late....
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 리코일러 제품모터조절 1
															// Late....
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// 리코일러 제품모터조절 2
															// Late....
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// 리코일러 제품모터조절 3
															// Late....
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 리코일러 제품모터조절 4
															// Late....
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 리코일러 제품 EPC POS 리미트 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 리코일러 제품 EPC NEG 리미트 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 리코일러 제품 EPC 원점 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 리코일러 전면 도어 센서(좌)
		pDoc->Status.bDoorRe[DOOR_FL_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
		pDoc->Status.bDoorRe[DOOR_FL_RC] = FALSE;
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 리코일러 전면 도어 센서(우)
		pDoc->Status.bDoorRe[DOOR_FR_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
		pDoc->Status.bDoorRe[DOOR_FR_RC] = FALSE;
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// 리코일러 측면 도어 센서
		pDoc->Status.bDoorRe[DOOR_S_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
		pDoc->Status.bDoorRe[DOOR_S_RC] = FALSE;
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	{
		*usInF |= (0x01 << 14);								// 리코일러 후면 도어 센서(좌)
		pDoc->Status.bDoorRe[DOOR_BL_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	{
		*usInF &= ~(0x01 << 14);
		pDoc->Status.bDoorRe[DOOR_BL_RC] = FALSE;
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// 리코일러 후면 도어 센서(우)
		pDoc->Status.bDoorRe[DOOR_BR_RC] = TRUE;
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
		pDoc->Status.bDoorRe[DOOR_BR_RC] = FALSE;
	}

}

void CGvisR2R_PunchView::DoRcSens2()
{
	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;

	unsigned short usIn = 0;
	unsigned short *usInF = NULL;

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 리코일러 간지텐션 POS 리미트 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 리코일러 간지텐션 상한 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 리코일러 간지텐션 2/3 지점 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 리코일러 간지텐션 1/3 지점 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 리코일러 간지텐션 하한 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// 리코일러 간지텐션 NEG 리미트 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// 리코일러 간지척 진공 스위치
															// Late....
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 리코일러 간지버퍼 압력 스위치
															// Late....
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
		// No Use....
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
		// No Use....
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
		// No Use....
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
		// No Use....
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
		// No Use....
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// SPARE
															// No Use....
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
		// No Use....
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	{
		*usInF |= (0x01 << 14);								// 리코일러 댄서롤 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	{
		*usInF &= ~(0x01 << 14);
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// 리코일러 댄서롤 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
	}
}

void CGvisR2R_PunchView::DoRcSens3()
{
	if (!pDoc->m_pMpeIo)// || !pDoc->m_pMpeIoF)
		return;

	unsigned short usIn = 0;
	unsigned short *usInF = NULL;

	if ((usIn & (0x01 << 0)) && !(*usInF & (0x01 << 0)))
	{
		*usInF |= (0x01 << 0);								// 리코일러 댄서롤 POS 리미트 센서
		pDoc->Status.bSensLmtBufRc[LMT_POS] = TRUE;
	}
	else if (!(usIn & (0x01 << 0)) && (*usInF & (0x01 << 0)))
	{
		*usInF &= ~(0x01 << 0);
		pDoc->Status.bSensLmtBufRc[LMT_POS] = FALSE;
	}

	if ((usIn & (0x01 << 1)) && !(*usInF & (0x01 << 1)))
	{
		*usInF |= (0x01 << 1);								// 리코일러 댄서롤 상한 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 1)) && (*usInF & (0x01 << 1)))
	{
		*usInF &= ~(0x01 << 1);
	}

	if ((usIn & (0x01 << 2)) && !(*usInF & (0x01 << 2)))
	{
		*usInF |= (0x01 << 2);								// 리코일러 댄서롤 2/3 지점 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 2)) && (*usInF & (0x01 << 2)))
	{
		*usInF &= ~(0x01 << 2);
	}

	if ((usIn & (0x01 << 3)) && !(*usInF & (0x01 << 3)))
	{
		*usInF |= (0x01 << 3);								// 리코일러 댄서롤 1/3 지점 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 3)) && (*usInF & (0x01 << 3)))
	{
		*usInF &= ~(0x01 << 3);
	}

	if ((usIn & (0x01 << 4)) && !(*usInF & (0x01 << 4)))
	{
		*usInF |= (0x01 << 4);								// 리코일러 댄서롤 하한 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 4)) && (*usInF & (0x01 << 4)))
	{
		*usInF &= ~(0x01 << 4);
	}

	if ((usIn & (0x01 << 5)) && !(*usInF & (0x01 << 5)))
	{
		*usInF |= (0x01 << 5);								// 리코일러 댄서롤 NEG 리미트 센서
		pDoc->Status.bSensLmtBufRc[LMT_NEG] = TRUE;
	}
	else if (!(usIn & (0x01 << 5)) && (*usInF & (0x01 << 5)))
	{
		*usInF &= ~(0x01 << 5);
		pDoc->Status.bSensLmtBufRc[LMT_NEG] = FALSE;
	}

	if ((usIn & (0x01 << 6)) && !(*usInF & (0x01 << 6)))
	{
		*usInF |= (0x01 << 6);								// 리코일러 제품척 진공 스위치
															// Late....
	}
	else if (!(usIn & (0x01 << 6)) && (*usInF & (0x01 << 6)))
	{
		*usInF &= ~(0x01 << 6);
	}

	if ((usIn & (0x01 << 7)) && !(*usInF & (0x01 << 7)))
	{
		*usInF |= (0x01 << 7);								// 리코일러 메인 에어 
															// Late....
	}
	else if (!(usIn & (0x01 << 7)) && (*usInF & (0x01 << 7)))
	{
		*usInF &= ~(0x01 << 7);
	}

	if ((usIn & (0x01 << 8)) && !(*usInF & (0x01 << 8)))
	{
		*usInF |= (0x01 << 8);								// 리코일러 제품휠 지지 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 8)) && (*usInF & (0x01 << 8)))
	{
		*usInF &= ~(0x01 << 8);
	}

	if ((usIn & (0x01 << 9)) && !(*usInF & (0x01 << 9)))
	{
		*usInF |= (0x01 << 9);								// 리코일러 제품휠 지지 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 9)) && (*usInF & (0x01 << 9)))
	{
		*usInF &= ~(0x01 << 9);
	}

	if ((usIn & (0x01 << 10)) && !(*usInF & (0x01 << 10)))
	{
		*usInF |= (0x01 << 10);								// 리코일러 제품 이음매(좌) 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 10)) && (*usInF & (0x01 << 10)))
	{
		*usInF &= ~(0x01 << 10);
	}

	if ((usIn & (0x01 << 11)) && !(*usInF & (0x01 << 11)))
	{
		*usInF |= (0x01 << 11);								// 리코일러 제품 이음매(좌) 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 11)) && (*usInF & (0x01 << 11)))
	{
		*usInF &= ~(0x01 << 11);
	}

	if ((usIn & (0x01 << 12)) && !(*usInF & (0x01 << 12)))
	{
		*usInF |= (0x01 << 12);								// 리코일러 제품 이음매(우) 상승 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 12)) && (*usInF & (0x01 << 12)))
	{
		*usInF &= ~(0x01 << 12);
	}

	if ((usIn & (0x01 << 13)) && !(*usInF & (0x01 << 13)))
	{
		*usInF |= (0x01 << 13);								// 리코일러 제품 이음매(우) 하강 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 13)) && (*usInF & (0x01 << 13)))
	{
		*usInF &= ~(0x01 << 13);
	}

	if ((usIn & (0x01 << 14)) && !(*usInF & (0x01 << 14)))
	{
		*usInF |= (0x01 << 14);								// 리코일러 제품 잔량 감지 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 14)) && (*usInF & (0x01 << 14)))
	{
		*usInF &= ~(0x01 << 14);
	}

	if ((usIn & (0x01 << 15)) && !(*usInF & (0x01 << 15)))
	{
		*usInF |= (0x01 << 15);								// 리코일러 제품휠 지지 정위치 센서
															// Late....
	}
	else if (!(usIn & (0x01 << 15)) && (*usInF & (0x01 << 15)))
	{
		*usInF &= ~(0x01 << 15);
	}
}

void CGvisR2R_PunchView::SetEngFdPitch(double dPitch)
{
	pDoc->SetEngFdPitch(dPitch);
}

void CGvisR2R_PunchView::SetAoiFdPitch(double dPitch)
{
	pDoc->SetAoiFdPitch(dPitch);
}

void CGvisR2R_PunchView::SetMkFdPitch(double dPitch)
{
	pDoc->SetMkFdPitch(dPitch);
	long lData = long(dPitch*1000.0);
	MpeWrite(Plc.DlgMenu04.FeedLeadPitchPunch, lData); // 마킹부 Feeding 롤러 Lead Pitch
}

void CGvisR2R_PunchView::SetBufInitPos(double dPos)
{
	pDoc->SetBufInitPos(dPos);
}

void CGvisR2R_PunchView::SetEngBufInitPos(double dPos)
{
	pDoc->SetEngBufInitPos(dPos);
}

void CGvisR2R_PunchView::SetBufHomeParam(double dVel, double dAcc)
{
	long lVel = long(dVel*1000.0);
	long lAcc = long(dAcc*1000.0);
}

LRESULT CGvisR2R_PunchView::OnBufThreadDone(WPARAM wPara, LPARAM lPara)
{
	return 0L;
}

//.........................................................................................

BOOL CGvisR2R_PunchView::WatiDispMain(int nDelay)
{
	if (m_nWatiDispMain % nDelay)
	{
		m_nWatiDispMain++;
		return TRUE;
	}

	m_nWatiDispMain = 0;
	m_nWatiDispMain++;
	return FALSE;
}

void CGvisR2R_PunchView::DispMain(CString sMsg, COLORREF rgb)
{
	pDoc->SetMonDispMain(sMsg);

	m_csDispMain.Lock();
	m_bDispMain = FALSE;
	stDispMain stData(sMsg, rgb);
	m_ArrayDispMain.Add(stData);
	m_bDispMain = TRUE;

	if (sMsg == _T("정 지"))
	{
		pDoc->SetMkMenu03(_T("Main"), _T("Stop"), TRUE);
		pDoc->SetMkMenu03(_T("Main"), _T("Run"), FALSE);
	}
	else
	{
		pDoc->SetMkMenu03(_T("Main"), _T("Run"), TRUE);
		pDoc->SetMkMenu03(_T("Main"), _T("Stop"), FALSE);
	}

	sMsg.Empty();
	m_csDispMain.Unlock();
}

int CGvisR2R_PunchView::DoDispMain()
{
	int nRtn = -1;

	if (!m_bDispMain)
		return nRtn;

	int nCount = m_ArrayDispMain.GetSize();
	if (nCount>0)
	{
		stDispMain stDispMsg;

		m_csDispMain.Lock();
		stDispMsg = m_ArrayDispMain.GetAt(0);
		m_ArrayDispMain.RemoveAt(0);
		m_csDispMain.Unlock();

		if (m_pDlgMenu01)
		{
			CString sMsg = stDispMsg.strMsg;
			COLORREF rgb = stDispMsg.rgb;
			m_sDispMain = sMsg;
			m_pDlgMenu01->DispMain(sMsg, rgb);
			CheckMonDispMainSignal();
			return 0;
		}
	}

	return nRtn;
}

BOOL CGvisR2R_PunchView::IsReady()
{
	if (m_pDlgMenu03)
		return m_pDlgMenu03->GetReady();

	return FALSE;
}

BOOL CGvisR2R_PunchView::IsAuto()
{
	if (pDoc->Status.bAuto)
		return TRUE;
	return FALSE;
}

void CGvisR2R_PunchView::Shift2DummyBuf()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sSrc, sDest;
	int nSerial;

	sSrc = pDoc->WorkingInfo.System.sPathVrsShareUp;
	sDest = pDoc->WorkingInfo.System.sPathVsDummyBufUp;

	if (pDoc->m_pFile)
	{
		nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
		pDoc->m_pFile->DelPcr(sSrc, nSerial);
		if (nSerial > 0)
		{
			MpeWrite(Plc.DlgMenu03.PcrReceivedSerialAoiUp, (long)nSerial);	// 검사한 Panel의 AOI 상 Serial
			Sleep(300);
			MpeWrite(Plc.DlgMenu03.PcrReceivedAoiUp, 1); // AOI 상 : PCR파일 Received
		}
	}

	if (bDualTest)
	{
		sSrc = pDoc->WorkingInfo.System.sPathVrsShareDn;
		sDest = pDoc->WorkingInfo.System.sPathVsDummyBufDn;

		if (pDoc->m_pFile)
		{
			nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
			pDoc->m_pFile->DelPcr(sSrc, nSerial);
			if (nSerial > 0)
			{
				MpeWrite(Plc.DlgMenu03.PcrReceivedSerialAoiDn , (long)nSerial);	// 검사한 Panel의 AOI 하 Serial
				//MpeWrite(Plc.DlgMenu03.PcrReceivedSerialAoiDn , (long)GetAoiDnAutoSerial() - 1);	// 검사한 Panel의 AOI 하 Serial
				Sleep(300);
				MpeWrite(Plc.DlgMenu03.PcrReceivedAoiDn, 1); // AOI 하 : PCR파일 Received
			}
		}
	}
}

void CGvisR2R_PunchView::Shift2Buf()	// 버퍼폴더의 마지막 시리얼과 Share폴더의 시리얼이 연속인지 확인 후 옮김.
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sSrc, sDest;
	int nSerial;

	if (pDoc->m_bVsStatusUp)
	{
		sSrc = pDoc->WorkingInfo.System.sPathVsShareUp;
		sDest = pDoc->WorkingInfo.System.sPathVrsBufUp;
		if (pDoc->m_pFile)
		{
			nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
			if (nSerial > 0)
				m_nShareUpS = nSerial;
			pDoc->m_pFile->DelPcr(sSrc, nSerial);
		}
		if (bDualTest)
		{
			sSrc = pDoc->WorkingInfo.System.sPathVsShareDn;
			sDest = pDoc->WorkingInfo.System.sPathVrsBufDn;
			if (pDoc->m_pFile)
			{
				nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
				if(nSerial > 0)
					m_nShareDnS = nSerial;
				pDoc->m_pFile->DelPcr(sSrc, nSerial);
			}
		}

		SetListBuf();
	}
	else
	{
		sSrc = pDoc->WorkingInfo.System.sPathVrsShareUp;
		sDest = pDoc->WorkingInfo.System.sPathVrsBufUp;

		if (pDoc->m_pFile)
		{
			nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
			pDoc->m_pFile->DelPcr(sSrc, nSerial);
			if (nSerial > 0)
			{
				m_nShareUpS = nSerial;
				MpeWrite(Plc.DlgMenu03.PcrReceivedSerialAoiUp, (long)nSerial);	// 검사한 Panel의 AOI 상 Serial
				Sleep(300);
				MpeWrite(Plc.DlgMenu03.PcrReceivedAoiUp, 1); // AOI 상 : PCR파일 Received
			}
		}

		if (bDualTest)
		{
			sSrc = pDoc->WorkingInfo.System.sPathVrsShareDn;
			sDest = pDoc->WorkingInfo.System.sPathVrsBufDn;

			if (pDoc->m_pFile)
			{
				nSerial = pDoc->m_pFile->CopyPcr(sSrc, sDest);
				pDoc->m_pFile->DelPcr(sSrc, nSerial);
				if (nSerial > 0)
				{
					m_nShareDnS = nSerial;
					MpeWrite(Plc.DlgMenu03.PcrReceivedSerialAoiDn , (long)nSerial);	// 검사한 Panel의 AOI 하 Serial
					//MpeWrite(Plc.DlgMenu03.PcrReceivedSerialAoiDn , (long)GetAoiDnAutoSerial() - 1);	// 검사한 Panel의 AOI 하 Serial
					Sleep(300);
					MpeWrite(Plc.DlgMenu03.PcrReceivedAoiDn, 1); // AOI 하 : PCR파일 Received
				}
			}
		}

		SetListBuf();
	}
}


void CGvisR2R_PunchView::CompletedMk(int nCam) // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
{
	int nSerial = -1;

	switch (nCam)
	{
	case 0:
		nSerial = m_nBufUpSerial[0];
		break;
	case 1:
		nSerial = m_nBufUpSerial[1];
		break;
	case 2:
		nSerial = m_nBufUpSerial[1];
		break;
	case 3:
		return;
	default:
		return;
	}

	pDoc->SetCompletedSerial(nSerial);
}

void CGvisR2R_PunchView::SetDelay(int mSec, int nId)
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	m_dwSetDlySt[nId] = GetTickCount();
	m_dwSetDlyEd[nId] = m_dwSetDlySt[nId] + mSec;
}

void CGvisR2R_PunchView::SetDelay0(int mSec, int nId)
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	m_dwSetDlySt0[nId] = GetTickCount();
	m_dwSetDlyEd0[nId] = m_dwSetDlySt0[nId] + mSec;
}

void CGvisR2R_PunchView::SetDelay1(int mSec, int nId)
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	m_dwSetDlySt1[nId] = GetTickCount();
	m_dwSetDlyEd1[nId] = m_dwSetDlySt1[nId] + mSec;
}

BOOL CGvisR2R_PunchView::WaitDelay(int nId) // F:Done, T:On Waiting....
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	DWORD dwCur = GetTickCount();
	if (dwCur < m_dwSetDlyEd[nId])
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::WaitDelay0(int nId) // F:Done, T:On Waiting....
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	DWORD dwCur = GetTickCount();
	if (dwCur < m_dwSetDlyEd0[nId])
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::WaitDelay1(int nId) // F:Done, T:On Waiting....
{
	if (nId > 10)
		nId = 9;
	else if (nId < 0)
		nId = 0;

	DWORD dwCur = GetTickCount();
	if (dwCur < m_dwSetDlyEd1[nId])
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::GetDelay(int &mSec, int nId) // F:Done, T:On Waiting....
{
	DWORD dwCur = GetTickCount();
	mSec = int(dwCur - m_dwSetDlySt[nId]);
	if (dwCur < m_dwSetDlyEd[nId])
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::GetDelay0(int &mSec, int nId) // F:Done, T:On Waiting....
{
	DWORD dwCur = GetTickCount();
	mSec = int(dwCur - m_dwSetDlySt0[nId]);
	if (dwCur < m_dwSetDlyEd0[nId])
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::GetDelay1(int &mSec, int nId) // F:Done, T:On Waiting....
{
	DWORD dwCur = GetTickCount();
	mSec = int(dwCur - m_dwSetDlySt1[nId]);
	if (dwCur < m_dwSetDlyEd1[nId])
		return TRUE;
	return FALSE;
}

void CGvisR2R_PunchView::MoveAoi(double dOffset)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	long lData = (long)(dOffset * 1000.0);
	if(bDualTest)
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiDn, lData);	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
	else
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiUp, lData);	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
	MpeWrite(Plc.DlgMenu01.FeedCwAoi, 1);	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
}

void CGvisR2R_PunchView::MoveMk(double dOffset)
{
	long lData = (long)(dOffset * 1000.0);
	MpeWrite(Plc.DlgFrameHigh.FeedOffsetPunch, lData);	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
	MpeWrite(Plc.DlgMenu01.FeedCwPunch, 1);		// 마킹부 피딩 CW ON (PLC가 피딩완료 후 OFF))
}

void CGvisR2R_PunchView::SetCycTime()
{
	DWORD dCur = GetTickCount();
	if (m_dwCycSt > 0)
	{
		m_dwCycTim = (double)(dCur - m_dwCycSt);
		if (m_dwCycTim < 0.0)
			m_dwCycTim *= (-1.0);
	}
	else
		m_dwCycTim = 0.0;
}

int CGvisR2R_PunchView::GetCycTime()
{
	if (m_dwCycTim < 0)
		m_dwCycTim = 0;

	int nTim = int(m_dwCycTim);
	return nTim;
}

//BOOL CGvisR2R_PunchView::IsMkFdDone()
//{
//	if (m_nShareDnCnt > 0)
//	{
//		if (m_nShareDnCnt % 2)
//			return TRUE;
//	}
//#ifdef USE_MPE
//	if (!(pDoc->m_pMpeSignal[5] & (0x01 << 1)))	// 마킹부 피딩 ON (PLC가 피딩완료 후 OFF)
//	{
//		pDoc->LogAuto(_T("PLC: 마킹부 피딩 OFF (PLC가 피딩완료 후 OFF)"));
//		return TRUE;
//	}
//#endif
//	return FALSE;
//}

//BOOL CGvisR2R_PunchView::IsAoiFdDone()
//{
//#ifdef USE_MPE
//	if (!(pDoc->m_pMpeSignal[5] & (0x01 << 0)))	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
//	{
//		pDoc->LogAuto(_T("PLC: 검사부 피딩 ON (PLC가 피딩완료 후 OFF)"));
//		return TRUE;
//	}
//#endif
//	return FALSE;
//}

double CGvisR2R_PunchView::GetAoi2InitDist()
{
	double dInitD = (_tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) - 1.0) * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	return dInitD;
}

double CGvisR2R_PunchView::GetMkInitDist()
{
	double dInitD0 = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	double dInitD1 = (_tstof(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist) + dInitD0) - (_tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen)*2.0);
	return dInitD1;
}

double CGvisR2R_PunchView::GetRemain()
{
	if (!m_pMpe || !pDoc->m_pMpeData)
		return 0.0;
	double dCurPosMkFd = (double)pDoc->m_pMpeData[2][2];	// 마킹부 Feeding 엔코더 값(단위 mm )
	double dInitD0 = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	double dRemain = _tstof(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist) + dInitD0 - dCurPosMkFd;
	return dRemain;
}

void CGvisR2R_PunchView::UpdateWorking()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateWorking();
	if (m_pEngrave)
		m_pEngrave->SwMenu01UpdateWorking(TRUE);
}

void CGvisR2R_PunchView::StopFromThread()
{
	m_bStopFromThread = TRUE;
}

void CGvisR2R_PunchView::Stop()
{
	CString sMsg;
	if (m_pDlgMenu03)
	{
		if (IsAuto() && IsRun())
		{
			m_bStopF_Verify = TRUE; pDoc->SetStatus(_T("General"), _T("bStopF_Verify"), m_bStopF_Verify);
		}

		m_pDlgMenu03->SwStop();
	}
}

BOOL CGvisR2R_PunchView::IsStop()
{
	if (m_sDispMain == _T("정 지"))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsRun()
{
	if (m_sDispMain == _T("정 지"))
		return FALSE;
	return TRUE;
	//if ( m_sDispMain == _T("초기운전")
	//	|| m_sDispMain == _T("운전중") || m_sDispMain == _T("운전준비")  
	//	|| m_sDispMain == _T("단면검사") || m_sDispMain == _T("단면샘플")
	//	|| m_sDispMain == _T("양면검사") || m_sDispMain == _T("양면샘플")
	//	|| m_sDispMain == _T("내층검사") || m_sDispMain == _T("외층검사") )		
	//	return TRUE;

	//return FALSE;
}

void CGvisR2R_PunchView::ShowLive(BOOL bShow)
{
	if (bShow)
	{
		if (!IsShowLive())
			SetTimer(TIM_SHOW_MENU02, 30, NULL);
	}
	else
	{
		if (IsShowLive())
			SetTimer(TIM_SHOW_MENU01, 30, NULL);
	}
}

BOOL CGvisR2R_PunchView::IsShowLive()
{
	if (m_pDlgMenu02)
	{
		if (m_pDlgMenu02->IsWindowVisible())
			return TRUE;
	}

	return FALSE;
}

void CGvisR2R_PunchView::SetLotSt()
{
	stLotTime LotTime;
	GetTime(LotTime);

	pDoc->WorkingInfo.Lot.StTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.StTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.StTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.StTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.StTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.StTime.nSec = LotTime.nSec;

	pDoc->WorkingInfo.Lot.CurTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.CurTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.CurTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.CurTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.CurTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.CurTime.nSec = LotTime.nSec;

	pDoc->WorkingInfo.Lot.EdTime.nYear = 0;
	pDoc->WorkingInfo.Lot.EdTime.nMonth = 0;
	pDoc->WorkingInfo.Lot.EdTime.nDay = 0;
	pDoc->WorkingInfo.Lot.EdTime.nHour = 0;
	pDoc->WorkingInfo.Lot.EdTime.nMin = 0;
	pDoc->WorkingInfo.Lot.EdTime.nSec = 0;

	m_dwLotSt = GetTickCount();
	pDoc->SaveLotTime(m_dwLotSt);
	DispLotTime();

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->SetLotSt();
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->SetLotSt();
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->SetLotSt();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->SetLotSt();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->SetLotSt();
	}
}

void CGvisR2R_PunchView::SetLotEd()
{
	stLotTime LotTime;
	GetTime(LotTime);

	pDoc->WorkingInfo.Lot.EdTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.EdTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.EdTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.EdTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.EdTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.EdTime.nSec = LotTime.nSec;

	pDoc->WorkingInfo.Lot.CurTime.nYear = LotTime.nYear;
	pDoc->WorkingInfo.Lot.CurTime.nMonth = LotTime.nMonth;
	pDoc->WorkingInfo.Lot.CurTime.nDay = LotTime.nDay;
	pDoc->WorkingInfo.Lot.CurTime.nHour = LotTime.nHour;
	pDoc->WorkingInfo.Lot.CurTime.nMin = LotTime.nMin;
	pDoc->WorkingInfo.Lot.CurTime.nSec = LotTime.nSec;

	m_dwLotEd = GetTickCount();

	pDoc->SaveLotTime(pDoc->WorkingInfo.Lot.dwStTick);
	DispLotTime();

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->SetLotEd();
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->SetLotEd();
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->SetLotEd();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->SetLotEd();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->SetLotEd();
	}

}

void CGvisR2R_PunchView::DispLotTime()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->DispLotTime();
}

BOOL CGvisR2R_PunchView::IsTest()
{
#ifdef USE_MPE
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn0 = (pDoc->m_pMpeIo[nInSeg + 10] & 0x01 << 8) ? TRUE : FALSE; //[34] 검사부 상 검사 시작 <-> Y4368 I/F
	BOOL bOn1 = (pDoc->m_pMpeIo[nInSeg + 14] & 0x01 << 8) ? TRUE : FALSE; //[38] 검사부 하 검사 시작 <-> Y4468 I/F

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (bOn0 || bOn1)
			return TRUE;
	}
	else
	{
		if (bOn0)
			return TRUE;
	}
#endif
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsTestUp()
{
#ifdef USE_MPE
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn0 = (pDoc->m_pMpeIo[nInSeg + 10] & 0x01 << 8) ? TRUE : FALSE; //[34] 검사부 상 검사 시작 <-> Y4368 I/F
	if (bOn0)
		return TRUE;
#endif
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsTestDn()
{
#ifdef USE_MPE
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return FALSE;

	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn1 = (pDoc->m_pMpeIo[nInSeg + 14] & 0x01 << 8) ? TRUE : FALSE; //[38] 검사부 하 검사 시작 <-> Y4468 I/F
	if (bOn1)
		return TRUE;
#endif
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsAoiTblVac()
{
#ifdef USE_MPE
	if (!pDoc->m_pMpeIo)
		return FALSE;

	// MpeIO
	int nInSeg = pDoc->MkIo.MpeIo.nInSeg;
	int nOutSeg = pDoc->MkIo.MpeIo.nOutSeg;

	BOOL bOn0 = (pDoc->m_pMpeIo[nInSeg + 10] & 0x01 << 9) ? TRUE : FALSE; //[34] 검사부 상 검사 테이블 진공 SOL <-> Y4469 I/F
	BOOL bOn1 = (pDoc->m_pMpeIo[nInSeg + 14] & 0x01 << 9) ? TRUE : FALSE; //[38] 검사부 하 검사 테이블 진공 SOL <-> Y4469 I/F
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (bOn0 || bOn1)
			return TRUE;
	}
	else
	{
		if (bOn0)
			return TRUE;
	}
#endif
	return FALSE;
}

void CGvisR2R_PunchView::SetTest(BOOL bOn)
{
	if (bOn)
	{
		MpeWrite(Plc.DlgMenu01.TestStartAoiUp, 1); // 검사부 상 검사 시작 <-> Y4368 I/F
		MpeWrite(Plc.DlgMenu01.TestStartAoiDn, 1); // 검사부 하 검사 시작 <-> Y4468 I/F
	}
	else
	{
		MpeWrite(Plc.DlgMenu01.TestStartAoiUp, 0); // 검사부 상 검사 시작 <-> Y4368 I/F
		MpeWrite(Plc.DlgMenu01.TestStartAoiDn, 0); // 검사부 하 검사 시작 <-> Y4468 I/F
	}
}

void CGvisR2R_PunchView::SetTest0(BOOL bOn)
{
	if (bOn)
		MpeWrite(Plc.DlgMenu01.TestStartAoiUp, 1); // 검사부 상 검사 시작 <-> Y4368 I/F
	else
		MpeWrite(Plc.DlgMenu01.TestStartAoiUp, 0); // 검사부 상 검사 시작 <-> Y4368 I/F
}

void CGvisR2R_PunchView::SetTest1(BOOL bOn)
{
	if (bOn)
		MpeWrite(Plc.DlgMenu01.TestStartAoiDn, 1); // 검사부 하 검사 시작 <-> Y4468 I/F
	else
		MpeWrite(Plc.DlgMenu01.TestStartAoiDn, 0); // 검사부 하 검사 시작 <-> Y4468 I/F
}

BOOL CGvisR2R_PunchView::IsTestDone()
{
#ifdef USE_MPE
	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 8)) ? TRUE : FALSE;	// 검사부 상 검사 완료 <-> X4328 I/F
	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01 << 8)) ? TRUE : FALSE;	// 검사부 하 검사 완료 <-> X4428 I/F
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (bOn0 && bOn1)
		{
			pDoc->LogAuto(_T("PLC: 검사부 상(X4328) & 하(X4428) 검사 완료"));
			return TRUE;
		}
	}
	else
	{
		if (bOn0)
		{
			pDoc->LogAuto(_T("PLC: 검사부 상(X4328) 검사 완료"));
			return TRUE;
		}
	}

	double dCurPosMkFd = (double)pDoc->m_pMpeData[2][2];	// 마킹부 Feeding 엔코더 값(단위 mm )
	double dTgtFd = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.Motion.sAoiFdDist);
	if (dCurPosMkFd + 10.0 < dTgtFd)//-_tstof(pDoc->WorkingInfo.Motion.sAoiFdDist))
	{
		if (bOn0)
			return TRUE;
	}
#endif
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsAoiTblVacDone()
{
#ifdef USE_MPE
	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 9)) ? TRUE : FALSE;	// 검사부 상 테이블 진공 완료 <-> X4329 I/F
	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01 << 9)) ? TRUE : FALSE;	// 검사부 하 테이블 진공 완료 <-> X4329 I/F
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (bOn0 && bOn1)
		{
			pDoc->LogAuto(_T("PLC: 검사부 상(X4329) & 하(X4329) 테이블 진공 완료"));
			return TRUE;
		}
	}
	else
	{
		if (bOn0)
		{
			pDoc->LogAuto(_T("PLC: 검사부 상(X4329) 테이블 진공 완료"));
			return TRUE;
		}
	}

	double dCurPosMkFd = (double)pDoc->m_pMpeData[2][2];	// 마킹부 Feeding 엔코더 값(단위 mm )
	double dTgtFd = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.Motion.sAoiFdDist);
	if (dCurPosMkFd + 10.0 < dTgtFd)//-_tstof(pDoc->WorkingInfo.Motion.sAoiFdDist))
	{
		if (bOn0)
			return TRUE;
	}
#endif
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsTestDoneUp()
{
#ifdef USE_MPE
	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 8)) ? TRUE : FALSE;	// 검사부 상 검사 완료 <-> X4328 I/F
	if (bOn0)
	{
		pDoc->LogAuto(_T("PLC: 검사부 상(X4328) 검사 완료"));
		return TRUE;
	}
#endif
	return TRUE;
}

BOOL CGvisR2R_PunchView::IsTestDoneDn()
{
#ifdef USE_MPE
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return TRUE;

	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01 << 8)) ? TRUE : FALSE;	// 검사부 하 검사 완료 <-> X4428 I/F
	if (bOn1)
	{
		pDoc->LogAuto(_T("PLC: 검사부 하(X4428) 검사 완료"));
		return TRUE;
	}
#endif
	return TRUE;
}

void CGvisR2R_PunchView::SetReMk(BOOL bMk0, BOOL bMk1)
{
	CfPoint ptPnt;
	int nSerial, nTot;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bMk0)
	{
		if (!m_bTHREAD_MK[0])
		{
			m_nStepMk[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			m_nMkPcs[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
			m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
			m_bTHREAD_MK[0] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[0]"), m_bTHREAD_MK[0]);

			nSerial = m_nBufUpSerial[0]; // Cam0

			nTot = GetTotDefPcs0(nSerial);
			if (nTot>0)
			{
				ptPnt = GetMkPnt0(nSerial, 0);
				m_dTarget[AXIS_X0] = ptPnt.x;
				m_dTarget[AXIS_Y0] = ptPnt.y;
				if (nTot>1)
				{
					ptPnt = GetMkPnt0(nSerial, 1);
					m_dNextTarget[AXIS_X0] = ptPnt.x;
					m_dNextTarget[AXIS_Y0] = ptPnt.y;
				}
				else
				{
					m_dNextTarget[AXIS_X0] = -1.0;
					m_dNextTarget[AXIS_Y0] = -1.0;
				}
			}
			else
			{
				m_dTarget[AXIS_X0] = -1.0;
				m_dTarget[AXIS_Y0] = -1.0;
				m_dNextTarget[AXIS_X0] = -1.0;
				m_dNextTarget[AXIS_Y0] = -1.0;
			}
		}
	}

	if (bMk1)
	{
		if (!m_bTHREAD_MK[1])
		{
			m_nStepMk[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			m_nMkPcs[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
			m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
			m_bTHREAD_MK[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);

			nSerial = m_nBufUpSerial[1]; // Cam1

			nTot = GetTotDefPcs1(nSerial);
			if (nTot>0)
			{
				ptPnt = GetMkPnt1(nSerial, 0);
				m_dTarget[AXIS_X1] = ptPnt.x;
				m_dTarget[AXIS_Y1] = ptPnt.y;
				if (nTot>1)
				{
					ptPnt = GetMkPnt1(nSerial, 1);
					m_dNextTarget[AXIS_X1] = ptPnt.x;
					m_dNextTarget[AXIS_Y1] = ptPnt.y;
				}
				else
				{
					m_dNextTarget[AXIS_X1] = -1.0;
					m_dNextTarget[AXIS_Y1] = -1.0;
				}
			}
			else
			{
				m_dTarget[AXIS_X1] = -1.0;
				m_dTarget[AXIS_Y1] = -1.0;
				m_dNextTarget[AXIS_X1] = -1.0;
				m_dNextTarget[AXIS_Y1] = -1.0;
			}
		}
	}
}

BOOL CGvisR2R_PunchView::SetMk(BOOL bRun)	// Marking Start
{
	CfPoint ptPnt;
	int nSerial, nTot, a, b;
	CString sItem;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	for (a = 0; a<2; a++)
	{
		for (b = 0; b<MAX_STRIP_NUM; b++)
		{
			m_nMkStrip[a][b] = 0;			// [nCam][nStrip]:[2][4] - [좌/우][] : 스트립에 펀칭한 피스 수 count
			m_bRejectDone[a][b] = FALSE;	// [nCam][nStrip]:[2][4] - [좌/우][] : 스트립에 펀칭한 피스 수 count가 스트립 폐기 설정수 완료 여부 
			sItem.Format(_T("bRejectDone[%d][%d]"), a, b);
			pDoc->SetStatus(_T("General"), sItem, m_bRejectDone[a][b]);
		}
	}

	if (bRun)
	{
		if (m_bDoMk[0] && (m_bMkSt[0] || m_bMkSt[1]))
		{
			if (!m_bTHREAD_MK[0])
			{
				m_nStepMk[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
				m_nMkPcs[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
				m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
				m_bTHREAD_MK[0] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[0]"), m_bTHREAD_MK[0]);

				nSerial = m_nBufUpSerial[0]; // Cam0

				m_nTotMk[0] = nTot = GetTotDefPcs0(nSerial); pDoc->SetStatusInt(_T("General"), _T("nTotMk[0]"), m_nTotMk[0]);
				m_nCurMk[0] = 0;
				if (nTot>0)
				{
					ptPnt = GetMkPnt0(nSerial, 0);
					m_dTarget[AXIS_X0] = ptPnt.x;
					m_dTarget[AXIS_Y0] = ptPnt.y;
					if (nTot>1)
					{
						ptPnt = GetMkPnt0(nSerial, 1);
						m_dNextTarget[AXIS_X0] = ptPnt.x;
						m_dNextTarget[AXIS_Y0] = ptPnt.y;
					}
					else
					{
						m_dNextTarget[AXIS_X0] = -1.0;
						m_dNextTarget[AXIS_Y0] = -1.0;
					}
				}
				else
				{
					m_dTarget[AXIS_X0] = -1.0;
					m_dTarget[AXIS_Y0] = -1.0;
					m_dNextTarget[AXIS_X0] = -1.0;
					m_dNextTarget[AXIS_Y0] = -1.0;
				}
			}
		}
		else
		{
			m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
		}

		if (m_bDoMk[1] && m_bMkSt[1])
		{
			if (!m_bTHREAD_MK[1])
			{
				m_nStepMk[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
				m_nMkPcs[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
				m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
				m_bTHREAD_MK[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);

				nSerial = m_nBufUpSerial[1]; // Cam1

				m_nTotMk[1] = nTot = GetTotDefPcs1(nSerial); pDoc->SetStatusInt(_T("General"), _T("nTotMk[1]"), m_nTotMk[1]);
				m_nCurMk[1] = 0;
				if (nTot>0)
				{
					ptPnt = GetMkPnt1(nSerial, 0);
					m_dTarget[AXIS_X1] = ptPnt.x;
					m_dTarget[AXIS_Y1] = ptPnt.y;
					if (nTot>1)
					{
						ptPnt = GetMkPnt1(nSerial, 1);
						m_dNextTarget[AXIS_X1] = ptPnt.x;
						m_dNextTarget[AXIS_Y1] = ptPnt.y;
					}
					else
					{
						m_dNextTarget[AXIS_X1] = -1.0;
						m_dNextTarget[AXIS_Y1] = -1.0;
					}
				}
				else
				{
					m_dTarget[AXIS_X1] = -1.0;
					m_dTarget[AXIS_Y1] = -1.0;
					m_dNextTarget[AXIS_X1] = -1.0;
					m_dNextTarget[AXIS_Y1] = -1.0;
				}
			}
		}
		else
		{
			m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
		}
	}
	else
	{
		m_bTHREAD_MK[0] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[0]"), m_bTHREAD_MK[0]);
		m_bTHREAD_MK[1] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);
	}
	return TRUE;
}

BOOL CGvisR2R_PunchView::IsReMk()
{
	if (IsMoveDone())
		return FALSE;

	if (m_bReMark[0] && m_bDoneMk[1])
	{
		m_bReMark[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReMark[0]"), m_bReMark[0]);
		SetReMk(TRUE, FALSE);
		return TRUE;
	}
	else if (m_bDoneMk[0] && m_bReMark[1])
	{
		m_bReMark[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReMark[1]"), m_bReMark[1]);
		SetReMk(FALSE, TRUE);
		return TRUE;
	}
	else if (m_bReMark[0] && m_bReMark[1])
	{
		m_bReMark[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReMark[0]"), m_bReMark[0]);
		m_bReMark[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReMark[1]"), m_bReMark[1]);
		SetReMk(TRUE, TRUE);
		return TRUE;
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::IsMkDone()
{
	if (m_bDoneMk[0] && m_bDoneMk[1] && !m_bTHREAD_DISP_DEF)
		return TRUE;

	return FALSE;
}

int CGvisR2R_PunchView::GetSerial()
{
	int nSerial = 0;
	if (m_pDlgMenu01)
		nSerial = m_pDlgMenu01->GetSerial();
	return nSerial;
}

void CGvisR2R_PunchView::SetMkFdLen()
{
	// 	int nLast = pDoc->GetLastSerial();
	int nLast = pDoc->GetLastShotMk();
	pDoc->WorkingInfo.Motion.sMkFdTotLen.Format(_T("%.1f"), (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen));

	CString sData, sPath = PATH_WORKING_INFO;
	sData = pDoc->WorkingInfo.Motion.sMkFdTotLen;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_TOTAL_DIST"), sData, sPath);
}

double CGvisR2R_PunchView::GetMkFdLen()
{
	int nLast = pDoc->GetLastShotMk();
	double dLen = (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);

	return dLen;
}

double CGvisR2R_PunchView::GetAoiUpFdLen()
{
	int nLast = pDoc->GetLastShotUp();

	double dLen = (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	return dLen;
}

double CGvisR2R_PunchView::GetAoiDnFdLen()
{
	int nLast = pDoc->GetLastShotDn();

	double dLen = (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	return dLen;
}

double CGvisR2R_PunchView::GetTotVel()
{
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

	int nTotSec = 0;
	double dMkFdLen = GetMkFdLen();
	if (!nStYear && !nStMonth && !nStDay && !nStHour && !nStMin && !nStSec)
	{
		return 0.0;
	}
	else if (!nEdYear && !nEdMonth && !nEdDay && !nEdHour && !nEdMin && !nEdSec)
	{
		nDiff = (GetTickCount() - pView->m_dwLotSt) / 1000;
		nHour = int(nDiff / 3600);
		nMin = int((nDiff - 3600 * nHour) / 60);
		nSec = nDiff % 60;
	}
	else
	{
		nDiff = (pView->m_dwLotEd - pView->m_dwLotSt) / 1000;
		nHour = int(nDiff / 3600);
		nMin = int((nDiff - 3600 * nHour) / 60);
		nSec = nDiff % 60;
	}

	nTotSec = nHour * 3600 + nMin * 60 + nSec;
	double dVel = 0.0;
	if (nTotSec > 0)
		dVel = dMkFdLen / (double)nTotSec; // [mm/sec]
	m_dTotVel = dVel;
	return dVel;
}

double CGvisR2R_PunchView::GetPartVel()
{
	double dLen = _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen) * 2.0;
	double dSec = (double)GetCycTime() / 1000.0;
	double dVel = 0.0;
	if (dSec > 0.0)
		dVel = dLen / dSec; // [mm/sec]
	m_dPartVel = dVel;
	return dVel;
}


BOOL CGvisR2R_PunchView::IsBuffer(int nNum)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_bVsStatusUp)
	{
		if(!m_bLotEnd)
			nNum = 1;

		if (m_bLastProc)
		{
			int nLastShot = GetLotEndSerial();
			if (bDualTest)
			{
				if (pDoc->m_ListBuf[0].GetLast() == nLastShot && pDoc->m_ListBuf[1].GetLast() == nLastShot)
				{
					nNum = 0;
					if (!m_bLotEnd)
					{
						m_bLotEndF = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEndF"), m_bLotEndF);
					}
				}
				else
					nNum = 1;
			}
			else
			{
				if (pDoc->m_ListBuf[0].GetLast() == nLastShot)
				{
					nNum = 0;
					if (!m_bLotEnd)
						m_bLotEndF = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEndF"), m_bLotEndF);
				}
				else
					nNum = 1;
			}
		}
	}

	if (bDualTest)
	{
		if (m_nBufTot[0] > nNum && m_nBufTot[1] > nNum) // [0]: AOI-Up , [1]: AOI-Dn
			return TRUE;

		if (m_bLastProc)
		{
			if ((m_nBufTot[0] > nNum || !m_bIsBuf[0]) && (m_nBufTot[1] > nNum || !m_bIsBuf[1])) // [0]: AOI-Up , [1]: AOI-Dn
				return TRUE;
			//if (m_nBufTot[0] > nNum && !pDoc->Status.PcrShare[1].bExistF) // [0]: AOI-Up
			//	return TRUE;
		}
	}
	else
	{
		if (m_nBufTot[0] > nNum) // [0]: AOI-Up
			return TRUE;
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::IsBufferUp()
{
	if (m_nBufTot[0] > 0)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsBufferDn()
{
	if (m_nBufTot[1] > 0)
		return TRUE;
	return FALSE;
}

int CGvisR2R_PunchView::GetBuffer(int *pPrevSerial)
{
	int nS0 = GetBufferUp(pPrevSerial);
	int nS1 = GetBufferDn(pPrevSerial);
	if (nS0 != nS1)
		return 0;
	return nS0;
}

int CGvisR2R_PunchView::GetBufferUp(int *pPrevSerial)
{
	if (IsBufferUp())
		return m_pBufSerial[0][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[0][0];
	return 0;
}

int CGvisR2R_PunchView::GetBufferDn(int *pPrevSerial)
{
	if (IsBufferDn())
		return m_pBufSerial[1][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[1][0];
	return 0;
}

BOOL CGvisR2R_PunchView::IsBuffer0()
{
	if (m_nBufTot[0] > 0 && m_nBufTot[1] > 0)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsBufferUp0()
{
	if (m_nBufTot[0] > 0)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsBufferDn0()
{
	if (m_nBufTot[1] > 0)
		return TRUE;
	return FALSE;
}

int CGvisR2R_PunchView::GetBuffer0(int *pPrevSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nS0, nS1;
	if (bDualTest)
	{
		nS0 = GetBufferUp0(pPrevSerial);
		nS1 = GetBufferDn0(pPrevSerial);
		if (nS0 != nS1)
			return 0;
	}
	else
	{
		nS0 = GetBufferUp0(pPrevSerial);
	}
	return nS0;
}

int CGvisR2R_PunchView::GetBufferUp0(int *pPrevSerial)
{
	if (IsBufferUp0())
		return m_pBufSerial[0][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[0][0];
	return 0;
}

int CGvisR2R_PunchView::GetBufferDn0(int *pPrevSerial)
{
	if (IsBufferDn0())
		return m_pBufSerial[1][0];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[1][0];
	return 0;
}



BOOL CGvisR2R_PunchView::IsBuffer1()
{
	if (m_nBufTot[0] > 1 && m_nBufTot[1] > 1)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsBufferUp1()
{
	if (m_nBufTot[0] > 1)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsBufferDn1()
{
	if (m_nBufTot[1] > 1)
		return TRUE;
	return FALSE;
}

int CGvisR2R_PunchView::GetBuffer1(int *pPrevSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nS0, nS1;
	if (bDualTest)
	{
		nS0 = GetBufferUp1(pPrevSerial);
		nS1 = GetBufferDn1(pPrevSerial);
		if (nS0 != nS1)
			return 0;
	}
	else
		nS0 = GetBufferUp1(pPrevSerial);

	return nS0;
}

int CGvisR2R_PunchView::GetBufferUp1(int *pPrevSerial)
{
	if (IsBufferUp1())
		return m_pBufSerial[0][1];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[0][1];
	return 0;
}

int CGvisR2R_PunchView::GetBufferDn1(int *pPrevSerial)
{
	if (IsBufferDn1())
		return m_pBufSerial[1][1];
	else if (pPrevSerial)
		*pPrevSerial = m_pBufSerial[1][1];
	return 0;
}

BOOL CGvisR2R_PunchView::IsShare()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		if (m_bWaitPcr[0] && m_bWaitPcr[1])
		{
			if (IsShareUp() && IsShareDn())
			{
				m_bWaitPcr[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[0]"), m_bWaitPcr[0]);
				m_bWaitPcr[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[1]"), m_bWaitPcr[1]);
				if (m_pDlgMenu01)
					m_pDlgMenu01->ChkAoiVsStatus();
				return TRUE;
			}
		}
		else if (m_bWaitPcr[0] && !m_bWaitPcr[1])
		{
			if (IsShareUp())
			{
				m_bWaitPcr[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[0]"), m_bWaitPcr[0]);
				if (m_pDlgMenu01)
					m_pDlgMenu01->ChkAoiVsStatus();
				return TRUE;
			}
		}
		else if (!m_bWaitPcr[0] && m_bWaitPcr[1])
		{
			if (IsShareDn())
			{
				m_bWaitPcr[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[1]"), m_bWaitPcr[1]);
				if (m_pDlgMenu01)
					m_pDlgMenu01->ChkAoiVsStatus();
				return TRUE;
			}
		}
		else
		{
			if (IsShareUp() || IsShareDn())
			{
				if (m_pDlgMenu01)
					m_pDlgMenu01->ChkAoiVsStatus();
				return TRUE;
			}
		}
	}
	else
	{
		if (m_bWaitPcr[0])
		{
			if (IsShareUp())
			{
				m_bWaitPcr[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[0]"), m_bWaitPcr[0]);
				if (m_pDlgMenu01)
					m_pDlgMenu01->ChkAoiVsStatus();
				return TRUE;
			}
		}
		else
		{
			if (IsShareUp())
			{
				if (m_pDlgMenu01)
					m_pDlgMenu01->ChkAoiVsStatus();
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsShareUp()
{
	return pDoc->Status.PcrShare[0].bExist;
}

BOOL CGvisR2R_PunchView::IsShareDn()
{
	return pDoc->Status.PcrShare[1].bExist;
}

int CGvisR2R_PunchView::GetShareUp()
{
	return pDoc->Status.PcrShare[0].nSerial;
}

int CGvisR2R_PunchView::GetShareDn()
{
	return pDoc->Status.PcrShare[1].nSerial;
}

BOOL CGvisR2R_PunchView::IsShareVs()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		if (IsShareVsUp() || IsShareVsDn())
			return TRUE;
	}
	else
	{
		if (IsShareVsUp())
			return TRUE;
	}
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsShareVsUp()
{
	return pDoc->Status.PcrShareVs[0].bExist;
}

BOOL CGvisR2R_PunchView::IsShareVsDn()
{
	return pDoc->Status.PcrShareVs[1].bExist;
}

int CGvisR2R_PunchView::GetShareVsUp()
{
	return pDoc->Status.PcrShareVs[0].nSerial;
}

int CGvisR2R_PunchView::GetShareVsDn()
{
	return pDoc->Status.PcrShareVs[1].nSerial;
}


BOOL CGvisR2R_PunchView::ChkLastProc()
{
	BOOL bRtn = FALSE;
	if (m_pDlgMenu01)
		bRtn = (m_pDlgMenu01->m_bLastProc);
	return bRtn;
}

BOOL CGvisR2R_PunchView::ChkLastProcFromUp()
{
	BOOL bRtn = TRUE;
	if (m_pDlgMenu01)
		bRtn = (m_pDlgMenu01->m_bLastProcFromUp);
	return bRtn;
}

BOOL CGvisR2R_PunchView::ChkLastProcFromEng()
{
	BOOL bRtn = TRUE;
	if (m_pDlgMenu01)
		bRtn = (m_pDlgMenu01->m_bLastProcFromEng);
	return bRtn;
}

BOOL CGvisR2R_PunchView::ChkLotEnd(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.25"));
		return FALSE;
	}

	BOOL bEnd0 = ChkLotEndUp(nSerial);
	BOOL bEnd1 = ChkLotEndDn(nSerial);
	if (bEnd0 || bEnd1)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::ChkLotEndUp(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.26"));
		return 0;
	}

	CString sPath;
	sPath.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
	return pDoc->ChkLotEnd(sPath);
}

BOOL CGvisR2R_PunchView::ChkLotEndDn(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return FALSE;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.27"));
		return 0;
	}

	CString sPath;
	sPath.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufDn, nSerial);
	return pDoc->ChkLotEnd(sPath);
}

BOOL CGvisR2R_PunchView::IsMkTmpStop()
{
	return m_bMkTmpStop;
}

BOOL CGvisR2R_PunchView::SetSerial(int nSerial, BOOL bDumy)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.28"));
		return 0;
	}

	if (!m_pDlgMenu01)
		return FALSE;

	int nPrevSerial = m_pDlgMenu01->GetCurSerial();

	if (nPrevSerial == nSerial)
		return TRUE;

	BOOL bRtn[2] = {1,1};
	bRtn[0] = m_pDlgMenu01->SetSerial(nSerial, bDumy);
	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_pDlgMenu06)
			bRtn[1] = m_pDlgMenu06->SetSerial(nSerial, bDumy);
	}
	else
		bRtn[1] = TRUE;

	return (bRtn[0] && bRtn[1]);
}

BOOL CGvisR2R_PunchView::SetSerialReelmap(int nSerial, BOOL bDumy)
{
	if (!m_pDlgMenu01)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error - SetSerialReelmap : m_pDlgMenu01 is NULL."));
		return FALSE;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (!m_pDlgMenu06)
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error - SetSerialReelmap : m_pDlgMenu06 is NULL."));
			return FALSE;
		}

		m_pDlgMenu06->SetSerialReelmap(nSerial, bDumy);
	}
	
	//if(pDoc->m_pReelMapDisp)
	//	pDoc->m_pReelMapDisp->ShiftMkedPcsDef(nSerial);

	return m_pDlgMenu01->SetSerialReelmap(nSerial, bDumy);
}

BOOL CGvisR2R_PunchView::SetSerialMkInfo(int nSerial, BOOL bDumy)
{
	if (!m_pDlgMenu01)
		return FALSE;
	return m_pDlgMenu01->SetSerialMkInfo(nSerial, bDumy);
}

void CGvisR2R_PunchView::InitAuto(BOOL bInit)
{
	CString sItem;

	if (!pDoc->WorkingInfo.LastJob.bSampleTest)
	{
		::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
		::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
	}

	m_sAoiUpAlarmReStartMsg = GetAoiUpAlarmRestartMsg();
	m_sAoiDnAlarmReStartMsg = GetAoiDnAlarmRestartMsg();
	m_sAoiUpAlarmReTestMsg = GetAoiUpAlarmReTestMsg();
	m_sAoiDnAlarmReTestMsg = GetAoiDnAlarmReTestMsg();

	SetAoiUpAutoStep(0);
	SetAoiDnAutoStep(0);

	m_nAoiUpAutoSerial = 0;
	m_nAoiUpAutoSerialPrev = 0;
	m_nAoiDnAutoSerial = 0;
	m_nAoiDnAutoSerialPrev = 0;

	int nCam, nPoint, kk, a, b;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	for (kk = 0; kk < 10; kk++)
	{
		m_bDispMsgDoAuto[kk] = FALSE;
		m_nStepDispMsg[kk] = 0;
	}
	m_sFixMsg[0] = _T("");
	m_sFixMsg[1] = _T("");

	m_bReadyDone = FALSE; pDoc->SetStatus(_T("General"), _T("bReadyDone"), pView->m_bReadyDone);
	m_bChkLastProcVs = FALSE;
	m_nDummy[0] = 0;
	m_nDummy[1] = 0;
	m_nAoiLastSerial[0] = 0;
	m_nAoiLastSerial[1] = 0;
	m_nStepAuto = 0; pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
	m_nPrevStepAuto = 0; pDoc->SetStatusInt(_T("General"), _T("nPrevStepAuto"), m_nPrevStepAuto);
	m_nPrevMkStAuto = 0; pDoc->SetStatusInt(_T("General"), _T("nPrevMkStAuto"), m_nPrevMkStAuto);
	m_bAoiLdRun = TRUE;
	m_bAoiLdRunF = FALSE;
	m_bNewModel = FALSE;
	m_nLotEndSerial = 0; pDoc->SetStatusInt(_T("General"), _T("nLotEndSerial"), m_nLotEndSerial);
	if(m_pDlgMenu01)
		m_pDlgMenu01->DispLotEndSerial(0);
	m_bCam = FALSE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
	m_bReview = FALSE;
	m_bChkBufIdx[0] = TRUE;
	m_bChkBufIdx[0] = TRUE;

	m_nErrCnt = 0;

	m_nStepMk[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
	m_nStepMk[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
	m_nStepMk[2] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
	m_nStepMk[3] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
	m_bTHREAD_MK[0] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[0]"), m_bTHREAD_MK[0]);
	m_bTHREAD_MK[1] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);
	m_bTHREAD_MK[2] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[2]"), m_bTHREAD_MK[2]);
	m_bTHREAD_MK[3] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[3]"), m_bTHREAD_MK[3]);
	m_nMkPcs[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
	m_nMkPcs[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
	m_nMkPcs[2] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[2]"), m_nMkPcs[2]);
	m_nMkPcs[3] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[3]"), m_nMkPcs[3]);

	m_bMkTmpStop = FALSE;

	m_bWaitPcr[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[0]"), m_bWaitPcr[0]);
	m_bWaitPcr[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[1]"), m_bWaitPcr[1]);


	m_nShareUpS = 0;
	m_nShareUpSerial[0] = 0;
	m_nShareUpSerial[1] = 0;
	m_nShareUpCnt = 0;

	m_nShareDnS = 0;
	m_nShareDnSerial[0] = 0;
	m_nShareDnSerial[1] = 0;
	m_nShareDnCnt = 0;

	m_nBufUpSerial[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[0]"), m_nBufUpSerial[0]);
	m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
	m_nBufUpCnt = 0;

	m_nBufDnSerial[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[0]"), m_nBufDnSerial[0]);
	m_nBufDnSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[1]"), m_nBufDnSerial[1]);
	m_nBufDnCnt = 0;

	if (m_pDlgMenu02)
	{
		for (nCam = 0; nCam < 2; nCam++)
		{
			for (nPoint = 0; nPoint < 4; nPoint++)
			{
				m_pDlgMenu02->m_dMkFdOffsetX[nCam][nPoint] = 0.0;
				m_pDlgMenu02->m_dMkFdOffsetY[nCam][nPoint] = 0.0;
			}
		}


		m_pDlgMenu02->m_dAoiUpFdOffsetX = 0.0;
		m_pDlgMenu02->m_dAoiUpFdOffsetY = 0.0;
		m_pDlgMenu02->m_dAoiDnFdOffsetX = 0.0;
		m_pDlgMenu02->m_dAoiDnFdOffsetY = 0.0;

		m_pDlgMenu02->InitMkPmRst();
		m_pDlgMenu02->DispMkPmScore(0);
		m_pDlgMenu02->DispMkPmScore(1);
	}

	m_bReAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][0]"), m_bReAlign[0][0]);	// [nCam][nPos]
	m_bReAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][1]"), m_bReAlign[0][1]);	// [nCam][nPos]
	m_bReAlign[0][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][2]"), m_bReAlign[0][2]);	// [nCam][nPos]
	m_bReAlign[0][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][3]"), m_bReAlign[0][3]);	// [nCam][nPos]
	m_bReAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][0]"), m_bReAlign[1][0]);	// [nCam][nPos]
	m_bReAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][1]"), m_bReAlign[1][1]);	// [nCam][nPos]
	m_bReAlign[1][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][2]"), m_bReAlign[1][2]);	// [nCam][nPos]
	m_bReAlign[1][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][3]"), m_bReAlign[1][3]);	// [nCam][nPos]

	m_bSkipAlign[0][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]); // [nCam][nPos]
	m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);	// [nCam][nPos]
	m_bSkipAlign[0][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][2]"), m_bSkipAlign[0][2]);	// [nCam][nPos]
	m_bSkipAlign[0][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][3]"), m_bSkipAlign[0][3]);	// [nCam][nPos]
	m_bSkipAlign[1][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);	// [nCam][nPos]
	m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);	// [nCam][nPos]
	m_bSkipAlign[1][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);	// [nCam][nPos]
	m_bSkipAlign[1][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);	// [nCam][nPos]

	m_bFailAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]);	// [nCam][nPos]
	m_bFailAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]);	// [nCam][nPos]
	m_bFailAlign[0][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][2]"), m_bFailAlign[0][2]);	// [nCam][nPos]
	m_bFailAlign[0][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][3]"), m_bFailAlign[0][3]);	// [nCam][nPos]
	m_bFailAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]);	// [nCam][nPos]
	m_bFailAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]);	// [nCam][nPos]
	m_bFailAlign[1][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][2]"), m_bFailAlign[1][2]);	// [nCam][nPos]
	m_bFailAlign[1][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][3]"), m_bFailAlign[1][3]);	// [nCam][nPos]

	m_bDoMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);			// [nCam]
	m_bDoMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);			// [nCam]
	m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);		// [nCam]
	m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);		// [nCam]
	m_bReMark[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReMark[0]"), m_bReMark[0]);		// [nCam]
	m_bReMark[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReMark[1]"), m_bReMark[1]);		// [nCam]

	m_nTotMk[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nTotMk[0]"), m_nTotMk[0]);
	m_nCurMk[0] = 0;
	m_nTotMk[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nTotMk[1]"), m_nTotMk[1]);
	m_nCurMk[1] = 0;
	m_nPrevTotMk[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nPrevTotMk[0]"), m_nPrevTotMk[0]);
	m_nPrevCurMk[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nPrevCurMk[0]"), m_nPrevCurMk[0]);
	m_nPrevTotMk[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nPrevTotMk[1]"), m_nPrevTotMk[1]);
	m_nPrevCurMk[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nPrevCurMk[1]"), m_nPrevCurMk[1]);	

	m_bMkSt[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt0"), m_bMkSt[0]);
	::WritePrivateProfileString(_T("Last Job"), _T("MkSt0"), _T("0"), PATH_WORKING_INFO);
	m_bMkSt[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt1"), m_bMkSt[1]);
	::WritePrivateProfileString(_T("Last Job"), _T("MkSt1"), _T("0"), PATH_WORKING_INFO);
	m_bMkStSw[0] = FALSE;
	m_bMkStSw[1] = FALSE;
	m_nMkStAuto = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);

	m_bEngSt = FALSE;
	m_bEngStSw = FALSE;
	m_nEngStAuto = 0;

	m_bEng2dSt = FALSE;
	m_bEng2dStSw = FALSE;
	m_nEng2dStAuto = 0;

	m_bLotEnd = FALSE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
	m_bLotEndF = FALSE; pDoc->SetStatus(_T("General"), _T("bLotEndF"), m_bLotEndF);
	m_nLotEndAuto = 0; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);

	m_bLastProc = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
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

	m_nLastProcAuto = 0; pDoc->SetStatusInt(_T("General"), _T("nLastProcAuto"), m_nLastProcAuto);

	pDoc->m_sAlmMsg = _T("");
	pDoc->m_sIsAlmMsg = _T("");
	pDoc->m_sPrevAlmMsg = _T("");

	m_dwCycSt = 0;
	m_sNewLotUp = _T("");
	m_sNewLotDn = _T("");

	m_nStop = 0;

	m_nStepTHREAD_DISP_DEF = 0; pDoc->SetStatusInt(_T("Thread"), _T("nStepTHREAD_DISP_DEF"), m_nStepTHREAD_DISP_DEF);
	m_bTHREAD_DISP_DEF = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_DISP_DEF"), m_bTHREAD_DISP_DEF);	// CopyDefImg Stop
	m_nStepTHREAD_DISP_DEF_INNER = 0; pDoc->SetStatusInt(_T("Thread"), _T("nStepTHREAD_DISP_DEF_INNER"), m_nStepTHREAD_DISP_DEF_INNER);
	m_bTHREAD_DISP_DEF_INNER = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_DISP_DEF_INNER"), m_bTHREAD_DISP_DEF_INNER); // DispDefImg Stop

	pDoc->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("0"));

	for (a = 0; a<2; a++)
	{
		for (b = 0; b<4; b++)
		{
			m_nMkStrip[a][b] = 0;
			m_bRejectDone[a][b] = FALSE;
			sItem.Format(_T("bRejectDone[%d][%d]"), a, b);
			pDoc->SetStatus(_T("General"), sItem, m_bRejectDone[a][b]);
		}
	}

	MpeWrite(Plc.DlgMenu01.PlcReadyDone, 0); // PLC 운전준비 완료(PC가 확인하고 Reset시킴.)
	MpeWrite(Plc.DlgMenu01.MarkingStart, 0); // 마킹시작(PC가 확인하고 Reset시킴.)
	MpeWrite(Plc.DlgMenu01.MarkingDoing, 0); // 마킹부 마킹중 ON (PC가 ON, OFF)
	MpeWrite(Plc.DlgMenu01.MarkingDone, 0); // 마킹완료(PLC가 확인하고 Reset시킴.)
	MpeWrite(Plc.DlgMenu01.MarkingDone, 0); // 마킹완료(PLC가 확인하고 Reset시킴.)
	MpeWrite(Plc.DlgMenu01.InitPlcData, 1); 

	InitAutoEngSignal();

	MoveInitPos1();
	Sleep(30);
	MoveInitPos0();

	InitIoWrite();
	//OpenShareUp();
	//OpenShareDn();
	SetTest(FALSE);	// 검사부 상/하 검사 시작 (Off)
	if (m_pDlgMenu01)
	{
		m_pDlgMenu01->m_bLastProc = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_pDlgMenu01->m_bLastProc);

		if (MODE_INNER != pDoc->GetTestMode())
		{
			if (!pDoc->WorkingInfo.System.bUseEngrave)
			{
				m_pDlgMenu01->m_bLastProcFromUp = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromUp"), m_pDlgMenu01->m_bLastProcFromUp);
				pView->m_bWaitPcr[0] = FALSE;
			}
			else
			{
				m_pDlgMenu01->m_bLastProcFromEng = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
			}
		}
		else
		{
			m_pDlgMenu01->m_bLastProcFromEng = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
		}

		m_pDlgMenu01->ResetSerial();
		m_pDlgMenu01->ResetLastProc();
		m_pDlgMenu01->ChkAoiVsStatus();
	}


	m_bTHREAD_UPDATAE_YIELD[0] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATAE_YIELD[0]"), m_bTHREAD_UPDATAE_YIELD[0]);
	m_bTHREAD_UPDATAE_YIELD[1] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATAE_YIELD[1]"), m_bTHREAD_UPDATAE_YIELD[1]);
	m_nSerialTHREAD_UPDATAE_YIELD[0] = 0;
	m_nSerialTHREAD_UPDATAE_YIELD[1] = 0;

	m_bTHREAD_UPDATE_REELMAP_UP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_UP"), m_bTHREAD_UPDATE_REELMAP_UP);
	m_bTHREAD_UPDATE_REELMAP_ALLUP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_ALLUP"), m_bTHREAD_UPDATE_REELMAP_ALLUP);
	m_bTHREAD_UPDATE_REELMAP_DN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_DN"), m_bTHREAD_UPDATE_REELMAP_DN);
	m_bTHREAD_UPDATE_REELMAP_ALLDN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_ALLDN"), m_bTHREAD_UPDATE_REELMAP_ALLDN);
	m_bTHREAD_UPDATE_REELMAP_ITS = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_ITS"), m_bTHREAD_UPDATE_REELMAP_ITS);
	m_bTHREAD_MAKE_ITS_FILE_UP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MAKE_ITS_FILE_UP"), m_bTHREAD_MAKE_ITS_FILE_UP);
	m_bTHREAD_MAKE_ITS_FILE_DN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MAKE_ITS_FILE_DN"), m_bTHREAD_MAKE_ITS_FILE_DN);
	m_bTHREAD_UPDATE_REELMAP_INNER_UP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_UP"), m_bTHREAD_UPDATE_REELMAP_INNER_UP);
	m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_ALLUP"), m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP);
	m_bTHREAD_UPDATE_REELMAP_INNER_DN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_DN"), m_bTHREAD_UPDATE_REELMAP_INNER_DN);
	m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_ALLDN"), m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN);

	m_bTHREAD_REELMAP_YIELD_UP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_UP"), m_bTHREAD_REELMAP_YIELD_UP);
	m_bTHREAD_REELMAP_YIELD_ALLUP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ALLUP"), m_bTHREAD_REELMAP_YIELD_ALLUP);
	m_bTHREAD_REELMAP_YIELD_DN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_DN"), m_bTHREAD_REELMAP_YIELD_DN);
	m_bTHREAD_REELMAP_YIELD_ALLDN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ALLDN"), m_bTHREAD_REELMAP_YIELD_ALLDN);
	m_bTHREAD_REELMAP_YIELD_ITS = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ITS"), m_bTHREAD_REELMAP_YIELD_ITS);

	m_nSnTHREAD_UPDATAE_YIELD = 0; pDoc->SetStatusInt(_T("Thread"), _T("nSnTHREAD_UPDATAE_YIELD"), m_nSnTHREAD_UPDATAE_YIELD);


	m_bTHREAD_UPDATE_YIELD_UP = FALSE;
	m_bTHREAD_UPDATE_YIELD_ALLUP = FALSE;
	m_bTHREAD_UPDATE_YIELD_DN = FALSE;
	m_bTHREAD_UPDATE_YIELD_ALLDN = FALSE;
	m_bTHREAD_UPDATE_YIELD_ITS = FALSE;

	m_bTHREAD_UPDATE_YIELD_INNER_UP = FALSE;
	m_bTHREAD_UPDATE_YIELD_INNER_ALLUP = FALSE;
	m_bTHREAD_UPDATE_YIELD_INNER_DN = FALSE;
	m_bTHREAD_UPDATE_YIELD_INNER_ALLDN = FALSE;

	m_bTHREAD_RELOAD_RST_UP = FALSE;
	m_bTHREAD_RELOAD_RST_ALLUP = FALSE;
	m_bTHREAD_RELOAD_RST_DN = FALSE;
	m_bTHREAD_RELOAD_RST_ALLDN = FALSE;
	m_bTHREAD_RELOAD_RST_ITS = FALSE;
	m_bTHREAD_RELOAD_RST_UP_INNER = FALSE;
	m_bTHREAD_RELOAD_RST_ALLUP_INNER = FALSE;
	m_bTHREAD_RELOAD_RST_DN_INNER = FALSE;
	m_bTHREAD_RELOAD_RST_ALLDN_INNER = FALSE;


	m_bTHREAD_UPDATE_REELMAP_UP_OFFLINE = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ALLUP_OFFLINE = FALSE;
	m_bTHREAD_UPDATE_REELMAP_DN_OFFLINE = FALSE;
	m_bTHREAD_UPDATE_REELMAP_ALLDN_OFFLINE = FALSE;

	m_bTHREAD_LOAD_PCR_UP_OFFLINE = FALSE;
	m_bTHREAD_LOAD_PCR_ALLUP_OFFLINE = FALSE;
	m_bTHREAD_LOAD_PCR_DN_OFFLINE = FALSE;
	m_bTHREAD_LOAD_PCR_ALLDN_OFFLINE = FALSE;

	m_bTHREAD_WRITE_LAST_INFO_REELMAP_UP_OFFLINE = FALSE;
	m_bTHREAD_WRITE_LAST_INFO_REELMAP_DN_OFFLINE = FALSE;
	m_bTHREAD_WRITE_LAST_INFO_REELMAP_ALLUP_OFFLINE = FALSE;
	m_bTHREAD_WRITE_LAST_INFO_REELMAP_ALLDN_OFFLINE = FALSE;


	pDoc->m_nEjectBufferLastShot = -1; pDoc->SetStatusInt(_T("General"), _T("nEjectBufferLastShot"), pDoc->m_nEjectBufferLastShot);
	m_bSerialDecrese = FALSE; pDoc->SetStatus(_T("General"), _T("bSerialDecrese"), m_bSerialDecrese);
	m_bStopF_Verify = TRUE; pDoc->SetStatus(_T("General"), _T("bStopF_Verify"), m_bStopF_Verify);
	m_bInitAuto = TRUE; pDoc->SetStatus(_T("General"), _T("bInitAuto"), m_bInitAuto);
	m_bInitAutoLoadMstInfo = TRUE; pDoc->SetStatus(_T("General"), _T("bInitAutoLoadMstInfo"), m_bInitAutoLoadMstInfo);

	//if (pView->m_pMotion)
	//	pView->m_pMotion->ResetPinPos();

	if (bInit) // 이어가기가 아닌경우.
	{
		MpeWrite(Plc.DlgMenu01.JoinJob, 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-20141121
		DispLotEnd(FALSE);

		if (m_pDlgMenu01)
			m_pDlgMenu01->ResetJoinContinue(); // m_bJoinContinue = FALSE;

		m_nRstNum = 0;
		m_bCont = FALSE;
		m_dTotVel = 0.0;
		m_dPartVel = 0.0;
		m_dwCycSt = 0;
		m_dwCycTim = 0;

		pDoc->m_nPrevSerial = 0;
		pDoc->m_bNewLotShare[0] = FALSE;
		pDoc->m_bNewLotShare[1] = FALSE;
		pDoc->m_bNewLotBuf[0] = FALSE;
		pDoc->m_bNewLotBuf[1] = FALSE;
		pDoc->m_bDoneChgLot = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneChgLot"), pDoc->m_bDoneChgLot);

		m_pDlgFrameHigh->m_nMkLastShot = 0;
		m_pDlgFrameHigh->m_nAoiLastShot[0] = 0;
		m_pDlgFrameHigh->m_nAoiLastShot[1] = 0;

		if (m_pDlgMenu01)
			m_pDlgMenu01->ResetLotTime();

		ClrMkInfo(); // 20220420 - Happen Release Trouble

		if (m_pDlgFrameHigh)
		{
			m_pDlgFrameHigh->SetMkLastShot(0);
			m_pDlgFrameHigh->SetAoiLastShot(0, 0);
			m_pDlgFrameHigh->SetAoiLastShot(1, 0);
			m_pDlgFrameHigh->SetEngraveLastShot(0);
		}

		//pDoc->m_ListBuf[0].Clear();
		//pDoc->m_ListBuf[1].Clear();
	}
	else
	{
		if ((pDoc->GetTestMode() == MODE_INNER || pDoc->WorkingInfo.System.bUseEngrave) && pDoc->WorkingInfo.LastJob.bDispLotEnd)
		{
			if (IDYES == MsgBox(_T("각인부에서 레이저 각인부터 시작하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			{
				MpeWrite(Plc.DlgMenu01.JoinJob, 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
			}
			else
			{
				if (IDYES == MsgBox(_T("각인부에서 2D 코드를 읽고 난 후 Last Shot을 확인하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
				{
					MpeWrite(Plc.DlgMenu01.JoinJob, 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
				}
				else
				{
					MpeWrite(Plc.DlgMenu01.JoinJob, 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
				}
			}
		}
		else if ((pDoc->GetTestMode() == MODE_INNER || pDoc->WorkingInfo.System.bUseEngrave) && !pDoc->WorkingInfo.LastJob.bDispLotEnd)
		{
			if (IDYES == MsgBox(_T("각인부에서 2D 코드를 읽고 난 후 Last Shot을 확인하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
			{
				MpeWrite(Plc.DlgMenu01.JoinJob, 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
			}
			else
			{
				if (IDYES == MsgBox(_T("각인부에서 레이저 각인부터 시작하시겠습니까?"), 0, MB_YESNO, DEFAULT_TIME_OUT, FALSE))
				{
					MpeWrite(Plc.DlgMenu01.JoinJob, 0); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-레이저 가공부터 시작
				}
				else
				{
					MpeWrite(Plc.DlgMenu01.JoinJob, 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-2D 코드 읽기부터 시작
				}
			}

		}
		else
		{
			MpeWrite(Plc.DlgMenu01.JoinJob, 1); // 이어가기(PC가 On시키고, PLC가 확인하고 Off시킴)-20141121
		}
		//DispStsBar("이어가기");

		if (pDoc->m_pReelMap)
			pDoc->m_pReelMap->ClrFixPcs();
		if (pDoc->m_pReelMapUp)
			pDoc->m_pReelMapUp->ClrFixPcs();

		if (bDualTest)
		{
			if (pDoc->m_pReelMapDn)
				pDoc->m_pReelMapDn->ClrFixPcs();
			if (pDoc->m_pReelMapAllUp)
				pDoc->m_pReelMapAllUp->ClrFixPcs();
			if (pDoc->m_pReelMapAllDn)
				pDoc->m_pReelMapAllDn->ClrFixPcs();
		}

		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if (pDoc->m_pReelMapInnerUp)
				pDoc->m_pReelMapInnerUp->ClrFixPcs();

			if (pDoc->WorkingInfo.LastJob.bDualTestInner)
			{
				if (pDoc->m_pReelMapInnerDn)
					pDoc->m_pReelMapInnerDn->ClrFixPcs();
				if (pDoc->m_pReelMapInnerAllUp)
					pDoc->m_pReelMapInnerAllUp->ClrFixPcs();
				if (pDoc->m_pReelMapInnerAllDn)
					pDoc->m_pReelMapInnerAllDn->ClrFixPcs();
			}
		}

#ifndef TEST_MODE
		int nLastShot = _tstoi(pDoc->WorkingInfo.LastJob.sMkLastShot);
		//20241221
		//ReloadReelmap(nLastShot);
		//UpdateYield(nLastShot);
		//while (!IsDoneReloadYield() || !IsDoneReloadReelmap())
		//{
		//	Sleep(30);
		//	DispMsg(_T("수율 및 릴맵 데이터를 다시 읽는 중입니다."), _T("On Reload Reelmap."), RGB_GREEN, 60000, TRUE);
		//}
		ClrDispMsg();
		UpdateRst();
#endif
		DispLotStTime();
		RestoreReelmap();
	}

}

void CGvisR2R_PunchView::SetListBuf()	// pDoc->m_ListBuf에 버퍼 폴더의 시리얼번호를 가지고 재갱신함.
{
	pDoc->m_ListBuf[0].Clear();
	if (ChkBufUp(m_pBufSerial[0], m_nBufTot[0]))
	{
		for (int i = 0; i<m_nBufTot[0]; i++)
			pDoc->m_ListBuf[0].Push(m_pBufSerial[0][i]);
	}

	pDoc->m_ListBuf[1].Clear();
	if (ChkBufDn(m_pBufSerial[1], m_nBufTot[1]))
	{
		for (int i = 0; i<m_nBufTot[1]; i++)
			pDoc->m_ListBuf[1].Push(m_pBufSerial[1][i]);
	}
}

void CGvisR2R_PunchView::DispLotStTime()
{
	//char szData[MAX_PATH];
	TCHAR szData[MAX_PATH];
	CString sPath = PATH_WORKING_INFO;
	// [Lot]
	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Tick"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.dwStTick = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.dwStTick = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Year"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nYear = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nYear = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Month"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nMonth = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nMonth = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Day"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nDay = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nDay = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Hour"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nHour = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nHour = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Minute"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nMin = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nMin = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Second"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.Lot.StTime.nSec = _tstoi(szData);
	else
		pDoc->WorkingInfo.Lot.StTime.nSec = 0;

	m_dwLotSt = (DWORD)pDoc->WorkingInfo.Lot.dwStTick;
	DispLotTime();
}

void CGvisR2R_PunchView::ClrMkInfo()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	pDoc->ClrPcr();
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->Clear();
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->Clear();
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->Clear();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->Clear();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->Clear();
	}
	if (m_pDlgMenu01)
	{
		m_pDlgMenu01->ResetMkInfo();
		m_pDlgMenu01->SetPnlNum();
		m_pDlgMenu01->SetPnlDefNum();
		m_pDlgMenu01->RefreshRmap();
		m_pDlgMenu01->UpdateRst();
	}


	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner;
	if (pDoc->GetCurrentInfoEng())
	{
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 3))
			{
				if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModel, sLayerUp))
				{
					if (m_pDlgMenu06)
					{
						m_pDlgMenu06->ResetMkInfo();
						m_pDlgMenu06->RefreshRmap();
						m_pDlgMenu06->UpdateRst();
					}
				}
			}
		}
	}

	UpdateWorking();
}

void CGvisR2R_PunchView::ModelChange(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn 
{
	if (nAoi == 0)
	{
		pDoc->SetModelInfoUp();
		pView->OpenReelmapUp(); // At Start...
		pView->SetPathAtBufUp();
		if (pView->m_pDlgMenu01)
		{
			pView->m_pDlgMenu01->UpdateData();
		}

		pDoc->m_pSpecLocal->MakeDir(pDoc->Status.PcrShare[0].sModel, pDoc->Status.PcrShare[0].sLayer);

		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			OpenReelmapInner();
		}
	}
	else if (nAoi == 1)
	{
		pDoc->SetModelInfoDn();
		pView->OpenReelmapDn(); // At Start...
		pView->SetPathAtBufDn();
		if (pView->m_pDlgMenu01)
		{
			pView->m_pDlgMenu01->UpdateData();
		}
		pDoc->m_pSpecLocal->MakeDir(pDoc->Status.PcrShare[1].sModel, pDoc->Status.PcrShare[1].sLayer);
	}
}

void CGvisR2R_PunchView::ResetMkInfo(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bDualTestInner, bGetCurrentInfoEng;
	CString sLot, sLayerUp, sLayerDn;
	bGetCurrentInfoEng = GetCurrentInfoEng();

	// CamMst Info...
	pDoc->GetCamPxlRes();

	if (nAoi == 0 || nAoi == 2)
	{
		if (!bDualTest)
		{
			m_bDrawGL_Menu01 = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->ResetMkInfo();
		}

		if (bGetCurrentInfoEng)
		{
			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 3))
				{
					if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModel, sLayerUp))
					{
						if (!bDualTestInner)
						{
							m_bDrawGL_Menu06 = FALSE;
							if (m_pDlgMenu06)
								m_pDlgMenu06->ResetMkInfo();
						}
					}
				}
			}
		}

		if (IsLastJob(0)) // Up
		{
			pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->WorkingInfo.LastJob.sLayerUp);
			pDoc->m_Master[0].LoadMstInfo();

			if (m_pEngrave)
				m_pEngrave->SwMenu01UpdateWorking(TRUE);

			if (bGetCurrentInfoEng)
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
					{
						if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModel, sLayerUp))
						{
							pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
								pDoc->WorkingInfo.LastJob.sModel,
								sLayerUp);
							pDoc->m_MasterInner[0].LoadMstInfo();

							if (bDualTestInner)
							{
								pDoc->m_MasterInner[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
									pDoc->WorkingInfo.LastJob.sModel,
									sLayerDn);
								pDoc->m_MasterInner[1].LoadMstInfo();

								m_bDrawGL_Menu06 = FALSE;
								if (m_pDlgMenu06)
									m_pDlgMenu06->ResetMkInfo();
							}
						}
					}
				}
			}
		}
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error - IsLastJob(0)..."));
		}

		InitReelmapUp();

		if (bGetCurrentInfoEng)
		{
			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
				{
					if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModel, sLayerUp))
					{
						InitReelmapInnerUp();
						if (bDualTestInner)
							InitReelmapInnerDn();
					}
				}
			}
		}

		OpenReelmap();
		SetAlignPosUp();

		if (m_pDlgMenu02)
		{
			m_pDlgMenu02->ChgModelUp();

			if (bDualTest)
				m_pDlgMenu02->ChgModelDn();
		}
		
		if (m_pDlgMenu01)
		{
			m_pDlgMenu01->InitCadImgUp();
		}

		if (bGetCurrentInfoEng)
		{
			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
				{
					if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModel, sLayerUp))
					{
						if (m_pDlgMenu06)
						{
							m_pDlgMenu06->InitCadImgUp();
							if (bDualTestInner)
								m_pDlgMenu06->InitCadImgDn();

							m_pDlgMenu06->InitGL();
							m_bDrawGL_Menu06 = TRUE;
							m_pDlgMenu06->RefreshRmap();
						}

					}
				}
			}
		}

		if (m_pDlgMenu01)
		{
			if (!bDualTest)
			{
				m_pDlgMenu01->InitGL();
				m_bDrawGL_Menu01 = TRUE;
				m_pDlgMenu01->RefreshRmap();
			}
		}
	}


	if (bDualTest)
	{
		if (nAoi == 1 || nAoi == 2)
		{
			m_bDrawGL_Menu01 = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->ResetMkInfo();

			if (IsLastJob(1)) // Dn
			{
				pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
					pDoc->WorkingInfo.LastJob.sModel,
					pDoc->WorkingInfo.LastJob.sLayerDn,
					pDoc->WorkingInfo.LastJob.sLayerUp);

				pDoc->m_Master[1].LoadMstInfo();
			}
			else
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Error - IsLastJob(1)..."));
			}

			InitReelmapDn();
			SetAlignPosDn();

			if (m_pDlgMenu02)
				m_pDlgMenu02->ChgModelDn();

			if (m_pDlgMenu01)
			{
				m_pDlgMenu01->InitCadImgDn();
				if (nAoi == 1)
				{
					if (bGetCurrentInfoEng)
					{
						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
							{
								if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModel, sLayerUp))
								{
									if (m_pDlgMenu06)
									{
										m_pDlgMenu06->InitCadImgUp();
										if (bDualTestInner)
											m_pDlgMenu06->InitCadImgDn();

										m_pDlgMenu06->InitGL();
										m_bDrawGL_Menu06 = TRUE;
										m_pDlgMenu06->RefreshRmap();
									}

								}
							}
						}
					}

					m_pDlgMenu01->InitGL();
					m_bDrawGL_Menu01 = TRUE;
					m_pDlgMenu01->RefreshRmap();
				}
			}

		}
	}
}

void CGvisR2R_PunchView::SetAlignPos()
{
	if (m_pMotion)
	{
		m_pMotion->m_dAlignPosX[0][0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[0];
		m_pMotion->m_dAlignPosY[0][0] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0];
		m_pMotion->m_dAlignPosX[0][1] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0];
		m_pMotion->m_dAlignPosY[0][1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0];

		m_pMotion->m_dAlignPosX[1][0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[1];
		m_pMotion->m_dAlignPosY[1][0] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[1];
		m_pMotion->m_dAlignPosX[1][1] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[1];
		m_pMotion->m_dAlignPosY[1][1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[1];
	}
}

void CGvisR2R_PunchView::SetAlignPosUp()
{
	if (m_pMotion)
	{
		m_pMotion->m_dAlignPosX[0][0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[0];
		m_pMotion->m_dAlignPosY[0][0] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0];
		m_pMotion->m_dAlignPosX[0][1] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0];
		m_pMotion->m_dAlignPosY[0][1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0];
	}
}

void CGvisR2R_PunchView::SetAlignPosDn()
{
	if (m_pMotion)
	{
		m_pMotion->m_dAlignPosX[1][0] = pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[1];
		m_pMotion->m_dAlignPosY[1][0] = pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[1];
		m_pMotion->m_dAlignPosX[1][1] = pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[1];
		m_pMotion->m_dAlignPosY[1][1] = pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[1];
	}
}

BOOL CGvisR2R_PunchView::InitMk()
{
	m_nStepElecChk = 0;
	int nRSer = ChkSerial(); // (nSerial0 - nLastShot) -> 0: Same Serial, -: Decrese Serial, +: Increase Serial 
	int nSerial = GetBuffer0();

	if (nRSer)
	{
		// syd - 20130224 : 보완 필요
		if (nRSer < 0)
		{
			m_bSerialDecrese = TRUE; pDoc->SetStatus(_T("General"), _T("bSerialDecrese"), m_bSerialDecrese);
		}
		else
		{
			m_bSerialDecrese = FALSE; pDoc->SetStatus(_T("General"), _T("bSerialDecrese"), m_bSerialDecrese);
		}

		if (pDoc->m_bUseRTRYShiftAdjust)
		{
			if (nSerial > 2)
				AdjPinPos();
		}

		return TRUE;
	}

	return FALSE;
}

int CGvisR2R_PunchView::GetErrCode(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.29"));
		return 0;
	}

#ifndef	TEST_MODE
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nErr[2];
	nErr[0] = GetErrCodeUp(nSerial);
	if (nErr[0] != 1)
		return nErr[0];
	if (bDualTest)
	{
		nErr[1] = GetErrCodeDn(nSerial);
		if (nErr[1] != 1)
			return nErr[1];
	}
#endif

	return 1;
}

int CGvisR2R_PunchView::GetErrCodeUp(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.30"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nErrCode = pDoc->m_pPcr[0][nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}

int CGvisR2R_PunchView::GetErrCodeDn(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.31"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nErrCode = pDoc->m_pPcr[1][nIdx]->m_nErrPnl;
	}
#endif

	return nErrCode;
}


int CGvisR2R_PunchView::GetErrCode0(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.32"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

#ifndef	TEST_MODE
	int nErr[2];
	nErr[0] = GetErrCodeUp0(nSerial);
	if (nErr[0] != 1)
		return nErr[0];
	if (bDualTest)
	{
		nErr[1] = GetErrCodeDn0(nSerial);
		if (nErr[1] != 1)
			return nErr[1];
	}
#endif

	return 1;
}

int CGvisR2R_PunchView::GetErrCodeUp0(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.33"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nErrCode = pDoc->m_pPcr[0][nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}

int CGvisR2R_PunchView::GetErrCodeDn0(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.34"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nErrCode = pDoc->m_pPcr[1][nIdx]->m_nErrPnl;
	}
#endif

	return nErrCode;
}


int CGvisR2R_PunchView::GetErrCode1(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.35"));
		return 0;
	}

#ifndef	TEST_MODE
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nErr[2];
	nErr[0] = GetErrCodeUp1(nSerial);
	if (nErr[0] != 1)
		return nErr[0];

	if (bDualTest)
	{
		nErr[1] = GetErrCodeDn1(nSerial);
		if (nErr[1] != 1)
			return nErr[1];
	}
#endif

	return 1;
}

int CGvisR2R_PunchView::GetErrCodeUp1(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.36"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nErrCode = pDoc->m_pPcr[0][nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}

int CGvisR2R_PunchView::GetErrCodeDn1(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.37"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nErrCode = pDoc->m_pPcr[1][nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}


int CGvisR2R_PunchView::GetTotDefPcs(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.38"));
		return 0;
	}

	int nTotDef = 0;
#ifndef TEST_MODE
	nTotDef = GetTotDefPcsUp(nSerial) + GetTotDefPcsDn(nSerial);
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_PunchView::GetTotDefPcsUp(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.39"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_PunchView::GetTotDefPcsDn(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.40"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nTotDef = pDoc->m_pPcr[1][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}


int CGvisR2R_PunchView::GetTotDefPcs0(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.41"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])
		{
			if (pDoc->m_pPcr[2][nIdx])
				nTotDef = pDoc->m_pPcr[2][nIdx]->m_nTotDef;
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])
		{
			if (pDoc->m_pPcr[0][nIdx])
				nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
		}
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_PunchView::GetTotDefPcsUp0(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.42"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_PunchView::GetTotDefPcsDn0(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.43"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nTotDef = pDoc->m_pPcr[1][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}


int CGvisR2R_PunchView::GetTotDefPcs1(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.44"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])
		{
			if (pDoc->m_pPcr[2][nIdx])
				nTotDef = pDoc->m_pPcr[2][nIdx]->m_nTotDef;
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])
		{
			if (pDoc->m_pPcr[0][nIdx])
				nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
		}
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_PunchView::GetTotDefPcsUp1(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.45"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[0])
	{
		if (pDoc->m_pPcr[0][nIdx])
			nTotDef = pDoc->m_pPcr[0][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_PunchView::GetTotDefPcsDn1(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.46"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcr[1])
	{
		if (pDoc->m_pPcr[1][nIdx])
			nTotDef = pDoc->m_pPcr[1][nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

BOOL CGvisR2R_PunchView::CheckMkPnt()
{
	if (GetMkPnt(0).x == GetMkPnt(1).x && GetMkPnt(0).y == GetMkPnt(1).y)
		return FALSE;

	return TRUE;
}

CfPoint CGvisR2R_PunchView::GetMkPnt(int nIdx)	// CamMaster의 피스순서 인덱스
{
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	if (pDoc->m_Master[0].m_pPcsRgn)
		ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt(nIdx); // CamMaster의 피스순서 인덱스
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

CfPoint CGvisR2R_PunchView::GetMkPnt0(int nIdx)	// CamMaster의 피스순서 인덱스
{
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	if (pDoc->m_Master[0].m_pPcsRgn)
		ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt0(nIdx); // CamMaster의 피스순서 인덱스
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

CfPoint CGvisR2R_PunchView::GetMkPnt1(int nIdx) // CamMaster의 피스순서 인덱스
{
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	if (pDoc->m_Master[0].m_pPcsRgn)
		ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt1(nIdx); // CamMaster의 피스순서 인덱스
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

CfPoint CGvisR2R_PunchView::GetMkPnt0(int nSerial, int nMkPcs) // pcr 시리얼, pcr 불량 피스 읽은 순서 인덱스
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.47"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx0(nSerial);
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	int nDefPcsId = 0;

	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[2][nIdx])
			{
				if (pDoc->m_pPcr[2][nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcr[2][nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt0(nDefPcsId); // Cam0의 Mk 포인트.
					}
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[0][nIdx])
			{
				if (pDoc->m_pPcr[0][nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcr[0][nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcr[0][nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt0(nDefPcsId); // Cam0의 Mk 포인트.
					}
				}
			}
		}
	}
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

int CGvisR2R_PunchView::GetMkStripIdx0(int nDefPcsId) // 0 : Fail , 1~4 : Strip Idx
{
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->m_nRow;
	int nStripY = int(nNodeY / MAX_STRIP);
	int nStripIdx = 0; 

#ifndef TEST_MODE
	int nRow = 0, nNum = 0, nMode = 0;
	nNum = int(nDefPcsId / nNodeY);
	nMode = nDefPcsId % nNodeY;
	if (nNum % 2) 	// 홀수.
		nRow = nNodeY - (nMode + 1);
	else		// 짝수.
		nRow = nMode;

	nStripIdx = int(nRow / nStripY) + 1;
#else
	nStripIdx = 1;
#endif

	return nStripIdx;
}

int CGvisR2R_PunchView::GetMkStripIdx1(int nDefPcsId) // 0 : Fail , 1~4 : Strip Idx
{
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->m_nRow;
	int nStripY = int(nNodeY / MAX_STRIP);
	int nStripIdx = 0;

#ifndef TEST_MODE
	int nRow = 0, nNum = 0, nMode = 0;
	nNum = int(nDefPcsId / nNodeY);
	nMode = nDefPcsId % nNodeY;
	if (nNum % 2) 	// 홀수.
		nRow = nNodeY - (nMode + 1);
	else		// 짝수.
		nRow = nMode;

	nStripIdx = int(nRow / nStripY) + 1;
#else
	nStripIdx = 1;
#endif

	return nStripIdx;
}

CString CGvisR2R_PunchView::GetMkInfo0(int nSerial, int nMkPcs) // return Cam0 : "Serial_Strip_Col_Row"
{
	CString sInfo;
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		MsgBox(_T("좌측 마킹위치 정보 실패."));
		sInfo.Format(_T("%04d_%c_%d_%d"), nSerial, '?', 0, 0);
		return sInfo;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nStrip = -1, nCol = -1, nRow = -1;
	int nPcsIdx = -1, nDefCode = -1;
	int nPcrIdx = pDoc->GetPcrIdx0(nSerial);

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->m_pPcrIts)
		{
			nPcsIdx = pDoc->m_pPcrIts[nPcrIdx]->m_pDefPcs[nMkPcs];
			nDefCode = pDoc->m_pPcrIts[nPcrIdx]->m_pDefType[nMkPcs];
		}

		if (nPcsIdx < 0)
		{
			MsgBox(_T("외층 작업에서 좌측 마킹이미지의 PCS Index를 설정하지 못했습니다."));
			pDoc->LogAuto(_T("외층 작업에서 좌측 마킹이미지의 PCS Index를 설정하지 못했습니다."));
			sInfo.Format(_T("%04d_%c_%d_%d"), nSerial, '?', 0, 0);
			return sInfo;
		}
	}
	else
	{
		if (bDualTest)
		{
			if (pDoc->m_pPcr[2])
			{
				nPcsIdx = pDoc->m_pPcr[2][nPcrIdx]->m_pDefPcs[nMkPcs];
				nDefCode = pDoc->m_pPcr[2][nPcrIdx]->m_pDefType[nMkPcs];
			}
		}
		else
		{
			if (pDoc->m_pPcr[0])
			{
				nPcsIdx = pDoc->m_pPcr[0][nPcrIdx]->m_pDefPcs[nMkPcs];
				nDefCode = pDoc->m_pPcr[0][nPcrIdx]->m_pDefType[nMkPcs];
			}
		}

		if (nPcsIdx < 0)
		{
			MsgBox(_T("양면or내층 작업에서 좌측 마킹이미지의 PCS Index를 설정하지 못했습니다."));
			pDoc->LogAuto(_T("양면or내층 작업에서 좌측 마킹이미지의 PCS Index를 설정하지 못했습니다."));
			sInfo.Format(_T("%04d_%c_%d_%d"), nSerial, '?', 0, 0);
			return sInfo;
		}
	}

	if (pDoc->m_Master[0].m_pPcsRgn)
	{
		pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[0].MasterInfo.nActionCode, nPcsIdx, nStrip, nCol, nRow);
		sInfo.Format(_T("%04d_%c_%d_%d"), nSerial, nStrip + 'A', nCol + 1, nRow + 1);
	}
	else
	{
		sInfo.Format(_T("%04d_%c_%d_%d"), nSerial, '?', 0, 0);
		MsgBox(_T("좌측 마킹위치 정보 실패."));
	}

	return sInfo;
}

CString CGvisR2R_PunchView::GetMkInfo1(int nSerial, int nMkPcs) // return Cam1 : "Serial_Strip_Col_Row"
{
	CString sInfo;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		MsgBox(_T("우측 마킹위치 정보 실패."));
		sInfo.Format(_T("%04d_%c_%d_%d"), nSerial, '?', 0, 0);
		return sInfo;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nPcsIdx = -1, nDefCode = -1;
	int nStrip = -1, nCol = -1, nRow = -1;
	int nPcrIdx = pDoc->GetPcrIdx1(nSerial);

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->m_pPcrIts)
		{
			nPcsIdx = pDoc->m_pPcrIts[nPcrIdx]->m_pDefPcs[nMkPcs];
			nDefCode = pDoc->m_pPcrIts[nPcrIdx]->m_pDefType[nMkPcs];
		}

		if (nPcsIdx < 0)
		{
			MsgBox(_T("외층 작업에서 우측 마킹이미지의 PCS Index를 설정하지 못했습니다."));
			pDoc->LogAuto(_T("외층 작업에서 우측 마킹이미지의 PCS Index를 설정하지 못했습니다."));
			sInfo.Format(_T("%04d_%c_%d_%d"), nSerial, '?', 0, 0);
			return sInfo;
		}
	}
	else
	{
		if (bDualTest)
		{
			if (pDoc->m_pPcr[2])
			{
				nPcsIdx = pDoc->m_pPcr[2][nPcrIdx]->m_pDefPcs[nMkPcs];
				nDefCode = pDoc->m_pPcr[2][nPcrIdx]->m_pDefType[nMkPcs];
			}
		}
		else
		{
			if (pDoc->m_pPcr[0])
			{
				nPcsIdx = pDoc->m_pPcr[0][nPcrIdx]->m_pDefPcs[nMkPcs];
				nDefCode = pDoc->m_pPcr[0][nPcrIdx]->m_pDefType[nMkPcs];
			}
		}

		if (nPcsIdx < 0)
		{
			MsgBox(_T("양면or내층 작업에서 우측 마킹이미지의 PCS Index를 설정하지 못했습니다."));
			pDoc->LogAuto(_T("양면or내층 작업에서 우측 마킹이미지의 PCS Index를 설정하지 못했습니다."));
			sInfo.Format(_T("%04d_%c_%d_%d"), nSerial, '?', 0, 0);
			return sInfo;
		}
	}

	if (pDoc->m_Master[0].m_pPcsRgn)
	{
		pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[0].MasterInfo.nActionCode, nPcsIdx, nStrip, nCol, nRow);
		sInfo.Format(_T("%04d_%c_%d_%d"), nSerial, nStrip + 'A', nCol + 1, nRow + 1);
	}
	else
	{
		sInfo.Format(_T("%04d_%c_%d_%d"), nSerial, '?', 0, 0);
		MsgBox(_T("우측 마킹위치 정보 실패."));
	}

	return sInfo;
}


int CGvisR2R_PunchView::GetMkStripIdx0(int nSerial, int nMkPcs) // 0 : Fail , 1~4 : Strip Idx
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.48"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->m_nRow;
	int nStripY = int(nNodeY / MAX_STRIP_NUM);
	int nStripIdx = 0;

#ifndef TEST_MODE
	int nDefPcsId = 0, nNum = 0, nMode = 0, nRow = 0;

	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[2][nIdx])
			{
				if (pDoc->m_pPcr[2][nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[0][nIdx])
			{
				if (pDoc->m_pPcr[0][nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcr[0][nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
#else
	nStripIdx = 1;
#endif

	return nStripIdx;
}

CfPoint CGvisR2R_PunchView::GetMkPnt1(int nSerial, int nMkPcs)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.50"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	int nDefPcsId = 0;
	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[2][nIdx])
			{
				if (pDoc->m_pPcr[2][nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcr[2][nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
						{
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt1(nDefPcsId); // Cam1의 Mk 포인트.
						}
					}
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[0][nIdx])
			{
				if (pDoc->m_pPcr[0][nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcr[0][nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcr[0][nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
						{
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt1(nDefPcsId); // Cam1의 Mk 포인트.
						}
					}
				}
			}
		}
	}
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

int CGvisR2R_PunchView::GetMkStripIdx1(int nSerial, int nMkPcs) // 0 : Fail , 1~4 : Strip Idx
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.51"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->m_nRow;
	int nStripY = int(nNodeY / MAX_STRIP_NUM);
	int nStripIdx = 0;

#ifndef TEST_MODE
	int nDefPcsId = 0, nNum = 0, nMode = 0, nRow = 0;
	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[2][nIdx])
			{
				if (pDoc->m_pPcr[2][nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[0][nIdx])
			{
				if (pDoc->m_pPcr[0][nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcr[0][nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
#else
	nStripIdx = 1;
#endif

	return nStripIdx;
}

void CGvisR2R_PunchView::Move0(CfPoint pt, BOOL bCam, BOOL bWait)
{
	//if (pView->IsClampOff())
	//{
	//	return;
	//}

	//if (m_bProbDn[0])
	//{
	//	if (m_pVoiceCoil[0])
	//	{
	//		m_pVoiceCoil[0]->SearchHomeSmac(0);
	//		m_pVoiceCoil[0]->MoveSmacShiftPos(0);
	//		m_bProbDn[0] = FALSE;
	//	}
	//}

	double dPosVoice = 0.0, dVoiceMargin = 3.0;
	double fLen, fVel, fAcc, fJerk;
	double pPos[2];

	if (pDoc->WorkingInfo.System.bNoMk || bCam)
	{
		pPos[0] = pt.x;
		pPos[1] = pt.y;
	}
	else
	{
		if (!IsAuto())
		{
			if (m_pVoiceCoil[0])
			{
				dPosVoice = m_pVoiceCoil[0]->GetSmacPosition(VOICE_COIL_FIRST_CAM);
				if ((_tstof(pDoc->WorkingInfo.Marking[0].sMarkingPos) - dVoiceMargin) < dPosVoice)
				{
					if ((_tstof(pDoc->WorkingInfo.Marking[0].sWaitPos) + 1.0) < dPosVoice)
					{
						MsgBox(_T("좌측 보이스코일의 마킹 팁의 위치가 인터록 조건보다 아래로 내려와 있습니다."));
						return;
					}
				}
			}
		}

		pPos[0] = pt.x + _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);
		pPos[1] = pt.y + _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);
	}

	if (pPos[0] < 0.0)
		pPos[0] = 0.0;
	if (pPos[1] < 0.0)
		pPos[1] = 0.0;

	double dCurrX = pView->m_dEnc[AXIS_X0];
	double dCurrY = pView->m_dEnc[AXIS_Y0];
	fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		m_pMotion->GetSpeedProfile0(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
		m_pMotion->Move0(MS_X0Y0, pPos, fVel, fAcc, fAcc, ABS, bWait);
	}
}

void CGvisR2R_PunchView::Move1(CfPoint pt, BOOL bCam, BOOL bWait)
{
	//if (pView->IsClampOff())
	//{
	//	return;
	//}

	//if (m_bProbDn[1])
	//{
	//	if (m_pVoiceCoil[1])
	//	{
	//		m_pVoiceCoil[1]->SearchHomeSmac(1);
	//		m_pVoiceCoil[1]->MoveSmacShiftPos(1);
	//		m_bProbDn[1] = FALSE;
	//	}
	//}

	double dPosVoice = 0.0, dVoiceMargin = 3.0;
	double fLen, fVel, fAcc, fJerk;
	double pPos[2];

	if (pDoc->WorkingInfo.System.bNoMk || bCam)
	{
		pPos[0] = pt.x;
		pPos[1] = pt.y;
	}
	else
	{
		if (!IsAuto())
		{
			if (m_pVoiceCoil[1])
			{
				dPosVoice = m_pVoiceCoil[1]->GetSmacPosition(VOICE_COIL_SECOND_CAM);
				if ((_tstof(pDoc->WorkingInfo.Marking[1].sMarkingPos) - dVoiceMargin) < dPosVoice)
				{
					if ((_tstof(pDoc->WorkingInfo.Marking[1].sWaitPos) + 1.0) < dPosVoice)
					{
						MsgBox(_T("우측 보이스코일의 마킹 팁의 위치가 인터록 조건보다 아래로 내려와 있습니다."));
						return;
					}
				}
			}
		}

		pPos[0] = pt.x + _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX);
		pPos[1] = pt.y + _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetY);
	}

	if (pPos[0] < 0.0)
		pPos[0] = 0.0;
	if (pPos[1] < 0.0)
		pPos[1] = 0.0;

	double dCurrX = pView->m_dEnc[AXIS_X1];
	double dCurrY = pView->m_dEnc[AXIS_Y1];
	fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		m_pMotion->GetSpeedProfile1(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
		m_pMotion->Move1(MS_X1Y1, pPos, fVel, fAcc, fAcc, ABS, bWait);
	}
}

BOOL CGvisR2R_PunchView::IsMoveDone()
{
	if (!m_pMotion)
		return FALSE;

	if (IsMoveDone0() && IsMoveDone1())
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsMoveDone0()
{
	if (!m_pMotion)
		return FALSE;

	if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_Y0))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsMoveDone1()
{
	if (!m_pMotion)
		return FALSE;

	if (m_pMotion->IsMotionDone(MS_X1) && m_pMotion->IsMotionDone(MS_Y1))
	{
		return TRUE;
	}
	return FALSE;
}

void CGvisR2R_PunchView::Mk0()
{
	if (m_pVoiceCoil[0])
	{
		m_pVoiceCoil[0]->SetMark(0);
		pDoc->AddMkCntL();
		m_nCurMk[0]++;
	}
}

void CGvisR2R_PunchView::Mk1()
{
	if (m_pVoiceCoil[1])
	{
		m_pVoiceCoil[1]->SetMark(1);
		pDoc->AddMkCntR();
		m_nCurMk[1]++;
	}
}

BOOL CGvisR2R_PunchView::LoadPcrUp(int nSerial, BOOL bFromShare)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.52"));
		return 0;
	}

	int nHeadInfo = pDoc->LoadPCR0(nSerial); // 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (nHeadInfo >= 2)
	{
		MsgBox(_T("Error-LoadPCR0()"));
		return FALSE;
	}
	return TRUE;
}

BOOL CGvisR2R_PunchView::LoadPcrDn(int nSerial, BOOL bFromShare)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.53"));
		return 0;
	}

	int nHeadInfo = pDoc->LoadPCR1(nSerial); // 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (nHeadInfo >= 2)
	{
		MsgBox(_T("Error-LoadPCR1()"));
		return FALSE;
	}
	return TRUE;
}

void CGvisR2R_PunchView::UpdateRMapUp()
{
	if (pDoc->m_pReelMapUp)
	{
		pDoc->m_pReelMapUp->Write(m_nSerialRmapUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn

		if (pDoc->m_pReelMap == pDoc->m_pReelMapUp)
		{
			DuplicateRmap(RMAP_UP);
		}
	}
}

void CGvisR2R_PunchView::UpdateRMapDn()
{
	if (pDoc->m_pReelMapDn)
	{
		pDoc->m_pReelMapDn->Write(m_nSerialRmapUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CGvisR2R_PunchView::UpdateRMapAllUp()
{
	if (pDoc->m_pReelMapAllUp)
	{
		pDoc->m_pReelMapAllUp->Write(m_nSerialRmapUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		if (pDoc->m_pReelMap == pDoc->m_pReelMapAllUp)
		{
			DuplicateRmap(RMAP_ALLUP);
		}
	}
}

void CGvisR2R_PunchView::UpdateRMapAllDn()
{
	if (pDoc->m_pReelMapAllDn)
	{
		pDoc->m_pReelMapAllDn->Write(m_nSerialRmapUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

UINT CGvisR2R_PunchView::ThreadProc6(LPVOID lpContext)	// UpdateRMapUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[6] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[6].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[6] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_UP) // Write Reelmap
		{
			if (!pThread->m_bTHREAD_REELMAP_YIELD_UP) // Yield Reelmap
			{
				pThread->UpdateRMapUp(); // Write Reelmap
				pThread->m_bTHREAD_UPDATE_REELMAP_UP = FALSE;
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[6] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc7(LPVOID lpContext)	// UpdateRMapDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[7] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[7].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[7] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_DN) // Write Reelmap
		{
			if (!pThread->m_bTHREAD_REELMAP_YIELD_DN) // Yield Reelmap
			{
				pThread->UpdateRMapDn(); // Write Reelmap
				pThread->m_bTHREAD_UPDATE_REELMAP_DN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_DN"), pThread->m_bTHREAD_UPDATE_REELMAP_DN);
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[7] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc8(LPVOID lpContext)	// UpdateRMapAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[8] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[8].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[8] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP) // Write Reelmap
		{
			if (!pThread->m_bTHREAD_REELMAP_YIELD_ALLUP) // Yield Reelmap
			{
				pThread->UpdateRMapAllUp(); // Write Reelmap
				pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_ALLUP"), pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP);
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[8] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc9(LPVOID lpContext)	// UpdateRMapAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[9] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[9].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[9] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			if (!pThread->m_bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
			{
				pThread->UpdateRMapAllDn(); // Write Reelmap
				pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_ALLDN"), pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN);
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[9] = FALSE;

	return 0;
}

void CGvisR2R_PunchView::InitInfo()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateData();

	if (m_pDlgMenu05)
	{
		m_pDlgMenu05->InitModel();
		if (m_pDlgMenu05->IsWindowVisible())
			m_pDlgMenu05->AtDlgShow();
	}
}

void CGvisR2R_PunchView::InitReelmap()
{
	pDoc->InitReelmap();
	pDoc->SetReelmap(ROT_NONE);
	pDoc->UpdateData();
}

void CGvisR2R_PunchView::InitReelmapUp()
{
	pDoc->InitReelmapUp();
	pDoc->SetReelmap(ROT_NONE);
	pDoc->UpdateData();
}

void CGvisR2R_PunchView::InitReelmapDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->InitReelmapDn();
		pDoc->SetReelmap(ROT_NONE);
		pDoc->UpdateData();
	}
}

BOOL CGvisR2R_PunchView::LoadMstInfo()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner, bGetCurrentInfoEng;

	bGetCurrentInfoEng = GetCurrentInfoEng(); // TRUE: MODE_INNER or MODE_OUTER
	pDoc->GetCamPxlRes();

	if (IsLastJob(0)) // Up
	{
		if (bGetCurrentInfoEng)
		{
			pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
				pDoc->m_sEngModel,
				pDoc->m_sEngLayerUp);
			pDoc->m_Master[0].LoadMstInfo();

			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 3))
				{
					if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->m_sEngModel, sLayerUp))
					{
						pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->m_sEngModel,
							sLayerUp);
						pDoc->m_MasterInner[0].LoadMstInfo();
					}
				}
			}
		}
		else
		{
			pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->WorkingInfo.LastJob.sLayerUp);
			pDoc->m_Master[0].LoadMstInfo();
		}
	}

	if (IsLastJob(1)) // Dn
	{
		if (bGetCurrentInfoEng)
		{
			pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
				pDoc->m_sEngModel,
				pDoc->m_sEngLayerDn,
				pDoc->m_sEngLayerUp);

			pDoc->m_Master[1].LoadMstInfo();
			
			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
				{
					if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->m_sEngModel, sLayerDn))
					{
						pDoc->m_MasterInner[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->m_sEngModel,
							sLayerDn,
							sLayerUp);
						pDoc->m_MasterInner[1].LoadMstInfo();
					}
				}
			}
		}
		else
		{
			pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				pDoc->WorkingInfo.LastJob.sLayerUp);

			pDoc->m_Master[1].LoadMstInfo();
		}
	}

	SetAlignPos();

	// Reelmap 정보 Loading.....
	InitReelmap(); // Delete & New

	if (bGetCurrentInfoEng)
	{
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if (pDoc->GetItsSerialInfo(0, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
			{
				if (pDoc->m_MasterInner[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->m_sEngModel, sLayerUp))
				{
					// Reelmap 정보 Loading.....
					InitReelmapInner(); // Delete & New

					if (m_pDlgMenu06)
					{
						m_pDlgMenu06->InitGL();
						m_pDlgMenu06->RefreshRmap();
						m_pDlgMenu06->InitCadImg();
						m_pDlgMenu06->SetPnlNum();
						m_pDlgMenu06->SetPnlDefNum();

						if (bDualTestInner)
							m_pDlgMenu06->SelMap(ALL);
						else
							m_pDlgMenu06->SelMap(UP);
					}
				}
				else
				{
					MsgBox(_T("InitReelmapInner()를 위한 GetItsSerialInfo의 정보가 없습니다.")); // syd-20231127
					return FALSE;
				}
			}
		}
	}

	if (m_pDlgMenu01)
	{
		m_pDlgMenu01->InitGL();
		m_bDrawGL_Menu01 = TRUE;
		m_pDlgMenu01->RefreshRmap();
		m_pDlgMenu01->InitCadImg();
		m_pDlgMenu01->SetPnlNum();
		m_pDlgMenu01->SetPnlDefNum();
	}

	if (m_pDlgMenu02)
	{
		m_pDlgMenu02->ChgModelUp(); // PinImg, AlignImg를 Display함.
		m_pDlgMenu02->InitCadImg();
	}
#ifndef TEST_MODE
	if (m_pDlgMenu01)
		m_pDlgMenu01->RedrawWindow();

	DispMsg(_T("릴맵을 초기화합니다."), _T("알림"), RGB_GREEN, DELAY_TIME_MSG);
	OpenReelmap();
#endif
	SetPathAtBuf(); // Reelmap path를 설정함.

	int nSrl = pDoc->GetLastShotMk();
	SetMkFdLen();
	if (nSrl >= 0)
	{
		if (bDualTest)
			m_pDlgMenu01->SelMap(ALL);
		else
			m_pDlgMenu01->SelMap(UP);
	}

	ApplyListTorq();

	return TRUE;
}

BOOL CGvisR2R_PunchView::IsPinMkData()
{
	if (pDoc->IsPinMkData())
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsPinData()
{
	return pDoc->IsPinData();
}

BOOL CGvisR2R_PunchView::CopyDefImg(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.55"));
		return 0;
	}

	return pDoc->CopyDefImg(nSerial);
}

BOOL CGvisR2R_PunchView::CopyDefImg(int nSerial, CString sNewLot)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.56"));
		return 0;
	}

	return pDoc->CopyDefImg(nSerial, sNewLot);
}

BOOL CGvisR2R_PunchView::CopyDefImgUp(int nSerial, CString sNewLot)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.57"));
		return 0;
	}

	return pDoc->CopyDefImgUp(nSerial, sNewLot);
}

BOOL CGvisR2R_PunchView::CopyDefImgDn(int nSerial, CString sNewLot)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return FALSE;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.58"));
		return FALSE;
	}

	return pDoc->CopyDefImgDn(nSerial, sNewLot);
}

BOOL CGvisR2R_PunchView::MovePinPos()
{
	if (!m_pDlgMenu02)
		return FALSE;

	return m_pDlgMenu02->MovePinPos();
}


BOOL CGvisR2R_PunchView::TwoPointAlign0(int nPos)
{
	if (!m_pDlgMenu02)
		return FALSE;

	return m_pDlgMenu02->Do2PtAlign0(nPos);
}

BOOL CGvisR2R_PunchView::TwoPointAlign1(int nPos)
{
	if (!m_pDlgMenu02)
		return FALSE;

	return m_pDlgMenu02->Do2PtAlign1(nPos);
}

BOOL CGvisR2R_PunchView::FourPointAlign0(int nPos)
{
	if (!m_pDlgMenu02)
		return FALSE;

	return m_pDlgMenu02->Do4PtAlign0(nPos);
}

BOOL CGvisR2R_PunchView::FourPointAlign1(int nPos)
{
	if (!m_pDlgMenu02)
		return FALSE;

	return m_pDlgMenu02->Do4PtAlign1(nPos);
}

BOOL CGvisR2R_PunchView::OnePointAlign(CfPoint &ptPnt)
{
	if (!m_pDlgMenu02)
		return FALSE;

	return m_pDlgMenu02->OnePointAlign(ptPnt);
}

BOOL CGvisR2R_PunchView::TwoPointAlign() // return FALSE; 인경우 정지.
{
	if (!m_pDlgMenu02)
		return FALSE;

	BOOL bRtn[2];

	// TwoPointAlign(0)
	bRtn[0] = m_pDlgMenu02->TwoPointAlign(0);

	if (!bRtn[0])
		return FALSE;

	// TwoPointAlign(1)
	bRtn[1] = m_pDlgMenu02->TwoPointAlign(1); // Align 보정.

	if (bRtn[0] && bRtn[1])
		return TRUE;

	return FALSE;
}

BOOL CGvisR2R_PunchView::IsNoMk()
{
	BOOL bNoMk = (pDoc->WorkingInfo.System.bNoMk | m_bCam); // pDoc->WorkingInfo.LastJob.bVerify
	return bNoMk;
}

BOOL CGvisR2R_PunchView::IsNoMk0()
{
	BOOL bNoMk = (pDoc->WorkingInfo.System.bNoMk | m_bCam); // pDoc->WorkingInfo.LastJob.bVerify
	return bNoMk;
}

BOOL CGvisR2R_PunchView::IsNoMk1()
{
	BOOL bNoMk = (pDoc->WorkingInfo.System.bNoMk | m_bCam); // pDoc->WorkingInfo.LastJob.bVerify
	return bNoMk;
}

BOOL CGvisR2R_PunchView::IsHomeDone(int nMsId)
{
	if (!m_pMotion)
		return FALSE;

	return m_pMotion->IsHomeDone(nMsId);
}

BOOL CGvisR2R_PunchView::GetAoiUpInfo(int nSerial, int *pNewLot, BOOL bFromBuf)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.59"));
		return 0;
	}

	return pDoc->GetAoiUpInfo(nSerial, pNewLot, bFromBuf);
}

BOOL CGvisR2R_PunchView::GetAoiDnInfo(int nSerial, int *pNewLot, BOOL bFromBuf)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.60"));
		return 0;
	}

	return pDoc->GetAoiDnInfo(nSerial, pNewLot, bFromBuf);
}

BOOL CGvisR2R_PunchView::LoadMySpec()
{
	return pDoc->LoadMySpec();
}

CString CGvisR2R_PunchView::GetProcessNum()
{
	return pDoc->GetProcessNum();
}

BOOL CGvisR2R_PunchView::GetEngOffset(CfPoint &OfSt)
{
	return pDoc->GetEngOffset(OfSt);
}

BOOL CGvisR2R_PunchView::GetAoiUpOffset(CfPoint &OfSt)
{
	return pDoc->GetAoiUpOffset(OfSt);
}

BOOL CGvisR2R_PunchView::GetAoiDnOffset(CfPoint &OfSt)
{
	return pDoc->GetAoiDnOffset(OfSt);
}

BOOL CGvisR2R_PunchView::GetMkOffset(CfPoint &OfSt)
{
	if (m_pDlgMenu02)
	{
		OfSt.x = m_pDlgMenu02->m_dMkFdOffsetX[0][0]; // -: 제품 덜옴, +: 제품 더옴.
		OfSt.y = m_pDlgMenu02->m_dMkFdOffsetY[0][0]; // -: 제품 덜옴, +: 제품 더옴.
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::IsAoiLdRun()
{
	return TRUE;

	BOOL bRtn = FALSE;
	if (m_pDlgMenu03)
		bRtn = m_pDlgMenu03->IsAoiLdRun();

	return bRtn;
}

BOOL CGvisR2R_PunchView::IsInitPos0()
{
	if (!m_pMotion)
		return FALSE;

	double pTgtPos[2];
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	double dCurrX = m_dEnc[AXIS_X0];
	double dCurrY = m_dEnc[AXIS_Y0];

	if (dCurrX < pTgtPos[0] - 2.0 || dCurrX > pTgtPos[0] + 2.0)
		return FALSE;
	if (dCurrY < pTgtPos[1] - 2.0 || dCurrY > pTgtPos[1] + 2.0)
		return FALSE;

	return TRUE;
}

void CGvisR2R_PunchView::MoveInitPos0(BOOL bWait)
{
	if (!m_pMotion)
		return;

	double pTgtPos[2];
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	double dCurrX = m_dEnc[AXIS_X0];
	double dCurrY = m_dEnc[AXIS_Y0];

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt(((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		m_pMotion->GetSpeedProfile0(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
		if(bWait)
			m_pMotion->Move0(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc, ABS, WAIT);
		else
			m_pMotion->Move0(MS_X0Y0, pTgtPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
	}
}

BOOL CGvisR2R_PunchView::IsInitPos1()
{
	if (!m_pMotion)
		return FALSE;

	double pTgtPos[2];
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
	double dCurrX = m_dEnc[AXIS_X1];
	double dCurrY = m_dEnc[AXIS_Y1];

	if (dCurrX < pTgtPos[0] - 2.0 || dCurrX > pTgtPos[0] + 2.0)
		return FALSE;
	if (dCurrY < pTgtPos[1] - 2.0 || dCurrY > pTgtPos[1] + 2.0)
		return FALSE;

	return TRUE;
}

BOOL CGvisR2R_PunchView::IsMkEdPos1()
{
	if (!m_pMotion)
		return FALSE;

	double pTgtPos[2];
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sMkEdPosX[1]);
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sMkEdPosY[1]);
	double dCurrX = m_dEnc[AXIS_X1];
	double dCurrY = m_dEnc[AXIS_Y1];

	if (dCurrX < pTgtPos[0] - 2.0 || dCurrX > pTgtPos[0] + 2.0)
		return FALSE;
	if (dCurrY < pTgtPos[1] - 2.0 || dCurrY > pTgtPos[1] + 2.0)
		return FALSE;

	return TRUE;
}

void CGvisR2R_PunchView::MoveInitPos1(BOOL bWait)
{
	if (!m_pMotion)
		return;

	double pTgtPos[2];
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
	double dCurrX = m_dEnc[AXIS_X1];
	double dCurrY = m_dEnc[AXIS_Y1];

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt(((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		m_pMotion->GetSpeedProfile1(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
		if(bWait)
			m_pMotion->Move1(MS_X1Y1, pTgtPos, fVel, fAcc, fAcc, ABS, WAIT);
		else
			m_pMotion->Move1(MS_X1Y1, pTgtPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
	}
}

void CGvisR2R_PunchView::MoveMkEdPos1()
{
	if (!m_pMotion)
		return;

	double pTgtPos[2];
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sMkEdPosX[1]);
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sMkEdPosY[1]);
	double dCurrX = m_dEnc[AXIS_X1];
	double dCurrY = m_dEnc[AXIS_Y1];

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt(((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		m_pMotion->GetSpeedProfile1(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
		m_pMotion->Move1(MS_X1Y1, pTgtPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
	}
}


void CGvisR2R_PunchView::LotEnd()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->LotEnd();

	m_bCont = FALSE;
	SetLotEd();

	MakeResultMDS();

	if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
	{
		FinalCopyItsFiles();
	}

	if (pDoc->m_pReelMap == pDoc->m_pReelMapUp)
	{
		DuplicateRmap(RMAP_UP);
	}
	else if (pDoc->m_pReelMap == pDoc->m_pReelMapAllUp)
	{
		DuplicateRmap(RMAP_ALLUP);
	}
	else if (pDoc->m_pReelMap == pDoc->m_pReelMapIts)
	{
		DuplicateRmap(RMAP_ITS);
	}

	if (m_pEngrave)
		m_pEngrave->SwMenu01DispDefImg(TRUE);
}

void CGvisR2R_PunchView::TimWinker(int nId, int nDly) // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
	m_bBtnWinker[nId] = TRUE;
	if (!m_bTimBtnWinker)
	{
		m_bTimBtnWinker = TRUE;
		m_nDlyWinker[nId] = nDly;
		SetTimer(TIM_BTN_WINKER, 100, NULL);
	}
}

void CGvisR2R_PunchView::StopTimWinker(int nId) // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
	m_bBtnWinker[nId] = FALSE;
	m_bTimBtnWinker = FALSE;
	for (int i = 0; i<4; i++)
	{
		if (m_bBtnWinker[i])
			m_bTimBtnWinker = TRUE;
	}
}

void CGvisR2R_PunchView::Winker(int nId, int nDly) // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
	if (nId == MN_RUN)
	{
		if (pView->m_pMpe)
		{
			m_bBtnWinker[nId] = TRUE;
			MpeWrite(Plc.DlgMenu01.AutoInitStatus, 1); // 자동 초기 운전상태(PC가 On/Off 시킴, PLC가 운전램프를 윙크동작, on->off시 운전램프 on, 다시 운전스위치가 눌러지면 off) - 20141017
		}
	}
}

void CGvisR2R_PunchView::ResetWinker() // 0:Ready, 1:Reset, 2:Run, 3:Stop
{
	MpeWrite(Plc.DlgMenu01.AutoInitStatus, 0);
	for (int i = 0; i<4; i++)
	{
		m_bBtnWinker[i] = FALSE;
		m_nCntBtnWinker[i] = FALSE;
	}
}

void CGvisR2R_PunchView::SetOrigin()
{
}

BOOL CGvisR2R_PunchView::IsSetLotEnd()
{
	if (m_nLotEndSerial > 0)
		return TRUE;
	return FALSE;
}

void CGvisR2R_PunchView::SetLotEnd(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.61"));
		return;
	}

	m_nLotEndSerial = nSerial; pDoc->SetStatusInt(_T("General"), _T("nLotEndSerial"), m_nLotEndSerial);
	m_nAoiLastSerial[0] = nSerial;

	CString str;
	str.Format(_T("%d"), m_nLotEndSerial);
	DispStsBar(str, 0);
	if (m_pDlgMenu01)
		m_pDlgMenu01->DispLotEndSerial(m_nLotEndSerial);
}

int CGvisR2R_PunchView::GetLotEndSerial()
{
	return m_nLotEndSerial; // 테이블상에 정지하는 Serial.
}

BOOL CGvisR2R_PunchView::StartLive()
{
	if (StartLive0() && StartLive1())
		return TRUE;

	return  FALSE;
}

BOOL CGvisR2R_PunchView::StartLive0()
{
	BOOL bRtn0 = FALSE;

#ifdef USE_VISION
	if (m_pVision[0])
		bRtn0 = m_pVision[0]->StartLive();
#endif
	if (bRtn0)
		return TRUE;

	return  FALSE;
}

BOOL CGvisR2R_PunchView::StartLive1()
{
	BOOL bRtn1 = FALSE;

#ifdef USE_VISION
	if (m_pVision[1])
		bRtn1 = m_pVision[1]->StartLive();
#endif

	if (bRtn1)
		return TRUE;

	return  FALSE;
}

BOOL CGvisR2R_PunchView::StopLive()
{
#ifdef TEST_MODE
	return TRUE;
#endif

	if (StopLive0() && StopLive1())
		return TRUE;

	return FALSE;
}

BOOL CGvisR2R_PunchView::StopLive0()
{
#ifdef TEST_MODE
	return TRUE;
#endif

	BOOL bRtn0 = FALSE;

#ifdef USE_VISION
	if (m_pVision[0])
		bRtn0 = m_pVision[0]->StopLive();
#endif
	if (bRtn0)
		return TRUE;

	return FALSE;
}

BOOL CGvisR2R_PunchView::StopLive1()
{
#ifdef TEST_MODE
	return TRUE;
#endif

	BOOL bRtn1 = FALSE;

#ifdef USE_VISION
	if (m_pVision[1])
		bRtn1 = m_pVision[1]->StopLive();
#endif

	if (bRtn1)
		return TRUE;

	return FALSE;
}

void CGvisR2R_PunchView::UpdateRst()
{
	if (m_pDlgMenu01)
		m_pDlgMenu01->UpdateRst();
}

BOOL CGvisR2R_PunchView::IsChkTmpStop()
{
	if (pDoc->WorkingInfo.LastJob.bTempPause)
	{
		double dFdLen = GetMkFdLen();
		if (dFdLen >= _tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen)*1000.0)
		{
			pDoc->WorkingInfo.LastJob.bTempPause = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->UpdateData();
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsVerify()
{
	BOOL bVerify = FALSE;
	int nSerial0 = m_nBufUpSerial[0]; // Cam0
	int nSerial1 = m_nBufUpSerial[1]; // Cam1
	int nPeriod = pDoc->WorkingInfo.LastJob.nVerifyPeriod;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;


	if (pDoc->WorkingInfo.LastJob.bVerify)
	{
		double dFdLen = GetMkFdLen();
		double dVerifyLen = _tstof(pDoc->WorkingInfo.LastJob.sVerifyLen)*1000.0;

		if (dFdLen < dVerifyLen)
		{
			if (nSerial0 == 1 || nPeriod == 0 || nPeriod == 1 || nPeriod == 2 || m_bStopF_Verify)
			{
				m_bStopF_Verify = FALSE; pDoc->SetStatus(_T("General"), _T("bStopF_Verify"), m_bStopF_Verify);
				bVerify = TRUE;
			}
			else
			{
				if(!(nSerial0 % nPeriod) || !(nSerial1 % nPeriod))
					bVerify = TRUE;
			}
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bVerify = FALSE;
			if (m_pDlgMenu01)
				m_pDlgMenu01->UpdateData();
		}
	}

	return bVerify;
}

int CGvisR2R_PunchView::GetVsBufLastSerial()
{
	int nLastShot = pDoc->GetLastShotMk();
	if (nLastShot > 0 && m_bCont)
		return (nLastShot + 4);

	return 4;
}

int CGvisR2R_PunchView::GetVsUpBufLastSerial()
{
	int nLastShot = pDoc->GetLastShotUp();
	if (nLastShot > 0 && m_bCont)
		return (nLastShot + 4);

	return 4;
}

int CGvisR2R_PunchView::GetVsDnBufLastSerial()
{
	int nLastShot = pDoc->GetLastShotDn();
	if (nLastShot > 0 && m_bCont)
		return (nLastShot + 4);

	return 4;
}

BOOL CGvisR2R_PunchView::IsFixPcsUp(int nSerial)
{
	if (!pDoc->m_pReelMapUp)
		return FALSE;

	CString sMsg = _T(""), str = _T("");
	int nStrip, pCol[2500], pRow[2500], nTot, nRepeat, nTotPcs;
	BOOL bCont = FALSE;

	if (pDoc->m_pReelMapUp->IsFixPcs(nSerial, pCol, pRow, nTot, bCont))
	{
		int nNodeX = pDoc->m_Master[0].m_pPcsRgn->m_nCol;
		int nNodeY = pDoc->m_Master[0].m_pPcsRgn->m_nRow;
		int nStPcsY = nNodeY / MAX_STRIP_NUM;

		if(bCont)
			sMsg.Format(_T("상면 연속 고정불량 발생"));
		else
			sMsg.Format(_T("상면 누적 고정불량 발생"));

		for (int i = 0; i<nTot; i++)
		{
			nStrip = int(pRow[i] / nStPcsY);
			nRepeat = pDoc->m_pReelMapUp->GetRptFixPcs(pCol[i], pRow[i]);
			nTotPcs = pDoc->m_pReelMapUp->GetTotalFixPcs(pCol[i], pRow[i]);
			if (!(i % 5))
			{
				if(nRepeat > 1)
					str.Format(_T("\r\n[%d:%c-%d,%d] (%d회, 누적%d개)"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1, nRepeat, nTotPcs);
				else
					str.Format(_T("\r\n[%d:%c-%d,%d] (누적%d개)"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1, nTotPcs);
			}
			else
			{
				if (nRepeat > 1)
					str.Format(_T(" , [%d:%c-%d,%d] (%d회, 누적%d개)"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1, nRepeat, nTotPcs);
				else
					str.Format(_T(" , [%d:%c-%d,%d] (누적%d개)"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1, nTotPcs);
			}

			sMsg += str;
		}
		str.Format(_T("\r\n누적 피스수 : %d"), nTotPcs);
		sMsg += str;
		m_sFixMsg[0] = sMsg;
		return TRUE;
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::IsFixPcsDn(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return FALSE;

	if (!pDoc->m_pReelMapUp)
		return FALSE;

	CString sMsg = _T(""), str = _T("");
	int nStrip, pCol[2500], pRow[2500], nTot, nRepeat, nTotPcs;
	BOOL bCont = FALSE;

	if (pDoc->m_pReelMapDn->IsFixPcs(nSerial, pCol, pRow, nTot, bCont))
	{
		int nNodeX = pDoc->m_Master[0].m_pPcsRgn->m_nCol;
		int nNodeY = pDoc->m_Master[0].m_pPcsRgn->m_nRow;
		int nStPcsY = nNodeY / MAX_STRIP_NUM;

		if (bCont)
			sMsg.Format(_T("하면 연속 고정불량 발생"));
		else
			sMsg.Format(_T("하면 누적 고정불량 발생"));

		for (int i = 0; i<nTot; i++)
		{
			nStrip = int(pRow[i] / nStPcsY);
			nRepeat = pDoc->m_pReelMapDn->GetRptFixPcs(pCol[i], pRow[i]);
			nTotPcs = pDoc->m_pReelMapDn->GetTotalFixPcs(pCol[i], pRow[i]);

			if (!(i % 5))
			{
				if (nRepeat > 1)
					str.Format(_T("\r\n[%d:%c-%d,%d] (%d회, 누적%d개)"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1, nRepeat, nTotPcs);
				else
					str.Format(_T("\r\n[%d:%c-%d,%d] (누적%d개)"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1), nTotPcs;
			}
			else
			{
				if (nRepeat > 1)
					str.Format(_T(" , [%d:%c-%d,%d] (%d회, 누적%d개)"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1, nRepeat, nTotPcs);
				else
					str.Format(_T(" , [%d:%c-%d,%d] (누적%d개)"), nSerial, 'A' + nStrip, pCol[i] + 1, (pRow[i] % nStPcsY) + 1, nTotPcs);
			}

			sMsg += str;
		}
		m_sFixMsg[1] = sMsg;
		return TRUE;
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::IsReview()
{
	return (pDoc->WorkingInfo.LastJob.bReview);
}

BOOL CGvisR2R_PunchView::IsReview0()
{
	return (pDoc->WorkingInfo.LastJob.bReview);
}

BOOL CGvisR2R_PunchView::IsReview1()
{
	return (pDoc->WorkingInfo.LastJob.bReview);
}


BOOL CGvisR2R_PunchView::IsJogRtDn()
{
	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIb[4] & (0x01 << 10) ? TRUE : FALSE;	// 마킹부 JOG 버튼(우)
#endif
	return bOn;
}

BOOL CGvisR2R_PunchView::IsJogRtUp()
{
	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIb[4] & (0x01 << 10) ? FALSE : TRUE;	// 마킹부 JOG 버튼(우)
#endif
	return bOn;
}

BOOL CGvisR2R_PunchView::IsJogRtDn0()
{
	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIb[4] & (0x01 << 10) ? TRUE : FALSE;	// 마킹부 JOG 버튼(우)
#endif
	return bOn;
}

BOOL CGvisR2R_PunchView::IsJogRtUp0()
{
	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIb[4] & (0x01 << 10) ? FALSE : TRUE;	// 마킹부 JOG 버튼(우)
#endif
	return bOn;
}

BOOL CGvisR2R_PunchView::IsJogRtDn1()
{
	BOOL bOn = FALSE;
#ifdef USE_MPE
	bOn = pDoc->m_pMpeIb[4] & (0x01 << 10) ? TRUE : FALSE;	// 마킹부 JOG 버튼(우)
#endif
	return bOn;
}

BOOL CGvisR2R_PunchView::IsJogRtUp1()
{
	BOOL bOn = FALSE;
	bOn = pDoc->m_pMpeIo[4] & (0x01 << 10) ? FALSE : TRUE;	// 마킹부 JOG 버튼(우)
	return bOn;
}

BOOL CGvisR2R_PunchView::IsVs()
{
	if (!m_bChkLastProcVs)
	{
		BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
		if (bDualTest)
		{
			if (pDoc->m_bVsStatusDn)
				return TRUE;
			else if (pDoc->m_bVsStatusUp)
				return TRUE;
		}
		else
		{
			if (pDoc->m_bVsStatusUp)
				return TRUE;
		}
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::IsVsUp()
{
	return pDoc->m_bVsStatusUp;
}

BOOL CGvisR2R_PunchView::IsVsDn()
{
	return pDoc->m_bVsStatusDn;
}

int CGvisR2R_PunchView::GetAoiUpSerial()
{
	int nSerial = 0;
#ifndef TEST_MODE
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiUpCurrInfo;
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Serial"), NULL, szData, sizeof(szData), sPath))
		nSerial = _tstoi(szData);
		else
			nSerial = 0;
		if (nSerial > 1)
			nSerial--;
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 Infomation에 Current Serial 정보가 없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	return nSerial;
}

BOOL CGvisR2R_PunchView::GetAoiUpVsStatus()
{
	BOOL bVsStatus = FALSE;
#ifndef TEST_MODE
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiUpCurrInfo;
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current VS Status"), NULL, szData, sizeof(szData), sPath))
			bVsStatus = _tstoi(szData) > 0 ? TRUE : FALSE;
	}
	else
	{
		sMsg.Format(_T("%s파일의 Infomation에 Current VS Status 정보가 없습니다.\r\n상면 AOI가 꺼진 것 같습니다."), sPath);
		pView->ClrDispMsg();
		MsgBox(sMsg);
		//AfxMessageBox(sMsg);
	}
#endif
	pDoc->m_bVsStatusUp = bVsStatus;
	return bVsStatus;
}

int CGvisR2R_PunchView::GetAoiDnSerial()
{
	int nSerial = 0;
#ifndef TEST_MODE
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiDnCurrInfo;
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Serial"), NULL, szData, sizeof(szData), sPath))
			nSerial = _tstoi(szData);
		else
			nSerial = 0;
		if (nSerial > 1)
			nSerial--;
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 Infomation에 Current Serial 정보가 없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	return nSerial;
}

BOOL CGvisR2R_PunchView::GetAoiDnVsStatus()
{
	BOOL bVsStatus = FALSE;
#ifndef TEST_MODE
	TCHAR szData[200];
	CString sPath = pDoc->WorkingInfo.System.sPathAoiDnCurrInfo;
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current VS Status"), NULL, szData, sizeof(szData), sPath))
			bVsStatus = _tstoi(szData) > 0 ? TRUE : FALSE;
	}
	else
	{
		sMsg.Format(_T("%s파일의 Infomation에 Current VS Status 정보가 없습니다.\r\n하면 AOI가 꺼진 것 같습니다."), sPath);
		pView->ClrDispMsg();
		MsgBox(sMsg);
		//AfxMessageBox(sMsg);
	}
#endif
	pDoc->m_bVsStatusDn = bVsStatus;
	return bVsStatus;
}

BOOL CGvisR2R_PunchView::IsDoneDispMkInfo()
{
	BOOL bRtn = FALSE;
	if (m_pDlgMenu01)
		bRtn = m_pDlgMenu01->IsDoneDispMkInfo();
	return bRtn;
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void CGvisR2R_PunchView::Marking()
{
}

void CGvisR2R_PunchView::DoReject0()
{
#ifdef TEST_MODE
	return;
#endif
	CString sItem;
	int nIdx, nRtn;
	CfPoint ptPnt;

	if (!IsRun() && m_bAuto)
	{
		if (IsOnMarking0())
		{
			if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0))
			{
				if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_Y0))
				{
					if (!IsInitPos0() && !IsPinPos0())
						MoveInitPos0();
				}
			}
		}
		return;
	}


	switch (m_nStepMk[2])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nMkStrip[0][0] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[0][0]"), m_nMkStrip[0][0]);
		m_nMkStrip[0][1] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[0][1]"), m_nMkStrip[0][1]);
		m_nMkStrip[0][2] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[0][2]"), m_nMkStrip[0][2]);
		m_nMkStrip[0][3] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[0][3]"), m_nMkStrip[0][3]);
		m_bRejectDone[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bRejectDone[0][0]"), m_bRejectDone[0][0]);
		m_bRejectDone[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bRejectDone[0][1]"), m_bRejectDone[0][1]);
		m_bRejectDone[0][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bRejectDone[0][2]"), m_bRejectDone[0][2]);
		m_bRejectDone[0][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bRejectDone[0][3]"), m_bRejectDone[0][3]);
		m_nStepMk[2]++;
		break;
	case 1:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)
		{
			m_nStepMk[2]++;
		}
		else
		{
			m_nStepMk[2] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
		}
		break;
	case 2:
		m_nStepMk[2]++;
		break;
	case 3:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)	// 마킹완료Check
		{
			ptPnt = GetMkPnt0(m_nMkPcs[2]);

			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[2]++;
				break;
			}

			nIdx = GetMkStripIdx0(m_nMkPcs[2]);
			if (nIdx > 0)
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[2]++;
					break;
				}
				else
				{
					if (m_nMkStrip[0][nIdx - 1] > pDoc->GetStripRejectMkNum())
					{
						m_bRejectDone[0][nIdx - 1] = TRUE;
						sItem.Format(_T("bRejectDone[0][%d]"), nIdx - 1);
						pDoc->SetStatus(_T("General"), sItem, m_bRejectDone[0][nIdx - 1]);
						m_nMkPcs[2]++;
						break;
					}
					else
						m_nMkStrip[0][nIdx - 1]++;
				}
			}
			else
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			if (m_bRejectDone[0][0] && m_bRejectDone[0][1] &&
				m_bRejectDone[0][2] && m_bRejectDone[0][3])
			{
				m_nStepMk[2] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
				break;
			}

			m_dTarget[AXIS_X0] = ptPnt.x;

			// Cam0 : m_bPriority[0], m_bPriority[3]
			// Cam1 : m_bPriority[1], m_bPriority[2]

			m_bCollision[0] = ChkCollision(AXIS_X0, ptPnt.x);
			if (!m_bCollision[0])
			{
				if (IsMoveDone0())
				{
					Move0(ptPnt, m_bCam);
					m_nStepMk[2]++;
				}
			}
			else if (m_bPriority[0]) // Cam0 <-
			{
				if (IsMoveDone0())
				{
					m_bCollision[0] = FALSE;
					m_bPriority[0] = FALSE;
					Move0(ptPnt, m_bCam);
					m_nStepMk[2]++;
				}
			}
			else if (m_bPriority[1]) // Cam1 ->
			{
				if (IsMoveDone0())
				{
					m_bCollision[0] = FALSE;
					m_bPriority[2] = FALSE;
					ptPnt.x = 0.0;
					ptPnt.y = m_dEnc[AXIS_Y0];
					Move0(ptPnt, FALSE);
				}
			}
			else if (m_bPriority[2] && m_bPriority[3])	// Cam1 <- && Cam0 ->
			{
				break;									// Cam1 이 안전위치로 대피할 때까지 기다린다.
			}
			//else if (m_bPriority[2]) // Cam1 <-
			//{
			//	m_bPriority[2] = FALSE;
			//	ptPnt.x = 0.0;
			//	ptPnt.y = m_dEnc[AXIS_Y0];
			//	Move0(ptPnt, m_bCam);
			//}
		}
		else
		{
			m_nStepMk[2] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
		}
		break;
	case 4:
		m_nStepMk[2]++;
		break;
	case 5:
		m_nStepMk[2]++;
		break;
	case 6:
		if (IsMoveDone0())
			m_nStepMk[2]++;
		break;
	case 7:
		if (!IsNoMk0())
		{
			m_dwStMkDn[0] = GetTickCount();
			Mk0();
		}
		else
			SetDelay0(pDoc->m_nDelayShow, 1);		// [mSec]
		m_nStepMk[2]++;
		break;
	case 8:
		if (IsNoMk0())
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
				m_nStepMk[2]++;
		}
		else
			m_nStepMk[2]++;
		break;
	case 9:
		m_nStepMk[2]++;
		break;
	case 10:
		if (!IsNoMk0())
		{
			//Mk0(FALSE);
			if (IsMk0Done())
			{
				m_nMkPcs[2]++;
				m_nStepMk[2]++;
				m_nStepMk[2]++;
			}
			else
			{
				if (m_dwStMkDn[0] + 5000 < GetTickCount())
				{
					Buzzer(TRUE, 0);
					//pView->DispStsBar(_T("정지-29"), 0);
					DispMain(_T("정 지"), RGB_RED);
					if (m_pVoiceCoil[0])
						m_pVoiceCoil[0]->SearchHomeSmac0();

					//nRtn = AsyncMsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					nRtn = MsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[2] = 7; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
					}
					else if (nRtn < 0)
						m_nStepMk[2]++;
					else
					{
						m_nMkPcs[2]++;
						m_nStepMk[2]++;
						m_nStepMk[2]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[2]++;
			m_nStepMk[2]++;
			m_nStepMk[2]++;
		}
		break;
	case 11:
		//m_nStepMk[2]++;
		//if(m_bRtnMyMsgBox[2])
		if ((nRtn = WaitRtnVal(3)) > -1)
		{
			//if(IDYES == m_nRtnMyMsgBox[2])
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[2] = 7; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
			}
			else
			{
				m_nMkPcs[2]++;
				m_nStepMk[2]++;
				Stop();
			}
		}
		break;
	case 12:
		m_nStepMk[2]++;
		break;
	case 13:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)	// 마킹완료Check
		{
			m_nStepMk[2] = 3; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
		}
		else
		{
			m_nStepMk[2] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
		}
		break;
	case MK_END:
		if (m_pVoiceCoil[0])
			m_pVoiceCoil[0]->SearchHomeSmac0();
		SetDelay0(500, 1);		// [mSec]
		m_nStepMk[2]++;
		break;
	case 101:
		if (!WaitDelay0(1))		// F:Done, T:On Waiting....
		{
			MoveInitPos0();
			m_nStepMk[2]++;
		}
		break;
	case 102:
		if (IsMoveDone0())
		{
			m_bTHREAD_MK[2] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[2]"), m_bTHREAD_MK[2]);
			m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
			m_nStepMk[2] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
			m_nMkStrip[0][0] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[0][0]"), m_nMkStrip[0][0]);
			m_nMkStrip[0][1] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[0][1]"), m_nMkStrip[0][1]);
			m_nMkStrip[0][2] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[0][2]"), m_nMkStrip[0][2]);
			m_nMkStrip[0][3] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[0][3]"), m_nMkStrip[0][3]);
		}
		break;
	}
}

void CGvisR2R_PunchView::DoReject1()
{
#ifdef TEST_MODE
	return;
#endif
	CString sItem;
	int nIdx, nRtn;
	CfPoint ptPnt;

	if (!IsRun() && m_bAuto)
	{
		if (IsOnMarking1())
		{
			if (m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
			{
				if (m_pMotion->IsMotionDone(MS_X1) && m_pMotion->IsMotionDone(MS_Y1))
				{
					if (!IsMkEdPos1() && !IsPinPos1())
						MoveMkEdPos1();
				}
			}
		}
		return;
	}


	switch (m_nStepMk[3])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nMkStrip[1][0] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[1][0]"), m_nMkStrip[1][0]);
		m_nMkStrip[1][1] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[1][1]"), m_nMkStrip[1][1]);
		m_nMkStrip[1][2] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[1][2]"), m_nMkStrip[1][2]);
		m_nMkStrip[1][3] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[1][3]"), m_nMkStrip[1][3]);
		m_bRejectDone[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bRejectDone[1][0]"), m_bRejectDone[1][0]);
		m_bRejectDone[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bRejectDone[1][1]"), m_bRejectDone[1][1]);
		m_bRejectDone[1][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bRejectDone[1][2]"), m_bRejectDone[1][2]);
		m_bRejectDone[1][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bRejectDone[1][3]"), m_bRejectDone[1][3]);
		m_nStepMk[3]++;
		break;
	case 1:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)
		{
			m_nStepMk[3]++;
		}
		else
		{
			m_nStepMk[3] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
		}
		break;
	case 2:
		m_nStepMk[3]++;
		break;
	case 3:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)	// 마킹완료Check
		{
			ptPnt = GetMkPnt1(m_nMkPcs[3]);

			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[3]++;
				break;
			}

			nIdx = GetMkStripIdx1(m_nMkPcs[3]);
			if (nIdx > 0)
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[3]++;
					break;
				}
				else
				{
					if (m_nMkStrip[1][nIdx - 1] > pDoc->GetStripRejectMkNum())
					{
						m_bRejectDone[1][nIdx - 1] = TRUE;
						sItem.Format(_T("bRejectDone[1][%d]"), nIdx - 1);
						pDoc->SetStatus(_T("General"), sItem, m_bRejectDone[1][nIdx - 1]);
						m_nMkPcs[3]++;
						break;
					}
					else
						m_nMkStrip[1][nIdx - 1]++;
				}
			}
			else
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			if (m_bRejectDone[1][0] && m_bRejectDone[1][1] &&
				m_bRejectDone[1][2] && m_bRejectDone[1][3])
			{
				m_nStepMk[2] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
				break;
			}

			m_dTarget[AXIS_X1] = ptPnt.x;

			// Cam0 : m_bPriority[0], m_bPriority[3]
			// Cam1 : m_bPriority[1], m_bPriority[2]

			m_bCollision[1] = ChkCollision(AXIS_X1, ptPnt.x);
			if (!m_bCollision[1])
			{
				if (IsMoveDone1())
				{
					Move1(ptPnt, m_bCam);
					m_nStepMk[3]++;
				}
			}
			else if (m_bPriority[1]) // Cam1 ->
			{
				if (IsMoveDone1())
				{
					m_bCollision[1] = FALSE;
					m_bPriority[1] = FALSE;
					Move1(ptPnt, m_bCam);
					m_nStepMk[3]++;
				}
			}
			else if (m_bPriority[0]) // Cam0 <-		// 우선 순위 결정에서 Cam0에 먼저 양보함.
			{
				if (IsMoveDone1())
				{
					m_bCollision[1] = FALSE;
					m_bPriority[3] = FALSE;
					ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
					ptPnt.y = m_dEnc[AXIS_Y1];
					Move1(ptPnt, FALSE);
				}
			}
			else if (m_bPriority[2]) // Cam1 <-		// 우선 순위 결정에서 Cam0에 먼저 양보함.
			{
				if (IsMoveDone1())
				{
					m_bCollision[1] = FALSE;
					m_bPriority[3] = FALSE;
					ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
					ptPnt.y = m_dEnc[AXIS_Y1];
					Move1(ptPnt, FALSE);
				}
			}
			else if (m_bPriority[3]) // Cam0 ->		// 우선 순위 결정에서 Cam0에 먼저 양보함.
			{
				if (IsMoveDone1())
				{
					m_bCollision[1] = FALSE;
					m_bPriority[3] = FALSE;
					ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
					ptPnt.y = m_dEnc[AXIS_Y1];
					Move1(ptPnt, m_bCam);
				}
			}
		}
		else
		{
			m_nStepMk[3] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
		}
		break;
	case 4:
		m_nStepMk[3]++;
		break;
	case 5:
		m_nStepMk[3]++;
		break;
	case 6:
		if (IsMoveDone1())
			m_nStepMk[3]++;
		break;
	case 7:
		if (!IsNoMk1())
		{
			m_dwStMkDn[1] = GetTickCount();
			Mk1();
		}
		else
			SetDelay1(pDoc->m_nDelayShow, 1);		// [mSec]
		m_nStepMk[3]++;
		break;
	case 8:
		if (IsNoMk1())
		{
			if (!WaitDelay1(1))		// F:Done, T:On Waiting....
				m_nStepMk[3]++;
		}
		else
			m_nStepMk[3]++;
		break;
	case 9:
		m_nStepMk[3]++;
		break;
	case 10:
		if (!IsNoMk1())
		{
			if (IsMk1Done())
			{
				m_nMkPcs[3]++;
				m_nStepMk[3]++;
				m_nStepMk[3]++;
			}
			else
			{
				if (m_dwStMkDn[1] + 5000 < GetTickCount())
				{
					Buzzer(TRUE, 0);
					DispMain(_T("정 지"), RGB_RED);
					if (m_pVoiceCoil[1])
						m_pVoiceCoil[1]->SearchHomeSmac1();

					nRtn = MsgBox(_T("보이스코일(우) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 2, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[3] = 7; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
					}
					else if (nRtn < 0)
						m_nStepMk[3]++;
					else
					{
						m_nMkPcs[3]++;
						m_nStepMk[3]++;
						m_nStepMk[3]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[3]++;
			m_nStepMk[3]++;
			m_nStepMk[3]++;
		}
		break;
	case 11:
		if ((nRtn = WaitRtnVal(4)) > -1)
		{
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[3] = 7; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
			}
			else
			{
				m_nMkPcs[3]++;
				m_nStepMk[3]++;
				Stop();
			}
		}
		break;
	case 12:
		m_nStepMk[3]++;
		break;
	case 13:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)	// 마킹완료Check
		{
			m_nStepMk[3] = 3; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
		}
		else
		{
			m_nStepMk[3] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
		}
		break;
	case MK_END:
		if (m_pVoiceCoil[1])
			m_pVoiceCoil[1]->SearchHomeSmac1();
		SetDelay1(500, 1);		// [mSec]
		m_nStepMk[3]++;
		break;
	case 101:
		if (!WaitDelay1(1))		// F:Done, T:On Waiting....
		{
			MoveMkEdPos1();
			m_nStepMk[3]++;
		}
		break;
	case 102:
		if (IsMoveDone1())
		{
			m_bTHREAD_MK[3] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[3]"), m_bTHREAD_MK[3]);
			m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
			m_nStepMk[3] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
			m_nMkStrip[1][0] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[1][0]"), m_nMkStrip[1][0]);
			m_nMkStrip[1][1] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[1][1]"), m_nMkStrip[1][1]);
			m_nMkStrip[1][2] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[1][2]"), m_nMkStrip[1][2]);
			m_nMkStrip[1][3] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStrip[1][3]"), m_nMkStrip[1][3]);
		}
		break;
	}
}

void CGvisR2R_PunchView::DoMark0All()
{
	CfPoint ptPnt;

	if (!IsRun())
	{
		if (IsOnMarking0())
		{
			if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0))
			{
				if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_Y0))
				{
					if (!IsInitPos0() && !IsPinPos0())
						MoveInitPos0();
				}
			}

			if (m_nStepMk[2] < 5 && m_nStepMk[2] > 3) // Mk0();
			{
				m_nStepMk[2] = 3; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);	// 마킹완료Check
			}
		}
		return;
	}


	switch (m_nStepMk[2])
	{
	case 0:
		m_nStepMk[2]++;
		break;
	case 1:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)
		{
			m_nStepMk[2]++;
		}
		else
		{
			m_nStepMk[2] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
		}
		break;
	case 2:
		m_nStepMk[2]++;
		break;
	case 3:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)	// 마킹완료Check
		{
			ptPnt = GetMkPnt0(m_nMkPcs[2]);

			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[2]++;
				break;
			}

			m_dTarget[AXIS_X0] = ptPnt.x;

			// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
			// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-

			m_bCollision[0] = ChkCollision(AXIS_X0, ptPnt.x);
			if (!m_bCollision[0])
			{
				if (IsMoveDone0())
				{
					Move0(ptPnt, m_bCam);
					m_nStepMk[2]++;
				}
			}
			else if (m_bPriority[0]) // Cam0 <-
			{
				if (IsMoveDone0())
				{
					m_bPriority[0] = FALSE;
					Move0(ptPnt, m_bCam);
					m_nStepMk[2]++;
				}
			}
			else if (m_bPriority[1]) // Cam1 ->
			{
				if (IsMoveDone0())
				{
					m_bCollision[0] = FALSE;
					m_bPriority[2] = FALSE;
					ptPnt.x = 0.0;
					ptPnt.y = m_dEnc[AXIS_Y0];
					Move0(ptPnt, FALSE);
				}
			}
			else if (m_bPriority[2] && m_bPriority[3]) // Cam1 <- && Cam0 ->
			{
				break; // Cam1 이 안전위치로 대피할 때까지 기다린다.
			}
			//else if (m_bPriority[2])
			//{
			//	if (IsMoveDone0())
			//	{
			//		m_bPriority[2] = FALSE;
			//		ptPnt.x = 0.0;
			//		ptPnt.y = m_dEnc[AXIS_Y0];
			//		Move0(ptPnt, FALSE);
			//	}
			//}
		}
		else
		{
			m_nStepMk[2] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
		}
		break;
	case 4:
		if (IsMoveDone0())
			m_nStepMk[2]++;
		break;
	case 5:
		if (!IsNoMk0())
			Mk0();
		else
			SetDelay0(pDoc->m_nDelayShow, 1);		// [mSec]
		m_nStepMk[2]++;
		break;
	case 6:
		if (IsNoMk0())
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
				m_nStepMk[2]++;
		}
		else
			m_nStepMk[2]++;
		break;
	case 7:
		m_nMkPcs[2]++;
		m_nStepMk[2]++;
		break;
	case 8:
		if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)	// 마킹완료Check
		{
			m_nStepMk[2] = 3; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
		}
		else
		{
			m_nStepMk[2] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
		}
		break;
	case MK_END:
		SetDelay0(100, 1);		// [mSec]
		m_nStepMk[2]++;
		break;
	case 101:
		if (!WaitDelay0(1))		// F:Done, T:On Waiting....
		{
			if (IsMoveDone0())
			{
				MoveInitPos0();
				m_nStepMk[2]++;
			}
		}
		break;
	case 102:
		if (IsMoveDone0())
			m_nStepMk[2]++;
		break;
	case 103:
		m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
		m_bTHREAD_MK[2] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[2]"), m_bTHREAD_MK[2]);
		break;
	}
}

void CGvisR2R_PunchView::DoMark1All()
{
	CfPoint ptPnt;

	if (!IsRun())
	{
		if (IsOnMarking1())
		{
			if (m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
			{
				if (m_pMotion->IsMotionDone(MS_X1) && m_pMotion->IsMotionDone(MS_Y1))
				{
					if (!IsMkEdPos1() && !IsPinPos1())
						MoveMkEdPos1();
				}
			}

			if (m_nStepMk[3] < 5 && m_nStepMk[3] > 3) // Mk1();
			{
				m_nStepMk[3] = 3; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);	// 마킹완료Check
			}
		}
		return;
	}


	switch (m_nStepMk[3])
	{
	case 0:
		m_nStepMk[3]++;
		break;
	case 1:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)
		{
			m_nStepMk[3]++;
		}
		else
		{
			m_nStepMk[3] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
		}
		break;
	case 2:
		m_nStepMk[3]++;
		break;
	case 3:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)	// 마킹완료Check
		{
			ptPnt = GetMkPnt1(m_nMkPcs[3]);

			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[3]++;
				break;
			}

			m_dTarget[AXIS_X1] = ptPnt.x;

			// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
			// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-

			m_bCollision[1] = ChkCollision(AXIS_X1, ptPnt.x);
			if (!m_bCollision[1])
			{
				if (IsMoveDone1())
				{
					Move1(ptPnt, m_bCam);
					m_nStepMk[3]++;
				}
			}
			else if (m_bPriority[1]) // Cam1 ->
			{
				if (IsMoveDone1())
				{
					m_bCollision[1] = FALSE;
					m_bPriority[1] = FALSE;
					Move1(ptPnt, m_bCam);
					m_nStepMk[3]++;
				}
			}
			else if (m_bPriority[0]) // Cam0 <-	// 우선 순위 결정에서 Cam0에 먼저 양보함.
			{
				if (IsMoveDone1())
				{
					m_bCollision[1] = FALSE;
					m_bPriority[3] = FALSE;
					ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
					ptPnt.y = m_dEnc[AXIS_Y1];
					Move1(ptPnt, FALSE);
				}
			}
			else if (m_bPriority[2]) // Cam1 <-		// 우선 순위 결정에서 Cam0에 먼저 양보함.
			{
				if (IsMoveDone1())
				{
					m_bCollision[1] = FALSE;
					m_bPriority[3] = FALSE;
					ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
					ptPnt.y = m_dEnc[AXIS_Y1];
					Move1(ptPnt, FALSE);
				}
			}
			else if (m_bPriority[3]) // Cam0 ->	// 우선 순위 결정에서 Cam0에 먼저 양보함.
			{
				if (IsMoveDone1())
				{
					m_bCollision[1] = FALSE;
					m_bPriority[3] = FALSE;
					ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
					ptPnt.y = m_dEnc[AXIS_Y1];
					Move1(ptPnt, FALSE);
				}
			}
		}
		else
		{
			m_nStepMk[3] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
		}
		break;
	case 4:
		if (IsMoveDone1())
			m_nStepMk[3]++;
		break;
	case 5:
		if (!IsNoMk1())
			Mk1();
		else
			SetDelay1(pDoc->m_nDelayShow, 1);		// [mSec]
		m_nStepMk[3]++;
		break;
	case 6:
		if (IsNoMk1())
		{
			if (!WaitDelay1(1))		// F:Done, T:On Waiting....
				m_nStepMk[3]++;
		}
		else
			m_nStepMk[3]++;
		break;
	case 7:
		m_nMkPcs[3]++;
		m_nStepMk[3]++;
		break;
	case 8:
		if (m_nMkPcs[3] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)	// 마킹완료Check
		{
			m_nStepMk[3] = 3; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
		}
		else
		{
			//StopLive1();
			m_nStepMk[3] = MK_END;
		}
		break;
	case MK_END:
		SetDelay1(100, 1);		// [mSec]
		m_nStepMk[3]++;
		break;
	case 101:
		if (!WaitDelay1(1))		// F:Done, T:On Waiting....
		{
			if (m_nMkPcs[2] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)
			{
				if (m_nMkPcs[2] > 0)
				{
					if (!(m_Flag & (0x01 << 0)))
					{
						if (IsMoveDone1())
						{
							m_Flag |= (0x01 << 0); pDoc->SetStatusInt(_T("General"), _T("Flag"), m_Flag);
							ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
							ptPnt.y = 0.0;//m_dEnc[AXIS_Y1];
							Move1(ptPnt, FALSE);
						}
					}
				}
				else
				{
					if (IsMoveDone1())
					{
						m_Flag &= ~(0x01 << 0); pDoc->SetStatusInt(_T("General"), _T("Flag"), m_Flag);
						MoveMkEdPos1();
						m_nStepMk[3]++;
					}
				}
			}
			else
			{
				if (IsMoveDone1())
				{
					m_Flag &= ~(0x01 << 0); pDoc->SetStatusInt(_T("General"), _T("Flag"), m_Flag);
					MoveMkEdPos1();
					m_nStepMk[3]++;
				}
			}
		}
		break;
	case 102:
		if (IsMoveDone0())
			m_nStepMk[3]++;
		break;
	case 103:
		m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
		m_bTHREAD_MK[3] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[3]"), m_bTHREAD_MK[3]);
		break;
	}
}

void CGvisR2R_PunchView::InsertPunchingData(int nCam)
{
	if (nCam == CAM_LF)
	{
		if (pDoc->WorkingInfo.System.bInsertPunchingToDts)
		{
			if (pView->m_pDts)
			{
				if (pView->m_pDts->IsUseDts())
				{
					CString sLot = pDoc->WorkingInfo.LastJob.sLot;
					int nSerial = pView->m_nBufUpSerial[0]; // Left Serial
					CString sProc = pDoc->WorkingInfo.LastJob.sProcessNum;
					double dPosX = pView->m_dTarget[AXIS_X0];
					double dPosY = pView->m_dTarget[AXIS_Y0];
					int nPunchCode = 1; // 1 : Mk()

					BOOL bRtn = pView->m_pDts->InsertPunchingData(sLot, nSerial, sProc, dPosX, dPosY, nPunchCode);
					if (!bRtn)
					{
						//pView->ClrDispMsg();
						//AfxMessageBox(_T("Error - InsertPunchingData()."), MB_ICONSTOP | MB_OK);
						//MsgBox(_T("Error - InsertPunchingData()."));
					}
				}
			}
		}
	}
	else if (nCam == CAM_RT)
	{
		if (pDoc->WorkingInfo.System.bInsertPunchingToDts)
		{
			if (pView->m_pDts)
			{
				if (pView->m_pDts->IsUseDts())
				{
					CString sLot = pDoc->WorkingInfo.LastJob.sLot;
					int nSerial = pView->m_nBufUpSerial[1]; // Right Serial
					CString sProc = pDoc->WorkingInfo.LastJob.sProcessNum;
					double dPosX = pView->m_dTarget[AXIS_X1];
					double dPosY = pView->m_dTarget[AXIS_Y1];
					int nPunchCode = 1; // 1 : Mk()

					BOOL bRtn = pView->m_pDts->InsertPunchingData(sLot, nSerial, sProc, dPosX, dPosY, nPunchCode);
					if (!bRtn)
					{
						//pView->ClrDispMsg();
						//AfxMessageBox(_T("Error - InsertPunchingData()."), MB_ICONSTOP | MB_OK);
						//MsgBox(_T("Error - InsertPunchingData()."));
					}
				}
			}
		}
	}
}

void CGvisR2R_PunchView::DoMark0()
{
#ifdef TEST_MODE
	return;
#endif

	if (!m_bAuto) return;

	//BOOL bOn;
	int nSerial, nIdx, nErrCode, nRtn;
	CfPoint ptPnt;
	CString sMsg;
	double dStripOut = (pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs / MAX_STRIP_NUM) * _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
	int nStripOut = int(dStripOut);
	if (dStripOut > nStripOut)
		nStripOut++;			// 스트립 양폐 비율

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!IsRun())																		// 정지상태에서
	{
		if (IsOnMarking0())																// 마킹중에
		{
			if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0))				// 모션이 Enable상태이고
			{
				if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_Y0))	// 모션 Done상태이면,
				{
					if (!IsInitPos0() && !IsPinPos0())									// 초기위치가 아니거나, 핀위치가 아닐때
						MoveInitPos0();													// 초기위치로 이동
				}
			}

			if (m_nStepMk[0] < 13 && m_nStepMk[0] > 8) // Mk0();
			{
				m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
			}
		}
		return;
	}

	m_sDispSts[0].Format(_T("%d"), m_nStepMk[0]);

	switch (m_nStepMk[0])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nStepMk[0]++;
		break;
	case 1:
		if (!IsInitPos0())
			MoveInitPos0();
		m_nStepMk[0]++;
		break;
	case 2:

		nSerial = m_nBufUpSerial[0]; // Cam0

		//nSerial = 1;

		if (nSerial > 0)
		{
			if ((nErrCode = GetErrCode0(nSerial)) != 1)
			{
				m_nMkPcs[0] = GetTotDefPcs0(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
				m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
			else
			{
				m_nStepMk[0]++;
			}
		}
		else
		{
			StopFromThread();
			MsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 1);
			BuzzerFromThread(TRUE, 0);
			DispMain(_T("정 지"), RGB_RED);
		}
		break;
	case 3:
		m_nStepMk[0]++;
		break;
	case 4:
		m_nStepMk[0]++;
		break;
	case 5:
		m_nStepMk[0]++;
		break;
	case 6:

		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0(nSerial))
		{
			if (!IsNoMk0())
			{
				;
			}
			else
			{
				if (!IsReview0())
				{
					if (m_bReview)
					{
						m_nMkPcs[0] = GetTotDefPcs0(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
						m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
						break;
					}
				}
			}
			// Punching On이거나 Review이면 다음으로 진행
			SetDelay0(100, 1);		// [mSec]
			m_nStepMk[0]++;
		}
		else
		{
			m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
		}
		break;
	case 7:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (!WaitDelay0(1))		// F:Done, T:On Waiting....		// Delay후에
		{
			m_nMkPcs[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);

			if (!IsNoMk0())										// Punching On이면
			{
				m_nStepMk[0]++;
			}
			else												// Punching이 Off이고
			{
				if (IsReview0())								// Review이면 다음으로
				{
					m_nStepMk[0]++;
				}
				else											// Review가 아니면
				{
					if (m_bReview)
					{
						m_nMkPcs[0] = GetTotDefPcs0(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
						m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
					}
					else
						m_nStepMk[0]++;
				}
			}
		}
		break;
	case 8:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0(nSerial))	// 마킹완료Check
		{
			if (m_nMkPcs[0] + 1 < GetTotDefPcs0(nSerial))		// 다음 마킹위치가 있으면
			{
				ptPnt = GetMkPnt0(nSerial, m_nMkPcs[0] + 1);	// 다음 마킹위치
				m_dNextTarget[AXIS_X0] = ptPnt.x;
				m_dNextTarget[AXIS_Y0] = ptPnt.y;
			}
			else												// 다음 마킹위치가 없으면
			{
				m_dNextTarget[AXIS_X0] = -1.0;
				m_dNextTarget[AXIS_Y0] = -1.0;
			}

			m_nMarkingOrder[0] = m_nMkPcs[0];					// 이번 마킹위치
			ptPnt = GetMkPnt0(nSerial, m_nMkPcs[0]);			// 이번 마킹위치
			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화. (마킹하지 않음)
			{
				m_nMkPcs[0]++;
				m_nStepMk[0] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
				break;
			}

			nIdx = GetMkStripIdx0(nSerial, m_nMkPcs[0]);		// 1 ~ 4 : strip index
			if (nIdx > 0)										// Strip index가 정상이면,
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[0]++;
					m_nStepMk[0] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
					break;
				}
				else
				{
					if (m_nMkStrip[0][nIdx - 1] >= nStripOut)
					{
						m_nMkPcs[0]++;
						m_nStepMk[0] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
						break;
					}
					else
						m_nMkStrip[0][nIdx - 1]++;
				}
			}
			else
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			m_dTarget[AXIS_X0] = ptPnt.x;
			m_dTarget[AXIS_Y0] = ptPnt.y;

			m_nStepMk[0]++;
		}
		else
		{
			m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
		}
		break;
	case 9:
		// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
		// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-
		ptPnt.x = m_dTarget[AXIS_X0];
		ptPnt.y = m_dTarget[AXIS_Y0];

		if (m_dNextTarget[AXIS_X0] < 0)
			m_bCollision[0] = ChkCollision(AXIS_X0, m_dTarget[AXIS_X0]);
		else
			m_bCollision[0] = ChkCollision(AXIS_X0, m_dTarget[AXIS_X0], m_dNextTarget[AXIS_X0]);

		//if (pDoc->m_Master[0].MasterInfo.nActionCode == 1 || pDoc->m_Master[0].MasterInfo.nActionCode == 3) // 1 : 좌우 미러 , 3 : 180 회전
		if (!m_bCollision[0])
		{
			if (IsMoveDone0())
			{
				Move0(ptPnt, m_bCam);
				m_nStepMk[0]++;
			}
		}
		else if (m_bPriority[0]) // Cam0 <-
		{
			if (IsMoveDone0())
			{
				m_bCollision[0] = FALSE;
				m_bPriority[0] = FALSE;
				Move0(ptPnt, m_bCam);
				m_nStepMk[0]++;
			}
		}
		else if (m_bPriority[1]) // Cam1 ->
		{
			if (IsMoveDone0())
			{
				m_bCollision[0] = FALSE;
				m_bPriority[2] = FALSE;
				ptPnt.x = 0.0;
				ptPnt.y = m_dEnc[AXIS_Y0];
				Move0(ptPnt, FALSE);
			}
		}
		else if (m_bPriority[2] && m_bPriority[3]) // Cam1 <- && Cam0 ->
		{
			break; // Cam1 이 안전위치로 대피할 때까지 기다린다.
		}
		//else if (m_bPriority[2]) // Cam1 <-
		//{
		//	if (IsMoveDone0())
		//	{
		//		m_bCollision[0] = FALSE;
		//		m_bPriority[2] = FALSE;
		//		ptPnt.x = 0.0;
		//		ptPnt.y = m_dEnc[AXIS_Y0];
		//		Move0(ptPnt, FALSE);
		//	}
		//}
		break;
	case 10:
		if (IsMoveDone0())
			m_nStepMk[0]++;
		break;
	case 11:
		m_nStepMk[0]++;
		break;
	case 12:
		m_nStepMk[0]++;
		break;
	case 13:
		if (!IsNoMk0())
		{
			m_dwStMkDn[0] = GetTickCount();
			Mk0();
			nSerial = m_nBufUpSerial[0]; // Cam0
			SetMkPcs0(nSerial, m_nMarkingOrder[0]); // Draw Mked Point On Reelmap Screen.
		}
		else
		{
			// Verify - Mk0
			InsertPunchingData(CAM_LF);
			SetDelay0(pDoc->m_nDelayShow, 1);		// [mSec]

			if (m_pDlgMenu02)
				m_pDlgMenu02->InitMkPmRst(0);

			if(!SaveMk0Img(m_nMkPcs[0]))
			{
				pView->ClrDispMsg();
				//AfxMessageBox(_T("Error-SaveMk0Img()"));
				if (pDoc->WorkingInfo.LastJob.bUseJudgeMk)
				{
#ifdef USE_VISION
					if (!m_pVision[0]->m_bMkJudge)
					{
						nRtn = MsgBox(_T("좌측 펀칭이 미마킹으로 보입니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
						if (IDYES == nRtn)
						{
							ptPnt.x = m_dTarget[AXIS_X0];
							ptPnt.y = m_dTarget[AXIS_Y0];

							Move0(ptPnt, FALSE, WAIT);
							Sleep(30);
							Mk0();
							Sleep(100);
							Move0(ptPnt, TRUE, WAIT);
							m_nMkPcs[0]--; // Repeat Current Position Verify.
							m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
							Stop();
							break;
						}
					}
#endif
				}
			}
		}
		m_nStepMk[0]++;
		break;
	case 14:
		m_nStepMk[0]++;
		break;
	case 15:
		m_nStepMk[0]++;
		break;
	case 16:
		if (IsNoMk0())
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
				m_nStepMk[0]++;
		}
		else
			m_nStepMk[0]++;
		break;
	case 17:
		if (!IsNoMk0())
		{
			if (IsMk0Done())
			{
				// One more MK On Start....
				if (!m_nMkPcs[0] /*&& pDoc->WorkingInfo.Probing[0].bUse*/ && !m_bAnswer[2])
				{
					m_bAnswer[2] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[2]"), m_bAnswer[2]);
					Mk0();
				}
				else
				{
					m_bAnswer[2] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[2]"), m_bAnswer[2]);
					m_nMkPcs[0]++;
					m_nStepMk[0]++;
					m_nStepMk[0]++;
				}
			}
			else
			{
				if (m_dwStMkDn[0] + 5000 < GetTickCount())
				{
					BuzzerFromThread(TRUE, 0);
					DispMain(_T("정 지"), RGB_RED);
					if (m_pVoiceCoil[0])
						m_pVoiceCoil[0]->SearchHomeSmac0();

					nRtn = MsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[0] = 13; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
					}
					else if (nRtn < 0)
						m_nStepMk[0]++; // Wait...
					else
					{
						m_bAnswer[2] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[2]"), m_bAnswer[2]);
						m_nMkPcs[0]++;
						m_nStepMk[0]++;
						m_nStepMk[0]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[0]++;
			m_nStepMk[0]++;
			m_nStepMk[0]++;
		}
		break;
	case 18:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[0] = 13; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
			else
			{
				m_bAnswer[2] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[2]"), m_bAnswer[2]);
				m_nMkPcs[0]++;
				m_nStepMk[0]++;
				StopFromThread();
			}
		}
		break;
	case 19:
		m_nStepMk[0] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
		break;
	case MK_DONE_CHECK:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0(nSerial))
		{
			if (IsNoMk0())
			{
				if (!pDoc->WorkingInfo.System.bNoMk)
				{
					if (IsReview0())
					{
						if (IsJogRtDn0())
							m_nStepMk[0]++;
					}
					else if (!pDoc->WorkingInfo.LastJob.bVerify)
					{
						m_nMkPcs[0] = GetTotDefPcs0(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
						m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
					}
					else
					{
						m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
					}
				}
				else
				{
					m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
				}
				
			}
			else
			{
				m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
			}
		}
		else
		{
			if (IsNoMk0())
			{
				if (IsReview0())
				{
					if (IsJogRtDn0())
					{
						m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
					}
				}
				else
				{
					m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
				}
			}
			else
			{
				m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
		}
		break;
	case MK_DONE_CHECK + 1:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0(nSerial))
		{
			if (IsNoMk0())
			{
				if (IsReview0())
				{
					if (IsJogRtUp0())
					{
						m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
					}
				}
				else
				{
					m_nMkPcs[0] = GetTotDefPcs0(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
					m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
				}
			}
			else
			{
				m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
			}
		}
		else
		{
			m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
		}
		break;

	case MK_END:
		if (IsMoveDone0())
			m_nStepMk[0]++;
		break;
	case 101:
		SetDelay0(100, 1);		// [mSec]
		m_nStepMk[0]++;
		break;
	case 102:
		if (!WaitDelay0(1))		// F:Done, T:On Waiting....
		{
			m_nStepMk[0]++;
		}
		break;
	case 103:
		if (!IsInitPos0())
		{
			m_dTarget[AXIS_X0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
			m_dTarget[AXIS_Y0] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
			m_dNextTarget[AXIS_X0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
			m_dNextTarget[AXIS_Y0] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);

			MoveInitPos0();
		}

		pDoc->SaveMkCntL();
		m_nStepMk[0]++;
		break;
	case 104:
		if (IsMoveDone0())
		{
			m_nStepMk[0]++;
			SetDelay0(10000, 1);		// [mSec]
		}
		break;
	case 105:
		if (IsInitPos0())
		{
			m_nStepMk[0]++;
		}
		else
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
			{
				m_nStepMk[0] = ERR_PROC; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
		}
		break;
	case 106: // MK Done....
		m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
		m_bTHREAD_MK[0] = FALSE; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
		break;

	case ERR_PROC:
		DispMain(_T("정 지"), RGB_RED);
		if (m_pVoiceCoil[0])
			m_pVoiceCoil[0]->SearchHomeSmac0();
		MsgBox(_T("보이스코일(좌) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 1);
		m_nStepMk[0]++;
		break;
	case ERR_PROC + 1:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_bCam)
			sMsg.Format(_T("%d번 Shot을 다시 불량확인을 하시겠습니까?"), nSerial);
		else
			sMsg.Format(_T("%d번 Shot을 다시 마킹하시겠습니까?"), nSerial);

		m_nRtnMyMsgBoxIdx = 0;
		m_bRtnMyMsgBox[0] = FALSE;
		m_nRtnMyMsgBox[0] = -1;
		MsgBox(sMsg, 1, MB_YESNO);
		sMsg.Empty();
		m_nStepMk[0]++;
		break;
	case ERR_PROC + 2:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[0] = ERR_PROC + 10; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
			else
			{
				m_nRtnMyMsgBoxIdx = 0;
				m_bRtnMyMsgBox[0] = FALSE;
				m_nRtnMyMsgBox[0] = -1;
				sMsg.Format(_T("계속 다음 작업을 진행하시겠습니까?"), nSerial);
				MsgBox(sMsg, 1, MB_YESNO);
				sMsg.Empty();

				m_nStepMk[0]++;
			}
		}
		break;
	case ERR_PROC + 3:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[0] = ERR_PROC + 20; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
			else
			{
				m_bDispMsgDoAuto[8] = TRUE;
				m_nStepDispMsg[8] = FROM_DOMARK0;
			}
		}
		break;
	case ERR_PROC + 10:
		m_bReMark[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bReMark[0]"), m_bReMark[0]);
		m_bTHREAD_MK[0] = FALSE; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
		m_nStepMk[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
		break;
	case ERR_PROC + 20: // MK Done....
		m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
		m_bTHREAD_MK[0] = FALSE; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
		break;
	}
}

BOOL CGvisR2R_PunchView::SaveMk0Img(int nMkPcsIdx) // Cam0
{
	if (!pDoc->WorkingInfo.System.bSaveMkImg) return TRUE;

	int nSerial;
	nSerial = m_nBufUpSerial[0]; // Cam0

	CString sSrc, sDest, sPath;
	stModelInfo stInfo;

	sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
	if (!pDoc->GetPcrInfo(sSrc, stInfo))
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->DispStsBar(_T("E(2)"), 5);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-GetPcrInfo(2)"));
		return FALSE;
	}

	if (!pDoc->MakeMkDir(stInfo))
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-MakeMkDir()"));
		return FALSE;
	}

	sDest.Format(_T("%s%s\\%s\\%s\\Punching"), pDoc->WorkingInfo.System.sPathOldFile, stInfo.sModel,
		stInfo.sLot, stInfo.sLayer);

	if (!pDoc->DirectoryExists(sDest))
		CreateDirectory(sDest, NULL);

	if (GetTotDefPcs0(nSerial) > 0)
	{
		sPath.Format(_T("%s\\%s.tif"), sDest, GetMkInfo0(nSerial, nMkPcsIdx));

#ifdef USE_VISION
		if (m_pVision[0])
		{
			BOOL bRtn = m_pVision[0]->SaveMkImg(sPath);
			//if (bRtn)
			{
				if(m_pDlgMenu02)
					m_pDlgMenu02->DispMkPmScore(0);
			}
			return bRtn;
		}
#endif
	}
	else
		return TRUE;

	return FALSE;
}

void CGvisR2R_PunchView::DoMark1()
{
#ifdef TEST_MODE
	return;
#endif

	if (!m_bAuto) return;

	int nSerial, nIdx, nErrCode, nRtn;
	CfPoint ptPnt;
	CString sMsg;
	double dStripOut = (pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs / MAX_STRIP_NUM) * _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
	int nStripOut = int(dStripOut);
	if (dStripOut > nStripOut)
		nStripOut++;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!IsRun())
	{
		if (IsOnMarking1())
		{
			if (m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
			{
				if (m_pMotion->IsMotionDone(MS_X1) && m_pMotion->IsMotionDone(MS_Y1))
				{
					if (!IsMkEdPos1() && !IsPinPos1())
						MoveMkEdPos1();
				}
			}

			if (m_nStepMk[1] < 13 && m_nStepMk[1] > 8) // Mk1();
			{
				m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
			}
		}
		return;
	}


	m_sDispSts[1].Format(_T("%d"), m_nStepMk[1]);

	switch (m_nStepMk[1])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nStepMk[1]++;
		break;
	case 1:
		if (!IsInitPos1())
			MoveInitPos1();
		m_nStepMk[1]++;
		break;
	case 2:

		nSerial = m_nBufUpSerial[1]; // Cam1

		if (nSerial > 0)
		{
			if ((nErrCode = GetErrCode1(nSerial)) != 1)
			{
				m_nMkPcs[1] = GetTotDefPcs1(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
				m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
			else
			{
				m_nStepMk[1]++;
			}
		}
		else
		{
			StopFromThread();
			MsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 2);
			BuzzerFromThread(TRUE, 0);
			DispMain(_T("정 지"), RGB_RED);
		}
		break;
	case 3:
		m_nStepMk[1]++;
		break;
	case 4:
		m_nStepMk[1]++;
		break;
	case 5:
		m_nStepMk[1]++;
		break;
	case 6:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1(nSerial))
		{
			if (!IsNoMk1())
			{
				;
			}
			else
			{
				if (!IsReview1())
				{
					if (m_bReview)
					{
						m_nMkPcs[1] = GetTotDefPcs1(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
						m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
						break;
					}
				}
			}
			SetDelay1(100, 6);		// [mSec]
			m_nStepMk[1]++;
		}
		else
		{
			m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
		}
		break;
	case 7:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (!WaitDelay1(6))		// F:Done, T:On Waiting....
		{
			m_nMkPcs[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);

			if (!IsNoMk1())
			{
				m_nStepMk[1]++;
			}
			else
			{
				if (IsReview1())
				{
					m_nStepMk[1]++;
				}
				else
				{
					if (m_bReview)
					{
						m_nMkPcs[1] = GetTotDefPcs1(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
						m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
					}
					else
						m_nStepMk[1]++;
				}
			}
		}
		break;
	case 8:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1(nSerial))	// 마킹완료Check
		{
			if (m_nMkPcs[1] + 1 < GetTotDefPcs1(nSerial))
			{
				ptPnt = GetMkPnt1(nSerial, m_nMkPcs[1] + 1);
				m_dNextTarget[AXIS_X1] = ptPnt.x;
				m_dNextTarget[AXIS_Y1] = ptPnt.y;
			}
			else
			{
				m_dNextTarget[AXIS_X1] = -1.0;
				m_dNextTarget[AXIS_Y1] = -1.0;
			}

			m_nMarkingOrder[1] = m_nMkPcs[1];					// 이번 마킹위치
			ptPnt = GetMkPnt1(nSerial, m_nMkPcs[1]);			// 이번 마킹위치
			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[1]++;
				m_nStepMk[1] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
				break;
			}

			nIdx = GetMkStripIdx1(nSerial, m_nMkPcs[1]);
			if (nIdx > 0)
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[1]++;
					m_nStepMk[1] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
					break;
				}
				else
				{
					if (m_nMkStrip[1][nIdx - 1] >= nStripOut)
					{
						m_nMkPcs[1]++;
						m_nStepMk[1] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
						break;
					}
					else
						m_nMkStrip[1][nIdx - 1]++;
				}
			}
			else
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			m_dTarget[AXIS_X1] = ptPnt.x;
			m_dTarget[AXIS_Y1] = ptPnt.y;

			m_nStepMk[1]++;
		}
		else
		{
			m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
		}
		break;
	case 9:
		// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
		// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-
		ptPnt.x = m_dTarget[AXIS_X1];
		ptPnt.y = m_dTarget[AXIS_Y1];

		if (m_dNextTarget[AXIS_X1] < 0)
			m_bCollision[1] = ChkCollision(AXIS_X1, m_dTarget[AXIS_X1]);
		else
			m_bCollision[1] = ChkCollision(AXIS_X1, m_dTarget[AXIS_X1], m_dNextTarget[AXIS_X1]);

		if (!m_bCollision[1])
		{
			if (IsMoveDone1())
			{
				Move1(ptPnt, m_bCam);
				m_nStepMk[1]++;
			}
		}
		else if (m_bPriority[1]) // Cam1 ->
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[1] = FALSE;
				Move1(ptPnt, m_bCam);
				m_nStepMk[1]++;
			}
		}
		else if (m_bPriority[0]) // Cam0 <-	// 우선 순위 결정에서 Cam0에 먼저 양보함.
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[3] = FALSE;
				ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
				ptPnt.y = m_dEnc[AXIS_Y1];
				Move1(ptPnt, FALSE);
			}
		}
		else if (m_bPriority[2]) // Cam1 <-		// 우선 순위 결정에서 Cam0에 먼저 양보함.
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[3] = FALSE;
				ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
				ptPnt.y = m_dEnc[AXIS_Y1];
				Move1(ptPnt, FALSE);
			}
		}
		else if (m_bPriority[3]) // Cam0 ->	// 우선 순위 결정에서 Cam0에 먼저 양보함.
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[3] = FALSE;
				ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
				ptPnt.y = m_dEnc[AXIS_Y1];
				Move1(ptPnt, FALSE);
			}
		}
		break;
	case 10:
		if (IsMoveDone1())
			m_nStepMk[1]++;
		break;
	case 11:
		m_nStepMk[1]++;
		break;
	case 12:
			m_nStepMk[1]++;
		break;
	case 13:
		if (!IsNoMk1())
		{
			m_dwStMkDn[1] = GetTickCount();
			Mk1();
			nSerial = m_nBufUpSerial[1]; // Cam1
			SetMkPcs1(nSerial, m_nMarkingOrder[1]);
		}
		else
		{
			// Verify - Mk1
			InsertPunchingData(CAM_RT);
			SetDelay1(pDoc->m_nDelayShow, 6);		// [mSec]

			if (m_pDlgMenu02)
				m_pDlgMenu02->InitMkPmRst(1);

			if(!SaveMk1Img(m_nMkPcs[1]))
			{
				pView->ClrDispMsg();
				//AfxMessageBox(_T("Error-SaveMk1Img()"));
				if (pDoc->WorkingInfo.LastJob.bUseJudgeMk)
				{
#ifdef USE_VISION
					if (!m_pVision[1]->m_bMkJudge)
					{
						nRtn = MsgBox(_T("우측 펀칭이 미마킹으로 보입니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
						if (IDYES == nRtn)
						{
							ptPnt.x = m_dTarget[AXIS_X1];
							ptPnt.y = m_dTarget[AXIS_Y1];

							Move1(ptPnt, FALSE, WAIT);
							Sleep(30);
							Mk1();
							Sleep(100);
							Move1(ptPnt, TRUE, WAIT);
							m_nMkPcs[1]--; // Repeat Current Position Verify.
							m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
							Stop();
							break;
						}
					}
#endif
				}
			}
		}
		m_nStepMk[1]++;
		break;
	case 14:
		m_nStepMk[1]++;
		break;
	case 15:
		m_nStepMk[1]++;
		break;
	case 16:
		if (IsNoMk1())
		{
			if (!WaitDelay1(6))		// F:Done, T:On Waiting....
				m_nStepMk[1]++;
		}
		else
			m_nStepMk[1]++;
		break;
	case 17:
		if (!IsNoMk1())
		{
			if (IsMk1Done())
			{
				// One more MK On Start....
				if (!m_nMkPcs[1] && !m_bAnswer[3])
				{
					m_bAnswer[3] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[3]"), m_bAnswer[3]);
					Mk1();
				}
				else
				{
					m_bAnswer[3] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[3]"), m_bAnswer[3]);
					m_nMkPcs[1]++;
					m_nStepMk[1]++;
					m_nStepMk[1]++;
				}

			}
			else
			{
				if (m_dwStMkDn[1] + 5000 < GetTickCount())
				{
					BuzzerFromThread(TRUE, 0);
					DispMain(_T("정 지"), RGB_RED);
					if (m_pVoiceCoil[1])
						m_pVoiceCoil[1]->SearchHomeSmac1();

					nRtn = MsgBox(_T("보이스코일(우) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 2, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[1] = 13; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
					}
					else if (nRtn < 0)
						m_nStepMk[1]++; // Wait...
					else
					{
						m_bAnswer[3] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[3]"), m_bAnswer[3]);
						m_nMkPcs[1]++;
						m_nStepMk[1]++;
						m_nStepMk[1]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[1]++;
			m_nStepMk[1]++;
			m_nStepMk[1]++;
		}
		break;
	case 18:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[1] = 13; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
			else
			{
				m_bAnswer[3] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[3]"), m_bAnswer[3]);
				m_nMkPcs[1]++;
				m_nStepMk[1]++;
				StopFromThread();
			}
		}
		break;
	case 19:
		m_nStepMk[1] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
		break;
	case MK_DONE_CHECK:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1(nSerial))
		{
			if (IsNoMk1())
			{
				if (!pDoc->WorkingInfo.System.bNoMk)
				{
					if (IsReview1())
					{
						if (IsJogRtDn1())
							m_nStepMk[1]++;
					}
					else if (!pDoc->WorkingInfo.LastJob.bVerify)
					{
						m_nMkPcs[1] = GetTotDefPcs1(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
						m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
					}
					else
					{
						m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
					}
				}
				else
				{
					m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
				}
			}
			else
			{
				m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
			}
		}
		else
		{
			if (IsNoMk1())
			{
				if (IsReview1())
				{
					if (IsJogRtDn1())
					{
						m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
					}
				}
				else
				{
					m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
				}
			}
			else
			{
				m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
		}
		break;
	case MK_DONE_CHECK + 1:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1(nSerial))
		{
			if (IsNoMk1())
			{
				if (IsReview1())
				{
					if (IsJogRtUp1())
					{
						m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
					}
				}
				else
				{
					m_nMkPcs[1] = GetTotDefPcs1(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
					m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
				}
			}
			else
			{
				m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
			}
		}
		else
		{
			m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
		}
		break;


	case MK_END:
		if (IsMoveDone1())
			m_nStepMk[1]++;
		break;
	case 101:
		SetDelay1(100, 6);		// [mSec]
		m_nStepMk[1]++;
		break;
	case 102:
		if (!WaitDelay1(6))		// F:Done, T:On Waiting....
			m_nStepMk[1]++;
		break;
	case 103:
		if (!IsMkEdPos1())
		{
			m_dTarget[AXIS_X1] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
			m_dTarget[AXIS_Y1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
			m_dNextTarget[AXIS_X1] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
			m_dNextTarget[AXIS_Y1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);

			MoveMkEdPos1();
		}

		pDoc->SaveMkCntR();
		m_nStepMk[1]++;
		break;
	case 104:
		if (IsMoveDone1())
		{
			m_nStepMk[1]++;
			SetDelay1(10000, 6);		// [mSec]
		}
		break;
	case 105:
		if (IsMkEdPos1())
		{
			m_nStepMk[1]++;
		}
		else
		{
			if (!WaitDelay1(6))		// F:Done, T:On Waiting....
			{
				m_nStepMk[1] = ERR_PROC; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
		}
		break;
	case 106: // MK Done....
		m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
		m_bTHREAD_MK[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);
		break;

	case ERR_PROC:
		DispMain(_T("정 지"), RGB_RED);
		if (m_pVoiceCoil[1])
			m_pVoiceCoil[1]->SearchHomeSmac1();
		MsgBox(_T("보이스코일(우) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 2);
		m_nStepMk[1]++;
		break;
	case ERR_PROC + 1:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_bCam)
			sMsg.Format(_T("%d번 Shot을 다시 불량확인을 하시겠습니까?"), nSerial);
		else
			sMsg.Format(_T("%d번 Shot을 다시 마킹하시겠습니까?"), nSerial);

		m_nRtnMyMsgBoxIdx = 1;
		m_bRtnMyMsgBox[1] = FALSE;
		m_nRtnMyMsgBox[1] = -1;
		MsgBox(sMsg, 2, MB_YESNO);
		sMsg.Empty();
		m_nStepMk[1]++;
		break;
	case ERR_PROC + 2:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[1] = ERR_PROC + 10; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
			else
			{
				m_nRtnMyMsgBoxIdx = 1;
				m_bRtnMyMsgBox[1] = FALSE;
				m_nRtnMyMsgBox[1] = -1;
				sMsg.Format(_T("계속 다음 작업을 진행하시겠습니까?"), nSerial);
				MsgBox(sMsg, 2, MB_YESNO);
				sMsg.Empty();

				m_nStepMk[1]++;

			}
		}
		break;
	case ERR_PROC + 3:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[1] = ERR_PROC + 20; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
			else
			{
				m_bDispMsgDoAuto[9] = TRUE;
				m_nStepDispMsg[9] = FROM_DOMARK1;
			}
		}
		break;
	case ERR_PROC + 10:
		m_bReMark[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bReMark[1]"), m_bReMark[1]);
		m_bTHREAD_MK[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);
		m_nStepMk[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
		break;
	case ERR_PROC + 20: // MK Done....
		m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
		m_bTHREAD_MK[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);
		break;
	}
}

BOOL CGvisR2R_PunchView::SaveMk1Img(int nMkPcsIdx) // Cam1
{
	if (!pDoc->WorkingInfo.System.bSaveMkImg)
		return TRUE;

	int nSerial;
	nSerial = m_nBufUpSerial[1]; // Cam1

	CString sSrc, sDest, sPath;
	stModelInfo stInfo;

	sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
	if (!pDoc->GetPcrInfo(sSrc, stInfo))
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->DispStsBar(_T("E(2)"), 5);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-GetPcrInfo(2)"));
		return FALSE;
	}

	if (!pDoc->MakeMkDir(stInfo))
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-MakeMkDir()"));
		return FALSE;
	}

	sDest.Format(_T("%s%s\\%s\\%s\\Punching"), pDoc->WorkingInfo.System.sPathOldFile, stInfo.sModel,
		stInfo.sLot, stInfo.sLayer);

	if (!pDoc->DirectoryExists(sDest))
		CreateDirectory(sDest, NULL);

	if (GetTotDefPcs1(nSerial) > 0) // Cam1
	{
		sPath.Format(_T("%s\\%s.tif"), sDest, GetMkInfo1(nSerial, nMkPcsIdx));

#ifdef USE_VISION
		if (m_pVision[1])
		{
			BOOL bRtn = m_pVision[1]->SaveMkImg(sPath);
			//if (bRtn)
			{
				if (m_pDlgMenu02)
					m_pDlgMenu02->DispMkPmScore(1);
			}
			return bRtn;
		}
#endif
	}
	else
		return TRUE;

	return FALSE;
}

void CGvisR2R_PunchView::DispDefImg()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nSerialL = m_nBufUpSerial[0]; // 좌측 Camera
	int nSerialR = m_nBufUpSerial[1]; // 우측 Camera
	CString sNewLot = m_sNewLotUp;;
	CString str;
	BOOL bNewModel = FALSE;
	int nNewLot = 0;
	int nBreak = 0;
	int nLastShot = 0;

	switch (m_nStepTHREAD_DISP_DEF)
	{
	// CopyDefImg Start ============================================
	case 0:
		m_nStepTHREAD_DISP_DEF++;

		if (nSerialL == pView->m_nLotEndSerial)
			nBreak = 1;

		if (nSerialL > 0)
		{
			if (!CopyDefImg(nSerialL, sNewLot)) // 좌측 Camera
			{
				sNewLot.Empty();
				m_bDispMsgDoAuto[7] = TRUE;
				m_nStepDispMsg[7] = FROM_DISPDEFIMG + 7;
				break;
			}

			if (pView->m_bSerialDecrese)
			{
				if (m_bLastProc && nSerialR < m_nLotEndSerial)
				{
					nLastShot = nSerialL;
				}
				else
				{
					if (nSerialR > 0)
					{
						if (!CopyDefImg(nSerialR, sNewLot)) // 우측 Camera
						{
							sNewLot.Empty();
							m_bDispMsgDoAuto[6] = TRUE;
							m_nStepDispMsg[6] = FROM_DISPDEFIMG + 6;
							break;
						}
					}
				}
			}
			else
			{
				if (m_bLastProc && nSerialR > m_nLotEndSerial)
				{
					nLastShot = nSerialL;
				}
				else
				{
					if (nSerialR > 0)
					{
						if (!CopyDefImg(nSerialR, sNewLot)) // 우측 Camera
						{
							sNewLot.Empty();
							m_bDispMsgDoAuto[6] = TRUE;
							m_nStepDispMsg[6] = FROM_DISPDEFIMG + 6;
							break;
						}
					}
				}
			}

		}
		else
		{
			if (!m_bLastProc)
			{
				m_bDispMsgDoAuto[5] = TRUE;
				m_nStepDispMsg[5] = FROM_DISPDEFIMG + 5;
			}
		}
		sNewLot.Empty();
		break;
	case 1:
		//Sleep(300);
		m_nStepTHREAD_DISP_DEF++;
		if (pDoc->m_pReelMap)
		{
			str = pDoc->m_pReelMap->GetIpPath();
			pDoc->SetMkMenu01(_T("DispDefImg"), _T("ReelmapPath"), str);
		}
		str.Format(_T("%d"), nSerialL);
		pDoc->SetMkMenu01(_T("DispDefImg"), _T("SerialL"), str);
		str.Format(_T("%d"), nSerialR);
		pDoc->SetMkMenu01(_T("DispDefImg"), _T("SerialR"), str);
		break;
	case 2:
		if (IsDoneDispMkInfo())	 // Check 불량이미지 Display End
		{
			if (pView->m_bSerialDecrese)
			{
				if (ChkLastProc() && (nSerialL < m_nLotEndSerial))
				{
					nLastShot = nSerialL;
					SetSerialReelmap(nSerialL, TRUE);	// Reelmap(좌) Display Start
					SetSerialMkInfo(nSerialL, TRUE);		// 불량이미지(좌) Display Start
				}
				else if (ChkLastProc() && nSerialL < 1)
				{
					SetSerialReelmap(m_nLotEndSerial + 1, TRUE);		// Reelmap(좌) Display Start
					SetSerialMkInfo(m_nLotEndSerial + 1, TRUE);		// 불량이미지(좌) Display Start
				}
				else
				{
					SetFixPcs(nSerialL);
					SetSerialReelmap(nSerialL);	// Reelmap(좌) Display Start
					SetSerialMkInfo(nSerialL);	// 불량이미지(좌) Display Start
				}
			}
			else
			{
				if (ChkLastProc() && (nSerialL > m_nLotEndSerial))
				{
					nLastShot = nSerialL;
					SetSerialReelmap(nSerialL, TRUE);	// Reelmap(좌) Display Start
					SetSerialMkInfo(nSerialL, TRUE);		// 불량이미지(좌) Display Start
				}
				else if (ChkLastProc() && nSerialL < 1)
				{
					SetSerialReelmap(m_nLotEndSerial + 1, TRUE);		// Reelmap(좌) Display Start
					SetSerialMkInfo(m_nLotEndSerial + 1, TRUE);		// 불량이미지(좌) Display Start
				}
				else
				{
					SetFixPcs(nSerialL);
					SetSerialReelmap(nSerialL);	// Reelmap(좌) Display Start
					SetSerialMkInfo(nSerialL);	// 불량이미지(좌) Display Start
				}
			}


			if (IsFixPcsUp(nSerialL))
			{
				m_bDispMsgDoAuto[2] = TRUE;
				m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
			}
			if (IsFixPcsDn(nSerialL))
			{
				m_bDispMsgDoAuto[3] = TRUE;
				m_nStepDispMsg[3] = FROM_DISPDEFIMG + 3;
			}

			m_nStepTHREAD_DISP_DEF++;
		}

		break;

	case 3:
		if (IsDoneDispMkInfo())	 // Check 불량이미지(좌) Display End
		{
			if (nSerialR > 0) // 우측 Camera
			{
				if (pView->m_bSerialDecrese)
				{
					if (ChkLastProc() && (nSerialR < m_nLotEndSerial))
						SetSerialMkInfo(nSerialR, TRUE);	// 불량이미지(우) Display Start
					else
						SetSerialMkInfo(nSerialR);		// 불량이미지(우) Display Start
				}
				else
				{
					if (ChkLastProc() && (nSerialR > m_nLotEndSerial))
						SetSerialMkInfo(nSerialR, TRUE);	// 불량이미지(우) Display Start
					else
						SetSerialMkInfo(nSerialR);		// 불량이미지(우) Display Start
				}
			}
			else
			{
				if (ChkLastProc())
				{
					if (pView->m_bSerialDecrese)
						SetSerialMkInfo(m_nLotEndSerial - 1, TRUE);	// 불량이미지(우) Display Start
					else
						SetSerialMkInfo(m_nLotEndSerial + 1, TRUE);	// 불량이미지(우) Display Start
				}
			}
			m_nStepTHREAD_DISP_DEF++;
		}
		break;
	case 4:
		if (nSerialR > 0) // 우측 Camera
		{
			m_nStepTHREAD_DISP_DEF++;

			if (pView->m_bSerialDecrese)
			{
				if (ChkLastProc() && (nSerialR < m_nLotEndSerial))
				{
					SetSerialReelmap(nSerialR, TRUE);	// Reelmap(우) Display Start
				}
				else
				{
					SetFixPcs(nSerialR);
					SetSerialReelmap(nSerialR);			// Reelmap(우) Display Start
				}
			}
			else
			{
				if (ChkLastProc() && (nSerialR > m_nLotEndSerial))
				{
					SetSerialReelmap(nSerialR, TRUE);	// Reelmap(우) Display Start
				}
				else
				{
					SetFixPcs(nSerialR);
					SetSerialReelmap(nSerialR);			// Reelmap(우) Display Start
				}
			}

			if (bDualTest)
			{
				if (IsFixPcsUp(nSerialR))
				{
					m_bDispMsgDoAuto[2] = TRUE;
					m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
				}
				if (IsFixPcsDn(nSerialR))
				{
					m_bDispMsgDoAuto[3] = TRUE;
					m_nStepDispMsg[3] = FROM_DISPDEFIMG + 3;
				}
			}
			else
			{
				if (IsFixPcsUp(nSerialR))
				{
					m_bDispMsgDoAuto[2] = TRUE;
					m_nStepDispMsg[2] = FROM_DISPDEFIMG + 2;
				}
			}
		}
		else
		{
			if (ChkLastProc())
			{
				m_nStepTHREAD_DISP_DEF++;
				if (pView->m_bSerialDecrese)
					SetSerialReelmap(m_nLotEndSerial - 1, TRUE);	// 불량이미지(우) Display Start
				else
					SetSerialReelmap(m_nLotEndSerial + 1, TRUE);	// 불량이미지(우) Display Start
			}
			else
			{
				if (m_bLastProc && nSerialL == m_nLotEndSerial)
					m_nStepTHREAD_DISP_DEF++;
				else
				{
					m_nStepTHREAD_DISP_DEF++;
				}
			}
		}
		break;
	case 5:
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 6:
		if (IsDoneDispMkInfo() && IsRun())	 // Check 불량이미지(우) Display End
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 7:
		if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP)
		{
		m_nStepTHREAD_DISP_DEF++;

			pDoc->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("0"));
			if (m_pEngrave)
				m_pEngrave->SwMenu01DispDefImg(TRUE); // _SigInx::_DispDefImg
		}
		break;
	case 8:
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 9:
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 10:
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 11:
		m_nStepTHREAD_DISP_DEF++;
		break;
	case 12:
		m_bTHREAD_DISP_DEF = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_DISP_DEF"), m_bTHREAD_DISP_DEF);
		break;
		// CopyDefImg End ============================================
	}

}

void CGvisR2R_PunchView::DoAuto()
{
	if (!IsAuto())
		return;

	CString str;
	str.Format(_T("%d : %d"), m_nStepTHREAD_DISP_DEF, m_bTHREAD_DISP_DEF ? 1 : 0);
	pView->DispStsBar(str, 6);

	// LotEnd Start
	if (DoAutoGetLotEndSignal())
	{
		return;
	}

	// 마킹시작 신호를 확인 (PLC)
	DoAtuoGetMkStSignal();

	// LastProc Start (PLC)
	DoAutoSetLastProcAtPlc();

	// AOI Feeding Offset Start on LastProc (PLC)
	DoAutoSetFdOffsetLastProc();

	// AOI Feeding Offset Start (PLC)
	DoAutoSetFdOffset();

	// Engrave Feeding Offset Start (PLC)
	//DoAutoSetFdOffsetEngrave();

	// 알람발생시 CycleStop : ChkReTestAlarmOnAoiUp/Dn() - 잔량처리시 재검사요구에 PCR파일 Received 신호 On됨
	DoAutoChkCycleStop();

	// DispMsg (m_pDlgMenu01에 메인상태표시 - 정지, 운전, ...)
	DoAutoDispMsg();

	// Check Share Folder Start
	if (pDoc->m_bVsStatusUp) // WorkingInfo.System.sPathAoiUpCurrInfo : _T("Infomation"), _T("Current VS Status")
		DoAutoChkShareVsFolder();
	else
		DoAutoChkShareFolder();

	// Marking Start
	DoAutoMarking();

	// Engrave Marking Start
	DoAutoMarkingEngrave();
}

BOOL CGvisR2R_PunchView::DoAutoGetLotEndSignal()
{
	int nSerial;

	if (MpeRead(_T("MB40039F"))) // PLC 작업종료 신호 - Last Shot 피딩 후 신호 On
	{
		MpeWrite(_T("MB40039F"), 0);
		m_nMkStAuto = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
		m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
		m_nLotEndAuto = LOT_END;
	}

	//if (m_pDlgMenu01)
	//{
	//	if (m_pDlgMenu01->m_bLotEnd && m_nStepAuto < LOT_END)
	//	{
	//		m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
	//		m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
	//	}
	//}
	//
	//if (!IsBuffer(0) && m_bLastProc && m_nLotEndAuto < LOT_END)
	//{
	//	if (!pDoc->m_bVsStatusUp)
	//	{
	//		m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
	//		m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
	//	}
	//}
	//else if(!IsBuffer(0) && m_nMkStAuto > MK_ST + (Mk2PtIdx::DoneMk) + 4)
	//{
	//	if (!pDoc->m_bVsStatusUp)
	//	{
	//		m_nMkStAuto = 0; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
	//		m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
	//		m_nLotEndAuto = LOT_END;
	//	}
	//}
	//
	//if (!IsBuffer(0) && m_bLotEndF)
	//{
	//	m_bLotEndF = FALSE; pDoc->SetStatus(_T("General"), _T("bLotEndF"), m_bLotEndF);
	//	m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
	//	m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
	//}


	if (m_bLotEnd)
	{
		if (m_nLotEndSerial > 0)
			nSerial = m_nLotEndSerial;
		else
			nSerial = pDoc->GetLastShotMk();

		switch (m_nLotEndAuto)
		{
		case LOT_END:
			if (!m_bTHREAD_REELMAP_YIELD_UP && !m_bTHREAD_REELMAP_YIELD_DN && !m_bTHREAD_REELMAP_YIELD_ALLUP && !m_bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
			{
				UpdateRst();
				m_nLotEndAuto++;
			}
			break;
		case LOT_END + 1:
			MpeWrite(Plc.DlgMenu01.JobEnd, 1);		// 작업종료(PC가 On시키고, PLC가 확인하고 Off시킴)
			//MpeWrite(_T("MB40019C"), 1);			// 작업종료알람(PC가 On시키고, PLC가 확인하고 Off시킴)
			DispMain(_T("작업종료"), RGB_RED);
			m_nLotEndAuto++;
			break;
		case LOT_END + 2:
			m_nLotEndAuto++;
			break;
		case LOT_END + 3:
			Buzzer(TRUE, 0);
			TowerLamp(RGB_YELLOW, TRUE);
			Stop();
			LotEnd();					
			m_nLotEndAuto++;
			break;

		case LOT_END + 4:
			MsgBox(_T("작업이 종료되었습니다."));
			m_nLotEndAuto++;
			m_bLastProc = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
			m_bMkSt[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt0"), m_bMkSt[0]);
			m_bMkSt[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt1"), m_bMkSt[1]);
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;
		case LOT_END + 5:
			break;
		}
	}

	return m_bLotEnd;
}

void CGvisR2R_PunchView::DoAtuoGetMkStSignal()
{
	if (!pDoc->m_pMpeSignal)
		return;

	BOOL bMk0 = MpeRead(_T("MB400236")); // MB400236 : MK0 - PLC 마킹 마킹 시작#1
	BOOL bMk1 = MpeRead(_T("MB400237")); // MB400237 : MK0 & MK1 - PLC 마킹 마킹 시작#2
	//BOOL bMk0 = pDoc->m_pMpeSignal[3] & (0x01 << 6); // MB400236 : MK0 - PLC 마킹 마킹 시작#1
	//BOOL bMk1 = pDoc->m_pMpeSignal[3] & (0x01 << 7); // MB400237 : MK0 & MK1 - PLC 마킹 마킹 시작#2

	if (m_pMpe)
	{
		if (!m_bMkSt[0] && !m_bMkSt[1])
		{
			if (IsRun())
			{
				if (bMk0 || bMk1)
				{
					if (!IsSetPinPos())
						return;
				}

				if (bMk1 || m_bMkStSw[1])	// 마킹시작(PC가 확인하고 Reset시킴.)
				{
					pDoc->LogAuto(_T("PLC: 마킹시작 #1 (PC가 확인하고 Reset시킴.)"));
					m_bMkStSw[1] = FALSE;
					MpeWrite(Plc.DlgMenu01.MarkingStart, 0);			// 마킹시작(PC가 확인하고 Reset시킴.)
					Sleep(30);
					MpeWrite(Plc.DlgMenu01.MarkingStart, 0);			// 마킹시작(PC가 확인하고 Reset시킴.)
					m_bMkSt[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bMkSt0"), m_bMkSt[0]);
					m_bMkSt[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bMkSt1"), m_bMkSt[1]);
					m_nMkStAuto = MK_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					::WritePrivateProfileString(_T("Last Job"), _T("MkSt0"), _T("1"), PATH_WORKING_INFO);
					::WritePrivateProfileString(_T("Last Job"), _T("MkSt1"), _T("1"), PATH_WORKING_INFO);
					if (!pDoc->GetLastShotMk())
						SetLotSt();		// 새로운 로트의 시작시간을 설정함. // 전체속도의 처음 시작되는 시간 설정.
				}
				else if (bMk0 || m_bMkStSw[0])	// 마킹시작(PC가 확인하고 Reset시킴.)
				{
					pDoc->LogAuto(_T("PLC: 마킹시작 #0 (PC가 확인하고 Reset시킴.)"));
					m_bMkStSw[0] = FALSE;
					MpeWrite(Plc.DlgMenu01.MarkingStart, 0);			// 마킹시작(PC가 확인하고 Reset시킴.)
					Sleep(30);
					MpeWrite(Plc.DlgMenu01.MarkingStart, 0);			// 마킹시작(PC가 확인하고 Reset시킴.)
					m_bMkSt[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bMkSt0"), m_bMkSt[0]);
					m_nMkStAuto = MK_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					::WritePrivateProfileString(_T("Last Job"), _T("MkSt0"), _T("1"), PATH_WORKING_INFO);
					if (!pDoc->GetLastShotMk())
						SetLotSt();		// 새로운 로트의 시작시간을 설정함. // 전체속도의 처음 시작되는 시간 설정.
				}

			}
		}
	}
}

void CGvisR2R_PunchView::DoAutoSetLastProcAtPlc()
{
	if (m_bLastProc)
	{
		switch (m_nLastProcAuto)
		{
		case LAST_PROC:	// 잔량처리 1
			if (IsRun())
			{
				if (ChkLastProcFromUp())
				{
					MpeWrite(Plc.DlgMenu03.PcrReceivedAoiUp, 1);			// AOI 상 : PCR파일 Received
				}
				m_nLastProcAuto++;
			}
			break;
		case LAST_PROC + 1:
			pDoc->LogAuto(_T("PC: 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)"));
			MpeWrite(Plc.DlgMenu01.DoLastJob, 1);			// 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
			m_nLastProcAuto++;
			break;
		case LAST_PROC + 2:
			;
			break;
		}
	}
}

void CGvisR2R_PunchView::DoAutoSetFdOffsetLastProc()
{
#ifdef USE_MPE
	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 11)) ? TRUE : FALSE;		// 검사부 상 자동 운전 <-> X432B I/F
	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01 << 11)) ? TRUE : FALSE;		// 검사부 하 자동 운전 <-> X442B I/F

	if (bOn0 && !(m_Flag & (0x01 << 2)))
	{
		m_Flag |= (0x01 << 2); pDoc->SetStatusInt(_T("General"), _T("Flag"), m_Flag);
	}
	else if (!bOn0 && (m_Flag & (0x01 << 2)))
	{
		m_Flag &= ~(0x01 << 2); pDoc->SetStatusInt(_T("General"), _T("Flag"), m_Flag);

		m_bAoiTest[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTest[0]"), m_bAoiTest[0]);
		m_bWaitPcr[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[0]"), m_bWaitPcr[0]);
		m_bAoiTestF[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTestF[0]"), m_bAoiTestF[0]);
		m_bAoiFdWriteF[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[0]"), m_bAoiFdWriteF[0]);
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneAoiUp, 0); // 검사부(상) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
		pDoc->LogAuto(_T("PLC: 검사부(상) Feeding Offset Write 완료(PC가 확인하고 MB440111 Reset시킴.)"));
	}

	if (bOn1 && !(m_Flag & (0x01 << 3)))
	{
		m_Flag |= (0x01 << 3); pDoc->SetStatusInt(_T("General"), _T("Flag"), m_Flag);
	}
	else if (!bOn1 && (m_Flag & (0x01 << 3)))
	{
		m_Flag &= ~(0x01 << 3); pDoc->SetStatusInt(_T("General"), _T("Flag"), m_Flag);

		m_bAoiTest[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTest[1]"), m_bAoiTest[1]);
		m_bWaitPcr[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[1]"), m_bWaitPcr[1]);
		m_bAoiTestF[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTestF[1]"), m_bAoiTestF[1]);
		m_bAoiFdWriteF[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[1]"), m_bAoiFdWriteF[1]);
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneAoiDn, 0); // 검사부(하) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
		pDoc->LogAuto(_T("PLC: 검사부(하) Feeding Offset Write 완료(PC가 확인하고 MB440112 Reset시킴.)"));
	}
#endif
}

void CGvisR2R_PunchView::DoAutoSetFdOffset()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
#ifdef USE_MPE
	double dAveX, dAveY;
	CfPoint OfStUp, OfStDn;

	BOOL bOnTestUp = MpeRead(_T("MB40023C")); // PLC AOI 상 검사 중

	//if (pDoc->m_pMpeSignal[3] & (0x01 << 12) && !m_bAoiTestF[0])		// PLC AOI 상 검사 중
	if (bOnTestUp && !m_bAoiTestF[0])		// PLC AOI 상 검사 중
	{
		m_bAoiTestF[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bAoiTestF[0]"), m_bAoiTestF[0]);
		m_bAoiTest[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bAoiTest[0]"), m_bAoiTest[0]);
		m_bWaitPcr[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[0]"), m_bWaitPcr[0]);
	}
	else if (!bOnTestUp && m_bAoiTestF[0])
	{
		m_bAoiTestF[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTestF[0]"), m_bAoiTestF[0]);
		m_bAoiTest[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTest[0]"), m_bAoiTest[0]);
		m_bAoiFdWriteF[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[0]"), pView->m_bAoiFdWriteF[0]);

		//MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneAoiUp, 0); // 검사부(상) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
		pDoc->LogAuto(_T("PLC: 검사부(상) Feeding Offset Write 완료(PC가 확인하고 MB440111 Reset시킴.)"));
	}

	BOOL bOnTestDn = MpeRead(_T("MB40023D")); // PLC AOI 하 검사 중

	//if (pDoc->m_pMpeSignal[3] & (0x01 << 13) && !m_bAoiTestF[1])		// PLC AOI 하 검사 중
	if (bOnTestDn && !m_bAoiTestF[1])		// PLC AOI 하 검사 중
	{
		m_bAoiTestF[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bAoiTestF[1]"), m_bAoiTestF[1]);
		m_bAoiTest[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bAoiTest[1]"), m_bAoiTest[1]);
		m_bWaitPcr[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[1]"), m_bWaitPcr[1]);
	}
	//else if (!(pDoc->m_pMpeSignal[3] & (0x01 << 13)) && m_bAoiTestF[1])
	else if (!bOnTestDn && m_bAoiTestF[1])
	{
		m_bAoiTestF[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTestF[1]"), m_bAoiTestF[1]);
		m_bAoiTest[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTest[1]"), m_bAoiTest[1]);
		m_bAoiFdWriteF[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[1]"), m_bAoiFdWriteF[1]);
		//MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneAoiDn, 0); // 검사부(하) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
		pDoc->LogAuto(_T("PLC: 검사부(하) Feeding Offset Write 완료(PC가 확인하고 MB440112 Reset시킴.)"));
	}

	BOOL bOffsetDoneUp = MpeRead(_T("MB40024D")); // PLC AOI 상 피딩 Offset Write(GUI Off)

	//if (pDoc->m_pMpeSignal[4] & (0x01 << 13) && !m_bAoiFdWrite[0])		// 검사부(상) Feeding Offset Write 완료
	if (bOffsetDoneUp && !m_bAoiFdWrite[0])		// 검사부(상) Feeding Offset Write 완료
	{
		m_bAoiFdWrite[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bAoiFdWrite[0]"), m_bAoiFdWrite[0]);
	}
	//else if (!(pDoc->m_pMpeSignal[4] & (0x01 << 13)) && m_bAoiFdWrite[0])
	else if (!bOffsetDoneUp && m_bAoiFdWrite[0])
	{
		m_bAoiFdWrite[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWrite[0]"), m_bAoiFdWrite[0]);
	}

	BOOL bOffsetDoneDn = MpeRead(_T("MB40024E")); // PLC AOI 하 피딩 Offset Write(GUI Off)

	//if (pDoc->m_pMpeSignal[4] & (0x01 << 14) && !m_bAoiFdWrite[1])		// 검사부(하) Feeding Offset Write 완료-20141103
	if (bOffsetDoneDn && !m_bAoiFdWrite[1])		// 검사부(하) Feeding Offset Write 완료-20141103
		m_bAoiFdWrite[1] = TRUE;
	//else if (!(pDoc->m_pMpeSignal[4] & (0x01 << 14)) && m_bAoiFdWrite[1])
	else if (!bOffsetDoneDn && m_bAoiFdWrite[1])
		m_bAoiFdWrite[1] = FALSE;


	if (bDualTest) // 20160721-syd-temp
	{
		if ((m_bAoiFdWrite[0] && m_bAoiFdWrite[1]) && (!m_bAoiFdWriteF[0] && !m_bAoiFdWriteF[1]))
		{
			m_bAoiFdWriteF[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[0]"), pView->m_bAoiFdWriteF[0]);
			m_bAoiFdWriteF[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[1]"), pView->m_bAoiFdWriteF[1]);

			GetAoiUpOffset(OfStUp);
			GetAoiDnOffset(OfStDn);

			dAveX = OfStUp.x;
			dAveY = OfStUp.y;
			//dAveX = (OfStUp.x + OfStDn.x) / 2.0;
			//dAveY = (OfStUp.y + OfStDn.y) / 2.0;

			if (m_pDlgMenu02)
			{
				m_pDlgMenu02->m_dAoiUpFdOffsetX = OfStUp.x;
				m_pDlgMenu02->m_dAoiUpFdOffsetY = OfStUp.y;
				m_pDlgMenu02->m_dAoiDnFdOffsetX = OfStDn.x;
				m_pDlgMenu02->m_dAoiDnFdOffsetY = OfStDn.y;
			}

			if (bDualTest)
			{
				MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiUp, (long)(-1.0*OfStUp.x*1000.0));
				MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiDn, (long)(-1.0*OfStDn.x*1000.0));
				//MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiDn, (long)(-1.0*dAveX*1000.0));
			}
			else
			{
				MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiUp, (long)(-1.0*OfStUp.x*1000.0));
				//MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiUp, (long)(-1.0*dAveX*1000.0));
			}

			MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneAoiUp, 0); // 검사부(상) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
			MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneAoiDn, 0); // 검사부(하) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
			pDoc->LogAuto(_T("PLC: 검사부(상MB440111,하MB440112) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
		}
		else if ((!m_bAoiFdWrite[0] && !m_bAoiFdWrite[1]) && (m_bAoiFdWriteF[0] && m_bAoiFdWriteF[1]))
		{
			m_bAoiFdWriteF[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[0]"), pView->m_bAoiFdWriteF[0]);
			m_bAoiFdWriteF[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[1]"), pView->m_bAoiFdWriteF[1]);
			m_bAoiTest[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTest[0]"), m_bAoiTest[0]);
			m_bAoiTest[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTest[1]"), m_bAoiTest[1]);
		}

		//if (m_bAoiTest[0] && !m_bAoiTest[1])
		{
			if (m_bAoiFdWrite[0] && !m_bAoiFdWriteF[0])
			{
				m_bAoiFdWriteF[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[0]"), pView->m_bAoiFdWriteF[0]);

				GetAoiUpOffset(OfStUp);

				if (m_pDlgMenu02)
				{
					m_pDlgMenu02->m_dAoiUpFdOffsetX = OfStUp.x;
					m_pDlgMenu02->m_dAoiUpFdOffsetY = OfStUp.y;
					m_pDlgMenu02->m_dAoiDnFdOffsetX = 0.0;
					m_pDlgMenu02->m_dAoiDnFdOffsetY = 0.0;
				}
				if (bDualTest)
				{
					MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiUp, (long)(-1.0*OfStUp.x*1000.0));
					//MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiDn, (long)(-1.0*OfStUp.x*1000.0));
				}
				else
				{
					MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiUp, (long)(-1.0*OfStUp.x*1000.0));
				}
				MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneAoiUp, 0); // 검사부(상) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
				pDoc->LogAuto(_T("PLC: 검사부(상MB440111) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
			}
			else if (!m_bAoiFdWrite[0] && m_bAoiFdWriteF[0])
			{
				m_bAoiFdWriteF[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[0]"), pView->m_bAoiFdWriteF[0]);
				m_bAoiTest[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTest[0]"), m_bAoiTest[0]);
			}
		}

		//if (!m_bAoiTest[0] && m_bAoiTest[1])
		{
			if (m_bAoiFdWrite[1] && !m_bAoiFdWriteF[1])
			{
				m_bAoiFdWriteF[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[1]"), pView->m_bAoiFdWriteF[1]);

				GetAoiDnOffset(OfStDn);

				if (m_pDlgMenu02)
				{
					m_pDlgMenu02->m_dAoiUpFdOffsetX = 0.0;
					m_pDlgMenu02->m_dAoiUpFdOffsetY = 0.0;
					m_pDlgMenu02->m_dAoiDnFdOffsetX = OfStDn.x;
					m_pDlgMenu02->m_dAoiDnFdOffsetY = OfStDn.y;
				}
				if (bDualTest)
				{
					MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiDn, (long)(-1.0*OfStDn.x*1000.0));
					//else
					//	MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiUp, (long)(-1.0*OfStDn.x*1000.0));
					MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneAoiDn, 0); // 검사부(하) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
					pDoc->LogAuto(_T("PLC: 검사부(하MB440112) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
				}
			}
			else if (!m_bAoiFdWrite[1] && m_bAoiFdWriteF[1])
			{
				m_bAoiFdWriteF[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[1]"), pView->m_bAoiFdWriteF[1]);
				m_bAoiTest[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTest[1]"), m_bAoiTest[1]);
			}
		}
	}
	else // 20160721-syd-temp
	{
		if (m_bAoiFdWrite[0] && !m_bAoiFdWriteF[0])
		{
			m_bAoiFdWriteF[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[0]"), pView->m_bAoiFdWriteF[0]);

			GetAoiUpOffset(OfStUp);

			dAveX = (OfStUp.x);
			dAveY = (OfStUp.y);

			if (m_pDlgMenu02)
			{
				m_pDlgMenu02->m_dAoiUpFdOffsetX = dAveX;
				m_pDlgMenu02->m_dAoiUpFdOffsetY = dAveY;
			}

			MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiUp, (long)(-1.0*OfStUp.x*1000.0));	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
			//MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiDn, (long)(-1.0*dAveX*1000.0));	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)

			MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneAoiUp, 0); // 검사부(상) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
			MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneAoiDn, 0); // 검사부(하) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103  // 20160721-syd-temp
			pDoc->LogAuto(_T("PLC: 검사부(상MB440111,하MB440112) Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
		}
		else if (!m_bAoiFdWrite[0] && m_bAoiFdWriteF[0])
		{
			m_bAoiFdWriteF[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiFdWriteF[0]"), pView->m_bAoiFdWriteF[0]);
			m_bAoiTest[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAoiTest[0]"), m_bAoiTest[0]);
		}
	}

	if (pDoc->GetTestMode() == MODE_INNER || pDoc->WorkingInfo.System.bUseEngrave)
	{
		CfPoint OfStEng;

		BOOL bOffsetDoneEng = MpeRead(Plc.DlgFrameHigh.FeedOffsetWriteDoneEngrave); // PLC 각인기 피딩 Offset Write(GUI Off) _T("MB40024C")

		//if (pDoc->m_pMpeSignal[4] & (0x01 << 12) && !m_bEngFdWrite)		// PLC: 각인부 Feeding Offset Write 완료
		if (bOffsetDoneEng && !m_bEngFdWrite)		// PLC: 각인부 Feeding Offset Write 완료
			m_bEngFdWrite = TRUE;
		//else if (!(pDoc->m_pMpeSignal[4] & (0x01 << 12)) && m_bEngFdWrite)
		else if (!bOffsetDoneEng && m_bEngFdWrite)
			m_bEngFdWrite = FALSE;

		if (m_bEngFdWrite && !m_bEngFdWriteF)
		{
			m_bEngFdWriteF = TRUE;

			GetEngOffset(OfStEng);

			dAveX = (OfStEng.x);
			dAveY = (OfStEng.y);

			if (m_pDlgMenu02)
			{
				m_pDlgMenu02->m_dEngFdOffsetX = dAveX;
				m_pDlgMenu02->m_dEngFdOffsetY = dAveY;
			}

			MpeWrite(Plc.DlgFrameHigh.FeedOffsetEngrave, (long)(-1.0*dAveX*1000.0));	// 각인부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
			MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneEngrave, 0); // 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)-20141103
			pDoc->LogAuto(_T("PLC: 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
		}
		else if (!m_bEngFdWrite && m_bEngFdWriteF)
		{
			m_bEngFdWriteF = FALSE; 
		}
	}

	BOOL bOffsetDonePunch = MpeRead(Plc.DlgFrameHigh.FeedOffsetWriteDonePunch); // PLC 각인기 피딩 Offset Write(GUI Off) _T("MB40024F")

	if (bOffsetDonePunch && !m_bMkFdWrite)		// PLC: 각인부 Feeding Offset Write 완료
		m_bMkFdWrite = TRUE;
	else if (!bOffsetDonePunch && m_bMkFdWrite)
		m_bMkFdWrite = FALSE;

	if (m_bMkFdWrite && !m_bMkFdWriteF)
	{
		m_bMkFdWriteF = TRUE;

		CfPoint OfStMk;
		GetMkOffset(OfStMk);

		dAveX = (OfStMk.x);
		dAveY = (OfStMk.y);

		MpeWrite(pView->Plc.DlgFrameHigh.FeedOffsetPunch, (long)(-1.0 * dAveX * 1000.0));	// 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDonePunch, 0); // 마킹부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
		pDoc->LogAuto(_T("PLC: 마킹부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
	}
	else if (!m_bMkFdWrite && m_bMkFdWriteF)
	{
		m_bMkFdWriteF = FALSE;
	}

#endif
}

void CGvisR2R_PunchView::DoAutoChkCycleStop()
{
	if (m_bCycleStop) // 알람발생시
	{
		m_bCycleStop = FALSE; pDoc->SetStatus(_T("General"), _T("bCycleStop"), m_bCycleStop);
		Buzzer(TRUE);
		TowerLamp(RGB_YELLOW, TRUE);
		if (!pDoc->m_sAlmMsg.IsEmpty())
		{
			pDoc->LogAuto(pDoc->m_sAlmMsg);
			MsgBox(pDoc->m_sAlmMsg, 0, MB_OK, DEFAULT_TIME_OUT, FALSE);
			MpeWrite(_T("MB400183"), 1); // 알람메시지 확인

			if (pDoc->m_sAlmMsg == m_sAoiUpAlarmReStartMsg || pDoc->m_sAlmMsg == m_sAoiUpAlarmReTestMsg)
			{
				ChkReTestAlarmOnAoiUp();
			}
			else if (pDoc->m_sAlmMsg == m_sAoiDnAlarmReStartMsg || pDoc->m_sAlmMsg == m_sAoiDnAlarmReTestMsg)
			{
				ChkReTestAlarmOnAoiDn();
			}
		}
		pDoc->m_sAlmMsg = _T("");
		pDoc->m_sIsAlmMsg = _T("");
		pDoc->m_sPrevAlmMsg = _T("");
	}
}

void CGvisR2R_PunchView::DoAutoDispMsg()
{
	BOOL bDispMsg = FALSE;
	int idx, nStepDispMsg;

	// [2] : 고정불량-상, [3] : 고정불량-하, [5] : nSerialL <= 0, [6] : CopyDefImg 우측 Camera,  
	// [7] : CopyDefImg 좌측 Camera, [8] : 보이스코일(좌) 초기위치 이동, [9] : 보이스코일(우) 초기위치 이동
	for (idx = 0; idx < 10; idx++)
	{
		if (m_bDispMsgDoAuto[idx]) 
		{
			bDispMsg = TRUE;
			nStepDispMsg = m_nStepDispMsg[idx];
			break;
		}
	}
	if (bDispMsg && IsRun())
	{
		switch (nStepDispMsg)
		{
		case 0:
			break;
		case FROM_DOMARK0:
			m_bDispMsgDoAuto[8] = FALSE;
			m_nStepDispMsg[8] = 0;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_YELLOW, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			break;
		case FROM_DOMARK1:
			m_bDispMsgDoAuto[9] = FALSE;
			m_nStepDispMsg[9] = 0;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_YELLOW, TRUE);
			Stop();
			DispMain(_T("정 지"), RGB_RED);
			break;
		case FROM_DISPDEFIMG:
			m_bDispMsgDoAuto[0] = FALSE;
			m_nStepDispMsg[0] = 0;
			Stop();
			MsgBox(_T("버퍼(우) Serial이 맞지않습니다."));
			TowerLamp(RGB_YELLOW, TRUE);
			break;
		case FROM_DISPDEFIMG + 1:
			m_bDispMsgDoAuto[1] = FALSE;
			m_nStepDispMsg[1] = 0;
			Stop();
			MsgBox(_T("버퍼(우) Serial이 맞지않습니다."));
			TowerLamp(RGB_YELLOW, TRUE);
			break;
		case FROM_DISPDEFIMG + 2: // IsFixUp
			m_bDispMsgDoAuto[2] = FALSE;
			m_nStepDispMsg[2] = 0;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(m_sFixMsg[0]);
			m_sFixMsg[0] = _T("");
			break;
		case FROM_DISPDEFIMG + 3: // IsFixDn
			m_bDispMsgDoAuto[3] = FALSE;
			m_nStepDispMsg[3] = 0;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			DispMain(_T("정 지"), RGB_RED);
			MsgBox(m_sFixMsg[1]);
			m_sFixMsg[1] = _T("");
			break;
		case FROM_DISPDEFIMG + 4:
			break;
		case FROM_DISPDEFIMG + 5:
			m_bDispMsgDoAuto[5] = FALSE;
			m_nStepDispMsg[5] = 0;
			Stop();
			MsgBox(_T("버퍼(좌) Serial이 맞지않습니다."));
			TowerLamp(RGB_YELLOW, TRUE);
			break;
		case FROM_DISPDEFIMG + 6:
			m_bDispMsgDoAuto[6] = FALSE;
			m_nStepDispMsg[6] = 0;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			DispMain(_T("정 지"), RGB_RED);
			break;
		case FROM_DISPDEFIMG + 7:
			m_bDispMsgDoAuto[7] = FALSE;
			m_nStepDispMsg[7] = 0;
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();
			m_bSwStopNow = TRUE;
			m_bSwRunF = FALSE;
			DispMain(_T("정 지"), RGB_RED);
			break;
		}
	}
}

void CGvisR2R_PunchView::DispMainOnAutoStart()
{
	if (pDoc->WorkingInfo.LastJob.bSampleTest)
	{
		if (pDoc->WorkingInfo.LastJob.bDualTest)
		{
			if (m_sDispMain != _T("양면샘플"))
				DispMain(_T("양면샘플"), RGB_GREEN);
		}
		else
		{
			if (m_sDispMain != _T("단면샘플"))
				DispMain(_T("단면샘플"), RGB_GREEN);
		}
	}
	else if (pDoc->GetTestMode() == MODE_INNER)
	{
		if (m_sDispMain != _T("내층검사"))
			DispMain(_T("내층검사"), RGB_GREEN);
	}
	else if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (m_sDispMain != _T("외층검사"))
			DispMain(_T("외층검사"), RGB_GREEN);
	}
	else if (pDoc->WorkingInfo.LastJob.bDualTest)
	{
		if (m_sDispMain != _T("양면검사"))
			DispMain(_T("양면검사"), RGB_GREEN);
	}
	else
	{
		if (m_sDispMain != _T("단면검사"))
			DispMain(_T("단면검사"), RGB_GREEN);
	}
}

void CGvisR2R_PunchView::CheckShareDn(int nSerial)
{
	if (pView->m_bSerialDecrese)
	{
		if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
		{
			// Delete PCR File : 검사부(하)에서 잔량처리 후 들어오는 PCR파일은 삭제
			pDoc->DelSharePcrDn();
		}
		else
		{
			m_bPcrInShare[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bPcrInShare[1]"), m_bPcrInShare[1]);
		}
	}
	else
	{
		if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
		{
			// Delete PCR File : 검사부(하)에서 잔량처리 후 들어오는 PCR파일은 삭제
			pDoc->DelSharePcrDn();
		}
		else
		{
			m_bPcrInShare[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bPcrInShare[1]"), m_bPcrInShare[1]);
		}
	}
}

void CGvisR2R_PunchView::CheckShareUp(int nSerial)
{
	if (pView->m_bSerialDecrese)
	{
		if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
		{
			// Delete PCR File : 검사부(상)에서 잔량처리 후 들어오는 PCR파일은 삭제
			pDoc->DelSharePcrUp();
		}
		else
		{
			m_bPcrInShare[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bPcrInShare[0]"), m_bPcrInShare[0]);
		}
	}
	else
	{
		if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
		{
			// Delete PCR File : 검사부(상)에서 잔량처리 후 들어오는 PCR파일은 삭제
			pDoc->DelSharePcrUp();
		}
		else
		{
			m_bPcrInShare[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bPcrInShare[0]"), m_bPcrInShare[0]);
		}
	}
}

void CGvisR2R_PunchView::CheckShareVsDn(int nSerial)
{
	if (pView->m_bSerialDecrese)
	{
		if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
		{
			// Delete PCR File : 검사부(하)에서 잔량처리 후 들어오는 PCR파일은 삭제
			pDoc->DelShareVsPcrDn();
		}
		else
		{
			m_nShareDnS = nSerial;
			m_bPcrInShareVs[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bPcrInShareVs[1]"), m_bPcrInShareVs[1]);
		}
	}
	else
	{
		if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
		{
			// Delete PCR File : 검사부(하)에서 잔량처리 후 들어오는 PCR파일은 삭제
			pDoc->DelShareVsPcrDn();
		}
		else
		{
			m_nShareDnS = nSerial;
			m_bPcrInShareVs[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bPcrInShareVs[1]"), m_bPcrInShareVs[1]);
		}
	}
}

void CGvisR2R_PunchView::CheckShareVsUp(int nSerial)
{
	if (pView->m_bSerialDecrese)
	{
		if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
		{
			// Delete PCR File : 검사부(상)에서 잔량처리 후 들어오는 PCR파일은 삭제
			pDoc->DelShareVsPcrUp();
		}
		else
		{
			m_nShareUpS = nSerial;
			m_bPcrInShareVs[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bPcrInShareVs[0]"), m_bPcrInShareVs[0]);
		}
	}
	else
	{
		if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
		{
			// Delete PCR File : 검사부(상)에서 잔량처리 후 들어오는 PCR파일은 삭제
			pDoc->DelShareVsPcrUp();
		}
		else
		{
			m_nShareUpS = nSerial;
			m_bPcrInShareVs[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bPcrInShareVs[0]"), m_bPcrInShareVs[0]);
		}
	}
}

void CGvisR2R_PunchView::TurnLastProcVs()
{
	m_nLastProcAuto = LAST_PROC; pDoc->SetStatusInt(_T("General"), _T("nLastProcAuto"), m_nLastProcAuto);
	m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
	int nSerial = GetShareVsUp();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		if (ChkLastProcFromEng())
		{
			if (IsSetLotEnd())
				nSerial = GetLotEndSerial();
			else
				nSerial = pDoc->GetCurrentInfoEngShotNum();
		}
		else if (ChkLastProcFromUp())
		{
			if (IsSetLotEnd())
				nSerial = GetLotEndSerial();
			else
				nSerial = pDoc->m_ListBuf[0].GetLast();
		}
		else
		{
			if (IsSetLotEnd())
				nSerial = GetLotEndSerial();
			else
				nSerial = pDoc->m_ListBuf[1].GetLast();
		}
	}
	else
	{
		if (ChkLastProcFromEng())
		{
			if (IsSetLotEnd())
				nSerial = GetLotEndSerial();
			else
				nSerial = pDoc->GetCurrentInfoEngShotNum();
		}
		else
		{
			if (IsSetLotEnd())
				nSerial = GetLotEndSerial();
			else
				nSerial = pDoc->m_ListBuf[0].GetLast();
		}
	}

	if (!IsSetLotEnd())
	{
		SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
	}
}

void CGvisR2R_PunchView::TurnLastProc()
{
	m_nLastProcAuto = LAST_PROC; pDoc->SetStatusInt(_T("General"), _T("nLastProcAuto"), m_nLastProcAuto);
	m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
	int nSerial = GetShareUp();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		if (ChkLastProcFromEng())
		{
			if (IsSetLotEnd())
				nSerial = GetLotEndSerial();
			else
				nSerial = pDoc->GetCurrentInfoEngShotNum();
		}
		else if (ChkLastProcFromUp())
		{
			if (IsSetLotEnd())
				nSerial = GetLotEndSerial();
			else
				nSerial = pDoc->m_ListBuf[0].GetLast();
		}
		else
		{
			if (IsSetLotEnd())
				nSerial = GetLotEndSerial();
			else
				nSerial = pDoc->m_ListBuf[1].GetLast();
		}
	}
	else
	{
		if (ChkLastProcFromEng())
		{
			if (IsSetLotEnd())
				nSerial = GetLotEndSerial();
			else
				nSerial = pDoc->GetCurrentInfoEngShotNum();
		}
		else
		{
			if (IsSetLotEnd())
				nSerial = GetLotEndSerial();
			else
				nSerial = pDoc->m_ListBuf[0].GetLast();
		}
	}

	if (!IsSetLotEnd())
	{
		SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
	}
}

void CGvisR2R_PunchView::DoAutoChkShareVsFolder()	// 잔량처리 시 계속적으로 반복해서 이함수가 호출됨으로 좌우 마킹 인덱스 동일 현상 발생.(case AT_LP + 8:)
{
	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = 0;
	CString sNewLot, sMsg;
	int nNewLot = 0;
	BOOL bNewModel = FALSE;

	switch (m_nStepAuto)
	{
	case 0:
		m_bSwRun = FALSE;
		m_bSwStop = TRUE;
		m_nStepAuto++;
		break;
	case 1:
		if (IsReady() || IsAuto())		// 운전준비
		{
			TowerLamp(RGB_YELLOW, TRUE, TRUE);
			m_nStepAuto++;
		}
		break;
	case 2:
		if (IsRun())
			m_nStepAuto++;
		break;
	case 3:
		ClrDispMsg();
		TowerLamp(RGB_YELLOW, TRUE, FALSE);
		m_nStepAuto++;
		break;
	case 4:
		if (IsRun())		// 초기운전
		{
			if (!IsAoiLdRun())
			{
				Stop();
				TowerLamp(RGB_YELLOW, TRUE);
			}
			else
			{
				ResetWinker(); // 운전스위치 램프 동작 문제로 수정.
				TowerLamp(RGB_GREEN, TRUE, TRUE);
				DispMainOnAutoStart();
				m_nVsBufLastSerial[0] = GetVsUpBufLastSerial();
				if (bDualTest)
					m_nVsBufLastSerial[1] = GetVsDnBufLastSerial();

				SetListBuf();

				if (MODE_INNER == pDoc->GetTestMode() || MODE_OUTER == pDoc->GetTestMode() || pDoc->WorkingInfo.System.bUseEngrave) // Please modify for outer mode.-20221226
				{
					GetCurrentInfoEng();
					if (m_pDlgMenu01)
						m_pDlgMenu01->UpdateData();
				}

				m_nStepAuto = AT_LP; pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
			}
		}
		else
			Winker(MN_RUN); // Run Button - 20151126 : 운전스위치 램프 동작 문제로 수정.
		break;

	case AT_LP + (AtLpVsIdx::ChkShare) :
		if (IsShare()) // ChkShare()
		{
			m_bPcrInShare[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bPcrInShare[0]"), m_bPcrInShare[0]);
			m_bPcrInShare[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bPcrInShare[1]"), m_bPcrInShare[1]);

			if (IsShareUp()) // 검사부(상) 검사중
			{
				nSerial = GetShareUp();
				if (nSerial > 0)
				{
					CheckShareUp(nSerial);
				}
			}

			if (bDualTest)
			{
				if (IsShareDn()) // 검사부(하) 검사중
				{
					nSerial = GetShareDn();
					if (nSerial > 0)
					{
						CheckShareDn(nSerial);
					}
				}

				if (m_bPcrInShare[0] || m_bPcrInShare[1])
					m_nStepAuto++;
			}
			else
			{
				if (m_bPcrInShare[0])
					m_nStepAuto++;
			}
		}
		else // if (IsShare())
		{
			if (IsShareVs())
			{
				m_nStepAuto = AT_LP + (AtLpVsIdx::ChkShareVs); pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
			}
		}
		break;

	case AT_LP + (AtLpVsIdx::ChkShare) + 1:
		m_nStepAuto++;
		break;
	case AT_LP + (AtLpVsIdx::ChkShare) + 2:
		m_nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::ChkShare) + 3:
		Shift2DummyBuf();			// PCR 이동(Share->Buffer)
		m_nStepAuto = AT_LP + (AtLpVsIdx::ChkShare); pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
		break;

	case AT_LP + (AtLpVsIdx::ChkShareVs) :
		if (IsShareVs()) // ChkShareVs()
		{
			m_bPcrInShareVs[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bPcrInShareVs[0]"), m_bPcrInShareVs[0]);
			m_bPcrInShareVs[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bPcrInShareVs[1]"), m_bPcrInShareVs[1]);

			if (IsShareVsUp()) // 검사부(상) 검사중
			{
				nSerial = GetShareVsUp();
				if (nSerial > 0)
				{
					CheckShareVsUp(nSerial);
				}
			}

			if (bDualTest)
			{
				if (IsShareVsDn()) // 검사부(하) 검사중
				{
					nSerial = GetShareVsDn();
					if (nSerial > 0)
					{
						CheckShareVsDn(nSerial);
							}
						}
	
				if (m_bPcrInShareVs[0] || m_bPcrInShareVs[1])
					m_nStepAuto++;
			}
			else
			{
				if (m_bPcrInShareVs[0])
					m_nStepAuto++;
			}
		}
		else // if (IsShareVs())
		{
			if (!m_bLastProc)
			{
				if (ChkLastProc())
				{
					TurnLastProcVs();	
					m_nStepAuto++;
				}
			}
			else // if (!m_bLastProc)
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
					}
				}

				m_bWaitPcr[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[0]"), m_bWaitPcr[0]);// 향후 제거해야 할 flag
				m_bWaitPcr[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[1]"), m_bWaitPcr[1]);
				m_nStepAuto++;
			}

			if (MODE_INNER == pDoc->GetTestMode() || pDoc->WorkingInfo.System.bUseEngrave)
			{
				if (IsSetLotEnd())
					nSerial = GetLotEndSerial();
				else
					nSerial = pDoc->GetCurrentInfoEngShotNum();

				if (ChkLastProc())
				{
					if (ChkLastProcFromEng())
					{
						if (!IsSetLotEnd())
							SetLotEnd(nSerial);
					}
				}
			}
		}
		break;

	case AT_LP + (AtLpVsIdx::ChkShareVs) + 1:
		if (!m_bCont) // 이어가기 아닌 경우.
		{
			if (!ChkStShotNum())
			{
				Stop();
				TowerLamp(RGB_YELLOW, TRUE);
			}
		}
		m_nStepAuto++;
		break;
	case AT_LP + (AtLpVsIdx::ChkShareVs) + 2:
		m_bBufEmpty[0] = pDoc->m_bBufEmpty[0]; pDoc->SetStatus(_T("General"), _T("bBufEmpty[0]"), pDoc->m_bBufEmpty[0]); // Up
		m_nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::ChkShareVs) + 3:
		if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_UPDATE_YIELD_UP || m_bTHREAD_UPDATE_YIELD_DN || m_bTHREAD_UPDATE_YIELD_ALLUP || m_bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		Shift2Buf();			// PCR 이동(Share->Buffer)
		m_nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::UpdateReelmap):
		if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_UPDATE_YIELD_UP || m_bTHREAD_UPDATE_YIELD_DN || m_bTHREAD_UPDATE_YIELD_ALLUP || m_bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (m_bTHREAD_MAKE_ITS_FILE_UP) // Write Reelmap
				break;

			if (bDualTest)
			{
				if (m_bTHREAD_MAKE_ITS_FILE_DN)
					break;
			}
		}

		m_nStepAuto++;

		if (m_nShareUpS > 0)
		{
			m_nShareUpCnt++;

			if (pDoc->GetCurrentInfoEng())
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					pDoc->GetItsSerialInfo(m_nShareUpS, bDualTestInner, sLot, sLayerUp, sLayerDn, 0);
				}
			}

			bNewModel = GetAoiUpInfo(m_nShareUpS, &nNewLot); // Buffer에서 PCR파일의 헤드 정보를 얻음.
			if (bNewModel)
			{
				Stop();
				break;
			}

			//if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			if (nNewLot)
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					pDoc->MakeDirRmap(RMAP_INNER_UP);
					pDoc->MakeDirRmap(RMAP_INNER_DN);
					pDoc->CopyReelmapInner(m_nShareUpS);
				}
				
				m_bNewModel = TRUE;
				InitInfo();
				ResetMkInfo(0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn	

				pDoc->GetCamPxlRes();

				if (m_bPcrInShare[0])
				{
					if (IsLastJob(0)) // Up
					{
						pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModel,
							pDoc->WorkingInfo.LastJob.sLayerUp);
						pDoc->m_Master[0].LoadMstInfo();

						if (m_pEngrave)
							m_pEngrave->SwMenu01UpdateWorking(TRUE);


						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
								pDoc->WorkingInfo.LastJob.sModel,
								pDoc->WorkingInfo.LastJob.sInnerLayerUp);
							pDoc->m_MasterInner[0].LoadMstInfo();
						}
					}
				}

				if (m_bPcrInShare[1])
				{
					if (IsLastJob(1)) // Dn
					{
						pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModel,
							pDoc->WorkingInfo.LastJob.sLayerDn,
							pDoc->WorkingInfo.LastJob.sLayerUp);
						pDoc->m_Master[1].LoadMstInfo();

						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
								pDoc->WorkingInfo.LastJob.sModel,
								pDoc->WorkingInfo.LastJob.sInnerLayerDn,
								pDoc->WorkingInfo.LastJob.sInnerLayerUp);
							pDoc->m_MasterInner[0].LoadMstInfo();
						}
					}
				}

				SetAlignPos();

				InitReelmap();

				if (pDoc->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sInnerLayerUp))
					InitReelmapInner();	

				ModelChange(0); // 0 : AOI-Up , 1 : AOI-Dn

				if (m_pDlgMenu01)
				{
					m_pDlgMenu01->InitGL();
					m_bDrawGL_Menu01 = TRUE;
					m_pDlgMenu01->RefreshRmap();
					m_pDlgMenu01->InitCadImg();
					m_pDlgMenu01->SetPnlNum();
					m_pDlgMenu01->SetPnlDefNum();
				}

				if (m_pDlgMenu02)
				{
					m_pDlgMenu02->ChgModelUp(); // PinImg, AlignImg를 Display함.
					m_pDlgMenu02->InitCadImg();
				}

				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (m_pDlgMenu06)
					{
						m_pDlgMenu06->InitGL();
						m_bDrawGL_Menu06 = TRUE;
						m_pDlgMenu06->RefreshRmap();
						m_pDlgMenu06->InitCadImg();
						m_pDlgMenu06->SetPnlNum();
						m_pDlgMenu06->SetPnlDefNum();
					}
				}
			}
			else
			{
				if (m_nShareUpS == 1)
				{
					pDoc->m_nAoiCamInfoStrPcs[0] = GetAoiUpCamMstInfo(); pDoc->SetStatusInt(_T("General"), _T("nAoiCamInfoStrPcs[0]"), pDoc->m_nAoiCamInfoStrPcs[0]);
					if ((pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE) != pDoc->WorkingInfo.System.bStripPcsRgnBin)
					{
						if (pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE)
							MsgBox(_T("현재 마킹부는 일반 모드 인데, \r\n상면 AOI는 DTS 모드에서 검사를 진행하였습니다."));
						else
							MsgBox(_T("현재 마킹부는 DTS 모드 인데, \r\n상면 AOI는 일반 모드에서 검사를 진행하였습니다."));

						Stop();
						TowerLamp(RGB_RED, TRUE);
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (!pDoc->m_bNewLotShare[0])
				{
					pDoc->m_bNewLotShare[0] = TRUE;// Lot Change.
					if (!bDualTest)
						OpenReelmapFromBuf(m_nShareUpS);
				}
			}

			LoadPcrUp(m_nShareUpS);				// Default: From Buffer, TRUE: From Share
			if (!CheckCamMstInfo(RMAP_UP, m_nShareUpS))
			{
				sMsg.Format(_T("Error - 상부AOI에서 생성된 파일이 캠마스터 정보와 다릅니다.\r\n버퍼의 파일을 리셋 후 다시 시작해야 합니다."));
				MsgBox(sMsg);
				ClearBuffer();
				ExitProgram();
				return;
			}

			if (bDualTest)
			{
				if (pDoc->m_ListBuf[0].nTot <= pDoc->m_ListBuf[1].nTot)
				{
					UpdateReelmap(m_nShareUpS); // 시리얼파일의 정보로 릴맵을 만듬
				}
			}
			else
				UpdateReelmap(m_nShareUpS); // 시리얼파일의 정보로 릴맵을 만듬


			if (!m_bLastProc)
			{
				if (ChkLastProc())
				{
					m_nLastProcAuto = LAST_PROC; pDoc->SetStatusInt(_T("General"), _T("nLastProcAuto"), m_nLastProcAuto);
					m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);

					if (bDualTest)
					{
						if (ChkLastProcFromEng())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->GetCurrentInfoEngShotNum();
						}
						else if (ChkLastProcFromUp())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[0].GetLast();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[1].GetLast();
						}
					}
					else
					{
						if (ChkLastProcFromEng())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->GetCurrentInfoEngShotNum();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[0].GetLast();
						}
					}

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
					}
				}
			}
			else
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
					}
				}
			}
		}
		break;

	case AT_LP + (AtLpVsIdx::UpdateReelmap) + 1:
		if (!bDualTest)
		{
			m_nStepAuto++;
			break;
		}

		if (m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_REELMAP_YIELD_DN || m_bTHREAD_REELMAP_YIELD_ALLUP || m_bTHREAD_REELMAP_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		m_nStepAuto++;

		if (IsSetLotEnd())
		{
			if (m_nShareDnS > GetLotEndSerial())
				break;
		}

		if (m_nShareDnS > 0)
		{
			if (m_nShareDnS % 2)
				m_nShareDnSerial[0] = m_nShareDnS; // 홀수
			else
				m_nShareDnSerial[1] = m_nShareDnS; // 짝수
			m_nShareDnCnt++;


			bNewModel = GetAoiDnInfo(m_nShareDnS, &nNewLot);
			if (bNewModel)
			{
				Stop();
				break;
			}

			//if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			if (nNewLot)
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					pDoc->MakeDirRmap(RMAP_INNER_UP);
					pDoc->MakeDirRmap(RMAP_INNER_DN);
					pDoc->CopyReelmapInner(m_nShareDnS);
				}

				if (m_bPcrInShare[0])
				{
					if (IsLastJob(0)) // Up
					{
						pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModel,
							pDoc->WorkingInfo.LastJob.sLayerUp);
						pDoc->m_Master[0].LoadMstInfo();

						if (m_pEngrave)
							m_pEngrave->SwMenu01UpdateWorking(TRUE);


						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
								pDoc->WorkingInfo.LastJob.sModel,
								pDoc->WorkingInfo.LastJob.sInnerLayerUp);
							pDoc->m_MasterInner[0].LoadMstInfo();
						}
					}
				}

				if (m_bPcrInShare[1])
				{
					if (IsLastJob(1)) // Dn
					{
						pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModel,
							pDoc->WorkingInfo.LastJob.sLayerDn,
							pDoc->WorkingInfo.LastJob.sLayerUp);
						pDoc->m_Master[1].LoadMstInfo();

						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
								pDoc->WorkingInfo.LastJob.sModel,
								pDoc->WorkingInfo.LastJob.sInnerLayerDn,
								pDoc->WorkingInfo.LastJob.sInnerLayerUp);
							pDoc->m_MasterInner[0].LoadMstInfo();
						}
					}
				}

				//MsgBox(_T("신규 모델에 의한 AOI(하)에서 로트 분리가 되었습니다.\r\n이전 로트를 잔량처리 합니다.");
				InitInfo();
				ResetMkInfo(1); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn	
				ModelChange(1); // 0 : AOI-Up , 1 : AOI-Dn

			}
			else
			{
				if (m_nShareDnS == 1)
				{
					pDoc->m_nAoiCamInfoStrPcs[1] = GetAoiDnCamMstInfo(); pDoc->SetStatusInt(_T("General"), _T("nAoiCamInfoStrPcs[1]"), pDoc->m_nAoiCamInfoStrPcs[1]);
					if ((pDoc->m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE) != pDoc->WorkingInfo.System.bStripPcsRgnBin)
					{
						if (pDoc->m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE)
							MsgBox(_T("현재 마킹부는 일반 모드 인데, \r\n하면 AOI는 DTS 모드에서 검사를 진행하였습니다."));
						else
							MsgBox(_T("현재 마킹부는 DTS 모드 인데, \r\n하면 AOI는 일반 모드에서 검사를 진행하였습니다."));

						Stop();
						TowerLamp(RGB_RED, TRUE);
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (!pDoc->m_bNewLotShare[1])
				{
					pDoc->m_bNewLotShare[1] = TRUE;// Lot Change.				
					if (bDualTest)
						OpenReelmapFromBuf(m_nShareDnS);
				}
			}

			LoadPcrDn(m_nShareDnS);
			if (!CheckCamMstInfo(RMAP_DN, m_nShareDnS))
			{
				sMsg.Format(_T("Error - 하부AOI에서 생성된 파일이 캠마스터 정보와 다릅니다.\r\n버퍼의 파일을 리셋 후 다시 시작해야 합니다."));
				MsgBox(sMsg);
				ClearBuffer();
				ExitProgram();
				return;
			}

			if (pDoc->m_ListBuf[1].nTot <= pDoc->m_ListBuf[0].nTot)
			{
				UpdateReelmap(m_nShareDnS); // 시리얼파일의 정보로 릴맵을 만듬
			}

			if (!m_bLastProc)
			{
				if (ChkLastProc())
				{
					m_nLastProcAuto = LAST_PROC; pDoc->SetStatusInt(_T("General"), _T("nLastProcAuto"), m_nLastProcAuto);
					m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);

					if (ChkLastProcFromEng())
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = pDoc->GetCurrentInfoEngShotNum();
					}
					else if (ChkLastProcFromUp())
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = pDoc->m_ListBuf[0].GetLast();
					}
					else
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = pDoc->m_ListBuf[1].GetLast();
					}

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
					}				
				}
			}
			else
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
					}
				}
			}
		}
		break;

	case AT_LP + (AtLpVsIdx::UpdateReelmap) + 2:
		m_nStepAuto++;
		if (m_nShareUpS > 0)
		{
			if (pView->m_pDlgFrameHigh)
				pView->m_pDlgFrameHigh->SetAoiLastShot(0, m_nShareUpS);
		}
		if (bDualTest)
		{
			if (m_nShareDnS > 0)
			{
				if (IsSetLotEnd())
				{
					if (m_nShareDnS > GetLotEndSerial())
						break;
				}

				if (pView->m_pDlgFrameHigh)
					pView->m_pDlgFrameHigh->SetAoiLastShot(1, m_nShareDnS);
			}
		}
		break;

	case AT_LP + (AtLpVsIdx::UpdateReelmap) + 3:
		if (bDualTest)
		{
			if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
				break;
		}
		else
		{
			if (m_bTHREAD_UPDATE_REELMAP_UP) // Write Reelmap
				break;
		}
		m_nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::MakeItsFile):
		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (!bDualTest)
			{
				if (m_nShareUpS > 0)
				{
					MakeItsFile(m_nShareUpS); // BOOL CReelMap::MakeItsFile(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
					if (pDoc->GetTestMode() == MODE_OUTER)
						UpdateReelmapInner(m_nShareUpS);
				}
			}
			else
			{
				if (m_nShareUpS > 0)
				{
					if (pDoc->m_ListBuf[0].nTot <= pDoc->m_ListBuf[1].nTot)
					{
						MakeItsFile(m_nShareUpS); // BOOL CReelMap::MakeItsFile(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
						if (pDoc->GetTestMode() == MODE_OUTER)
							UpdateReelmapInner(m_nShareUpS);
					}
				}
				if (m_nShareDnS > 0)
				{
					if (pDoc->m_ListBuf[1].nTot <= pDoc->m_ListBuf[0].nTot)
					{
						MakeItsFile(m_nShareDnS); // BOOL CReelMap::MakeItsFile(int nSerial, int nLayer) // RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
						if (pDoc->GetTestMode() == MODE_OUTER)
							UpdateReelmapInner(m_nShareDnS);
					}
				}
			}
		}

		m_nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::MakeItsFile) + 1:
		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (m_bTHREAD_MAKE_ITS_FILE_UP) // makeIts
				break;

			if (pDoc->WorkingInfo.LastJob.bDualTest)
			{
				if (m_bTHREAD_MAKE_ITS_FILE_DN) // makeIts
					break;
			}
		}
		m_nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::ResetShare):
		m_nShareUpS = 0;
		m_nShareDnS = 0;
		m_nStepAuto++;
		break;

	case AT_LP + (AtLpVsIdx::ResetShare) + 1:
		m_bLoadShare[0] = FALSE;
		m_bLoadShare[1] = FALSE;
		m_nStepAuto = AT_LP + (AtLpVsIdx::ChkShare); pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
		break;


	case LAST_PROC_VS_ALL:			 // 잔량처리 3
		m_nDummy[0] = 0;
		m_nDummy[1] = 0;
		m_bChkLastProcVs = TRUE;
		TowerLamp(RGB_GREEN, TRUE);
		DispMain(_T("상면VS잔량"), RGB_GREEN);
		if (m_nAoiLastSerial[0] < 1)
			m_nAoiLastSerial[0] = GetAoiUpSerial();
		if (!IsSetLotEnd())
			SetLotEnd(m_nAoiLastSerial[0]);
		m_nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 1:
		if (IsVsUp())
			m_nStepAuto++;
		else
		{
			m_nStepAuto = m_nPrevStepAuto; pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
		}
		break;

	case LAST_PROC_VS_ALL + 2:
		m_nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 3:
		m_nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 4:
		m_nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 5:
		m_nStepAuto = m_nPrevStepAuto; pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
		break;		
	}
}

void CGvisR2R_PunchView::DoAutoChkShareFolder()	// 20170727-잔량처리 시 계속적으로 반복해서 이함수가 호출됨으로 좌우 마킹 인덱스 동일 현상 발생.(case AT_LP + 8:)
{
	CString sLot, sLayerUp, sLayerDn;
	BOOL bDualTestInner;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = 0;
	CString sNewLot, sMsg;
	int nNewLot = 0;
	BOOL bNewModel = FALSE;

	switch (m_nStepAuto)
	{
	case 0:
		m_bSwRun = FALSE;
		m_bSwStop = TRUE;
		m_nStepAuto++;
		break;
	case 1:
		if (IsReady() || IsAuto())		// 운전준비
		{
			TowerLamp(RGB_YELLOW, TRUE, TRUE);
			m_nStepAuto++;
		}
		break;
	case 2:
		if (IsRun())
			m_nStepAuto++;
		break;
	case 3:
		ClrDispMsg();
		TowerLamp(RGB_YELLOW, TRUE, FALSE);
		m_nStepAuto++;
		break;
	case 4:
		if (IsRun())		// 초기운전
		{
			if (!IsAoiLdRun())
			{
				Stop();
				TowerLamp(RGB_YELLOW, TRUE);
			}
			else
			{
				ResetWinker(); // 20151126 : 운전스위치 램프 동작 문제로 수정.
				TowerLamp(RGB_GREEN, TRUE, TRUE);
				DispMainOnAutoStart();
				m_nVsBufLastSerial[0] = GetVsUpBufLastSerial();
				if (bDualTest)
					m_nVsBufLastSerial[1] = GetVsDnBufLastSerial();

				SetListBuf();

				if (MODE_INNER == pDoc->GetTestMode() || MODE_OUTER == pDoc->GetTestMode() || pDoc->WorkingInfo.System.bUseEngrave) // Please modify for outer mode.-20221226
				{
					GetCurrentInfoEng();
					if (m_pDlgMenu01)
						m_pDlgMenu01->UpdateData();
				}

				
				m_nStepAuto = AT_LP; pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
			}
		}
		else
			Winker(MN_RUN); // Run Button - 20151126 : 운전스위치 램프 동작 문제로 수정.
		break;

	case AT_LP:
		if (IsShare()) // ChkShare()
		{
			m_bPcrInShare[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bPcrInShare[0]"), m_bPcrInShare[0]);
			m_bPcrInShare[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bPcrInShare[1]"), m_bPcrInShare[1]);

			if (IsShareUp())
			{
				nSerial = GetShareUp();
				if (nSerial > 0)
				{
					CheckShareUp(nSerial);
				}
				else
				{
					m_bLoadShare[0] = FALSE;
				}
			}
			else
				m_bLoadShare[0] = FALSE;


			if (bDualTest)
			{
				if (IsShareDn())
				{
					nSerial = GetShareDn();
					if (nSerial > 0)
					{
						CheckShareDn(nSerial);
					}
					else
					{
						m_bLoadShare[1] = FALSE;
					}
				}
				else
					m_bLoadShare[1] = FALSE;

				if (m_bPcrInShare[0] || m_bPcrInShare[1])
					m_nStepAuto++;
			}
			else
			{
				if (m_bPcrInShare[0])
					m_nStepAuto++;
			}
		}
		else // if (IsShare())
		{
			if (!m_bLastProc)
			{
				if (ChkLastProc())
				{
					TurnLastProc();
					m_nStepAuto++;
				}
			}
			else
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
					}
				}

				m_bWaitPcr[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[0]"), m_bWaitPcr[0]);
				m_bWaitPcr[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bWaitPcr[1]"), m_bWaitPcr[1]);
				m_nStepAuto++;
			}
			
			if (MODE_INNER == pDoc->GetTestMode() || pDoc->WorkingInfo.System.bUseEngrave)
			{
				if (IsSetLotEnd())
					nSerial = GetLotEndSerial();
				else
					nSerial = pDoc->GetCurrentInfoEngShotNum();

				if (ChkLastProc())
				{
					if (ChkLastProcFromEng())
					{
						if (!IsSetLotEnd())
							SetLotEnd(nSerial);
					}
				}
			}

		}
		break;

	case AT_LP + 1:
		if (!m_bCont) // 이어가기 아닌 경우.
		{
			if (!ChkStShotNum())
			{
				Stop();
				TowerLamp(RGB_YELLOW, TRUE);
			}
		}

		m_nStepAuto++;
		break;
	case AT_LP + 2:
		if (IsRun())
		{
			m_bBufEmpty[0] = pDoc->m_bBufEmpty[0]; pDoc->SetStatus(_T("General"), _T("bBufEmpty[0]"), pDoc->m_bBufEmpty[0]); // Up
			m_nStepAuto++;
		}
		break;

	case AT_LP + 3:
		if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_UPDATE_YIELD_UP || m_bTHREAD_UPDATE_YIELD_DN || m_bTHREAD_UPDATE_YIELD_ALLUP || m_bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		Shift2Buf();			// PCR 이동(Share->Buffer)
		m_nStepAuto++;
		break;

	case AT_LP + 4:
		if (!IsRun())
			break;

		if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_UPDATE_YIELD_UP || m_bTHREAD_UPDATE_YIELD_DN || m_bTHREAD_UPDATE_YIELD_ALLUP || m_bTHREAD_UPDATE_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (m_bTHREAD_MAKE_ITS_FILE_UP)
				break;

			if (bDualTest)
			{
				if (m_bTHREAD_MAKE_ITS_FILE_DN)
					break;
			}
		}

		if (!bDualTest)
		{
			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_INNER_UP) // Write Reelmap
					break;

				if (pDoc->WorkingInfo.LastJob.bDualTestInner)
				{
					if (m_bTHREAD_UPDATE_REELMAP_INNER_DN || m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP || m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN) // Write Reelmap
						break;
				}
			}
		}

		m_nStepAuto++;

		if (m_nShareUpS > 0)
		{
			m_nShareUpCnt++;

			if (pDoc->GetCurrentInfoEng())
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
					pDoc->GetItsSerialInfo(m_nShareUpS, bDualTestInner, sLot, sLayerUp, sLayerDn, 0);
			}

			bNewModel = GetAoiUpInfo(m_nShareUpS, &nNewLot); // Buffer에서 PCR파일의 헤드 정보를 얻음.
			if (bNewModel)
			{
				Stop();
				break;
			}

			//if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			if (nNewLot)
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					pDoc->MakeDirRmap(RMAP_INNER_UP);
					pDoc->MakeDirRmap(RMAP_INNER_DN);
					pDoc->CopyReelmapInner(m_nShareUpS);
				}

				m_bNewModel = TRUE;
				InitInfo();
				ResetMkInfo(0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn	

				pDoc->GetCamPxlRes();

				if (m_bPcrInShare[0])
				{
					if (IsLastJob(0)) // Up
					{
						pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModel,
							pDoc->WorkingInfo.LastJob.sLayerUp);
						pDoc->m_Master[0].LoadMstInfo();


						if (m_pEngrave)
							m_pEngrave->SwMenu01UpdateWorking(TRUE);


						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
								pDoc->WorkingInfo.LastJob.sModel,
								pDoc->WorkingInfo.LastJob.sInnerLayerUp);
							pDoc->m_MasterInner[0].LoadMstInfo();
						}
					}
				}

				if (m_bPcrInShare[1])
				{
					if (IsLastJob(1)) // Dn
					{
						pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModel,
							pDoc->WorkingInfo.LastJob.sLayerDn,
							pDoc->WorkingInfo.LastJob.sLayerUp);
						pDoc->m_Master[1].LoadMstInfo();

						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
								pDoc->WorkingInfo.LastJob.sModel,
								pDoc->WorkingInfo.LastJob.sInnerLayerDn,
								pDoc->WorkingInfo.LastJob.sInnerLayerUp);
							pDoc->m_MasterInner[0].LoadMstInfo();
						}
					}

				}
				SetAlignPos();
				InitReelmap();

				if (pDoc->m_Master[0].IsMstSpec(pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sInnerLayerUp))
					InitReelmapInner();

				ModelChange(0); // 0 : AOI-Up , 1 : AOI-Dn

				if (m_pDlgMenu01)
				{
					m_pDlgMenu01->InitGL();
					m_bDrawGL_Menu01 = TRUE;
					m_pDlgMenu01->RefreshRmap();
					m_pDlgMenu01->InitCadImg();
					m_pDlgMenu01->SetPnlNum();
					m_pDlgMenu01->SetPnlDefNum();
				}

				if (m_pDlgMenu02)
				{
					m_pDlgMenu02->ChgModelUp(); // PinImg, AlignImg를 Display함.
					m_pDlgMenu02->InitCadImg();
				}

				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (m_pDlgMenu06)
					{
						m_pDlgMenu06->InitGL();
						m_bDrawGL_Menu06 = TRUE;
						m_pDlgMenu06->RefreshRmap();
						m_pDlgMenu06->InitCadImg();
						m_pDlgMenu06->SetPnlNum();
						m_pDlgMenu06->SetPnlDefNum();
					}
				}

			}
			else
			{
				if (m_nShareUpS == 1)
				{
					pDoc->m_nAoiCamInfoStrPcs[0] = GetAoiUpCamMstInfo(); pDoc->SetStatusInt(_T("General"), _T("nAoiCamInfoStrPcs[0]"), pDoc->m_nAoiCamInfoStrPcs[0]);
					if ((pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE) != pDoc->WorkingInfo.System.bStripPcsRgnBin)
					{
						if (pDoc->m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE)
							MsgBox(_T("현재 마킹부는 일반 모드 인데, \r\n상면 AOI는 DTS 모드에서 검사를 진행하였습니다."));
						else
							MsgBox(_T("현재 마킹부는 DTS 모드 인데, \r\n상면 AOI는 일반 모드에서 검사를 진행하였습니다."));

						Stop();
						TowerLamp(RGB_RED, TRUE);
						break;

					}
				}
			}
			
			if (nNewLot)
			{
				if (!pDoc->m_bNewLotShare[0])
				{
					pDoc->m_bNewLotShare[0] = TRUE;// Lot Change.
					if (!bDualTest)
						OpenReelmapFromBuf(m_nShareUpS);
				}
			}
			
			LoadPcrUp(m_nShareUpS);				// Default: From Buffer, TRUE: From Share
			if (!CheckCamMstInfo(RMAP_UP, m_nShareUpS))
			{
				sMsg.Format(_T("Error - 상부AOI에서 생성된 파일이 캠마스터 정보와 다릅니다.\r\n버퍼의 파일을 리셋 후 다시 시작해야 합니다."));
				MsgBox(sMsg);
				ClearBuffer();
				ExitProgram();
				return;
			}

			if (bDualTest)
			{
				if (pDoc->m_ListBuf[0].nTot <= pDoc->m_ListBuf[1].nTot)
				{
					UpdateReelmap(m_nShareUpS); // 시리얼파일의 정보로 릴맵을 만듬
				}
			}
			else
				UpdateReelmap(m_nShareUpS); // 시리얼파일의 정보로 릴맵을 만듬

			if (!m_bLastProc)
			{
				if (ChkLastProc())
				{
					m_nLastProcAuto = LAST_PROC; pDoc->SetStatusInt(_T("General"), _T("nLastProcAuto"), m_nLastProcAuto);
					m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);

					if (bDualTest)
					{
						if (ChkLastProcFromEng())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->GetCurrentInfoEngShotNum();
						}
						else if (ChkLastProcFromUp())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[0].GetLast();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[1].GetLast();
						}
					}
					else
					{
						if (ChkLastProcFromEng())
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->GetCurrentInfoEngShotNum();
						}
						else
						{
							if (IsSetLotEnd())
								nSerial = GetLotEndSerial();
							else
								nSerial = pDoc->m_ListBuf[0].GetLast();
						}
					}

					if (!IsSetLotEnd()) // 20160810
					{
						SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
					}
				}
			}
			else
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
					}
				}
			}
		}
		break;

	case AT_LP + 5:
		if (!IsRun())
			break;

		if (!bDualTest)
		{
			m_nStepAuto++;
			break;
		}

		if (m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		if (m_bTHREAD_REELMAP_YIELD_DN || m_bTHREAD_REELMAP_YIELD_ALLUP || m_bTHREAD_REELMAP_YIELD_ALLDN) // Write Reelmap
		{
			Sleep(100);
			break;
		}

		m_nStepAuto++;
		if (IsSetLotEnd())
		{
			if (m_nShareDnS > GetLotEndSerial())
				break;
		}

		if (m_nShareDnS > 0)
		{
			if (m_nShareDnS % 2)
				m_nShareDnSerial[0] = m_nShareDnS; // 홀수
			else
				m_nShareDnSerial[1] = m_nShareDnS; // 짝수
			m_nShareDnCnt++;


			bNewModel = GetAoiDnInfo(m_nShareDnS, &nNewLot);
			if (bNewModel)
			{
				Stop();
				break;
			}

			//if (bNewModel)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
			if (nNewLot)
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					pDoc->MakeDirRmap(RMAP_INNER_UP);
					pDoc->MakeDirRmap(RMAP_INNER_DN);
					pDoc->CopyReelmapInner(m_nShareDnS);
				}

				if (m_bPcrInShare[0])
				{
					if (IsLastJob(0)) // Up
					{
						pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModel,
							pDoc->WorkingInfo.LastJob.sLayerUp);
						pDoc->m_Master[0].LoadMstInfo();


						if (m_pEngrave)
							m_pEngrave->SwMenu01UpdateWorking(TRUE);


						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
								pDoc->WorkingInfo.LastJob.sModel,
								pDoc->WorkingInfo.LastJob.sInnerLayerUp);
							pDoc->m_MasterInner[0].LoadMstInfo();
						}
					}
				}

				if (m_bPcrInShare[1])
				{
					if (IsLastJob(1)) // Dn
					{
						pDoc->m_Master[1].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
							pDoc->WorkingInfo.LastJob.sModel,
							pDoc->WorkingInfo.LastJob.sLayerDn,
							pDoc->WorkingInfo.LastJob.sLayerUp);
						pDoc->m_Master[1].LoadMstInfo();

						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							pDoc->m_MasterInner[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
								pDoc->WorkingInfo.LastJob.sModel,
								pDoc->WorkingInfo.LastJob.sInnerLayerDn,
								pDoc->WorkingInfo.LastJob.sInnerLayerUp);
							pDoc->m_MasterInner[0].LoadMstInfo();
						}
					}

				}

				InitInfo();
				ResetMkInfo(1); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn	
				ModelChange(1); // 0 : AOI-Up , 1 : AOI-Dn

			}
			else
			{
				if (m_nShareDnS == 1)
				{
					pDoc->m_nAoiCamInfoStrPcs[1] = GetAoiDnCamMstInfo(); pDoc->SetStatusInt(_T("General"), _T("nAoiCamInfoStrPcs[1]"), pDoc->m_nAoiCamInfoStrPcs[1]);
					if ((pDoc->m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE) != pDoc->WorkingInfo.System.bStripPcsRgnBin)
					{
						if (pDoc->m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE)
							MsgBox(_T("현재 마킹부는 일반 모드 인데, \r\n하면 AOI는 DTS 모드에서 검사를 진행하였습니다."));
						else
							MsgBox(_T("현재 마킹부는 DTS 모드 인데, \r\n하면 AOI는 일반 모드에서 검사를 진행하였습니다."));

						Stop();
						TowerLamp(RGB_RED, TRUE);
						break;
					}
				}
			}

			if (nNewLot)
			{
				if (!pDoc->m_bNewLotShare[1])
				{
					pDoc->m_bNewLotShare[1] = TRUE;// Lot Change.				
					if (bDualTest)
						OpenReelmapFromBuf(m_nShareDnS);
				}
			}

			LoadPcrDn(m_nShareDnS);
			if (!CheckCamMstInfo(RMAP_DN, m_nShareDnS))
			{
				sMsg.Format(_T("Error - 하부AOI에서 생성된 파일이 캠마스터 정보와 다릅니다.\r\n버퍼의 파일을 리셋 후 다시 시작해야 합니다."));
				MsgBox(sMsg);
				ClearBuffer();
				ExitProgram();
				return;
			}

			if (pDoc->m_ListBuf[1].nTot <= pDoc->m_ListBuf[0].nTot)
			{
				UpdateReelmap(m_nShareDnS); // 시리얼파일의 정보로 릴맵을 만듬
			}

			if (!m_bLastProc)
			{
				if (ChkLastProc())
				{
					m_nLastProcAuto = LAST_PROC; pDoc->SetStatusInt(_T("General"), _T("nLastProcAuto"), m_nLastProcAuto);
					m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);

					if (ChkLastProcFromEng())
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = pDoc->GetCurrentInfoEngShotNum();
					}
					else if (ChkLastProcFromUp())
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = pDoc->m_ListBuf[0].GetLast();
					}
					else
					{
						if (IsSetLotEnd())
							nSerial = GetLotEndSerial();
						else
							nSerial = pDoc->m_ListBuf[1].GetLast();
					}

					if (!IsSetLotEnd()) // 20160810
					{
						SetLotEnd(nSerial);//+pDoc->AoiDummyShot[1]); // 3
					}
				}
			}
			else
			{
				if (ChkLastProcFromEng())
				{
					if (IsSetLotEnd())
						nSerial = GetLotEndSerial();
					else
						nSerial = pDoc->GetCurrentInfoEngShotNum();

					if (!IsSetLotEnd())
					{
						SetLotEnd(nSerial);
					}
				}
			}
		}

		break;

	case AT_LP + 6:
		m_nStepAuto++;
		if (m_nShareUpS > 0)
		{
			if (pView->m_pDlgFrameHigh)
				pView->m_pDlgFrameHigh->SetAoiLastShot(0, m_nShareUpS);
		}
		if (bDualTest)
		{
			if (m_nShareDnS > 0)
			{
				if (IsSetLotEnd())
				{
					if (m_nShareDnS > GetLotEndSerial())
						break;
				}

				if (pView->m_pDlgFrameHigh)
					pView->m_pDlgFrameHigh->SetAoiLastShot(1, m_nShareDnS);
			}
		}
		break;

	case AT_LP + 7:
		if (IsRun())
		{
			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (m_bTHREAD_UPDATE_REELMAP_INNER_UP) // Write Reelmap
					break;
				if (pDoc->WorkingInfo.LastJob.bDualTestInner)
				{
					if (m_bTHREAD_UPDATE_REELMAP_INNER_DN || m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP || m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN) // Write Reelmap
						break;
				}
			}

			if (bDualTest)
			{
				if (m_bTHREAD_UPDATE_REELMAP_UP || m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
					break;
			}
			else
			{
				if (m_bTHREAD_UPDATE_REELMAP_UP) // Write Reelmap
					break;
			}
			m_nStepAuto++;
		}
		break;


	case AT_LP + 8:
		if (IsRun())
		{
			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (!bDualTest)
				{
					if (m_nShareUpS > 0)
					{
						MakeItsFile(m_nShareUpS);
						if (pDoc->GetTestMode() == MODE_OUTER)
							UpdateReelmapInner(m_nShareUpS);
					}
				}
				else
				{
					if (m_nShareDnS > 0)
					{
						MakeItsFile(m_nShareDnS);
						if (pDoc->GetTestMode() == MODE_OUTER)
							UpdateReelmapInner(m_nShareDnS);
					}
				}
			}

			m_nStepAuto++;
		}
		break;

	case AT_LP + 9:
		if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		{
			if (m_bTHREAD_MAKE_ITS_FILE_UP) // makeIts
				break;

			if (pDoc->WorkingInfo.LastJob.bDualTest)
			{
				if (m_bTHREAD_MAKE_ITS_FILE_DN) // makeIts
					break;
			}

			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				if (pDoc->WorkingInfo.LastJob.bDualTestInner)
				{
					if (m_bTHREAD_UPDATE_REELMAP_INNER_DN || m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP || m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN) // Write Reelmap
						break;
				}
			}
			else
			{
				if (pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (m_bTHREAD_UPDATE_REELMAP_DN || m_bTHREAD_UPDATE_REELMAP_ALLUP || m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
						break;
				}
			}
		}
		m_nStepAuto++;
		break;

	case AT_LP + 10:
		m_nShareUpS = 0;
		m_nShareDnS = 0;
		m_nStepAuto++;
		break;

	case AT_LP + 11:
		m_bLoadShare[0] = FALSE;
		m_bLoadShare[1] = FALSE;
		m_nStepAuto = AT_LP; pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
		break;


	case LAST_PROC_VS_ALL:			 // 잔량처리 3
		m_nDummy[0] = 0;
		m_nDummy[1] = 0;
		m_bChkLastProcVs = TRUE;
		TowerLamp(RGB_GREEN, TRUE);
		DispMain(_T("상면VS잔량"), RGB_GREEN);
		if (m_nAoiLastSerial[0] < 1)
			m_nAoiLastSerial[0] = GetAoiUpSerial();
		if (!IsSetLotEnd())
			SetLotEnd(m_nAoiLastSerial[0]);
		m_nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 1:
		if (IsVsUp())
			m_nStepAuto++;
		else
		{
			m_nStepAuto = m_nPrevStepAuto; pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
		}
		break;

	case LAST_PROC_VS_ALL + 2:
		m_nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 3:
		m_nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 4:
		m_nStepAuto++;
		break;

	case LAST_PROC_VS_ALL + 5:
		m_nStepAuto = m_nPrevStepAuto; pDoc->SetStatusInt(_T("General"), _T("nStepAuto"), m_nStepAuto);
		break;
	}
}

void CGvisR2R_PunchView::DoAutoMarking()
{
	if(pDoc->WorkingInfo.LastJob.nAlignMethode == TWO_POINT)
		MarkingWith2PointAlign();
	else if(pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT)
		MarkingWith4PointAlign();
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("마킹을 위한 Align방식이 정해지지 않았습니다."));
	}
}

void CGvisR2R_PunchView::Mk2PtReady()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sMsg;

	if ( (m_bMkSt[0] || m_bMkSt[1]) && IsBuffer() )
	{
		switch (m_nMkStAuto)
		{
		case MK_ST:	// PLC MK 신호 확인	
			if (IsRun())
			{
				pDoc->LogAuto(_T("PC: 마킹부 마킹중 ON (PC가 ON, OFF)"));
				MpeWrite(Plc.DlgMenu01.MarkingDoing, 1);// 마킹부 마킹중 ON (PC가 ON, OFF)
#ifdef USE_SR1000W
				if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
				{
					if (m_pSr1000w && m_pSr1000w->IsConnected())
					{
						m_sGet2dCodeLot = _T("");
						m_nGet2dCodeSerial = 0;
						//Set2dRead(TRUE);
					}
					else
					{
						Stop();
						pView->ClrDispMsg();
						sMsg.Format(_T("2D 리더기가 연결이 되지 않았습니다."));
						AfxMessageBox(sMsg);
						m_nMkStAuto = MK_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
						break;
					}
				}
#endif
				m_nMkStAuto++;
			}
			break;
		case MK_ST + 1:
			if (!m_bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				if (bDualTest)
				{
					if ((pDoc->m_ListBuf[0].nTot > 0) && (pDoc->m_ListBuf[1].nTot > 0)) // Buf Up/Dn 에 파일이 있으면 다음으로 진행.
					{
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
					else
					{
						if (!GetAoiUpVsStatus())
						{
							m_bLotEnd = TRUE;
							m_nLotEndAuto = LOT_END;
						}
					}
				}
				else
				{
					if (pDoc->m_ListBuf[0].nTot > 0) // Buf Up 에 파일이 있으면 다음으로 진행.
					{
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
					else
					{
						if (!GetAoiUpVsStatus())
						{
							m_bLotEnd = TRUE;
							m_nLotEndAuto = LOT_END;
						}
					}
				}
			}
			break;
		case MK_ST + (Mk2PtIdx::Start) :	// 2
			if (!IsRun())
				break;

			if (bDualTest)
			{
				if (pDoc->m_ListBuf[1].nTot > 0) // AOI-Dn
				{
					if (m_bMkSt[0] || m_bMkSt[1]) // Left
					{
						// Serial - Left
						m_nBufUpSerial[0] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[0]"), m_nBufUpSerial[0]);
						m_nBufDnSerial[0] = pDoc->m_ListBuf[1].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[0]"), m_nBufDnSerial[0]);
						//m_nBufUpSerial[0] = m_nBufDnSerial[0];
					}
					else
					{
						Stop();
						pView->SetAlarmToPlc(UNIT_PUNCH);
						pView->ClrDispMsg();
						AfxMessageBox(_T("좌측 마킹시작신호가 꺼져있습니다."));
						break;
					}

					m_nMkStAuto++;

					if (m_bMkSt[1]) // Right
					{
						if (pDoc->m_ListBuf[1].nTot > 0) // AOI-Dn
						{
							// Serial - Right
							m_nBufUpSerial[1] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufDnSerial[1]);
							m_nBufDnSerial[1] = pDoc->m_ListBuf[1].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[1]"), m_nBufDnSerial[1]);
							//m_nBufUpSerial[1] = m_nBufDnSerial[1]; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
						}
						else
						{
							m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
							m_nBufDnSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[1]"), m_nBufDnSerial[1]);
						}
					}
					else
					{
						m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
						m_nBufDnSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[1]"), m_nBufDnSerial[1]);
					}
				}
				else
				{
					if (!pDoc->m_bVsStatusUp)
					{
						m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
						m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
					}
				}

				//if (pDoc->WorkingInfo.LastJob.bSampleTest)
				//{
				//	if (m_nBufUpSerial[0] == 1)
				//	{
				//		m_nLotEndSerial = _tstoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum); pDoc->SetStatusInt(_T("General"), _T("nLotEndSerial"), m_nLotEndSerial);
				//		m_bLastProcFromUp = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProcFromUp"), m_bLastProcFromUp);
				//		m_bLastProcFromEng = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
				//		m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
				//		if (m_pDlgMenu01)
				//			m_pDlgMenu01->m_bLastProc = TRUE;
				//		if (m_pMpe)
				//		{
				//			//MpeWrite(Plc.DlgMenu01.LastJobFromAoiDn, 1);	// 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
				//			MpeWrite(Plc.DlgMenu01.DoLastJob, 1);			// 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
				//			pDoc->LogAuto(_T("PC: 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)"));
				//		}
				//	}
				//}
			}
			else
			{
				if (pDoc->m_ListBuf[0].nTot > 0) // AOI-Up
				{
					if (m_bMkSt[0] || m_bMkSt[1]) // Left
					{
						// Serial - Left
						m_nBufUpSerial[0] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[0]"), m_nBufUpSerial[0]);
					}
					else
					{
						Stop();
						pView->SetAlarmToPlc(UNIT_PUNCH);
						pView->ClrDispMsg();
						AfxMessageBox(_T("좌측 마킹시작신호가 꺼져있습니다."));
						break;
					}

					m_nMkStAuto++;

					if (m_bMkSt[1]) // Right
					{
						if (pDoc->m_ListBuf[0].nTot > 0) // AOI-Up
						{
							// Serial - Right
							m_nBufUpSerial[1] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
						}
						else
						{
							m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
						}
					}
					else
					{
						m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
					}
				}
				else
				{
					if (!pDoc->m_bVsStatusUp)
					{
						m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
						m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
					}
				}

				//if (pDoc->WorkingInfo.LastJob.bSampleTest)
				//{
				//	if (m_nBufUpSerial[0] == 1)
				//	{
				//		m_nLotEndSerial = _tstoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum); pDoc->SetStatusInt(_T("General"), _T("nLotEndSerial"), m_nLotEndSerial);
				//		m_bLastProcFromUp = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProcFromUp"), m_bLastProcFromUp);
				//		m_bLastProcFromEng = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
				//		m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
				//		if (m_pDlgMenu01)
				//			m_pDlgMenu01->m_bLastProc = TRUE;
				//		if (m_pMpe)
				//		{
				//			//MpeWrite(Plc.DlgMenu01.LastJobFromAoiDn, 1);			// 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
				//			MpeWrite(Plc.DlgMenu01.DoLastJob, 1);			// 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
				//			pDoc->LogAuto(_T("PC: 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)"));
				//		}
				//	}
				//}
			}
			break;
		case MK_ST + (Mk2PtIdx::Start) + 1:
			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN && !m_bTHREAD_MAKE_ITS_FILE_UP && !m_bTHREAD_MAKE_ITS_FILE_DN && !m_bTHREAD_UPDATE_REELMAP_INNER_UP && !m_bTHREAD_UPDATE_REELMAP_INNER_DN && !m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP && !m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN)
				{
					m_nMkStAuto = MK_ST + (Mk2PtIdx::ChkSn); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				}
			}
			else
			{
				if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
				{
					m_nMkStAuto = MK_ST + (Mk2PtIdx::ChkSn); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				}
			}
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtChkSerial()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sNewLot;
	BOOL bNewModel = FALSE;
	int nSerial = 0;
	int nNewLot = 0;
	int nLastShot = 0;
	double dFdEnc;
	CString sLot, sMsg;

	if (m_bTHREAD_SHIFT2MK || !IsRun())
		return;

	if ( (m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::ChkSn) :
			if (m_nBufUpSerial[0] == m_nBufUpSerial[1])
			{
				Stop();
				pView->ClrDispMsg();
				MsgBox(_T("좌/우 마킹 시리얼이 같습니다."));
				SetListBuf();
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				break;
			}

			if (bDualTest)
			{
				if (m_nBufUpSerial[0] != m_nBufDnSerial[0])
				{
					Stop();
					pView->ClrDispMsg();
					MsgBox(_T("좌측 상/하면 시리얼이 다릅니다."));
					SetListBuf();
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					break;
				}

				if (m_nBufUpSerial[1] != m_nBufDnSerial[1])
				{
					Stop();
					pView->ClrDispMsg();
					MsgBox(_T("우측 상/하면 시리얼이 다릅니다."));
					SetListBuf();
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					break;
				}
			}

			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN && !m_bTHREAD_MAKE_ITS_FILE_UP && !m_bTHREAD_MAKE_ITS_FILE_DN && !m_bTHREAD_UPDATE_REELMAP_INNER_UP && !m_bTHREAD_UPDATE_REELMAP_INNER_DN && !m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP && !m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN)
				{
					if (!m_bTHREAD_DISP_DEF && !m_bTHREAD_DISP_DEF_INNER)
					{
						m_nMkStAuto++;
						m_nStepTHREAD_DISP_DEF = 0; pDoc->SetStatusInt(_T("Thread"), _T("nStepTHREAD_DISP_DEF"), m_nStepTHREAD_DISP_DEF);
						m_nStepTHREAD_DISP_DEF_INNER = 0; pDoc->SetStatusInt(_T("Thread"), _T("nStepTHREAD_DISP_DEF_INNER"), m_nStepTHREAD_DISP_DEF_INNER);

						m_bTHREAD_DISP_DEF = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_DISP_DEF"), m_bTHREAD_DISP_DEF);				// DispDefImg() : CopyDefImg Start -> Disp Reelmap Start
						if (pDoc->GetTestMode() == MODE_OUTER)
							m_bTHREAD_DISP_DEF_INNER = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_DISP_DEF_INNER"), m_bTHREAD_DISP_DEF_INNER); // DispDefImgInner() : Disp Reelmap Start

						pDoc->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("1"));
					}
				}
			}
			else
			{
				if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
				{
					if (!m_bTHREAD_DISP_DEF)
					{
						m_nMkStAuto++;
						m_nStepTHREAD_DISP_DEF = 0; pDoc->SetStatusInt(_T("Thread"), _T("nStepTHREAD_DISP_DEF"), m_nStepTHREAD_DISP_DEF);

						m_bTHREAD_DISP_DEF = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_DISP_DEF"), m_bTHREAD_DISP_DEF);		// DispDefImg() : CopyDefImg Start -> Disp Reelmap Start
						pDoc->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("1"));
					}
				}
			}

			break;

		case MK_ST + (Mk2PtIdx::ChkSn) + 1:
			m_nMkStAuto++;
			nSerial = m_nBufUpSerial[0]; // Left
			sNewLot = m_sNewLotUp;

			if (nSerial > 0)
			{
				if (pView->m_bSerialDecrese)
				{
					if ( m_bLastProc && (m_nBufUpSerial[1] < m_nLotEndSerial || m_nBufUpSerial[1] == 0) ) // Right
						nLastShot = m_nBufUpSerial[0];
				}
				else
				{
					if ( m_bLastProc && (m_nBufUpSerial[1] > m_nLotEndSerial || m_nBufUpSerial[1] == 0) ) // Right
					{
						nLastShot = m_nBufUpSerial[0];
						//nSerial = m_nBufUpSerial[0];
					}
				}

				bNewModel = GetAoiUpInfo(nSerial, &nNewLot, TRUE);
				if (bDualTest)
				{
					bNewModel = GetAoiDnInfo(nSerial, &nNewLot, TRUE);

					if (!IsSameUpDnLot() && !m_bContDiffLot)
					{
						m_nMkStAuto = MK_ST + (Mk2PtIdx::LotDiff); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
				}

				if (bNewModel)
				{
					Stop();
					break;
				}
				if (nNewLot)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
				{
					// Lot Change.
					ChgLot();
#ifdef USE_MPE
					if(pDoc->m_pMpeData)
						dFdEnc = (double)pDoc->m_pMpeData[2][2];	// 마킹부 Feeding 엔코더 값(단위 mm )
					if ((pDoc->WorkingInfo.LastJob.bLotSep || pDoc->m_bDoneChgLot) && (dFdEnc + _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen)*2.0) > _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen)*1000.0)
					{
						pDoc->m_bDoneChgLot = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneChgLot"), pDoc->m_bDoneChgLot);
						SetPathAtBuf();
					}
#endif
				}
			}
			else
			{
				Stop();
				MsgBox(_T("버퍼(좌) Serial이 1 이하입니다."));
				TowerLamp(RGB_YELLOW, TRUE);
			}
			break;

		case MK_ST + (Mk2PtIdx::ChkSn) + 2:

#ifdef USE_SR1000W
			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (m_pSr1000w && m_pSr1000w->IsConnected())
				{
					if (m_sGet2dCodeLot != _T("") && m_nGet2dCodeSerial != 0)
					{
						if (m_nBufUpSerial[0] == m_nGet2dCodeSerial)
						{
							m_nMkStAuto = MK_ST + (Mk2PtIdx::InitMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);			// InitMk()
						}
						else
						{
							Stop();
							pView->ClrDispMsg();
							sMsg.Format(_T("2D각인 시리얼(%d)과 검사파일 시리얼(%d)이 다릅니다."), m_nGet2dCodeSerial, m_nBufUpSerial[0]);
							AfxMessageBox(sMsg);
							m_nMkStAuto = MK_ST + (Mk2PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
							break;
						}
					}
					else
						m_nMkStAuto = MK_ST + (Mk2PtIdx::InitMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);			// InitMk()

				}
				else
				{
					Stop();
					pView->ClrDispMsg();
					sMsg.Format(_T("2D 리더기가 연결이 되지 않았습니다."));
					AfxMessageBox(sMsg);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					break;
				}
			}
			else
				m_nMkStAuto = MK_ST + (Mk2PtIdx::InitMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);					// InitMk()
#else
			m_nMkStAuto = MK_ST + (Mk2PtIdx::InitMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);					// InitMk()
#endif
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtInit()
{
	if (m_bTHREAD_SHIFT2MK)
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if ((m_bMkSt[0] || m_bMkSt[1] ) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::InitMk) :
			if (InitMk()) // 시리얼이 증가 또는 감소 : TRUE 아니고 같으면 : FALSE
			{
				if (pView->m_bSerialDecrese)
				{
					if ((m_nBufUpSerial[0] < m_nLotEndSerial/* || m_nBufUpSerial[1] < m_nLotEndSerial*/) && m_nLotEndSerial > 0) // Left
					{
						//MpeWrite(Plc.DlgMenu01.MarkingInitDone, 1); // 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off)
						pDoc->LogAuto(_T("PC: 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off)"));
					}
				}
				else
				{
					if ((m_nBufUpSerial[0] > m_nLotEndSerial/* || m_nBufUpSerial[1] >= m_nLotEndSerial*/) && m_nLotEndSerial > 0)
					{
						//MpeWrite(Plc.DlgMenu01.MarkingInitDone, 1); // 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off) - 20160718
						pDoc->LogAuto(_T("PC: 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off)"));
					}
				}
			}
			else // Same Serial
			{
				Stop();
				MsgBox(_T("Serial 연속 되지않습니다."));
				TowerLamp(RGB_YELLOW, TRUE);
			}
			m_nMkStAuto++;
			break;

		case MK_ST + (Mk2PtIdx::InitMk) + 1:
			if (IsRun())
			{
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Move - Cam1 - Pt0
			}
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtAlignPt0()
{
	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::Move0Cam1) :	// Move - Cam1 - Pt0
			if (!IsRun())
				break;
			if (pView->IsClampOff())
				break;

			if (bDualTest)
			{
				if (m_bMkSt[1])	// Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufDnSerial[1] < m_nLotEndSerial)	// Right - AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufDnSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0))	// Move - Cam1 - Pt0
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufDnSerial[1] > m_nLotEndSerial)	//  Right - AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufDnSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0))	// Move - Cam1 - Pt0
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			else
			{
				if (m_bMkSt[1])	// Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufUpSerial[1] < m_nLotEndSerial)	// Right - AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0)) 	// Move - Cam1 - Pt0
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufUpSerial[1] > m_nLotEndSerial)	// Right - AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0)) 	// Move - Cam1 - Pt0
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			break;
		case MK_ST + (Mk2PtIdx::Move0Cam1) + 1:
			if (IsRun())
			{
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}
			break;
		case MK_ST + (Mk2PtIdx::Move0Cam0) :	// Move - Cam0 - Pt0
			if (MoveAlign0(0))
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move0Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move0Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Align1_0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}
			break;
		case MK_ST + (Mk2PtIdx::Align1_0) :	// 2PtAlign - Cam1 - Pt0
			if (m_bMkSt[1])	// Right
			{
				if (!m_bSkipAlign[1][0])
				{
					if (TwoPointAlign1(0))	// Right
					{
						m_bFailAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]);
					}
					else
					{
						m_bFailAlign[1][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]);
					}
				}
				else
				{
					m_bFailAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]);
				}
			}
			else
			{
				m_bFailAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]);
			}
			m_nMkStAuto = MK_ST + (Mk2PtIdx::Align0_0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			break;
		case MK_ST + (Mk2PtIdx::Align0_0) :	// 2PtAlign - Cam0 - Pt0
			if (m_bMkSt[0] || m_bMkSt[1])	// Left
			{
				if (!m_bSkipAlign[0][0])
				{
					if (TwoPointAlign0(0))	// Left
					{
						m_bFailAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]);
					}
					else
					{
						m_bFailAlign[0][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]);
					}
				}
				else
				{
					m_bFailAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]);
				}
			}
			else
			{
				m_bFailAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]);
			}
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Align0_0) + 1:
			if (m_bFailAlign[0][0])	// Left
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);
				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][0]"), m_bReAlign[0][0]);
						m_bSkipAlign[0][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]);
						m_bSkipAlign[0][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
						m_bSkipAlign[0][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][2]"), m_bSkipAlign[0][2]);
						m_bSkipAlign[0][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][3]"), m_bSkipAlign[0][3]);
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
						else
						{
							m_bDoMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
					}
					else
					{
						m_bReAlign[0][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][0]"), m_bReAlign[0][0]);
						m_bSkipAlign[0][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]);
						m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
						m_bSkipAlign[0][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][2]"), m_bSkipAlign[0][2]);
						m_bSkipAlign[0][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][3]"), m_bSkipAlign[0][3]);
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign0(0) 으로 진행. - 카메라 재정렬
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][0]"), m_bReAlign[0][0]);
					m_bSkipAlign[0][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]);
					m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign0(0) 으로 진행. - 카메라 재정렬
					Stop();
				}
			}
			if (m_bFailAlign[1][0])	// Right
			{
				Buzzer(TRUE, 0);

				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][0]"), m_bReAlign[1][0]);
						m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
						m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
						m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
						m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
						else
						{
							m_bDoMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
					}
					else
					{
						m_bReAlign[1][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][0]"), m_bReAlign[1][0]);
						m_bSkipAlign[1][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
						m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
						m_bSkipAlign[1][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
						m_bSkipAlign[1][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
						Stop();
						TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][0]"), m_bReAlign[1][0]);
					m_bSkipAlign[1][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
					Stop();
					TowerLamp(RGB_YELLOW, TRUE);
				}
			}

			if (m_bFailAlign[0][0] || m_bFailAlign[1][0]) // Left or Right
			{
				if (!m_bReAlign[0][0] && !m_bReAlign[1][0])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						m_nMkStAuto++; //m_nMkStAuto = MK_ST + 27; // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
				{
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
				}
			}
			else
				m_nMkStAuto++;

			break;
		case MK_ST + (Mk2PtIdx::Align0_0) + 2:
			if (IsRun())
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtAlignPt1()
{
	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::Move1Cam1) :
			if (pView->IsClampOff())
				break;

			if (bDualTest)
			{
				if (m_bMkSt[1]) // Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufDnSerial[1] < m_nLotEndSerial)	//  Right - AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufDnSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufDnSerial[1] > m_nLotEndSerial)	// AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufDnSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			else
			{
				if (m_bMkSt[1]) // Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufUpSerial[1] < m_nLotEndSerial)	// Right - AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufUpSerial[1] > m_nLotEndSerial)	// Right - AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
		   break;
		case MK_ST + (Mk2PtIdx::Move1Cam1) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move1Cam0) :
			if (MoveAlign0(1))	// Move - Cam0 - Pt1
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move1Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Move1Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Align1_1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}
			break;
		case MK_ST + (Mk2PtIdx::Align1_1) :	// 2PtAlign - Cam1 - Pt1
			if (m_bMkSt[1])	// Right
			{
				if (!m_bSkipAlign[1][1])	// Right
				{
					if (!TwoPointAlign1(1))
					{
						m_bFailAlign[1][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]);
					}
					else
					{
						m_bFailAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]);
					}
				}
				else
				{
					m_bFailAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]);
				}
			}
			else
			{
				m_bFailAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]);
			}
			m_nMkStAuto = MK_ST + (Mk2PtIdx::Align0_1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			break;
		case MK_ST + (Mk2PtIdx::Align0_1) :	// 2PtAlign - Cam0 - Pt1
			if (m_bMkSt[0] || m_bMkSt[1])	// Left
			{
				if (!m_bSkipAlign[0][1])	// Left
				{
					if (!TwoPointAlign0(1))
					{
						m_bFailAlign[0][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]);
					}
					else
					{
						m_bFailAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]);
					}
				}
				else
				{
					m_bFailAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]);
				}
			}
			else
			{
				m_bFailAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]);
			}

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Align0_1) + 1:
			if (m_bFailAlign[0][1])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][1]"), m_bReAlign[0][1]);
						m_bSkipAlign[0][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
						else
						{
							m_bDoMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
					}
					else
					{
						m_bReAlign[0][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][1]"), m_bReAlign[0][1]);
						m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign0(1) 으로 진행. - 카메라 재정렬
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][1]"), m_bReAlign[0][1]);
					m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign1(1) 으로 진행. - 카메라 재정렬
					Stop();
				}
			}
			if (m_bFailAlign[1][1])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][1]"), m_bReAlign[1][1]);
						m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
						else
						{
							m_bDoMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
					}
					else
					{
						m_bReAlign[1][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][1]"), m_bReAlign[1][1]);
						m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign1(1) 으로 진행. - 카메라 재정렬
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][1]"), m_bReAlign[1][1]);
					m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign1(1) 으로 진행. - 카메라 재정렬
					Stop();
					//TowerLamp(RGB_YELLOW, TRUE);
				}
			}

			if (m_bFailAlign[0][1] || m_bFailAlign[1][1])
			{
				if (!m_bReAlign[0][1] && !m_bReAlign[1][1])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
					{
						m_nMkStAuto = MK_ST + (Mk2PtIdx::MoveInitPt); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); //m_nMkStAuto = MK_ST + 29;  // MoveInitPos0()
					}
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
				{
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Move1Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign1(1) 으로 진행. - 카메라 재정렬
				}
			}
			else
			{
				m_nMkStAuto = MK_ST + (Mk2PtIdx::MoveInitPt); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}

			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtMoveInitPos()
{

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::MoveInitPt) :
			if (pView->IsClampOff())
				break;
			MoveInitPos0(FALSE);
			MoveInitPos1(FALSE);
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::MoveInitPt) + 1:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::MoveInitPt) + 2:
			if (IsMoveDone())
			{
				m_nMkStAuto = MK_ST + (Mk2PtIdx::ChkElec); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtElecChk()
{
	CString sRst;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::ChkElec) : // DoElecChk
			if (DoElecChk(sRst))
			{
				if (pDoc->WorkingInfo.Probing[0].bUse)
				{
					if (sRst == _T("Open"))
					{
						if (pDoc->WorkingInfo.Probing[0].bStopOnOpen)
						{
							m_nMkStAuto = REJECT_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
						}
						else
							m_nMkStAuto++;
					}
					else if (sRst == _T("Error"))
					{
						m_nMkStAuto = ERROR_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
					else
					{
						m_nMkStAuto++;
					}
				}
				else
					m_nMkStAuto++;
			}
		   break;

		case MK_ST + (Mk2PtIdx::ChkElec) + 1:
			if (ChkLightErr())
			{
				m_bChkLightErr = FALSE;
				m_nMkStAuto++;
			}
			else
			{
				if (MODE_INNER != pDoc->GetTestMode())
				{
					m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); 	// Mk 마킹 시작
				}
				else
				{
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Shift2Mk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				}
			}
			break;

		case MK_ST + (Mk2PtIdx::ChkElec) + 2:
			if (IsRun())
			{
				if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0) &&
					m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
				{
					if (MODE_INNER != pDoc->GetTestMode())
					{
						m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); 	// Mk 마킹 시작
					}
					else
					{
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Shift2Mk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
				}
				else
				{
					Stop();
					MsgBox(_T("마킹부 모션이 비활성화 되었습니다."));
					TowerLamp(RGB_RED, TRUE);
				}
			}
			else
			{
				if (!m_bChkLightErr)
				{
					m_bChkLightErr = TRUE;
					MsgBox(_T("노광불량 정지 - 기판을 확인하세요.\r\n계속진행하려면 운전스위치를 누르세요."));
				}
			}
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtDoMarking()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sRst, sMsg, sItem;
	int a, b, nSerial, nPrevSerial;
	BOOL bChk = FALSE;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::DoMk) :				// Mk 마킹 시작
			if (!IsRun())
				break;
			if (CheckMkPnt())
			{
				pDoc->WorkingInfo.LastJob.sProcessNum = pDoc->GetProcessNum(); // for DTS

				if (pDoc->GetTestMode() == MODE_OUTER)
					SetMkIts(TRUE);						// ITS 마킹 시작
				else
					SetMk(TRUE);						// Mk 마킹 시작

				m_nMkStAuto++;
			}
			else
			{
				MsgBox(_T("캠마스터의 마킹위치좌표가 설정되어 있지않습니다.\r\n확인하세요."));
				Stop();
				TowerLamp(RGB_RED, TRUE);
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1);
			}
			break;

		case MK_ST + (Mk2PtIdx::DoMk) + 1:
			if (!m_bUpdateYield)
			{
				if (!m_bTHREAD_UPDATAE_YIELD[0] && !m_bTHREAD_UPDATAE_YIELD[1])	// Left Shot의 수율과 Right Shot의 수율을 업데이트함.
				{
					if (!m_bTHREAD_UPDATE_YIELD_UP && !m_bTHREAD_UPDATE_YIELD_DN && !m_bTHREAD_UPDATE_YIELD_INNER_UP && !m_bTHREAD_UPDATE_YIELD_INNER_DN 
						&& !m_bTHREAD_UPDATE_YIELD_ALLUP && !m_bTHREAD_UPDATE_YIELD_ALLDN && !m_bTHREAD_UPDATE_YIELD_INNER_ALLUP && !m_bTHREAD_UPDATE_YIELD_INNER_ALLDN)	// Left Shot의 수율과 Right Shot의 수율을 업데이트함.
					{
						m_bUpdateYield = TRUE; pDoc->SetStatus(_T("General"), _T("bUpdateYield"), m_bUpdateYield);
						UpdateYield(); // Cam[0],  Cam[1]
						m_nMkStAuto++;
					}
				}
			}
			else
			{
				Sleep(100);
				m_nMkStAuto++;
			}
			break;

		case MK_ST + (Mk2PtIdx::DoMk) + 2:
			m_nMkStAuto++;

			//if (!m_bTHREAD_UPDATAE_YIELD[0] && !m_bTHREAD_UPDATAE_YIELD[1])
			//{
			//	if (!m_bUpdateYieldOnRmap)
			//	{
			//		if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN)
			//		{
			//			if (!m_bTHREAD_UPDATE_YIELD_UP && !m_bTHREAD_UPDATE_YIELD_DN && !m_bTHREAD_UPDATE_YIELD_ALLUP && !m_bTHREAD_UPDATE_YIELD_ALLDN)
			//			{
			//				m_bUpdateYieldOnRmap = TRUE;
			//				pDoc->UpdateYieldOnRmap(); // 20241115
			//				m_nMkStAuto++;
			//			}
			//			else
			//				Sleep(100);
			//		}
			//		else
			//			Sleep(100);
			//	}
			//	else
			//	{
			//		Sleep(100);
			//		m_nMkStAuto++; // 마킹 및 verify가 완전히 끝나지 않은 경우.
			//	}
			//}
			break;

		case MK_ST + (Mk2PtIdx::Verify) :
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !m_nPrevMkStAuto)
				{
					m_nPrevMkStAuto = MK_ST + (Mk2PtIdx::Verify); pDoc->SetStatusInt(_T("General"), _T("nPrevMkStAuto"), m_nPrevMkStAuto);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);		// Mk 마킹 시작
					m_bCam = TRUE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
				}
				else
				{
					if (IsReview())
					{
						if (!m_bCam)
						{
							m_nPrevStepAuto = MK_ST + (Mk2PtIdx::Verify); pDoc->SetStatusInt(_T("General"), _T("nPrevStepAuto"), m_nPrevStepAuto);
							m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);		// Mk 마킹 시작
							m_bCam = TRUE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
							MsgBox(_T("▶ Jog 버튼을 이용하여 마킹위치를 확인하여 주세요."));
						}
						else
						{
							m_bCam = FALSE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
							m_nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Mk 마킹 완료
							if (m_bInitAuto)
								ShowLive(FALSE);
						}
					}
					else
					{
						m_nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Mk 마킹 완료
						if (m_bInitAuto)
							ShowLive(FALSE);
					}
				}
			}
			else if (IsReMk())
			{
				m_nPrevMkStAuto = MK_ST + (Mk2PtIdx::Verify); pDoc->SetStatusInt(_T("General"), _T("nPrevMkStAuto"), m_nPrevMkStAuto);
				m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);		// Mk 재시작
			}
			else
			{
				sMsg = _T("");
				sMsg += m_sDispSts[0];
				sMsg += _T(",");
				sMsg += m_sDispSts[1];
			}
			break;

		case MK_ST + (Mk2PtIdx::DoneMk) :	 // Align변수 초기화
			if (!IsRun()) 
				break;

			if (m_bInitAuto)
			{
				m_bInitAuto = FALSE; pDoc->SetStatus(_T("General"), _T("bInitAuto"), m_bInitAuto);
				Stop();
				MsgBox(_T("마킹위치를 확인하세요."));
				TowerLamp(RGB_YELLOW, TRUE);
				break;
			}

			if (m_nBufUpSerial[0] == 0)
			{
				m_bSkipAlign[0][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]);
				m_bSkipAlign[0][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
			}
			if (m_nBufUpSerial[1] == 0)
			{
				m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
				m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
			}

			if ((!m_bSkipAlign[0][0] && !m_bSkipAlign[0][1]) && (!m_bSkipAlign[1][0] && !m_bSkipAlign[1][1]))
			{
				if (m_bMkSt[1])
					CompletedMk(2); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
				else
					CompletedMk(0); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
			}
			else if ((m_bSkipAlign[0][0] || m_bSkipAlign[0][1]) && (!m_bSkipAlign[1][0] && !m_bSkipAlign[1][1]))
			{
				if (m_bMkSt[1])
					CompletedMk(1); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
				else
					CompletedMk(3); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
			}
			else if( (!m_bSkipAlign[0][0] && !m_bSkipAlign[0][1]) && (m_bSkipAlign[1][0] || m_bSkipAlign[1][1]) )
				CompletedMk(0); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
			else
				CompletedMk(3); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 1:  // Mk변수 초기화
			m_bReAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][0]"), m_bReAlign[0][0]); // [nCam][nPos] 
			m_bReAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][1]"), m_bReAlign[0][1]); // [nCam][nPos] 
			m_bReAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][2]"), m_bReAlign[0][2]); // [nCam][nPos] 
			m_bReAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][3]"), m_bReAlign[0][3]); // [nCam][nPos] 

			m_bSkipAlign[0][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]); // [nCam][nPos] 
			m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]); // [nCam][nPos] 
			m_bSkipAlign[1][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]); // [nCam][nPos] 
			m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]); // [nCam][nPos] 

			m_bFailAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]); // [nCam][nPos] 
			m_bFailAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]); // [nCam][nPos] 
			m_bFailAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]); // [nCam][nPos] 
			m_bFailAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]); // [nCam][nPos] 

			m_bDoMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
			m_bDoMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
			m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
			m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
			m_bReMark[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReMark[0]"), m_bReMark[0]);
			m_bReMark[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReMark[1]"), m_bReMark[1]);
			m_bCam = FALSE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
			m_nPrevMkStAuto = 0; pDoc->SetStatusInt(_T("General"), _T("nPrevMkStAuto"), m_nPrevMkStAuto);

			m_bUpdateYield = FALSE; pDoc->SetStatus(_T("General"), _T("bUpdateYield"), m_bUpdateYield);
			m_bUpdateYieldOnRmap = FALSE;

			for (a = 0; a < 2; a++)
			{
				for (b = 0; b < MAX_STRIP_NUM; b++)
				{
					m_nMkStrip[a][b] = 0;
					m_bRejectDone[a][b] = FALSE;
					sItem.Format(_T("bRejectDone[%d][%d]"), a, b);
					pDoc->SetStatus(_T("General"), sItem, m_bRejectDone[a][b]);
				}
			}

			m_nSaveMk0Img = 0;
			m_nSaveMk1Img = 0;

			MpeWrite(Plc.DlgMenu01.MarkingDoing, 0);	// 마킹부 마킹중 ON (PC가 ON, OFF)
			MpeWrite(Plc.DlgMenu01.MarkingDone, 1);	// 마킹완료(PLC가 확인하고 Reset시킴.)-20141029

			MpeWrite(Plc.DlgMenu03.PcrMarkedSerialLeft, m_nBufUpSerial[0]);
			MpeWrite(Plc.DlgMenu03.PcrMarkedSerialRight, m_nBufUpSerial[1]);
			MpeWrite(Plc.DlgMenu03.FeedingReadyPunch, 1);

			pDoc->LogAuto(_T("PC: 마킹완료(PLC가 확인하고 Reset시킴.)"));
			if (IsNoMk() || IsShowLive())
				ShowLive(FALSE);

			m_nMkStAuto++;
			break;

		case MK_ST + (Mk2PtIdx::DoneMk) + 2:
			bChk = MpeRead(_T("MB400243")); // PLC 마킹 피딩 완료
			//if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[4] & (0x01 << 3))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
			if (bChk)	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
			{
				MpeWrite(_T("MB400243"), 0);// PLC 마킹 피딩 완료
				pDoc->LogAuto(_T("PLC: 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)"));
				m_nMkStAuto++;
			}
			break;

		case MK_ST + (Mk2PtIdx::DoneMk) + 3:
			if (!m_bTHREAD_SHIFT2MK)
			{
				MpeWrite(Plc.DlgMenu01.TableVacuumAoiUp, 0);	// 마킹부 Feeding완료
				pDoc->LogAuto(_T("PC: 마킹부 Feeding완료 OFF"));

				m_bShift2Mk = TRUE;
				DoShift2Mk();

				SetMkFdLen();
				SetCycTime();
				m_dwCycSt = GetTickCount();

				UpdateRst();

				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 4:
			if (!m_bTHREAD_SHIFT2MK && !m_bTHREAD_REELMAP_YIELD_UP && !m_bTHREAD_REELMAP_YIELD_DN && !m_bTHREAD_REELMAP_YIELD_ALLUP && !m_bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (m_bTHREAD_REELMAP_YIELD_ITS) // Yield Reelmap
						break;
				}

				m_nMkStAuto++;
				ChkYield();
			}
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 5:
			if (!m_bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				ChkLotCutPos();
				UpdateWorking();	// Update Working Info...
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 6:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 7:
			m_nMkStAuto++;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;
		case MK_ST + (Mk2PtIdx::DoneMk) + 8:
			m_bMkSt[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt0"), m_bMkSt[0]);
			m_bMkSt[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt1"), m_bMkSt[1]);
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtShift2Mk() // MODE_INNER
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sRst, sMsg;
	int a, b, nSerial, nPrevSerial;
	BOOL bChk = FALSE;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::Shift2Mk) :
			if (!m_bTHREAD_UPDATAE_YIELD[0] && !m_bTHREAD_UPDATAE_YIELD[1])
			{
				if (!m_bTHREAD_UPDATE_YIELD_UP && !m_bTHREAD_UPDATE_YIELD_DN && !m_bTHREAD_UPDATE_YIELD_INNER_UP && !m_bTHREAD_UPDATE_YIELD_INNER_DN
					&& !m_bTHREAD_UPDATE_YIELD_ALLUP && !m_bTHREAD_UPDATE_YIELD_ALLDN && !m_bTHREAD_UPDATE_YIELD_INNER_ALLUP && !m_bTHREAD_UPDATE_YIELD_INNER_ALLDN)	// Left Shot의 수율과 Right Shot의 수율을 업데이트함.
				{
					UpdateYield(); // Cam[0],  Cam[1]
					m_nMkStAuto++;
				}
			}
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 1:
			if (!m_bTHREAD_UPDATAE_YIELD[0] && !m_bTHREAD_UPDATAE_YIELD[1])
			{
				if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN)
				{
					if (!m_bTHREAD_UPDATE_YIELD_UP && !m_bTHREAD_UPDATE_YIELD_DN && !m_bTHREAD_UPDATE_YIELD_INNER_UP && !m_bTHREAD_UPDATE_YIELD_INNER_DN
						&& !m_bTHREAD_UPDATE_YIELD_ALLUP && !m_bTHREAD_UPDATE_YIELD_ALLDN && !m_bTHREAD_UPDATE_YIELD_INNER_ALLUP && !m_bTHREAD_UPDATE_YIELD_INNER_ALLDN)	// Left Shot의 수율과 Right Shot의 수율을 업데이트함.
					{
						UpdateRst();
						UpdateWorking();	// Update Working Info...
						//pDoc->UpdateYieldOnRmap(); // 20241115
						m_nMkStAuto++;
					}
					else
						Sleep(100);
				}
				else
					Sleep(100);
			}
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 2:
			MpeWrite(Plc.DlgMenu01.MarkingDoing, 0);	// 마킹부 마킹중 ON (PC가 ON, OFF)
			MpeWrite(Plc.DlgMenu01.MarkingDone, 1);	// 마킹완료(PLC가 확인하고 Reset시킴.)-20141029
			pDoc->LogAuto(_T("PC: 마킹완료(PLC가 확인하고 Reset시킴.)"));
			m_nMkStAuto++;
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 3:
			bChk = MpeRead(_T("MB400243")); // PLC 마킹 피딩 완료
			//if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[4] & (0x01 << 3))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
			if (bChk)	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
			{
				pDoc->LogAuto(_T("PLC: 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)"));
				m_nMkStAuto++;
			}
			break;

		case MK_ST + (Mk2PtIdx::Shift2Mk) + 4:
			if (!m_bTHREAD_SHIFT2MK)
			{
				pDoc->LogAuto(_T("PC: 마킹부 Feeding완료 OFF"));
				MpeWrite(Plc.DlgMenu01.FeedingDonePunch, 0);	// 마킹부 Feeding완료
				m_bShift2Mk = TRUE;
				DoShift2Mk();

				SetMkFdLen();
				SetCycTime();
				m_dwCycSt = GetTickCount();
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 5:
			if (!m_bTHREAD_UPDATAE_YIELD[0] && !m_bTHREAD_UPDATAE_YIELD[1])
			{
				//if (!m_bTHREAD_SHIFT2MK && !m_bTHREAD_REELMAP_YIELD_UP && !m_bTHREAD_REELMAP_YIELD_DN && !m_bTHREAD_REELMAP_YIELD_ALLUP && !m_bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
				if (!m_bTHREAD_SHIFT2MK)
				{
					if (!m_bTHREAD_SHIFT2MK && !m_bTHREAD_UPDATE_YIELD_UP && !m_bTHREAD_UPDATE_YIELD_DN && !m_bTHREAD_UPDATE_YIELD_INNER_UP && !m_bTHREAD_UPDATE_YIELD_INNER_DN
						&& !m_bTHREAD_UPDATE_YIELD_ALLUP && !m_bTHREAD_UPDATE_YIELD_ALLDN && !m_bTHREAD_UPDATE_YIELD_INNER_ALLUP && !m_bTHREAD_UPDATE_YIELD_INNER_ALLDN)	// Left Shot의 수율과 Right Shot의 수율을 업데이트함.
					{
						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							if (m_bTHREAD_REELMAP_YIELD_ITS) // Yield Reelmap
								break;
						}

						m_nMkStAuto++;
						UpdateRst();
						UpdateWorking();	// Update Working Info...
						ChkYield();
					}
				}
			}
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 6:
			if (!m_bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				ChkLotCutPos();
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 7:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 8:
			m_nMkStAuto++;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;
		case MK_ST + (Mk2PtIdx::Shift2Mk) + 9:
			m_bMkSt[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt0"), m_bMkSt[0]);
			m_bMkSt[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt1"), m_bMkSt[1]);
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtLotDiff()
{
	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk2PtIdx::LotDiff) :
			Stop();
			TowerLamp(RGB_YELLOW, TRUE);
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::LotDiff) + 1:
			if (IDYES == MsgBox(_T("상면과 하면의 Lot가 다릅니다.\r\n계속 작업을 진행하시겠습니까?"), 0, MB_YESNO))
			{
				m_bContDiffLot = TRUE; pDoc->SetStatus(_T("General"), _T("bContDiffLot"), m_bContDiffLot);
			}
			else
			{
				m_bContDiffLot = FALSE; pDoc->SetStatus(_T("General"), _T("bContDiffLot"), m_bContDiffLot);
				m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
				m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
			}
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk2PtIdx::LotDiff) + 2:
			if (IsRun())
			{
				if (m_bContDiffLot)
				{
					m_nMkStAuto = MK_ST + (Mk2PtIdx::ChkSn); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				}
				else
					m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk2PtIdx::LotDiff) + 3:
			m_bContDiffLot = FALSE; pDoc->SetStatus(_T("General"), _T("bContDiffLot"), m_bContDiffLot);
			m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
			m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtReject()
{
	int a, b;
	CString sItem;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case REJECT_ST:
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();

			if (IDYES == MsgBox(_T("쇼트 체크 불량입니다.\r\n리젝 처리를 진행하시겠습니까?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[0]"), m_bAnswer[0]);
				m_nMkStAuto++;
			}
			else
			{
				if (IDYES == MsgBox(_T("리젝 처리를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[1]"), m_bAnswer[1]);
					m_nMkStAuto++;
				}
			}

			Buzzer(FALSE, 0);
			break;
		case REJECT_ST + 1:
			if (IsRun())
			{
				if (m_bAnswer[0])
				{
					m_bAnswer[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[0]"), m_bAnswer[0]);
					m_nMkStAuto++;
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[1]"), m_bAnswer[1]);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Mk 마킹 시작
				}
				else
				{
					m_nMkStAuto = REJECT_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				}
			}
			break;
		case REJECT_ST + 2:
			SetReject();
			m_nMkStAuto++;
			break;
		case REJECT_ST + 3:
			m_nMkStAuto++;
			break;
		case REJECT_ST + 4:
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !m_nPrevMkStAuto)
				{
					m_nPrevMkStAuto = REJECT_ST + 4; pDoc->SetStatusInt(_T("General"), _T("nPrevMkStAuto"), m_nPrevMkStAuto);
					m_nMkStAuto = REJECT_ST + 2; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);		// Mk 마킹 시작
					m_bCam = TRUE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);

					m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
					m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);

					for (a = 0; a < 2; a++)
					{
						for (b = 0; b < MAX_STRIP_NUM; b++)
						{
							m_nMkStrip[a][b] = 0;
							m_bRejectDone[a][b] = FALSE;
							sItem.Format(_T("bRejectDone[%d][%d]"), a, b);
							pDoc->SetStatus(_T("General"), sItem, m_bRejectDone[a][b]);
						}
					}
				}
				else
				{
					if (IsReview())
					{
						if (!m_bCam)
						{
							m_nPrevStepAuto = REJECT_ST + 4; pDoc->SetStatusInt(_T("General"), _T("nPrevStepAuto"), m_nPrevStepAuto);
							m_nMkStAuto = REJECT_ST + 2; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);		// Mk 마킹 시작
							m_bCam = TRUE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
							MsgBox(_T("▶ Jog 버튼을 이용하여 마킹위치를 확인하여 주세요."));
						}
						else
						{
							m_bCam = FALSE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
							m_nMkStAuto++;	// Mk 마킹 완료
						}
					}
					else
					{
						m_nMkStAuto++;	// Mk 마킹 완료
					}
				}
			}
			break;
		case REJECT_ST + 5:
			m_nMkStAuto = MK_ST + (Mk2PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);			// Align변수 초기화
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk2PtErrStop()
{
	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case ERROR_ST:
			Stop();
			TowerLamp(RGB_RED, TRUE);

			//if(IDYES == DoMyMsgBox(_T("쇼트 체크 Error입니다.\r\n다시 쇼트 체크를 진행하시겠습니까?"), MB_YESNO))
			if (IDYES == MsgBox(_T("쇼트 체크 Error입니다.\r\n다시 쇼트 체크를 진행하시겠습니까?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[0]"), m_bAnswer[0]);
				m_nMkStAuto++;
			}
			else
			{
				//if(IDYES == DoMyMsgBox(_T("쇼트 체크를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), MB_YESNO))
				if (IDYES == MsgBox(_T("쇼트 체크를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[1]"), m_bAnswer[1]);
					m_nMkStAuto++;
				}
				else
					m_nMkStAuto++;
			}
			break;
		case ERROR_ST + 1:
			m_nMkStAuto++;
			break;
		case ERROR_ST + 2:
			if (IsRun())
			{
				if (m_bAnswer[0])
				{
					m_bAnswer[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[0]"), m_bAnswer[0]);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::ChkElec); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // DoElecChk
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[1]"), m_bAnswer[1]);
					m_nMkStAuto = MK_ST + (Mk2PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Mk 마킹 시작
				}
				else
					m_nMkStAuto++;
			}
			break;
		case ERROR_ST + 3:
			m_nMkStAuto = ERROR_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			break;
		}
	}
}

void CGvisR2R_PunchView::MarkingWith2PointAlign()
{
	Mk2PtReady();			// 상하면 시리얼 설정
	Mk2PtChkSerial();		// 상하면 시리얼 체크
	Mk2PtInit();
	Mk2PtAlignPt0();
	Mk2PtAlignPt1();
	Mk2PtMoveInitPos();
	Mk2PtElecChk();
	if (MODE_INNER != pDoc->GetTestMode())
		Mk2PtDoMarking();
	else
		Mk2PtShift2Mk();
	Mk2PtLotDiff();
	Mk2PtReject();
	Mk2PtErrStop();
}

void CGvisR2R_PunchView::Mk4PtReady()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST:	// PLC MK 신호 확인	
			if (IsRun())
			{
				pDoc->LogAuto(_T("PC: 마킹부 마킹중 ON (PC가 ON, OFF)"));
				MpeWrite(Plc.DlgMenu01.MarkingDoing, 1);// 마킹부 마킹중 ON (PC가 ON, OFF)
				m_nMkStAuto++;
			}
			break;
		case MK_ST + 1:
			if (!m_bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				if (bDualTest)
				{
					if ((pDoc->m_ListBuf[0].nTot > 0) && (pDoc->m_ListBuf[1].nTot > 0)) // Buf Up/Dn 에 파일이 있으면 다음으로 진행.
					{
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
					else
					{
						if (!GetAoiUpVsStatus())
						{
							m_bLotEnd = TRUE;
							m_nLotEndAuto = LOT_END;
						}
					}
				}
				else
				{
					if (pDoc->m_ListBuf[0].nTot > 0) // Buf Up 에 파일이 있으면 다음으로 진행.
					{
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
					else
					{
						if (!GetAoiUpVsStatus())
						{
							m_bLotEnd = TRUE;
							m_nLotEndAuto = LOT_END;
						}
					}
				}
			}
			break;
		case MK_ST + (Mk4PtIdx::Start) :	// 2
			if (!IsRun())
				break;

			if (bDualTest)
			{
				if (pDoc->m_ListBuf[1].nTot > 0) // AOI-Dn
				{
					if (m_bMkSt[0] || m_bMkSt[1]) // Left
					{
						// Serial - Left
						m_nBufUpSerial[0] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[0]"), m_nBufUpSerial[0]);
						m_nBufDnSerial[0] = pDoc->m_ListBuf[1].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[0]"), m_nBufDnSerial[0]);
					}
					else
					{
						Stop();
						pView->SetAlarmToPlc(UNIT_PUNCH);
						pView->ClrDispMsg();
						AfxMessageBox(_T("좌측 마킹시작신호가 꺼져있습니다."));
						break;
					}

					m_nMkStAuto++;

					if (m_bMkSt[1]) // Right
					{
						if (pDoc->m_ListBuf[1].nTot > 0) // AOI-Dn
						{
							// Serial - Right
							m_nBufUpSerial[1] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufDnSerial[1]);
							m_nBufDnSerial[1] = pDoc->m_ListBuf[1].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[1]"), m_nBufDnSerial[1]);
						}
						else
						{
							m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
							m_nBufDnSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[1]"), m_nBufDnSerial[1]);
						}
					}
					else
					{
						m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
						m_nBufDnSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[1]"), m_nBufDnSerial[1]);
					}
				}
				else
				{
					if (!pDoc->m_bVsStatusUp)
					{
						m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
						m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
					}
				}
			}
			else
			{
				if (pDoc->m_ListBuf[0].nTot > 0) // AOI-Up
				{
					if (m_bMkSt[0] || m_bMkSt[1]) // Left
					{
						// Serial - Left
						m_nBufUpSerial[0] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[0]"), m_nBufUpSerial[0]);
					}
					else
					{
						Stop();
						pView->SetAlarmToPlc(UNIT_PUNCH);
						pView->ClrDispMsg();
						AfxMessageBox(_T("좌측 마킹시작신호가 꺼져있습니다."));
						break;
					}

					m_nMkStAuto++;

					if (m_bMkSt[1]) // Right
					{
						if (pDoc->m_ListBuf[0].nTot > 0) // AOI-Up
						{
							// Serial - Right
							m_nBufUpSerial[1] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
						}
						else
						{
							m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
						}
					}
					else
					{
						m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
					}
				}
				else
				{
					if (!pDoc->m_bVsStatusUp)
					{
						m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
						m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
					}
				}
			}
			break;
		case MK_ST + (Mk4PtIdx::Start) + 1:
			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN && !m_bTHREAD_MAKE_ITS_FILE_UP && !m_bTHREAD_MAKE_ITS_FILE_DN && !m_bTHREAD_UPDATE_REELMAP_INNER_UP && !m_bTHREAD_UPDATE_REELMAP_INNER_DN && !m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP && !m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN)
				{
					m_nMkStAuto = MK_ST + (Mk4PtIdx::ChkSn); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				}
			}
			else
			{
				if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
				{
					m_nMkStAuto = MK_ST + (Mk4PtIdx::ChkSn); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				}
			}
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtChkSerial()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sNewLot;
	BOOL bNewModel = FALSE;
	int nSerial = 0;
	int nNewLot = 0;
	int nLastShot = 0;
	double dFdEnc;
	CString sLot, sMsg;

	if (m_bTHREAD_SHIFT2MK || !IsRun())
		return;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::ChkSn) :
			if (m_nBufUpSerial[0] == m_nBufUpSerial[1])
			{
				Stop();
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("좌/우 마킹 시리얼이 같습니다."));
				SetListBuf();
				m_nMkStAuto = MK_ST + (Mk4PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				break;
			}

			if (bDualTest)
			{
				if (m_nBufUpSerial[0] != m_nBufDnSerial[0])
				{
					Stop();
					pView->ClrDispMsg();
					MsgBox(_T("좌측 상/하면 시리얼이 다릅니다."));
					SetListBuf();
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					break;
				}

				if (m_nBufUpSerial[1] != m_nBufDnSerial[1])
				{
					Stop();
					pView->ClrDispMsg();
					MsgBox(_T("우측 상/하면 시리얼이 다릅니다."));
					SetListBuf();
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					break;
				}
			}

			if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
			{
				if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN && !m_bTHREAD_MAKE_ITS_FILE_UP && !m_bTHREAD_MAKE_ITS_FILE_DN && !m_bTHREAD_UPDATE_REELMAP_INNER_UP && !m_bTHREAD_UPDATE_REELMAP_INNER_DN && !m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP && !m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN)
				{
					if (!m_bTHREAD_DISP_DEF && !m_bTHREAD_DISP_DEF_INNER)
					{
						m_nMkStAuto++;
						m_nStepTHREAD_DISP_DEF = 0; pDoc->SetStatusInt(_T("Thread"), _T("nStepTHREAD_DISP_DEF"), m_nStepTHREAD_DISP_DEF);
						m_nStepTHREAD_DISP_DEF_INNER = 0; pDoc->SetStatusInt(_T("Thread"), _T("nStepTHREAD_DISP_DEF_INNER"), m_nStepTHREAD_DISP_DEF_INNER);

						m_bTHREAD_DISP_DEF = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_DISP_DEF"), m_bTHREAD_DISP_DEF);				// DispDefImg() : CopyDefImg Start -> Disp Reelmap Start
						if (pDoc->GetTestMode() == MODE_OUTER)
							m_bTHREAD_DISP_DEF_INNER = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_DISP_DEF_INNER"), m_bTHREAD_DISP_DEF_INNER); // DispDefImgInner() : Disp Reelmap Start

						pDoc->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("1"));
					}
				}
			}
			else
			{
				if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
				{
					if (!m_bTHREAD_DISP_DEF)
					{
						m_nMkStAuto++;
						m_nStepTHREAD_DISP_DEF = 0; pDoc->SetStatusInt(_T("Thread"), _T("nStepTHREAD_DISP_DEF"), m_nStepTHREAD_DISP_DEF);

						m_bTHREAD_DISP_DEF = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_DISP_DEF"), m_bTHREAD_DISP_DEF);		// DispDefImg() : CopyDefImg Start -> Disp Reelmap Start
						pDoc->SetMkMenu01(_T("Signal"), _T("DispDefImg"), _T("1"));
					}
				}
			}

			break;

		case MK_ST + (Mk4PtIdx::ChkSn) + 1:
			m_nMkStAuto++;
			nSerial = m_nBufUpSerial[0]; // Left
			sNewLot = m_sNewLotUp;

			if (nSerial > 0)
			{
				if (pView->m_bSerialDecrese)
				{
					if (m_bLastProc && (m_nBufUpSerial[1] < m_nLotEndSerial || m_nBufUpSerial[1] == 0)) // Right
						nLastShot = m_nBufUpSerial[0];
				}
				else
				{
					if (m_bLastProc && (m_nBufUpSerial[1] > m_nLotEndSerial || m_nBufUpSerial[1] == 0)) // Right
					{
						nLastShot = m_nBufUpSerial[0];
						//nSerial = m_nBufUpSerial[0];
					}
				}

				bNewModel = GetAoiUpInfo(nSerial, &nNewLot, TRUE);
				if (bDualTest)
				{
					bNewModel = GetAoiDnInfo(nSerial, &nNewLot, TRUE);

					if (!IsSameUpDnLot() && !m_bContDiffLot)
					{
						m_nMkStAuto = MK_ST + (Mk4PtIdx::LotDiff); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
				}

				if (bNewModel)
				{
					Stop();
					break;
				}
				if (nNewLot)	// AOI 정보(AoiCurrentInfoPath) -> AOI Feeding Offset
				{
					// Lot Change.
					ChgLot();
#ifdef USE_MPE
					if (pDoc->m_pMpeData)
						dFdEnc = (double)pDoc->m_pMpeData[2][2];	// 마킹부 Feeding 엔코더 값(단위 mm )
					if ((pDoc->WorkingInfo.LastJob.bLotSep || pDoc->m_bDoneChgLot) && (dFdEnc + _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen)*2.0) > _tstof(pDoc->WorkingInfo.LastJob.sLotSepLen)*1000.0)
					{
						pDoc->m_bDoneChgLot = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneChgLot"), pDoc->m_bDoneChgLot);
						SetPathAtBuf();
					}
#endif
				}
			}
			else
			{
				Stop();
				MsgBox(_T("버퍼(좌) Serial이 1 이하입니다."));
				TowerLamp(RGB_YELLOW, TRUE);
			}
			break;

		case MK_ST + (Mk4PtIdx::ChkSn) + 2:
			m_nMkStAuto = MK_ST + (Mk4PtIdx::InitMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);					// InitMk()
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtInit()
{
	if (m_bTHREAD_SHIFT2MK)
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::InitMk) :
			if (InitMk()) // 시리얼이 증가 또는 감소 : TRUE 아니고 같으면 : FALSE
			{
				if (pView->m_bSerialDecrese)
				{
					if ((m_nBufUpSerial[0] < m_nLotEndSerial/* || m_nBufUpSerial[1] < m_nLotEndSerial*/) && m_nLotEndSerial > 0) // Left
					{
						//MpeWrite(Plc.DlgMenu01.MarkingInitDone, 1); // 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off)
						pDoc->LogAuto(_T("PC: 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off)"));
					}
				}
				else
				{
					if ((m_nBufUpSerial[0] > m_nLotEndSerial/* || m_nBufUpSerial[1] >= m_nLotEndSerial*/) && m_nLotEndSerial > 0)
					{
						//MpeWrite(Plc.DlgMenu01.MarkingInitDone, 1); // 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off) - 20160718
						pDoc->LogAuto(_T("PC: 마킹부 작업완료.(PC가 On, PLC가 확인 후 Off)"));
					}
				}
			}
			else // Same Serial
			{
				Stop();
				MsgBox(_T("Serial 연속 되지않습니다."));
				TowerLamp(RGB_YELLOW, TRUE);
			}
			m_nMkStAuto++;
			break;

		case MK_ST + (Mk4PtIdx::InitMk) + 1:
			if (IsRun())
			{
				m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Move - Cam1 - Pt0
			}
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtAlignPt0()
{
	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move0Cam1) :	// Move - Cam1 - Pt0
			if (pView->IsClampOff())
				break;

			if (bDualTest)
			{
				if (m_bMkSt[1])	// Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufDnSerial[1] < m_nLotEndSerial)	// Right - AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufDnSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0))	// Move - Cam1 - Pt0
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufDnSerial[1] > m_nLotEndSerial)	//  Right - AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufDnSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0))	// Move - Cam1 - Pt0
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			else
			{
				if (m_bMkSt[1])	// Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufUpSerial[1] < m_nLotEndSerial)	// Right - AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0)) 	// Move - Cam1 - Pt0
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufUpSerial[1] > m_nLotEndSerial)	// Right - AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(0)) 	// Move - Cam1 - Pt0
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			break;
		case MK_ST + (Mk4PtIdx::Move0Cam1) + 1:
			if (IsRun())
			{
				m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}
			break;
		case MK_ST + (Mk4PtIdx::Move0Cam0) :	// Move - Cam0 - Pt0
			if (MoveAlign0(0))
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move0Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move0Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}
			break;
		case MK_ST + (Mk4PtIdx::Align1_0) :	// 4PtAlign - Cam1 - Pt0
			if (m_bMkSt[1])	// Right
			{
				if (!m_bSkipAlign[1][0])
				{
					if (FourPointAlign1(0))
					{
						m_bFailAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]);
					}
					else
					{
						m_bFailAlign[1][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]);
					}
				}
				else
				{
					m_bFailAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]);
				}
			}
			else
			{
				m_bFailAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]);
			}
			m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			break;
		case MK_ST + (Mk4PtIdx::Align0_0) :	// 4PtAlign - Cam0 - Pt0
			if (m_bMkSt[0] || m_bMkSt[1])	// Left
			{
				if (!m_bSkipAlign[0][0])
				{
					if (FourPointAlign0(0))
					{
						m_bFailAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]);
					}
					else
					{
						m_bFailAlign[0][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]);
					}
				}
				else
				{
					m_bFailAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]);
				}
			}
			else
			{
				m_bFailAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]);
			}
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_0) + 1:
			if (m_bFailAlign[0][0])	// Left
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][0]"), m_bReAlign[0][0]);
						m_bSkipAlign[0][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]);
						m_bSkipAlign[0][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
						m_bSkipAlign[0][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][2]"), m_bSkipAlign[0][2]);
						m_bSkipAlign[0][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][3]"), m_bSkipAlign[0][3]);
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
						else
						{
							m_bDoMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
					}
					else
					{
						m_bReAlign[0][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][0]"), m_bReAlign[0][0]);
						m_bSkipAlign[0][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]);
						m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
						m_bSkipAlign[0][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][2]"), m_bSkipAlign[0][2]);
						m_bSkipAlign[0][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][3]"), m_bSkipAlign[0][3]);
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign0(0) 으로 진행. - 카메라 재정렬
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][0]"), m_bReAlign[0][0]);
					m_bSkipAlign[0][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]);
					m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign0(0) 으로 진행. - 카메라 재정렬
					Stop();
				}
			}
			if (m_bFailAlign[1][0])	// Right
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][0]"), m_bReAlign[1][0]);
						m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
						m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
						m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
						m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
						else
						{
							m_bDoMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
					}
					else
					{
						m_bReAlign[1][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][0]"), m_bReAlign[1][0]);
						m_bSkipAlign[1][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
						m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
						m_bSkipAlign[1][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
						m_bSkipAlign[1][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(0) 으로 진행. - 카메라 재정렬
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][0] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][0]"), m_bReAlign[1][0]);
					m_bSkipAlign[1][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(0) 으로 진행. - 카메라 재정렬
					Stop();
				}
			}

			if (m_bFailAlign[0][0] || m_bFailAlign[1][0])
			{
				if (!m_bReAlign[0][0] && !m_bReAlign[1][0])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						m_nMkStAuto++; //m_nMkStAuto = MK_ST + 27; // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
				{
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move0Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // TwoPointAlign1(0) 으로 진행. - 카메라 재정렬
				}
			}
			else
				m_nMkStAuto++;

			break;
		case MK_ST + (Mk4PtIdx::Align0_0) + 2:
			if (IsRun())
				m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtAlignPt1()
{
	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move1Cam1) :
			if (pView->IsClampOff())
				break;

			if (bDualTest)
			{
				if (m_bMkSt[1]) // Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufDnSerial[0] < m_nLotEndSerial)	// AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufDnSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufDnSerial[0] > m_nLotEndSerial)	// AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufDnSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			else // Single
			{
				if (m_bMkSt[1]) // Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufUpSerial[1] < m_nLotEndSerial)	// AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufUpSerial[1] > m_nLotEndSerial)	// AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(1))	// Move - Cam1 - Pt1
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			break;
		case MK_ST + (Mk4PtIdx::Move1Cam1) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move1Cam0) :
			if (MoveAlign0(1))	// Move - Cam0 - Pt1
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move1Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move1Cam0) + 2:
			if (IsMoveDone())
			{ 
				Sleep(100);
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Align1_1) :	// 4PtAlign - Cam1 - Pt1
			if (m_bMkSt[1])	// Right
			{
				if (!m_bSkipAlign[1][1])	// Right
				{
					if (!FourPointAlign1(1))
					{
						m_bFailAlign[1][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]);
					}
					else
					{
						m_bFailAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]);
					}
				}
				else
				{
					m_bFailAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]);
				}
			}
			else
			{
				m_bFailAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]);
			}
			m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			break;
		case MK_ST + (Mk4PtIdx::Align0_1) :	// 4PtAlign - Cam0 - Pt1
			if (m_bMkSt[0] || m_bMkSt[1])	// Left
			{
				if (!m_bSkipAlign[0][1])	// Left
				{
					if (!FourPointAlign0(1))
					{
						m_bFailAlign[0][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]);
					}
					else
					{
						m_bFailAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]);
					}
				}
				else
				{
					m_bFailAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]);
				}
			}
			else
			{
				m_bFailAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]);
			}

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_1) + 1:
			if (m_bFailAlign[0][1])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][1]"), m_bReAlign[0][1]);
						m_bSkipAlign[0][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
						else
						{
							m_bDoMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
					}
					else
					{
						m_bReAlign[0][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][1]"), m_bReAlign[0][1]);
						m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign0(1) 으로 진행. - 카메라 재정렬
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][1]"), m_bReAlign[0][1]);
					m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
					Stop();
				}
			}
			if (m_bFailAlign[1][1])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][1]"), m_bReAlign[1][1]);
						m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
						else
						{
							m_bDoMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
					}
					else
					{
						m_bReAlign[1][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][1]"), m_bReAlign[1][1]);
						m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][1] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][1]"), m_bReAlign[1][1]);
					m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
					Stop();
				}
			}

			if (m_bFailAlign[0][1] || m_bFailAlign[1][1])
			{
				if (!m_bReAlign[0][1] && !m_bReAlign[1][1])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
					{
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
				{
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move1Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(1) 으로 진행. - 카메라 재정렬
				}
			}
			else
			{
				m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}

			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtAlignPt2()
{
	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move2Cam1) :
			if (pView->IsClampOff())
				break;

			if (bDualTest)
			{
				if (m_bMkSt[1]) // Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufDnSerial[0] < m_nLotEndSerial)	// AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufDnSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(2))	// Move - Cam1 - Pt2
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufDnSerial[0] > m_nLotEndSerial)	// AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufDnSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(2))	// Move - Cam1 - Pt2
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			else // Single
			{
				if (m_bMkSt[1]) // Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufUpSerial[0] < m_nLotEndSerial)	// AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(2))	// Move - Cam1 - Pt2
								m_nMkStAuto++;
						}
					}
					else if (m_nBufUpSerial[1] == 0)
					{
						m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
						m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
						m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
						m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
						m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
						m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						m_nMkStAuto++;
					}
					else
					{
						if (m_bLastProc && m_nBufUpSerial[0] > m_nLotEndSerial)	// AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(2))	// Move - Cam1 - Pt2
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			break;
		case MK_ST + (Mk4PtIdx::Move2Cam1) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move2Cam0) :
			if (MoveAlign0(2))	// Move - Cam0 - Pt2
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move2Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move2Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_2); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}
			break;
		case MK_ST + (Mk4PtIdx::Align1_2) :	// 4PtAlign - Cam1 - Pt2
			if (m_bMkSt[1])	// Right
			{
				if (!m_bSkipAlign[1][1])
				{
					if (!m_bSkipAlign[1][2])	// Right
					{
						if (!FourPointAlign1(2))
						{
							m_bFailAlign[1][2] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][2]"), m_bFailAlign[1][2]);
						}
						else
						{
							m_bFailAlign[1][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][2]"), m_bFailAlign[1][2]);
						}
					}
					else
					{
						m_bFailAlign[1][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][2]"), m_bFailAlign[1][2]);
					}
				}
				else
				{
					m_bFailAlign[1][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][2]"), m_bFailAlign[0][2]);
				}
			}
			else
			{
				m_bFailAlign[1][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]);
			}
			m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_2); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			break;
		case MK_ST + (Mk4PtIdx::Align0_2) :	// 4PtAlign - Cam0 - Pt2
			if (m_bMkSt[0] || m_bMkSt[1])	// Left
			{
				if (!m_bSkipAlign[0][1])
				{
					if (!m_bSkipAlign[0][2])
					{
						if (!FourPointAlign0(2))
						{
							m_bFailAlign[0][2] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][2]"), m_bFailAlign[0][2]);
						}
						else
						{
							m_bFailAlign[0][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][2]"), m_bFailAlign[0][2]);
						}
					}
					else
					{
						m_bFailAlign[0][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][2]"), m_bFailAlign[0][2]);
					}
				}
				else
				{
					m_bFailAlign[0][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][2]"), m_bFailAlign[0][2]);
				}
			}
			else
			{
				m_bFailAlign[0][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][2]"), m_bFailAlign[0][2]);
			}

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_2) + 1:
			if (m_bFailAlign[0][2])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][2] = FALSE;
						m_bSkipAlign[0][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][2]"), m_bSkipAlign[0][2]);
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
						else
						{
							m_bDoMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
					}
					else
					{
						m_bReAlign[0][2] = TRUE;
						m_bSkipAlign[0][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][2]"), m_bSkipAlign[0][2]);
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign0(2) 으로 진행. - 카메라 재정렬
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][2] = TRUE;
					m_bSkipAlign[0][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][2]"), m_bSkipAlign[0][2]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign0(2) 으로 진행. - 카메라 재정렬
					Stop();
				}
			}
			if (m_bFailAlign[1][2])
			{
				Buzzer(TRUE, 0);
				TowerLamp(RGB_YELLOW, TRUE);

				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][2] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][2]"), m_bReAlign[1][2]);
						m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
						else
						{
							m_bDoMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
					}
					else
					{
						m_bReAlign[1][2] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][2]"), m_bReAlign[1][2]);
						m_bSkipAlign[1][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(2) 으로 진행. - 카메라 재정렬
						Stop();
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][2] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][2]"), m_bReAlign[1][2]);
					m_bSkipAlign[1][2] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(2) 으로 진행. - 카메라 재정렬
					Stop();
				}
			}

			if (m_bFailAlign[0][2] || m_bFailAlign[1][2])
			{
				if (!m_bReAlign[0][2] && !m_bReAlign[1][2])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
					{
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move2Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(2) 으로 진행. - 카메라 재정렬
			}
			else
				m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);

			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtAlignPt3()
{
	if (!IsRun())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Move3Cam1) :
			if (pView->IsClampOff())
				break;

			if (bDualTest)
			{
				if (m_bMkSt[1]) // Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufDnSerial[0] < m_nLotEndSerial)	// AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(3))	// Move - Cam1 - Pt3
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufDnSerial[0] > m_nLotEndSerial)	// AOI하면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(3))	// Move - Cam1 - Pt3
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			else // Single
			{
				if (m_bMkSt[1]) // Right
				{
					if (pView->m_bSerialDecrese)
					{
						if (m_bLastProc && m_nBufUpSerial[0] < m_nLotEndSerial)	// AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(3))	// Move - Cam1 - Pt3
								m_nMkStAuto++;
						}
					}
					else
					{
						if (m_bLastProc && m_nBufUpSerial[0] > m_nLotEndSerial)	// AOI상면 Serial
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else if (m_nBufUpSerial[1] == 0)
						{
							m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
							m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
							m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
							m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
							m_nMkStAuto++;
						}
						else
						{
							if (MoveAlign1(3))	// Move - Cam1 - Pt3
								m_nMkStAuto++;
						}
					}
				}
				else
				{
					m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
					m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
					m_bSkipAlign[1][2] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][2]"), m_bSkipAlign[1][2]);
					m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
					m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
					m_nMkStAuto++;
				}
			}
			break;
		case MK_ST + (Mk4PtIdx::Move3Cam1) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move3Cam0) :
			if (MoveAlign0(3))	// Move - Cam0 - Pt3
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move3Cam0) + 1:
			if (IsRun())
				m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Move3Cam0) + 2:
			if (IsMoveDone())
			{
				Sleep(100);
				m_nMkStAuto = MK_ST + (Mk4PtIdx::Align1_3); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}
			break;
		case MK_ST + (Mk4PtIdx::Align1_3) :	// 4PtAlign - Cam1 - Pt3
			if (m_bMkSt[1])	// Right
			{
				if (!m_bSkipAlign[1][1])	// Right
				{
					if (!m_bSkipAlign[1][2])	// Right
					{
						if (!m_bSkipAlign[1][3])
						{
							if (!FourPointAlign1(3))
							{
								m_bFailAlign[1][3] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][3]"), m_bFailAlign[1][3]);
							}
							else
							{
								m_bFailAlign[1][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][3]"), m_bFailAlign[1][3]);
							}
						}
						else
						{
							m_bFailAlign[1][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][3]"), m_bFailAlign[1][3]);
						}
					}
					else
					{
						m_bFailAlign[1][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][3]"), m_bFailAlign[1][3]);
					}
				}
				else
				{
					m_bFailAlign[1][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][3]"), m_bFailAlign[1][3]);
				}
			}
			else
			{
				m_bFailAlign[1][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][3]"), m_bFailAlign[1][3]);
			}

			m_nMkStAuto = MK_ST + (Mk4PtIdx::Align0_3); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			break;
		case MK_ST + (Mk4PtIdx::Align0_3) :	// 4PtAlign - Cam0 - Pt3
			if (m_bMkSt[0] || m_bMkSt[1])	// Left
			{
				if (!m_bSkipAlign[0][1])
				{
					if (!m_bSkipAlign[0][2])
					{
						if (!m_bSkipAlign[0][3])
						{
							if (!FourPointAlign0(3))
							{
								m_bFailAlign[0][3] = TRUE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][3]"), m_bFailAlign[0][3]);
							}
							else
							{
								m_bFailAlign[0][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][3]"), m_bFailAlign[0][3]);
							}
						}
						else
						{
							m_bFailAlign[0][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][3]"), m_bFailAlign[0][3]);
						}
					}
					else
					{
						m_bFailAlign[0][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][3]"), m_bFailAlign[0][3]);
					}
				}
				else
				{
					m_bFailAlign[0][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][3]"), m_bFailAlign[0][3]);
				}
			}
			else
			{
				m_bFailAlign[0][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][3]"), m_bFailAlign[0][3]);
			}

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Align0_3) + 1:
			if (m_bFailAlign[0][3])
			{
				Buzzer(TRUE, 0);

				if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(좌)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[0][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][3]"), m_bReAlign[0][3]);
						m_bSkipAlign[0][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][3]"), m_bSkipAlign[0][3]);
						if (IDNO == MsgBox(_T("카메라(좌)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
						else
						{
							m_bDoMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
							m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
						}
					}
					else
					{
						m_bReAlign[0][3] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][3]"), m_bReAlign[0][3]);
						m_bSkipAlign[0][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][3]"), m_bSkipAlign[0][3]);
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign0(3) 으로 진행. - 카메라 재정렬
						Stop();
						TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[0][3] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][3]"), m_bReAlign[0][3]);
					m_bSkipAlign[0][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][3]"), m_bSkipAlign[0][3]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam0); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign0(3) 으로 진행. - 카메라 재정렬
					Stop();
					TowerLamp(RGB_YELLOW, TRUE);
				}
			}
			if (m_bFailAlign[1][3])
			{
				Buzzer(TRUE, 0);

				if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 다시 정렬하시겠습니까?"), 0, MB_YESNO))
				{
					Buzzer(FALSE, 0);

					if (IDYES == MsgBox(_T("카메라(우)의 검사판넬 정렬을 정말 않하시겠습니까?"), 0, MB_YESNO))
					{
						m_bReAlign[1][3] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][3]"), m_bReAlign[1][3]);
						m_bSkipAlign[1][3] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
						if (IDNO == MsgBox(_T("카메라(우)의 검사판넬을 불량마킹 하시겠습니까?"), 0, MB_YESNO))
						{
							m_bDoMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
						else
						{
							m_bDoMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
							m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
						}
					}
					else
					{
						m_bReAlign[1][3] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][3]"), m_bReAlign[1][3]);
						m_bSkipAlign[1][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(3) 으로 진행. - 카메라 재정렬
						Stop();
						TowerLamp(RGB_YELLOW, TRUE);
					}
				}
				else
				{
					Buzzer(FALSE, 0);

					m_bReAlign[1][3] = TRUE; pDoc->SetStatus(_T("General"), _T("bReAlign[1][3]"), m_bReAlign[1][3]);
					m_bSkipAlign[1][3] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][3]"), m_bSkipAlign[1][3]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(3) 으로 진행. - 카메라 재정렬
					Stop();
					TowerLamp(RGB_YELLOW, TRUE);
				}
			}

			if (m_bFailAlign[0][3] || m_bFailAlign[1][3])
			{
				if (!m_bReAlign[0][3] && !m_bReAlign[1][3])
				{
					if (m_bDoMk[0] || m_bDoMk[1])
						m_nMkStAuto++;//m_nMkStAuto = MK_ST + 29;  // MoveInitPos0()
					else
					{
						if (!IsInitPos0())
							MoveInitPos0();
						if (!IsInitPos1())
							MoveInitPos1();

						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // // 불량이미지 Display, Align변수 초기화 (Skip 65 : Mk())
					}
				}
				else
				{
					m_nMkStAuto = MK_ST + (Mk4PtIdx::Move3Cam1); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // FourPointAlign1(3) 으로 진행. - 카메라 재정렬
				}
			}
			else
				m_nMkStAuto = MK_ST + (Mk4PtIdx::MoveInitPt); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);

			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtMoveInitPos()
{
	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::MoveInitPt) :
			MoveInitPos0();
			MoveInitPos1(FALSE);
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::MoveInitPt) + 1:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::MoveInitPt) + 2:
			if (IsMoveDone())
			{
				m_nMkStAuto = MK_ST + (Mk4PtIdx::ChkElec); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			}
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtElecChk()
{
	CString sRst;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::ChkElec) : // DoElecChk
			if (DoElecChk(sRst))
			{
				if (pDoc->WorkingInfo.Probing[0].bUse)
				{
					if (sRst == _T("Open"))
					{
						if (pDoc->WorkingInfo.Probing[0].bStopOnOpen)
						{
							m_nMkStAuto = REJECT_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
						}
						else
							m_nMkStAuto++;
					}
					else if (sRst == _T("Error"))
					{
						m_nMkStAuto = ERROR_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
					else
					{
						m_nMkStAuto++;
					}
				}
				else
					m_nMkStAuto++;
			}
		   break;

		case MK_ST + (Mk4PtIdx::ChkElec) + 1:
			if (ChkLightErr())
			{
				m_bChkLightErr = FALSE;
				m_nMkStAuto++;
			}
			else
			{
				m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Mk 마킹 시작
			}
			break;

		case MK_ST + (Mk4PtIdx::ChkElec) + 2:
			if (IsRun())
			{
				if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0) &&
					m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
				{
					if (MODE_INNER != pDoc->GetTestMode())
					{
						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); 	// Mk 마킹 시작
					}
					else
					{
						m_nMkStAuto = MK_ST + (Mk4PtIdx::Shift2Mk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
				}
				else
				{
					Stop();
					MsgBox(_T("마킹부 모션이 비활성화 되었습니다."));
					TowerLamp(RGB_RED, TRUE);
				}
			}
			else
			{
				if (!m_bChkLightErr)
				{
					m_bChkLightErr = TRUE;
					MsgBox(_T("노광불량 정지 - 기판을 확인하세요.\r\n계속진행하려면 운전스위치를 누르세요."));
				}
			}
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtDoMarking()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sRst, sMsg, sItem;
	int a, b, nSerial, nPrevSerial;
	BOOL bChk = FALSE;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::DoMk) :				// Mk 마킹 시작
			if (!IsRun())
				break;
			if (CheckMkPnt())
			{
				pDoc->WorkingInfo.LastJob.sProcessNum = pDoc->GetProcessNum(); // for DTS

				if (pDoc->GetTestMode() == MODE_OUTER)
					SetMkIts(TRUE);						// ITS 마킹 시작
				else
					SetMk(TRUE);						// Mk 마킹 시작

				m_nMkStAuto++;
			}
			else
			{
				MsgBox(_T("캠마스터의 마킹위치좌표가 설정되어 있지않습니다.\r\n확인하세요."));
				Stop();
				TowerLamp(RGB_RED, TRUE);
				m_nMkStAuto = MK_ST + (Mk2PtIdx::Move0Cam1);
			}
			break;

		case MK_ST + (Mk4PtIdx::DoMk) + 1:
			if (!m_bUpdateYield)
			{
				if (!m_bTHREAD_UPDATAE_YIELD[0] && !m_bTHREAD_UPDATAE_YIELD[1])	// Left Shot의 수율과 Right Shot의 수율을 업데이트함.
				{
					if (!m_bTHREAD_UPDATE_YIELD_UP && !m_bTHREAD_UPDATE_YIELD_DN && !m_bTHREAD_UPDATE_YIELD_INNER_UP && !m_bTHREAD_UPDATE_YIELD_INNER_DN
						&& !m_bTHREAD_UPDATE_YIELD_ALLUP && !m_bTHREAD_UPDATE_YIELD_ALLDN && !m_bTHREAD_UPDATE_YIELD_INNER_ALLUP && !m_bTHREAD_UPDATE_YIELD_INNER_ALLDN)	// Left Shot의 수율과 Right Shot의 수율을 업데이트함.
					{
						m_bUpdateYield = TRUE; pDoc->SetStatus(_T("General"), _T("bUpdateYield"), m_bUpdateYield);
						UpdateYield(); // Cam[0],  Cam[1]
						m_nMkStAuto++;
					}
				}
			}
			else
			{
				Sleep(100);
				m_nMkStAuto++;
			}
			break;

		case MK_ST + (Mk4PtIdx::Verify) :
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !m_nPrevMkStAuto)
				{
					m_nPrevMkStAuto = MK_ST + (Mk4PtIdx::Verify); pDoc->SetStatusInt(_T("General"), _T("nPrevMkStAuto"), m_nPrevMkStAuto);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);		// Mk 마킹 시작
					m_bCam = TRUE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
				}
				else
				{
					if (IsReview())
					{
						if (!m_bCam)
						{
							m_nPrevStepAuto = MK_ST + (Mk4PtIdx::Verify); pDoc->SetStatusInt(_T("General"), _T("nPrevStepAuto"), m_nPrevStepAuto);
							m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);		// Mk 마킹 시작
							m_bCam = TRUE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
							MsgBox(_T("▶ Jog 버튼을 이용하여 마킹위치를 확인하여 주세요."));
						}
						else
						{
							m_bCam = FALSE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
							m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Mk 마킹 완료
							if (m_bInitAuto)
								ShowLive(FALSE);
						}
					}
					else
					{
						m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Mk 마킹 완료
						if (m_bInitAuto)
							ShowLive(FALSE);
					}
				}
			}
			else if (IsReMk())
			{
				m_nPrevMkStAuto = MK_ST + (Mk4PtIdx::Verify); pDoc->SetStatusInt(_T("General"), _T("nPrevMkStAuto"), m_nPrevMkStAuto);
				m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);		// Mk 재시작
			}
			else
			{
				sMsg = _T("");
				sMsg += m_sDispSts[0];
				sMsg += _T(",");
				sMsg += m_sDispSts[1];
			}
			break;

		case MK_ST + (Mk4PtIdx::DoneMk) :	 // Align변수 초기화
			if (!IsRun())
				break;

			if (m_bInitAuto)
			{
				m_bInitAuto = FALSE; pDoc->SetStatus(_T("General"), _T("bInitAuto"), m_bInitAuto);
				Stop();
				MsgBox(_T("마킹위치를 확인하세요."));
				TowerLamp(RGB_YELLOW, TRUE);
				break;
			}

			if (m_nBufUpSerial[0] == 0)
			{
				m_bSkipAlign[0][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]);
				m_bSkipAlign[0][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]);
			}
			if (m_nBufUpSerial[1] == 0)
			{
				m_bSkipAlign[1][0] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]);
				m_bSkipAlign[1][1] = TRUE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]);
			}

			if ((!m_bSkipAlign[0][0] && !m_bSkipAlign[0][1] && !m_bSkipAlign[0][2] && !m_bSkipAlign[0][3]) && (!m_bSkipAlign[1][0] && !m_bSkipAlign[1][1] && !m_bSkipAlign[1][2] && !m_bSkipAlign[1][3]))
			{
				if (m_bMkSt[1])
					CompletedMk(2); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
				else
					CompletedMk(0); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
			}
			else if ((m_bSkipAlign[0][0] || m_bSkipAlign[0][1] || m_bSkipAlign[0][2] || m_bSkipAlign[0][3]) && (!m_bSkipAlign[1][0] && !m_bSkipAlign[1][2] && !m_bSkipAlign[1][1] && !m_bSkipAlign[1][3]))
			{
				if (m_bMkSt[1])
					CompletedMk(1); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
				else
					CompletedMk(3); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
			}
			else if ((!m_bSkipAlign[0][0] && !m_bSkipAlign[0][1] && !m_bSkipAlign[0][2] && !m_bSkipAlign[0][3]) && (m_bSkipAlign[1][0] || m_bSkipAlign[1][1] || m_bSkipAlign[1][2] || m_bSkipAlign[1][3]))
				CompletedMk(0); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None
			else
				CompletedMk(3); // 0: Only Cam0, 1: Only Cam1, 2: Cam0 and Cam1, 3: None

			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 1:  // Mk변수 초기화
			m_bReAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][0]"), m_bReAlign[0][0]); // [nCam][nPos] 
			m_bReAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][1]"), m_bReAlign[0][1]); // [nCam][nPos] 
			m_bReAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][2]"), m_bReAlign[0][2]); // [nCam][nPos] 
			m_bReAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReAlign[0][3]"), m_bReAlign[0][3]); // [nCam][nPos] 

			m_bSkipAlign[0][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][0]"), m_bSkipAlign[0][0]); // [nCam][nPos] 
			m_bSkipAlign[0][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[0][1]"), m_bSkipAlign[0][1]); // [nCam][nPos] 
			m_bSkipAlign[1][0] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][0]"), m_bSkipAlign[1][0]); // [nCam][nPos] 
			m_bSkipAlign[1][1] = FALSE;	pDoc->SetStatus(_T("General"), _T("bSkipAlign[1][1]"), m_bSkipAlign[1][1]); // [nCam][nPos] 

			m_bFailAlign[0][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][0]"), m_bFailAlign[0][0]); // [nCam][nPos] 
			m_bFailAlign[0][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[0][1]"), m_bFailAlign[0][1]); // [nCam][nPos] 
			m_bFailAlign[1][0] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][0]"), m_bFailAlign[1][0]); // [nCam][nPos] 
			m_bFailAlign[1][1] = FALSE; pDoc->SetStatus(_T("General"), _T("bFailAlign[1][1]"), m_bFailAlign[1][1]); // [nCam][nPos] 

			m_bDoMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[0]"), m_bDoMk[0]);
			m_bDoMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoMk[1]"), m_bDoMk[1]);
			m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
			m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
			m_bReMark[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bReMark[0]"), m_bReMark[0]);
			m_bReMark[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bReMark[1]"), m_bReMark[1]);
			m_bCam = FALSE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
			m_nPrevMkStAuto = 0; pDoc->SetStatusInt(_T("General"), _T("nPrevMkStAuto"), m_nPrevMkStAuto);

			for (a = 0; a < 2; a++)
			{
				for (b = 0; b < MAX_STRIP_NUM; b++)
				{
					m_nMkStrip[a][b] = 0;
					m_bRejectDone[a][b] = FALSE;
					sItem.Format(_T("bRejectDone[%d][%d]"), a, b);
					pDoc->SetStatus(_T("General"), sItem, m_bRejectDone[a][b]);
				}
			}

			m_nSaveMk0Img = 0;
			m_nSaveMk1Img = 0;

			MpeWrite(Plc.DlgMenu01.MarkingDoing, 0);	// 마킹부 마킹중 ON (PC가 ON, OFF)
			MpeWrite(Plc.DlgMenu01.MarkingDone, 1);	// 마킹완료(PLC가 확인하고 Reset시킴.)-20141029

			MpeWrite(Plc.DlgMenu03.PcrMarkedSerialLeft, m_nBufUpSerial[0]);
			MpeWrite(Plc.DlgMenu03.PcrMarkedSerialRight, m_nBufUpSerial[1]);
			MpeWrite(Plc.DlgMenu03.FeedingReadyPunch, 1);

			pDoc->LogAuto(_T("PC: 마킹완료(PLC가 확인하고 Reset시킴.)"));
			if (IsNoMk() || IsShowLive())
				ShowLive(FALSE);

			m_nMkStAuto++;
			break;

		case MK_ST + (Mk4PtIdx::DoneMk) + 2:
			bChk = MpeRead(_T("MB400243")); // PLC 마킹 피딩 완료
											//if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[4] & (0x01 << 3))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
			if (bChk)	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
			{
				MpeWrite(_T("MB400243"), 0);// PLC 마킹 피딩 완료
				pDoc->LogAuto(_T("PLC: 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)"));
				m_nMkStAuto++;
			}

			break;

		case MK_ST + (Mk4PtIdx::DoneMk) + 3:
			if (!m_bTHREAD_SHIFT2MK)
			{
				MpeWrite(Plc.DlgMenu01.TableVacuumAoiUp, 0);	// 마킹부 Feeding완료
				pDoc->LogAuto(_T("PC: 마킹부 Feeding완료 OFF"));

				m_bShift2Mk = TRUE;
				DoShift2Mk();

				SetMkFdLen();
				SetCycTime();
				m_dwCycSt = GetTickCount();

				UpdateRst();

				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 4:
			//ChkLotCutPos();
			if (!m_bTHREAD_SHIFT2MK && !m_bTHREAD_REELMAP_YIELD_UP && !m_bTHREAD_REELMAP_YIELD_DN && !m_bTHREAD_REELMAP_YIELD_ALLUP && !m_bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
			{
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (m_bTHREAD_REELMAP_YIELD_ITS) // Yield Reelmap
						break;
				}

				m_nMkStAuto++;
				ChkYield();
			}
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 5:
			if (!m_bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				ChkLotCutPos();
				UpdateWorking();	// Update Working Info...
				m_nMkStAuto++;
			}
			//m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 6:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::DoneMk) + 7:
			m_nMkStAuto++;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;

		case MK_ST + (Mk4PtIdx::DoneMk) + 8:
			m_bMkSt[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt0"), m_bMkSt[0]);
			m_bMkSt[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt1"), m_bMkSt[1]);
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtLotDiff()
{
	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::LotDiff) :
			Stop();
			TowerLamp(RGB_YELLOW, TRUE);
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::LotDiff) + 1:
			if (IDYES == MsgBox(_T("상면과 하면의 Lot가 다릅니다.\r\n계속 작업을 진행하시겠습니까?"), 0, MB_YESNO))
			{
				m_bContDiffLot = TRUE; pDoc->SetStatus(_T("General"), _T("bContDiffLot"), m_bContDiffLot);
			}
			else
			{
				m_bContDiffLot = FALSE; pDoc->SetStatus(_T("General"), _T("bContDiffLot"), m_bContDiffLot);
				m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
				m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
			}
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::LotDiff) + 2:
			if (IsRun())
			{
				if (m_bContDiffLot)
				{
					m_nMkStAuto = MK_ST + (Mk4PtIdx::ChkSn); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				}
				else
					m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::LotDiff) + 3:
			m_bContDiffLot = FALSE; pDoc->SetStatus(_T("General"), _T("bContDiffLot"), m_bContDiffLot);
			m_bLotEnd = TRUE; pDoc->SetStatus(_T("General"), _T("bLotEnd"), m_bLotEnd);
			m_nLotEndAuto = LOT_END; pDoc->SetStatusInt(_T("General"), _T("nLotEndAuto"), m_nLotEndAuto);
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtReject()
{
	int a, b;
	CString sItem;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case REJECT_ST:
			Buzzer(TRUE, 0);
			TowerLamp(RGB_RED, TRUE);
			Stop();

			if (IDYES == MsgBox(_T("쇼트 체크 불량입니다.\r\n리젝 처리를 진행하시겠습니까?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[0]"), m_bAnswer[0]);
				m_nMkStAuto++;
			}
			else
			{
				if (IDYES == MsgBox(_T("리젝 처리를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[1]"), m_bAnswer[1]);
					m_nMkStAuto++;
				}
			}

			Buzzer(FALSE, 0);
			break;
		case REJECT_ST + 1:
			if (IsRun())
			{
				if (m_bAnswer[0])
				{
					m_bAnswer[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[0]"), m_bAnswer[0]);
					m_nMkStAuto++;
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[1]"), m_bAnswer[1]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Mk 마킹 시작
				}
				else
				{
					m_nMkStAuto = REJECT_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				}
			}
			break;
		case REJECT_ST + 2:
			SetReject();
			m_nMkStAuto++;
			break;
		case REJECT_ST + 3:
			m_nMkStAuto++;
			break;
		case REJECT_ST + 4:
			if (IsMkDone() && IsMoveDone())
			{
				if (IsVerify() && !m_nPrevMkStAuto)
				{
					m_nPrevMkStAuto = REJECT_ST + 4; pDoc->SetStatusInt(_T("General"), _T("nPrevMkStAuto"), m_nPrevMkStAuto);
					m_nMkStAuto = REJECT_ST + 2; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);		// Mk 마킹 시작
					m_bCam = TRUE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);

					m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
					m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);

					for (a = 0; a < 2; a++)
					{
						for (b = 0; b < MAX_STRIP_NUM; b++)
						{
							m_nMkStrip[a][b] = 0;
							m_bRejectDone[a][b] = FALSE;
							sItem.Format(_T("bRejectDone[%d][%d]"), a, b);
							pDoc->SetStatus(_T("General"), sItem, m_bRejectDone[a][b]);
						}
					}
				}
				else
				{
					if (IsReview())
					{
						if (!m_bCam)
						{
							m_nPrevStepAuto = REJECT_ST + 4; pDoc->SetStatusInt(_T("General"), _T("nPrevStepAuto"), m_nPrevStepAuto);
							m_nMkStAuto = REJECT_ST + 2; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);		// Mk 마킹 시작
							m_bCam = TRUE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
							MsgBox(_T("▶ Jog 버튼을 이용하여 마킹위치를 확인하여 주세요."));
						}
						else
						{
							m_bCam = FALSE; pDoc->SetStatus(_T("General"), _T("bCam"), m_bCam);
							m_nMkStAuto++;	// Mk 마킹 완료
						}
					}
					else
					{
						m_nMkStAuto++;	// Mk 마킹 완료
					}
				}
			}
			break;
		case REJECT_ST + 5:
			m_nMkStAuto = MK_ST + (Mk4PtIdx::DoneMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);				// Align변수 초기화
			break;
		}
	}
}

void CGvisR2R_PunchView::Mk4PtErrStop()
{
	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case ERROR_ST:
			Stop();
			TowerLamp(RGB_RED, TRUE);

			if (IDYES == MsgBox(_T("쇼트 체크 Error입니다.\r\n다시 쇼트 체크를 진행하시겠습니까?"), 0, MB_YESNO))
			{
				m_bAnswer[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[0]"), m_bAnswer[0]);
				m_nMkStAuto++;
			}
			else
			{
				if (IDYES == MsgBox(_T("쇼트 체크를 취소하였습니다.\r\n불량만 마킹 처리를 진행하시겠습니까?"), 0, MB_YESNO))
				{
					m_bAnswer[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[1]"), m_bAnswer[1]);
					m_nMkStAuto++;
				}
				else
					m_nMkStAuto++;
			}
			break;
		case ERROR_ST + 1:
			m_nMkStAuto++;
			break;
		case ERROR_ST + 2:
			if (IsRun())
			{
				if (m_bAnswer[0])
				{
					m_bAnswer[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[0]"), m_bAnswer[0]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::ChkElec); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto); // DoElecChk
				}
				else if (m_bAnswer[1])
				{
					m_bAnswer[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[1]"), m_bAnswer[1]);
					m_nMkStAuto = MK_ST + (Mk4PtIdx::DoMk); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);	// Mk 마킹 시작
				}
				else
					m_nMkStAuto++;
			}
			break;
		case ERROR_ST + 3:
			m_nMkStAuto = ERROR_ST; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
			break;
		}
	}
}

void CGvisR2R_PunchView::MarkingWith4PointAlign()
{
	Mk4PtReady();
	Mk4PtChkSerial();
	Mk4PtInit();
	Mk4PtAlignPt0();
	Mk4PtAlignPt1();
	Mk4PtAlignPt2();
	Mk4PtAlignPt3();
	Mk4PtMoveInitPos();
	Mk4PtElecChk();
	if (MODE_INNER != pDoc->GetTestMode())
		Mk4PtDoMarking();
	else
		Mk4PtShift2Mk();
	Mk4PtLotDiff();
	Mk4PtReject();
	Mk4PtErrStop();
}

BOOL CGvisR2R_PunchView::IsSameUpDnLot()
{
	if (pDoc->Status.PcrShare[0].sLot == pDoc->Status.PcrShare[1].sLot)
		return TRUE;

	return FALSE;
}

int CGvisR2R_PunchView::GetAutoStep()
{
	return m_nStepAuto;
}

void CGvisR2R_PunchView::MoveMkInitPos()
{
	MoveMk0InitPos();
	MoveMk1InitPos();
}

void CGvisR2R_PunchView::MoveMk0InitPos()
{
	double pTgtPos[2];
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
	double dCurrX = pView->m_dEnc[AXIS_X0];
	double dCurrY = pView->m_dEnc[AXIS_Y0];

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt(((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		if (pView->IsClampOff())
			return;

		pView->m_pMotion->GetSpeedProfile0(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
		if (!pView->m_pMotion->Move0(MS_X0Y0, pTgtPos, fVel / 2.0, fAcc / 2.0, fAcc / 2.0))
		{
			if (!pView->m_pMotion->Move0(MS_X0Y0, pTgtPos, fVel / 2.0, fAcc / 2.0, fAcc / 2.0))
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Move X0Y0 Error..."));
			}
		}
	}
}

void CGvisR2R_PunchView::MoveMk1InitPos()
{
	double pTgtPos[2];
	pTgtPos[1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
	pTgtPos[0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
	double dCurrX = pView->m_dEnc[AXIS_X1];
	double dCurrY = pView->m_dEnc[AXIS_Y1];

	double fLen, fVel, fAcc, fJerk;
	fLen = sqrt(((pTgtPos[0] - dCurrX) * (pTgtPos[0] - dCurrX)) + ((pTgtPos[1] - dCurrY) * (pTgtPos[1] - dCurrY)));
	if (fLen > 0.001)
	{
		if (pView->IsClampOff())
			return;

		pView->m_pMotion->GetSpeedProfile1(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
		if (!pView->m_pMotion->Move1(MS_X1Y1, pTgtPos, fVel / 2.0, fAcc / 2.0, fAcc / 2.0))
		{
			if (!pView->m_pMotion->Move1(MS_X1Y1, pTgtPos, fVel / 2.0, fAcc / 2.0, fAcc / 2.0))
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Move X1Y1 Error..."));
			}
		}
	}
}


BOOL CGvisR2R_PunchView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message != WM_KEYDOWN)
		return CFormView::PreTranslateMessage(pMsg);

	if ((pMsg->lParam & 0x40000000) == 0)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			m_bEscape = TRUE;
			return TRUE;
		case 'S':
		case 's':
			if (GetKeyState(VK_CONTROL) < 0) // Ctrl 키가 눌려진 상태
			{
				WINDOWPLACEMENT wndPlace;
				AfxGetMainWnd()->GetWindowPlacement(&wndPlace);
				wndPlace.showCmd |= SW_MAXIMIZE;
				AfxGetMainWnd()->SetWindowPlacement(&wndPlace);
			}
			break;
		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}


int CGvisR2R_PunchView::MyPassword(CString strMsg, int nCtrlId)
{
	CDlgMyPassword dlg1(this);
	dlg1.SetMsg(strMsg, nCtrlId);
	dlg1.DoModal();
	return (dlg1.m_nRtnVal);

}

BOOL CGvisR2R_PunchView::ReloadReelmapInner()
{
	double dRatio = 0.0;
	CString sVal = _T("");

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	BOOL bRtn[7] = { 1,1,1,1,1,1,1 };

	if (GetLastBufferSerial(0) < 1)
		return FALSE;

	if (pDoc->m_pReelMapInnerUp)
	{
		bRtn[1] = pDoc->m_pReelMapInnerUp->ReloadReelmapFromThread(GetLastBufferSerial(0));
	}

	if (pDoc->m_pReelMapIts)
	{
		bRtn[2] = pDoc->m_pReelMapIts->ReloadReelmapFromThread(GetLastBufferSerial(1));
	}

	bRtn[3] = TRUE;

	if (bDualTest)
	{
		if (pDoc->m_pReelMapInnerDn)
		{
			bRtn[4] = pDoc->m_pReelMapInnerDn->ReloadReelmapFromThread(GetLastBufferSerial(1));
		}
		if (pDoc->m_pReelMapInnerAllUp)
		{
			bRtn[5] = pDoc->m_pReelMapInnerAllUp->ReloadReelmapFromThread(GetLastBufferSerial(1));
		}
		if (pDoc->m_pReelMapInnerAllDn)
		{
			bRtn[6] = pDoc->m_pReelMapInnerAllDn->ReloadReelmapFromThread(GetLastBufferSerial(1));
		}
	}
	else
	{
		bRtn[4] = TRUE;
		bRtn[5] = TRUE;
		bRtn[6] = TRUE;
	}

	for (int i = 1; i < 7; i++)
	{
		if (!bRtn[i])
			return FALSE;
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::IsDoneReloadReelmapInner(int& nProc)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	int nRatio[7] = { 0 };
	BOOL bDone[7] = { 0 };

	bDone[0] = TRUE;

	if (pDoc->m_pReelMapInnerUp)
	{
		nRatio[1] = pDoc->m_pReelMapInnerUp->GetProgressReloadReelmap();
		bDone[1] = pDoc->m_pReelMapInnerUp->IsDoneReloadReelmap();
	}
	else
		bDone[1] = TRUE;

	if (pDoc->m_pReelMapIts)
	{
		nRatio[2] = pDoc->m_pReelMapIts->GetProgressReloadReelmap();
		bDone[2] = pDoc->m_pReelMapIts->IsDoneReloadReelmap();
	}
	else
		bDone[2] = TRUE;

	bDone[3] = TRUE;

	if (bDualTest)
	{
		if (pDoc->m_pReelMapInnerDn)
		{
			nRatio[4] = pDoc->m_pReelMapInnerDn->GetProgressReloadReelmap();
			bDone[4] = pDoc->m_pReelMapInnerDn->IsDoneReloadReelmap();
		}
		else
			bDone[4] = TRUE;

		if (pDoc->m_pReelMapInnerAllUp)
		{
			nRatio[5] = pDoc->m_pReelMapInnerAllUp->GetProgressReloadReelmap();
			bDone[5] = pDoc->m_pReelMapInnerAllUp->IsDoneReloadReelmap();
		}
		else
			bDone[5] = TRUE;

		if (pDoc->m_pReelMapAllDn)
		{
			nRatio[6] = pDoc->m_pReelMapInnerAllDn->GetProgressReloadReelmap();
			bDone[6] = pDoc->m_pReelMapInnerAllDn->IsDoneReloadReelmap();
		}
		else
			bDone[6] = TRUE;
	}
	else
	{
		bDone[4] = TRUE;
		bDone[5] = TRUE;
		bDone[6] = TRUE;
	}

	nProc = nRatio[0] + nRatio[1] + nRatio[2] + nRatio[3] + nRatio[4] + nRatio[5] + nRatio[6];

	if (bDone[0] && bDone[1] && bDone[2] && bDone[3] && bDone[4] && bDone[5] && bDone[6])
		return TRUE;

	return FALSE;
}


BOOL CGvisR2R_PunchView::IsDoneReloadReelmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bDualTestInner = pDoc->WorkingInfo.LastJob.bDualTestInner;


	BOOL bRtn[5] = { 0 };
	int nSt = 0;

	if (pDoc->m_pReelMapUp)
		bRtn[1] = pDoc->m_pReelMapUp->m_bThreadAliveReloadReelmap;

	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			bRtn[2] = pDoc->m_pReelMapDn->m_bThreadAliveReloadReelmap;

		if (pDoc->m_pReelMapAllUp)
			bRtn[3] = pDoc->m_pReelMapAllUp->m_bThreadAliveReloadReelmap;

		if (pDoc->m_pReelMapAllDn)
			bRtn[4] = pDoc->m_pReelMapAllDn->m_bThreadAliveReloadReelmap;
	}
	else
	{
		bRtn[2] = FALSE;
		bRtn[3] = FALSE;
		bRtn[4] = FALSE;
	}

	if (bRtn[1] || bRtn[2] || bRtn[3] || bRtn[4])
		return FALSE;

	return TRUE;
}

BOOL CGvisR2R_PunchView::ReloadReelmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bDualTestInner = pDoc->WorkingInfo.LastJob.bDualTestInner;

	int nLastShot = _tstoi(pDoc->WorkingInfo.LastJob.sMkLastShot);
	
	if (nLastShot <= 0)
		return TRUE;

	BOOL bRtn[5] = { 1,1,1,1,1 };
	int nSt = 0;

	if (pDoc->m_pReelMapUp)
		bRtn[1] = pDoc->m_pReelMapUp->ReloadReelmapFromThread(nLastShot);

	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			bRtn[2] = pDoc->m_pReelMapDn->ReloadReelmapFromThread(nLastShot);

		if (pDoc->m_pReelMapAllUp)
			bRtn[3] = pDoc->m_pReelMapAllUp->ReloadReelmapFromThread(nLastShot);

		if (pDoc->m_pReelMapAllDn)
			bRtn[4] = pDoc->m_pReelMapAllDn->ReloadReelmapFromThread(nLastShot);
	}
	else
	{
		bRtn[2] = TRUE;
		bRtn[3] = TRUE;
		bRtn[4] = TRUE;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		bRtn[0] = ReloadReelmapInner();
	}
	else
		nSt = 1;

	for (int i = nSt; i < 5; i++)
	{
		if (!bRtn[i])
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("ReloadReelmap() is Failed."));
			return FALSE;
		}
	}

	double dRatio = 0.0;
	CString sVal = _T("");
	CDlgProgress dlg;
	sVal.Format(_T("On Reloading Reelmap."));
	dlg.Create(sVal);

	int nRatio[5] = { 0 };
	BOOL bDone[5] = { 0 };
	int nTo = 0;
	if (bDualTest)
	{
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if (bDualTestInner)
				nTo = 400 + 500; //[%]
			else
				nTo = 400 + 200; //[%]
		}
		else
		{
			nTo = 400; //[%]
		}
	}
	else
	{
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if (bDualTestInner)
				nTo = 100 + 500; //[%]
			else
				nTo = 100 + 200; //[%]
		}
		else
		{
			nTo = 100; //[%]
		}
	}


	dlg.SetRange(0, nTo);
	dlg.SetPos(1);

	int nProc=0, nProcOutter=0, nProcInner=0;
	for (nProc = 0; nProc < nTo;)
	{
		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if ( (bDone[0] = IsDoneReloadReelmap(nProcOutter)) && (bDone[1] = IsDoneReloadReelmapInner(nProcInner)) )
				break;
			nProc = nProcOutter + nProcInner;
		}
		else
		{
			if (bDone[0] = IsDoneReloadReelmap(nProc))
				break;
		}
		dlg.SetPos(nProc);
		Sleep(30);
	}

	dlg.DestroyWindow();	


	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (!bDone[0] || !bDone[1])
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("IsDoneReloadReelmap or IsDoneReloadReelmapInner is FALSE."));
			return FALSE;
		}
	}
	else
	{
		if (!bDone[0])
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("IsDoneReloadReelmap is FALSE."));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::IsDoneReloadReelmap(int& nProc)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nRatio[5] = { 0 };
	BOOL bDone[5] = { 0 };

	if (pDoc->m_pReelMapUp)
	{
		nRatio[1] = pDoc->m_pReelMapUp->GetProgressReloadReelmap();
		bDone[1] = pDoc->m_pReelMapUp->IsDoneReloadReelmap();
	}
	else
		bDone[1] = TRUE;

	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
		{
			nRatio[2] = pDoc->m_pReelMapDn->GetProgressReloadReelmap();
			bDone[2] = pDoc->m_pReelMapDn->IsDoneReloadReelmap();
		}
		else
			bDone[2] = TRUE;
		if (pDoc->m_pReelMapAllUp)
		{
			nRatio[3] = pDoc->m_pReelMapAllUp->GetProgressReloadReelmap();
			bDone[3] = pDoc->m_pReelMapAllUp->IsDoneReloadReelmap();
		}
		else
			bDone[3] = TRUE;

		if (pDoc->m_pReelMapAllDn)
		{
			nRatio[4] = pDoc->m_pReelMapAllDn->GetProgressReloadReelmap();
			bDone[4] = pDoc->m_pReelMapAllDn->IsDoneReloadReelmap();
		}
		else
			bDone[4] = TRUE;
	}
	else
	{
		bDone[2] = TRUE;
		bDone[3] = TRUE; 
		bDone[4] = TRUE;
	}

	nProc = nRatio[1] + nRatio[2] + nRatio[3] + nRatio[4];

	if (bDone[1] && bDone[2] && bDone[3] && bDone[4])
		return TRUE;

	return FALSE;
}

void CGvisR2R_PunchView::ReloadReelmapUp()
{
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->ReloadReelmap();
}

void CGvisR2R_PunchView::ReloadReelmapAllUp()
{
	if (pDoc->m_pReelMapAllUp)
		pDoc->m_pReelMapAllUp->ReloadReelmap();
}

void CGvisR2R_PunchView::ReloadReelmapDn()
{
	if (pDoc->m_pReelMapDn)
		pDoc->m_pReelMapDn->ReloadReelmap();
}

void CGvisR2R_PunchView::ReloadReelmapAllDn()
{
	if (pDoc->m_pReelMapAllDn)
		pDoc->m_pReelMapAllDn->ReloadReelmap();
}

BOOL CGvisR2R_PunchView::ReloadReelmap(int nSerial)
{
	if (nSerial < 1)
		return TRUE;

	m_nReloadReelmapSerial = nSerial;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	m_bTHREAD_RELOAD_RST_UP = TRUE;
	if (bDualTest)
	{
		m_bTHREAD_RELOAD_RST_DN = TRUE;
		m_bTHREAD_RELOAD_RST_ALLUP = TRUE;
		m_bTHREAD_RELOAD_RST_ALLDN = TRUE;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		m_bTHREAD_RELOAD_RST_UP_INNER = TRUE;
		m_bTHREAD_RELOAD_RST_ITS = TRUE;
		if (pDoc->WorkingInfo.LastJob.bDualTestInner)
		{
			m_bTHREAD_RELOAD_RST_DN_INNER = TRUE;
			m_bTHREAD_RELOAD_RST_ALLUP_INNER = TRUE;
			m_bTHREAD_RELOAD_RST_ALLDN_INNER = TRUE;
		}
	}

	Sleep(100);

	return TRUE;
}


void CGvisR2R_PunchView::ReloadReelmapUpInner()
{
	if (pDoc->m_pReelMapInnerUp)
		pDoc->m_pReelMapInnerUp->ReloadReelmap();
}

void CGvisR2R_PunchView::ReloadReelmapAllUpInner()
{
	if (pDoc->m_pReelMapInnerAllUp)
		pDoc->m_pReelMapInnerAllUp->ReloadReelmap();
}

void CGvisR2R_PunchView::ReloadReelmapDnInner()
{
	if (pDoc->m_pReelMapInnerDn)
		pDoc->m_pReelMapInnerDn->ReloadReelmap();
}

void CGvisR2R_PunchView::ReloadReelmapAllDnInner()
{
	if (pDoc->m_pReelMapInnerAllDn)
		pDoc->m_pReelMapInnerAllDn->ReloadReelmap();
}

void CGvisR2R_PunchView::ReloadReelmapIts()
{
	if (pDoc->m_pReelMapIts)
		pDoc->m_pReelMapIts->ReloadReelmap();
}


BOOL CGvisR2R_PunchView::OpenReelmapFromBuf(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	stModelInfo stInfoUp, stInfoDn;
	CString sSrc;
	sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);
	if (!pDoc->GetPcrInfo(sSrc, stInfoUp))
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->DispStsBar(_T("E(6)"), 5);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error-GetPcrInfo(6)"));
		return FALSE;
	}
	if (bDualTest) 
	{
		sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufDn, nSerial);
		if (!pDoc->GetPcrInfo(sSrc, stInfoDn))
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->DispStsBar(_T("E(7)"), 5);
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error-GetPcrInfo(7)"));
			return FALSE;
		}
	}

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->Open();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->Open();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->Open();
	}

	return TRUE;
}

void CGvisR2R_PunchView::OpenReelmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->Open();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->Open();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->Open();
	}
}

void CGvisR2R_PunchView::OpenReelmapUp()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->Open();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->Open();
	}
}

void CGvisR2R_PunchView::OpenReelmapDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	if (pDoc->m_pReelMapDn)
		pDoc->m_pReelMapDn->Open();
	if (pDoc->m_pReelMapAllDn)
		pDoc->m_pReelMapAllDn->Open();
}

void CGvisR2R_PunchView::EStop()
{
	if (m_pMotion)
	{
		m_pMotion->EStop(MS_X0Y0);
		m_pMotion->EStop(MS_X1Y1);
		Sleep(30);
		ResetMotion(MS_X0Y0);
		Sleep(30);
		ResetMotion(MS_X1Y1);
		Sleep(30);
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("X축 충돌 범위에 의한 정지입니다."));

		double dCurrX = pView->m_dEnc[AXIS_X1];
		double dCurrY = pView->m_dEnc[AXIS_Y1];
		double pPos[2], fVel, fAcc, fJerk;
		double fLen = 2.0;
		pPos[0] = dCurrX + fLen;
		pPos[1] = dCurrY;
		m_pMotion->GetSpeedProfile1(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
		m_pMotion->Move1(MS_X1Y1, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT);
	}
}

void CGvisR2R_PunchView::ResetPriority()
{
	m_bPriority[0] = FALSE;
	m_bPriority[1] = FALSE;
	m_bPriority[2] = FALSE;
	m_bPriority[3] = FALSE;
}

void CGvisR2R_PunchView::SetPriority()
{
	int nDir[2];
	nDir[0] = (m_dTarget[AXIS_X0] - m_dEnc[AXIS_X0]) >= 0.0 ? 1 : -1;
	nDir[1] = (m_dTarget[AXIS_X1] - m_dEnc[AXIS_X1]) >= 0.0 ? 1 : -1;

	// Cam0 : m_bPriority[0], m_bPriority[3]
	// Cam1 : m_bPriority[1], m_bPriority[2]
	if (pDoc->m_Master[0].MasterInfo.nActionCode == 1 || pDoc->m_Master[0].MasterInfo.nActionCode == 3) // 1 : 좌우 미러 , 3 : 180 회전
	{
		if (nDir[0] < 0) // Cam0 <-
		{
			m_bPriority[0] = TRUE;
			m_bPriority[1] = FALSE;
			m_bPriority[2] = FALSE;
			m_bPriority[3] = FALSE;
		}
		else if (nDir[1] > 0) // Cam1 ->
		{
			m_bPriority[0] = FALSE;
			m_bPriority[1] = TRUE;
			m_bPriority[2] = FALSE;
			m_bPriority[3] = FALSE;
		}
		else if (nDir[0] > 0)// Cam0 ->
		{
			m_bPriority[0] = FALSE;
			m_bPriority[1] = FALSE;
			m_bPriority[2] = FALSE;
			m_bPriority[3] = TRUE;
		}
		else // Cam1 <-
		{
			m_bPriority[0] = FALSE;
			m_bPriority[1] = FALSE;
			m_bPriority[2] = TRUE;
			m_bPriority[3] = FALSE;
		}
	}
	else
	{
		if (nDir[1] > 0) // Cam1 ->
		{
			m_bPriority[0] = FALSE;
			m_bPriority[1] = TRUE;
			m_bPriority[2] = FALSE;
			m_bPriority[3] = FALSE;
		}
		else if (nDir[0] < 0) // Cam0 <-
		{
			m_bPriority[0] = TRUE;
			m_bPriority[1] = FALSE;
			m_bPriority[2] = FALSE;
			m_bPriority[3] = FALSE;
		}
		else if (nDir[1] < 0) // Cam1 <-
		{
			m_bPriority[0] = FALSE;
			m_bPriority[1] = FALSE;
			m_bPriority[2] = TRUE;
			m_bPriority[3] = FALSE;
		}
		else // Cam0 ->
		{
			m_bPriority[0] = FALSE;
			m_bPriority[1] = FALSE;
			m_bPriority[2] = FALSE;
			m_bPriority[3] = TRUE;
		}
	}
}

BOOL CGvisR2R_PunchView::IsRunAxisX()
{
	if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_X1))
		return FALSE;
	return TRUE;
}

BOOL CGvisR2R_PunchView::ChkCollision()
{
	double dMg = _tstof(pDoc->WorkingInfo.Motion.sCollisionLength) - _tstof(pDoc->WorkingInfo.Motion.sCollisionMargin);

	if (m_bTHREAD_MK[0] || m_bTHREAD_MK[1] || m_bTHREAD_MK[2] || m_bTHREAD_MK[3])
	{
		if (pDoc->WorkingInfo.System.bNoMk || m_bCam)
			dMg += (_tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX) - _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX));
	}

	if ((m_dEnc[AXIS_X0] - dMg) > m_dEnc[AXIS_X1])
		return TRUE;

	if (m_dEnc[AXIS_X0] < -1.0 || m_dEnc[AXIS_X1] < -1.0)
		return TRUE;

	return FALSE;
}

BOOL CGvisR2R_PunchView::ChkCollision(int nAxisId, double dTgtPosX)
{
	double dMg = _tstof(pDoc->WorkingInfo.Motion.sCollisionLength) - _tstof(pDoc->WorkingInfo.Motion.sCollisionMargin);


	if (m_bTHREAD_MK[0] || m_bTHREAD_MK[1] || m_bTHREAD_MK[2] || m_bTHREAD_MK[3])
	{
		if (pDoc->WorkingInfo.System.bNoMk || m_bCam)
			dMg += (_tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX) - _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX));
	}

	if (nAxisId == AXIS_X0)
	{
		if (dTgtPosX > m_dEnc[AXIS_X1] + dMg)
			return TRUE;
		if (m_bDoMk[1] && !m_bDoneMk[1])
		{
			if (!m_bDoneMk[0] && m_bTHREAD_MK[0])
			{
				if (m_dEnc[AXIS_X1] > _tstof(pDoc->WorkingInfo.Motion.sSafeZone) - 2.0)
					return FALSE;

				if (m_dTarget[AXIS_X1] > 0.0)
				{
					if (dTgtPosX > m_dTarget[AXIS_X1] + dMg && m_dTarget[AXIS_X1] > 0.0)
						return TRUE;
					else if (m_dNextTarget[AXIS_X1] > 0.0)
					{
						if (dTgtPosX > m_dNextTarget[AXIS_X1] + dMg)
							return TRUE;
					}
				}
			}
		}
	}
	else if (nAxisId == AXIS_X1)
	{
		if (m_dEnc[AXIS_X0] < 2.0)
			return FALSE;

		if (dTgtPosX < m_dEnc[AXIS_X0] - dMg)
			return TRUE;
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::ChkCollision(int nAxisId, double dTgtPosX, double dTgtNextPosX)
{
	double dMg = _tstof(pDoc->WorkingInfo.Motion.sCollisionLength) - _tstof(pDoc->WorkingInfo.Motion.sCollisionMargin);

	if (m_bTHREAD_MK[0] || m_bTHREAD_MK[1] || m_bTHREAD_MK[2] || m_bTHREAD_MK[3])
	{
		if (pDoc->WorkingInfo.System.bNoMk || m_bCam)
			dMg += (_tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX) - _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX));
	}

	if (nAxisId == AXIS_X0)
	{
		if (dTgtPosX > m_dEnc[AXIS_X1] + dMg)
			return TRUE;
		if (m_bDoMk[1] && !m_bDoneMk[1])
		{
			if (!m_bDoneMk[0] && m_bTHREAD_MK[0])
			{
				if (m_dEnc[AXIS_X1] > _tstof(pDoc->WorkingInfo.Motion.sSafeZone) - 2.0)
					return FALSE;

				if (dTgtPosX > m_dTarget[AXIS_X1] + dMg && m_dTarget[AXIS_X1] > 0.0)
					return TRUE;
				else if (dTgtPosX > m_dNextTarget[AXIS_X1] + dMg && m_dNextTarget[AXIS_X1] > 0.0)
					return TRUE;
			}
		}
	}
	else if (nAxisId == AXIS_X1)
	{
		if (dTgtPosX < m_dEnc[AXIS_X0] - dMg)
			return TRUE;
		if (m_bDoMk[0])
		{
			if (!m_bDoneMk[1] && m_bTHREAD_MK[1])
			{
				if (!m_pMotion->IsMotionDone(MS_X0))
				{
					if (m_dEnc[AXIS_X0] < 2.0)
						return FALSE;

					if (dTgtPosX < m_dTarget[AXIS_X0] - dMg && m_dTarget[AXIS_X0] > 0.0)
						return TRUE;
					else if (dTgtPosX < m_dNextTarget[AXIS_X0] - dMg && m_dNextTarget[AXIS_X0] > 0.0)
						return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void CGvisR2R_PunchView::StopAllMk()
{
	// Mk0
	m_nStepMk[2] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);

	// Mk1
	m_nStepMk[3] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
}

void CGvisR2R_PunchView::DoAllMk(int nCam)
{
	if (nCam == CAM_LF)
	{
		double dCurrX = pView->m_dEnc[AXIS_X1];
		double dCurrY = pView->m_dEnc[AXIS_Y1];

		double pPos[2];
		pPos[0] = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
		pPos[1] = 0.0;

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
		if (fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
			if (!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
			{
				if (!pView->m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
				{
					pView->SetAlarmToPlc(UNIT_PUNCH);
					pView->ClrDispMsg();
					AfxMessageBox(_T("Move X1Y1 Error..."));
				}
			}
		}
		if (!m_bTHREAD_MK[2])
		{
			m_nStepMk[2] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
			m_nMkPcs[2] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[2]"), m_nMkPcs[2]);
			m_bTHREAD_MK[2] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[2]"), m_bTHREAD_MK[2]);
		}
		else
		{
			m_bTHREAD_MK[2] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[2]"), m_bTHREAD_MK[2]);
		}
	}
	else if (nCam == CAM_RT)
	{
		double dCurrX = pView->m_dEnc[AXIS_X0];
		double dCurrY = pView->m_dEnc[AXIS_Y0];

		double pPos[2];
		pPos[0] = 0.0;
		pPos[1] = 0.0;

		double fLen, fVel, fAcc, fJerk;
		fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
		if (fLen > 0.001)
		{
			pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
			if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
			{
				if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
				{
					pView->SetAlarmToPlc(UNIT_PUNCH);
					pView->ClrDispMsg();
					AfxMessageBox(_T("Move X0Y0 Error..."));
			}
		}
		}
		if (!m_bTHREAD_MK[3])
		{
			m_nStepMk[3] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
			m_nMkPcs[3] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[3]"), m_nMkPcs[3]);
			m_bTHREAD_MK[3] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[3]"), m_bTHREAD_MK[3]);
		}
		else
		{
			m_bTHREAD_MK[3] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[3]"), m_bTHREAD_MK[3]);
		}
	}
	else if (nCam == CAM_BOTH)
	{
		if (!m_bTHREAD_MK[2])
		{
			m_nStepMk[2] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
			m_nMkPcs[2] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[2]"), m_nMkPcs[2]);
			m_bTHREAD_MK[2] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[2]"), m_bTHREAD_MK[2]);
		}
		else
		{
			m_bTHREAD_MK[2] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[2]"), m_bTHREAD_MK[2]);
		}

		if (!m_bTHREAD_MK[3])
		{
			m_nStepMk[3] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
			m_nMkPcs[3] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[3]"), m_nMkPcs[3]);
			m_bTHREAD_MK[3] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[3]"), m_bTHREAD_MK[3]);
		}
		else
		{
			m_bTHREAD_MK[3] = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[3]"), m_bTHREAD_MK[3]);
		}
	}
}


void CGvisR2R_PunchView::InitPLC()
{
	long lData;
	lData = (long)(_tstof(pDoc->WorkingInfo.Lot.sTotalReelDist) * 1000.0);
	MpeWrite(Plc.DlgInfo.TotalReelLength, lData);	// 전체 Reel 길이 (단위 M * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Lot.sSeparateDist) * 1000.0);
	MpeWrite(Plc.DlgInfo.LotLength, lData);	// Lot 분리 길이 (단위 M * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Lot.sCuttingDist) * 1000.0);
	MpeWrite(Plc.DlgInfo.LotCutLength, lData);	// Lot 분리 후 절단위치 (단위 M * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Lot.sStopDist) * 1000.0);
	MpeWrite(Plc.DlgInfo.StopLength, lData);	// 일시정지 길이 (단위 M * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sAoiTq) * 1000.0);
	//MpeWrite(Plc.DlgMenu04.FeedTorqueCwAoi, lData);	// 검사부 Tension 모터 토크값 (단위 Kgf * 1000)
	MpeWrite(Plc.DlgMenu04.FeedTorqueCcwAoi, lData);	// 검사부 Tension 모터 토크값 (단위 Kgf * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkTq) * 1000.0);
	//MpeWrite(Plc.DlgMenu04.FeedTorqueCwPunch, lData);	// 마킹부 Tension 모터 토크값 (단위 Kgf * 1000) _T("ML45044")
	MpeWrite(Plc.DlgMenu04.FeedTorqueCcwPunch, lData);	// 마킹부 Tension 모터 토크값 (단위 Kgf * 1000) _T("ML45044")
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sEngraveTq) * 1000.0);
	//MpeWrite(Plc.DlgMenu04.FeedTorqueCwEngrave, lData);	// 각인부 Tension 모터 토크값 (단위 Kgf * 1000)
	MpeWrite(Plc.DlgMenu04.FeedTorqueCcwEngrave, lData);	// 각인부 Tension 모터 토크값 (단위 Kgf * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen) * 1000.0);
	MpeWrite(Plc.DlgInfo.OneShotLength, lData);	// 한 판넬 길이 (단위 mm * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdVel) * 1000.0);
	MpeWrite(Plc.DlgMenu04.FeedSpeed, lData);	// 한 판넬 Feeding 속도 (단위 mm/sec * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkJogVel) * 1000.0);
	MpeWrite(Plc.DlgMenu04.FeedOnSpeed, lData);	// 연속공급 속도 (단위 mm/sec * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sAoiJogAcc) * 1000.0);
	MpeWrite(Plc.DlgMenu04.FeedOnAcc, lData);	// 연속공급 가속도 (단위 mm/s^2 * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdAcc) * 1000.0);
	MpeWrite(Plc.DlgMenu04.FeedAcc, lData);	// 한 판넬 Feeding 가속도 (단위 mm/s^2 * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sAoiFdLead) * 1000.0);
	MpeWrite(Plc.DlgMenu04.FeedLeadPitchAoi, lData);	// 검사부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
	MpeWrite(Plc.DlgMenu04.FeedLeadPitchEngrave, lData);	// 각인부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdLead) * 1000.0);
	MpeWrite(Plc.DlgMenu04.FeedLeadPitchPunch, lData);	// 마킹부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sStBufPos) * 1000.0);
	MpeWrite(Plc.DlgMenu04.BufferPosStAoi, lData);	// 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist) * 1000.0);
	MpeWrite(Plc.DlgMenu04.FeedLengthFromAoiToPunch, lData);	// AOI(하)에서 마킹까지 거리 (단위 mm * 1000)
	lData = (long)(_tstoi(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * 1000);
	MpeWrite(Plc.DlgMenu04.ShotNumFromAoiUpToAoiDn, lData);	// AOI(상)에서 AOI(하) Shot수 (단위 Shot수 * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist) * 1000.0);
	MpeWrite(Plc.DlgMenu04.FeedLengthFromEngraveToAoi, lData);	// 각인부에서 AOI(상)까지 거리 (단위 mm * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdBarcodeOffsetVel) * 1000.0);
	//MpeWrite(Plc.DlgMenu04.FeedSpeed2dCode, lData);	// 2D 바코드 리더기위치까지 Feeding 속도 (단위 mm/sec * 1000)
	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdBarcodeOffsetAcc) * 1000.0);
	//MpeWrite(Plc.DlgMenu04.FeedAcc2dCode, lData);	// 2D 바코드 리더기위치까지 Feeding 가속도 (단위 mm/s^2 * 1000)

	lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim) * 100.0);
	//MpeWrite(Plc.DlgInfo.UltraSonicStTimeAoiDn, lData);	// AOI_Dn (단위 [초] * 100) : 1 is 10 mSec.
	//MpeWrite(Plc.DlgInfo.UltraSonicStTimeAoiUp, lData);	// AOI_Up (단위 [초] * 100) : 1 is 10 mSec.

	MpeWrite(Plc.DlgInfo.Laser380mm, (pDoc->WorkingInfo.LastJob.bUse380mm) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.Laser346mm, (pDoc->WorkingInfo.LastJob.bUse346mm) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.Laser340mm, (pDoc->WorkingInfo.LastJob.bUse340mm) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.UseEngrave, (pDoc->WorkingInfo.System.bUseEngrave) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.UseAoiUp, (pDoc->WorkingInfo.System.bUseAoiUp) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.UseAoiDn, (pDoc->WorkingInfo.System.bUseAoiDn) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.UsePunch, (pDoc->WorkingInfo.System.bUsePunch) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.UsePunchOnly, (pDoc->WorkingInfo.System.bUsePunchOnly) ? 1 : 0);
}

BOOL CGvisR2R_PunchView::SetCollision(double dCollisionMargin)
{
	BOOL bRtn = FALSE;
	if (m_pMotion)
		bRtn = m_pMotion->SetCollision(dCollisionMargin);
	return bRtn;
}

void CGvisR2R_PunchView::IoWrite(CString sMReg, long lData)
{
	int i = 0;
	for (i = 0; i<TOT_M_IO; i++)
	{
		if (pDoc->m_pIo[i].nIdx < 0)
		{
			pDoc->m_pIo[i].nIdx = i;
			pDoc->m_pIo[i].sAddr = sMReg;
			pDoc->m_pIo[i].lData = lData;
			break;
		}
	}
	if (i >= TOT_M_IO)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Not enought TOT_M_IO Num!!!"));
	}
}

BOOL CGvisR2R_PunchView::IsRdyTest()
{
	BOOL bOn0 = IsRdyTest0();
	BOOL bOn1 = IsRdyTest1();

	if (bOn0 && bOn1)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsRdyTest0()
{
	BOOL bOn0 = (pDoc->m_pMpeIb[10] & (0x01 << 11)) ? TRUE : FALSE;		// 검사부 상 자동 운전 <-> X432B I/F
	BOOL bOn1 = (pDoc->m_pMpeIb[10] & (0x01 << 9)) ? TRUE : FALSE;		// 검사부 상 테이블 진공 완료 <-> X4329 I/F

	if (bOn0 && bOn1)
	{
		pDoc->LogAuto(_T("PLC: 검사부 상 자동(X432B) 운전 & 테이블 진공(X4329) 완료"));
		return TRUE;
	}
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsRdyTest1()
{
	if (!m_pMpe || !pDoc->m_pMpeData || !pDoc->m_pMpeIb)
		return FALSE;
	BOOL bOn0 = (pDoc->m_pMpeIb[14] & (0x01 << 11)) ? TRUE : FALSE;		// 검사부 하 자동 운전 <-> X442B I/F
	BOOL bOn1 = (pDoc->m_pMpeIb[14] & (0x01 << 9)) ? TRUE : FALSE;		// 검사부 하 테이블 진공 완료 <-> X4329 I/F
	double dCurPosMkFd = (double)pDoc->m_pMpeData[2][2];				// 마킹부 Feeding 엔코더 값(단위 mm )
	double dTgtFd = _tstof(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * _tstof(pDoc->WorkingInfo.Motion.sAoiFdDist);
	if (dCurPosMkFd > dTgtFd - _tstof(pDoc->WorkingInfo.Motion.sAoiFdDist) / 2.0)
	{
		BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
		if (bDualTest)
		{
			if (bOn0 && bOn1)
			{
				pDoc->LogAuto(_T("PLC: 검사부 하 자동(X442B) 운전 & 테이블 진공(X4329) 완료"));
				return TRUE;
			}
		}
		else
			return TRUE;
	}
	return FALSE;
}

BOOL CGvisR2R_PunchView::IsMk0Done()
{
	BOOL bDone = FALSE;

	if (m_pVoiceCoil[0])
		bDone = m_pVoiceCoil[0]->IsDoneMark(0);

	return bDone;
}

BOOL CGvisR2R_PunchView::IsMk1Done()
{
	BOOL bDone = FALSE;

	if (m_pVoiceCoil[1])
		bDone = m_pVoiceCoil[1]->IsDoneMark(1);

	return bDone;
}

void CGvisR2R_PunchView::SetPlcParam()
{
	if (!m_pMpe)
		return;

	MpeWrite(Plc.DlgInfo.StopLength, long(_tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen)*1000.0));	// 일시정지 길이 (단위 M * 1000)
	MpeWrite(Plc.DlgInfo.LotLength, long(_tstof(pDoc->WorkingInfo.LastJob.sLotSepLen)*1000.0));	// Lot 분리 길이 (단위 M * 1000)
	MpeWrite(Plc.DlgInfo.LotCutLength, long(_tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen)*1000.0));	// Lot 분리 후 절단위치 (단위 M * 1000)
	MpeWrite(Plc.DlgInfo.SampleTestShotNum, (long)_tstoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum));	// 샘플검사 Shot수

	if (pDoc->WorkingInfo.LastJob.bTempPause)
	{
		MpeWrite(Plc.DlgInfo.UseStopLength, 1);	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)
		ChkTempStop(TRUE);
	}
	else
	{
		MpeWrite(Plc.DlgInfo.UseStopLength, 0);	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)
		ChkTempStop(FALSE);
	}

	if (pDoc->WorkingInfo.LastJob.bLotSep)
	{
		SetLotLastShot();
		MpeWrite(Plc.DlgInfo.UseLotLength, 1);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)-20141031
	}
	else
		MpeWrite(Plc.DlgInfo.UseLotLength, 0);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)-20141031

	//MpeWrite(pView->Plc.DlgInfo.SingleMode, (pDoc->WorkingInfo.LastJob.bDualTest ? 0 : 1));		// 단면 검사 On
	//MpeWrite(Plc.DlgInfo.SampleTest, (pDoc->WorkingInfo.LastJob.bSampleTest) ? 1 : 0);		// Sample 검사 On
	//MpeWrite(Plc.DlgInfo.OneMetal, (pDoc->WorkingInfo.LastJob.bOneMetal) ? 1 : 0);		// One Metal On
	//MpeWrite(Plc.DlgInfo.TwoMetal, (pDoc->WorkingInfo.LastJob.bTwoMetal) ? 1 : 0);		// Two Metal On
	//MpeWrite(Plc.DlgInfo.ModeInner, (pDoc->GetTestMode() == MODE_INNER) ? 1 : 0);		// 내층모드 On
	//MpeWrite(Plc.DlgInfo.ModeOutter, (pDoc->GetTestMode() == MODE_OUTER) ? 1 : 0);		// 외층모드 On
	//MpeWrite(pView->Plc.DlgMenu03.UseCore150mmRecoiler, (pDoc->WorkingInfo.LastJob.bCore150Recoiler) ? 1 : 0);	// Recoiler Core 150mm On
	//MpeWrite(pView->Plc.DlgMenu03.UseCore150mmUncoiler, (pDoc->WorkingInfo.LastJob.bCore150Uncoiler) ? 1 : 0);	// Uncoiler Core 150mm On
	//MpeWrite(Plc.DlgInfo.FeedCcwRecoiler, (pView->Plc.DlgInfo.FeedCcwRecoiler) ? 1 : 0);
	//MpeWrite(Plc.DlgInfo.FeedCcwUncoiler, (pView->Plc.DlgInfo.FeedCcwUncoiler) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.DoorSensorUncoiler, (pDoc->WorkingInfo.LastJob.bUclDrSen) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.DoorSensorEngrave, (pDoc->WorkingInfo.LastJob.bEngvDrSen) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.DoorSensorAoiUp, (pDoc->WorkingInfo.LastJob.bAoiUpDrSen) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.DoorSensorAoiDn, (pDoc->WorkingInfo.LastJob.bAoiDnDrSen) ? 1 : 0);

	MpeWrite(Plc.DlgInfo.CleanRollerAoiUp, (pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0));
	MpeWrite(Plc.DlgInfo.CleanRollerAoiDn, (pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0));
	MpeWrite(Plc.DlgInfo.UltraSonicAoi, (pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic ? 1 : 0));
	MpeWrite(Plc.DlgInfo.UltraSonicEngrave, (pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic ? 1 : 0));

	MpeWrite(Plc.DlgInfo.Laser380mm, (pDoc->WorkingInfo.LastJob.bUse380mm) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.Laser346mm, (pDoc->WorkingInfo.LastJob.bUse346mm) ? 1 : 0);
	MpeWrite(Plc.DlgInfo.Laser340mm, (pDoc->WorkingInfo.LastJob.bUse340mm) ? 1 : 0);
}

void CGvisR2R_PunchView::GetPlcParam()
{
	if (!m_pMpe)
		return;
	// Main
	pDoc->BtnStatus.Main.Ready = MpeRead(Plc.DlgMenu03.LampReady) ? TRUE : FALSE;	// 마킹부 운전준비 스위치
	pDoc->BtnStatus.Main.Run = MpeRead(Plc.DlgMenu03.LampRun) ? TRUE : FALSE;		// 마킹부 운전 스위치
	pDoc->BtnStatus.Main.Reset = MpeRead(Plc.DlgMenu03.LampReset) ? TRUE : FALSE;	// 마킹부 리셋 스위치
	pDoc->BtnStatus.Main.Stop = MpeRead(Plc.DlgMenu03.LampStop) ? TRUE : FALSE;	// 마킹부 정지 스위치
	//pDoc->BtnStatus.Main.Auto = MpeRead(_T("MB005505")) ? TRUE : FALSE;	// 마킹부 자동/수동 (ON)
	//pDoc->BtnStatus.Main.Manual = MpeRead(_T("MB005505")) ? FALSE : TRUE;	// 마킹부 자동/수동 (OFF)
	pDoc->BtnStatus.Main.Auto = MpeRead(Plc.DlgMenu03.LampAuto) ? TRUE : FALSE;	// 마킹부 자동 상태 스위치 램프
	pDoc->BtnStatus.Main.Manual = MpeRead(Plc.DlgMenu03.LampAuto) ? FALSE : TRUE;	// 마킹부 자동 상태 스위치 램프

	if (pDoc->BtnStatus.Main.PrevReady != pDoc->BtnStatus.Main.Ready)
	{
		pDoc->BtnStatus.Main.PrevReady = pDoc->BtnStatus.Main.Ready;
		pDoc->SetMkMenu03(_T("Main"), _T("Ready"), pDoc->BtnStatus.Main.Ready);
	}
	if (pDoc->BtnStatus.Main.PrevRun != pDoc->BtnStatus.Main.Run)
	{
		pDoc->BtnStatus.Main.PrevRun = pDoc->BtnStatus.Main.Run;
		pDoc->SetMkMenu03(_T("Main"), _T("Run"), pDoc->BtnStatus.Main.Run);
		pDoc->SetMkMenu03(_T("Main"), _T("Stop"), !pDoc->BtnStatus.Main.Run);
	}
	if (pDoc->BtnStatus.Main.PrevReset != pDoc->BtnStatus.Main.Reset)
	{
		pDoc->BtnStatus.Main.PrevReset = pDoc->BtnStatus.Main.Reset;
		pDoc->SetMkMenu03(_T("Main"), _T("Reset"), pDoc->BtnStatus.Main.Reset);
	}
	if (pDoc->BtnStatus.Main.PrevStop != pDoc->BtnStatus.Main.Stop)
	{
		pDoc->BtnStatus.Main.PrevStop = pDoc->BtnStatus.Main.Stop;
		pDoc->SetMkMenu03(_T("Main"), _T("Stop"), pDoc->BtnStatus.Main.Stop);
		pDoc->SetMkMenu03(_T("Main"), _T("Run"), !pDoc->BtnStatus.Main.Stop);
	}
	if (pDoc->BtnStatus.Main.PrevAuto != pDoc->BtnStatus.Main.Auto)
	{
		pDoc->BtnStatus.Main.PrevAuto = pDoc->BtnStatus.Main.Auto;
		pDoc->SetMkMenu03(_T("Main"), _T("Auto"), pDoc->BtnStatus.Main.Auto);
		pDoc->SetMkMenu03(_T("Main"), _T("Manual"), !pDoc->BtnStatus.Main.Auto);
	}
	if (pDoc->BtnStatus.Main.PrevManual != pDoc->BtnStatus.Main.Manual)
	{
		pDoc->BtnStatus.Main.PrevManual = pDoc->BtnStatus.Main.Manual;
		pDoc->SetMkMenu03(_T("Main"), _T("Manual"), pDoc->BtnStatus.Main.Manual);
		pDoc->SetMkMenu03(_T("Main"), _T("Auto"), !pDoc->BtnStatus.Main.Manual);
	}

	// RunMode
	pDoc->BtnStatus.RunMode.ConnectModule = MpeRead(Plc.DlgMenu03.LampConnectModule) ? TRUE : FALSE;
	pDoc->BtnStatus.RunMode.FeedOnePanel = MpeRead(Plc.DlgMenu03.LampFeedOnePanel) ? TRUE : FALSE;

	if (pDoc->BtnStatus.RunMode.ConnectModule != pDoc->BtnStatus.RunMode.ConnectModule)
	{
		pDoc->BtnStatus.RunMode.PrevConnectModule = pDoc->BtnStatus.RunMode.ConnectModule;
		pDoc->SetMkMenu03(_T("RunMode"), _T("ConnectModule"), pDoc->BtnStatus.RunMode.ConnectModule);
	}
	if (pDoc->BtnStatus.RunMode.PrevFeedOnePanel != pDoc->BtnStatus.RunMode.FeedOnePanel)
	{
		pDoc->BtnStatus.RunMode.PrevFeedOnePanel = pDoc->BtnStatus.RunMode.FeedOnePanel;
		pDoc->SetMkMenu03(_T("RunMode"), _T("FeedOnePanel"), pDoc->BtnStatus.RunMode.FeedOnePanel);
	}

	// Laser
	pDoc->BtnStatus.Lsr.Laser340mm = MpeRead(Plc.DlgInfo.LampLaser340mm) ? TRUE : FALSE;
	pDoc->BtnStatus.Lsr.Laser346mm = MpeRead(Plc.DlgInfo.LampLaser346mm) ? TRUE : FALSE;
	pDoc->BtnStatus.Lsr.Laser380mm = MpeRead(Plc.DlgInfo.LampLaser380mm) ? TRUE : FALSE;

	if (pDoc->BtnStatus.Lsr.PrevLaser340mm != pDoc->BtnStatus.Lsr.Laser340mm)
	{
		pDoc->BtnStatus.Lsr.PrevLaser340mm = pDoc->BtnStatus.Lsr.Laser340mm;
		pDoc->SetMkMenu03(_T("IDD_DLG_INFO"), _T("Laser340mm"), pDoc->BtnStatus.Lsr.Laser340mm);
	}
	if (pDoc->BtnStatus.Lsr.PrevLaser346mm != pDoc->BtnStatus.Lsr.Laser346mm)
	{
		pDoc->BtnStatus.Lsr.PrevLaser346mm = pDoc->BtnStatus.Lsr.Laser346mm;
		pDoc->SetMkMenu03(_T("IDD_DLG_INFO"), _T("Laser346mm"), pDoc->BtnStatus.Lsr.Laser346mm);
	}
	if (pDoc->BtnStatus.Lsr.PrevLaser380mm != pDoc->BtnStatus.Lsr.Laser380mm)
	{
		pDoc->BtnStatus.Lsr.PrevLaser380mm = pDoc->BtnStatus.Lsr.Laser380mm;
		pDoc->SetMkMenu03(_T("IDD_DLG_INFO"), _T("Laser380mm"), pDoc->BtnStatus.Lsr.Laser380mm);
	}

	// TorqueMotor
	pDoc->BtnStatus.Tq.Mk = MpeRead(Plc.DlgMenu03.LampTensionCcwPunch) ? TRUE : FALSE;
	pDoc->BtnStatus.Tq.Aoi = MpeRead(Plc.DlgMenu03.LampTensionCcwAoiUp) ? TRUE : FALSE;
	pDoc->BtnStatus.Tq.Eng = MpeRead(Plc.DlgMenu03.LampTensionCcwEngrave) ? TRUE : FALSE;

	if (pDoc->BtnStatus.Tq.PrevMk != pDoc->BtnStatus.Tq.Mk)
	{
		pDoc->BtnStatus.Tq.PrevMk = pDoc->BtnStatus.Tq.Mk;
		pDoc->SetMkMenu03(_T("TqMotor"), _T("MkTq"), pDoc->BtnStatus.Tq.Mk);
	}
	if (pDoc->BtnStatus.Tq.PrevAoi != pDoc->BtnStatus.Tq.Aoi)
	{
		pDoc->BtnStatus.Tq.PrevAoi = pDoc->BtnStatus.Tq.Aoi;
		pDoc->SetMkMenu03(_T("TqMotor"), _T("AoiTq"), pDoc->BtnStatus.Tq.Aoi);
	}
	if (pDoc->BtnStatus.Tq.PrevEng != pDoc->BtnStatus.Tq.Eng)
	{
		pDoc->BtnStatus.Tq.PrevEng = pDoc->BtnStatus.Tq.Eng;
		pDoc->SetMkMenu03(_T("TqMotor"), _T("EngTq"), pDoc->BtnStatus.Tq.Eng);
	}

	// 회전방향	
	pDoc->WorkingInfo.LastJob.bFeedCcwRecoiler = pDoc->BtnStatus.FeedDir.FeedCcwRecoiler = pView->MpeRead(pView->Plc.DlgInfo.LampFeedCcwRecoiler) ? TRUE : FALSE;
	pDoc->WorkingInfo.LastJob.bFeedCcwUncoiler = pDoc->BtnStatus.FeedDir.FeedCcwUncoiler = pView->MpeRead(pView->Plc.DlgInfo.LampFeedCcwUncoiler) ? TRUE : FALSE;
	
	if (pDoc->BtnStatus.FeedDir.PrevFeedCcwRecoiler != pDoc->BtnStatus.FeedDir.FeedCcwRecoiler)
	{
		pDoc->BtnStatus.FeedDir.PrevFeedCcwRecoiler = pDoc->BtnStatus.FeedDir.FeedCcwRecoiler;
		pDoc->SetMkMenu03(_T("IDD_DLG_INFO"), _T("FeedCcwRecoiler"), pDoc->BtnStatus.FeedDir.FeedCcwRecoiler);
	}
	if (pDoc->BtnStatus.FeedDir.PrevFeedCcwUncoiler != pDoc->BtnStatus.FeedDir.FeedCcwUncoiler)
	{
		pDoc->BtnStatus.FeedDir.PrevFeedCcwUncoiler = pDoc->BtnStatus.FeedDir.FeedCcwUncoiler;
		pDoc->SetMkMenu03(_T("IDD_DLG_INFO"), _T("FeedCcwUncoiler"), pDoc->BtnStatus.FeedDir.FeedCcwUncoiler);
	}

	// 댄서롤/버퍼롤
	pDoc->BtnStatus.Dancer.AllDancerUpDn = pView->MpeRead(pView->Plc.DlgMenu03.LampAllDancerUpDn) ? TRUE : FALSE;
	pDoc->BtnStatus.Dancer.AllDancerFixOnOff = pView->MpeRead(pView->Plc.DlgMenu03.LampAllDancerFixOnOff) ? TRUE : FALSE;

	if (pDoc->BtnStatus.Dancer.PrevAllDancerUpDn != pDoc->BtnStatus.Dancer.AllDancerUpDn)
	{
		pDoc->BtnStatus.Dancer.PrevAllDancerUpDn = pDoc->BtnStatus.Dancer.AllDancerUpDn;
		pDoc->SetMkMenu03(_T("IDD_DLG_MENU_03"), _T("AllDancerUpDn"), pDoc->BtnStatus.Dancer.AllDancerUpDn);
	}
	if (pDoc->BtnStatus.Dancer.PrevAllDancerFixOnOff != pDoc->BtnStatus.Dancer.AllDancerFixOnOff)
	{
		pDoc->BtnStatus.Dancer.PrevAllDancerFixOnOff = pDoc->BtnStatus.Dancer.AllDancerFixOnOff;
		pDoc->SetMkMenu03(_T("IDD_DLG_MENU_03"), _T("AllDancerFixOnOff"), pDoc->BtnStatus.Dancer.AllDancerFixOnOff);
	}

	// InductionMotor
	//pDoc->BtnStatus.Induct.Rc = MpeRead(Plc.DlgInfo.LampOneMetal) ? TRUE : FALSE;	//pView->SetTwoMetal(FALSE, TRUE);	// One Metal IDC_CHK_68		
	//pDoc->BtnStatus.Induct.Uc = MpeRead(Plc.DlgInfo.LampTwoMetal) ? TRUE : FALSE;	//pView->SetTwoMetal(TRUE, TRUE);	// Two Metal IDC_CHK_69

	//if (pDoc->BtnStatus.Induct.PrevRc != pDoc->BtnStatus.Induct.Rc)
	//{
	//	pDoc->BtnStatus.Induct.PrevRc = pDoc->BtnStatus.Induct.Rc;
	//	pDoc->SetMkMenu03(_T("Induction"), _T("RcCcw"), pDoc->BtnStatus.Induct.Rc);
	//}
	//if (pDoc->BtnStatus.Induct.PrevUc != pDoc->BtnStatus.Induct.Uc)
	//{
	//	pDoc->BtnStatus.Induct.PrevUc = pDoc->BtnStatus.Induct.Uc;
	//	pDoc->SetMkMenu03(_T("Induction"), _T("UcCcw"), pDoc->BtnStatus.Induct.Uc);
	//}

	// Core150mm
	//pDoc->BtnStatus.Core150.Rc = MpeRead(pView->Plc.DlgMenu03.UseCore150mmRecoiler) ? TRUE : FALSE;	// SetCore150mmRecoiler(TRUE);	// Recoiler IDC_CHK_70	
	//pDoc->BtnStatus.Core150.Uc = MpeRead(pView->Plc.DlgMenu03.UseCore150mmUncoiler) ? TRUE : FALSE;	// SetCore150mmUncoiler(TRUE);	// Uncoiler IDC_CHK_71	

	//if (pDoc->BtnStatus.Core150.PrevRc != pDoc->BtnStatus.Core150.Rc)
	//{
	//	pDoc->BtnStatus.Core150.PrevRc = pDoc->BtnStatus.Core150.Rc;
	//	pDoc->SetMkMenu03(_T("Core"), _T("Rc150"), pDoc->BtnStatus.Core150.Rc);
	//}
	//if (pDoc->BtnStatus.Core150.PrevUc != pDoc->BtnStatus.Core150.Uc)
	//{
	//	pDoc->BtnStatus.Core150.PrevUc = pDoc->BtnStatus.Core150.Uc;
	//	pDoc->SetMkMenu03(_T("Core"), _T("Uc150"), pDoc->BtnStatus.Core150.Uc);
	//}

	// Recoiler
	pDoc->BtnStatus.Rc.ChuckPcb = MpeRead(Plc.DlgMenu03.LampChuckPcbRecoiler) ? TRUE : FALSE;					// 리코일러 제품척 클램프 스위치 램프
	pDoc->BtnStatus.Rc.ChuckPaper = MpeRead(Plc.DlgMenu03.LampChuckPaperRecoiler) ? TRUE : FALSE;				// 리코일러 간지척 클램프 스위치 램프
	pDoc->BtnStatus.Rc.JoinClamp = MpeRead(Plc.DlgMenu03.LampJoinClampRecoiler) ? TRUE : FALSE;					// 리코일러 이음매 클램프 스위치 램프
	pDoc->BtnStatus.Rc.PcbShaftSupport = MpeRead(Plc.DlgMenu03.LampPcbShaftSupportRecoiler) ? TRUE : FALSE;		// 리코일러 제품샤프트 지지대 스위치 램프
	pDoc->BtnStatus.Rc.EpcActHome = MpeRead(Plc.DlgMenu03.LampEpcActHomeRecoiler) ? TRUE : FALSE;				// 리코일러 EPC 액추에이터 HOME 스위치 램프
	pDoc->BtnStatus.Rc.EpcActFirst = MpeRead(Plc.DlgMenu03.LampEpcActFirstRecoiler) ? TRUE : FALSE;				// 리코일러 EPC 액추에이터 #1 스위치 램프
	pDoc->BtnStatus.Rc.EpcActSecond = MpeRead(Plc.DlgMenu03.LampEpcActSecondRecoiler) ? TRUE : FALSE;			// 리코일러 EPC 액추에이터 #2 스위치 램프
	pDoc->BtnStatus.Rc.EpcActThird = MpeRead(Plc.DlgMenu03.LampEpcActThirdRecoiler) ? TRUE : FALSE;				// 리코일러 EPC 액추에이터 #3 스위치 램프
	pDoc->BtnStatus.Rc.EpcAuto = MpeRead(Plc.DlgMenu03.LampEpcAutoRecoiler) ? TRUE : FALSE;						// 리코일러 EPC 자동 스위치 램프
	pDoc->BtnStatus.Rc.EpcManual = MpeRead(Plc.DlgMenu03.LampEpcManualRecoiler) ? TRUE : FALSE;					// 리코일러 EPC 수동 스위치 램프
	pDoc->BtnStatus.Rc.EpcHome = MpeRead(Plc.DlgMenu03.LampEpcHomeRecoiler) ? TRUE : FALSE;						// 리코일러 EPC 원점 스위치 램프
	pDoc->BtnStatus.Rc.EpcIn = MpeRead(Plc.DlgMenu03.LampEpcInRecoiler) ? TRUE : FALSE;							// 리코일러 EPC In 스위치 램프
	pDoc->BtnStatus.Rc.EpcOut = MpeRead(Plc.DlgMenu03.LampEpcOutRecoiler) ? TRUE : FALSE;						// 리코일러 EPC Out 스위치 램프
	pDoc->BtnStatus.Rc.PcbInverterCw = MpeRead(Plc.DlgMenu03.LampPcbInverterCwRecoiler) ? TRUE : FALSE;			// 리코일러 제품 인버터 정방향 스위치 램프
	pDoc->BtnStatus.Rc.PcbInverterCcw = MpeRead(Plc.DlgMenu03.LampPcbInverterCcwRecoiler) ? TRUE : FALSE;		// 리코일러 제품 인버터 역방향 스위치 램프
	pDoc->BtnStatus.Rc.PaperInverterCw = MpeRead(Plc.DlgMenu03.LampPaperInverterCwRecoiler) ? TRUE : FALSE;		// 리코일러 간지 인버터 정방향 스위치 램프
	pDoc->BtnStatus.Rc.PaperInverterCcw = MpeRead(Plc.DlgMenu03.LampPaperInverterCcwRecoiler) ? TRUE : FALSE;	// 리코일러 간지 인버터 역방향 스위치 램프
	pDoc->BtnStatus.Rc.DancerUpper = MpeRead(Plc.DlgMenu03.LampDancerUpperRecoiler) ? TRUE : FALSE;				// 리코일러 댄서롤 상승 스위치 램프
	pDoc->BtnStatus.Rc.DancerFixer = MpeRead(Plc.DlgMenu03.LampDancerFixerRecoiler) ? TRUE : FALSE;				// 리코일러 댄서롤 고정 스위치 램프

	if (pDoc->BtnStatus.Rc.PrevChuckPcb != pDoc->BtnStatus.Rc.ChuckPcb)
	{
		pDoc->BtnStatus.Rc.PrevChuckPcb = pDoc->BtnStatus.Rc.ChuckPcb;
		//pDoc->SetMkMenu03(_T("Main"), _T("Relation"), pDoc->BtnStatus.Rc.Relation);
	}
	if (pDoc->BtnStatus.Rc.PrevChuckPaper != pDoc->BtnStatus.Rc.ChuckPaper)
	{
		pDoc->BtnStatus.Rc.PrevChuckPaper = pDoc->BtnStatus.Rc.ChuckPaper;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("MvCw"), pDoc->BtnStatus.Rc.FdCw);
	}
	if (pDoc->BtnStatus.Rc.PrevJoinClamp != pDoc->BtnStatus.Rc.JoinClamp)
	{
		pDoc->BtnStatus.Rc.PrevJoinClamp = pDoc->BtnStatus.Rc.JoinClamp;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("MvCcw"), pDoc->BtnStatus.Rc.FdCcw);
	}
	if (pDoc->BtnStatus.Rc.PrevPcbShaftSupport != pDoc->BtnStatus.Rc.PcbShaftSupport)
	{
		pDoc->BtnStatus.Rc.PrevPcbShaftSupport = pDoc->BtnStatus.Rc.PcbShaftSupport;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PrdChuck"), pDoc->BtnStatus.Rc.ReelChuck);
	}
	if (pDoc->BtnStatus.Rc.PrevEpcActHome != pDoc->BtnStatus.Rc.EpcActHome)
	{
		pDoc->BtnStatus.Rc.PrevEpcActHome = pDoc->BtnStatus.Rc.EpcActHome;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("DancerUp"), pDoc->BtnStatus.Rc.DcRlUpDn);
	}
	if (pDoc->BtnStatus.Rc.PrevEpcActFirst != pDoc->BtnStatus.Rc.EpcActFirst)
	{
		pDoc->BtnStatus.Rc.PrevEpcActFirst = pDoc->BtnStatus.Rc.EpcActFirst;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PasteUpLf"), pDoc->BtnStatus.Rc.ReelJoinL);
	}
	if (pDoc->BtnStatus.Rc.PrevEpcActSecond != pDoc->BtnStatus.Rc.EpcActSecond)
	{
		pDoc->BtnStatus.Rc.PrevEpcActSecond = pDoc->BtnStatus.Rc.EpcActSecond;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PasteUpRt"), pDoc->BtnStatus.Rc.ReelJoinR);
	}
	if (pDoc->BtnStatus.Rc.PrevEpcActThird != pDoc->BtnStatus.Rc.EpcActThird)
	{
		pDoc->BtnStatus.Rc.PrevEpcActThird = pDoc->BtnStatus.Rc.EpcActThird;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PasteVac"), pDoc->BtnStatus.Rc.ReelJoinVac);
	}
	if (pDoc->BtnStatus.Rc.PrevEpcAuto != pDoc->BtnStatus.Rc.EpcAuto)
	{
		pDoc->BtnStatus.Rc.PrevEpcAuto = pDoc->BtnStatus.Rc.EpcAuto;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PprChuck"), pDoc->BtnStatus.Rc.PprChuck);
	}
	if (pDoc->BtnStatus.Rc.PrevEpcManual != pDoc->BtnStatus.Rc.EpcManual)
	{
		pDoc->BtnStatus.Rc.PrevEpcManual = pDoc->BtnStatus.Rc.EpcManual;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PprCw"), pDoc->BtnStatus.Rc.PprCw);
	}
	if (pDoc->BtnStatus.Rc.PrevEpcHome != pDoc->BtnStatus.Rc.EpcHome)
	{
		pDoc->BtnStatus.Rc.PrevEpcHome = pDoc->BtnStatus.Rc.EpcHome;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PprCcw"), pDoc->BtnStatus.Rc.PprCcw);
	}
	if (pDoc->BtnStatus.Rc.PrevEpcIn != pDoc->BtnStatus.Rc.EpcIn)
	{
		pDoc->BtnStatus.Rc.PrevEpcIn = pDoc->BtnStatus.Rc.EpcIn;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("DoRewind"), pDoc->BtnStatus.Rc.Rewine);
	}
	if (pDoc->BtnStatus.Rc.PrevEpcOut != pDoc->BtnStatus.Rc.EpcOut)
	{
		pDoc->BtnStatus.Rc.PrevEpcOut = pDoc->BtnStatus.Rc.EpcOut;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PrdPprRewind"), pDoc->BtnStatus.Rc.RewineReelPpr);
	}
	if (pDoc->BtnStatus.Rc.PrevPcbInverterCw != pDoc->BtnStatus.Rc.PcbInverterCw)
	{
		pDoc->BtnStatus.Rc.PrevPcbInverterCw = pDoc->BtnStatus.Rc.PcbInverterCw;
	}
	if (pDoc->BtnStatus.Rc.PrevPcbInverterCcw != pDoc->BtnStatus.Rc.PcbInverterCcw)
	{
		pDoc->BtnStatus.Rc.PrevPcbInverterCcw = pDoc->BtnStatus.Rc.PcbInverterCcw;
	}
	if (pDoc->BtnStatus.Rc.PrevPaperInverterCw != pDoc->BtnStatus.Rc.PaperInverterCw)
	{
		pDoc->BtnStatus.Rc.PrevPaperInverterCw = pDoc->BtnStatus.Rc.PaperInverterCw;
	}
	if (pDoc->BtnStatus.Rc.PrevPaperInverterCcw != pDoc->BtnStatus.Rc.PaperInverterCcw)
	{
		pDoc->BtnStatus.Rc.PrevPaperInverterCcw = pDoc->BtnStatus.Rc.PaperInverterCcw;
	}
	if (pDoc->BtnStatus.Rc.PrevDancerUpper != pDoc->BtnStatus.Rc.DancerUpper)
	{
		pDoc->BtnStatus.Rc.PrevDancerUpper = pDoc->BtnStatus.Rc.DancerUpper;
	}
	if (pDoc->BtnStatus.Rc.PrevDancerFixer != pDoc->BtnStatus.Rc.DancerFixer)
	{
		pDoc->BtnStatus.Rc.PrevDancerFixer = pDoc->BtnStatus.Rc.DancerFixer;
	}

	// Punch
	pDoc->BtnStatus.Mk.FeedCw = MpeRead(Plc.DlgMenu03.LampFeedCwPunch) ? TRUE : FALSE;							// 마킹부 피딩 정방향 스위치 램프
	pDoc->BtnStatus.Mk.FeedCcw = MpeRead(Plc.DlgMenu03.LampFeedCcwPunch) ? TRUE : FALSE;						// 마킹부 피딩 역방향 스위치 램프
	pDoc->BtnStatus.Mk.TensionCw = MpeRead(Plc.DlgMenu03.LampTensionCwPunch) ? TRUE : FALSE;					// 마킹부 텐션 정방향 스위치 램프
	pDoc->BtnStatus.Mk.TensionCcw = MpeRead(Plc.DlgMenu03.LampTensionCcwPunch) ? TRUE : FALSE;					// 마킹부 텐션 역방향 스위치 램프
	pDoc->BtnStatus.Mk.FeedHome = MpeRead(Plc.DlgMenu03.LampFeedHomePunch) ? TRUE : FALSE;						// 마킹부 피딩 HOME 스위치 램프
	pDoc->BtnStatus.Mk.FeedVacuum = MpeRead(Plc.DlgMenu03.LampFeedVacuumPunch) ? TRUE : FALSE;					// 마킹부 피딩 진공 스위치 램프
	pDoc->BtnStatus.Mk.FeedClamp = MpeRead(Plc.DlgMenu03.LampFeedClampPunch) ? TRUE : FALSE;					// 마킹부 피딩 클램프 스위치 램프	
	pDoc->BtnStatus.Mk.TensionClamp = MpeRead(Plc.DlgMenu03.LampTensionClampPunch) ? TRUE : FALSE;				// 마킹부 텐션 클램프 스위치 램프  
	pDoc->BtnStatus.Mk.TableVacuum = MpeRead(Plc.DlgMenu03.LampTableVacuumPunch) ? TRUE : FALSE;				// 마킹부 테이블 진공 스위치 램프
	pDoc->BtnStatus.Mk.TableBlower = MpeRead(Plc.DlgMenu03.LampTableBlowerPunch) ? TRUE : FALSE;				// 마킹부 테이블 에어블로어 스위치 
	pDoc->BtnStatus.Mk.TableCylinder = MpeRead(Plc.DlgMenu03.LampTableCylinderPunch) ? TRUE : FALSE;			// 마킹부 테이블 실린더 스위치 램프
	pDoc->BtnStatus.Mk.TableClampDn = MpeRead(Plc.DlgMenu03.LampTableClampDnPunch) ? TRUE : FALSE;				// 마킹부 테이블 클램프 하강 스위치 램프
	pDoc->BtnStatus.Mk.TableClampForward = MpeRead(Plc.DlgMenu03.LampTableClampForwardPunch) ? TRUE : FALSE;	// 마킹부 테이블 클램프 전진 스위치 램프
	pDoc->BtnStatus.Mk.PunchStart = MpeRead(Plc.DlgMenu03.LampPunchStartPunch) ? TRUE : FALSE;					// 마킹부 마킹 시작 스위치 램프

	if (pDoc->BtnStatus.Mk.PrevFeedCw != pDoc->BtnStatus.Mk.FeedCw)
	{
		pDoc->BtnStatus.Mk.PrevFeedCw = pDoc->BtnStatus.Mk.FeedCw;
	}
	if (pDoc->BtnStatus.Mk.PrevFeedCcw != pDoc->BtnStatus.Mk.FeedCcw)
	{
		pDoc->BtnStatus.Mk.PrevFeedCcw = pDoc->BtnStatus.Mk.FeedCcw;
	}
	if (pDoc->BtnStatus.Mk.PrevTensionCw != pDoc->BtnStatus.Mk.TensionCw)
	{
		pDoc->BtnStatus.Mk.PrevTensionCw = pDoc->BtnStatus.Mk.TensionCw;
	}
	if (pDoc->BtnStatus.Mk.PrevTensionCcw != pDoc->BtnStatus.Mk.TensionCcw)
	{
		pDoc->BtnStatus.Mk.PrevTensionCcw = pDoc->BtnStatus.Mk.TensionCcw;
	}
	if (pDoc->BtnStatus.Mk.PrevFeedHome != pDoc->BtnStatus.Mk.FeedHome)
	{
		pDoc->BtnStatus.Mk.PrevFeedHome = pDoc->BtnStatus.Mk.FeedHome;
	}
	if (pDoc->BtnStatus.Mk.PrevFeedVacuum != pDoc->BtnStatus.Mk.FeedVacuum)
	{
		pDoc->BtnStatus.Mk.PrevFeedVacuum = pDoc->BtnStatus.Mk.FeedVacuum;
	}
	if (pDoc->BtnStatus.Mk.PrevFeedClamp != pDoc->BtnStatus.Mk.FeedClamp)
	{
		pDoc->BtnStatus.Mk.PrevFeedClamp = pDoc->BtnStatus.Mk.FeedClamp;
	}
	if (pDoc->BtnStatus.Mk.PrevTensionClamp != pDoc->BtnStatus.Mk.TensionClamp)
	{
		pDoc->BtnStatus.Mk.PrevTensionClamp = pDoc->BtnStatus.Mk.TensionClamp;
	}
	if (pDoc->BtnStatus.Mk.PrevTableVacuum != pDoc->BtnStatus.Mk.TableVacuum)
	{
		pDoc->BtnStatus.Mk.PrevTableVacuum = pDoc->BtnStatus.Mk.TableVacuum;
	}
	if (pDoc->BtnStatus.Mk.PrevTableBlower != pDoc->BtnStatus.Mk.TableBlower)
	{
		pDoc->BtnStatus.Mk.PrevTableBlower = pDoc->BtnStatus.Mk.TableBlower;
	}
	if (pDoc->BtnStatus.Mk.PrevTableCylinder != pDoc->BtnStatus.Mk.TableCylinder)
	{
		pDoc->BtnStatus.Mk.PrevTableCylinder = pDoc->BtnStatus.Mk.TableCylinder;
	}
	if (pDoc->BtnStatus.Mk.PrevTableClampDn != pDoc->BtnStatus.Mk.TableClampDn)
	{
		pDoc->BtnStatus.Mk.PrevTableClampDn = pDoc->BtnStatus.Mk.TableClampDn;
	}
	if (pDoc->BtnStatus.Mk.PrevTableClampForward != pDoc->BtnStatus.Mk.TableClampForward)
	{
		pDoc->BtnStatus.Mk.PrevTableClampForward = pDoc->BtnStatus.Mk.TableClampForward;
	}
	if (pDoc->BtnStatus.Mk.PrevPunchStart != pDoc->BtnStatus.Mk.PunchStart)
	{
		pDoc->BtnStatus.Mk.PrevPunchStart = pDoc->BtnStatus.Mk.PunchStart;
	}

	// AOIDn
	pDoc->BtnStatus.AoiDn.FeedCw = MpeRead(Plc.DlgMenu03.LampFeedCwAoiDn) ? TRUE : FALSE;						// 검사부 하 피딩 정방향 스위치 램프
	pDoc->BtnStatus.AoiDn.FeedCcw = MpeRead(Plc.DlgMenu03.LampFeedCcwAoiDn) ? TRUE : FALSE;						// 검사부 하 피딩 역방향 스위치 램프
	pDoc->BtnStatus.AoiDn.TensionCw = MpeRead(Plc.DlgMenu03.LampTensionCwAoiDn) ? TRUE : FALSE;					// 검사부 하 텐션 정방향 스위치 램프
	pDoc->BtnStatus.AoiDn.TensionCcw = MpeRead(Plc.DlgMenu03.LampTensionCcwAoiDn) ? TRUE : FALSE;				// 검사부 하 텐션 역방향 스위치 램프
	pDoc->BtnStatus.AoiDn.FeedHome = MpeRead(Plc.DlgMenu03.LampFeedHomeAoiDn) ? TRUE : FALSE;					// 검사부 하 피딩 HOME 스위치 램프
	pDoc->BtnStatus.AoiDn.FeedVacuum = MpeRead(Plc.DlgMenu03.LampFeedVacuumAoiDn) ? TRUE : FALSE;				// 검사부 하 피딩 진공 스위치 램프
	pDoc->BtnStatus.AoiDn.FeedClamp = MpeRead(Plc.DlgMenu03.LampFeedClampAoiDn) ? TRUE : FALSE;					// 검사부 하 피딩 클램프 스위치 램프
	pDoc->BtnStatus.AoiDn.TensionClamp = MpeRead(Plc.DlgMenu03.LampTensionClampAoiDn) ? TRUE : FALSE;			// 검사부 하 텐션 클램프 스위치 램프
	pDoc->BtnStatus.AoiDn.TableVacuum = MpeRead(Plc.DlgMenu03.LampTableVacuumAoiDn) ? TRUE : FALSE;				// 검사부 하 테이블 진공 스위치 램프
	pDoc->BtnStatus.AoiDn.TableBlower = MpeRead(Plc.DlgMenu03.LampTableBlowerAoiDn) ? TRUE : FALSE;				// 검사부 하 테이블 에어블로어 스위치 램프
	pDoc->BtnStatus.AoiDn.TableCylinder = MpeRead(Plc.DlgMenu03.LampTableCylinderAoiDn) ? TRUE : FALSE;			// 검사부 하 테이블 실린더 스위치 램프
	pDoc->BtnStatus.AoiDn.CleanRollerUp = MpeRead(Plc.DlgMenu03.LampCleanRollerUpAoiDn) ? TRUE : FALSE;			// 검사부 하 클린롤러 상부 스위치 램프
	pDoc->BtnStatus.AoiDn.CleanRollerDn = MpeRead(Plc.DlgMenu03.LampCleanRollerDnAoiDn) ? TRUE : FALSE;			// 검사부 하 클린롤러 하부 스위치 램프
	pDoc->BtnStatus.AoiDn.CleanRollerPush = MpeRead(Plc.DlgMenu03.LampCleanRollerPushAoiDn) ? TRUE : FALSE;		// 검사부 하 클린롤러 누름 스위치 램프
	pDoc->BtnStatus.AoiDn.TestStart = MpeRead(Plc.DlgMenu03.LampTestStartAoiDn) ? TRUE : FALSE;					// 검사부 하 AOI 상면 검사 시작 스위치 램프
	pDoc->BtnStatus.AoiDn.BufferRollerUp = MpeRead(Plc.DlgMenu03.LampBufferRollerUpAoiDn) ? TRUE : FALSE;		// 검사부 하 버퍼롤 상승 스위치 램프
	pDoc->BtnStatus.AoiDn.BufferRollerFix = MpeRead(Plc.DlgMenu03.LampBufferRollerFixAoiDn) ? TRUE : FALSE;		// 검사부 하 버퍼롤 고정 스위치 램프

	if (pDoc->BtnStatus.AoiDn.PrevFeedCw != pDoc->BtnStatus.AoiDn.FeedCw)
	{
		pDoc->BtnStatus.AoiDn.PrevFeedCw = pDoc->BtnStatus.AoiDn.FeedCw;
	}
	if (pDoc->BtnStatus.AoiDn.PrevFeedCcw != pDoc->BtnStatus.AoiDn.FeedCcw)
	{
		pDoc->BtnStatus.AoiDn.PrevFeedCcw = pDoc->BtnStatus.AoiDn.FeedCcw;
	}
	if (pDoc->BtnStatus.AoiDn.PrevTensionCw != pDoc->BtnStatus.AoiDn.TensionCw)
	{
		pDoc->BtnStatus.AoiDn.PrevTensionCw = pDoc->BtnStatus.AoiDn.TensionCw;
	}
	if (pDoc->BtnStatus.AoiDn.PrevTensionCcw != pDoc->BtnStatus.AoiDn.TensionCcw)
	{
		pDoc->BtnStatus.AoiDn.PrevTensionCcw = pDoc->BtnStatus.AoiDn.TensionCcw;
	}
	if (pDoc->BtnStatus.AoiDn.PrevFeedHome != pDoc->BtnStatus.AoiDn.FeedHome)
	{
		pDoc->BtnStatus.AoiDn.PrevFeedHome = pDoc->BtnStatus.AoiDn.FeedHome;
	}
	if (pDoc->BtnStatus.AoiDn.PrevFeedVacuum != pDoc->BtnStatus.AoiDn.FeedVacuum)
	{
		pDoc->BtnStatus.AoiDn.PrevFeedVacuum = pDoc->BtnStatus.AoiDn.FeedVacuum;
	}
	if (pDoc->BtnStatus.AoiDn.PrevFeedClamp != pDoc->BtnStatus.AoiDn.FeedClamp)
	{
		pDoc->BtnStatus.AoiDn.PrevFeedClamp = pDoc->BtnStatus.AoiDn.FeedClamp;
	}
	if (pDoc->BtnStatus.AoiDn.PrevTensionClamp != pDoc->BtnStatus.AoiDn.TensionClamp)
	{
		pDoc->BtnStatus.AoiDn.PrevTensionClamp = pDoc->BtnStatus.AoiDn.TensionClamp;
	}
	if (pDoc->BtnStatus.AoiDn.PrevTableVacuum != pDoc->BtnStatus.AoiDn.TableVacuum)
	{
		pDoc->BtnStatus.AoiDn.PrevTableVacuum = pDoc->BtnStatus.AoiDn.TableVacuum;
	}
	if (pDoc->BtnStatus.AoiDn.PrevTableBlower != pDoc->BtnStatus.AoiDn.TableBlower)
	{
		pDoc->BtnStatus.AoiDn.PrevTableBlower = pDoc->BtnStatus.AoiDn.TableBlower;
	}
	if (pDoc->BtnStatus.AoiDn.PrevTableCylinder != pDoc->BtnStatus.AoiDn.TableCylinder)
	{
		pDoc->BtnStatus.AoiDn.PrevTableCylinder = pDoc->BtnStatus.AoiDn.TableCylinder;
	}
	if (pDoc->BtnStatus.AoiDn.PrevCleanRollerUp != pDoc->BtnStatus.AoiDn.CleanRollerUp)
	{
		pDoc->BtnStatus.AoiDn.PrevCleanRollerUp = pDoc->BtnStatus.AoiDn.CleanRollerUp;
	}
	if (pDoc->BtnStatus.AoiDn.PrevCleanRollerDn != pDoc->BtnStatus.AoiDn.CleanRollerDn)
	{
		pDoc->BtnStatus.AoiDn.PrevCleanRollerDn = pDoc->BtnStatus.AoiDn.CleanRollerDn;
	}
	if (pDoc->BtnStatus.AoiDn.PrevCleanRollerPush != pDoc->BtnStatus.AoiDn.CleanRollerPush)
	{
		pDoc->BtnStatus.AoiDn.PrevCleanRollerPush = pDoc->BtnStatus.AoiDn.CleanRollerPush;
	}
	if (pDoc->BtnStatus.AoiDn.PrevTestStart != pDoc->BtnStatus.AoiDn.TestStart)
	{
		pDoc->BtnStatus.AoiDn.PrevTestStart = pDoc->BtnStatus.AoiDn.TestStart;
	}
	if (pDoc->BtnStatus.AoiDn.PrevBufferRollerUp != pDoc->BtnStatus.AoiDn.BufferRollerUp)
	{
		pDoc->BtnStatus.AoiDn.PrevBufferRollerUp = pDoc->BtnStatus.AoiDn.BufferRollerUp;
	}
	if (pDoc->BtnStatus.AoiDn.PrevBufferRollerFix != pDoc->BtnStatus.AoiDn.BufferRollerFix)
	{
		pDoc->BtnStatus.AoiDn.PrevBufferRollerFix = pDoc->BtnStatus.AoiDn.BufferRollerFix;
	}

	// AOIUp
	pDoc->BtnStatus.AoiUp.FeedCw = MpeRead(Plc.DlgMenu03.LampFeedCwAoiUp) ? TRUE : FALSE;						// 검사부 상 피딩 정방향 스위치 램프
	pDoc->BtnStatus.AoiUp.FeedCcw = MpeRead(Plc.DlgMenu03.LampFeedCcwAoiUp) ? TRUE : FALSE;						// 검사부 상 피딩 역방향 스위치 램프
	pDoc->BtnStatus.AoiUp.TensionCw = MpeRead(Plc.DlgMenu03.LampTensionCwAoiUp) ? TRUE : FALSE;					// 검사부 상 텐션 정방향 스위치 램프
	pDoc->BtnStatus.AoiUp.TensionCcw = MpeRead(Plc.DlgMenu03.LampTensionCcwAoiUp) ? TRUE : FALSE;				// 검사부 상 텐션 역방향 스위치 램프
	pDoc->BtnStatus.AoiUp.FeedHome = MpeRead(Plc.DlgMenu03.LampFeedHomeAoiUp) ? TRUE : FALSE;					// 검사부 상 피딩 HOME 스위치 램프
	pDoc->BtnStatus.AoiUp.FeedVacuum = MpeRead(Plc.DlgMenu03.LampFeedVacuumAoiUp) ? TRUE : FALSE;				// 검사부 상 피딩 진공 스위치 램프
	pDoc->BtnStatus.AoiUp.FeedClamp = MpeRead(Plc.DlgMenu03.LampFeedClampAoiUp) ? TRUE : FALSE;					// 검사부 상 피딩 클램프 스위치 램프
	pDoc->BtnStatus.AoiUp.TensionClamp = MpeRead(Plc.DlgMenu03.LampTensionClampAoiUp) ? TRUE : FALSE;			// 검사부 상 텐션 클램프 스위치 램프
	pDoc->BtnStatus.AoiUp.TableVacuum = MpeRead(Plc.DlgMenu03.LampTableVacuumAoiUp) ? TRUE : FALSE;				// 검사부 상 테이블 진공 스위치 램프
	pDoc->BtnStatus.AoiUp.TableBlower = MpeRead(Plc.DlgMenu03.LampTableBlowerAoiUp) ? TRUE : FALSE;				// 검사부 상 테이블 에어블로어 스위치 램프
	pDoc->BtnStatus.AoiUp.TableCylinder = MpeRead(Plc.DlgMenu03.LampTableCylinderAoiUp) ? TRUE : FALSE;			// 검사부 상 테이블 실린더 스위치 램프
	pDoc->BtnStatus.AoiUp.CleanRollerUp = MpeRead(Plc.DlgMenu03.LampCleanRollerUpAoiUp) ? TRUE : FALSE;			// 검사부 상 클린롤러 상부 스위치 램프
	pDoc->BtnStatus.AoiUp.CleanRollerDn = MpeRead(Plc.DlgMenu03.LampCleanRollerDnAoiUp) ? TRUE : FALSE;			// 검사부 상 클린롤러 하부 스위치 램프
	pDoc->BtnStatus.AoiUp.CleanRollerPush = MpeRead(Plc.DlgMenu03.LampCleanRollerPushAoiUp) ? TRUE : FALSE;		// 검사부 상 클린롤러 누름 스위치 램프
	pDoc->BtnStatus.AoiUp.TestStart = MpeRead(Plc.DlgMenu03.LampTestStartAoiUp) ? TRUE : FALSE;					// 검사부 상 검사 시작 스위치 램프
	pDoc->BtnStatus.AoiUp.UltrasonicDn = MpeRead(Plc.DlgMenu03.LampUltrasonicDnAoiUp) ? TRUE : FALSE;			// 검사부 상 초음파 세정기 하강 스위치 램프
	pDoc->BtnStatus.AoiUp.UltrasonicRun = MpeRead(Plc.DlgMenu03.LampUltrasonicRunAoiUp) ? TRUE : FALSE;			// 검사부 상 초음파 세정기 운전 스위치 램프
	pDoc->BtnStatus.AoiUp.UltrasonicSpeed = MpeRead(Plc.DlgMenu03.LampUltrasonicSpeedAoiUp) ? TRUE : FALSE;		// 검사부 상 초음파 세정기 속도 스위치 램프

	if (pDoc->BtnStatus.AoiUp.PrevFeedCw != pDoc->BtnStatus.AoiUp.FeedCw)
	{
		pDoc->BtnStatus.AoiUp.PrevFeedCw = pDoc->BtnStatus.AoiUp.FeedCw;
	}
	if (pDoc->BtnStatus.AoiUp.PrevFeedCcw != pDoc->BtnStatus.AoiUp.FeedCcw)
	{
		pDoc->BtnStatus.AoiUp.PrevFeedCcw = pDoc->BtnStatus.AoiUp.FeedCcw;
	}
	if (pDoc->BtnStatus.AoiUp.PrevTensionCw != pDoc->BtnStatus.AoiUp.TensionCw)
	{
		pDoc->BtnStatus.AoiUp.PrevTensionCw = pDoc->BtnStatus.AoiUp.TensionCw;
	}
	if (pDoc->BtnStatus.AoiUp.PrevTensionCcw != pDoc->BtnStatus.AoiUp.TensionCcw)
	{
		pDoc->BtnStatus.AoiUp.PrevTensionCcw = pDoc->BtnStatus.AoiUp.TensionCcw;
	}
	if (pDoc->BtnStatus.AoiUp.PrevFeedHome != pDoc->BtnStatus.AoiUp.FeedHome)
	{
		pDoc->BtnStatus.AoiUp.PrevFeedHome = pDoc->BtnStatus.AoiUp.FeedHome;
	}
	if (pDoc->BtnStatus.AoiUp.PrevFeedVacuum != pDoc->BtnStatus.AoiUp.FeedVacuum)
	{
		pDoc->BtnStatus.AoiUp.PrevFeedVacuum = pDoc->BtnStatus.AoiUp.FeedVacuum;
	}
	if (pDoc->BtnStatus.AoiUp.PrevFeedClamp != pDoc->BtnStatus.AoiUp.FeedClamp)
	{
		pDoc->BtnStatus.AoiUp.PrevFeedClamp = pDoc->BtnStatus.AoiUp.FeedClamp;
	}
	if (pDoc->BtnStatus.AoiUp.PrevTensionClamp != pDoc->BtnStatus.AoiUp.TensionClamp)
	{
		pDoc->BtnStatus.AoiUp.PrevTensionClamp = pDoc->BtnStatus.AoiUp.TensionClamp;
	}
	if (pDoc->BtnStatus.AoiUp.PrevTableVacuum != pDoc->BtnStatus.AoiUp.TableVacuum)
	{
		pDoc->BtnStatus.AoiUp.PrevTableVacuum = pDoc->BtnStatus.AoiUp.TableVacuum;
	}
	if (pDoc->BtnStatus.AoiUp.PrevTableBlower != pDoc->BtnStatus.AoiUp.TableBlower)
	{
		pDoc->BtnStatus.AoiUp.PrevTableBlower = pDoc->BtnStatus.AoiUp.TableBlower;
	}
	if (pDoc->BtnStatus.AoiUp.PrevTableCylinder != pDoc->BtnStatus.AoiUp.TableCylinder)
	{
		pDoc->BtnStatus.AoiUp.PrevTableCylinder = pDoc->BtnStatus.AoiUp.TableCylinder;
	}
	if (pDoc->BtnStatus.AoiUp.PrevCleanRollerUp != pDoc->BtnStatus.AoiUp.CleanRollerUp)
	{
		pDoc->BtnStatus.AoiUp.PrevCleanRollerUp = pDoc->BtnStatus.AoiUp.CleanRollerUp;
	}
	if (pDoc->BtnStatus.AoiUp.PrevCleanRollerDn != pDoc->BtnStatus.AoiUp.CleanRollerDn)
	{
		pDoc->BtnStatus.AoiUp.PrevCleanRollerDn = pDoc->BtnStatus.AoiUp.CleanRollerDn;
	}
	if (pDoc->BtnStatus.AoiUp.PrevCleanRollerPush != pDoc->BtnStatus.AoiUp.CleanRollerPush)
	{
		pDoc->BtnStatus.AoiUp.PrevCleanRollerPush = pDoc->BtnStatus.AoiUp.CleanRollerPush;
	}
	if (pDoc->BtnStatus.AoiUp.PrevTestStart != pDoc->BtnStatus.AoiUp.TestStart)
	{
		pDoc->BtnStatus.AoiUp.PrevTestStart = pDoc->BtnStatus.AoiUp.TestStart;
	}
	if (pDoc->BtnStatus.AoiUp.PrevUltrasonicDn != pDoc->BtnStatus.AoiUp.UltrasonicDn)
	{
		pDoc->BtnStatus.AoiUp.PrevUltrasonicDn = pDoc->BtnStatus.AoiUp.UltrasonicDn;
	}
	if (pDoc->BtnStatus.AoiUp.PrevUltrasonicRun != pDoc->BtnStatus.AoiUp.UltrasonicRun)
	{
		pDoc->BtnStatus.AoiUp.PrevUltrasonicRun = pDoc->BtnStatus.AoiUp.UltrasonicRun;
	}
	if (pDoc->BtnStatus.AoiUp.PrevUltrasonicSpeed != pDoc->BtnStatus.AoiUp.UltrasonicSpeed)
	{
		pDoc->BtnStatus.AoiUp.PrevUltrasonicSpeed = pDoc->BtnStatus.AoiUp.UltrasonicSpeed;
	}

	// Engrave
	pDoc->BtnStatus.Eng.FeedCw = MpeRead(Plc.DlgMenu03.LampFeedCwEngrave) ? TRUE : FALSE;						// 각인부 피딩 정방향 스위치 램프
	pDoc->BtnStatus.Eng.FeedCcw = MpeRead(Plc.DlgMenu03.LampFeedCcwEngrave) ? TRUE : FALSE;						// 각인부 피딩 역방향 스위치 램프
	pDoc->BtnStatus.Eng.TensionCw = MpeRead(Plc.DlgMenu03.LampTensionCwEngrave) ? TRUE : FALSE;					// 각인부 텐션 정방향 스위치 램프
	pDoc->BtnStatus.Eng.TensionCcw = MpeRead(Plc.DlgMenu03.LampTensionCcwEngrave) ? TRUE : FALSE;				// 각인부 텐션 역방향 스위치 램프
	pDoc->BtnStatus.Eng.FeedHome = MpeRead(Plc.DlgMenu03.LampFeedHomeEngrave) ? TRUE : FALSE;					// 각인부 피딩 HOME 스위치 램프
	pDoc->BtnStatus.Eng.FeedVacuum = MpeRead(Plc.DlgMenu03.LampFeedVacuumEngrave) ? TRUE : FALSE;				// 각인부 피딩 진공 스위치 램프
	pDoc->BtnStatus.Eng.FeedClamp = MpeRead(Plc.DlgMenu03.LampFeedClampEngrave) ? TRUE : FALSE;					// 각인부 피딩 클램프 스위치 램프
	pDoc->BtnStatus.Eng.TensionClamp = MpeRead(Plc.DlgMenu03.LampTensionClampEngrave) ? TRUE : FALSE;			// 각인부 텐션 클램프 스위치 램프
	pDoc->BtnStatus.Eng.TableVacuum = MpeRead(Plc.DlgMenu03.LampTableVacuumEngrave) ? TRUE : FALSE;				// 각인부 테이블 진공 스위치 램프
	pDoc->BtnStatus.Eng.TableBlower = MpeRead(Plc.DlgMenu03.LampTableBlowerEngrave) ? TRUE : FALSE;				// 각인부 테이블 에어블로어 스위치 램프
	pDoc->BtnStatus.Eng.TableCylinder = MpeRead(Plc.DlgMenu03.LampTableCylinderEngrave) ? TRUE : FALSE;			// 각인부 테이블 실린더 스위치 램프
	pDoc->BtnStatus.Eng.UltrasonicDown = MpeRead(Plc.DlgMenu03.LampUltrasonicDownEngrave) ? TRUE : FALSE;		// 각인부 초음파 세정기 하강 스위치 램프
	pDoc->BtnStatus.Eng.UltrasonicRun = MpeRead(Plc.DlgMenu03.LampUltrasonicRunEngrave) ? TRUE : FALSE;			// 각인부 초음파 세정기 운전 스위치 램프
	pDoc->BtnStatus.Eng.UltrasonicSpeed = MpeRead(Plc.DlgMenu03.LampUltrasonicSpeedEngrave) ? TRUE : FALSE;		// 각인부 초음파 세정기 속도 스위치 램프
	pDoc->BtnStatus.Eng.DancerUpper = MpeRead(Plc.DlgMenu03.LampDancerUpperEngrave) ? TRUE : FALSE;				// 각인부 버퍼롤 상승 스위치 램프
	pDoc->BtnStatus.Eng.DancerFixer = MpeRead(Plc.DlgMenu03.LampDancerFixerEngrave) ? TRUE : FALSE;				// 각인부 버퍼롤 고정 스위치 램프
	pDoc->BtnStatus.Eng.AlignStart = MpeRead(Plc.DlgMenu03.LampAlignStartEngrave) ? TRUE : FALSE;				// 각인부 정렬 시작 스위치 램프
	pDoc->BtnStatus.Eng.LaserStart = MpeRead(Plc.DlgMenu03.LampLaserStartEngrave) ? TRUE : FALSE;				// 각인부 각인 시작 스위치 램프
	pDoc->BtnStatus.Eng.ReadStart = MpeRead(Plc.DlgMenu03.LampReadStartEngrave) ? TRUE : FALSE;					// 각인부 리딩 시작 스위치 램프

	if (pDoc->BtnStatus.Eng.PrevFeedCw != pDoc->BtnStatus.Eng.FeedCw)
	{
		pDoc->BtnStatus.Eng.PrevFeedCw = pDoc->BtnStatus.Eng.FeedCw;
	}
	if (pDoc->BtnStatus.Eng.PrevFeedCcw != pDoc->BtnStatus.Eng.FeedCcw)
	{
		pDoc->BtnStatus.Eng.PrevFeedCcw = pDoc->BtnStatus.Eng.FeedCcw;
	}
	if (pDoc->BtnStatus.Eng.PrevTensionCw != pDoc->BtnStatus.Eng.TensionCw)
	{
		pDoc->BtnStatus.Eng.PrevTensionCw = pDoc->BtnStatus.Eng.TensionCw;
	}
	if (pDoc->BtnStatus.Eng.PrevTensionCcw != pDoc->BtnStatus.Eng.TensionCcw)
	{
		pDoc->BtnStatus.Eng.PrevTensionCcw = pDoc->BtnStatus.Eng.TensionCcw;
	}
	if (pDoc->BtnStatus.Eng.PrevFeedHome != pDoc->BtnStatus.Eng.FeedHome)
	{
		pDoc->BtnStatus.Eng.PrevFeedHome = pDoc->BtnStatus.Eng.FeedHome;
	}
	if (pDoc->BtnStatus.Eng.PrevFeedVacuum != pDoc->BtnStatus.Eng.FeedVacuum)
	{
		pDoc->BtnStatus.Eng.PrevFeedVacuum = pDoc->BtnStatus.Eng.FeedVacuum;
	}
	if (pDoc->BtnStatus.Eng.PrevFeedClamp != pDoc->BtnStatus.Eng.FeedClamp)
	{
		pDoc->BtnStatus.Eng.PrevFeedClamp = pDoc->BtnStatus.Eng.FeedClamp;
	}
	if (pDoc->BtnStatus.Eng.PrevTensionClamp != pDoc->BtnStatus.Eng.TensionClamp)
	{
		pDoc->BtnStatus.Eng.PrevTensionClamp = pDoc->BtnStatus.Eng.TensionClamp;
	}
	if (pDoc->BtnStatus.Eng.PrevTableVacuum != pDoc->BtnStatus.Eng.TableVacuum)
	{
		pDoc->BtnStatus.Eng.PrevTableVacuum = pDoc->BtnStatus.Eng.TableVacuum;
	}
	if (pDoc->BtnStatus.Eng.PrevTableBlower != pDoc->BtnStatus.Eng.TableBlower)
	{
		pDoc->BtnStatus.Eng.PrevTableBlower = pDoc->BtnStatus.Eng.TableBlower;
	}
	if (pDoc->BtnStatus.Eng.PrevTableCylinder != pDoc->BtnStatus.Eng.TableCylinder)
	{
		pDoc->BtnStatus.Eng.PrevTableCylinder = pDoc->BtnStatus.Eng.TableCylinder;
	}
	if (pDoc->BtnStatus.Eng.PrevUltrasonicDown != pDoc->BtnStatus.Eng.UltrasonicDown)
	{
		pDoc->BtnStatus.Eng.PrevUltrasonicDown = pDoc->BtnStatus.Eng.UltrasonicDown;
	}
	if (pDoc->BtnStatus.Eng.PrevUltrasonicRun != pDoc->BtnStatus.Eng.UltrasonicRun)
	{
		pDoc->BtnStatus.Eng.PrevUltrasonicRun = pDoc->BtnStatus.Eng.UltrasonicRun;
	}
	if (pDoc->BtnStatus.Eng.PrevUltrasonicSpeed != pDoc->BtnStatus.Eng.UltrasonicSpeed)
	{
		pDoc->BtnStatus.Eng.PrevUltrasonicSpeed = pDoc->BtnStatus.Eng.UltrasonicSpeed;
	}
	if (pDoc->BtnStatus.Eng.PrevDancerUpper != pDoc->BtnStatus.Eng.DancerUpper)
	{
		pDoc->BtnStatus.Eng.PrevDancerUpper = pDoc->BtnStatus.Eng.DancerUpper;
	}
	if (pDoc->BtnStatus.Eng.PrevDancerFixer != pDoc->BtnStatus.Eng.DancerFixer)
	{
		pDoc->BtnStatus.Eng.PrevAlignStart = pDoc->BtnStatus.Eng.AlignStart;
	}
	if (pDoc->BtnStatus.Eng.PrevLaserStart != pDoc->BtnStatus.Eng.LaserStart)
	{
		pDoc->BtnStatus.Eng.PrevLaserStart = pDoc->BtnStatus.Eng.LaserStart;
	}
	if (pDoc->BtnStatus.Eng.PrevReadStart != pDoc->BtnStatus.Eng.ReadStart)
	{
		pDoc->BtnStatus.Eng.PrevReadStart = pDoc->BtnStatus.Eng.ReadStart;
	}

	// Uncoiler
	pDoc->BtnStatus.Uc.ChuckPcb = MpeRead(Plc.DlgMenu03.LampChuckPcbUncoiler) ? TRUE : FALSE;					// 언코일러 제품척 클램프 스위치 램프
	pDoc->BtnStatus.Uc.ChuckPaper = MpeRead(Plc.DlgMenu03.LampChuckPaperUncoiler) ? TRUE : FALSE;				// 언코일러 간지척 클램프 스위치 램프
	pDoc->BtnStatus.Uc.JoinClamp = MpeRead(Plc.DlgMenu03.LampJoinClampUncoiler) ? TRUE : FALSE;					// 언코일러 이음매 클램프 스위치 램프
	pDoc->BtnStatus.Uc.PcbShaftSupport = MpeRead(Plc.DlgMenu03.LampPcbShaftSupportUncoiler) ? TRUE : FALSE;		// 언코일러 제품샤프트 지지대 스위치 램프
	pDoc->BtnStatus.Uc.EpcActHome = MpeRead(Plc.DlgMenu03.LampEpcActHomeUncoiler) ? TRUE : FALSE;				// 언코일러 EPC 액추에이터 HOME 스위치 램프
	pDoc->BtnStatus.Uc.EpcActFirst = MpeRead(Plc.DlgMenu03.LampEpcActFirstUncoiler) ? TRUE : FALSE;				// 언코일러 EPC 액추에이터 #1 스위치 램프
	pDoc->BtnStatus.Uc.EpcActSecond = MpeRead(Plc.DlgMenu03.LampEpcActSecondUncoiler) ? TRUE : FALSE;			// 언코일러 EPC 액추에이터 #2 스위치 램프
	pDoc->BtnStatus.Uc.EpcActThird = MpeRead(Plc.DlgMenu03.LampEpcActThirdUncoiler) ? TRUE : FALSE;				// 언코일러 EPC 액추에이터 #3 스위치 램프
	pDoc->BtnStatus.Uc.EpcAuto = MpeRead(Plc.DlgMenu03.LampEpcAutoUncoiler) ? TRUE : FALSE;						// 언코일러 EPC 자동 스위치 램프
	pDoc->BtnStatus.Uc.EpcManual = MpeRead(Plc.DlgMenu03.LampEpcManualUncoiler) ? TRUE : FALSE;					// 언코일러 EPC 수동 스위치 램프
	pDoc->BtnStatus.Uc.EpcHome = MpeRead(Plc.DlgMenu03.LampEpcHomeUncoiler) ? TRUE : FALSE;						// 언코일러 EPC 원점 스위치 램프
	pDoc->BtnStatus.Uc.EpcIn = MpeRead(Plc.DlgMenu03.LampEpcInUncoiler) ? TRUE : FALSE;							// 언코일러 EPC In 스위치 램프
	pDoc->BtnStatus.Uc.EpcOut = MpeRead(Plc.DlgMenu03.LampEpcOutUncoiler) ? TRUE : FALSE;						// 언코일러 EPC Out 스위치 램프
	pDoc->BtnStatus.Uc.PcbInverterCw = MpeRead(Plc.DlgMenu03.LampPcbInverterCwUncoiler) ? TRUE : FALSE;			// 언코일러 제품 인버터 정방향 스위치 램프
	pDoc->BtnStatus.Uc.PcbInverterCcw = MpeRead(Plc.DlgMenu03.LampPcbInverterCcwUncoiler) ? TRUE : FALSE;		// 언코일러 제품 인버터 역방향 스위치 램프
	pDoc->BtnStatus.Uc.PaperInverterCw = MpeRead(Plc.DlgMenu03.LampPaperInverterCwUncoiler) ? TRUE : FALSE;		// 언코일러 간지 인버터 정방향 스위치 램프
	pDoc->BtnStatus.Uc.PaperInverterCcw = MpeRead(Plc.DlgMenu03.LampPaperInverterCcwUncoiler) ? TRUE : FALSE;	// 언코일러 간지 인버터 역방향 스위치 램프
	pDoc->BtnStatus.Uc.DancerUpper = MpeRead(Plc.DlgMenu03.LampDancerUpperUncoiler) ? TRUE : FALSE;				// 언코일러 댄서롤 상승 스위치 램프
	pDoc->BtnStatus.Uc.DancerFixer = MpeRead(Plc.DlgMenu03.LampDancerFixerUncoiler) ? TRUE : FALSE;				// 언코일러 댄서롤 고정 스위치 램프

	if (pDoc->BtnStatus.Uc.PrevChuckPcb != pDoc->BtnStatus.Uc.ChuckPcb)
	{
		pDoc->BtnStatus.Uc.PrevChuckPcb = pDoc->BtnStatus.Uc.ChuckPcb;
		//pDoc->SetMkMenu03(_T("Main"), _T("Relation"), pDoc->BtnStatus.Rc.Relation);
	}
	if (pDoc->BtnStatus.Uc.PrevChuckPaper != pDoc->BtnStatus.Uc.ChuckPaper)
	{
		pDoc->BtnStatus.Uc.PrevChuckPaper = pDoc->BtnStatus.Uc.ChuckPaper;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("MvCw"), pDoc->BtnStatus.Rc.FdCw);
	}
	if (pDoc->BtnStatus.Uc.PrevJoinClamp != pDoc->BtnStatus.Uc.JoinClamp)
	{
		pDoc->BtnStatus.Uc.PrevJoinClamp = pDoc->BtnStatus.Uc.JoinClamp;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("MvCcw"), pDoc->BtnStatus.Rc.FdCcw);
	}
	if (pDoc->BtnStatus.Uc.PrevPcbShaftSupport != pDoc->BtnStatus.Uc.PcbShaftSupport)
	{
		pDoc->BtnStatus.Uc.PrevPcbShaftSupport = pDoc->BtnStatus.Uc.PcbShaftSupport;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PrdChuck"), pDoc->BtnStatus.Rc.ReelChuck);
	}
	if (pDoc->BtnStatus.Uc.PrevEpcActHome != pDoc->BtnStatus.Uc.EpcActHome)
	{
		pDoc->BtnStatus.Uc.PrevEpcActHome = pDoc->BtnStatus.Uc.EpcActHome;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("DancerUp"), pDoc->BtnStatus.Rc.DcRlUpDn);
	}
	if (pDoc->BtnStatus.Uc.PrevEpcActFirst != pDoc->BtnStatus.Uc.EpcActFirst)
	{
		pDoc->BtnStatus.Uc.PrevEpcActFirst = pDoc->BtnStatus.Uc.EpcActFirst;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PasteUpLf"), pDoc->BtnStatus.Rc.ReelJoinL);
	}
	if (pDoc->BtnStatus.Uc.PrevEpcActSecond != pDoc->BtnStatus.Uc.EpcActSecond)
	{
		pDoc->BtnStatus.Uc.PrevEpcActSecond = pDoc->BtnStatus.Uc.EpcActSecond;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PasteUpRt"), pDoc->BtnStatus.Rc.ReelJoinR);
	}
	if (pDoc->BtnStatus.Uc.PrevEpcActThird != pDoc->BtnStatus.Uc.EpcActThird)
	{
		pDoc->BtnStatus.Uc.PrevEpcActThird = pDoc->BtnStatus.Uc.EpcActThird;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PasteVac"), pDoc->BtnStatus.Rc.ReelJoinVac);
	}
	if (pDoc->BtnStatus.Uc.PrevEpcAuto != pDoc->BtnStatus.Uc.EpcAuto)
	{
		pDoc->BtnStatus.Uc.PrevEpcAuto = pDoc->BtnStatus.Uc.EpcAuto;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PprChuck"), pDoc->BtnStatus.Rc.PprChuck);
	}
	if (pDoc->BtnStatus.Uc.PrevEpcManual != pDoc->BtnStatus.Uc.EpcManual)
	{
		pDoc->BtnStatus.Uc.PrevEpcManual = pDoc->BtnStatus.Uc.EpcManual;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PprCw"), pDoc->BtnStatus.Rc.PprCw);
	}
	if (pDoc->BtnStatus.Uc.PrevEpcHome != pDoc->BtnStatus.Uc.EpcHome)
	{
		pDoc->BtnStatus.Uc.PrevEpcHome = pDoc->BtnStatus.Uc.EpcHome;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PprCcw"), pDoc->BtnStatus.Rc.PprCcw);
	}
	if (pDoc->BtnStatus.Uc.PrevEpcIn != pDoc->BtnStatus.Uc.EpcIn)
	{
		pDoc->BtnStatus.Uc.PrevEpcIn = pDoc->BtnStatus.Uc.EpcIn;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("DoRewind"), pDoc->BtnStatus.Rc.Rewine);
	}
	if (pDoc->BtnStatus.Uc.PrevEpcOut != pDoc->BtnStatus.Uc.EpcOut)
	{
		pDoc->BtnStatus.Uc.PrevEpcOut = pDoc->BtnStatus.Uc.EpcOut;
		//pDoc->SetMkMenu03(_T("Recoiler"), _T("PrdPprRewind"), pDoc->BtnStatus.Rc.RewineReelPpr);
	}
	if (pDoc->BtnStatus.Uc.PrevPcbInverterCw != pDoc->BtnStatus.Uc.PcbInverterCw)
	{
		pDoc->BtnStatus.Uc.PrevPcbInverterCw = pDoc->BtnStatus.Uc.PcbInverterCw;
	}
	if (pDoc->BtnStatus.Uc.PrevPcbInverterCcw != pDoc->BtnStatus.Uc.PcbInverterCcw)
	{
		pDoc->BtnStatus.Uc.PrevPcbInverterCcw = pDoc->BtnStatus.Uc.PcbInverterCcw;
	}
	if (pDoc->BtnStatus.Uc.PrevPaperInverterCw != pDoc->BtnStatus.Uc.PaperInverterCw)
	{
		pDoc->BtnStatus.Uc.PrevPaperInverterCw = pDoc->BtnStatus.Uc.PaperInverterCw;
	}
	if (pDoc->BtnStatus.Uc.PrevPaperInverterCcw != pDoc->BtnStatus.Uc.PaperInverterCcw)
	{
		pDoc->BtnStatus.Uc.PrevPaperInverterCcw = pDoc->BtnStatus.Uc.PaperInverterCcw;
	}
	if (pDoc->BtnStatus.Uc.PrevDancerUpper != pDoc->BtnStatus.Uc.DancerUpper)
	{
		pDoc->BtnStatus.Uc.PrevDancerUpper = pDoc->BtnStatus.Uc.DancerUpper;
	}
	if (pDoc->BtnStatus.Uc.PrevDancerFixer != pDoc->BtnStatus.Uc.DancerFixer)
	{
		pDoc->BtnStatus.Uc.PrevDancerFixer = pDoc->BtnStatus.Uc.DancerFixer;
	}
}

void CGvisR2R_PunchView::InitIoWrite()
{
	return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!m_pMpe)
		return;

	MpeWrite(Plc.DlgMenu01.Buzzer1, 0); // 부저1 On  (PC가 ON, OFF) - 20141020
	MpeWrite(Plc.DlgMenu01.Buzzer2, 0); // 부저2 On  (PC가 ON, OFF) - 20141020

	MpeWrite(Plc.DlgMenu01.TestStartAoiUp, 0); // 검사부 상 검사 시작 <-> Y4368 I/F
	MpeWrite(Plc.DlgMenu01.TableVacuumAoiUp, 0); // 검사부 상 검사 테이블 진공 SOL <-> Y4369 I/F
	MpeWrite(Plc.DlgMenu01.ResetAoiUp, 0); // 검사부 상 Reset <-> Y436A I/F
	MpeWrite(Plc.DlgMenu01.LotEndAoiUp, 0); // 검사부 상 Lot End <-> Y436B I/F

	MpeWrite(Plc.DlgMenu01.TestStartAoiDn, 0); // 검사부 하 검사 시작 <-> Y4468 I/F
	MpeWrite(Plc.DlgMenu01.TableVacuumAoiDn, 0); // 검사부 하 검사 테이블 진공 SOL <-> Y4369 I/F
	MpeWrite(Plc.DlgMenu01.ResetAoiDn, 0); // 검사부 하 Reset <-> Y436A I/F
	MpeWrite(Plc.DlgMenu01.LotEndAoiDn, 0); // 검사부 하 Lot End <-> Y436B I/F

	MpeWrite(Plc.DlgMenu01.AutoInitStatus, 0); // 자동 초기 운전상태(PC가 On/Off 시킴, PLC가 운전램프를 윙크동작, on->off시 운전램프 on, 다시 운전스위치가 눌러지면 off) - 20141017
	if(bDualTest)
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiDn, 0); // 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)
	else
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiUp, 0);
	MpeWrite(Plc.DlgFrameHigh.FeedOffsetPunch, 0); // 마킹부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)

	MpeWrite(Plc.DlgMenu01.AlarmMonitor, 0); // PC가 PLC의 Alarm 발생여부를 확인
	MpeWrite(Plc.DlgMenu01.AlarmMonitorRtn, 0); // PC가 PLC의 Alarm 발생여부를 확인
	MpeWrite(Plc.DlgMenu01.AlarmPage, 0); // 알람이 발생한 페이지 지정(PLC가 표시 할 알람의 페이지를 가리킴).
}

BOOL CGvisR2R_PunchView::MoveAlign0(int nPos)
{
	if (!m_pMotion)
		return FALSE;

	if (m_pDlgMenu02)
		m_pDlgMenu02->SetLight();

	int nAlignMethode = pDoc->WorkingInfo.LastJob.nAlignMethode;

	if (m_pMotion->m_dPinPosY[0] > 0.0 && m_pMotion->m_dPinPosX[0] > 0.0)
	{
		double dCurrX = m_dEnc[AXIS_X0];
		double dCurrY = m_dEnc[AXIS_Y0];

		if (nAlignMethode == FOUR_POINT)
		{
			double pPos[4];
			if (nPos == 0)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X0 + m_pMotion->m_dPinPosX[0];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y0 + m_pMotion->m_dPinPosY[0];
			}
			else if (nPos == 1)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X1 + m_pMotion->m_dPinPosX[0];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y1 + m_pMotion->m_dPinPosY[0];
			}
			else if (nPos == 2)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X2 + m_pMotion->m_dPinPosX[0];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y2 + m_pMotion->m_dPinPosY[0];
			}
			else if (nPos == 3)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X3 + m_pMotion->m_dPinPosX[0];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y3 + m_pMotion->m_dPinPosY[0];
			}

			if (ChkCollision(AXIS_X0, pPos[0]))
				return FALSE;

			double fLen, fVel, fAcc, fJerk;
			fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
			if (fLen > 0.001)
			{
				pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
				if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT))
				{
					if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT))
					{
						pView->SetAlarmToPlc(UNIT_PUNCH);
						pView->ClrDispMsg();
						AfxMessageBox(_T("Error - Move MoveAlign0 ..."));
						return FALSE;
					}
				}
			}
		}
		else
		{
			double pPos[2];
			if (nPos == 0)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk.X0 + m_pMotion->m_dPinPosX[0];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y0 + m_pMotion->m_dPinPosY[0];
			}
			else if (nPos == 1)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk.X1 + m_pMotion->m_dPinPosX[0];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y1 + m_pMotion->m_dPinPosY[0];
			}

			if (ChkCollision(AXIS_X0, pPos[0]))
				return FALSE;

			double fLen, fVel, fAcc, fJerk;
			fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
			if (fLen > 0.001)
			{
				pView->m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
				if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT))
				{
					if (!pView->m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT))
					{
						pView->SetAlarmToPlc(UNIT_PUNCH);
						pView->ClrDispMsg();
						AfxMessageBox(_T("Error - Move MoveAlign0 ..."));
						return FALSE;
					}
				}
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::MoveAlign1(int nPos)
{
	if (!m_pMotion)
		return FALSE;

	if (m_pDlgMenu02)
		m_pDlgMenu02->SetLight2();

	int nAlignMethode = pDoc->WorkingInfo.LastJob.nAlignMethode;

	if (m_pMotion->m_dPinPosY[1] > 0.0 && m_pMotion->m_dPinPosX[1] > 0.0)
	{
		double dCurrX = m_dEnc[AXIS_X1];
		double dCurrY = m_dEnc[AXIS_Y1];

		if (nAlignMethode == FOUR_POINT)
		{
			double pPos[4];
			if (nPos == 0)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X0 + m_pMotion->m_dPinPosX[1];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y0 + m_pMotion->m_dPinPosY[1];
			}
			else if (nPos == 1)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X1 + m_pMotion->m_dPinPosX[1];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y1 + m_pMotion->m_dPinPosY[1];
			}
			else if (nPos == 2)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X2 + m_pMotion->m_dPinPosX[1];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y2 + m_pMotion->m_dPinPosY[1];
			}
			else if (nPos == 3)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk2.X3 + m_pMotion->m_dPinPosX[1];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk2.Y3 + m_pMotion->m_dPinPosY[1];
			}

			if (ChkCollision(AXIS_X1, pPos[0]))
				return FALSE;

			double fLen, fVel, fAcc, fJerk;
			fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
			if (fLen > 0.001)
			{
				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
				if (!m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT))
				{
					if (!m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT))
					{
						pView->SetAlarmToPlc(UNIT_PUNCH);
						pView->ClrDispMsg();
						AfxMessageBox(_T("Error - Move MoveAlign1 ..."));
						return FALSE;
					}
				}
			}
		}
		else
		{
			double pPos[2];
			if (nPos == 0)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk.X0 + m_pMotion->m_dPinPosX[1];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y0 + m_pMotion->m_dPinPosY[1];
			}
			else if (nPos == 1)
			{
				pPos[0] = pDoc->m_Master[0].m_stAlignMk.X1 + m_pMotion->m_dPinPosX[1];
				pPos[1] = pDoc->m_Master[0].m_stAlignMk.Y1 + m_pMotion->m_dPinPosY[1];
			}

			if (ChkCollision(AXIS_X1, pPos[0]))
				return FALSE;

			double fLen, fVel, fAcc, fJerk;
			fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
			if (fLen > 0.001)
			{
				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X1, fLen, fVel, fAcc, fJerk);
				if (!m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT))
				{
					if (!m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc, ABS, NO_WAIT))
					{
						pView->SetAlarmToPlc(UNIT_PUNCH);
						pView->ClrDispMsg();
						AfxMessageBox(_T("Error - Move MoveAlign1 ..."));
						return FALSE;
					}
				}
			}
		}

		return TRUE;
	}

	return FALSE;
}

void CGvisR2R_PunchView::SetLastProc()
{

	if (m_pDlgMenu01)
		m_pDlgMenu01->SetLastProc();
}

BOOL CGvisR2R_PunchView::IsLastProc()
{
	BOOL bRtn = FALSE;
	if (m_pDlgMenu01)
		bRtn = m_pDlgMenu01->IsLastProc();
	else
		bRtn = FALSE;

	return bRtn;
}

BOOL CGvisR2R_PunchView::IsLastJob(int nAoi) // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
{
	switch (nAoi)
	{
	case 0: // AOI-Up
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModel.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerUp.IsEmpty())
			return FALSE;
		break;
	case 1: // AOI-Dn
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModel.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerDn.IsEmpty())
			return FALSE;
		break;
	case 2: // AOI-UpDn
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModel.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerUp.IsEmpty())
			return FALSE;
		if (pDoc->WorkingInfo.System.sPathCamSpecDir.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sModel.IsEmpty() ||
			pDoc->WorkingInfo.LastJob.sLayerDn.IsEmpty())
			return FALSE;
		break;
	}

	return TRUE;
}

void CGvisR2R_PunchView::MonPlcSignal()
{
#ifdef USE_MPE
	BOOL bChk = MpeRead(_T("MB400108")); // GUI 각인기 리딩 완료
	//if (pDoc->m_pMpeSignal[30] & (0x01 << 8))		// 각인부 2D Leading 작업완료(PLC가 ON/OFF) - MB440130
	if (bChk)		// 각인부 2D Leading 작업완료(PLC가 ON/OFF) - MB440130
	{
		if (m_pDlgMenu03)
			m_pDlgMenu03->SetLed(0, TRUE);
	}
	else
	{
		if (m_pDlgMenu03)
			m_pDlgMenu03->SetLed(0, FALSE);
	}

	bChk = MpeRead(_T("MB400105")); // GUI 각인기 레이저 완료
	//if (pDoc->m_pMpeSignal[30] & (0x01 << 5))		// 각인부 Laser 작업완료(PLC가 ON/OFF) - MB440132
	if (bChk)		// 각인부 Laser 작업완료(PLC가 ON/OFF) - MB440132
	{
		if (m_pDlgMenu03)
			m_pDlgMenu03->SetLed(1, TRUE);
	}
	else
	{
		if (m_pDlgMenu03)
			m_pDlgMenu03->SetLed(1, FALSE);
	}

	bChk = MpeRead(_T("MB400122")); // GUI AOI 상면 검사 완료
	//if (pDoc->m_pMpeSignal[32] & (0x01 << 2))		// 검사부 상면 검사 작업완료(PLC가 ON/OFF) - MB440133
	if (bChk)		// 검사부 상면 검사 작업완료(PLC가 ON/OFF) - MB440133
	{
		if (m_pDlgMenu03)
			m_pDlgMenu03->SetLed(2, TRUE);
	}
	else
	{
		if (m_pDlgMenu03)
			m_pDlgMenu03->SetLed(2, FALSE);
	}

	bChk = MpeRead(_T("MB400138")); // GUI AOI 하면 검사 완료
	//if (pDoc->m_pMpeSignal[33] & (0x01 << 8))		// 검사부 하면 검사 작업완료(PLC가 ON/OFF) - MB440134
	if (bChk)		// 검사부 하면 검사 작업완료(PLC가 ON/OFF) - MB440134
	{
		if (m_pDlgMenu03)
			m_pDlgMenu03->SetLed(3, TRUE);
	}
	else
	{
		if (m_pDlgMenu03)
			m_pDlgMenu03->SetLed(3, FALSE);
	}

	bChk = MpeRead(_T("MB400152")); // GUI 마킹 검사 완료
	//if (pDoc->m_pMpeSignal[35] & (0x01 << 2))		// 마킹부 마킹 작업완료(PLC가 ON/OFF) - MB440135
	if (bChk)		// 마킹부 마킹 작업완료(PLC가 ON/OFF) - MB440135
	{
		if (m_pDlgMenu03)
			m_pDlgMenu03->SetLed(4, TRUE);
	}
	else
	{
		if (m_pDlgMenu03)
			m_pDlgMenu03->SetLed(4, FALSE);
	}

	bChk = MpeRead(_T("MB400009")); // GUI 자동 이어가기 운전
	//if (pDoc->m_pMpeSignal[20] & (0x01 << 9))		// 내층 제품시 이어가기 상태 표시 - MB440125
	if (bChk)		// 내층 제품시 이어가기 상태 표시 - MB440125
	{
		DispContRun(TRUE);
	}
	else
	{
		DispContRun(FALSE);
	}

#endif
}


void CGvisR2R_PunchView::DispContRun(BOOL bOn)
{
	if (pDoc->WorkingInfo.LastJob.bDispContRun != bOn)
	{
		pDoc->WorkingInfo.LastJob.bDispContRun = bOn;
		pDoc->SetMkInfo(_T("Signal"), _T("DispContRun"), bOn);

#ifdef USE_ENGRAVE
		if (pView && pView->m_pEngrave)
			pView->m_pEngrave->SetDispContRun();	//_stSigInx::_DispContRun
#endif

	}
}

void CGvisR2R_PunchView::DispLotEnd(BOOL bOn)
{
	if (pDoc->WorkingInfo.LastJob.bDispLotEnd != bOn)
	{
		pDoc->WorkingInfo.LastJob.bDispLotEnd = bOn;
		pDoc->SetMkInfo(_T("Signal"), _T("DispLotEnd"), bOn);

#ifdef USE_ENGRAVE
		if (pView && pView->m_pEngrave)
			pView->m_pEngrave->SetDispLotEnd();	//_stSigInx::_DispLotEnd
#endif

	}
}

void CGvisR2R_PunchView::MonDispMain() // PLC의 운전상태 표시
{
	BOOL bDispStop = TRUE;
	if (!pDoc->m_pMpeSignal)
		return;

	BOOL bChk = MpeRead(_T("MB400206")); // PLC 자동 운전 중
	//if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[0] & (0x01 << 6))		// 운전중(PLC가 PC에 알려주는 설비 상태) - 20141031
	if (bChk)		// 운전중(PLC가 PC에 알려주는 설비 상태) - 20141031
	{
		if (m_sDispMain != _T("운전중"))
		{
			DispMain(_T("운전중"), RGB_GREEN);	
			pDoc->SetMkMenu03(_T("Main"), _T("Run"), TRUE);
			pDoc->SetMkMenu03(_T("Main"), _T("Stop"), FALSE);
		}
	}

	BOOL bChkReady = MpeRead(_T("MB400203")); // PLC 운전 준비 완료
	BOOL bChkOnStopping = MpeRead(_T("MB400200")); // PLC 정지 중
	BOOL bChkAutoInit = MpeRead(_T("MB400208")); // PLC 자동 초기 모드 중

	//if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[0] & (0x01 << 3))		// 운전준비(PLC가 PC에 알려주는 설비 상태) - 20141031
	if (bChkReady)		// 운전준비(PLC가 PC에 알려주는 설비 상태) - 20141031
	{
		bDispStop = FALSE;
		if (m_sDispMain != _T("운전준비"))
		{
			DispMain(_T("운전준비"), RGB_GREEN);
			pDoc->SetMkMenu03(_T("Main"), _T("Ready"), TRUE);
		}
	}
	//else if(!(pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[0] & (0x01 << 0)))
	else if (!bChkOnStopping) // PLC 정지 중
	{
		//if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[0] & (0x01 << 8))		// 초기운전(PLC가 PC에 알려주는 설비 상태) - 20141031
		if (bChkAutoInit)		// 초기운전(PLC가 PC에 알려주는 설비 상태) - 20141031
		{
			bDispStop = FALSE;
			{
				if (pDoc->WorkingInfo.LastJob.bSampleTest)
				{
					if (pDoc->WorkingInfo.LastJob.bDualTest)
					{
						if (m_sDispMain != _T("양면샘플"))
						{
							DispMain(_T("양면샘플"), RGB_GREEN);
						}
					}
					else
					{
						if (m_sDispMain != _T("단면샘플"))
						{
							DispMain(_T("단면샘플"), RGB_GREEN);
						}
					}
				}
				else if (pDoc->GetTestMode() == MODE_INNER)
				{
					if (m_sDispMain != _T("내층검사"))
						DispMain(_T("내층검사"), RGB_GREEN);
				}
				else if (pDoc->GetTestMode() == MODE_OUTER)
				{
					if (m_sDispMain != _T("외층검사"))
						DispMain(_T("외층검사"), RGB_GREEN);
				}
				else if (pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (m_sDispMain != _T("양면검사"))
					{
						DispMain(_T("양면검사"), RGB_GREEN);
					}
				}
				else
				{
					if (m_sDispMain != _T("단면검사"))
					{
						DispMain(_T("단면검사"), RGB_GREEN);
					}
				}
			}
		}
		else
		{
			if (m_sDispMain != _T("운전준비"))
			{
				bDispStop = TRUE;
			}
			else
			{
				bDispStop = FALSE;
			}
		}
	}


	//if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[0] & (0x01 << 0))		// 정지(PLC가 PC에 알려주는 설비 상태) - 20141031
	if (bChkOnStopping)		// 정지(PLC가 PC에 알려주는 설비 상태) - 20141031
	{
		if (m_sDispMain != _T("정 지"))
		{
			DispMain(_T("정 지"), RGB_RED);
			pDoc->SetMkMenu03(_T("Main"), _T("Stop"), TRUE);
			pDoc->SetMkMenu03(_T("Main"), _T("Run"), FALSE);
		}
	}
}

void CGvisR2R_PunchView::MonPlcAlm()
{
	BOOL bMon, bClr;
	long lOn = MpeRead(Plc.DlgMenu01.AlarmMonitor);

	bMon = lOn & (0x01 << 0);
	bClr = lOn & (0x01 << 1);
	if (bMon)
		PlcAlm(bMon, 0);
	else if (bClr)
		PlcAlm(0, bClr);
	else
		PlcAlm(bMon, bClr);

	if (!pDoc->m_sAlmMsg.IsEmpty())
	{
		if (pDoc->m_sAlmMsg != pDoc->m_sPrevAlmMsg)
		{
			pDoc->m_sPrevAlmMsg = pDoc->m_sAlmMsg;
			CycleStop(); // 알람발생시
		}
	}
}

void CGvisR2R_PunchView::PlcAlm(BOOL bMon, BOOL bClr)
{
	if (bMon && !m_nMonAlmF)
	{
		m_nMonAlmF = 1;
		FindAlarm();
		if (pView->m_pEngrave)
		{
			pDoc->m_sIsAlmMsg = _T("");
			pView->m_pEngrave->SetAlarm(pDoc->m_sAlmMsg);
		}

		Sleep(300);
		MpeWrite(Plc.DlgMenu01.AlarmMonitorRtn, 1);
	}
	else if (!bMon && m_nMonAlmF)
	{
		m_nMonAlmF = 0;
		ResetMonAlm();
	}


	if (bClr && !m_nClrAlmF)
	{
		m_nClrAlmF = 1;
		ClrAlarm();
		if (pView->m_pEngrave)
		{
			pDoc->m_sAlmMsg = _T("");
			pView->m_pEngrave->SetAlarm(pDoc->m_sAlmMsg);
		}
		Sleep(300);
		MpeWrite(Plc.DlgMenu01.AlarmClearRtn, 1);
	}
	else if (!bClr && m_nClrAlmF)
	{
		m_nClrAlmF = 0;
		ResetClear();
	}
}

void CGvisR2R_PunchView::FindAlarm()
{
	// 알람이 발생한 페이지
	long lAlmPage = -1;
	lAlmPage = MpeRead(Plc.DlgMenu01.AlarmPage);

	TCHAR szData[200];
	CString str1, str2, str3, strM, str, strH = _T("");

	str1.Format(_T("%d"), lAlmPage);
	str2 = _T("Address");
	if (0 < ::GetPrivateProfileString(str1, str2, NULL, szData, sizeof(szData), PATH_ALARM))
		strM = CString(szData);
	else
		strM = _T("");

	if (strM.IsEmpty())
		return;

	long lAlm = MpeRead(strM); // lAlm : (32Bits << Row)
	for (int i = 0; i<32; i++)
	{
		if (lAlm & (0x01 << i))
		{
			str3.Format(_T("%d"), i);
			if (0 < ::GetPrivateProfileString(str1, str3, NULL, szData, sizeof(szData), PATH_ALARM))
				strH = CString(szData);
			else
				strH.Format(_T("%s = %d"), strM, lAlm);
				//strH = _T("");

			if (str.IsEmpty())
				str = strH;
			else
			{
				str += _T("\r\n");
				str += strH;
			}
		}
	}

	pDoc->m_sAlmMsg = strH;
}

void CGvisR2R_PunchView::ResetMonAlm()
{
	MpeWrite(Plc.DlgMenu01.AlarmMonitorRtn, 0);
}

void CGvisR2R_PunchView::ClrAlarm()
{
	if (!pDoc->m_sAlmMsg.IsEmpty())
	{
		pDoc->m_sAlmMsg = _T("");
		pDoc->m_sIsAlmMsg = _T("");
		pDoc->m_sPrevAlmMsg = _T("");
	}
}

void CGvisR2R_PunchView::ResetClear()
{
	MpeWrite(Plc.DlgMenu01.AlarmClearRtn, 0);
}

void CGvisR2R_PunchView::ChkTempStop(BOOL bChk)
{
	if (bChk)
	{
		if (!m_bTIM_CHK_TEMP_STOP)
		{
			m_bTIM_CHK_TEMP_STOP = TRUE;
			SetTimer(TIM_CHK_TEMP_STOP, 500, NULL);
		}
	}
	else
	{
		m_bTIM_CHK_TEMP_STOP = FALSE;
	}
}

void CGvisR2R_PunchView::ChgLot()
{
	pDoc->WorkingInfo.LastJob.sLot = pDoc->Status.PcrShare[0].sLot;
	pDoc->SetModelInfoUp();
	pDoc->SetModelInfoDn();
	SetPathAtBuf();
}

BOOL CGvisR2R_PunchView::LoadPcrFromBuf() // 프로그램 처음 시작시 버퍼에 저장된 파일을 릴맵에 최초로 업데이트 함.
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString str, sTemp;
	BOOL bRtn = FALSE;

	if (ChkBufUp(m_pBufSerial[0], m_nBufTot[0]))
	{
		for (int i = 0; i < m_nBufTot[0]; i++)
		{
			if (!LoadPcrUp(m_pBufSerial[0][i]))
				return FALSE;

			if (!bDualTest)
			{
				UpdateReelmap(m_pBufSerial[0][i]);
				bRtn = TRUE;
			}
		}
	}

	if (bDualTest)
	{
		if (ChkBufDn(m_pBufSerial[1], m_nBufTot[1]))
		{
			for (int i = 0; i < m_nBufTot[1]; i++)
			{
				if (!LoadPcrDn(m_pBufSerial[1][i]))
					return FALSE;

				UpdateReelmap(m_pBufSerial[1][i]); // After inspect bottom side.
				bRtn = TRUE;
			}
		}
	}

	return bRtn;
}

void CGvisR2R_PunchView::SetPathAtBuf()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->SetPathAtBuf();
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->SetPathAtBuf();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->SetPathAtBuf();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->SetPathAtBuf();
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->m_pReelMapIts)
			pDoc->m_pReelMapIts->SetPathAtBuf();
	}
}

void CGvisR2R_PunchView::SetPathAtBufUp()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->SetPathAtBuf();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->SetPathAtBuf();
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		SetInnerPathAtBufUp();
	}
}

void CGvisR2R_PunchView::SetPathAtBufDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	if (pDoc->m_pReelMapDn)
		pDoc->m_pReelMapDn->SetPathAtBuf();
	if (pDoc->m_pReelMapAllDn)
		pDoc->m_pReelMapAllDn->SetPathAtBuf();

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		SetInnerPathAtBufDn();
	}
}


void CGvisR2R_PunchView::SetInnerPathAtBuf()
{
}

void CGvisR2R_PunchView::SetInnerPathAtBufUp()
{
}

void CGvisR2R_PunchView::SetInnerPathAtBufDn()
{
}



void  CGvisR2R_PunchView::SetLotLastShot()
{
	pDoc->m_nLotLastShot = int(_tstof(pDoc->WorkingInfo.LastJob.sLotSepLen)*1000.0 / _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen));
}

BOOL CGvisR2R_PunchView::IsMkStrip(int nStripIdx)
{
	if (!m_pDlgMenu01 || nStripIdx < 1 || nStripIdx > MAX_STRIP_NUM)
		return TRUE;

	return (m_pDlgMenu01->GetChkStrip(nStripIdx - 1));
}

void CGvisR2R_PunchView::CycleStop()
{
	m_bCycleStop = TRUE; pDoc->SetStatus(_T("General"), _T("bCycleStop"), m_bCycleStop);
}

BOOL CGvisR2R_PunchView::ChkLotCutPos()
{
	if (pDoc->WorkingInfo.LastJob.bLotSep && pDoc->m_bDoneChgLot)
	{
		double dFdTotLen = GetMkFdLen();
		double dLotCutPos = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen)*1000.0;
		if (dFdTotLen >= dLotCutPos)
		{
			pDoc->WorkingInfo.LastJob.bLotSep = FALSE;
			MpeWrite(Plc.DlgInfo.UseLotLength, 0);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)-20141031
			if (pDoc->m_pReelMap)
				pDoc->m_pReelMap->m_bUseLotSep = FALSE;

			::WritePrivateProfileString(_T("Last Job"), _T("Use Lot seperate"), _T("0"), PATH_WORKING_INFO);

			if (m_pDlgMenu01)
				m_pDlgMenu01->UpdateData();

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::ChkStShotNum()
{
	CString sMsg;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_ListBuf[0].nTot == 0)
	{
		if (m_nShareUpS > 0 && !(m_nShareUpS % 2))
		{
			sMsg.Format(_T("AOI 상면의 시리얼이 짝수로 시작하였습니다.\r\n- 시리얼 번호: %d"), m_nShareUpS);
			MsgBox(sMsg);
			return FALSE;
		}
	}

	if (bDualTest)
	{
		if (pDoc->m_ListBuf[1].nTot == 0)
		{
			if (m_nShareDnS > 0 && !(m_nShareDnS % 2))
			{
				sMsg.Format(_T("AOI 하면의 시리얼이 짝수로 시작하였습니다.\r\n- 시리얼 번호: %d"), m_nShareDnS);
				MsgBox(sMsg);
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::ChkContShotNum()
{
	CString sMsg;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!m_pDlgFrameHigh)
		return FALSE;

	if (pDoc->m_ListBuf[0].nTot == 0)
	{
		if (m_nShareUpS > 0 && m_pDlgFrameHigh->m_nMkLastShot + 1 != m_nShareUpS)
		{
			sMsg.Format(_T("AOI 상면의 시작Shot(%d)이 마지막Shot(%d)과 불연속입니다.\r\n계속 진행하시겠습니까?"), m_nShareUpS, m_pDlgFrameHigh->m_nMkLastShot);
			if (IDNO == MsgBox(sMsg, 0, MB_YESNO))
				return FALSE;
		}
	}

	return TRUE;
}

void CGvisR2R_PunchView::SetFixPcs(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->SetFixPcs(nSerial);
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->SetFixPcs(nSerial);
	}
}

BOOL CGvisR2R_PunchView::DoElecChk(CString &sRst)
{
	BOOL bDone = FALSE;
#ifdef USE_FLUCK
	double dVal;
	if (!pDoc->WorkingInfo.Fluck.bUse)
		return TRUE;

	if (!pDoc->WorkingInfo.Probing[0].bUse)
		return TRUE;

	if (!m_pFluck || !m_pVoiceCoil[0] || !m_pVoiceCoil[1])
		return FALSE;

	switch (m_nStepElecChk)
	{
	case 0: // Move to left point for measuring.
		pDoc->m_sElecChk = _T("");
		m_dElecChkVal = 0.0;
		pDoc->WorkingInfo.Probing[0].nRepeatTest = 0;
		MoveMeasPos(0);
		m_nStepElecChk++;
		break;
	case 1: // Check move done
		if (m_pMotion->IsMotionDone(MS_X0))
			m_nStepElecChk++;
		break;
	case 2: // Move to right point for measuring.
		MoveMeasPos(1);
		m_nStepElecChk++;
		break;
	case 3: // Check move done
		if (m_pMotion->IsMotionDone(MS_X1))
			m_nStepElecChk++;
		break;
	case 4: // Delay
		m_nStepElecChk++;
		break;
	case 5: // left & right Prob down
		if (m_pVoiceCoil[0])
			m_pVoiceCoil[0]->SetProbing(0);
		m_nStepElecChk++;
		break;
	case 6: // Delay
		Sleep(100);
		m_nStepElecChk++;
		break;
	case 7: // left & right Prob down
		if (m_pVoiceCoil[1])
			m_pVoiceCoil[1]->SetProbing(1);
		m_nStepElecChk++;
		break;
	case 8: // Delay
		Sleep(30);
		m_nStepElecChk++;
		break;
	case 9: // Delay
		m_pFluck->Clear();
		Sleep(100);
		m_nStepElecChk++;
		break;
	case 10: // Delay
		m_nStepElecChk++;
		break;
	case 11: // Measure registance
		m_pFluck->Trig();
		m_nStepElecChk++;
		break;
	case 12: // Delay
		Sleep(100);
		if (m_pFluck->Get(dVal))
		{
			m_dElecChkVal += dVal;
			m_nStepElecChk++;
		}
		m_nStepElecChk++;
		break;
	case 13: // Measure registance
		m_pFluck->Trig();
		m_nStepElecChk++;
		break;
	case 14: // Delay

		Sleep(100);
		if (m_pFluck->Get(dVal))
		{
			m_dElecChkVal += dVal;
			m_nStepElecChk++;
		}
		break;
	case 15: // Measure registance
		m_pFluck->Trig();
		m_nStepElecChk++;
		break;
	case 16: // Delay
		Sleep(100);
		if (m_pFluck->Get(dVal))
		{
			m_dElecChkVal += dVal;
			m_nStepElecChk++;
		}
		break;
	case 17: // Result OK , NG
		dVal = m_dElecChkVal / 3.0;

		if (dVal >= FLUCK_OVER)
		{
			sRst.Format(_T("Open"), dVal);
			if (pDoc->WorkingInfo.Probing[0].nRepeatTest < _tstoi(pDoc->WorkingInfo.Probing[0].sRepeatTestNum))
			{
				pDoc->WorkingInfo.Probing[0].nRepeatTest++;
				Sleep(100);
				m_nStepElecChk = 4;
				break;
			}
		}
		else if (dVal == FLUCK_ERROR)
		{
			sRst.Format(_T("Error"), dVal);
			if (pDoc->WorkingInfo.Probing[0].nRepeatTest < _tstoi(pDoc->WorkingInfo.Probing[0].sRepeatTestNum))
			{
				pDoc->WorkingInfo.Probing[0].nRepeatTest++;
				Sleep(100);
				m_nStepElecChk = 4;
				break;
			}
		}
		else
		{
			sRst.Format(_T("%8f"), dVal);
		}

		pDoc->m_sElecChk = sRst;
		m_nStepElecChk++;
		break;
	case 18: // Write Data
		pDoc->WriteElecData(pDoc->m_sElecChk);
		m_nStepElecChk++;
		break;
	case 19: // left & right Prob up
		if (m_pVoiceCoil[1])
			m_pVoiceCoil[1]->SearchHomeSmac1();
		m_nStepElecChk++;
		break;
	case 20: // Delay
		m_nStepElecChk++;
		break;
	case 21: // left & right Prob up
		if (m_pVoiceCoil[0])
			m_pVoiceCoil[0]->SearchHomeSmac0();
		m_nStepElecChk++;
		break;
	case 22: // Delay
		m_nStepElecChk++;
		break;
	case 23: // Move to left init pos
		if (m_pVoiceCoil[0] && m_pVoiceCoil[1])
		{
			if (m_pVoiceCoil[0]->IsDoneSearchHomeSmac0() && m_pVoiceCoil[1]->IsDoneSearchHomeSmac1())
			{
				if (!IsInitPos0())
					MoveInitPos0();
				if (!IsInitPos1())
					MoveInitPos1();
				m_nStepElecChk++;
			}
		}
		else
			m_nStepElecChk++;
		break;
	case 24: // Move to right init pos
		if (IsMoveDone0() && IsMoveDone1())
		{
			m_nStepElecChk++;
		}
		break;
	case 25: // Move to left init pos
		if (m_pVoiceCoil[1])
			m_pVoiceCoil[1]->MoveSmacShiftPos1();
		m_nStepElecChk++;
		break;
	case 26: // Delay
		m_nStepElecChk++;
		break;
	case 27: // Move to left init pos
		if (m_pVoiceCoil[0])
			m_pVoiceCoil[0]->MoveSmacShiftPos0();
		m_nStepElecChk++;
		break;
	case 28: // Delay
		m_nStepElecChk++;
		break;
	case 29: // Move to left init pos
		if (m_pVoiceCoil[0] && m_pVoiceCoil[1])
		{
			if (m_pVoiceCoil[0]->IsDoneMoveSmacShiftPos0() && m_pVoiceCoil[1]->IsDoneMoveSmacShiftPos1())
			{
				m_nStepElecChk++;
			}
		}
		else
			m_nStepElecChk++;
		break;
	case 30: // Delay
		m_nStepElecChk++;
		break;
	case 31: // Done Elec Check.
		bDone = TRUE;
		break;
	}

	sRst = pDoc->m_sElecChk;
#else
	bDone = TRUE;
#endif
	return bDone;
}

BOOL CGvisR2R_PunchView::MoveMeasPos(int nId)
{
	if (!m_pMotion)
		return FALSE;

	if (!m_pLight)
		return FALSE;

	if (nId == 0)
	{
		if (m_pDlgMenu02)
			m_pDlgMenu02->SetLight();

		double dMkOffsetX, dMkOffsetY;
		if (pDoc->WorkingInfo.Vision[0].sMkOffsetX.IsEmpty())
			dMkOffsetX = 0.0;
		else
			dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetX);

		if (pDoc->WorkingInfo.Vision[0].sMkOffsetY.IsEmpty())
			dMkOffsetY = 0.0;
		else
			dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[0].sMkOffsetY);


		double pPos[2];
		if (m_bFailAlign[0][0] || m_bFailAlign[0][1])
		{
			pPos[0] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosX) + dMkOffsetX;
			pPos[1] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosY) + dMkOffsetY;
		}
		else
		{
			pPos[0] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosX) + dMkOffsetX - m_pDlgMenu02->m_dMkFdOffsetX[0][0];
			pPos[1] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosY) + dMkOffsetY - m_pDlgMenu02->m_dMkFdOffsetY[0][0];

		}

		if (pPos[1] > 0.0 && pPos[0] > 0.0)
		{
			double dCurrX = m_dEnc[AXIS_X0];
			double dCurrY = m_dEnc[AXIS_Y0];

			if (ChkCollision(AXIS_X0, pPos[0]))
			{
				CfPoint ptPnt;
				ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
				ptPnt.y = 0.0; // m_dEnc[AXIS_Y1];
				Move1(ptPnt);
			}

			double fLen, fVel, fAcc, fJerk;
			fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
			if (fLen > 0.001)
			{
				if (pView->IsClampOff())
					return FALSE;

				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
				if (!m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
				{
					if (!m_pMotion->Move(MS_X0Y0, pPos, fVel, fAcc, fAcc))
					{
						pView->SetAlarmToPlc(UNIT_PUNCH);
						pView->ClrDispMsg();
						AfxMessageBox(_T("Move XY Error..."));
					}
				}
			}

			return TRUE;
		}
	}
	else if (nId == 1)
	{
		if (m_pDlgMenu02)
			m_pDlgMenu02->SetLight2();

		double dMkOffsetX, dMkOffsetY;
		if (pDoc->WorkingInfo.Vision[1].sMkOffsetX.IsEmpty())
			dMkOffsetX = 0.0;
		else
			dMkOffsetX = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetX);

		if (pDoc->WorkingInfo.Vision[1].sMkOffsetY.IsEmpty())
			dMkOffsetY = 0.0;
		else
			dMkOffsetY = _tstof(pDoc->WorkingInfo.Vision[1].sMkOffsetY);


		double pPos[2];
		if (m_bFailAlign[1][0] || m_bFailAlign[1][1])
		{
			pPos[0] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosX) + dMkOffsetX;
			pPos[1] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosY) + dMkOffsetY;
		}
		else
		{
			pPos[0] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosX) + dMkOffsetX - m_pDlgMenu02->m_dMkFdOffsetX[1][0];
			pPos[1] = _tstof(pDoc->WorkingInfo.Probing[nId].sMeasurePosY) + dMkOffsetY - m_pDlgMenu02->m_dMkFdOffsetY[1][0];
		}

		if (pPos[1] > 0.0 && pPos[0] > 0.0)
		{
			double dCurrX = m_dEnc[AXIS_X1];
			double dCurrY = m_dEnc[AXIS_Y1];

			if (ChkCollision(AXIS_X1, pPos[0]))
			{
				CfPoint ptPnt;
				ptPnt.x = 0.0;
				ptPnt.y = 0.0;//m_dEnc[AXIS_Y0];
				Move0(ptPnt);
			}

			double fLen, fVel, fAcc, fJerk;
			fLen = sqrt(((pPos[0] - dCurrX) * (pPos[0] - dCurrX)) + ((pPos[1] - dCurrY) * (pPos[1] - dCurrY)));
			if (fLen > 0.001)
			{
				m_pMotion->GetSpeedProfile(TRAPEZOIDAL, AXIS_X0, fLen, fVel, fAcc, fJerk);
				if (!m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
				{
					if (!m_pMotion->Move(MS_X1Y1, pPos, fVel, fAcc, fAcc))
					{
						pView->SetAlarmToPlc(UNIT_PUNCH);
						pView->ClrDispMsg();
						AfxMessageBox(_T("Move XY Error..."));
					}
				}
			}

			return TRUE;
		}
	}

	return FALSE;
}

void CGvisR2R_PunchView::SetReject()
{
	CfPoint ptPnt;

	if (m_bDoMk[0] && m_bMkSt[0])
	{
		if (!m_bTHREAD_MK[2])
		{
			m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
			m_nStepMk[2] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[2]"), m_nStepMk[2]);
			m_nMkPcs[2] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[2]"), m_nMkPcs[2]);
			m_bTHREAD_MK[2] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[2]"), m_bTHREAD_MK[2]);
		}
	}
	else
	{
		m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
	}

	if (m_bDoMk[1] && m_bMkSt[1])
	{
		if (!m_bTHREAD_MK[3])
		{
			m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
			m_nStepMk[3] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[3]"), m_nStepMk[3]);
			m_nMkPcs[3] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[3]"), m_nMkPcs[3]);
			m_bTHREAD_MK[3] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MK[3]"), m_bTHREAD_MK[3]);
		}
	}
	else
	{
		m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
	}
}

void CGvisR2R_PunchView::DoInterlock()
{
	if (m_dEnc[AXIS_Y0] < 20.0 && m_dEnc[AXIS_Y1] < 20.0)
	{
		if (m_bStopFeeding)
		{
			m_bStopFeeding = FALSE;
			MpeWrite(Plc.DlgMenu01.ProhibitFeedingPunch, 0); // 마킹부Feeding금지
		}
	}
	else
	{
		if (!m_bStopFeeding)
		{
			m_bStopFeeding = TRUE;
			MpeWrite(Plc.DlgMenu01.ProhibitFeedingPunch, 1); // 마킹부Feeding금지
		}
	}
}

BOOL CGvisR2R_PunchView::ChkLightErr()
{
	int nSerial, nErrCode;
	BOOL bError = FALSE;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	nSerial = m_nBufUpSerial[0]; // Cam0

	if (pView->m_bSerialDecrese)
	{
		if (nSerial > 0 && nSerial >= m_nLotEndSerial)
		{
			if ((nErrCode = GetErrCode0(nSerial)) != 1)
			{
				if (nErrCode == -1) // -1(Align Error, 노광불량)
					bError = TRUE;
			}
		}

		nSerial = m_nBufUpSerial[1]; // Cam1

		if (nSerial > 0 && nSerial >= m_nLotEndSerial)
		{
			if ((nErrCode = GetErrCode1(nSerial)) != 1)
			{
				if (nErrCode == -1) // -1(Align Error, 노광불량)
					bError = TRUE;
			}
		}
	}
	else
	{
		if (nSerial > 0 && nSerial <= m_nLotEndSerial)
		{
			if ((nErrCode = GetErrCode0(nSerial)) != 1)
			{
				if (nErrCode == -1) // -1(Align Error, 노광불량)
					bError = TRUE;
			}
		}

		nSerial = m_nBufUpSerial[1]; // Cam1

		if (nSerial > 0 && nSerial <= m_nLotEndSerial)
		{
			if ((nErrCode = GetErrCode1(nSerial)) != 1)
			{
				if (nErrCode == -1) // -1(Align Error, 노광불량)
					bError = TRUE;
			}
		}
	}

	if (bError)
	{
		Buzzer(TRUE, 0);
		TowerLamp(RGB_RED, TRUE);
		Stop();
		DispMain(_T("정 지"), RGB_RED);
	}

	return bError;
}

void CGvisR2R_PunchView::CntMk()
{
	if (m_nPrevTotMk[0] != m_nTotMk[0])
	{
		m_nPrevTotMk[0] = m_nTotMk[0]; pDoc->SetStatusInt(_T("General"), _T("nPrevTotMk[0]"), m_nPrevTotMk[0]);
		MpeWrite(Plc.DlgMenu01.MarkingTotalLeft, (long)m_nTotMk[0]);	// 마킹부 (좌) 총 마킹수 
	}
	if (m_nPrevCurMk[0] != m_nMkPcs[0])//m_nCurMk[0])
	{
		m_nPrevCurMk[0] = m_nMkPcs[0]; pDoc->SetStatusInt(_T("General"), _T("nPrevCurMk[0]"), m_nPrevCurMk[0]);//m_nCurMk[0];
		MpeWrite(Plc.DlgMenu01.MarkingCurrLeft, (long)m_nMkPcs[0]);	// 마킹부 (좌) 현재 마킹한 수
	}

	if (m_nPrevTotMk[1] != m_nTotMk[1])
	{
		m_nPrevTotMk[1] = m_nTotMk[1]; pDoc->SetStatusInt(_T("General"), _T("nPrevTotMk[1]"), m_nPrevTotMk[1]);
		MpeWrite(Plc.DlgMenu01.MarkingTotalRight, (long)m_nTotMk[1]);	// 마킹부 (우) 총 마킹수 
	}
	if (m_nPrevCurMk[1] != m_nMkPcs[1])//m_nCurMk[1])
	{
		m_nPrevCurMk[1] = m_nMkPcs[1]; pDoc->SetStatusInt(_T("General"), _T("nPrevCurMk[1]"), m_nPrevCurMk[1]);//m_nCurMk[1];
		MpeWrite(Plc.DlgMenu01.MarkingCurrRight, (long)m_nMkPcs[1]);	// 마킹부 (우) 현재 마킹한 수
	}
}

BOOL CGvisR2R_PunchView::IsOnMarking0()
{
	if (m_nMkPcs[0] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)	// 마킹완료Check
		return TRUE;

	return FALSE;
}

BOOL CGvisR2R_PunchView::IsOnMarking1()
{
	if (m_nMkPcs[1] < pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs)	// 마킹완료Check
		return TRUE;

	return FALSE;
}

void CGvisR2R_PunchView::SetDualTest(BOOL bOn)
{
	//MpeWrite(pView->Plc.DlgInfo.SingleMode, bOn ? 0 : 1);		// 단면 검사 On
	if (m_pDlgFrameHigh)
		m_pDlgFrameHigh->SetDualTest(bOn);
	if (m_pDlgMenu01)
		m_pDlgMenu01->SetDualTest(bOn);
	if (m_pDlgMenu03)
		m_pDlgMenu03->SetDualTest(bOn);
	if (m_pDlgMenu06)
		m_pDlgMenu06->SetDualTest(bOn);
}

void CGvisR2R_PunchView::SetTwoMetal(BOOL bSel, BOOL bOn)
{
	if (bSel)
	{
		if (bOn)
		{
			pDoc->WorkingInfo.LastJob.bTwoMetal = TRUE;
			MpeWrite(Plc.DlgInfo.TwoMetal, 1);
			MpeWrite(Plc.DlgInfo.OneMetal, 0);
			::WritePrivateProfileString(_T("Last Job"), _T("Two Metal On"), _T("1"), PATH_WORKING_INFO);// IDC_CHK_TWO_METAL - Uncoiler\r역방향 ON : TRUE	
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bTwoMetal = FALSE;
			MpeWrite(Plc.DlgInfo.TwoMetal, 0);
			::WritePrivateProfileString(_T("Last Job"), _T("Two Metal On"), _T("0"), PATH_WORKING_INFO);// IDC_CHK_TWO_METAL - Uncoiler\r정방향 ON : TRUE	
		}

//#ifdef USE_ENGRAVE
//		if (pView && pView->m_pEngrave)
//			pView->m_pEngrave->SetUncoilerCcw();	//_stSigInx::_UncoilerCcw
//#endif
	}
	else
	{
		if (bOn)
		{
			pDoc->WorkingInfo.LastJob.bOneMetal = TRUE;
			MpeWrite(Plc.DlgInfo.OneMetal, 1);
			MpeWrite(Plc.DlgInfo.TwoMetal, 0);
			::WritePrivateProfileString(_T("Last Job"), _T("One Metal On"), _T("1"), PATH_WORKING_INFO);// IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bOneMetal = FALSE;
			MpeWrite(Plc.DlgInfo.OneMetal, 0);
			::WritePrivateProfileString(_T("Last Job"), _T("One Metal On"), _T("0"), PATH_WORKING_INFO);// IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
		}

//#ifdef USE_ENGRAVE
//		if (pView && pView->m_pEngrave)
//			pView->m_pEngrave->SetRecoilerCcw();	//_stSigInx::_RecoilerCcw
//#endif
	}
}

void CGvisR2R_PunchView::SetFeedDir(int nUnit, BOOL bCcw)
{
	if (nUnit)
	{
		if (bCcw)
		{
			pDoc->WorkingInfo.LastJob.bFeedCcwUncoiler = TRUE;
			MpeWrite(Plc.DlgInfo.FeedCcwUncoiler, 1);
			::WritePrivateProfileString(_T("Last Job"), _T("FeedUncoilerCcw"), _T("1"), PATH_WORKING_INFO);// IDC_CHK_UNCOILER_CCW - Uncoiler\r역방향 ON : TRUE	
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bFeedCcwUncoiler = FALSE;
			MpeWrite(Plc.DlgInfo.FeedCcwUncoiler, 0);
			::WritePrivateProfileString(_T("Last Job"), _T("FeedUncoilerCcw"), _T("0"), PATH_WORKING_INFO);// IDC_CHK_UNCOILER_CCW - Uncoiler\r정방향 ON : TRUE	
		}

		//#ifdef USE_ENGRAVE
		//		if (pView && pView->m_pEngrave)
		//			pView->m_pEngrave->SetUncoilerCcw();	//_stSigInx::_UncoilerCcw
		//#endif
	}
	else
	{
		if (bCcw)
		{
			pDoc->WorkingInfo.LastJob.bFeedCcwRecoiler = TRUE;
			MpeWrite(Plc.DlgInfo.FeedCcwRecoiler, 1);
			::WritePrivateProfileString(_T("Last Job"), _T("FeedRecoilerCcw"), _T("1"), PATH_WORKING_INFO);// IDC_CHK_RECOILER_CCW - Recoiler\r정방향 CW : FALSE
		}
		else
		{
			pDoc->WorkingInfo.LastJob.bFeedCcwRecoiler = FALSE;
			MpeWrite(Plc.DlgInfo.FeedCcwRecoiler, 0);
			::WritePrivateProfileString(_T("Last Job"), _T("FeedRecoilerCcw"), _T("0"), PATH_WORKING_INFO);// IDC_CHK_RECOILER_CCW - Recoiler\r정방향 CW : FALSE
		}

		//#ifdef USE_ENGRAVE
		//		if (pView && pView->m_pEngrave)
		//			pView->m_pEngrave->SetRecoilerCcw();	//_stSigInx::_RecoilerCcw
		//#endif
	}
}

void CGvisR2R_PunchView::RestoreReelmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->RestoreReelmap();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->RestoreReelmap();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->RestoreReelmap();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->RestoreReelmap();
	}
}

void CGvisR2R_PunchView::AdjPinPos()
{
	if (m_pDlgMenu02)
	{
		if (m_pDlgMenu02->m_dMkFdOffsetY[0][0] > -2.0 && m_pDlgMenu02->m_dMkFdOffsetY[1][0] > -2.0 &&
			m_pDlgMenu02->m_dMkFdOffsetY[0][0] < 2.0 && m_pDlgMenu02->m_dMkFdOffsetY[1][0] < 2.0)
		{
			double dOffsetY0 = -1.0 * m_pDlgMenu02->m_dMkFdOffsetY[0][0];
			dOffsetY0 *= pDoc->m_dShiftAdjustRatio;
			double dOffsetY1 = -1.0 * m_pDlgMenu02->m_dMkFdOffsetY[1][0];
			dOffsetY1 *= pDoc->m_dShiftAdjustRatio;

			CfPoint ptPnt[2];
			ptPnt[0].x = _tstof(pDoc->WorkingInfo.Motion.sPinPosX[0]);
			ptPnt[0].y = _tstof(pDoc->WorkingInfo.Motion.sPinPosY[0]) + dOffsetY0;
			ptPnt[1].x = _tstof(pDoc->WorkingInfo.Motion.sPinPosX[1]);
			ptPnt[1].y = _tstof(pDoc->WorkingInfo.Motion.sPinPosY[1]) + dOffsetY1;

			m_pDlgMenu02->SetPinPos(0, ptPnt[0]);
			m_pDlgMenu02->SetPinPos(1, ptPnt[1]);

			CString sData, sPath = PATH_WORKING_INFO;
			pDoc->WorkingInfo.Fluck.dMeasPosY[0] = _tstof(pDoc->WorkingInfo.Probing[0].sMeasurePosY) + dOffsetY0;
			sData.Format(_T("%.2f"), pDoc->WorkingInfo.Fluck.dMeasPosY[0]);
			pDoc->WorkingInfo.Probing[0].sMeasurePosY = sData;
			::WritePrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSY"), sData, sPath);

			pDoc->WorkingInfo.Fluck.dMeasPosY[1] = _tstof(pDoc->WorkingInfo.Probing[1].sMeasurePosY) + dOffsetY1;
			sData.Format(_T("%.2f"), pDoc->WorkingInfo.Fluck.dMeasPosY[1]);
			pDoc->WorkingInfo.Probing[1].sMeasurePosY = sData;
			::WritePrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSY"), sData, sPath);
		}
	}
}


// Engrave
void CGvisR2R_PunchView::MoveEngrave(double dOffset)
{
	long lData = (long)(dOffset * 1000.0);
	MpeWrite(Plc.DlgFrameHigh.FeedOffsetEngrave, lData);		// 각인부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)
	MpeWrite(Plc.DlgMenu01.FeedCwEngrave, 1);		// 각인부 피딩 CW ON (PLC가 피딩완료 후 OFF)
}

BOOL CGvisR2R_PunchView::IsEngraveFd()
{
	BOOL bChk = FALSE;

	if (m_nShareDnCnt > 0)
	{
		if (!(m_nShareDnCnt % 2))
		{
			bChk = MpeRead(_T("MB400286")); // PLC 각인기 피딩 정방향 Run
			//if ((pDoc->m_pMpeSignal[8] & (0x01 << 6))/* || (pDoc->m_pMpeSignal[8] & (0x01 << 7))*/)	// 각인부 피딩 CW ON (PLC가 피딩완료 후 OFF)
			if (bChk)	// 각인부 피딩 CW ON (PLC가 피딩완료 후 OFF)
			{
				pDoc->LogAuto(_T("PLC: 각인부 피딩 CW ON (PLC가 피딩완료 후 OFF)"));
				return TRUE;
			}
			return FALSE;
		}
	}
	else
	{
		if (m_nShareUpCnt > 0)
		{
			if (!(m_nShareUpCnt % 2))
			{
				bChk = MpeRead(_T("MB400286")); // PLC 각인기 피딩 정방향 Run
				if (bChk)	// 각인부 피딩 CW ON (PLC가 피딩완료 후 OFF)
				{
					pDoc->LogAuto(_T("PLC: 각인부 피딩 CW ON (PLC가 피딩완료 후 OFF)"));
					return TRUE;
				}
				return FALSE;
			}
		}
	}

	return TRUE;
}


double CGvisR2R_PunchView::GetEngraveFdLen()
{
	int nLast = pDoc->GetLastShotEngrave();

	double dLen = (double)nLast * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	return dLen;
}

double CGvisR2R_PunchView::GetAoiInitDist()
{
	double dInitD1 = _tstof(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist) - _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	return dInitD1;
}

double CGvisR2R_PunchView::GetAoiRemain()
{
	if (!m_pMpe || !pDoc->m_pMpeData)
		return 0.0;
	double dCurPosEngraveFd = (double)pDoc->m_pMpeData[1][14];	// ML44052	,	각인부 Feeding 엔코더 값(단위 mm)
	double dRemain = _tstof(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist) - dCurPosEngraveFd;
	return dRemain;
}


LRESULT CGvisR2R_PunchView::wmClientReceived(WPARAM wParam, LPARAM lParam)
{
	if (!m_pEngrave)
		return (LRESULT)0;

	int nAcceptId = (int)wParam;
	SOCKET_DATA sSockData;
	SOCKET_DATA *pSocketData = (SOCKET_DATA*)lParam;
	SOCKET_DATA rSockData = *pSocketData;
	//SOCKET_DATA rSockData = m_pEngrave->GetSocketData();
	int nCmdCode = rSockData.nCmdCode;
	int nMsgId = rSockData.nMsgID;

	switch (nCmdCode)
	{
	case _GetSig:
		break;
	case _GetData:
		break;
	case _SetSig:
		//if (m_pEngrave && m_pEngrave->IsConnected())
		//	m_pEngrave->GetSysSignal(rSockData);

		pView->m_bSetSig = TRUE;
		break;
	case _SetData:
		if (m_pEngrave && m_pEngrave->IsConnected())
			m_pEngrave->GetSysData(rSockData);

		pView->m_bSetData = TRUE;
		break;
	default:
		break;
	}


	return (LRESULT)1;
}

LRESULT CGvisR2R_PunchView::wmClientReceivedSr(WPARAM wParam, LPARAM lParam)
{
	int nCmd = (int)wParam;
	CString* sReceived = (CString*)lParam;

	switch (nCmd)
	{
	case SrTriggerInputOn:
		Get2dCode(m_sGet2dCodeLot, m_nGet2dCodeSerial);
		break;
	default:
		break;
	}

	return (LRESULT)1;
}
void CGvisR2R_PunchView::SetEngraveFdPitch(double dPitch)
{
	pDoc->SetEngraveFdPitch(dPitch);
}

BOOL CGvisR2R_PunchView::IsConnectedEng()
{
	if (m_bDestroyedView)
		return FALSE;

#ifdef USE_ENGRAVE
	if (m_pEngrave)
	{
		if(m_pEngrave->IsConnected())
		{
			if(!m_bContEngraveF)
			{
				m_bContEngraveF = TRUE;
				DWORD dwStartTick = GetTickCount();
			}
			return TRUE;
		}
		else
		{
			if (m_bContEngraveF)
			{
				m_bContEngraveF = FALSE;
			}
			return FALSE;
		}
	}
#endif
	return FALSE;
}


BOOL CGvisR2R_PunchView::IsPinPos0()
{
	if (!m_pMotion)
		return FALSE;

	double pPos[2];
	pPos[0] = m_pMotion->m_dPinPosX[0];
	pPos[1] = m_pMotion->m_dPinPosY[0];

	double dCurrX = m_dEnc[AXIS_X0];
	double dCurrY = m_dEnc[AXIS_Y0];

	if (dCurrX < pPos[0] - 4.0 || dCurrX > pPos[0] + 4.0)
		return FALSE;
	if (dCurrY < pPos[1] - 4.0 || dCurrY > pPos[1] + 4.0)
		return FALSE;

	return TRUE;
}

BOOL CGvisR2R_PunchView::IsPinPos1()
{
	if (!m_pMotion)
		return FALSE;

	double pPos[2];
	pPos[0] = m_pMotion->m_dPinPosX[1];
	pPos[1] = m_pMotion->m_dPinPosY[1];

	double dCurrX = m_dEnc[AXIS_X1];
	double dCurrY = m_dEnc[AXIS_Y1];

	if (dCurrX < pPos[0] - 4.0 || dCurrX > pPos[0] + 4.0)
		return FALSE;
	if (dCurrY < pPos[1] - 4.0 || dCurrY > pPos[1] + 4.0)
		return FALSE;

	return TRUE;
}

void CGvisR2R_PunchView::RunShift2Mk()
{
	Shift2Mk();			// PCR 이동(Buffer->Marked) // 기록(WorkingInfo.LastJob.sSerial)
}


// Call from Thread Function
void CGvisR2R_PunchView::UpdateYield(int nSerial)
{
	if (nSerial < 1)
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	m_nSnTHREAD_UPDATAE_YIELD = nSerial; pDoc->SetStatusInt(_T("Thread"), _T("nSnTHREAD_UPDATAE_YIELD"), m_nSnTHREAD_UPDATAE_YIELD);
	m_bTHREAD_UPDATE_YIELD_UP = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_UP"), m_bTHREAD_UPDATE_YIELD_UP);
	if (bDualTest)
	{
		m_bTHREAD_UPDATE_YIELD_DN = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_DN"), m_bTHREAD_UPDATE_YIELD_DN);
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
		m_bTHREAD_UPDATE_YIELD_INNER_UP = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_INNER_UP"), m_bTHREAD_UPDATE_YIELD_INNER_UP);
		if (bDualTest)
		{
			m_bTHREAD_UPDATE_YIELD_INNER_DN = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_INNER_DN"), m_bTHREAD_UPDATE_YIELD_INNER_DN);
		}
	}
}

void CGvisR2R_PunchView::UpdateYieldUp(int nSerial)
{
	pDoc->UpdateYieldUp(nSerial);
}

void CGvisR2R_PunchView::UpdateYieldDn(int nSerial)
{
	pDoc->UpdateYieldDn(nSerial);
}

void CGvisR2R_PunchView::UpdateYieldAllUp(int nSerial)
{
	pDoc->UpdateYieldAllUp(nSerial);
}

void CGvisR2R_PunchView::UpdateYieldAllDn(int nSerial)
{
	pDoc->UpdateYieldAllDn(nSerial);
}

void CGvisR2R_PunchView::UpdateYieldInnerUp(int nSerial)
{
	pDoc->UpdateYieldInnerUp(nSerial);
}

void CGvisR2R_PunchView::UpdateYieldInnerDn(int nSerial)
{
	pDoc->UpdateYieldInnerDn(nSerial);
}

void CGvisR2R_PunchView::UpdateYieldInnerAllUp(int nSerial)
{
	pDoc->UpdateYieldInnerAllUp(nSerial);
}

void CGvisR2R_PunchView::UpdateYieldInnerAllDn(int nSerial)
{
	pDoc->UpdateYieldInnerAllDn(nSerial);
}

void CGvisR2R_PunchView::UpdateYieldIts(int nSerial)
{
	pDoc->UpdateYieldIts(nSerial);
}

void CGvisR2R_PunchView::UpdateYield()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial = m_nBufUpSerial[0];

	if (nSerial < 1)
	{
		Stop();
		MsgBox(_T("Error-UpdateYield() : m_nBufUpSerial[0] < 1"));
		return;
	}
	
	if (m_bLastProc && m_nBufUpSerial[0] == m_nLotEndSerial)
	{
		m_nSerialTHREAD_UPDATAE_YIELD[0] = nSerial;
		m_bTHREAD_UPDATAE_YIELD[0] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATAE_YIELD[0]"), m_bTHREAD_UPDATAE_YIELD[0]); // Cam[0]
		Sleep(30);
	}
	else
	{
		m_nSerialTHREAD_UPDATAE_YIELD[0] = nSerial;
		m_bTHREAD_UPDATAE_YIELD[0] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATAE_YIELD[0]"), m_bTHREAD_UPDATAE_YIELD[0]); // Cam[0]
		Sleep(30);

		if (m_nBufUpSerial[1] > 0)
		{
			m_nSerialTHREAD_UPDATAE_YIELD[1] = m_nBufUpSerial[1];
			m_bTHREAD_UPDATAE_YIELD[1] = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATAE_YIELD[1]"), m_bTHREAD_UPDATAE_YIELD[1]); // Cam[1]
			Sleep(30);
		}
		else if(m_nBufUpSerial[1] < 0)
		{
			return;
		}
	}
}


void CGvisR2R_PunchView::DoShift2Mk()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nSerial;

	if (m_bLastProc && m_nBufUpSerial[0] == m_nLotEndSerial)
	{
		nSerial = m_nBufUpSerial[0];
		if (nSerial > 0)
		{
			m_bTHREAD_SHIFT2MK = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_SHIFT2MK"), m_bTHREAD_SHIFT2MK);
		}
	}
	else
	{
		nSerial = m_nBufUpSerial[0];
		if (!m_bCont)
		{
			if (nSerial > 0)
			{
				m_bTHREAD_SHIFT2MK = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_SHIFT2MK"), m_bTHREAD_SHIFT2MK);
			}
			else
				Stop();
		}
		else
		{
			if (nSerial > 0)
			{
				m_bTHREAD_SHIFT2MK = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_SHIFT2MK"), m_bTHREAD_SHIFT2MK);
			}
			else
				Stop();
		}
	}
}

void CGvisR2R_PunchView::DoAutoEng()
{
	if (!IsAuto() || (MODE_INNER != pDoc->GetTestMode() && !pDoc->WorkingInfo.System.bUseEngrave))
		return;

	// 각인부 마킹시작 신호를 확인
	DoAtuoGetEngStSignal();

	// 각인부 2D 코드 Reading신호를 확인
	DoAtuoGet2dReadStSignal();
}

void CGvisR2R_PunchView::DoAtuoGetEngStSignal()
{
	CString sVal;
	BOOL bMk = MpeRead(Plc.DlgMenu01.MarkingStartEngrave); // MB400303 : PLC 각인부 마킹 시작
	
	if (m_pDlgMenu02)
	{
		sVal.Format(_T("%d"), bMk ? 1 : 0);
		m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_MK_ST)->SetWindowText(sVal);
	}

	//if ((pDoc->m_pMpeSignal[10] & (0x01 << 3) || m_bEngStSw) && !pDoc->BtnStatus.EngAuto.MkStF)// 2D(GUI) 각인 동작 Start신호(PLC On->PC Off)
	if ((bMk || m_bEngStSw) && !pDoc->BtnStatus.EngAuto.MkStF)// 2D(GUI) 각인 동작 Start신호(PLC On->PC Off)
	{
		MpeWrite(Plc.DlgMenu01.MarkingStartEngrave, 0);			// 2D(GUI) 각인 동작 Start신호(PLC On->PC Off)
		pDoc->BtnStatus.EngAuto.MkStF = TRUE;
		m_bEngStSw = FALSE;

		pDoc->BtnStatus.EngAuto.IsMkSt = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt, TRUE);
		//CheckCurrentInfoSignal();
	}
	//else if (pDoc->BtnStatus.EngAuto.IsMkSt && pDoc->BtnStatus.EngAuto.MkStF)
	else if (!bMk && pDoc->BtnStatus.EngAuto.MkStF)
	{
		pDoc->BtnStatus.EngAuto.MkStF = FALSE;
		//pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt, FALSE);
		//CheckCurrentInfoSignal();
	}

	BOOL bFdDone = MpeRead(Plc.DlgMenu01.FeedingDoneEngrave); // MB400240 : 각인부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)

	if (m_pDlgMenu02)
	{
		sVal.Format(_T("%d"), bFdDone ? 1 : 0);
		m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_FD_DONE)->SetWindowText(sVal);
	}

	//if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[4] & (0x01 << 0) && !pDoc->BtnStatus.EngAuto.FdDoneF)	// 각인부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
	if (bFdDone && !pDoc->BtnStatus.EngAuto.FdDoneF)	// 각인부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
	{
		MpeWrite(Plc.DlgMenu01.FeedingDoneEngrave, 0);		// 각인부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)
		pDoc->BtnStatus.EngAuto.FdDoneF = TRUE;

		pDoc->BtnStatus.EngAuto.IsFdDone = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone, TRUE);
		//CheckCurrentInfoSignal();
	}
	//else if (pDoc->BtnStatus.EngAuto.IsFdDone && pDoc->BtnStatus.EngAuto.FdDoneF)
	else if (!bFdDone && pDoc->BtnStatus.EngAuto.FdDoneF)
	{
		pDoc->BtnStatus.EngAuto.FdDoneF = FALSE;
		//pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone, FALSE);
		//CheckCurrentInfoSignal();
	}

}

void CGvisR2R_PunchView::DoAtuoGet2dReadStSignal()
{
	CString sVal;
	BOOL bRead = MpeRead(Plc.DlgMenu01.Reading2dStartEngrave); // MB400306 : PLC 각인부 Reading 시작

	if (m_pDlgMenu02)
	{
		sVal.Format(_T("%d"), bRead ? 1 : 0);
		m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_RD_ST)->SetWindowText(sVal);
		//Sleep(100);
	}

	//if ((pDoc->m_pMpeSignal[10] & (0x01 << 6) || m_bEng2dStSw) && !pDoc->BtnStatus.EngAuto.Read2dStF)// 각인부 2D 리더 시작신호(PLC On->PC Off)
	if ((bRead || m_bEng2dStSw) && !pDoc->BtnStatus.EngAuto.Read2dStF)// 각인부 2D 리더 시작신호(PLC On->PC Off)
	{
		MpeWrite(Plc.DlgMenu01.Reading2dStartEngrave, 0);			// 각인부 2D 리더 시작신호(PLC On->PC Off)
		pDoc->BtnStatus.EngAuto.Read2dStF = TRUE;
		m_bEng2dStSw = FALSE;

		pDoc->BtnStatus.EngAuto.IsRead2dSt = FALSE;
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt, TRUE);
		//CheckCurrentInfoSignal();
	}
	//else if (pDoc->BtnStatus.EngAuto.IsRead2dSt && pDoc->BtnStatus.EngAuto.Read2dStF)
	else if (!bRead && pDoc->BtnStatus.EngAuto.Read2dStF)
	{
		pDoc->BtnStatus.EngAuto.Read2dStF = FALSE;
		//pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt, FALSE);
		//CheckCurrentInfoSignal();
	}
}


void CGvisR2R_PunchView::DoAutoSetFdOffsetEngrave()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	double dAveX, dAveY;
	CfPoint OfSt;

	if ((pDoc->BtnStatus.EngAuto.IsOnMking && !m_bEngTestF) || (pDoc->BtnStatus.EngAuto.IsOnRead2d && !m_bEngTestF)) // 각인부 검사중
	{
		m_bEngTestF = TRUE; pDoc->SetStatus(_T("General"), _T("bEngTestF"), m_bEngTestF);
		m_bEngTest = TRUE; pDoc->SetStatus(_T("General"), _T("bEngTest"), m_bEngTest);
	}
	else if ((!pDoc->BtnStatus.EngAuto.IsOnMking && m_bEngTestF) || (!pDoc->BtnStatus.EngAuto.IsOnRead2d && m_bEngTestF))
	{
		m_bEngTestF = FALSE; pDoc->SetStatus(_T("General"), _T("bEngTestF"), m_bEngTestF);
		m_bEngTest = FALSE; pDoc->SetStatus(_T("General"), _T("bEngTest"), m_bEngTest);
		m_bEngFdWriteF = FALSE; pDoc->SetStatus(_T("General"), _T("bEngFdWriteF"), m_bEngFdWriteF);
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneEngrave, 0);					// 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
		pDoc->LogAuto(_T("PC: 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
	}

	BOOL bChk = MpeRead(Plc.DlgFrameHigh.FeedOffsetWriteDoneEngrave); // PLC 각인기 피딩 Offset Write(GUI Off) _T("MB40024C")
	//if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[4] & (0x01 << 12) && !m_bEngFdWrite)		// 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
	if (bChk && !m_bEngFdWrite)		// 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
	{
		m_bEngFdWrite = TRUE; pDoc->SetStatus(_T("General"), _T("bEngFdWrite"), m_bEngFdWrite);
	}
	else if (!bChk && m_bEngFdWrite)
	{
		m_bEngFdWrite = FALSE; pDoc->SetStatus(_T("General"), _T("bEngFdWrite"), m_bEngFdWrite);
	}


	if (m_bEngFdWrite && !m_bEngFdWriteF)
	{
		m_bEngFdWriteF = TRUE; pDoc->SetStatus(_T("General"), _T("bEngFdWriteF"), m_bEngFdWriteF);

		GetEngOffset(OfSt);

		dAveX = OfSt.x;
		dAveY = OfSt.y;

		if (m_pDlgMenu02)
		{
			m_pDlgMenu02->m_dEngFdOffsetX = OfSt.x;
			m_pDlgMenu02->m_dEngFdOffsetY = OfSt.y;
		}

		pDoc->LogAuto(_T("PC: 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)"));
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetEngrave, (long)(dAveX*1000.0));	// 각인부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄, PC가 쓰고 PLC에서 지움)
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetWriteDoneEngrave, 0);					// 각인부 Feeding Offset Write 완료(PC가 확인하고 Reset시킴.)
		Sleep(10);
	}
	else if (!m_bEngFdWrite && m_bEngFdWriteF)
	{
		m_bEngFdWriteF = FALSE; pDoc->SetStatus(_T("General"), _T("bEngFdWriteF"), m_bEngFdWriteF);
		m_bEngTest = FALSE; pDoc->SetStatus(_T("General"), _T("bEngTest"), m_bEngTest);
	}

}

void CGvisR2R_PunchView::DoAutoMarkingEngrave()
{	
	// 각인부 마킹중 ON (PC가 ON, OFF)
	CString sVal;
	BOOL bChk = MpeRead(Plc.DlgMenu01.MarkingOnEngrave); // PLC 각인기 작업 중 _T("MB40023B")

	if ( pDoc->BtnStatus.EngAuto.IsOnMking && !bChk ) // 각인부 마킹중 ON (PC가 ON, OFF)
	{
		MpeWrite(Plc.DlgMenu01.MarkingOnEngrave, 1); // B400104 : 2D(GUI) 각인 동작Running신호(PC On->PC Off)
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, TRUE);
		pDoc->LogAuto(_T("PC: 2D(GUI) 각인 동작Running신호 ON (PC On->PC Off)"));
		if (m_pDlgMenu02)
		{
			sVal.Format(_T("%d"), 1);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_MK)->SetWindowText(sVal);
		}
	}
	else if ( !pDoc->BtnStatus.EngAuto.IsOnMking && bChk)
	{
		MpeWrite(Plc.DlgMenu01.MarkingOnEngrave, 0); // 2D(GUI) 각인 동작Running신호(PC On->PC Off)
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, FALSE);
		pDoc->LogAuto(_T("PC: 2D(GUI) 각인 동작Running신호 OFF (PC On->PC Off)"));
		if (m_pDlgMenu02)
		{
			sVal.Format(_T("%d"), 0);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_MK)->SetWindowText(sVal);
		}
	}

	// 각인부 마킹완료 ON (PC가 ON, OFF)
	bChk = MpeRead(Plc.DlgMenu01.MarkingDoneEngrave); // PLC 각인기 마킹 작업 완료 _T("MB400245")
	if (pDoc->BtnStatus.EngAuto.MkDone && !bChk) // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
	{
		pDoc->BtnStatus.EngAuto.MkDone = FALSE;
		MpeWrite(Plc.DlgMenu01.MarkingDoneEngrave, 1); // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
		pDoc->SetCurrentInfoSignal(_SigInx::_IsEngAutoSeqMkDone, TRUE);
		pDoc->LogAuto(_T("PC: 각인부 작업완료 ON (PC가 On, PLC가 확인 후 Off)"));

		long nSerialEng = (long)pDoc->GetCurrentInfoEngShotNum();
		long nSerialRead = (long)pDoc->GetCurrentInfoReadShotNum();
		MpeWrite(_T("ML41122"), nSerialRead); // 각인부 읽은 시리얼
		MpeWrite(_T("ML41120"), nSerialEng); // 각인부 각인된 시리얼
		MpeWrite(Plc.DlgMenu03.FeedingReadyEngrave, 1); // 각인부 피딩가능 신호.(PC가 On, PLC가 확인 후 Off)

		if (m_pDlgMenu02)
		{
			sVal.Format(_T("%d"), 1);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_MK_DONE)->SetWindowText(sVal);
			sVal.Format(_T("%d"), nSerialRead);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_READ_SN)->SetWindowText(sVal);
			sVal.Format(_T("%d"), nSerialEng);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_SN)->SetWindowText(sVal);
			sVal.Format(_T("%d"), 1);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_FD)->SetWindowText(sVal);
		}
	}
	else if (!pDoc->BtnStatus.EngAuto.MkDone && bChk) // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)
	{
		if (m_pDlgMenu02)
		{
			sVal.Format(_T("%d"), 0);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_MK_DONE)->SetWindowText(sVal);
			sVal.Format(_T("%d"), 0);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_FD)->SetWindowText(sVal);
		}
	}

	// 각인부 2D 리더 작업중 신호
	bChk = MpeRead(Plc.DlgMenu01.Reading2dOnEngrave); // GUI 각인기 리딩 중 _T("MB400034")
	if (pDoc->BtnStatus.EngAuto.IsOnRead2d && !bChk) // 각인부 2D 리더 작업중 신호(PC On->PC Off)
	{
		MpeWrite(Plc.DlgMenu01.Reading2dOnEngrave, 1); // 각인부 2D 리더 작업중 신호(PC On->PC Off)
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, TRUE);
		pDoc->LogAuto(_T("PC: 각인부 2D 리더 작업중 신호 ON (PC On->PC Off)"));
		if (m_pDlgMenu02)
		{
			sVal.Format(_T("%d"), 1);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_RD)->SetWindowText(sVal);
		}
	}
	else if (!pDoc->BtnStatus.EngAuto.IsOnRead2d && bChk)
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, FALSE);
		pDoc->LogAuto(_T("PC: 각인부 2D 리더 작업중 신호 OFF (PC On->PC Off)"));
		MpeWrite(Plc.DlgMenu01.Reading2dOnEngrave, 0); // 각인부 2D 리더 작업중 신호(PC On->PC Off)
		if (m_pDlgMenu02)
		{
			sVal.Format(_T("%d"), 0);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_RD)->SetWindowText(sVal);
		}
	}

	// 각인부 2D 리더 작업완료 신호
	bChk = MpeRead(Plc.DlgMenu01.Reading2dDoneEngrave); // GUI 각인기 리딩 완료(PLC Off) _T("MB400035")
	if (pDoc->BtnStatus.EngAuto.Read2dDone && !bChk) // 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)
	{
		pDoc->BtnStatus.EngAuto.Read2dDone = FALSE;

		MpeWrite(Plc.DlgMenu01.Reading2dDoneEngrave, 1); // 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)
		pDoc->SetCurrentInfoSignal(_SigInx::_IsEngAutoSeq2dReadDone, TRUE);
		pDoc->LogAuto(_T("PC: 각인부 2D 리더 작업완료 신호 ON (PC가 On, PLC가 확인 후 Off)"));
		if (m_pDlgMenu02)
		{
			sVal.Format(_T("%d"), 1);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_RD_DONE)->SetWindowText(sVal);
		}
	}
	else if (!pDoc->BtnStatus.EngAuto.Read2dDone && bChk)
	{
		if (m_pDlgMenu02)
		{
			sVal.Format(_T("%d"), 0);
			m_pDlgMenu02->GetDlgItem(IDC_STATIC_ENG_RD_DONE)->SetWindowText(sVal);
		}
	}

}


void CGvisR2R_PunchView::MoveEng(double dOffset)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	long lData = (long)(dOffset * 1000.0);
	MpeWrite(Plc.DlgMenu01.FeedCwAoi, 1);	// 검사부 피딩 ON (PLC가 피딩완료 후 OFF)
	if (bDualTest)
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiDn, lData);	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
	else
		MpeWrite(Plc.DlgFrameHigh.FeedOffsetAoiUp, lData);	// 검사부 Feeding 롤러 Offset(*1000, +:더 보냄, -덜 보냄)
}

void CGvisR2R_PunchView::SetMyMsgYes()
{
	//if (pDoc->GetCurrentInfoSignal(_SigInx::_MyMsgYes))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgYes, FALSE);

		if (m_pDlgMyMsg)
		{
			if (m_pDlgMyMsg->m_pDlgMyMsgSub01)
			{
				((CDlgMyMsgSub01*)(m_pDlgMyMsg->m_pDlgMyMsgSub01))->ClickYes();
			}
		}
	}
}

void CGvisR2R_PunchView::SetMyMsgNo()
{
	//if (pDoc->GetCurrentInfoSignal(_SigInx::_MyMsgNo))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgNo, FALSE);

		if (m_pDlgMyMsg)
		{
			if (m_pDlgMyMsg->m_pDlgMyMsgSub01)
			{
				((CDlgMyMsgSub01*)(m_pDlgMyMsg->m_pDlgMyMsgSub01))->ClickNo();
			}
		}
	}
}

void CGvisR2R_PunchView::SetMyMsgOk()
{
	//if (pDoc->GetCurrentInfoSignal(_SigInx::_MyMsgOk))
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgOk, FALSE);

		if (m_pDlgMyMsg)
		{
			if (m_pDlgMyMsg->m_pDlgMyMsgSub02)
			{
				((CDlgMyMsgSub02*)(m_pDlgMyMsg->m_pDlgMyMsgSub02))->ClickOk();
			}
		}
	}
}

void CGvisR2R_PunchView::CheckCurrentInfoSignal(int nMsgID, int nData)
{
	if (m_pEngrave)
		m_pEngrave->SetCurrentInfoSignal(nMsgID, nData);

	//m_bTIM_CHK_RCV_CURR_INFO_SIG = TRUE;
	//SetTimer(TIM_CHK_RCV_CURR_INFO_SIG, 500, NULL);
}

void CGvisR2R_PunchView::CheckMonDispMainSignal()
{
	if (m_pEngrave)
		m_pEngrave->SetMonDispMainSignal();

	//m_bTIM_CHK_RCV_MON_DISP_MAIN_SIG = TRUE;
	//SetTimer(TIM_CHK_RCV_MON_DISP_MAIN_SIG, 500, NULL);
}

void CGvisR2R_PunchView::InitAutoEngSignal()
{
	pDoc->BtnStatus.EngAuto._Init();

	m_bEngFdWrite = FALSE; pDoc->SetStatus(_T("General"), _T("bEngFdWrite"), m_bEngFdWrite);
	m_bEngFdWriteF = FALSE; pDoc->SetStatus(_T("General"), _T("bEngFdWriteF"), m_bEngFdWriteF);
	m_bEngTest = FALSE; pDoc->SetStatus(_T("General"), _T("bEngTest"), m_bEngTest);
	m_bEngTestF = FALSE; pDoc->SetStatus(_T("General"), _T("bEngTestF"), m_bEngTestF);

	MpeWrite(Plc.DlgMenu01.MarkingStartEngrave, 0); // 2D(GUI) 각인 동작 Start신호(PLC On->PC Off)
	MpeWrite(Plc.DlgMenu01.MarkingOnEngrave, 0); // 2D(GUI) 각인 동작Running신호(PC On->PC Off)
	MpeWrite(Plc.DlgMenu01.MarkingDoneEngrave, 0); // 각인부 작업완료.(PC가 On, PLC가 확인 후 Off)

	MpeWrite(Plc.DlgMenu01.Reading2dStartEngrave, 0); // 각인부 2D 리더 시작신호(PLC On->PC Off)
	MpeWrite(Plc.DlgMenu01.Reading2dOnEngrave, 0); // 각인부 2D 리더 작업중 신호(PC On->PC Off)
	MpeWrite(Plc.DlgMenu01.Reading2dDoneEngrave, 0); // 각인부 2D 리더 작업완료 신호.(PC가 On, PLC가 확인 후 Off)

	MpeWrite(Plc.DlgMenu01.FeedingDoneEngrave, 0); // 각인부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)

	//pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt, FALSE);
	//pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt, FALSE);
	//pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng, FALSE);
	//pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone, FALSE);
	//pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d, FALSE);
	//pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone, FALSE);
	//pDoc->SetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone, FALSE);

	pDoc->BtnStatus.EngAuto.Read2dSt = FALSE;
	pDoc->BtnStatus.EngAuto.IsRead2dSt = FALSE;
	pDoc->BtnStatus.EngAuto.OnRead2d = FALSE;
	pDoc->BtnStatus.EngAuto.IsOnRead2d = FALSE;
	pDoc->BtnStatus.EngAuto.Read2dDone = FALSE;
	pDoc->BtnStatus.EngAuto.IsRead2dDone = FALSE;

	pDoc->BtnStatus.EngAuto.MkSt = FALSE;
	pDoc->BtnStatus.EngAuto.IsMkSt = FALSE;
	pDoc->BtnStatus.EngAuto.OnMking = FALSE;
	pDoc->BtnStatus.EngAuto.IsOnMking = FALSE;
	pDoc->BtnStatus.EngAuto.MkDone = FALSE;
	pDoc->BtnStatus.EngAuto.IsMkDone = FALSE;


	//CheckCurrentInfoSignal();
}

BOOL CGvisR2R_PunchView::GetCurrentInfoSignal()
{
	if (!m_pEngrave) return FALSE;

	if (m_pEngrave->m_bRcvSig[_SigInx::_EngAutoInit])
	{
		pDoc->BtnStatus.EngAuto.IsInit = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoInit);
	}
	if (m_pEngrave->m_bRcvSig[_SigInx::_EngAutoSeqMkSt])
	{
		pDoc->BtnStatus.EngAuto.IsMkSt = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt);
	}
	if (m_pEngrave->m_bRcvSig[_SigInx::_EngAutoSeqOnMkIng])
	{
		pDoc->BtnStatus.EngAuto.IsOnMking = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng); pDoc->SetStatus(_T("Engrave"), _T("IsOnMking"), pDoc->BtnStatus.EngAuto.IsOnMking);
	}
	if (m_pEngrave->m_bRcvSig[_SigInx::_EngAutoSeqMkDone])
	{
		pDoc->BtnStatus.EngAuto.IsMkDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone); pDoc->SetStatus(_T("Engrave"), _T("IsMkDone"), pDoc->BtnStatus.EngAuto.IsMkDone);
	}
	if (m_pEngrave->m_bRcvSig[_SigInx::_EngAutoSeq2dReadSt])
	{
		pDoc->BtnStatus.EngAuto.IsRead2dSt = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt);
	}
	if (m_pEngrave->m_bRcvSig[_SigInx::_EngAutoSeqOnReading2d])
	{
		pDoc->BtnStatus.EngAuto.IsOnRead2d = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d); pDoc->SetStatus(_T("Engrave"), _T("IsOnRead2d"), pDoc->BtnStatus.EngAuto.IsOnRead2d);
	}
	if (m_pEngrave->m_bRcvSig[_SigInx::_EngAutoSeq2dReadDone])
	{
		pDoc->BtnStatus.EngAuto.IsRead2dDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone); pDoc->SetStatus(_T("Engrave"), _T("IsRead2dDone"), pDoc->BtnStatus.EngAuto.IsRead2dDone);
	}
	if (m_pEngrave->m_bRcvSig[_SigInx::_EngAutoSeqFdDone])
	{
		pDoc->BtnStatus.EngAuto.IsFdDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqFdDone);
	}

	// 상대방의 로컬파일에서 확인
	if (pDoc->GetCurrentInfoSignal(_SigInx::_MyMsgYes) && !m_pEngrave->m_bRcvSig[_SigInx::_MyMsgYes])
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgYes, FALSE);
		m_pEngrave->m_bRcvSig[_SigInx::_MyMsgYes] = TRUE;
	}
	if (pDoc->GetCurrentInfoSignal(_SigInx::_MyMsgNo) && !m_pEngrave->m_bRcvSig[_SigInx::_MyMsgNo])
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgNo, FALSE);
		m_pEngrave->m_bRcvSig[_SigInx::_MyMsgNo] = TRUE;
	}
	if (pDoc->GetCurrentInfoSignal(_SigInx::_MyMsgOk) && !m_pEngrave->m_bRcvSig[_SigInx::_MyMsgOk])
	{
		pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgOk, FALSE);
		m_pEngrave->m_bRcvSig[_SigInx::_MyMsgOk] = TRUE;
	}

	return TRUE;
}

void CGvisR2R_PunchView::SetLastSerialEng(int nSerial)
{
	if (m_pDlgFrameHigh)
		m_pDlgFrameHigh->SetEngraveLastShot(nSerial);

	pDoc->SetLastSerialEng(nSerial);
}

BOOL CGvisR2R_PunchView::GetCurrentInfoEng()
{
	return pDoc->GetCurrentInfoEng();
}

UINT CGvisR2R_PunchView::ThreadProc10(LPVOID lpContext)	// UpdateReelmapYieldUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[10] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[10].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[10] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_REELMAP_YIELD_UP) // Yield Reelmap
		{
			if (!pThread->m_bTHREAD_UPDATE_REELMAP_UP) // Write Reelmap
			{
				pThread->UpdateReelmapYieldUp(); // Yield Reelmap
				pThread->m_bTHREAD_REELMAP_YIELD_UP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_UP"), pThread->m_bTHREAD_REELMAP_YIELD_UP);
				if (!pDoc->WorkingInfo.LastJob.bDualTest)
				{
					if (pDoc->GetTestMode() == MODE_OUTER)
					{
						pThread->m_bTHREAD_REELMAP_YIELD_ITS = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ITS"), pThread->m_bTHREAD_REELMAP_YIELD_ITS);
					}
				}

				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[10] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc11(LPVOID lpContext)	// UpdateReelmapYieldDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[11] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[11].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[11] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_REELMAP_YIELD_DN) // Yield Reelmap
		{
			if (!pThread->m_bTHREAD_UPDATE_REELMAP_DN && !pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP && !pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
			{
				pThread->UpdateReelmapYieldDn(); // Yield Reelmap
				pThread->m_bTHREAD_REELMAP_YIELD_DN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_DN"), pThread->m_bTHREAD_REELMAP_YIELD_DN);
				pThread->m_bTHREAD_REELMAP_YIELD_ALLUP = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ALLUP"), pThread->m_bTHREAD_REELMAP_YIELD_ALLUP);
				pThread->m_bTHREAD_REELMAP_YIELD_ALLDN = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ALLDN"), pThread->m_bTHREAD_REELMAP_YIELD_ALLDN);
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[11] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc12(LPVOID lpContext)	// UpdateReelmapYieldAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[12] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[12].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[12] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_REELMAP_YIELD_ALLUP) // Yield Reelmap
		{
			if (!pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP) // Write Reelmap
			{
				pThread->UpdateReelmapYieldAllUp(); // Yield Reelmap
				pThread->m_bTHREAD_REELMAP_YIELD_ALLUP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ALLUP"), pThread->m_bTHREAD_REELMAP_YIELD_ALLUP);
				if (pDoc->GetTestMode() == MODE_OUTER)
				{
					pThread->m_bTHREAD_REELMAP_YIELD_ITS = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ITS"), pThread->m_bTHREAD_REELMAP_YIELD_ITS);
				}
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[12] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc13(LPVOID lpContext)	// UpdateReelmapYieldAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[13] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[13].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[13] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
		{
			if (!pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN) // Write Reelmap
			{
				pThread->UpdateReelmapYieldAllDn(); // Yield Reelmap
				pThread->m_bTHREAD_REELMAP_YIELD_ALLDN = FALSE;; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ALLDN"), pThread->m_bTHREAD_REELMAP_YIELD_ALLDN);
				Sleep(0);
			}
			else
				Sleep(30);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[13] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc14(LPVOID lpContext)	// ReloadReelmapUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[14] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[14].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[14] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_UP)
		{
			pThread->ReloadReelmapUp();
			pThread->m_bTHREAD_RELOAD_RST_UP = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[14] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc15(LPVOID lpContext)	// ReloadReelmapDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[15] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[15].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[15] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_DN)
		{
			pThread->ReloadReelmapDn();
			pThread->m_bTHREAD_RELOAD_RST_DN = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[15] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc16(LPVOID lpContext)	// ReloadReelmapAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[16] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[16].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[16] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_ALLUP)
		{
			pThread->ReloadReelmapAllUp();
			pThread->m_bTHREAD_RELOAD_RST_ALLUP = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[16] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc17(LPVOID lpContext)	// ReloadReelmapAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[17] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[17].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[17] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_ALLDN)
		{
			pThread->ReloadReelmapAllDn();
			pThread->m_bTHREAD_RELOAD_RST_ALLDN = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[17] = FALSE;

	return 0;
}

void CGvisR2R_PunchView::UpdateReelmapYieldUp()
{
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->UpdateReelmapYield();					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
}

void CGvisR2R_PunchView::UpdateReelmapYieldAllUp()
{
	if (pDoc->m_pReelMapAllUp)
		pDoc->m_pReelMapAllUp->UpdateReelmapYield();					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
}

void CGvisR2R_PunchView::UpdateReelmapYieldDn()
{
	if (pDoc->m_pReelMapDn)
		pDoc->m_pReelMapDn->UpdateReelmapYield();					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
}

void CGvisR2R_PunchView::UpdateReelmapYieldAllDn()
{
	if (pDoc->m_pReelMapAllDn)
		pDoc->m_pReelMapAllDn->UpdateReelmapYield();					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
}

void CGvisR2R_PunchView::UpdateReelmapYieldIts()
{
	if (pDoc->m_pReelMapIts)
		pDoc->m_pReelMapIts->UpdateReelmapYield();					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
}

BOOL CGvisR2R_PunchView::IsConnectedSr()
{
	if (m_bDestroyedView)
		return FALSE;

	if (m_pSr1000w)
	{
		return m_pSr1000w->IsConnected();
	}

	return FALSE;
}

BOOL CGvisR2R_PunchView::Set2dRead(BOOL bRun)	// Marking Start
{
	if (!pView || !pView->m_pSr1000w)
		return FALSE;

	return (pView->m_pSr1000w->DoRead2DCode());
}

BOOL  CGvisR2R_PunchView::Is2dReadDone()
{
	if (!pView || !pView->m_pSr1000w)
		return FALSE;

	return (!pView->m_pSr1000w->IsRunning());
}

BOOL CGvisR2R_PunchView::Get2dCode(CString &sLot, int &nSerial)
{
	if (!m_pSr1000w)
		return FALSE;

	CString sData;
	if (m_pSr1000w->Get2DCode(sData))
	{
		int nPos = sData.ReverseFind('-');
		if (nPos != -1)
		{
			pDoc->m_sOrderNum = sData.Left(nPos);
			pDoc->m_sShotNum = sData.Right(sData.GetLength() - nPos - 1);
			pDoc->m_nShotNum = _tstoi(pDoc->m_sShotNum);
			sLot = pDoc->m_sOrderNum;
			nSerial = pDoc->m_nShotNum;
		}
		else
		{
			MsgBox(sData);
		}

		return TRUE;
	}

	return FALSE;
}
CString CGvisR2R_PunchView::GetCurrentDBName()
{
	BOOL bRtn = FALSE;
	CString sName = _T("");

	if (m_pDts)
	{
		if (m_pDts->IsUseDts())
		{
			bRtn = m_pDts->GetCurrentDBName(sName);
			if (!bRtn)
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Error - GetCurrentDBName()."), MB_ICONSTOP | MB_OK);
			}
		}
	}

	return sName;
}

BOOL CGvisR2R_PunchView::GetDtsPieceOut(int nSerial, int* pPcsOutIdx, int& nTotPcsOut)
{
	BOOL bRtn = FALSE;
	int nIdx = pDoc->GetPcrIdx0(nSerial);					// 릴맵화면버퍼 인덱스
	CString sLot = pDoc->m_pPcr[0][nIdx]->m_sLot;

	if (m_pDts)
	{
		if (m_pDts->IsUseDts())
		{
			bRtn = m_pDts->LoadPieceOut(sLot, nSerial, pPcsOutIdx, nTotPcsOut);
			if (!bRtn)
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Error - GetDtsPieceOut()."), MB_ICONSTOP | MB_OK);
			}
		}
	}

	return bRtn;
}

UINT CGvisR2R_PunchView::ThreadProc18(LPVOID lpContext) // WriteReelmapIts()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[18] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[18].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[18] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_ITS) // Write Reelmap
		{
			pThread->WriteReelmapIts();
			pThread->m_bTHREAD_UPDATE_REELMAP_ITS = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[18] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc19(LPVOID lpContext) // DispDefImgInner()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[19] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[19].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[19] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (!bLock)
		{
			bLock = TRUE;

			if (pThread->m_bTHREAD_DISP_DEF_INNER)
			{
				pThread->DispDefImgInner();
				Sleep(0);
			}
			else
				Sleep(30);


			bLock = FALSE;
		}
	}
	pThread->m_bThread[19] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc20(LPVOID lpContext)	// UpdateReelmapYieldIts()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[20] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[20].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[20] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_REELMAP_YIELD_ITS) // Yield Reelmap
		{
			pThread->UpdateReelmapYieldIts(); // Yield Reelmap
			pThread->m_bTHREAD_REELMAP_YIELD_ITS = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ITS"), pThread->m_bTHREAD_REELMAP_YIELD_ITS);
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[20] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc21(LPVOID lpContext)	// ReloadReelmapUpInner()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[21] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[21].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[21] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_UP_INNER)
		{
			pThread->ReloadReelmapUpInner();
			pThread->m_bTHREAD_RELOAD_RST_UP_INNER = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[21] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc22(LPVOID lpContext)	// ReloadReelmapDnInner()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[22] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[22].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[22] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_DN_INNER)
		{
			pThread->ReloadReelmapDnInner();
			pThread->m_bTHREAD_RELOAD_RST_DN_INNER = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[22] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc23(LPVOID lpContext)	// ReloadReelmapAllUpInner()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[23] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[23].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[23] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_ALLUP_INNER)
		{
			pThread->ReloadReelmapAllUpInner();
			pThread->m_bTHREAD_RELOAD_RST_ALLUP_INNER = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[23] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc24(LPVOID lpContext)	// ReloadReelmapAllDnInner()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[24] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[24].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[24] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_ALLDN_INNER)
		{
			pThread->ReloadReelmapAllDnInner();
			pThread->m_bTHREAD_RELOAD_RST_ALLDN_INNER = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[24] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc25(LPVOID lpContext)	// ReloadReelmapIts()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[25] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[25].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[25] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_RELOAD_RST_ITS)
		{
			pThread->ReloadReelmapIts();
			pThread->m_bTHREAD_RELOAD_RST_ITS = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[25] = FALSE;

	return 0;
}



BOOL CGvisR2R_PunchView::WriteReelmapIts()
{
	BOOL bRtn = pDoc->m_pReelMapIts->WriteIts(m_nSerialRmapUpdate);

	if (pDoc->m_pReelMap == pDoc->m_pReelMapIts)
	{
		DuplicateRmap(RMAP_ITS);
	}

	return bRtn;
}


void CGvisR2R_PunchView::DoMark0Its()
{
#ifdef TEST_MODE
	return;
#endif

	if (!m_bAuto)
		return;

	int nSerial, nIdx, nErrCode, nRtn;
	CfPoint ptPnt;
	CString sMsg;
	double dStripOut = (pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs / MAX_STRIP_NUM) * _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
	int nStripOut = int(dStripOut);
	if (dStripOut > nStripOut)
		nStripOut++;			// 스트립 양폐 비율

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!IsRun())																		// 정지상태에서
	{
		if (IsOnMarking0())																// 마킹중에
		{
			if (m_pMotion->IsEnable(MS_X0) && m_pMotion->IsEnable(MS_Y0))				// 모션이 Enable상태이고
			{
				if (m_pMotion->IsMotionDone(MS_X0) && m_pMotion->IsMotionDone(MS_Y0))	// 모션 Done상태이면,
				{
					if (!IsInitPos0() && !IsPinPos0())									// 초기위치가 아니거나, 핀위치가 아닐때
						MoveInitPos0();													// 초기위치로 이동
				}
			}

			if (m_nStepMk[0] < 13 && m_nStepMk[0] > 8) // Mk0();
			{
				m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
			}
		}
		return;
	}

	m_sDispSts[0].Format(_T("%d"), m_nStepMk[0]);

	switch (m_nStepMk[0])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nStepMk[0]++;
		break;
	case 1:
		if (!IsInitPos0())
			MoveInitPos0();
		m_nStepMk[0]++;
		break;
	case 2:
		nSerial = m_nBufUpSerial[0]; // Cam0
		if (m_nBufUpSerial[1] < m_nBufUpSerial[0])
		{
			sMsg.Format(_T("%d:%d"), nSerial, GetTotDefPcs0Its(nSerial));
			pDoc->LogAuto(sMsg);
		}
		if (nSerial > 0)
		{
			if ((nErrCode = GetErrCode0Its(nSerial)) != 1)
			{
				m_nMkPcs[0] = GetTotDefPcs0Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
				m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
			else
			{
				m_nStepMk[0]++;
			}
		}
		else
		{
			StopFromThread();
			MsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 1);
			BuzzerFromThread(TRUE, 0);
			DispMain(_T("정 지"), RGB_RED);
		}
		break;
	case 3:
		m_nStepMk[0]++;
		break;
	case 4:
		m_nStepMk[0]++;
		break;
	case 5:
		m_nStepMk[0]++;
		break;
	case 6:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0Its(nSerial))
		{
			if (!IsNoMk0())
			{
				;
			}
			else
			{
				if (!IsReview0())
				{
					if (m_bReview)
					{
						m_nMkPcs[0] = GetTotDefPcs0Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
						m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
						break;
					}
				}
			}
			// Punching On이거나 Review이면 다음으로 진행
			SetDelay0(100, 1);		// [mSec]
			m_nStepMk[0]++;
		}
		else
		{
			m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
		}
		break;
	case 7:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (!WaitDelay0(1))		// F:Done, T:On Waiting....		// Delay후에
		{
			m_nMkPcs[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);

			if (!IsNoMk0())										// Punching On이면
			{
				m_nStepMk[0]++;
			}
			else												// Punching이 Off이고
			{
				if (IsReview0())								// Review이면 다음으로
				{
					m_nStepMk[0]++;
				}
				else											// Review가 아니면
				{
					if (m_bReview)
					{
						m_nMkPcs[0] = GetTotDefPcs0Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
						m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
					}
					else
						m_nStepMk[0]++;
				}
			}
		}
		break;
	case 8:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0Its(nSerial))	// 마킹완료Check
		{
			if (m_nMkPcs[0] + 1 < GetTotDefPcs0Its(nSerial))		// 다음 마킹위치가 있으면
			{
				ptPnt = GetMkPnt0Its(nSerial, m_nMkPcs[0] + 1);	// 다음 마킹위치
				m_dNextTarget[AXIS_X0] = ptPnt.x;
				m_dNextTarget[AXIS_Y0] = ptPnt.y;
			}
			else												// 다음 마킹위치가 없으면
			{
				m_dNextTarget[AXIS_X0] = -1.0;
				m_dNextTarget[AXIS_Y0] = -1.0;
			}

			ptPnt = GetMkPnt0Its(nSerial, m_nMkPcs[0]);			// 이번 마킹위치
			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화. (마킹하지 않음)
			{
				m_nMkPcs[0]++;
				m_nStepMk[0] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
				break;
			}

			nIdx = GetMkStripIdx0Its(nSerial, m_nMkPcs[0]);		// 1 ~ 4 : strip index
			if (nIdx > 0)										// Strip index가 정상이면,
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[0]++;
					m_nStepMk[0] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
					break;
				}
				else
				{
					if (m_nMkStrip[0][nIdx - 1] >= nStripOut)
					{
						m_nMkPcs[0]++;
						m_nStepMk[0] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
						break;
					}
					else
						m_nMkStrip[0][nIdx - 1]++;
				}
			}
			else
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			m_dTarget[AXIS_X0] = ptPnt.x;
			m_dTarget[AXIS_Y0] = ptPnt.y;

			m_nStepMk[0]++;
		}
		else
		{
			m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
		}
		break;
	case 9:
		// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
		// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-
		ptPnt.x = m_dTarget[AXIS_X0];
		ptPnt.y = m_dTarget[AXIS_Y0];

		if (m_dNextTarget[AXIS_X0] < 0)
			m_bCollision[0] = ChkCollision(AXIS_X0, m_dTarget[AXIS_X0]);
		else
			m_bCollision[0] = ChkCollision(AXIS_X0, m_dTarget[AXIS_X0], m_dNextTarget[AXIS_X0]);

		if (!m_bCollision[0])
		{
			if (IsMoveDone0())
			{
				Move0(ptPnt, m_bCam);
				m_nStepMk[0]++;
			}
		}
		else if (m_bPriority[0]) // Cam0 <-
		{
			if (IsMoveDone0())
			{
				m_bCollision[0] = FALSE;
				m_bPriority[0] = FALSE;
				Move0(ptPnt, m_bCam);
				m_nStepMk[0]++;
			}
		}
		else if (m_bPriority[2]) // Cam1 ->
		{
			if (IsMoveDone0())
			{
				m_bCollision[0] = FALSE;
				m_bPriority[2] = FALSE;
				ptPnt.x = 0.0;
				ptPnt.y = m_dEnc[AXIS_Y0];
				Move0(ptPnt, FALSE);
			}
		}
		else if (m_bPriority[2] && m_bPriority[3]) // Cam1 <- && Cam0 ->
		{
			break; // Cam1 이 안전위치로 대피할 때까지 기다린다.
		}
		break;
	case 10:
		m_nStepMk[0]++;
		break;
	case 11:
		m_nStepMk[0]++;
		break;
	case 12:
		if (IsMoveDone0())
			m_nStepMk[0]++;
		break;
	case 13:
		if (!IsNoMk0())
		{
			m_dwStMkDn[0] = GetTickCount();
			Mk0();
		}
		else
		{
			// Verify - Mk0
			SetDelay0(pDoc->m_nDelayShow, 1);		// [mSec]

			if (m_pDlgMenu02)
				m_pDlgMenu02->InitMkPmRst(0);

			if (!SaveMk0Img(m_nMkPcs[0]))
			{
				pView->ClrDispMsg();
				//AfxMessageBox(_T("Error-SaveMk0Img()"));
				if (pDoc->WorkingInfo.LastJob.bUseJudgeMk)
				{
#ifdef USE_VISION
					if (!m_pVision[0]->m_bMkJudge)
					{
						nRtn = MsgBox(_T("좌측 펀칭이 미마킹으로 보입니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
						if (IDYES == nRtn)
						{
							ptPnt.x = m_dTarget[AXIS_X0];
							ptPnt.y = m_dTarget[AXIS_Y0];

							Move0(ptPnt, FALSE, WAIT);
							Sleep(30);
							Mk0();
							Sleep(100);
							Move0(ptPnt, TRUE, WAIT);
							m_nMkPcs[0]--; // Repeat Current Position Verify.
							m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
							Stop();
							break;
						}
					}
#endif
				}
			}
		}
		m_nStepMk[0]++;
		break;
	case 14:
		m_nStepMk[0]++;
		break;
	case 15:
		m_nStepMk[0]++;
		break;
	case 16:
		if (IsNoMk0())
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
				m_nStepMk[0]++;
		}
		else
			m_nStepMk[0]++;
		break;
	case 17:
		if (!IsNoMk0())
		{
			if (IsMk0Done())
			{
				// One more MK On Start....
				if (!m_nMkPcs[0] && !m_bAnswer[2])
				{
					m_bAnswer[2] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[2]"), m_bAnswer[2]);
					Mk0();
				}
				else
				{
					m_bAnswer[2] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[2]"), m_bAnswer[2]);
					m_nMkPcs[0]++;
					m_nStepMk[0]++;
					m_nStepMk[0]++;
				}
			}
			else
			{
				if (m_dwStMkDn[0] + 5000 < GetTickCount())
				{
					BuzzerFromThread(TRUE, 0);
					DispMain(_T("정 지"), RGB_RED);
					if (m_pVoiceCoil[0])
						m_pVoiceCoil[0]->SearchHomeSmac0();

					//nRtn = AsyncMsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					nRtn = MsgBox(_T("보이스코일(좌) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[0] = 13; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
					}
					else if (nRtn < 0)
						m_nStepMk[0]++; // Wait...
					else
					{
						m_bAnswer[2] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[2]"), m_bAnswer[2]);
						m_nMkPcs[0]++;
						m_nStepMk[0]++;
						m_nStepMk[0]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[0]++;
			m_nStepMk[0]++;
			m_nStepMk[0]++;
		}
		break;
	case 18:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[0] = 13; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
			else
			{
				m_bAnswer[2] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[2]"), m_bAnswer[2]);
				m_nMkPcs[0]++;
				m_nStepMk[0]++;
				StopFromThread();
			}
		}
		break;
	case 19:
		m_nStepMk[0] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
		break;
	case MK_DONE_CHECK:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0Its(nSerial))
		{
			if (IsNoMk0())
			{
				if (!pDoc->WorkingInfo.System.bNoMk)
				{
					if (IsReview0())
					{
						if (IsJogRtDn0())
							m_nStepMk[0]++;
					}
					else if (!pDoc->WorkingInfo.LastJob.bVerify)
					{
						m_nMkPcs[0] = GetTotDefPcs0Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
						m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
					}
					else
					{
						m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
					}
				}
				else
				{
					m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
				}
			}
			else
			{
				m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
			}
		}
		else
		{
			if (IsNoMk0())
			{
				if (IsReview0())
				{
					if (IsJogRtDn0())
					{
						m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
					}
				}
				else
				{
					m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
				}
			}
			else
			{
				m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
		}
		break;
	case MK_DONE_CHECK + 1:
		nSerial = m_nBufUpSerial[0]; // Cam0

		if (m_nMkPcs[0] < GetTotDefPcs0Its(nSerial))
		{
			if (IsNoMk0())
			{
				if (IsReview0())
				{
					if (IsJogRtUp0())
					{
						m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
					}
				}
				else
				{
					m_nMkPcs[0] = GetTotDefPcs0Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
					m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
				}
			}
			else
			{
				m_nStepMk[0] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);	// 마킹완료Check
			}
		}
		else
		{
			m_nStepMk[0] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
		}
		break;

	case MK_END:
		if (IsMoveDone0())
			m_nStepMk[0]++;
		break;
	case 101:
		SetDelay0(100, 1);		// [mSec]
		m_nStepMk[0]++;
		break;
	case 102:
		if (!WaitDelay0(1))		// F:Done, T:On Waiting....
		{
			m_nStepMk[0]++;
		}
		break;
	case 103:
		if (!IsInitPos0())
		{
			m_dTarget[AXIS_X0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
			m_dTarget[AXIS_Y0] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);
			m_dNextTarget[AXIS_X0] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[0]);
			m_dNextTarget[AXIS_Y0] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[0]);

			MoveInitPos0();
		}

		pDoc->SaveMkCntL();
		m_nStepMk[0]++;
		break;
	case 104:
		if (IsMoveDone0())
		{
			m_nStepMk[0]++;
			SetDelay0(10000, 1);		// [mSec]
		}
		break;
	case 105:
		if (IsInitPos0())
		{
			m_nStepMk[0]++;
		}
		else
		{
			if (!WaitDelay0(1))		// F:Done, T:On Waiting....
			{
				m_nStepMk[0] = ERR_PROC; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
		}
		break;
	case 106: // MK Done....
		m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
		m_bTHREAD_MK[0] = FALSE; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
		break;

	case ERR_PROC:
		DispMain(_T("정 지"), RGB_RED);
		if (m_pVoiceCoil[0])
			m_pVoiceCoil[0]->SearchHomeSmac0();
		MsgBox(_T("보이스코일(좌) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 1);
		m_nStepMk[0]++;
		break;
	case ERR_PROC + 1:
		nSerial = m_nBufUpSerial[0];

		if (m_bCam)
			sMsg.Format(_T("%d번 Shot을 다시 불량확인을 하시겠습니까?"), nSerial);
		else
			sMsg.Format(_T("%d번 Shot을 다시 마킹하시겠습니까?"), nSerial);

		m_nRtnMyMsgBoxIdx = 0;
		m_bRtnMyMsgBox[0] = FALSE;
		m_nRtnMyMsgBox[0] = -1;
		MsgBox(sMsg, 1, MB_YESNO);
		sMsg.Empty();
		m_nStepMk[0]++;
		break;
	case ERR_PROC + 2:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[0] = ERR_PROC + 10; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
			else
			{
				m_nRtnMyMsgBoxIdx = 0;
				m_bRtnMyMsgBox[0] = FALSE;
				m_nRtnMyMsgBox[0] = -1;
				sMsg.Format(_T("계속 다음 작업을 진행하시겠습니까?"), nSerial);
				MsgBox(sMsg, 1, MB_YESNO);
				sMsg.Empty();

				m_nStepMk[0]++;
			}
		}
		break;
	case ERR_PROC + 3:
		if ((nRtn = WaitRtnVal(1)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[0] = ERR_PROC + 20; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
			}
			else
			{
				m_bDispMsgDoAuto[8] = TRUE;
				m_nStepDispMsg[8] = FROM_DOMARK0;
			}
		}
		break;
	case ERR_PROC + 10:
		m_bReMark[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bReMark[0]"), m_bReMark[0]);
		m_bTHREAD_MK[0] = FALSE; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
		m_nStepMk[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
		break;
	case ERR_PROC + 20: // MK Done....
		m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
		m_bTHREAD_MK[0] = FALSE; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
		break;
	}
}


void CGvisR2R_PunchView::DoMark1Its()
{
#ifdef TEST_MODE
	return;
#endif

	if (!m_bAuto)
		return;

	int nSerial, nIdx, nErrCode, nRtn;
	CfPoint ptPnt;
	CString sMsg;
	double dStripOut = (pDoc->m_Master[0].m_pPcsRgn->m_nTotPcs / MAX_STRIP_NUM) * _tstof(pDoc->WorkingInfo.LastJob.sStripOutRatio) / 100.0;
	int nStripOut = int(dStripOut);
	if (dStripOut > nStripOut)
		nStripOut++;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;


	if (!IsRun())
	{
		if (IsOnMarking1())
		{
			if (m_pMotion->IsEnable(MS_X1) && m_pMotion->IsEnable(MS_Y1))
			{
				if (m_pMotion->IsMotionDone(MS_X1) && m_pMotion->IsMotionDone(MS_Y1))
				{
					if (!IsMkEdPos1() && !IsPinPos1())
						MoveMkEdPos1();
				}
			}

			if (m_nStepMk[1] < 13 && m_nStepMk[1] > 8) // Mk1();
			{
				m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
			}
		}
		return;
	}


	m_sDispSts[1].Format(_T("%d"), m_nStepMk[1]);

	switch (m_nStepMk[1])
	{
	case 0:
		if (IsNoMk())
			ShowLive();
		m_nStepMk[1]++;
		break;
	case 1:
		if (!IsInitPos1())
			MoveInitPos1();
		m_nStepMk[1]++;
		break;
	case 2:
		nSerial = m_nBufUpSerial[1]; // Cam1
		sMsg.Format(_T("%d:%d,%d:%d"), nSerial-1, GetTotDefPcs0Its(nSerial-1), nSerial, GetTotDefPcs1Its(nSerial));
		pDoc->LogAuto(sMsg);
		if (nSerial > 0)
		{
			if ((nErrCode = GetErrCode1Its(nSerial)) != 1)
			{
				m_nMkPcs[1] = GetTotDefPcs1Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
				m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
			else
			{
				m_nStepMk[1]++;
			}
		}
		else
		{
			StopFromThread();
			MsgBox(_T("버퍼의 시리얼이 맞지않습니다."), 2);
			BuzzerFromThread(TRUE, 0);
			DispMain(_T("정 지"), RGB_RED);
		}
		break;
	case 3:
		m_nStepMk[1]++;
		break;
	case 4:
		m_nStepMk[1]++;
		break;
	case 5:
		m_nStepMk[1]++;
		break;
	case 6:

		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1Its(nSerial))
		{
			if (!IsNoMk1())
			{
				;
			}
			else
			{
				if (!IsReview1())
				{
					if (m_bReview)
					{
						m_nMkPcs[1] = GetTotDefPcs1Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
						m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
						break;
					}
				}
			}
			SetDelay1(100, 6);		// [mSec]
			m_nStepMk[1]++;
		}
		else
		{
			m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
		}
		break;
	case 7:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (!WaitDelay1(6))		// F:Done, T:On Waiting....
		{
			m_nMkPcs[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);

			if (!IsNoMk1())
			{
				m_nStepMk[1]++;
			}
			else
			{
				if (IsReview1())
				{
					m_nStepMk[1]++;
				}
				else
				{
					if (m_bReview)
					{
						m_nMkPcs[1] = GetTotDefPcs1Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
						m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
					}
					else
						m_nStepMk[1]++;
				}
			}
		}
		break;
	case 8:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1Its(nSerial))	// 마킹완료Check
		{
			if (m_nMkPcs[1] + 1 < GetTotDefPcs1Its(nSerial))
			{
				ptPnt = GetMkPnt1Its(nSerial, m_nMkPcs[1] + 1);
				m_dNextTarget[AXIS_X1] = ptPnt.x;
				m_dNextTarget[AXIS_Y1] = ptPnt.y;
			}
			else
			{
				m_dNextTarget[AXIS_X1] = -1.0;
				m_dNextTarget[AXIS_Y1] = -1.0;
			}

			ptPnt = GetMkPnt1Its(nSerial, m_nMkPcs[1]);
			if (ptPnt.x < 0.0 && ptPnt.y < 0.0) // 양품화.
			{
				m_nMkPcs[1]++;
				m_nStepMk[1] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
				break;
			}

			nIdx = GetMkStripIdx1Its(nSerial, m_nMkPcs[1]);
			if (nIdx > 0)
			{
				if (!IsMkStrip(nIdx)) // Strip[] Mk Off
				{
					m_nMkPcs[1]++;
					m_nStepMk[1] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
					break;
				}
				else
				{
					if (m_nMkStrip[1][nIdx - 1] >= nStripOut)
					{
						m_nMkPcs[1]++;
						m_nStepMk[1] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
						break;
					}
					else
						m_nMkStrip[1][nIdx - 1]++;
				}
			}
			else
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Strip Index Failed."));
				break;
			}

			m_dTarget[AXIS_X1] = ptPnt.x;
			m_dTarget[AXIS_Y1] = ptPnt.y;

			m_nStepMk[1]++;
		}
		else
		{
			m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
		}
		break;
	case 9:
		// Cam0 : m_bPriority[0] <-, m_bPriority[3] ->
		// Cam1 : m_bPriority[1] ->, m_bPriority[2] <-
		ptPnt.x = m_dTarget[AXIS_X1];
		ptPnt.y = m_dTarget[AXIS_Y1];

		if (m_dNextTarget[AXIS_X1] < 0)
			m_bCollision[1] = ChkCollision(AXIS_X1, m_dTarget[AXIS_X1]);
		else
			m_bCollision[1] = ChkCollision(AXIS_X1, m_dTarget[AXIS_X1], m_dNextTarget[AXIS_X1]);

		if (!m_bCollision[1])
		{
			if (IsMoveDone1())
			{
				Move1(ptPnt, m_bCam);
				m_nStepMk[1]++;
			}
		}
		else if (m_bPriority[1]) // Cam1 ->
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[1] = FALSE;
				Move1(ptPnt, m_bCam);
				m_nStepMk[1]++;
			}
		}
		else if (m_bPriority[0]) // Cam0 <-	// 우선 순위 결정에서 Cam0에 먼저 양보함.
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[3] = FALSE;
				ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
				ptPnt.y = m_dEnc[AXIS_Y1];
				Move1(ptPnt, FALSE);
			}
		}
		else if (m_bPriority[2]) // Cam1 <-		// 우선 순위 결정에서 Cam0에 먼저 양보함.
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[3] = FALSE;
				ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
				ptPnt.y = m_dEnc[AXIS_Y1];
				Move1(ptPnt, FALSE);
			}
		}
		else if (m_bPriority[3])// Cam0 ->	// 우선 순위 결정에서 Cam0에 먼저 양보함.
		{
			if (IsMoveDone1())
			{
				m_bCollision[1] = FALSE;
				m_bPriority[3] = FALSE;
				ptPnt.x = _tstof(pDoc->WorkingInfo.Motion.sSafeZone);
				ptPnt.y = m_dEnc[AXIS_Y1];
				Move1(ptPnt, FALSE);
			}
		}
		break;
	case 10:
		m_nStepMk[1]++;
		break;
	case 11:
		m_nStepMk[1]++;
		break;
	case 12:
		if (IsMoveDone1())
			m_nStepMk[1]++;
		break;
	case 13:
		if (!IsNoMk1())
		{
			m_dwStMkDn[1] = GetTickCount();
			Mk1();
		}
		else
		{
			// Verify - Mk1
			SetDelay1(pDoc->m_nDelayShow, 6);		// [mSec]

			if (m_pDlgMenu02)
				m_pDlgMenu02->InitMkPmRst(1);

			if (!SaveMk1Img(m_nMkPcs[1]))
			{
				pView->ClrDispMsg();
				//AfxMessageBox(_T("Error-SaveMk1Img()"));
				if (pDoc->WorkingInfo.LastJob.bUseJudgeMk)
				{
#ifdef USE_VISION
					if (!m_pVision[1]->m_bMkJudge)
					{
						nRtn = MsgBox(_T("우측 펀칭이 미마킹으로 보입니다.\r\n마킹을 다시 시도하시겠습니까?"), 1, MB_YESNO);
						if (IDYES == nRtn)
						{
							ptPnt.x = m_dTarget[AXIS_X1];
							ptPnt.y = m_dTarget[AXIS_Y1];

							Move1(ptPnt, FALSE, WAIT);
							Sleep(30);
							Mk1();
							Sleep(100);
							Move1(ptPnt, TRUE, WAIT);
							m_nMkPcs[1]--; // Repeat Current Position Verify.
							m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
							Stop();
							break;
						}
					}
#endif
				}
			}
		}
		m_nStepMk[1]++;
		break;
	case 14:
		m_nStepMk[1]++;
		break;
	case 15:
		m_nStepMk[1]++;
		break;
	case 16:
		if (IsNoMk1())
		{
			if (!WaitDelay1(6))		// F:Done, T:On Waiting....
				m_nStepMk[1]++;
		}
		else
			m_nStepMk[1]++;
		break;
	case 17:
		if (!IsNoMk1())
		{
			if (IsMk1Done())
			{
				// One more MK On Start....
				if (!m_nMkPcs[1] && !m_bAnswer[3])
				{
					m_bAnswer[3] = TRUE; pDoc->SetStatus(_T("General"), _T("bAnswer[3]"), m_bAnswer[3]);
					Mk1();
				}
				else
				{
					m_bAnswer[3] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[3]"), m_bAnswer[3]);
					m_nMkPcs[1]++;
					m_nStepMk[1]++;
					m_nStepMk[1]++;
				}

			}
			else
			{
				if (m_dwStMkDn[1] + 5000 < GetTickCount())
				{
					BuzzerFromThread(TRUE, 0);
					DispMain(_T("정 지"), RGB_RED);
					if (m_pVoiceCoil[1])
						m_pVoiceCoil[1]->SearchHomeSmac1();

					nRtn = MsgBox(_T("보이스코일(우) 통신완료가 않됩니다.\r\n마킹을 다시 시도하시겠습니까?"), 2, MB_YESNO);
					if (IDYES == nRtn)
					{
						DispMain(_T("운전중"), RGB_RED);
						m_nStepMk[1] = 13; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
					}
					else if (nRtn < 0)
						m_nStepMk[1]++; // Wait...
					else
					{
						m_bAnswer[3] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[3]"), m_bAnswer[3]);
						m_nMkPcs[1]++;
						m_nStepMk[1]++;
						m_nStepMk[1]++;
						Stop();
					}
				}
			}
		}
		else
		{
			m_nMkPcs[1]++;
			m_nStepMk[1]++;
			m_nStepMk[1]++;
		}
		break;
	case 18:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				DispMain(_T("운전중"), RGB_RED);
				m_nStepMk[1] = 13; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
			else
			{
				m_bAnswer[3] = FALSE; pDoc->SetStatus(_T("General"), _T("bAnswer[3]"), m_bAnswer[3]);
				m_nMkPcs[1]++;
				m_nStepMk[1]++;
				StopFromThread();
			}
		}
		break;
	case 19:
		m_nStepMk[1] = MK_DONE_CHECK; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
		break;
	case MK_DONE_CHECK:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1Its(nSerial))
		{
			if (IsNoMk1())
			{
				if (!pDoc->WorkingInfo.System.bNoMk)
				{
					if (IsReview1())
					{
						if (IsJogRtDn1())
							m_nStepMk[1]++;
					}
					else if (!pDoc->WorkingInfo.LastJob.bVerify)
					{
						m_nMkPcs[1] = GetTotDefPcs1Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
						m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
					}
					else
					{
						m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
					}
				}
				else
				{
					m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
				}
			}
			else
			{
				m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
			}
		}
		else
		{
			if (IsNoMk1())
			{
				if (IsReview1())
				{
					if (IsJogRtDn1())
					{
						m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
					}
				}
				else
				{
					m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
				}
			}
			else
			{
				m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
		}
		break;
	case MK_DONE_CHECK + 1:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_nMkPcs[1] < GetTotDefPcs1Its(nSerial))
		{
			if (IsNoMk1())
			{
				if (IsReview1())
				{
					if (IsJogRtUp1())
					{
						m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
					}
				}
				else
				{
					m_nMkPcs[1] = GetTotDefPcs1Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
					m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
				}
			}
			else
			{
				m_nStepMk[1] = 8; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);	// 마킹완료Check
			}
		}
		else
		{
			m_nStepMk[1] = MK_END; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
		}
		break;


	case MK_END:
		if (IsMoveDone1())
			m_nStepMk[1]++;
		break;
	case 101:
		SetDelay1(100, 6);		// [mSec]
		m_nStepMk[1]++;
		break;
	case 102:
		if (!WaitDelay1(6))		// F:Done, T:On Waiting....
			m_nStepMk[1]++;
		break;
	case 103:
		if (!IsMkEdPos1())
		{
			m_dTarget[AXIS_X1] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
			m_dTarget[AXIS_Y1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);
			m_dNextTarget[AXIS_X1] = _tstof(pDoc->WorkingInfo.Motion.sStPosX[1]);
			m_dNextTarget[AXIS_Y1] = _tstof(pDoc->WorkingInfo.Motion.sStPosY[1]);

			MoveMkEdPos1();
		}

		pDoc->SaveMkCntR();
		m_nStepMk[1]++;
		break;
	case 104:
		if (IsMoveDone1())
		{
			m_nStepMk[1]++;
			SetDelay1(10000, 6);		// [mSec]
		}
		break;
	case 105:
		if (IsMkEdPos1())
		{
			m_nStepMk[1]++;
		}
		else
		{
			if (!WaitDelay1(6))		// F:Done, T:On Waiting....
			{
				m_nStepMk[1] = ERR_PROC; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
		}
		break;
	case 106: // MK Done....
		m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
		m_bTHREAD_MK[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);
		break;

	case ERR_PROC:
		DispMain(_T("정 지"), RGB_RED);
		if (m_pVoiceCoil[1])
			m_pVoiceCoil[1]->SearchHomeSmac1();
		MsgBox(_T("보이스코일(우) 초기위치 이동이 되지 않습니다.\r\n마킹상태를 확인하세요."), 2);
		m_nStepMk[1]++;
		break;
	case ERR_PROC + 1:
		nSerial = m_nBufUpSerial[1]; // Cam1

		if (m_bCam)
			sMsg.Format(_T("%d번 Shot을 다시 불량확인을 하시겠습니까?"), nSerial);
		else
			sMsg.Format(_T("%d번 Shot을 다시 마킹하시겠습니까?"), nSerial);

		m_nRtnMyMsgBoxIdx = 1;
		m_bRtnMyMsgBox[1] = FALSE;
		m_nRtnMyMsgBox[1] = -1;
		MsgBox(sMsg, 2, MB_YESNO);
		sMsg.Empty();
		m_nStepMk[1]++;
		break;
	case ERR_PROC + 2:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[1] = ERR_PROC + 10; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
			else
			{
				m_nRtnMyMsgBoxIdx = 1;
				m_bRtnMyMsgBox[1] = FALSE;
				m_nRtnMyMsgBox[1] = -1;
				sMsg.Format(_T("계속 다음 작업을 진행하시겠습니까?"), nSerial);
				MsgBox(sMsg, 2, MB_YESNO);
				sMsg.Empty();

				m_nStepMk[1]++;

			}
		}
		break;
	case ERR_PROC + 3:
		if ((nRtn = WaitRtnVal(2)) > -1)
		{
			if (IDYES == nRtn)
			{
				m_nStepMk[1] = ERR_PROC + 20; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
			}
			else
			{
				m_bDispMsgDoAuto[9] = TRUE;
				m_nStepDispMsg[9] = FROM_DOMARK1;
			}
		}
		break;
	case ERR_PROC + 10:
		m_bReMark[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bReMark[1]"), m_bReMark[1]);
		m_bTHREAD_MK[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);
		m_nStepMk[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
		break;
	case ERR_PROC + 20: // MK Done....
		m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
		m_bTHREAD_MK[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);
		break;
	}
}


int CGvisR2R_PunchView::GetErrCode0Its(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.32"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

#ifndef	TEST_MODE
	int nErr[2];
	nErr[0] = GetErrCodeUp0Its(nSerial);
	if (nErr[0] != 1)
		return nErr[0];
#endif

	return 1;
}

int CGvisR2R_PunchView::GetErrCodeUp0Its(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.33"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcrIts)
	{
		if (pDoc->m_pPcrIts[nIdx])
			nErrCode = pDoc->m_pPcrIts[nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}

int CGvisR2R_PunchView::GetErrCodeDn0Its(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.34"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcrIts)
	{
		if (pDoc->m_pPcrIts[nIdx])
			nErrCode = pDoc->m_pPcrIts[nIdx]->m_nErrPnl;
	}
#endif

	return nErrCode;
}


int CGvisR2R_PunchView::GetErrCode1Its(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.35"));
		return 0;
	}

#ifndef	TEST_MODE
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nErr[2];
	nErr[0] = GetErrCodeUp1Its(nSerial);
	if (nErr[0] != 1)
		return nErr[0];
#endif

	return 1;
}

int CGvisR2R_PunchView::GetErrCodeUp1Its(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.36"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcrIts)
	{
		if (pDoc->m_pPcrIts[nIdx])
			nErrCode = pDoc->m_pPcrIts[nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}

int CGvisR2R_PunchView::GetErrCodeDn1Its(int nSerial) // 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.37"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nErrCode = 1;

#ifndef	TEST_MODE
	if (pDoc->m_pPcrIts)
	{
		if (pDoc->m_pPcrIts[nIdx])
			nErrCode = pDoc->m_pPcrIts[nIdx]->m_nErrPnl;
		else
			return 2;	// Code Setting Error.
	}
	else
		return 2;	// Code Setting Error.
#endif

	return nErrCode;
}


int CGvisR2R_PunchView::GetTotDefPcs0Its(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.41"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (bDualTest)
	{
		if (pDoc->m_pPcrIts)
		{
			if (pDoc->m_pPcrIts[nIdx])
				nTotDef = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
		}
	}
	else
	{
		if (pDoc->m_pPcrIts)
		{
			if (pDoc->m_pPcrIts[nIdx])
				nTotDef = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
		}
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_PunchView::GetTotDefPcsUp0Its(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.42"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcrIts)
	{
		if (pDoc->m_pPcrIts[nIdx])
			nTotDef = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_PunchView::GetTotDefPcsDn0Its(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.43"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcrIts)
	{
		if (pDoc->m_pPcrIts[nIdx])
			nTotDef = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}


int CGvisR2R_PunchView::GetTotDefPcs1Its(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.44"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (bDualTest)
	{
		if (pDoc->m_pPcrIts)
		{
			if (pDoc->m_pPcrIts[nIdx])
				nTotDef = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
		}
	}
	else
	{
		if (pDoc->m_pPcrIts)
		{
			if (pDoc->m_pPcrIts[nIdx])
				nTotDef = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
		}
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_PunchView::GetTotDefPcsUp1Its(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.45"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcrIts)
	{
		if (pDoc->m_pPcrIts[nIdx])
			nTotDef = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}

int CGvisR2R_PunchView::GetTotDefPcsDn1Its(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.46"));
		return 0;
	}

	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nTotDef = 0;

#ifndef TEST_MODE
	if (pDoc->m_pPcrIts)
	{
		if (pDoc->m_pPcrIts[nIdx])
			nTotDef = pDoc->m_pPcrIts[nIdx]->m_nTotDef;
	}
#else
	nTotDef = 1;
#endif

	return nTotDef;
}


CfPoint CGvisR2R_PunchView::GetMkPnt0Its(int nSerial, int nMkPcs) // pcr 시리얼, pcr 불량 피스 읽은 순서 인덱스
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.47"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx0(nSerial);
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	int nDefPcsId = 0;

	if (bDualTest)
	{
		if (pDoc->m_pPcrIts)	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcrIts[nIdx])
			{
				if (pDoc->m_pPcrIts[nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcrIts[nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt0(nDefPcsId); // Cam0의 Mk 포인트.
					}
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcrIts)	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcrIts[nIdx])
			{
				if (pDoc->m_pPcrIts[nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcrIts[nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt0(nDefPcsId); // Cam0의 Mk 포인트.
					}
				}
			}
		}
	}
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}

CfPoint CGvisR2R_PunchView::GetMkPnt1Its(int nSerial, int nMkPcs)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.50"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	CfPoint ptPnt;
	ptPnt.x = -1.0;
	ptPnt.y = -1.0;

#ifndef TEST_MODE
	int nDefPcsId = 0;
	if (bDualTest)
	{
		if (pDoc->m_pPcrIts)	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcrIts[nIdx])
			{
				if (pDoc->m_pPcrIts[nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcrIts[nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
						{
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt1(nDefPcsId); // Cam1의 Mk 포인트.
						}
					}
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcrIts)	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcrIts[nIdx])
			{
				if (pDoc->m_pPcrIts[nIdx]->m_pDefPcs)
				{
					if (pDoc->m_pPcrIts[nIdx]->m_pMk[nMkPcs] != -2) // -2 (NoMarking)
					{
						nDefPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[nMkPcs];
						if (pDoc->m_Master[0].m_pPcsRgn)
						{
							ptPnt = pDoc->m_Master[0].m_pPcsRgn->GetMkPnt1(nDefPcsId); // Cam1의 Mk 포인트.
						}
					}
				}
			}
		}
	}
#else
	ptPnt.x = 1.0;
	ptPnt.y = 1.0;
#endif

	return ptPnt;
}


int CGvisR2R_PunchView::GetMkStripIdx0Its(int nSerial, int nMkPcs) // 0 : Fail , 1~4 : Strip Idx
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		AfxMessageBox(_T("Serial Error.48"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx0(nSerial);
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->m_nRow;
	int nStripY = int(nNodeY / MAX_STRIP_NUM);
	int nStripIdx = 0;

#ifndef TEST_MODE
	int nDefPcsId = 0, nNum = 0, nMode = 0, nRow = 0;

	if (bDualTest)
	{
		if (pDoc->m_pPcrIts)	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcrIts[nIdx])
			{
				if (pDoc->m_pPcrIts[nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcrIts)	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcrIts[nIdx])
			{
				if (pDoc->m_pPcrIts[nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
#else
	nStripIdx = 1;
#endif

	return nStripIdx;
}

int CGvisR2R_PunchView::GetMkStripIdx1Its(int nSerial, int nMkPcs) // 0 : Fail , 1~4 : Strip Idx
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.51"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->m_nRow;
	int nStripY = int(nNodeY / MAX_STRIP_NUM);
	int nStripIdx = 0;

#ifndef TEST_MODE
	int nDefPcsId = 0, nNum = 0, nMode = 0, nRow = 0;
	if (bDualTest)
	{
		if (pDoc->m_pPcrIts)	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcrIts[nIdx])
			{
				if (pDoc->m_pPcrIts[nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
	else
	{
		if (pDoc->m_pPcrIts)	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcrIts[nIdx])
			{
				if (pDoc->m_pPcrIts[nIdx]->m_pDefPcs)
				{
					nDefPcsId = pDoc->m_pPcrIts[nIdx]->m_pDefPcs[nMkPcs];
					nNum = int(nDefPcsId / nNodeY);
					nMode = nDefPcsId % nNodeY;
					if (nNum % 2) 	// 홀수.
						nRow = nNodeY - (nMode + 1);
					else		// 짝수.
						nRow = nMode;

					nStripIdx = int(nRow / nStripY) + 1;
				}
			}
		}
	}
#else
	nStripIdx = 1;
#endif

	return nStripIdx;
}

BOOL CGvisR2R_PunchView::SetMkIts(BOOL bRun)	// Marking Start
{
	CfPoint ptPnt;
	int nSerial, nTot, a, b;
	CString sItem;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	for (a = 0; a < 2; a++)
	{
		for (b = 0; b < MAX_STRIP_NUM; b++)
		{
			m_nMkStrip[a][b] = 0;			// [nCam][nStrip]:[2][4] - [좌/우][] : 스트립에 펀칭한 피스 수 count
			m_bRejectDone[a][b] = FALSE;	// [nCam][nStrip]:[2][4] - [좌/우][] : 스트립에 펀칭한 피스 수 count가 스트립 폐기 설정수 완료 여부 
			sItem.Format(_T("bRejectDone[%d][%d]"), a, b);
			pDoc->SetStatus(_T("General"), sItem, m_bRejectDone[a][b]);
		}
	}

	if (bRun)
	{
		if (m_bDoMk[0] && m_bMkSt[0])
		{
			if (!m_bTHREAD_MK[0])
			{
				m_nStepMk[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[0]"), m_nStepMk[0]);
				m_nMkPcs[0] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
				m_bDoneMk[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
				m_bTHREAD_MK[0] = TRUE; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);

				nSerial = m_nBufUpSerial[0]; // Cam0

				m_nTotMk[0] = nTot = GetTotDefPcs0Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nTotMk[0]"), m_nTotMk[0]);
				m_nCurMk[0] = 0;
				if (nTot > 0)
				{
					ptPnt = GetMkPnt0Its(nSerial, 0);
					m_dTarget[AXIS_X0] = ptPnt.x;
					m_dTarget[AXIS_Y0] = ptPnt.y;
					if (nTot > 1)
					{
						ptPnt = GetMkPnt0Its(nSerial, 1);
						m_dNextTarget[AXIS_X0] = ptPnt.x;
						m_dNextTarget[AXIS_Y0] = ptPnt.y;
					}
					else
					{
						m_dNextTarget[AXIS_X0] = -1.0;
						m_dNextTarget[AXIS_Y0] = -1.0;
					}
				}
				else
				{
					m_dTarget[AXIS_X0] = -1.0;
					m_dTarget[AXIS_Y0] = -1.0;
					m_dNextTarget[AXIS_X0] = -1.0;
					m_dNextTarget[AXIS_Y0] = -1.0;
				}
			}
		}
		else
		{
			m_bDoneMk[0] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[0]"), m_bDoneMk[0]);
		}

		if (m_bDoMk[1] && m_bMkSt[1])
		{
			if (!m_bTHREAD_MK[1])
			{
				m_nStepMk[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nStepMk[1]"), m_nStepMk[1]);
				m_nMkPcs[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
				m_bDoneMk[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
				m_bTHREAD_MK[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bTHREAD_MK[1]"), m_bTHREAD_MK[1]);

				nSerial = m_nBufUpSerial[1]; // Cam1

				m_nTotMk[1] = nTot = GetTotDefPcs1Its(nSerial); pDoc->SetStatusInt(_T("General"), _T("nTotMk[1]"), m_nTotMk[1]);
				m_nCurMk[1] = 0;
				if (nTot > 0)
				{
					ptPnt = GetMkPnt1Its(nSerial, 0);
					m_dTarget[AXIS_X1] = ptPnt.x;
					m_dTarget[AXIS_Y1] = ptPnt.y;
					if (nTot > 1)
					{
						ptPnt = GetMkPnt1Its(nSerial, 1);
						m_dNextTarget[AXIS_X1] = ptPnt.x;
						m_dNextTarget[AXIS_Y1] = ptPnt.y;
					}
					else
					{
						m_dNextTarget[AXIS_X1] = -1.0;
						m_dNextTarget[AXIS_Y1] = -1.0;
					}
				}
				else
				{
					m_dTarget[AXIS_X1] = -1.0;
					m_dTarget[AXIS_Y1] = -1.0;
					m_dNextTarget[AXIS_X1] = -1.0;
					m_dNextTarget[AXIS_Y1] = -1.0;
				}
			}
		}
		else
		{
			m_bDoneMk[1] = TRUE; pDoc->SetStatus(_T("General"), _T("bDoneMk[1]"), m_bDoneMk[1]);
		}
	}
	else
	{
		m_bTHREAD_MK[0] = FALSE; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[0]"), m_nMkPcs[0]);
		m_bTHREAD_MK[1] = FALSE; pDoc->SetStatusInt(_T("General"), _T("nMkPcs[1]"), m_nMkPcs[1]);
	}
	return TRUE;
}

BOOL CGvisR2R_PunchView::ReloadReelmapInner(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	BOOL bRtn[7] = { 0 };
	if (pDoc->m_pReelMapInner)
		bRtn[0] = pDoc->m_pReelMapInner->ReloadReelmap(nSerial);
	if (pDoc->m_pReelMapInnerUp)
		bRtn[1] = pDoc->m_pReelMapInnerUp->ReloadReelmap(nSerial);
	bRtn[3] = TRUE;

	if (pDoc->m_pReelMapIts)
		bRtn[2] = pDoc->m_pReelMapIts->ReloadReelmap(nSerial);

	if (bDualTest)
	{
		if (pDoc->m_pReelMapInnerDn)
			bRtn[4] = pDoc->m_pReelMapInnerDn->ReloadReelmap(nSerial);
		if (pDoc->m_pReelMapInnerAllUp)
			bRtn[5] = pDoc->m_pReelMapInnerAllUp->ReloadReelmap(nSerial);
		if (pDoc->m_pReelMapInnerAllDn)
			bRtn[6] = pDoc->m_pReelMapInnerAllDn->ReloadReelmap(nSerial);

		for (int i = 0; i < 7; i++)
		{
			if (!bRtn[i])
				return FALSE;
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			if (!bRtn[i])
				return FALSE;
		}
	}

	return TRUE;
}

void CGvisR2R_PunchView::OpenReelmapInner()
{
	//GetCurrentInfoEng();
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (pDoc->m_pReelMapInnerUp)
		pDoc->m_pReelMapInnerUp->Open();

	if (pDoc->m_pReelMapIts)
		pDoc->m_pReelMapIts->Open();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapInnerDn)
			pDoc->m_pReelMapInnerDn->Open();
		if (pDoc->m_pReelMapInnerAllUp)
			pDoc->m_pReelMapInnerAllUp->Open();
		if (pDoc->m_pReelMapInnerAllDn)
			pDoc->m_pReelMapInnerAllDn->Open();
	}
}

void CGvisR2R_PunchView::OpenReelmapInnerUp()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (pDoc->m_pReelMapInnerUp)
		pDoc->m_pReelMapInnerUp->Open();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapInnerAllUp)
			pDoc->m_pReelMapInnerAllUp->Open();
	}
}

void CGvisR2R_PunchView::OpenReelmapInnerDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	if (!bDualTest)
		return;

	if (pDoc->m_pReelMapInnerDn)
		pDoc->m_pReelMapInnerDn->Open();
	if (pDoc->m_pReelMapInnerAllDn)
		pDoc->m_pReelMapInnerAllDn->Open();
}


void CGvisR2R_PunchView::UpdateRstInner()
{
	if (m_pDlgMenu06)
		m_pDlgMenu06->UpdateRst();
}

void CGvisR2R_PunchView::InitReelmapInner()
{
	pDoc->InitReelmapInner();
	pDoc->SetReelmapInner(ROT_NONE);
}

void CGvisR2R_PunchView::InitReelmapInnerUp()
{
	pDoc->InitReelmapInnerUp();
	pDoc->SetReelmapInner(ROT_NONE);
}

void CGvisR2R_PunchView::InitReelmapInnerDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if (bDualTest)
	{
		pDoc->InitReelmapInnerDn();
		pDoc->SetReelmapInner(ROT_NONE);
	}
}


void CGvisR2R_PunchView::DispDefImgInner()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	CString sNewLot;
	int nSerial = 0;
	int nNewLot = 0;
	int nBreak = 0;

	switch (m_nStepTHREAD_DISP_DEF_INNER)
	{
		// CopyDefImg Start ============================================
	case 0:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 1:
		Sleep(300);
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 2:
		nSerial = m_nBufUpSerial[0]; // 좌측 Camera

		if (IsDoneDispMkInfoInner())	 // Check 불량이미지 Display End
		{
			if (pView->m_bSerialDecrese)
			{
				if (ChkLastProc() && (nSerial < m_nLotEndSerial))
				{
					SetSerialReelmapInner(nSerial, TRUE);	// Reelmap(좌) Display Start
					SetSerialMkInfoInner(nSerial, TRUE);		// 불량이미지(좌) Display Start
				}
				else if (ChkLastProc() && nSerial < 1)
				{
					SetSerialReelmapInner(m_nLotEndSerial - 1, TRUE);		// Reelmap(좌) Display Start
					SetSerialMkInfoInner(m_nLotEndSerial - 1, TRUE);		// 불량이미지(좌) Display Start
				}
				else
				{
					SetSerialReelmapInner(nSerial);	// Reelmap(좌) Display Start
					SetSerialMkInfoInner(nSerial);	// 불량이미지(좌) Display Start
				}
			}
			else
			{
				if (ChkLastProc() && (nSerial > m_nLotEndSerial))
				{
					SetSerialReelmapInner(nSerial, TRUE);	// Reelmap(좌) Display Start
					SetSerialMkInfoInner(nSerial, TRUE);		// 불량이미지(좌) Display Start
				}
				else if (ChkLastProc() && nSerial < 1)
				{
					SetSerialReelmapInner(m_nLotEndSerial + 1, TRUE);		// Reelmap(좌) Display Start
					SetSerialMkInfoInner(m_nLotEndSerial + 1, TRUE);		// 불량이미지(좌) Display Start
				}
				else
				{
					SetSerialReelmapInner(nSerial);	// Reelmap(좌) Display Start
					SetSerialMkInfoInner(nSerial);	// 불량이미지(좌) Display Start
				}
			}
			m_nStepTHREAD_DISP_DEF_INNER++;
		}

		break;

	case 3:
		if (IsDoneDispMkInfoInner())	 // Check 불량이미지(좌) Display End
		{
			nSerial = m_nBufUpSerial[1]; // 우측 Camera

			if (nSerial > 0)
			{
				if (pView->m_bSerialDecrese)
				{
					if (ChkLastProc() && (nSerial < m_nLotEndSerial))
						SetSerialMkInfoInner(nSerial, TRUE);	// 불량이미지(우) Display Start
					else
						SetSerialMkInfoInner(nSerial);		// 불량이미지(우) Display Start
				}
				else
				{
					if (ChkLastProc() && (nSerial > m_nLotEndSerial))
						SetSerialMkInfoInner(nSerial, TRUE);	// 불량이미지(우) Display Start
					else
						SetSerialMkInfoInner(nSerial);		// 불량이미지(우) Display Start
				}
			}
			else
			{
				if (pView->m_bSerialDecrese)
				{
					if (ChkLastProc())
						SetSerialMkInfoInner(m_nLotEndSerial - 1, TRUE);	// 불량이미지(우) Display Start
				}
				else
				{
					if (ChkLastProc())
						SetSerialMkInfoInner(m_nLotEndSerial + 1, TRUE);	// 불량이미지(우) Display Start
				}
			}
			m_nStepTHREAD_DISP_DEF_INNER++;
		}
		break;
	case 4:
		nSerial = m_nBufUpSerial[1]; // 우측 Camera

		if (nSerial > 0)
		{
			m_nStepTHREAD_DISP_DEF_INNER++;

			if (pView->m_bSerialDecrese)
			{
				if (ChkLastProc() && (nSerial < m_nLotEndSerial))
				{
					SetSerialReelmapInner(nSerial, TRUE);	// Reelmap(우) Display Start
				}
				else
				{
					SetSerialReelmapInner(nSerial);			// Reelmap(우) Display Start
				}
			}
			else
			{
				if (ChkLastProc() && (nSerial > m_nLotEndSerial))
				{
					SetSerialReelmapInner(nSerial, TRUE);	// Reelmap(우) Display Start
				}
				else
				{
					SetSerialReelmapInner(nSerial);			// Reelmap(우) Display Start
				}
			}
		}
		else
		{
			if (ChkLastProc())
			{
				m_nStepTHREAD_DISP_DEF_INNER++;
				if (pView->m_bSerialDecrese)
					SetSerialReelmapInner(m_nLotEndSerial - 1, TRUE);	// 불량이미지(우) Display Start
				else
					SetSerialReelmapInner(m_nLotEndSerial + 1, TRUE);	// 불량이미지(우) Display Start
			}
			else
			{
				if (bDualTest)
				{
					if (m_bLastProc && m_nBufDnSerial[0] == m_nLotEndSerial)
						m_nStepTHREAD_DISP_DEF_INNER++;
					else
					{
						m_nStepTHREAD_DISP_DEF_INNER++;
					}
				}
				else
				{
					if (m_bLastProc && m_nBufUpSerial[0] == m_nLotEndSerial)
						m_nStepTHREAD_DISP_DEF_INNER++;
					else
					{
						m_nStepTHREAD_DISP_DEF_INNER++;
					}
				}
			}
		}
		break;
	case 5:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 6:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 7:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 8:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 9:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 10:
		m_nStepTHREAD_DISP_DEF_INNER++;
		break;

	case 11:
		if (IsDoneDispMkInfoInner() && IsRun())	 // Check 불량이미지(우) Display End
			m_nStepTHREAD_DISP_DEF_INNER++;
		break;
	case 12:
		m_bTHREAD_DISP_DEF_INNER = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_DISP_DEF_INNER"), m_bTHREAD_DISP_DEF_INNER);
		break;
		// Disp DefImg End ============================================
	}

}

BOOL CGvisR2R_PunchView::IsDoneDispMkInfoInner()
{
	BOOL bRtn = FALSE;
	if (m_pDlgMenu06)
		bRtn = m_pDlgMenu06->IsDoneDispMkInfo();
	return bRtn;
}

BOOL CGvisR2R_PunchView::SetSerialReelmapInner(int nSerial, BOOL bDumy)
{
	if (!m_pDlgMenu06)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Error - SetSerialReelmap : m_pDlgMenu06 is NULL."));
		return FALSE;
	}

	return m_pDlgMenu06->SetSerialReelmap(nSerial, bDumy);
}

BOOL CGvisR2R_PunchView::SetSerialMkInfoInner(int nSerial, BOOL bDumy)
{
	if (!m_pDlgMenu06)
		return FALSE;
	return m_pDlgMenu06->SetSerialMkInfo(nSerial, bDumy);
}

CString CGvisR2R_PunchView::GetTimeIts()
{
	stLotTime ItsTime;

	CString strVal;
	time_t osBinTime;			// C run-time time (defined in <time.h>)
	time(&osBinTime);		// Get the current time from the 
							// operating system.
	CTime Tim(osBinTime);

	ItsTime.nYear = Tim.GetYear();
	ItsTime.nMonth = Tim.GetMonth();
	ItsTime.nDay = Tim.GetDay();
	ItsTime.nHour = Tim.GetHour();
	ItsTime.nMin = Tim.GetMinute();
	ItsTime.nSec = Tim.GetSecond();

	strVal.Format(_T("%04d%02d%02d%02d%02d%02d"),
		ItsTime.nYear, ItsTime.nMonth, ItsTime.nDay,
		ItsTime.nHour, ItsTime.nMin, ItsTime.nSec);

	return strVal;
}

void CGvisR2R_PunchView::MakeItsFileUp(int nSerial)
{
	if (pDoc->GetTestMode() == MODE_INNER)
	{
		if (pDoc->m_pReelMapUp)
			pDoc->m_pReelMapUp->MakeItsFile(nSerial, RMAP_INNER_UP);
	}
	
	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->m_pReelMapUp)
			pDoc->m_pReelMapUp->MakeItsFile(nSerial, RMAP_UP);
	}
}

void CGvisR2R_PunchView::MakeItsFileDn(int nSerial)
{
	if (pDoc->GetTestMode() == MODE_INNER)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->MakeItsFile(nSerial, RMAP_INNER_DN);
	}
	
	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->MakeItsFile(nSerial, RMAP_DN);
	}
}

void CGvisR2R_PunchView::UpdateRMapInnerUp()
{
	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->WorkingInfo.System.bRemakeReelmapInner)
		{
			if (pDoc->m_pReelMapInnerUp)
				pDoc->m_pReelMapInnerUp->Write(m_nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		}
		else
		{
			if (pDoc->m_pReelMapInnerUp)
				pDoc->m_pReelMapInnerUp->Read(m_nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		}
	}
}

void CGvisR2R_PunchView::UpdateRMapInnerDn()
{
	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->WorkingInfo.System.bRemakeReelmapInner)
		{
			if (pDoc->m_pReelMapInnerDn)
				pDoc->m_pReelMapInnerDn->Write(m_nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		}
		else
		{
			if (pDoc->m_pReelMapInnerDn)
				pDoc->m_pReelMapInnerDn->Read(m_nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		}
	}
}

void CGvisR2R_PunchView::UpdateRMapInnerAllUp()
{
	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->WorkingInfo.System.bRemakeReelmapInner)
		{
			if (pDoc->m_pReelMapInnerAllUp)
				pDoc->m_pReelMapInnerAllUp->Write(m_nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		}
		else
		{
			if (pDoc->m_pReelMapInnerAllUp)
				pDoc->m_pReelMapInnerAllUp->Read(m_nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		}
	}
}

void CGvisR2R_PunchView::UpdateRMapInnerAllDn()
{
	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->WorkingInfo.System.bRemakeReelmapInner)
		{
			if (pDoc->m_pReelMapInnerAllDn)
				pDoc->m_pReelMapInnerAllDn->Write(m_nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		}
		else
		{
			if (pDoc->m_pReelMapInnerAllDn)
				pDoc->m_pReelMapInnerAllDn->Read(m_nSerialRmapInnerUpdate); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		}
	}
}

UINT CGvisR2R_PunchView::ThreadProc26(LPVOID lpContext)	// UpdateRMapInnerUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[26] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[26].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[26] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_INNER_UP) // MakeReelmapInnerFile
		{
			pThread->UpdateRMapInnerUp();
			pThread->m_bTHREAD_UPDATE_REELMAP_INNER_UP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_UP"), pThread->m_bTHREAD_UPDATE_REELMAP_INNER_UP);
			Sleep(0);
		}

		Sleep(30);
	}

	pThread->m_bThread[26] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc27(LPVOID lpContext)	// UpdateRMapInnerDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[27] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[27].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[27] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_INNER_DN) // MakeReelmapInnerFile
		{
			pThread->UpdateRMapInnerDn();
			pThread->m_bTHREAD_UPDATE_REELMAP_INNER_DN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_DN"), pThread->m_bTHREAD_UPDATE_REELMAP_INNER_DN);
			Sleep(0);
		}

		Sleep(30);
	}

	pThread->m_bThread[27] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc28(LPVOID lpContext)	// UpdateRMapAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[28] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[28].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[28] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP) // Write Reelmap
		{
			pThread->UpdateRMapInnerAllUp();
			pThread->m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_ALLUP"), pThread->m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP);
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[28] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc29(LPVOID lpContext)	// UpdateRMapInnerAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[29] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[29].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[29] = GetTickCount() - dwTick;
		dwTick = GetTickCount();
		
		if (pThread->m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN) // Write Reelmap
		{
			pThread->UpdateRMapInnerAllDn();
			pThread->m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_ALLDN"), pThread->m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN);
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[29] = FALSE;

	return 0;
}


void CGvisR2R_PunchView::MakeResultMDS()
{
	CString sPath, strMsg;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	pDoc->WorkingInfo.LastJob.sProcessNum = pDoc->GetProcessNum();
	pDoc->UpdateProcessNum(pDoc->WorkingInfo.LastJob.sProcessNum);

	// for SAPP3
	GetResult();		// Load data From Reelmap.txt
	MakeResult();		// Result.txt
	MakeResultIts();	// Result.txt
	MakeSapp3();		// GetSapp3Txt()

	RemakeReelmap();	// MDS(해성DS) Style의 릴맵으로 재생성

	if (pDoc->GetTestMode() == MODE_OUTER)
		RemakeReelmapInner();
}

BOOL CGvisR2R_PunchView::RemakeReelmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sReelmapSrc, str;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->StartThreadRemakeReelmap();

	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->StartThreadRemakeReelmap();

		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->StartThreadRemakeReelmap();
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::IsDoneRemakeReelmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bSuccess[3] = { FALSE };
	DWORD dwSt = GetTickCount();

	Sleep(100);

	do
	{
		if (GetTickCount() - dwSt > 600000)
			break;
	} while (!pDoc->m_pReelMapUp->m_bThreadAliveRemakeReelmap && !pDoc->m_pReelMapDn->m_bThreadAliveRemakeReelmap && !pDoc->m_pReelMapAllUp->m_bThreadAliveRemakeReelmap);

	if (bDualTest)
	{
		bSuccess[0] = pDoc->m_pReelMapUp->m_bRtnThreadRemakeReelmap;
		bSuccess[1] = pDoc->m_pReelMapDn->m_bRtnThreadRemakeReelmap;
		bSuccess[2] = pDoc->m_pReelMapAllUp->m_bRtnThreadRemakeReelmap;

		if (!bSuccess[0] || !bSuccess[2] || !bSuccess[1])
		{
			MsgBox(_T("ReelMap Converting Failed."));
			return FALSE;
		}
	}
	else
	{
		if (!pDoc->m_pReelMapUp->m_bRtnThreadRemakeReelmap)
		{
			MsgBox(_T("ReelMap Converting Failed."));
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::RemakeReelmapInner()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sReelmapSrc, str;

	if (pDoc->m_pReelMapIts)
		pDoc->m_pReelMapIts->StartThreadRemakeReelmap();

	return TRUE;
}

BOOL CGvisR2R_PunchView::IsDoneRemakeReelmapInner()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	BOOL bAlive[4] = { FALSE };
	BOOL bSuccess[4] = { 1,1,1,1 };
	BOOL bWait = TRUE;
	DWORD dwSt = GetTickCount();

	Sleep(100);

	do
	{
		if (GetTickCount() - dwSt > 600000)
			break;

		bAlive[0] = pDoc->m_pReelMapInnerUp->m_bThreadAliveRemakeReelmap;
		bAlive[1] = pDoc->m_pReelMapInnerDn->m_bThreadAliveRemakeReelmap;
		bAlive[2] = pDoc->m_pReelMapInnerAllUp->m_bThreadAliveRemakeReelmap;
		bAlive[3] = pDoc->m_pReelMapIts->m_bThreadAliveRemakeReelmap;

		if (bAlive[0] || bAlive[1] || bAlive[2] || bAlive[3])
			bWait = TRUE;
		else
			bWait = FALSE;

	} while (bWait);

	if (bDualTest)
	{
		bSuccess[0] = pDoc->m_pReelMapInnerUp->m_bRtnThreadRemakeReelmap;
		bSuccess[1] = pDoc->m_pReelMapInnerDn->m_bRtnThreadRemakeReelmap;
		bSuccess[2] = pDoc->m_pReelMapInnerAllUp->m_bRtnThreadRemakeReelmap;
		bSuccess[3] = pDoc->m_pReelMapIts->m_bRtnThreadRemakeReelmap;

		if (!bSuccess[0] || !bSuccess[1] || !bSuccess[2] || !bSuccess[3])
		{
			MsgBox(_T("ReelMap Converting Failed."));
			return FALSE;
		}
	}
	else
	{
		bSuccess[0] = pDoc->m_pReelMapInnerUp->m_bRtnThreadRemakeReelmap;
		bSuccess[1] = pDoc->m_pReelMapIts->m_bRtnThreadRemakeReelmap;

		if (!bSuccess[0] || !bSuccess[1])
		{
			MsgBox(_T("ReelMap Converting Failed."));
			return FALSE;
		}
	}

	return TRUE;
}

void CGvisR2R_PunchView::GetResult()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->GetResult();
	}
	else
	{
		if (pDoc->m_pReelMapUp)
			pDoc->m_pReelMapUp->GetResult();
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->m_pReelMapIts)
			pDoc->m_pReelMapIts->GetResult();
	}
}

void CGvisR2R_PunchView::MakeResult()
{	
	// TODO: Add your control notification handler code here
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	// File Save......

	CString strDestPath;
	strDestPath.Format(_T("%s%s\\%s\\%s.txt"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sLot,
		_T("Result"));

	CFileFind cFile;
	if (cFile.FindFile(strDestPath))
		DeleteFile(strDestPath);

	//////////////////////////////////////////////////////////
	// Directory location of Work file
	CString strData;
	if (bDualTest)
	{
		if (pDoc->m_pReelMapAllUp)
			strData = pDoc->m_pReelMapAllUp->GetResultTxt();
	}
	else
	{
		if (pDoc->m_pReelMapUp)
			strData = pDoc->m_pReelMapUp->GetResultTxt();
	}


	TCHAR lpszCurDirPathFile[MAX_PATH];
	_stprintf(lpszCurDirPathFile, _T("%s"), strDestPath);

	CFile file;
	CFileException pError;
	if (!file.Open(lpszCurDirPathFile, CFile::modeWrite, &pError))
	{
		if (!file.Open(lpszCurDirPathFile, CFile::modeCreate | CFile::modeWrite, &pError))
		{
			// 파일 오픈에 실패시 
#ifdef _DEBUG
			afxDump << _T("File could not be opened ") << pError.m_cause << _T("\n");
#endif
			return;
		}
	}
	//버퍼의 내용을 file에 복사한다.
	char* pRtn = NULL;
	file.SeekToBegin();
	file.Write(pRtn = StringToChar(strData), strData.GetLength());
	file.Close();
	if (pRtn)
		delete pRtn;
}

void CGvisR2R_PunchView::MakeResultIts()
{
	// TODO: Add your control notification handler code here
	if (pDoc->GetTestMode() != MODE_OUTER)
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;

	// File Save......

	CString strDestPath;
	strDestPath.Format(_T("%s%s\\%s\\%s.txt"), pDoc->WorkingInfo.System.sPathItsFile,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sEngItsCode,
		_T("Result"));

	CFileFind cFile;
	if (cFile.FindFile(strDestPath))
		DeleteFile(strDestPath);

	//////////////////////////////////////////////////////////
	// Directory location of Work file
	CString strData;
	if (pDoc->m_pReelMapIts)
		strData = pDoc->m_pReelMapIts->GetResultTxt();


	TCHAR lpszCurDirPathFile[MAX_PATH];
	_stprintf(lpszCurDirPathFile, _T("%s"), strDestPath);

	CFile file;
	CFileException pError;
	if (!file.Open(lpszCurDirPathFile, CFile::modeWrite, &pError))
	{
		if (!file.Open(lpszCurDirPathFile, CFile::modeCreate | CFile::modeWrite, &pError))
		{
			// 파일 오픈에 실패시 
#ifdef _DEBUG
			afxDump << _T("File could not be opened ") << pError.m_cause << _T("\n");
#endif
			return;
		}
	}
	//버퍼의 내용을 file에 복사한다.
	char* pRtn = NULL;
	file.SeekToBegin();
	file.Write(pRtn = StringToChar(strData), strData.GetLength());
	file.Close();
	if (pRtn)
		delete pRtn;
}

void CGvisR2R_PunchView::MakeSapp3()
{
	if (pDoc->WorkingInfo.LastJob.sProcessNum.IsEmpty() || pDoc->WorkingInfo.System.sPathSapp3.IsEmpty())
		return;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	FILE *fp = NULL;
	TCHAR FileName[MAX_PATH];
	CString sPath, sVal, strMsg, sSapp3Data;

	if (bDualTest)
	{
		if (pDoc->m_pReelMapAllUp)
			sSapp3Data = pDoc->m_pReelMapAllUp->GetSapp3Txt();
	}
	else
	{
		if (pDoc->m_pReelMapUp)
			sSapp3Data = pDoc->m_pReelMapUp->GetSapp3Txt();
	}

	sPath.Format(_T("%s%9s_%4s_%5s.txt"), pDoc->WorkingInfo.System.sPathSapp3,
		pDoc->WorkingInfo.LastJob.sLot,
		pDoc->WorkingInfo.LastJob.sProcessNum,
		pDoc->WorkingInfo.System.sMcName);

	_stprintf(FileName, _T("%s"), sPath);
	char* pRtn = NULL;
	fp = fopen(pRtn = TCHARToChar(FileName), "w+");
	if (pRtn) delete pRtn;
	pRtn = NULL;

	if (fp != NULL)
	{
		fprintf(fp, "%s\n", pRtn = StringToChar(sSapp3Data));
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		strMsg.Format(_T("It is trouble to open file.\r\n%s"), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(strMsg, MB_ICONWARNING | MB_OK);
	}

	if (pRtn)
		delete pRtn;
	fclose(fp);
}

BOOL CGvisR2R_PunchView::UpdateReelmap(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.54"));
		return 0;
	}

	if (!pDoc->MakeMkDir())
		return FALSE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	m_nSerialRmapUpdate = nSerial; pDoc->SetStatusInt(_T("Thread"), _T("nSerialRmapUpdate"), m_nSerialRmapUpdate);

	m_bTHREAD_UPDATE_REELMAP_UP = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_UP"), m_bTHREAD_UPDATE_REELMAP_UP);
	if (bDualTest)
	{
		m_bTHREAD_UPDATE_REELMAP_DN = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_DN"), m_bTHREAD_UPDATE_REELMAP_DN);
		m_bTHREAD_UPDATE_REELMAP_ALLUP = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_ALLUP"), m_bTHREAD_UPDATE_REELMAP_ALLUP);
		m_bTHREAD_UPDATE_REELMAP_ALLDN = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_ALLDN"), m_bTHREAD_UPDATE_REELMAP_ALLDN);
	}

	Sleep(100);
	return TRUE;
}

BOOL CGvisR2R_PunchView::UpdateReelmapInner(int nSerial)
{
	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (nSerial <= 0)
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("Serial Error.54"));
			return FALSE;
		}

		if (!pDoc->MakeMkDir())
			return FALSE;

		BOOL bDualTestInner = pDoc->WorkingInfo.LastJob.bDualTestInner;
		m_nSerialRmapInnerUpdate = nSerial; pDoc->SetStatusInt(_T("Thread"), _T("nSerialRmapInnerUpdate"), m_nSerialRmapInnerUpdate);
		m_bTHREAD_UPDATE_REELMAP_INNER_UP = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_UP"), m_bTHREAD_UPDATE_REELMAP_INNER_UP);

		if (bDualTestInner)
		{
			m_bTHREAD_UPDATE_REELMAP_INNER_DN = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_DN"), m_bTHREAD_UPDATE_REELMAP_INNER_DN);
			m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_ALLUP"), m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP);
			m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_ALLDN"), m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN);
		}
		m_bTHREAD_UPDATE_REELMAP_ITS = TRUE;
	}

	Sleep(100);
	return TRUE;
}

BOOL CGvisR2R_PunchView::MakeItsFile(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.54"));
		return FALSE;
	}

	if (!pDoc->MakeMkDir())
		return FALSE;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	m_nSerialMakeItsFile = nSerial;

	m_bTHREAD_MAKE_ITS_FILE_UP = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MAKE_ITS_FILE_UP"), m_bTHREAD_MAKE_ITS_FILE_UP);
	if (bDualTest)
	{
		m_bTHREAD_MAKE_ITS_FILE_DN = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MAKE_ITS_FILE_DN"), m_bTHREAD_MAKE_ITS_FILE_DN);
	}

	Sleep(100);
	return TRUE;
}

void CGvisR2R_PunchView::Shift2Mk()
{
	int nSerialL = m_nBufUpSerial[0];
	int nSerialR = m_nBufUpSerial[1];
	int nLastShot = 0;

	if (m_bLastProc && nSerialL == m_nLotEndSerial)
	{
		if (nSerialL > 0)
			pDoc->Shift2Mk(nSerialL);	// Cam0

		if (m_pDlgFrameHigh)
			m_pDlgFrameHigh->SetMkLastShot(nSerialL);
	}
	else
	{
		if (nSerialL > 0)
		{
			pDoc->Shift2Mk(nSerialL);	// Cam0
			nLastShot = nSerialL;

			if (nSerialR > 0)
			{
				pDoc->Shift2Mk(nSerialR);	// Cam1
				nLastShot = nSerialR;
			}

			if (m_pDlgFrameHigh)
				m_pDlgFrameHigh->SetMkLastShot(nLastShot);
		}
	}

	m_bShift2Mk = FALSE;
}

void CGvisR2R_PunchView::DelOverLotEndSerialUp(int nSerial)
{
	CString sSrc;

	if (nSerial > 0)
	{
		sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufUp, nSerial);

		if (pView->m_bSerialDecrese)
		{
			if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
			{
				// Delete PCR File
				pDoc->m_pFile->DeleteFolerOrFile(sSrc);
			}
		}
		else
		{
			if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
			{
				// Delete PCR File
				pDoc->m_pFile->DeleteFolerOrFile(sSrc);
			}
		}
	}

}


void CGvisR2R_PunchView::DelOverLotEndSerialDn(int nSerial)
{
	CString sSrc;

	if (nSerial > 0)
	{
		sSrc.Format(_T("%s%04d.pcr"), pDoc->WorkingInfo.System.sPathVrsBufDn, nSerial);

		if (pView->m_bSerialDecrese)
		{
			if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
			{
				// Delete PCR File
				pDoc->m_pFile->DeleteFolerOrFile(sSrc);
			}
		}
		else
		{
			if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
			{
				// Delete PCR File
				pDoc->m_pFile->DeleteFolerOrFile(sSrc);
			}
		}
	}

}



void CGvisR2R_PunchView::ChkReTestAlarmOnAoiUp()
{
	CString sMsg;
	sMsg.Format(_T("U%03d"), GetAoiUpAutoSerial());
	DispStsBar(sMsg, 0);

	int nSerial = m_pBufSerial[0][m_nBufTot[0] - 1];

	if (pView->m_bSerialDecrese)
	{
		if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
		{
			SetAoiUpAutoStep(2); // Wait for AOI 검사시작 신호.
			Sleep(300);
				MpeWrite(Plc.DlgMenu01.ReTestAlarmAoiUp, 1); // 검사부 상부 재작업 (시작신호) : PC가 On시키고 PLC가 Off : PLC가 처음부터 다시 시작
		}
		else if(m_nLotEndSerial > 0 && nSerial <= m_nLotEndSerial)
		{
			MpeWrite(Plc.DlgMenu03.PcrReceivedAoiUp, 1); // AOI 상 : PCR파일 Received
		}
	}
	else
	{
		if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
		{
			SetAoiUpAutoStep(2); // Wait for AOI 검사시작 신호.
			Sleep(300);
				MpeWrite(Plc.DlgMenu01.ReTestAlarmAoiUp, 1); // 검사부 상부 재작업 (시작신호) : PC가 On시키고 PLC가 Off : PLC가 처음부터 다시 시작
		}
		else if (m_nLotEndSerial > 0 && nSerial >= m_nLotEndSerial)
		{
			MpeWrite(Plc.DlgMenu03.PcrReceivedAoiUp, 1); // AOI 상 : PCR파일 Received
		}
	}

}


void CGvisR2R_PunchView::ChkReTestAlarmOnAoiDn()
{
	CString sMsg;
	sMsg.Format(_T("D%03d"), GetAoiDnAutoSerial());
	DispStsBar(sMsg, 0);

	int nSerial = m_pBufSerial[1][m_nBufTot[1] - 1];

	if (pView->m_bSerialDecrese)
	{
		if (m_nLotEndSerial > 0 && nSerial > m_nLotEndSerial)
		{
			SetAoiDnAutoStep(2); // Wait for AOI 검사시작 신호.
			Sleep(300);
			MpeWrite(Plc.DlgMenu01.ReTestAlarmAoiDn, 1); // 검사부 하부 재작업 (시작신호) : PC가 On시키고 PLC가 Off : PLC가 처음부터 다시 시작
		}
	}
	else
	{
		if (m_nLotEndSerial > 0 && nSerial < m_nLotEndSerial)
		{
			SetAoiDnAutoStep(2); // Wait for AOI 검사시작 신호.
			Sleep(300);
			MpeWrite(Plc.DlgMenu01.ReTestAlarmAoiDn, 1); // 검사부 하부 재작업 (시작신호) : PC가 On시키고 PLC가 Off : PLC가 처음부터 다시 시작
		}
	}

}

void CGvisR2R_PunchView::ShowMsgBox()
{
	if (m_pDlgMyMsg)
	{
		m_pDlgMyMsg->ShowWindow(SW_SHOW);
		m_pDlgMyMsg->SetForegroundWindow();
		m_pDlgMyMsg->RedrawWindow();
	}
}

BOOL CGvisR2R_PunchView::FinalCopyItsFiles()
{
	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->StartThreadFinalCopyItsFiles();

	return TRUE;
}

int CGvisR2R_PunchView::GetAoiUpCamMstInfo()
{
	return pDoc->GetAoiUpCamMstInfo();
}

int CGvisR2R_PunchView::GetAoiDnCamMstInfo()
{
	return pDoc->GetAoiDnCamMstInfo();
}

void CGvisR2R_PunchView::DuplicateRmap(int nRmap)
{
	if (!pDoc->WorkingInfo.System.bDuplicateRmap)
		return;

	CFileFind finder;
	CString strTemp, sSrcPath, sDstPath;
	sDstPath = _T("C:\\PunchWork\\Reelmap.txt");

	switch (nRmap)
	{
	case RMAP_UP:
		sSrcPath = pDoc->m_pReelMapUp->GetRmapPath(RMAP_UP);
		break;
	case RMAP_ALLUP:
		sSrcPath = pDoc->m_pReelMapAllUp->GetRmapPath(RMAP_ALLUP);
		break;
	case RMAP_ITS:
		sSrcPath = pDoc->m_pReelMapUp->GetRmapPath(RMAP_ITS);
		break;
	}

	while (m_nStepTHREAD_DISP_DEF > 7 && m_bTHREAD_DISP_DEF)
	{
		Sleep(30);
	}

	if (finder.FindFile(sSrcPath))
	{
		if (!CopyFile((LPCTSTR)sSrcPath, (LPCTSTR)sDstPath, FALSE))
		{
			Sleep(100);
			if (!CopyFile((LPCTSTR)sSrcPath, (LPCTSTR)sDstPath, FALSE))
			{
				Sleep(300);
				if (!CopyFile((LPCTSTR)sSrcPath, (LPCTSTR)sDstPath, FALSE))
				{
					Sleep(500);
					if (!CopyFile((LPCTSTR)sSrcPath, (LPCTSTR)sDstPath, FALSE))
					{
						return;
					}
				}
			}					
		}
	}
	else
	{
		Sleep(30);
		strTemp.Format(_T("%s \r\n: Reelmap File Not Exist"), sSrcPath);
		MsgBox(strTemp);
		return;
	}

}

int CGvisR2R_PunchView::GetMkStAuto()
{
	int nStep = 0;
	TCHAR szData[512];

	CString strFolder, strTemp;
	strFolder.Format(_T("%s\\AutoStep.ini"), pDoc->WorkingInfo.System.sPathMkWork);

	if (0 < ::GetPrivateProfileString(_T("Auto"), _T("nStep"), NULL, szData, sizeof(szData), strFolder))
		nStep = _ttoi(szData);
	else
		nStep = 0;

	m_nMkStAuto = nStep; pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);

	return nStep;
}

void CGvisR2R_PunchView::SetMkStAuto()
{
	CString strFolder, strTemp;
	strFolder.Format(_T("%s\\AutoStep.ini"), pDoc->WorkingInfo.System.sPathMkWork);
	strTemp.Format(_T("%d"), m_nMkStAuto);
	::WritePrivateProfileString(_T("Auto"), _T("nStep"), strTemp, strFolder);
}

void CGvisR2R_PunchView::LoadSerial()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	SetListBuf();

	if (bDualTest)
	{
		if (m_nMkStAuto > MK_ST + (Mk2PtIdx::Start) && m_nMkStAuto < MK_ST + (Mk2PtIdx::DoneMk) + 3)
		{
			if (pDoc->m_ListBuf[1].nTot > 0) // AOI-Dn
			{
				m_nBufDnSerial[0] = pDoc->m_ListBuf[1].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[0]"), m_nBufDnSerial[0]);
				m_nBufUpSerial[0] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[0]"), m_nBufUpSerial[0]);
				if (pDoc->m_ListBuf[1].nTot > 0) // AOI-Dn
				{
					m_nBufDnSerial[1] = pDoc->m_ListBuf[1].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[1]"), m_nBufDnSerial[1]);
					m_nBufUpSerial[1] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
				}
				else
				{
					m_nBufDnSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufDnSerial[1]"), m_nBufDnSerial[1]);
					m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
				}

				if (m_nBufUpSerial[0] == m_nBufUpSerial[1])
				{
					Stop();
					pView->ClrDispMsg();
					MsgBox(_T("좌/우 마킹 시리얼이 같습니다."));
					SetListBuf();
					m_nMkStAuto = MK_ST + (Mk2PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
				}

				if (bDualTest)
				{
					if (m_nBufUpSerial[0] != m_nBufDnSerial[0])
					{
						Stop();
						pView->ClrDispMsg();
						MsgBox(_T("좌측 상/하면 시리얼이 다릅니다."));
						SetListBuf();
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}

					if (m_nBufUpSerial[1] != m_nBufDnSerial[1])
					{
						Stop();
						pView->ClrDispMsg();
						MsgBox(_T("우측 상/하면 시리얼이 다릅니다."));
						SetListBuf();
						m_nMkStAuto = MK_ST + (Mk2PtIdx::Start); pDoc->SetStatusInt(_T("General"), _T("nMkStAuto"), pView->m_nMkStAuto);
					}
				}

			}
		}

		if (pDoc->WorkingInfo.LastJob.bSampleTest)
		{
			if (m_nBufUpSerial[0] == 1)
			{
				m_nLotEndSerial = _tstoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum); pDoc->SetStatusInt(_T("General"), _T("nLotEndSerial"), m_nLotEndSerial);
				m_bLastProcFromUp = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProcFromUp"), m_bLastProcFromUp);
				m_bLastProcFromEng = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
				m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
				if (m_pDlgMenu01)
					m_pDlgMenu01->m_bLastProc = TRUE;
				//MpeWrite(Plc.DlgMenu01.LastJobFromAoiDn, 1);			// 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
				MpeWrite(Plc.DlgMenu01.DoLastJob, 1);			// 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
			}
		}
	}
	else
	{
		if (pDoc->m_ListBuf[0].nTot > 0) // AOI-Up
		{
			m_nBufUpSerial[0] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[0]"), m_nBufUpSerial[0]);
			if (pDoc->m_ListBuf[0].nTot > 0) // AOI-Up
			{
				m_nBufUpSerial[1] = pDoc->m_ListBuf[0].Pop(); pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
			}
			else
			{
				m_nBufUpSerial[1] = 0; pDoc->SetStatusInt(_T("General"), _T("nBufUpSerial[1]"), m_nBufUpSerial[1]);
			}
		}

		if (pDoc->WorkingInfo.LastJob.bSampleTest)
		{
			if (m_nBufUpSerial[0] == 1)
			{
				m_nLotEndSerial = _tstoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum); pDoc->SetStatusInt(_T("General"), _T("nLotEndSerial"), m_nLotEndSerial);
				m_bLastProcFromUp = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProcFromUp"), m_bLastProcFromUp);
				m_bLastProcFromEng = FALSE; pDoc->SetStatus(_T("General"), _T("bLastProcFromEng"), m_bLastProcFromEng);
				m_bLastProc = TRUE; pDoc->SetStatus(_T("General"), _T("bLastProc"), m_bLastProc);
				if (m_pDlgMenu01)
					m_pDlgMenu01->m_bLastProc = TRUE;
				//MpeWrite(Plc.DlgMenu01.LastJobFromAoiDn, 1);			// 잔량처리 AOI(하) 부터(PC가 On시키고, PLC가 확인하고 Off시킴)-20141112
				MpeWrite(Plc.DlgMenu01.DoLastJob, 1);			// 잔량처리(PC가 On시키고, PLC가 확인하고 Off시킴)-20141031
			}
		}
	}

	SetListBuf();
}


UINT CGvisR2R_PunchView::ThreadProc30(LPVOID lpContext)	// UpdateYieldUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[30] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[30].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[30] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_UP)
		{
			pThread->UpdateYieldUp(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_UP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_UP"), pThread->m_bTHREAD_UPDATE_YIELD_UP);
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[30] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc31(LPVOID lpContext)	// UpdateYieldDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[31] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[31].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[31] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_DN)
		{
			pThread->UpdateYieldDn(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_DN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_DN"), pThread->m_bTHREAD_UPDATE_YIELD_DN);
			pThread->m_bTHREAD_UPDATE_YIELD_ALLUP = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_ALLUP"), pThread->m_bTHREAD_UPDATE_YIELD_ALLUP);
			pThread->m_bTHREAD_UPDATE_YIELD_ALLDN = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_ALLDN"), pThread->m_bTHREAD_UPDATE_YIELD_ALLDN);
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[31] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc32(LPVOID lpContext)	// UpdateYieldAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[32] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[32].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[32] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_ALLUP && !pThread->m_bTHREAD_UPDATE_YIELD_UP)
		{
			pThread->UpdateYieldAllUp(pThread->m_nSnTHREAD_UPDATAE_YIELD);

			if (pDoc->GetTestMode() == MODE_OUTER)
			{
				pThread->m_nSnTHREAD_UPDATAE_YIELD_ITS = pThread->m_nSnTHREAD_UPDATAE_YIELD;
				pThread->m_bTHREAD_UPDATE_YIELD_ITS = TRUE;
			}
			pThread->m_bTHREAD_UPDATE_YIELD_ALLUP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_ALLUP"), pThread->m_bTHREAD_UPDATE_YIELD_ALLUP);

			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[32] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc33(LPVOID lpContext)	// UpdateYieldAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[33] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[33].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[33] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_ALLDN && !pThread->m_bTHREAD_UPDATE_YIELD_UP)
		{
			pThread->UpdateYieldAllDn(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_ALLDN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_ALLDN"), pThread->m_bTHREAD_UPDATE_YIELD_ALLDN);
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[33] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc34(LPVOID lpContext)	// UpdateYieldInnerUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[34] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[34].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[34] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_INNER_UP)
		{
			pThread->UpdateYieldInnerUp(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_UP = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[34] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc35(LPVOID lpContext)	// UpdateYieldInnerDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[35] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[35].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[35] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_INNER_DN)
		{
			pThread->UpdateYieldInnerDn(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_DN = FALSE;
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLUP = TRUE;
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLDN = TRUE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[35] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc36(LPVOID lpContext)	// UpdateYieldInnerAllUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[36] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[36].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[36] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLUP)
		{
			pThread->UpdateYieldInnerAllUp(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLUP = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[36] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc37(LPVOID lpContext)	// UpdateYieldInnerAllDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[37] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[37].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[37] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLDN)
		{
			pThread->UpdateYieldInnerAllDn(pThread->m_nSnTHREAD_UPDATAE_YIELD);
			pThread->m_bTHREAD_UPDATE_YIELD_INNER_ALLDN = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[37] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc38(LPVOID lpContext)	// UpdateYieldIts()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[38] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[38].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[38] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_YIELD_ITS)
		{
			pThread->UpdateYieldIts(pThread->m_nSnTHREAD_UPDATAE_YIELD_ITS);
			pThread->m_bTHREAD_UPDATE_YIELD_ITS = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[38] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc39(LPVOID lpContext)	// MakeItsFileUp()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[39] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[39].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[39] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_MAKE_ITS_FILE_UP) // MakeItsFile
		{
			pThread->MakeItsFileUp(pThread->m_nSerialMakeItsFile);
			pThread->m_bTHREAD_MAKE_ITS_FILE_UP = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MAKE_ITS_FILE_UP"), pThread->m_bTHREAD_MAKE_ITS_FILE_UP);
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[39] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc40(LPVOID lpContext)	// MakeItsFileDn()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[40] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[39].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[40] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_MAKE_ITS_FILE_DN) // MakeItsFile
		{
			pThread->MakeItsFileDn(pThread->m_nSerialMakeItsFile);
			pThread->m_bTHREAD_MAKE_ITS_FILE_DN = FALSE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_MAKE_ITS_FILE_DN"), pThread->m_bTHREAD_MAKE_ITS_FILE_DN);
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[40] = FALSE;

	return 0;
}

void CGvisR2R_PunchView::ChkRcvSig()
{
	if (!m_pEngrave) return;

	long lData = 0;
	int nMsgID = 0;

	for (nMsgID = 0; nMsgID < _SigInx::_EndIdx; nMsgID++)
	{
		GetCurrentInfoSignal();

		if (m_pEngrave->m_bRcvSig[nMsgID])
		{
			m_pEngrave->m_bRcvSig[nMsgID] = FALSE;
			switch (nMsgID)
			{
			case _SigInx::_Ready:
					MpeWrite(Plc.DlgMenu03.LampStop, (long)0); // 마킹부 정지 스위치 램프 ON(PC가 On, PLC가 확인하고 Off시킴)
					break;
			case _SigInx::_Run:
					MpeWrite(Plc.DlgMenu03.LampStop, (long)0); // 마킹부 정지 스위치 램프 ON(PC가 On, PLC가 확인하고 Off시킴)
					break;
			case _SigInx::_Reset:
					MpeWrite(Plc.DlgMenu03.LampStop, (long)0); // 마킹부 정지 스위치 램프 ON(PC가 On, PLC가 확인하고 Off시킴)
					break;
			case _SigInx::_Stop:
					MpeWrite(Plc.DlgMenu03.LampStop, (long)(pDoc->BtnStatus.Main.Stop ? 1 : 0)); // 마킹부 정지 스위치 램프 ON(PC가 On, PLC가 확인하고 Off시킴)
				break;
			case _SigInx::_MkTq:
					MpeWrite(Plc.DlgMenu01.TensionOnPunch, (long)(pDoc->BtnStatus.Tq.Mk ? 1 : 0)); // 마킹부 텐션 ON (PC가 ON/OFF시킴)
				break;
			case _SigInx::_AoiTq:
					MpeWrite(Plc.DlgMenu01.TensionOnAoi, (long)(pDoc->BtnStatus.Tq.Aoi ? 1 : 0)); // 검사부/각인부 텐션 ON (PC가 ON/OFF시킴)
				break;
			case _SigInx::_EngTq:
					MpeWrite(Plc.DlgMenu01.TensionOnAoi, (long)(pDoc->BtnStatus.Tq.Eng ? 1 : 0)); // 검사부/각인부 텐션 ON (PC가 ON/OFF시킴)
				break;
			case _SigInx::_CcwModRe:
					MpeWrite(Plc.DlgInfo.OneMetal, (long)(pDoc->BtnStatus.Induct.Rc ? 1 : 0)); // 마킹(GUI) 리코일러 정방향(Off->CW/On->CCW)
				break;
			case _SigInx::_CcwModUn:
					MpeWrite(Plc.DlgInfo.TwoMetal, (long)(pDoc->BtnStatus.Induct.Uc ? 1 : 0)); // 마킹(GUI) 언코일러 정방향(Off->CW/On->CCW)
				break;
			case _SigInx::_MvCwRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_5, 1);	// 리코일러 제품휠 정회전 스위치
				break;
			case _SigInx::_MvCcwRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_6, 1);	// 리코일러 제품휠 역회전 스위치
				break;
			case _SigInx::_PrdChuckRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_41, 1);	// 리코일러 제품척 클램프 스위치
				break;
			case _SigInx::_DancerUpRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_42, 1);	// 리코일러 댄서롤 상승/하강 스위치
				break;
			case _SigInx::_PasteUpLfRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_43, 1);	// 리코일러 제품 이음매(상/좌) 스위치
				break;
			case _SigInx::_PasteUpRtRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_7, 1);	// 리코일러 제품 이음매(하/우) 스위치
				break;
			case _SigInx::_PasteVacRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_8, 1);	// 리코일러 제품 이음매 진공 스위치
				break;
			case _SigInx::_PprChuckRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_44, 1);	// 리코일러 간지척 클램프 스위치
				break;
			case _SigInx::_PprCwRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_45, 1);	// 리코일러 간지휠 정회전 스위치
				break;
			case _SigInx::_PprCcwRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_46, 1);	// 리코일러 간지휠 역회전 스위치
				break;
			case _SigInx::_DoRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_66, 1);	// 리코일러 Rewinder 동작 스위치
				break;
			case _SigInx::_PrdPprRe:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_67, 1);	// 리코일러 Rewinder 제품 & 간지 스위치
				break;
			case _SigInx::_Relation:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_9, 1);	// 마킹부 연동 온/오프 스위치
				break;
			case _SigInx::_MvCwMk:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_10, 1);	// 마킹부 피딩 정회전 스위치
				break;
			case _SigInx::_MvCcwMk:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_11, 1);	// 마킹부 피딩 역회전 스위치
				break;
			case _SigInx::_FdVacMk:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_12, 1);	// 마킹부 피딩 진공 스위치
				break;
			case _SigInx::_PushUpMk:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_13, 1);	// 마킹부 제품푸쉬 스위치 // (토크 진공 스위치) - X
				break;
			case _SigInx::_TblBlwMk:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_14, 1);	// 마킹부 테이블 브로워 스위치
				break;
			case _SigInx::_TblVacMk:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_15, 1);	// 마킹부 테이블 진공 스위치
				break;
			case _SigInx::_FdClampMk:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_51, 1);	// 마킹부 피딩 클램프 스위치
				break;
			case _SigInx::_TensClampMk:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_52, 1);	// 마킹부 텐션 클램프 스위치
				break;
			case _SigInx::_OnePnlMk:				
				//MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, (long)(pDoc->BtnStatus.Mk.MvOne ? 1: 0));
				break;
			case _SigInx::_DancerUpMk:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_48, 1);	// 마킹부 댄서롤 상승/하강 스위치
				break;
			case _SigInx::_MvCwAoiDn:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_56, 1);	// 검사부 하 피딩 정회전 스위치
				break;
			case _SigInx::_MvCcwAoiDn:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_57, 1);	// 검사부 하 피딩 역회전 스위치
				break;
			case _SigInx::_FdVacAoiDn:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_58, 1);	// 검사부 하 피딩 진공 스위치
				break;
			case _SigInx::_PushUpAoiDn:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_59, 1);	// 검사부 하 제품푸쉬 스위치 // (토크 진공 스위치) - X
				break;
			case _SigInx::_TblBlwAoiDn:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_60, 1);	// 검사부 하 테이블 브로워 스위치
				break;
			case _SigInx::_TblVacAoiDn:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_61, 1);	// 검사부 하 테이블 진공 스위치
				break;
			case _SigInx::_FdClampAoiDn:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_64, 1);	// 검사부 하 피딩 클램프 스위치
				break;
			case _SigInx::_TensClampAoiDn:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_65, 1);	// 검사부 하 텐션 클램프 스위치
				break;
			case _SigInx::_OnePnlAoiDn:
				MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, (long)(pDoc->BtnStatus.RunMode.FeedOnePanel ? 1 : 0)); // 한판넬 이송상태 ON (PC가 ON, OFF)
				break;
			case _SigInx::_VelClrSonicAoiDn:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_88, 1);	// AOI(하) 초음파 세정기 속도 ON (PC가 ON/OFF시킴)
				break;
			case _SigInx::_OnePnlAoiUp:
				MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, (long)(pDoc->BtnStatus.RunMode.FeedOnePanel ? 1 : 0)); // 한판넬 이송상태 ON (PC가 ON, OFF)
				break;
			case _SigInx::_OnePnlEng:
				MpeWrite(pView->Plc.DlgMenu03.FeedOnePanel, (long)(pDoc->BtnStatus.RunMode.FeedOnePanel ? 1 : 0)); // 한판넬 이송상태 ON (PC가 ON, OFF)
				break;
			case _SigInx::_VelClrSonicEng:
				if (m_pDlgMenu03)
					m_pDlgMenu03->SwMpeBtn(IDC_CHK_87, 1);	// 각인부 초음파 세정기 속도 ON (PC가 ON/OFF시킴)
				break;
			case _SigInx::_MyMsgYes:
				SetMyMsgYes();
				pView->m_pEngrave->SetMyMsgYes();	//_SigInx::_MyMsgYes
				break;
			case _SigInx::_MyMsgNo:
				SetMyMsgNo();
				pView->m_pEngrave->SetMyMsgNo();	//_SigInx::_MyMsgNo
				break;
			case _SigInx::_MyMsgOk:
				SetMyMsgOk();
				pView->m_pEngrave->SetMyMsgOk();	//_SigInx::_MyMsgOk
				break;
			case _SigInx::_DualTest:
				SetDualTest(pDoc->WorkingInfo.LastJob.bDualTest);
				break;
			case _SigInx::_SampleTest:
					MpeWrite(Plc.DlgInfo.SampleTest, (pDoc->WorkingInfo.LastJob.bSampleTest) ? 1 : 0);		// Sample 검사 On
				break;
			//case _SigInx::_TestMode:
			//	pDoc->SetTestMode(pDoc->WorkingInfo.LastJob.nTestMode); // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
			//	break;
			case _SigInx::_RecoilerCcw:
					MpeWrite(Plc.DlgInfo.OneMetal, (long)(pDoc->BtnStatus.Induct.Rc ? 1 : 0));
				::WritePrivateProfileString(_T("Last Job"), _T("One Metal On"), pDoc->BtnStatus.Induct.Rc ? _T("1") : _T("0"), PATH_WORKING_INFO);// IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
				break;
			case _SigInx::_UncoilerCcw:
					MpeWrite(Plc.DlgInfo.TwoMetal, (long)(pDoc->BtnStatus.Induct.Uc ? 1 : 0));
				::WritePrivateProfileString(_T("Last Job"), _T("Two Metal On"), pDoc->BtnStatus.Induct.Uc ? _T("1") : _T("0"), PATH_WORKING_INFO);// IDC_CHK_ONE_METAL - Recoiler\r정방향 CW : FALSE
				break;
			case _SigInx::_DoorRecoiler:
					MpeWrite(Plc.DlgInfo.DoorSensorRecoiler, (long)(pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0));	// 리코일러Door센서 사용
				break;
			case _SigInx::_DoorAoiUp:
					MpeWrite(Plc.DlgInfo.DoorSensorAoiUp, (long)(pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0));	// AOI(상) Door센서 사용
				break;
			case _SigInx::_DoorAoiDn:
					MpeWrite(Plc.DlgInfo.DoorSensorAoiDn, (long)(pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0));	// AOI(하) Door센서 사용
				break;
			case _SigInx::_DoorMk:
					MpeWrite(Plc.DlgInfo.DoorSensorPunch, (long)(pDoc->WorkingInfo.LastJob.bMkDrSen ? 1 : 0));	// 마킹Door센서 사용
				break;
			case _SigInx::_DoorEngrave:
					MpeWrite(Plc.DlgInfo.DoorSensorEngrave, (long)(pDoc->WorkingInfo.LastJob.bEngvDrSen ? 1 : 0));	// 각인부 Door센서 사용
				break;
			case _SigInx::_DoorUncoiler:
					MpeWrite(Plc.DlgInfo.DoorSensorUncoiler, (long)(pDoc->WorkingInfo.LastJob.bUclDrSen ? 1 : 0));	// 언코일러Door센서 사용
				break;
			case _SigInx::_CleannerAoiUp:
				MpeWrite(pView->Plc.DlgInfo.CleanRollerAoiUp, (long)(pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0));
				break;
			case _SigInx::_CleannerAoiDn:
				MpeWrite(pView->Plc.DlgInfo.CleanRollerAoiDn, (long)(pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0));
				break;
			case _SigInx::_UltraSonicAoiDn:
				MpeWrite(pView->Plc.DlgInfo.UltraSonicAoi, (long)(pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic ? 1 : 0));
				break;
			case _SigInx::_UltraSonicEngrave:
				MpeWrite(pView->Plc.DlgInfo.UltraSonicEngrave, (long)(pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic ? 1 : 0));
				break;
			case _SigInx::_Use380mm:
				MpeWrite(Plc.DlgInfo.Laser380mm, (long)(pDoc->WorkingInfo.LastJob.bUse380mm ? 1 : 0));	// EPC실린더(제품소->OFF/제품대->ON)
				break;
			case _SigInx::_TempPause:
				MpeWrite(Plc.DlgInfo.UseStopLength, (long)(pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0));	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)
				::WritePrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), pDoc->WorkingInfo.LastJob.bTempPause ? _T("1") : _T("0"), PATH_WORKING_INFO);
				break;
			default:
				break;
			}
		}
	}
}

BOOL CGvisR2R_PunchView::MpeWrite(CString strRegAddr, long lData, BOOL bCheck)
{
	if (!m_pMpe)
		return FALSE;
	return m_pMpe->Write(strRegAddr, lData, bCheck);
}

long CGvisR2R_PunchView::MpeRead(CString strRegAddr)
{
	if (!m_pMpe)
		return FALSE;
	return m_pMpe->Read(strRegAddr);
}

int CGvisR2R_PunchView::GetLastBufferSerial(int nAoi) // 0: Up, 1: Dn
{
	return m_pBufSerial[nAoi][m_nBufTot[nAoi] - 1];
}

void CGvisR2R_PunchView::SetAlarmToPlc(int nFromUnit)
{
	switch (nFromUnit)
	{
	case UNIT_UNCOILER:
		//MpeWrite(_T(""), 1);
		break;
	case UNIT_ENGRAVE:
		MpeWrite(_T("MB40019A"), 1);
		break;
	case UNIT_AOIUP:
		//MpeWrite(_T(""), 1);
		break;
	case UNIT_AOIDN:
		//MpeWrite(_T(""), 1);
		break;
	case UNIT_PUNCH:
		MpeWrite(_T("MB40019B"), 1);
		break;
	case UNIT_RECOILER:
		//MpeWrite(_T(""), 1);
		break;
	}
}

void CGvisR2R_PunchView::ClampOn()
{
	MpeWrite(Plc.DlgMenu03.TableClampDnPunch, 1); // 마킹부 테이블 클램프 하강 스위치
}

BOOL CGvisR2R_PunchView::IsClampOff()
{
	if (!pView->MpeRead(_T("MB40034B")))
	{
		if(!m_bTIM_INIT_VIEW)
			MsgBox(_T("마킹부 클램프 테이블 척이 올라와 있습니다."));
		return TRUE;
	}
	return FALSE;
}

void CGvisR2R_PunchView::SetMkReStart()
{
	if (m_bReadyDone && m_bAuto)
	{
		m_nMkStAuto = 0;
		MpeWrite(_T("MB40095D"), 1); // 마킹부 마킹 시작 재요청 신호
	}
}

void CGvisR2R_PunchView::ErrorRead2dCode(int nMcId)
{
	switch (nMcId)
	{
	case _PcId::_Engrave:
		SetAlarmToPlc(UNIT_ENGRAVE);
		break;
	case _PcId::_AoiUp:
		SetAlarmToPlc(UNIT_AOIUP);
		break;
	case _PcId::_AoiDn:
		SetAlarmToPlc(UNIT_AOIDN);
		break;
	default:
		break;
	}
}

void CGvisR2R_PunchView::SetJobEndEngrave()
{
	if (m_pEngrave)
		m_pEngrave->SetJobEndEngrave();
}

BOOL CGvisR2R_PunchView::CheckCamMstInfo(int nLayer, int nSerial)
{
	int nNodeX = pDoc->m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = pDoc->m_Master[0].m_pPcsRgn->m_nRow;
	int i, nPcsId;
	CString sMsg;
	int nTotDefPcs = 0;

	int nIdx = pDoc->GetPcrIdx(nSerial);

	if (pDoc->m_pPcr[nLayer])
	{
		if (pDoc->m_pPcr[nLayer][nIdx])
			nTotDefPcs = pDoc->m_pPcr[nLayer][nIdx]->m_nTotDef;
	}
	else
	{
		sMsg.Format(_T("Error - m_pPcr[%d][%d]의 메모리가 설정되지 않았습니다."), nLayer, nIdx);
		MsgBox(sMsg);
		return FALSE;
	}

	for (i = 0; i < nTotDefPcs; i++)
	{
		nPcsId = pDoc->m_pPcr[nLayer][nIdx]->m_pDefPcs[i];
		if ((nNodeX * nNodeY - 1) < nPcsId)
		{
			sMsg.Format(_T("Error - Defect Pcs Index(%d) is over total pcs - 1 ."), nPcsId);
			MsgBox(sMsg);
			return FALSE;
		}
	}

	return TRUE;
}

void CGvisR2R_PunchView::ClearBuffer()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CFileFind cFile;
	BOOL bExistSup, bExistBup, bExistSdn, bExistBdn;
	BOOL bExistSVsup, bExistBVsup, bExistSVsdn, bExistBVsdn;

	bExistSup = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareUp + _T("*.pcr"));
	bExistBup = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufUp + _T("*.pcr"));
	bExistSVsup = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsShareUp + _T("*.pcr"));
	bExistBVsup = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsDummyBufUp + _T("*.pcr"));
	if (bDualTest)
	{
		bExistSdn = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsShareDn + _T("*.pcr"));
		bExistBdn = cFile.FindFile(pDoc->WorkingInfo.System.sPathVrsBufDn + _T("*.pcr"));
		bExistSVsdn = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsShareDn + _T("*.pcr"));
		bExistBVsdn = cFile.FindFile(pDoc->WorkingInfo.System.sPathVsDummyBufDn + _T("*.pcr"));
		if (bExistSup || bExistSdn ||
			bExistBup || bExistBdn ||
			bExistSVsup || bExistSVsdn ||
			bExistBVsup || bExistBVsdn)
		{
			pDoc->DelPcrAll();
		}
		else
		{
			pView->m_bIsBuf[0] = FALSE;
			pView->m_bIsBuf[1] = FALSE;
		}
	}
	else
	{
		if (bExistSup || bExistBup || bExistSVsup || bExistBVsup)
		{
			pDoc->DelPcrAll();
		}
		else
		{
			pView->m_bIsBuf[0] = FALSE;
			pView->m_bIsBuf[1] = FALSE;
		}
	}
}

// RemakeReelmapFromPcr - Start
BOOL CGvisR2R_PunchView::RemakeReelmapFromPcr(CString sModel, CString sLot, CString sLayerUp, CString sLayerDn)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sReelmapSrc, str, sMsg;

	if (!pDoc->m_pReelMapUp)
	{
		sMsg.Format(_T("m_pReelMapUp이 Create되지않았습니다.\r\n%s"));
		pView->MsgBox(sMsg);
		return FALSE;
	}

	pDoc->WorkingInfo.LastJob.sModel = sModel;
	pDoc->WorkingInfo.LastJob.sLot = sLot;
	pDoc->WorkingInfo.LastJob.sLayerUp = sLayerUp;
	pDoc->WorkingInfo.LastJob.sLayerDn = sLayerDn;

	int nOffline = IsOfflineFolder(); // 0 : Not exist, 1 : Exist only Up, 2 : Exist only Dn, 3 : Exist Up and Dn

	if (nOffline == 0)
	{
		sMsg.Format(_T("OFFLINE 폴더가 존재하지 않습니다."));
		pView->MsgBox(sMsg);
		return FALSE;
	}

	//CopyRmapToOffline(nOffline);

	StartThreadRemakeRmapFromPcr();

	int nLastShot = GetLastShotFromPcr(nOffline);

	if (nOffline & 0x02)
	{
		bDualTest = TRUE;
		ResetMkInfo(2); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
	}
	else
	{
		bDualTest = FALSE;
		ResetMkInfo(0); // 0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
	}

	ClrDispMsg();

	RemakeRmapFromPcr(nLastShot, nOffline);

	StopThreadRemakeRmapFromPcr();


	if (m_pDlgMenu05)
	{
		int nIndex = m_pDlgMenu05->GetIdxTopLayer();
		if (nIndex > -1)
		{
			m_pDlgMenu05->SelchangeComboLayer(nIndex);
		}
	}

	return TRUE;
}

int CGvisR2R_PunchView::IsOfflineFolder() // 0 : Not exist, 1 : Exist only Up, 2 : Exist only Dn, 3 : Exist Up and Dn
{
	int nRtn = 0;
	CString sPath, str;
	BOOL bWorking;

	//검색 클래스
	CFileFind finder;

	str = _T("OFFLINE");
	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sLot,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		str);

	//CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함 
	bWorking = finder.FindFile(sPath);

	if (bWorking)
		nRtn |= 0x01;

	str = _T("OFFLINE");
	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sLot,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		str);

	//CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함 
	bWorking = finder.FindFile(sPath);

	if (bWorking)
		nRtn |= 0x02;

	return nRtn;
}

void CGvisR2R_PunchView::StartThreadRemakeRmapFromPcr()
{
	// LoadPcrUpOnOffline
	if (!m_bThread[41])
		m_Thread[41].Start(GetSafeHwnd(), this, ThreadProc41);

	// LoadPcrDnOnOffline
	if (!m_bThread[42])
		m_Thread[42].Start(GetSafeHwnd(), this, ThreadProc42);

	// LoadPcrUpAllOnOffline
	if (!m_bThread[43])
		m_Thread[43].Start(GetSafeHwnd(), this, ThreadProc43);

	// LoadPcrDnAllOnOffline
	if (!m_bThread[44])
		m_Thread[44].Start(GetSafeHwnd(), this, ThreadProc44);

	// UpdateRMapUpOnOffline
	if (!m_bThread[45])
		m_Thread[45].Start(GetSafeHwnd(), this, ThreadProc45);

	// UpdateRMapDnOnOffline
	if (!m_bThread[46])
		m_Thread[46].Start(GetSafeHwnd(), this, ThreadProc46);

	// UpdateRMapUpAllOnOffline
	if (!m_bThread[47])
		m_Thread[47].Start(GetSafeHwnd(), this, ThreadProc47);

	// UpdateRMapDnAllOnOffline
	if (!m_bThread[48])
		m_Thread[48].Start(GetSafeHwnd(), this, ThreadProc48);
}

UINT CGvisR2R_PunchView::ThreadProc41(LPVOID lpContext)	// LoadPcrUpOnOffline
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[39] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[39].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[39] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_LOAD_PCR_UP_OFFLINE)
		{
			pThread->LoadPcrUpOnOffline();
			pThread->m_bTHREAD_LOAD_PCR_UP_OFFLINE = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[39] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc42(LPVOID lpContext)	// LoadPcrDnOnOffline
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[40] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[40].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[40] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_LOAD_PCR_DN_OFFLINE)
		{
			pThread->LoadPcrDnOnOffline();
			pThread->m_bTHREAD_LOAD_PCR_DN_OFFLINE = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[40] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc43(LPVOID lpContext)	// LoadPcrUpAllOnOffline
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[41] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[41].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[41] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_LOAD_PCR_ALLUP_OFFLINE)
		{
			pThread->LoadPcrUpAllOnOffline();
			pThread->m_bTHREAD_LOAD_PCR_ALLUP_OFFLINE = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[41] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc44(LPVOID lpContext)	// LoadPcrDnAllOnOffline
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[42] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[42].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[42] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_LOAD_PCR_ALLDN_OFFLINE)
		{
			pThread->LoadPcrDnAllOnOffline();
			pThread->m_bTHREAD_LOAD_PCR_ALLDN_OFFLINE = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[42] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc45(LPVOID lpContext)	// m_pReelMapUp->WriteOnOffline()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[43] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[43].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[43] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_UP_OFFLINE)
		{
			pThread->UpdateRMapUpOnOffline();
			pThread->m_bTHREAD_UPDATE_REELMAP_UP_OFFLINE = FALSE;
			Sleep(0);
		}
		else if (pThread->m_bTHREAD_WRITE_LAST_INFO_REELMAP_UP_OFFLINE) // WriteLastRMapUpInfoOnOffline()
		{
			pThread->WriteLastRMapUpInfoOnOffline();
			pThread->m_bTHREAD_WRITE_LAST_INFO_REELMAP_UP_OFFLINE = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[43] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc46(LPVOID lpContext)	// m_pReelMapDn->WriteOnOffline()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[44] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[44].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[44] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_DN_OFFLINE)
		{
			pThread->UpdateRMapDnOnOffline();
			pThread->m_bTHREAD_UPDATE_REELMAP_DN_OFFLINE = FALSE;
			Sleep(0);
		}
		else if (pThread->m_bTHREAD_WRITE_LAST_INFO_REELMAP_DN_OFFLINE) // WriteLastRMapDnInfoOnOffline()
		{
			pThread->WriteLastRMapDnInfoOnOffline();
			pThread->m_bTHREAD_WRITE_LAST_INFO_REELMAP_DN_OFFLINE = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[44] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc47(LPVOID lpContext)	// m_pReelMapAllUp->WriteOnOffline()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[45] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[45].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[45] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP_OFFLINE)
		{
			pThread->UpdateRMapUpAllOnOffline();
			pThread->m_bTHREAD_UPDATE_REELMAP_ALLUP_OFFLINE = FALSE;
			Sleep(0);
		}
		else if (pThread->m_bTHREAD_WRITE_LAST_INFO_REELMAP_ALLUP_OFFLINE) // WriteLastRMapUpAllInfoOnOffline()
		{
			pThread->WriteLastRMapUpAllInfoOnOffline();
			pThread->m_bTHREAD_WRITE_LAST_INFO_REELMAP_ALLUP_OFFLINE = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[45] = FALSE;

	return 0;
}

UINT CGvisR2R_PunchView::ThreadProc48(LPVOID lpContext)	// m_pReelMapAllDn->WriteOnOffline()
{
	// Turn the passed in 'this' pointer back into a CProgressMgr instance
	CGvisR2R_PunchView* pThread = reinterpret_cast<CGvisR2R_PunchView*>(lpContext);

	BOOL bLock = FALSE;
	DWORD dwTick = GetTickCount();
	DWORD dwShutdownEventCheckPeriod = 0; // thread shutdown event check period

	pThread->m_bThread[46] = TRUE;
	while (WAIT_OBJECT_0 != WaitForSingleObject(pThread->m_Thread[46].GetShutdownEvent(), dwShutdownEventCheckPeriod))
	{
		pThread->m_dwThreadTick[46] = GetTickCount() - dwTick;
		dwTick = GetTickCount();

		if (pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN_OFFLINE)
		{
			pThread->UpdateRMapDnAllOnOffline();
			pThread->m_bTHREAD_UPDATE_REELMAP_ALLDN_OFFLINE = FALSE;
			Sleep(0);
		}
		else if (pThread->m_bTHREAD_WRITE_LAST_INFO_REELMAP_ALLDN_OFFLINE) // WriteLastRMapDnAllInfoOnOffline()
		{
			pThread->WriteLastRMapDnAllInfoOnOffline();
			pThread->m_bTHREAD_WRITE_LAST_INFO_REELMAP_ALLDN_OFFLINE = FALSE;
			Sleep(0);
		}
		else
			Sleep(30);
	}

	pThread->m_bThread[46] = FALSE;

	return 0;
}

void CGvisR2R_PunchView::LoadPcrUpOnOffline()
{
	int nSerial = m_nSerialRmapUpdateOnOffline;
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\OFFLINE\\%04d.pcr"),
		pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sLot,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		nSerial);

	// 검색 클래스
	CFileFind findfile;

	if (findfile.FindFile(sPath))
	{
		pDoc->LoadPcrUp(sPath);
	}
}

void CGvisR2R_PunchView::LoadPcrDnOnOffline()
{
	int nSerial = m_nSerialRmapUpdateOnOffline;
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\OFFLINE\\%04d.pcr"),
		pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sLot,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		nSerial);

	// 검색 클래스
	CFileFind findfile;

	if (findfile.FindFile(sPath))
	{
		pDoc->LoadPcrDn(sPath);
	}
}

void CGvisR2R_PunchView::LoadPcrUpAllOnOffline()
{
	int nSerial = m_nSerialRmapUpdateOnOffline;
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\OFFLINE\\%04d.pcr"),
		pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sLot,
		pDoc->WorkingInfo.LastJob.sLayerUp,
		nSerial);

	// 검색 클래스
	CFileFind findfile;

	if (findfile.FindFile(sPath))
	{
		pDoc->LoadPcrAllUp(sPath);
	}
}

void CGvisR2R_PunchView::LoadPcrDnAllOnOffline()
{
	int nSerial = m_nSerialRmapUpdateOnOffline;
	CString sPath;
	sPath.Format(_T("%s%s\\%s\\%s\\OFFLINE\\%04d.pcr"),
		pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sLot,
		pDoc->WorkingInfo.LastJob.sLayerDn,
		nSerial);

	// 검색 클래스
	CFileFind findfile;

	if (findfile.FindFile(sPath))
	{
		pDoc->LoadPcrAllDn(sPath);
	}
}

void CGvisR2R_PunchView::UpdateRMapUpOnOffline()
{
	int nSerial = m_nSerialRmapUpdateOnOffline;
	if (pDoc->m_pReelMapUp)
	{
		pDoc->m_pReelMapUp->WriteOnOffline(nSerial); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CGvisR2R_PunchView::UpdateRMapDnOnOffline()
{
	int nSerial = m_nSerialRmapUpdateOnOffline;
	if (pDoc->m_pReelMapDn)
	{
		pDoc->m_pReelMapDn->WriteOnOffline(nSerial); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CGvisR2R_PunchView::UpdateRMapUpAllOnOffline()
{
	int nSerial = m_nSerialRmapUpdateOnOffline;
	if (pDoc->m_pReelMapAllUp)
	{
		pDoc->m_pReelMapAllUp->WriteOnOffline(nSerial); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CGvisR2R_PunchView::UpdateRMapDnAllOnOffline()
{
	int nSerial = m_nSerialRmapUpdateOnOffline;
	if (pDoc->m_pReelMapAllDn)
	{
		pDoc->m_pReelMapAllDn->WriteOnOffline(nSerial); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
}

void CGvisR2R_PunchView::SetLastSerialOnOffline(int nSerial, int nOffline)
{
	if (nSerial <= 0)
	{
		ClrDispMsg();
		AfxMessageBox(_T("Serial Error.57"));
		return;
	}

	BOOL bDualTest = nOffline & 0x02;

	if (nSerial > 0)
	{
		if (pDoc->m_pReelMapUp)
			pDoc->m_pReelMapUp->SetLastSerialOnOffline(nSerial);					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
		if (bDualTest)
		{
			if (pDoc->m_pReelMapDn)
				pDoc->m_pReelMapDn->SetLastSerialOnOffline(nSerial);					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
			if (pDoc->m_pReelMapAllUp)
				pDoc->m_pReelMapAllUp->SetLastSerialOnOffline(nSerial);					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
			if (pDoc->m_pReelMapAllDn)
				pDoc->m_pReelMapAllDn->SetLastSerialOnOffline(nSerial);					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
		}
	}
}

void CGvisR2R_PunchView::StopThreadRemakeRmapFromPcr()
{
	if (m_bThread[39])
	{
		m_Thread[39].Stop();
		Sleep(20);
		while (m_bThread[39])
		{
			Sleep(20);
		}
	}

	if (m_bThread[40])
	{
		m_Thread[40].Stop();
		Sleep(20);
		while (m_bThread[40])
		{
			Sleep(20);
		}
	}

	if (m_bThread[41])
	{
		m_Thread[41].Stop();
		Sleep(20);
		while (m_bThread[41])
		{
			Sleep(20);
		}
	}

	if (m_bThread[42])
	{
		m_Thread[42].Stop();
		Sleep(20);
		while (m_bThread[42])
		{
			Sleep(20);
		}
	}

	if (m_bThread[43])
	{
		m_Thread[43].Stop();
		Sleep(20);
		while (m_bThread[43])
		{
			Sleep(20);
		}
	}

	if (m_bThread[44])
	{
		m_Thread[44].Stop();
		Sleep(20);
		while (m_bThread[44])
		{
			Sleep(20);
		}
	}

	if (m_bThread[45])
	{
		m_Thread[45].Stop();
		Sleep(20);
		while (m_bThread[45])
		{
			Sleep(20);
		}
	}

	if (m_bThread[46])
	{
		m_Thread[46].Stop();
		Sleep(20);
		while (m_bThread[46])
		{
			Sleep(20);
		}
	}
}

BOOL CGvisR2R_PunchView::WriteLastRmapInfo(int nLastShot, int nOffline)
{
	if (!pDoc->MakeMkDir())
		return FALSE;

	m_nLastShotOnOffline = nLastShot;
	BOOL bDualTest = nOffline & 0x02;
	//m_bTHREAD_WRITE_LAST_INFO_REELMAP_UP_OFFLINE = TRUE;
	WriteLastRMapUpInfoOnOffline();
	if (bDualTest)
	{
		WriteLastRMapDnInfoOnOffline();
		WriteLastRMapUpAllInfoOnOffline();
		WriteLastRMapDnAllInfoOnOffline();
		//m_bTHREAD_WRITE_LAST_INFO_REELMAP_DN_OFFLINE = TRUE;
		//m_bTHREAD_WRITE_LAST_INFO_REELMAP_ALLUP_OFFLINE = TRUE;
		//m_bTHREAD_WRITE_LAST_INFO_REELMAP_ALLDN_OFFLINE = TRUE;
	}

	return TRUE;
}

void CGvisR2R_PunchView::WriteLastRMapUpInfoOnOffline()
{
	if (m_nLastShotOnOffline <= 0)
	{
		ClrDispMsg();
		AfxMessageBox(_T("Serial Error.150"));
		return;
	}

	if (pDoc->m_pReelMapUp)
	{
		pDoc->m_pReelMapUp->WriteLastRmapInfoOnOffline(); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		pDoc->m_pReelMapUp->SetLastSerialOnOffline(m_nLastShotOnOffline);
	}
}

void CGvisR2R_PunchView::WriteLastRMapDnInfoOnOffline()
{
	if (m_nLastShotOnOffline <= 0)
	{
		ClrDispMsg();
		AfxMessageBox(_T("Serial Error.151"));
		return;
	}

	if (pDoc->m_pReelMapDn)
	{
		pDoc->m_pReelMapDn->WriteLastRmapInfoOnOffline(); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		pDoc->m_pReelMapDn->SetLastSerialOnOffline(m_nLastShotOnOffline);
	}
}

void CGvisR2R_PunchView::WriteLastRMapUpAllInfoOnOffline()
{
	if (m_nLastShotOnOffline <= 0)
	{
		ClrDispMsg();
		AfxMessageBox(_T("Serial Error.152"));
		return;
	}

	if (pDoc->m_pReelMapAllUp)
	{
		pDoc->m_pReelMapAllUp->WriteLastRmapInfoOnOffline(); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		pDoc->m_pReelMapAllUp->SetLastSerialOnOffline(m_nLastShotOnOffline);
	}
}

void CGvisR2R_PunchView::WriteLastRMapDnAllInfoOnOffline()
{
	if (m_nLastShotOnOffline <= 0)
	{
		ClrDispMsg();
		AfxMessageBox(_T("Serial Error.153"));
		return;
	}

	if (pDoc->m_pReelMapAllDn)
	{
		pDoc->m_pReelMapAllDn->WriteLastRmapInfoOnOffline(); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		pDoc->m_pReelMapAllDn->SetLastSerialOnOffline(m_nLastShotOnOffline);
	}
}

void CGvisR2R_PunchView::DeleteReelmapOnOffline()
{
	CFileFind findfile;
	CString sPath = _T("");

	if (pDoc->m_pReelMapUp)
		sPath = pDoc->m_pReelMapUp->GetRmapPathOnOffline(RMAP_UP);
	if (!sPath.IsEmpty())
	{
		if (findfile.FindFile(sPath))
			DeleteFile(sPath);
		sPath = _T("");
	}

	if (pDoc->m_pReelMapDn)
		sPath = pDoc->m_pReelMapDn->GetRmapPathOnOffline(RMAP_DN);
	if (!sPath.IsEmpty())
	{
		if (findfile.FindFile(sPath))
			DeleteFile(sPath);
		sPath = _T("");
	}

	if (pDoc->m_pReelMapAllUp)
		sPath = pDoc->m_pReelMapAllUp->GetRmapPathOnOffline(RMAP_ALLUP);
	if (!sPath.IsEmpty())
	{
		if (findfile.FindFile(sPath))
			DeleteFile(sPath);
		sPath = _T("");
	}

	if (pDoc->m_pReelMapAllDn)
		sPath = pDoc->m_pReelMapAllDn->GetRmapPathOnOffline(RMAP_ALLDN);
	if (!sPath.IsEmpty())
	{
		if (findfile.FindFile(sPath))
			DeleteFile(sPath);
		sPath = _T("");
	}
}

int CGvisR2R_PunchView::GetLastShotFromPcr(int nOffline)
{
	CString sPath = _T("폴더 경로/*.*");

	if (nOffline & 0x01)
	{
		sPath.Format(_T("%s%s\\%s\\%s\\OFFLINE\\*.pcr"),
			pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModel,
			pDoc->WorkingInfo.LastJob.sLot,
			pDoc->WorkingInfo.LastJob.sLayerUp);
	}
	else if (nOffline & 0x02)
	{
		sPath.Format(_T("%s%s\\%s\\%s\\OFFLINE\\*.pcr"),
			pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModel,
			pDoc->WorkingInfo.LastJob.sLot,
			pDoc->WorkingInfo.LastJob.sLayerDn);
	}

	// 검색할 경로 및 파일
	// 특정 확장자를 갖는 파일을 검색하고 싶으면 '경로/*.pcr' 등으로 사용

	// 검색 클래스
	CFileFind finder;

	// CFileFind는 파일, 디렉터리가 존재하면 TRUE 를 반환함
	BOOL bWorking = finder.FindFile(sPath);

	CString fileName;
	CString DirName;
	int nSerial = 0, nLastShot = 0, nPos = -1;

	while (bWorking)
	{
		//다음 파일 or 폴더 가 존재하면다면 TRUE 반환
		bWorking = finder.FindNextFile();

		// folder 일 경우는 continue
		if (finder.IsDirectory() || finder.IsDots())
			continue;

		// 파일 일때
		// 파일의 이름
		CString _fileName = finder.GetFileName();

		// 현재폴더 상위폴더 썸네일파일은 제외
		if (_fileName == _T("Thumbs.db")) continue;

		fileName = finder.GetFileTitle();

		//읽어온 파일 이름에서 LastShot 번호를 찾는다.
		nSerial = _tstoi(fileName);

		if (nSerial > nLastShot)
			nLastShot = nSerial;
	}

	return nLastShot;
}

BOOL CGvisR2R_PunchView::RemakeRmapFromPcr(int nLastShot, int nOffline)// [nOffline] 0 : Not exist, 1 : Exist only Up, 2 : Exist only Dn, 3 : Exist Up and Dn
{
	CString sMsg, sVal, sUpPath, sDnPath, sPcrUpPath, sPcrDnPath;
	int nPnl;

	sPcrUpPath.Format(_T("%s%s\\%s\\%s\\OFFLINE"),
		pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sLot,
		pDoc->WorkingInfo.LastJob.sLayerUp);

	sPcrDnPath.Format(_T("%s%s\\%s\\%s\\OFFLINE"),
		pDoc->WorkingInfo.System.sPathOldFile,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sLot,
		pDoc->WorkingInfo.LastJob.sLayerDn);

	ResetYield(nOffline);
	DeleteReelmapOnOffline();
	DeleteYieldOnOffline();

	// 검색 클래스
	CFileFind findfile;

	double dProgressRatio = 0.0;
	int nProgress = 0, nSerial = 0;
	CDlgProgress dlg;
	sVal.Format(_T("On Remaking Reelmap from PCR."));
	dlg.Create(sVal);
	dlg.SetRange(0, 100);
	dlg.SetPos(1);

	for (nPnl = 0; nPnl < nLastShot; nPnl++)
	{
		dProgressRatio = double(nPnl + 1) / double(nLastShot) * 100.0;
		nProgress = int(dProgressRatio);
		dlg.SetPos(nProgress);

		nSerial = nPnl + 1;

		LoadPcrOnOffline(nSerial, nOffline);

		while (m_bTHREAD_LOAD_PCR_UP_OFFLINE || m_bTHREAD_LOAD_PCR_DN_OFFLINE)
		{
			Sleep(100);
		}

		LoadPcrAllOnOffline(nSerial, nOffline);

		while (m_bTHREAD_LOAD_PCR_ALLUP_OFFLINE || m_bTHREAD_LOAD_PCR_ALLDN_OFFLINE)
		{
			Sleep(100);
		}

		UpdateRMapOnOffline(nSerial, nOffline);

		while (m_bTHREAD_UPDATE_REELMAP_UP_OFFLINE || m_bTHREAD_UPDATE_REELMAP_DN_OFFLINE)
		{
			Sleep(100);
		}

		UpdateRMapAllOnOffline(nSerial, nOffline);

		while (m_bTHREAD_UPDATE_REELMAP_ALLUP_OFFLINE || m_bTHREAD_UPDATE_REELMAP_ALLDN_OFFLINE)
		{
			Sleep(100);
		}
	}

	WriteLastRmapInfo(nLastShot, nOffline);

	return TRUE;
}

void CGvisR2R_PunchView::ResetYield(int nOffline)
{
	BOOL bDualTest = nOffline & 0x02;

	if (pDoc->m_pReelMapUp)
		pDoc->m_pReelMapUp->ResetYield();
	if (bDualTest)
	{
		if (pDoc->m_pReelMapDn)
			pDoc->m_pReelMapDn->ResetYield();
		if (pDoc->m_pReelMapAllUp)
			pDoc->m_pReelMapAllUp->ResetYield();
		if (pDoc->m_pReelMapAllDn)
			pDoc->m_pReelMapAllDn->ResetYield();
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (pDoc->m_pReelMapInnerUp)
			pDoc->m_pReelMapInnerUp->ResetYield();

		if (pDoc->WorkingInfo.LastJob.bDualTestInner)
		{
			if (pDoc->m_pReelMapInnerDn)
				pDoc->m_pReelMapInnerDn->ResetYield();
			if (pDoc->m_pReelMapInnerAllUp)
				pDoc->m_pReelMapInnerAllUp->ResetYield();
			if (pDoc->m_pReelMapInnerAllDn)
				pDoc->m_pReelMapInnerAllDn->ResetYield();
		}

		if (pDoc->m_pReelMapIts)
			pDoc->m_pReelMapIts->ResetYield();
	}
}

BOOL CGvisR2R_PunchView::LoadPcrOnOffline(int nSerial, int nOffline)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.55"));
		return FALSE;
	}

	if (!pDoc->MakeMkDir())
		return FALSE;

	BOOL bDualTest = nOffline & 0x02;

	m_nSerialRmapUpdateOnOffline = nSerial;

	m_bTHREAD_LOAD_PCR_UP_OFFLINE = TRUE;
	if (bDualTest)
	{
		m_bTHREAD_LOAD_PCR_DN_OFFLINE = TRUE;
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::LoadPcrAllOnOffline(int nSerial, int nOffline)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.55"));
		return FALSE;
	}

	if (!pDoc->MakeMkDir())
		return FALSE;

	BOOL bDualTest = nOffline & 0x02;

	m_nSerialRmapUpdateOnOffline = nSerial;

	if (bDualTest)
	{
		m_bTHREAD_LOAD_PCR_ALLUP_OFFLINE = TRUE;
		m_bTHREAD_LOAD_PCR_ALLDN_OFFLINE = TRUE;
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::UpdateRMapOnOffline(int nSerial, int nOffline)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.56"));
		return FALSE;
	}

	if (!pDoc->MakeMkDir())
		return FALSE;

	BOOL bDualTest = nOffline & 0x02;

	m_nSerialRmapUpdateOnOffline = nSerial;

	m_bTHREAD_UPDATE_REELMAP_UP_OFFLINE = TRUE;
	if (bDualTest)
	{
		m_bTHREAD_UPDATE_REELMAP_DN_OFFLINE = TRUE;
	}

	return TRUE;
}

BOOL CGvisR2R_PunchView::UpdateRMapAllOnOffline(int nSerial, int nOffline)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.58"));
		return FALSE;
	}

	if (!pDoc->MakeMkDir())
		return FALSE;

	BOOL bDualTest = nOffline & 0x02;

	m_nSerialRmapUpdateOnOffline = nSerial;

	if (bDualTest)
	{
		m_bTHREAD_UPDATE_REELMAP_ALLUP_OFFLINE = TRUE;
		m_bTHREAD_UPDATE_REELMAP_ALLDN_OFFLINE = TRUE;
	}

	return TRUE;
}


void CGvisR2R_PunchView::DeleteYieldOnOffline()
{
	CFileFind findfile;
	CString sPath = _T("");

	if (pDoc->m_pReelMapUp)
		sPath = pDoc->m_pReelMapUp->GetRmapPathOnOffline(RMAP_UP);
	if (!sPath.IsEmpty())
	{
		if (findfile.FindFile(sPath))
			DeleteFile(sPath);
		sPath = _T("");
	}

	if (pDoc->m_pReelMapDn)
		sPath = pDoc->m_pReelMapDn->GetRmapPathOnOffline(RMAP_DN);
	if (!sPath.IsEmpty())
	{
		if (findfile.FindFile(sPath))
			DeleteFile(sPath);
		sPath = _T("");
	}

	if (pDoc->m_pReelMapAllUp)
		sPath = pDoc->m_pReelMapAllUp->GetRmapPathOnOffline(RMAP_ALLUP);
	if (!sPath.IsEmpty())
	{
		if (findfile.FindFile(sPath))
			DeleteFile(sPath);
		sPath = _T("");
	}

	if (pDoc->m_pReelMapAllDn)
		sPath = pDoc->m_pReelMapAllDn->GetRmapPathOnOffline(RMAP_ALLDN);
	if (!sPath.IsEmpty())
	{
		if (findfile.FindFile(sPath))
			DeleteFile(sPath);
		sPath = _T("");
	}
}

// RemakeReelmapFromPcr - End

void CGvisR2R_PunchView::StartThread()
{
	// ChkCollision
	if (!m_bThread[1])
		m_Thread[1].Start(GetSafeHwnd(), this, ThreadProc1);

	// DispDefImg
	if (!m_bThread[2])
		m_Thread[2].Start(GetSafeHwnd(), this, ThreadProc2);

	// UpdateYield
	if (!m_bThread[3])
		m_Thread[3].Start(GetSafeHwnd(), this, ThreadProc3);

	// DoShift2Mk
	if (!m_bThread[4])
		m_Thread[4].Start(GetSafeHwnd(), this, ThreadProc4);

	// Engrave Auto Sequence - Response Check
	if (!m_bThread[5])
		m_Thread[5].Start(GetSafeHwnd(), this, ThreadProc5);

	// UpdataeReelmapUp
	if (!m_bThread[6])
		m_Thread[6].Start(GetSafeHwnd(), this, ThreadProc6);

	// UpdataeReelmapAllUp
	if (!m_bThread[7])
		m_Thread[7].Start(GetSafeHwnd(), this, ThreadProc7);

	// UpdataeReelmapDn
	if (!m_bThread[8])
		m_Thread[8].Start(GetSafeHwnd(), this, ThreadProc8);

	// UpdataeReelmapAllDn
	if (!m_bThread[9])
		m_Thread[9].Start(GetSafeHwnd(), this, ThreadProc9);

	// UpdataeRstUp
	if (!m_bThread[10])
		m_Thread[10].Start(GetSafeHwnd(), this, ThreadProc10);

	// UpdataeRstDn
	if (!m_bThread[11])
		m_Thread[11].Start(GetSafeHwnd(), this, ThreadProc11);

	// UpdataeRstAllUp
	if (!m_bThread[12])
		m_Thread[12].Start(GetSafeHwnd(), this, ThreadProc12);

	// UpdataeRstAllDn
	if (!m_bThread[13])
		m_Thread[13].Start(GetSafeHwnd(), this, ThreadProc13);

	// UpdataeRstUp
	if (!m_bThread[14])
		m_Thread[14].Start(GetSafeHwnd(), this, ThreadProc14);

	// UpdataeRstDn
	if (!m_bThread[15])
		m_Thread[15].Start(GetSafeHwnd(), this, ThreadProc15);

	// UpdataeRstAllUp
	if (!m_bThread[16])
		m_Thread[16].Start(GetSafeHwnd(), this, ThreadProc16);

	// UpdataeRstAllDn
	if (!m_bThread[17])
		m_Thread[17].Start(GetSafeHwnd(), this, ThreadProc17);

	// UpdataeReelmapITS
	if (!m_bThread[18])
		m_Thread[18].Start(GetSafeHwnd(), this, ThreadProc18);

	// DispDefImgInner
	if (!m_bThread[19])
		m_Thread[19].Start(GetSafeHwnd(), this, ThreadProc19);

	// UpdateRstIts
	if (!m_bThread[20])
		m_Thread[20].Start(GetSafeHwnd(), this, ThreadProc20);

	// ReloadReelmapUpInner
	if (!m_bThread[21])
		m_Thread[21].Start(GetSafeHwnd(), this, ThreadProc21);

	// ReloadReelmapDnInner
	if (!m_bThread[22])
		m_Thread[22].Start(GetSafeHwnd(), this, ThreadProc22);

	// ReloadReelmapAllUpInner
	if (!m_bThread[23])
		m_Thread[23].Start(GetSafeHwnd(), this, ThreadProc23);

	// ReloadReelmapAllUpInner
	if (!m_bThread[24])
		m_Thread[24].Start(GetSafeHwnd(), this, ThreadProc24);

	// ReloadReelmapIts
	if (!m_bThread[25])
		m_Thread[25].Start(GetSafeHwnd(), this, ThreadProc25);

	// UpdataeReelmapUp
	if (!m_bThread[26])
		m_Thread[26].Start(GetSafeHwnd(), this, ThreadProc26);

	// UpdataeReelmapAllUp
	if (!m_bThread[27])
		m_Thread[27].Start(GetSafeHwnd(), this, ThreadProc27);

	// UpdataeReelmapDn
	if (!m_bThread[28])
		m_Thread[28].Start(GetSafeHwnd(), this, ThreadProc28);

	// UpdataeReelmapAllDn
	if (!m_bThread[29])
		m_Thread[29].Start(GetSafeHwnd(), this, ThreadProc29);

	// UpdataeYieldUp
	if (!m_bThread[30])
		m_Thread[30].Start(GetSafeHwnd(), this, ThreadProc30);

	// UpdataeYieldDn
	if (!m_bThread[31])
		m_Thread[31].Start(GetSafeHwnd(), this, ThreadProc31);

	// UpdataeYieldAllUp
	if (!m_bThread[32])
		m_Thread[32].Start(GetSafeHwnd(), this, ThreadProc32);

	// UpdataeYieldAllDn
	if (!m_bThread[33])
		m_Thread[33].Start(GetSafeHwnd(), this, ThreadProc33);

	// UpdataeYieldInnerUp
	if (!m_bThread[34])
		m_Thread[34].Start(GetSafeHwnd(), this, ThreadProc34);

	// UpdataeYieldInnerDn
	if (!m_bThread[35])
		m_Thread[35].Start(GetSafeHwnd(), this, ThreadProc35);

	// UpdataeYieldInnerAllUp
	if (!m_bThread[36])
		m_Thread[36].Start(GetSafeHwnd(), this, ThreadProc36);

	// UpdataeYieldInnerAllDn
	if (!m_bThread[37])
		m_Thread[37].Start(GetSafeHwnd(), this, ThreadProc37);

	// UpdataeYieldIts
	if (!m_bThread[38])
		m_Thread[38].Start(GetSafeHwnd(), this, ThreadProc38);

	// MakeItsFileUp
	if (!m_bThread[39])
		m_Thread[39].Start(GetSafeHwnd(), this, ThreadProc39);

	// MakeItsFileDn
	if (!m_bThread[40])
		m_Thread[40].Start(GetSafeHwnd(), this, ThreadProc40);
}

void CGvisR2R_PunchView::ReloadYield()
{
	int nLastShot = _tstoi(pDoc->WorkingInfo.LastJob.sSerialUp);
	CString sLastModel = pDoc->WorkingInfo.LastJob.sModel;
	UpdateYield(nLastShot);
}


BOOL CGvisR2R_PunchView::IsDoneReloadYield()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bDualTestInner = pDoc->WorkingInfo.LastJob.bDualTestInner;

	BOOL bRtn[5] = { 0 };
	bRtn[0] = m_bTHREAD_UPDATE_YIELD_UP;

	if (bDualTest)
	{
		bRtn[1] = m_bTHREAD_UPDATE_YIELD_DN;
		bRtn[2] = m_bTHREAD_UPDATE_YIELD_ALLUP;
		bRtn[3] = m_bTHREAD_UPDATE_YIELD_ALLDN;
	}
	else
	{
		bRtn[1] = FALSE;
		bRtn[2] = FALSE;
		bRtn[3] = FALSE;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		BOOL bRtnInner[5] = { 0 };
		bRtnInner[0] = m_bTHREAD_UPDATE_YIELD_INNER_UP;

		if (bDualTestInner)
		{
			bRtnInner[1] = m_bTHREAD_UPDATE_YIELD_INNER_DN;
			bRtnInner[2] = m_bTHREAD_UPDATE_YIELD_INNER_ALLUP;
			bRtnInner[3] = m_bTHREAD_UPDATE_YIELD_INNER_ALLDN;
		}

		if (bRtnInner[0] || bRtnInner[1] || bRtnInner[2] || bRtnInner[3])
			return FALSE;
	}

	if (bRtn[0] || bRtn[1] || bRtn[2] || bRtn[3])
		return FALSE;

	return TRUE;
}

void CGvisR2R_PunchView::InitVoiceCoil(int nCam)
{
	if (m_pVoiceCoil[nCam])
	{
		m_pVoiceCoil[nCam]->SetMarkShiftData(nCam);
		Sleep(100);
		m_pVoiceCoil[nCam]->SetMarkFinalData(nCam);
		Sleep(100);
	}
}

BOOL CGvisR2R_PunchView::IsSetPinPos()
{
	if (m_pMotion)
	{
		CString sMsg;
		double dPinPosX[2], dPinPosY[2];
		m_pMotion->GetPinPos(0, dPinPosX[0], dPinPosY[0]);
		m_pMotion->GetPinPos(1, dPinPosX[1], dPinPosY[1]);
		if (dPinPosX[0] < 1.0 && dPinPosY[0] < 1.0)
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			sMsg.Format(_T("좌측 핀위치가 설정되지 않았습니다."));
			Sleep(1000);
			ClrDispMsg();
			MsgBox(sMsg);
			return FALSE;
		}
		if (dPinPosX[1] < 1.0 && dPinPosY[1] < 1.0)
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			sMsg.Format(_T("우측 핀위치가 설정되지 않았습니다."));
			Sleep(1000);
			ClrDispMsg();
			MsgBox(sMsg);
			return FALSE;
		}
	}
	else
		return FALSE;

	return TRUE;
}

BOOL CGvisR2R_PunchView::SetMkPcs0(int nSerial, int nMkPcs) // pcr 시리얼, pcr 불량 피스 마킹 순서 인덱스
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.47"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nDefPcsId;
	int nIdx = pDoc->GetPcrIdx0(nSerial);
	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn	
		{
			if (pDoc->m_pPcr[2][nIdx])
			{
				nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
				if (pDoc->m_pReelMapAllUp)
					pDoc->m_pReelMapAllUp->SetPcsMkOut(0, nDefPcsId);
				else
					return FALSE;
			}
			else
				return FALSE;
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[0][nIdx])
			{
				nDefPcsId = pDoc->m_pPcr[0][nIdx]->m_pDefPcs[nMkPcs];
				if (pDoc->m_pReelMapUp)
					pDoc->m_pReelMapUp->SetPcsMkOut(0, nDefPcsId);
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}

	if(m_pDlgMenu01)
		m_pDlgMenu01->RefreshRmap();

	return TRUE;
}

BOOL CGvisR2R_PunchView::SetMkPcs1(int nSerial, int nMkPcs) // pcr 시리얼, pcr 불량 피스 마킹 순서 인덱스
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.50"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nDefPcsId;
	int nIdx = pDoc->GetPcrIdx1(nSerial);
	if (bDualTest)
	{
		if (pDoc->m_pPcr[2])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn	
		{
			if (pDoc->m_pPcr[2][nIdx])
			{
				nDefPcsId = pDoc->m_pPcr[2][nIdx]->m_pDefPcs[nMkPcs];
				if (pDoc->m_pReelMapAllUp)
					pDoc->m_pReelMapAllUp->SetPcsMkOut(1, nDefPcsId);
				else
					return FALSE;
			}
			else
				return FALSE;
		}
	}
	else
	{
		if (pDoc->m_pPcr[0])	// [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		{
			if (pDoc->m_pPcr[0][nIdx])
			{
				nDefPcsId = pDoc->m_pPcr[0][nIdx]->m_pDefPcs[nMkPcs];
				if (pDoc->m_pReelMapUp)
					pDoc->m_pReelMapUp->SetPcsMkOut(1, nDefPcsId);
			}
			else
				return FALSE;
		}
		else
			return FALSE;
	}

	if (m_pDlgMenu01)
		m_pDlgMenu01->RefreshRmap();

	return TRUE;
}

BOOL CGvisR2R_PunchView::ApplyListTorq()
{
	if (pDoc->WorkingInfo.System.sPathTorqSpecDir.IsEmpty())
		return FALSE;

	CString sPathList;
	sPathList.Format(_T("%sPunchingTorqueList.ini"), pDoc->WorkingInfo.System.sPathTorqSpecDir);

	CString sModel, sMsg;
	if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
		sModel = pDoc->m_sEngModel;
	else
		sModel = pDoc->WorkingInfo.LastJob.sModel;

	int nThick = GetModelThickness(sModel);
	if (nThick < 1)
	{
		sMsg.Format(_T("캠마스터의 토크 리스트에 %s 모델의 토크값이 설정되지 않았습니다.\r\n%s"), sModel, sPathList);
		MsgBox(sMsg);
		return FALSE;
	}

	int nUnit = _tstoi(pDoc->WorkingInfo.System.sMcName.Right(2));
	double dTorqL, dTorqR;
	CString sVal;
	if (!GetTorque(nUnit, nThick, dTorqL, dTorqR))
	{
		sMsg.Format(_T("캠마스터의 토크 리스트에 %s 설비의 %s 에대한 토크값이 설정되지 않았습니다.\r\n"), pDoc->WorkingInfo.System.sMcName, GetThicknessName(nThick));
		MsgBox(sMsg);
		return FALSE;
	}
	
	if (dTorqL <= 0.0 || dTorqR <= 0.0)
	{
		sMsg.Format(_T("캠마스터의 토크 리스트에 %s 설비의 %s 에대한 토크값이 설정되지 않았습니다.\r\n좌측 토크 : %.3f, 우측 토크 : %.3f"), pDoc->WorkingInfo.System.sMcName, GetThicknessName(nThick), dTorqL, dTorqR);
		MsgBox(sMsg);
		return FALSE;
	}

	if(!ApplyTorq(dTorqL, dTorqR))
	{
		sMsg.Format(_T("%s 에대한 토크값이 %s 설비에 적용되지 않았습니다.\r\n - 토크값 적용 실패."), GetThicknessName(nThick), pDoc->WorkingInfo.System.sMcName);
		MsgBox(sMsg);
		return FALSE;
	}

	//if (m_pDlgMenu04)
	//	m_pDlgMenu04->Disp();

	return TRUE;
}

BOOL CGvisR2R_PunchView::ApplyTorq(double dTorqL, double dTorqR)
{
	pDoc->SetMarkingToq1(dTorqL);

	if (pView->m_pVoiceCoil[0])
		pView->m_pVoiceCoil[0]->SetMarkFinalData(0);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetMkFnlTqLf();	//_ItemInx::_MkFnlTqLf
#endif
	Sleep(100);

	pDoc->SetMarkingToq2(dTorqR);

	if (pView->m_pVoiceCoil[1])
		pView->m_pVoiceCoil[1]->SetMarkFinalData(1);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetMkFnlTqRt();	//_ItemInx::_MkFnlTqRt
#endif

	return TRUE;
}

CString CGvisR2R_PunchView::GetThicknessName(int nIdx) // 설정없음[0], 얇은 두께[1], 중간 두께[2], 두꺼운 두께[3]
{
	CString sName = _T("");
	switch (nIdx)
	{
	case 0:
		sName = THICKNESS_0;
		break;
	case 1:
		sName = THICKNESS_1;
		break;
	case 2:
		sName = THICKNESS_2;
		break;
	case 3:
		sName = THICKNESS_3;
		break;
	default:
		sName = THICKNESS_0;
		break;
	}
	return sName;
}

BOOL CGvisR2R_PunchView::GetTorque(int nUnit, int nThick, double &dTorqL, double &dTorqR)
{
	if (pDoc->WorkingInfo.System.sPathTorqSpecDir.IsEmpty())
		return FALSE;

	CString sPathList;
	sPathList.Format(_T("%sPunchingTorqueList.ini"), pDoc->WorkingInfo.System.sPathTorqSpecDir);

	int nTotalUnit;
	TCHAR szData[MAX_PATH];
	TCHAR sep[] = { _T(",;\r\n\t") };
	TCHAR *token;

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Machines"), NULL, szData, sizeof(szData), sPathList))
		 nTotalUnit = _ttoi(szData);
	else
	{
		nTotalUnit = 0;
		return FALSE;
	}

	CString sThickIdx, sUnitNum;
	sThickIdx.Format(_T("%d"), nThick);

	sUnitNum.Format(_T("%d"), nUnit);
	if (0 < ::GetPrivateProfileString(sThickIdx, sUnitNum, NULL, szData, sizeof(szData), sPathList))
	{
		token = _tcstok(szData, sep);
		dTorqL = _ttof(token);
		token = _tcstok(NULL, sep);
		dTorqR = _ttof(token);
	}
	else
		return FALSE;

	return TRUE;;
}

int CGvisR2R_PunchView::GetModelThickness(CString sModel)
{
	if (pDoc->WorkingInfo.System.sPathTorqSpecDir.IsEmpty())
		return -1; // Not found.

	CString sPathList;
	sPathList.Format(_T("%sPunchingTorqueList.ini"), pDoc->WorkingInfo.System.sPathTorqSpecDir);

	int nModelIdx, nThickness;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	nModelIdx = SearchModel(sModel);
	if (nModelIdx < 1)
		return -1; // Not found.

	sModelIdx.Format(_T("%d"), nModelIdx);
	if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), sPathList))
	{
		token = _tcstok(szData, sep);
		sListModel = CString(token);
		token = _tcstok(NULL, sep);
		nThickness = _ttoi(token);
	}
	else
		return -1; // Not found.

	return nThickness;
}

int CGvisR2R_PunchView::SearchModel(CString sModel)
{
	if (pDoc->WorkingInfo.System.sPathTorqSpecDir.IsEmpty())
		return -1; // Not found.

	CString sPathList;
	sPathList.Format(_T("%sPunchingTorqueList.ini"), pDoc->WorkingInfo.System.sPathTorqSpecDir);

	int nModelIdx, nThickness, nTotalModels;
	CString sModelIdx, sListModel;
	TCHAR szData[MAX_PATH];
	TCHAR *token;
	TCHAR sep[] = { _T(",;\r\n\t") };

	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Total_Models"), NULL, szData, sizeof(szData), sPathList))
		nTotalModels = _ttoi(szData);
	else
		nTotalModels = 0;

	if (nTotalModels < 1)
		return -1; // Not found.

	for (nModelIdx = 0; nModelIdx < nTotalModels; nModelIdx++)
	{
		sModelIdx.Format(_T("%d"), nModelIdx);
		if (0 < ::GetPrivateProfileString(_T("Model"), sModelIdx, NULL, szData, sizeof(szData), sPathList))
		{
			token = _tcstok(szData, sep);
			sListModel = CString(token);
			token = _tcstok(NULL, sep);
			nThickness = _ttoi(token);
		}
		else
			sListModel = _T("");

		if (sListModel == sModel)
			return nModelIdx;
	}

	return -1; // Not Found
}

void CGvisR2R_PunchView::Mk4PtShift2Mk() // MODE_INNER
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sRst, sMsg;
	int a, b, nSerial, nPrevSerial;
	BOOL bChk = FALSE;

	if ((m_bMkSt[0] || m_bMkSt[1]) && IsBuffer())
	{
		switch (m_nMkStAuto)
		{
		case MK_ST + (Mk4PtIdx::Shift2Mk) :
			if (!m_bTHREAD_UPDATAE_YIELD[0] && !m_bTHREAD_UPDATAE_YIELD[1])
			{
				if (!m_bTHREAD_UPDATE_YIELD_UP && !m_bTHREAD_UPDATE_YIELD_DN && !m_bTHREAD_UPDATE_YIELD_INNER_UP && !m_bTHREAD_UPDATE_YIELD_INNER_DN
					&& !m_bTHREAD_UPDATE_YIELD_ALLUP && !m_bTHREAD_UPDATE_YIELD_ALLDN && !m_bTHREAD_UPDATE_YIELD_INNER_ALLUP && !m_bTHREAD_UPDATE_YIELD_INNER_ALLDN)	// Left Shot의 수율과 Right Shot의 수율을 업데이트함.
				{
					UpdateYield(); // Cam[0],  Cam[1]
					m_nMkStAuto++;
				}
			}
										  break;

		case MK_ST + (Mk4PtIdx::Shift2Mk) + 1:
			if (!m_bTHREAD_UPDATAE_YIELD[0] && !m_bTHREAD_UPDATAE_YIELD[1])
			{
				if (!m_bTHREAD_UPDATE_REELMAP_UP && !m_bTHREAD_UPDATE_REELMAP_DN && !m_bTHREAD_UPDATE_REELMAP_ALLUP && !m_bTHREAD_UPDATE_REELMAP_ALLDN)
				{
					if (!m_bTHREAD_UPDATE_YIELD_UP && !m_bTHREAD_UPDATE_YIELD_DN && !m_bTHREAD_UPDATE_YIELD_INNER_UP && !m_bTHREAD_UPDATE_YIELD_INNER_DN
						&& !m_bTHREAD_UPDATE_YIELD_ALLUP && !m_bTHREAD_UPDATE_YIELD_ALLDN && !m_bTHREAD_UPDATE_YIELD_INNER_ALLUP && !m_bTHREAD_UPDATE_YIELD_INNER_ALLDN)	// Left Shot의 수율과 Right Shot의 수율을 업데이트함.
					{
						UpdateRst();
						UpdateWorking();	// Update Working Info...
											//pDoc->UpdateYieldOnRmap(); // 20241115
						m_nMkStAuto++;
					}
					else
						Sleep(100);
				}
				else
					Sleep(100);
			}
			break;

		case MK_ST + (Mk4PtIdx::Shift2Mk) + 2:
			MpeWrite(Plc.DlgMenu01.MarkingDoing, 0);	// 마킹부 마킹중 ON (PC가 ON, OFF)
			MpeWrite(Plc.DlgMenu01.MarkingDone, 1);	// 마킹완료(PLC가 확인하고 Reset시킴.)-20141029
			pDoc->LogAuto(_T("PC: 마킹완료(PLC가 확인하고 Reset시킴.)"));
			m_nMkStAuto++;
			break;

		case MK_ST + (Mk4PtIdx::Shift2Mk) + 3:
			bChk = MpeRead(_T("MB400243")); // PLC 마킹 피딩 완료
											//if (pDoc->m_pMpeSignal && pDoc->m_pMpeSignal[4] & (0x01 << 3))	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
			if (bChk)	// 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)-20141030
			{
				pDoc->LogAuto(_T("PLC: 마킹부 Feeding완료(PLC가 On시키고 PC가 확인하고 Reset시킴.)"));
				m_nMkStAuto++;
			}
			break;

		case MK_ST + (Mk4PtIdx::Shift2Mk) + 4:
			if (!m_bTHREAD_SHIFT2MK)
			{
				pDoc->LogAuto(_T("PC: 마킹부 Feeding완료 OFF"));
				MpeWrite(Plc.DlgMenu01.FeedingDonePunch, 0);	// 마킹부 Feeding완료
				m_bShift2Mk = TRUE;
				DoShift2Mk();

				SetMkFdLen();
				SetCycTime();
				m_dwCycSt = GetTickCount();
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Shift2Mk) + 5:
			if (!m_bTHREAD_UPDATAE_YIELD[0] && !m_bTHREAD_UPDATAE_YIELD[1])
			{
				//if (!m_bTHREAD_SHIFT2MK && !m_bTHREAD_REELMAP_YIELD_UP && !m_bTHREAD_REELMAP_YIELD_DN && !m_bTHREAD_REELMAP_YIELD_ALLUP && !m_bTHREAD_REELMAP_YIELD_ALLDN) // Yield Reelmap
				if (!m_bTHREAD_SHIFT2MK)
				{
					if (!m_bTHREAD_SHIFT2MK && !m_bTHREAD_UPDATE_YIELD_UP && !m_bTHREAD_UPDATE_YIELD_DN && !m_bTHREAD_UPDATE_YIELD_INNER_UP && !m_bTHREAD_UPDATE_YIELD_INNER_DN
						&& !m_bTHREAD_UPDATE_YIELD_ALLUP && !m_bTHREAD_UPDATE_YIELD_ALLDN && !m_bTHREAD_UPDATE_YIELD_INNER_ALLUP && !m_bTHREAD_UPDATE_YIELD_INNER_ALLDN)	// Left Shot의 수율과 Right Shot의 수율을 업데이트함.
					{
						if (pDoc->GetTestMode() == MODE_OUTER)
						{
							if (m_bTHREAD_REELMAP_YIELD_ITS) // Yield Reelmap
								break;
						}

						m_nMkStAuto++;
						UpdateRst();
						UpdateWorking();	// Update Working Info...
						ChkYield();
					}
				}
			}
			break;
		case MK_ST + (Mk4PtIdx::Shift2Mk) + 6:
			if (!m_bTHREAD_SHIFT2MK)
			{
				SetListBuf();
				ChkLotCutPos();
				m_nMkStAuto++;
			}
			break;
		case MK_ST + (Mk4PtIdx::Shift2Mk) + 7:
			m_nMkStAuto++;
			break;
		case MK_ST + (Mk4PtIdx::Shift2Mk) + 8:
			m_nMkStAuto++;
			::WritePrivateProfileString(_T("Last Job"), _T("MkSt"), _T("0"), PATH_WORKING_INFO);
			break;
		case MK_ST + (Mk4PtIdx::Shift2Mk) + 9:
			m_bMkSt[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt0"), m_bMkSt[0]);
			m_bMkSt[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bMkSt1"), m_bMkSt[1]);
			break;
		}
	}
}
