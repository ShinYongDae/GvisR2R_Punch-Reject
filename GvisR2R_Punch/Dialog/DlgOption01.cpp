// DlgOption01.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "DlgOption01.h"
//#include "afxdialogex.h"


#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;


// CDlgOption01 대화 상자입니다.

//IMPLEMENT_DYNAMIC(CDlgOption01, CDialog)

CDlgOption01::CDlgOption01(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOption01::IDD, pParent)
{
	m_pRect = NULL;
	m_bTIM_DISP_STS = FALSE;
}

CDlgOption01::~CDlgOption01()
{
	m_bTIM_DISP_STS = FALSE;
	if (m_pRect)
	{
		delete m_pRect;
		m_pRect = NULL;
	}
}

BOOL CDlgOption01::Create()
{
	return CDialog::Create(CDlgOption01::IDD);
}

void CDlgOption01::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgOption01, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_CHECK1, &CDlgOption01::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CDlgOption01::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CDlgOption01::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CDlgOption01::OnBnClickedCheck4)
	ON_EN_CHANGE(IDC_EDIT1, &CDlgOption01::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CDlgOption01::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_CHECK5, &CDlgOption01::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CDlgOption01::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK7, &CDlgOption01::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, &CDlgOption01::OnBnClickedCheck8)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK9, &CDlgOption01::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK10, &CDlgOption01::OnBnClickedCheck10)
END_MESSAGE_MAP()


// CDlgOption01 메시지 처리기입니다.


void CDlgOption01::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	//if (!m_pRect)
	//{
	//	m_pRect = new CRect;

	//	this->GetClientRect(m_pRect);
	//	m_pRect->top = 375;
	//	m_pRect->bottom += 375 +GetSystemMetrics(SM_CYSIZE);
	//	m_pRect->left = 3;
	//	m_pRect->right += 3;
	//	this->MoveWindow(m_pRect, TRUE);
	//}

	if (bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}
}

void CDlgOption01::AtDlgShow()
{
}

void CDlgOption01::AtDlgHide()
{

}


BOOL CDlgOption01::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	DispStatus();
	m_bTIM_DISP_STS = TRUE;
	SetTimer(TIM_DISP_STS, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgOption01::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	pDoc->m_bOffLogAuto = bOn;

	if(bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("OffLogAuto"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("OffLogAuto"), _T("0"), PATH_WORKING_INFO);
	}
}


void CDlgOption01::OnBnClickedCheck2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	pDoc->m_bOffLogPLC = bOn;

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("OffLogPLC"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("OffLogPLC"), _T("0"), PATH_WORKING_INFO);
	}
}


void CDlgOption01::OnBnClickedCheck3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
	pDoc->m_bDebugGrabAlign = bOn;

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("DebugGrabAlign"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("DebugGrabAlign"), _T("0"), PATH_WORKING_INFO);
	}
}


void CDlgOption01::OnBnClickedCheck4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();
	pDoc->m_bUseRTRYShiftAdjust = bOn; pDoc->SetStatus(_T("General"), _T("bUseRTRYShiftAdjust"), pDoc->m_bUseRTRYShiftAdjust);

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("USE_RTR_SHIFT_ADJUST"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("USE_RTR_SHIFT_ADJUST"), _T("0"), PATH_WORKING_INFO);
	}
}


void CDlgOption01::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sPath = PATH_WORKING_INFO;
	CString sData;
	GetDlgItem(IDC_EDIT1)->GetWindowText(sData);
	pDoc->m_dShiftAdjustRatio = _tstof(sData);
	::WritePrivateProfileString(_T("System"), _T("RTR_SHIFT_ADJUST_RATIO"), sData, sPath);
}


void CDlgOption01::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString sPath = PATH_WORKING_INFO;
	CString sData;
	GetDlgItem(IDC_EDIT2)->GetWindowText(sData);
	pDoc->m_nDelayShow = _ttoi(sData);
	::WritePrivateProfileString(_T("System"), _T("Delay Show Time"), sData, sPath);
}


void CDlgOption01::OnBnClickedCheck5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck();
	pDoc->m_bUseStatus = bOn;

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("UseStatus"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("UseStatus"), _T("0"), PATH_WORKING_INFO);
	}
}

void CDlgOption01::OnBnClickedCheck6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();
	pDoc->WorkingInfo.System.bSaveReelmapTable = bOn;

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("SaveReelmapTable"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("SaveReelmapTable"), _T("0"), PATH_WORKING_INFO);
	}
}


void CDlgOption01::OnBnClickedCheck7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck();
	pDoc->WorkingInfo.System.bRemakeReelmapInner = bOn;

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("RemakeReelmapInner"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("RemakeReelmapInner"), _T("0"), PATH_WORKING_INFO);
	}
}


void CDlgOption01::OnBnClickedCheck8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck();
	pDoc->WorkingInfo.System.bDuplicateRmap = bOn;

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("DuplicateReelmap"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("DuplicateReelmap"), _T("0"), PATH_WORKING_INFO);
	}
}

void CDlgOption01::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == TIM_DISP_STS)
	{
		KillTimer(TIM_DISP_STS);
		if (this->IsWindowVisible())
		{			
			m_bTIM_DISP_STS = FALSE;
		}
		else
		{
			this->ShowWindow(SW_SHOW);
		}
		if (m_bTIM_DISP_STS)
			SetTimer(TIM_DISP_STS, 100, NULL);
	}

	CDialog::OnTimer(nIDEvent);
}

void CDlgOption01::DispStatus()
{
	CString str;

	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(pDoc->m_bOffLogAuto);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(pDoc->m_bOffLogPLC);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(pDoc->m_bDebugGrabAlign);

	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(pDoc->m_bUseRTRYShiftAdjust);
	str.Format(_T("%3.2f"), pDoc->m_dShiftAdjustRatio);
	GetDlgItem(IDC_EDIT1)->SetWindowText(str);

	str.Format(_T("%d"), pDoc->m_nDelayShow);
	GetDlgItem(IDC_EDIT2)->SetWindowText(str);
	((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(pDoc->m_bUseStatus);
	((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(pDoc->WorkingInfo.System.bSaveReelmapTable);
	((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(pDoc->WorkingInfo.System.bRemakeReelmapInner);
	((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(pDoc->WorkingInfo.System.bDuplicateRmap);
	((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(pDoc->WorkingInfo.System.bInsertPunchingToDts);
	((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(pDoc->WorkingInfo.System.bDebugEngSig);

	if (pView->m_pMotion)
	{
		int nAlignMethodeOnCamMst = pDoc->m_Master[0].GetAlignMethode();
		int nAlignMethode = pDoc->WorkingInfo.LastJob.nAlignMethode;
		BOOL b2PointAlign = nAlignMethodeOnCamMst & TWO_POINT;
		BOOL b4PointAlign = nAlignMethodeOnCamMst & FOUR_POINT;

		if (b4PointAlign)
		{
			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[0], pDoc->m_Master[0].m_stAlignMk2.Y0 + pView->m_pMotion->m_dPinPosY[0]);
			GetDlgItem(IDC_STATIC_ALIGN_L0)->SetWindowText(str);
			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[0], pDoc->m_Master[0].m_stAlignMk2.Y1 + pView->m_pMotion->m_dPinPosY[0]);
			GetDlgItem(IDC_STATIC_ALIGN_L1)->SetWindowText(str);
			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk2.X2 + pView->m_pMotion->m_dPinPosX[0], pDoc->m_Master[0].m_stAlignMk2.Y2 + pView->m_pMotion->m_dPinPosY[0]);
			GetDlgItem(IDC_STATIC_ALIGN_L2)->SetWindowText(str);
			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk2.X3 + pView->m_pMotion->m_dPinPosX[0], pDoc->m_Master[0].m_stAlignMk2.Y3 + pView->m_pMotion->m_dPinPosY[0]);
			GetDlgItem(IDC_STATIC_ALIGN_L3)->SetWindowText(str);

			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk2.X0 + pView->m_pMotion->m_dPinPosX[1], pDoc->m_Master[0].m_stAlignMk2.Y0 + pView->m_pMotion->m_dPinPosY[1]);
			GetDlgItem(IDC_STATIC_ALIGN_R0)->SetWindowText(str);
			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk2.X1 + pView->m_pMotion->m_dPinPosX[1], pDoc->m_Master[0].m_stAlignMk2.Y1 + pView->m_pMotion->m_dPinPosY[1]);
			GetDlgItem(IDC_STATIC_ALIGN_R1)->SetWindowText(str);
			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk2.X2 + pView->m_pMotion->m_dPinPosX[1], pDoc->m_Master[0].m_stAlignMk2.Y2 + pView->m_pMotion->m_dPinPosY[1]);
			GetDlgItem(IDC_STATIC_ALIGN_R2)->SetWindowText(str);
			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk2.X3 + pView->m_pMotion->m_dPinPosX[1], pDoc->m_Master[0].m_stAlignMk2.Y3 + pView->m_pMotion->m_dPinPosY[1]);
			GetDlgItem(IDC_STATIC_ALIGN_R3)->SetWindowText(str);
		}
		else
		{
			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN_L0)->SetWindowText(str);
			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN_L1)->SetWindowText(str);
			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN_L2)->SetWindowText(str);
			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN_L3)->SetWindowText(str);

			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN_R0)->SetWindowText(str);
			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN_R1)->SetWindowText(str);
			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN_R2)->SetWindowText(str);
			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN_R3)->SetWindowText(str);
		}

		if(b2PointAlign)
		{
			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[0], pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[0]);
			GetDlgItem(IDC_STATIC_ALIGN2_L0)->SetWindowText(str);
			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[0], pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[0]);
			GetDlgItem(IDC_STATIC_ALIGN2_L1)->SetWindowText(str);

			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk.X0 + pView->m_pMotion->m_dPinPosX[1], pDoc->m_Master[0].m_stAlignMk.Y0 + pView->m_pMotion->m_dPinPosY[1]);
			GetDlgItem(IDC_STATIC_ALIGN2_R0)->SetWindowText(str);
			str.Format(_T("%.3f,%.3f"), pDoc->m_Master[0].m_stAlignMk.X1 + pView->m_pMotion->m_dPinPosX[1], pDoc->m_Master[0].m_stAlignMk.Y1 + pView->m_pMotion->m_dPinPosY[1]);
			GetDlgItem(IDC_STATIC_ALIGN2_R1)->SetWindowText(str);
		}
		else
		{
			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN2_L0)->SetWindowText(str);
			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN2_L1)->SetWindowText(str);

			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN2_R0)->SetWindowText(str);
			str.Format(_T(""));
			GetDlgItem(IDC_STATIC_ALIGN2_R1)->SetWindowText(str);
		}
	}

	// 마킹 금지 구역 - Left
	str.Format(_T("%.3f"), pDoc->m_pntNoMkLeft[0][0]); // LT
	GetDlgItem(IDC_EDIT_NO_MK_L0_LT_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkLeft[0][1]); // LB
	GetDlgItem(IDC_EDIT_NO_MK_L0_LB_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkLeft[0][2]); // RB
	GetDlgItem(IDC_EDIT_NO_MK_L0_RB_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkLeft[0][3]); // RT
	GetDlgItem(IDC_EDIT_NO_MK_L0_RT_X)->SetWindowText(str);

	str.Format(_T("%.3f"), pDoc->m_pntNoMkLeft[1][0]); // LT
	GetDlgItem(IDC_EDIT_NO_MK_L1_LT_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkLeft[1][1]); // LB
	GetDlgItem(IDC_EDIT_NO_MK_L1_LB_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkLeft[1][2]); // RB
	GetDlgItem(IDC_EDIT_NO_MK_L1_RB_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkLeft[1][3]); // RT
	GetDlgItem(IDC_EDIT_NO_MK_L1_RT_X)->SetWindowText(str);

	// 마킹 금지 구역 - Right
	str.Format(_T("%.3f"), pDoc->m_pntNoMkRight[0][0]); // LT
	GetDlgItem(IDC_EDIT_NO_MK_R0_LT_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkRight[0][1]); // LB
	GetDlgItem(IDC_EDIT_NO_MK_R0_LB_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkRight[0][2]); // RB
	GetDlgItem(IDC_EDIT_NO_MK_R0_RB_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkRight[0][3]); // RT
	GetDlgItem(IDC_EDIT_NO_MK_R0_RT_X)->SetWindowText(str);

	str.Format(_T("%.3f"), pDoc->m_pntNoMkRight[1][0]); // LT
	GetDlgItem(IDC_EDIT_NO_MK_R1_LT_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkRight[1][1]); // LB
	GetDlgItem(IDC_EDIT_NO_MK_R1_LB_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkRight[1][2]); // RB
	GetDlgItem(IDC_EDIT_NO_MK_R1_RB_X)->SetWindowText(str);
	str.Format(_T("%.3f"), pDoc->m_pntNoMkRight[1][3]); // RT
	GetDlgItem(IDC_EDIT_NO_MK_R1_RT_X)->SetWindowText(str);
}


void CDlgOption01::OnBnClickedCheck9()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck();
	pDoc->WorkingInfo.System.bInsertPunchingToDts = bOn;

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("InsertPunchingToDts"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("InsertPunchingToDts"), _T("0"), PATH_WORKING_INFO);
	}
}


void CDlgOption01::OnBnClickedCheck10()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();
	pDoc->WorkingInfo.System.bDebugEngSig = bOn;

	if (bOn)
	{
		::WritePrivateProfileString(_T("System"), _T("DebugEngSig"), _T("1"), PATH_WORKING_INFO);
	}
	else
	{
		::WritePrivateProfileString(_T("System"), _T("DebugEngSig"), _T("0"), PATH_WORKING_INFO);
	}

	if(pView->m_pDlgMenu02)
		pView->m_pDlgMenu02->ShowDebugEngSig();
}

