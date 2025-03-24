#include "stdafx.h"
#include "Engrave.h"

#include "EngraveDef.h"
#include "../Global/GlobalDefine.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

BEGIN_MESSAGE_MAP(CEngrave, CWnd)
	//{{AFX_MSG_MAP(CEngrave)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_MESSAGE(WM_UPDATE_CONNECTION, OnUpdateConnection)
	ON_WM_TIMER()
	ON_MESSAGE(WM_CLIENT_RECEIVED, wmClientReceived)
	ON_MESSAGE(WM_CLIENT_CLOSED, wmClientClosed)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()


CEngrave::CEngrave(CString sAddrCli, CString sAddrSvr, CString sPortSvr, CWnd* pParent /*=NULL*/)
{
	int nMsgID;

	if(pParent)
		m_hParentWnd = pParent->GetSafeHwnd();

	m_bTIM_CHK_RCV_SIG = FALSE;
	for (nMsgID = 0; nMsgID < _SigInx::_EndIdx; nMsgID++)
	{
		m_bSendSig[nMsgID] = FALSE;
		m_bRcvSig[nMsgID] = FALSE;
		m_nSendSigData[nMsgID] = 0;
		m_nRcvSigData[nMsgID] = 0;
	}

	m_pClient = NULL;
	m_nServerID = ID_ENGRAVE;
	m_bWaitForResponse = FALSE;
	m_strResponse = _T("");
	m_nCmd = _None_;
	m_bCmdF = FALSE;
	m_bAutoConnect = TRUE;

	RECT rt = { 0,0,0,0 };
	Create(NULL, _T("None"), WS_CHILD, rt, FromHandle(m_hParentWnd), (UINT)this);

//#ifndef TEST_MODE
	StartClient(sAddrCli, sAddrSvr, sPortSvr);
	Sleep(10);
//#endif

	m_pThread = NULL;
	m_bTIM_CHECK_CONNECT = FALSE;
	//m_bTIM_MPE_OFFSET_INITPOS_MOVE = FALSE;

	m_bGetOpInfo = FALSE; m_bGetInfo = FALSE; m_bGetEngInfo = FALSE;
	m_bGetSignalMain = FALSE; m_bGetSignalTorqueMotor = FALSE; m_bGetSignalInductionMotor = FALSE; m_bGetSignalCore150mm = FALSE; m_bGetSignalEtc = FALSE;
	m_bGetSignalRecoiler = FALSE; m_bGetSignalPunch = FALSE; m_bGetSignalAOIDn = FALSE; m_bGetSignalAOIUp = FALSE; m_bGetSignalEngrave = FALSE; m_bGetSignalUncoiler = FALSE;
	m_bGetSignalEngraveAutoSequence = FALSE;
	m_bGetTotRatio = FALSE; m_bGetStTime = FALSE; m_bGetRunTime = FALSE; m_bGetEdTime = FALSE; m_bGetStripRatio = FALSE; m_bGetDef = FALSE;
	m_bGet2DReader = FALSE; m_bGetEngInfo = FALSE; m_bGetFdInfo = FALSE; m_bGetAoiInfo = FALSE; m_bGetMkInfo = FALSE; m_bGetMkInfoLf = FALSE; m_bGetMkInfoRt = FALSE;

	SetSignalName();
}


CEngrave::~CEngrave()
{
	m_bTIM_CHECK_CONNECT = FALSE;
	//m_bTIM_MPE_OFFSET_INITPOS_MOVE = FALSE;
	m_bTIM_CHK_RCV_SIG = FALSE;

	if (m_pClient)
	{
		m_bAutoConnect = FALSE;
		StopClient();
	}
}


///////////////////////////////////////////////////////////////////////////////
// OnUpdateConnection
// This message is sent by server manager to indicate connection status
LRESULT CEngrave::OnUpdateConnection(WPARAM wParam, LPARAM lParam)
{
	UINT uEvent = (UINT)wParam;
	CEngrave* pClient = reinterpret_cast<CEngrave*>(lParam);

	if (pClient != NULL)
	{
		// Server socket is now connected, we need to pick a new one
		if (uEvent == EVT_CONSUCCESS)
		{
			//pGlobalDoc->m_bConnect = TRUE;
			m_bWaitForResponse = FALSE;
			//m_bTIM_CHECK_CONNECT = TRUE;
			//SetTimer(TIM_CHECK_CONNECT, DELAY_CHECK_CONNECT, NULL);
		}
		else if (uEvent == EVT_CONFAILURE || uEvent == EVT_CONDROP)
		{
			// Disconnect socket
			StopClient();
			// 자동접속시도
			if (uEvent == EVT_CONDROP && m_bAutoConnect)
				SetTimer(TIM_CONNECT, 9000, NULL);
		}
	}

	return 1L;
}


void CEngrave::SetHwnd(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
}

void CEngrave::StartClient(CString sAddrCli, CString sAddrSvr, CString sPortSvr)
{
	if (!m_pClient)
	{
		m_strAddrCli = sAddrCli;
		m_strAddrSvr = sAddrSvr;
		m_strPortSvr = sPortSvr;

		m_pClient = new CTcpIpClient(this);
		m_pClient->Init(sAddrCli, sAddrSvr, _tstoi(sPortSvr));
		m_pClient->SetServer(m_nServerID);
		m_pClient->Start();
	}
}

void CEngrave::StopClient()
{
	if (m_pClient)
	{
		m_pClient->Stop();
		Sleep(10);
		m_pClient->StopThread();
		Sleep(10);
		delete m_pClient;
		m_pClient = NULL;
		Sleep(10);
	}
}

LRESULT CEngrave::wmClientClosed(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;

	switch (nServerID)
	{
	case ID_ENGRAVE:
		if (m_pClient)
			m_pClient = NULL;
		break;
	}

	return (LRESULT)1;
}


SOCKET_DATA CEngrave::GetSocketData()
{
	return m_SocketData;
}

LRESULT CEngrave::wmClientReceived(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;

	if (nServerID == ID_ENGRAVE)
	{
		m_SocketData = m_pClient->GetSocketData();
	}


	return (LRESULT)OnClientReceived(wParam, lParam);
}

int CEngrave::OnClientReceived(WPARAM wParam, LPARAM lParam)
{
	int nServerID = (int)wParam;
	//SOCKET_DATA *pSocketData = (SOCKET_DATA*)lParam;
	SOCKET_DATA rSockData = m_SocketData;
	int nMsgID = rSockData.nMsgID;
	int nRequireRtn = rSockData.nData2;
	if (nRequireRtn != 1)
		nRequireRtn = 0;

	if (pView->m_pDlgMenu02)
		pView->m_pDlgMenu02->GetDlgItem(IDC_STATIC_SIGNAL_RCV)->SetWindowText(GetSignalName(nMsgID));

	GetSysSignal(rSockData);

	if (nMsgID == _SigInx::_MyMsgYes || nMsgID == _SigInx::_MyMsgNo || nMsgID == _SigInx::_MyMsgOk)
		return 1;

	// 받은 신호에 대해 상대방에게 신호 받은 것을 알림. (Hand Shaking)
	if (nMsgID < RCV_OFFSET)
	{
		SetSignal(nMsgID + RCV_OFFSET, 1);		// 1 : Return RCV_Signal, 0 : No Return
	}
	else
	{
		if (nRequireRtn)
		{
			SetSignal(nMsgID, 0);					// 1 : Return RCV_Signal, 0 : No Return
		}
		else
		{
			m_bSendSig[nMsgID] = FALSE;					// 최종 메시지 확인 후 재전송을 멈춤.
			m_bSendSig[nMsgID - RCV_OFFSET] = FALSE;	// 최종 메시지 확인 후 재전송을 멈춤.
			m_nSendSigData[nMsgID] = 0;					// 최종 메시지 확인 후 재전송을 멈춤.
			m_nSendSigData[nMsgID - RCV_OFFSET] = 0;	// 최종 메시지 확인 후 재전송을 멈춤.
		}
	}
	
	if (m_hParentWnd)
		::PostMessage(m_hParentWnd, WM_CLIENT_RECEIVED, (WPARAM)ID_ENGRAVE, (LPARAM)&m_SocketData); // "OrderNum-ShotNum" (9bytes'-'3bytes)
	m_bWaitForResponse = FALSE;
	return 1;
}

BOOL CEngrave::CheckResponse(int nCmd, CString sResponse)
{
	int nPos;
	CString sParsing;

	switch (nCmd)
	{
		;
	}

	return FALSE;
}


BOOL CEngrave::IsRunning()
{
	return (m_bWaitForResponse);
}

BOOL CEngrave::ReadComm(CString &sData)
{
	if (!m_bWaitForResponse)
	{
		sData = m_strResponse;
		return TRUE;
	}

	return FALSE;
}

int CEngrave::WriteComm(CString sMsg, DWORD dwTimeout)
{
	return m_pClient->WriteComm(sMsg);
}


// Thread
void CEngrave::StartThread() // Worker Thread 구동관련 Step8
{
	if (m_pThread == NULL)
	{
		m_bModify = TRUE;
		m_evtThread.ResetEvent();
		m_pThread = AfxBeginThread(RunThread, this);
		if (m_pThread)
			m_hThread = m_pThread->m_hThread;
	}
}

void CEngrave::StopThread() // Worker Thread 구동관련 Step9
{
	if (m_pThread != NULL)
	{
		m_evtThread.SetEvent();
		WaitUntilThreadEnd(m_hThread);
	}
	m_pThread = NULL;
	m_bModify = FALSE;
}

void CEngrave::WaitUntilThreadEnd(HANDLE hThread) // Worker Thread 구동관련 Step6
{
	TRACE("WaitUntilThreadEnd(0x%08x:RunThread) Entering\n", hThread);
	MSG message;
	const DWORD dwTimeOut = 500000;
	DWORD dwStartTick = GetTickCount();
	DWORD dwExitCode;
	while (GetExitCodeThread(hThread, &dwExitCode) && dwExitCode == STILL_ACTIVE && m_bAlive) {
		// Time Out Check
		if (GetTickCount() >= (dwStartTick + dwTimeOut))
		{
			pView->ClrDispMsg();
			AfxMessageBox(_T("WaitUntilThreadEnd() Time Out!!!", NULL, MB_OK | MB_ICONSTOP));
			return;
		}
		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
	TRACE("WaitUntilThreadEnd(0x%08x:DispMsgThread) Exit\n", hThread);
}

UINT CEngrave::RunThread(LPVOID pParam)
{
	CEngrave* pMk = (CEngrave*)pParam;
	DWORD dwMilliseconds = 10;
	pMk->m_bAlive = TRUE;

	MSG message;
	CString sMsg;

	while (WAIT_OBJECT_0 != ::WaitForSingleObject(pMk->m_evtThread, dwMilliseconds))
	{
		Sleep(100);
	}

	pMk->m_bAlive = FALSE;
	return 0;
}

void CEngrave::Close()
{
	StopThread();
	Sleep(10);
}


// General Function

BOOL CEngrave::SendCommand(SOCKET_DATA SocketData, BOOL bWait)
{
	if (!m_pClient)
		return FALSE;

	BOOL bRtn = TRUE;
	SocketData.nTxPC = _Punch;		// Client
	SocketData.nRxPC = _Engrave;	// Server
	m_pClient->WriteCommData(SocketData, INFINITE);

	if (bWait)
	{
		m_bWaitForResponse = TRUE;
		bRtn = WaitResponse();
	}

	return bRtn;
}

BOOL CEngrave::WaitResponse()
{
	MSG message;
	DWORD dwStartTick = GetTickCount();

	while (IsRunning())
	{
		if (GetTickCount() - dwStartTick > DELAY_RESPONSE)
		{
			// Disconnect socket
			//AfxMessageBox(_T("WaitResponse() Time Out!!!"));
			m_pClient->SetConnectedStatus(FALSE);
			return FALSE;
		}

		if (::PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
		Sleep(30);
	}

	return TRUE;
}

BOOL CEngrave::IsConnected()
{
	if (!m_pClient)
		return FALSE;

	return m_pClient->IsConnected();
}

void CEngrave::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nMsgID = 0;
	SOCKET_DATA SocketData;

	switch (nIDEvent)
	{
	case TIM_CONNECT:
		KillTimer(TIM_CONNECT);
		SetTimer(TIM_CONNECT + 1, 1000, NULL);
		break;
	case TIM_CONNECT + 1:
		KillTimer(TIM_CONNECT + 1);
#ifndef TEST_MODE
		StartClient(m_strAddrCli, m_strAddrSvr, m_strPortSvr);
#endif
		SetTimer(TIM_CONNECT + 2, 1000, NULL);
		break;
	case TIM_CONNECT + 2:
		KillTimer(TIM_CONNECT + 2);
		if(!IsConnected())
			SetTimer(TIM_CONNECT + 1, 1000, NULL);
		break;
	case TIM_CHECK_CONNECT:
		KillTimer(TIM_CHECK_CONNECT);
		if (m_pClient && m_bTIM_CHECK_CONNECT)
		{
			if(!IsRunning())
			{
				SocketData.nCmdCode = _GetSig;
				SocketData.nMsgID = _SigInx::_Connect;
				SendCommand(SocketData);
			}
			if (m_bTIM_CHECK_CONNECT)
				SetTimer(TIM_CHECK_CONNECT, DELAY_CHECK_CONNECT, NULL);
		}
		break;
	case TIM_CHK_RCV_SIG:
		KillTimer(TIM_CHK_RCV_SIG);
		for (nMsgID = 0; nMsgID < _SigInx::_EndIdx; nMsgID++)
		{
			if (m_bSendSig[nMsgID])		// 상대방이 받지 못하면 500mSec 후에 다시 신호를 전송함.
			{
				SetSignal(nMsgID, m_nSendSigData[nMsgID]);		// 1 : Return RCV_Signal, 0 : No Return
				if (!m_nSendSigData[nMsgID])
					m_bSendSig[nMsgID] = FALSE;
			}
		}
		if (m_bTIM_CHK_RCV_SIG)
			SetTimer(TIM_CHK_RCV_SIG, 500, NULL);
		break;

	}
	CWnd::OnTimer(nIDEvent);
}

// Communcation

void CEngrave::GetSysSignal(SOCKET_DATA SockData)
{
	GetOpInfo(SockData);
	GetInfo(SockData);
	GetEngInfo(SockData);

	GetSignalEngraveAutoSequence(SockData);
	GetSignalMyMsg(SockData);

	GetSignal2dEng(SockData);
	GetCurrentInfoSignal(SockData);
}

void CEngrave::GetSignalDisp(SOCKET_DATA SockData)
{
}


void CEngrave::GetSignalMain(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalTorqueMotor(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalInductionMotor(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalCore150mm(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalEtc(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalRecoiler(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalPunch(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalAOIDn(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalAOIUp(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalEngrave(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalUncoiler(SOCKET_DATA SockData)
{
}

void CEngrave::GetSignalEngraveAutoSequence(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_EngAutoInit:
			//pDoc->BtnStatus.EngAuto.Init = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.EngAuto.Init = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoInit);
			m_bRcvSig[_SigInx::_EngAutoInit] = TRUE;
			break;
		case _SigInx::_EngAutoSeqMkSt:
			//pDoc->BtnStatus.EngAuto.MkSt = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.EngAuto.MkSt = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqMkSt);
			m_bRcvSig[_SigInx::_EngAutoSeqMkSt] = TRUE;
			break;
		case _SigInx::_EngAutoSeqOnMkIng:
			//pDoc->BtnStatus.EngAuto.OnMking = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.EngAuto.MkSt = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqOnMkIng);
			m_bRcvSig[_SigInx::_EngAutoSeqOnMkIng] = TRUE;
			break;
		case _SigInx::_EngAutoSeqMkDone:
			//pDoc->BtnStatus.EngAuto.MkDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.EngAuto.MkDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqMkDone);
			m_bRcvSig[_SigInx::_EngAutoSeqMkDone] = TRUE;
			break;
		case _SigInx::_EngAutoSeq2dReadSt:
			//pDoc->BtnStatus.EngAuto.Read2dSt = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.EngAuto.Read2dSt = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadSt);
			m_bRcvSig[_SigInx::_EngAutoSeq2dReadSt] = TRUE;
			break;
		case _SigInx::_EngAutoSeqOnReading2d:
			//pDoc->BtnStatus.EngAuto.OnRead2d = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.EngAuto.OnRead2d = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeqOnReading2d);
			m_bRcvSig[_SigInx::_EngAutoSeqOnReading2d] = TRUE;
			break;
		case _SigInx::_EngAutoSeq2dReadDone:
			//pDoc->BtnStatus.EngAuto.Read2dDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.EngAuto.Read2dDone = pDoc->GetCurrentInfoSignal(_SigInx::_EngAutoSeq2dReadDone);
			m_bRcvSig[_SigInx::_EngAutoSeq2dReadDone] = TRUE;
			break;
		case _SigInx::_MyMsg:
			pDoc->m_sEngAlarm = CharToString(SockData.strData);
			m_bRcvSig[_SigInx::_MyMsg] = TRUE;
			break;
			// Is
		case _SigInx::_IsEngAutoInit:
			//pDoc->BtnStatus.EngAuto.IsInit = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.EngAuto.IsInit = pDoc->GetCurrentInfoSignal(_SigInx::_IsEngAutoInit);
			m_bRcvSig[_SigInx::_IsEngAutoInit] = TRUE;
			break;
		case _SigInx::_IsEngAutoSeqMkSt:
			//pDoc->BtnStatus.EngAuto.IsMkSt = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.EngAuto.IsMkSt = pDoc->GetCurrentInfoSignal(_SigInx::_IsEngAutoSeqMkSt);
			m_bRcvSig[_SigInx::_IsEngAutoSeqMkSt] = TRUE;
			break;
		case _SigInx::_IsEngAutoSeqOnMkIng:
			//pDoc->BtnStatus.EngAuto.IsOnMking = (SockData.nData1 > 0) ? TRUE : FALSE; pDoc->SetStatus(_T("Engrave"), _T("IsOnMking"), pDoc->BtnStatus.EngAuto.IsOnMking);
			pDoc->BtnStatus.EngAuto.IsOnMking = pDoc->GetCurrentInfoSignal(_SigInx::_IsEngAutoSeqOnMkIng);
			m_bRcvSig[_SigInx::_IsEngAutoSeqOnMkIng] = TRUE;
			break;
		case _SigInx::_IsEngAutoSeqMkDone:
			//pDoc->BtnStatus.EngAuto.IsMkDone = (SockData.nData1 > 0) ? TRUE : FALSE; pDoc->SetStatus(_T("Engrave"), _T("IsMkDone"), pDoc->BtnStatus.EngAuto.IsMkDone);
			pDoc->BtnStatus.EngAuto.IsMkDone = pDoc->GetCurrentInfoSignal(_SigInx::_IsEngAutoSeqMkDone);
			m_bRcvSig[_SigInx::_IsEngAutoSeqMkDone] = TRUE;
			break;
		case _SigInx::_IsEngAutoSeq2dReadSt:
			//pDoc->BtnStatus.EngAuto.IsRead2dSt = (SockData.nData1 > 0) ? TRUE : FALSE;
			pDoc->BtnStatus.EngAuto.IsRead2dSt = pDoc->GetCurrentInfoSignal(_SigInx::_IsEngAutoSeq2dReadSt);
			m_bRcvSig[_SigInx::_IsEngAutoSeq2dReadSt] = TRUE;
			break;
		case _SigInx::_IsEngAutoSeqOnReading2d:
			//pDoc->BtnStatus.EngAuto.IsOnRead2d = (SockData.nData1 > 0) ? TRUE : FALSE; pDoc->SetStatus(_T("Engrave"), _T("IsOnRead2d"), pDoc->BtnStatus.EngAuto.IsOnRead2d);
			pDoc->BtnStatus.EngAuto.IsOnRead2d = pDoc->GetCurrentInfoSignal(_SigInx::_IsEngAutoSeqOnReading2d);
			m_bRcvSig[_SigInx::_IsEngAutoSeqOnReading2d] = TRUE;
			break;
		case _SigInx::_IsEngAutoSeq2dReadDone:
			//pDoc->BtnStatus.EngAuto.IsRead2dDone = (SockData.nData1 > 0) ? TRUE : FALSE; pDoc->SetStatus(_T("Engrave"), _T("IsRead2dDone"), pDoc->BtnStatus.EngAuto.IsRead2dDone);
			pDoc->BtnStatus.EngAuto.IsRead2dDone = pDoc->GetCurrentInfoSignal(_SigInx::_IsEngAutoSeq2dReadDone);
			m_bRcvSig[_SigInx::_IsEngAutoSeq2dReadDone] = TRUE;
			break;
		}
	}
}

void CEngrave::GetSignalMyMsg(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_MyMsgYes:
			pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgYes, FALSE);
			//m_bRcvSig[_SigInx::_MyMsgYes] = TRUE;
			//pView->SetMyMsgYes();
			//Sleep(500);
			break;
		case _SigInx::_MyMsgNo:
			pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgNo, FALSE);
			//m_bRcvSig[_SigInx::_MyMsgNo] = TRUE;
			//pView->SetMyMsgNo();
			//Sleep(500);
			break;
		case _SigInx::_MyMsgOk:
			pDoc->SetCurrentInfoSignal(_SigInx::_MyMsgOk, FALSE);
			//m_bRcvSig[_SigInx::_MyMsgOk] = TRUE;
			//pView->SetMyMsgOk();
			//Sleep(500);
			break;
		case _SigInx::_ErrorRead2dCode:
			pView->ErrorRead2dCode(SockData.nData1); // 각인부에서 메시지처리 후 들어오는 곳
			break;
			// Is
		case _SigInx::_IsMyMsgYes:
			pDoc->BtnStatus.Msg.IsYes = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_IsMyMsgYes] = TRUE;
			break;
		case _SigInx::_IsMyMsgNo:
			pDoc->BtnStatus.Msg.IsNo = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_IsMyMsgNo] = TRUE;
			break;
		case _SigInx::_IsMyMsgOk:
			pDoc->BtnStatus.Msg.IsOk = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_IsMyMsgOk] = TRUE;
			break;
		}
	}
}

void CEngrave::GetCurrentInfoSignal(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_GetCurrentInfoSignal:
			pDoc->BtnStatus.EngAuto.GetCurrentInfoSignal = (SockData.nData1 > 0) ? TRUE : FALSE;
			m_bRcvSig[SockData.nData1] = TRUE;
			//m_bRcvSig[_SigInx::_GetCurrentInfoSignal] = TRUE;
			break;
		case _SigInx::_GetMonDispMainSignal:
			pDoc->BtnStatus.EngAuto.GetMonDispMainSignal = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_GetMonDispMainSignal] = TRUE;
			break;
			// Is
		case _SigInx::_IsGetCurrentInfoSignal:
			pDoc->BtnStatus.EngAuto.IsGetCurrentInfoSignal = (SockData.nData1 > 0) ? TRUE : FALSE;
			pView->m_bTIM_CHK_RCV_CURR_INFO_SIG = FALSE;
			//m_bRcvSig[_SigInx::_IsGetCurrentInfoSignal] = TRUE;
			break;
		case _SigInx::_IsGetMonDispMainSignal:
			pDoc->BtnStatus.EngAuto.IsGetMonDispMainSignal = (SockData.nData1 > 0) ? TRUE : FALSE;
			pView->m_bTIM_CHK_RCV_MON_DISP_MAIN_SIG = FALSE;
			//m_bRcvSig[_SigInx::_IsGetMonDispMainSignal] = TRUE;
			break;
		}
	}
}

// Start for GetSysData()

void CEngrave::GetSysData(SOCKET_DATA SockData)
{
	GetOpInfo(SockData);
	GetInfo(SockData);
	GetTotRatio(SockData);
	GetStTime(SockData);
	GetRunTime(SockData);
	GetEdTime(SockData);
	GetStripRatio(SockData);
	GetDef(SockData);
	Get2DReader(SockData);
	GetEngInfo(SockData);
	GetFdInfo(SockData);
	GetAoiInfo(SockData);
	GetMkInfo(SockData);
	GetMkInfoLf(SockData);
	GetMkInfoRt(SockData);
	GetAlarmMsg(SockData);
}

void CEngrave::GetOpInfo(SOCKET_DATA SockData)
{
	long lData;

	int nCmdCode = SockData.nCmdCode; // _SetSig or _SetData
	int nMsgId = SockData.nMsgID;

	CString sVal;
	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_DualTest:
			if (pDoc->WorkingInfo.LastJob.bDualTest != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->SetDualTest();
				//pDoc->WorkingInfo.LastJob.bDualTest = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_DualTest] = TRUE;
			}
			break;
		case _SigInx::_SampleTest:
			//if (pDoc->WorkingInfo.LastJob.bSampleTest != (SockData.nData1 > 0) ? TRUE : FALSE)
			//{
			//	m_bGetOpInfo = TRUE;
			//	pDoc->WorkingInfo.LastJob.bSampleTest = (SockData.nData1 > 0) ? TRUE : FALSE;
			//	//m_bRcvSig[_SigInx::_SampleTest] = TRUE;
			//}
			break;
		//case _SigInx::_TestMode:
		//	if(pDoc->GetTestMode() != (int)SockData.nData1) // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
		//	{
		//		m_bGetOpInfo = TRUE;
		//		pDoc->WorkingInfo.LastJob.nTestMode = (int)SockData.nData1; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
		//		m_bRcvSig[_SigInx::_TestMode] = TRUE;
		//	}
		//	break;
		case _SigInx::_RecoilerCcw:
			//if(pDoc->WorkingInfo.LastJob.bOneMetal != (SockData.nData1 > 0) ? TRUE : FALSE)	// OneMetal : TRUE -> SetTwoMetal(FALSE);
			//if(pDoc->BtnStatus.FeedDir.FeedCcwRecoiler != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->RecoilerCcw();
				//pDoc->BtnStatus.Induct.Rc = pDoc->WorkingInfo.LastJob.bOneMetal = (SockData.nData1 > 0) ? TRUE : FALSE;	// OneMetal : TRUE -> SetTwoMetal(FALSE);
				//pDoc->BtnStatus.FeedDir.FeedCcwRecoiler = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_RecoilerCcw] = TRUE;
			}
			break;
		case _SigInx::_UncoilerCcw:
			//if(pDoc->WorkingInfo.LastJob.bTwoMetal != (SockData.nData1 > 0) ? TRUE : FALSE)	// TwoMetal : TRUE -> SetTwoMetal(TRUE);
			//if(pDoc->BtnStatus.FeedDir.FeedCcwUncoiler != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->UncoilerCcw();
				//pDoc->BtnStatus.Induct.Uc = pDoc->WorkingInfo.LastJob.bTwoMetal = (SockData.nData1 > 0) ? TRUE : FALSE;	// TwoMetal : TRUE -> SetTwoMetal(TRUE);
				//pDoc->BtnStatus.FeedDir.FeedCcwUncoiler = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_UncoilerCcw] = TRUE;
			}
			break;
		case _SigInx::_AlignMethode:
			if(pDoc->WorkingInfo.LastJob.nAlignMethode != (int)SockData.nData1) // TWO_POINT, FOUR_POINT
			{
				m_bGetOpInfo = TRUE;
				pDoc->SetAlignMethode();
				//pDoc->WorkingInfo.LastJob.nAlignMethode = (int)SockData.nData1; // TWO_POINT, FOUR_POINT
				//m_bRcvSig[_SigInx::_AlignMethode] = TRUE;
			}
			break;
		case _SigInx::_DoorRecoiler:
			//if (pDoc->WorkingInfo.LastJob.bRclDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->SetDoorRecoiler();
				//pDoc->WorkingInfo.LastJob.bRclDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_DoorRecoiler] = TRUE;
			}
			break;
		case _SigInx::_DoorAoiUp:
			//if (pDoc->WorkingInfo.LastJob.bAoiUpDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->SetDoorAoiUp();
				//pDoc->WorkingInfo.LastJob.bAoiUpDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_DoorAoiUp] = TRUE;
			}
			break;
		case _SigInx::_DoorAoiDn:
			if (pDoc->WorkingInfo.LastJob.bAoiDnDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bAoiDnDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_DoorAoiDn] = TRUE;
			}
			break;
		case _SigInx::_DoorMk:
			if (pDoc->WorkingInfo.LastJob.bMkDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bMkDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_DoorMk] = TRUE;
			}
			break;
		case _SigInx::_DoorEngrave:
			if (pDoc->WorkingInfo.LastJob.bEngvDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bEngvDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_DoorEngrave] = TRUE;
			}
			break;

		case _SigInx::_DoorUncoiler:
			if (pDoc->WorkingInfo.LastJob.bUclDrSen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUclDrSen = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_DoorUncoiler] = TRUE;
			}
			break;
		case _SigInx::_SaftyMk:
			if (pDoc->WorkingInfo.LastJob.bMkSftySen != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bMkSftySen = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_SaftyMk] = TRUE;
			}
			break;
		case _SigInx::_CleannerAoiUp:
			if (pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_CleannerAoiUp] = TRUE;
			}
			break;
		case _SigInx::_CleannerAoiDn:
			if (pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_CleannerAoiDn] = TRUE;
			}
			break;
		case _SigInx::_UltraSonicAoiDn:
			if(pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_UltraSonicAoiDn] = TRUE;
			}
			break;
		case _SigInx::_UltraSonicEngrave:
			if (pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_UltraSonicEngrave] = TRUE;
			}
			break;
		case _SigInx::_Use380mm:
			if (pDoc->WorkingInfo.LastJob.bUse380mm != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bUse380mm = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_Use380mm] = TRUE;
			}
			break;
		case _SigInx::_TempPause:
			if (pDoc->WorkingInfo.LastJob.bTempPause != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bTempPause = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_TempPause] = TRUE;
			}
			break;
		case _SigInx::_LotCut:
			if (pDoc->WorkingInfo.LastJob.bLotSep = (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bLotSep = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_LotCut] = TRUE;
			}
			break;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_TestMode:
			//if (pDoc->GetTestMode() != (int)SockData.nData1) // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
			{
				m_bGetOpInfo = TRUE;
				pDoc->SetTestMode();
				//pDoc->WorkingInfo.LastJob.nTestMode = (int)SockData.nData1; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
				//m_bRcvSig[_SigInx::_TestMode] = TRUE;
				//pDoc->SetTestMode(pDoc->WorkingInfo.LastJob.nTestMode); // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
			}
			break;
		case _ItemInx::_OpName:
			if (pDoc->WorkingInfo.LastJob.sSelUserName != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sSelUserName = CharToString(SockData.strData);

				if (pDoc && pDoc->m_pReelMap)
					pDoc->m_pReelMap->m_sUser = pDoc->WorkingInfo.LastJob.sSelUserName;
				if (pDoc)
					::WritePrivateProfileString(_T("Last Job"), _T("Operator Name"), pDoc->WorkingInfo.LastJob.sSelUserName, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_SampleShotNum:
			sVal.Format(_T("%d"), (int)SockData.nData1);
			if (pDoc->WorkingInfo.LastJob.sSampleTestShotNum != sVal)
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sSampleTestShotNum = sVal;

				::WritePrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), sVal, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_TotReelLen:
			if (pDoc->WorkingInfo.LastJob.sReelTotLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sReelTotLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sTotalReelDist = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Reel Total Length"), pDoc->WorkingInfo.LastJob.sReelTotLen, PATH_WORKING_INFO);
				::WritePrivateProfileString(_T("Lot"), _T("LOT_TOTAL_REEL_DIST"), pDoc->WorkingInfo.Lot.sTotalReelDist, PATH_WORKING_INFO);

				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 1000.0);
				pView->MpeWrite(pView->Plc.DlgInfo.TotalReelLength, lData);	// 전체 Reel 길이 (단위 M * 1000)
			}
			break;
		case _ItemInx::_OnePnlLen:
			if (pDoc->WorkingInfo.Motion.sMkFdDist != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.Motion.sMkFdDist = CharToString(SockData.strData);

				if (pDoc->m_pReelMap)
					pDoc->m_pReelMap->m_dPnlLen = _tstof(pDoc->WorkingInfo.Motion.sMkFdDist);
				::WritePrivateProfileString(_T("Last Job"), _T("One Panel Length"), pDoc->WorkingInfo.Motion.sMkFdDist, PATH_WORKING_INFO);
				::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), pDoc->WorkingInfo.Motion.sMkFdDist, PATH_WORKING_INFO);
				::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), pDoc->WorkingInfo.Motion.sMkFdDist, PATH_WORKING_INFO);
				if (pDoc->m_pReelMap)
				{
					lData = (long)(pDoc->m_pReelMap->m_dPnlLen * 1000.0);
					pView->MpeWrite(pView->Plc.DlgInfo.OneShotLength, lData);	// 한 판넬 길이 (단위 mm * 1000)
				}
			}
			break;
		case _ItemInx::_TempStopLen:
			if (pDoc->WorkingInfo.LastJob.sTempPauseLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sTempPauseLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sStopDist = CharToString(SockData.strData);

				if (pDoc->m_pReelMap)
					pDoc->m_pReelMap->m_dTempPauseLen = _tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen);
				::WritePrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), pDoc->WorkingInfo.LastJob.sTempPauseLen, PATH_WORKING_INFO);

				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sTempPauseLen) * 1000.0);
				pView->MpeWrite(pView->Plc.DlgInfo.StopLength, lData);	// 일시정지 길이 (단위 M * 1000)
			}
			break;
		case _ItemInx::_LotCutLen:
			if (pDoc->WorkingInfo.LastJob.sLotSepLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotSepLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sSeparateDist = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LotCutPosLen:
			if (pDoc->WorkingInfo.LastJob.sLotCutPosLen != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLotCutPosLen = CharToString(SockData.strData);
				pDoc->WorkingInfo.Lot.sCuttingDist = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Lot"), _T("LOT_CUTTING_DIST"), pDoc->WorkingInfo.LastJob.sLotCutPosLen, PATH_WORKING_INFO);
				if (pDoc->m_pReelMap)
					pDoc->m_pReelMap->m_dLotCutPosLen = _tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
				::WritePrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), pDoc->WorkingInfo.LastJob.sLotCutPosLen, PATH_WORKING_INFO);

				lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sLotCutPosLen) * 1000.0);
				pView->MpeWrite(pView->Plc.DlgInfo.LotCutLength, lData);	// Lot 분리 후 절단위치 (단위 M * 1000)
			}
			break;
		case _ItemInx::_LmtTotYld:
			if(pDoc->WorkingInfo.LastJob.sLmtTotYld != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLmtTotYld = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Limit Total Yield"), pDoc->WorkingInfo.LastJob.sLmtTotYld, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_LmtPatlYld:
			if(pDoc->WorkingInfo.LastJob.sLmtPatlYld != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sLmtPatlYld = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Limit Partial Yield"), pDoc->WorkingInfo.LastJob.sLmtPatlYld, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_StripOutRatio:
			if(pDoc->WorkingInfo.LastJob.sStripOutRatio != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sStripOutRatio = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Strip Out Ratio"), pDoc->WorkingInfo.LastJob.sStripOutRatio, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_CustomNeedRatio:
			if (pDoc->WorkingInfo.LastJob.sCustomNeedRatio != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sCustomNeedRatio = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Custom Need Ratio"), pDoc->WorkingInfo.LastJob.sCustomNeedRatio, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_NumRangeFixDef:
			if (pDoc->WorkingInfo.LastJob.sNumRangeFixDef != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sNumRangeFixDef = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Shot Num of Range in Fix Defect"), pDoc->WorkingInfo.LastJob.sNumRangeFixDef, PATH_WORKING_INFO);
			}
			break;
		case _ItemInx::_NumContFixDef:
			if (pDoc->WorkingInfo.LastJob.sNumContFixDef != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sNumContFixDef = CharToString(SockData.strData);

				::WritePrivateProfileString(_T("Last Job"), _T("Number of Continuous Fix Defect"), pDoc->WorkingInfo.LastJob.sNumContFixDef, PATH_WORKING_INFO);
			}
			break;
		//case _ItemInx::_UltraSonicStTim:
		//	if(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim != CharToString(SockData.strData))
		//	{
		//		m_bGetOpInfo = TRUE;
		//		pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim = CharToString(SockData.strData);

		//		::WritePrivateProfileString(_T("Last Job"), _T("Ultra Sonic Cleanner Start Time"), pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim, PATH_WORKING_INFO);
		//		lData = (long)(_tstof(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim) * 100.0);
		//		pView->MpeWrite(pView->Plc.DlgInfo.UltraSonicStTimeAoiDn, (long)lData);	// AOI_Dn (단위 [초] * 100) : 1 is 10 mSec.
		//		pView->MpeWrite(pView->Plc.DlgInfo.UltraSonicStTimeAoiUp, (long)lData);	// AOI_Up (단위 [초] * 100) : 1 is 10 mSec.
		//	}
		//	break;
		case _ItemInx::_EngItsCode:
			if (pDoc->WorkingInfo.LastJob.sEngItsCode != CharToString(SockData.strData))
			{
				m_bGetOpInfo = TRUE;
				pDoc->SetEngItsCode();
				//pDoc->m_sItsCode = pDoc->WorkingInfo.LastJob.sEngItsCode = CharToString(SockData.strData);
				//pDoc->SetEngItsCode(pDoc->WorkingInfo.LastJob.sEngItsCode);

				//::WritePrivateProfileString(_T("Last Job"), _T("Engrave Order Num"), pDoc->WorkingInfo.LastJob.sEngOrderNum, PATH_WORKING_INFO);
			}
			break;
		}
	}
}

void CEngrave::GetInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	//m_bGetInfo = FALSE;
	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_TempPause:
			if(pDoc->WorkingInfo.LastJob.bTempPause != (SockData.nData1 > 0) ? TRUE : FALSE)
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.bTempPause = (SockData.nData1 > 0) ? TRUE : FALSE;
				//m_bRcvSig[_SigInx::_TempPause] = TRUE;
			}
			break;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_OpName:
			if (pDoc->WorkingInfo.LastJob.sSelUserName != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.LastJob.sSelUserName = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_ModelUpName:
			if (pDoc->WorkingInfo.EngInfo.sModel != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.EngInfo.sModel = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LotUpName:
			if (pDoc->WorkingInfo.EngInfo.sLot != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.EngInfo.sLot = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LotDnName:
			//if (pDoc->WorkingInfo.EngInfo.sLotDn != CharToString(SockData.strData))
			//{
			//	m_bGetInfo = TRUE;
			//	pDoc->WorkingInfo.EngInfo.sLotDn = CharToString(SockData.strData);
			//}
			break;
		case _ItemInx::_LayerUpName:
			if (pDoc->WorkingInfo.EngInfo.sLayerUp != CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.EngInfo.sLayerUp = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LayerDnName:
			if (pDoc->WorkingInfo.EngInfo.sLayerDn = CharToString(SockData.strData))
			{
				m_bGetInfo = TRUE;
				pDoc->WorkingInfo.EngInfo.sLayerDn = CharToString(SockData.strData);
			}
			break;
		case _ItemInx::_LoadMstInfo:
			pView->m_bLoadMstInfo = TRUE; pDoc->SetStatus(_T("General"), _T("bLoadMstInfo"), pView->m_bLoadMstInfo);
			break;
		case _ItemInx::_TotReelLen:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sReelTotLen = CharToString(SockData.strData);
			pDoc->WorkingInfo.Lot.sTotalReelDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_PartVel:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sPartialSpd = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Last Job"), _T("Partial Speed"), pDoc->WorkingInfo.LastJob.sPartialSpd, PATH_WORKING_INFO);
			break;
		case _ItemInx::_TempStopLen:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sTempPauseLen = CharToString(SockData.strData);
			pDoc->WorkingInfo.Lot.sStopDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_LotCutLen:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sLotSepLen = CharToString(SockData.strData);
			pDoc->WorkingInfo.Lot.sSeparateDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_LotCutPosLen:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sLotCutPosLen = CharToString(SockData.strData);
			pDoc->WorkingInfo.Lot.sCuttingDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_LotSerial:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sLotSerial = CharToString(SockData.strData);
			break;
		case _ItemInx::_MkVerfyLen:
			m_bGetInfo = TRUE;
			pDoc->WorkingInfo.LastJob.sVerifyLen = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetTotRatio(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
			;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_DefNumUp:
			pDoc->m_nBad[0] = SockData.nData1;
			break;
		case _ItemInx::_DefRtoUp:
			pDoc->m_dBadRatio[0] = (double)SockData.fData1;
			break;
		case _ItemInx::_GoodNumUp:
			pDoc->m_nGood[0] = SockData.nData1;
			break;
		case _ItemInx::_GoodRtoUp:
			pDoc->m_dGoodRatio[0] = (double)SockData.fData1;
			break;
		case _ItemInx::_TestNumUp:
			pDoc->m_nTestNum[0] = SockData.nData1;
			break;
		case _ItemInx::_DefNumDn:
			pDoc->m_nBad[1] = SockData.nData1;
			break;
		case _ItemInx::_DefRtoDn:
			pDoc->m_dBadRatio[1] = (double)SockData.fData1;
			break;
		case _ItemInx::_GoodNumDn:
			pDoc->m_nGood[1] = SockData.nData1;
			break;
		case _ItemInx::_GoodRtoDn:
			pDoc->m_dGoodRatio[1] = (double)SockData.fData1;
			break;
		case _ItemInx::_TestNumDn:
			pDoc->m_nTestNum[1] = SockData.nData1;
			break;
		case _ItemInx::_DefNumTot:
			pDoc->m_nBad[2] = SockData.nData1;
			break;
		case _ItemInx::_DefRtoTot:
			pDoc->m_dBadRatio[2] = (double)SockData.fData1;
			break;
		case _ItemInx::_GoodNumTot:
			pDoc->m_nGood[2] = SockData.nData1;
			break;
		case _ItemInx::_GoodRtoTot:
			pDoc->m_dGoodRatio[2] = (double)SockData.fData1;
			break;
		case _ItemInx::_TestNumTot:
			pDoc->m_nTestNum[2] = SockData.nData1;
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetStTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_LotStTime:
			pDoc->m_sLotStTime = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetRunTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case  _ItemInx::_LotRunTime:
			pDoc->m_sLotRunTime = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetEdTime(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_LotEdTime:
			pDoc->m_sLotEdTime = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetStripRatio(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_1LnGoodRtoUp:
			pDoc->m_dStripRatio[0][0] = (double)SockData.fData1;
			break;
		case _ItemInx::_2LnGoodRtoUp:
			pDoc->m_dStripRatio[0][1] = (double)SockData.fData1;
			break;
		case _ItemInx::_3LnGoodRtoUp:
			pDoc->m_dStripRatio[0][2] = (double)SockData.fData1;
			break;
		case _ItemInx::_4LnGoodRtoUp:
			pDoc->m_dStripRatio[0][3] = (double)SockData.fData1;
			break;
		case _ItemInx::_AllLnGoodRtoUp:
			pDoc->m_dStripRatio[0][4] = (double)SockData.fData1;
			break;
		case _ItemInx::_1LnGoodRtoDn:
			pDoc->m_dStripRatio[1][0] = (double)SockData.fData1;
			break;
		case _ItemInx::_2LnGoodRtoDn:
			pDoc->m_dStripRatio[1][1] = (double)SockData.fData1;
			break;
		case _ItemInx::_3LnGoodRtoDn:
			pDoc->m_dStripRatio[1][2] = (double)SockData.fData1;
			break;
		case _ItemInx::_4LnGoodRtoDn:
			pDoc->m_dStripRatio[1][3] = (double)SockData.fData1;
			break;
		case _ItemInx::_AllLnGoodRtoDn:
			pDoc->m_dStripRatio[1][4] = (double)SockData.fData1;
			break;
		case _ItemInx::_1LnGoodRtoTot:
			pDoc->m_dStripRatio[2][0] = (double)SockData.fData1;
			break;
		case _ItemInx::_2LnGoodRtoTot:
			pDoc->m_dStripRatio[2][1] = (double)SockData.fData1;
			break;
		case _ItemInx::_3LnGoodRtoTot:
			pDoc->m_dStripRatio[2][2] = (double)SockData.fData1;
			break;
		case _ItemInx::_4LnGoodRtoTot:
			pDoc->m_dStripRatio[2][3] = (double)SockData.fData1;
			break;
		case _ItemInx::_AllLnGoodRtoTot:
			pDoc->m_dStripRatio[2][4] = (double)SockData.fData1;
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetDef(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_DefNumOpen:
			pDoc->m_nDef[DEF_OPEN] = SockData.nData1; // IDC_STC_DEF_OPEN
			break;
		case _ItemInx::_DefNumShort:
			pDoc->m_nDef[DEF_SHORT] = SockData.nData1; // IDC_STC_DEF_SHORT
			break;
		case _ItemInx::_DefNumUshort:
			pDoc->m_nDef[DEF_USHORT] = SockData.nData1; // IDC_STC_DEF_U_SHORT
			break;
		case _ItemInx::_DefNumLnW:
			pDoc->m_nDef[DEF_SPACE] = SockData.nData1; // IDC_STC_DEF_SPACE
			break;
		case _ItemInx::_DefExtr:
			pDoc->m_nDef[DEF_EXTRA] = SockData.nData1; // IDC_STC_DEF_EXTRA
			break;
		case _ItemInx::_DefNumProt:
			pDoc->m_nDef[DEF_PROTRUSION] = SockData.nData1; // IDC_STC_DEF_PROT
			break;
		case _ItemInx::_DefNumPhole:
			pDoc->m_nDef[DEF_PINHOLE] = SockData.nData1; // IDC_STC_DEF_P_HOLE
			break;
		case _ItemInx::_DefNumPad:
			pDoc->m_nDef[DEF_PAD] = SockData.nData1; // IDC_STC_DEF_PAD
			break;
		case _ItemInx::_DefNumHopen:
			pDoc->m_nDef[DEF_HOLE_OPEN] = SockData.nData1; // IDC_STC_DEF_H_OPEN
			break;
		case _ItemInx::_DefNumHmiss:
			pDoc->m_nDef[DEF_HOLE_MISS] = SockData.nData1; // IDC_STC_DEF_H_MISS
			break;
		case _ItemInx::_DefNumHpos:
			pDoc->m_nDef[DEF_HOLE_POSITION] = SockData.nData1; // IDC_STC_DEF_H_POS
			break;
		case _ItemInx::_DefNumHdef:
			pDoc->m_nDef[DEF_HOLE_DEFECT] = SockData.nData1; // IDC_STC_DEF_H_DEF
			break;
		case _ItemInx::_DefNumNick:
			pDoc->m_nDef[DEF_NICK] = SockData.nData1; // IDC_STC_DEF_NICK
			break;
		case _ItemInx::_DefNumPoi:
			pDoc->m_nDef[DEF_POI] = SockData.nData1; // IDC_STC_DEF_POI
			break;
		case _ItemInx::_DefNumVhOpen:
			pDoc->m_nDef[DEF_VH_OPEN] = SockData.nData1; // IDC_STC_DEF_VH_OPEN
			break;
		case _ItemInx::_DefNumVhMiss:
			pDoc->m_nDef[DEF_VH_MISS] = SockData.nData1; // IDC_STC_DEF_VH_MISS
			break;
		case _ItemInx::_DefNumVhPos:
			pDoc->m_nDef[DEF_VH_POSITION] = SockData.nData1; // IDC_STC_DEF_VH_POS
			break;
		case _ItemInx::_DefNumVhd:
			pDoc->m_nDef[DEF_VH_DEF] = SockData.nData1; // IDC_STC_DEF_VH_DEF
			break;
		case _ItemInx::_DefNumLight:
			pDoc->m_nDef[DEF_LIGHT] = SockData.nData1; // IDC_STC_DEF_LIGHT
			break;
		case _ItemInx::_DefNumEnick:
			pDoc->m_nDef[DEF_EDGE_NICK] = SockData.nData1;
			break;
		case _ItemInx::_DefNumEprot:
			pDoc->m_nDef[DEF_EDGE_PROT] = SockData.nData1;
			break;
		case _ItemInx::_DefNumEspace:
			pDoc->m_nDef[DEF_EDGE_SPACE] = SockData.nData1;
			break;
		case _ItemInx::_DefNumUdd1:
			pDoc->m_nDef[DEF_USER_DEFINE_1] = SockData.nData1;
			break;
		case _ItemInx::_DefNumNrw:
			pDoc->m_nDef[DEF_NARROW] = SockData.nData1;
			break;
		case _ItemInx::_DefNumWide:
			pDoc->m_nDef[DEF_WIDE] = SockData.nData1;
			break;
		default:
			break;
		}
	}
}

void CEngrave::Get2DReader(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_2DEngLen:
			pDoc->WorkingInfo.Motion.s2DEngLen = CharToString(SockData.strData);
			pDoc->SetEngraveReaderDist(_tstof(pDoc->WorkingInfo.Motion.s2DEngLen));
			break;
		case _ItemInx::_2DAoiLen:
			pDoc->WorkingInfo.Motion.s2DAoiLen = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DMkLen:
			pDoc->WorkingInfo.Motion.s2DMkLen = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DMoveVel:
			pDoc->WorkingInfo.Motion.s2DMoveVel = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DMoveAcc:
			pDoc->WorkingInfo.Motion.s2DMoveAcc = CharToString(SockData.strData);
			break;
		case _ItemInx::_2DOneShotLen:
			pDoc->WorkingInfo.Motion.s2DOneShotRemainLen = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetEngInfo(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_EngBuffJogCw:
			pDoc->WorkingInfo.Motion.bEngBuffJogCw = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_EngBuffJogCw] = TRUE;
			break;
		case _SigInx::_EngBuffJogCcw:
			pDoc->WorkingInfo.Motion.bEngBuffJogCcw = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_EngBuffJogCcw] = TRUE;
			break;
		case _SigInx::_EngBuffOrgMv:
			pDoc->WorkingInfo.Motion.bEngBuffHomming = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_EngBuffOrgMv] = TRUE;
			break;
		case _SigInx::_EngBuffOrgMvDone:
			pDoc->WorkingInfo.Motion.bEngBuffHommingDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_EngBuffOrgMvDone] = TRUE;
			break;
		case _SigInx::_EngBuffInitPosMv:
			pDoc->WorkingInfo.Motion.bEngBuffInitMv = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_EngBuffInitPosMv] = TRUE;
			break;
		case _SigInx::_EngBuffInitPosMvDone:
			pDoc->WorkingInfo.Motion.bEngBuffInitMvDone = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_EngBuffInitPosMvDone] = TRUE;
			break;
		case _SigInx::_EngBuffInitPosSave:
			pDoc->WorkingInfo.Motion.bEngBuffInitPosSave = (SockData.nData1 > 0) ? TRUE : FALSE;
			//m_bRcvSig[_SigInx::_EngBuffInitPosSave] = TRUE;
			break;
		}
	}
	else if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_EngLeadPitch:
			pDoc->WorkingInfo.Motion.sEngraveFdLead = CharToString(SockData.strData);
			pDoc->SetEngraveFdPitch(_tstof(pDoc->WorkingInfo.Motion.sEngraveFdLead));
			break;
		case _ItemInx::_EngPushOffLen:
			pDoc->WorkingInfo.Motion.sEngraveFdVacOff = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngTqVal:
			pDoc->WorkingInfo.Motion.sEngraveTq = CharToString(SockData.strData);
			pDoc->SetEngraveToq(_tstof(pDoc->WorkingInfo.Motion.sEngraveTq));
			break;
		case _ItemInx::_EngAoiLen:
			pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist = CharToString(SockData.strData);
			pDoc->SetEngraveAoiDist(_tstoi(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist));
			break;
		case _ItemInx::_EngFdDiffMax:
			pDoc->WorkingInfo.Motion.sEngFdDiffMax = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngFdDiffRng:
			pDoc->WorkingInfo.Motion.sEngFdDiffRng = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngFdDiffNum:
			pDoc->WorkingInfo.Motion.sEngFdDiffNum = CharToString(SockData.strData);
			break;
		case _ItemInx::_EngBuffInitPos:
			pDoc->WorkingInfo.Motion.sEngBuffInitPos = CharToString(SockData.strData);
			pDoc->SetEngBufInitPos(_tstof(pDoc->WorkingInfo.Motion.sEngBuffInitPos));
			break;
		case _ItemInx::_EngBuffCurrPos:
			pDoc->WorkingInfo.Motion.sEngBuffCurrPos = CharToString(SockData.strData);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetFdInfo(SOCKET_DATA SockData)
{
	long lData;

	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	
	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_FdVel:
			pDoc->WorkingInfo.Motion.sMkJogVel = pDoc->WorkingInfo.Motion.sAoiJogVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), pDoc->WorkingInfo.Motion.sMkJogVel, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), pDoc->WorkingInfo.Motion.sAoiJogVel, PATH_WORKING_INFO);
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkJogVel) * 1000.0);
			pView->MpeWrite(pView->Plc.DlgMenu04.FeedOnSpeed, lData);	// 연속공급 속도 (단위 mm/sec * 1000)
			break;
		case _ItemInx::_FdAcc:
			pDoc->WorkingInfo.Motion.sMkJogAcc = pDoc->WorkingInfo.Motion.sAoiJogAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), pDoc->WorkingInfo.Motion.sMkJogAcc, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), pDoc->WorkingInfo.Motion.sAoiJogAcc, PATH_WORKING_INFO);
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkJogAcc) * 1000.0);
			pView->MpeWrite(pView->Plc.DlgMenu04.FeedOnAcc, lData);	// 연속공급 가속도 (단위 mm/s^2 * 1000)
			break;
		case _ItemInx::_OnePnlLen:
			pDoc->WorkingInfo.Motion.sMkFdDist = CharToString(SockData.strData);
			break;
		case _ItemInx::_OnePnlVel:
			pDoc->WorkingInfo.Motion.sMkFdVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), pDoc->WorkingInfo.Motion.sMkFdVel, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), pDoc->WorkingInfo.Motion.sMkFdVel, PATH_WORKING_INFO);
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdVel) * 1000.0);
			pView->MpeWrite(pView->Plc.DlgMenu04.FeedSpeed, lData);	// 한 판넬 Feeding 속도 (단위 mm/sec * 1000)
			break;
		case _ItemInx::_OnePnlAcc:
			pDoc->WorkingInfo.Motion.sMkFdAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), pDoc->WorkingInfo.Motion.sMkFdAcc, PATH_WORKING_INFO);
			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), pDoc->WorkingInfo.Motion.sMkFdAcc, PATH_WORKING_INFO);
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkFdAcc) * 1000.0);
			pView->MpeWrite(pView->Plc.DlgMenu04.FeedAcc, lData);	// 한 판넬 Feeding 가속도 (단위 mm/s^2 * 1000)
			break;
		case _ItemInx::_FdDiffMax:
			pDoc->WorkingInfo.Motion.sLmtFdErr = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), pDoc->WorkingInfo.Motion.sLmtFdErr, PATH_WORKING_INFO);
			break;
		case _ItemInx::_FdDiffRng:
			pDoc->WorkingInfo.Motion.sLmtFdAdjOffSet = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), pDoc->WorkingInfo.Motion.sLmtFdAdjOffSet, PATH_WORKING_INFO);
			break;
		case _ItemInx::_FdDiffNum:
			pDoc->WorkingInfo.Motion.sLmtFdOvrNum = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), pDoc->WorkingInfo.Motion.sLmtFdOvrNum, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetAoiInfo(SOCKET_DATA SockData)
{
	long lData;

	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_AoiLeadPitch:
			pDoc->WorkingInfo.Motion.sAoiFdLead = CharToString(SockData.strData);

			pDoc->SetAoiFdPitch(_tstof(pDoc->WorkingInfo.Motion.sAoiFdLead));
			break;
		case _ItemInx::_AoiPushOffLen:
			pDoc->WorkingInfo.Motion.sAoiFdVacOff = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), pDoc->WorkingInfo.Motion.sAoiFdVacOff, PATH_WORKING_INFO);
			break;
		//case _ItemInx::_AoiTqVal:
		//	pDoc->WorkingInfo.Motion.sAoiTq = CharToString(SockData.strData);

		//	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), pDoc->WorkingInfo.Motion.sAoiTq, PATH_WORKING_INFO);
		//	lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sAoiTq) * 1000.0);
		//	pView->MpeWrite(pView->Plc.DlgMenu04.FeedTorqueAoi, lData);	// 검사부 Tension 모터 토크값 (단위 Kgf * 1000)
		//	break;
		case _ItemInx::_AoiBuffShotNum:
			pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot, PATH_WORKING_INFO);
			lData = (long)(_tstoi(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot) * 1000);
			pView->MpeWrite(pView->Plc.DlgMenu04.ShotNumFromAoiUpToAoiDn, lData);	// AOI(상)에서 AOI(하) Shot수 (단위 Shot수 * 1000)
			break;
		case _ItemInx::_AoiMkLen:
			pDoc->WorkingInfo.Motion.sFdMkAoiInitDist = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), pDoc->WorkingInfo.Motion.sFdMkAoiInitDist, PATH_WORKING_INFO);
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist) * 1000.0);
			pView->MpeWrite(pView->Plc.DlgMenu04.FeedLengthFromAoiToPunch, lData);	// AOI(하)에서 마킹까지 거리 (단위 mm * 1000)
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetMkInfo(SOCKET_DATA SockData)
{
	long lData;
	CString sData;

	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_MkLeadPitch:
			pDoc->WorkingInfo.Motion.sMkFdLead = CharToString(SockData.strData);

			pDoc->SetMkFdPitch(_tstof(pDoc->WorkingInfo.Motion.sMkFdLead));
			break;
		case _ItemInx::_MkPushOffLen:
			pDoc->WorkingInfo.Motion.sMkFdVacOff = CharToString(SockData.strData);
	
			::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), pDoc->WorkingInfo.Motion.sMkFdVacOff, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkTqVal:
			pDoc->WorkingInfo.Motion.sMkTq = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), pDoc->WorkingInfo.Motion.sMkTq, PATH_WORKING_INFO);
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sMkTq) * 1000.0);
			pView->MpeWrite(pView->Plc.DlgMenu04.FeedTorqueCcwPunch, lData);	// 마킹부 Tension 모터 토크값 (단위 Kgf * 1000)
			break;
		case _ItemInx::_MkBuffInitPos:
			pDoc->WorkingInfo.Motion.sStBufPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), pDoc->WorkingInfo.Motion.sStBufPos, PATH_WORKING_INFO);
			lData = (long)(_tstof(pDoc->WorkingInfo.Motion.sStBufPos) * 1000.0);
			pView->MpeWrite(pView->Plc.DlgMenu04.BufferPosStAoi, lData);	// 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
			break;
		case _ItemInx::_MkBuffCurrPos:
			pDoc->m_dMkBuffCurrPos = (double)SockData.fData1;
			break;
		case _ItemInx::_MkNumLf:
			pDoc->WorkingInfo.Marking[0].nMkCnt = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[0].nMkCnt);
			::WritePrivateProfileString(_T("Marking0"), _T("Marking Count"), sData, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkNumRt:
			pDoc->WorkingInfo.Marking[1].nMkCnt = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[1].nMkCnt);
			::WritePrivateProfileString(_T("Marking1"), _T("Marking Count"), sData, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkMaxNumLf:
			pDoc->WorkingInfo.Marking[0].nMkLimit = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[0].nMkLimit);
			::WritePrivateProfileString(_T("Marking0"), _T("Marking Limit"), sData, PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkMaxNumRt:
			pDoc->WorkingInfo.Marking[1].nMkLimit = SockData.nData1;

			sData.Format(_T("%d"), pDoc->WorkingInfo.Marking[1].nMkLimit);
			::WritePrivateProfileString(_T("Marking1"), _T("Marking Limit"), sData, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetMkInfoLf(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_MkInitPosLf:
			pDoc->WorkingInfo.Marking[0].sWaitPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_POS"), pDoc->WorkingInfo.Marking[0].sWaitPos, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[0])
				pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
			break;
		case _ItemInx::_MkInitVelLf:
			pDoc->WorkingInfo.Marking[0].sWaitVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_VEL"), pDoc->WorkingInfo.Marking[0].sWaitVel, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[0])
				pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
			break;
		case _ItemInx::_MkInitAccLf:
			pDoc->WorkingInfo.Marking[0].sWaitAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_ACC"), pDoc->WorkingInfo.Marking[0].sWaitAcc, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[0])
				pView->m_pVoiceCoil[0]->SetMarkShiftData(0);
			break;
		case _ItemInx::_MkFnlPosLf:
			pDoc->WorkingInfo.Marking[0].sMarkingPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_POS"), pDoc->WorkingInfo.Marking[0].sMarkingPos, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[0])
				pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkFnlVelLf:
			pDoc->WorkingInfo.Marking[0].sMarkingVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_VEL"), pDoc->WorkingInfo.Marking[0].sMarkingVel, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[0])
				pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkFnlAccLf:
			pDoc->WorkingInfo.Marking[0].sMarkingAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_ACC"), pDoc->WorkingInfo.Marking[0].sMarkingAcc, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[0])
				pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkFnlTqLf:
			pDoc->WorkingInfo.Marking[0].sMarkingToq = CharToString(SockData.strData);
			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ"), pDoc->WorkingInfo.Marking[0].sMarkingToq, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[0])
				pView->m_pVoiceCoil[0]->SetMarkFinalData(0);
			break;
		case _ItemInx::_MkHgtPosX1Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX1"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY1Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY1"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX2Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX2"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY2Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY2"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX3Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX3"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY3Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY3"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX4Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosX[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX4"), pDoc->WorkingInfo.Marking[0].sMeasurePosX[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY4Lf:
			pDoc->WorkingInfo.Marking[0].sMeasurePosY[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY4"), pDoc->WorkingInfo.Marking[0].sMeasurePosY[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtAvgPosLf:
			pDoc->WorkingInfo.Marking[0].sAverDist = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), pDoc->WorkingInfo.Marking[0].sAverDist, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetMkInfoRt(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _ItemInx::_MkInitPosRt:
			pDoc->WorkingInfo.Marking[1].sWaitPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_POS"), pDoc->WorkingInfo.Marking[1].sWaitPos, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[1])
				pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
			break;
		case _ItemInx::_MkInitVelRt:
			pDoc->WorkingInfo.Marking[1].sWaitVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_VEL"), pDoc->WorkingInfo.Marking[1].sWaitVel, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[1])
				pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
			break;
		case _ItemInx::_MkInitAccRt:
			pDoc->WorkingInfo.Marking[1].sWaitAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_ACC"), pDoc->WorkingInfo.Marking[1].sWaitAcc, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[1])
				pView->m_pVoiceCoil[1]->SetMarkShiftData(1);
			break;
		case _ItemInx::_MkFnlPosRt:
			pDoc->WorkingInfo.Marking[1].sMarkingPos = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_POS"), pDoc->WorkingInfo.Marking[1].sMarkingPos, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[1])
				pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkFnlVelRt:
			pDoc->WorkingInfo.Marking[1].sMarkingVel = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_VEL"), pDoc->WorkingInfo.Marking[1].sMarkingVel, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[1])
				pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkFnlAccRt:
			pDoc->WorkingInfo.Marking[1].sMarkingAcc = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_ACC"), pDoc->WorkingInfo.Marking[1].sMarkingAcc, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[1])
				pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkFnlTqRt:
			pDoc->WorkingInfo.Marking[1].sMarkingToq = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ"), pDoc->WorkingInfo.Marking[1].sMarkingToq, PATH_WORKING_INFO);
			if (pView->m_pVoiceCoil[1])
				pView->m_pVoiceCoil[1]->SetMarkFinalData(1);
			break;
		case _ItemInx::_MkHgtPosX1Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX1"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY1Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[0] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY1"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[0], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX2Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX2"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY2Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[1] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY2"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[1], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX3Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX3"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY3Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[2] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY3"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[2], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosX4Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosX[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX4"), pDoc->WorkingInfo.Marking[1].sMeasurePosX[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtPosY4Rt:
			pDoc->WorkingInfo.Marking[1].sMeasurePosY[3] = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY4"), pDoc->WorkingInfo.Marking[1].sMeasurePosY[3], PATH_WORKING_INFO);
			break;
		case _ItemInx::_MkHgtAvgPosRt:
			pDoc->WorkingInfo.Marking[1].sAverDist = CharToString(SockData.strData);

			::WritePrivateProfileString(_T("Marking1"), _T("MARKING_AVER_DIST"), pDoc->WorkingInfo.Marking[1].sAverDist, PATH_WORKING_INFO);
			break;
		default:
			break;
		}
	}
}

void CEngrave::GetAlarmMsg(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _stAlarmInx::_Alarm:
			//pDoc->m_sAlmMsg = CharToString(SockData.strData);
			break;
		case _stAlarmInx::_IsAlarm:
			pDoc->m_sIsAlmMsg = CharToString(SockData.strData);
			break;
		}
	}
}

void CEngrave::GetMsgBox(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;

	if (nCmdCode == _SetData)
	{
		switch (nMsgId)
		{
		case _stMsgBoxInx::_MsgBox:
			pDoc->m_sMsgBox = CharToString(SockData.strData);
			break;
		case _stMsgBoxInx::_IsMsgBox:
			pDoc->m_sIsMsgBox = CharToString(SockData.strData);
			break;
		}
	}
}

// End for GetSysData()

// Start for SetSysSignal()

BOOL CEngrave::SetSysSignal()
{
	if (!IsRunning())
	{
		SetSignalMain();
		SetSignalTorqueMotor();
		SetSignalInductionMotor();
		SetSignalCore150mm();
		//SetSignalEtc();
		SetSignalRecoiler();
		SetSignalPunch();
		SetSignalAOIDn();
		SetSignalAOIUp();
		SetSignalEngrave();
		SetSignalUncoiler();

		return TRUE;
	}

	return FALSE;
}

void CEngrave::SetSignalMain()
{
}

void CEngrave::SetSignalTorqueMotor()
{
}

void CEngrave::SetSignalInductionMotor()
{
}

void CEngrave::SetSignalCore150mm()
{
}

void CEngrave::SetSignalEtc()
{
}

void CEngrave::SetSignalRecoiler()
{
}

void CEngrave::SetSignalPunch()
{
}

void CEngrave::SetSignalAOIDn()
{
}

void CEngrave::SetSignalAOIUp()
{
}

void CEngrave::SetSignalEngrave()
{
}

void CEngrave::SetSignalUncoiler()
{
}

// End SetSysSignal()

// Start for SetSysData()

BOOL CEngrave::SetSysData()
{
	if (!IsRunning())
	{
		SetOpInfo();
		SetInfo();
		SetTotRatio();
		SetStTime();
		SetRunTime();
		SetEdTime();
		SetStripRatio();
		SetDef();
		Set2DReader();
		SetEngInfo();
		SetFdInfo();
		SetAoiInfo();
		SetMkInfo();
		SetMkInfoLf();
		SetMkInfoRt();

		return TRUE;
	}

	return FALSE;
}

void CEngrave::SetOpInfo()
{
	SetOpName();
	SetDualTest();
	SetSampleTest();
	SetSampleShotNum();
	//SetTestMode();
	SetRecoilerCcw();
	SetUncoilerCcw();
	SetAlignMethode();
	SetDoorRecoiler();
	SetDoorAoiUp();
	SetDoorAoiDn();
	SetDoorMk();
	SetDoorEngrave();
	SetDoorUncoiler();
	SetSaftyMk();
	SetCleannerAoiUp();
	SetCleannerAoiDn();
	SetUltraSonicAoiDn();
	SetUltraSonicEngrave();
	SetTotReelLen();
	SetOnePnlLen();
	SetTempPause();
	SetTempStopLen();
	SetLotCut();
	SetLotCutLen();
	SetLotCutPosLen();
	SetLmtTotYld();
	SetLmtPatlYld();
	SetStripOutRatio();
	SetCustomNeedRatio();
	SetNumRangeFixDef();
	SetFixDef();
	SetNumContFixDef();
	SetUltraSonicStTim();
	SetEngItsCode();
}

void CEngrave::SetInfo()
{
	SetModelName();
	//SetModelDnName();
	SetLotName();
	//SetLotDnName();
	SetLayerUpName();
	SetLayerDnName();
	//SetOrderNum();
	SetShotNum();
	SetTotOpRto();
	SetTotVel();
	SetPartVel();
	SetMkDoneLen();
	SetAoiDnDoneLen();
	SetAoiUpDoneLen();
	SetLotSerial();
	SetMkVerfyLen();
}

void CEngrave::SetTotRatio()
{
	SetDefNumUp();
	SetDefRtoUp();
	SetGoodNumUp();
	SetGoodRtoUp();
	SetTestNumUp();
	SetDefNumDn();
	SetDefRtoDn();
	SetGoodNumDn();
	SetGoodRtoDn();
	SetTestNumDn();
	SetDefNumTot();
	SetDefRtoTot();
	SetGoodNumTot();
	SetGoodRtoTot();
	SetTestNumTot();
}

void CEngrave::SetStTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if(pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetStTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _stItemInx::_LotStTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetRunTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetRunTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _stItemInx::_LotRunTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetEdTime()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	CString str = _T("");
	if (pView)
	{
		if (pView->m_pDlgMenu01)
		{
			str = pView->m_pDlgMenu01->GetEdTime();
			char cData[BUFFER_DATA_SIZE];
			SocketData.nMsgID = _stItemInx::_LotEdTime;
			StringToChar(str, cData);
			sprintf(SocketData.strData, "%s", cData);
			SendCommand(SocketData);
		}
	}
}

void CEngrave::SetStripRatio()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString str;
	int nGood = 0, nBad = 0, nTot = 0, nStTot = 0, nSum = 0, nVal[2][4];
	int nMer[MAX_STRIP];
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (!pDoc->m_pReelMapUp)
		return;

	if (bDualTest)
	{
		if (!pDoc->m_pReelMapDn || !pDoc->m_pReelMapAllUp)
			return;
	}

	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 4; k++)
			nVal[i][k] = 0;
	}

	// < 스트립 별 수율 >
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad);
	nTot = nGood + nBad;
	nStTot = nTot / MAX_STRIP;

	// 상면
	nSum = 0;
	nVal[0][0] = pDoc->m_pReelMapUp->GetDefStrip(0);
	nSum += nVal[0][0];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][0]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_1LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][1] = pDoc->m_pReelMapUp->GetDefStrip(1);
	nSum += nVal[0][1];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][1]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_2LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][2] = pDoc->m_pReelMapUp->GetDefStrip(2);
	nSum += nVal[0][2];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][2]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_3LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	nVal[0][3] = pDoc->m_pReelMapUp->GetDefStrip(3);
	nSum += nVal[0][3];
	if (nTot > 0)
		dRatio = ((double)(nStTot - nVal[0][3]) / (double)nStTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_4LnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);

	if (nTot > 0)
		dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_AllLnGoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);


	if (bDualTest)
	{
		// 하면
		nSum = 0;
		nVal[1][0] = pDoc->m_pReelMapDn->GetDefStrip(0);
		nSum += nVal[1][0];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][0]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_1LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][1] = pDoc->m_pReelMapDn->GetDefStrip(1);
		nSum += nVal[1][1];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][1]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_2LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][2] = pDoc->m_pReelMapDn->GetDefStrip(2);
		nSum += nVal[1][2];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][2]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_3LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nVal[1][3] = pDoc->m_pReelMapDn->GetDefStrip(3);
		nSum += nVal[1][3];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nVal[1][3]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_4LnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_AllLnGoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		// 상면 + 하면
		nSum = 0;
		nMer[0] = pDoc->m_pReelMapAllUp->GetDefStrip(0);
		nSum += nMer[0];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[0]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_1LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[1] = pDoc->m_pReelMapAllUp->GetDefStrip(1);
		nSum += nMer[1];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[1]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_2LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[2] = pDoc->m_pReelMapAllUp->GetDefStrip(2);
		nSum += nMer[2];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[2]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_3LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		nMer[3] = pDoc->m_pReelMapAllUp->GetDefStrip(3);
		nSum += nMer[3];
		if (nTot > 0)
			dRatio = ((double)(nStTot - nMer[3]) / (double)nStTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_4LnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);

		if (nTot > 0)
			dRatio = ((double)(nTot - nSum) / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_AllLnGoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDef()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	CString str;
	int nNum = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CReelMap* pReelMap=NULL;

	//if (bDualTest)
	//{
	//	switch (pView->m_nSelRmap)
	//	{
	//	case RMAP_UP:
	//		pReelMap = pDoc->m_pReelMapUp;
	//		break;
	//	case RMAP_DN:
	//		pReelMap = pDoc->m_pReelMapDn;
	//		break;
	//	case RMAP_ALLUP:
	//		pReelMap = pDoc->m_pReelMapAllUp;
	//		break;
	//	case RMAP_ALLDN:
	//		pReelMap = pDoc->m_pReelMapAllDn;
	//		break;
	//	}
	//}
	//else
	//	pReelMap = pDoc->m_pReelMapUp;


	switch (pView->m_nSelRmap)
	{
	case RMAP_UP:
		if (pDoc->m_pReelMapUp)
			pReelMap = pDoc->m_pReelMapUp;
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapUp is NULL."));
		}
		break;
	case RMAP_DN:
		if (pDoc->m_pReelMapDn)
			pReelMap = pDoc->m_pReelMapDn;
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapDn is NULL."));
		}
		break;
	case RMAP_ALLUP:
		if (pDoc->m_pReelMapAllUp)
			pReelMap = pDoc->m_pReelMapAllUp;
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapAllUp is NULL."));
		}
		break;
	case RMAP_ALLDN:
		if (pDoc->m_pReelMapAllDn)
			pReelMap = pDoc->m_pReelMapAllDn;
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapAllDn is NULL."));
		}
		break;
	case RMAP_INNER_UP:
		if (pDoc->m_pReelMapInnerUp)
			pReelMap = pDoc->m_pReelMapInnerUp;
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapInnerUp is NULL."));
		}
		break;
	case RMAP_INNER_DN:
		if (pDoc->m_pReelMapInnerDn)
			pReelMap = pDoc->m_pReelMapInnerDn;
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapInnerDn is NULL."));
		}
		break;
	case RMAP_INNER_ALLUP:
		if (pDoc->m_pReelMapInnerAllUp)
			pReelMap = pDoc->m_pReelMapInnerAllUp;
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapInnerAllUp is NULL."));
		}
		break;
	case RMAP_INNER_ALLDN:
		if (pDoc->m_pReelMapInnerAllDn)
			pReelMap = pDoc->m_pReelMapInnerAllDn;
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapInnerAllDn is NULL."));
		}
		break;
	case RMAP_ITS:
		if (pDoc->m_pReelMapIts)
			pReelMap = pDoc->m_pReelMapIts;
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("m_pReelMapIts is NULL."));
		}
		break;
	}

	if(pReelMap)
	{
		nNum = pReelMap->GetDefNum(DEF_OPEN); // IDC_STC_DEF_OPEN
		SocketData.nMsgID = _stItemInx::_DefNumOpen;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_SHORT); // IDC_STC_DEF_SHORT
		SocketData.nMsgID = _stItemInx::_DefNumShort;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_USHORT); // IDC_STC_DEF_U_SHORT
		SocketData.nMsgID = _stItemInx::_DefNumUshort;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_SPACE); // IDC_STC_DEF_SPACE
		SocketData.nMsgID = _stItemInx::_DefNumLnW;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EXTRA); // IDC_STC_DEF_EXTRA
		SocketData.nMsgID = _stItemInx::_DefExtr;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_PROTRUSION); // IDC_STC_DEF_PROT
		SocketData.nMsgID = _stItemInx::_DefNumProt;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_PINHOLE); // IDC_STC_DEF_P_HOLE
		SocketData.nMsgID = _stItemInx::_DefNumPhole;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_PAD); // IDC_STC_DEF_PAD
		SocketData.nMsgID = _stItemInx::_DefNumPad;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_OPEN); // IDC_STC_DEF_H_OPEN
		SocketData.nMsgID = _stItemInx::_DefNumHopen;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_MISS); // IDC_STC_DEF_H_MISS
		SocketData.nMsgID = _stItemInx::_DefNumHmiss;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_POSITION); // IDC_STC_DEF_H_POS
		SocketData.nMsgID = _stItemInx::_DefNumHpos;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_HOLE_DEFECT); // IDC_STC_DEF_H_DEF
		SocketData.nMsgID = _stItemInx::_DefNumHdef;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_NICK); // IDC_STC_DEF_NICK
		SocketData.nMsgID = _stItemInx::_DefNumNick;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_POI); // IDC_STC_DEF_POI
		SocketData.nMsgID = _stItemInx::_DefNumPoi;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_OPEN); // IDC_STC_DEF_VH_OPEN
		SocketData.nMsgID = _stItemInx::_DefNumVhOpen;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_MISS); // IDC_STC_DEF_VH_MISS
		SocketData.nMsgID = _stItemInx::_DefNumVhMiss;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_POSITION); // IDC_STC_DEF_VH_POS
		SocketData.nMsgID = _stItemInx::_DefNumVhPos;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_VH_DEF); // IDC_STC_DEF_VH_DEF
		SocketData.nMsgID = _stItemInx::_DefNumVhd;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_LIGHT); // IDC_STC_DEF_LIGHT
		SocketData.nMsgID = _stItemInx::_DefNumLight;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EDGE_NICK);
		SocketData.nMsgID = _stItemInx::_DefNumEnick;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EDGE_PROT);
		SocketData.nMsgID = _stItemInx::_DefNumEprot;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_EDGE_SPACE);
		SocketData.nMsgID = _stItemInx::_DefNumEspace;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_USER_DEFINE_1);
		SocketData.nMsgID = _stItemInx::_DefNumUdd1;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_NARROW);
		SocketData.nMsgID = _stItemInx::_DefNumNrw;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);

		nNum = pReelMap->GetDefNum(DEF_WIDE);
		SocketData.nMsgID = _stItemInx::_DefNumWide;
		SocketData.nData1 = nNum;
		SendCommand(SocketData);
	}
}

void CEngrave::Set2DReader()
{
	Set2DEngLen();
	Set2DAoiLen();
	Set2DMkLen();
	Set2DMoveVel();
	Set2DMoveAcc();
	Set2DOneShotLen();
}

void CEngrave::SetEngInfo()
{
	SetEngLeadPitch();
	SetEngPushOffLen();
	SetEngTqVal();
	SetEngAoiLen();
	SetEngFdDiffMax();
	SetEngFdDiffRng();
	SetEngFdDiffNum();
	SetEngBuffInitPos();
	SetEngBuffCurrPos();
}

void CEngrave::SetFdInfo()
{
	SetFdVel();
	SetFdAcc();
	SetOnePnlVel();
	SetOnePnlAcc();
	SetFdDiffMax();
	SetFdDiffRng();
	SetFdDiffNum();
}

void CEngrave::SetAoiInfo()
{
	SetAoiLeadPitch();
	SetAoiPushOffLen();
	SetAoiTqVal();
	SetAoiBuffShotNum();
	SetAoiMkLen();
}

void CEngrave::SetMkInfo()
{
	SetMkLeadPitch();
	SetMkPushOffLen();
	SetMkTqVal();
	SetMkBuffInitPos();
	SetMkBuffCurrPos();
	SetMkNumLf();
	SetMkNumRt();
	SetMkMaxNumLf();
	SetMkMaxNumRt();
}

void CEngrave::SetMkInfoLf()
{
	SetMkInitPosLf();
	SetMkInitVelLf();
	SetMkInitAccLf();
	SetMkFnlPosLf();
	SetMkFnlVelLf();
	SetMkFnlAccLf();
	SetMkFnlTqLf();
	SetMkHgtPosX1Lf();
	SetMkHgtPosY1Lf();
	SetMkHgtPosX2Lf();
	SetMkHgtPosY2Lf();
	SetMkHgtPosX3Lf();
	SetMkHgtPosY3Lf();
	SetMkHgtPosX4Lf();
	SetMkHgtPosY4Lf();
	SetMkHgtAvgPosLf();
}

void CEngrave::SetMkInfoRt()
{
	SetMkInitPosRt();
	SetMkInitVelRt();
	SetMkInitAccRt();
	SetMkFnlPosRt();
	SetMkFnlVelRt();
	SetMkFnlAccRt();
	SetMkFnlTqRt();
	SetMkHgtPosX1Rt();
	SetMkHgtPosY1Rt();
	SetMkHgtPosX2Rt();
	SetMkHgtPosY2Rt();
	SetMkHgtPosX3Rt();
	SetMkHgtPosY3Rt();
	SetMkHgtPosX4Rt();
	SetMkHgtPosY4Rt();
	SetMkHgtAvgPosRt();
}

// SetOpInfo()
void CEngrave::SetOpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_OpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sSelUserName, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetDualTest()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DualTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDualTest ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispContRun()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DispContRun;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDispContRun ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispLotEnd()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DispLotEnd;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bDispLotEnd ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSampleTest()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_SampleTest;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bSampleTest ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSampleShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_SampleShotNum;
	SocketData.nData1 = _ttoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
	SendCommand(SocketData);
}

void CEngrave::SetTestMode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _ItemInx::_TestMode;
	SocketData.nData1 = pDoc->GetTestMode(); // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
	//SocketData.nData1 = pDoc->WorkingInfo.LastJob.nTestMode; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
	SendCommand(SocketData);
}

void CEngrave::SetRecoilerCcw()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_RecoilerCcw;						// OneMetal : TRUE -> SetTwoMetal(FALSE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bOneMetal ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUncoilerCcw()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UncoilerCcw;						// TwoMetal : TRUE -> SetTwoMetal(TRUE);
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTwoMetal ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetAlignMethode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _stSigInx::_AlignMethode;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.nAlignMethode; // TWO_POINT, FOUR_POINT
	SendCommand(SocketData);
}

void CEngrave::SetDoorRecoiler()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorRecoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorAoiUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorMk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorEngrave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bEngvDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDoorUncoiler()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoorUncoiler;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUclDrSen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetSaftyMk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_SaftyMk;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bMkSftySen ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetCleannerAoiUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CleannerAoiUp;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetCleannerAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CleannerAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicAoiDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UltraSonicAoiDn;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicEngrave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UltraSonicEngrave;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetTotReelLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_TotReelLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sReelTotLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_OnePnlLen;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlLen());
	SendCommand(SocketData);
}

void CEngrave::SetTempPause()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TempPause;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bTempPause ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetUse380mm()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Use380mm;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bUse380mm ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetTempStopLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_TempStopLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sTempPauseLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotCut()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LotCut;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bLotSep ? 1 : 0;	// pDoc->m_pReelMap->m_bUseLotSep
	SendCommand(SocketData);
}

void CEngrave::SetLotCutLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotCutLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSepLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotCutPosLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotCutPosLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotCutPosLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLmtTotYld()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LmtTotYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtTotYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLmtPatlYld()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LmtPatlYld;
	StringToChar(pDoc->WorkingInfo.LastJob.sLmtPatlYld, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetStripOutRatio()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_StripOutRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sStripOutRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetCustomNeedRatio()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_CustomNeedRatio;
	StringToChar(pDoc->WorkingInfo.LastJob.sCustomNeedRatio, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetNumRangeFixDef()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_NumRangeFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumRangeFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetFixDef()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FixDef;
	SocketData.nData1 = pDoc->WorkingInfo.LastJob.bContFixDef ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetNumContFixDef()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_NumContFixDef;
	StringToChar(pDoc->WorkingInfo.LastJob.sNumContFixDef, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetUltraSonicStTim()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_UltraSonicStTim;
	StringToChar(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngItsCode()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngItsCode;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngItsCode, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetInfo()
void CEngrave::SetModelName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_ModelUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sModel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

//void CEngrave::SetModelDnName()
//{
//	if (!pDoc)
//		return;
//
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetData;
//	char cData[BUFFER_DATA_SIZE];
//
//	SocketData.nMsgID = _stItemInx::_ModelDnName;
//	StringToChar(pDoc->WorkingInfo.LastJob.sModelDn, cData);
//	sprintf(SocketData.strData, "%s", cData);
//	SendCommand(SocketData);
//}

void CEngrave::SetLotName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

//void CEngrave::SetLotDnName()
//{
//	if (!pDoc)
//		return;
//
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetData;
//	char cData[BUFFER_DATA_SIZE];
//
//	SocketData.nMsgID = _stItemInx::_LotDnName;
//	StringToChar(pDoc->WorkingInfo.LastJob.sLotDn, cData);
//	sprintf(SocketData.strData, "%s", cData);
//	SendCommand(SocketData);
//}

void CEngrave::SetLayerUpName()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LayerUpName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerUp, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLayerDnName()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LayerDnName;
	StringToChar(pDoc->WorkingInfo.LastJob.sLayerDn, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLoadMstInfo()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_LoadMstInfo;
	SendCommand(SocketData);
}

void CEngrave::SetOrderNum()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_OrderNum;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngraveOrderNum, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetShotNum()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_ShotNum;
	StringToChar(pDoc->WorkingInfo.LastJob.sEngraveLastShot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetTotOpRto()
{
	if (!pDoc || !pView || !pView->m_pDlgFrameHigh) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_TotOpRto;
	double dFdTotLen = (double)pView->m_pDlgFrameHigh->m_nMkLastShot * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	sVal.Format(_T("%d"), (int)(dFdTotLen / _tstof(pDoc->WorkingInfo.LastJob.sReelTotLen) * 100.0));
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetTotVel()
{
	if (!pView) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_TotVel;
	sVal.Format(_T("%.1f"), pView->GetTotVel());
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetPartVel()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_PartVel;
	StringToChar(pDoc->WorkingInfo.LastJob.sPartialSpd, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkDoneLen()
{
	if (!pDoc || !pView || !pView->m_pDlgFrameHigh) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_MkDoneLen;
	double dFdTotLen = (double)pView->m_pDlgFrameHigh->m_nMkLastShot * _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen);
	sVal.Format(_T("%.2f"), dFdTotLen / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiDnDoneLen()
{
	if (!pView) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_AoiDnDoneLen;
	sVal.Format(_T("%.2f"), pView->GetAoiDnFdLen() / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiUpDoneLen()
{
	if (!pView) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];
	CString sVal;

	SocketData.nMsgID = _stItemInx::_AoiUpDoneLen;
	sVal.Format(_T("%.2f"), pView->GetAoiUpFdLen() / 1000.0);	// [M]
	StringToChar(sVal, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetLotSerial()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_LotSerial;
	StringToChar(pDoc->WorkingInfo.LastJob.sLotSerial, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkVerfyLen()
{
	if (!pDoc) return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkVerfyLen;
	StringToChar(pDoc->WorkingInfo.LastJob.sVerifyLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetTotRatio()
void CEngrave::SetDefNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;

	
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면
	nTot = nGood + nBad;

	SocketData.nMsgID = _stItemInx::_DefNumUp;
	SocketData.nData1 = nBad;
	SendCommand(SocketData);
}

void CEngrave::SetDefRtoUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	if (nTot > 0)
		dRatio = ((double)nBad / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_DefRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);
}

void CEngrave::SetGoodNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	
	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	SocketData.nMsgID = _stItemInx::_GoodNumUp;
	SocketData.nData1 = nGood;
	SendCommand(SocketData);
}

void CEngrave::SetGoodRtoUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;

	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면

	if (nTot > 0)
		dRatio = ((double)nGood / (double)nTot) * 100.0;
	else
		dRatio = 0.0;

	SocketData.nMsgID = _stItemInx::_GoodRtoUp;
	SocketData.fData1 = dRatio;
	SendCommand(SocketData);
}

void CEngrave::SetTestNumUp()
{
	if (!pDoc || !pDoc->m_pReelMapUp)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;

	pDoc->m_pReelMapUp->GetPcsNum(nGood, nBad); // 상면
	nTot = nGood + nBad;

	SocketData.nMsgID = _stItemInx::_TestNumUp;
	SocketData.nData1 = nTot;
	SendCommand(SocketData);
}

void CEngrave::SetDefNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{		
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_DefNumDn;
		SocketData.nData1 = nBad;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefRtoDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nBad / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_DefRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면

		SocketData.nMsgID = _stItemInx::_GoodNumDn;
		SocketData.nData1 = nGood;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodRtoDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_GoodRtoDn;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetTestNumDn()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapDn->GetPcsNum(nGood, nBad); // 하면
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_TestNumDn;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{		
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_DefNumTot;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

void CEngrave::SetDefRtoTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nBad / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_DefRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체

		SocketData.nMsgID = _stItemInx::_GoodNumTot;
		SocketData.nData1 = nGood;
		SendCommand(SocketData);
	}
}

void CEngrave::SetGoodRtoTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	double dRatio = 0.0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		if (nTot > 0)
			dRatio = ((double)nGood / (double)nTot) * 100.0;
		else
			dRatio = 0.0;

		SocketData.nMsgID = _stItemInx::_GoodRtoTot;
		SocketData.fData1 = dRatio;
		SendCommand(SocketData);
	}
}

void CEngrave::SetTestNumTot()
{
	if (!pDoc || !pDoc->m_pReelMapDn)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	int nGood = 0, nBad = 0, nTot = 0;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (bDualTest)
	{
		pDoc->m_pReelMapAllDn->GetPcsNum(nGood, nBad); // 전체
		nTot = nGood + nBad;

		SocketData.nMsgID = _stItemInx::_TestNumTot;
		SocketData.nData1 = nTot;
		SendCommand(SocketData);
	}
}

// Set2DReader()
void CEngrave::Set2DEngLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DEngLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DEngLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DAoiLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DAoiLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMkLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DMkLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMkLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMoveVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DMoveVel;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DMoveAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DMoveAcc;
	StringToChar(pDoc->WorkingInfo.Motion.s2DMoveAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::Set2DOneShotLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_2DOneShotLen;
	StringToChar(pDoc->WorkingInfo.Motion.s2DOneShotRemainLen, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetEngInfo()
void CEngrave::SetEngLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngAoiLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffMax()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngFdDiffMax;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffMax, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffRng()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngFdDiffRng;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffRng, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngFdDiffNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngFdDiffNum;
	StringToChar(pDoc->WorkingInfo.Motion.sEngFdDiffNum, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffInitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffCurrPos()
{
	if (!pDoc || !pDoc->m_pMpeData)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	CString str;
	double dBufEnc = (double)pDoc->m_pMpeData[1][15] / 1000.0;	// 각인부 버퍼 엔코더 값(단위 mm * 1000)
	str.Format(_T("%.1f"), dBufEnc);
	pDoc->WorkingInfo.Motion.sEngBuffCurrPos = str;

	SocketData.nMsgID = _stItemInx::_EngBuffCurrPos;
	StringToChar(pDoc->WorkingInfo.Motion.sEngBuffCurrPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffUp()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffJogCw;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffJogCw ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffDn()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffJogCcw;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffJogCcw ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffHome()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffOrgMv;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffHomming ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffHomeDone()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffOrgMvDone;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffHommingDone ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitMove()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffInitPosMv;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffInitMv ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitMoveDone()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffInitPosMvDone;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffInitMvDone ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetEngBuffInitPosSave()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngBuffInitPosSave;
	SocketData.nData1 = pDoc->WorkingInfo.Motion.bEngBuffInitPosSave ? 1 : 0;
	SendCommand(SocketData);
}


// SetFdInfo()
void CEngrave::SetFdVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_FdVel;
	//StringToChar(pDoc->WorkingInfo.Motion.sMkFdVel, cData);
	StringToChar(pDoc->WorkingInfo.Motion.sMkJogVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetFdAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_FdAcc;
	//StringToChar(pDoc->WorkingInfo.Motion.sMkFdAcc, cData);
	StringToChar(pDoc->WorkingInfo.Motion.sMkJogAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlVel()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_OnePnlVel;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlVel());
	SendCommand(SocketData);
}

void CEngrave::SetOnePnlAcc()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_OnePnlAcc;
	sprintf(SocketData.strData, "%.3f", pDoc->GetOnePnlAcc());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffMax()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_FdDiffMax;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrLmt());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffRng()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_FdDiffRng;
	sprintf(SocketData.strData, "%.3f", pDoc->GetFdErrRng());
	SendCommand(SocketData);
}

void CEngrave::SetFdDiffNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_FdDiffNum;
	sprintf(SocketData.strData, "%d", pDoc->GetFdErrNum());
	SendCommand(SocketData);
}

// SetAoiInfo()
void CEngrave::SetAoiLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sAoiTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiBuffShotNum()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiBuffShotNum;
	StringToChar(pDoc->WorkingInfo.Motion.sFdAoiAoiDistShot, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetAoiMkLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_AoiMkLen;
	StringToChar(pDoc->WorkingInfo.Motion.sFdMkAoiInitDist, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// SetMkInfo()
void CEngrave::SetMkLeadPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkPushOffLen()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkPushOffLen;
	StringToChar(pDoc->WorkingInfo.Motion.sMkFdVacOff, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkTqVal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkTqVal;
	StringToChar(pDoc->WorkingInfo.Motion.sMkTq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkBuffInitPos()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkBuffInitPos;
	StringToChar(pDoc->WorkingInfo.Motion.sStBufPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkBuffCurrPos() // (Engrave)
{
	if (!pDoc || !pDoc->m_pMpeData)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkBuffCurrPos;
	double dBufEnc = (double)pDoc->m_pMpeData[1][15] / 1000.0;	// 각인부 버퍼 엔코더 값(단위 mm * 1000)
																//sprintf(SocketData.strData, "%.1f", dBufEnc);
	SocketData.fData1 = (float)dBufEnc;
	SendCommand(SocketData);
}

void CEngrave::SetMkNumLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkNumLf;
	SocketData.nData1 = pDoc->GetMkCntL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntL());
	SendCommand(SocketData);
}

void CEngrave::SetMkNumRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkNumRt;
	SocketData.nData1 = pDoc->GetMkCntR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkCntR());
	SendCommand(SocketData);
}

void CEngrave::SetMkMaxNumLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkMaxNumLf;
	SocketData.nData1 = pDoc->GetMkLimitL();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitL());
	SendCommand(SocketData);
}

void CEngrave::SetMkMaxNumRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkMaxNumRt;
	SocketData.nData1 = pDoc->GetMkLimitR();
	//sprintf(SocketData.strData, "%d", pDoc->GetMkLimitR());
	SendCommand(SocketData);
}

// SetMkInfoLf()
void CEngrave::SetMkInitPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitPosLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitVelLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitVelLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitAccLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitAccLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlPosLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlVelLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlVelLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlAccLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlAccLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlTqLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlTqLf;
	StringToChar(pDoc->WorkingInfo.Marking[0].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX1Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY1Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY1Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX2Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY2Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY2Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX3Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY3Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY3Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX4Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX1_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY4Lf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY4Lf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY1_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtAvgPosLf()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtAvgPosLf;
	sprintf(SocketData.strData, "%.3f", pDoc->GetAverDist1());
	SendCommand(SocketData);
}

// SetMkInfoRt()
void CEngrave::SetMkInitPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitPosRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitVelRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _ItemInx::_MkInitVelRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkInitAccRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkInitAccRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sWaitAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlPosRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingPos, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlVelRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlVelRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingVel, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlAccRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlAccRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingAcc, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkFnlTqRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_MkFnlTqRt;
	StringToChar(pDoc->WorkingInfo.Marking[1].sMarkingToq, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX1Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY1Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY1Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_1());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX2Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY2Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY2Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_2());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX3Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY3Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY3Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_3());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosX4Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosX4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosX2_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtPosY4Rt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtPosY4Rt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetPosY2_4());
	SendCommand(SocketData);
}

void CEngrave::SetMkHgtAvgPosRt()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;

	SocketData.nMsgID = _stItemInx::_MkHgtAvgPosRt;
	sprintf(SocketData.strData, "%.3f", pDoc->GetAverDist2());
	SendCommand(SocketData);
}

// End for SetSysData()


// Start Switch

// Main
void CEngrave::SwReady(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Main.Ready = bOn;
	SocketData.nMsgID = _SigInx::_Ready;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwReady(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsReady;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRun(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Main.Run = bOn;
	SocketData.nMsgID = _SigInx::_Run;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRun(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRun;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Reset;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsReset;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwStop(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Stop;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwStop(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsStop;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAuto(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Auto;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAuto(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsAuto;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwManual(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Manual;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwManual(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsManual;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Torque Motor
void CEngrave::SwMkTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MkTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMkTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_AoiTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsAoiTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EngTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTq(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsEngTq;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Induction Motor
void CEngrave::SwRcInductionCcw(BOOL bOn)	// SetOneMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CcwModRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcInductionCcw(BOOL bOn)	// SetOneMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCcwModRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcInductionCcw(BOOL bOn)	// SetTwoMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_CcwModUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcInductionCcw(BOOL bOn)	// SetTwoMetal
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCcwModUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Core 150mm
void CEngrave::SwRcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Core150Re;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCore150Re;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Core150Un;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcCore150mm(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsCore150Un;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// Recoiler
void CEngrave::SwRcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PrdChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPrdChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpLfRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpLfRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpRtRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpRtRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteVacRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteVacRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprChuckRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCcwRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcRewinder(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DoRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcRewinder(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDoRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwRcRewinderReelPaper(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PrdPprRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwRcRewinderReelPaper(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPrdPprRe;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Punching
void CEngrave::SwMkRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTblBlw(BOOL bOn)	
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMkDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwMkDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpMk;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// AOIDn
void CEngrave::SwAoiDnRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiDnClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_VelClrSonicAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsVelClrSonicAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiDnTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TestAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTestAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ResetAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsResetAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiDnLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LotEndAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiDnLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLotEndAoiDn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}



// AOIUp
void CEngrave::SwAoiUpRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpClrRoll(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::SwAoiUpTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TestAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTestAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ResetAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpReset(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsResetAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwAoiUpLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LotEndAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiUpLotEnd(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLotEndAoiUp;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Engrave
void CEngrave::SwEngRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PushUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngPushUp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPushUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblBlwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTblBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblBlwEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TblVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTblVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTblVacEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_FdClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngFdClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsFdClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_TensClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngTqClp(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsTensClampEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_OnePnlEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngMvOne(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsOnePnlEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_LsrEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngLsrPt(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsLsrEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngDcRSol(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_VelClrSonicEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngVelSonicBlw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsVelClrSonicEng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Uncoiler
void CEngrave::SwUcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_Relation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcRelation(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsRelation;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcFdCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_MvCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcFdCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsMvCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PrdChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPrdChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DancerUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcDcRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDancerUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpLfUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelJoinL(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpLfUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteUpRtUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelJoinR(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteUpRtUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PasteVacUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcReelJoinVac(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPasteVacUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcPaperChuck(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprChuckUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcPaperCw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_PprCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcPaperCcw(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsPprCcwUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcClRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcClRlUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollUpUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwUcClRlPshUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_ClrRollPushUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwUcClRlPshUpDn(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsClrRollPushUn;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Etc
void CEngrave::SwAoiEmg(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_EmgAoi;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwAoiEmg(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsEmgAoi;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

// EngraveAutoSequence

void CEngrave::EngDispAlarm(BOOL bOn) // 각인부에 Alarm 표시
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.Init = bOn;
	SocketData.nMsgID = _SigInx::_MyMsg;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsEngDispAlarm(BOOL bOn) // 각인부에 Alarm 표시 Return 확인
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.Init = bOn;
	SocketData.nMsgID = _SigInx::_IsMyMsg;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoInit(BOOL bOn) // 각인부 초기화(Reset)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.Init = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoInit;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoInit(BOOL bOn) // 각인부 초기화(Reset)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.Init = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoInit;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoMkSt(BOOL bOn) // 각인부 마킹시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.MkSt = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqMkSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoMkSt(BOOL bOn) // 각인부 마킹시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.MkSt = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqMkSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoOnMking(BOOL bOn) // 각인부 마킹중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.OnMking = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqOnMkIng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoOnMking(BOOL bOn) // 각인부 마킹중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.OnMking = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqOnMkIng;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoMkDone(BOOL bOn) // 각인부 마킹완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.MkDone = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqMkDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoMkDone(BOOL bOn) // 각인부 마킹완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.MkDone = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqMkDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAuto2dReadSt(BOOL bOn) // 각인부 2D Read 시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.Read2dSt = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeq2dReadSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAuto2dReadSt(BOOL bOn) // 각인부 2D Read 시작 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.Read2dSt = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeq2dReadSt;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAutoOnReading2d(BOOL bOn) // 각인부 Read중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.OnRead2d = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeqOnReading2d;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAutoOnReading2d(BOOL bOn) // 각인부 Read중 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.OnRead2d = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeqOnReading2d;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwEngAuto2dReadDone(BOOL bOn) // 각인부 2D Read 완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.Read2dDone = bOn;
	SocketData.nMsgID = _SigInx::_EngAutoSeq2dReadDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSwEngAuto2dReadDone(BOOL bOn) // 각인부 2D Read 완료 ON (PC가 ON, OFF)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.EngAuto.Read2dDone = bOn;
	SocketData.nMsgID = _SigInx::_IsEngAutoSeq2dReadDone;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


// Set Engrave Data
void CEngrave::SetEngraveAoiDist()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngAoiLen;
	StringToChar(pDoc->WorkingInfo.Motion.sFdEngraveAoiInitDist, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetEngraveFdPitch()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stItemInx::_EngLeadPitch;
	StringToChar(pDoc->WorkingInfo.Motion.sEngraveFdLead, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// DlgMyMsg
void CEngrave::SetMyMsgYes()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Msg.Yes = TRUE;
	SocketData.nMsgID = _SigInx::_MyMsgYes;
	SocketData.nData1 =1;
	SendCommand(SocketData);
}

void CEngrave::IsSetMyMsgYes()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.Msg.IsYes = TRUE;
	SocketData.nMsgID = _SigInx::_IsMyMsgYes;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::SetMyMsgNo()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Msg.No = TRUE;
	SocketData.nMsgID = _SigInx::_MyMsgNo;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::IsSetMyMsgNo()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.Msg.IsNo = TRUE;
	SocketData.nMsgID = _SigInx::_IsMyMsgNo;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::SetMyMsgOk()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Msg.No = TRUE;
	SocketData.nMsgID = _SigInx::_MyMsgOk;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::IsSetMyMsgOk()
{
	if (!pDoc)	return;	

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//pDoc->BtnStatus.Msg.IsNo = TRUE;
	SocketData.nMsgID = _SigInx::_IsMyMsgOk;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

// CurrentInfoSignal
void CEngrave::SetCurrentInfoSignal(int nMsgID, int nData)
{
	if (!pDoc)	return;	

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	//SocketData.nMsgID = _SigInx::_GetCurrentInfoSignal;
	//SocketData.nData1 = nIdxSig;
	SocketData.nMsgID = nMsgID;
	SocketData.nData1 = nData;
	SendCommand(SocketData);
}

void CEngrave::IsSetCurrentInfoSignal()
{
	if (!pDoc)	return;	

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsGetCurrentInfoSignal;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

// MonDispMain
void CEngrave::SetMonDispMainSignal()
{
	if (!pDoc)	return;	

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_GetMonDispMainSignal;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

void CEngrave::IsSetMonDispMainSignal()
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsGetMonDispMainSignal;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}

// Disp
void CEngrave::SetDispReady(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.Ready = TRUE;
	SocketData.nMsgID = _SigInx::_DispReady;
	SocketData.nData1 = pDoc->BtnStatus.Disp.Ready > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispReady(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispReady;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispRun(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.Run = TRUE;
	SocketData.nMsgID = _SigInx::_DispRun;
	SocketData.nData1 = pDoc->BtnStatus.Disp.Run > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispRun(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispRun;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispStop(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.Stop = TRUE;
	SocketData.nMsgID = _SigInx::_DispStop;
	SocketData.nData1 = pDoc->BtnStatus.Disp.Stop > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispStop(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispStop;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispDualSample(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.DualSample = TRUE;
	SocketData.nMsgID = _SigInx::_DispDualSample;
	SocketData.nData1 = pDoc->BtnStatus.Disp.DualSample > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispDualSample(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispDualSample;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispSingleSample(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.SingleSample = TRUE;
	SocketData.nMsgID = _SigInx::_DispSingleSample;
	SocketData.nData1 = pDoc->BtnStatus.Disp.SingleSample > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispSingleSample(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispSingleSample;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispDualTest(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.DualTest = TRUE;
	SocketData.nMsgID = _SigInx::_DispDualTest;
	SocketData.nData1 = pDoc->BtnStatus.Disp.DualTest > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispDualTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispDualTest;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SetDispSingleTest(BOOL bOn)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	pDoc->BtnStatus.Disp.Init();
	pDoc->BtnStatus.Disp.SingleTest = TRUE;
	SocketData.nMsgID = _SigInx::_DispSingleTest;
	SocketData.nData1 = pDoc->BtnStatus.Disp.SingleTest > 0 ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::IsSetDispSingleTest(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsDispSingleTest;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


//// Disp
//void CEngrave::SetDispReady(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	pDoc->BtnStatus.Disp.Ready = bOn;
//	SocketData.nMsgID = _SigInx::_DispReady;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}
//
//void CEngrave::IsSetDispReady(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	SocketData.nMsgID = _SigInx::_IsDispReady;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}
//
//void CEngrave::SetDispRun(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	pDoc->BtnStatus.Disp.Run = bOn;
//	SocketData.nMsgID = _SigInx::_DispRun;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}
//
//void CEngrave::IsSetDispRun(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	SocketData.nMsgID = _SigInx::_IsDispRun;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}
//
//void CEngrave::SetDispStop(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	pDoc->BtnStatus.Disp.Stop = bOn;
//	SocketData.nMsgID = _SigInx::_DispStop;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}
//
//void CEngrave::IsSetDispStop(BOOL bOn)
//{
//	SOCKET_DATA SocketData;
//	SocketData.nCmdCode = _SetSig;
//
//	SocketData.nMsgID = _SigInx::_IsDispStop;
//	SocketData.nData1 = bOn ? 1 : 0;
//	SendCommand(SocketData);
//}

// End Switch

// Alarm

void CEngrave::SetAlarm(CString sMsg)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stAlarmInx::_Alarm;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::IsSetAlarm(CString sMsg)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stAlarmInx::_IsAlarm;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

void CEngrave::SetMsgBox(CString sMsg, int nType)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stMsgBoxInx::_MsgBox;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SocketData.nData1 = nType;
	SendCommand(SocketData);
}

void CEngrave::IsSetMsgBox(CString sMsg)
{
	if (!pDoc)
		return;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetData;
	char cData[BUFFER_DATA_SIZE];

	SocketData.nMsgID = _stMsgBoxInx::_IsMsgBox;
	StringToChar(sMsg, cData);
	sprintf(SocketData.strData, "%s", cData);
	SendCommand(SocketData);
}

// On Running Auto

BOOL CEngrave::UpdateWorking()
{
	SetTotOpRto();		// 전체진행율
						// 로트진행율
	SetTotVel();		// 전체속도
	SetPartVel();		// 구간속도
	SetMkDoneLen();		// 마킹부 : Distance (FdDone) [M]
	SetAoiDnDoneLen();	// 검사부(하) : Distance (FdDone) [M]
	SetAoiUpDoneLen();	// 검사부(상) : Distance (FdDone) [M]
						// 각인부 : Distance (FdDone) [M]

	return TRUE;
}

BOOL CEngrave::UpdateRst()
{
	UpdateTotRatio();
	UpdateStripRatio();

	return TRUE;
}

BOOL CEngrave::UpdateTotRatio()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	// 상면
	SetDefNumUp();			// IDC_STC_DEFECT_NUM
	SetDefRtoUp();			// IDC_STC_DEFECT_RATIO
	SetGoodNumUp();			// IDC_STC_GOOD_NUM
	SetGoodRtoUp();			// IDC_STC_GOOD_RATIO
	SetTestNumUp();			// IDC_STC_TOTAL_NUM

	if (bDualTest)
	{
		// 하면
		SetDefNumDn();		// IDC_STC_DEFECT_NUM_DN
		SetDefRtoDn();		// IDC_STC_DEFECT_RATIO_DN
		SetGoodNumDn();		// IDC_STC_GOOD_NUM_DN
		SetGoodRtoDn();		// IDC_STC_GOOD_RATIO_DN
		SetTestNumDn();		// IDC_STC_TOTAL_NUM_DN

							// 전체
		SetDefNumTot();		// IDC_STC_DEFECT_NUM_ALL
		SetDefRtoTot();		// IDC_STC_DEFECT_RATIO_ALL
		SetGoodNumTot();	// IDC_STC_GOOD_NUM_ALL
		SetGoodRtoTot();	// IDC_STC_GOOD_RATIO_ALL
		SetTestNumTot();	// IDC_STC_TOTAL_NUM_ALL
	}

	return TRUE;
}

BOOL CEngrave::UpdateStripRatio()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	return TRUE;
}

BOOL CEngrave::UpdateDef()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	return TRUE;
}


void CEngrave::SwMenu01UpdateWorking(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_UpdateWork;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::IsSwMenu01UpdateWorking(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_IsUpdateWork;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::SwMenu01DispDefImg(BOOL bOn)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_DispDefImg;
	SocketData.nData1 = bOn ? 1 : 0;
	SendCommand(SocketData);
}


void CEngrave::Set2DOffsetInitPosMove(BOOL bOn)
{
	if (!pDoc)
		return;

	pDoc->BtnStatus.SettingEng.OffsetInitPosMove = bOn;

	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _stSigInx::_2DOffsetInitPosMove;
	SocketData.nData1 = pDoc->BtnStatus.SettingEng.OffsetInitPosMove ? 1 : 0;
	SendCommand(SocketData);
}

void CEngrave::GetSignal2dEng(SOCKET_DATA SockData)
{
	int nCmdCode = SockData.nCmdCode;
	int nMsgId = SockData.nMsgID;
	CString sVal;
	long lData;

	if (nCmdCode == _SetSig)
	{
		switch (nMsgId)
		{
		case _SigInx::_2DOffsetInitPos:
			//m_bRcvSig[_SigInx::_2DOffsetInitPos] = TRUE;
			break;
		}
	}
}


int CEngrave::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_bTIM_CHK_RCV_SIG = TRUE;
	SetTimer(TIM_CHK_RCV_SIG, 500, NULL);

	return 0;
}

void CEngrave::SetSignal(int nMsgID, int nData)
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = nMsgID;
	SocketData.nData2 = nData;
	SendCommand(SocketData);
	m_bSendSig[nMsgID] = TRUE; // 상대방이 받지 못하면 500mSec 후에 다시 신호를 전송함.
	m_nSendSigData[nMsgID] = nData;

	if (pView->m_pDlgMenu02)
		pView->m_pDlgMenu02->GetDlgItem(IDC_STATIC_SIGNAL_SEND)->SetWindowText(GetSignalName(nMsgID));
}

CString CEngrave::GetSignalName(int nMsgID)
{
	if (nMsgID >= _SigInx::_EndIdx)
		return _T("");
	return m_sSignalName[nMsgID];
}

void CEngrave::SetSignalName()
{
	m_sSignalName[_SigInx::_Idle] = _T("_Idle"); m_sSignalName[_SigInx::_Busy] = _T("_Busy"); m_sSignalName[_SigInx::_Connect] = _T("_Connect");
	m_sSignalName[_SigInx::_DispDualSample] = _T("_DispDualSample"); m_sSignalName[_SigInx::_DispDualTest] = _T("_DispDualTest"); m_sSignalName[_SigInx::_DispSingleSample] = _T("_DispSingleSample");
	m_sSignalName[_SigInx::_DispSingleTest] = _T("_DispSingleTest"); m_sSignalName[_SigInx::_DispInitRun] = _T("_DispInitRun"); m_sSignalName[_SigInx::_DispRun] = _T("_DispRun");
	m_sSignalName[_SigInx::_DispStop] = _T("_DispStop"); m_sSignalName[_SigInx::_DispReady] = _T("_DispReady"); m_sSignalName[_SigInx::_DualTest] = _T("_DualTest");
	//m_sSignalName[_SigInx::_SampleTest] = _T("_SampleTest"); m_sSignalName[_SigInx::_TestMode] = _T("_TestMode"); m_sSignalName[_SigInx::_RecoilerCcw] = _T("_RecoilerCcw");
	m_sSignalName[_SigInx::_UncoilerCcw] = _T("_UncoilerCcw"); m_sSignalName[_SigInx::_AlignMethode] = _T("_AlignMethode"); m_sSignalName[_SigInx::_DoorRecoiler] = _T("_DoorRecoiler"); 
	m_sSignalName[_SigInx::_DoorAoiUp] = _T("_DoorAoiUp"); m_sSignalName[_SigInx::_DoorAoiDn] = _T("_DoorAoiDn"); m_sSignalName[_SigInx::_DoorMk] = _T("_DoorMk");
	m_sSignalName[_SigInx::_DoorEngrave] = _T("_DoorEngrave"); m_sSignalName[_SigInx::_DoorUncoiler] = _T("_DoorUncoiler"); m_sSignalName[_SigInx::_SaftyMk] = _T("_SaftyMk"); 
	m_sSignalName[_SigInx::_CleannerAoiUp] = _T("_CleannerAoiUp"); m_sSignalName[_SigInx::_CleannerAoiDn] = _T("_CleannerAoiDn"); m_sSignalName[_SigInx::_UltraSonicAoiDn] = _T("_UltraSonicAoiDn");
	m_sSignalName[_SigInx::_UltraSonicEngrave] = _T("_UltraSonicEngrave"); m_sSignalName[_SigInx::_FixDef] = _T("_FixDef"); m_sSignalName[_SigInx::_Use380mm] = _T("_Use380mm");
	m_sSignalName[_SigInx::_DispContRun] = _T("_DispContRun"); m_sSignalName[_SigInx::_DispLotEnd] = _T("_DispLotEnd"); m_sSignalName[_SigInx::_TempStop] = _T("_TempStop");
	m_sSignalName[_SigInx::_TempPause] = _T("_TempPause"); m_sSignalName[_SigInx::_LotCut] = _T("_LotCut"); m_sSignalName[_SigInx::_MkPosMv] = _T("_MkPosMv"); m_sSignalName[_SigInx::_MkVerify] = _T("_MkVerify");
	m_sSignalName[_SigInx::_ReelmapUp] = _T("_ReelmapUp"); m_sSignalName[_SigInx::_ReelmapDn] = _T("_ReelmapDn"); m_sSignalName[_SigInx::_ReelmapTot] = _T("_ReelmapTot"); 
	m_sSignalName[_SigInx::_RemainMode] = _T("_RemainMode"); m_sSignalName[_SigInx::_EndWork] = _T("_EndWork"); m_sSignalName[_SigInx::_ReMk] = _T("_ReMk"); m_sSignalName[_SigInx::_2Layer] = _T("_2Layer"); 
	m_sSignalName[_SigInx::_1LnMk] = _T("_1LnMk"); m_sSignalName[_SigInx::_2LnMk] = _T("_2LnMk"); m_sSignalName[_SigInx::_3LnMk] = _T("_3LnMk"); m_sSignalName[_SigInx::_4LnMk] = _T("_4LnMk"); 
	m_sSignalName[_SigInx::_TotLnMk] = _T("_TotLnMk"); m_sSignalName[_SigInx::_UpdateWork] = _T("_UpdateWork"); m_sSignalName[_SigInx::_DispDefImg] = _T("_DispDefImg");
	m_sSignalName[_SigInx::_2DEngLenMvCw] = _T("_2DEngLenMvCw"); m_sSignalName[_SigInx::_2DEngLenMvCcw] = _T("_2DEngLenMvCcw"); m_sSignalName[_SigInx::_2DAoiLenMvCw] = _T("_2DAoiLenMvCw"); 
	m_sSignalName[_SigInx::_2DAoiLenMvCcw] = _T("_2DAoiLenMvCcw"); m_sSignalName[_SigInx::_2DMkLenMvCw] = _T("_2DMkLenMvCw"); m_sSignalName[_SigInx::_2DMkLenMvCcw] = _T("_2DMkLenMvCcw");
	m_sSignalName[_SigInx::_2DOneShotLenCw] = _T("_2DOneShotLenCw"); m_sSignalName[_SigInx::_2DOneShotLenCcw] = _T("_2DOneShotLenCcw"); m_sSignalName[_SigInx::_2DOffsetInitPos] = _T("_2DOffsetInitPos"); 
	m_sSignalName[_SigInx::_2DOffsetInitPosMove] = _T("_2DOffsetInitPosMove"); m_sSignalName[_SigInx::_Ready] = _T("_Ready"); m_sSignalName[_SigInx::_Run] = _T("_Run");
	m_sSignalName[_SigInx::_Reset] = _T("_Reset"); m_sSignalName[_SigInx::_Stop] = _T("_Stop"); m_sSignalName[_SigInx::_Auto] = _T("_Auto"); m_sSignalName[_SigInx::_OneCycle] = _T("_OneCycle"); 
	m_sSignalName[_SigInx::_Manual] = _T("_Manual"); m_sSignalName[_SigInx::_MkTq] = _T("_MkTq");
	m_sSignalName[_SigInx::_AoiTq] = _T("_AoiTq"); m_sSignalName[_SigInx::_EngTq] = _T("_EngTq"); m_sSignalName[_SigInx::_CcwModRe] = _T("_CcwModRe"); m_sSignalName[_SigInx::_CcwModUn] = _T("_CcwModUn"); 
	m_sSignalName[_SigInx::_Core150Re] = _T("_Core150Re"); m_sSignalName[_SigInx::_Core150Un] = _T("_Core150Un"); m_sSignalName[_SigInx::_Relation] = _T("_Relation"); 
	m_sSignalName[_SigInx::_JoinSelRe] = _T("_JoinSelRe"); m_sSignalName[_SigInx::_MvCwRe] = _T("_MvCwRe"); m_sSignalName[_SigInx::_MvCcwRe] = _T("_MvCcwRe"); m_sSignalName[_SigInx::_PrdChuckRe] = _T("_PrdChuckRe"); 
	m_sSignalName[_SigInx::_DancerUpRe] = _T("_DancerUpRe"); m_sSignalName[_SigInx::_PasteUpLfRe] = _T("_PasteUpLfRe"); m_sSignalName[_SigInx::_PasteUpRtRe] = _T("_PasteUpRtRe"); 
	m_sSignalName[_SigInx::_PasteVacRe] = _T("_PasteVacRe"); m_sSignalName[_SigInx::_PprChuckRe] = _T("_PprChuckRe"); m_sSignalName[_SigInx::_PprCwRe] = _T("_PprCwRe"); m_sSignalName[_SigInx::_PprCcwRe] = _T("_PprCcwRe");
	m_sSignalName[_SigInx::_DoRe] = _T("_DoRe"); m_sSignalName[_SigInx::_PrdPprRe] = _T("_PrdPprRe"); m_sSignalName[_SigInx::_JoinSelMk] = _T("_JoinSelMk"); m_sSignalName[_SigInx::_MvCwMk] = _T("_MvCwMk"); 
	m_sSignalName[_SigInx::_MvCcwMk] = _T("_MvCcwMk"); m_sSignalName[_SigInx::_FdVacMk] = _T("_FdVacMk"); m_sSignalName[_SigInx::_PushUpMk] = _T("_PushUpMk"); m_sSignalName[_SigInx::_TblBlwMk] = _T("_TblBlwMk"); 
	m_sSignalName[_SigInx::_TblVacMk] = _T("_TblVacMk"); m_sSignalName[_SigInx::_FdClampMk] = _T("_FdClampMk"); m_sSignalName[_SigInx::_TensClampMk] = _T("_TensClampMk"); m_sSignalName[_SigInx::_OnePnlMk] = _T("_OnePnlMk");
	m_sSignalName[_SigInx::_LsrMk] = _T("_LsrMk"); m_sSignalName[_SigInx::_DancerUpMk] = _T("_DancerUpMk"); m_sSignalName[_SigInx::_TqVacMk] = _T("_TqVacMk"); m_sSignalName[_SigInx::_JoinSelAoiDn] = _T("_JoinSelAoiDn"); 
	m_sSignalName[_SigInx::_MvCwAoiDn] = _T("_MvCwAoiDn"); m_sSignalName[_SigInx::_MvCcwAoiDn] = _T("_MvCcwAoiDn"); m_sSignalName[_SigInx::_FdVacAoiDn] = _T("_FdVacAoiDn");
	m_sSignalName[_SigInx::_PushUpAoiDn] = _T("_PushUpAoiDn"); m_sSignalName[_SigInx::_TblBlwAoiDn] = _T("_TblBlwAoiDn"); m_sSignalName[_SigInx::_TblVacAoiDn] = _T("_TblVacAoiDn"); 
	m_sSignalName[_SigInx::_FdClampAoiDn] = _T("_FdClampAoiDn"); m_sSignalName[_SigInx::_TensClampAoiDn] = _T("_TensClampAoiDn"); m_sSignalName[_SigInx::_OnePnlAoiDn] = _T("_OnePnlAoiDn"); 
	m_sSignalName[_SigInx::_LsrAoiDn] = _T("_LsrAoiDn"); m_sSignalName[_SigInx::_ClrRollAoiDn] = _T("_ClrRollAoiDn"); m_sSignalName[_SigInx::_VelClrSonicAoiDn] = _T("_VelClrSonicAoiDn");
	m_sSignalName[_SigInx::_TestAoiDn] = _T("_TestAoiDn"); m_sSignalName[_SigInx::_ResetAoiDn] = _T("_ResetAoiDn"); m_sSignalName[_SigInx::_LotEndAoiDn] = _T("_LotEndAoiDn"); 
	m_sSignalName[_SigInx::_JoinSelAoiUp] = _T("_JoinSelAoiUp"); m_sSignalName[_SigInx::_MvCwAoiUp] = _T("_MvCwAoiUp"); m_sSignalName[_SigInx::_MvCcwAoiUp] = _T("_MvCcwAoiUp"); 
	m_sSignalName[_SigInx::_FdVacAoiUp] = _T("_FdVacAoiUp"); m_sSignalName[_SigInx::_PushUpAoiUp] = _T("_PushUpAoiUp"); m_sSignalName[_SigInx::_TblBlwAoiUp] = _T("_TblBlwAoiUp");
	m_sSignalName[_SigInx::_TblVacAoiUp] = _T("_TblVacAoiUp"); m_sSignalName[_SigInx::_FdClampAoiUp] = _T("_FdClampAoiUp"); m_sSignalName[_SigInx::_TensClampAoiUp] = _T("_TensClampAoiUp"); 
	m_sSignalName[_SigInx::_OnePnlAoiUp] = _T("_OnePnlAoiUp"); m_sSignalName[_SigInx::_LsrAoiUp] = _T("_LsrAoiUp"); m_sSignalName[_SigInx::_ClrRollAoiUp] = _T("_ClrRollAoiUp"); 
	m_sSignalName[_SigInx::_TestAoiUp] = _T("_TestAoiUp"); m_sSignalName[_SigInx::_ResetAoiUp] = _T("_ResetAoiUp"); m_sSignalName[_SigInx::_LotEndAoiUp] = _T("_LotEndAoiUp");
	m_sSignalName[_SigInx::_JoinSelEng] = _T("_JoinSelEng"); m_sSignalName[_SigInx::_MvCwEng] = _T("_MvCwEng"); m_sSignalName[_SigInx::_MvCcwEng] = _T("_MvCcwEng"); m_sSignalName[_SigInx::_FdVacEng] = _T("_FdVacEng"); 
	m_sSignalName[_SigInx::_PushUpEng] = _T("_PushUpEng"); m_sSignalName[_SigInx::_TblBlwEng] = _T("_TblBlwEng"); m_sSignalName[_SigInx::_TblVacEng] = _T("_TblVacEng"); m_sSignalName[_SigInx::_FdClampEng] = _T("_FdClampEng");
	m_sSignalName[_SigInx::_TensClampEng] = _T("_TensClampEng"); m_sSignalName[_SigInx::_OnePnlEng] = _T("_OnePnlEng"); m_sSignalName[_SigInx::_LsrEng] = _T("_LsrEng"); m_sSignalName[_SigInx::_DancerUpEng] = _T("_DancerUpEng");
	m_sSignalName[_SigInx::_VelClrSonicEng] = _T("_VelClrSonicEng"); m_sSignalName[_SigInx::_JoinSelUn] = _T("_JoinSelUn"); m_sSignalName[_SigInx::_MvCwUn] = _T("_MvCwUn"); m_sSignalName[_SigInx::_MvCcwUn] = _T("_MvCcwUn"); 
	m_sSignalName[_SigInx::_PrdChuckUn] = _T("_PrdChuckUn"); m_sSignalName[_SigInx::_DancerUpUn] = _T("_DancerUpUn"); m_sSignalName[_SigInx::_PasteUpLfUn] = _T("_PasteUpLfUn"); m_sSignalName[_SigInx::_PasteUpRtUn] = _T("_PasteUpRtUn");
	m_sSignalName[_SigInx::_PasteVacUn] = _T("_PasteVacUn"); m_sSignalName[_SigInx::_PprChuckUn] = _T("_PprChuckUn"); m_sSignalName[_SigInx::_PprCwUn] = _T("_PprCwUn"); m_sSignalName[_SigInx::_PprCcwUn] = _T("_PprCcwUn"); 
	m_sSignalName[_SigInx::_ClrRollUpUn] = _T("_ClrRollUpUn"); m_sSignalName[_SigInx::_ClrRollPushUn] = _T("_ClrRollPushUn"); m_sSignalName[_SigInx::_EmgAoi] = _T("_EmgAoi");
	m_sSignalName[_SigInx::_MkResetLf] = _T("_MkResetLf"); m_sSignalName[_SigInx::_MkResetRt] = _T("_MkResetRt"); m_sSignalName[_SigInx::_MkBuffJogCw] = _T("_MkBuffJogCw"); 
	m_sSignalName[_SigInx::_MkBuffJogCcw] = _T("_MkBuffJogCcw"); m_sSignalName[_SigInx::_MkBuffOrgMv] = _T("_MkBuffOrgMv"); m_sSignalName[_SigInx::_MkBuffInitPosMv] = _T("_MkBuffInitPosMv"); 
	m_sSignalName[_SigInx::_MkBuffPosSave] = _T("_MkBuffPosSave"); m_sSignalName[_SigInx::_EngBuffJogCw] = _T("_EngBuffJogCw"); m_sSignalName[_SigInx::_EngBuffJogCcw] = _T("_EngBuffJogCcw");
	m_sSignalName[_SigInx::_EngBuffOrgMv] = _T("_EngBuffOrgMv"); m_sSignalName[_SigInx::_EngBuffOrgMvDone] = _T("_EngBuffOrgMvDone"); m_sSignalName[_SigInx::_EngBuffInitPosMv] = _T("_EngBuffInitPosMv"); 
	m_sSignalName[_SigInx::_EngBuffInitPosMvDone] = _T("_EngBuffInitPosMvDone"); m_sSignalName[_SigInx::_EngBuffInitPosSave] = _T("_EngBuffInitPosSave"); m_sSignalName[_SigInx::_EngAutoInit] = _T("_EngAutoInit");
	m_sSignalName[_SigInx::_EngAutoSeqMkSt] = _T("_EngAutoSeqMkSt"); m_sSignalName[_SigInx::_EngAutoSeqOnMkIng] = _T("_EngAutoSeqOnMkIng"); m_sSignalName[_SigInx::_EngAutoSeqMkDone] = _T("_EngAutoSeqMkDone"); 
	m_sSignalName[_SigInx::_EngAutoSeq2dReadSt] = _T("_EngAutoSeq2dReadSt"); m_sSignalName[_SigInx::_EngAutoSeqOnReading2d] = _T("_EngAutoSeqOnReading2d"); 
	m_sSignalName[_SigInx::_EngAutoSeq2dReadDone] = _T("_EngAutoSeq2dReadDone"); m_sSignalName[_SigInx::_EngAutoSeqFdDone] = _T("_EngAutoSeqFdDone"); m_sSignalName[_SigInx::_MyMsgYes] = _T("_MyMsgYes");
	m_sSignalName[_SigInx::_MyMsgNo] = _T("_MyMsgNo"); m_sSignalName[_SigInx::_MyMsgCancel] = _T("_MyMsgCancel"); m_sSignalName[_SigInx::_MyMsgOk] = _T("_MyMsgOk"); m_sSignalName[_SigInx::_MyMsg] = _T("_MyMsg");
	m_sSignalName[_SigInx::_Buzzer] = _T("_Buzzer"); m_sSignalName[_SigInx::_TowerLamp] = _T("_TowerLamp"); m_sSignalName[_SigInx::_ErrorRead2dCode] = _T("_ErrorRead2dCode"); 
	m_sSignalName[_SigInx::_GetCurrentInfoSignal] = _T("_GetCurrentInfoSignal"); m_sSignalName[_SigInx::_GetMonDispMainSignal] = _T("_GetMonDispMainSignal"); 
	m_sSignalName[_SigInx::_IsIdle] = _T("_IsIdle"); m_sSignalName[_SigInx::_IsBusy] = _T("_IsBusy"); m_sSignalName[_SigInx::_IsConnect] = _T("_IsConnect");
	m_sSignalName[_SigInx::_IsDispDualSample] = _T("_IsDispDualSample"); m_sSignalName[_SigInx::_IsDispDualTest] = _T("_IsDispDualTest"); m_sSignalName[_SigInx::_IsDispSingleSample] = _T("_IsDispSingleSample");
	m_sSignalName[_SigInx::_IsDispSingleTest] = _T("_IsDispSingleTest"); m_sSignalName[_SigInx::_IsDispInitRun] = _T("_IsDispInitRun"); m_sSignalName[_SigInx::_IsDispRun] = _T("_IsDispRun");
	m_sSignalName[_SigInx::_IsDispStop] = _T("_IsDispStop"); m_sSignalName[_SigInx::_IsDispReady] = _T("_IsDispReady"); m_sSignalName[_SigInx::_IsDualTest] = _T("_IsDualTest"); 
	//m_sSignalName[_SigInx::_IsSampleTest] = _T("_IsSampleTest"); m_sSignalName[_SigInx::_IsTestMode] = _T("_IsTestMode"); m_sSignalName[_SigInx::_IsRecoilerCcw] = _T("_IsRecoilerCcw");
	m_sSignalName[_SigInx::_IsUncoilerCcw] = _T("_IsUncoilerCcw"); m_sSignalName[_SigInx::_IsAlignMethode] = _T("_IsAlignMethode"); m_sSignalName[_SigInx::_IsDoorRecoiler] = _T("_IsDoorRecoiler"); 
	m_sSignalName[_SigInx::_IsDoorAoiUp] = _T("_IsDoorAoiUp"); m_sSignalName[_SigInx::_IsDoorAoiDn] = _T("_IsDoorAoiDn"); m_sSignalName[_SigInx::_IsDoorMk] = _T("_IsDoorMk");
	m_sSignalName[_SigInx::_IsDoorEngrave] = _T("_IsDoorEngrave"); m_sSignalName[_SigInx::_IsDoorUncoiler] = _T("_IsDoorUncoiler"); m_sSignalName[_SigInx::_IsSaftyMk] = _T("_IsSaftyMk"); 
	m_sSignalName[_SigInx::_IsCleannerAoiUp] = _T("_IsCleannerAoiUp"); m_sSignalName[_SigInx::_IsCleannerAoiDn] = _T("_IsCleannerAoiDn"); m_sSignalName[_SigInx::_IsUltraSonicAoiDn] = _T("_IsUltraSonicAoiDn");
	m_sSignalName[_SigInx::_IsUltraSonicEngrave] = _T("_IsUltraSonicEngrave"); m_sSignalName[_SigInx::_IsFixDef] = _T("_IsFixDef"); m_sSignalName[_SigInx::_IsUse380mm] = _T("_IsUse380mm"); 
	m_sSignalName[_SigInx::_IsDispContRun] = _T("_IsDispContRun"); m_sSignalName[_SigInx::_IsDispLotEnd] = _T("_IsDispLotEnd"); m_sSignalName[_SigInx::_IsTempStop] = _T("_IsTempStop");
	m_sSignalName[_SigInx::_IsTempPause] = _T("_IsTempPause"); m_sSignalName[_SigInx::_IsLotCut] = _T("_IsLotCut"); m_sSignalName[_SigInx::_IsMkPosMv] = _T("_IsMkPosMv"); m_sSignalName[_SigInx::_IsMkVerify] = _T("_IsMkVerify"); 
	m_sSignalName[_SigInx::_IsReelmapUp] = _T("_IsReelmapUp"); m_sSignalName[_SigInx::_IsReelmapDn] = _T("_IsReelmapDn"); m_sSignalName[_SigInx::_IsReelmapTot] = _T("_IsReelmapTot"); 
	m_sSignalName[_SigInx::_IsRemainMode] = _T("_IsRemainMode"); m_sSignalName[_SigInx::_IsEndWork] = _T("_IsEndWork"); m_sSignalName[_SigInx::_IsReMk] = _T("_IsReMk"); m_sSignalName[_SigInx::_Is2Layer] = _T("_Is2Layer"); 
	m_sSignalName[_SigInx::_Is1LnMk] = _T("_Is1LnMk"); m_sSignalName[_SigInx::_Is2LnMk] = _T("_Is2LnMk"); m_sSignalName[_SigInx::_Is3LnMk] = _T("_Is3LnMk"); m_sSignalName[_SigInx::_Is4LnMk] = _T("_Is4LnMk"); 
	m_sSignalName[_SigInx::_IsTotLnMk] = _T("_IsTotLnMk"); m_sSignalName[_SigInx::_IsUpdateWork] = _T("_IsUpdateWork"); m_sSignalName[_SigInx::_IsDispDefImg] = _T("_IsDispDefImg");
	m_sSignalName[_SigInx::_Is2DEngLenMvCw] = _T("_Is2DEngLenMvCw"); m_sSignalName[_SigInx::_Is2DEngLenMvCcw] = _T("_Is2DEngLenMvCcw"); m_sSignalName[_SigInx::_Is2DAoiLenMvCw] = _T("_Is2DAoiLenMvCw");
	m_sSignalName[_SigInx::_Is2DAoiLenMvCcw] = _T("_Is2DAoiLenMvCcw"); m_sSignalName[_SigInx::_Is2DMkLenMvCw] = _T("_Is2DMkLenMvCw"); m_sSignalName[_SigInx::_Is2DMkLenMvCcw] = _T("_Is2DMkLenMvCcw");
	m_sSignalName[_SigInx::_Is2DOneShotLenCw] = _T("_Is2DOneShotLenCw"); m_sSignalName[_SigInx::_Is2DOneShotLenCcw] = _T("_Is2DOneShotLenCcw"); m_sSignalName[_SigInx::_Is2DOffsetInitPos] = _T("_Is2DOffsetInitPos");
	m_sSignalName[_SigInx::_Is2DOffsetInitPosMove] = _T("_Is2DOffsetInitPosMove"); m_sSignalName[_SigInx::_IsReady] = _T("_IsReady"); m_sSignalName[_SigInx::_IsRun] = _T("_IsRun");
	m_sSignalName[_SigInx::_IsReset] = _T("_IsReset"); m_sSignalName[_SigInx::_IsStop] = _T("_IsStop"); m_sSignalName[_SigInx::_IsAuto] = _T("_IsAuto"); m_sSignalName[_SigInx::_IsOneCycle] = _T("_IsOneCycle"); 
	m_sSignalName[_SigInx::_IsManual] = _T("_IsManual"); m_sSignalName[_SigInx::_IsMkTq] = _T("_IsMkTq"); m_sSignalName[_SigInx::_IsAoiTq] = _T("_IsAoiTq"); m_sSignalName[_SigInx::_IsEngTq] = _T("_IsEngTq"); 
	m_sSignalName[_SigInx::_IsCcwModRe] = _T("_IsCcwModRe"); m_sSignalName[_SigInx::_IsCcwModUn] = _T("_IsCcwModUn"); m_sSignalName[_SigInx::_IsCore150Re] = _T("_IsCore150Re");
	m_sSignalName[_SigInx::_IsCore150Un] = _T("_IsCore150Un"); m_sSignalName[_SigInx::_IsRelation] = _T("_IsRelation"); m_sSignalName[_SigInx::_IsJoinSelRe] = _T("_IsJoinSelRe"); 
	m_sSignalName[_SigInx::_IsMvCwRe] = _T("_IsMvCwRe"); m_sSignalName[_SigInx::_IsMvCcwRe] = _T("_IsMvCcwRe"); m_sSignalName[_SigInx::_IsPrdChuckRe] = _T("_IsPrdChuckRe"); m_sSignalName[_SigInx::_IsDancerUpRe] = _T("_IsDancerUpRe");
	m_sSignalName[_SigInx::_IsPasteUpLfRe] = _T("_IsPasteUpLfRe"); m_sSignalName[_SigInx::_IsPasteUpRtRe] = _T("_IsPasteUpRtRe"); m_sSignalName[_SigInx::_IsPasteVacRe] = _T("_IsPasteVacRe");
	m_sSignalName[_SigInx::_IsPprChuckRe] = _T("_IsPprChuckRe"); m_sSignalName[_SigInx::_IsPprCwRe] = _T("_IsPprCwRe"); m_sSignalName[_SigInx::_IsPprCcwRe] = _T("_IsPprCcwRe");
	m_sSignalName[_SigInx::_IsDoRe] = _T("_IsDoRe"); m_sSignalName[_SigInx::_IsPrdPprRe] = _T("_IsPrdPprRe"); m_sSignalName[_SigInx::_IsJoinSelMk] = _T("_IsJoinSelMk"); m_sSignalName[_SigInx::_IsMvCwMk] = _T("_IsMvCwMk"); 
	m_sSignalName[_SigInx::_IsMvCcwMk] = _T("_IsMvCcwMk"); m_sSignalName[_SigInx::_IsFdVacMk] = _T("_IsFdVacMk"); m_sSignalName[_SigInx::_IsPushUpMk] = _T("_IsPushUpMk"); m_sSignalName[_SigInx::_IsTblBlwMk] = _T("_IsTblBlwMk"); 
	m_sSignalName[_SigInx::_IsTblVacMk] = _T("_IsTblVacMk"); m_sSignalName[_SigInx::_IsFdClampMk] = _T("_IsFdClampMk"); m_sSignalName[_SigInx::_IsTensClampMk] = _T("_IsTensClampMk"); 
	m_sSignalName[_SigInx::_IsOnePnlMk] = _T("_IsOnePnlMk"); m_sSignalName[_SigInx::_IsLsrMk] = _T("_IsLsrMk"); m_sSignalName[_SigInx::_IsDancerUpMk] = _T("_IsDancerUpMk"); m_sSignalName[_SigInx::_IsTqVacMk] = _T("_IsTqVacMk"); 
	m_sSignalName[_SigInx::_IsJoinSelAoiDn] = _T("_IsJoinSelAoiDn"); m_sSignalName[_SigInx::_IsMvCwAoiDn] = _T("_IsMvCwAoiDn"); m_sSignalName[_SigInx::_IsMvCcwAoiDn] = _T("_IsMvCcwAoiDn");
	m_sSignalName[_SigInx::_IsFdVacAoiDn] = _T("_IsFdVacAoiDn"); m_sSignalName[_SigInx::_IsPushUpAoiDn] = _T("_IsPushUpAoiDn"); m_sSignalName[_SigInx::_IsTblBlwAoiDn] = _T("_IsTblBlwAoiDn"); 
	m_sSignalName[_SigInx::_IsTblVacAoiDn] = _T("_IsTblVacAoiDn"); m_sSignalName[_SigInx::_IsFdClampAoiDn] = _T("_IsFdClampAoiDn"); m_sSignalName[_SigInx::_IsTensClampAoiDn] = _T("_IsTensClampAoiDn");
	m_sSignalName[_SigInx::_IsOnePnlAoiDn] = _T("_IsOnePnlAoiDn"); m_sSignalName[_SigInx::_IsLsrAoiDn] = _T("_IsLsrAoiDn"); m_sSignalName[_SigInx::_IsClrRollAoiDn] = _T("_IsClrRollAoiDn");
	m_sSignalName[_SigInx::_IsVelClrSonicAoiDn] = _T("_IsVelClrSonicAoiDn"); m_sSignalName[_SigInx::_IsTestAoiDn] = _T("_IsTestAoiDn"); m_sSignalName[_SigInx::_IsResetAoiDn] = _T("_IsResetAoiDn");
	m_sSignalName[_SigInx::_IsLotEndAoiDn] = _T("_IsLotEndAoiDn"); m_sSignalName[_SigInx::_IsJoinSelAoiUp] = _T("_IsJoinSelAoiUp"); m_sSignalName[_SigInx::_IsMvCwAoiUp] = _T("_IsMvCwAoiUp"); 
	m_sSignalName[_SigInx::_IsMvCcwAoiUp] = _T("_IsMvCcwAoiUp"); m_sSignalName[_SigInx::_IsFdVacAoiUp] = _T("_IsFdVacAoiUp"); m_sSignalName[_SigInx::_IsPushUpAoiUp] = _T("_IsPushUpAoiUp");
	m_sSignalName[_SigInx::_IsTblBlwAoiUp] = _T("_IsTblBlwAoiUp"); m_sSignalName[_SigInx::_IsTblVacAoiUp] = _T("_IsTblVacAoiUp"); m_sSignalName[_SigInx::_IsFdClampAoiUp] = _T("_IsFdClampAoiUp"); 
	m_sSignalName[_SigInx::_IsTensClampAoiUp] = _T("_IsTensClampAoiUp"); m_sSignalName[_SigInx::_IsOnePnlAoiUp] = _T("_IsOnePnlAoiUp"); m_sSignalName[_SigInx::_IsLsrAoiUp] = _T("_IsLsrAoiUp");
	m_sSignalName[_SigInx::_IsClrRollAoiUp] = _T("_IsClrRollAoiUp"); m_sSignalName[_SigInx::_IsTestAoiUp] = _T("_IsTestAoiUp"); m_sSignalName[_SigInx::_IsResetAoiUp] = _T("_IsResetAoiUp"); 
	m_sSignalName[_SigInx::_IsLotEndAoiUp] = _T("_IsLotEndAoiUp"); m_sSignalName[_SigInx::_IsJoinSelEng] = _T("_IsJoinSelEng"); m_sSignalName[_SigInx::_IsMvCwEng] = _T("_IsMvCwEng");
	m_sSignalName[_SigInx::_IsMvCcwEng] = _T("_IsMvCcwEng"); m_sSignalName[_SigInx::_IsFdVacEng] = _T("_IsFdVacEng"); m_sSignalName[_SigInx::_IsPushUpEng] = _T("_IsPushUpEng"); 
	m_sSignalName[_SigInx::_IsTblBlwEng] = _T("_IsTblBlwEng"); m_sSignalName[_SigInx::_IsTblVacEng] = _T("_IsTblVacEng"); m_sSignalName[_SigInx::_IsFdClampEng] = _T("_IsFdClampEng");
	m_sSignalName[_SigInx::_IsTensClampEng] = _T("_IsTensClampEng"); m_sSignalName[_SigInx::_IsOnePnlEng] = _T("_IsOnePnlEng"); m_sSignalName[_SigInx::_IsLsrEng] = _T("_IsLsrEng");
	m_sSignalName[_SigInx::_IsDancerUpEng] = _T("_IsDancerUpEng"); m_sSignalName[_SigInx::_IsVelClrSonicEng] = _T("_IsVelClrSonicEng"); m_sSignalName[_SigInx::_IsJoinSelUn] = _T("_IsJoinSelUn");
	m_sSignalName[_SigInx::_IsMvCwUn] = _T("_IsMvCwUn"); m_sSignalName[_SigInx::_IsMvCcwUn] = _T("_IsMvCcwUn"); m_sSignalName[_SigInx::_IsPrdChuckUn] = _T("_IsPrdChuckUn"); 
	m_sSignalName[_SigInx::_IsDancerUpUn] = _T("_IsDancerUpUn"); m_sSignalName[_SigInx::_IsPasteUpLfUn] = _T("_IsPasteUpLfUn"); m_sSignalName[_SigInx::_IsPasteUpRtUn] = _T("_IsPasteUpRtUn");
	m_sSignalName[_SigInx::_IsPasteVacUn] = _T("_IsPasteVacUn"); m_sSignalName[_SigInx::_IsPprChuckUn] = _T("_IsPprChuckUn"); m_sSignalName[_SigInx::_IsPprCwUn] = _T("_IsPprCwUn"); 
	m_sSignalName[_SigInx::_IsPprCcwUn] = _T("_IsPprCcwUn"); m_sSignalName[_SigInx::_IsClrRollUpUn] = _T("_IsClrRollUpUn"); m_sSignalName[_SigInx::_IsClrRollPushUn] = _T("_IsClrRollPushUn");
	m_sSignalName[_SigInx::_IsEmgAoi] = _T("_IsEmgAoi"); m_sSignalName[_SigInx::_IsMkResetLf] = _T("_IsMkResetLf"); m_sSignalName[_SigInx::_IsMkResetRt] = _T("_IsMkResetRt");
	m_sSignalName[_SigInx::_IsMkBuffJogCw] = _T("_IsMkBuffJogCw"); m_sSignalName[_SigInx::_IsMkBuffJogCcw] = _T("_IsMkBuffJogCcw"); m_sSignalName[_SigInx::_IsMkBuffOrgMv] = _T("_IsMkBuffOrgMv");
	m_sSignalName[_SigInx::_IsMkBuffInitPosMv] = _T("_IsMkBuffInitPosMv"); m_sSignalName[_SigInx::_IsMkBuffPosSave] = _T("_IsMkBuffPosSave"); m_sSignalName[_SigInx::_IsEngBuffJogCw] = _T("_IsEngBuffJogCw"); 
	m_sSignalName[_SigInx::_IsEngBuffJogCcw] = _T("_IsEngBuffJogCcw"); m_sSignalName[_SigInx::_IsEngBuffOrgMv] = _T("_IsEngBuffOrgMv"); m_sSignalName[_SigInx::_IsEngBuffOrgMvDone] = _T("_IsEngBuffOrgMvDone");
	m_sSignalName[_SigInx::_IsEngBuffInitPosMv] = _T("_IsEngBuffInitPosMv"); m_sSignalName[_SigInx::_IsEngBuffInitPosMvDone] = _T("_IsEngBuffInitPosMvDone"); m_sSignalName[_SigInx::_IsEngBuffInitPosSave] = _T("_IsEngBuffInitPosSave");
	m_sSignalName[_SigInx::_IsEngAutoInit] = _T("_IsEngAutoInit"); m_sSignalName[_SigInx::_IsEngAutoSeqMkSt] = _T("_IsEngAutoSeqMkSt"); m_sSignalName[_SigInx::_IsEngAutoSeqOnMkIng] = _T("_IsEngAutoSeqOnMkIng");
	m_sSignalName[_SigInx::_IsEngAutoSeqMkDone] = _T("_IsEngAutoSeqMkDone"); m_sSignalName[_SigInx::_IsEngAutoSeq2dReadSt] = _T("_IsEngAutoSeq2dReadSt"); m_sSignalName[_SigInx::_IsEngAutoSeqOnReading2d] = _T("_IsEngAutoSeqOnReading2d"); 
	m_sSignalName[_SigInx::_IsEngAutoSeq2dReadDone] = _T("_IsEngAutoSeq2dReadDone"); m_sSignalName[_SigInx::_IsEngAutoSeqFdDone] = _T("_IsEngAutoSeqFdDone"); 
	m_sSignalName[_SigInx::_IsMyMsgYes] = _T("_IsMyMsgYes"); m_sSignalName[_SigInx::_IsMyMsgNo] = _T("_IsMyMsgNo"); m_sSignalName[_SigInx::_IsMyMsgCancel] = _T("_IsMyMsgCancel"); m_sSignalName[_SigInx::_IsMyMsgOk] = _T("_IsMyMsgOk"); 
	m_sSignalName[_SigInx::_IsMyMsg] = _T("_IsMyMsg"); m_sSignalName[_SigInx::_IsBuzzer] = _T("_IsBuzzer"); m_sSignalName[_SigInx::_IsTowerLamp] = _T("_IsTowerLamp");
	m_sSignalName[_SigInx::_IsErrorRead2dCode] = _T("_IsErrorRead2dCode"); m_sSignalName[_SigInx::_IsGetCurrentInfoSignal] = _T("_IsGetCurrentInfoSignal"); m_sSignalName[_SigInx::_IsGetMonDispMainSignal] = _T("_IsGetMonDispMainSignal"); 
}

void CEngrave::SetJobEndEngrave()
{
	SOCKET_DATA SocketData;
	SocketData.nCmdCode = _SetSig;

	SocketData.nMsgID = _SigInx::_JobEnd;
	SocketData.nData1 = 1;
	SendCommand(SocketData);
}