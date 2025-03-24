// DlgUtil08.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgUtil08.h"
#include "afxdialogex.h"


#include "../MainFrm.h"
#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CMainFrame* pFrm;
extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

// CDlgUtil08 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgUtil08, CDialog)

CDlgUtil08::CDlgUtil08(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_UTIL_08, pParent)
{
	m_bConfirm = FALSE;
	m_bTIM_EXTRACT_REELMAP = FALSE;
	m_nStep_TIM_EXTRACT_REELMAP = 0;
}

CDlgUtil08::~CDlgUtil08()
{
	if (!m_bConfirm)
	{
		m_bConfirm = TRUE;
		DoConfirm();
	}
}

void CDlgUtil08::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUtil08, CDialog)
	ON_BN_CLICKED(IDC_BTN_REELMAP, &CDlgUtil08::OnBnClickedBtnReelmap)
	ON_BN_CLICKED(IDC_BTN_CONFIRM, &CDlgUtil08::OnBnClickedBtnConfirm)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgUtil08 메시지 처리기입니다.

BOOL CDlgUtil08::FileBrowse(CString& sPath)
{
	sPath = _T("");

	/* Load from file */
	CString FilePath;
	CFileFind finder;
	CString SrchPath, strTitleMsg, strErrMsg;

	CWaitCursor mCursor;

	CString DirPath[10];
	CString strWorkDir;
	BOOL bResult;

	CString strMcNum;
	int nAoiMachineNum = 0;

	// File Open Filter 
	static TCHAR BASED_CODE szFilter[] = _T("Mst Files (*.tif)|*.tif|All Files (*.*)|*.*||");

	// CFileDialog 
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);

	// Win2k Style FileDialog Box
	dlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12; // +12를 Win2k Style로 다이얼로그 박스가 Open됨.

													   // Open Directory
	//TCHAR strPrevDir[MAX_PATH];
	//DWORD dwLength = GetCurrentDirectory(MAX_PATH, strPrevDir);
	//strWorkDir = strPrevDir;	
	strWorkDir = pDoc->WorkingInfo.System.sPathOldFile;
	if (strWorkDir.Right(1) == _T("\\"))
		strWorkDir = strWorkDir.Left(strWorkDir.GetLength() - 1);

	dlg.m_ofn.lpstrInitialDir = strWorkDir;

	bResult = 0;

	// Dialog Open
	if (dlg.DoModal() == IDOK)
	{
		sPath = FilePath = dlg.GetPathName();
		return TRUE;
	}

	return FALSE;
}

void CDlgUtil08::OnBnClickedBtnReelmap()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sPath = _T("");

	if (!FileBrowse(sPath))
		return;
	GetDlgItem(IDC_STC_PATH_REELMAP)->SetWindowText(sPath);
	if (!sPath.IsEmpty())
	{
		ExtractReelmap(sPath);
	}
}


void CDlgUtil08::OnBnClickedBtnConfirm()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_bConfirm)
	{
		m_bConfirm = TRUE;
		DoConfirm();
	}

	OnOK();
}

BOOL CDlgUtil08::ExtractReelmap(CString sPath)
{
	CString sMsg;
	CDataFile *pDataFile = new CDataFile;

	if (!pDataFile->Open(sPath))
	{
		sMsg.Format(_T("%s File not found."), sPath);
		pView->MsgBox(sMsg);
		delete pDataFile;
		return FALSE;
	}

	if (!ExtractInfo(pDataFile))
	{
		sMsg.Format(_T("Error - ExtractInfo(pDataFile)"));
		pView->MsgBox(sMsg);
		delete pDataFile;
		return FALSE;
	}

	if (!LoadCamMstInfo())
	{
		sMsg.Format(_T("Error - LoadCamMstInfo()"));
		pView->MsgBox(sMsg);
		delete pDataFile;
		return FALSE;
	}

	if (!MakePcrDir(sPath))
	{
		sMsg.Format(_T("Error - MakePcrDir(sPath)"));
		pView->MsgBox(sMsg);
		delete pDataFile;
		return FALSE;
	}

	if (!ExtractPcr(pDataFile))
	{
		sMsg.Format(_T("Error - ExtractPcr(pDataFile)"));
		pView->MsgBox(sMsg);
		delete pDataFile;
		return FALSE;
	}

	delete pDataFile;
	return TRUE;
}

BOOL CDlgUtil08::ExtractInfo(CDataFile *pDataFile)
{
	int i, nPos;
	CString sLine = _T(""), sModel = _T(""), sLot = _T(""), sLayerUp = _T(""), sLayerDn = _T("");
	int nTotLine = pDataFile->GetTotalLines();
	for (i = 1; i <= nTotLine; i++)
	{
		sLine = pDataFile->GetLineString(i);

		nPos = sLine.Find(_T("모      델"), 0);
		if (nPos >= 0)
		{
			//sLine.Delete(0, nPos + 1);
			nPos = sLine.ReverseFind(_T(':'));
			sLine = sLine.Right(nPos + 1);
			sLine.Trim();
			sModel = sLine;
		}

		nPos = sLine.Find(_T("로      트"), 0);
		if (nPos >= 0)
		{
			//sLine.Delete(0, nPos + 1);
			nPos = sLine.ReverseFind(_T(':'));
			sLine = sLine.Right(nPos + 1);
			sLine.Trim();
			sLot = sLine;
		}

		nPos = sLine.Find(_T("상면레이어"), 0);
		if (nPos >= 0)
		{
			//sLine.Delete(0, nPos + 1);
			nPos = sLine.ReverseFind(_T(':'));
			sLine = sLine.Right(nPos + 1);
			sLine.Trim();
			sLayerUp = sLine;
		}

		nPos = sLine.Find(_T("하면레이어"), 0);
		if (nPos >= 0)
		{
			//sLine.Delete(0, nPos + 1);
			nPos = sLine.ReverseFind(_T(':'));
			sLine = sLine.Right(nPos + 1);
			sLine.Trim();
			sLayerDn = sLine;
			break;
		}
	}

	if (sModel.IsEmpty() || sLot.IsEmpty())
		return FALSE;

	pDoc->ReelmapReject.sModel = sModel;
	pDoc->ReelmapReject.sLot = sLot;
	pDoc->ReelmapReject.sLayerUp = sLayerUp;
	pDoc->ReelmapReject.sLayerDn = sLayerDn;

	return TRUE;
}

BOOL CDlgUtil08::LoadCamMstInfo()
{
	CString sMsg;

	sMsg.Format(_T("CamMaster에서 %s 모델의 데이터를 업로드 할까요?"), pDoc->ReelmapReject.sModel);
	if (IDNO == pView->MsgBox(sMsg, 0, MB_YESNO))
	{
		return FALSE;
	}

	pDoc->WorkingInfo.LastJob.sModel = pDoc->ReelmapReject.sModel;
	pDoc->WorkingInfo.LastJob.sLayerUp = pDoc->ReelmapReject.sLayerUp;
	pDoc->WorkingInfo.LastJob.sLayerDn = pDoc->ReelmapReject.sLayerDn;

	//pView->m_bLoadMstInfo = TRUE;
	pDoc->m_Master[0].Init(pDoc->WorkingInfo.System.sPathCamSpecDir,
		pDoc->WorkingInfo.LastJob.sModel,
		pDoc->WorkingInfo.LastJob.sLayerUp);
	pDoc->m_Master[0].LoadMstInfo();

	//m_nStep_TIM_EXTRACT_REELMAP = 0;
	//m_bTIM_EXTRACT_REELMAP = TRUE;
	//SetTimer(TIM_EXTRACT_REELMAP, 100, NULL);
	return TRUE;
}

BOOL CDlgUtil08::MakePcrDir(CString sPath)
{
	CString sPathDirPcr, sPathPcr;
	int nPos = sPath.ReverseFind(_T('\\'));
	CString sPathDir = sPath.Left(nPos);
	sPathDirPcr.Format(_T("%s\\PCR"), sPathDir);

	if (!pDoc->DirectoryExists(sPathDirPcr))
	{
		CreateDirectory(sPathDirPcr, NULL);
	}
	else
	{
		CFileFind cFile;
		BOOL bExist;
		sPathPcr.Format(_T("%s\\*.pcr"), sPathDirPcr);
		bExist = cFile.FindFile(sPathPcr);
		if(bExist)
			pDoc->m_pFile->DelPcrAll(sPathPcr);
	}

	return TRUE;
}

BOOL CDlgUtil08::ExtractPcr(CDataFile *pDataFile)
{
	int i, nPos;
	CString sLine = _T("");

	int nTotLine = pDataFile->GetTotalLines();
	for (i = 1; i <= nTotLine; i++)
	{
		sLine = pDataFile->GetLineString(i);
	}

	return TRUE;
}

void CDlgUtil08::DoConfirm()
{
	//UpdateData(TRUE);
	//pDoc->m_sItsCode = m_sItsCode;

	//m_sLotNum.MakeUpper();
	//pDoc->m_sLotNum = m_sLotNum;
	//pDoc->m_sProcessNum = m_sProcessNum;


	//pView->m_bLoadMstInfo = FALSE;

	//if ((pDoc->WorkingInfo.LastJob.sModelUp != pDoc->m_sModelUp) && !pDoc->m_sModelUp.IsEmpty())
	//{
	//	pDoc->WorkingInfo.LastJob.sModelUp = pDoc->m_sModelUp;
	//}

	//if ((pDoc->WorkingInfo.LastJob.sLayerUp != pDoc->m_sLayerUp) && !pDoc->m_sLayerUp.IsEmpty())
	//{
	//	pDoc->WorkingInfo.LastJob.sLayerUp = pDoc->m_sLayerUp;
	//	pDoc->m_bLoadMstInfo[0] = TRUE;
	//	pView->m_bLoadMstInfo = TRUE;
	//}
	//if ((pDoc->WorkingInfo.LastJob.sLayerDn != pDoc->m_sLayerDn) && !pDoc->m_sLayerDn.IsEmpty())
	//{
	//	pDoc->WorkingInfo.LastJob.sLayerDn = pDoc->m_sLayerDn;
	//	pDoc->m_bLoadMstInfo[1] = TRUE;
	//	pView->m_bLoadMstInfo = TRUE;
	//}
	//if ((pDoc->WorkingInfo.LastJob.sLotUp != pDoc->m_sLotNum) && !pDoc->m_sLayerUp.IsEmpty())
	//{
	//	pDoc->WorkingInfo.LastJob.sLotUp = pDoc->m_sLotNum;
	//}
	//if ((pDoc->WorkingInfo.LastJob.sLotDn != pDoc->m_sLotNum) && !pDoc->m_sLayerDn.IsEmpty())
	//{
	//	pDoc->WorkingInfo.LastJob.sLotDn = pDoc->m_sLotNum;
	//}

	//pDoc->SetCurrentInfo();
}


void CDlgUtil08::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (nIDEvent == TIM_EXTRACT_REELMAP)
	{
		KillTimer(TIM_EXTRACT_REELMAP);
		if (m_bTIM_EXTRACT_REELMAP)
			SetTimer(TIM_EXTRACT_REELMAP, 100, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}
