
// GvisR2R_PunchDoc.cpp : CGvisR2R_PunchDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GvisR2R_Punch.h"
#endif

#include "GvisR2R_PunchDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Process/DataFile.h"
//#include "Device/Vicdefs.h"
#include "Device/Vic7defs.h"
#include "Global/GlobalFunc.h"


#include "MainFrm.h"
#include "GvisR2R_PunchView.h"

extern CMainFrame* pFrm;
CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

#include "safelockdoc.h"
CCriticalSection g_LogLockAuto;
CCriticalSection g_LogLockPLC;


// CGvisR2R_PunchDoc

IMPLEMENT_DYNCREATE(CGvisR2R_PunchDoc, CDocument)

BEGIN_MESSAGE_MAP(CGvisR2R_PunchDoc, CDocument)
END_MESSAGE_MAP()


// CGvisR2R_PunchDoc 생성/소멸

CGvisR2R_PunchDoc::CGvisR2R_PunchDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	int i, k;
	pDoc = this;

	//m_dVerifyPunchScore = 80.0;

	m_sEngAlarm = _T("");

	m_bVsStatusUp = FALSE;
	m_bVsStatusDn = FALSE;

	m_bOffLogAuto = FALSE;
	m_bOffLogPLC = FALSE;
	m_strUserNameList = _T("");

	m_bBufEmpty[0] = FALSE; // Exist
	m_bBufEmpty[1] = FALSE; // Exist
	m_bBufEmptyF[0] = FALSE; // Exist
	m_bBufEmptyF[1] = FALSE; // Exist


	// 	m_pCellRgn = NULL;
	// 	m_pPcsRgn = NULL;
	m_pReelMapDisp = NULL;
	m_pReelMap = NULL;
	m_pReelMapUp = NULL;
	m_pReelMapDn = NULL;
	m_pReelMapAllUp = NULL;
	m_pReelMapAllDn = NULL;

	m_pReelMapInner = NULL;
	m_pReelMapInnerUp = NULL;
	m_pReelMapInnerDn = NULL;
	m_pReelMapInnerAllUp = NULL;
	m_pReelMapInnerAllDn = NULL;
	//m_pReelMapInOuterUp = NULL;
	//m_pReelMapInOuterDn = NULL;
	m_pReelMapIts = NULL;

	for (i = 0; i < MAX_PCR; i++)
	{
		for (k = 0; k < MAX_PCR_PNL; k++)
		{
			m_pPcr[i][k] = NULL;
			m_pPcrInner[i][k] = NULL;
		}
	}
	for (k = 0; k < MAX_PCR_PNL; k++)
		m_pPcrIts[k] = NULL;

	//for (k = 0; k < MAX_PCR_PNL; k++)
	//{
	//	m_pPcrMk[k] = NULL;
	//	m_pPcrMkInner[k] = NULL;
	//}

	pMkInfo = NULL;

	AoiDummyShot[0] = 3;
	AoiDummyShot[1] = 3;

	// 	for(i = 0; i < MAX_CELL_NUM; i++)
	// 	{
	// 		m_pCADCellImg[i] = NULL;
	// 		m_CADFileSize[i] = 0;
	// 	}

	// 	m_pPinImg = NULL;
	// 	m_PinFileSize = 0;
	// 	m_pAlignImg[0] = NULL;
	// 	m_pAlignImg[1] = NULL;
	// 	m_AlignFileSize[0] = 0;
	// 	m_AlignFileSize[1] = 0;
	// 
	// 	m_pPcsImg = NULL;
	// 	m_PcsFileSize = 0;

	m_nSliceIo = 0;

	m_nMpeIb = 0;
	m_pMpeIb = NULL;
	m_pMpeIF = NULL;

	m_nMpeIo = 0;
	m_pMpeIo = NULL;
	m_pMpeIoF = NULL;

	m_nMpeSignal = 0;
	m_pMpeSignal = NULL;

	m_nMpeData = 0;
	m_pMpeData = NULL;

	// 	m_pPrevMpeIo = NULL;

	// 	m_pSliceIo = NULL;
	// 	m_pSliceIoF = NULL;
	// 	m_pPrevSliceIo = NULL;
	// 	
	// 	m_pSliceMpeIo = NULL;
	// 	m_pSliceMpeIoF = NULL;
	// 	m_pPrevSliceMpeIo = NULL;

	m_pSpecLocal = NULL;

	m_pFile = new CMyFile();
	InitPcr();

	m_sPassword = _T("");
	m_sAlmMsg = _T("");
	m_sIsAlmMsg = _T("");
	m_sPrevAlmMsg = _T("");

	m_sMsgBox = _T("");
	m_sIsMsgBox = _T("");
	m_sPrevMsgBox = _T("");

	m_nPrevSerial = 0;
	m_bNewLotShare[0] = FALSE;
	m_bNewLotShare[1] = FALSE;
	m_bNewLotBuf[0] = FALSE;
	m_bNewLotBuf[1] = FALSE;
	m_bDoneChgLot = FALSE;

	m_cBigDefCode[0] = '*';		//	None
	m_cBigDefCode[1] = 'N';		//	NICK
	m_cBigDefCode[2] = 'D';		//	PROTRUSION
	m_cBigDefCode[3] = 'A';		//	SPACE
	m_cBigDefCode[4] = 'O';		//	OPEN
	m_cBigDefCode[5] = 'S';		//	SHORT
	m_cBigDefCode[6] = 'U';		//	USHORT
	m_cBigDefCode[7] = 'I';		//	PINHOLE
	m_cBigDefCode[8] = 'H';		//	HOLE_MISS
	m_cBigDefCode[9] = 'E';		//	EXTRA
	m_cBigDefCode[10] = 'P';	//	PAD
	m_cBigDefCode[11] = 'L';	//	HOLE_POSITION
	m_cBigDefCode[12] = 'X';	//	POI
	m_cBigDefCode[13] = 'T';	//	VH_POSITION
	m_cBigDefCode[14] = 'M';	//	VH_MISS
	m_cBigDefCode[15] = 'F';	//	HOLE_DEFECT
	m_cBigDefCode[16] = 'C';	//	HOLE_OPEN
	m_cBigDefCode[17] = 'G';	//	VH_OPEN
	m_cBigDefCode[18] = 'V';	//	VH_DEF
	m_cBigDefCode[19] = 'K';	//	E.Nick
	m_cBigDefCode[20] = 'B';	//	E.Prot
	m_cBigDefCode[21] = 'J';	//	E.Space
	m_cBigDefCode[22] = 'Q';	//	UDD1
	m_cBigDefCode[23] = 'R';	//	Narrow
	m_cBigDefCode[24] = 'W';	//	Wide
	m_cBigDefCode[25] = '?';	//	Light

	m_cSmallDefCode[0] = '*';	//	None
	m_cSmallDefCode[1] = 'n';	//	NICK
	m_cSmallDefCode[2] = 'd';	//	PROTRUSION
	m_cSmallDefCode[3] = 'a';	//	SPACE
	m_cSmallDefCode[4] = 'o';
	m_cSmallDefCode[5] = 's';
	m_cSmallDefCode[6] = 'u';
	m_cSmallDefCode[7] = 'i';
	m_cSmallDefCode[8] = 'h';
	m_cSmallDefCode[9] = 'e';
	m_cSmallDefCode[10] = 'p';
	m_cSmallDefCode[11] = 'l';
	m_cSmallDefCode[12] = 'x';
	m_cSmallDefCode[13] = 't';
	m_cSmallDefCode[14] = 'm';
	m_cSmallDefCode[15] = 'f';
	m_cSmallDefCode[16] = 'c';
	m_cSmallDefCode[17] = 'g';
	m_cSmallDefCode[18] = 'v';
	m_cSmallDefCode[19] = 'k';	//	E.Nick
	m_cSmallDefCode[20] = 'b';	//	E.Prot
	m_cSmallDefCode[21] = 'j';	//	E.Space
	m_cSmallDefCode[22] = 'q';	//	UDD1
	m_cSmallDefCode[23] = 'r';	//	Narrow
	m_cSmallDefCode[24] = 'w';	//	Wide
	m_cSmallDefCode[25] = '?';	//	Light

// 	for(i=19; i<MAX_DEF; i++)
// 	{
// 		m_cBigDefCode[i] = '?';	//	VH_DEF
// 		m_cSmallDefCode[i] = '?';
// 	}

	m_bUseDts = FALSE;
	m_bUseIts = FALSE;
	// 20160926-syd
	m_bUseRTRYShiftAdjust = FALSE;
	m_dRTRShiftVal = 0.0;
	m_dShiftAdjustRatio = 0.0;

	m_nOrderNum = 0;
	m_nShotNum = 0;
	m_sOrderNum = _T("");
	m_sShotNum = _T("");
	m_nTestOrderNum = 0;
	m_nTestShotNum = 0;
	m_sTestOrderNum = _T("");
	m_sTestShotNum = _T("");

	// Communcation
	m_nBad[0] = 0; m_nBad[1] = 0; m_nBad[2] = 0;
	m_nGood[0] = 0; m_nGood[1] = 0; m_nGood[2] = 0;
	m_dBadRatio[0] = 0.0; m_dBadRatio[1] = 0.0; m_dBadRatio[2] = 0.0;
	m_dGoodRatio[0] = 0.0; m_dGoodRatio[1] = 0.0; m_dGoodRatio[2] = 0.0;
	m_nTestNum[0] = 0; m_nTestNum[1] = 0; m_nTestNum[2] = 0;
	m_sLotStTime = _T(""); m_sLotEdTime = _T(""); m_sLotRunTime = _T("");

	for (i = 0; i < 3; i++)
	{
		for (k = 0; k < 5; k++)
		{
			m_dStripRatio[i][k] = 0.0;				// [3]: Up/Dn/ALL , [5]: Strip 0~3, ALL
			m_dTotRatio[i][k];						// [3]: Up/Dn/ALL , [5]: Strip 0~3, ALL
		}
	}

	for (i = 0; i < MAX_DEF; i++)
		m_nDef[i] = 0;

	m_dMkBuffCurrPos = 0.0;

	// for ITS
	m_bEngDualTest = TRUE;
	m_sItsCode = _T("");
	m_sEngLotNum = _T("");
	m_sEngProcessNum = _T("");
	m_sEngModel = _T("");
	m_sEngLayerUp = _T("");
	m_sEngLayerDn = _T("");
	m_nWritedItsSerial = 0;

	m_nEjectBufferLastShot = -1;
	m_bDebugGrabAlign = FALSE;

	SetCurrentInfoSignal(_SigInx::_MyMsgYes, FALSE);
	SetCurrentInfoSignal(_SigInx::_MyMsgNo, FALSE);
	SetCurrentInfoSignal(_SigInx::_MyMsgOk, FALSE);
}


void CGvisR2R_PunchDoc::DestroyDoc()
{

	if (m_pFile)
	{
		delete m_pFile;
		m_pFile = NULL;
	}

	if (pMkInfo)
	{
		delete[] pMkInfo;
		pMkInfo = NULL;
	}
}

CGvisR2R_PunchDoc::~CGvisR2R_PunchDoc()
{
	int k, i;

	SetCurrentInfoSignal(_SigInx::_MyMsgYes, FALSE);
	SetCurrentInfoSignal(_SigInx::_MyMsgNo, FALSE);
	SetCurrentInfoSignal(_SigInx::_MyMsgOk, FALSE);

	SaveWorkingInfo();

	// 	PinImgFree();
	// 	AlignImgFree();
	// 	PcsImgFree();
	// 	CADLinkImgFree();

	//DestroyDoc();

	//if (m_pReelMap)
	//{
	//	delete m_pReelMap;
	//	m_pReelMap = NULL;
	//}

	if (m_pReelMapUp)
	{
		delete m_pReelMapUp;
		m_pReelMapUp = NULL;
	}

	if (m_pReelMapDn)
	{
		delete m_pReelMapDn;
		m_pReelMapDn = NULL;
	}

	if (m_pReelMapAllUp)
	{
		delete m_pReelMapAllUp;
		m_pReelMapAllUp = NULL;
	}

	if (m_pReelMapAllDn)
	{
		delete m_pReelMapAllDn;
		m_pReelMapAllDn = NULL;
	}

	if (m_pReelMapIts)
	{
		delete m_pReelMapIts;
		m_pReelMapIts = NULL;
	}

	if (m_pReelMapInnerUp)
	{
		delete m_pReelMapInnerUp;
		m_pReelMapInnerUp = NULL;
	}

	if (m_pReelMapInnerDn)
	{
		delete m_pReelMapInnerDn;
		m_pReelMapInnerDn = NULL;
	}

	if (m_pReelMapInnerAllUp)
	{
		delete m_pReelMapInnerAllUp;
		m_pReelMapInnerAllUp = NULL;
	}

	if (m_pReelMapInnerAllDn)
	{
		delete m_pReelMapInnerAllDn;
		m_pReelMapInnerAllDn = NULL;
	}

	//if (m_pReelMapInOuterUp)
	//{
	//	delete m_pReelMapInOuterUp;
	//	m_pReelMapInOuterUp = NULL;
	//}

	//if (m_pReelMapInOuterDn)
	//{
	//	delete m_pReelMapInOuterDn;
	//	m_pReelMapInOuterDn = NULL;
	//}

	if (m_pMpeIb)
	{
		delete[] m_pMpeIb;
		m_pMpeIb = NULL;
	}

	if (m_pMpeIF)
	{
		delete[] m_pMpeIF;
		m_pMpeIF = NULL;
	}

	if (m_pMpeIo)
	{
		delete[] m_pMpeIo;
		m_pMpeIo = NULL;
	}

	if (m_pMpeIoF)
	{
		delete[] m_pMpeIoF;
		m_pMpeIoF = NULL;
	}

	if (m_pMpeSignal)
	{
		delete[] m_pMpeSignal;
		m_pMpeSignal = NULL;
	}

	if (m_pMpeData)
	{
		for (k = 0; k < m_nMpeData; k++)
		{
			if (m_pMpeData[k])
			{
				delete[] m_pMpeData[k];
				m_pMpeData[k] = NULL;
			}
		}
		delete[] m_pMpeData;
		m_pMpeData = NULL;
	}

// 	if(m_pPrevMpeIo)
// 	{
// 		delete[] m_pPrevMpeIo;
// 		m_pPrevMpeIo = NULL;
// 	}

	if (m_pSpecLocal)
	{
		delete m_pSpecLocal;
		m_pSpecLocal = NULL;
	}

	//Sleep(3000);

	for (i = 0; i < MAX_PCR; i++)
	{
		for (k = 0; k < MAX_PCR_PNL; k++)
		{
			if (m_pPcr[i][k])
			{
				delete m_pPcr[i][k];
				m_pPcr[i][k] = NULL;
			}

			if (m_pPcrInner[i][k])
			{
				delete m_pPcrInner[i][k];
				m_pPcrInner[i][k] = NULL;
			}
		}
		//delete[] m_pPcr[i];
		//m_pPcr[i] = NULL;
	}
	for (k = 0; k < MAX_PCR_PNL; k++)
	{
		if (m_pPcrIts[k])
		{
			delete m_pPcrIts[k];
			m_pPcrIts[k] = NULL;
		}
	}

	//for (k = 0; k < MAX_PCR_PNL; k++)
	//{
	//	if (m_pPcrMk[k])
	//	{
	//		delete m_pPcrMk[k];
	//		m_pPcrMk[k] = NULL;
	//	}
	//	if (m_pPcrMkInner[k])
	//	{
	//		delete m_pPcrMkInner[k];
	//		m_pPcrMkInner[k] = NULL;
	//	}
	//}


}

BOOL CGvisR2R_PunchDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CGvisR2R_PunchDoc serialization

void CGvisR2R_PunchDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CGvisR2R_PunchDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CGvisR2R_PunchDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGvisR2R_PunchDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGvisR2R_PunchDoc 진단

#ifdef _DEBUG
void CGvisR2R_PunchDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGvisR2R_PunchDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGvisR2R_PunchDoc 명령

void CGvisR2R_PunchDoc::UpdateData()
{
	if (!m_pReelMap)
		return;

	m_pReelMap->m_sMc = WorkingInfo.System.sMcName;
	m_pReelMap->m_sUser = WorkingInfo.LastJob.sSelUserName;

	m_pReelMap->m_bUseLotSep = WorkingInfo.LastJob.bLotSep;
	m_pReelMap->m_bUseTempPause = WorkingInfo.LastJob.bTempPause;

	m_pReelMap->m_dTotLen = _tstof(WorkingInfo.LastJob.sReelTotLen)*1000.0;
	m_pReelMap->m_dPnlLen = _tstof(WorkingInfo.LastJob.sOnePnlLen);
	m_pReelMap->m_dLotCutPosLen = _tstof(WorkingInfo.LastJob.sLotCutPosLen);
	if (m_pReelMap->m_bUseLotSep)
		m_pReelMap->m_dLotLen = _tstof(WorkingInfo.LastJob.sLotSepLen);
	else
		m_pReelMap->m_dLotLen = m_pReelMap->m_dTotLen;
	m_pReelMap->m_dTempPauseLen = _tstof(WorkingInfo.LastJob.sTempPauseLen);

}

BOOL CGvisR2R_PunchDoc::LoadMySpec() // 상면 기준으로 마킹부 상면작업 로컬데이타만.
{
	if (m_pSpecLocal)
	{
		delete m_pSpecLocal;
		m_pSpecLocal = NULL;
	}
	if (!WorkingInfo.LastJob.sModel.IsEmpty() && !WorkingInfo.LastJob.sLayerUp.IsEmpty())
	{
		if (!m_pSpecLocal)
		{
			m_pSpecLocal = new CMySpec(WorkingInfo.LastJob.sModel, WorkingInfo.LastJob.sLayerUp);
			return FALSE;
		}
		else
		{
			if (IsPinData())
				m_pSpecLocal->Load();
		}
	}

	return TRUE;
}


BOOL CGvisR2R_PunchDoc::LoadIoInfo()
{
	int nSeg, k;
	CString strPath;

	TCHAR sep[] = { _T(",;\r\n\t")};
	TCHAR szData[200];
	TCHAR *token1, *token2, *token3, *token4;// , *token5;

	CString strIdx, strInSeg, strOutSeg;
	CString strIReg, strAddr, strMReg, strSymbol, strName, strMBoth;

	// MpeIO
	strPath.Format(_T("%s"), PATH_MP2100_IO);

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("MAX_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nMaxSeg = (long)_tstoi(szData);
	else
		MkIo.MpeIo.nMaxSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("IN_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nInSeg = (long)_tstoi(szData);
	else
		MkIo.MpeIo.nInSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("OUT_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nOutSeg = (long)_tstoi(szData);
	else
		MkIo.MpeIo.nOutSeg = (long)0L;


	if (MkIo.MpeIo.nInSeg > MkIo.MpeIo.nOutSeg)
		nSeg = MkIo.MpeIo.nInSeg;
	else
		nSeg = MkIo.MpeIo.nOutSeg;

	MkIo.MpeIo.pAddrIn = new CString*[MkIo.MpeIo.nInSeg];
	for (k = 0; k < MkIo.MpeIo.nInSeg; k++)
		MkIo.MpeIo.pAddrIn[k] = new CString[16];

	MkIo.MpeIo.pAddrOut = new CString*[MkIo.MpeIo.nOutSeg];
	for (k = 0; k < MkIo.MpeIo.nOutSeg; k++)
		MkIo.MpeIo.pAddrOut[k] = new CString[16];

	MkIo.MpeIo.pSymIn = new CString*[MkIo.MpeIo.nInSeg];
	for (k = 0; k < MkIo.MpeIo.nInSeg; k++)
		MkIo.MpeIo.pSymIn[k] = new CString[16];

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("GROUP_STEP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nGrpStep = (int)_tstoi(szData);
	else
		MkIo.MpeIo.nGrpStep = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nGrpIn = (int)_tstoi(szData);
	else
		MkIo.MpeIo.nGrpIn = 0;
	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nGrpInSt = (int)_tstoi(szData);
	else
		MkIo.MpeIo.nGrpInSt = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nGrpOut = (int)_tstoi(szData);
	else
		MkIo.MpeIo.nGrpOut = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeIo.nGrpOutSt = (int)_tstoi(szData);
	else
		MkIo.MpeIo.nGrpOutSt = 0;


	for (k = 0; k < nSeg; k++)
	{
		strInSeg.Format(_T("%d"), k);
		strOutSeg.Format(_T("%d"), MkIo.MpeIo.nInSeg + k);

		for (int nR = 0; nR < 16; nR++)
		{
			if (k < MkIo.MpeIo.nInSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strInSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);
					token3 = _tcstok(NULL, sep);

					strAddr = CString(token1);
					strSymbol = CString(token2);
					strName = CString(token3);
				}
				else
				{
					strAddr = _T("");
					strSymbol = _T("");
					strName = _T("");
				}
				MkIo.MpeIo.pAddrIn[k][nR] = strAddr;
				strIReg = strSymbol.Right(4);
				MkIo.MpeIo.pSymIn[k][nR] = _T("IB0") + strIReg;
			}

			if (k < MkIo.MpeIo.nOutSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strOutSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);
					token3 = _tcstok(NULL, sep);
					token4 = _tcstok(NULL, sep);
					//token5 = _tcstok(NULL,sep);

					strAddr = CString(token1);
					//strMReg = CString(token2);
					strSymbol = CString(token2);
					strName = CString(token3);
					strMBoth = CString(token4);
				}
				else
				{
					strAddr = _T("");
					//strMReg = _T("");
					strSymbol = _T("");
					strName = _T("");
					strMBoth = _T("");
				}
				MkIo.MpeIo.pAddrOut[k][nR] = strAddr;//strMReg;
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::LoadSignalInfo()
{
	int nSeg, k;
	CString strPath;

	TCHAR sep[] = { _T(",;\r\n\t") };
	TCHAR szData[200];
	TCHAR *token1, *token2, *token3;// , *token4;

	CString strIdx, strInSeg, strOutSeg;
	CString strAddr, strName, strMBoth;


	// MpeSignal
	strPath.Format(_T("%s"), PATH_MP2100_SIGNAL);

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("MAX_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nMaxSeg = (long)_tstoi(szData);
	else
		MkIo.MpeSignal.nMaxSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("IN_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nInSeg = (long)_tstoi(szData);
	else
		MkIo.MpeSignal.nInSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("OUT_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nOutSeg = (long)_tstoi(szData);
	else
		MkIo.MpeSignal.nOutSeg = (long)0L;


	if (MkIo.MpeSignal.nInSeg > MkIo.MpeSignal.nOutSeg)
		nSeg = MkIo.MpeSignal.nInSeg;
	else
		nSeg = MkIo.MpeSignal.nOutSeg;

	MkIo.MpeSignal.pAddrIn = new CString*[MkIo.MpeSignal.nInSeg];
	for (k = 0; k < MkIo.MpeSignal.nInSeg; k++)
		MkIo.MpeSignal.pAddrIn[k] = new CString[16];

	MkIo.MpeSignal.pAddrOut = new CString*[MkIo.MpeSignal.nOutSeg];
	for (k = 0; k < MkIo.MpeSignal.nOutSeg; k++)
		MkIo.MpeSignal.pAddrOut[k] = new CString[16];


	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("GROUP_STEP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nGrpStep = (int)_tstoi(szData);
	else
		MkIo.MpeSignal.nGrpStep = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nGrpIn = (int)_tstoi(szData);
	else
		MkIo.MpeSignal.nGrpIn = 0;
	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nGrpInSt = (int)_tstoi(szData);
	else
		MkIo.MpeSignal.nGrpInSt = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nGrpOut = (int)_tstoi(szData);
	else
		MkIo.MpeSignal.nGrpOut = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeSignal.nGrpOutSt = (int)_tstoi(szData);
	else
		MkIo.MpeSignal.nGrpOutSt = 0;


	for (k = 0; k < nSeg; k++)
	{
		strInSeg.Format(_T("%d"), k);
		strOutSeg.Format(_T("%d"), MkIo.MpeSignal.nInSeg + k);

		for (int nR = 0; nR < 16; nR++)
		{
			if (k < MkIo.MpeSignal.nInSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strInSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);

					strAddr = CString(token1);
					strName = CString(token2);
				}
				else
				{
					strAddr = _T("");
					strName = _T("");
				}
				MkIo.MpeSignal.pAddrIn[k][nR] = strAddr;
			}

			if (k < MkIo.MpeSignal.nOutSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strOutSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);
					token3 = _tcstok(NULL, sep);

					strAddr = CString(token1);
					strName = CString(token2);
					strMBoth = CString(token3);
				}
				else
				{
					strAddr = _T("");
					strName = _T("");
					strMBoth = _T("");
				}
				MkIo.MpeSignal.pAddrOut[k][nR] = strAddr;
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::LoadDataInfo()
{
	int nSeg, k;
	CString strPath;

	TCHAR sep[] = {_T(",;\r\n\t")};
	TCHAR szData[200];
	TCHAR *token1, *token2, *token3;// , *token4;

	CString strIdx, strInSeg, strOutSeg;
	CString strAddr, strName, strMBoth;


	// MpeSignal
	strPath.Format(_T("%s"), PATH_MP2100_DATA);

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("MAX_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nMaxSeg = (long)_tstoi(szData);
	else
		MkIo.MpeData.nMaxSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("IN_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nInSeg = (long)_tstoi(szData);
	else
		MkIo.MpeData.nInSeg = (long)0L;

	if (0 < ::GetPrivateProfileString(_T("TOTAL"), _T("OUT_SEGMENT"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nOutSeg = (long)_tstoi(szData);
	else
		MkIo.MpeData.nOutSeg = (long)0L;


	if (MkIo.MpeData.nInSeg > MkIo.MpeData.nOutSeg)
		nSeg = MkIo.MpeData.nInSeg;
	else
		nSeg = MkIo.MpeData.nOutSeg;

	MkIo.MpeData.pAddrIn = new CString*[MkIo.MpeData.nInSeg];
	for (k = 0; k < MkIo.MpeData.nInSeg; k++)
		MkIo.MpeData.pAddrIn[k] = new CString[16];

	MkIo.MpeData.pAddrOut = new CString*[MkIo.MpeData.nOutSeg];
	for (k = 0; k < MkIo.MpeData.nOutSeg; k++)
		MkIo.MpeData.pAddrOut[k] = new CString[16];


	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("GROUP_STEP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nGrpStep = (int)_tstoi(szData);
	else
		MkIo.MpeData.nGrpStep = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nGrpIn = (int)_tstoi(szData);
	else
		MkIo.MpeData.nGrpIn = 0;
	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("IN_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nGrpInSt = (int)_tstoi(szData);
	else
		MkIo.MpeData.nGrpInSt = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_GROUP"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nGrpOut = (int)_tstoi(szData);
	else
		MkIo.MpeData.nGrpOut = 0;

	if (0 < ::GetPrivateProfileString(_T("GROUP START SEG"), _T("OUT_START"), NULL, szData, sizeof(szData), strPath))
		MkIo.MpeData.nGrpOutSt = (int)_tstoi(szData);
	else
		MkIo.MpeData.nGrpOutSt = 0;


	for (k = 0; k < nSeg; k++)
	{
		strInSeg.Format(_T("%d"), k);
		strOutSeg.Format(_T("%d"), MkIo.MpeData.nInSeg + k);

		for (int nR = 0; nR < 16; nR++)
		{
			if (k < MkIo.MpeData.nInSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strInSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);

					strAddr = CString(token1);
					strName = CString(token2);
				}
				else
				{
					strAddr = _T("");
					strName = _T("");
				}
				MkIo.MpeData.pAddrIn[k][nR] = strAddr;
			}

			if (k < MkIo.MpeData.nOutSeg)
			{
				strIdx.Format(_T("%d"), nR);
				if (0 < ::GetPrivateProfileString(strOutSeg, strIdx, NULL, szData, sizeof(szData), strPath))
				{
					token1 = _tcstok(szData, sep);
					token2 = _tcstok(NULL, sep);
					token3 = _tcstok(NULL, sep);

					strAddr = CString(token1);
					strName = CString(token2);
					strMBoth = CString(token3);
				}
				else
				{
					strAddr = _T("");
					strName = _T("");
					strMBoth = _T("");
				}
				MkIo.MpeData.pAddrOut[k][nR] = strAddr;
			}
		}
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::LoadWorkingInfo()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;
	pView->ClrDispMsg();

	if (0 < ::GetPrivateProfileString(_T("DTS"), _T("UseDts"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_bUseDts = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bUseDts = FALSE;

	// [System]

	//if (0 < ::GetPrivateProfileString(_T("System"), _T("VerifyPunchScore"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
	//	m_dVerifyPunchScore = _ttof(szData);
	//else
	//	m_dVerifyPunchScore = 80.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("OffLogAuto"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_bOffLogAuto = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bOffLogAuto = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("OffLogPLC"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_bOffLogPLC = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bOffLogPLC = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("DebugGrabAlign"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_bDebugGrabAlign = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bDebugGrabAlign = FALSE;

	//if (0 < ::GetPrivateProfileString(_T("DTS"), _T("UseIts"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
	//	m_bUseIts = _ttoi(szData) ? TRUE : FALSE;
	//else
	//	m_bUseIts = FALSE;

	// 20160926 - syd
	if (0 < ::GetPrivateProfileString(_T("System"), _T("USE_RTR_SHIFT_ADJUST"), NULL, szData, sizeof(szData), sPath))
	{
		m_bUseRTRYShiftAdjust = (BOOL)(_ttoi(szData) ? TRUE : FALSE);
	}
	else
		m_bUseRTRYShiftAdjust = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("RTR_SHIFT_ADJUST_RATIO"), NULL, szData, sizeof(szData), sPath))
	{
		m_dShiftAdjustRatio = (double)_ttof(szData);
	}
	else
		m_dShiftAdjustRatio = 0.5;


	if (0 < ::GetPrivateProfileString(_T("System"), _T("Password"), NULL, szData, sizeof(szData), sPath))
		pDoc->m_sPassword = CString(szData);
	else
		pDoc->m_sPassword = _T("gvis");

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MachineName"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sMcName = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("설비이름이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sMcName = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("CamSpecDir"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathCamSpecDir = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("CamMaster의 Spec디렉토리 Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathCamSpecDir = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("TorqueSpecDir"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathTorqSpecDir = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("TorqueSpecDir의 Spec디렉토리 Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathTorqSpecDir = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("EngravePath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathEng = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("EngravePath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathEng = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("EngraveCurrentInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathEngCurrInfo = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("EngraveCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathEngCurrInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("EngraveSignalInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathEngSignalInfo = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("EngraveSignalInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathEngSignalInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("PunchingCurrentInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkCurrInfo = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("PunchingCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkCurrInfo = CString(_T("C:\\PunchWork\\CurrentInfo.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("PunchingSignalInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkSignalInfo = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("PunchingSignalInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkSignalInfo = CString(_T("C:\\PunchWork\\SignalInfo.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUpStatusInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpStatusInfo = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AoiUpStatusInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpStatusInfo = CString(_T("C:\\AOIWork\\Status.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDnStatusInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnStatusInfo = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AoiDnStatusInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnStatusInfo = CString(_T("C:\\AOIWork\\Status.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MkWorkPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkWork = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("MonDispMainPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkWork = CString(_T("C:\\PunchWork"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MonDispMainPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMonDispMain = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("MonDispMainPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMonDispMain = CString(_T("C:\\PunchWork\\MonDispMain.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MkMenu01Path"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkMenu01 = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("MkMenu01Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkMenu01 = CString(_T("C:\\PunchWork\\MkMenu01.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MkMenu03Path"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkMenu03 = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("MkMenu03Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkMenu03 = CString(_T("C:\\PunchWork\\MkMenu03.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MkStatusPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkStatus = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("MkStatusPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkStatus = CString(_T("C:\\R2RSet\\Status.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MkInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkInfo = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("MkInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkInfo = CString(_T("C:\\PunchWork\\MkInfoPath.ini"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("PunchingCurrentInfoBufPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathMkCurrInfoBuf = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("PunchingCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathMkCurrInfoBuf = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("EngraveCurrentOffsetInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathEngOffset = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("EngraveCurrentOffsetInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathEngOffset = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIUpPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUp = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOIUpPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIUpCamInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpCamInfo = CString(szData);
	else
	{
		//AfxMessageBox(_T("AOIUpCamInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpCamInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseDTS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bUseDTS = _ttoi(szData);
	else
	{
		//pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("UseDTS가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.bUseDTS = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseITS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bUseITS = _ttoi(szData);
	else
	{
		//pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("UseITS가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.bUseITS = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseDevicePartial"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bUseDevicePartial = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.System.bUseDevicePartial = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseEngrave"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bUseEngrave = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.System.bUseEngrave = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseAoiUp"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bUseAoiUp = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.System.bUseAoiUp = TRUE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseAoiDn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bUseAoiDn = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.System.bUseAoiDn = TRUE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UsePunch"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bUsePunch = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.System.bUsePunch = TRUE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UsePunchOnly"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bUsePunchOnly = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.System.bUsePunchOnly = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIUpDtsPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpDts = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOIUpDtsPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpDts = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIUpDefImgPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpDefImg = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOIUpDtsPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpDefImg = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUpCurrentInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpCurrInfo = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AoiUpCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpCurrInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUpCurrentOffsetInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpOffset = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AoiUpCurrentOffsetInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiUp VRSData Folder"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiUpVrsData = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOIUp VRSData 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiUpVrsData = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIDnPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDn = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOIDnPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIDnCamInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnCamInfo = CString(szData);
	else
	{
		//AfxMessageBox(_T("AOIDnCamInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnCamInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIDnDtsPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnDts = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOIDnDtsPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnDts = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AOIDnDefImgPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnDefImg = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOIDnDtsPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnDefImg = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDnCurrentInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnCurrInfo = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AoiDnCurrentInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnCurrInfo = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDnCurrentOffsetInfoPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnOffset = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AoiDnCurrentOffsetInfoPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("AoiDn VRSData Folder"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathAoiDnVrsData = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOIDn VRSData 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathAoiDnVrsData = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsShareUpDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsShareUp = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("VrsShareUpDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsShareUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsBufferUpDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsBufUp = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("VrsBufferUpDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsBufUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsShareDnDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsShareDn = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("VrsShareDnDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsShareDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsBufferDnDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVrsBufDn = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("VrsBufferDnDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVrsBufDn = CString(_T(""));
	}

	// Share folder For VS
	if (0 < ::GetPrivateProfileString(_T("System"), _T("VsShareUpDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVsShareUp = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("VsShareUpDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVsShareUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VsBufferDummyUpDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVsDummyBufUp = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("VsBufferUpDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVsDummyBufUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VsShareDnDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVsShareDn = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("VsShareDnDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVsShareDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VsBufferDummyDnDirPath "), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathVsDummyBufDn = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("VsBufferDnDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathVsDummyBufDn = CString(_T(""));
	}


	//if (0 < ::GetPrivateProfileString(_T("System"), _T("VsUp Data Folder"), NULL, szData, sizeof(szData), sPath))
	//	WorkingInfo.System.sPathVsShareUp = CString(szData);
	//else
	//{
	//	AfxMessageBox(_T("VSUp Share 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
	//	WorkingInfo.System.sPathVsShareUp = CString(_T(""));
	//}
	//
	//if (0 < ::GetPrivateProfileString(_T("System"), _T("VsDn Data Folder"), NULL, szData, sizeof(szData), sPath))
	//	WorkingInfo.System.sPathVsShareDn = CString(szData);
	//else
	//{
	//	AfxMessageBox(_T("VSDn Share 폴더가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
	//	WorkingInfo.System.sPathVsShareDn = CString(_T(""));
	//}


	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsOldFileDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathOldFile = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("VRS 완료 파일 Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathOldFile = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("ItsOldFileDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathItsFile = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("ItsOldFileDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathItsFile = CString(_T(""));
	}

	//if (0 < ::GetPrivateProfileString(_T("System"), _T("ItsFileDirPath"), NULL, szData, sizeof(szData), sPath))
	//	WorkingInfo.System.sPathIts = CString(szData);
	//else
	//{
	//	pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("ItsFileDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
	//	WorkingInfo.System.sPathIts = CString(_T(""));
	//}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("ItsFileInnerDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathItsInner = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("ItsFileInnerDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathItsInner = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("ItsFileOuterDirPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathItsOuter = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("ItsFileOuterDirPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathItsOuter = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VrsOldFileDirIpPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpPathOldFile = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("VRS 완료 파일 IpPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sIpPathOldFile = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("ItsOldFileDirIpPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpPathItsFile = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("ITS 완료 파일 ItsOldFileDirIpPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sIpPathItsFile = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("ItsFileDirIpPath"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpPathIts = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("ITS 완료 파일 ItsFileDirIpPath가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sIpPathIts = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Sapp3Path"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPathSapp3 = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Sapp3Path가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sPathSapp3 = CString(_T("C:\\LOSS\\"));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SaveReelmapTable"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bSaveReelmapTable = _ttoi(szData);
	else
		WorkingInfo.System.bSaveReelmapTable = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("RemakeReelmapInner"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bRemakeReelmapInner = _ttoi(szData);
	else
		WorkingInfo.System.bRemakeReelmapInner = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("DuplicateReelmap"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bDuplicateRmap = _ttoi(szData);
	else
		WorkingInfo.System.bDuplicateRmap = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("InsertPunchingToDts"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bInsertPunchingToDts = _ttoi(szData);
	else
		WorkingInfo.System.bInsertPunchingToDts = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("DebugEngSig"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bDebugEngSig = _ttoi(szData);
	else
		WorkingInfo.System.bDebugEngSig = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SaveLog"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bSaveLog = _ttoi(szData);
	else
		WorkingInfo.System.bSaveLog = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NO_MARKING"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bNoMk = _ttoi(szData);	// 0 : 마킹모드, 1 : 비젼모드
	else
		WorkingInfo.System.bNoMk = FALSE;			

	if (0 < ::GetPrivateProfileString(_T("System"), _T("VerifyPunching"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bVerifyPunching = _ttoi(szData);	// 0 : 마킹여부 확인 않함, 1 : 마킹여부 확인
	else
		WorkingInfo.System.bVerifyPunching = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SaveMkImage"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bSaveMkImg = _ttoi(szData);	
	else
		WorkingInfo.System.bSaveMkImg = FALSE;			

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SaveGrabImage"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bSaveGrabImg = _ttoi(szData);
	else
		WorkingInfo.System.bSaveGrabImg = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseStripPcsRgnBin"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bStripPcsRgnBin = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.System.bStripPcsRgnBin = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("REVIEW_MARKING_LENGTH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sReViewMkLen = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 ReView가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.System.sReViewMkLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("System"), _T("USE_REVIEW_MARKING"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.bReViewMk = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.System.bReViewMk = FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("MAX_DISP_DEF_IMAGE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sMaxDispDefImg = CString(szData);
	else
		WorkingInfo.System.sMaxDispDefImg = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Cam1Sn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sCamSn[0] = CString(szData);
	else
		WorkingInfo.System.sCamSn[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Cam2Sn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sCamSn[1] = CString(szData);
	else
		WorkingInfo.System.sCamSn[1] = CString(_T(""));

	// READER 2D BARCODE
	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_CLIENT_SR-1000W"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpClient[ID_SR1000W] = CString(szData);
	else
		WorkingInfo.System.sIpClient[ID_SR1000W] = CString(_T("192.168.100.99"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_SERVER_SR-1000W"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpServer[ID_SR1000W] = CString(szData);
	else
		WorkingInfo.System.sIpServer[ID_SR1000W] = CString(_T("192.168.100.100"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Port_SR-1000W"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPort[ID_SR1000W] = CString(szData);
	else
		WorkingInfo.System.sPort[ID_SR1000W] = CString(_T("9004"));

	// ENGRAVE PC
	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_SERVER_ENGRAVE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpServer[ID_ENGRAVE] = CString(szData);
	else
		WorkingInfo.System.sIpServer[ID_ENGRAVE] = CString(_T("100.100.100.119"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Port_ENGRAVE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPort[ID_ENGRAVE] = CString(szData);
	else
		WorkingInfo.System.sPort[ID_ENGRAVE] = CString(_T("8800"));

	// PUNCH PC
	if (0 < ::GetPrivateProfileString(_T("System"), _T("IP_CLIENT_PUNCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sIpClient[ID_PUNCH] = CString(szData);
	else
		WorkingInfo.System.sIpClient[ID_PUNCH] = CString(_T("100.100.101.225"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Port_PUNCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.System.sPort[ID_PUNCH] = CString(szData);
	else
		WorkingInfo.System.sPort[ID_PUNCH] = CString(_T("8800"));

	if (0 < ::GetPrivateProfileString(_T("System"), _T("Delay Show Time"), NULL, szData, sizeof(szData), sPath))
		m_nDelayShow = _ttoi(szData);
	else
		m_nDelayShow = 500;

	// 마킹 금지 구역 - Left
	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft0_Lt_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[0][0].x = _ttof(szData);
	else
		m_pntNoMkLeft[0][0].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft0_Lb_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[0][1].x = _ttof(szData);
	else
		m_pntNoMkLeft[0][1].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft0_Rb_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[0][2].x = _ttof(szData);
	else
		m_pntNoMkLeft[0][2].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft0_Rt_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[0][3].x = _ttof(szData);
	else
		m_pntNoMkLeft[0][3].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft0_Lt_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[0][0].y = _ttof(szData);
	else
		m_pntNoMkLeft[0][0].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft0_Lb_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[0][1].y = _ttof(szData);
	else
		m_pntNoMkLeft[0][1].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft0_Rb_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[0][2].y = _ttof(szData);
	else
		m_pntNoMkLeft[0][2].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft0_Rt_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[0][3].y = _ttof(szData);
	else
		m_pntNoMkLeft[0][3].y = 0.0;


	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft1_Lt_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[1][0].x = _ttof(szData);
	else
		m_pntNoMkLeft[1][0].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft1_Lb_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[1][1].x = _ttof(szData);
	else
		m_pntNoMkLeft[1][1].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft1_Rb_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[1][2].x = _ttof(szData);
	else
		m_pntNoMkLeft[1][2].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft1_Rt_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[1][3].x = _ttof(szData);
	else
		m_pntNoMkLeft[1][3].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft1_Lt_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[1][0].y = _ttof(szData);
	else
		m_pntNoMkLeft[1][0].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft1_Lb_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[1][1].y = _ttof(szData);
	else
		m_pntNoMkLeft[1][1].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft1_Rb_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[1][2].y = _ttof(szData);
	else
		m_pntNoMkLeft[1][2].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkLeft1_Rt_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkLeft[1][3].y = _ttof(szData);
	else
		m_pntNoMkLeft[1][3].y = 0.0;


	// 마킹 금지 구역 - Right
	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight0_Lt_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[0][0].x = _ttof(szData);
	else
		m_pntNoMkRight[0][0].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight0_Lb_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[0][1].x = _ttof(szData);
	else
		m_pntNoMkRight[0][1].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight0_Rb_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[0][2].x = _ttof(szData);
	else
		m_pntNoMkRight[0][2].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight0_Rt_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[0][3].x = _ttof(szData);
	else
		m_pntNoMkRight[0][3].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight0_Lt_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[0][0].y = _ttof(szData);
	else
		m_pntNoMkRight[0][0].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight0_Lb_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[0][1].y = _ttof(szData);
	else
		m_pntNoMkRight[0][1].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight0_Rb_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[0][2].y = _ttof(szData);
	else
		m_pntNoMkRight[0][2].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight0_Rt_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[0][3].y = _ttof(szData);
	else
		m_pntNoMkRight[0][3].y = 0.0;


	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight1_Lt_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[1][0].x = _ttof(szData);
	else
		m_pntNoMkRight[1][0].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight1_Lb_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[1][1].x = _ttof(szData);
	else
		m_pntNoMkRight[1][1].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight1_Rb_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[1][2].x = _ttof(szData);
	else
		m_pntNoMkRight[1][2].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight1_Rt_X"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[1][3].x = _ttof(szData);
	else
		m_pntNoMkRight[1][3].x = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight1_Lt_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[1][0].y = _ttof(szData);
	else
		m_pntNoMkRight[1][0].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight1_Lb_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[1][1].y = _ttof(szData);
	else
		m_pntNoMkRight[1][1].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight1_Rb_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[1][2].y = _ttof(szData);
	else
		m_pntNoMkRight[1][2].y = 0.0;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("NoMkRight1_Rt_Y"), NULL, szData, sizeof(szData), sPath))
		m_pntNoMkRight[1][3].y = _ttof(szData);
	else
		m_pntNoMkRight[1][3].y = 0.0;



	// [Last Job]
	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("MkSt0"), NULL, szData, sizeof(szData), sPath))
		pView->m_bMkSt[0] = _ttoi(szData) > 0 ? TRUE : FALSE;
	else
		pView->m_bMkSt[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("MkSt1"), NULL, szData, sizeof(szData), sPath))
		pView->m_bMkSt[1] = _ttoi(szData) > 0 ? TRUE : FALSE;
	else
		pView->m_bMkSt[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("MkStAuto"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStAuto = _ttoi(szData);
	else
		pView->m_nMkStAuto = 0;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Process Code"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sProcessNum = CString(szData);
	else
	{
		WorkingInfo.LastJob.sProcessNum = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Model Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sModel = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Model이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sModel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LayerUp Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLayerUp = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("LayerUp가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLayerUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Test Mode"), NULL, szData, sizeof(szData), sPath)) // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2
		WorkingInfo.LastJob.nTestMode = _ttoi(szData);
	else
		WorkingInfo.LastJob.nTestMode = 0;

	if (WorkingInfo.LastJob.nTestMode == MODE_INNER)
	{
		pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), TRUE);
		pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), FALSE);
	}
	else if (WorkingInfo.LastJob.nTestMode == MODE_OUTER)
	{
		pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), FALSE);
		pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), TRUE);
	}
	else
	{
		pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), FALSE);
		pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), FALSE);
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Dual AOI"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDualTest = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bDualTest = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("Use Dual AOI"), WorkingInfo.LastJob.bDualTest);

	if (WorkingInfo.LastJob.bDualTest)
	{
		if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("LayerDn Name"), NULL, szData, sizeof(szData), sPath))
			WorkingInfo.LastJob.sLayerDn = CString(szData);
		else
		{
			pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("LayerDn가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
			WorkingInfo.LastJob.sLayerDn = CString(_T(""));
		}
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Lot No"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLot = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("LotUp가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLot = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Last SerialUp"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSerialUp = CString(szData);
	else
	{
		//pView->ClrDispMsg(); AfxMessageBox(_T("SerialUp이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSerialUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Last SerialDn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSerialDn = CString(szData);
	else
	{
		//pView->ClrDispMsg(); AfxMessageBox(_T("SerialDn이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSerialDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Last Serial Eng"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSerialEng = CString(szData);
	else
	{
		//AfxMessageBox(_T("Engrave Serial이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSerialEng = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sEngraveLastShot = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Engrave Last Shot이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sEngraveLastShot = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Completed SerialUp"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCompletedSerialUp = CString(szData);
	else
	{
		//AfxMessageBox(_T("SerialUp이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sCompletedSerialUp = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Completed SerialDn"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCompletedSerialDn = CString(szData);
	else
	{
		//AfxMessageBox(_T("SerialDn이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sCompletedSerialDn = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Operator Name"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSelUserName = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("작업자가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sSelUserName = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Reel Total Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sReelTotLen = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Reel 전체 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sReelTotLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("One Panel Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sOnePnlLen = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("한 판넬 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sOnePnlLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Lot seperate"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bLotSep = _tstoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bLotSep = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("SeparateLot"), WorkingInfo.LastJob.bLotSep);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Lot Seperate Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLotSepLen = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Lot분리 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLotSepLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLotCutPosLen = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Lot절단위치 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLotCutPosLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bTempPause = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bTempPause = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("TempStop"), WorkingInfo.LastJob.bTempPause);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Merging Layer"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nMergingLayer = _ttoi(szData);
	else
		WorkingInfo.LastJob.nMergingLayer = 0; 	// [0]:AOI-Up , [1]:AOI-Dn

	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_bUseTempPause = WorkingInfo.LastJob.bTempPause;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sTempPauseLen = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("일시정지 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sTempPauseLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Limit Total Yield"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLmtTotYld = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("전체 수율 제한 범위가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLmtTotYld = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Limit Partial Yield"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sLmtPatlYld = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("부분 수율 제한 범위가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sLmtPatlYld = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Continuous Fix Defect"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bContFixDef = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bContFixDef = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("FixBed"), pDoc->WorkingInfo.LastJob.bContFixDef);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Shot Num of Range in Fix Defect"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sNumRangeFixDef = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("연속 고정불량 판정 누적 판넬수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sNumRangeFixDef = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Number of Continuous Fix Defect"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sNumContFixDef = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("연속 고정불량 판정 발생 판넬수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.LastJob.sNumContFixDef = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Judge Marking"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseJudgeMk = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUseJudgeMk = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Judge Marking Ratio"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nJudgeMkRatio = _ttoi(szData);
	else
		WorkingInfo.LastJob.nJudgeMkRatio = 85;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Ultra Sonic Cleanner Start Time"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sUltraSonicCleannerStTim = CString(szData);
	else
		WorkingInfo.LastJob.sUltraSonicCleannerStTim = CString(_T("5.0"));


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Recoiler Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bRclDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bRclDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensRecoil"), pDoc->WorkingInfo.LastJob.bRclDrSen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Marking Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bMkDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensPunch"), pDoc->WorkingInfo.LastJob.bMkDrSen);


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use AoiUp Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiUpDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiUpDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensAoiUp"), pDoc->WorkingInfo.LastJob.bAoiUpDrSen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use AoiDn Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiDnDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiDnDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensAoiDn"), pDoc->WorkingInfo.LastJob.bAoiDnDrSen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Uncoiler Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUclDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUclDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensUncoil"), pDoc->WorkingInfo.LastJob.bUclDrSen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Engrave Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bEngvDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bEngvDrSen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensEngrave"), pDoc->WorkingInfo.LastJob.bEngvDrSen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Buffer Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bBufDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bBufDrSen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use 380mm Roll"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUse380mm = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUse380mm = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use 346mm Roll"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUse346mm = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUse346mm = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use 340mm Roll"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUse340mm = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUse340mm = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Display Marked Piece"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDispMkPcs = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bDispMkPcs = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Fix Defect Stop"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bStopFixDef = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bStopFixDef = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Marking Safty Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkSftySen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bMkSftySen = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("SaftySensPunch"), pDoc->WorkingInfo.LastJob.bMkSftySen);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Aoi Safty Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiSftySen = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiSftySen = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Strip Out Ratio"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sStripOutRatio = CString(szData);
	else
		WorkingInfo.LastJob.sStripOutRatio = _T("20.0"); // Minimum 20%

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Custom Need Ratio"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCustomNeedRatio = CString(szData);
	else
		WorkingInfo.LastJob.sCustomNeedRatio = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Partial Speed"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sPartialSpd = CString(szData);
	else
		WorkingInfo.LastJob.sPartialSpd = _T("10.0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Up Aoi CleanRoler"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseAoiUpCleanRoler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUseAoiUpCleanRoler = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("CleanRolerAoiUp"), pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Dn Aoi CleanRoler"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseAoiDnCleanRoler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUseAoiDnCleanRoler = TRUE;

	pDoc->SetMkInfo(_T("Signal"), _T("CleanRolerAoiDn"), pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Its Code"), NULL, szData, sizeof(szData), sPath))
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = CString(szData);
	else
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Current ShotNum"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sCurrentShotNum = CString(szData);
	else
		WorkingInfo.LastJob.sCurrentShotNum = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Setting ShotNum"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSettingShotNum = CString(szData);
	else
		WorkingInfo.LastJob.sSettingShotNum = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Alarm Time AOI"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nAlarmTimeAoi = _ttoi(szData);
	else
		WorkingInfo.LastJob.nAlarmTimeAoi = 10800;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Alarm Time Puncking"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nAlarmTimePunch = _ttoi(szData);
	else
		WorkingInfo.LastJob.nAlarmTimePunch = 10800;


	// 	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Light Value"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.LastJob.sLightVal = CString(szData);
	// 	else
	// 		WorkingInfo.LastJob.sLightVal = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Light On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bLightOn = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bLightOn = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Jog Speed"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sJogSpd = CString(szData);
	else
		WorkingInfo.LastJob.sJogSpd = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkOnePnl = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bMkOnePnl = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiOnePnl = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bAoiOnePnl = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave One Pannel Move On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bEngraveOnePnl = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bEngraveOnePnl = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Verify"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bVerify = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bVerify = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Verify Length"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sVerifyLen = CString(szData);
	else
		WorkingInfo.LastJob.sVerifyLen = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Verify Period"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nVerifyPeriod = _ttoi(szData);
	else
		WorkingInfo.LastJob.nVerifyPeriod = 0;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Review"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bReview = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bReview = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Shot Num for Furge"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sFurgeNum = CString(szData);
	else
		WorkingInfo.LastJob.sFurgeNum = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Furge Num at Run"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sFurgeRun = CString(szData);
	else
		WorkingInfo.LastJob.sFurgeRun = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Mk Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sMkLastShot = CString(szData);
	else
		WorkingInfo.LastJob.sMkLastShot = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AoiUp Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sAoiLastShot[0] = CString(szData);
	else
		WorkingInfo.LastJob.sAoiLastShot[0] = _T("0");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AoiDn Last Shot"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sAoiLastShot[1] = CString(szData);
	else
		WorkingInfo.LastJob.sAoiLastShot[1] = _T("0");


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("One Metal On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bOneMetal = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bOneMetal = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("FeedCcwRecoiler"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bFeedCcwRecoiler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bFeedCcwRecoiler = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("RecoilerCcw"), WorkingInfo.LastJob.bFeedCcwRecoiler);


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Two Metal On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bTwoMetal = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bTwoMetal = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("FeedCcwUncoiler"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bFeedCcwUncoiler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bFeedCcwUncoiler = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("UncoilerCcw"), WorkingInfo.LastJob.bFeedCcwUncoiler);


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Sample Test On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bSampleTest = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bSampleTest = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("Sample Test On"), WorkingInfo.LastJob.bSampleTest);


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSampleTestShotNum = CString(szData);
	else
		WorkingInfo.LastJob.sSampleTestShotNum = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Recoiler Core 150mm"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bCore150Recoiler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bCore150Recoiler = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Uncoiler Core 150mm"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bCore150Uncoiler = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bCore150Uncoiler = FALSE;


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use 2Layer"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUse2Layer = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bUse2Layer = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Order Num"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sEngraveOrderNum = CString(szData);
	else
		WorkingInfo.LastJob.sEngraveOrderNum = _T("");


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sSampleTestShotNum = CString(szData);
	else
		WorkingInfo.LastJob.sSampleTestShotNum = _T("");

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Align Methode"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nAlignMethode = _ttoi(szData);
	else
		WorkingInfo.LastJob.nAlignMethode = TWO_POINT;

	switch (pDoc->WorkingInfo.LastJob.nAlignMethode)
	{
	case TWO_POINT:
		pDoc->SetMkInfo(_T("Signal"), _T("2PtAlign"), TRUE);
		pDoc->SetMkInfo(_T("Signal"), _T("4PtAlign"), FALSE);
		break;
	case FOUR_POINT:
		pDoc->SetMkInfo(_T("Signal"), _T("2PtAlign"), FALSE);
		pDoc->SetMkInfo(_T("Signal"), _T("4PtAlign"), TRUE);
		break;
	default:
		pDoc->SetMkInfo(_T("Signal"), _T("2PtAlign"), TRUE);
		pDoc->SetMkInfo(_T("Signal"), _T("4PtAlign"), FALSE);
		break;
	}


	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Engrave Cleanner"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseEngraveUltrasonic = _ttoi(szData);
	else
		WorkingInfo.LastJob.bUseEngraveUltrasonic= FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("UltrasonicEngrave"), pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use AoiDn Cleanner"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bUseAoiDnUltrasonic = _ttoi(szData);
	else
		WorkingInfo.LastJob.bUseAoiDnUltrasonic = FALSE;

	pDoc->SetMkInfo(_T("Signal"), _T("UltrasonicAoi"), pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic);

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Cleanner Vel"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bVelEngraveUltrasonic = _ttoi(szData);
	else
		WorkingInfo.LastJob.bVelEngraveUltrasonic = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("AoiDn Cleanner Vel"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bVelAoiDnUltrasonic = _ttoi(szData);
	else
		WorkingInfo.LastJob.bVelAoiDnUltrasonic = FALSE;


	// [Motion]
	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("BUFFER_ROLLAR_HOMMING_RUN"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.bBufHomming = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Motion.bBufHomming = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sLmtFdAdjOffSet = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding량 보정 제한범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sLmtFdAdjOffSet = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sLmtFdOvrNum = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding량 보정 연속 초과횟수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sLmtFdOvrNum = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sLmtFdErr = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding량 보정 Error범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sLmtFdErr = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkTq = CString(szData); // Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 텐션서보 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiTq = CString(szData); // Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI 텐션서보 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdDist = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdDist = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdVel = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkJogVel = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding Jog 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkJogVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdAcc = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkJogAcc = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding Jog 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkJogAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_TOTAL_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdTotLen = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding 진행량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdTotLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TOTAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkTotVel = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 전체 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkTotVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_PARTIAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkPatlVel = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 부분 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkPatlVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdDist = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdDist = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdVel = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiJogVel = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding Jog 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiJogVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdAcc = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiJogAcc = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding Jog 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiJogAcc = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_TOTAL_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdTotLen = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding 진행량이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdTotLen = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TOTAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiTotVel = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI 전체 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiTotVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_PARTIAL_CYCLE_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiPatlVel = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI 부분 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiPatlVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS0_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosX[0] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Pin0 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS0_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosY[0] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Pin0 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS1_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosX[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Pin1 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PIN_POS1_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sPinPosY[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Pin1 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sPinPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_X0"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosX[0] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹0 초기 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_Y0"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosY[0] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹0 초기 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_X1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosX[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹1 초기 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_POSITION_Y1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStPosY[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹1 초기 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sStPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MK_END_POSITION_X1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkEdPosX[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹1 초기 위치(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkEdPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MK_END_POSITION_Y1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkEdPosY[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹1 초기 위치(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkEdPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sStBufPos = CString(szData);
	else
		WorkingInfo.Motion.sStBufPos = _T("");

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("BUFFER_HOME_SPEED"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sBufHomeSpd = CString(szData);
	else
		WorkingInfo.Motion.sBufHomeSpd = _T("");

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("BUFFER_HOME_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sBufHomeAcc = CString(szData);
	else
		WorkingInfo.Motion.sBufHomeAcc = _T("");

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_DRUM_LEAD_PITCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdLead = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹부 Feeding 드럼의 원주가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdLead = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_DRUM_LEAD_PITCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdLead = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI Feeding 드럼의 원주가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdLead = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdMkAoiInitDist = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI에서 마킹부까지 진행거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdMkAoiInitDist = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdAoiAoiDistShot = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI(상)에서 AOI(하)까지 진행 Shot수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdAoiAoiDistShot = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiTq = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI 텐션 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkTq = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 텐션 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkTq = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdVacOff = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI롤러 연속 피딩 푸쉬 오프 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdVacOff = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdVacOff = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹롤러 연속 피딩 푸쉬 오프 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdVacOff = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ON_START_WITH_NEW_PRODUCT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.bStNewPrdt = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Motion.bStNewPrdt = FALSE;

	// 	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE_ON"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.Motion.bMkTq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	WorkingInfo.Motion.bMkTq = TRUE;

	// 	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE_ON"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.Motion.bAoiTq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	WorkingInfo.Motion.bAoiTq = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdVacOff = CString(szData);
	else
		WorkingInfo.Motion.sMkFdVacOff = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdVacOff = CString(szData);
	else
		WorkingInfo.Motion.sAoiFdVacOff = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("SAFE_ZONE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sSafeZone = CString(szData);
	else
		WorkingInfo.Motion.sSafeZone = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("COLLISION_LENGTH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sCollisionLength = CString(szData);
	else
		WorkingInfo.Motion.sCollisionLength = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("COLLISION_MARGIN"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sCollisionMargin = CString(szData);
	else
		WorkingInfo.Motion.sCollisionMargin = CString(_T(""));

	// ENGRAVE ==========================================================================================================

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_BARCODE_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveFdBarcodeOffset = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("각인위치에서 2D 코드 리더기까지 Offset거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveFdBarcodeOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("AOI_BARCODE_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sAoiFdBarcodeOffset = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("AOI 검사위치에서 2D 코드 리더기까지 Offset거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sAoiFdBarcodeOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("PUNCHING_BARCODE_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sMkFdBarcodeOffset = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Punching 시작위치에서 2D 코드 리더기까지 Offset거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sMkFdBarcodeOffset = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("INIT_POSITION_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sOffsetInitPos = CString(szData);
	else
	{
		//AfxMessageBox(_T("각인부, 검사부, 마킹부 offset 이송 값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sOffsetInitPos = CString(_T("0.0"));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("2DREADER_FEEDING_SERVO_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdBarcodeOffsetVel = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("바코드 위치 이송 Feeding 속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdBarcodeOffsetVel = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("2DREADER_FEEDING_SERVO_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdBarcodeOffsetAcc = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("바코드 위치 이송 Feeding 가속도가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdBarcodeOffsetAcc = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LEN_AUTO_LIMIT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLaserAdjOffSetLimitAuto = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("바코드 각인 위치 자동 보정 한계값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLaserAdjOffSetLimitAuto = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LEN_MANUAL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLaserAdjOffSetManual = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("바코드 각인 위치 수동 보정값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLaserAdjOffSetManual = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_DRUM_LEAD_PITCH"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveFdLead = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("각인부 Feeding 드럼의 원주가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveFdLead = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_VACUUM_OFF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveFdVacOff = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("각인부 롤러 연속 피딩 푸쉬 오프 길이가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveFdVacOff = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveTq = CString(szData); // Tension Servo 0 Gain Torque Value. Plus Value is Feeding direction torque[Kgf].
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("각인기 텐션서보 토크가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveTq = CString(_T(""));
	}

	WorkingInfo.Motion.bEngraveTq = TRUE;	// "ENGRAVE_TENSION_SERVO_TORQUE_ON"


	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("FEEDING_ENGRAVE_AOI_INIT_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sFdEngraveAoiInitDist = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("각인부에서 마킹부까지 진행거리가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sFdEngraveAoiInitDist = CString(_T(""));
	}


	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_ERROR_VAL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLmtFdErr = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("각인부 Feeding량 보정 Error범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLmtFdErr = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLmtFdAdjOffSet = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("각인부 Feeding량 보정 제한범위값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLmtFdAdjOffSet = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OVER_NUM"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveLmtFdOvrNum = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("각인부 Feeding량 보정 연속 초과횟수가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Motion.sEngraveLmtFdOvrNum = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Motion"), _T("ENGRAVE_START_BUFFER_POSITION"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Motion.sEngraveStBufPos = CString(szData);
	else
		WorkingInfo.Motion.sEngraveStBufPos = _T("");


	// [VoiceCoil0]

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].sPort = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("보이스코일0 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[0].sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].sBaudRate = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("보이스코일0 통신  Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[0].sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[0].bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[0].bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[0].bLf = FALSE;

	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_FINAL_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalToq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacFinalToq = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil0"), _T("VOICE_COIL_SHIFT_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftToq = _tstoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[0].sSmacShiftToq = FALSE;

	// [VoiceCoil1]

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].sPort = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("보이스코일1 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[1].sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].sBaudRate = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("보이스코일1 통신  Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.VoiceCoil[1].sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[1].bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.VoiceCoil[1].bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.VoiceCoil[1].bLf = FALSE;

	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_FINAL_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalToq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacFinalToq = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_POS"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftPos = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftPos = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_VEL"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftVel = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftVel = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_ACC"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftAcc = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftAcc = FALSE;
	// 
	// 	if (0 < ::GetPrivateProfileString(_T("VoiceCoil1"), _T("VOICE_COIL_SHIFT_TOQ"), NULL, szData, sizeof(szData), sPath))
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftToq = _ttoi(szData)?TRUE:FALSE;
	// 	else
	// 		WorkingInfo.VoiceCoil[1].sSmacShiftToq = FALSE;

	// [Fluck RS232 DATA]
#ifdef USE_FLUCK
	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("USE_FLUCK"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.bUse = _ttoi(szData) ? TRUE : FALSE;
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Fluck 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Fluck.bUse = FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_CONTROL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sPort = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Fluck 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Fluck.sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_CONTROL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sBaudRate = CString(szData);
	else
	{
		pView->ClrDispMsg(); AfxMessageBox(_T("Fluck 통신 Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Fluck.sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Fluck.bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("FLUCK_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Fluck.bLf = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("PROBING_THRESHOLD"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sThreshold = CString(szData);
	else
		WorkingInfo.Fluck.sThreshold = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Fluck"), _T("PROBING_REJECT_MK"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sRejectMkNum = CString(szData);
	else
		WorkingInfo.Fluck.sRejectMkNum = CString(_T("20"));
#else

	if (0 < ::GetPrivateProfileString(_T("MarkingTest"), _T("TOTAL_REJECT_MK"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Fluck.sRejectMkNum = CString(szData);
	else
		WorkingInfo.Fluck.sRejectMkNum = CString(_T("20"));
#endif

	// [Light RS232 DATA]
	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_CONTROL_COM_PORT"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sPort = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("조명 포트가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sPort = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_CONTROL_BAUDRATE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sBaudRate = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("조명 통신 Baud Rate가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sBaudRate = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_CR"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.bCr = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Light.bCr = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Light"), _T("LIGHT_LF"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.bLf = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Light.bLf = FALSE;

	// [Light0]

	if (0 < ::GetPrivateProfileString(_T("Light0"), _T("LIGHT_CHANNEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sCh[0] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("조명0의 Channel이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sCh[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light0"), _T("LIGHT_VALUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sVal[0] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("조명0의 통신 밝기값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sVal[0] = CString(_T(""));
	}

	// [Light1]

	if (0 < ::GetPrivateProfileString(_T("Light1"), _T("LIGHT_CHANNEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sCh[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("조명1의 Channel이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sCh[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Light1"), _T("LIGHT_VALUE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Light.sVal[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("조명1의 통신 밝기값이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Light.sVal[1] = CString(_T(""));
	}



	// [Vision0]
	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("PIN_IMG_SIZE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sPinImgSz = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Pin 이미지 크기가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sPinImgSz = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("RESOLUTION_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sResX = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Vision 해상도(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sResX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("RESOLUTION_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sResY = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Vision 해상도(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sResY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sMkOffsetX = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 Offset(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sMkOffsetX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sMkOffsetY = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 Offset(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sMkOffsetY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_STD_SCORE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sStdScr = CString(szData);
	else
	{
		// AfxMessageBox(_T("마킹 ALIGN_STD_SCORE이 설정되어 있지 않습니다."),MB_ICONWARNING|MB_OK);
		WorkingInfo.Vision[0].sStdScr = CString(_T("40"));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS0_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosX[0] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS0_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosY[0] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS1_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosX[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("ALIGN_POS1_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sAlgnPosY[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sAlgnPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision0"), _T("PtMatchingScore"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[0].sPtMatScr = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("패턴 Matching Score가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[0].sPtMatScr = CString(_T(""));
	}

	// [Vision1]
	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("PIN_IMG_SIZE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sPinImgSz = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Pin 이미지 크기가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sPinImgSz = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("RESOLUTION_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sResX = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Vision 해상도(X)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sResX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("RESOLUTION_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sResY = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("Vision 해상도(Y)가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sResY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sMkOffsetX = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 Offset(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sMkOffsetX = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sMkOffsetY = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 Offset(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sMkOffsetY = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_STD_SCORE"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sStdScr = CString(szData);
	else
	{
		//		AfxMessageBox(_T("마킹 ALIGN_STD_SCORE이 설정되어 있지 않습니다."),MB_ICONWARNING|MB_OK);
		WorkingInfo.Vision[1].sStdScr = CString(_T("40"));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS0_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosX[0] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosX[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS0_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosY[0] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosY[0] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS1_X"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosX[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(X)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosX[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("ALIGN_POS1_Y"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sAlgnPosY[1] = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("마킹 첫번째 Align(Y)이 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sAlgnPosY[1] = CString(_T(""));
	}

	if (0 < ::GetPrivateProfileString(_T("Vision1"), _T("PtMatchingScore"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Vision[1].sPtMatScr = CString(szData);
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH); pView->ClrDispMsg(); AfxMessageBox(_T("패턴 Matching Score가 설정되어 있지 않습니다."), MB_ICONWARNING | MB_OK);
		WorkingInfo.Vision[1].sPtMatScr = CString(_T(""));
	}


	// [Lot]
	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Tick"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.dwStTick = _ttoi(szData);
	else
		WorkingInfo.Lot.dwStTick = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Year"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nYear = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nYear = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Month"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nMonth = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nMonth = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Day"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nDay = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nDay = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Hour"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nHour = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nHour = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Minute"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nMin = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nMin = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("Start Second"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.StTime.nSec = _ttoi(szData);
	else
		WorkingInfo.Lot.StTime.nSec = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Year"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nYear = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nYear = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Month"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nMonth = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nMonth = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Day"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nDay = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nDay = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Hour"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nHour = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nHour = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Minute"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nMin = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nMin = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("End Second"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.EdTime.nSec = _ttoi(szData);
	else
		WorkingInfo.Lot.EdTime.nSec = 0;

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_TOTAL_REEL_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sTotalReelDist = CString(szData);
	else
		WorkingInfo.Lot.sTotalReelDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_SEPAREATE_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sSeparateDist = CString(szData);
	else
		WorkingInfo.Lot.sSeparateDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_CUTTING_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sCuttingDist = CString(szData);
	else
		WorkingInfo.Lot.sCuttingDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Lot"), _T("LOT_STOP_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Lot.sStopDist = CString(szData);
	else
		WorkingInfo.Lot.sStopDist = CString(_T(""));


	// [Marking0]

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sWaitPos = CString(szData);
	else
		WorkingInfo.Marking[0].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sWaitVel = CString(szData);
	else
		WorkingInfo.Marking[0].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sWaitAcc = CString(szData);
	else
		WorkingInfo.Marking[0].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingPos = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingVel = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingAcc = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingToq = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_DISP1_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingDisp1Toq = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingDisp1Toq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_DISP2_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingDisp2Toq = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingDisp2Toq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_DISP3_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingDisp3Toq = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingDisp3Toq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMarkingToqOffset = CString(szData);
	else
		WorkingInfo.Marking[0].sMarkingToqOffset = CString(_T("0"));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[0] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[0] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[1] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[1] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[2] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[2] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosX[3] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosX[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sMeasurePosY[3] = CString(szData);
	else
		WorkingInfo.Marking[0].sMeasurePosY[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].sAverDist = CString(szData);
	else
		WorkingInfo.Marking[0].sAverDist = CString(_T(""));


	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("Marking Count"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].nMkCnt = _ttoi(szData);
	else
		WorkingInfo.Marking[0].nMkCnt = 0;

	if (0 < ::GetPrivateProfileString(_T("Marking0"), _T("Marking Limit"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[0].nMkLimit = _ttoi(szData);
	else
		WorkingInfo.Marking[0].nMkLimit = 31536000;


	// [Marking1]

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sWaitPos = CString(szData);
	else
		WorkingInfo.Marking[1].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sWaitVel = CString(szData);
	else
		WorkingInfo.Marking[1].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sWaitAcc = CString(szData);
	else
		WorkingInfo.Marking[1].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingPos = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingVel = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingAcc = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingToq = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_DISP1_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingDisp1Toq = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingDisp1Toq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_DISP2_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingDisp2Toq = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingDisp2Toq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_DISP3_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingDisp3Toq = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingDisp3Toq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ_OFFSET"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMarkingToqOffset = CString(szData);
	else
		WorkingInfo.Marking[1].sMarkingToqOffset = CString(_T("0"));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[0] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY1"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[0] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[0] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[1] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY2"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[1] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[1] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[2] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY3"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[2] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[2] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosX[3] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosX[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY4"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sMeasurePosY[3] = CString(szData);
	else
		WorkingInfo.Marking[1].sMeasurePosY[3] = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("MARKING_AVER_DIST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].sAverDist = CString(szData);
	else
		WorkingInfo.Marking[1].sAverDist = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("Marking Count"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].nMkCnt = _ttoi(szData);
	else
		WorkingInfo.Marking[1].nMkCnt = 0;

	if (0 < ::GetPrivateProfileString(_T("Marking1"), _T("Marking Limit"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Marking[1].nMkLimit = _ttoi(szData);
	else
		WorkingInfo.Marking[1].nMkLimit = 31536000;



	if (0 < ::GetPrivateProfileString(_T("Elec Check"), _T("Use"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].bUse = WorkingInfo.Probing[1].bUse = _ttoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Probing[0].bUse = WorkingInfo.Probing[1].bUse = TRUE;

	if (0 < ::GetPrivateProfileString(_T("Elec Check"), _T("StopOnOpen"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].bStopOnOpen = WorkingInfo.Probing[1].bStopOnOpen = _tstoi(szData) ? TRUE : FALSE;
	else
		WorkingInfo.Probing[0].bStopOnOpen = WorkingInfo.Probing[1].bStopOnOpen = TRUE;


	// [ElecCheck0]

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sWaitPos = CString(szData);
	else
		WorkingInfo.Probing[0].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sWaitVel = CString(szData);
	else
		WorkingInfo.Probing[0].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sWaitAcc = CString(szData);
	else
		WorkingInfo.Probing[0].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingPos = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingVel = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingAcc = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sProbingToq = CString(szData);
	else
		WorkingInfo.Probing[0].sProbingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSX"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sMeasurePosX = CString(szData);
	else
		WorkingInfo.Probing[0].sMeasurePosX = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSY"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sMeasurePosY = CString(szData);
	else
		WorkingInfo.Probing[0].sMeasurePosY = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing0"), _T("REPEAT_TEST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[0].sRepeatTestNum = CString(szData);
	else
		WorkingInfo.Probing[0].sRepeatTestNum = CString(_T("3"));


	// [ElecCheck1]

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sWaitPos = CString(szData);
	else
		WorkingInfo.Probing[1].sWaitPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sWaitVel = CString(szData);
	else
		WorkingInfo.Probing[1].sWaitVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sWaitAcc = CString(szData);
	else
		WorkingInfo.Probing[1].sWaitAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_POS"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingPos = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingPos = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_VEL"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingVel = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingVel = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_ACC"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingAcc = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingAcc = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_TOQ"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sProbingToq = CString(szData);
	else
		WorkingInfo.Probing[1].sProbingToq = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSX"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sMeasurePosX = CString(szData);
	else
		WorkingInfo.Probing[1].sMeasurePosX = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSY"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sMeasurePosY = CString(szData);
	else
		WorkingInfo.Probing[1].sMeasurePosY = CString(_T(""));

	if (0 < ::GetPrivateProfileString(_T("Probing1"), _T("REPEAT_TEST"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.Probing[1].sRepeatTestNum = CString(szData);
	else
		WorkingInfo.Probing[1].sRepeatTestNum = CString(_T("3"));


	// Sapp3code =====================================================================================

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("OPEN"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_OPEN] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_OPEN] = 0;

	// 	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("SHORT_USHORT"), NULL, szData, sizeof(szData), sPath))
	// 		m_nSapp3Code[SAPP3_SHORT_USHORT] = _ttoi(szData);
	// 	else
	// 		m_nSapp3Code[SAPP3_SHORT_USHORT] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("SHORT"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_SHORT] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_SHORT] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("USHORT"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_USHORT] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_USHORT] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("NICK"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_NICK] = _tstoi(szData);
	else
		m_nSapp3Code[SAPP3_NICK] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("SPACE_EXTRA_PROTRUSION"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] = 0;

	// 	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("PINHOLE_PAD"), NULL, szData, sizeof(szData), sPath))
	// 		m_nSapp3Code[SAPP3_PINHOLE_PAD] = _ttoi(szData);
	// 	else
	// 		m_nSapp3Code[SAPP3_PINHOLE_PAD] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("PINHOLE"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_PINHOLE] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_PINHOLE] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("PAD"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_PAD] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_PAD] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("HOPEN"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_HOPEN] = _ttoi(szData);
	else
		m_nSapp3Code[SAPP3_HOPEN] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("HMISS_HPOS_HBAD"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] = _tstoi(szData);
	else
		m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] = 0;

	if (0 < ::GetPrivateProfileString(_T("Sapp3Code"), _T("VHOPEN_NOVH_VHALIGN_VHDEF"), NULL, szData, sizeof(szData), sPath))
		m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] = _tstoi(szData);
	else
		m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] = 0;


	return TRUE;
}


int CGvisR2R_PunchDoc::GetMkCntL() // 1[year] = 31536000[sec]
{
	return (WorkingInfo.Marking[0].nMkCnt);
}

void CGvisR2R_PunchDoc::AddMkCntL()
{
	WorkingInfo.Marking[0].nMkCnt++;
	pView->InsertPunchingData(CAM_LF);
}

void CGvisR2R_PunchDoc::SaveMkCntL()
{
	int nNum = WorkingInfo.Marking[0].nMkCnt;
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	::WritePrivateProfileString(_T("Marking0"), _T("Marking Count"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetMkCntL(int nNum) // 1[year] = 31536000[sec]
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	WorkingInfo.Marking[0].nMkCnt = nNum;
	::WritePrivateProfileString(_T("Marking0"), _T("Marking Count"), sData, sPath);

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetMkNumLf();	//_ItemInx::_MkNumLf
//#endif
}

int CGvisR2R_PunchDoc::GetMkLimitL() // 1[year] = 31536000[sec]
{
	return (WorkingInfo.Marking[0].nMkLimit);
}

void CGvisR2R_PunchDoc::SetMkLimitL(int nNum) // 1[year] = 31536000[sec]
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	WorkingInfo.Marking[0].nMkLimit = nNum;
	::WritePrivateProfileString(_T("Marking0"), _T("Marking Limit"), sData, sPath);

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetMkMaxNumLf();	//_ItemInx::_MkMaxNumLf
//#endif
}

int CGvisR2R_PunchDoc::GetMkCntR() // 1[year] = 31536000[sec]
{
	return (WorkingInfo.Marking[1].nMkCnt);
}

void CGvisR2R_PunchDoc::AddMkCntR()
{
	WorkingInfo.Marking[1].nMkCnt++;
	pView->InsertPunchingData(CAM_RT);
}

void CGvisR2R_PunchDoc::SaveMkCntR()
{
	int nNum = WorkingInfo.Marking[1].nMkCnt;
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	::WritePrivateProfileString(_T("Marking1"), _T("Marking Count"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetMkCntR(int nNum) // 1[year] = 31536000[sec]
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	WorkingInfo.Marking[1].nMkCnt = nNum;
	::WritePrivateProfileString(_T("Marking1"), _T("Marking Count"), sData, sPath);

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetMkNumRt();	//_ItemInx::_MkNumRt
//#endif
}

int CGvisR2R_PunchDoc::GetMkLimitR() // 1[year] = 31536000[sec]
{
	return (WorkingInfo.Marking[1].nMkLimit);
}

void CGvisR2R_PunchDoc::SetMkLimitR(int nNum) // 1[year] = 31536000[sec]
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	WorkingInfo.Marking[1].nMkLimit = nNum;
	::WritePrivateProfileString(_T("Marking1"), _T("Marking Limit"), sData, sPath);

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetMkMaxNumRt();	//_ItemInx::_MkMaxNumRt
//#endif
}

void CGvisR2R_PunchDoc::SaveWorkingInfo()
{
	CString sData, sPath = PATH_WORKING_INFO;

	// [System]

	sData.Format(_T("%d"), WorkingInfo.System.bNoMk ? 1 : 0);					// 0 : 마킹모드, 1 : 비젼모드
	::WritePrivateProfileString(_T("System"), _T("NO_MARKING"), sData, sPath);

	sData = WorkingInfo.System.sReViewMkLen;
	::WritePrivateProfileString(_T("System"), _T("REVIEW_MARKING_LENGTH"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.System.bReViewMk ? 1 : 0);
	::WritePrivateProfileString(_T("System"), _T("USE_REVIEW_MARKING"), sData, sPath);


	// [Last Job]
	sData = WorkingInfo.LastJob.sProcessNum;
	::WritePrivateProfileString(_T("Last Job"), _T("Process Code"), sData, sPath);

	sData = WorkingInfo.LastJob.sModel;
	::WritePrivateProfileString(_T("Last Job"), _T("Model Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLayerUp;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerUp Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLot;
	::WritePrivateProfileString(_T("Last Job"), _T("Lot No"), sData, sPath);

	sData = WorkingInfo.LastJob.sEngItsCode;
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Its Code"), sData, sPath);

	sData = WorkingInfo.LastJob.sSerialUp;
	::WritePrivateProfileString(_T("Last Job"), _T("Last SerialUp"), sData, sPath);

	sData = WorkingInfo.LastJob.sSerialEng;
	::WritePrivateProfileString(_T("Last Job"), _T("Last Serial Eng"), sData, sPath);

	sData = WorkingInfo.LastJob.sCompletedSerialUp;
	::WritePrivateProfileString(_T("Last Job"), _T("Completed SerialUp"), sData, sPath);

	//sData = WorkingInfo.LastJob.sModelDn;
	//::WritePrivateProfileString(_T("Last Job"), _T("ModelDn Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLayerDn;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerDn Name"), sData, sPath);

	//sData = WorkingInfo.LastJob.sLotDn;
	//::WritePrivateProfileString(_T("Last Job"), _T("LotDn No"), sData, sPath);

	sData = WorkingInfo.LastJob.sSerialDn;
	::WritePrivateProfileString(_T("Last Job"), _T("Last SerialDn"), sData, sPath);

	sData = WorkingInfo.LastJob.sCompletedSerialDn;
	::WritePrivateProfileString(_T("Last Job"), _T("Completed SerialDn"), sData, sPath);

	sData = WorkingInfo.LastJob.sSelUserName;
	::WritePrivateProfileString(_T("Last Job"), _T("Operator Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sReelTotLen;
	::WritePrivateProfileString(_T("Last Job"), _T("Reel Total Length"), sData, sPath);

	sData = WorkingInfo.LastJob.sOnePnlLen;
	::WritePrivateProfileString(_T("Last Job"), _T("One Panel Length"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bLotSep ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Lot seperate"), sData, sPath);

	sData = WorkingInfo.LastJob.sLotSepLen;
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Seperate Length"), sData, sPath);

	sData = WorkingInfo.LastJob.sLotCutPosLen;
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bTempPause ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.nMergingLayer); 	// [0]:AOI-Up , [1]:AOI-Dn
	::WritePrivateProfileString(_T("Last Job"), _T("Merging Layer"), sData, sPath);

	sData = WorkingInfo.LastJob.sTempPauseLen;
	::WritePrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), sData, sPath);

	sData = WorkingInfo.LastJob.sLmtTotYld;
	::WritePrivateProfileString(_T("Last Job"), _T("Limit Total Yield"), sData, sPath);

	sData = WorkingInfo.LastJob.sLmtPatlYld;
	::WritePrivateProfileString(_T("Last Job"), _T("Limit Partial Yield"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bContFixDef ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Continuous Fix Defect"), sData, sPath);

	sData = WorkingInfo.LastJob.sNumRangeFixDef;
	::WritePrivateProfileString(_T("Last Job"), _T("Shot Num of Range in Fix Defect"), sData, sPath);

	sData = WorkingInfo.LastJob.sNumContFixDef;
	::WritePrivateProfileString(_T("Last Job"), _T("Number of Continuous Fix Defect"), sData, sPath);

	sData = WorkingInfo.LastJob.sUltraSonicCleannerStTim;
	::WritePrivateProfileString(_T("Last Job"), _T("Ultra Sonic Cleanner Start Time"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bRclDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Recoiler Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bMkDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Marking Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Aoi Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Aoi Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bEngvDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Engrave Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bUclDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Uncoiler Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bBufDrSen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Buffer Door Sensor"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bUse380mm ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use 380mm Roll"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bDispMkPcs ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Display Marked Piece"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bStopFixDef ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Fix Defect Stop"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bMkSftySen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Marking Safty Senso"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bAoiSftySen ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Aoi Safty Sensor"), sData, sPath);

	// 	sData = WorkingInfo.LastJob.sLightVal;
	// 	::WritePrivateProfileString(_T("Last Job"), _T("Light Value"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bLightOn ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Light On"), sData, sPath);

	sData = WorkingInfo.LastJob.sJogSpd;
	::WritePrivateProfileString(_T("Last Job"), _T("Jog Speed"), sData, sPath);

	sData = WorkingInfo.LastJob.sLotSerial;
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Serial"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bMkOnePnl ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Marking One Pannel Move On"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bAoiOnePnl ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("AOI One Pannel Move On"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bEngraveOnePnl ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave One Pannel Move On"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bVerify ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Verify"), sData, sPath);

	sData = WorkingInfo.LastJob.sVerifyLen;
	::WritePrivateProfileString(_T("Last Job"), _T("Verify Length"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.nVerifyPeriod);
	::WritePrivateProfileString(_T("Last Job"), _T("Verify Period"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bReview ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Use Review"), sData, sPath);

	// 	sData = WorkingInfo.LastJob.sDistAoiMk;
	// 	::WritePrivateProfileString(_T("Last Job"), _T("DistanceAoiMk"), sData, sPath);

	sData = WorkingInfo.LastJob.sFurgeNum;
	::WritePrivateProfileString(_T("Last Job"), _T("Shot Num for Furge"), sData, sPath);

	sData = WorkingInfo.LastJob.sFurgeRun;
	::WritePrivateProfileString(_T("Last Job"), _T("Furge Num at Run"), sData, sPath);

	sData = WorkingInfo.LastJob.sMkLastShot;
	::WritePrivateProfileString(_T("Last Job"), _T("Mk Last Shot"), sData, sPath);

	sData = WorkingInfo.LastJob.sAoiLastShot[0];
	::WritePrivateProfileString(_T("Last Job"), _T("AoiUp Last Shot"), sData, sPath);

	sData = WorkingInfo.LastJob.sAoiLastShot[1];
	::WritePrivateProfileString(_T("Last Job"), _T("AoiDn Last Shot"), sData, sPath);

	sData = WorkingInfo.LastJob.sEngraveLastShot;
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Last Shot"), sData, sPath);

	sData = WorkingInfo.LastJob.sStripOutRatio;
	::WritePrivateProfileString(_T("Last Job"), _T("Strip Out Ratio"), sData, sPath);

	sData = WorkingInfo.LastJob.sCustomNeedRatio;
	::WritePrivateProfileString(_T("Last Job"), _T("Custom Need Ratio"), sData, sPath);

	sData = WorkingInfo.LastJob.sPartialSpd;
	::WritePrivateProfileString(_T("Last Job"), _T("Partial Speed"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bVelEngraveUltrasonic ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Cleanner Vel"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.LastJob.bVelAoiDnUltrasonic ? 1 : 0);
	::WritePrivateProfileString(_T("Last Job"), _T("AoiDn Cleanner Vel"), sData, sPath);



	// [Motion]
	sData.Format(_T("%d"), WorkingInfo.Motion.bBufHomming ? 1 : 0);
	::WritePrivateProfileString(_T("Motion"), _T("BUFFER_ROLLAR_HOMMING_RUN"), sData, sPath);

	sData = WorkingInfo.Motion.sLmtFdAdjOffSet;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sLmtFdOvrNum;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), sData, sPath);

	sData = WorkingInfo.Motion.sLmtFdErr;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), sData, sPath);

	sData = WorkingInfo.Motion.sMkTq;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiTq;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdDist;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdVel;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sMkJogVel;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdAcc;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), sData, sPath);

	sData = WorkingInfo.Motion.sMkJogAcc;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdTotLen;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_TOTAL_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sMkTotVel;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TOTAL_CYCLE_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sMkPatlVel;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_PARTIAL_CYCLE_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdDist;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdVel;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiJogVel;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdAcc;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiJogAcc;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdTotLen;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_TOTAL_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiTotVel;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TOTAL_CYCLE_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiPatlVel;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_PARTIAL_CYCLE_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sPinPosX[0];
	::WritePrivateProfileString(_T("Motion"), _T("PIN_POS0_X"), sData, sPath);

	sData = WorkingInfo.Motion.sPinPosY[0];
	::WritePrivateProfileString(_T("Motion"), _T("PIN_POS0_Y"), sData, sPath);

	sData = WorkingInfo.Motion.sPinPosX[1];
	::WritePrivateProfileString(_T("Motion"), _T("PIN_POS1_X"), sData, sPath);

	sData = WorkingInfo.Motion.sPinPosY[1];
	::WritePrivateProfileString(_T("Motion"), _T("PIN_POS1_Y"), sData, sPath);

	sData = WorkingInfo.Motion.sStPosX[0];
	::WritePrivateProfileString(_T("Motion"), _T("START_POS0_X"), sData, sPath);

	sData = WorkingInfo.Motion.sStPosY[0];
	::WritePrivateProfileString(_T("Motion"), _T("START_POS0_Y"), sData, sPath);

	sData = WorkingInfo.Motion.sStPosX[1];
	::WritePrivateProfileString(_T("Motion"), _T("START_POS1_X"), sData, sPath);

	sData = WorkingInfo.Motion.sStPosY[1];
	::WritePrivateProfileString(_T("Motion"), _T("START_POS1_Y"), sData, sPath);

	sData = WorkingInfo.Motion.sStBufPos;
	::WritePrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdLead;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_DRUM_LEAD_PITCH"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdLead;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_DRUM_LEAD_PITCH"), sData, sPath);

	sData = WorkingInfo.Motion.sFdMkAoiInitDist;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sFdAoiAoiDistShot;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), sData, sPath);

	// 	sData = WorkingInfo.Motion.sBufStdPosDist;
	// 	::WritePrivateProfileString(_T("Motion"), _T("BUF_STD_POS_FEEDING_INIT_DIST"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Motion.bStNewPrdt ? 1 : 0);
	::WritePrivateProfileString(_T("Motion"), _T("ON_START_WITH_NEW_PRODUCT"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Motion.bMkTq ? 1 : 0);
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE_ON"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Motion.bAoiTq ? 1 : 0);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE_ON"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Motion.bEngraveTq ? 1 : 0);
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE_ON"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdVacOff;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_VACUUM_OFF"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdVacOff;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_VACUUM_OFF"), sData, sPath);

	// Engrave =======================================================================================
	sData = WorkingInfo.Motion.sEngraveFdBarcodeOffset;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_BARCODE_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sAoiFdBarcodeOffset;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_BARCODE_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sMkFdBarcodeOffset;
	::WritePrivateProfileString(_T("Motion"), _T("PUNCHING_BARCODE_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sOffsetInitPos;
	::WritePrivateProfileString(_T("Motion"), _T("INIT_POSITION_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sFdBarcodeOffsetVel;
	::WritePrivateProfileString(_T("Motion"), _T("2DREADER_FEEDING_SERVO_VEL"), sData, sPath);

	sData = WorkingInfo.Motion.sFdBarcodeOffsetAcc;
	::WritePrivateProfileString(_T("Motion"), _T("2DREADER_FEEDING_SERVO_ACC"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveLaserAdjOffSetLimitAuto;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LEN_AUTO_LIMIT"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveLaserAdjOffSetManual;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LEN_MANUAL"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveFdLead;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_DRUM_LEAD_PITCH"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveFdVacOff;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_VACUUM_OFF"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveTq;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE"), sData, sPath);


	sData = WorkingInfo.Motion.sFdEngraveAoiInitDist;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_ENGRAVE_AOI_INIT_DIST"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveLmtFdErr;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_ERROR_VAL"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveLmtFdAdjOffSet;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OFFSET"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveLmtFdOvrNum;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OVER_NUM"), sData, sPath);

	sData = WorkingInfo.Motion.sEngraveStBufPos;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_START_BUFFER_POSITION"), sData, sPath);


	// [Light]
	sData = WorkingInfo.Light.sPort;
	::WritePrivateProfileString(_T("Light"), _T("LIGHT_CONTROL_COM_PORT"), sData, sPath);

	sData = WorkingInfo.Light.sBaudRate;
	::WritePrivateProfileString(_T("Light"), _T("LIGHT_CONTROL_BAUDRATE"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Light.bCr ? 1 : 0);
	::WritePrivateProfileString(_T("Light"), _T("LIGHT_CR"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Light.bLf ? 1 : 0);
	::WritePrivateProfileString(_T("Light"), _T("LIGHT_LF"), sData, sPath);

	// [Light0]

	sData = WorkingInfo.Light.sCh[0];
	::WritePrivateProfileString(_T("Light0"), _T("LIGHT_CHANNEL"), sData, sPath);

	sData = WorkingInfo.Light.sVal[0];
	::WritePrivateProfileString(_T("Light0"), _T("LIGHT_VALUE"), sData, sPath);

	// [Light1]

	sData = WorkingInfo.Light.sCh[1];
	::WritePrivateProfileString(_T("Light1"), _T("LIGHT_CHANNEL"), sData, sPath);

	sData = WorkingInfo.Light.sVal[1];
	::WritePrivateProfileString(_T("Light1"), _T("LIGHT_VALUE"), sData, sPath);


	// [Vision0]
	sData = WorkingInfo.Vision[0].sPinImgSz;
	::WritePrivateProfileString(_T("Vision0"), _T("PIN_IMG_SIZE"), sData, sPath);

	sData = WorkingInfo.Vision[0].sResX;
	::WritePrivateProfileString(_T("Vision0"), _T("RESOLUTION_X"), sData, sPath);

	sData = WorkingInfo.Vision[0].sResY;
	::WritePrivateProfileString(_T("Vision0"), _T("RESOLUTION_Y"), sData, sPath);

	sData = WorkingInfo.Vision[0].sMkOffsetX;
	::WritePrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_X"), sData, sPath);

	sData = WorkingInfo.Vision[0].sMkOffsetY;
	::WritePrivateProfileString(_T("Vision0"), _T("MARKING_OFFSET_Y"), sData, sPath);

	sData = WorkingInfo.Vision[0].sStdScr;
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_STD_SCORE"), sData, sPath);

	sData = WorkingInfo.Vision[0].sAlgnPosX[0];
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_POS0_X"), sData, sPath);

	sData = WorkingInfo.Vision[0].sAlgnPosY[0];
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_POS0_Y"), sData, sPath);

	sData = WorkingInfo.Vision[0].sAlgnPosX[1];
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_POS1_X"), sData, sPath);

	sData = WorkingInfo.Vision[0].sAlgnPosY[1];
	::WritePrivateProfileString(_T("Vision0"), _T("ALIGN_POS1_Y"), sData, sPath);

	sData = WorkingInfo.Vision[0].sPtMatScr;
	::WritePrivateProfileString(_T("Vision0"), _T("PtMatchingScore"), sData, sPath);

	// [Vision1]
	sData = WorkingInfo.Vision[1].sPinImgSz;
	::WritePrivateProfileString(_T("Vision1"), _T("PIN_IMG_SIZE"), sData, sPath);

	sData = WorkingInfo.Vision[1].sResX;
	::WritePrivateProfileString(_T("Vision1"), _T("RESOLUTION_X"), sData, sPath);

	sData = WorkingInfo.Vision[1].sResY;
	::WritePrivateProfileString(_T("Vision1"), _T("RESOLUTION_Y"), sData, sPath);

	sData = WorkingInfo.Vision[1].sMkOffsetX;
	::WritePrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_X"), sData, sPath);

	sData = WorkingInfo.Vision[1].sMkOffsetY;
	::WritePrivateProfileString(_T("Vision1"), _T("MARKING_OFFSET_Y"), sData, sPath);

	sData = WorkingInfo.Vision[1].sStdScr;
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_STD_SCORE"), sData, sPath);

	sData = WorkingInfo.Vision[1].sAlgnPosX[0];
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_POS0_X"), sData, sPath);

	sData = WorkingInfo.Vision[1].sAlgnPosY[0];
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_POS0_Y"), sData, sPath);

	sData = WorkingInfo.Vision[1].sAlgnPosX[1];
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_POS1_X"), sData, sPath);

	sData = WorkingInfo.Vision[1].sAlgnPosY[1];
	::WritePrivateProfileString(_T("Vision1"), _T("ALIGN_POS1_Y"), sData, sPath);

	sData = WorkingInfo.Vision[1].sPtMatScr;
	::WritePrivateProfileString(_T("Vision1"), _T("PtMatchingScore"), sData, sPath);

	// [Lot]
	sData.Format(_T("%d"), WorkingInfo.Lot.dwStTick);
	::WritePrivateProfileString(_T("Lot"), _T("Start Tick"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nYear);
	::WritePrivateProfileString(_T("Lot"), _T("Start Year"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nMonth);
	::WritePrivateProfileString(_T("Lot"), _T("Start Month"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nDay);
	::WritePrivateProfileString(_T("Lot"), _T("Start Day"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nHour);
	::WritePrivateProfileString(_T("Lot"), _T("Start Hour"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nMin);
	::WritePrivateProfileString(_T("Lot"), _T("Start Minute"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nSec);
	::WritePrivateProfileString(_T("Lot"), _T("Start Second"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nYear);
	::WritePrivateProfileString(_T("Lot"), _T("End Year"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nMonth);
	::WritePrivateProfileString(_T("Lot"), _T("End Month"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nDay);
	::WritePrivateProfileString(_T("Lot"), _T("End Day"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nHour);
	::WritePrivateProfileString(_T("Lot"), _T("End Hour"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nMin);
	::WritePrivateProfileString(_T("Lot"), _T("End Minute"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nSec);
	::WritePrivateProfileString(_T("Lot"), _T("End Second"), sData, sPath);
}


void CGvisR2R_PunchDoc::ClrPcr()
{
	//CString sMsg;

	int nIdx, i, k;
	for (i = 0; i < MAX_PCR; i++)
	{
		for (nIdx = 0; nIdx < MAX_PCR_PNL; nIdx++)
		{
			if (m_pPcr[i][0])
			{
				m_pPcr[i][nIdx]->m_nIdx = -1;							// m_nIdx : From 0 to nTot.....
				m_pPcr[i][nIdx]->m_nSerial = 0;
				m_pPcr[i][nIdx]->m_nErrPnl = 0;
				m_pPcr[i][nIdx]->m_sModel = _T("");
				m_pPcr[i][nIdx]->m_sLayer = _T("");
				m_pPcr[i][nIdx]->m_sLot = _T("");

				m_pPcr[i][nIdx]->m_nCamId = 0;
				m_pPcr[i][nIdx]->m_nTotDef = 0;
				m_pPcr[i][nIdx]->m_nTotRealDef = 0;
			}

			if (m_pPcrInner[i][0])
			{
				m_pPcrInner[i][nIdx]->m_nIdx = -1;							// m_nIdx : From 0 to nTot.....
				m_pPcrInner[i][nIdx]->m_nSerial = 0;
				m_pPcrInner[i][nIdx]->m_nErrPnl = 0;
				m_pPcrInner[i][nIdx]->m_sModel = _T("");
				m_pPcrInner[i][nIdx]->m_sLayer = _T("");
				m_pPcrInner[i][nIdx]->m_sLot = _T("");

				m_pPcrInner[i][nIdx]->m_nCamId = 0;
				m_pPcrInner[i][nIdx]->m_nTotDef = 0;
				m_pPcrInner[i][nIdx]->m_nTotRealDef = 0;
			}
		}
	}

	for (k = 0; k < MAX_PCR_PNL; k++)
	{
		if (m_pPcrIts[k])
		{
			m_pPcrIts[k]->m_nIdx = -1;							// m_nIdx : From 0 to nTot.....
			m_pPcrIts[k]->m_nSerial = 0;
			m_pPcrIts[k]->m_nErrPnl = 0;
			m_pPcrIts[k]->m_sModel = _T("");
			m_pPcrIts[k]->m_sLayer = _T("");
			m_pPcrIts[k]->m_sLot = _T("");

			m_pPcrIts[k]->m_nCamId = 0;
			m_pPcrIts[k]->m_nTotDef = 0;
			m_pPcrIts[k]->m_nTotRealDef = 0;
		}
	}
}

BOOL CGvisR2R_PunchDoc::InitReelmap()
{
	if (!m_Master[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (m_pReelMapUp)
	{
		delete m_pReelMapUp;
		m_pReelMapUp = NULL;
	}
	m_pReelMapUp = new CReelMap(RMAP_UP, MAX_DISP_PNL, nTotPcs);

	if (bDualTest)
	{
		if (m_pReelMapDn)
		{
			delete m_pReelMapDn;
			m_pReelMapDn = NULL;
		}
		m_pReelMapDn = new CReelMap(RMAP_DN, MAX_DISP_PNL, nTotPcs);

		if (m_pReelMapAllUp)
		{
			delete m_pReelMapAllUp;
			m_pReelMapAllUp = NULL;
		}
		m_pReelMapAllUp = new CReelMap(RMAP_ALLUP, MAX_DISP_PNL, nTotPcs);

		if (m_pReelMapAllDn)
		{
			delete m_pReelMapAllDn;
			m_pReelMapAllDn = NULL;
		}
		m_pReelMapAllDn = new CReelMap(RMAP_ALLDN, MAX_DISP_PNL, nTotPcs);

		if(pDoc->GetTestMode() != MODE_OUTER)
			m_pReelMap = m_pReelMapAllUp;
		else
		{
			if (m_pReelMapIts)
			{
				delete m_pReelMapIts;
				m_pReelMapIts = NULL;
			}
			m_pReelMapIts = new CReelMap(RMAP_ITS, MAX_DISP_PNL, nTotPcs); // Default: RMAP_NONE (RMAP_INNER -> RMAP_INNER_UP)
			m_pReelMap = m_pReelMapIts;
		}
	}
	else
	{
		if (pDoc->GetTestMode() != MODE_OUTER)
			m_pReelMap = m_pReelMapUp;
		else
		{
			if (m_pReelMapIts)
				m_pReelMap = m_pReelMapIts;
		}
	}

	//if (pDoc->GetTestMode() == MODE_OUTER)
	//{
	//	InitReelmapInner();
	//}

	if (pMkInfo)
	{
		delete[] pMkInfo;
		pMkInfo = NULL;
	}
	if (!pMkInfo)
		pMkInfo = new CString[nTotPcs];

	if (m_pReelMap)
	{
		m_pReelMapDisp = m_pReelMap;

		if (bDualTest)
			pView->m_pDlgMenu01->SelMap(ALL);
		else
			pView->m_pDlgMenu01->SelMap(UP);
	}
	return TRUE;
}

BOOL CGvisR2R_PunchDoc::InitReelmapUp()
{
	if (!m_Master[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->MsgBox(strMsg);
		return FALSE;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;

	if (m_pReelMapUp)
	{
		delete m_pReelMapUp;
		m_pReelMapUp = NULL;
	}
	m_pReelMapUp = new CReelMap(RMAP_UP, MAX_DISP_PNL, nTotPcs);

	if (bDualTest)
	{
		if (m_pReelMapAllUp)
		{
			delete m_pReelMapAllUp;
			m_pReelMapAllUp = NULL;
		}
		m_pReelMapAllUp = new CReelMap(RMAP_ALLUP, MAX_DISP_PNL, nTotPcs);

		if (pDoc->GetTestMode() != MODE_OUTER)
 			m_pReelMap = m_pReelMapAllUp;
		else
		{
			if (m_pReelMapIts)
			{
				delete m_pReelMapIts;
				m_pReelMapIts = NULL;
			}
			m_pReelMapIts = new CReelMap(RMAP_ITS, MAX_DISP_PNL, nTotPcs); // Default: RMAP_NONE (RMAP_INNER -> RMAP_INNER_UP)
				m_pReelMap = m_pReelMapIts;
		}
	}
	else
	{
		if (pDoc->GetTestMode() != MODE_OUTER)
			m_pReelMap = m_pReelMapUp;
		else
		{
			if (m_pReelMapIts)
				m_pReelMap = m_pReelMapIts;
		}
	}

	if (m_pReelMap->m_nLayer == RMAP_UP || m_pReelMap->m_nLayer == RMAP_ALLUP)
	{
		if (pMkInfo)
		{
			delete[] pMkInfo;
			pMkInfo = NULL;
		}
		if (!pMkInfo)
			pMkInfo = new CString[nTotPcs];
	}

	if (pView->m_pDlgMenu01)
		pView->m_pDlgMenu01->SwitchReelmapDisp(pView->m_nSelRmap);

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::InitReelmapDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return TRUE;

	if (!m_Master[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->MsgBox(strMsg);
		return FALSE;
	}

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;

	if (m_pReelMapDn)
	{
		delete m_pReelMapDn;
		m_pReelMapDn = NULL;
	}
	m_pReelMapDn = new CReelMap(RMAP_DN, MAX_DISP_PNL, nTotPcs);

	if (m_pReelMapAllDn)
	{
		delete m_pReelMapAllDn;
		m_pReelMapAllDn = NULL;
	}
	m_pReelMapAllDn = new CReelMap(RMAP_ALLDN, MAX_DISP_PNL, nTotPcs);

	if (m_pReelMap->m_nLayer == RMAP_DN || m_pReelMap->m_nLayer == RMAP_ALLDN)
	{
		if (pMkInfo)
		{
			delete[] pMkInfo;
			pMkInfo = NULL;
		}
		if (!pMkInfo)
			pMkInfo = new CString[nTotPcs];
	}

	if (pView->m_pDlgMenu01)
		pView->m_pDlgMenu01->SwitchReelmapDisp(pView->m_nSelRmap);

	return TRUE;
}

void CGvisR2R_PunchDoc::InitPcr()
{
	int k, i;
	for (i = 0; i < MAX_PCR; i++)
	{
		for (k = 0; k < MAX_PCR_PNL; k++)
		{
			if (m_pPcr[i][k])
			{
				delete m_pPcr[i][k];
				m_pPcr[i][k] = NULL;
			}

			m_pPcr[i][k] = new CDataMarking();

			if (m_pPcrInner[i][k])
			{
				delete m_pPcrInner[i][k];
				m_pPcrInner[i][k] = NULL;
			}

			m_pPcrInner[i][k] = new CDataMarking();
		}
	}

	for (k = 0; k < MAX_PCR_PNL; k++)
	{
		if (m_pPcrIts[k])
		{
			delete m_pPcrIts[k];
			m_pPcrIts[k] = NULL;
		}

		m_pPcrIts[k] = new CDataMarking();
	}
}

void CGvisR2R_PunchDoc::SetReelmap(int nDir)
{
	if (!m_pReelMap || !m_Master[0].m_pPcsRgn)
		return;

	if (!m_pReelMap->pFrmRgn || !m_pReelMap->pPcsRgn)
		return;

	int i, k;
	double fData1, fData2, fData3, fData4, fDistX, fDistY;
	double fWidth, fHeight, fRight, fBottom;

	m_pReelMap->nDir = nDir;

	int nTotPnl = m_pReelMap->nTotPnl;
	int nTotPcs = m_pReelMap->nTotPcs;

	double dScale = (m_Master[0].MasterInfo.dPixelSize / 10.0);
	m_pReelMap->SetAdjRatio(dScale);
	dScale = m_pReelMap->GetAdjRatio();

	for (k = 0; k < nTotPnl; k++)
	{
		switch (nDir)
		{
		case ROT_NONE:
			fWidth = (m_Master[0].m_pPcsRgn->pPcs[0].right - m_Master[0].m_pPcsRgn->pPcs[0].left);
			fHeight = (m_Master[0].m_pPcsRgn->pPcs[0].bottom - m_Master[0].m_pPcsRgn->pPcs[0].top);
			fRight = m_Master[0].m_pPcsRgn->rtFrm.right - fWidth * (1.0 - RMAP_PCS_SCALE);
			fBottom = m_Master[0].m_pPcsRgn->rtFrm.bottom - fHeight * (1.0 - RMAP_PCS_SCALE);;
			m_pReelMap->pFrmRgn[k].left = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.left;
			m_pReelMap->pFrmRgn[k].top = m_Master[0].m_pPcsRgn->rtFrm.top;
			m_pReelMap->pFrmRgn[k].right = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fRight;
			m_pReelMap->pFrmRgn[k].bottom = fBottom;
			if (m_pReelMapDisp)
			{
				m_pReelMapDisp->pFrmRgn[k].left = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.left;
				m_pReelMapDisp->pFrmRgn[k].top = m_Master[0].m_pPcsRgn->rtFrm.top;
				m_pReelMapDisp->pFrmRgn[k].right = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fRight;
				m_pReelMapDisp->pFrmRgn[k].bottom = fBottom;
			}

			for (i = 0; i < nTotPcs; i++)
			{
				fWidth = (m_Master[0].m_pPcsRgn->pPcs[i].right - m_Master[0].m_pPcsRgn->pPcs[i].left);
				fHeight = (m_Master[0].m_pPcsRgn->pPcs[i].bottom - m_Master[0].m_pPcsRgn->pPcs[i].top);
				fData1 = m_Master[0].m_pPcsRgn->pPcs[i].left;	// left
				fData2 = m_Master[0].m_pPcsRgn->pPcs[i].top;	// top
				fData3 = fData1 + fWidth * RMAP_PCS_SCALE; // right
				fData4 = fData2 + fHeight * RMAP_PCS_SCALE; // bottom

				m_pReelMap->pPcsRgn[k][i].left = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
				m_pReelMap->pPcsRgn[k][i].top = fData2;
				m_pReelMap->pPcsRgn[k][i].right = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
				m_pReelMap->pPcsRgn[k][i].bottom = fData4;

				if (m_pReelMapDisp)
				{
					m_pReelMapDisp->pPcsRgn[k][i].left = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
					m_pReelMapDisp->pPcsRgn[k][i].top = fData2;
					m_pReelMapDisp->pPcsRgn[k][i].right = (m_Master[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
					m_pReelMapDisp->pPcsRgn[k][i].bottom = fData4;
				}
			}
			break;
		case ROT_CCW_90: // right->bottom, top->left, bottom->right, left->top ; Dir (x *= 1, y *= -1) 
			fDistX = 0;
			fDistY = m_Master[0].m_pPcsRgn->rtFrm.left + m_Master[0].m_pPcsRgn->rtFrm.right;
			m_pReelMap->pFrmRgn[k].left = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.top;
			m_pReelMap->pFrmRgn[k].top = fDistY - m_Master[0].m_pPcsRgn->rtFrm.right;
			m_pReelMap->pFrmRgn[k].right = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.bottom;
			m_pReelMap->pFrmRgn[k].bottom = fDistY - m_Master[0].m_pPcsRgn->rtFrm.left;

			if (m_pReelMapDisp)
			{
				m_pReelMapDisp->pFrmRgn[k].left = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.top;
				m_pReelMapDisp->pFrmRgn[k].top = fDistY - m_Master[0].m_pPcsRgn->rtFrm.right;
				m_pReelMapDisp->pFrmRgn[k].right = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_Master[0].m_pPcsRgn->rtFrm.bottom;
				m_pReelMapDisp->pFrmRgn[k].bottom = fDistY - m_Master[0].m_pPcsRgn->rtFrm.left;
			}

			for (i = 0; i < nTotPcs; i++)
			{
				fData1 = m_Master[0].m_pPcsRgn->pPcs[i].top;	// left
				fData2 = fDistY - m_Master[0].m_pPcsRgn->pPcs[i].right;	// top
				fData3 = m_Master[0].m_pPcsRgn->pPcs[i].bottom;	// right
				fData4 = fDistY - m_Master[0].m_pPcsRgn->pPcs[i].left; // bottom

				m_pReelMap->pPcsRgn[k][i].left = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
				m_pReelMap->pPcsRgn[k][i].top = fData2;
				m_pReelMap->pPcsRgn[k][i].right = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
				m_pReelMap->pPcsRgn[k][i].bottom = fData4;

				if (m_pReelMapDisp)
				{
					m_pReelMapDisp->pPcsRgn[k][i].left = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
					m_pReelMapDisp->pPcsRgn[k][i].top = fData2;
					m_pReelMapDisp->pPcsRgn[k][i].right = (m_Master[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
					m_pReelMapDisp->pPcsRgn[k][i].bottom = fData4;
				}
			}
			break;
		}
	}
}

CString CGvisR2R_PunchDoc::GetCamPxlRes()
{
	CString sRes = _T("");
	CString sPath;
#ifdef TEST_MODE
	sPath = PATH_PIN_IMG_;
#else
	if (pDoc->WorkingInfo.System.sPathCamSpecDir.Right(1) != "\\")
		sPath.Format(_T("%s\\%s\\%s.mst"), pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sLayerUp);
	else
		sPath.Format(_T("%s%s\\%s.mst"), pDoc->WorkingInfo.System.sPathCamSpecDir, pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sLayerUp);
#endif

	int nPos = sPath.ReverseFind('-');
	if (nPos > 0)
	{
		sRes = sPath.Right(sPath.GetLength() - (nPos + 1));
		nPos = sRes.ReverseFind('.');
		if (nPos > 0)
			sRes = sRes.Left(nPos);
		WorkingInfo.Vision[0].sCamPxlRes = sRes; // CamMaster Pixel Resolution.
		WorkingInfo.Vision[1].sCamPxlRes = sRes; // CamMaster Pixel Resolution.
	}
	return sRes;
}

int CGvisR2R_PunchDoc::GetPcrIdx(int nSerial, BOOL bNewLot)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.8"));
		return 0;
	}

	int nIdx = (nSerial - 1) % MAX_PCR_PNL;

	return nIdx;
}

int CGvisR2R_PunchDoc::GetPcrIdx0(int nSerial, BOOL bNewLot)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.9"));
		return 0;
	}

	int nIdx;

	if (!bNewLot)
	{
		if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
		{
			if (!m_bDoneChgLot && nSerial <= m_nLotLastShot)
				nIdx = (nSerial - 1) % MAX_PCR_PNL;
			else // New Lot...
				nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
		}
		else
		{
			nIdx = (nSerial - 1) % MAX_PCR_PNL;
		}
	}
	else
	{
		if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
			nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
		else
			nIdx = (nSerial - 1) % MAX_PCR_PNL;
	}
	return nIdx;
}

int CGvisR2R_PunchDoc::GetPcrIdx1(int nSerial, BOOL bNewLot)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.10"));
		return 0;
	}

	int nIdx;

	if (!bNewLot)
	{
		if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
		{
			if (!m_bDoneChgLot && nSerial <= m_nLotLastShot)
				nIdx = (nSerial - 1) % MAX_PCR_PNL;
			else // New Lot...
				nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
		}
		else
		{
			nIdx = (nSerial - 1) % MAX_PCR_PNL;
		}
	}
	else
	{
		if (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot)
			nIdx = (m_nLotLastShot + nSerial - 1) % MAX_PCR_PNL;
		else
			nIdx = (nSerial - 1) % MAX_PCR_PNL;
	}
	return nIdx;
}

BOOL CGvisR2R_PunchDoc::GetAoiUpInfo(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.11"));
		return 0;
	}

	BOOL Info0;//, Info1;
	Info0 = GetAoiInfoUp(nSerial, pNewLot, bFromBuf);

	if (Info0)
		return TRUE;

	return FALSE;
}

BOOL CGvisR2R_PunchDoc::GetAoiDnInfo(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.12"));
		return 0;
	}

	BOOL Info1;//Info0, 
	Info1 = GetAoiInfoDn(nSerial, pNewLot, bFromBuf);
	if (Info1)
		return TRUE;
	return FALSE;
}

BOOL CGvisR2R_PunchDoc::GetAoiInfoUp(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	*pNewLot = 0;

	if (nSerial < 1)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufUp, nSerial);
#endif

	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData = CharToString(FileData);
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[0].sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[0].sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[0].sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[0].sItsCode = sItsCode;

	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);



	if (Status.PcrShare[0].sModel.IsEmpty() || Status.PcrShare[0].sLayer.IsEmpty() || Status.PcrShare[0].sLot.IsEmpty())
	{
		pView->MsgBox(_T("Error - Aoi Information."));
		return FALSE;
	}

	BOOL bUpdate = FALSE;

	if (WorkingInfo.LastJob.sLot != Status.PcrShare[0].sLot || WorkingInfo.LastJob.sEngItsCode != Status.PcrShare[0].sItsCode)
	{
		bUpdate = TRUE;
		WorkingInfo.LastJob.sLot = Status.PcrShare[0].sLot;
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = Status.PcrShare[0].sItsCode;
		*pNewLot = 1;
	}

	if (WorkingInfo.LastJob.sModel != Status.PcrShare[0].sModel || WorkingInfo.LastJob.sLayerUp != Status.PcrShare[0].sLayer || pView->m_bInitAutoLoadMstInfo)
	{
		bUpdate = TRUE;
		WorkingInfo.LastJob.sModel = Status.PcrShare[0].sModel;
		WorkingInfo.LastJob.sLayerUp = Status.PcrShare[0].sLayer;
		*pNewLot = 1;
		if (pView->m_bInitAutoLoadMstInfo)
		{
			pView->m_bInitAutoLoadMstInfo = FALSE; pDoc->SetStatus(_T("General"), _T("bInitAutoLoadMstInfo"), pView->m_bInitAutoLoadMstInfo);
			return TRUE;
		}
	}

	if (m_bBufEmptyF[0])
	{
		if (!m_bBufEmpty[0]) // 상부 AOI 버퍼에 처음으로 시리얼이 들어올 때
		{
			bUpdate = TRUE;
			m_bBufEmptyF[0] = FALSE; pDoc->SetStatus(_T("General"), _T("bBufEmptyF[0]"), m_bBufEmptyF[0]);

			m_nAoiCamInfoStrPcs[0] = GetAoiUpCamMstInfo(); pDoc->SetStatusInt(_T("General"), _T("nAoiCamInfoStrPcs[0]"), m_nAoiCamInfoStrPcs[0]);
			if (m_nAoiCamInfoStrPcs[0] > -1)
			{
				if ((m_nAoiCamInfoStrPcs[0] == 1 ? TRUE : FALSE) != WorkingInfo.System.bStripPcsRgnBin)
				{
					pView->MsgBox(_T("WorkingInfo.ini 파일의 StripPcsRgnBin정보와 상부 AOI CamMst정보가 다릅니다."));
					return FALSE;
				}
			}

			pView->m_bInitAutoLoadMstInfo = FALSE; pDoc->SetStatus(_T("General"), _T("bInitAutoLoadMstInfo"), pView->m_bInitAutoLoadMstInfo);
			//return TRUE;
		}
	}

	if (bUpdate)
	{
		WriteChangedModel();

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->UpdateData();

		if (m_pReelMapUp)
			m_pReelMapUp->ResetReelmapPath();

		if (GetTestMode() == MODE_OUTER)
		{
			BOOL bDualTestInner;
			CString sLot, sLayerUp, sLayerDn, str;
			if (!pDoc->GetItsSerialInfo(nSerial, bDualTestInner, sLot, sLayerUp, sLayerDn))
			{
				str.Format(_T("It is trouble to read GetItsSerialInfo()."));
				pView->MsgBox(str);
				return FALSE; // TRUE: CHANGED, FALSE: NO CHANGED 
			}

			if(m_pReelMapInnerUp)
				m_pReelMapInnerUp->ResetReelmapPath();

			if (bDualTestInner)
			{
				if (m_pReelMapInnerDn)
					m_pReelMapInnerDn->ResetReelmapPath();
				if (m_pReelMapInnerAllUp)
					m_pReelMapInnerAllUp->ResetReelmapPath();
				if (m_pReelMapInnerAllDn)
					m_pReelMapInnerAllDn->ResetReelmapPath();
			}
		}
	}

	return FALSE; // TRUE: CHANGED, FALSE: NO CHANGED 
}

BOOL CGvisR2R_PunchDoc::GetAoiInfoDn(int nSerial, int *pNewLot, BOOL bFromBuf) // TRUE: CHANGED, FALSE: NO CHANGED 
{
	*pNewLot = 0;

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return TRUE;

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp;// , i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial < 1)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufDn, nSerial);
#endif

	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData.Format(_T("%s"), CharToString(FileData));
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[1].sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[1].sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[1].sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	Status.PcrShare[1].sItsCode = sItsCode;

	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);



	if (Status.PcrShare[1].sModel.IsEmpty() || Status.PcrShare[1].sLayer.IsEmpty() || Status.PcrShare[1].sLot.IsEmpty())
	{
		pView->MsgBox(_T("Error - Aoi Information."));
		return FALSE;
	}

	BOOL bUpdate = FALSE;

	if (WorkingInfo.LastJob.sLot != Status.PcrShare[1].sLot || WorkingInfo.LastJob.sEngItsCode != Status.PcrShare[1].sItsCode)
	{
		bUpdate = TRUE;
		WorkingInfo.LastJob.sLot = Status.PcrShare[1].sLot;
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = Status.PcrShare[1].sItsCode;
		*pNewLot = 1;
	}

	if (WorkingInfo.LastJob.sModel != Status.PcrShare[1].sModel || WorkingInfo.LastJob.sLayerDn != Status.PcrShare[1].sLayer)
	{
		bUpdate = TRUE;
		WorkingInfo.LastJob.sModel = Status.PcrShare[1].sModel;
		WorkingInfo.LastJob.sLayerDn = Status.PcrShare[1].sLayer;
		*pNewLot = 1;
	}

	if (m_bBufEmptyF[1])
	{
		if (!m_bBufEmpty[1]) // 하부 AOI 버퍼에 처음으로 시리얼이 들어올 때
		{
			bUpdate = TRUE;
			m_bBufEmptyF[1] = FALSE; pDoc->SetStatus(_T("General"), _T("bBufEmptyF[1]"), m_bBufEmptyF[1]);

			m_nAoiCamInfoStrPcs[1] = GetAoiDnCamMstInfo(); pDoc->SetStatusInt(_T("General"), _T("nAoiCamInfoStrPcs[1]"), m_nAoiCamInfoStrPcs[1]);
			if (m_nAoiCamInfoStrPcs[1] > -1)
			{
				if ((m_nAoiCamInfoStrPcs[1] == 1 ? TRUE : FALSE) != WorkingInfo.System.bStripPcsRgnBin)
				{
					pView->MsgBox(_T("WorkingInfo.ini 파일의 StripPcsRgnBin정보와 하부 AOI CamMst정보가 다릅니다."));
					return FALSE;
				}
			}
			//return TRUE;
		}
	}

	if (bUpdate)
	{
		WriteChangedModel();

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->UpdateData();

		if (m_pReelMapDn)
			m_pReelMapDn->ResetReelmapPath();

		if (m_pReelMapAllUp)
			m_pReelMapAllUp->ResetReelmapPath();

		if (m_pReelMapAllDn)
			m_pReelMapAllDn->ResetReelmapPath();
	}

	return FALSE;
}

int CGvisR2R_PunchDoc::LoadPCR0(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = WorkingInfo.LastJob.bDualTest;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.14"));
		return 0;
	}

	int nRtn[4] = { 1,1,1,1 };
	nRtn[0] = LoadPCRUp(nSerial, bFromShare);
	if (nRtn[0] != 1)
		return nRtn[0];
	if (pDoc->m_bVsStatusUp)
	{
		if (pDoc->m_ListBuf[0].nTot < pDoc->m_ListBuf[1].nTot)
		{
			nRtn[1] = LoadPCRAllDn(nSerial, bFromShare);
			if (nRtn[1] != 1)
				return nRtn[1];

			nRtn[2] = LoadPCRAllUp(nSerial, bFromShare);
			if (nRtn[2] != 1)
				return nRtn[2];

			if (GetTestMode() == MODE_OUTER)
			{
				nRtn[3] = LoadPCRIts(nSerial, bFromShare);
				if (nRtn[3] != 1)
					return nRtn[3];
			}
		}
		else
		{
			if (GetTestMode() == MODE_OUTER)
			{
				int nRtnInner[5] = { 1,1,1,1,1 };
				nRtnInner[0] = LoadPCRUpInner(nSerial, bFromShare);
				nRtnInner[1] = LoadPCRDnInner(nSerial, bFromShare);
				nRtnInner[2] = LoadPCRAllUpInner(nSerial, bFromShare);
				nRtnInner[3] = LoadPCRAllDnInner(nSerial, bFromShare);

				if (nRtnInner[0] != 1)
					return nRtnInner[0];
				if (nRtnInner[1] != 1)
					return nRtnInner[1];
				if (nRtnInner[2] != 1)
					return nRtnInner[2];
				if (nRtnInner[3] != 1)
					return nRtnInner[3];

				if (!bDualTest)
				{
					nRtnInner[4] = LoadPCRIts(nSerial, bFromShare);
					if (nRtnInner[4] != 1)
						return nRtnInner[4];
				}
			}
		}
	}
	else
	{
		if (GetTestMode() == MODE_OUTER)
		{
			int nRtnInner[5] = { 1,1,1,1,1 };

			nRtnInner[0] = LoadPCRUpInner(nSerial, bFromShare);
			if (nRtnInner[0] != 1)
				return nRtnInner[0];

			nRtnInner[1] = LoadPCRDnInner(nSerial, bFromShare);
			if (nRtnInner[1] != 1)
				return nRtnInner[1];

			nRtnInner[2] = LoadPCRAllUpInner(nSerial, bFromShare);
			if (nRtnInner[2] != 1)
				return nRtnInner[2];

			nRtnInner[3] = LoadPCRAllDnInner(nSerial, bFromShare);
			if (nRtnInner[3] != 1)
				return nRtnInner[3];

			if (!bDualTest)
			{
				nRtnInner[4] = LoadPCRIts(nSerial, bFromShare);
				if (nRtnInner[4] != 1)
					return nRtnInner[4];
			}
		}
	}

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCR1(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.15"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	int nRtn[4] = { 1,1,1,1 };
	nRtn[0] = LoadPCRDn(nSerial, bFromShare);
	if (nRtn[0] != 1)
		return nRtn[0];

	if (pDoc->m_bVsStatusDn)
	{
		if (pDoc->m_ListBuf[1].nTot < pDoc->m_ListBuf[0].nTot)
		{
			nRtn[1] = LoadPCRAllDn(nSerial, bFromShare);
			if (nRtn[1] != 1)
				return nRtn[1];

			nRtn[2] = LoadPCRAllUp(nSerial, bFromShare);
			if (nRtn[2] != 1)
				return nRtn[2];

			if (GetTestMode() == MODE_OUTER)
			{
				nRtn[3] = LoadPCRIts(nSerial, bFromShare);
				if (nRtn[3] != 1)
					return nRtn[3];
			}
		}
		else
		{
			if (GetTestMode() == MODE_OUTER)
			{
				int nRtnInner[5] = { 1,1,1,1,1 };
				nRtnInner[0] = LoadPCRUpInner(nSerial, bFromShare);
				nRtnInner[1] = LoadPCRDnInner(nSerial, bFromShare);
				nRtnInner[2] = LoadPCRAllUpInner(nSerial, bFromShare);
				nRtnInner[3] = LoadPCRAllDnInner(nSerial, bFromShare);

				if (nRtnInner[0] != 1)
					return nRtnInner[0];
				if (nRtnInner[1] != 1)
					return nRtnInner[1];
				if (nRtnInner[2] != 1)
					return nRtnInner[2];
				if (nRtnInner[3] != 1)
					return nRtnInner[3];
			}
		}
	}
	else
	{
		nRtn[1] = LoadPCRAllDn(nSerial, bFromShare);
		if (nRtn[1] != 1)
			return nRtn[1];

		nRtn[2] = LoadPCRAllUp(nSerial, bFromShare);
		if (nRtn[2] != 1)
			return nRtn[2];

		if (GetTestMode() == MODE_OUTER)
		{
			nRtn[3] = LoadPCRIts(nSerial, bFromShare);
			if (nRtn[3] != 1)
				return nRtn[3];
		}
	}

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCRAllUp(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
#ifdef TEST_MODE
	return 0;
#endif

	BOOL bDualTest = WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.16"));
		return 0;
	}

	int i, idx;//, k
	CString str;

	if (nSerial < 0)
	{
		str.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(str);
		return(2);
	}

	if (!m_pPcr[2])
	{
		str.Format(_T("PCR[2]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(str);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[0] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx0(nSerial, TRUE); // 릴맵화면 표시 인덱스
	else
		nIdx = GetPcrIdx0(nSerial);

	if (!m_pPcr[0] || !m_pPcr[1] || !m_pPcr[2])
		return(2);
	if (!m_pPcr[0][nIdx] || !m_pPcr[1][nIdx] || !m_pPcr[2][nIdx])
		return(2);

	m_pPcr[2][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[2][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcr[0][nIdx]->m_nErrPnl < 0)
		m_pPcr[2][nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;
	else if (m_pPcr[1][nIdx]->m_nErrPnl < 0)
		m_pPcr[2][nIdx]->m_nErrPnl = m_pPcr[1][nIdx]->m_nErrPnl;
	else
		m_pPcr[2][nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;

	// Model
	m_pPcr[2][nIdx]->m_sModel = m_pPcr[0][nIdx]->m_sModel;

	// Layer
	m_pPcr[2][nIdx]->m_sLayer = m_pPcr[0][nIdx]->m_sLayer;

	// Lot
	m_pPcr[2][nIdx]->m_sLot = m_pPcr[0][nIdx]->m_sLot;

	int nTotDef[3] = { 0 };									// [0]: 상면, [1]: 하면, [2]: 상/하면 Merge
	nTotDef[0] = m_pPcr[0][nIdx]->m_nTotDef;			// 상면 불량 피스 수
	nTotDef[1] = m_pPcr[1][nIdx]->m_nTotDef;			// 하면 불량 피스 수

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow;
	//int nTotPcsInner = m_MasterInner[0].m_pPcsRgn->m_nTotPcs;
	//int nNodeXInner = m_MasterInner[0].m_pPcsRgn->m_nCol;
	//int nNodeYInner = m_MasterInner[0].m_pPcsRgn->m_nRow;
	//if (nTotPcs < 0 || nTotPcsInner < 0 || nTotPcsInner != nTotPcs || nNodeXInner != nNodeX || nNodeYInner != nNodeY)
	if (nTotPcs < 0)
	{
		str.Format(_T("It is trouble to run LoadPCRAllUp()."));
		pView->MsgBox(str);
		return 0;
	}
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs + 1];

	int nPcsId;
	int nTotVerifyed[2] = { 0 };												// nPcsId : CamMaster Pcs Index
															//pPcrMgr테이블의 nIdx에 하면의 불량을 먼저 기록하고 상면의 불량을 엎어서 최종 merge불량 테이블을 만듬.
	for (i = 0; i < nTotDef[1]; i++)						// 하면 불량 피스 수
	{
		if (m_pPcr[1][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcr[1][nIdx]->m_pDefPcs[i];			// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdx = i;	// Up+Dn			// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			pPcrMgr[nPcsId].nIdxDn = i; // Dn				// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[1]++;
		}
	}
	for (i = 0; i < nTotDef[0]; i++)						// 상면 불량 피스 수
	{
		if (m_pPcr[0][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcr[0][nIdx]->m_pDefPcs[i];			// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdx = i;	// Up+Dn			// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			pPcrMgr[nPcsId].nIdxUp = i;	// Up				// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[0]++;
		}
	}


	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)							// Shot내 총 Piece수
	{
		if (pPcrMgr[i].nIdx > -1)	// Up+Dn				// 상하면 Merge [i:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			nTotDef[2]++;									// 상 / 하면 Merge한 총 불량피스수.
	}

	m_pPcr[2][nIdx]->Init(nSerial, nTotDef[2]);				// 제품시리얼, Shot내 총불량 피스수

	int nId[2], Ord; // [0]: 상면 0~불량피스순서, [1]: 하면 0~불량피스순서
	idx = 0; // 마킹순서 0~불량피스수만큼 정하기위해 현시점의 idx를 초기화함.
	if (nTotDef[2] > 0) // 상 / 하면 Merge한 총 불량피스수.
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp; // 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서 임. nId[0]: 상면에서의 PCR파일순서 인덱스
			nId[1] = pPcrMgr[nPcsId].nIdxDn; // 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서 임. nId[1]: 하면에서의 PCR파일순서 인덱스

			if (nId[0] > -1)
			{
				Ord = nId[0];
				// Cam ID
				m_pPcr[2][nIdx]->m_nCamId = m_pPcr[0][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[2][nIdx]->m_pDefPcs[idx] = m_pPcr[0][nIdx]->m_pDefPcs[Ord];
				m_pPcr[2][nIdx]->m_pLayer[idx] = m_pPcr[0][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcr[2][nIdx]->m_pDefPos[idx].x = m_pPcr[0][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcr[2][nIdx]->m_pDefPos[idx].y = m_pPcr[0][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcr[2][nIdx]->m_pDefType[idx] = m_pPcr[0][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcr[2][nIdx]->m_pCell[idx] = m_pPcr[0][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcr[2][nIdx]->m_pImgSz[idx] = m_pPcr[0][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcr[2][nIdx]->m_pImg[idx] = m_pPcr[0][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[2][nIdx]->m_pMk[idx] = m_pPcr[0][nIdx]->m_pMk[Ord];

				idx++;
			}
			else if (nId[1] > -1)
			{
				Ord = nId[1];
				// Cam ID
				m_pPcr[2][nIdx]->m_nCamId = m_pPcr[1][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[2][nIdx]->m_pDefPcs[idx] = m_pPcr[1][nIdx]->m_pDefPcs[Ord];
				m_pPcr[2][nIdx]->m_pLayer[idx] = m_pPcr[1][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcr[2][nIdx]->m_pDefPos[idx].x = m_pPcr[1][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcr[2][nIdx]->m_pDefPos[idx].y = m_pPcr[1][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcr[2][nIdx]->m_pDefType[idx] = m_pPcr[1][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcr[2][nIdx]->m_pCell[idx] = m_pPcr[1][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcr[2][nIdx]->m_pImgSz[idx] = m_pPcr[1][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcr[2][nIdx]->m_pImg[idx] = m_pPcr[1][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[2][nIdx]->m_pMk[idx] = m_pPcr[1][nIdx]->m_pMk[Ord];

				idx++;
			}
		}
	}

	delete[] pPcrMgr;

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCRAllDn(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	int i, idx;//k, 
	CString str;

	if (nSerial <= 0)
	{
		str.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(str);
		return(2);
	}

	if (!m_pPcr[3])
	{
		str.Format(_T("PCR[3]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(str);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[1] && (pDoc->WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx1(nSerial, TRUE);
	else
		nIdx = GetPcrIdx1(nSerial);

	if (!m_pPcr[0] || !m_pPcr[1] || !m_pPcr[3])
		return(2);
	if (!m_pPcr[0][nIdx] || !m_pPcr[1][nIdx] || !m_pPcr[3][nIdx])
		return(2);

	m_pPcr[3][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[3][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcr[1][nIdx]->m_nErrPnl < 0)
		m_pPcr[3][nIdx]->m_nErrPnl = m_pPcr[1][nIdx]->m_nErrPnl;
	else if (m_pPcr[0][nIdx]->m_nErrPnl < 0)
		m_pPcr[3][nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;
	else
		m_pPcr[3][nIdx]->m_nErrPnl = m_pPcr[1][nIdx]->m_nErrPnl;

	// Model
	m_pPcr[3][nIdx]->m_sModel = m_pPcr[1][nIdx]->m_sModel;

	// Layer
	m_pPcr[3][nIdx]->m_sLayer = m_pPcr[1][nIdx]->m_sLayer;

	// Lot
	m_pPcr[3][nIdx]->m_sLot = m_pPcr[1][nIdx]->m_sLot;

	int nTotDef[3];
	nTotDef[0] = m_pPcr[0][nIdx]->m_nTotDef;
	nTotDef[1] = m_pPcr[1][nIdx]->m_nTotDef;

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow;
	//int nTotPcsInner = m_MasterInner[0].m_pPcsRgn->m_nTotPcs;
	//int nNodeXInner = m_MasterInner[0].m_pPcsRgn->m_nCol;
	//int nNodeYInner = m_MasterInner[0].m_pPcsRgn->m_nRow;
	//if (nTotPcs < 0 || nTotPcsInner < 0 || nTotPcsInner != nTotPcs || nNodeXInner != nNodeX || nNodeYInner != nNodeY)
	if (nTotPcs < 0)
	{
		str.Format(_T("It is trouble to run LoadPCRAllDn()."));
		pView->MsgBox(str);
		return 0;
	}
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs + 1];

	int nComp, nPcsId;
	if (nTotDef[0] > nTotDef[1])
		nComp = nTotDef[0];
	else
		nComp = nTotDef[1];

	int nTotVerifyed[2] = { 0 };												// nPcsId : CamMaster Pcs Index

	for (i = 0; i < nTotDef[0]; i++)
	{
		if (m_pPcr[0][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcr[0][nIdx]->m_pDefPcs[i];
			pPcrMgr[nPcsId].nIdx = i;
			pPcrMgr[nPcsId].nIdxUp = i;	// Up
		}
		else
		{
			nTotVerifyed[0]++;
		}
	}
	for (i = 0; i < nTotDef[1]; i++)
	{
		if (m_pPcr[1][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcr[1][nIdx]->m_pDefPcs[i];
			pPcrMgr[nPcsId].nIdx = i;
			pPcrMgr[nPcsId].nIdxDn = i; // Dn
		}
		else
		{
			nTotVerifyed[1]++;
		}
	}

	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)
	{
		if (pPcrMgr[i].nIdx > -1)
			nTotDef[2]++;
	}

	m_pPcr[3][nIdx]->Init(nSerial, nTotDef[2]);

	int nId[2], Ord;
	idx = 0;
	if (nTotDef[2] > 0)
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp;
			nId[1] = pPcrMgr[nPcsId].nIdxDn;

			if (nId[1] > -1)
			{
				Ord = nId[1];
				// Cam ID
				m_pPcr[3][nIdx]->m_nCamId = m_pPcr[1][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[3][nIdx]->m_pDefPcs[idx] = m_pPcr[1][nIdx]->m_pDefPcs[Ord];
				m_pPcr[3][nIdx]->m_pLayer[idx] = m_pPcr[1][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcr[3][nIdx]->m_pDefPos[idx].x = m_pPcr[1][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcr[3][nIdx]->m_pDefPos[idx].y = m_pPcr[1][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcr[3][nIdx]->m_pDefType[idx] = m_pPcr[1][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcr[3][nIdx]->m_pCell[idx] = m_pPcr[1][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcr[3][nIdx]->m_pImgSz[idx] = m_pPcr[1][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcr[3][nIdx]->m_pImg[idx] = m_pPcr[1][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[3][nIdx]->m_pMk[idx] = m_pPcr[1][nIdx]->m_pMk[Ord];

				idx++;
			}
			else if (nId[0] > -1)
			{
				Ord = nId[0];
				// Cam ID
				m_pPcr[3][nIdx]->m_nCamId = m_pPcr[0][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[3][nIdx]->m_pDefPcs[idx] = m_pPcr[0][nIdx]->m_pDefPcs[Ord];
				m_pPcr[3][nIdx]->m_pLayer[idx] = m_pPcr[0][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcr[3][nIdx]->m_pDefPos[idx].x = m_pPcr[0][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcr[3][nIdx]->m_pDefPos[idx].y = m_pPcr[0][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcr[3][nIdx]->m_pDefType[idx] = m_pPcr[0][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcr[3][nIdx]->m_pCell[idx] = m_pPcr[0][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcr[3][nIdx]->m_pImgSz[idx] = m_pPcr[0][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcr[3][nIdx]->m_pImg[idx] = m_pPcr[0][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[3][nIdx]->m_pMk[idx] = m_pPcr[0][nIdx]->m_pMk[Ord];

				idx++;
			}
		}
	}

	delete[] pPcrMgr;

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCRUp(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp, i, nC, nR;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial <= 0)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcr[0])
	{
		strFileData.Format(_T("PCR[0]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[0] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx0(nSerial, TRUE);
	else
		nIdx = GetPcrIdx0(nSerial);

	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	if (bFromShare)
		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsShareUp, nSerial);
	else
		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufUp, nSerial);
#endif

	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData = CharToString(FileData);
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR[Up] 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcr[0]) return(2);
	if (!m_pPcr[0][nIdx]) return(2);

	BOOL bResetMkInfo = FALSE;

	m_pPcr[0][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[0][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sItsCode = sItsCode;

	// Total Defect Numbers
	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
	{
		pDoc->m_sEngModel = strModel;
		pDoc->m_sEngLotNum = strLot;
		pDoc->m_sEngLayerUp = strLayer;
		if (WorkingInfo.System.bUseITS)
			pDoc->m_sItsCode = sItsCode;
	}

	BOOL bUpdate = FALSE;

	if (WorkingInfo.LastJob.sLot != strLot || WorkingInfo.LastJob.sEngItsCode != sItsCode)
	{
		if (WorkingInfo.LastJob.sLot != strLot)
		{
			WorkingInfo.LastJob.sLot = strLot;
			if(pView->m_pEngrave)
				pView->m_pEngrave->SetLotName();
		}
		bUpdate = TRUE;
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = sItsCode;
	}

	if (WorkingInfo.LastJob.sModel != strModel || WorkingInfo.LastJob.sLayerUp != strLayer)
	{
		if (WorkingInfo.LastJob.sModel != strModel)
		{
			WorkingInfo.LastJob.sModel = strModel;
			if (pView->m_pEngrave)
				pView->m_pEngrave->SetModelName();
		}
		if (WorkingInfo.LastJob.sLayerUp != strLayer)
		{
			WorkingInfo.LastJob.sLayerUp = strLayer;
			if (pView->m_pEngrave)
				pView->m_pEngrave->SetLayerUpName();
		}

		bUpdate = TRUE;
		pView->ResetMkInfo(0); // CAD 데이터 리로딩   0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
	}

	if (bUpdate)
	{
		WriteChangedModel();

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->UpdateData();

		if (GetTestMode() == MODE_OUTER)
		{
			BOOL bDualTestInner;
			CString sLot, sLayerUp, sLayerDn, str;
			if (!pDoc->GetItsSerialInfo(nSerial, bDualTestInner, sLot, sLayerUp, sLayerDn))
			{
				str.Format(_T("It is trouble to read GetItsSerialInfo()."));
				pView->MsgBox(str);
				return FALSE; // TRUE: CHANGED, FALSE: NO CHANGED 
			}

			if (m_pReelMapInnerUp)
				m_pReelMapInnerUp->ResetReelmapPath();

			if (m_pReelMapIts)
				m_pReelMapIts->ResetReelmapPath();

			if (bDualTestInner)
			{
				if (m_pReelMapInnerDn)
					m_pReelMapInnerDn->ResetReelmapPath();
				if (m_pReelMapInnerAllUp)
					m_pReelMapInnerAllUp->ResetReelmapPath();
				if (m_pReelMapInnerAllDn)
					m_pReelMapInnerAllDn->ResetReelmapPath();
			}
		}
	}

	int nTotDef = _tstoi(strTotalBadPieceNum);

	m_pPcr[0][nIdx]->Init(nSerial, nTotDef);

	if (nTotDef > 0)
	{
		for (i = 0; i < nTotDef; i++)
		{
			// Cam ID
			nTemp = strFileData.Find(',', 0);
			strCamID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_nCamId = _tstoi(strCamID);

			// Piece Number
			nTemp = strFileData.Find(',', 0);
			strPieceID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;

			// LoadStripPieceRegion_Binary()에 의해 PCS Index가 결정됨.
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
			{
				m_pPcr[0][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);
			}
			else
				m_pPcr[0][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);

			m_pPcr[0][nIdx]->m_pLayer[i] = 0; // Up

			// BadPointPosX
			nTemp = strFileData.Find(',', 0);
			strBadPointPosX = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pDefPos[i].x = (long)_tstoi(strBadPointPosX);

			// BadPointPosY
			nTemp = strFileData.Find(',', 0);
			strBadPointPosY = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pDefPos[i].y = (long)_tstoi(strBadPointPosY);

			// BadName
			nTemp = strFileData.Find(',', 0);
			strBadName = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pDefType[i] = _tstoi(strBadName);

			pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[0].MasterInfo.nActionCode, m_pPcr[0][nIdx]->m_pDefPcs[i], nC, nR);
			m_pPcr[0][nIdx]->m_arDefType[nR][nC] = m_pPcr[0][nIdx]->m_pDefType[i];
			m_pPcr[0][nIdx]->m_arPcrLineNum[nR][nC] = i;
			m_pPcr[0][nIdx]->m_arDefTypeForIts[nR][nC] = m_pPcr[0][nIdx]->m_pDefType[i];
			m_pPcr[0][nIdx]->m_arPcrLineNumForIts[nR][nC] = i;

			// CellNum
			nTemp = strFileData.Find(',', 0);
			strCellNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pCell[i] = _tstoi(strCellNum);

			// ImageSize
			nTemp = strFileData.Find(',', 0);
			strImageSize = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pImgSz[i] = _tstoi(strImageSize);

			// ImageNum
			nTemp = strFileData.Find(',', 0);
			strImageNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pImg[i] = _tstoi(strImageNum);

			// strMarkingCode : -2 (NoMarking)
			nTemp = strFileData.Find('\n', 0);
			strMarkingCode = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pMk[i] = _tstoi(strMarkingCode);
		}
	}

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCRDn(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp, i, nC, nR;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial <= 0)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcr[1])
	{
		strFileData.Format(_T("PCR[1]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[1] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx1(nSerial, TRUE);
	else
		nIdx = GetPcrIdx1(nSerial);
	CString sPath;

#ifdef TEST_MODE
	sPath = PATH_PCR;	// for Test
#else
	if (bFromShare)
		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsShareDn, nSerial);
	else
		sPath.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufDn, nSerial);
#endif

	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData = CharToString(FileData);
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR[Dn] 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcr[1]) return(2);
	if (!m_pPcr[1][nIdx]) return(2);

	m_pPcr[1][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[1][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sItsCode = sItsCode;

	// Total Defect Numbers
	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;


	if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
	{
		pDoc->m_sEngModel = strModel;
		pDoc->m_sEngLotNum = strLot;
		pDoc->m_sEngLayerDn = strLayer;
		if (WorkingInfo.System.bUseITS)
			pDoc->m_sItsCode = sItsCode;
	}

	BOOL bUpdate = FALSE;

	if (WorkingInfo.LastJob.sLot != strLot || WorkingInfo.LastJob.sEngItsCode != sItsCode)
	{
		if (WorkingInfo.LastJob.sLot != strLot)
		{
			WorkingInfo.LastJob.sLot = strLot;
			if (pView->m_pEngrave)
				pView->m_pEngrave->SetLotName();
		}
		bUpdate = TRUE;
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = sItsCode;
	}

	if (WorkingInfo.LastJob.sModel != strModel || WorkingInfo.LastJob.sLayerDn != strLayer)
	{
		if (WorkingInfo.LastJob.sModel != strModel)
		{
			WorkingInfo.LastJob.sModel = strModel;
			if (pView->m_pEngrave)
				pView->m_pEngrave->SetModelName();
		}
		if (WorkingInfo.LastJob.sLayerDn != strLayer)
		{
			WorkingInfo.LastJob.sLayerDn = strLayer;
			if (pView->m_pEngrave)
				pView->m_pEngrave->SetLayerDnName();
		}
		bUpdate = TRUE;
	}

	if (bUpdate)
	{
		WriteChangedModel();

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->UpdateData();
	}

	int nTotDef = _tstoi(strTotalBadPieceNum);

	m_pPcr[1][nIdx]->Init(nSerial, nTotDef);

	if (nTotDef > 0)
	{
		for (i = 0; i < nTotDef; i++)
		{
			// Cam ID
			nTemp = strFileData.Find(',', 0);
			strCamID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_nCamId = _tstoi(strCamID);

			// Piece Number
			nTemp = strFileData.Find(',', 0);
			strPieceID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;

			// LoadStripPieceRegion_Binary()에 의해 PCS Index가 결정됨.
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
			{
				m_pPcr[1][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);
			}
			else
				m_pPcr[1][nIdx]->m_pDefPcs[i] = MirrorLR(_tstoi(strPieceID));	// 초기 양면검사기용

			m_pPcr[1][nIdx]->m_pLayer[i] = 1; // Dn

			// BadPointPosX
			nTemp = strFileData.Find(',', 0);
			strBadPointPosX = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pDefPos[i].x = (long)_tstoi(strBadPointPosX);

			// BadPointPosY
			nTemp = strFileData.Find(',', 0);
			strBadPointPosY = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pDefPos[i].y = (long)_tstoi(strBadPointPosY);

			// BadName
			nTemp = strFileData.Find(',', 0);
			strBadName = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pDefType[i] = _tstoi(strBadName);

			// Temp for ITS - m_pPcr[0][nIdx]->m_pDefPcs[i] = Rotate180(_tstoi(strPieceID));
			pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[1].MasterInfo.nActionCode, Rotate180(m_pPcr[1][nIdx]->m_pDefPcs[i]), nC, nR);
			m_pPcr[1][nIdx]->m_arDefType[nR][nC] = m_pPcr[1][nIdx]->m_pDefType[i];
			m_pPcr[1][nIdx]->m_arPcrLineNum[nR][nC] = i;
			pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[1].MasterInfo.nActionCode, m_pPcr[1][nIdx]->m_pDefPcs[i], nC, nR);
			m_pPcr[1][nIdx]->m_arDefTypeForIts[nR][nC] = m_pPcr[1][nIdx]->m_pDefType[i];
			m_pPcr[1][nIdx]->m_arPcrLineNumForIts[nR][nC] = i;

			// CellNum
			nTemp = strFileData.Find(',', 0);
			strCellNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pCell[i] = _tstoi(strCellNum);

			// ImageSize
			nTemp = strFileData.Find(',', 0);
			strImageSize = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pImgSz[i] = _tstoi(strImageSize);

			// ImageNum
			nTemp = strFileData.Find(',', 0);
			strImageNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pImg[i] = _tstoi(strImageNum);

			// strMarkingCode : -2 (NoMarking)
			nTemp = strFileData.Find('\n', 0);
			strMarkingCode = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pMk[i] = _tstoi(strMarkingCode);
		}
	}

	return (1); // 1(정상)
}

BOOL CGvisR2R_PunchDoc::CopyDefImg(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.17"));
		return 0;
	}

	BOOL bRtn[2];
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		bRtn[0] = CopyDefImgUp(nSerial);
		bRtn[1] = CopyDefImgDn(nSerial);

		if (!bRtn[0] || !bRtn[1])
			return FALSE;
	}
	else
	{
		bRtn[0] = CopyDefImgUp(nSerial);

		if (!bRtn[0])
			return FALSE;
	}
	return TRUE;
}

BOOL CGvisR2R_PunchDoc::CopyDefImg(int nSerial, CString sNewLot)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	BOOL bUp, bDn;
	if (bDualTest)
	{
		bUp = CopyDefImgUp(nSerial, sNewLot);
		bDn = CopyDefImgDn(nSerial, sNewLot);

		if (!bUp || !bDn)
			return FALSE;
	}
	else
	{
		bUp = CopyDefImgUp(nSerial, sNewLot);

		if (!bUp)
			return FALSE;
	}

	return TRUE;
}

void CGvisR2R_PunchDoc::MakeImageDirUp(int nSerial)
{
	CString strDefImgPathS, strDefImgPathD, strMakeFolderPath;
	CString sLot = WorkingInfo.LastJob.sLot;


	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel);
	else
		strMakeFolderPath.Format(_T("%s%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);


	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImagePos"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImagePos"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\CadImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\CadImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp,
			nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp,
			nSerial);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp,
			nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImagePos\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp,
			nSerial);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\CadImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp,
			nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\CadImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerUp,
			nSerial);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);
}


BOOL CGvisR2R_PunchDoc::CopyDefImgUp(int nSerial, CString sNewLot)
{
	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.18"));
		return 0;
	}

	CString strDefImgPathS, strDefImgPathD;// , strMakeFolderPath;
	int i;
	CFileFind finder;
	CString strTemp;
	CString sLot;

	CString strAOIImgDataPath;
	strAOIImgDataPath.Format(_T("%s\\VRSImage"), WorkingInfo.System.sPathAoiUpDefImg);

	if (sNewLot.IsEmpty())
		sLot = WorkingInfo.LastJob.sLot;
	else
		sLot = sNewLot;

	MakeImageDirUp(nSerial);

	int nIdx = GetIdxPcrBufUp(nSerial);
	if (nIdx < 0)
		return FALSE;

	int nTotDef = 0;
	if (m_pPcr[0])
	{
		if (m_pPcr[0][nIdx])
			nTotDef = m_pPcr[0][nIdx]->m_nTotDef;
	}
	if (nTotDef <= 0)
		return TRUE;

	int nErrorCnt = 0;
	for (i = 0; i < nTotDef; i++)
	{
		if (pDoc->m_pPcr[0][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
			int nDefImg = pDoc->m_pPcr[0][nIdx]->m_pImg[i];
			if (strAOIImgDataPath.Right(1) != "\\")
				strDefImgPathS.Format(_T("%s\\%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModel,
					WorkingInfo.LastJob.sLayerUp,
					sLot,
					nSerial,
					nDefImg);
			else
				strDefImgPathS.Format(_T("%s%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModel,
					WorkingInfo.LastJob.sLayerUp,
					sLot,
					nSerial,
					nDefImg);

			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					sLot,
					WorkingInfo.LastJob.sLayerUp,
					nSerial,
					nDefImg);
			else
				strDefImgPathD.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					sLot,
					WorkingInfo.LastJob.sLayerUp,
					nSerial,
					nDefImg);

			if (finder.FindFile(strDefImgPathS))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
					{
						strTemp.Format(_T("%s \r\n: Defect Image File Copy Fail"), strDefImgPathS);
						pView->MsgBox(strTemp);
						return FALSE;
					}
				}
			}
			else
			{
				Sleep(30);
				if (nErrorCnt > 10)
				{
					nErrorCnt = 0;
					strTemp.Format(_T("%s \r\n: Defect Image File Not Exist"), strDefImgPathS);
					//AfxMessageBox(strTemp);
					return TRUE;
				}
				else
				{
					nErrorCnt++;
					i--;
					continue;
				}
			}


			int nStrip = -1, nCol = -1, nRow = -1;
			int nPcrIdx = pDoc->GetPcrIdx0(nSerial);
			int nPcsIdx = pDoc->m_pPcr[0][nPcrIdx]->m_pDefPcs[i];
			int nDefCode = pDoc->m_pPcr[0][nPcrIdx]->m_pDefType[i];
			if (pDoc->m_Master[0].m_pPcsRgn)
				pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(nPcsIdx, nStrip, nCol, nRow);

			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
			{
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d\\%05d_%s_%c_%d_%d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					sLot,
					WorkingInfo.LastJob.sLayerUp,
					nSerial,
					nDefImg, pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
			}
			else
			{
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d\\%d_%05d_%s_%c_%d_%d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					sLot,
					WorkingInfo.LastJob.sLayerUp,
					nSerial, nSerial,
					nDefImg, pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
			}

			if (finder.FindFile(strDefImgPathS))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
					{
						strTemp.Format(_T("%s \r\n: Defect Image Position File Copy Fail"), strDefImgPathS);
						pView->MsgBox(strTemp);
						return FALSE;
					}
				}
			}
			else
			{
				Sleep(30);
				if (nErrorCnt > 10)
				{
					nErrorCnt = 0;
					strTemp.Format(_T("%s \r\n: Defect Image Position File Not Exist"), strDefImgPathS);
					//AfxMessageBox(strTemp);
					return TRUE;
				}
				else
				{
					nErrorCnt++;
					i--;
					continue;
				}
			}
		}
	}

	return TRUE;
}


void CGvisR2R_PunchDoc::MakeImageDirDn(int nSerial)
{
	CString strDefImgPathS, strDefImgPathD, strMakeFolderPath;
	CString sLot = WorkingInfo.LastJob.sLot;
	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel);
	else
		strMakeFolderPath.Format(_T("%s%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImagePos"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImagePos"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);


	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\CadImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\CadImage"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn,
			nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn,
			nSerial);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);

	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn,
			nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\DefImagePos\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn,
			nSerial);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);


	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strMakeFolderPath.Format(_T("%s\\%s\\%s\\%s\\CadImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn,
			nSerial);
	else
		strMakeFolderPath.Format(_T("%s%s\\%s\\%s\\CadImage\\%d"), WorkingInfo.System.sPathOldFile,
			WorkingInfo.LastJob.sModel,
			sLot,
			WorkingInfo.LastJob.sLayerDn,
			nSerial);

	if (!pDoc->DirectoryExists(strMakeFolderPath))
		CreateDirectory(strMakeFolderPath, NULL);
}

BOOL CGvisR2R_PunchDoc::CopyDefImgDn(int nSerial, CString sNewLot)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.19"));
		return 0;
	}

	CString strDefImgPathS, strDefImgPathD;// , strMakeFolderPath;
	int i;
	CFileFind finder;
	CString strTemp;
	CString sLot;

	if (sNewLot.IsEmpty())
		sLot = WorkingInfo.LastJob.sLot;
	else
		sLot = sNewLot;

	CString strAOIImgDataPath;
	strAOIImgDataPath.Format(_T("%s\\VRSImage"), WorkingInfo.System.sPathAoiDnDefImg);

	MakeImageDirDn(nSerial);

	int nIdx = GetIdxPcrBufDn(nSerial);
	if (nIdx < 0)
		return FALSE;

	int nTotDef = 0;
	if (m_pPcr[1])
	{
		if (m_pPcr[1][nIdx])
			nTotDef = m_pPcr[1][nIdx]->m_nTotDef;
	}
	if (nTotDef <= 0)
		return TRUE;

	int nErrorCnt = 0;
	for (i = 0; i < nTotDef; i++)
	{
		if (pDoc->m_pPcr[1][nIdx]->m_pMk[i] != -2) // -2 (NoMarking)
		{
			int nDefImg = pDoc->m_pPcr[1][nIdx]->m_pImg[i];

			if (strAOIImgDataPath.Right(1) != "\\")
				strDefImgPathS.Format(_T("%s\\%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModel,
					//WorkingInfo.LastJob.sModelDn,
					WorkingInfo.LastJob.sLayerDn,
					sLot,
					nSerial,
					nDefImg);
			else
				strDefImgPathS.Format(_T("%s%s\\%s\\%s\\%d\\%05d.tif"), strAOIImgDataPath,
					WorkingInfo.LastJob.sModel,
					//WorkingInfo.LastJob.sModelDn,
					WorkingInfo.LastJob.sLayerDn,
					sLot,
					nSerial,
					nDefImg);

			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					//WorkingInfo.LastJob.sModelDn,
					sLot,
					WorkingInfo.LastJob.sLayerDn,
					nSerial,
					nDefImg);
			else
				strDefImgPathD.Format(_T("%s%s\\%s\\%s\\DefImage\\%d\\%05d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					//WorkingInfo.LastJob.sModelDn,
					sLot,
					WorkingInfo.LastJob.sLayerDn,
					nSerial,
					nDefImg);

			if (finder.FindFile(strDefImgPathS))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
					{
						strTemp.Format(_T("%s \r\n: Defect Image File Copy Fail"), strDefImgPathS);
						pView->MsgBox(strTemp);
						return FALSE;
					}
				}
			}
			else
			{
				Sleep(30);
				if (nErrorCnt > 10)
				{
					nErrorCnt = 0;
					strTemp.Format(_T("%s \r\n: Defect Image File Not Exist"), strDefImgPathS);
					//AfxMessageBox(strTemp);
					return TRUE;
				}
				else
				{
					nErrorCnt++;
					i--;
					continue;
				}
			}


			int nStrip = -1, nCol = -1, nRow = -1;
			int nPcrIdx = pDoc->GetPcrIdx1(nSerial);
			int nPcsIdx = pDoc->m_pPcr[1][nPcrIdx]->m_pDefPcs[i];
			int nDefCode = pDoc->m_pPcr[1][nPcrIdx]->m_pDefType[i];
			if (pDoc->m_Master[0].m_pPcsRgn)
				pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(nPcsIdx, nStrip, nCol, nRow);

			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
			{
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d\\%05d_%s_%c_%d_%d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					sLot,
					WorkingInfo.LastJob.sLayerDn,
					nSerial,
					nDefImg, pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
			}
			else
			{
				strDefImgPathD.Format(_T("%s\\%s\\%s\\%s\\DefImagePos\\%d\\%d_%05d_%s_%c_%d_%d.tif"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					sLot,
					WorkingInfo.LastJob.sLayerDn,
					nSerial, nSerial,
					nDefImg, pDoc->m_pReelMap->m_sKorDef[nDefCode], nStrip + 'A', nCol + 1, nRow + 1);
			}

			if (finder.FindFile(strDefImgPathS))
			{
				if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
				{
					if (!CopyFile((LPCTSTR)strDefImgPathS, (LPCTSTR)strDefImgPathD, FALSE))
					{
						strTemp.Format(_T("%s \r\n: Defect Image Position File Copy Fail"), strDefImgPathS);
						pView->MsgBox(strTemp);
						return FALSE;
					}
				}
			}
			else
			{
				Sleep(30);
				if (nErrorCnt > 10)
				{
					nErrorCnt = 0;
					strTemp.Format(_T("%s \r\n: Defect Image Position File Not Exist"), strDefImgPathS);
					//AfxMessageBox(strTemp);
					return TRUE;
				}
				else
				{
					nErrorCnt++;
					i--;
					continue;
				}
			}
		}
	}

	return TRUE;
}


int CGvisR2R_PunchDoc::GetIdxPcrBuf(int nSerial) // 릴맵화면표시의 판넬 인덱스를 반환(MAX_PCR_PNL)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.20"));
		return 0;
	}

	int i, nIdx = -1;

	if (m_pPcr[0])
	{
		for (i = 0; i < MAX_PCR_PNL; i++)
		{
			if (m_pPcr[0][i]->m_nSerial == nSerial)
			{
				nIdx = i;
				break;
			}
		}
	}

	return nIdx;
}

int CGvisR2R_PunchDoc::GetIdxPcrBufUp(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.21"));
		return 0;
	}

	int i, nIdx = -1;

	if (m_pPcr[0])
	{
		for (i = 0; i < MAX_PCR_PNL; i++)
		{
			if (m_pPcr[0][i]->m_nSerial == nSerial)
			{
				nIdx = i;
				break;
			}
		}
	}

	return nIdx;
}

int CGvisR2R_PunchDoc::GetIdxPcrBufDn(int nSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.22"));
		return 0;
	}

	int i, nIdx = -1;

	if (m_pPcr[1])
	{
		for (i = 0; i < MAX_PCR_PNL; i++)
		{
			if (m_pPcr[1][i]->m_nSerial == nSerial)
			{
				nIdx = i;
				break;
			}
		}
	}

	return nIdx;
}

void CGvisR2R_PunchDoc::DeleteFileInFolder(CString sPathDir)
{
	BOOL bRes;
	CFileFind ff;

	if (CheckPath(sPathDir) == PATH_IS_FILE)
	{
		DeleteFile(sPathDir);
		return;
	}

	if (sPathDir.Right(1) != _T("\\"))
		sPathDir.Append(_T("\\")); //sPathDir += _T("\\");

	sPathDir.Append(_T("*.*")); //sPathDir += _T("*.*");

	bRes = ff.FindFile(sPathDir);

	while (bRes)
	{
		bRes = ff.FindNextFile();
		if (ff.IsDots())
			continue;
		if (ff.IsDirectory())
		{
			sPathDir = ff.GetFilePath();
			DeleteFileInFolder(sPathDir);
		}
		else
			DeleteFileInFolder(ff.GetFilePath());
	}

	ff.Close();
}

int CGvisR2R_PunchDoc::CheckPath(CString strPath)
{
	DWORD dwAttr = GetFileAttributes(strPath);
	if (dwAttr == 0xffffffff)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND)
			return PATH_NOT_FOUND;
		return PATH_ERROR;
	}
	if (dwAttr & FILE_ATTRIBUTE_DIRECTORY) return PATH_IS_FOLDER;
	return PATH_IS_FILE;
}

int CGvisR2R_PunchDoc::GetLastSerial()
{
	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nMkLastShot;

	return (nLastShot);
}

int CGvisR2R_PunchDoc::GetLastShotMk()	// m_pDlgFrameHigh에서 얻거나 없으면, sPathOldFile폴더의 ReelMapDataDn.txt에서 _T("Info"), _T("Marked Shot") 찾음.
{
	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nMkLastShot;

	BOOL bDualTest = WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (!nLastShot && pView->m_bCont)
		{
			CFileFind FF;
			CString sPath, sRmapPath, sMsg;
			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				sPath.Format(_T("%s\\%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					WorkingInfo.LastJob.sLot,
					WorkingInfo.LastJob.sLayerDn);
			else
				sPath.Format(_T("%s%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					WorkingInfo.LastJob.sLot,
					WorkingInfo.LastJob.sLayerDn);

			sRmapPath.Format(_T("%s\\ReelMapDataDn.txt"), sPath);
			BOOL bExist = FF.FindFile(sRmapPath);

			if (!bExist) // Can not find file.
			{
				return 0; // Failed.
			}

			TCHAR szData[MAX_PATH];
			if (0 < ::GetPrivateProfileString(_T("Info"), _T("Marked Shot"), NULL, szData, sizeof(szData), sRmapPath))
				nLastShot = _tstoi(szData);
			else
				nLastShot = 0; // Failed.
		}
	}
	else
	{
		if (!nLastShot && pView->m_bCont)
		{
			CFileFind FF;
			CString sPath, sRmapPath, sMsg;
			if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
				sPath.Format(_T("%s\\%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					WorkingInfo.LastJob.sLot,
					WorkingInfo.LastJob.sLayerUp);
			else
				sPath.Format(_T("%s%s\\%s\\%s"), WorkingInfo.System.sPathOldFile,
					WorkingInfo.LastJob.sModel,
					WorkingInfo.LastJob.sLot,
					WorkingInfo.LastJob.sLayerUp);

			sRmapPath.Format(_T("%s\\ReelMapDataUp.txt"), sPath);
			BOOL bExist = FF.FindFile(sRmapPath);

			if (!bExist) // Can not find file.
			{
				return 0; // Failed.
			}

			TCHAR szData[MAX_PATH];
			if (0 < ::GetPrivateProfileString(_T("Info"), _T("Marked Shot"), NULL, szData, sizeof(szData), sRmapPath))
				nLastShot = _tstoi(szData);
			else
				nLastShot = 0; // Failed.
		}
	}

	return (nLastShot);
}


int CGvisR2R_PunchDoc::GetLastShotUp()
{
	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nAoiLastShot[0];

	return (nLastShot);
}

int CGvisR2R_PunchDoc::GetLastShotDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nAoiLastShot[1];

	return (nLastShot);
}

int CGvisR2R_PunchDoc::GetLotSerial()
{
	int nLotSerial;

	double dLen = _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen) * (m_pReelMap->m_nSerial - 1);
	if (dLen < _tstof(pDoc->WorkingInfo.LastJob.sOnePnlLen))
		nLotSerial = 1;
	else
		nLotSerial = 2;

	return nLotSerial;
}

void CGvisR2R_PunchDoc::SetMkPnt(int nCam)	// Pin위치에 의한 정렬.
{
	if (m_Master[0].m_pPcsRgn)
		m_Master[0].m_pPcsRgn->SetMkPnt(nCam);
}

void CGvisR2R_PunchDoc::SetOnePnlLen(double dLen)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dLen);
	WorkingInfo.LastJob.sOnePnlLen = sVal;
	WorkingInfo.Motion.sMkFdDist = WorkingInfo.Motion.sAoiFdDist = sVal;
	m_pReelMap->m_dPnlLen = dLen;
	::WritePrivateProfileString(_T("Last Job"), _T("One Panel Length"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_DIST"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_DIST"), sVal, sPath);
	long lData = (long)(dLen * 1000.0);
	pView->MpeWrite(pView->Plc.DlgInfo.OneShotLength, lData);	// 한 판넬 길이 (단위 mm * 1000)

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetOnePnlLen();	//_ItemInx::_OnePnlLen
//#endif
}

double CGvisR2R_PunchDoc::GetOnePnlLen()
{
	return (_tstof(WorkingInfo.Motion.sMkFdDist));
}

void CGvisR2R_PunchDoc::SetOnePnlVel(double dVel)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sMkFdVel = WorkingInfo.Motion.sAoiFdVel = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_VEL"), sVal, sPath);
	long lData = (long)(dVel * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedSpeed, lData);	// 한 판넬 Feeding 속도 (단위 mm/sec * 1000)
}

void CGvisR2R_PunchDoc::SetFdJogVel(double dVel)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sMkJogVel = WorkingInfo.Motion.sAoiJogVel = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_VEL"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_VEL"), sVal, sPath);
	long lData = (long)(dVel * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedOnSpeed, lData);	// 연속공급 속도 (단위 mm/sec * 1000)

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetFdVel();	//_ItemInx::_FdVel
//#endif
}

void CGvisR2R_PunchDoc::SetFdJogAcc(double dVel)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sMkJogAcc = WorkingInfo.Motion.sAoiJogAcc = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_JOG_ACC"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_JOG_ACC"), sVal, sPath);
	long lData = (long)(dVel * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedOnAcc, lData);	// 연속공급 가속도 (단위 mm/s^2 * 1000)

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetFdAcc();	//_ItemInx::_FdAcc
//#endif
}

double CGvisR2R_PunchDoc::GetOnePnlVel()
{
	return (_tstof(WorkingInfo.Motion.sMkFdVel));
}

double CGvisR2R_PunchDoc::GetFdJogVel()
{
	return (_tstof(WorkingInfo.Motion.sMkJogVel));
}

double CGvisR2R_PunchDoc::GetFdJogAcc()
{
	return (_tstof(WorkingInfo.Motion.sMkJogAcc));
}

void CGvisR2R_PunchDoc::SetOnePnlAcc(double dAcc)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dAcc);
	WorkingInfo.Motion.sMkFdAcc = WorkingInfo.Motion.sAoiFdAcc = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), sVal, sPath);
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_SERVO_ACC"), sVal, sPath);
	long lData = (long)(dAcc * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedAcc, lData);	// 한 판넬 Feeding 가속도 (단위 mm/s^2 * 1000)

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetOnePnlAcc();	//_ItemInx::_OnePnlAcc
//#endif
}

double CGvisR2R_PunchDoc::GetOnePnlAcc()
{
	return (_tstof(WorkingInfo.Motion.sMkFdAcc));
}

void CGvisR2R_PunchDoc::SetEngFdPitch(double dPitch)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dPitch);
	WorkingInfo.Motion.sEngraveFdLead = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_DRUM_LEAD_PITCH"), sVal, sPath);
	long lData = (long)(dPitch * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedLeadPitchEngrave, lData);	// 검사부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
}

void CGvisR2R_PunchDoc::SetAoiFdPitch(double dPitch)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dPitch);
	WorkingInfo.Motion.sAoiFdLead = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_FEEDING_DRUM_LEAD_PITCH"), sVal, sPath);
	long lData = (long)(dPitch * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedLeadPitchAoi, lData);	// 검사부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
	//pView->MpeWrite(pView->Plc.DlgMenu04.FeedLeadPitchEngrave, lData);	// 검사부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
}

double CGvisR2R_PunchDoc::GetAoiFdPitch()
{
	return (_tstof(WorkingInfo.Motion.sAoiFdLead));
}

void CGvisR2R_PunchDoc::SetMkFdPitch(double dPitch)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dPitch);
	WorkingInfo.Motion.sMkFdLead = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_DRUM_LEAD_PITCH"), sVal, sPath);
	long lData = (long)(dPitch * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedLeadPitchPunch, lData);	// 마킹부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
}

double CGvisR2R_PunchDoc::GetMkFdPitch()
{
	return (_tstof(WorkingInfo.Motion.sMkFdLead));
}

void CGvisR2R_PunchDoc::SetFdErrLmt(double dLmt)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dLmt);
	WorkingInfo.Motion.sLmtFdErr = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_ERROR_VAL"), sVal, sPath);

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetFdDiffMax();	//_ItemInx::_FdDiffMax
//#endif
}

double CGvisR2R_PunchDoc::GetFdErrLmt()
{
	return (_tstof(WorkingInfo.Motion.sLmtFdErr));
}

void CGvisR2R_PunchDoc::SetFdErrRng(double dRng)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dRng);
	WorkingInfo.Motion.sLmtFdAdjOffSet = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OFFSET"), sVal, sPath);

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetFdDiffRng();	//_ItemInx::_FdDiffRng
//#endif
}

double CGvisR2R_PunchDoc::GetFdErrRng()
{
	return (_tstof(WorkingInfo.Motion.sLmtFdAdjOffSet));
}

void CGvisR2R_PunchDoc::SetFdErrNum(int nNum)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%d"), nNum);
	WorkingInfo.Motion.sLmtFdOvrNum = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ADJUST_LIMIT_FEEDING_OVER_NUM"), sVal, sPath);

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetFdDiffNum();	//_ItemInx::_FdDiffNum
//#endif
}

int CGvisR2R_PunchDoc::GetFdErrNum()
{
	return (_tstoi(WorkingInfo.Motion.sLmtFdOvrNum));
}

void CGvisR2R_PunchDoc::SetBufInitPos(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Motion.sStBufPos = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_BUFFER_POSITION"), sData, sPath);
	long lData = (long)(dPos * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.BufferPosStAoi, lData);	// 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetMkBuffInitPos();	//_ItemInx::_MkBuffInitPos
//#endif
}

void CGvisR2R_PunchDoc::SetEngBufInitPos(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Motion.sStEngBufPos = sData;
	::WritePrivateProfileString(_T("Motion"), _T("START_ENG_BUFFER_POSITION"), sData, sPath);
	long lData = (long)(dPos * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.BufferPosStEngrave, lData);	// 각인부 버퍼 관련 설정 롤러 초기위치(단위 mm * 1000)
}

void CGvisR2R_PunchDoc::SetBufInitPos(double dVel, double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Motion.sBufHomeSpd = sData;
	::WritePrivateProfileString(_T("Motion"), _T("BUFFER_HOME_SPEED"), sData, sPath);
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Motion.sBufHomeAcc = sData;
	::WritePrivateProfileString(_T("Motion"), _T("BUFFER_HOME_ACC"), sData, sPath);
}


double CGvisR2R_PunchDoc::GetBuffInitPos()
{
	return (_tstof(WorkingInfo.Motion.sStBufPos));
}

void CGvisR2R_PunchDoc::SetAoiMkDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.sFdMkAoiInitDist = sData;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_PUNCH_AOI_INIT_DIST"), sData, sPath);
	long lData = (long)(dLen * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedLengthFromAoiToPunch, lData);	// AOI(하)에서 마킹까지 거리 (단위 mm * 1000)

//#ifdef USE_ENGRAVE
//	if (pView && pView->m_pEngrave)
//		pView->m_pEngrave->SetAoiMkLen();	//_ItemInx::_AoiMkLen
//#endif
}

void CGvisR2R_PunchDoc::SetAoiAoiDist(int nShot)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nShot);
	WorkingInfo.Motion.sFdAoiAoiDistShot = sData;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_AOI_AOI_SHOT_NUM"), sData, sPath);
	long lData = (long)(nShot * 1000);
	pView->MpeWrite(pView->Plc.DlgMenu04.ShotNumFromAoiUpToAoiDn, lData);	// AOI(상)에서 AOI(하) Shot수 (단위 Shot수 * 1000)
}


double CGvisR2R_PunchDoc::GetAoiMkDist()
{
	return (_tstof(WorkingInfo.Motion.sFdMkAoiInitDist));
}

int CGvisR2R_PunchDoc::GetAoiAoiDist()
{
	return (_tstoi(WorkingInfo.Motion.sFdAoiAoiDistShot));
}

void CGvisR2R_PunchDoc::SetWaitPos1(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[0].sWaitPos = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_POS"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitPos1()
{
	return (_tstof(WorkingInfo.Marking[0].sWaitPos));
}

void CGvisR2R_PunchDoc::SetWaitVel1(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[0].sWaitVel = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_VEL"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitVel1()
{
	return (_tstof(WorkingInfo.Marking[0].sWaitVel));
}

void CGvisR2R_PunchDoc::SetWaitAcc1(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[0].sWaitAcc = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_WAIT_ACC"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitAcc1()
{
	return (_tstof(WorkingInfo.Marking[0].sWaitAcc));
}

void CGvisR2R_PunchDoc::SetMarkingPos1(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[0].sMarkingPos = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_POS"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingPos1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingPos));
}

void CGvisR2R_PunchDoc::SetMarkingVel1(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[0].sMarkingVel = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_VEL"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingVel1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingVel));
}

void CGvisR2R_PunchDoc::SetMarkingAcc1(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[0].sMarkingAcc = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_ACC"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingAcc1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingAcc));
}

void CGvisR2R_PunchDoc::SetMarkingToq1(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Marking[0].sMarkingToq = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetMarkingToq1Offset(double dToqOffset)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToqOffset);
	WorkingInfo.Marking[0].sMarkingToqOffset = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MARKING_TOQ_OFFSET"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingToq1()
{
	return (_tstof(WorkingInfo.Marking[0].sMarkingToq));
}

void CGvisR2R_PunchDoc::SetPosX1_1(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[0] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX1"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX1_1()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[0]));
}

void CGvisR2R_PunchDoc::SetPosY1_1(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[0] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY1"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY1_1()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[0]));
}

void CGvisR2R_PunchDoc::SetPosX1_2(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[1] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX2"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX1_2()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[1]));
}

void CGvisR2R_PunchDoc::SetPosY1_2(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[1] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY2"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY1_2()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[1]));
}

void CGvisR2R_PunchDoc::SetPosX1_3(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[2] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX3"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX1_3()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[2]));
}

void CGvisR2R_PunchDoc::SetPosY1_3(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[2] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY3"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY1_3()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[2]));
}

void CGvisR2R_PunchDoc::SetPosX1_4(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[0].sMeasurePosX[3] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSX4"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX1_4()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[3]));
}

void CGvisR2R_PunchDoc::SetPosY1_4(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[0].sMeasurePosY[3] = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_MEASURE_POSY4"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY1_4()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosY[3]));
}

void CGvisR2R_PunchDoc::SetAverDist1(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Marking[0].sAverDist = sData;
	::WritePrivateProfileString(_T("Marking0"), _T("MARKING_AVER_DIST"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetAverDist1()
{
	return (_tstof(WorkingInfo.Marking[0].sAverDist));
}

void CGvisR2R_PunchDoc::SetWaitPos2(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[1].sWaitPos = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_POS"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitPos2()
{
	return (_tstof(WorkingInfo.Marking[1].sWaitPos));
}

void CGvisR2R_PunchDoc::SetWaitVel2(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[1].sWaitVel = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_VEL"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitVel2()
{
	return (_tstof(WorkingInfo.Marking[1].sWaitVel));
}

void CGvisR2R_PunchDoc::SetWaitAcc2(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[1].sWaitAcc = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_WAIT_ACC"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetWaitAcc2()
{
	return (_tstof(WorkingInfo.Marking[1].sWaitAcc));
}

void CGvisR2R_PunchDoc::SetMarkingPos2(double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	WorkingInfo.Marking[1].sMarkingPos = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_POS"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingPos2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingPos));
}

void CGvisR2R_PunchDoc::SetMarkingVel2(double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	WorkingInfo.Marking[1].sMarkingVel = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_VEL"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingVel2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingVel));
}

void CGvisR2R_PunchDoc::SetMarkingAcc2(double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	WorkingInfo.Marking[1].sMarkingAcc = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_ACC"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingAcc2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingAcc));
}

void CGvisR2R_PunchDoc::SetMarkingToq2(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Marking[1].sMarkingToq = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetMarkingToq2Offset(double dToqOffset)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToqOffset);
	WorkingInfo.Marking[1].sMarkingToqOffset = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MARKING_TOQ_OFFSET"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetMarkingToq2()
{
	return (_tstof(WorkingInfo.Marking[1].sMarkingToq));
}

void CGvisR2R_PunchDoc::SetPosX2_1(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[0] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX1"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX2_1()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosX[0]));
}

void CGvisR2R_PunchDoc::SetPosY2_1(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[0] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY1"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY2_1()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[0]));
}

void CGvisR2R_PunchDoc::SetPosX2_2(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[1] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX2"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX2_2()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosX[1]));
}

void CGvisR2R_PunchDoc::SetPosY2_2(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[1] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY2"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY2_2()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[1]));
}

void CGvisR2R_PunchDoc::SetPosX2_3(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[2] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX3"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX2_3()
{
	return (_tstof(WorkingInfo.Marking[0].sMeasurePosX[2]));
}

void CGvisR2R_PunchDoc::SetPosY2_3(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[2] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY3"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY2_3()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[2]));
}

void CGvisR2R_PunchDoc::SetPosX2_4(double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	WorkingInfo.Marking[1].sMeasurePosX[3] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSX4"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosX2_4()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosX[3]));
}

void CGvisR2R_PunchDoc::SetPosY2_4(double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	WorkingInfo.Marking[1].sMeasurePosY[3] = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_MEASURE_POSY4"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetPosY2_4()
{
	return (_tstof(WorkingInfo.Marking[1].sMeasurePosY[3]));
}

void CGvisR2R_PunchDoc::SetAverDist2(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Marking[1].sAverDist = sData;
	::WritePrivateProfileString(_T("Marking1"), _T("MARKING_AVER_DIST"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetAverDist2()
{
	return (_tstof(WorkingInfo.Marking[1].sAverDist));
}

void CGvisR2R_PunchDoc::SetTotalReelDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sTotalReelDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_TOTAL_REEL_DIST"), sData, sPath);


	WorkingInfo.LastJob.sReelTotLen = sData;
	if (m_pReelMap)
		m_pReelMap->m_dTotLen = _tstof(sData) * 1000.0;
	::WritePrivateProfileString(_T("Last Job"), _T("Reel Total Length"), sData, PATH_WORKING_INFO);

	long lData = (long)(dDist * 1000.0);
	if(pView && pView->m_pMpe)
		pView->MpeWrite(pView->Plc.DlgInfo.TotalReelLength, lData);	// 전체 Reel 길이 (단위 M * 1000)
}

double CGvisR2R_PunchDoc::GetTotalReelDist()
{
	return (_tstof(WorkingInfo.Lot.sTotalReelDist));
}

void CGvisR2R_PunchDoc::SetSeparateDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sSeparateDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_SEPAREATE_DIST"), sData, sPath);

	pDoc->WorkingInfo.LastJob.sLotSepLen = sData;
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_dLotLen = _tstof(sData);
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Seperate Length"), sData, PATH_WORKING_INFO);

	long lData = (long)(dDist * 1000.0);
	pView->MpeWrite(pView->Plc.DlgInfo.LotLength, lData);	// Lot 분리 길이 (단위 M * 1000)
}

double CGvisR2R_PunchDoc::GetSeparateDist()
{
	return (_tstof(WorkingInfo.Lot.sSeparateDist));
}

void CGvisR2R_PunchDoc::SetCuttingDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sCuttingDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_CUTTING_DIST"), sData, sPath);

	pDoc->WorkingInfo.LastJob.sLotCutPosLen = sData;
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_dLotCutPosLen = _tstof(sData);
	::WritePrivateProfileString(_T("Last Job"), _T("Lot Cut Position Length"), sData, PATH_WORKING_INFO);

	long lData = (long)(dDist * 1000.0);
	pView->MpeWrite(pView->Plc.DlgInfo.LotCutLength, lData);	// Lot 분리 후 절단위치 (단위 M * 1000)
}

double CGvisR2R_PunchDoc::GetCuttingDist()
{
	return (_tstof(WorkingInfo.Lot.sCuttingDist));
}

void CGvisR2R_PunchDoc::SetStopDist(double dDist)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dDist);
	WorkingInfo.Lot.sStopDist = sData;
	::WritePrivateProfileString(_T("Lot"), _T("LOT_STOP_DIST"), sData, sPath);


	pDoc->WorkingInfo.LastJob.sTempPauseLen = sData;
	if (pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_dTempPauseLen = _tstof(sData);
	::WritePrivateProfileString(_T("Last Job"), _T("Temporary Pause Length"), sData, PATH_WORKING_INFO);

	long lData = (long)(dDist * 1000.0);
	pView->MpeWrite(pView->Plc.DlgInfo.StopLength, lData);	// 일시정지 길이 (단위 M * 1000)
}

double CGvisR2R_PunchDoc::GetStopDist()
{
	return (_tstof(WorkingInfo.Lot.sStopDist));
}

void CGvisR2R_PunchDoc::SetAOIToq(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Motion.sAoiTq = sData;
	::WritePrivateProfileString(_T("Motion"), _T("AOI_TENSION_SERVO_TORQUE"), sData, sPath);
	long lData = (long)(dToq * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedTorqueCwAoi, lData);	// 검사부 Tension 모터 토크값 (단위 Kgf * 1000)
}

double CGvisR2R_PunchDoc::GetAOIToq()
{
	return (_tstof(WorkingInfo.Motion.sAoiTq));
}

void CGvisR2R_PunchDoc::SetMarkingToq(double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	WorkingInfo.Motion.sMkTq = sData;
	::WritePrivateProfileString(_T("Motion"), _T("MARKING_TENSION_SERVO_TORQUE"), sData, sPath);
	long lData = (long)(dToq * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedTorqueCwPunch, lData);	// 마킹부 Tension 모터 토크값 (단위 Kgf * 1000)
}

double CGvisR2R_PunchDoc::GetMarkingToq()
{
	return (_tstof(WorkingInfo.Motion.sMkTq));
}

double CGvisR2R_PunchDoc::GetEngraveToq()
{
	return (_tstof(WorkingInfo.Motion.sEngraveTq));
}

int CGvisR2R_PunchDoc::CopyPcrAll()  // from share to buffer ; return : Serial
{
	int nS0, nS1;
	if (pView->m_bLoadShare[0])
	{
		nS0 = CopyPcrUp();
	}
	if (pView->m_bLoadShare[1])
	{
		nS1 = CopyPcrDn();
	}

	if (pView->m_bLoadShare[0] && pView->m_bLoadShare[1])
	{
		if (nS0 > 0 && nS1 > 0)
			return TRUE;
	}
	else if (pView->m_bLoadShare[0] && !pView->m_bLoadShare[1])
	{
		if (nS0 > 0)
			return TRUE;
	}
	else if (!pView->m_bLoadShare[0] && pView->m_bLoadShare[1])
	{
		if (nS1 > 0)
			return TRUE;
	}

	return FALSE;
}

int CGvisR2R_PunchDoc::CopyPcrUp()  // return : Serial
{
	int nSerial;
	CString sSrc = WorkingInfo.System.sPathVrsShareUp;
	CString sDest = WorkingInfo.System.sPathVrsBufUp;

	if (m_pFile)
		nSerial = m_pFile->CopyPcrAll(sSrc, sDest);

	return nSerial;
}

int CGvisR2R_PunchDoc::CopyPcrDn()  // return : Serial
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	int nSerial;
	CString sSrc = WorkingInfo.System.sPathVrsShareDn;
	CString sDest = WorkingInfo.System.sPathVrsBufDn;

	if (m_pFile)
		nSerial = m_pFile->CopyPcrAll(sSrc, sDest);

	return nSerial;
}

void CGvisR2R_PunchDoc::DelSharePcr()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (bDualTest)
	{
		if (pView->m_bLoadShare[0])
			DelSharePcrUp();
		if (pView->m_bLoadShare[1])
			DelSharePcrDn();
	}
	else
	{
		if (pView->m_bLoadShare[0])
			DelSharePcrUp();
		if (pView->IsShareDn())
			DelSharePcrDn();
	}
}

void CGvisR2R_PunchDoc::DelSharePcrUp()
{
	CString sPath = WorkingInfo.System.sPathVrsShareUp;

	if (m_pFile)
	{
		while(m_pFile->IsPcrExist(sPath))
		{
			m_pFile->DelPcrAll(sPath);
			Sleep(30);
		}
	}
}

void CGvisR2R_PunchDoc::DelSharePcrDn()
{
	CString sPath = WorkingInfo.System.sPathVrsShareDn;

	while (m_pFile->IsPcrExist(sPath))
	{
		if (m_pFile)
			m_pFile->DelPcrAll(sPath);
		Sleep(30);
	}
}

void CGvisR2R_PunchDoc::DelShareVsPcrUp()
{
	CString sPath = WorkingInfo.System.sPathVsShareUp;

	if (m_pFile)
	{
		while (m_pFile->IsPcrExist(sPath))
		{
			m_pFile->DelPcrAll(sPath);
			Sleep(30);
		}
	}
}

void CGvisR2R_PunchDoc::DelShareVsPcrDn()
{
	CString sPath = WorkingInfo.System.sPathVsShareDn;

	while (m_pFile->IsPcrExist(sPath))
	{
		if (m_pFile)
			m_pFile->DelPcrAll(sPath);
		Sleep(30);
	}
}

void CGvisR2R_PunchDoc::DelPcrAll()
{
	DelSharePcrUp();
	DelSharePcrDn();
	DelShareVsPcrUp();
	DelShareVsPcrDn();

	DelPcrUp();
	DelPcrDn();

	pView->m_bIsBuf[0] = FALSE;
	pView->m_bIsBuf[1] = FALSE;

}

void CGvisR2R_PunchDoc::DelPcrUp()
{
	CString sPath;

	if (m_pFile)
	{
		sPath = WorkingInfo.System.sPathVrsShareUp;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVrsBufUp;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVsShareUp;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVsDummyBufUp;
		m_pFile->DelPcrAll(sPath);
	}
}

void CGvisR2R_PunchDoc::DelPcrDn()
{
	CString sPath;

	if (m_pFile)
	{
		sPath = WorkingInfo.System.sPathVrsShareDn;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVrsBufDn;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVsShareDn;
		m_pFile->DelPcrAll(sPath);
		sPath = WorkingInfo.System.sPathVsDummyBufDn;
		m_pFile->DelPcrAll(sPath);
	}
}

void CGvisR2R_PunchDoc::SaveLotTime(DWORD dwStTick)
{
	CString sData, sPath = PATH_WORKING_INFO;

	// [Lot]
	WorkingInfo.Lot.dwStTick = dwStTick;
	sData.Format(_T("%d"), dwStTick);
	::WritePrivateProfileString(_T("Lot"), _T("Start Tick"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nYear);
	::WritePrivateProfileString(_T("Lot"), _T("Start Year"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nMonth);
	::WritePrivateProfileString(_T("Lot"), _T("Start Month"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nDay);
	::WritePrivateProfileString(_T("Lot"), _T("Start Day"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nHour);
	::WritePrivateProfileString(_T("Lot"), _T("Start Hour"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nMin);
	::WritePrivateProfileString(_T("Lot"), _T("Start Minute"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.StTime.nSec);
	::WritePrivateProfileString(_T("Lot"), _T("Start Second"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nYear);
	::WritePrivateProfileString(_T("Lot"), _T("End Year"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nMonth);
	::WritePrivateProfileString(_T("Lot"), _T("End Month"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nDay);
	::WritePrivateProfileString(_T("Lot"), _T("End Day"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nHour);
	::WritePrivateProfileString(_T("Lot"), _T("End Hour"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nMin);
	::WritePrivateProfileString(_T("Lot"), _T("End Minute"), sData, sPath);

	sData.Format(_T("%d"), WorkingInfo.Lot.EdTime.nSec);
	::WritePrivateProfileString(_T("Lot"), _T("End Second"), sData, sPath);
}

BOOL CGvisR2R_PunchDoc::Shift2Mk(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.23"));
		return 0;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	stModelInfo stInfo;
	CString sSrc, sDest;

	BOOL bRst[4] = { 0 };
	if (m_pFile)
	{
		sSrc.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufUp, nSerial);
		if (!GetPcrInfo(sSrc, stInfo))
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->DispStsBar(_T("E(2)"), 5);
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error-GetPcrInfo(2)"));
			return FALSE;
		}

		if (!MakeMkDir(stInfo))
		{
			pView->SetAlarmToPlc(UNIT_PUNCH);
			pView->ClrDispMsg();
			AfxMessageBox(_T("Error-MakeMkDir()"));
			return FALSE;
		}
		sDest.Format(_T("%s%s\\%s\\%s\\%04d.pcr"), pDoc->WorkingInfo.System.sPathOldFile, stInfo.sModel,
			stInfo.sLot, stInfo.sLayer, nSerial);

		bRst[0] = m_pFile->Copy(sSrc, sDest);			// 마킹한 파일을 복사한다.
		bRst[1] = m_pFile->DeleteFolerOrFile(sSrc);		// 마킹한 파일을 지운다.


		if (bDualTest)
		{
			sSrc.Format(_T("%s%04d.pcr"), WorkingInfo.System.sPathVrsBufDn, nSerial);
			if (!GetPcrInfo(sSrc, stInfo))
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->DispStsBar(_T("E(3)"), 5);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Error-GetPcrInfo(3)"));
				return FALSE;
			}

			if (!MakeMkDir(stInfo))
			{
				pView->SetAlarmToPlc(UNIT_PUNCH);
				pView->ClrDispMsg();
				AfxMessageBox(_T("Error-MakeMkDir()"));
				return FALSE;
			}
			sDest.Format(_T("%s%s\\%s\\%s\\%04d.pcr"), pDoc->WorkingInfo.System.sPathOldFile, stInfo.sModel,
				stInfo.sLot, stInfo.sLayer, nSerial);
			bRst[2] = m_pFile->Copy(sSrc, sDest);			// 마킹한 파일을 복사한다.
			bRst[3] = m_pFile->DeleteFolerOrFile(sSrc);		// 마킹한 파일을 지운다.
		}
		else
		{
			bRst[2] = TRUE;
			bRst[3] = TRUE;
		}
	}
	else
		return FALSE;

	if (bRst[0] && bRst[1] && bRst[2] && bRst[3])
	{
		SetLastSerial(nSerial);
		return TRUE;
	}

	return FALSE;
}

void CGvisR2R_PunchDoc::SetLastSerial(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.24"));
		return;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (nSerial > 0)
	{
		CString str, sPath = PATH_WORKING_INFO;
		str.Format(_T("%d"), nSerial);
		WorkingInfo.LastJob.sSerialUp = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Last SerialUp"), str, sPath);
		WorkingInfo.LastJob.sSerialDn = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Last SerialDn"), str, sPath);

		// 		if(m_pReelMap)
		// 			m_pReelMap->SetLastSerial(nSerial);
		if (m_pReelMapUp)
			m_pReelMapUp->SetLastSerial(nSerial);					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
		if (bDualTest)
		{
			if (m_pReelMapDn)
				m_pReelMapDn->SetLastSerial(nSerial);					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
			if (m_pReelMapAllUp)
				m_pReelMapAllUp->SetLastSerial(nSerial);					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
			if (m_pReelMapAllDn)
				m_pReelMapAllDn->SetLastSerial(nSerial);					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
		}

		if (pDoc->GetTestMode() == MODE_OUTER)
		{
			if (m_pReelMap)
				m_pReelMap->SetLastSerial(nSerial);					// 릴맵 텍스트 파일의 수율정보를 업데이트함.
		}
	}
}

void CGvisR2R_PunchDoc::UpdateYieldOnRmap()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	pView->m_bTHREAD_REELMAP_YIELD_UP = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_UP"), pView->m_bTHREAD_REELMAP_YIELD_UP); // UpdateReelmapYieldUp(); // Yield Reelmap
	if (bDualTest)
	{
		pView->m_bTHREAD_REELMAP_YIELD_DN = TRUE; pDoc->SetStatus(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_DN"), pView->m_bTHREAD_REELMAP_YIELD_DN); // UpdateReelmapYieldDn(); // Yield Reelmap
	}
	Sleep(100);
}

void CGvisR2R_PunchDoc::UpdateYieldUp(int nSerial)
{
	if (m_pReelMapUp)
		m_pReelMapUp->UpdateYield(nSerial);
}

void CGvisR2R_PunchDoc::UpdateYieldDn(int nSerial)
{
	if (m_pReelMapDn)
		m_pReelMapDn->UpdateYield(nSerial);
}

void CGvisR2R_PunchDoc::UpdateYieldAllUp(int nSerial)
{
	if (m_pReelMapAllUp)
		m_pReelMapAllUp->UpdateYield(nSerial);
}

void CGvisR2R_PunchDoc::UpdateYieldAllDn(int nSerial)
{
	if (m_pReelMapAllDn)
		m_pReelMapAllDn->UpdateYield(nSerial);
}

void CGvisR2R_PunchDoc::UpdateYieldInnerUp(int nSerial)
{
	if (m_pReelMapInnerUp)
		m_pReelMapInnerUp->UpdateYield(nSerial);
}

void CGvisR2R_PunchDoc::UpdateYieldInnerDn(int nSerial)
{
	if (m_pReelMapInnerDn)
		m_pReelMapInnerDn->UpdateYield(nSerial);
}

void CGvisR2R_PunchDoc::UpdateYieldInnerAllUp(int nSerial)
{
	if (m_pReelMapInnerAllUp)
		m_pReelMapInnerAllUp->UpdateYield(nSerial);
}

void CGvisR2R_PunchDoc::UpdateYieldInnerAllDn(int nSerial)
{
	if (m_pReelMapInnerAllDn)
		m_pReelMapInnerAllDn->UpdateYield(nSerial);
}

void CGvisR2R_PunchDoc::UpdateYieldIts(int nSerial)
{
	if (m_pReelMapIts)
		m_pReelMapIts->UpdateYield(nSerial);
}

void CGvisR2R_PunchDoc::SetCompletedSerial(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.49"));
		return;
	}

	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	if (nSerial > 0)
	{
		CString str, sPath = PATH_WORKING_INFO;
		str.Format(_T("%d"), nSerial);
		WorkingInfo.LastJob.sCompletedSerialUp = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Completed SerialUp"), str, sPath);
		WorkingInfo.LastJob.sCompletedSerialDn = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Completed SerialDn"), str, sPath);

		// 		if(m_pReelMap)
		// 			m_pReelMap->SetCompletedSerial(nSerial);
		if (m_pReelMapUp)
			m_pReelMapUp->SetCompletedSerial(nSerial);
		if (bDualTest)
		{
			if (m_pReelMapDn)
				m_pReelMapDn->SetCompletedSerial(nSerial);
			if (m_pReelMapAllUp)
				m_pReelMapAllUp->SetCompletedSerial(nSerial);
			if (m_pReelMapAllDn)
				m_pReelMapAllDn->SetCompletedSerial(nSerial);
		}
	}
}

void CGvisR2R_PunchDoc::SetModelInfoUp()
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData = WorkingInfo.LastJob.sModel;
	::WritePrivateProfileString(_T("Last Job"), _T("Model Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLayerUp;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerUp Name"), sData, sPath);

	sData = WorkingInfo.LastJob.sLot;
	::WritePrivateProfileString(_T("Last Job"), _T("Lot No"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetModelInfoDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return;

	CString sData, sPath = PATH_WORKING_INFO;
	sData = WorkingInfo.LastJob.sLayerDn;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerDn Name"), sData, sPath);
}

BOOL CGvisR2R_PunchDoc::MakeMkDir(CString sModel, CString sLot, CString sLayer)
{
	CString sMsg = _T("");
	CFileFind finder;
	CString sPath;

	sPath.Format(_T("%s"), pDoc->WorkingInfo.System.sPathOldFile);
	int pos = sPath.ReverseFind('\\');
	if (pos != -1)
		sPath.Delete(pos, sPath.GetLength() - pos);

	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if (sModel.IsEmpty() || sLot.IsEmpty() || sLayer.IsEmpty())
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("모델이나 로뜨 또는 레이어명이 없습니다."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s%s"), pDoc->WorkingInfo.System.sPathOldFile,
		sModel);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		sModel,
		sLot);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		sModel,
		sLot,
		sLayer);
	//if (!finder.FindFile(sPath))
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::MakeMkDir()
{
	pView->OpenReelmap();
	pView->OpenReelmapInner();
	return TRUE;
}

BOOL CGvisR2R_PunchDoc::ChkLotEnd(CString sPath)
{
	if (!m_pFile)
		return FALSE;

	return (m_pFile->ChkLotEnd(sPath));
}

BOOL CGvisR2R_PunchDoc::IsPinMkData()
{
	if (m_pSpecLocal)
		return m_pSpecLocal->IsPinMkData();

	return FALSE;
}

BOOL CGvisR2R_PunchDoc::IsPinData()
{
	if (m_pSpecLocal)
		return m_pSpecLocal->IsPinData();

	return FALSE;
}

CString CGvisR2R_PunchDoc::GetProcessNum()
{
	CString sPath = WorkingInfo.System.sPathAoiUpCurrInfo;
	TCHAR szData[200];
	CString sMsg, sCode = _T("");
#ifndef TEST_MODE
	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Process Unit Code"), NULL, szData, sizeof(szData), sPath))
			sCode = CString(szData);
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 Infomation에 Process Unit Code 정보가 없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	return sCode;
}

BOOL CGvisR2R_PunchDoc::GetEngOffset(CfPoint &OfSt)
{
	CString sPath = WorkingInfo.System.sPathEngOffset;
	TCHAR szData[200];
	BOOL bRtn = TRUE;

#ifndef TEST_MODE
	CString sMsg;
	CFileFind finder;
	if (finder.FindFile(sPath))
	{

		if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN X"), NULL, szData, sizeof(szData), sPath))
			OfSt.x = _tstof(szData);
		else
		{
			OfSt.x = 0.0;
			bRtn = FALSE;
		}

		if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN Y"), NULL, szData, sizeof(szData), sPath))
			OfSt.y = _tstof(szData);
		else
		{
			OfSt.y = 0.0;
			bRtn = FALSE;
		}
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 OFFSET에 ALIGN 정보가  없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	return bRtn;
}

BOOL CGvisR2R_PunchDoc::GetAoiUpOffset(CfPoint &OfSt)
{
	CString sPath = WorkingInfo.System.sPathAoiUpOffset;
	TCHAR szData[200];
	BOOL bRtn = TRUE;

#ifndef TEST_MODE
	CString sMsg;
	CFileFind finder;
	if (finder.FindFile(sPath))
	{

		if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN X"), NULL, szData, sizeof(szData), sPath))
			OfSt.x = _tstof(szData);
		else
		{
			OfSt.x = 0.0;
			bRtn = FALSE;
		}

		if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN Y"), NULL, szData, sizeof(szData), sPath))
			OfSt.y = _tstof(szData);
		else
		{
			OfSt.y = 0.0;
			bRtn = FALSE;
		}
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 OFFSET에 ALIGN 정보가  없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	return bRtn;
}

BOOL CGvisR2R_PunchDoc::GetAoiDnOffset(CfPoint &OfSt)
{
	CString sPath = WorkingInfo.System.sPathAoiDnOffset;
	TCHAR szData[200];
	BOOL bRtn = TRUE;

#ifndef TEST_MODE
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN X"), NULL, szData, sizeof(szData), sPath))
		OfSt.x = -1.0*_tstof(szData);
		else
		{
			OfSt.x = 0.0;
			bRtn = FALSE;
		}

		if (0 < ::GetPrivateProfileString(_T("OFFSET"), _T("ALIGN Y"), NULL, szData, sizeof(szData), sPath))
			OfSt.y = _tstof(szData);
		else
		{
			OfSt.y = 0.0;
			bRtn = FALSE;
		}
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 Region에 Piece Region Type 정보가 없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	return bRtn;
}

CString CGvisR2R_PunchDoc::GetMin(int nDlgId, int nCtrlId)
{
	if (nDlgId == IDD_DLG_INFO)
	{
		switch (nCtrlId)
		{
		case IDC_STC_174:
			return _T("20");
		case IDC_STC_61:
			return _T("0");
		}
	}

	if (nDlgId == IDD_DLG_MENU_04)
	{
		switch (nCtrlId)
		{
		case IDC_STC_010:
			return _T("30");
		case IDC_STC_007:
			return _T("30");
		case IDC_STC_25:
			return _T("30");
		case IDC_STC_148:
			return _T("-12");
		case IDC_STC_152:
			return _T("-12");
		case IDC_STC_48:
			return _T("-5");
		case IDC_STC_93:
			return _T("-5");
		case IDC_STC_58:
			return _T("-5");
		case IDC_STC_113:
			return _T("-5");
		case IDC_STC_52:
			return _T("0");
		case IDC_STC_62:
			return _T("0");
		case IDC_STC_117:
			return _T("0");
		case IDC_STC_55:
			return _T("0");
		case IDC_STC_110:
			return _T("0");
		case IDC_STC_65:
			return _T("0");
		case IDC_STC_120:
			return _T("0");
		case IDC_STC_68:
			return _T("0");
		case IDC_STC_123:
			return _T("0");
		case IDC_STC_97:
			return _T("0");
		}
	}

#ifdef USE_FLUCK
	if (nDlgId == IDD_DLG_UTIL_06)
	{
		switch (nCtrlId)
		{
		case IDC_STC_132:
			return _T("-5");
		case IDC_STC_136:
			return _T("0");
		case IDC_STC_139:
			return _T("0");

		case IDC_STC_143:
			return _T("-5");
		case IDC_STC_147:
			return _T("0");
		case IDC_STC_150:
			return _T("0");
		case IDC_STC_153:
			return _T("0");


		case IDC_STC_93:
			return _T("-5");
		case IDC_STC_97:
			return _T("0");
		case IDC_STC_110:
			return _T("0");

		case IDC_STC_113:
			return _T("-5");
		case IDC_STC_117:
			return _T("0");
		case IDC_STC_120:
			return _T("0");
		case IDC_STC_123:
			return _T("0");
		}
	}
#endif

	return _T("");
}

CString CGvisR2R_PunchDoc::GetMax(int nDlgId, int nCtrlId)
{
	CString sVal;

	if (nDlgId == IDD_DLG_INFO)
	{
		switch (nCtrlId)
		{
		case IDC_STC_174:
			return _T("100");
		case IDC_STC_61:
			sVal.Format(_T("%d"), FIX_PCS_SHOT_MAX);
			return sVal;
		}
	}

	if (nDlgId == IDD_DLG_MENU_04)
	{
		switch (nCtrlId)
		{
		case IDC_STC_010:
			return _T("100000000");
		case IDC_STC_007:
			return _T("1000");
		case IDC_STC_25:
			return _T("300");
		case IDC_STC_148:
			return _T("12");
		case IDC_STC_152:
			return _T("12");
		case IDC_STC_48:
			return _T("20");
		case IDC_STC_93:
			return _T("20");
		case IDC_STC_58:
			return _T("25");
			//return _T("10";
		case IDC_STC_113:
			return _T("25");
			//return _T("10";
		case IDC_STC_52:
			return _T("2000");
		case IDC_STC_97:
			return _T("2000");
		case IDC_STC_62:
			return _T("2000");
		case IDC_STC_117:
			return _T("2000");
		case IDC_STC_55:
			return _T("20000");
		case IDC_STC_110:
			return _T("20000");
		case IDC_STC_65:
			return _T("20000");
		case IDC_STC_120:
			return _T("20000");
		case IDC_STC_68:
			return _T("1900");
		case IDC_STC_123:
			return _T("1900");

		}
	}
#ifdef USE_FLUCK
	if (nDlgId == IDD_DLG_UTIL_06)
	{
		switch (nCtrlId)
		{
		case IDC_STC_132:
			return _T("10");
		case IDC_STC_136:
			return _T("2000");
		case IDC_STC_139:
			return _T("20000");

		case IDC_STC_143:
			return _T("10");
		case IDC_STC_147:
			return _T("2000");
		case IDC_STC_150:
			return _T("20000");
		case IDC_STC_153:
			return _T("1900");


		case IDC_STC_93:
			return _T("10");
		case IDC_STC_97:
			return _T("2000");
		case IDC_STC_110:
			return _T("20000");

		case IDC_STC_113:
			return _T("10");
		case IDC_STC_117:
			return _T("2000");
		case IDC_STC_120:
			return _T("20000");
		case IDC_STC_123:
			return _T("1900");
		}
	}
#endif

	return _T("");
}

BOOL CGvisR2R_PunchDoc::MakeMkDir(stModelInfo stInfo)
{
	CString sMsg = _T("");
	CFileFind finder;
	CString sPath;

	sPath.Format(_T("%s"), pDoc->WorkingInfo.System.sPathOldFile);
	int pos = sPath.ReverseFind('\\');
	if (pos != -1)
		sPath.Delete(pos, sPath.GetLength() - pos);

	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if (stInfo.sModel.IsEmpty() || stInfo.sLot.IsEmpty() || stInfo.sLayer.IsEmpty())
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("모델이나 로뜨 또는 레이어명이 없습니다."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s%s"), pDoc->WorkingInfo.System.sPathOldFile,
		stInfo.sModel);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		stInfo.sModel,
		stInfo.sLot);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
		stInfo.sModel,
		stInfo.sLot,
		stInfo.sLayer);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s\\Punching"), pDoc->WorkingInfo.System.sPathOldFile,
		stInfo.sModel,
		stInfo.sLot,
		stInfo.sLayer);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::GetPcrInfo(CString sPath, stModelInfo &stInfo)
{

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp;// , i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;


	//strcpy(FileD, sPath);
	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		//FileData = (char*)malloc( nFileSize );
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		//strFileData.Format(_T("%s"), CharToString(FileData));
		strFileData = CharToString(FileData);
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(FALSE);
	}

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(_T(','), 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Model
	nTemp = strFileData.Find(_T(','), 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Layer
	nTemp = strFileData.Find(_T(','), 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	// Lot
	nTemp = strFileData.Find(_T(','), 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	//Status.PcrShare[1].sLot = strLot;

	// Its Code
	nTemp = strFileData.Find(_T('\n'), 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	nTemp = strFileData.Find(_T('\n'), 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);

	stInfo.sModel = strModel;
	stInfo.sLayer = strLayer;
	stInfo.sLot = strLot;
	stInfo.sItsCode = sItsCode;

	return TRUE;
}

void CGvisR2R_PunchDoc::UpdateProcessNum(CString sProcessNum)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	pDoc->m_pReelMapUp->UpdateProcessNum(sProcessNum, 0); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	if (bDualTest)
	{
		pDoc->m_pReelMapDn->UpdateProcessNum(sProcessNum, 1); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		pDoc->m_pReelMapAllUp->UpdateProcessNum(sProcessNum, 2); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
		pDoc->m_pReelMapAllDn->UpdateProcessNum(sProcessNum, 3); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
	}
	//pDoc->m_pReelMap->UpdateProcessNum(sProcessNum, pView->m_nSelRmap); // [0]:AOI-Up , [1]:AOI-Dn , [2]:AOI-AllUp , [3]:AOI-AllDn
}


void CGvisR2R_PunchDoc::SetProbWaitPos(int nProb, double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sWaitPos = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_POS"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[0].sWaitPos = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_POS"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbWaitPos(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sWaitPos));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[1].sWaitPos));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbWaitVel(int nProb, double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sWaitVel = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_VEL"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sWaitVel = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_VEL"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbWaitVel(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Marking[0].sWaitVel));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Marking[1].sWaitVel));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbWaitAcc(int nProb, double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sWaitAcc = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_WAIT_ACC"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sWaitAcc = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_WAIT_ACC"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbWaitAcc(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sWaitAcc));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[1].sWaitAcc));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbingPos(int nProb, double dPos)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPos);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sProbingPos = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_POS"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sProbingPos = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_POS"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbingPos(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sProbingPos));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[1].sProbingPos));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbingVel(int nProb, double dVel)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dVel);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sProbingVel = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_VEL"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sProbingVel = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_VEL"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbingVel(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sProbingVel));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[0].sProbingVel));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbingAcc(int nProb, double dAcc)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dAcc);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sProbingAcc = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_ACC"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sProbingAcc = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_PROBING_ACC"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbingAcc(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sProbingAcc));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[1].sProbingAcc));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbingToq(int nProb, double dToq)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dToq);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sProbingToq = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_TOQ"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[0].sProbingToq = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_PROBING_TOQ"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbingToq(int nProb)
{
	double dVal;
	if (nProb == 0)
		dVal = (_tstof(WorkingInfo.Probing[0].sProbingToq));
	else if (nProb == 1)
		dVal = (_tstof(WorkingInfo.Probing[1].sProbingToq));
	return dVal;
}

void CGvisR2R_PunchDoc::SetProbPosX(int nProb, double dPosX)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosX);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sMeasurePosX = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSX"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sMeasurePosX = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSX"), sData, sPath);
	}
}

void CGvisR2R_PunchDoc::SetProbPosY(int nProb, double dPosY)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dPosY);
	if (nProb == 0)
	{
		WorkingInfo.Probing[0].sMeasurePosY = sData;
		::WritePrivateProfileString(_T("Probing0"), _T("PROBING_MEASURE_POSY"), sData, sPath);
	}
	else if (nProb == 1)
	{
		WorkingInfo.Probing[1].sMeasurePosY = sData;
		::WritePrivateProfileString(_T("Probing1"), _T("PROBING_MEASURE_POSY"), sData, sPath);
	}
}

double CGvisR2R_PunchDoc::GetProbPosX(int nProb)
{
	double dPos;
	if (nProb == 0)
		dPos = (_tstof(WorkingInfo.Probing[0].sMeasurePosX));
	else if (nProb == 1)
		dPos = (_tstof(WorkingInfo.Probing[1].sMeasurePosX));
	return dPos;
}

double CGvisR2R_PunchDoc::GetProbPosY(int nProb)
{
	double dPos;
	if (nProb == 0)
		dPos = (_tstof(WorkingInfo.Probing[0].sMeasurePosY));
	else if (nProb == 1)
		dPos = (_tstof(WorkingInfo.Probing[1].sMeasurePosY));
	return dPos;
}

void CGvisR2R_PunchDoc::SetProbingThreshold(double dThreshold)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dThreshold);
	WorkingInfo.Fluck.sThreshold = sData;
	::WritePrivateProfileString(_T("Fluck"), _T("PROBING_THRESHOLD"), sData, sPath);
}

double CGvisR2R_PunchDoc::GetProbingThreshold()
{
	return (_tstof(WorkingInfo.Fluck.sThreshold));
}

void CGvisR2R_PunchDoc::SetStripRejectMkNum(int nNum)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%d"), nNum);
	WorkingInfo.Fluck.sRejectMkNum = sData;
	::WritePrivateProfileString(_T("Fluck"), _T("PROBING_REJECT_MK"), sData, sPath);
}

int CGvisR2R_PunchDoc::GetStripRejectMkNum()
{
	return (_tstoi(WorkingInfo.Fluck.sRejectMkNum));
}

int CGvisR2R_PunchDoc::GetLastShotEngrave()
{
	int nLastShot = 0;
	if (pView->m_pDlgFrameHigh)
		nLastShot = pView->m_pDlgFrameHigh->m_nEngraveLastShot;

	return (nLastShot);
}

void CGvisR2R_PunchDoc::WriteElecData(CString sData)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CString strDestPath, sLot, sPnl;
	int nSerial;

	sLot = WorkingInfo.LastJob.sLot;
	if (WorkingInfo.System.sPathOldFile.Right(1) != "\\")
		strDestPath.Format(_T("%s\\%s\\%s\\%s.txt"), WorkingInfo.System.sPathOldFile, WorkingInfo.LastJob.sModel,
			sLot, sLot);
	else
		strDestPath.Format(_T("%s%s\\%s\\%s.txt"), WorkingInfo.System.sPathOldFile, WorkingInfo.LastJob.sModel,
			sLot, sLot);
	nSerial = pView->m_nBufUpSerial[1];

	sPnl.Format(_T("%04d"), nSerial);
	::WritePrivateProfileString( sLot, sPnl, sData, strDestPath );
}

double CGvisR2R_PunchDoc::GetEngraveAoiDist()
{
	return (_tstof(WorkingInfo.Motion.sFdEngraveAoiInitDist));
}

void CGvisR2R_PunchDoc::SetEngraveAoiDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.sFdEngraveAoiInitDist = sData;
	::WritePrivateProfileString(_T("Motion"), _T("FEEDING_ENGRAVE_AOI_INIT_DIST"), sData, sPath);
	long lData = (long)(dLen * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedLengthFromEngraveToAoi, lData);	// 각인부에서 AOI(상)까지 거리 (단위 mm * 1000)
}

void CGvisR2R_PunchDoc::SetEngraveReaderDist(double dLen)
{
	CString sData, sPath = PATH_WORKING_INFO;
	sData.Format(_T("%.3f"), dLen);
	WorkingInfo.Motion.s2DEngLen = WorkingInfo.Motion.sEngraveFdBarcodeOffset = sData;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_BARCODE_OFFSET"), sData, sPath);
	long lData = (long)(dLen * 1000.0);
	// 한 샷 이동시 기구적으로 고정된 위치임.
	//pView->MpeWrite(pView->Plc.DlgMenu04.FeedLengthFromLaserToReader, lData);	// 각인위치에서 2D 바코드 리더기까지의 Offset (단위 mm * 1000)
}

double CGvisR2R_PunchDoc::GetEngraveReaderDist()
{
	return (_tstof(WorkingInfo.Motion.sEngraveFdBarcodeOffset));
}

void CGvisR2R_PunchDoc::SetAoiReaderDist(double dLen)
{
	//CString sData, sPath = PATH_WORKING_INFO;
	//sData.Format(_T("%.3f"), dLen);
	//WorkingInfo.Motion.sAoiFdBarcodeOffset = sData;
	//::WritePrivateProfileString(_T("Motion"), _T("AOI_BARCODE_OFFSET"), sData, sPath);
	//long lData = (long)(dLen * 1000.0);
	//pView->MpeWrite(_T("ML45092"), lData);	// AOI 검사위치에서 2D 바코드 리더기까지의 Offset (단위 mm * 1000)
}

double CGvisR2R_PunchDoc::GetAoiReaderDist()
{
	return (_tstof(WorkingInfo.Motion.sAoiFdBarcodeOffset));
}

void CGvisR2R_PunchDoc::SetMkReaderDist(double dLen)
{
	//CString sData, sPath = PATH_WORKING_INFO;
	//sData.Format(_T("%.3f"), dLen);
	//WorkingInfo.Motion.sMkFdBarcodeOffset = sData;
	//::WritePrivateProfileString(_T("Motion"), _T("PUNCHING_BARCODE_OFFSET"), sData, sPath);
	//long lData = (long)(dLen * 1000.0);
	//pView->MpeWrite(_T("ML45094"), lData);	// Punching 시작위치에서 2D 바코드 리더기까지의 Offset (단위 mm * 1000)
}

double CGvisR2R_PunchDoc::GetMkReaderDist()
{
	return (_tstof(WorkingInfo.Motion.sMkFdBarcodeOffset));
}

void CGvisR2R_PunchDoc::Set2DReaderPosMoveVel(double dVel)
{
	//CString sPath = PATH_WORKING_INFO;
	//CString sVal;
	//sVal.Format(_T("%.3f"), dVel);
	//WorkingInfo.Motion.sFdBarcodeOffsetVel = sVal;
	//::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_VEL"), sVal, sPath);
	//long lData = (long)(dVel * 1000.0);
	//pView->MpeWrite(pView->Plc.DlgMenu04.FeedSpeed2dCode, lData);	// 2D 바코드 리더기위치까지 Feeding 속도 (단위 mm/sec * 1000)
}

void CGvisR2R_PunchDoc::Set2DReaderPosMoveAcc(double dAcc)
{
	//CString sPath = PATH_WORKING_INFO;
	//CString sVal;
	//sVal.Format(_T("%.3f"), dAcc);
	//WorkingInfo.Motion.sFdBarcodeOffsetAcc = sVal;
	//::WritePrivateProfileString(_T("Motion"), _T("MARKING_FEEDING_SERVO_ACC"), sVal, sPath);
	//long lData = (long)(dAcc * 1000.0);
	//pView->MpeWrite(pView->Plc.DlgMenu04.FeedAcc2dCode, lData);	// 2D 바코드 리더기위치까지 Feeding 가속도 (단위 mm/sec * 1000)
}

double CGvisR2R_PunchDoc::Get2DReaderPosMoveVel()
{
	return (_tstof(WorkingInfo.Motion.sFdBarcodeOffsetVel));
}

double CGvisR2R_PunchDoc::Get2DReaderPosMoveAcc()
{
	return (_tstof(WorkingInfo.Motion.sFdBarcodeOffsetAcc));
}

void CGvisR2R_PunchDoc::SetEngraveFdPitch(double dPitch)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dPitch);
	WorkingInfo.Motion.sEngraveFdLead = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_FEEDING_DRUM_LEAD_PITCH"), sVal, sPath);
	long lData = (long)(dPitch * 1000.0);
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedLeadPitchEngrave, lData);	// 각인부 Feeding 롤러 Lead Pitch (단위 mm * 1000)
}

double CGvisR2R_PunchDoc::GetEngraveFdPitch()
{
	return (_tstof(WorkingInfo.Motion.sEngraveFdLead));
}

void CGvisR2R_PunchDoc::SetEngraveFdErrLmt(double dLmt)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dLmt);
	WorkingInfo.Motion.sEngraveLmtFdErr = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_ERROR_VAL"), sVal, sPath);
}

double CGvisR2R_PunchDoc::GetEngraveFdErrLmt()
{
	return (_tstof(WorkingInfo.Motion.sEngraveLmtFdErr));
}

void CGvisR2R_PunchDoc::SetEngraveFdErrRng(double dRng)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dRng);
	WorkingInfo.Motion.sEngraveLmtFdAdjOffSet = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OFFSET"), sVal, sPath);
}

double CGvisR2R_PunchDoc::GetEngraveFdErrRng()
{
	return (_tstof(WorkingInfo.Motion.sEngraveLmtFdAdjOffSet));
}

void CGvisR2R_PunchDoc::SetEngraveFdErrNum(int nNum)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%d"), nNum);
	WorkingInfo.Motion.sEngraveLmtFdOvrNum = sVal;
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_ADJUST_LIMIT_FEEDING_OVER_NUM"), sVal, sPath);
}

int CGvisR2R_PunchDoc::GetEngraveFdErrNum()
{
	return (_tstoi(WorkingInfo.Motion.sEngraveLmtFdOvrNum));
}


double CGvisR2R_PunchDoc::GetEngraveBuffInitPos()
{
	return (_tstof(WorkingInfo.Motion.sEngraveStBufPos));
}

void CGvisR2R_PunchDoc::SetEngraveToq(double dToq)
{
	CString sPath = PATH_WORKING_INFO;
	CString sVal;
	sVal.Format(_T("%.3f"), dToq);
	WorkingInfo.Motion.sEngraveTq = sVal;
	long lData = (long)(_tstof(WorkingInfo.Motion.sEngraveTq) * 1000.0);
	::WritePrivateProfileString(_T("Motion"), _T("ENGRAVE_TENSION_SERVO_TORQUE"), sVal, sPath);
	//MpeWrite(pView->Plc.DlgMenu04.FeedTorqueCwEngrave, lData);	// 각인부 Tension 모터 토크값 (단위 Kgf * 1000)
	pView->MpeWrite(pView->Plc.DlgMenu04.FeedTorqueCcwEngrave, lData);	// 각인부 Tension 모터 토크값 (단위 Kgf * 1000)
}


BOOL CGvisR2R_PunchDoc::DirectoryExists(LPCTSTR szPath)
{
	DWORD dwAttrib = GetFileAttributes(szPath);

	return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
		(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

int CGvisR2R_PunchDoc::GetTestMode()
{
	return WorkingInfo.LastJob.nTestMode;
}

void CGvisR2R_PunchDoc::SetTestMode(int nMode)
{
	//if (!pView->m_bJoinContinue)
	{
		WorkingInfo.LastJob.nTestMode = nMode; // MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2 .

		CString sData;
		sData.Format(_T("%d"), nMode);
		::WritePrivateProfileString(_T("Last Job"), _T("Test Mode"), sData, PATH_WORKING_INFO);

		CString sPath = WorkingInfo.System.sPathMkCurrInfo;

		if (sPath.IsEmpty())
			return;

		::WritePrivateProfileString(_T("Infomation"), _T("Test Mode"), sData, sPath);

		::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), sPath);
		::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);

		if (pView && pView->m_pMpe)
		{
			if (pDoc->GetTestMode() == MODE_INNER)
			{
				pView->MpeWrite(pView->Plc.DlgInfo.ModeInner, 1);// 내층 검사 사용/미사용 
				pView->MpeWrite(pView->Plc.DlgInfo.ModeOutter, 0);// 외층 검사 사용/미사용
				pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), TRUE);
				pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), FALSE);
				if (pView->m_pDlgMenu01)
					pView->m_pDlgMenu01->EnableItsMode(FALSE);
			}
			else if (pDoc->GetTestMode() == MODE_OUTER)
			{
				pView->MpeWrite(pView->Plc.DlgInfo.ModeInner, 0);// 내층 검사 사용/미사용
				pView->MpeWrite(pView->Plc.DlgInfo.ModeOutter, 1);// 외층 검사 사용/미사용
				pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), FALSE);
				pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), TRUE);
				if (pView->m_pDlgMenu01)
					pView->m_pDlgMenu01->EnableItsMode();
			}
			else
			{
				pView->MpeWrite(pView->Plc.DlgInfo.ModeInner, 0);// 내층 검사 사용/미사용
				pView->MpeWrite(pView->Plc.DlgInfo.ModeOutter, 0);// 외층 검사 사용/미사용
				pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), FALSE);
				pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), FALSE);
				if (pView->m_pDlgMenu01)
					pView->m_pDlgMenu01->EnableItsMode(FALSE);
			}
		}
	}

}


void CGvisR2R_PunchDoc::SetEngItsCode(CString sItsCode)
{
	m_sItsCode = WorkingInfo.LastJob.sEngItsCode = sItsCode;
	::WritePrivateProfileString(_T("Last Job"), _T("Engrave Its Code"), sItsCode, PATH_WORKING_INFO);
}

void CGvisR2R_PunchDoc::SetCurrentInfoSignal(int nMsgID, BOOL bOn)
{
	CString sData, sIdx, sPath = WorkingInfo.System.sPathMkSignalInfo;

	if (sPath.IsEmpty())
		return;

	sIdx.Format(_T("%d"), nMsgID);
	sData.Format(_T("%d"), bOn ? 1 : 0);
	::WritePrivateProfileString(_T("Signal"), sIdx, sData, sPath);

	if (nMsgID == _SigInx::_MyMsgYes || nMsgID == _SigInx::_MyMsgNo || nMsgID == _SigInx::_MyMsgOk)
		return;

	pView->CheckCurrentInfoSignal(nMsgID, (bOn ? 1 : 0));
}


BOOL CGvisR2R_PunchDoc::GetCurrentInfoSignal(int nMsgID)
{
	TCHAR szData[200];
	CString sData, sIdx, sPath = WorkingInfo.System.sPathEngSignalInfo;

	if (sPath.IsEmpty())
		return FALSE;
#ifndef TEST_MODE
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		sIdx.Format(_T("%d"), nMsgID);
		if (0 < ::GetPrivateProfileString(_T("Signal"), sIdx, NULL, szData, sizeof(szData), sPath))
			return (_ttoi(szData) > 0 ? TRUE : FALSE);
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 Signal에 인덱스 정보가 없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	return FALSE;
}

void CGvisR2R_PunchDoc::SetLastSerialEng(int nSerial)
{
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.24-1"));
		return;
	}

	if (nSerial > 0)
	{
		CString str, sPath = PATH_WORKING_INFO;
		str.Format(_T("%d"), nSerial);
		WorkingInfo.LastJob.sSerialEng = str;
		::WritePrivateProfileString(_T("Last Job"), _T("Last Serial Eng"), str, sPath);
	}
}


BOOL CGvisR2R_PunchDoc::GetCurrentInfoEng()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	CString sPath = WorkingInfo.System.sPathEngCurrInfo;
	TCHAR szData[512];
	BOOL bRtn = FALSE;

#ifdef TEST_MODE
	return bRtn;
#endif
	if (sPath.IsEmpty() || (GetTestMode() != MODE_INNER && GetTestMode() != MODE_OUTER))
		return bRtn;
	
#ifndef TEST_MODE
	CString sMsg;
	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Dual Test"), NULL, szData, sizeof(szData), sPath))
			m_bEngDualTest = _ttoi(szData) > 0 ? TRUE : FALSE;

		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Its Code"), NULL, szData, sizeof(szData), sPath))
			m_sItsCode = CString(szData);

		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Lot"), NULL, szData, sizeof(szData), sPath))
			m_sEngLotNum = CString(szData);

		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Process Unit Code"), NULL, szData, sizeof(szData), sPath))
			m_sEngProcessNum = CString(szData);

		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Model Up"), NULL, szData, sizeof(szData), sPath))
			m_sEngModel = CString(szData);

		if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Layer Up"), NULL, szData, sizeof(szData), sPath))
			m_sEngLayerUp = CString(szData);

		if (bDualTest)
		{
			if (0 < ::GetPrivateProfileString(_T("Infomation"), _T("Current Layer Dn"), NULL, szData, sizeof(szData), sPath))
				m_sEngLayerDn = CString(szData);
		}

		if (m_sItsCode.IsEmpty() || m_sEngLotNum.IsEmpty() || m_sEngModel.IsEmpty() || m_sEngLayerUp.IsEmpty())
			return bRtn;
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 Infomation에 정보가 없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	return TRUE;
}

int CGvisR2R_PunchDoc::GetCurrentInfoEngShotNum()
{
	CString sPath = WorkingInfo.System.sPathEngCurrInfo;
	TCHAR szData[512];
#ifdef TEST_MODE
	return 0;
#endif

	if (sPath.IsEmpty())
		return 0;

	CString sMsg;

#ifndef TEST_MODE
	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Work"), _T("Eng Shot Num"), NULL, szData, sizeof(szData), sPath))
			return(_ttoi(szData));
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 Work에 Shot Num 정보가 없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	return 0;
}

int CGvisR2R_PunchDoc::GetCurrentInfoReadShotNum()
{
	CString sPath = WorkingInfo.System.sPathEngCurrInfo;
	TCHAR szData[512];
#ifdef TEST_MODE
	return 0;
#endif

	if (sPath.IsEmpty())
		return 0;

	CString sMsg;

#ifndef TEST_MODE
	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Work"), _T("Read Shot Num"), NULL, szData, sizeof(szData), sPath))
			return(_ttoi(szData));
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 Work에 Shot Num 정보가 없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
	}
#endif
	return 0;
}



void CGvisR2R_PunchDoc::SetCurrentInfoBufUpTot(int nTotal)
{
	if (pView->m_bShift2Mk)
		return;

	CString sPath = WorkingInfo.System.sPathMkCurrInfoBuf;
	TCHAR szData[512];
	CString sData = _T("");

	if (sPath.IsEmpty())
		return;

	sData.Format(_T("%d"), nTotal);
	::WritePrivateProfileString(_T("Up"), _T("Total"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetCurrentInfoBufUp(int nIdx, int nData)
{
	if (pView->m_bShift2Mk)
		return;

	CString sPath = WorkingInfo.System.sPathMkCurrInfoBuf;
	TCHAR szData[512];
	CString sIdx, sData;

	if (sPath.IsEmpty())
		return;

	sIdx.Format(_T("%d"), nIdx);
	sData.Format(_T("%d"), nData);

	::WritePrivateProfileString(_T("Up"), sIdx, sData, sPath);
}

void CGvisR2R_PunchDoc::SetCurrentInfoBufDnTot(int nTotal)
{
	if (pView->m_bShift2Mk)
		return;

	CString sPath = WorkingInfo.System.sPathMkCurrInfoBuf;
	TCHAR szData[512];
	CString sData = _T("");

	if (sPath.IsEmpty())
		return;

	sData.Format(_T("%d"), nTotal);
	::WritePrivateProfileString(_T("Dn"), _T("Total"), sData, sPath);
}

void CGvisR2R_PunchDoc::SetCurrentInfoBufDn(int nIdx, int nData)
{
	if (pView->m_bShift2Mk)
		return;

	CString sPath = WorkingInfo.System.sPathMkCurrInfoBuf;
	TCHAR szData[512];
	CString sIdx, sData;

	if (sPath.IsEmpty())
		return;

	sIdx.Format(_T("%d"), nIdx);
	sData.Format(_T("%d"), nData);

	::WritePrivateProfileString(_T("Dn"), sIdx, sData, sPath);
}

//CString CGvisR2R_PunchDoc::GetMonDispMain()
//{
//	CString sPath = WorkingInfo.System.sPathMonDispMain;
//	TCHAR szData[512];
//
//	if (sPath.IsEmpty())
//		return _T("");
//
//	if (0 < ::GetPrivateProfileString(_T("Info"), _T("Disp"), NULL, szData, sizeof(szData), sPath))
//		return(CString(szData));
//
//	return _T("");
//}

void CGvisR2R_PunchDoc::SetMonDispMain(CString sDisp)
{
	CString sPath = WorkingInfo.System.sPathMonDispMain;
	if (sPath.IsEmpty())
		return;

	::WritePrivateProfileString(_T("Info"), _T("Disp"), sDisp, sPath);
}

void CGvisR2R_PunchDoc::SetMkMenu01(CString sMenu, CString sItem, CString sData)
{
	CString sPath = WorkingInfo.System.sPathMkMenu01;

	if (sPath.IsEmpty())
		return;

	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

void CGvisR2R_PunchDoc::SetMkMenu03(CString sMenu, CString sItem, BOOL bOn)
{
	CString sPath = WorkingInfo.System.sPathMkMenu03;
	CString sData = _T("");

	if (sPath.IsEmpty())
		return;

	sData.Format(_T("%d"), bOn > 0 ? 1 : 0);
	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

void CGvisR2R_PunchDoc::GetMkInfo()
{
	CString sPath = WorkingInfo.System.sPathMkInfo;
	TCHAR szData[512];

	if (sPath.IsEmpty())
		return;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DispContRun"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDispContRun = (_ttoi(szData) > 0) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bDispContRun = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DispLotEnd"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDispLotEnd = (_ttoi(szData) > 0) ? TRUE : FALSE;
	else
		WorkingInfo.LastJob.bDispLotEnd = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("Use Dual AOI"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDualTest = (_ttoi(szData) > 0) ? TRUE : FALSE;

	//WorkingInfo.LastJob.nTestMode = MODE_NONE;
	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("Inner Test On"), NULL, szData, sizeof(szData), sPath))
	{
		if ((_ttoi(szData) > 0) ? TRUE : FALSE)
			SetTestMode(MODE_INNER);	//WorkingInfo.LastJob.nTestMode = MODE_INNER;			
	}

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("Outer Test On"), NULL, szData, sizeof(szData), sPath))
	{
		if ((_ttoi(szData) > 0) ? TRUE : FALSE)
			SetTestMode(MODE_OUTER);	//WorkingInfo.LastJob.nTestMode = MODE_OUTER;
	}

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("Sample Test On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bSampleTest = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("UncoilerCcw"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bFeedCcwUncoiler = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("RecoilerCcw"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bFeedCcwRecoiler = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("2PtAlign"), NULL, szData, sizeof(szData), sPath))
	{
		if ((_ttoi(szData) > 0) ? TRUE : FALSE)
			WorkingInfo.LastJob.nAlignMethode = TWO_POINT;
	}

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("4PtAlign"), NULL, szData, sizeof(szData), sPath))
	{
		if ((_ttoi(szData) > 0) ? TRUE : FALSE)
			WorkingInfo.LastJob.nAlignMethode = FOUR_POINT;
		else
			WorkingInfo.LastJob.nAlignMethode = TWO_POINT;
	}
	else
		WorkingInfo.LastJob.nAlignMethode = TWO_POINT;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensRecoil"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bRclDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensPunch"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bMkDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensAoiUp"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bAoiUpDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensAoiDn"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bAoiDnDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensEngrave"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bEngvDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("DoorSensUncoil"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bUclDrSen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("SaftySensPunch"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bMkSftySen = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("CleanRolerAoiUp"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("CleanRolerAoiDn"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("UltrasonicAoi"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("UltrasonicEngrave"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("TempStop"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bTempPause = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("SeparateLot"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bLotSep = (_ttoi(szData) > 0) ? TRUE : FALSE;

	if (0 < ::GetPrivateProfileString(_T("Signal"), _T("FixBed"), NULL, szData, sizeof(szData), sPath))
		pDoc->WorkingInfo.LastJob.bContFixDef = (_ttoi(szData) > 0) ? TRUE : FALSE;

}

void CGvisR2R_PunchDoc::SetMkInfo(CString sMenu, CString sItem, BOOL bOn)
{
	CString sPath = WorkingInfo.System.sPathMkInfo;
	CString sData = _T("");

	if (sPath.IsEmpty())
		return;

	sData.Format(_T("%d"), bOn > 0 ? 1 : 0);
	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

void CGvisR2R_PunchDoc::SetMkInfo(CString sMenu, CString sItem, CString sData)
{
	CString sPath = WorkingInfo.System.sPathMkInfo;

	if (sPath.IsEmpty())
		return;

	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

int CGvisR2R_PunchDoc::MirrorLR(int nPcsId) // 좌우 미러링
{
#ifndef USE_CAM_MASTER
	return 0;
#endif

	int nId, nCol, nRow, nC, nR;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol; // 1 ~
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow; // 1 ~

	nCol = int(nPcsId / nNodeY); // 0 ~
	if (nCol % 2)
		nRow = nNodeY*(nCol + 1) - nPcsId - 1;
	else
		nRow = nPcsId - nNodeY*nCol; // 0 ~

	nR = nRow; // 0 ~
	nC = (nNodeX - 1) - nCol; // 0 ~
	if (nC % 2) // 홀수 : 시작 감소
		nId = nNodeY*(nC + 1) - nR - 1;
	else		// 짝수 : 시작 증가
		nId = nR + nNodeY*nC;

	return nId;
}

int CGvisR2R_PunchDoc::MirrorUD(int nPcsId) // 상하 미러링
{
#ifndef USE_CAM_MASTER
	return 0;
#endif

	int nId, nCol, nRow, nC, nR;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol; // 1 ~
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow; // 1 ~

	nCol = int(nPcsId / nNodeY); // 0 ~
	if (nCol % 2)
		nRow = nPcsId - nNodeY*nCol; // 0 ~
	else
		nRow = nNodeY*(nCol + 1) - nPcsId - 1;

	nR = nRow; // 0 ~
	nC = nCol; // 0 ~
	if (nC % 2) // 홀수 : 시작 감소
		nId = nNodeY*(nC + 1) - nR - 1;
	else		// 짝수 : 시작 증가
		nId = nR + nNodeY*nC;

	return nId;
}

int CGvisR2R_PunchDoc::Rotate180(int nPcsId) // 180도 회전
{
#ifndef USE_CAM_MASTER
	return 0;
#endif

	int nId, nCol, nRow, nC, nR;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol; // 1 ~
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow; // 1 ~

	if (nNodeX % 2)		// 홀수 : 시작 감소
	{
		nCol = (nNodeX - 1) - int(nPcsId / nNodeY); // 0 ~
		if (nCol % 2)
			nRow = nPcsId - nNodeY * (nNodeX - nCol - 1); // 0 ~
		else
			nRow = nNodeY * (nNodeX - nCol) - nPcsId - 1; // 0 ~

		//nR = (nNodeY - 1) - nRow; // 0 ~
		//nC = (nNodeX - 1) - nCol; // 0 ~
		nR = nRow;
		nC = nCol;

		if (nC % 2)
			nId = nNodeY*(nC + 1) - nR - 1; // 0 ~
		else
			nId = nR + nNodeY*nC;
	}
	else				// 짝수 : 시작 증가
	{
		nCol = (nNodeX - 1) - int(nPcsId / nNodeY); // 0 ~
		if (nCol % 2)
			nRow = nNodeY * (nNodeX - nCol) - nPcsId - 1; // 0 ~
		else
			nRow = nPcsId - nNodeY * (nNodeX - nCol - 1); // 0 ~

		//nR = (nNodeY - 1) - nRow; // 0 ~
		//nC = (nNodeX - 1) - nCol; // 0 ~
		nR = nRow;
		nC = nCol;

		if (nC % 2)
			nId = nNodeY*(nC + 1) - nR - 1; // 0 ~
		else
			nId = nR + nNodeY*nC;
	}
	return nId;
}


BOOL CGvisR2R_PunchDoc::SetItsSerialInfo(int nItsSerial)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CString sName = _T("");
	CString str, sPath, Path[6];

	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->m_sEngModel;
	Path[2] = pDoc->m_sItsCode;
	Path[3] = pDoc->m_sEngLotNum;
	Path[4] = pDoc->m_sEngLayerUp;
	Path[5] = pDoc->m_sEngLayerDn;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty() || Path[3].IsEmpty() || Path[4].IsEmpty())
	{
		sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ITS_Code.txt
		str.Format(_T("It is trouble to write SetItsSerialInfo.txt\r\n%s"), sPath);
		pView->MsgBox(str);
		return FALSE;
	}

	if (bDualTest)
	{
		if (Path[5].IsEmpty())
			return FALSE;
	}

	sPath.Format(_T("%s%s"), Path[0], Path[1]);					// 모델 폴더
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);	// ITS 코드 폴더
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sName.Format(_T("%s.txt"), pDoc->m_sItsCode);
	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ITS_Code.txt

	CString sItsSerail;
	sItsSerail.Format(_T("%d"), nItsSerial);

	// ITS_Code.txt 파일의 정보
	str.Format(_T("%d"), pDoc->GetTestMode());
	::WritePrivateProfileString(sItsSerail, _T("Test Mode"), str, sPath);
	str.Format(_T("%d"), (bDualTest ? 1 : 0));
	::WritePrivateProfileString(sItsSerail, _T("Dual Test"), str, sPath);
	::WritePrivateProfileString(sItsSerail, _T("Process Unit Code"), pDoc->m_sEngProcessNum, sPath);
	::WritePrivateProfileString(sItsSerail, _T("Current Model"), pDoc->m_sEngModel, sPath);
	::WritePrivateProfileString(sItsSerail, _T("Its Code"), pDoc->m_sItsCode, sPath);
	::WritePrivateProfileString(sItsSerail, _T("Current Lot"), pDoc->m_sEngLotNum, sPath);
	::WritePrivateProfileString(sItsSerail, _T("Current Layer Up"), pDoc->m_sEngLayerUp, sPath);

	if (bDualTest)
		::WritePrivateProfileString(sItsSerail, _T("Current Layer Dn"), pDoc->m_sEngLayerDn, sPath);
	else
		::WritePrivateProfileString(sItsSerail, _T("Current Layer Dn"), _T(""), sPath);

	return TRUE;
}

int CGvisR2R_PunchDoc::SearchFirstShotOnIts()
{
	CString sName, sPath, Path[3], sMsg;

	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModel;
	Path[2] = pDoc->m_sItsCode;

	sName.Format(_T("%s.txt"), pDoc->m_sItsCode);
	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ITS_Code.txt

	CDataFile *pDataFile = new CDataFile;

	if (!pDataFile->Open(sPath))
	{
		Path[1] = pDoc->m_sEngModel;
		sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ITS_Code.txt

		if (!pDataFile->Open(sPath))
		{
			sMsg.Format(_T("%s File not found."), sPath);
			pView->MsgBox(sMsg);
			delete pDataFile;
			return 0;
		}
	}

	int i = 0, nLastShot = 0; 
	CString sLine;
	int nTotLine = pDataFile->GetTotalLines();
	for (i = 1; i <= nTotLine; i++)
	{
		sLine = pDataFile->GetLineString(i);
		int nPos = sLine.Find(_T('['), 0);
		if (nPos >= 0)
		{
			sLine.Delete(0, nPos+1);
			nPos = sLine.ReverseFind(_T(']'));
			sLine = sLine.Left(nPos);
			nLastShot = _ttoi(sLine);
			break;
		}
	}

	delete pDataFile;

	return nLastShot;
}

BOOL CGvisR2R_PunchDoc::GetItsSerialInfo(int nItsSerial, BOOL &bDualTest, CString &sLot, CString &sLayerUp, CString &sLayerDn, int nOption)		// 내층에서의 ITS 시리얼의 정보
{
	TCHAR szData[512];
	CString str, sName, sPath, Path[3];
	BOOL bRtn = TRUE;

	Path[0] = WorkingInfo.System.sPathItsFile;
	Path[1] = WorkingInfo.LastJob.sModel;
	Path[2] = m_sItsCode;

	sName.Format(_T("%s.txt"), pDoc->m_sItsCode);
	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ITS_Code.txt

	if (sPath.IsEmpty())
		return FALSE;

	CString strTemp;
	CFileFind finder;
	if (finder.FindFile(sPath) == FALSE)
	{
		Path[1] = m_sEngModel;
		sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ITS_Code.txt
		if (finder.FindFile(sPath) == FALSE)
		{
			strTemp.Format(_T("GetItsSerialInfo - Didn't find file.\r\n%s"), sPath);
			pView->MsgBox(strTemp);
			return FALSE;
		}
	}

	if (nItsSerial == 0)
		nItsSerial = SearchFirstShotOnIts();

	CString sItsSerail;
	sItsSerail.Format(_T("%d"), nItsSerial);


	// Option 1
	if (nOption == 0 || nOption == 1)
	{
		if (0 < ::GetPrivateProfileString(sItsSerail, _T("Dual Test"), NULL, szData, sizeof(szData), sPath))
		{
			bDualTest = (_ttoi(szData) > 0) ? TRUE : FALSE;
			WorkingInfo.LastJob.bDualTestInner = bDualTest;
		}
		else
			bRtn = FALSE;
	}

	// Option 2
	if (nOption == 0 || nOption == 2)
	{
		if (0 < ::GetPrivateProfileString(sItsSerail, _T("Current Lot"), NULL, szData, sizeof(szData), sPath))
		{
			sLot = CString(szData);
			WorkingInfo.LastJob.sInnerLot = sLot;
		}
		else
		{
			//bRtn = FALSE;
			strTemp.Format(_T("내층 작업정보에 %d 시리얼에 대한 정보가 없습니다.\r\n%s"), nItsSerial, sPath);
			pView->MsgBox(strTemp);
			return FALSE;
		}
	}

	// Option 3
	if (nOption == 0 || nOption == 3)
	{
		if (0 < ::GetPrivateProfileString(sItsSerail, _T("Current Layer Up"), NULL, szData, sizeof(szData), sPath))
		{
			sLayerUp = CString(szData);
			WorkingInfo.LastJob.sInnerLayerUp = sLayerUp;
		}
		else
			bRtn = FALSE;
	}

	// Option 4
	if (nOption == 0 || nOption == 4)
	{
		if (bDualTest)
		{
			if (0 < ::GetPrivateProfileString(sItsSerail, _T("Current Layer Dn"), NULL, szData, sizeof(szData), sPath))
			{
				sLayerDn = CString(szData);
				WorkingInfo.LastJob.sInnerLayerDn = sLayerDn;
			}
			else
				bRtn = FALSE;
		}
	}

	if (!bRtn)
	{
		strTemp.Format(_T("내층정보에 시리얼 (%d)이 없습니다.\r\n%s"), nItsSerial, sPath);
		pView->MsgBox(strTemp);
	}

	return bRtn;
}


char* CGvisR2R_PunchDoc::StrToChar(CString str) // char* returned must be deleted... 
{
	char*		szStr = NULL;
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 
	szStr = new char[nLenth];  //메모리 할당 

							   //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, szStr, nLenth, 0, 0);
	return szStr;
}

void CGvisR2R_PunchDoc::StrToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 

																			  //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}


BOOL CGvisR2R_PunchDoc::GetInnerYieldPath(int nItsSerial, CString  &sUp, CString &sDn, CString  &sAllUp, CString &sAllDn)
{
	BOOL bDualTest;
	CString sLot, sLayerUp, sLayerDn;
	if (!GetItsSerialInfo(nItsSerial, bDualTest, sLot, sLayerUp, sLayerDn, 0))
	{
		CString str;
		str.Format(_T("It is trouble to read GetItsSerialInfo()."));
		pView->MsgBox(str);
		return FALSE;
	}

	CString  Path[5];
	CString sPath = _T("");

	Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModel;
	Path[2] = sLot;
	Path[3] = sLayerUp;
	Path[4] = sLayerDn;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty() || Path[3].IsEmpty())
		return FALSE;

	sUp.Format(_T("%s%s\\%s\\%s\\YieldUp.txt"), Path[0], Path[1], Path[2], Path[3]); // ITS Inner Up Folder Path
	if (bDualTest)
		sAllUp.Format(_T("%s%s\\%s\\%s\\YieldAll.txt"), Path[0], Path[1], Path[2], Path[3]); // ITS Inner Up Folder Path

	if (bDualTest)
	{
		if (Path[4].IsEmpty())
			return FALSE;
		sDn.Format(_T("%s%s\\%s\\%s\\YieldDn.txt"), Path[0], Path[1], Path[2], Path[4]); // ITS Inner Dn Folder Path
		sAllDn.Format(_T("%s%s\\%s\\%s\\YieldAll.txt"), Path[0], Path[1], Path[2], Path[4]); // ITS Inner Dn Folder Path
	}
	else
	{
		sDn = _T("");
		sAllUp = _T("");
		sAllDn = _T("");
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::GetInnerReelmapPath(int nItsSerial, CString  &sUp, CString &sDn, CString  &sAllUp, CString &sAllDn)
{
	BOOL bDualTest;
	CString sLot, sLayerUp, sLayerDn;
	if (!GetItsSerialInfo(nItsSerial, bDualTest, sLot, sLayerUp, sLayerDn, 0))
	{
		CString str;
		str.Format(_T("It is trouble to read GetItsSerialInfo()."));
		pView->MsgBox(str);
		return FALSE;
	}

	CString  Path[5];
	CString sPath = _T("");

	Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModel;
	Path[2] = sLot;
	Path[3] = sLayerUp;
	Path[4] = sLayerDn;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty() || Path[3].IsEmpty())
		return FALSE;

	int nOffline = IsOfflineFolder(); // 0 : Not exist, 1 : Exist only Up, 2 : Exist only Dn, 3 : Exist Up and Dn

	if (nOffline & 0x01)
	{
		sUp.Format(_T("%s%s\\%s\\%s\\OFFLINE\\ReelMapDataUp.txt"), Path[0], Path[1], Path[2], Path[3]); // ITS Inner Up Folder Path
		if (bDualTest)
			sAllUp.Format(_T("%s%s\\%s\\%s\\OFFLINE\\ReelMapDataAll.txt"), Path[0], Path[1], Path[2], Path[3]); // ITS Inner Up Folder Path
	}
	else
	{
		sUp.Format(_T("%s%s\\%s\\%s\\ReelMapDataUp.txt"), Path[0], Path[1], Path[2], Path[3]); // ITS Inner Up Folder Path
		if (bDualTest)
			sAllUp.Format(_T("%s%s\\%s\\%s\\ReelMapDataAll.txt"), Path[0], Path[1], Path[2], Path[3]); // ITS Inner Up Folder Path
	}

	if (bDualTest)
	{
		if (Path[4].IsEmpty())
			return FALSE;
		if (nOffline & 0x02)
		{
			sDn.Format(_T("%s%s\\%s\\%s\\OFFLINE\\ReelMapDataDn.txt"), Path[0], Path[1], Path[2], Path[4]); // ITS Inner Dn Folder Path
			sAllDn.Format(_T("%s%s\\%s\\%s\\OFFLINE\\ReelMapDataAll.txt"), Path[0], Path[1], Path[2], Path[4]); // ITS Inner Dn Folder Path
		}
		else
		{
			sDn.Format(_T("%s%s\\%s\\%s\\ReelMapDataDn.txt"), Path[0], Path[1], Path[2], Path[4]); // ITS Inner Dn Folder Path
			sAllDn.Format(_T("%s%s\\%s\\%s\\ReelMapDataAll.txt"), Path[0], Path[1], Path[2], Path[4]); // ITS Inner Dn Folder Path
		}
	}
	else
	{
		sDn = _T("");
		sAllUp = _T("");
		sAllDn = _T("");
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::GetInnerFolderPath(int nItsSerial, CString  &sUp, CString &sDn)
{
	BOOL bDualTest;
	CString sLot, sLayerUp, sLayerDn;
	if (!GetItsSerialInfo(nItsSerial, bDualTest, sLot, sLayerUp, sLayerDn, 0))
	{
		CString str;
		str.Format(_T("It is trouble to read GetItsSerialInfo()."));
		pView->MsgBox(str);
		return FALSE;
	}

	CString  Path[5];
	CString sPath = _T("");

	Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModel;
	Path[2] = sLot;
	Path[3] = sLayerUp;
	Path[4] = sLayerDn;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty() || Path[3].IsEmpty())
		return FALSE;

	int nOffline = IsOfflineFolder(); // 0 : Not exist, 1 : Exist only Up, 2 : Exist only Dn, 3 : Exist Up and Dn

	if (nOffline & 0x01)
		sUp.Format(_T("%s%s\\%s\\%s\\OFFLINE\\"), Path[0], Path[1], Path[2], Path[3]); // ITS Inner Up Folder Path
	else
		sUp.Format(_T("%s%s\\%s\\%s\\"), Path[0], Path[1], Path[2], Path[3]); // ITS Inner Up Folder Path

	if (bDualTest)
	{
		if (Path[4].IsEmpty())
			return FALSE;
		if (nOffline & 0x02)
			sDn.Format(_T("%s%s\\%s\\%s\\OFFLINE\\"), Path[0], Path[1], Path[2], Path[4]); // ITS Inner Dn Folder Path
		else
			sDn.Format(_T("%s%s\\%s\\%s\\"), Path[0], Path[1], Path[2], Path[4]); // ITS Inner Dn Folder Path
	}
	else
		sDn = _T("");	

	return TRUE;
}

CString CGvisR2R_PunchDoc::GetItsFolderPath()
{
	CString  Path[3];
	CString sPath = _T("");

	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
	Path[1] = pDoc->WorkingInfo.LastJob.sModel;
	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;

	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
	{
		if (Path[2].IsEmpty())
		{
			if (pDoc->GetCurrentInfoEng())
			{
				pDoc->WorkingInfo.LastJob.sEngItsCode = m_sItsCode;
				Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
			}
		}

		if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
			return sPath;
	}

	sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]); // ITS Folder Path

	return sPath;
}

CString CGvisR2R_PunchDoc::GetItsTargetFolderPath()
{
	CString sItsPath = _T("");// = WorkingInfo.System.sPathIts;
	if (GetTestMode() == MODE_INNER)
		sItsPath = pDoc->WorkingInfo.System.sPathItsInner;
	else if(GetTestMode() == MODE_OUTER)
		sItsPath = pDoc->WorkingInfo.System.sPathItsOuter;

	if (sItsPath.IsEmpty())
	{
		pView->MsgBox(_T("내/외층 설정이 않되어서 ITS 폴더를 설정할 수 없습니다."));
		return _T("");
	}

	int pos = sItsPath.ReverseFind('\\');
	if (pos != -1)
		sItsPath.Delete(pos, sItsPath.GetLength() - pos);

	return sItsPath;
}

//CString CGvisR2R_PunchDoc::GetItsReelmapPath()
//{
//	CString  Path[3];
//	CString sPath = _T("");
//	CString sName = _T("ReelMapDataIts.txt");
//
//	Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
//	Path[1] = pDoc->WorkingInfo.LastJob.sModel;
//	Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
//
//	if (Path[0].IsEmpty() || Path[1].IsEmpty() || Path[2].IsEmpty())
//		return sPath;
//
//	sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], sName); // ReelmapIts.txt
//
//	return sPath;
//}


// For MODE_OUTER ============================================

int CGvisR2R_PunchDoc::LoadPCRAllUpInner(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
#ifndef USE_CAM_MASTER
	return 0;
#endif
	BOOL bDualTest;
	CString sLot, sLayerUp, sLayerDn;
	if (!GetItsSerialInfo(nSerial, bDualTest, sLot, sLayerUp, sLayerDn, 1))
	{
		CString str;
		str.Format(_T("It is trouble to read GetItsSerialInfo()."));
		pView->MsgBox(str);
		return 0;
	}

	if (!bDualTest)
		return 1;

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.16"));
		return 0;
	}

	int i, idx;//, k
	CString str;

	if (nSerial < 0)
	{
		str.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(str);
		return(2);
	}

	if (!m_pPcrInner[2])
	{
		str.Format(_T("PCR[2]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(str);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[0] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx0(nSerial, TRUE); // 릴맵화면 표시 인덱스
	else
		nIdx = GetPcrIdx0(nSerial);

	if (!m_pPcrInner[0] || !m_pPcrInner[1] || !m_pPcrInner[2])
		return(2);
	if (!m_pPcrInner[0][nIdx] || !m_pPcrInner[1][nIdx] || !m_pPcrInner[2][nIdx])
		return(2);

	m_pPcrInner[2][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcrInner[2][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcrInner[0][nIdx]->m_nErrPnl < 0)
		m_pPcrInner[2][nIdx]->m_nErrPnl = m_pPcrInner[0][nIdx]->m_nErrPnl;
	else if (m_pPcrInner[1][nIdx]->m_nErrPnl < 0)
		m_pPcrInner[2][nIdx]->m_nErrPnl = m_pPcrInner[1][nIdx]->m_nErrPnl;
	else
		m_pPcrInner[2][nIdx]->m_nErrPnl = m_pPcrInner[0][nIdx]->m_nErrPnl;

	// Model
	m_pPcrInner[2][nIdx]->m_sModel = m_pPcrInner[0][nIdx]->m_sModel;

	// Layer
	m_pPcrInner[2][nIdx]->m_sLayer = m_pPcrInner[0][nIdx]->m_sLayer;

	// Lot
	m_pPcrInner[2][nIdx]->m_sLot = m_pPcrInner[0][nIdx]->m_sLot;

	int nTotDef[3] = { 0 };										// [0]: 상면, [1]: 하면, [2]: 상/하면 Merge
	nTotDef[0] = m_pPcrInner[0][nIdx]->m_nTotDef;			// 상면 불량 피스 수
	nTotDef[1] = m_pPcrInner[1][nIdx]->m_nTotDef;			// 하면 불량 피스 수

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow;
	int nTotPcsInner = m_MasterInner[0].m_pPcsRgn->m_nTotPcs;
	int nNodeXInner = m_MasterInner[0].m_pPcsRgn->m_nCol;
	int nNodeYInner = m_MasterInner[0].m_pPcsRgn->m_nRow;
	if (nTotPcs < 0 || nTotPcsInner < 0 || nTotPcsInner != nTotPcs || nNodeXInner != nNodeX || nNodeYInner != nNodeY)
	{
		str.Format(_T("It is trouble to run LoadPCRAllUpInner()."));
		pView->MsgBox(str);
		return 0;
	}
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs + 1];

	int nPcsId;												// nPcsId : CamMaster Pcs Index
															//pPcrMgr테이블의 nIdx에 하면의 불량을 먼저 기록하고 상면의 불량을 엎어서 최종 merge불량 테이블을 만듬.
	
	int nTotVerifyed[2] = { 0 };

	for (i = 0; i < nTotDef[1]; i++)						// 하면 불량 피스 수
	{
		if (m_pPcrInner[1][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcrInner[1][nIdx]->m_pDefPcs[i];		// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdx = i;	// Up+Dn				// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			pPcrMgr[nPcsId].nIdxDn = i; // Dn					// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[1]++;
		}
	}
	for (i = 0; i < nTotDef[0]; i++)						// 상면 불량 피스 수
	{
		if (m_pPcrInner[0][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcrInner[0][nIdx]->m_pDefPcs[i];				// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdx = i;	// Up+Dn				// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			pPcrMgr[nPcsId].nIdxUp = i;	// Up					// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[0]++;
		}
	}


	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)							// Shot내 총 Piece수
	{
		if (pPcrMgr[i].nIdx > -1)	// Up+Dn				// 상하면 Merge [i:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			nTotDef[2]++;									// 상 / 하면 Merge한 총 불량피스수.
	}

	m_pPcrInner[2][nIdx]->Init(nSerial, nTotDef[2]);				// 제품시리얼, Shot내 총불량 피스수

	int nId[2], Ord; // [0]: 상면 0~불량피스순서, [1]: 하면 0~불량피스순서
	idx = 0; // 마킹순서 0~불량피스수만큼 정하기위해 현시점의 idx를 초기화함.
	if (nTotDef[2] > 0) // 상 / 하면 Merge한 총 불량피스수.
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp; // 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서 임. nId[0]: 상면에서의 PCR파일순서 인덱스
			nId[1] = pPcrMgr[nPcsId].nIdxDn; // 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서 임. nId[1]: 하면에서의 PCR파일순서 인덱스

			if (nId[0] > -1)
			{
				Ord = nId[0];
				// Cam ID
				m_pPcrInner[2][nIdx]->m_nCamId = m_pPcrInner[0][nIdx]->m_nCamId;
				// Piece Number
				m_pPcrInner[2][nIdx]->m_pDefPcs[idx] = m_pPcrInner[0][nIdx]->m_pDefPcs[Ord];
				m_pPcrInner[2][nIdx]->m_pLayer[idx] = m_pPcrInner[0][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcrInner[2][nIdx]->m_pDefPos[idx].x = m_pPcrInner[0][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcrInner[2][nIdx]->m_pDefPos[idx].y = m_pPcrInner[0][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcrInner[2][nIdx]->m_pDefType[idx] = m_pPcrInner[0][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcrInner[2][nIdx]->m_pCell[idx] = m_pPcrInner[0][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcrInner[2][nIdx]->m_pImgSz[idx] = m_pPcrInner[0][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcrInner[2][nIdx]->m_pImg[idx] = m_pPcrInner[0][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcrInner[2][nIdx]->m_pMk[idx] = m_pPcrInner[0][nIdx]->m_pMk[Ord];

				idx++;
			}
			else if (nId[1] > -1)
			{
				Ord = nId[1];
				// Cam ID
				m_pPcrInner[2][nIdx]->m_nCamId = m_pPcrInner[1][nIdx]->m_nCamId;
				// Piece Number
				m_pPcrInner[2][nIdx]->m_pDefPcs[idx] = m_pPcrInner[1][nIdx]->m_pDefPcs[Ord];
				m_pPcrInner[2][nIdx]->m_pLayer[idx] = m_pPcrInner[1][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcrInner[2][nIdx]->m_pDefPos[idx].x = m_pPcrInner[1][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcrInner[2][nIdx]->m_pDefPos[idx].y = m_pPcrInner[1][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcrInner[2][nIdx]->m_pDefType[idx] = m_pPcrInner[1][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcrInner[2][nIdx]->m_pCell[idx] = m_pPcrInner[1][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcrInner[2][nIdx]->m_pImgSz[idx] = m_pPcrInner[1][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcrInner[2][nIdx]->m_pImg[idx] = m_pPcrInner[1][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcrInner[2][nIdx]->m_pMk[idx] = m_pPcrInner[1][nIdx]->m_pMk[Ord];

				idx++;
			}
		}
	}

	delete[] pPcrMgr;

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCRAllDnInner(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest;
	CString sLot, sLayerUp, sLayerDn;
	if (!GetItsSerialInfo(nSerial, bDualTest, sLot, sLayerUp, sLayerDn, 1))
	{
		CString str;
		str.Format(_T("It is trouble to read GetItsSerialInfo()."));
		pView->MsgBox(str);
		return 0;
	}

	if (!bDualTest)
		return 1;

	int i, idx;//k, 
	CString str;

	if (nSerial <= 0)
	{
		str.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(str);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	if (!m_pPcr[3])
	{
		str.Format(_T("PCR[3]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(str);
		// 		AfxMessageBox(strFileData);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[1] && (pDoc->WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx1(nSerial, TRUE);
	else
		nIdx = GetPcrIdx1(nSerial);

	if (!m_pPcrInner[0] || !m_pPcrInner[1] || !m_pPcrInner[3])
		return(2);
	if (!m_pPcrInner[0][nIdx] || !m_pPcrInner[1][nIdx] || !m_pPcrInner[3][nIdx])
		return(2);

	m_pPcrInner[3][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcrInner[3][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcrInner[1][nIdx]->m_nErrPnl < 0)
		m_pPcrInner[3][nIdx]->m_nErrPnl = m_pPcrInner[1][nIdx]->m_nErrPnl;
	else if (m_pPcrInner[0][nIdx]->m_nErrPnl < 0)
		m_pPcrInner[3][nIdx]->m_nErrPnl = m_pPcrInner[0][nIdx]->m_nErrPnl;
	else
		m_pPcrInner[3][nIdx]->m_nErrPnl = m_pPcrInner[1][nIdx]->m_nErrPnl;

	// Model
	m_pPcrInner[3][nIdx]->m_sModel = m_pPcrInner[1][nIdx]->m_sModel;

	// Layer
	m_pPcrInner[3][nIdx]->m_sLayer = m_pPcrInner[1][nIdx]->m_sLayer;

	// Lot
	m_pPcrInner[3][nIdx]->m_sLot = m_pPcrInner[1][nIdx]->m_sLot;

	int nTotDef[3];
	nTotDef[0] = m_pPcrInner[0][nIdx]->m_nTotDef;
	nTotDef[1] = m_pPcrInner[1][nIdx]->m_nTotDef;

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow;
	int nTotPcsInner = m_MasterInner[0].m_pPcsRgn->m_nTotPcs;
	int nNodeXInner = m_MasterInner[0].m_pPcsRgn->m_nCol;
	int nNodeYInner = m_MasterInner[0].m_pPcsRgn->m_nRow;
	if (nTotPcs < 0 || nTotPcsInner < 0 || nTotPcsInner != nTotPcs || nNodeXInner != nNodeX || nNodeYInner != nNodeY)
	{
		str.Format(_T("It is trouble to run LoadPCRAllDnInner()."));
		pView->MsgBox(str);
		return 0;
	}
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs + 1];


	int nComp, nPcsId;
	if (nTotDef[0] > nTotDef[1])
		nComp = nTotDef[0];
	else
		nComp = nTotDef[1];

	for (i = 0; i < nTotDef[0]; i++)
	{
		nPcsId = m_pPcrInner[0][nIdx]->m_pDefPcs[i];
		pPcrMgr[nPcsId].nIdx = i;
		pPcrMgr[nPcsId].nIdxUp = i;	// Up
	}
	for (i = 0; i < nTotDef[1]; i++)
	{
		nPcsId = m_pPcrInner[1][nIdx]->m_pDefPcs[i];
		pPcrMgr[nPcsId].nIdx = i;
		pPcrMgr[nPcsId].nIdxDn = i; // Dn
	}

	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)
	{
		if (pPcrMgr[i].nIdx > -1)
			nTotDef[2]++;
	}

	m_pPcrInner[3][nIdx]->Init(nSerial, nTotDef[2]);

	int nId[2], Ord;
	idx = 0;
	if (nTotDef[2] > 0)
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp;
			nId[1] = pPcrMgr[nPcsId].nIdxDn;

			if (nId[1] > -1)
			{
				Ord = nId[1];
				// Cam ID
				m_pPcrInner[3][nIdx]->m_nCamId = m_pPcrInner[1][nIdx]->m_nCamId;
				// Piece Number
				m_pPcrInner[3][nIdx]->m_pDefPcs[idx] = m_pPcrInner[1][nIdx]->m_pDefPcs[Ord];
				m_pPcrInner[3][nIdx]->m_pLayer[idx] = m_pPcrInner[1][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcrInner[3][nIdx]->m_pDefPos[idx].x = m_pPcrInner[1][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcrInner[3][nIdx]->m_pDefPos[idx].y = m_pPcrInner[1][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcrInner[3][nIdx]->m_pDefType[idx] = m_pPcrInner[1][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcrInner[3][nIdx]->m_pCell[idx] = m_pPcrInner[1][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcrInner[3][nIdx]->m_pImgSz[idx] = m_pPcrInner[1][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcrInner[3][nIdx]->m_pImg[idx] = m_pPcrInner[1][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcrInner[3][nIdx]->m_pMk[idx] = m_pPcrInner[1][nIdx]->m_pMk[Ord];

				idx++;
			}
			else if (nId[0] > -1)
			{
				Ord = nId[0];
				// Cam ID
				m_pPcrInner[3][nIdx]->m_nCamId = m_pPcrInner[0][nIdx]->m_nCamId;
				// Piece Number
				m_pPcrInner[3][nIdx]->m_pDefPcs[idx] = m_pPcrInner[0][nIdx]->m_pDefPcs[Ord];
				m_pPcrInner[3][nIdx]->m_pLayer[idx] = m_pPcrInner[0][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcrInner[3][nIdx]->m_pDefPos[idx].x = m_pPcrInner[0][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcrInner[3][nIdx]->m_pDefPos[idx].y = m_pPcrInner[0][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcrInner[3][nIdx]->m_pDefType[idx] = m_pPcrInner[0][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcrInner[3][nIdx]->m_pCell[idx] = m_pPcrInner[0][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcrInner[3][nIdx]->m_pImgSz[idx] = m_pPcrInner[0][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcrInner[3][nIdx]->m_pImg[idx] = m_pPcrInner[0][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcrInner[3][nIdx]->m_pMk[idx] = m_pPcrInner[0][nIdx]->m_pMk[Ord];

				idx++;
			}
		}
	}

	delete[] pPcrMgr;

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCRUpInner(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp, i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial <= 0)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		//AfxMessageBox(strFileData);
		return(2);
	}

	if (!m_pPcrInner[0])
	{
		strFileData.Format(_T("PCR[0]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(strFileData);
		//AfxMessageBox(strFileData);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[0] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx0(nSerial, TRUE);
	else
		nIdx = GetPcrIdx0(nSerial);

	CString sPath, sUpPath, sDnPath;
	
	if (!GetInnerFolderPath(nSerial, sUpPath, sDnPath))
	{
		strFileData.Format(_T("GetInnerFolderPath가 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}
#ifndef USE_CAM_MASTER
	sPath = PATH_PCR;	// for Test
#else
	sPath.Format(_T("%s%04d.pcr"), sUpPath, nSerial);
#endif

	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData = CharToString(FileData);
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCRInner[Up] 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcrInner[0])
		return(2);
	if (!m_pPcrInner[0][nIdx])
		return(2);

	BOOL bResetMkInfo = FALSE;

	m_pPcrInner[0][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcrInner[0][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcrInner[0][nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	if (m_pPcrInner[1][nIdx]->m_nErrPnl == -1)
	{
		int syd = 1;
	}
	
	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcrInner[0][nIdx]->m_sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcrInner[0][nIdx]->m_sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcrInner[0][nIdx]->m_sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sItsCode = sItsCode;

	// Total Defect Numbers
	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);

	m_pPcrInner[0][nIdx]->Init(nSerial, nTotDef);

	if (nTotDef > 0)
	{
		for (i = 0; i < nTotDef; i++)
		{
			// Cam ID
			nTemp = strFileData.Find(',', 0);
			strCamID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[0][nIdx]->m_nCamId = _tstoi(strCamID);

			// Piece Number
			nTemp = strFileData.Find(',', 0);
			strPieceID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;

			// LoadStripPieceRegion_Binary()에 의해 PCS Index가 결정됨.
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
				m_pPcrInner[0][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);
			else
				m_pPcrInner[0][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);

			m_pPcrInner[0][nIdx]->m_pLayer[i] = 0; // Up

											  // BadPointPosX
			nTemp = strFileData.Find(',', 0);
			strBadPointPosX = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[0][nIdx]->m_pDefPos[i].x = (long)_tstoi(strBadPointPosX);

			// BadPointPosY
			nTemp = strFileData.Find(',', 0);
			strBadPointPosY = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[0][nIdx]->m_pDefPos[i].y = (long)_tstoi(strBadPointPosY);

			// BadName
			nTemp = strFileData.Find(',', 0);
			strBadName = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[0][nIdx]->m_pDefType[i] = _tstoi(strBadName);

			// CellNum
			nTemp = strFileData.Find(',', 0);
			strCellNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[0][nIdx]->m_pCell[i] = _tstoi(strCellNum);

			// ImageSize
			nTemp = strFileData.Find(',', 0);
			strImageSize = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[0][nIdx]->m_pImgSz[i] = _tstoi(strImageSize);

			// ImageNum
			nTemp = strFileData.Find(',', 0);
			strImageNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[0][nIdx]->m_pImg[i] = _tstoi(strImageNum);

			// strMarkingCode : -2 (NoMarking)
			nTemp = strFileData.Find('\n', 0);
			strMarkingCode = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[0][nIdx]->m_pMk[i] = _tstoi(strMarkingCode);
		}
	}

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPCRDnInner(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest;
	CString sLot, sLayerUp, sLayerDn;
	if (!GetItsSerialInfo(nSerial, bDualTest, sLot, sLayerUp, sLayerDn, 1))
	{
		CString str;
		str.Format(_T("It is trouble to read GetItsSerialInfo()."));
		pView->MsgBox(str);
		return 0;
	}

	if (!bDualTest)
		return 1;

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp, i;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	if (nSerial <= 0)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcrInner[1])
	{
		strFileData.Format(_T("PCR[1]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

	int nIdx;
	if (m_bNewLotShare[1] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx1(nSerial, TRUE);
	else
		nIdx = GetPcrIdx1(nSerial);

	CString sPath, sUpPath, sDnPath;

	if (!GetInnerFolderPath(nSerial, sUpPath, sDnPath))
	{
		strFileData.Format(_T("GetInnerFolderPath가 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

#ifndef USE_CAM_MASTER
	sPath = PATH_PCR;	// for Test
#else
	sPath.Format(_T("%s%04d.pcr"), sDnPath, nSerial);
#endif

	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData = CharToString(FileData);
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCRInner[Dn] 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcrInner[1])
		return(2);
	if (!m_pPcrInner[1][nIdx])
		return(2);

	m_pPcrInner[1][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcrInner[1][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcrInner[1][nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	if (m_pPcrInner[1][nIdx]->m_nErrPnl == -1)
	{
		int syd = 1;
	}

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcrInner[1][nIdx]->m_sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcrInner[1][nIdx]->m_sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcrInner[1][nIdx]->m_sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sItsCode = sItsCode;

	// Total Defect Numbers
	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	int nTotDef = _tstoi(strTotalBadPieceNum);

	m_pPcrInner[1][nIdx]->Init(nSerial, nTotDef);

	if (nTotDef > 0)
	{
		for (i = 0; i < nTotDef; i++)
		{
			// Cam ID
			nTemp = strFileData.Find(',', 0);
			strCamID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[1][nIdx]->m_nCamId = _tstoi(strCamID);

			// Piece Number
			nTemp = strFileData.Find(',', 0);
			strPieceID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;

			// LoadStripPieceRegion_Binary()에 의해 PCS Index가 결정됨.
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
				m_pPcrInner[1][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);
			else
				m_pPcrInner[1][nIdx]->m_pDefPcs[i] = MirrorLR(_tstoi(strPieceID));	// 초기 양면검사기용

			m_pPcrInner[1][nIdx]->m_pLayer[i] = 1; // Dn

											  // BadPointPosX
			nTemp = strFileData.Find(',', 0);
			strBadPointPosX = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[1][nIdx]->m_pDefPos[i].x = (long)_tstoi(strBadPointPosX);

			// BadPointPosY
			nTemp = strFileData.Find(',', 0);
			strBadPointPosY = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[1][nIdx]->m_pDefPos[i].y = (long)_tstoi(strBadPointPosY);

			// BadName
			nTemp = strFileData.Find(',', 0);
			strBadName = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[1][nIdx]->m_pDefType[i] = _tstoi(strBadName);

			// CellNum
			nTemp = strFileData.Find(',', 0);
			strCellNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[1][nIdx]->m_pCell[i] = _tstoi(strCellNum);

			// ImageSize
			nTemp = strFileData.Find(',', 0);
			strImageSize = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[1][nIdx]->m_pImgSz[i] = _tstoi(strImageSize);

			// ImageNum
			nTemp = strFileData.Find(',', 0);
			strImageNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[1][nIdx]->m_pImg[i] = _tstoi(strImageNum);

			// strMarkingCode : -2 (NoMarking)
			nTemp = strFileData.Find('\n', 0);
			strMarkingCode = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcrInner[1][nIdx]->m_pMk[i] = _tstoi(strMarkingCode);
		}
	}

	return (1); // 1(정상)
}



//============================================================

int CGvisR2R_PunchDoc::GetLastItsSerial()
{
	return m_nWritedItsSerial;
}


int CGvisR2R_PunchDoc::LoadPCRIts(int nSerial, BOOL bFromShare)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
#ifndef USE_CAM_MASTER
	return 0;
#endif
	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.16"));
		return 0;
	}

	if (!m_pPcrIts)
	{
		pView->ClrDispMsg();
		pView->MsgBox(_T("PCR[2]관련 메모리가 할당되지 않았습니다."));
		//AfxMessageBox(strFileData);
		return(2);
	}

	BOOL bDualTestInner;
	CString sLot, sLayerUp, sLayerDn;
	if (!GetItsSerialInfo(nSerial, bDualTestInner, sLot, sLayerUp, sLayerDn, 0))
	{
		CString str;
		str.Format(_T("It is trouble to read GetItsSerialInfo()."));
		pView->MsgBox(str);
		return FALSE;
	}

	BOOL bDualTest = WorkingInfo.LastJob.bDualTest;

	if (bDualTest && bDualTestInner)	// 11 -> 외층 : 양면, 내층 : 양면
		LoadPCRIts11(nSerial);
	else if (bDualTest && !bDualTestInner)	// 10 -> 외층 : 양면, 내층 : 단면
		LoadPCRIts10(nSerial);
	else if (!bDualTest && bDualTestInner)	// 01 -> 외층 : 단면, 내층 : 양면
		LoadPCRIts01(nSerial);
	else 								// 00 -> 외층 : 단면, 내층 : 단면
		LoadPCRIts00(nSerial);

	return (1); // 1(정상)
}

void CGvisR2R_PunchDoc::LoadPCRIts11(int nSerial) // 11 -> 외층 : 양면, 내층 : 양면
{
	int i, idx;//, k
	CString str;
	int nIdx;

	if (m_bNewLotShare[0] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx0(nSerial, TRUE); // 릴맵화면 표시 인덱스
	else
		nIdx = GetPcrIdx0(nSerial);

	m_pPcrIts[nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcrIts[nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcr[2][nIdx]->m_nErrPnl < 0)
		m_pPcrIts[nIdx]->m_nErrPnl = m_pPcr[2][nIdx]->m_nErrPnl;
	else if (m_pPcrInner[2][nIdx]->m_nErrPnl < 0)
		m_pPcrIts[nIdx]->m_nErrPnl = m_pPcrInner[2][nIdx]->m_nErrPnl;
	else
		m_pPcrIts[nIdx]->m_nErrPnl = m_pPcr[2][nIdx]->m_nErrPnl;

	// Model
	m_pPcrIts[nIdx]->m_sModel = m_pPcr[0][nIdx]->m_sModel;

	// Layer
	m_pPcrIts[nIdx]->m_sLayerOutUp = m_pPcr[0][nIdx]->m_sLayer;
	m_pPcrIts[nIdx]->m_sLayerOutDn = m_pPcr[1][nIdx]->m_sLayer;
	m_pPcrIts[nIdx]->m_sLayerInUp = m_pPcrInner[0][nIdx]->m_sLayer;
	m_pPcrIts[nIdx]->m_sLayerInDn = m_pPcrInner[1][nIdx]->m_sLayer;

	// Lot
	m_pPcrIts[nIdx]->m_sLot = m_pPcr[0][nIdx]->m_sLot;

	int nTotDef[3] = { 0 };									// [0]: 외층, [1]: 내층, [2]: 내/외층 Merge
	nTotDef[0] = m_pPcr[2][nIdx]->m_nTotDef;			// 외층 불량 피스 수
	nTotDef[1] = m_pPcrInner[2][nIdx]->m_nTotDef;		// 내층 불량 피스 수

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow;
	int nTotPcsInner = m_MasterInner[0].m_pPcsRgn->m_nTotPcs;
	int nNodeXInner = m_MasterInner[0].m_pPcsRgn->m_nCol;
	int nNodeYInner = m_MasterInner[0].m_pPcsRgn->m_nRow;
	if (nTotPcs < 0 || nTotPcsInner < 0 || nTotPcsInner != nTotPcs || nNodeXInner != nNodeX || nNodeYInner != nNodeY)
	{
		str.Format(_T("It is trouble to run LoadPCRIts11()."));
		pView->MsgBox(str);
		return;
	}
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs + 1];

	int nPcsId;												// nPcsId : CamMaster Pcs Index
															// pPcrMgr테이블의 nIdx에 내층의 불량을 먼저 기록하고 외층의 불량을 엎어서 최종 merge불량 테이블을 만듬.
	int nTotVerifyed[2] = { 0 };

	for (i = 0; i < nTotDef[0]; i++)						// 외층 불량 피스 수
	{
		if (m_pPcr[2][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcr[2][nIdx]->m_pDefPcs[i];				// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdxUp = i;	// 외층					// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[0]++;
		}
	}
	for (i = 0; i < nTotDef[1]; i++)						// 내층 불량 피스 수
	{
		if (m_pPcrInner[2][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcrInner[2][nIdx]->m_pDefPcs[i];		// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdxDn = i; // 내층					// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[1]++;
		}
	}


	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)								// Shot내 총 Piece수
	{
		if (pPcrMgr[i].nIdxUp > -1 || pPcrMgr[i].nIdxDn > -1)	// 외층+내층			// 내외층 Merge [i:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			nTotDef[2]++;										// 내외층 Merge한 총 불량피스수.
	}

	m_pPcrIts[nIdx]->Init(nSerial, nTotDef[2]);				// 제품시리얼, Shot내 총불량 피스수

	int nId[2], Ord;										// [0]: 외층 0~불량피스순서, [1]: 내층 0~불량피스순서
	idx = 0;												// 마킹순서 0~불량피스수만큼 정하기위해 현시점의 idx를 초기화함.
	if (nTotDef[2] > 0)										// 내외층 Merge한 총 불량피스수.
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp;				// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서 임. nId[0]: 외층에서의 PCR파일순서 인덱스
			nId[1] = pPcrMgr[nPcsId].nIdxDn;				// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서 임. nId[1]: 내층에서의 PCR파일순서 인덱스

			if (nId[0] > -1) // 외층에서의 PCR파일순서 인덱스
			{
				Ord = nId[0];
				// Cam ID
				m_pPcrIts[nIdx]->m_nCamId = m_pPcr[2][nIdx]->m_nCamId;
				// Piece Number
				m_pPcrIts[nIdx]->m_pDefPcs[idx] = m_pPcr[2][nIdx]->m_pDefPcs[Ord];
				m_pPcrIts[nIdx]->m_pLayer[idx] = m_pPcr[2][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcrIts[nIdx]->m_pDefPos[idx].x = m_pPcr[2][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcrIts[nIdx]->m_pDefPos[idx].y = m_pPcr[2][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcrIts[nIdx]->m_pDefType[idx] = m_pPcr[2][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcrIts[nIdx]->m_pCell[idx] = m_pPcr[2][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcrIts[nIdx]->m_pImgSz[idx] = m_pPcr[2][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcrIts[nIdx]->m_pImg[idx] = m_pPcr[2][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcrIts[nIdx]->m_pMk[idx] = m_pPcr[2][nIdx]->m_pMk[Ord];

				idx++;
			}
			else if (nId[1] > -1) // 내층에서의 PCR파일순서 인덱스
			{
				Ord = nId[1];
				// Cam ID
				m_pPcrIts[nIdx]->m_nCamId = m_pPcrInner[2][nIdx]->m_nCamId;
				// Piece Number
				m_pPcrIts[nIdx]->m_pDefPcs[idx] = m_pPcrInner[2][nIdx]->m_pDefPcs[Ord];
				m_pPcrIts[nIdx]->m_pLayer[idx] = m_pPcrInner[2][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcrIts[nIdx]->m_pDefPos[idx].x = m_pPcrInner[2][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcrIts[nIdx]->m_pDefPos[idx].y = m_pPcrInner[2][nIdx]->m_pDefPos[Ord].y;
				// BadName
				//m_pPcrIts[nIdx]->m_pDefType[idx] = m_pPcrInner[2][nIdx]->m_pDefType[Ord];
				m_pPcrIts[nIdx]->m_pDefType[idx] = DEF_INNER;									// 내층 불량 표시
				// CellNum
				m_pPcrIts[nIdx]->m_pCell[idx] = m_pPcrInner[2][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcrIts[nIdx]->m_pImgSz[idx] = m_pPcrInner[2][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcrIts[nIdx]->m_pImg[idx] = m_pPcrInner[2][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcrIts[nIdx]->m_pMk[idx] = m_pPcrInner[2][nIdx]->m_pMk[Ord];

				idx++;
			}
		}
	}
	delete[] pPcrMgr;
}

void CGvisR2R_PunchDoc::LoadPCRIts10(int nSerial) // 10 -> 외층 : 양면, 내층 : 단면
{
	int i, idx;//, k
	CString str;
	int nIdx;

	if (m_bNewLotShare[0] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx0(nSerial, TRUE); // 릴맵화면 표시 인덱스
	else
		nIdx = GetPcrIdx0(nSerial);

	m_pPcrIts[nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcrIts[nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcr[2][nIdx]->m_nErrPnl < 0)
		m_pPcrIts[nIdx]->m_nErrPnl = m_pPcr[2][nIdx]->m_nErrPnl;
	else if (m_pPcrInner[0][nIdx]->m_nErrPnl < 0)
		m_pPcrIts[nIdx]->m_nErrPnl = m_pPcrInner[0][nIdx]->m_nErrPnl;
	else
		m_pPcrIts[nIdx]->m_nErrPnl = m_pPcr[2][nIdx]->m_nErrPnl;

	// Model
	m_pPcrIts[nIdx]->m_sModel = m_pPcr[0][nIdx]->m_sModel;

	// Layer
	m_pPcrIts[nIdx]->m_sLayerOutUp = m_pPcr[0][nIdx]->m_sLayer;
	m_pPcrIts[nIdx]->m_sLayerOutDn = m_pPcr[1][nIdx]->m_sLayer;
	m_pPcrIts[nIdx]->m_sLayerInUp = m_pPcrInner[0][nIdx]->m_sLayer;
	m_pPcrIts[nIdx]->m_sLayerInDn = _T(""); // m_pPcrInner[1][nIdx]->m_sLayer;

	// Lot
	m_pPcrIts[nIdx]->m_sLot = m_pPcr[0][nIdx]->m_sLot;

	int nTotDef[3] = { 0 };									// [0]: 외층, [1]: 내층, [2]: 내/외층 Merge
	nTotDef[0] = m_pPcr[2][nIdx]->m_nTotDef;			// 외층 불량 피스 수
	nTotDef[1] = m_pPcrInner[0][nIdx]->m_nTotDef;		// 내층 불량 피스 수

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow;
	int nTotPcsInner = m_MasterInner[0].m_pPcsRgn->m_nTotPcs;
	int nNodeXInner = m_MasterInner[0].m_pPcsRgn->m_nCol;
	int nNodeYInner = m_MasterInner[0].m_pPcsRgn->m_nRow;
	if (nTotPcs < 0 || nTotPcsInner < 0 || nTotPcsInner != nTotPcs || nNodeXInner != nNodeX || nNodeYInner != nNodeY)
	{
		str.Format(_T("It is trouble to run LoadPCRIts10()."));
		pView->MsgBox(str);
		return;
	}
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs + 1];

	int nPcsId;												// nPcsId : CamMaster Pcs Index
															// pPcrMgr테이블의 nIdx에 내층의 불량을 먼저 기록하고 외층의 불량을 엎어서 최종 merge불량 테이블을 만듬.
	int nTotVerifyed[2] = { 0 };

	for (i = 0; i < nTotDef[0]; i++)						// 외층 불량 피스 수
	{
		if (m_pPcr[2][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcr[2][nIdx]->m_pDefPcs[i];				// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdxUp = i;	// 외층					// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[0]++;
		}
	}
	for (i = 0; i < nTotDef[1]; i++)						// 내층 불량 피스 수
	{
		if (m_pPcrInner[2][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcrInner[2][nIdx]->m_pDefPcs[i];		// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdxDn = i; // 내층					// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[1]++;
		}
	}

	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)								// Shot내 총 Piece수
	{
		if (pPcrMgr[i].nIdxUp > -1 || pPcrMgr[i].nIdxDn > -1)	// 외층+내층			// 내외층 Merge [i:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			nTotDef[2]++;										// 내외층 Merge한 총 불량피스수.
	}

	m_pPcrIts[nIdx]->Init(nSerial, nTotDef[2]);				// 제품시리얼, Shot내 총불량 피스수

	int nId[2], Ord;										// [0]: 외층 0~불량피스순서, [1]: 내층 0~불량피스순서
	idx = 0;												// 마킹순서 0~불량피스수만큼 정하기위해 현시점의 idx를 초기화함.
	if (nTotDef[2] > 0)										// 내외층 Merge한 총 불량피스수.
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp;				// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서 임. nId[0]: 외층에서의 PCR파일순서 인덱스
			nId[1] = pPcrMgr[nPcsId].nIdxDn;				// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서 임. nId[1]: 내층에서의 PCR파일순서 인덱스

			if (nId[0] > -1)
			{
				Ord = nId[0];
				// Cam ID
				m_pPcrIts[nIdx]->m_nCamId = m_pPcr[2][nIdx]->m_nCamId;
				// Piece Number
				m_pPcrIts[nIdx]->m_pDefPcs[idx] = m_pPcr[2][nIdx]->m_pDefPcs[Ord];
				m_pPcrIts[nIdx]->m_pLayer[idx] = m_pPcr[2][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcrIts[nIdx]->m_pDefPos[idx].x = m_pPcr[2][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcrIts[nIdx]->m_pDefPos[idx].y = m_pPcr[2][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcrIts[nIdx]->m_pDefType[idx] = m_pPcr[2][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcrIts[nIdx]->m_pCell[idx] = m_pPcr[2][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcrIts[nIdx]->m_pImgSz[idx] = m_pPcr[2][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcrIts[nIdx]->m_pImg[idx] = m_pPcr[2][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcrIts[nIdx]->m_pMk[idx] = m_pPcr[2][nIdx]->m_pMk[Ord];

				idx++;
			}
			else if (nId[1] > -1)
			{
				Ord = nId[1];
				// Cam ID
				m_pPcrIts[nIdx]->m_nCamId = m_pPcrInner[0][nIdx]->m_nCamId;
				// Piece Number
				m_pPcrIts[nIdx]->m_pDefPcs[idx] = m_pPcrInner[0][nIdx]->m_pDefPcs[Ord];
				m_pPcrIts[nIdx]->m_pLayer[idx] = m_pPcrInner[0][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcrIts[nIdx]->m_pDefPos[idx].x = m_pPcrInner[0][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcrIts[nIdx]->m_pDefPos[idx].y = m_pPcrInner[0][nIdx]->m_pDefPos[Ord].y;
				// BadName
				//m_pPcrIts[nIdx]->m_pDefType[idx] = m_pPcrInner[0][nIdx]->m_pDefType[Ord];
				m_pPcrIts[nIdx]->m_pDefType[idx] = DEF_INNER;									// 내층 불량 표시
				// CellNum
				m_pPcrIts[nIdx]->m_pCell[idx] = m_pPcrInner[0][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcrIts[nIdx]->m_pImgSz[idx] = m_pPcrInner[0][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcrIts[nIdx]->m_pImg[idx] = m_pPcrInner[0][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcrIts[nIdx]->m_pMk[idx] = m_pPcrInner[0][nIdx]->m_pMk[Ord];

				idx++;
			}
		}
	}
	delete[] pPcrMgr;
}

void CGvisR2R_PunchDoc::LoadPCRIts01(int nSerial) // 01 -> 외층 : 단면, 내층 : 양면
{
	int i, idx;//, k
	CString str;
	int nIdx;

	if (m_bNewLotShare[0] && (WorkingInfo.LastJob.bLotSep || m_bDoneChgLot))
		nIdx = GetPcrIdx0(nSerial, TRUE); // 릴맵화면 표시 인덱스
	else
		nIdx = GetPcrIdx0(nSerial);

	m_pPcrIts[nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcrIts[nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcr[0][nIdx]->m_nErrPnl < 0)
		m_pPcrIts[nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;
	else if (m_pPcrInner[2][nIdx]->m_nErrPnl < 0)
		m_pPcrIts[nIdx]->m_nErrPnl = m_pPcrInner[2][nIdx]->m_nErrPnl;
	else
		m_pPcrIts[nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;

	// Model
	m_pPcrIts[nIdx]->m_sModel = m_pPcr[0][nIdx]->m_sModel;

	// Layer
	m_pPcrIts[nIdx]->m_sLayerOutUp = m_pPcr[0][nIdx]->m_sLayer;
	m_pPcrIts[nIdx]->m_sLayerOutDn = _T(""); // m_pPcr[1][nIdx]->m_sLayer;
	m_pPcrIts[nIdx]->m_sLayerInUp = m_pPcrInner[0][nIdx]->m_sLayer;
	m_pPcrIts[nIdx]->m_sLayerInDn = m_pPcrInner[1][nIdx]->m_sLayer;

	// Lot
	m_pPcrIts[nIdx]->m_sLot = m_pPcr[0][nIdx]->m_sLot;

	int nTotDef[3] = { 0 };									// [0]: 외층, [1]: 내층, [2]: 내/외층 Merge
	nTotDef[0] = m_pPcr[0][nIdx]->m_nTotDef;			// 외층 불량 피스 수
	nTotDef[1] = m_pPcrInner[2][nIdx]->m_nTotDef;		// 내층 불량 피스 수

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow;
	int nTotPcsInner = m_MasterInner[0].m_pPcsRgn->m_nTotPcs;
	int nNodeXInner = m_MasterInner[0].m_pPcsRgn->m_nCol;
	int nNodeYInner = m_MasterInner[0].m_pPcsRgn->m_nRow;
	if (nTotPcs < 0 || nTotPcsInner < 0 || nTotPcsInner != nTotPcs || nNodeXInner != nNodeX || nNodeYInner != nNodeY)
	{
		str.Format(_T("It is trouble to run LoadPCRIts01()."));
		pView->MsgBox(str);
		return;
	}
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs + 1];

	int nPcsId;												// nPcsId : CamMaster Pcs Index
															// pPcrMgr테이블의 nIdx에 내층의 불량을 먼저 기록하고 외층의 불량을 엎어서 최종 merge불량 테이블을 만듬.
	int nTotVerifyed[2] = { 0 };

	for (i = 0; i < nTotDef[0]; i++)						// 외층 불량 피스 수
	{
		if (m_pPcr[2][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcr[2][nIdx]->m_pDefPcs[i];				// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdxUp = i;	// 외층					// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[0]++;
		}
	}
	for (i = 0; i < nTotDef[1]; i++)						// 내층 불량 피스 수
	{
		if (m_pPcrInner[2][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcrInner[2][nIdx]->m_pDefPcs[i];		// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdxDn = i; // 내층					// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[1]++;
		}
	}
	
	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)								// Shot내 총 Piece수
	{
		if (pPcrMgr[i].nIdxUp > -1 || pPcrMgr[i].nIdxDn > -1)	// 외층+내층			// 내외층 Merge [i:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			nTotDef[2]++;										// 내외층 Merge한 총 불량피스수.
	}

	m_pPcrIts[nIdx]->Init(nSerial, nTotDef[2]);				// 제품시리얼, Shot내 총불량 피스수

	int nId[2], Ord;										// [0]: 외층 0~불량피스순서, [1]: 내층 0~불량피스순서
	idx = 0;												// 마킹순서 0~불량피스수만큼 정하기위해 현시점의 idx를 초기화함.
	if (nTotDef[2] > 0)										// 내외층 Merge한 총 불량피스수.
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp;				// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서 임. nId[0]: 외층에서의 PCR파일순서 인덱스
			nId[1] = pPcrMgr[nPcsId].nIdxDn;				// 내외층 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서 임. nId[1]: 내층에서의 PCR파일순서 인덱스

			if (nId[0] > -1)
			{
				Ord = nId[0];
				// Cam ID
				m_pPcrIts[nIdx]->m_nCamId = m_pPcr[0][nIdx]->m_nCamId;
				// Piece Number
				m_pPcrIts[nIdx]->m_pDefPcs[idx] = m_pPcr[0][nIdx]->m_pDefPcs[Ord];
				m_pPcrIts[nIdx]->m_pLayer[idx] = m_pPcr[0][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcrIts[nIdx]->m_pDefPos[idx].x = m_pPcr[0][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcrIts[nIdx]->m_pDefPos[idx].y = m_pPcr[0][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcrIts[nIdx]->m_pDefType[idx] = m_pPcr[0][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcrIts[nIdx]->m_pCell[idx] = m_pPcr[0][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcrIts[nIdx]->m_pImgSz[idx] = m_pPcr[0][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcrIts[nIdx]->m_pImg[idx] = m_pPcr[0][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcrIts[nIdx]->m_pMk[idx] = m_pPcr[0][nIdx]->m_pMk[Ord];

				idx++;
			}
			else if (nId[1] > -1)
			{
				Ord = nId[1];
				// Cam ID
				m_pPcrIts[nIdx]->m_nCamId = m_pPcrInner[2][nIdx]->m_nCamId;
				// Piece Number
				m_pPcrIts[nIdx]->m_pDefPcs[idx] = m_pPcrInner[2][nIdx]->m_pDefPcs[Ord];
				m_pPcrIts[nIdx]->m_pLayer[idx] = m_pPcrInner[2][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcrIts[nIdx]->m_pDefPos[idx].x = m_pPcrInner[2][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcrIts[nIdx]->m_pDefPos[idx].y = m_pPcrInner[2][nIdx]->m_pDefPos[Ord].y;
				// BadName
				//m_pPcrIts[nIdx]->m_pDefType[idx] = m_pPcrInner[2][nIdx]->m_pDefType[Ord];
				m_pPcrIts[nIdx]->m_pDefType[idx] = DEF_INNER;									// 내층 불량 표시
				// CellNum
				m_pPcrIts[nIdx]->m_pCell[idx] = m_pPcrInner[2][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcrIts[nIdx]->m_pImgSz[idx] = m_pPcrInner[2][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcrIts[nIdx]->m_pImg[idx] = m_pPcrInner[2][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcrIts[nIdx]->m_pMk[idx] = m_pPcrInner[2][nIdx]->m_pMk[Ord];

				idx++;
			}
		}
	}
	delete[] pPcrMgr;
}

void CGvisR2R_PunchDoc::LoadPCRIts00(int nSerial) // 00 -> 외층 : 단면, 내층 : 단면
{
	pView->ClrDispMsg();
	pView->MsgBox(_T("LoadPCRIts00 - 내외층 모두 단면인 경우는 프로그램이 없습니다."));
	return;
}


BOOL CGvisR2R_PunchDoc::InitReelmapInner()
{
	if (!m_MasterInner[0].m_pPcsRgn)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->ClrDispMsg();
		AfxMessageBox(strMsg, MB_ICONSTOP);
		return FALSE;
	}
	
	//GetCurrentInfoEng();
	int nTotPcs = m_MasterInner[0].m_pPcsRgn->GetTotPcs();
	BOOL bDualTest = WorkingInfo.LastJob.bDualTestInner;

	if (m_pReelMapInnerUp)
	{
		delete m_pReelMapInnerUp;
		m_pReelMapInnerUp = NULL;
	}
	m_pReelMapInnerUp = new CReelMap(RMAP_INNER_UP, MAX_DISP_PNL, nTotPcs);

	if (bDualTest)
	{
		if (m_pReelMapInnerDn)
		{
			delete m_pReelMapInnerDn;
			m_pReelMapInnerDn = NULL;
		}
		m_pReelMapInnerDn = new CReelMap(RMAP_INNER_DN, MAX_DISP_PNL, nTotPcs);

		if (m_pReelMapInnerAllUp)
		{
			delete m_pReelMapInnerAllUp;
			m_pReelMapInnerAllUp = NULL;
		}
		m_pReelMapInnerAllUp = new CReelMap(RMAP_INNER_ALLUP, MAX_DISP_PNL, nTotPcs);

		if (m_pReelMapInnerAllDn)
		{
			delete m_pReelMapInnerAllDn;
			m_pReelMapInnerAllDn = NULL;
		}
		m_pReelMapInnerAllDn = new CReelMap(RMAP_INNER_ALLDN, MAX_DISP_PNL, nTotPcs);
		m_pReelMapInner = m_pReelMapInnerAllUp;
	}
	else
	{
		m_pReelMapInner = m_pReelMapInnerUp;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if(m_pReelMapIts)
			m_pReelMap = m_pReelMapIts;
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::InitReelmapInnerUp()
{
	if (!m_MasterInner[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->MsgBox(strMsg);
		return FALSE;
	}

	//GetCurrentInfoEng();
	BOOL bDualTest = WorkingInfo.LastJob.bDualTestInner;
	int nTotPcs = m_MasterInner[0].m_pPcsRgn->GetTotPcs();

	if (m_pReelMapInnerUp)
	{
		delete m_pReelMapInnerUp;
		m_pReelMapInnerUp = NULL;
	}
	m_pReelMapInnerUp = new CReelMap(RMAP_INNER_UP, MAX_DISP_PNL, nTotPcs);

	if (bDualTest)
	{

		if (m_pReelMapInnerAllUp)
		{
			delete m_pReelMapInnerAllUp;
			m_pReelMapInnerAllUp = NULL;
		}
		m_pReelMapInnerAllUp = new CReelMap(RMAP_INNER_ALLUP, MAX_DISP_PNL, nTotPcs);
		m_pReelMapInner = m_pReelMapInnerAllUp;
	}
	else
	{
		m_pReelMapInner = m_pReelMapInnerUp;
	}

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		m_pReelMap = m_pReelMapIts;
	}

	return TRUE;
}

BOOL CGvisR2R_PunchDoc::InitReelmapInnerDn()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTestInner;
	if (!bDualTest)
		return TRUE;

	if (!m_MasterInner[0].m_pPcsRgn)
	{
		CString strMsg;
		strMsg.Format(_T("피스 영역이 존재하지 않습니다."));
		pView->MsgBox(strMsg);
		return FALSE;
	}

	int nTotPcs = m_MasterInner[0].m_pPcsRgn->GetTotPcs();

	if (m_pReelMapInnerDn)
	{
		delete m_pReelMapInnerDn;
		m_pReelMapInnerDn = NULL;
	}
	m_pReelMapInnerDn = new CReelMap(RMAP_INNER_DN, MAX_DISP_PNL, nTotPcs);

	if (m_pReelMapInnerAllDn)
	{
		delete m_pReelMapInnerAllDn;
		m_pReelMapInnerAllDn = NULL;
	}
	m_pReelMapInnerAllDn = new CReelMap(RMAP_INNER_ALLDN, MAX_DISP_PNL, nTotPcs);

	return TRUE;
}


void CGvisR2R_PunchDoc::SetReelmapInner(int nDir)
{
	if (!m_pReelMapInner || !m_MasterInner[0].m_pPcsRgn)
		return;

	if (!m_pReelMapInner->pFrmRgn || !m_pReelMapInner->pPcsRgn)
		return;

	int i, k;
	double fData1, fData2, fData3, fData4, fDistX, fDistY;
	double fWidth, fHeight, fRight, fBottom;

	m_pReelMapInner->nDir = nDir;

	int nTotPnl = m_pReelMapInner->nTotPnl;
	int nTotPcs = m_pReelMapInner->nTotPcs;

	double dScale = (m_MasterInner[0].MasterInfo.dPixelSize / 10.0);
	m_pReelMapInner->SetAdjRatio(dScale);
	dScale = m_pReelMapInner->GetAdjRatio();

	for (k = 0; k < nTotPnl; k++)
	{
		switch (nDir)
		{
		case ROT_NONE:
			fWidth = (m_MasterInner[0].m_pPcsRgn->pPcs[0].right - m_MasterInner[0].m_pPcsRgn->pPcs[0].left);
			fHeight = (m_MasterInner[0].m_pPcsRgn->pPcs[0].bottom - m_MasterInner[0].m_pPcsRgn->pPcs[0].top);
			fRight = m_MasterInner[0].m_pPcsRgn->rtFrm.right - fWidth * (1.0 - RMAP_PCS_SCALE);
			fBottom = m_MasterInner[0].m_pPcsRgn->rtFrm.bottom - fHeight * (1.0 - RMAP_PCS_SCALE);;
			m_pReelMapInner->pFrmRgn[k].left = (m_MasterInner[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_MasterInner[0].m_pPcsRgn->rtFrm.left;
			m_pReelMapInner->pFrmRgn[k].top = m_MasterInner[0].m_pPcsRgn->rtFrm.top;
			m_pReelMapInner->pFrmRgn[k].right = (m_MasterInner[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fRight;
			m_pReelMapInner->pFrmRgn[k].bottom = fBottom;

			for (i = 0; i < nTotPcs; i++)
			{
				fWidth = (m_MasterInner[0].m_pPcsRgn->pPcs[i].right - m_MasterInner[0].m_pPcsRgn->pPcs[i].left);
				fHeight = (m_MasterInner[0].m_pPcsRgn->pPcs[i].bottom - m_MasterInner[0].m_pPcsRgn->pPcs[i].top);
				fData1 = m_MasterInner[0].m_pPcsRgn->pPcs[i].left;	// left
				fData2 = m_MasterInner[0].m_pPcsRgn->pPcs[i].top;	// top
				fData3 = fData1 + fWidth * RMAP_PCS_SCALE; // right
				fData4 = fData2 + fHeight * RMAP_PCS_SCALE; // bottom

				m_pReelMapInner->pPcsRgn[k][i].left = (m_MasterInner[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
				m_pReelMapInner->pPcsRgn[k][i].top = fData2;
				m_pReelMapInner->pPcsRgn[k][i].right = (m_MasterInner[0].m_pPcsRgn->rtFrm.right + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
				m_pReelMapInner->pPcsRgn[k][i].bottom = fData4;
			}
			break;
		case ROT_CCW_90: // right->bottom, top->left, bottom->right, left->top ; Dir (x *= 1, y *= -1) 
			fDistX = 0;
			fDistY = m_MasterInner[0].m_pPcsRgn->rtFrm.left + m_MasterInner[0].m_pPcsRgn->rtFrm.right;
			m_pReelMapInner->pFrmRgn[k].left = (m_MasterInner[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_MasterInner[0].m_pPcsRgn->rtFrm.top;
			m_pReelMapInner->pFrmRgn[k].top = fDistY - m_Master[0].m_pPcsRgn->rtFrm.right;
			m_pReelMapInner->pFrmRgn[k].right = (m_MasterInner[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + m_MasterInner[0].m_pPcsRgn->rtFrm.bottom;
			m_pReelMapInner->pFrmRgn[k].bottom = fDistY - m_MasterInner[0].m_pPcsRgn->rtFrm.left;

			for (i = 0; i < nTotPcs; i++)
			{
				fData1 = m_MasterInner[0].m_pPcsRgn->pPcs[i].top;	// left
				fData2 = fDistY - m_MasterInner[0].m_pPcsRgn->pPcs[i].right;	// top
				fData3 = m_MasterInner[0].m_pPcsRgn->pPcs[i].bottom;	// right
				fData4 = fDistY - m_MasterInner[0].m_pPcsRgn->pPcs[i].left; // bottom

				m_pReelMapInner->pPcsRgn[k][i].left = (m_MasterInner[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData1;
				m_pReelMapInner->pPcsRgn[k][i].top = fData2;
				m_pReelMapInner->pPcsRgn[k][i].right = (m_MasterInner[0].m_pPcsRgn->rtFrm.bottom + MYGL_GAP_PNL*dScale)*(nTotPnl - 1 - k) + fData3;
				m_pReelMapInner->pPcsRgn[k][i].bottom = fData4;
			}
			break;
		}
	}
}

CString CGvisR2R_PunchDoc::GetItsTargetPath(int nSerial, int nLayer)	// RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
{
	CString sPath, str;
	CString sItsFolderPath = GetItsTargetFolderPath();
	CString sTime = pView->GetTimeIts();

	switch (nLayer)
	{
	case RMAP_UP: // 외층 Top
		str.Format(_T("%s_L2_%04d_T_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
		sPath.Format(_T("%s\\%s"), sItsFolderPath, str);
		break;
	case RMAP_DN: // 외층 Bottom
		str.Format(_T("%s_L2_%04d_B_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
		sPath.Format(_T("%s\\%s"), sItsFolderPath, str);
		break;
	case RMAP_INNER_UP: // 내층 Top
		str.Format(_T("%s_L1_%04d_T_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
		sPath.Format(_T("%s\\%s"), sItsFolderPath, str);
		break;
	case RMAP_INNER_DN: // 내층 Bottom
		str.Format(_T("%s_L1_%04d_B_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
		sPath.Format(_T("%s\\%s"), sItsFolderPath, str);
		break;
	}

	return sPath;
}


CString CGvisR2R_PunchDoc::GetItsPath(int nSerial, int nLayer)	// RMAP_UP, RMAP_DN, RMAP_INNER_UP, RMAP_INNER_DN
{
	CString sPath, str;
	CString sItsFolderPath = GetItsFolderPath();
	CString sTime = pView->GetTimeIts();

	int nPos = WorkingInfo.LastJob.sSelUserName.Find(_T('\r'));
	if (nPos > 0)
		WorkingInfo.LastJob.sSelUserName = WorkingInfo.LastJob.sSelUserName.Left(nPos);

	switch (nLayer)
	{
	case RMAP_UP: // 외층 Top
		str.Format(_T("%s_L2_%04d_T_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
		sPath.Format(_T("%s\\Outer\\%s"), sItsFolderPath, str);
		break;
	case RMAP_DN: // 외층 Bottom
		str.Format(_T("%s_L2_%04d_B_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
		sPath.Format(_T("%s\\Outer\\%s"), sItsFolderPath, str);
		break;
	case RMAP_INNER_UP: // 내층 Top
		str.Format(_T("%s_L1_%04d_T_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
		sPath.Format(_T("%s\\Inner\\%s"), sItsFolderPath, str);
		break;
	case RMAP_INNER_DN: // 내층 Bottom
		str.Format(_T("%s_L1_%04d_B_%s_%s_AVR01_%s.dat"), m_sItsCode, nSerial, WorkingInfo.LastJob.sSelUserName, WorkingInfo.System.sMcName, sTime);
		sPath.Format(_T("%s\\Inner\\%s"), sItsFolderPath, str);
		break;
	}

	return sPath;
}

int CGvisR2R_PunchDoc::GetItsDefCode(int nDefCode)
{
	CString sDefCode;
	sDefCode.Format(_T("%c"), pDoc->m_cBigDefCode[nDefCode]);

	// [Sapp3Code]
	if (sDefCode == _T("N"))		//1 NICK = 137 -> m_nSapp3Code[SAPP3_NICK]
		return m_nSapp3Code[SAPP3_NICK];
	else if (sDefCode == _T("D"))	//2 SPACE_EXTRA_PROTRUSION = 160 -> m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] : PROTRUSION
		return m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION];
	else if (sDefCode == _T("A"))	//3 SPACE_EXTRA_PROTRUSION = 160 -> m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] : SPACE 
		return m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION];
	else if (sDefCode == _T("O"))	//4 OPEN = 102 -> m_nSapp3Code[SAPP3_OPEN]
		return m_nSapp3Code[SAPP3_OPEN];
	else if (sDefCode == _T("S"))	//5 SHORT = 129 -> m_nSapp3Code[SAPP3_SHORT]
		return m_nSapp3Code[SAPP3_SHORT];
	else if (sDefCode == _T("U"))	//6 USHORT = 129 -> m_nSapp3Code[SAPP3_USHORT]
		return m_nSapp3Code[SAPP3_USHORT];
	else if (sDefCode == _T("I"))	//7 PINHOLE = 134 -> m_nSapp3Code[SAPP3_PINHOLE]
		return m_nSapp3Code[SAPP3_PINHOLE];
	else if (sDefCode == _T("H"))	//8 HMISS_HPOS_HBAD = 309 -> m_nSapp3Code[	SAPP3_HMISS_HPOS_HBAD] : No Hole
		return m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD];
	else if (sDefCode == _T("E"))	//9 SPACE_EXTRA_PROTRUSION = 160 -> m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION] : EXTRA
		return m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION];
	else if (sDefCode == _T("P"))	//10 PAD = 316 -> m_nSapp3Code[SAPP3_PAD]
		return m_nSapp3Code[SAPP3_PAD];
	else if (sDefCode == _T("L"))	//11 HMISS_HPOS_HBAD = 309 -> m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] : Hole Align
		return m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD];
	else if (sDefCode == _T("X"))	//12 POI -> m_nSapp3Code[SAPP3_SHORT]
		return m_nSapp3Code[SAPP3_SHORT];
	else if (sDefCode == _T("T"))	//13 VH_POSITION = 379 -> m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] : VH Align
		return m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF];
	else if (sDefCode == _T("M"))	//14 VH_MISS = 379 -> m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] : No VH
		return m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF];
	else if (sDefCode == _T("F"))	//15 HMISS_HPOS_HBAD = 309 -> m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD] : Hole Defect
		return m_nSapp3Code[SAPP3_HMISS_HPOS_HBAD];
	else if (sDefCode == _T("C"))	//16 HOPEN = 308 -> m_nSapp3Code[SAPP3_HOPEN]
		return m_nSapp3Code[SAPP3_HOPEN];
	else if (sDefCode == _T("G"))	//17 VH_OPEN = 379 -> m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] : VH Open
		return m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF];
	else if (sDefCode == _T("V"))	//18 VH_DEF = 379 -> m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF] : VH Def
		return m_nSapp3Code[SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF];
	else if (sDefCode == _T("K"))	//19 E.Nick = 137 -> m_nSapp3Code[SAPP3_NICK]
		return m_nSapp3Code[SAPP3_NICK];
	else if (sDefCode == _T("R"))	//20 E.Prot = 160 -> m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION]
		return m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION];
	else if (sDefCode == _T("B"))	//21 E.Space = 160 -> m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION]
		return m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION];
	else if (sDefCode == _T("J"))	//22 UDD1 = 160 -> m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION]
		return m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION];
	else if (sDefCode == _T("Q"))	//23 Narrow = 160 -> m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION]
		return m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION];
	else if (sDefCode == _T("W"))	//24 Wide = 160 -> m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION]
		return m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION];
	else if (sDefCode == _T("?"))	//25 Light = 160 -> m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION]
		return m_nSapp3Code[SAPP3_SPACE_EXTRA_PROTRUSION];

	return 0;
}

BOOL CGvisR2R_PunchDoc::MakeItsDir(CString sModel, CString sLot, CString sLayer)
{
	CString sMsg = _T("");
	CFileFind finder;
	CString sPath;

	sPath.Format(_T("%s"), pDoc->WorkingInfo.System.sPathItsFile);
	int pos = sPath.ReverseFind('\\');
	if (pos != -1)
		sPath.Delete(pos, sPath.GetLength() - pos);

	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if (sModel.IsEmpty() || sLot.IsEmpty() || sLayer.IsEmpty())
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("모델이나 로뜨 또는 레이어명이 없습니다."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s%s"), pDoc->WorkingInfo.System.sPathItsFile, sModel);

	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
	
	sPath.Format(_T("%s%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile, sModel, WorkingInfo.LastJob.sEngItsCode);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile, sModel, WorkingInfo.LastJob.sEngItsCode, sLot);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile, sModel, WorkingInfo.LastJob.sEngItsCode, sLot, sLayer);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	return TRUE;
}

void CGvisR2R_PunchDoc::WriteChangedModel()
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;

	CString sData, sPath = PATH_WORKING_INFO;

	sData = WorkingInfo.LastJob.sModel;
	::WritePrivateProfileString(_T("Last Job"), _T("Model Name"), sData, sPath);
	SetMkMenu01(_T("Info"), _T("Model"), sData);

	sData = WorkingInfo.LastJob.sLayerUp;
	::WritePrivateProfileString(_T("Last Job"), _T("LayerUp Name"), sData, sPath);
	SetMkMenu01(_T("Info"), _T("LayerUp"), sData);

	sData = WorkingInfo.LastJob.sLot;
	::WritePrivateProfileString(_T("Last Job"), _T("Lot No"), sData, sPath);
	SetMkMenu01(_T("Info"), _T("Lot"), sData);


	if (bDualTest)
	{
		sData = WorkingInfo.LastJob.sLayerDn;
		::WritePrivateProfileString(_T("Last Job"), _T("LayerDn Name"), sData, sPath);
		SetMkMenu01(_T("Info"), _T("LayerDn"), sData);
	}

	if (pView->m_pDlgMenu01)
		pView->m_pDlgMenu01->DispChangedModel();
}

int CGvisR2R_PunchDoc::GetAoiUpCamMstInfo()
{
	TCHAR szData[200];
	CString sPath, sMsg;
	if (pDoc->WorkingInfo.System.sPathAoiUpVrsData.Right(1) != "\\")
	{
		sPath.Format(_T("%s\\%s\\%s\\%s\\DataOut.ini"), pDoc->WorkingInfo.System.sPathAoiUpVrsData,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sLayerUp, pDoc->WorkingInfo.LastJob.sLot);
	}
	else
	{
		sPath.Format(_T("%s%s\\%s\\%s\\DataOut.ini"), pDoc->WorkingInfo.System.sPathAoiUpVrsData,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sLayerUp, pDoc->WorkingInfo.LastJob.sLot);
	}
		
#ifndef TEST_MODE
	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Region"), _T("Piece Region Type"), NULL, szData, sizeof(szData), sPath))
			pDoc->m_Master[0].MasterInfo.nOutFileOnAoi = _ttoi(szData);
		else
			pDoc->m_Master[0].MasterInfo.nOutFileOnAoi = -1;
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 Region에 Piece Region Type 정보가 없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		pDoc->m_Master[0].MasterInfo.nOutFileOnAoi = -1;
	}
#endif
	return pDoc->m_Master[0].MasterInfo.nOutFileOnAoi;
}

int CGvisR2R_PunchDoc::GetAoiDnCamMstInfo()
{
	TCHAR szData[200];
	CString sPath;
	if (pDoc->WorkingInfo.System.sPathAoiDnVrsData.Right(1) != "\\")
	{
		sPath.Format(_T("%s\\%s\\%s\\%s\\DataOut.ini"), pDoc->WorkingInfo.System.sPathAoiDnVrsData,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sLayerDn, pDoc->WorkingInfo.LastJob.sLot);
	}
	else
	{
		sPath.Format(_T("%s%s\\%s\\%s\\DataOut.ini"), pDoc->WorkingInfo.System.sPathAoiDnVrsData,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sLayerDn, pDoc->WorkingInfo.LastJob.sLot);
	}

#ifndef TEST_MODE
	CString sMsg;

	CFileFind finder;
	if (finder.FindFile(sPath))
	{
		if (0 < ::GetPrivateProfileString(_T("Region"), _T("Piece Region Type"), NULL, szData, sizeof(szData), sPath))
			pDoc->m_Master[1].MasterInfo.nOutFileOnAoi = _ttoi(szData);
		else
			pDoc->m_Master[1].MasterInfo.nOutFileOnAoi = -1;
	}
	else
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s파일의 Region에 Piece Region Type 정보가 없습니다."), sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		pDoc->m_Master[1].MasterInfo.nOutFileOnAoi = -1;
	}
#endif
	return pDoc->m_Master[1].MasterInfo.nOutFileOnAoi;
}

// Write Log for Auto
void CGvisR2R_PunchDoc::LogAuto(CString strMsg, int nType)
{
	if (m_bOffLogAuto)
		return;

	CSafeLockDoc lock(&g_LogLockAuto);

	TCHAR szFile[MAX_PATH] = { 0, };
	TCHAR szPath[MAX_PATH] = { 0, };
	TCHAR* pszPos = NULL;

	_stprintf(szPath, PATH_LOG);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	_stprintf(szPath, PATH_LOG_AUTO);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();

	switch (nType)
	{
	case 0:
		_stprintf(szFile, _T("%s\\%s.txt"), szPath, COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")));
		break;
	}

	CString strDate;
	CString strContents;
	CTime now;

	strDate.Format(_T("%s: "), COleDateTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")));
	strContents = strDate;
	strContents += strMsg;
	strContents += _T("\r\n");
	strContents += _T("\r\n");

	CFile file;

	if (file.Open(szFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone) == 0)
		return;

	char cameraKey[1024];
	StringToChar(strContents, cameraKey);

	file.SeekToEnd();
	int nLenth = strContents.GetLength();
	int nLenth2 = strlen(cameraKey);
	file.Write(cameraKey, nLenth2);
	file.Flush();
	file.Close();
}

// Write Log for PLC
void CGvisR2R_PunchDoc::LogPLC(CString strMsg, int nType)
{
	if (m_bOffLogPLC)
		return;

	CSafeLockDoc lock(&g_LogLockPLC);

	TCHAR szFile[MAX_PATH] = { 0, };
	TCHAR szPath[MAX_PATH] = { 0, };
	TCHAR* pszPos = NULL;

	_stprintf(szPath, PATH_LOG);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	_stprintf(szPath, PATH_LOG_PLC);
	if (!DirectoryExists(szPath))
		CreateDirectory(szPath, NULL);

	COleDateTime time = COleDateTime::GetCurrentTime();

	switch (nType)
	{
	case 0:
		_stprintf(szFile, _T("%s\\%s.txt"), szPath, COleDateTime::GetCurrentTime().Format(_T("%Y%m%d")));
		break;
	}

	CString strDate;
	CString strContents;
	CTime now;

	strDate.Format(_T("%s: "), COleDateTime::GetCurrentTime().Format(_T("%Y/%m/%d %H:%M:%S")));
	strContents = strDate;
	strContents += strMsg;
	strContents += _T("\r\n");
	strContents += _T("\r\n");

	CFile file;

	if (file.Open(szFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone) == 0)
		return;

	char cameraKey[1024];
	StringToChar(strContents, cameraKey);

	file.SeekToEnd();
	int nLenth = strContents.GetLength();
	int nLenth2 = strlen(cameraKey);
	file.Write(cameraKey, nLenth2);
	file.Flush();
	file.Close();
}

void CGvisR2R_PunchDoc::StringToChar(CString str, char* pCh) // char* returned must be deleted... 
{
	wchar_t*	wszStr;
	int				nLenth;

	USES_CONVERSION;
	//1. CString to wchar_t* conversion
	wszStr = T2W(str.GetBuffer(str.GetLength()));

	//2. wchar_t* to char* conversion
	nLenth = WideCharToMultiByte(CP_ACP, 0, wszStr, -1, NULL, 0, NULL, NULL); //char* 형에 대한길이를 구함 

																			  //3. wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wszStr, -1, pCh, nLenth, 0, 0);
	return;
}


void CGvisR2R_PunchDoc::DelItsAll(CString strPath)
{
	if (m_pFile)
	{
		m_pFile->DelItsAll(strPath);
	}
}


int CGvisR2R_PunchDoc::LoadPcrUp(CString sPath)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp, i, nC, nR;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	int nSerial = 0;
	CString sRmapPath = sPath, sSerial;
	int nPos = sRmapPath.ReverseFind('\\');
	if (nPos != -1)
	{
		sSerial = sRmapPath.Right(sRmapPath.GetLength() - nPos - 1);
		sRmapPath.Delete(nPos, sPath.GetLength() - nPos);
		sSerial.Delete(4, 4);
		nSerial = _ttoi(sSerial);
	}

	if (nSerial <= 0)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcr[0])
	{
		strFileData.Format(_T("PCR[0]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

	int nIdx;
	nIdx = GetPcrIdx0(nSerial);

	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData = CharToString(FileData);
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR[Up] 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcr[0]) return(2);
	if (!m_pPcr[0][nIdx]) return(2);

	BOOL bResetMkInfo = FALSE;

	m_pPcr[0][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[0][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[0][nIdx]->m_sItsCode = sItsCode;

	// Total Defect Numbers
	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;

	if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
	{
		pDoc->m_sEngModel = strModel;
		pDoc->m_sEngLotNum = strLot;
		pDoc->m_sEngLayerUp = strLayer;
		if (WorkingInfo.System.bUseITS)
			pDoc->m_sItsCode = sItsCode;
	}

	BOOL bUpdate = FALSE;

	if (WorkingInfo.LastJob.sLot != strLot || WorkingInfo.LastJob.sEngItsCode != sItsCode)
	{
		if (WorkingInfo.LastJob.sLot != strLot)
		{
			WorkingInfo.LastJob.sLot = strLot;
			pView->m_pEngrave->SetLotName();
		}
		bUpdate = TRUE;
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = sItsCode;
	}

	if (WorkingInfo.LastJob.sModel != strModel || WorkingInfo.LastJob.sLayerUp != strLayer)
	{
		if (WorkingInfo.LastJob.sModel != strModel)
		{
			WorkingInfo.LastJob.sModel = strModel;
			pView->m_pEngrave->SetModelName();
		}
		if (WorkingInfo.LastJob.sLayerUp != strLayer)
		{
			WorkingInfo.LastJob.sLayerUp = strLayer;
			pView->m_pEngrave->SetLayerUpName();
		}

		bUpdate = TRUE;
		pView->ResetMkInfo(0); // CAD 데이터 리로딩   0 : AOI-Up , 1 : AOI-Dn , 2 : AOI-UpDn
	}

	if (bUpdate)
	{
		WriteChangedModel();

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->UpdateData();

		if (GetTestMode() == MODE_OUTER)
		{
			BOOL bDualTestInner;
			CString sLot, sLayerUp, sLayerDn, str;
			if (!pDoc->GetItsSerialInfo(nSerial, bDualTestInner, sLot, sLayerUp, sLayerDn))
			{
				str.Format(_T("It is trouble to read GetItsSerialInfo()."));
				pView->MsgBox(str);
				return FALSE; // TRUE: CHANGED, FALSE: NO CHANGED 
			}

			if (m_pReelMapInnerUp)
				m_pReelMapInnerUp->ResetReelmapPath();

			if (m_pReelMapIts)
				m_pReelMapIts->ResetReelmapPath();

			if (bDualTestInner)
			{
				if (m_pReelMapInnerDn)
					m_pReelMapInnerDn->ResetReelmapPath();
				if (m_pReelMapInnerAllUp)
					m_pReelMapInnerAllUp->ResetReelmapPath();
				if (m_pReelMapInnerAllDn)
					m_pReelMapInnerAllDn->ResetReelmapPath();
			}
		}
	}

	int nTotDef = _tstoi(strTotalBadPieceNum);

	m_pPcr[0][nIdx]->Init(nSerial, nTotDef);

	if (nTotDef > 0)
	{
		for (i = 0; i < nTotDef; i++)
		{
			// Cam ID
			nTemp = strFileData.Find(',', 0);
			strCamID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_nCamId = _tstoi(strCamID);

			// Piece Number
			nTemp = strFileData.Find(',', 0);
			strPieceID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;

			// LoadStripPieceRegion_Binary()에 의해 PCS Index가 결정됨.
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
			{
				m_pPcr[0][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);
			}
			else
				m_pPcr[0][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);

			m_pPcr[0][nIdx]->m_pLayer[i] = 0; // Up

											  // BadPointPosX
			nTemp = strFileData.Find(',', 0);
			strBadPointPosX = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pDefPos[i].x = (long)_tstoi(strBadPointPosX);

			// BadPointPosY
			nTemp = strFileData.Find(',', 0);
			strBadPointPosY = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pDefPos[i].y = (long)_tstoi(strBadPointPosY);

			// BadName
			nTemp = strFileData.Find(',', 0);
			strBadName = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pDefType[i] = _tstoi(strBadName);

			pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[0].MasterInfo.nActionCode, m_pPcr[0][nIdx]->m_pDefPcs[i], nC, nR);
			m_pPcr[0][nIdx]->m_arDefType[nR][nC] = m_pPcr[0][nIdx]->m_pDefType[i];
			m_pPcr[0][nIdx]->m_arPcrLineNum[nR][nC] = i;
			m_pPcr[0][nIdx]->m_arDefTypeForIts[nR][nC] = m_pPcr[0][nIdx]->m_pDefType[i];
			m_pPcr[0][nIdx]->m_arPcrLineNumForIts[nR][nC] = i;

			// CellNum
			nTemp = strFileData.Find(',', 0);
			strCellNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pCell[i] = _tstoi(strCellNum);

			// ImageSize
			nTemp = strFileData.Find(',', 0);
			strImageSize = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pImgSz[i] = _tstoi(strImageSize);

			// ImageNum
			nTemp = strFileData.Find(',', 0);
			strImageNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pImg[i] = _tstoi(strImageNum);

			// strMarkingCode : -2 (NoMarking)
			nTemp = strFileData.Find('\n', 0);
			strMarkingCode = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[0][nIdx]->m_pMk[i] = _tstoi(strMarkingCode);
		}
	}

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPcrDn(CString sPath)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	FILE *fp;
	char FileD[200];
	size_t nFileSize, nRSize;
	char *FileData;
	CString strFileData;
	int nTemp, i, nC, nR;
	CString strHeaderErrorInfo, strModel, strLayer, strLot, sItsCode, strTotalBadPieceNum;
	CString strCamID, strPieceID, strBadPointPosX, strBadPointPosY, strBadName,
		strCellNum, strImageSize, strImageNum, strMarkingCode;

	int nSerial = 0;
	CString sRmapPath = sPath, sSerial;
	int nPos = sRmapPath.ReverseFind('\\');
	if (nPos != -1)
	{
		sSerial = sRmapPath.Right(sRmapPath.GetLength() - nPos - 1);
		sRmapPath.Delete(nPos, sPath.GetLength() - nPos);
		sSerial.Delete(4, 4);
		nSerial = _ttoi(sSerial);
	}

	if (nSerial <= 0)
	{
		strFileData.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcr[1])
	{
		strFileData.Format(_T("PCR[1]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(strFileData);
		return(2);
	}

	int nIdx;
	nIdx = GetPcrIdx1(nSerial);

	StringToChar(sPath, FileD);

	if ((fp = fopen(FileD, "r")) != NULL)
	{
		fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		/* Allocate space for a path name */
		FileData = (char*)calloc(nFileSize + 1, sizeof(char));

		nRSize = fread(FileData, sizeof(char), nFileSize, fp);
		strFileData = CharToString(FileData);
		fclose(fp);
		free(FileData);
	}
	else
	{
		strFileData.Format(_T("PCR[Dn] 파일이 존재하지 않습니다.\r\n%s"), sPath);
		pView->MsgBox(strFileData);
		return(2);
	}

	if (!m_pPcr[1])
		return(2);
	if (!m_pPcr[1][nIdx])
		return(2);

	m_pPcr[1][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[1][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	nTemp = strFileData.Find(',', 0);
	strHeaderErrorInfo = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_nErrPnl = _tstoi(strHeaderErrorInfo);

	// Model
	nTemp = strFileData.Find(',', 0);
	strModel = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sModel = strModel;

	// Layer
	nTemp = strFileData.Find(',', 0);
	strLayer = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sLayer = strLayer;

	// Lot
	nTemp = strFileData.Find(',', 0);
	strLot = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sLot = strLot;

	// Its Code
	nTemp = strFileData.Find('\n', 0);
	sItsCode = strFileData.Left(nTemp);
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;
	m_pPcr[1][nIdx]->m_sItsCode = sItsCode;

	// Total Defect Numbers
	nTemp = strFileData.Find('\n', 0);
	strTotalBadPieceNum = strFileData.Left(nTemp);;
	strFileData.Delete(0, nTemp + 1);
	nFileSize = nFileSize - nTemp - 1;


	if (pDoc->GetTestMode() == MODE_INNER || pDoc->GetTestMode() == MODE_OUTER)
	{
		pDoc->m_sEngModel = strModel;
		pDoc->m_sEngLotNum = strLot;
		pDoc->m_sEngLayerDn = strLayer;
		if (WorkingInfo.System.bUseITS)
			pDoc->m_sItsCode = sItsCode;
	}

	BOOL bUpdate = FALSE;

	if (WorkingInfo.LastJob.sLot != strLot || WorkingInfo.LastJob.sEngItsCode != sItsCode)
	{
		if (WorkingInfo.LastJob.sLot != strLot)
		{
			WorkingInfo.LastJob.sLot = strLot;
			pView->m_pEngrave->SetLotName();
		}
		bUpdate = TRUE;
		m_sItsCode = WorkingInfo.LastJob.sEngItsCode = sItsCode;
	}

	if (WorkingInfo.LastJob.sModel != strModel || WorkingInfo.LastJob.sLayerDn != strLayer)
	{
		if (WorkingInfo.LastJob.sModel != strModel)
		{
			WorkingInfo.LastJob.sModel = strModel;
			pView->m_pEngrave->SetModelName();
		}
		if (WorkingInfo.LastJob.sLayerDn != strLayer)
		{
			WorkingInfo.LastJob.sLayerDn = strLayer;
			pView->m_pEngrave->SetLayerDnName();
		}
		bUpdate = TRUE;
	}

	if (bUpdate)
	{
		WriteChangedModel();

		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->UpdateData();
	}

	int nTotDef = _tstoi(strTotalBadPieceNum);

	m_pPcr[1][nIdx]->Init(nSerial, nTotDef);

	if (nTotDef > 0)
	{
		for (i = 0; i < nTotDef; i++)
		{
			// Cam ID
			nTemp = strFileData.Find(',', 0);
			strCamID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_nCamId = _tstoi(strCamID);

			// Piece Number
			nTemp = strFileData.Find(',', 0);
			strPieceID = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;

			// LoadStripPieceRegion_Binary()에 의해 PCS Index가 결정됨.
			if (pDoc->WorkingInfo.System.bStripPcsRgnBin)	// DTS용
			{
				m_pPcr[1][nIdx]->m_pDefPcs[i] = _tstoi(strPieceID);
			}
			else
			{
				m_pPcr[1][nIdx]->m_pDefPcs[i] = MirrorLR(_tstoi(strPieceID));	// 초기 양면검사기용
			}

			m_pPcr[1][nIdx]->m_pLayer[i] = 1; // Dn

											  // BadPointPosX
			nTemp = strFileData.Find(',', 0);
			strBadPointPosX = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pDefPos[i].x = (long)_tstoi(strBadPointPosX);

			// BadPointPosY
			nTemp = strFileData.Find(',', 0);
			strBadPointPosY = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pDefPos[i].y = (long)_tstoi(strBadPointPosY);

			// BadName
			nTemp = strFileData.Find(',', 0);
			strBadName = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pDefType[i] = _tstoi(strBadName);

			// Temp for ITS - m_pPcr[0][nIdx]->m_pDefPcs[i] = Rotate180(_tstoi(strPieceID));
			pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[0].MasterInfo.nActionCode, Rotate180(m_pPcr[1][nIdx]->m_pDefPcs[i]), nC, nR);
			m_pPcr[1][nIdx]->m_arDefType[nR][nC] = m_pPcr[1][nIdx]->m_pDefType[i];
			m_pPcr[1][nIdx]->m_arPcrLineNum[nR][nC] = i;
			pDoc->m_Master[0].m_pPcsRgn->GetMkMatrix(pDoc->m_Master[0].MasterInfo.nActionCode, m_pPcr[1][nIdx]->m_pDefPcs[i], nC, nR);
			m_pPcr[1][nIdx]->m_arDefTypeForIts[nR][nC] = m_pPcr[1][nIdx]->m_pDefType[i];
			m_pPcr[1][nIdx]->m_arPcrLineNumForIts[nR][nC] = i;

			// CellNum
			nTemp = strFileData.Find(',', 0);
			strCellNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pCell[i] = _tstoi(strCellNum);

			// ImageSize
			nTemp = strFileData.Find(',', 0);
			strImageSize = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pImgSz[i] = _tstoi(strImageSize);

			// ImageNum
			nTemp = strFileData.Find(',', 0);
			strImageNum = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pImg[i] = _tstoi(strImageNum);

			// strMarkingCode : -2 (NoMarking)
			nTemp = strFileData.Find('\n', 0);
			strMarkingCode = strFileData.Left(nTemp);
			strFileData.Delete(0, nTemp + 1);
			nFileSize = nFileSize - nTemp - 1;
			m_pPcr[1][nIdx]->m_pMk[i] = _tstoi(strMarkingCode);
		}
	}

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPcrAllUp(CString sPath)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 0;

	int i, idx;//, k
	CString str;

	int nSerial = 0;
	CString sRmapPath = sPath, sSerial;
	int nPos = sRmapPath.ReverseFind('\\');
	if (nPos != -1)
	{
		sSerial = sRmapPath.Right(sRmapPath.GetLength() - nPos - 1);
		sRmapPath.Delete(nPos, sPath.GetLength() - nPos);
		sSerial.Delete(4, 4);
		nSerial = _ttoi(sSerial);
	}

	if (nSerial <= 0)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		pView->ClrDispMsg();
		AfxMessageBox(_T("Serial Error.16"));
		return 0;
	}

	if (nSerial < 0)
	{
		str.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(str);
		return(2);
	}

	if (!m_pPcr[2])
	{
		str.Format(_T("PCR[2]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(str);
		return(2);
	}

	int nIdx;
	nIdx = GetPcrIdx0(nSerial);

	if (!m_pPcr[0] || !m_pPcr[1] || !m_pPcr[2])
		return(2);
	if (!m_pPcr[0][nIdx] || !m_pPcr[1][nIdx] || !m_pPcr[2][nIdx])
		return(2);

	m_pPcr[2][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[2][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcr[0][nIdx]->m_nErrPnl < 0)
		m_pPcr[2][nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;
	else if (m_pPcr[1][nIdx]->m_nErrPnl < 0)
		m_pPcr[2][nIdx]->m_nErrPnl = m_pPcr[1][nIdx]->m_nErrPnl;
	else
		m_pPcr[2][nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;

	// Model
	m_pPcr[2][nIdx]->m_sModel = m_pPcr[0][nIdx]->m_sModel;

	// Layer
	m_pPcr[2][nIdx]->m_sLayer = m_pPcr[0][nIdx]->m_sLayer;

	// Lot
	m_pPcr[2][nIdx]->m_sLot = m_pPcr[0][nIdx]->m_sLot;

	int nTotDef[3] = { 0 };									// [0]: 상면, [1]: 하면, [2]: 상/하면 Merge
	nTotDef[0] = m_pPcr[0][nIdx]->m_nTotDef;			// 상면 불량 피스 수
	nTotDef[1] = m_pPcr[1][nIdx]->m_nTotDef;			// 하면 불량 피스 수

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow;
	int nTotPcsInner = m_MasterInner[0].m_pPcsRgn->m_nTotPcs;
	int nNodeXInner = m_MasterInner[0].m_pPcsRgn->m_nCol;
	int nNodeYInner = m_MasterInner[0].m_pPcsRgn->m_nRow;
	if (nTotPcs < 0 || nTotPcsInner < 0 || nTotPcsInner != nTotPcs || nNodeXInner != nNodeX || nNodeYInner != nNodeY)
	{
		str.Format(_T("It is trouble to run LoadPcrAllUp()."));
		pView->MsgBox(str);
		return 0;
	}
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs + 1];

	int nPcsId;												// nPcsId : CamMaster Pcs Index
															//pPcrMgr테이블의 nIdx에 하면의 불량을 먼저 기록하고 상면의 불량을 엎어서 최종 merge불량 테이블을 만듬.
	int nTotVerifyed[2] = { 0 };

	for (i = 0; i < nTotDef[1]; i++)						// 하면 불량 피스 수
	{
		if (m_pPcr[1][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcr[1][nIdx]->m_pDefPcs[i];				// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdx = i;	// Up+Dn				// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			pPcrMgr[nPcsId].nIdxDn = i; // Dn					// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[1]++;
		}
	}
	for (i = 0; i < nTotDef[0]; i++)						// 상면 불량 피스 수
	{
		if (m_pPcr[0][nIdx]->m_pMk[i] != -2)				// -2 (NoMarking)
		{
			nPcsId = m_pPcr[0][nIdx]->m_pDefPcs[i];				// nPcsId : CamMaster Pcs Index
			pPcrMgr[nPcsId].nIdx = i;	// Up+Dn				// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			pPcrMgr[nPcsId].nIdxUp = i;	// Up					// 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서임.
		}
		else
		{
			nTotVerifyed[0]++;
		}
	}


	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)							// Shot내 총 Piece수
	{
		if (pPcrMgr[i].nIdx > -1)	// Up+Dn				// 상하면 Merge [i:CamMaster Pcs Index]의 nIdx (0~)는 불량표시순서임.
			nTotDef[2]++;									// 상 / 하면 Merge한 총 불량피스수.
	}

	m_pPcr[2][nIdx]->Init(nSerial, nTotDef[2]);				// 제품시리얼, Shot내 총불량 피스수

	int nId[2], Ord; // [0]: 상면 0~불량피스순서, [1]: 하면 0~불량피스순서
	idx = 0; // 마킹순서 0~불량피스수만큼 정하기위해 현시점의 idx를 초기화함.
	if (nTotDef[2] > 0) // 상 / 하면 Merge한 총 불량피스수.
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp; // 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxUp (0~)는 불량표시순서 임. nId[0]: 상면에서의 PCR파일순서 인덱스
			nId[1] = pPcrMgr[nPcsId].nIdxDn; // 상하면 Merge [nPcsId:CamMaster Pcs Index]의 nIdxDn (0~)는 불량표시순서 임. nId[1]: 하면에서의 PCR파일순서 인덱스

			if (nId[0] > -1)
			{
				Ord = nId[0];
				// Cam ID
				m_pPcr[2][nIdx]->m_nCamId = m_pPcr[0][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[2][nIdx]->m_pDefPcs[idx] = m_pPcr[0][nIdx]->m_pDefPcs[Ord];
				m_pPcr[2][nIdx]->m_pLayer[idx] = m_pPcr[0][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcr[2][nIdx]->m_pDefPos[idx].x = m_pPcr[0][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcr[2][nIdx]->m_pDefPos[idx].y = m_pPcr[0][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcr[2][nIdx]->m_pDefType[idx] = m_pPcr[0][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcr[2][nIdx]->m_pCell[idx] = m_pPcr[0][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcr[2][nIdx]->m_pImgSz[idx] = m_pPcr[0][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcr[2][nIdx]->m_pImg[idx] = m_pPcr[0][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[2][nIdx]->m_pMk[idx] = m_pPcr[0][nIdx]->m_pMk[Ord];

				idx++;
			}
			else if (nId[1] > -1)
			{
				Ord = nId[1];
				// Cam ID
				m_pPcr[2][nIdx]->m_nCamId = m_pPcr[1][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[2][nIdx]->m_pDefPcs[idx] = m_pPcr[1][nIdx]->m_pDefPcs[Ord];
				m_pPcr[2][nIdx]->m_pLayer[idx] = m_pPcr[1][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcr[2][nIdx]->m_pDefPos[idx].x = m_pPcr[1][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcr[2][nIdx]->m_pDefPos[idx].y = m_pPcr[1][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcr[2][nIdx]->m_pDefType[idx] = m_pPcr[1][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcr[2][nIdx]->m_pCell[idx] = m_pPcr[1][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcr[2][nIdx]->m_pImgSz[idx] = m_pPcr[1][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcr[2][nIdx]->m_pImg[idx] = m_pPcr[1][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[2][nIdx]->m_pMk[idx] = m_pPcr[1][nIdx]->m_pMk[Ord];

				idx++;
			}
		}
	}

	delete[] pPcrMgr;

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::LoadPcrAllDn(CString sPath)	// return : 2(Failed), 1(정상), -1(Align Error, 노광불량), -2(Lot End)
{
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
	if (!bDualTest)
		return 1;

	int i, idx;//k, 
	CString str;

	int nSerial = 0;
	CString sRmapPath = sPath, sSerial;
	int nPos = sRmapPath.ReverseFind('\\');
	if (nPos != -1)
	{
		sSerial = sRmapPath.Right(sRmapPath.GetLength() - nPos - 1);
		sRmapPath.Delete(nPos, sPath.GetLength() - nPos);
		sSerial.Delete(4, 4);
		nSerial = _ttoi(sSerial);
	}

	if (nSerial <= 0)
	{
		str.Format(_T("PCR파일이 설정되지 않았습니다."));
		pView->MsgBox(str);
		return(2);
	}

	if (!m_pPcr[3])
	{
		str.Format(_T("PCR[3]관련 메모리가 할당되지 않았습니다."));
		pView->MsgBox(str);
		return(2);
	}

	int nIdx;
	nIdx = GetPcrIdx1(nSerial);

	if (!m_pPcr[0] || !m_pPcr[1] || !m_pPcr[3])
		return(2);
	if (!m_pPcr[0][nIdx] || !m_pPcr[1][nIdx] || !m_pPcr[3][nIdx])
		return(2);

	m_pPcr[3][nIdx]->m_nIdx = nIdx;							// m_nIdx : From 0 to nTot.....
	m_pPcr[3][nIdx]->m_nSerial = nSerial;

	// Error Code											// 1(정상), -1(Align Error, 노광불량), -2(Lot End)
	if (m_pPcr[1][nIdx]->m_nErrPnl < 0)
		m_pPcr[3][nIdx]->m_nErrPnl = m_pPcr[1][nIdx]->m_nErrPnl;
	else if (m_pPcr[0][nIdx]->m_nErrPnl < 0)
		m_pPcr[3][nIdx]->m_nErrPnl = m_pPcr[0][nIdx]->m_nErrPnl;
	else
		m_pPcr[3][nIdx]->m_nErrPnl = m_pPcr[1][nIdx]->m_nErrPnl;

	// Model
	m_pPcr[3][nIdx]->m_sModel = m_pPcr[1][nIdx]->m_sModel;

	// Layer
	m_pPcr[3][nIdx]->m_sLayer = m_pPcr[1][nIdx]->m_sLayer;

	// Lot
	m_pPcr[3][nIdx]->m_sLot = m_pPcr[1][nIdx]->m_sLot;

	int nTotDef[3];
	nTotDef[0] = m_pPcr[0][nIdx]->m_nTotDef;
	nTotDef[1] = m_pPcr[1][nIdx]->m_nTotDef;
	//	nTotDef[2] = nTotDef[0] + nTotDef[1];

	int nTotPcs = m_Master[0].m_pPcsRgn->m_nTotPcs;
	int nNodeX = m_Master[0].m_pPcsRgn->m_nCol;
	int nNodeY = m_Master[0].m_pPcsRgn->m_nRow;
	int nTotPcsInner = m_MasterInner[0].m_pPcsRgn->m_nTotPcs;
	int nNodeXInner = m_MasterInner[0].m_pPcsRgn->m_nCol;
	int nNodeYInner = m_MasterInner[0].m_pPcsRgn->m_nRow;
	if (nTotPcs < 0 || nTotPcsInner < 0 || nTotPcsInner != nTotPcs || nNodeXInner != nNodeX || nNodeYInner != nNodeY)
	{
		str.Format(_T("It is trouble to run LoadPcrAllDn()."));
		pView->MsgBox(str);
		return 0;
	}
	stPcrMerge *pPcrMgr = new stPcrMerge[nTotPcs + 1];

	int nComp, nPcsId;
	if (nTotDef[0] > nTotDef[1])
		nComp = nTotDef[0];
	else
		nComp = nTotDef[1];

	for (i = 0; i < nTotDef[0]; i++)
	{
		nPcsId = m_pPcr[0][nIdx]->m_pDefPcs[i];
		pPcrMgr[nPcsId].nIdx = i;
		pPcrMgr[nPcsId].nIdxUp = i;	// Up
	}
	for (i = 0; i < nTotDef[1]; i++)
	{
		nPcsId = m_pPcr[1][nIdx]->m_pDefPcs[i];
		pPcrMgr[nPcsId].nIdx = i;
		pPcrMgr[nPcsId].nIdxDn = i; // Dn
	}

	nTotDef[2] = 0;
	for (i = 0; i < nTotPcs; i++)
	{
		if (pPcrMgr[i].nIdx > -1)
			nTotDef[2]++;
	}

	m_pPcr[3][nIdx]->Init(nSerial, nTotDef[2]);

	int nId[2], Ord;
	idx = 0;
	if (nTotDef[2] > 0)
	{
		for (nPcsId = 0; nPcsId < nTotPcs; nPcsId++)
		{
			nId[0] = pPcrMgr[nPcsId].nIdxUp;
			nId[1] = pPcrMgr[nPcsId].nIdxDn;

			if (nId[1] > -1)
			{
				Ord = nId[1];
				// Cam ID
				m_pPcr[3][nIdx]->m_nCamId = m_pPcr[1][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[3][nIdx]->m_pDefPcs[idx] = m_pPcr[1][nIdx]->m_pDefPcs[Ord];
				m_pPcr[3][nIdx]->m_pLayer[idx] = m_pPcr[1][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcr[3][nIdx]->m_pDefPos[idx].x = m_pPcr[1][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcr[3][nIdx]->m_pDefPos[idx].y = m_pPcr[1][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcr[3][nIdx]->m_pDefType[idx] = m_pPcr[1][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcr[3][nIdx]->m_pCell[idx] = m_pPcr[1][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcr[3][nIdx]->m_pImgSz[idx] = m_pPcr[1][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcr[3][nIdx]->m_pImg[idx] = m_pPcr[1][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[3][nIdx]->m_pMk[idx] = m_pPcr[1][nIdx]->m_pMk[Ord];

				idx++;
			}
			else if (nId[0] > -1)
			{
				Ord = nId[0];
				// Cam ID
				m_pPcr[3][nIdx]->m_nCamId = m_pPcr[0][nIdx]->m_nCamId;
				// Piece Number
				m_pPcr[3][nIdx]->m_pDefPcs[idx] = m_pPcr[0][nIdx]->m_pDefPcs[Ord];
				m_pPcr[3][nIdx]->m_pLayer[idx] = m_pPcr[0][nIdx]->m_pLayer[Ord];
				// BadPointPosX
				m_pPcr[3][nIdx]->m_pDefPos[idx].x = m_pPcr[0][nIdx]->m_pDefPos[Ord].x;
				// BadPointPosY
				m_pPcr[3][nIdx]->m_pDefPos[idx].y = m_pPcr[0][nIdx]->m_pDefPos[Ord].y;
				// BadName
				m_pPcr[3][nIdx]->m_pDefType[idx] = m_pPcr[0][nIdx]->m_pDefType[Ord];
				// CellNum
				m_pPcr[3][nIdx]->m_pCell[idx] = m_pPcr[0][nIdx]->m_pCell[Ord];
				// ImageSize
				m_pPcr[3][nIdx]->m_pImgSz[idx] = m_pPcr[0][nIdx]->m_pImgSz[Ord];
				// ImageNum
				m_pPcr[3][nIdx]->m_pImg[idx] = m_pPcr[0][nIdx]->m_pImg[Ord];
				// strMarkingCode : -2 (NoMarking)
				m_pPcr[3][nIdx]->m_pMk[idx] = m_pPcr[0][nIdx]->m_pMk[Ord];

				idx++;
			}
		}
	}

	delete[] pPcrMgr;

	return (1); // 1(정상)
}

int CGvisR2R_PunchDoc::IsOfflineFolder() // 0 : Not exist, 1 : Exist only Up, 2 : Exist only Dn, 3 : Exist Up and Dn
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

void CGvisR2R_PunchDoc::SetStatus(CString sMenu, CString sItem, BOOL bOn)
{
	if (!m_bUseStatus) return;
	CString sPath = WorkingInfo.System.sPathMkStatus;
	CString sData = _T("");

	if (sPath.IsEmpty())
		return;

	sData.Format(_T("%d"), bOn > 0 ? 1 : 0);
	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

void CGvisR2R_PunchDoc::SetStatusInt(CString sMenu, CString sItem, int nData)
{
	if (!m_bUseStatus) return;
	CString sPath = WorkingInfo.System.sPathMkStatus;
	if (sPath.IsEmpty())
		return;
	CString sData;
	sData.Format(_T("%d"), nData);
	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
	return;
}

void CGvisR2R_PunchDoc::SetStatus(CString sMenu, CString sItem, CString sData)
{
	if (!m_bUseStatus) return;
	CString sPath = WorkingInfo.System.sPathMkStatus;
	if (sPath.IsEmpty())
		return;

	::WritePrivateProfileString(sMenu, sItem, sData, sPath);
}

CString CGvisR2R_PunchDoc::GetStatus(CString sMenu, CString sItem)
{
	if (!m_bUseStatus) return _T("");
	CString sPath = WorkingInfo.System.sPathMkStatus;
	TCHAR szData[512];

	if (sPath.IsEmpty())
		return _T("");

	if (0 < ::GetPrivateProfileString(sMenu, sItem, NULL, szData, sizeof(szData), sPath))
		return (CString(szData));

	return _T("");
}

BOOL CGvisR2R_PunchDoc::LoadStatus()
{
	CString sPath = WorkingInfo.System.sPathMkStatus;
	TCHAR szData[512];

	if (sPath.IsEmpty())
		return FALSE;

	if (0 < ::GetPrivateProfileString(_T("System"), _T("UseStatus"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		m_bUseStatus = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bUseStatus = FALSE;

	if (!m_bUseStatus) return FALSE;

	// [General] =================================================================================================================
	if (0 < ::GetPrivateProfileString(_T("General"), _T("bLotEnd"), NULL, szData, sizeof(szData), sPath))
		pView->m_bLotEnd = pView->m_pDlgMenu01->m_bLotEnd = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bLotEnd = pView->m_pDlgMenu01->m_bLotEnd = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bLotEndF"), NULL, szData, sizeof(szData), sPath))
		pView->m_bLotEndF = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bLotEndF = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bLastProc"), NULL, szData, sizeof(szData), sPath))
		pView->m_bLastProc = pView->m_pDlgMenu01->m_bLastProc = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bLastProc = pView->m_pDlgMenu01->m_bLastProc = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bMkSt0"), NULL, szData, sizeof(szData), sPath))
		pView->m_bMkSt[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bMkSt[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bMkSt1"), NULL, szData, sizeof(szData), sPath))
		pView->m_bMkSt[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bMkSt[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bCam"), NULL, szData, sizeof(szData), sPath))
		pView->m_bCam = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bCam = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bLastProcFromUp"), NULL, szData, sizeof(szData), sPath))
		pView->m_bLastProcFromUp = pView->m_pDlgMenu01->m_bLastProcFromUp = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bLastProcFromUp = pView->m_pDlgMenu01->m_bLastProcFromUp = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bLastProcFromEng"), NULL, szData, sizeof(szData), sPath))
		pView->m_bLastProcFromEng = pView->m_pDlgMenu01->m_bLastProcFromEng = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bLastProcFromEng = pView->m_pDlgMenu01->m_bLastProcFromEng = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bCycleStop"), NULL, szData, sizeof(szData), sPath))
		pView->m_bCycleStop = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bCycleStop = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bSerialDecrese"), NULL, szData, sizeof(szData), sPath))
		pView->m_bSerialDecrese = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bSerialDecrese = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bContDiffLot"), NULL, szData, sizeof(szData), sPath))
		pView->m_bContDiffLot = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bContDiffLot = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bDoneChgLot"), NULL, szData, sizeof(szData), sPath))
		m_bDoneChgLot = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bDoneChgLot = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bUseRTRYShiftAdjust"), NULL, szData, sizeof(szData), sPath))
		m_bUseRTRYShiftAdjust = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bUseRTRYShiftAdjust = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bUpdateYield"), NULL, szData, sizeof(szData), sPath))
		pView->m_bUpdateYield = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bUpdateYield = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bSkipAlign[0][0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bSkipAlign[0][0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bSkipAlign[0][0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bSkipAlign[0][1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bSkipAlign[0][1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bSkipAlign[0][1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bSkipAlign[0][2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bSkipAlign[0][2] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bSkipAlign[0][2] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bSkipAlign[0][3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bSkipAlign[0][3] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bSkipAlign[0][3] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bSkipAlign[1][0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bSkipAlign[1][0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bSkipAlign[1][0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bSkipAlign[1][1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bSkipAlign[1][1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bSkipAlign[1][1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bSkipAlign[1][2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bSkipAlign[1][2] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bSkipAlign[1][2] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bSkipAlign[1][3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bSkipAlign[1][3] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bSkipAlign[1][3] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReAlign[0][0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReAlign[0][0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReAlign[0][0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReAlign[0][1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReAlign[0][1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReAlign[0][1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReAlign[0][2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReAlign[0][2] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReAlign[0][2] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReAlign[0][3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReAlign[0][3] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReAlign[0][3] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReAlign[1][0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReAlign[1][0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReAlign[1][0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReAlign[1][1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReAlign[1][1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReAlign[1][1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReAlign[1][2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReAlign[1][2] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReAlign[1][2] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReAlign[1][3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReAlign[1][3] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReAlign[1][3] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bFailAlign[0][0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bFailAlign[0][0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bFailAlign[0][0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bFailAlign[0][1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bFailAlign[0][1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bFailAlign[0][1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bFailAlign[0][2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bFailAlign[0][2] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bFailAlign[0][2] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bFailAlign[0][3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bFailAlign[0][3] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bFailAlign[0][3] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bFailAlign[1][0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bFailAlign[1][0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bFailAlign[1][0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bFailAlign[1][1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bFailAlign[1][1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bFailAlign[1][1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bFailAlign[1][2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bFailAlign[1][2] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bFailAlign[1][2] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bFailAlign[1][3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bFailAlign[1][3] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bFailAlign[1][3] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bDoMk[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bDoMk[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bDoMk[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bDoMk[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bDoMk[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bDoMk[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bDoneMk[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bDoneMk[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bDoneMk[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bDoneMk[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bDoneMk[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bDoneMk[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReMark[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReMark[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReMark[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReMark[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReMark[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReMark[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAnswer[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAnswer[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAnswer[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAnswer[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAnswer[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAnswer[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAnswer[2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAnswer[2] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAnswer[2] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAnswer[3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAnswer[3] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAnswer[3] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bInitAuto"), NULL, szData, sizeof(szData), sPath))
		pView->m_bInitAuto = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bInitAuto = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bLoadMstInfo"), NULL, szData, sizeof(szData), sPath))
		pView->m_bLoadMstInfo = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bLoadMstInfo = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bBufEmpty[0]"), NULL, szData, sizeof(szData), sPath))
		m_bBufEmpty[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bBufEmpty[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bBufEmpty[1]"), NULL, szData, sizeof(szData), sPath))
		m_bBufEmpty[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bBufEmpty[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bBufEmptyF[0]"), NULL, szData, sizeof(szData), sPath))
		m_bBufEmptyF[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bBufEmptyF[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bBufEmptyF[1]"), NULL, szData, sizeof(szData), sPath))
		m_bBufEmptyF[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		m_bBufEmptyF[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bCont"), NULL, szData, sizeof(szData), sPath))
		pView->m_bCont = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bCont = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAoiTest[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAoiTest[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAoiTest[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAoiTest[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAoiTest[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAoiTest[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bWaitPcr[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bWaitPcr[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bWaitPcr[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bWaitPcr[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bWaitPcr[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bWaitPcr[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bEngTest"), NULL, szData, sizeof(szData), sPath))
		pView->m_bEngTest = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bEngTest = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bEngFdWrite"), NULL, szData, sizeof(szData), sPath))
		pView->m_bEngFdWrite = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bEngFdWrite = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bMkFdWrite"), NULL, szData, sizeof(szData), sPath))
		pView->m_bMkFdWrite = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bMkFdWrite = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bReadyDone"), NULL, szData, sizeof(szData), sPath))
		pView->m_bReadyDone = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bReadyDone = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bStopF_Verify"), NULL, szData, sizeof(szData), sPath))
		pView->m_bStopF_Verify = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bStopF_Verify = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bInitAutoLoadMstInfo"), NULL, szData, sizeof(szData), sPath))
		pView->m_bInitAutoLoadMstInfo = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bInitAutoLoadMstInfo = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bRejectDone[0][0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bRejectDone[0][0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bRejectDone[0][0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bRejectDone[0][1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bRejectDone[0][1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bRejectDone[0][1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bRejectDone[0][2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bRejectDone[0][2] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bRejectDone[0][2] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bRejectDone[0][3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bRejectDone[0][3] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bRejectDone[0][3] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bRejectDone[1][0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bRejectDone[1][0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bRejectDone[1][0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bRejectDone[1][1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bRejectDone[1][1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bRejectDone[1][1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bRejectDone[1][2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bRejectDone[1][2] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bRejectDone[1][2] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bRejectDone[1][3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bRejectDone[1][3] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bRejectDone[1][3] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bEngTest"), NULL, szData, sizeof(szData), sPath))
		pView->m_bEngTest = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bEngTest = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bEngTestF"), NULL, szData, sizeof(szData), sPath))
		pView->m_bEngTestF = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bEngTestF = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAoiTestF[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAoiTestF[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAoiTestF[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAoiTestF[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAoiTestF[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAoiTestF[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAoiFdWrite[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAoiFdWrite[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAoiFdWrite[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAoiFdWrite[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAoiFdWrite[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAoiFdWrite[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAoiFdWriteF[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAoiFdWriteF[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAoiFdWriteF[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bAoiFdWriteF[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bAoiFdWriteF[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bAoiFdWriteF[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bEngTestF"), NULL, szData, sizeof(szData), sPath))
		pView->m_bEngTestF = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bEngTestF = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bEngFdWriteF"), NULL, szData, sizeof(szData), sPath))
		pView->m_bEngFdWriteF = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bEngFdWriteF = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bMkFdWriteF"), NULL, szData, sizeof(szData), sPath))
		pView->m_bMkFdWriteF = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bMkFdWriteF = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bPcrInShare[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bPcrInShare[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bPcrInShare[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bPcrInShare[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bPcrInShare[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bPcrInShare[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bPcrInShareVs[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bPcrInShareVs[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bPcrInShareVs[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("bPcrInShareVs[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bPcrInShareVs[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bPcrInShareVs[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nLotEndAuto"), NULL, szData, sizeof(szData), sPath))
		pView->m_nLotEndAuto = _ttoi(szData);
	else
		pView->m_nLotEndAuto = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkStAuto"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStAuto = _ttoi(szData);
	else
		pView->m_nMkStAuto = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nPrevMkStAuto"), NULL, szData, sizeof(szData), sPath))
		pView->m_nPrevMkStAuto = _ttoi(szData);
	else
		pView->m_nPrevMkStAuto = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nLastProcAuto"), NULL, szData, sizeof(szData), sPath))
		pView->m_nLastProcAuto = _ttoi(szData);
	else
		pView->m_nLastProcAuto = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nLotEndSerial"), NULL, szData, sizeof(szData), sPath))
		pView->m_nLotEndSerial = _ttoi(szData);
	else
		pView->m_nLotEndSerial = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nStepAuto"), NULL, szData, sizeof(szData), sPath))
		pView->m_nStepAuto = _ttoi(szData);
	else
		pView->m_nStepAuto = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nPrevStepAuto"), NULL, szData, sizeof(szData), sPath))
		pView->m_nPrevStepAuto = _ttoi(szData);
	else
		pView->m_nPrevStepAuto = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nTotMk[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nTotMk[0] = _ttoi(szData);
	else
		pView->m_nTotMk[0] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nTotMk[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nTotMk[1] = _ttoi(szData);
	else
		pView->m_nTotMk[1] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nPrevTotMk[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nPrevTotMk[0] = _ttoi(szData);
	else
		pView->m_nPrevTotMk[0] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nPrevTotMk[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nPrevTotMk[1] = _ttoi(szData);
	else
		pView->m_nPrevTotMk[1] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nPrevMkPcs[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nPrevCurMk[0] = _ttoi(szData);
	else
		pView->m_nPrevCurMk[0] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nPrevMkPcs[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nPrevCurMk[1] = _ttoi(szData);
	else
		pView->m_nPrevCurMk[1] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nAoiCamInfoStrPcs[0]"), NULL, szData, sizeof(szData), sPath))
		m_nAoiCamInfoStrPcs[0] = _ttoi(szData);
	else
		m_nAoiCamInfoStrPcs[0] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nAoiCamInfoStrPcs[1]"), NULL, szData, sizeof(szData), sPath))
		m_nAoiCamInfoStrPcs[1] = _ttoi(szData);
	else
		m_nAoiCamInfoStrPcs[1] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nStepMk[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nStepMk[0] = _ttoi(szData);
	else
		pView->m_nStepMk[0] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nStepMk[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nStepMk[1] = _ttoi(szData);
	else
		pView->m_nStepMk[1] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nStepMk[2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nStepMk[2] = _ttoi(szData);
	else
		pView->m_nStepMk[2] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nStepMk[3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nStepMk[3] = _ttoi(szData);
	else
		pView->m_nStepMk[3] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkPcs[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkPcs[0] = _ttoi(szData);
	else
		pView->m_nMkPcs[0] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkPcs[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkPcs[1] = _ttoi(szData);
	else
		pView->m_nMkPcs[1] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkPcs[2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkPcs[2] = _ttoi(szData);
	else
		pView->m_nMkPcs[2] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkPcs[3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkPcs[3] = _ttoi(szData);
	else
		pView->m_nMkPcs[3] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkStrip[0][0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStrip[0][0] = _ttoi(szData);
	else
		pView->m_nMkStrip[0][0] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkStrip[0][1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStrip[0][1] = _ttoi(szData);
	else
		pView->m_nMkStrip[0][1] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkStrip[0][2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStrip[0][2] = _ttoi(szData);
	else
		pView->m_nMkStrip[0][2] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkStrip[0][3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStrip[0][3] = _ttoi(szData);
	else
		pView->m_nMkStrip[0][3] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkStrip[1][0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStrip[1][0] = _ttoi(szData);
	else
		pView->m_nMkStrip[1][0] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkStrip[1][1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStrip[1][1] = _ttoi(szData);
	else
		pView->m_nMkStrip[1][1] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkStrip[1][2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStrip[1][2] = _ttoi(szData);
	else
		pView->m_nMkStrip[1][2] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nMkStrip[1][3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nMkStrip[1][3] = _ttoi(szData);
	else
		pView->m_nMkStrip[1][3] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nEjectBufferLastShot"), NULL, szData, sizeof(szData), sPath))
		m_nEjectBufferLastShot = _ttoi(szData);
	else
		m_nEjectBufferLastShot = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nBufUpSerial[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nBufUpSerial[0] = _ttoi(szData);
	else
		pView->m_nBufUpSerial[0] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nBufUpSerial[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nBufUpSerial[1] = _ttoi(szData);
	else
		pView->m_nBufUpSerial[1] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nBufDnSerial[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nBufDnSerial[0] = _ttoi(szData);
	else
		pView->m_nBufDnSerial[0] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("nBufDnSerial[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_nBufDnSerial[1] = _ttoi(szData);
	else
		pView->m_nBufDnSerial[1] = 0;

	if (0 < ::GetPrivateProfileString(_T("General"), _T("Flag"), NULL, szData, sizeof(szData), sPath))
		pView->m_Flag = _ttoi(szData);
	else
		pView->m_Flag = 0;

	// [Thread] =================================================================================================================
	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_DISP_DEF"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_DISP_DEF = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_DISP_DEF = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_SHIFT2MK"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_SHIFT2MK = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_SHIFT2MK = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_UP"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_REELMAP_UP = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_REELMAP_UP = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_DN"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_REELMAP_DN = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_REELMAP_DN = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_ALLUP"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_REELMAP_ALLUP = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_REELMAP_ALLUP = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_ALLDN"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_REELMAP_ALLDN = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_REELMAP_ALLDN = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_UP"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_REELMAP_INNER_UP = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_REELMAP_INNER_UP = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_DN"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_REELMAP_INNER_DN = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_REELMAP_INNER_DN = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_ALLUP"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_REELMAP_INNER_ALLUP = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_REELMAP_INNER_ALLDN"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_REELMAP_INNER_ALLDN = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_DISP_DEF_INNER"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_DISP_DEF_INNER = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_DISP_DEF_INNER = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATAE_YIELD[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATAE_YIELD[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATAE_YIELD[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATAE_YIELD[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATAE_YIELD[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATAE_YIELD[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_UP"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_YIELD_UP = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_YIELD_UP = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_DN"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_YIELD_DN = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_YIELD_DN = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_ALLUP"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_YIELD_ALLUP = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_YIELD_ALLUP = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_UPDATE_YIELD_ALLDN"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_UPDATE_YIELD_ALLDN = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_UPDATE_YIELD_ALLDN = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_UP"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_REELMAP_YIELD_UP = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_REELMAP_YIELD_UP = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_DN"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_REELMAP_YIELD_DN = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_REELMAP_YIELD_DN = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ALLUP"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_REELMAP_YIELD_ALLUP = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_REELMAP_YIELD_ALLUP = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ALLDN"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_REELMAP_YIELD_ALLDN = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_REELMAP_YIELD_ALLDN = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_REELMAP_YIELD_ITS"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_REELMAP_YIELD_ITS = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_REELMAP_YIELD_ITS = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_MAKE_ITS_FILE_UP"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_MAKE_ITS_FILE_UP = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_MAKE_ITS_FILE_UP = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_MAKE_ITS_FILE_DN"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_MAKE_ITS_FILE_DN = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_MAKE_ITS_FILE_DN = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_MK[0]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_MK[0] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_MK[0] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_MK[1]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_MK[1] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_MK[1] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_MK[2]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_MK[2] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_MK[2] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("bTHREAD_MK[3]"), NULL, szData, sizeof(szData), sPath))
		pView->m_bTHREAD_MK[3] = _ttoi(szData) ? TRUE : FALSE;
	else
		pView->m_bTHREAD_MK[3] = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("nStepTHREAD_DISP_DEF"), NULL, szData, sizeof(szData), sPath))
		pView->m_nStepTHREAD_DISP_DEF = _ttoi(szData);
	else
		pView->m_nStepTHREAD_DISP_DEF = 0;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("nStepTHREAD_DISP_DEF_INNER"), NULL, szData, sizeof(szData), sPath))
		pView->m_nStepTHREAD_DISP_DEF_INNER = _ttoi(szData);
	else
		pView->m_nStepTHREAD_DISP_DEF_INNER = 0;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("nSerialRmapUpdate"), NULL, szData, sizeof(szData), sPath))
		pView->m_nSerialRmapUpdate = _ttoi(szData);
	else
		pView->m_nSerialRmapUpdate = 0;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("nSerialRmapInnerUpdate"), NULL, szData, sizeof(szData), sPath))
		pView->m_nSerialRmapInnerUpdate = _ttoi(szData);
	else
		pView->m_nSerialRmapInnerUpdate = 0;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("nSnTHREAD_UPDATAE_YIELD"), NULL, szData, sizeof(szData), sPath))
		pView->m_nSnTHREAD_UPDATAE_YIELD = _ttoi(szData);
	else
		pView->m_nSnTHREAD_UPDATAE_YIELD = 0;

	if (0 < ::GetPrivateProfileString(_T("Thread"), _T("nSnTHREAD_UPDATAE_YIELD_ITS"), NULL, szData, sizeof(szData), sPath))
		pView->m_nSnTHREAD_UPDATAE_YIELD_ITS = _ttoi(szData);
	else
		pView->m_nSnTHREAD_UPDATAE_YIELD_ITS = 0;

	// [Engrave] =================================================================================================================
	if (0 < ::GetPrivateProfileString(_T("Engrave"), _T("IsOnMking"), NULL, szData, sizeof(szData), sPath))
		BtnStatus.EngAuto.IsOnMking = _ttoi(szData) ? TRUE : FALSE;
	else
		BtnStatus.EngAuto.IsOnMking = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Engrave"), _T("IsMkDone"), NULL, szData, sizeof(szData), sPath))
		BtnStatus.EngAuto.IsMkDone = _ttoi(szData) ? TRUE : FALSE;
	else
		BtnStatus.EngAuto.IsMkDone = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Engrave"), _T("IsOnRead2d"), NULL, szData, sizeof(szData), sPath))
		BtnStatus.EngAuto.IsOnRead2d = _ttoi(szData) ? TRUE : FALSE;
	else
		BtnStatus.EngAuto.IsOnRead2d = FALSE;

	if (0 < ::GetPrivateProfileString(_T("Engrave"), _T("IsRead2dDone"), NULL, szData, sizeof(szData), sPath))
		BtnStatus.EngAuto.IsRead2dDone = _ttoi(szData) ? TRUE : FALSE;
	else
		BtnStatus.EngAuto.IsRead2dDone = FALSE;

	return TRUE;
}


BOOL CGvisR2R_PunchDoc::CheckPathCamMstSpec()
{
	CString sPathAoiUp, sPathAoiDn, sPathPunch, sMsg;
	CString sSpPunch, sSpAoiUp, sSpAoiDn;

	if (pDoc->WorkingInfo.System.sPathAoiUp.Right(1) != "\\")
		sPathAoiUp.Format(_T("%s\\AoiSet\\Mlbaoi.ini"), pDoc->WorkingInfo.System.sPathAoiUp);
	else
		sPathAoiUp.Format(_T("%sAoiSet\\Mlbaoi.ini"), pDoc->WorkingInfo.System.sPathAoiUp);

	if (pDoc->WorkingInfo.System.sPathAoiDn.Right(1) != "\\")
		sPathAoiDn.Format(_T("%s\\AoiSet\\Mlbaoi.ini"), pDoc->WorkingInfo.System.sPathAoiUp);
	else
		sPathAoiDn.Format(_T("%sAoiSet\\Mlbaoi.ini"), pDoc->WorkingInfo.System.sPathAoiUp);

	// PATH_WORKING_INFO

#ifndef TEST_MODE
	CFileFind finder;
	if (!finder.FindFile(sPathAoiUp))
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s\r\n파일을 공유할 수없습니다."), sPathAoiUp);
		pView->ClrDispMsg();
		pView->MsgBox(sMsg);
		return FALSE;
	}
	if (!finder.FindFile(sPathAoiDn))
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("%s\r\n파일을 공유할 수없습니다."), sPathAoiDn);
		pView->ClrDispMsg();
		pView->MsgBox(sMsg);
		return FALSE;
	}

	TCHAR szData[MAX_PATH];

	if (0 < ::GetPrivateProfileString(_T("System"), _T("CamSpecDir"), NULL, szData, sizeof(szData), PATH_WORKING_INFO))
		sSpPunch = CString(szData);
	else
		sSpPunch = _T("");

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SpecDir"), NULL, szData, sizeof(szData), sPathAoiUp))
		sSpAoiUp = CString(szData);
	else
		sSpAoiUp = _T("");

	if (0 < ::GetPrivateProfileString(_T("System"), _T("SpecDir"), NULL, szData, sizeof(szData), sPathAoiDn))
		sSpAoiDn = CString(szData);
	else
		sSpAoiDn = _T("");

	sSpPunch.MakeUpper();
	sSpAoiUp.MakeUpper();
	sSpAoiDn.MakeUpper();

	if (sSpPunch != sSpAoiUp || sSpPunch != sSpAoiDn)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("Punch부와 AOI 상하부의 캠마스터 스펙 폴더가 다릅니다."), sPathAoiDn);
		pView->ClrDispMsg();
		pView->MsgBox(sMsg);
		return FALSE;
	}

	if (sSpAoiUp != sSpAoiDn)
	{
		pView->SetAlarmToPlc(UNIT_PUNCH);
		sMsg.Format(_T("AOI 상하부의 캠마스터 스펙 폴더가 다릅니다."), sPathAoiDn);
		pView->ClrDispMsg();
		pView->MsgBox(sMsg);
		return FALSE;
	}
#endif
	return TRUE;
}

BOOL CGvisR2R_PunchDoc::CopyReelmapInner(int nItsSerial) // 외층 펀칭 작업 처음 시작할 때 한번만 수행
{
	CString sPathSrc, sPathDest, sMsg;
	CString sPathInnerUp, sPathInnerDn, sPathInnerAllUp, sPathInnerAllDn;
	CFileFind finder;

	if (pDoc->GetTestMode() == MODE_OUTER)
	{
		if (GetInnerReelmapPath(nItsSerial, sPathInnerUp, sPathInnerDn, sPathInnerAllUp, sPathInnerAllDn))
		{
			sPathSrc = sPathInnerUp;
			sPathDest = GetRmapPath(RMAP_INNER_UP);
			if (finder.FindFile(sPathSrc))
			{
				if (!CopyFile((LPCTSTR)sPathSrc, (LPCTSTR)sPathDest, FALSE))
				{
					sMsg.Format(_T("%s \r\n: 내층 릴맵 Up File Copy Fail"), sPathDest);
					pView->MsgBox(sMsg);
					return FALSE;
				}
			}
			else
			{
				sMsg.Format(_T("%s \r\n: 내층 릴맵 Up File이 없습니다."), sPathSrc);
				pView->MsgBox(sMsg);
				return FALSE;
			}

			sPathSrc = sPathInnerDn;
			sPathDest = GetRmapPath(RMAP_INNER_DN);
			if (finder.FindFile(sPathSrc))
			{
				if (!CopyFile((LPCTSTR)sPathSrc, (LPCTSTR)sPathDest, FALSE))
				{
					sMsg.Format(_T("%s \r\n: 내층 릴맵 Dn File Copy Fail"), sPathDest);
					pView->MsgBox(sMsg);
					return FALSE;
				}
			}
			else
			{
				sMsg.Format(_T("%s \r\n: 내층 릴맵 Dn File이 없습니다."), sPathSrc);
				pView->MsgBox(sMsg);
				return FALSE;
			}

			sPathSrc = sPathInnerAllUp;
			sPathDest = GetRmapPath(RMAP_INNER_ALLUP);
			if (finder.FindFile(sPathSrc))
			{
				if (!CopyFile((LPCTSTR)sPathSrc, (LPCTSTR)sPathDest, FALSE))
				{
					sMsg.Format(_T("%s \r\n: 내층 AllUp 릴맵 File Copy Fail"), sPathDest);
					pView->MsgBox(sMsg);
					return FALSE;
				}
			}
			else
			{
				sMsg.Format(_T("%s \r\n: 내층 릴맵 AllUp File이 없습니다."), sPathSrc);
				pView->MsgBox(sMsg);
				return FALSE;
			}

			sPathSrc = sPathInnerAllDn;
			sPathDest = GetRmapPath(RMAP_INNER_ALLDN);
			if (finder.FindFile(sPathSrc))
			{
				if (!CopyFile((LPCTSTR)sPathSrc, (LPCTSTR)sPathDest, FALSE))
				{
					sMsg.Format(_T("%s \r\n: 내층 릴맵 AllDn File Copy Fail"), sPathDest);
					pView->MsgBox(sMsg);
					return FALSE;
				}
			}
			else
			{
				sMsg.Format(_T("%s \r\n: 내층 릴맵 AllDn File이 없습니다."), sPathSrc);
				pView->MsgBox(sMsg);
				return FALSE;
			}
		}

		if (GetInnerYieldPath(nItsSerial, sPathInnerUp, sPathInnerDn, sPathInnerAllUp, sPathInnerAllDn))
		{
			sPathSrc = sPathInnerUp;
			sPathDest = GetYieldPath(RMAP_INNER_UP);
			if (finder.FindFile(sPathSrc))
			{
				if (!CopyFile((LPCTSTR)sPathSrc, (LPCTSTR)sPathDest, FALSE))
				{
					sMsg.Format(_T("%s \r\n: 내층 수율 Up File Copy Fail"), sPathDest);
					pView->MsgBox(sMsg);
					return FALSE;
				}
			}
			else
			{
				sMsg.Format(_T("%s \r\n: 내층 수율 Up File이 없습니다."), sPathSrc);
				pView->MsgBox(sMsg);
				return FALSE;
			}

			sPathSrc = sPathInnerDn;
			sPathDest = GetYieldPath(RMAP_INNER_DN);
			if (finder.FindFile(sPathSrc))
			{
				if (!CopyFile((LPCTSTR)sPathSrc, (LPCTSTR)sPathDest, FALSE))
				{
					sMsg.Format(_T("%s \r\n: 내층 수율 Dn File Copy Fail"), sPathDest);
					pView->MsgBox(sMsg);
					return FALSE;
				}
			}
			else
			{
				sMsg.Format(_T("%s \r\n: 내층 수율 Dn File이 없습니다."), sPathSrc);
				pView->MsgBox(sMsg);
				return FALSE;
			}

			sPathSrc = sPathInnerAllUp;
			sPathDest = GetYieldPath(RMAP_INNER_ALLUP);
			if (finder.FindFile(sPathSrc))
			{
				if (!CopyFile((LPCTSTR)sPathSrc, (LPCTSTR)sPathDest, FALSE))
				{
					sMsg.Format(_T("%s \r\n: 내층 수율 AllUp File Copy Fail"), sPathDest);
					pView->MsgBox(sMsg);
					return FALSE;
				}
			}
			else
			{
				sMsg.Format(_T("%s \r\n: 내층 수율 AllUp File이 없습니다."), sPathSrc);
				pView->MsgBox(sMsg);
				return FALSE;
			}

			sPathSrc = sPathInnerAllDn;
			sPathDest = GetYieldPath(RMAP_INNER_ALLDN);
			if (finder.FindFile(sPathSrc))
			{
				if (!CopyFile((LPCTSTR)sPathSrc, (LPCTSTR)sPathDest, FALSE))
				{
					sMsg.Format(_T("%s \r\n: 내층 수율 AllDn File Copy Fail"), sPathDest);
					pView->MsgBox(sMsg);
					return FALSE;
				}
			}
			else
			{
				sMsg.Format(_T("%s \r\n: 내층 수율 AllDn File이 없습니다."), sPathSrc);
				pView->MsgBox(sMsg);
				return FALSE;
			}
		}
	}
	else
		return FALSE;
}

CString CGvisR2R_PunchDoc::GetRmapPath(int nRmap)
{
	CString sPath = _T("");
	CString Path[5], str;

	switch (nRmap)
	{
	case RMAP_UP:
		str = _T("ReelMapDataUp.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->WorkingInfo.LastJob.sLot,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
		break;
	case RMAP_ALLUP:
		str = _T("ReelMapDataAll.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->WorkingInfo.LastJob.sLot,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerUp,
				str);
		}
		break;
	case RMAP_DN:
		str = _T("ReelMapDataDn.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->WorkingInfo.LastJob.sLot,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
		break;
	case RMAP_ALLDN:
		str = _T("ReelMapDataAll.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->WorkingInfo.LastJob.sLot,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
		{
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
				pDoc->WorkingInfo.LastJob.sModel,
				pDoc->Status.PcrShare[0].sLot,
				pDoc->WorkingInfo.LastJob.sLayerDn,
				str);
		}
		break;
	case RMAP_INNER_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLot;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLot;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLot;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLDN:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLot;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INOUTER_UP:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sLot, pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_INOUTER_DN:
		str = _T("ReelMapDataIO.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sLot, pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ITS:
		pDoc->GetCurrentInfoEng();
		str = _T("ReelMapDataIts.txt");
		sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModel,
			pDoc->WorkingInfo.LastJob.sEngItsCode,	//pDoc->m_sItsCode,
			str);
		break;
	}

	return sPath;
}

CString CGvisR2R_PunchDoc::GetYieldPath(int nRmap)
{
	CString sPath;
	CString str;

	switch (nRmap)
	{
	case RMAP_UP:
		str = _T("YieldUp.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModel,
			pDoc->WorkingInfo.LastJob.sLot,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_ALLUP:
		str = _T("YieldAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModel,
			pDoc->WorkingInfo.LastJob.sLot,
			pDoc->WorkingInfo.LastJob.sLayerUp,
			str);
		break;
	case RMAP_DN:
		str = _T("YieldDn.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModel,
			pDoc->WorkingInfo.LastJob.sLot,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_ALLDN:
		str = _T("YieldAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathOldFile,
			pDoc->WorkingInfo.LastJob.sModel,
			pDoc->WorkingInfo.LastJob.sLot,
			pDoc->WorkingInfo.LastJob.sLayerDn,
			str);
		break;
	case RMAP_INNER_UP:
		str = _T("YieldUp.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sInnerLot, pDoc->WorkingInfo.LastJob.sInnerLayerUp,
			str);
		break;
	case RMAP_INNER_DN:
		str = _T("YieldDn.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sInnerLot, pDoc->WorkingInfo.LastJob.sInnerLayerDn,
			str);
		break;
	case RMAP_INNER_ALLUP:
		str = _T("YieldAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sInnerLot, pDoc->WorkingInfo.LastJob.sInnerLayerUp,
			str);
		break;
	case RMAP_INNER_ALLDN:
		str = _T("YieldAll.txt");
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sEngItsCode,
			pDoc->WorkingInfo.LastJob.sInnerLot, pDoc->WorkingInfo.LastJob.sInnerLayerDn,
			str);
		break;
	case RMAP_INOUTER_UP:
		break;
	case RMAP_INOUTER_DN:
		break;
	case RMAP_ITS:
		str = _T("YieldIts.txt");
		sPath.Format(_T("%s%s\\%s\\%s"), pDoc->WorkingInfo.System.sPathItsFile,
			pDoc->WorkingInfo.LastJob.sModel, pDoc->WorkingInfo.LastJob.sEngItsCode, //pDoc->m_sItsCode,
			str);
		break;
	}

	return sPath;
}

double CGvisR2R_PunchDoc::GetVerifyPunchScore()
{
	//return m_dVerifyPunchScore;
	double dVerifyPunchScore = 85.0;
#ifdef USE_VISION
	if (pView->m_pVision[0])
		dVerifyPunchScore = pView->m_pVision[0]->GetVerifyPunchScore();
#endif
	return dVerifyPunchScore;
}

void CGvisR2R_PunchDoc::SetVerifyPunchScore(double dScore)
{
	//m_dVerifyPunchScore = dScore;
	//CString sData;
	//sData.Format(_T("%d"), int(m_dVerifyPunchScore));
	//::WritePrivateProfileString(_T("System"), _T("VerifyPunchScore"), sData, PATH_WORKING_INFO);
#ifdef USE_VISION
	if (pView->m_pVision[0])
		pView->m_pVision[0]->SetVerifyPunchScore(dScore);
	if (pView->m_pVision[1])
		pView->m_pVision[1]->SetVerifyPunchScore(dScore);
#endif
}

BOOL CGvisR2R_PunchDoc::MakeDirRmap(int nRmap)
{
	CString sPath = _T("");
	CString Path[5], str;

	switch (nRmap)
	{
	case RMAP_UP:
		str = _T("ReelMapDataUp.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
		{
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModel;
			Path[2] = pDoc->WorkingInfo.LastJob.sLot;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
			Path[4] = _T("");
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
		{
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModel;
			Path[2] = pDoc->WorkingInfo.LastJob.sLot;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
			Path[4] = _T("");
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		}
		break;
	case RMAP_ALLUP:
		str = _T("ReelMapDataAll.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[0])
		{
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModel;
			Path[2] = pDoc->WorkingInfo.LastJob.sLot;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
			Path[4] = _T("");
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[0])
		{
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModel;
			Path[2] = pDoc->WorkingInfo.LastJob.sLot;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerUp;
			Path[4] = _T("");
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		}
		break;
	case RMAP_DN:
		str = _T("ReelMapDataDn.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
		{
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModel;
			Path[2] = pDoc->WorkingInfo.LastJob.sLot;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
			Path[4] = _T("");
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
		{
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModel;
			Path[2] = pDoc->WorkingInfo.LastJob.sLot;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
			Path[4] = _T("");
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		}
		break;
	case RMAP_ALLDN:
		str = _T("ReelMapDataAll.txt");
		if (pDoc->m_bDoneChgLot || !pDoc->m_bNewLotShare[1])
		{
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModel;
			Path[2] = pDoc->WorkingInfo.LastJob.sLot;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
			Path[4] = _T("");
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		}
		else if (!pDoc->m_bDoneChgLot && pDoc->m_bNewLotShare[1])
		{
			Path[0] = pDoc->WorkingInfo.System.sPathOldFile;
			Path[1] = pDoc->WorkingInfo.LastJob.sModel;
			Path[2] = pDoc->WorkingInfo.LastJob.sLot;
			Path[3] = pDoc->WorkingInfo.LastJob.sLayerDn;
			Path[4] = _T("");
			sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], str);
		}
		break;
	case RMAP_INNER_UP:
		str = _T("ReelMapDataUp.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLot;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_DN:
		str = _T("ReelMapDataDn.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLot;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLUP:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLot;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INNER_ALLDN:
		str = _T("ReelMapDataAll.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sInnerLot;
		Path[4] = pDoc->WorkingInfo.LastJob.sInnerLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INOUTER_UP:
		str = _T("ReelMapDataIO.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sLot;
		Path[4] = pDoc->WorkingInfo.LastJob.sLayerUp;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_INOUTER_DN:
		str = _T("ReelMapDataIO.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = pDoc->WorkingInfo.LastJob.sLot;
		Path[4] = pDoc->WorkingInfo.LastJob.sLayerDn;
		sPath.Format(_T("%s%s\\%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	case RMAP_ITS:
		pDoc->GetCurrentInfoEng();
		str = _T("ReelMapDataIts.txt");
		Path[0] = pDoc->WorkingInfo.System.sPathItsFile;
		Path[1] = pDoc->WorkingInfo.LastJob.sModel;
		Path[2] = pDoc->WorkingInfo.LastJob.sEngItsCode;
		Path[3] = _T("");
		Path[4] = _T("");
		sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4], str);
		break;
	}

	if (!Path[0].IsEmpty() && !Path[1].IsEmpty())
	{
		sPath.Format(_T("%s%s"), Path[0], Path[1]);
		if (!DirectoryExists(sPath))
			CreateDirectory(sPath, NULL);

		if (!Path[2].IsEmpty())
		{
			sPath.Format(_T("%s%s\\%s"), Path[0], Path[1], Path[2]);
			if (!DirectoryExists(sPath))
				CreateDirectory(sPath, NULL);

			if (!Path[3].IsEmpty())
			{
				sPath.Format(_T("%s%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3]);
				if (!DirectoryExists(sPath))
					CreateDirectory(sPath, NULL);

				if (!Path[4].IsEmpty())
				{
					sPath.Format(_T("%s%s\\%s\\%s\\%s"), Path[0], Path[1], Path[2], Path[3], Path[4]);
					if (!DirectoryExists(sPath))
						CreateDirectory(sPath, NULL);
				}
			}
		}
	}

	return TRUE;
}

void CGvisR2R_PunchDoc::SetDualTest()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Dual AOI"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bDualTest = _ttoi(szData) ? TRUE : FALSE;
	else
		return;

	CString sData;

	if (WorkingInfo.LastJob.bDualTest)
	{
		sData = _T("6");
		pView->SetTwoMetal(TRUE, TRUE); // (TwoMetal, On)
	}
	else
	{
		sData = _T("12");
		pView->SetTwoMetal(FALSE, TRUE); // (OneMetal, On)
	}

	pDoc->WorkingInfo.System.sMaxDispDefImg = sData;
	::WritePrivateProfileString(_T("System"), _T("MAX_DISP_DEF_IMAGE"), sData, PATH_WORKING_INFO);
	pView->SetDualTest(WorkingInfo.LastJob.bDualTest);
}

void CGvisR2R_PunchDoc::SetAlignMethode()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Align Methode"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nAlignMethode = _ttoi(szData);
	else
		return;

	switch (WorkingInfo.LastJob.nAlignMethode)
	{
	case TWO_POINT:
		pDoc->SetMkInfo(_T("Signal"), _T("2PtAlign"), TRUE);
		pDoc->SetMkInfo(_T("Signal"), _T("4PtAlign"), FALSE);
		break;
	case FOUR_POINT:
		pDoc->SetMkInfo(_T("Signal"), _T("2PtAlign"), FALSE);
		pDoc->SetMkInfo(_T("Signal"), _T("4PtAlign"), TRUE);
		break;
	default:
		pDoc->SetMkInfo(_T("Signal"), _T("2PtAlign"), TRUE);
		pDoc->SetMkInfo(_T("Signal"), _T("4PtAlign"), FALSE);
		break;
	}
}

void CGvisR2R_PunchDoc::SetTestMode()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Test Mode"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.nTestMode = _ttoi(szData);
	else
		return;

	sPath = WorkingInfo.System.sPathMkCurrInfo;

	if (sPath.IsEmpty())
		return;

	CString sData;
	sData.Format(_T("%d"), WorkingInfo.LastJob.nTestMode);
	::WritePrivateProfileString(_T("Infomation"), _T("Test Mode"), sData, sPath);
	::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), sPath);
	::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);

	if (pView && pView->m_pMpe)
	{
		if (pDoc->GetTestMode() == MODE_INNER)
		{
			pView->MpeWrite(pView->Plc.DlgInfo.ModeInner, 1);// 내층 검사 사용/미사용 
			pView->MpeWrite(pView->Plc.DlgInfo.ModeOutter, 0);// 외층 검사 사용/미사용
			pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), TRUE);
			pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), FALSE);
			if (pView->m_pDlgMenu01)
				pView->m_pDlgMenu01->EnableItsMode(FALSE);
		}
		else if (pDoc->GetTestMode() == MODE_OUTER)
		{
			pView->MpeWrite(pView->Plc.DlgInfo.ModeInner, 0);// 내층 검사 사용/미사용
			pView->MpeWrite(pView->Plc.DlgInfo.ModeOutter, 1);// 외층 검사 사용/미사용
			pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), FALSE);
			pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), TRUE);
			if (pView->m_pDlgMenu01)
				pView->m_pDlgMenu01->EnableItsMode();
		}
		else
		{
			pView->MpeWrite(pView->Plc.DlgInfo.ModeInner, 0);// 내층 검사 사용/미사용
			pView->MpeWrite(pView->Plc.DlgInfo.ModeOutter, 0);// 외층 검사 사용/미사용
			pDoc->SetMkInfo(_T("Signal"), _T("Inner Test On"), FALSE);
			pDoc->SetMkInfo(_T("Signal"), _T("Outer Test On"), FALSE);
			if (pView->m_pDlgMenu01)
				pView->m_pDlgMenu01->EnableItsMode(FALSE);
		}
	}
}

void CGvisR2R_PunchDoc::RecoilerCcw()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("One Metal On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bOneMetal = _ttoi(szData) ? TRUE : FALSE;
	else
		return;

	BOOL bCCW = WorkingInfo.LastJob.bOneMetal;
	pView->SetFeedDir(0, bCCW); // Recoiler, CCW

	pDoc->SetMkInfo(_T("Signal"), _T("RecoilerCcw"), bCCW);
	pView->MpeWrite(pView->Plc.DlgInfo.OneMetal, bCCW ? 1 : 0);

	::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
	::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
}

void CGvisR2R_PunchDoc::UncoilerCcw()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Two Metal On"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bTwoMetal = _ttoi(szData) ? TRUE : FALSE;
	else
		return;

	BOOL bCCW = WorkingInfo.LastJob.bTwoMetal;
	pView->SetFeedDir(1, bCCW); // Uncoiler, CCW

	pDoc->SetMkInfo(_T("Signal"), _T("UncoilerCcw"), bCCW);
	pView->MpeWrite(pView->Plc.DlgInfo.TwoMetal, bCCW ? 1 : 0);

	::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
	::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
}

void CGvisR2R_PunchDoc::SetEngItsCode()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Engrave Its Code"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.sEngItsCode = CString(szData);
	else
		return;
}

void CGvisR2R_PunchDoc::SetDoorRecoiler()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use Recoiler Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bRclDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		return;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensRecoil"), pDoc->WorkingInfo.LastJob.bRclDrSen);
	pView->MpeWrite(pView->Plc.DlgInfo.DoorSensorRecoiler, pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0);	// 리코일러Door센서 사용
}

void CGvisR2R_PunchDoc::SetDoorAoiUp()
{
	TCHAR szData[200];
	CString sVal, sPath = PATH_WORKING_INFO;

	if (0 < ::GetPrivateProfileString(_T("Last Job"), _T("Use AoiUp Door Sensor"), NULL, szData, sizeof(szData), sPath))
		WorkingInfo.LastJob.bAoiUpDrSen = _ttoi(szData) ? TRUE : FALSE;
	else
		return;

	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensAoiUp"), pDoc->WorkingInfo.LastJob.bAoiUpDrSen);
	pView->MpeWrite(pView->Plc.DlgInfo.DoorSensorAoiUp, pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0);	// AOI(상) Door센서 사용
}

