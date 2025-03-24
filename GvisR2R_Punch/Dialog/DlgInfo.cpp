// DlgInfo.cpp : implementation file
//

#include "stdafx.h"
#include "../gvisr2r_punch.h"
#include "DlgInfo.h"
#include "DlgKeyNum.h"
#include "DlgUser.h"
#include "DlgUtil08.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../GvisR2R_PunchDoc.h"
#include "../GvisR2R_PunchView.h"

extern CGvisR2R_PunchDoc* pDoc;
extern CGvisR2R_PunchView* pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo dialog


CDlgInfo::CDlgInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bLoadImg = FALSE;
	m_bTIM_DISP_STS = FALSE;
}

CDlgInfo::~CDlgInfo()
{
	CString sData;

	m_bTIM_DISP_STS = FALSE;

	sData.Format(_T("%d"), pDoc->WorkingInfo.System.bUseDevicePartial ? 1 : 0);
	::WritePrivateProfileString(_T("System"), _T("UseDevicePartial"), sData, PATH_WORKING_INFO);
	sData.Format(_T("%d"), pDoc->WorkingInfo.System.bUseEngrave ? 1 : 0);
	::WritePrivateProfileString(_T("System"), _T("UseEngrave"), sData, PATH_WORKING_INFO);
	sData.Format(_T("%d"), pDoc->WorkingInfo.System.bUseAoiUp ? 1 : 0);
	::WritePrivateProfileString(_T("System"), _T("UseAoiUp"), sData, PATH_WORKING_INFO);
	sData.Format(_T("%d"), pDoc->WorkingInfo.System.bUseAoiDn ? 1 : 0);
	::WritePrivateProfileString(_T("System"), _T("UseAoiDn"), sData, PATH_WORKING_INFO);
	sData.Format(_T("%d"), pDoc->WorkingInfo.System.bUsePunch ? 1 : 0);
	::WritePrivateProfileString(_T("System"), _T("UsePunch"), sData, PATH_WORKING_INFO);
	sData.Format(_T("%d"), pDoc->WorkingInfo.System.bUsePunchOnly ? 1 : 0);
	::WritePrivateProfileString(_T("System"), _T("UsePunchOnly"), sData, PATH_WORKING_INFO);

	if (pView->m_pDlgMenu01)
		pView->m_pDlgMenu01->UpdateData();
}

//BOOL CDlgInfo::Create()
//{
//	return CDialog::Create(IDD_DLG_INFO);
//	//return CDialog::Create(CDlgInfo::IDD);
//}

void CDlgInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInfo, CDialog)
	//{{AFX_MSG_MAP(CDlgInfo)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_STC_0008, OnStc0008)
	ON_BN_CLICKED(IDC_CHK_000, OnChk000)
	ON_BN_CLICKED(IDC_CHK_001, OnChk001)
	ON_BN_CLICKED(IDC_CHK_002, OnChk002)
	ON_BN_CLICKED(IDC_CHK_003, OnChk003)
	ON_BN_CLICKED(IDC_CHK_004, OnChk004)
	ON_BN_CLICKED(IDC_CHK_005, OnChk005)
	ON_BN_CLICKED(IDC_CHK_006, OnChk006)
	ON_BN_CLICKED(IDC_CHK_007, OnChk007)
	ON_BN_CLICKED(IDC_CHK_008, OnChk008)
	ON_BN_CLICKED(IDC_CHK_009, OnChk009)
	ON_BN_CLICKED(IDC_CHK_010, OnChk010)
	ON_BN_CLICKED(IDC_CHK_011, OnChk011)
	ON_BN_CLICKED(IDC_CHK_26, OnChk26)
	ON_BN_CLICKED(IDC_STC_0012, OnStc0012)
	ON_BN_CLICKED(IDC_STC_0016, OnStc0016)
	ON_BN_CLICKED(IDC_STC_0020, OnStc0020)
	ON_BN_CLICKED(IDC_STC_0024, OnStc0024)
	ON_BN_CLICKED(IDC_STC_0030, OnStc0030)
	ON_BN_CLICKED(IDC_STC_0034, OnStc0034)
	ON_BN_CLICKED(IDC_STC_0037, OnStc0037)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_STC_174, OnStc174)
	ON_BN_CLICKED(IDC_STC_178, OnStc178)
	ON_BN_CLICKED(IDC_STC_32, OnStc32)
	ON_BN_CLICKED(IDC_STC_183, OnStc183)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_STC_61, OnStc61)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_DUAL, OnChkUseAoiDual)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_SINGLE, OnChkUseAoiSingle)
	ON_BN_CLICKED(IDC_CHK_SAMPLE_TEST, OnChkSampleTest)
	ON_BN_CLICKED(IDC_CHK_RECOILER_CCW, OnChkRecoilerCcw)
	ON_BN_CLICKED(IDC_CHK_UNCOILER_CCW, OnChkUncoilerCcw)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_INNER, OnChkUseAoiInner)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_OUTER, OnChkUseAoiOuter)
	ON_BN_CLICKED(IDC_STC_181, OnStc181)
	ON_BN_CLICKED(IDC_CHK_19, OnChk19)
	ON_BN_CLICKED(IDC_CHK_23, OnChk23)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHK_4_POINT_ALIGN, &CDlgInfo::OnBnClickedChk4PointAlign)
	ON_BN_CLICKED(IDC_CHK_2_POINT_ALIGN, &CDlgInfo::OnBnClickedChk2PointAlign)
	ON_BN_CLICKED(IDC_CHK_86, &CDlgInfo::OnBnClickedChk86)
	ON_BN_CLICKED(IDC_CHK_85, &CDlgInfo::OnBnClickedChk85)
	ON_BN_CLICKED(IDC_CHK_1187, &CDlgInfo::OnBnClickedChk1187)
	ON_BN_CLICKED(IDC_CHK_1188, &CDlgInfo::OnBnClickedChk1188)
	ON_BN_CLICKED(IDC_CHK_USE_ENGRAVE, OnChkUseEngrave)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_UP, OnChkUseAoiUp)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_DN, OnChkUseAoiDn)
	ON_BN_CLICKED(IDC_CHK_USE_PUNCH, OnChkUsePunch)
	ON_BN_CLICKED(IDC_CHK_USE_PUNCH_ONLY, OnChkUsePunchOnly)
	ON_STN_CLICKED(IDC_STC_36, &CDlgInfo::OnStnClickedStc36)
	ON_STN_CLICKED(IDC_STC_17, &CDlgInfo::OnStnClickedStc17)
	ON_STN_CLICKED(IDC_STC_41, &CDlgInfo::OnStnClickedStc41)
	ON_STN_CLICKED(IDC_STC_43, &CDlgInfo::OnStnClickedStc43)
	ON_STN_CLICKED(IDC_STC_82, &CDlgInfo::OnStnClickedStc82)
	ON_STN_CLICKED(IDC_STC_83, &CDlgInfo::OnStnClickedStc83)
	ON_STN_CLICKED(IDC_STC_187, &CDlgInfo::OnStnClickedStc187)
	ON_BN_CLICKED(IDC_CHK_USE_AOI_MIDDLE, &CDlgInfo::OnBnClickedChkUseAoiMiddle)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInfo message handlers

void CDlgInfo::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(bShow)
	{
		AtDlgShow();
	}
	else
	{
		AtDlgHide();
	}	
}

void CDlgInfo::AtDlgShow()
{
	LoadImg();
	Disp();
	if (pView->m_pDlgMenu01)
		pView->m_pDlgMenu01->ChkAoiVsStatus();
}

void CDlgInfo::AtDlgHide()
{
	DelImg();
}

void CDlgInfo::LoadImg()
{
	if(m_bLoadImg)
		return;
	m_bLoadImg = TRUE;

	int i;
	for(i=0; i<MAX_INFO_BTN; i++)
	{
		myBtn[i].LoadBkImage(IMG_BTN_UP_DlgInfo, BTN_IMG_UP);
		myBtn[i].LoadBkImage(IMG_BTN_DN_DlgInfo, BTN_IMG_DN);
	}

	myBtnExit.LoadBkImage(MSG_BTN_BK_UP, BTN_IMG_UP);
	myBtnExit.LoadBkImage(MSG_BTN_BK_DN, BTN_IMG_DN);
 	myBtnExit.LoadImage(ICO_BTN_EXIT, BTN_IMG_UP, CSize(40,40), BTN_POS_LEFT);
 	myBtnExit.LoadImage(ICO_BTN_EXIT, BTN_IMG_DN, CSize(40,40), BTN_POS_LEFT);
}

void CDlgInfo::DelImg()
{
	if(!m_bLoadImg)
		return;
	m_bLoadImg = FALSE;

	int i;
	for(i=0; i<MAX_INFO_BTN; i++)
		myBtn[i].DelImgList();

	myBtnExit.DelImgList();
}

BOOL CDlgInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStatic();
	InitBtn();

	//GetDlgItem(IDC_STC_65)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_2_POINT_ALIGN)->ShowWindow(SW_HIDE);
	//GetDlgItem(IDC_CHK_4_POINT_ALIGN)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STC_40)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_41)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_42)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_43)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_186)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_83)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_185)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_82)->ShowWindow(SW_HIDE);

// 	GetDlgItem(IDC_STC_0014)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_CHK_001)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0015)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0016)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0017)->ShowWindow(SW_HIDE);
// 
// 	GetDlgItem(IDC_STC_0018)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0019)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0020)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0021)->ShowWindow(SW_HIDE);
// 
 	//GetDlgItem(IDC_STC_0043)->ShowWindow(SW_HIDE);
 	//GetDlgItem(IDC_CHK_008)->ShowWindow(SW_HIDE);
 	GetDlgItem(IDC_STC_0044)->ShowWindow(SW_HIDE);
 	GetDlgItem(IDC_CHK_009)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_STC_0046)->ShowWindow(SW_HIDE);
// 	GetDlgItem(IDC_CHK_011)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHK_USE_AOI_MIDDLE)->ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_CHK_SAMPLE_TEST)->ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_STC_64)->ShowWindow(SW_HIDE);

//	GetDlgItem(IDC_STC_181)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STC_181)->SetWindowText(_T("")); // 샘플검사 Shot수
	
	m_bTIM_DISP_STS = TRUE;
	SetTimer(TIM_DISP_STS, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInfo::InitBtn()
{
	myBtn[0].SubclassDlgItem(IDC_CHK_000, this);
	myBtn[0].SetHwnd(this->GetSafeHwnd(), IDC_CHK_000);
	myBtn[0].SetBtnType(BTN_TYPE_CHECK);

	myBtn[1].SubclassDlgItem(IDC_CHK_001, this);
	myBtn[1].SetHwnd(this->GetSafeHwnd(), IDC_CHK_001);
	myBtn[1].SetBtnType(BTN_TYPE_CHECK);

	myBtn[2].SubclassDlgItem(IDC_CHK_002, this);
	myBtn[2].SetHwnd(this->GetSafeHwnd(), IDC_CHK_002);	
	myBtn[2].SetBtnType(BTN_TYPE_CHECK);

	myBtn[3].SubclassDlgItem(IDC_CHK_003, this);
	myBtn[3].SetHwnd(this->GetSafeHwnd(), IDC_CHK_003);
	myBtn[3].SetBtnType(BTN_TYPE_CHECK);

	myBtn[4].SubclassDlgItem(IDC_CHK_004, this);
	myBtn[4].SetHwnd(this->GetSafeHwnd(), IDC_CHK_004);
	myBtn[4].SetBtnType(BTN_TYPE_CHECK);

	myBtn[5].SubclassDlgItem(IDC_CHK_005, this);
	myBtn[5].SetHwnd(this->GetSafeHwnd(), IDC_CHK_005);
	myBtn[5].SetBtnType(BTN_TYPE_CHECK);

	myBtn[6].SubclassDlgItem(IDC_CHK_006, this);
	myBtn[6].SetHwnd(this->GetSafeHwnd(), IDC_CHK_006);
	myBtn[6].SetBtnType(BTN_TYPE_CHECK);

	myBtn[7].SubclassDlgItem(IDC_CHK_007, this);
	myBtn[7].SetHwnd(this->GetSafeHwnd(), IDC_CHK_007);	
	myBtn[7].SetBtnType(BTN_TYPE_CHECK);

	myBtn[8].SubclassDlgItem(IDC_CHK_008, this);
	myBtn[8].SetHwnd(this->GetSafeHwnd(), IDC_CHK_008);
	myBtn[8].SetBtnType(BTN_TYPE_CHECK);

	myBtn[9].SubclassDlgItem(IDC_CHK_009, this);
	myBtn[9].SetHwnd(this->GetSafeHwnd(), IDC_CHK_009);
	myBtn[9].SetBtnType(BTN_TYPE_CHECK);

	myBtn[10].SubclassDlgItem(IDC_CHK_010, this);
	myBtn[10].SetHwnd(this->GetSafeHwnd(), IDC_CHK_010);
	myBtn[10].SetBtnType(BTN_TYPE_CHECK);

	myBtn[11].SubclassDlgItem(IDC_CHK_011, this);
	myBtn[11].SetHwnd(this->GetSafeHwnd(), IDC_CHK_011);
	myBtn[11].SetBtnType(BTN_TYPE_CHECK);

	myBtn[12].SubclassDlgItem(IDC_CHK_USE_AOI_DUAL, this);
	myBtn[12].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_DUAL);
	myBtn[12].SetBtnType(BTN_TYPE_CHECK);

	myBtn[13].SubclassDlgItem(IDC_CHK_USE_AOI_SINGLE, this);
	myBtn[13].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_SINGLE);
	myBtn[13].SetBtnType(BTN_TYPE_CHECK);

	myBtn[14].SubclassDlgItem(IDC_CHK_SAMPLE_TEST, this);
	myBtn[14].SetHwnd(this->GetSafeHwnd(), IDC_CHK_SAMPLE_TEST);
	myBtn[14].SetBtnType(BTN_TYPE_CHECK);

	myBtn[15].SubclassDlgItem(IDC_CHK_RECOILER_CCW, this);
	myBtn[15].SetHwnd(this->GetSafeHwnd(), IDC_CHK_RECOILER_CCW);
	myBtn[15].SetBtnType(BTN_TYPE_CHECK);

	myBtn[16].SubclassDlgItem(IDC_CHK_UNCOILER_CCW, this);
	myBtn[16].SetHwnd(this->GetSafeHwnd(), IDC_CHK_UNCOILER_CCW);
	myBtn[16].SetBtnType(BTN_TYPE_CHECK);

	myBtn[17].SubclassDlgItem(IDC_CHK_2_POINT_ALIGN, this);
	myBtn[17].SetHwnd(this->GetSafeHwnd(), IDC_CHK_2_POINT_ALIGN);
	myBtn[17].SetBtnType(BTN_TYPE_CHECK);

	myBtn[18].SubclassDlgItem(IDC_CHK_4_POINT_ALIGN, this);
	myBtn[18].SetHwnd(this->GetSafeHwnd(), IDC_CHK_4_POINT_ALIGN);
	myBtn[18].SetBtnType(BTN_TYPE_CHECK);

	myBtn[19].SubclassDlgItem(IDC_CHK_85, this); //하면AOI 클린롤러 
	myBtn[19].SetHwnd(this->GetSafeHwnd(), IDC_CHK_85);
	myBtn[19].SetBtnType(BTN_TYPE_CHECK);

	myBtn[20].SubclassDlgItem(IDC_CHK_86, this); //상면AOI 클린롤러 
	myBtn[20].SetHwnd(this->GetSafeHwnd(), IDC_CHK_86);
	myBtn[20].SetBtnType(BTN_TYPE_CHECK);

	myBtn[21].SubclassDlgItem(IDC_CHK_1187, this); //AOI초음파세정기
	myBtn[21].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1187);
	myBtn[21].SetBtnType(BTN_TYPE_CHECK);

	myBtn[22].SubclassDlgItem(IDC_CHK_1188, this); //각인부초음파세정기
	myBtn[22].SetHwnd(this->GetSafeHwnd(), IDC_CHK_1188);
	myBtn[22].SetBtnType(BTN_TYPE_CHECK);

	myBtn[23].SubclassDlgItem(IDC_CHK_USE_AOI_INNER, this); //WORK_MODE : MODE_INNER
	myBtn[23].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_INNER);
	myBtn[23].SetBtnType(BTN_TYPE_CHECK);

	myBtn[24].SubclassDlgItem(IDC_CHK_USE_AOI_OUTER, this); //WORK_MODE : MODE_OUTER
	myBtn[24].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_OUTER);
	myBtn[24].SetBtnType(BTN_TYPE_CHECK);

	myBtn[25].SubclassDlgItem(IDC_CHK_USE_AOI_MIDDLE, this); //WORK_MODE : MODE_MIDDLE
	myBtn[25].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_MIDDLE);
	myBtn[25].SetBtnType(BTN_TYPE_CHECK);

	myBtn[26].SubclassDlgItem(IDC_CHK_19, this);
	myBtn[26].SetHwnd(this->GetSafeHwnd(), IDC_CHK_19);
	myBtn[26].SetBtnType(BTN_TYPE_CHECK);

	myBtn[27].SubclassDlgItem(IDC_CHK_23, this);
	myBtn[27].SetHwnd(this->GetSafeHwnd(), IDC_CHK_23);
	myBtn[27].SetBtnType(BTN_TYPE_CHECK);

	myBtn[28].SubclassDlgItem(IDC_CHK_26, this);
	myBtn[28].SetHwnd(this->GetSafeHwnd(), IDC_CHK_26);
	myBtn[28].SetBtnType(BTN_TYPE_CHECK);


	myBtn[29].SubclassDlgItem(IDC_CHK_USE_ENGRAVE, this);
	myBtn[29].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_ENGRAVE);
	myBtn[29].SetBtnType(BTN_TYPE_CHECK);
	myBtn[29].SetTextColor(RGB_BLUE);
	myBtn[29].SetFont(_T("굴림체"), 14, TRUE);

	myBtn[30].SubclassDlgItem(IDC_CHK_USE_AOI_UP, this);
	myBtn[30].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_UP);
	myBtn[30].SetBtnType(BTN_TYPE_CHECK);
	myBtn[30].SetTextColor(RGB_BLUE);
	myBtn[30].SetFont(_T("굴림체"), 14, TRUE);

	myBtn[31].SubclassDlgItem(IDC_CHK_USE_AOI_DN, this);
	myBtn[31].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_AOI_DN);
	myBtn[31].SetBtnType(BTN_TYPE_CHECK);
	myBtn[31].SetTextColor(RGB_BLUE);
	myBtn[31].SetFont(_T("굴림체"), 14, TRUE);

	myBtn[32].SubclassDlgItem(IDC_CHK_USE_PUNCH, this);
	myBtn[32].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_PUNCH);
	myBtn[32].SetBtnType(BTN_TYPE_CHECK);
	myBtn[32].SetTextColor(RGB_BLUE);
	myBtn[32].SetFont(_T("굴림체"), 14, TRUE);

	myBtn[33].SubclassDlgItem(IDC_CHK_USE_PUNCH_ONLY, this);
	myBtn[33].SetHwnd(this->GetSafeHwnd(), IDC_CHK_USE_PUNCH_ONLY);
	myBtn[33].SetBtnType(BTN_TYPE_CHECK);
	myBtn[33].SetTextColor(RGB_BLUE);
	myBtn[33].SetFont(_T("굴림체"), 14, TRUE);
	

	int i;
	for(i=0; i<MAX_INFO_BTN; i++)
	{
		if(0 == i || 12 == i || 13 == i || 14 == i || 23 == i || 24 == i || 25 == i)
		{
			myBtn[i].SetFont(_T("굴림체"),16,TRUE);
			myBtn[i].SetTextColor(RGB_BLACK);
		}

		if (15 == i || 16 == i)
		{
			myBtn[i].SetFont(_T("돋움체"),22,TRUE);
		}

		if (0 != i && 12 != i && 13 != i && 14 != i && 15 != i && 16 != i  && 17 != i && 18 != i && 23 != i && 24 != i && 25 != i
			&& 29 != i && 30 != i && 31 != i && 32 != i && 33 != i)
		{
			myBtn[i].SetFont(_T("굴림체"),16,TRUE);
			myBtn[i].SetTextColor(RGB_BLACK);
			myBtn[i].SetText(_T("사용\rOff"), BTN_UP);
			myBtn[i].SetText(_T("사용\rON"), BTN_DN);
		}
	}

	myBtnExit.SubclassDlgItem(IDC_BTN_EXIT, this);
	myBtnExit.SetHwnd(this->GetSafeHwnd(), IDC_BTN_EXIT);
	myBtnExit.SetFont(_T("굴림체"),24,TRUE);
	myBtnExit.SetTextColor(RGB_BLACK);
}

void CDlgInfo::InitStatic()
{
	InitStcTitle();
	InitStcData();
}

void CDlgInfo::InitStcTitle()
{
	myStcTitle[0].SubclassDlgItem(IDC_STC_0000, this);
	myStcTitle[1].SubclassDlgItem(IDC_STC_0001, this);
	myStcTitle[2].SubclassDlgItem(IDC_STC_0002, this);
	myStcTitle[3].SubclassDlgItem(IDC_STC_0004, this);
	myStcTitle[4].SubclassDlgItem(IDC_STC_0005, this);
	myStcTitle[5].SubclassDlgItem(IDC_STC_0006, this);
	myStcTitle[6].SubclassDlgItem(IDC_STC_0007, this);
	myStcTitle[7].SubclassDlgItem(IDC_STC_0009, this);
	myStcTitle[8].SubclassDlgItem(IDC_STC_0010, this);
	myStcTitle[9].SubclassDlgItem(IDC_STC_0011, this);
	myStcTitle[10].SubclassDlgItem(IDC_STC_0013, this);
	myStcTitle[11].SubclassDlgItem(IDC_STC_0014, this);
	myStcTitle[12].SubclassDlgItem(IDC_STC_0015, this);
	myStcTitle[13].SubclassDlgItem(IDC_STC_0017, this);
	myStcTitle[14].SubclassDlgItem(IDC_STC_0018, this);
	myStcTitle[15].SubclassDlgItem(IDC_STC_0019, this);
	myStcTitle[16].SubclassDlgItem(IDC_STC_0021, this);
	myStcTitle[17].SubclassDlgItem(IDC_STC_0022, this);
	myStcTitle[18].SubclassDlgItem(IDC_STC_0023, this);
	myStcTitle[19].SubclassDlgItem(IDC_STC_0025, this);
	myStcTitle[20].SubclassDlgItem(IDC_STC_0026, this);
	myStcTitle[21].SubclassDlgItem(IDC_STC_0027, this);
	myStcTitle[22].SubclassDlgItem(IDC_STC_0028, this);
	myStcTitle[23].SubclassDlgItem(IDC_STC_0029, this);
	myStcTitle[24].SubclassDlgItem(IDC_STC_0031, this);
	myStcTitle[25].SubclassDlgItem(IDC_STC_0032, this);
	myStcTitle[26].SubclassDlgItem(IDC_STC_0033, this);
	myStcTitle[27].SubclassDlgItem(IDC_STC_0035, this);
	myStcTitle[28].SubclassDlgItem(IDC_STC_0036, this);
	myStcTitle[29].SubclassDlgItem(IDC_STC_0038, this);
	myStcTitle[30].SubclassDlgItem(IDC_STC_0039, this);
	myStcTitle[31].SubclassDlgItem(IDC_STC_0040, this);
	myStcTitle[32].SubclassDlgItem(IDC_STC_0041, this);
	myStcTitle[33].SubclassDlgItem(IDC_STC_0042, this);
	myStcTitle[34].SubclassDlgItem(IDC_STC_0043, this); // 380mm
	myStcTitle[35].SubclassDlgItem(IDC_STC_0044, this);
	myStcTitle[36].SubclassDlgItem(IDC_STC_0045, this);
	myStcTitle[37].SubclassDlgItem(IDC_STC_0046, this);
	myStcTitle[38].SubclassDlgItem(IDC_STC_59, this);
	myStcTitle[39].SubclassDlgItem(IDC_STC_173, this);
	myStcTitle[40].SubclassDlgItem(IDC_STC_175, this);
	myStcTitle[41].SubclassDlgItem(IDC_STC_176, this);
	myStcTitle[42].SubclassDlgItem(IDC_STC_177, this);
	myStcTitle[43].SubclassDlgItem(IDC_STC_38, this);
	myStcTitle[44].SubclassDlgItem(IDC_STC_62, this);
	myStcTitle[45].SubclassDlgItem(IDC_STC_60, this);
	myStcTitle[46].SubclassDlgItem(IDC_STC_63, this);
	myStcTitle[47].SubclassDlgItem(IDC_STC_64, this);
	myStcTitle[48].SubclassDlgItem(IDC_STC_65, this);
	myStcTitle[49].SubclassDlgItem(IDC_STC_1145, this); //하면AOI 클린롤러
	myStcTitle[50].SubclassDlgItem(IDC_STC_1146, this); //상면AOI 클린롤러
	myStcTitle[51].SubclassDlgItem(IDC_STC_1147, this); //AOI초음파세정기
	myStcTitle[52].SubclassDlgItem(IDC_STC_1148, this); //각인부초음파세정기

	myStcTitle[53].SubclassDlgItem(IDC_STC_30, this); //초음파세정기 동작
	myStcTitle[54].SubclassDlgItem(IDC_STC_31, this); //검사시작 후
	myStcTitle[55].SubclassDlgItem(IDC_STC_33, this); //초

	myStcTitle[56].SubclassDlgItem(IDC_STC_73, this); //고객출하수율
	myStcTitle[57].SubclassDlgItem(IDC_STC_182, this); //양품율
	myStcTitle[58].SubclassDlgItem(IDC_STC_184, this); //%

	myStcTitle[59].SubclassDlgItem(IDC_STC_34, this); //불량 확인
	myStcTitle[60].SubclassDlgItem(IDC_STC_35, this); //Period
	myStcTitle[61].SubclassDlgItem(IDC_STC_37, this); //Shot

	myStcTitle[62].SubclassDlgItem(IDC_STC_16, this); //ITS코드 라벨
	myStcTitle[63].SubclassDlgItem(IDC_STC_40, this); //Shot수 현재값
	myStcTitle[64].SubclassDlgItem(IDC_STC_42, this); //Shot수 설정값

	myStcTitle[65].SubclassDlgItem(IDC_STC_185, this); //검사부 AOI 상하면 재작업 알람 시간
	myStcTitle[66].SubclassDlgItem(IDC_STC_186, this); //마킹부 재작업 알람 시간

	myStcTitle[67].SubclassDlgItem(IDC_STC_44, this); // 340mm
	myStcTitle[68].SubclassDlgItem(IDC_STC_47, this); // 346mm

	myStcTitle[69].SubclassDlgItem(IDC_STC_103, this);
	myStcTitle[70].SubclassDlgItem(IDC_STC_87, this);
	myStcTitle[71].SubclassDlgItem(IDC_STC_19, this);
	myStcTitle[72].SubclassDlgItem(IDC_STC_22, this);
	myStcTitle[73].SubclassDlgItem(IDC_STC_188, this);


	for(int i=0; i<MAX_INFO_STC; i++)
	{
		myStcTitle[i].SetFontName(_T("Arial"));
// 		if(47 == i)
// 			myStcTitle[i].SetFontSize(12);
// 		else
			myStcTitle[i].SetFontSize(14);
		
		switch(i)
		{
		case 0:
		case 3:
		case 20:
		case 62:
		case 63:
		case 64:
		case 65:
		case 66:
		case 69:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTDKORANGE);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		case 1:
		case 4:
		case 21:
		case 47:
		case 70:
			myStcTitle[i].SetTextColor(RGB_WHITE);
			myStcTitle[i].SetBkColor(RGB_DARKGREEN);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		case 10:
		case 43:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_WHITE);
			myStcTitle[i].SetFontBold(FALSE);
			break;
		case 7:
		case 13:
		case 16:
		case 19:
		case 24:
		case 27:
		case 40:
		case 55:
		case 58:
		case 61:
		case 73:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_WHITE);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		default:
			myStcTitle[i].SetTextColor(RGB_NAVY);
			myStcTitle[i].SetBkColor(RGB_LTYELLOW);
			myStcTitle[i].SetFontBold(TRUE);
			break;
		}
	}
}

void CDlgInfo::InitStcData()
{
	myStcData[0].SubclassDlgItem(IDC_STC_0003, this);
	myStcData[1].SubclassDlgItem(IDC_STC_0008, this);
	myStcData[2].SubclassDlgItem(IDC_STC_0012, this);
	myStcData[3].SubclassDlgItem(IDC_STC_0016, this);
	myStcData[4].SubclassDlgItem(IDC_STC_0020, this);
	myStcData[5].SubclassDlgItem(IDC_STC_0024, this);
	myStcData[6].SubclassDlgItem(IDC_STC_0030, this);
	myStcData[7].SubclassDlgItem(IDC_STC_0034, this);
	myStcData[8].SubclassDlgItem(IDC_STC_0037, this);
	myStcData[9].SubclassDlgItem(IDC_STC_174, this);
	myStcData[10].SubclassDlgItem(IDC_STC_178, this);
	myStcData[11].SubclassDlgItem(IDC_STC_61, this);
	myStcData[12].SubclassDlgItem(IDC_STC_181, this);

	myStcData[13].SubclassDlgItem(IDC_STC_32, this); // 초음파세정기 동작 검사시작 후 시작시간 [초]
	myStcData[14].SubclassDlgItem(IDC_STC_183, this); // 고객출하수율

	myStcData[15].SubclassDlgItem(IDC_STC_36, this); // 불량 확인 Period [Shot]

	myStcData[16].SubclassDlgItem(IDC_STC_17, this); // ITS코드
	myStcData[17].SubclassDlgItem(IDC_STC_41, this); // Shot수 현재값
	myStcData[18].SubclassDlgItem(IDC_STC_43, this); // Shot수 설정값

	myStcData[19].SubclassDlgItem(IDC_STC_82, this); // 검사부 AOI 상하면 재작업 알람 시간
	myStcData[20].SubclassDlgItem(IDC_STC_83, this); // 마킹부 재작업 알람 시간

	myStcData[21].SubclassDlgItem(IDC_STC_187, this); // 마킹부 마킹여부 확인 미마킹일치율

	for(int i=0; i<MAX_INFO_STC_DATA; i++)
	{
		myStcData[i].SetFontName(_T("Arial"));
// 		if(12==i)
// 			myStcData[i].SetFontSize(12);
// 		else
			myStcData[i].SetFontSize(20);
		myStcData[i].SetFontBold(TRUE);
		myStcData[i].SetTextColor(RGB_BLACK);
		myStcData[i].SetBkColor(RGB_WHITE);
	}
}

BOOL CDlgInfo::ShowKeypad(int nCtlID, CPoint ptSt, int nDir)
{
	BOOL bAdj = TRUE;
	CString strData, strPrev;
	GetDlgItemText(nCtlID, strData);
	strPrev = strData;

	CString strMin, strMax;
	CRect rect(0,0,0,0);
	CDlgKeyNum *pDlg = new CDlgKeyNum(&strData, &rect, ptSt, nDir);
	pDlg->m_strCurr = strData;
	pDlg->m_strMin = strMin = pDoc->GetMin(IDD_DLG_INFO, nCtlID);
	pDlg->m_strMax = strMax = pDoc->GetMax(IDD_DLG_INFO, nCtlID);
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
	
// 	ScreenToClient(&rect);
// 	InvalidateRect(rect, FALSE);
	return bAdj;
}

void CDlgInfo::Disp()
{
	CString str, sMsg;

	myStcData[0].SetText(pDoc->WorkingInfo.LastJob.sSelUserName);
	myStcData[1].SetText(pDoc->WorkingInfo.LastJob.sReelTotLen);
	myStcData[2].SetText(pDoc->WorkingInfo.LastJob.sOnePnlLen);	// pDoc->WorkingInfo.Motion.sMkFdDist
	myStcData[3].SetText(pDoc->WorkingInfo.LastJob.sLotSepLen);
	myStcData[4].SetText(pDoc->WorkingInfo.LastJob.sLotCutPosLen);
	myStcData[5].SetText(pDoc->WorkingInfo.LastJob.sTempPauseLen);
	myStcData[6].SetText(pDoc->WorkingInfo.LastJob.sLmtTotYld);
	myStcData[7].SetText(pDoc->WorkingInfo.LastJob.sLmtPatlYld);
	myStcData[8].SetText(pDoc->WorkingInfo.LastJob.sNumContFixDef);
	myStcData[9].SetText(pDoc->WorkingInfo.LastJob.sStripOutRatio);
	myStcData[10].SetText(pDoc->WorkingInfo.LastJob.sPartialSpd);
 	myStcData[11].SetText(pDoc->WorkingInfo.LastJob.sNumRangeFixDef);
 	myStcData[12].SetText(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);
	myStcData[13].SetText(pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim);
	myStcData[14].SetText(pDoc->WorkingInfo.LastJob.sCustomNeedRatio);
	str.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nVerifyPeriod);
	myStcData[15].SetText(str);
	myStcData[16].SetText(pDoc->WorkingInfo.LastJob.sEngItsCode);		// IDC_STC_17	ITS코드
	myStcData[17].SetText(pDoc->WorkingInfo.LastJob.sCurrentShotNum);	// IDC_STC_41	Shot수 현재값
	myStcData[18].SetText(pDoc->WorkingInfo.LastJob.sSettingShotNum);	// IDC_STC_43	Shot수 설정값
	str.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nAlarmTimeAoi);
	myStcData[19].SetText(str);											// IDC_STC_82	검사부 AOI 상하면 재작업 알람 시간
	str.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nAlarmTimePunch);
	myStcData[20].SetText(str);											// IDC_STC_83	마킹부 재작업 알람 시간

	if(pDoc->WorkingInfo.LastJob.bLotSep)
		myBtn[1].SetCheck(TRUE);
	else
		myBtn[1].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bTempPause)
		myBtn[2].SetCheck(TRUE);
	else
		myBtn[2].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bContFixDef)
		myBtn[3].SetCheck(TRUE);
	else
		myBtn[3].SetCheck(FALSE);

	if (pDoc->WorkingInfo.LastJob.bUseJudgeMk)
		myBtn[28].SetCheck(TRUE);
	else
		myBtn[28].SetCheck(FALSE);

	str.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nJudgeMkRatio);
	myStcData[21].SetText(str);

	pDoc->WorkingInfo.LastJob.bRclDrSen = pView->MpeRead(pView->Plc.DlgInfo.LampDoorSensorRecoiler) ? TRUE : FALSE;
	if(pDoc->WorkingInfo.LastJob.bRclDrSen)
		myBtn[4].SetCheck(TRUE);
	else
		myBtn[4].SetCheck(FALSE);

	pDoc->WorkingInfo.LastJob.bAoiUpDrSen = pView->MpeRead(pView->Plc.DlgInfo.LampDoorSensorAoiUp) ? TRUE : FALSE;
	if(pDoc->WorkingInfo.LastJob.bAoiUpDrSen)
		myBtn[5].SetCheck(TRUE);
	else
		myBtn[5].SetCheck(FALSE);

	pDoc->WorkingInfo.LastJob.bMkDrSen = pView->MpeRead(pView->Plc.DlgInfo.LampDoorSensorPunch) ? TRUE : FALSE;
	if(pDoc->WorkingInfo.LastJob.bMkDrSen)
		myBtn[6].SetCheck(TRUE);
	else
		myBtn[6].SetCheck(FALSE);

	pDoc->WorkingInfo.LastJob.bUclDrSen = pView->MpeRead(pView->Plc.DlgInfo.LampDoorSensorUncoiler) ? TRUE : FALSE;
	if(pDoc->WorkingInfo.LastJob.bUclDrSen)
		myBtn[7].SetCheck(TRUE);
	else
		myBtn[7].SetCheck(FALSE);

	pDoc->WorkingInfo.LastJob.bUse380mm = pView->MpeRead(pView->Plc.DlgMenu03.LampLaser380mm) ? TRUE : FALSE;
	if (pDoc->WorkingInfo.LastJob.bUse380mm)
		myBtn[8].SetCheck(TRUE);
	else
		myBtn[8].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bStopFixDef)
		myBtn[9].SetCheck(TRUE);
	else
		myBtn[9].SetCheck(FALSE);

	if(pDoc->WorkingInfo.LastJob.bMkSftySen)
		myBtn[10].SetCheck(TRUE);
	else
		myBtn[10].SetCheck(FALSE);

	pDoc->WorkingInfo.LastJob.bAoiDnDrSen = pView->MpeRead(pView->Plc.DlgInfo.LampDoorSensorAoiDn) ? TRUE : FALSE;
	if (pDoc->WorkingInfo.LastJob.bAoiDnDrSen)
		myBtn[11].SetCheck(TRUE);
	else
		myBtn[11].SetCheck(FALSE);
	
	pDoc->WorkingInfo.LastJob.bTwoMetal = pView->MpeRead(pView->Plc.DlgInfo.LampTwoMetal) ? TRUE : FALSE;
	if(pDoc->WorkingInfo.LastJob.bDualTest)
	{
		myBtn[12].SetCheck(TRUE);
		myBtn[13].SetCheck(FALSE);
	}
	else
	{
		myBtn[12].SetCheck(FALSE);
		myBtn[13].SetCheck(TRUE);
	}

	pDoc->WorkingInfo.LastJob.bSampleTest = pView->MpeRead(pView->Plc.DlgInfo.LampSampleTest) ? TRUE : FALSE;		// Sample 검사 On
	if(pDoc->WorkingInfo.LastJob.bSampleTest)
		myBtn[14].SetCheck(TRUE);
	else
		myBtn[14].SetCheck(FALSE);

	pDoc->WorkingInfo.LastJob.bFeedCcwRecoiler = pView->MpeRead(pView->Plc.DlgInfo.LampFeedCcwRecoiler) ? TRUE : FALSE;
	if(pDoc->WorkingInfo.LastJob.bFeedCcwRecoiler)
	{
		myBtn[15].SetCheck(TRUE);
		myBtn[15].SetWindowText(_T("Recoiler\r역방향"));	
		myBtn[15].SetTextColor(RGB_DARKRED);
	}
	else
	{
		myBtn[15].SetCheck(FALSE);
		myBtn[15].SetWindowText(_T("Recoiler\r정방향"));
		myBtn[15].SetTextColor(RGB_BLUE);
	}

	pDoc->WorkingInfo.LastJob.bFeedCcwUncoiler = pView->MpeRead(pView->Plc.DlgInfo.LampFeedCcwUncoiler) ? TRUE : FALSE;
	if(pDoc->WorkingInfo.LastJob.bFeedCcwUncoiler)
	{
		myBtn[16].SetCheck(TRUE);
		myBtn[16].SetWindowText(_T("Uncoiler\r역방향"));
		myBtn[16].SetTextColor(RGB_DARKRED);
	}
	else
	{
		myBtn[16].SetCheck(FALSE);
		myBtn[16].SetWindowText(_T("Uncoiler\r정방향"));
		myBtn[16].SetTextColor(RGB_BLUE);
	}

	pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = pView->MpeRead(pView->Plc.DlgInfo.LampCleanRollerAoiDn) ? TRUE : FALSE;
	if (pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler)
		myBtn[19].SetCheck(TRUE);
	else
		myBtn[19].SetCheck(FALSE);

	pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = pView->MpeRead(pView->Plc.DlgInfo.LampCleanRollerAoiUp) ? TRUE : FALSE;
	if (pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler)
		myBtn[20].SetCheck(TRUE);
	else
		myBtn[20].SetCheck(FALSE);

	pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic = pView->MpeRead(pView->Plc.DlgInfo.LampUltraSonicAoi) ? TRUE : FALSE;
	if (pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic) //AOI초음파세정기
		myBtn[21].SetCheck(TRUE);
	else
		myBtn[21].SetCheck(FALSE);

	pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic = pView->MpeRead(pView->Plc.DlgInfo.LampUltraSonicEngrave) ? TRUE : FALSE;
	if (pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic) //각인부초음파세정기
		myBtn[22].SetCheck(TRUE);
	else
		myBtn[22].SetCheck(FALSE);

	pDoc->WorkingInfo.LastJob.nTestMode = GetTestModeFromPlc();
	switch (pDoc->WorkingInfo.LastJob.nTestMode) // GetTestMode()
	{
	case MODE_NONE:		// 0 
		myBtn[23].SetCheck(FALSE); // IDC_CHK_USE_AOI_INNER
		myBtn[24].SetCheck(FALSE); // IDC_CHK_USE_AOI_OUTER
		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->EnableItsMode(FALSE);
		break;
	case MODE_INNER:	// 1 
		myBtn[23].SetCheck(TRUE); // IDC_CHK_USE_AOI_INNER
		myBtn[24].SetCheck(FALSE); // IDC_CHK_USE_AOI_OUTER
		myBtn[25].SetCheck(FALSE); // IDC_CHK_USE_AOI_MIDDLE
		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->EnableItsMode(FALSE);
		break;
	case MODE_OUTER:	// 2 
		myBtn[23].SetCheck(FALSE); // IDC_CHK_USE_AOI_INNER
		myBtn[24].SetCheck(TRUE); // IDC_CHK_USE_AOI_OUTER
		myBtn[25].SetCheck(FALSE); // IDC_CHK_USE_AOI_MIDDLE
		if(pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->EnableItsMode();
		break;
	case MODE_MIDDLE:	// 3 
		myBtn[23].SetCheck(FALSE); // IDC_CHK_USE_AOI_INNER
		myBtn[24].SetCheck(TRUE); // IDC_CHK_USE_AOI_OUTER
		myBtn[25].SetCheck(TRUE); // IDC_CHK_USE_AOI_MIDDLE
		if (pView->m_pDlgMenu01)
			pView->m_pDlgMenu01->EnableItsMode(FALSE);
		break;
	}

	//if (pDoc && pDoc->m_pMpeData)
	//{
	//	str.Format(_T("%d"), pDoc->m_pMpeData[2][7]); // Shot수 현재값
	//	myStcData[17].SetWindowText(str);

	//	str.Format(_T("%d"), pDoc->m_pMpeData[7][6]); //Shot수 설정값
	//	myStcData[18].SetWindowText(str);

	//	str.Format(_T("%d"), pDoc->m_pMpeData[0][10]); //검사부 AOI 상[10]하[11]면 재작업 알람 시간
	//	myStcData[19].SetWindowText(str);

	//	str.Format(_T("%d"), pDoc->m_pMpeData[0][12]); //마킹부[12] 재작업 알람 시간
	//	myStcData[20].SetWindowText(str);
	//}

	if (pView->IsAuto())
	{
		GetDlgItem(IDC_CHK_RECOILER_CCW)->EnableWindow(FALSE); // myBtn[15] IDC_CHK_RECOILER_CCW - Recoiler\r정방향 CW : FALSE
		GetDlgItem(IDC_CHK_UNCOILER_CCW)->EnableWindow(FALSE); // myBtn[16] IDC_CHK_UNCOILER_CCW - Uncoiler\r정방향 CW : FALSE
	}
	else
	{
		GetDlgItem(IDC_CHK_RECOILER_CCW)->EnableWindow(TRUE); // myBtn[15] IDC_CHK_RECOILER_CCW - Recoiler\r정방향 CW : FALSE
		GetDlgItem(IDC_CHK_UNCOILER_CCW)->EnableWindow(TRUE); // myBtn[16] IDC_CHK_TWO_METAL - Uncoiler\r정방향 CW : FALSE
	}

	pDoc->WorkingInfo.LastJob.bUse340mm = pView->MpeRead(pView->Plc.DlgMenu03.LampLaser340mm) ? TRUE : FALSE;
	if (pDoc->WorkingInfo.LastJob.bUse340mm)
		myBtn[26].SetCheck(TRUE);
	else
		myBtn[26].SetCheck(FALSE);


	pDoc->WorkingInfo.LastJob.bUse346mm = pView->MpeRead(pView->Plc.DlgMenu03.LampLaser346mm) ? TRUE : FALSE;
	if (pDoc->WorkingInfo.LastJob.bUse346mm)
		myBtn[27].SetCheck(TRUE);
	else
		myBtn[27].SetCheck(FALSE);


	int nAlignMethodeOnCamMst = pDoc->m_Master[0].GetAlignMethode();
	int nAlignMethode = pDoc->WorkingInfo.LastJob.nAlignMethode;
	BOOL b2PointAlign = nAlignMethodeOnCamMst & TWO_POINT;
	BOOL b4PointAlign = nAlignMethodeOnCamMst & FOUR_POINT;
#ifdef TEST_MODE
	b2PointAlign = TRUE;
	b4PointAlign = FALSE;
#endif

	if (b4PointAlign && b2PointAlign)
	{
		if (pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT)
		{
			myBtn[17].SetCheck(FALSE);
			myBtn[18].SetCheck(TRUE);
		}
		else if (pDoc->WorkingInfo.LastJob.nAlignMethode == TWO_POINT)
		{
			myBtn[17].SetCheck(TRUE);
			myBtn[18].SetCheck(FALSE);
		}
		else
		{
			pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT;
			myBtn[17].SetCheck(FALSE);
			myBtn[18].SetCheck(TRUE);
		}
	}
	else if (b4PointAlign)
	{
		pDoc->WorkingInfo.LastJob.nAlignMethode == FOUR_POINT;
		myBtn[17].SetCheck(FALSE);
		myBtn[18].SetCheck(TRUE);
	}
	else if (b2PointAlign)
	{
		pDoc->WorkingInfo.LastJob.nAlignMethode == TWO_POINT;
		myBtn[17].SetCheck(TRUE);
		myBtn[18].SetCheck(FALSE);
	}
	else
	{
		myBtn[17].SetCheck(FALSE);
		myBtn[18].SetCheck(FALSE);
		sMsg.Format(_T("캠마스터에 %s 모델의 Align 설정이 없습니다."), pDoc->WorkingInfo.LastJob.sModel);
		pView->MsgBox(sMsg);
	}

	DispDevicePartial();
}

int CDlgInfo::GetTestModeFromPlc()
{
	// MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2 .
	if (pView->MpeRead(pView->Plc.DlgInfo.LampModeInner))
		pDoc->WorkingInfo.LastJob.nTestMode = MODE_INNER;	
	else if (pView->MpeRead(pView->Plc.DlgInfo.LampModeOutter))
		pDoc->WorkingInfo.LastJob.nTestMode = MODE_OUTER;
	//else
	//	pDoc->WorkingInfo.LastJob.nTestMode = MODE_NONE;

	return pDoc->WorkingInfo.LastJob.nTestMode;
}

void CDlgInfo::OnStc0008()
{
	// TODO: Add your control notification handler code here
	myStcData[1].SetBkColor(RGB_RED);
	myStcData[1].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0008)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0008, pt, TO_BOTTOM | TO_RIGHT);

	myStcData[1].SetBkColor(RGB_WHITE);
	myStcData[1].RedrawWindow();

	CString sVal;
	GetDlgItem(IDC_STC_0008)->GetWindowText(sVal);

	pDoc->SetTotalReelDist(_tstof(sVal));
}

void CDlgInfo::OnStc0012() 
{
	// TODO: Add your control notification handler code here
	myStcData[2].SetBkColor(RGB_RED);
	myStcData[2].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0012)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0012, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[2].SetBkColor(RGB_WHITE);
	myStcData[2].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0012)->GetWindowText(sVal);
	pDoc->SetOnePnlLen(_tstof(sVal));

	pView->SetLotLastShot();
}

void CDlgInfo::OnStc0016() 
{
	// TODO: Add your control notification handler code here
	myStcData[3].SetBkColor(RGB_RED);
	myStcData[3].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0016)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0016, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[3].SetBkColor(RGB_WHITE);
	myStcData[3].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0016)->GetWindowText(sVal);
	pDoc->SetSeparateDist(_tstof(sVal));
	pView->SetLotLastShot();
}

void CDlgInfo::OnStc0020() 
{
	// TODO: Add your control notification handler code here
	myStcData[4].SetBkColor(RGB_RED);
	myStcData[4].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0020)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0020, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[4].SetBkColor(RGB_WHITE);
	myStcData[4].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0020)->GetWindowText(sVal);
	pDoc->SetCuttingDist(_tstof(sVal));

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetLotCutLen();	//_ItemInx::_LotCutLen
#endif
}

void CDlgInfo::OnStc0024() 
{
	// TODO: Add your control notification handler code here
	myStcData[5].SetBkColor(RGB_RED);
	myStcData[5].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0024)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0024, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[5].SetBkColor(RGB_WHITE);
	myStcData[5].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0024)->GetWindowText(sVal);
 	pDoc->SetStopDist(_tstof(sVal));	

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetTempStopLen();	//_ItemInx::_LotCutPosLen
#endif
}

void CDlgInfo::OnStc0030() 
{
	// TODO: Add your control notification handler code here
	myStcData[6].SetBkColor(RGB_RED);
	myStcData[6].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0030)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0030, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[6].SetBkColor(RGB_WHITE);
	myStcData[6].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0030)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sLmtTotYld = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Limit Total Yield"), sVal, PATH_WORKING_INFO);	

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetLmtTotYld();	//_ItemInx::_LmtTotYld
#endif
}

void CDlgInfo::OnStc0034() 
{
	// TODO: Add your control notification handler code here
	myStcData[7].SetBkColor(RGB_RED);
	myStcData[7].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0034)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0034, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[7].SetBkColor(RGB_WHITE);
	myStcData[7].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0034)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sLmtPatlYld = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Limit Partial Yield"), sVal, PATH_WORKING_INFO);	

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetLmtPatlYld();	//_ItemInx::_LmtPatlYld
#endif
}

void CDlgInfo::OnStc0037() 
{
	// TODO: Add your control notification handler code here
	myStcData[8].SetBkColor(RGB_RED);
	myStcData[8].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_0037)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_0037, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[8].SetBkColor(RGB_WHITE);
	myStcData[8].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_0037)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sNumContFixDef = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Number of Continuous Fix Defect"), sVal, PATH_WORKING_INFO);	

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetNumContFixDef();	//_ItemInx::_NumContFixDef
#endif
}

void CDlgInfo::OnStc32()
{
	// TODO: Add your control notification handler code here
	myStcData[13].SetBkColor(RGB_RED);
	myStcData[13].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_32)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_32, pt, TO_BOTTOM | TO_RIGHT);

	myStcData[13].SetBkColor(RGB_WHITE);
	myStcData[13].RedrawWindow();

	CString sVal;
	GetDlgItem(IDC_STC_32)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sUltraSonicCleannerStTim = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Ultra Sonic Cleanner Start Time"), sVal, PATH_WORKING_INFO);

	double dTime = _tstof(sVal) * 100.0;
	int nTime = int(dTime);
	//pView->MpeWrite(pView->Plc.DlgInfo.UltraSonicStTimeAoiDn, (long)nTime);	// AOI_Dn (단위 [초] * 100) : 1 is 10 mSec.
	//pView->MpeWrite(pView->Plc.DlgInfo.UltraSonicStTimeAoiUp, (long)nTime);	// AOI_Up (단위 [초] * 100) : 1 is 10 mSec.
	pView->MpeWrite(pView->Plc.DlgInfo.UltraSonicStTimeAoi, (long)nTime);	// AOI(단위 [초] * 100) : 1 is 10 mSec.

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetUltraSonicStTim();	//_ItemInx::_UltraSonicStTim
#endif
}

void CDlgInfo::OnChk000() 
{
	// TODO: Add your control notification handler code here
	if (pView)
		pView->ClrDispMsg();

	CDlgUser Dlg;
	Dlg.DoModal();

	myStcData[0].SetText(pDoc->WorkingInfo.LastJob.sSelUserName);
	myBtn[0].SetCheck(FALSE);

	if(pDoc && pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_sUser = pDoc->WorkingInfo.LastJob.sSelUserName;
	if(pDoc)
		::WritePrivateProfileString(_T("Last Job"), _T("Operator Name"), pDoc->WorkingInfo.LastJob.sSelUserName, PATH_WORKING_INFO);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetOpName();	//_ItemInx::_OpName
#endif
}

void CDlgInfo::OnChk001() 
{
	// TODO: Add your control notification handler code here
	BOOL bUse;
	if(myBtn[1].GetCheck())
	{
		bUse = TRUE;
		pView->MpeWrite(pView->Plc.DlgInfo.UseLotLength, 1);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)-20141031
		pView->SetLotLastShot();
	}
	else
	{
		bUse = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.UseLotLength, 0);	// 로트분리사용(PC가 On시키고, PC가 확인하고 Off시킴)-20141031
	}

	pDoc->WorkingInfo.LastJob.bLotSep = bUse;
	if(pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_bUseLotSep = bUse;

	CString sData = bUse ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Lot seperate"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("SeparateLot"), bUse);
#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetLotCut();	//_ItemInx::_LotCut
#endif
}

void CDlgInfo::OnChk002() 
{
	// TODO: Add your control notification handler code here
	BOOL bUse = !pDoc->WorkingInfo.LastJob.bTempPause;
	Sleep(100);
	if (bUse)
	{
		pView->MpeWrite(pView->Plc.DlgInfo.UseStopLength, 1);	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)
		pView->ChkTempStop(TRUE);
		if (!myBtn[2].GetCheck())
			myBtn[2].SetCheck(TRUE);
	}
	else
	{
		pView->MpeWrite(pView->Plc.DlgInfo.UseStopLength, 0);	// 일시정지사용(PC가 On시키고, PLC가 확인하고 Off시킴)
		pView->ChkTempStop(FALSE);

		if (myBtn[2].GetCheck())
			myBtn[2].SetCheck(FALSE);
	}

	pDoc->WorkingInfo.LastJob.bTempPause = bUse;
	if(pDoc->m_pReelMap)
		pDoc->m_pReelMap->m_bUseTempPause = bUse;

	CString sData = bUse ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Temporary Pause"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("TempStop"), bUse);
#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetTempPause();	//_stSigInx::_TempPause
#endif
}

void CDlgInfo::OnChk003() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[3].GetCheck())
		pDoc->WorkingInfo.LastJob.bContFixDef = TRUE;
	else
		pDoc->WorkingInfo.LastJob.bContFixDef = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bContFixDef ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Continuous Fix Defect"), sData, PATH_WORKING_INFO);	
	pDoc->SetMkInfo(_T("Signal"), _T("FixBed"), pDoc->WorkingInfo.LastJob.bContFixDef);
#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetFixDef();	//_stSigInx::_FixDef
#endif
}

void CDlgInfo::OnChk004() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[4].GetCheck())
	{
		pDoc->WorkingInfo.LastJob.bRclDrSen = TRUE;
		pDoc->Status.bDoorReF[DOOR_FR_RC] = FALSE;
		pDoc->Status.bDoorReF[DOOR_S_RC] = FALSE;
		pDoc->Status.bDoorReF[DOOR_BL_RC] = FALSE;
		pDoc->Status.bDoorReF[DOOR_BR_RC] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bRclDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bRclDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Recoiler Door Sensor"), sData, PATH_WORKING_INFO);	
	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensRecoil"), pDoc->WorkingInfo.LastJob.bRclDrSen);

	pView->MpeWrite(pView->Plc.DlgInfo.DoorSensorRecoiler, pDoc->WorkingInfo.LastJob.bRclDrSen ? 1 : 0);	// 리코일러Door센서 사용

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetDoorRecoiler();	//_stSigInx::_DoorRecoiler
#endif
}

void CDlgInfo::OnChk005() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[5].GetCheck())		// 상면 AOI DOOR SENSOR
	{
		pDoc->WorkingInfo.LastJob.bAoiUpDrSen = TRUE;
		pDoc->Status.bDoorAoiF[DOOR_FM_AOI_UP] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_FL_AOI_UP] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_FR_AOI_UP] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BM_AOI_UP] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BL_AOI_UP] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BR_AOI_UP] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bAoiUpDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use AoiUp Door Sensor"), sData, PATH_WORKING_INFO);	
	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensAoiUp"), pDoc->WorkingInfo.LastJob.bAoiUpDrSen);

	pView->MpeWrite(pView->Plc.DlgInfo.DoorSensorAoiUp, pDoc->WorkingInfo.LastJob.bAoiUpDrSen ? 1 : 0);	// AOI(상) Door센서 사용

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetDoorAoiUp();	//_stSigInx::_DoorAoiUp
#endif
}

void CDlgInfo::OnChk006() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[6].GetCheck())
	{
		pDoc->WorkingInfo.LastJob.bMkDrSen = TRUE;
		pDoc->Status.bDoorMkF[DOOR_FL_MK] = FALSE;
		pDoc->Status.bDoorMkF[DOOR_FR_MK] = FALSE;
		pDoc->Status.bDoorMkF[DOOR_BL_MK] = FALSE;
		pDoc->Status.bDoorMkF[DOOR_BR_MK] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bMkDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bMkDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Marking Door Sensor"), sData, PATH_WORKING_INFO);	
	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensPunch"), pDoc->WorkingInfo.LastJob.bMkDrSen);

	pView->MpeWrite(pView->Plc.DlgInfo.DoorSensorPunch, pDoc->WorkingInfo.LastJob.bMkDrSen ? 1 : 0);	// 마킹Door센서 사용

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetDoorMk();	//_stSigInx::_DoorMk
#endif
}

void CDlgInfo::OnChk007() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[7].GetCheck())
	{
		pDoc->WorkingInfo.LastJob.bUclDrSen = TRUE;
		pDoc->Status.bDoorUcF[DOOR_FL_UC] = FALSE;
		pDoc->Status.bDoorUcF[DOOR_FR_UC] = FALSE;
		pDoc->Status.bDoorUcF[DOOR_BL_UC] = FALSE;
		pDoc->Status.bDoorUcF[DOOR_BR_UC] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bUclDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bUclDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Uncoiler Door Sensor"), sData, PATH_WORKING_INFO);	
	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensUncoil"), pDoc->WorkingInfo.LastJob.bUclDrSen);

	pView->MpeWrite(pView->Plc.DlgInfo.DoorSensorUncoiler, pDoc->WorkingInfo.LastJob.bUclDrSen ? 1 : 0);	// 언코일러Door센서 사용

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetDoorUncoiler();	//_stSigInx::_DoorUncoiler
#endif
}

void CDlgInfo::OnChk008() 
{
	if(myBtn[8].GetCheck())
		pDoc->WorkingInfo.LastJob.bUse380mm = TRUE;
	else
		pDoc->WorkingInfo.LastJob.bUse380mm = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bUse380mm ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use 380mm Roll"), sData, PATH_WORKING_INFO);

	pView->MpeWrite(pView->Plc.DlgInfo.Laser380mm, pDoc->WorkingInfo.LastJob.bUse380mm ? 1 : 0);	// EPC실린더(제품소->OFF/제품대->ON)

	if (pDoc->WorkingInfo.LastJob.bUse380mm)
	{
		pDoc->WorkingInfo.LastJob.bUse340mm = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.Laser340mm, 0);
		if (pDoc->WorkingInfo.LastJob.bUse340mm)
			myBtn[26].SetCheck(TRUE);
		else
			myBtn[26].SetCheck(FALSE);

		pDoc->WorkingInfo.LastJob.bUse346mm = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.Laser346mm, 0);
		if (pDoc->WorkingInfo.LastJob.bUse346mm)
			myBtn[27].SetCheck(TRUE);
		else
			myBtn[27].SetCheck(FALSE);
	}

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetUse380mm();	//_stSigInx::_Use380mm
#endif

}

void CDlgInfo::OnChk19()
{
	if (myBtn[26].GetCheck())
		pDoc->WorkingInfo.LastJob.bUse340mm = TRUE;
	else
		pDoc->WorkingInfo.LastJob.bUse340mm = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bUse340mm ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use 340mm Roll"), sData, PATH_WORKING_INFO);

	pView->MpeWrite(pView->Plc.DlgInfo.Laser340mm, pDoc->WorkingInfo.LastJob.bUse340mm ? 1 : 0);	// EPC실린더(제품소->OFF/제품대->ON)

	if (pDoc->WorkingInfo.LastJob.bUse340mm)
	{
		pDoc->WorkingInfo.LastJob.bUse346mm = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.Laser346mm, 0);
		if (pDoc->WorkingInfo.LastJob.bUse346mm)
			myBtn[27].SetCheck(TRUE);
		else
			myBtn[27].SetCheck(FALSE);

		pDoc->WorkingInfo.LastJob.bUse380mm = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.Laser380mm, 0);
		if (pDoc->WorkingInfo.LastJob.bUse380mm)
			myBtn[8].SetCheck(TRUE);
		else
			myBtn[8].SetCheck(FALSE);
	}

#ifdef USE_ENGRAVE
	//if (pView && pView->m_pEngrave)
	//	pView->m_pEngrave->SetUse340mm();	//_stSigInx::_Use340mm
#endif

}

void CDlgInfo::OnChk23()
{
	if (myBtn[27].GetCheck())
		pDoc->WorkingInfo.LastJob.bUse346mm = TRUE;
	else
		pDoc->WorkingInfo.LastJob.bUse346mm = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bUse346mm ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use 346mm Roll"), sData, PATH_WORKING_INFO);

	pView->MpeWrite(pView->Plc.DlgInfo.Laser346mm, pDoc->WorkingInfo.LastJob.bUse346mm ? 1 : 0);	// EPC실린더(제품소->OFF/제품대->ON)

	if (pDoc->WorkingInfo.LastJob.bUse346mm)
	{
		pDoc->WorkingInfo.LastJob.bUse340mm = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.Laser340mm, 0);
		if (pDoc->WorkingInfo.LastJob.bUse340mm)
			myBtn[26].SetCheck(TRUE);
		else
			myBtn[26].SetCheck(FALSE);

		pDoc->WorkingInfo.LastJob.bUse380mm = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.Laser380mm, 0);
		if (pDoc->WorkingInfo.LastJob.bUse380mm)
			myBtn[8].SetCheck(TRUE);
		else
			myBtn[8].SetCheck(FALSE);

		//pDoc->WorkingInfo.LastJob.bUse346mm = FALSE;
		//pView->MpeWrite(pView->Plc.DlgInfo.Laser346mm, 0);
		//if (pDoc->WorkingInfo.LastJob.bUse346mm)
		//	myBtn[27].SetCheck(TRUE);
		//else
		//	myBtn[27].SetCheck(FALSE);
	}

#ifdef USE_ENGRAVE
	//if (pView && pView->m_pEngrave)
	//	pView->m_pEngrave->SetUse347mm();	//_stSigInx::_Use346mm
#endif

}

void CDlgInfo::OnChk009() 
{
	// TODO: Add your control notification handler code here
	if (myBtn[9].GetCheck())
	{
		pDoc->WorkingInfo.LastJob.bEngvDrSen = TRUE;
		pDoc->Status.bDoorEngvF[DOOR_FL_ENGV] = FALSE;
		pDoc->Status.bDoorEngvF[DOOR_FR_ENGV] = FALSE;
		pDoc->Status.bDoorEngvF[DOOR_BL_ENGV] = FALSE;
		pDoc->Status.bDoorEngvF[DOOR_BR_ENGV] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bEngvDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bEngvDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Engrave Door Sensor"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensEngrave"), pDoc->WorkingInfo.LastJob.bEngvDrSen);

	pView->MpeWrite(pView->Plc.DlgInfo.DoorSensorEngrave, pDoc->WorkingInfo.LastJob.bEngvDrSen ? 1 : 0);	// 각인부 Door센서 사용

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetDoorEngrave();	//_stSigInx::_DoorEngrave
#endif
}

void CDlgInfo::OnChk010() 
{
	// TODO: Add your control notification handler code here
	if(myBtn[10].GetCheck())
		pDoc->WorkingInfo.LastJob.bMkSftySen = TRUE;
	else
		pDoc->WorkingInfo.LastJob.bMkSftySen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bMkSftySen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Marking Safty Sensor"), sData, PATH_WORKING_INFO);	
	pDoc->SetMkInfo(_T("Signal"), _T("SaftySensPunch"), pDoc->WorkingInfo.LastJob.bMkSftySen);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetSaftyMk();	//_stSigInx::_SaftyMk
#endif
}

void CDlgInfo::OnChk011() 
{
	// TODO: Add your control notification handler code here
	if (myBtn[11].GetCheck())		// 하면 AOI DOOR SENSOR
	{
		pDoc->WorkingInfo.LastJob.bAoiDnDrSen = TRUE;
		pDoc->Status.bDoorAoiF[DOOR_FM_AOI_DN] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_FL_AOI_DN] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_FR_AOI_DN] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BM_AOI_DN] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BL_AOI_DN] = FALSE;
		pDoc->Status.bDoorAoiF[DOOR_BR_AOI_DN] = FALSE;
	}
	else
		pDoc->WorkingInfo.LastJob.bAoiDnDrSen = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use AoiDn Door Sensor"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("DoorSensAoiDn"), pDoc->WorkingInfo.LastJob.bAoiDnDrSen);

	pView->MpeWrite(pView->Plc.DlgInfo.DoorSensorAoiDn, pDoc->WorkingInfo.LastJob.bAoiDnDrSen ? 1 : 0);	// AOI(하) Door센서 사용

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetDoorAoiDn();	//_stSigInx::_DoorAoiDn
#endif
}

void CDlgInfo::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	if (pView->m_pDlgMenu01)
		pView->m_pDlgMenu01->UpdateData();

	OnOK();
}

void CDlgInfo::OnStc174() 
{
	// TODO: Add your control notification handler code here
	myStcData[9].SetBkColor(RGB_RED);
	myStcData[9].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_174)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_174, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[9].SetBkColor(RGB_WHITE);
	myStcData[9].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_174)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sStripOutRatio = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Strip Out Ratio"), sVal, PATH_WORKING_INFO);	

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetStripOutRatio();	//_ItemInx::_StripOutRatio
#endif
}

void CDlgInfo::OnStc183() 
{
	// TODO: Add your control notification handler code here
	myStcData[14].SetBkColor(RGB_RED);
	myStcData[14].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_183)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_183, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[14].SetBkColor(RGB_WHITE);
	myStcData[14].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_183)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sCustomNeedRatio = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Custom Need Ratio"), sVal, PATH_WORKING_INFO);	

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetCustomNeedRatio();	//_ItemInx::_CustomNeedRatio
#endif
}

void CDlgInfo::OnStc178() 
{
	// TODO: Add your control notification handler code here
	myStcData[10].SetBkColor(RGB_RED);
	myStcData[10].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_178)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_178, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[10].SetBkColor(RGB_WHITE);
	myStcData[10].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_178)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sPartialSpd = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Partial Speed"), sVal, PATH_WORKING_INFO);	

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetPartVel();	//_ItemInx::_PartVel
#endif
}

HBRUSH CDlgInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG) {
   		pDC->SetBkMode(TRANSPARENT);
     	hbr = CreateSolidBrush(RGB(0,0,0));
  	} 	
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgInfo::OnStc61() 
{
	// TODO: Add your control notification handler code here
	myStcData[11].SetBkColor(RGB_RED);
	myStcData[11].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_61)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_61, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[11].SetBkColor(RGB_WHITE);
	myStcData[11].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_61)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sNumRangeFixDef = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Shot Num of Range in Fix Defect"), sVal, PATH_WORKING_INFO);	

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetNumRangeFixDef();	//_ItemInx::_NumRangeFixDef
#endif

}

int CDlgInfo::GetTestMode()
{
	return pDoc->GetTestMode();
}

void CDlgInfo::SetTestMode(int nMode)
{
	pDoc->SetTestMode(nMode);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetTestMode();	//_ItemInx::_TestMode
#endif

	myBtn[23].RedrawWindow();
	myBtn[24].RedrawWindow();
}

void CDlgInfo::SetDualTest(BOOL bOn)
{
	pDoc->WorkingInfo.LastJob.bDualTest = bOn;

	CString sData = bOn ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Dual AOI"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("Use Dual AOI"), bOn);
	if(pView->m_pEngrave)
		::WritePrivateProfileString(_T("Last Job"), _T("Use Dual AOI"), sData, PATH_WORKING_INFO);

	::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
	::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
	GetDlgItem(IDC_STC_181)->SetWindowText(_T(""));

	if(bOn)
	{
		sData = _T("6");
		myBtn[12].SetCheck(TRUE); // 양면 검사
		myBtn[13].SetCheck(FALSE); // 단면 검사
		pView->SetTwoMetal(TRUE, TRUE); // (TwoMetal, On)
	}
	else
	{
		sData = _T("12");
		myBtn[12].SetCheck(FALSE); // 양면 검사
		myBtn[13].SetCheck(TRUE); // 단면 검사
		pView->SetTwoMetal(FALSE, TRUE); // (OneMetal, On)
	}
	
	myBtn[12].RedrawWindow();
	myBtn[13].RedrawWindow();

	pDoc->WorkingInfo.System.sMaxDispDefImg = sData;
	::WritePrivateProfileString(_T("System"), _T("MAX_DISP_DEF_IMAGE"), sData, PATH_WORKING_INFO);
	pView->SetDualTest(bOn);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetDualTest();	//_stSigInx::_DualTest
#endif
	DispDevicePartial();
}

void CDlgInfo::SetFeedDir(int nUnit)
{
	BOOL bChk[2];
	bChk[0] = myBtn[15].GetCheck(); // IDC_CHK_RECOILER_CCW - Recoiler\r정방향 CW : FALSE
	bChk[1] = myBtn[16].GetCheck(); // IDC_CHK_UNCOILER_CCW - Uncoiler\r정방향 CW : FALSE

	if(nUnit == 1)
	{
		pView->SetFeedDir(nUnit, bChk[1]);
		//pView->SetTwoMetal(bOn, bChk[1]); // (TwoMetal, On)

		if(bChk[1])
		{
			myBtn[16].SetTextColor(RGB_DARKRED);
			myBtn[16].SetWindowText(_T("Uncoiler\r역방향"));
			pDoc->SetMkInfo(_T("Signal"), _T("UncoilerCcw"), TRUE);
		}
		else
		{
			myBtn[16].SetTextColor(RGB_BLUE);
			myBtn[16].SetWindowText(_T("Uncoiler\r정방향"));
			pDoc->SetMkInfo(_T("Signal"), _T("UncoilerCcw"), FALSE);
		}
#ifdef USE_ENGRAVE
		if (pView && pView->m_pEngrave)
			pView->m_pEngrave->SetUncoilerCcw();	//_stSigInx::_UncoilerCcw
#endif

// 		if(bChk[0])
// 			myBtn[15].SetCheck(FALSE);
// 		if(!bChk[1])
// 			myBtn[16].SetCheck(TRUE);

	}
	else
	{
		pView->SetFeedDir(nUnit, bChk[0]);
		//pView->SetTwoMetal(bOn, bChk[0]); // (OneMetal, On)

		if(bChk[0])
		{
			myBtn[15].SetTextColor(RGB_DARKRED);
			myBtn[15].SetWindowText(_T("Recoiler\r역방향"));
			pDoc->SetMkInfo(_T("Signal"), _T("RecoilerCcw"), TRUE);
		}
		else
		{
			myBtn[15].SetTextColor(RGB_BLUE);
			myBtn[15].SetWindowText(_T("Recoiler\r정방향"));
			pDoc->SetMkInfo(_T("Signal"), _T("RecoilerCcw"), FALSE);
		}
#ifdef USE_ENGRAVE
		if (pView && pView->m_pEngrave)
			pView->m_pEngrave->SetRecoilerCcw();	//_stSigInx::_RecoilerCcw
#endif

// 		if(!bChk[0])
// 			myBtn[15].SetCheck(TRUE);
// 		if(bChk[1])
// 			myBtn[16].SetCheck(FALSE);
	}

}

void CDlgInfo::OnChkUseAoiInner() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn[3];
	bOn[0] = myBtn[23].GetCheck();
	bOn[1] = myBtn[24].GetCheck();
	bOn[2] = myBtn[25].GetCheck();
	GetDlgItem(IDC_STC_181)->SetWindowText(_T(""));

	if (bOn[0] && ( bOn[1] || bOn[2] ))
	{
		myBtn[24].SetCheck(FALSE);
		myBtn[25].SetCheck(FALSE);
		SetTestMode(MODE_INNER);
	}
	else if (bOn[0] && !(bOn[1] || bOn[2]))
		SetTestMode(MODE_INNER);
	else if (!bOn[0] && bOn[1] && bOn[2])
		SetTestMode(MODE_MIDDLE);
	else if (!bOn[0] && bOn[1] && !bOn[2])
		SetTestMode(MODE_OUTER);
	else
		SetTestMode(MODE_NONE);

	DispDevicePartial();
}

void CDlgInfo::OnChkUseAoiOuter() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn[3] = { 0 };
	bOn[0] = myBtn[23].GetCheck();
	bOn[1] = myBtn[24].GetCheck();
	bOn[2] = myBtn[25].GetCheck();
	GetDlgItem(IDC_STC_181)->SetWindowText(_T(""));

	if (bOn[0] && bOn[1])
	{
		myBtn[23].SetCheck(FALSE);
		myBtn[25].SetCheck(FALSE);
		SetTestMode(MODE_OUTER);
	}
	else if (bOn[0] && !(bOn[1] || bOn[2]))
		SetTestMode(MODE_INNER);
	else if (!bOn[0] && bOn[1] && bOn[2])
		SetTestMode(MODE_MIDDLE);
	else if (!bOn[0] && bOn[1] && !bOn[2])
		SetTestMode(MODE_OUTER);
	else
		SetTestMode(MODE_NONE);

	DispDevicePartial();
}

void CDlgInfo::OnChkUseAoiDual() 
{
	// TODO: Add your control notification handler code here
	SetDualTest(TRUE);
}

void CDlgInfo::OnChkUseAoiSingle() 
{
	// TODO: Add your control notification handler code here
	SetDualTest(FALSE);	
}

void CDlgInfo::OnChkSampleTest() 
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[14].GetCheck();
	pDoc->WorkingInfo.LastJob.bSampleTest = bOn;

	CString sData = bOn ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Sample Test On"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("Sample Test On"), bOn);

	::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), sData, pDoc->WorkingInfo.System.sPathMkCurrInfo);
	if(!bOn)
		::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);

	GetDlgItem(IDC_STC_181)->SetWindowText(_T(""));
	pView->MpeWrite(pView->Plc.DlgInfo.SampleTest, (pDoc->WorkingInfo.LastJob.bSampleTest)?1:0);		// Sample 검사 On

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetSampleTest();	//_stSigInx::_SampleTest
#endif
}

void CDlgInfo::OnChkRecoilerCcw()
{
	// TODO: Add your control notification handler code here
	if (pView->IsAuto())
		return;

	SetFeedDir(0); // SetFeedDir (0): Recoiler , (1): Uncoiler

	BOOL bOn = myBtn[15].GetCheck();
	//BOOL bOn = !pDoc->WorkingInfo.LastJob.bOneMetal;
	if (bOn)
	{
		pDoc->WorkingInfo.LastJob.bOneMetal = TRUE;
		pView->MpeWrite(pView->Plc.DlgInfo.OneMetal, 1);
		::WritePrivateProfileString(_T("Last Job"), _T("One Metal On"), _T("1"), PATH_WORKING_INFO);// IDC_CHK_RECOILER_CCW - Recoiler\r정방향 CW : FALSE
	}
	else
	{
		pDoc->WorkingInfo.LastJob.bOneMetal = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.OneMetal, 0);
		::WritePrivateProfileString(_T("Last Job"), _T("One Metal On"), _T("0"), PATH_WORKING_INFO);// IDC_CHK_RECOILER_CCW - Recoiler\r정방향 CW : FALSE
	}
	pDoc->SetMkInfo(_T("Signal"), _T("RecoilerCcw"), bOn);

	::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
	::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
	GetDlgItem(IDC_STC_181)->SetWindowText(_T(""));

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetRecoilerCcw();	//_stSigInx::_RecoilerCcw
#endif
}

void CDlgInfo::OnChkUncoilerCcw()
{
	// TODO: Add your control notification handler code here
	if (pView->IsAuto())
		return;

	SetFeedDir(1); // SetFeedDir (0): Recoiler , (1): Uncoiler

	BOOL bOn = myBtn[16].GetCheck();
	//BOOL bOn = !pDoc->WorkingInfo.LastJob.bTwoMetal;
	if(bOn)
	{
		pDoc->WorkingInfo.LastJob.bTwoMetal = TRUE;
		pView->MpeWrite(pView->Plc.DlgInfo.TwoMetal, 1);
		::WritePrivateProfileString(_T("Last Job"), _T("Two Metal On"), _T("1"), PATH_WORKING_INFO);// IDC_CHK_TWO_METAL - Uncoiler\r역방향 ON : TRUE	
	}
	else
	{
		pDoc->WorkingInfo.LastJob.bTwoMetal = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.TwoMetal, 0);
		::WritePrivateProfileString(_T("Last Job"), _T("Two Metal On"), _T("0"), PATH_WORKING_INFO);// IDC_CHK_TWO_METAL - Uncoiler\r역방향 ON : TRUE	
	}
	pDoc->SetMkInfo(_T("Signal"), _T("UncoilerCcw"), bOn);

	::WritePrivateProfileString(_T("Infomation"), _T("Lot End"), _T("0"), pDoc->WorkingInfo.System.sPathMkCurrInfo);
	::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), _T("10000"), pDoc->WorkingInfo.System.sPathMkCurrInfo);

	GetDlgItem(IDC_STC_181)->SetWindowText(_T(""));

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetUncoilerCcw();	//_stSigInx::_UncoilerCcw
#endif
}

void CDlgInfo::OnStc181() 
{
	// TODO: Add your control notification handler code here
	myStcData[12].SetBkColor(RGB_RED);
	myStcData[12].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_181)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_181, pt, TO_BOTTOM|TO_RIGHT);

	myStcData[12].SetBkColor(RGB_WHITE);
	myStcData[12].RedrawWindow();
	
	CString sVal;
	GetDlgItem(IDC_STC_181)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.sSampleTestShotNum = sVal;
	::WritePrivateProfileString(_T("Last Job"), _T("Sample Test Shot Num"), sVal, PATH_WORKING_INFO);

	::WritePrivateProfileString(_T("Infomation"), _T("Last Shot"), sVal, pDoc->WorkingInfo.System.sPathMkCurrInfo);
	
	long lData = (long)_tstoi(pDoc->WorkingInfo.LastJob.sSampleTestShotNum);//atoi
	pView->MpeWrite(pView->Plc.DlgInfo.SampleTestShotNum, lData);	// 샘플검사 Shot수

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetSampleShotNum();	//_stSigInx::_SampleShotNum
#endif
}


void CDlgInfo::OnBnClickedChk2PointAlign()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bChk = myBtn[17].GetCheck();
	CString sMsg;

	int nAlignMethodeOnCamMst = pDoc->m_Master[0].GetAlignMethode();
	if (bChk)
	{
		if ( !(nAlignMethodeOnCamMst & TWO_POINT) )
		{
			myBtn[17].SetCheck(FALSE);
			sMsg.Format(_T("%s 모델의 Align 설정이 2PointAlign 이 아닙니다."), pDoc->WorkingInfo.LastJob.sModel);
			pView->MsgBox(sMsg);
			return;
		}
	}

	myBtn[18].SetCheck(FALSE);

	if (bChk)
	{
		CString sVal;
		pDoc->WorkingInfo.LastJob.nAlignMethode = TWO_POINT;
		sVal.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nAlignMethode);
		::WritePrivateProfileString(_T("Last Job"), _T("Align Methode"), sVal, PATH_WORKING_INFO);
	}
	else
	{
		myBtn[17].SetCheck(TRUE);
	}

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

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetAlignMethode();	//_stSigInx::_AlignMethode  // TWO_POINT, FOUR_POINT
#endif
}


void CDlgInfo::OnBnClickedChk4PointAlign()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bChk = myBtn[18].GetCheck();
	CString sMsg;

	int nAlignMethodeOnCamMst = pDoc->m_Master[0].GetAlignMethode();
	if (bChk)
	{
		if ( !(nAlignMethodeOnCamMst & FOUR_POINT))
		{
			myBtn[18].SetCheck(FALSE);
			sMsg.Format(_T("%s 모델의 Align 설정이 4PointAlign 이 아닙니다."), pDoc->WorkingInfo.LastJob.sModel);
			pView->MsgBox(sMsg);
			return;
		}
	}

	myBtn[17].SetCheck(FALSE);

	if (bChk)
	{
		CString sVal;
		pDoc->WorkingInfo.LastJob.nAlignMethode = FOUR_POINT;
		sVal.Format(_T("%d"), pDoc->WorkingInfo.LastJob.nAlignMethode);
		::WritePrivateProfileString(_T("Last Job"), _T("Align Methode"), sVal, PATH_WORKING_INFO);
	}
	else
	{
		myBtn[18].SetCheck(TRUE);
	}

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

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetAlignMethode();	//_stSigInx::_AlignMethode  // TWO_POINT, FOUR_POINT
#endif
}

void CDlgInfo::OnBnClickedChk85()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (myBtn[19].GetCheck()) //하면AOI 클린롤러
	{
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = TRUE;
		pView->MpeWrite(pView->Plc.DlgInfo.CleanRollerAoiDn, 1);
	}
	else
	{
		pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.CleanRollerAoiDn, 0);
	}

	CString sData = pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Dn Aoi CleanRoler"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("CleanRolerAoiDn"), pDoc->WorkingInfo.LastJob.bUseAoiDnCleanRoler);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetCleannerAoiDn();	//_stSigInx::_CleannerAoiDn
#endif
}

void CDlgInfo::OnBnClickedChk86()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (myBtn[20].GetCheck()) //상면AOI 클린롤러
	{
		pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = TRUE;
		pView->MpeWrite(pView->Plc.DlgInfo.CleanRollerAoiUp, 1);
	}
	else
	{
		pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.CleanRollerAoiUp, 0);
	}

	CString sData = pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Up Aoi CleanRoler"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("CleanRolerAoiUp"), pDoc->WorkingInfo.LastJob.bUseAoiUpCleanRoler);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetCleannerAoiUp();	//_stSigInx::_CleannerAoiUp
#endif
}


void CDlgInfo::OnBnClickedChk1187()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (myBtn[21].GetCheck()) //AOI초음파세정기
	{
		pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic= TRUE;
		pView->MpeWrite(pView->Plc.DlgInfo.UltraSonicAoi, 1);
	}
	else
	{
		pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.UltraSonicAoi, 0);
	}

	CString sData = pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use AoiDn Cleanner"), sData, PATH_WORKING_INFO); //AOI초음파세정기
	pDoc->SetMkInfo(_T("Signal"), _T("UltrasonicAoi"), pDoc->WorkingInfo.LastJob.bUseAoiDnUltrasonic);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetUltraSonicAoiDn();	//_stSigInx::_UltraSonicAoiDn
#endif
}


void CDlgInfo::OnBnClickedChk1188()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (myBtn[22].GetCheck()) //각인부초음파세정기
	{
		pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic = TRUE;
		pView->MpeWrite(pView->Plc.DlgInfo.UltraSonicEngrave, 1);
	}
	else
	{
		pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic = FALSE;
		pView->MpeWrite(pView->Plc.DlgInfo.UltraSonicEngrave, 0);
	}

	CString sData = pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Engrave Cleanner"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("UltrasonicEngrave"), pDoc->WorkingInfo.LastJob.bUseEngraveUltrasonic);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetUltraSonicEngrave();	//_stSigInx::_UltraSonicEngrave
#endif
}

void CDlgInfo::UpdateData()
{
	Disp();
}


void CDlgInfo::OnStnClickedStc36()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	myStcData[15].SetBkColor(RGB_RED);
	myStcData[15].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_36)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_36, pt, TO_BOTTOM | TO_RIGHT);

	myStcData[15].SetBkColor(RGB_WHITE);
	myStcData[15].RedrawWindow();

	CString sVal;
	GetDlgItem(IDC_STC_36)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.nVerifyPeriod = _ttoi(sVal);

	::WritePrivateProfileString(_T("Last Job"), _T("Verify Period"), sVal, PATH_WORKING_INFO);
}


void CDlgInfo::OnStnClickedStc17()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	myStcData[16].SetBkColor(RGB_RED);
	myStcData[16].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_17)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_17, pt, TO_BOTTOM | TO_RIGHT);

	myStcData[16].SetBkColor(RGB_WHITE);
	myStcData[16].RedrawWindow();

	CString sData;
	GetDlgItem(IDC_STC_17)->GetWindowText(sData);
	pDoc->SetEngItsCode(sData);

#ifdef USE_ENGRAVE
	if (pView && pView->m_pEngrave)
		pView->m_pEngrave->SetEngItsCode();	//_ItemInx::_EngItsCode
#endif
}


void CDlgInfo::OnStnClickedStc41()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgInfo::OnStnClickedStc43()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgInfo::OnStnClickedStc82()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgInfo::OnStnClickedStc83()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgInfo::OnBnClickedChkUseAoiMiddle()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	BOOL bOn[3];
	bOn[0] = myBtn[23].GetCheck();
	bOn[1] = myBtn[24].GetCheck();
	bOn[2] = myBtn[25].GetCheck();
	GetDlgItem(IDC_STC_181)->SetWindowText(_T(""));

	if (bOn[0] && bOn[2])
	{
		myBtn[23].SetCheck(FALSE);
		myBtn[24].SetCheck(TRUE);
		SetTestMode(MODE_MIDDLE);
	}
	else if (bOn[0] && !(bOn[1] || bOn[2]))
		SetTestMode(MODE_INNER);
	else if (!bOn[0] && bOn[1] && bOn[2])
		SetTestMode(MODE_MIDDLE);
	else if (!bOn[0] && bOn[1] && !bOn[2])
		SetTestMode(MODE_OUTER);
	else
		SetTestMode(MODE_NONE);
}


void CDlgInfo::OnTimer(UINT_PTR nIDEvent)
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

void CDlgInfo::OnChk26()
{
	// TODO: Add your control notification handler code here
	CString strFileNReject, strTemp;
	CString sPathCamSpecDir = pDoc->WorkingInfo.System.sPathCamSpecDir;
	CString sModel = pDoc->WorkingInfo.LastJob.sModel;
	CString sLayer = pDoc->WorkingInfo.LastJob.sLayerUp;

	if (myBtn[28].GetCheck())
	{
		if (sPathCamSpecDir.Right(1) != "\\")
			strFileNReject.Format(_T("%s\\%s\\%s_RejectMark.TIF"), sPathCamSpecDir, sModel, sLayer);
		else
			strFileNReject.Format(_T("%s%s\\%s_RejectMark.TIF"), sPathCamSpecDir, sModel, sLayer);

		CFileFind finder;
		if (!finder.FindFile(strFileNReject))
		{
			strTemp.Format(_T("%s \r\n: Reject 마크 이미지가 없습니다."), strFileNReject);
			pView->MsgBox(strTemp);
			myBtn[28].SetCheck(FALSE);
			return;
		}

		pDoc->WorkingInfo.LastJob.bUseJudgeMk = TRUE;
	}
	else
		pDoc->WorkingInfo.LastJob.bUseJudgeMk = FALSE;

	CString sData = pDoc->WorkingInfo.LastJob.bUseJudgeMk ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("Last Job"), _T("Use Judge Marking"), sData, PATH_WORKING_INFO);
	//pDoc->SetMkInfo(_T("Signal"), _T("JudgeMk"), pDoc->WorkingInfo.LastJob.bUseJudgeMk);
}

void CDlgInfo::OnStnClickedStc187()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	myStcData[21].SetBkColor(RGB_RED);
	myStcData[21].RedrawWindow();

	CPoint pt;	CRect rt;
	GetDlgItem(IDC_STC_187)->GetWindowRect(&rt);
	pt.x = rt.right; pt.y = rt.bottom;
	ShowKeypad(IDC_STC_187, pt, TO_BOTTOM | TO_RIGHT);

	myStcData[21].SetBkColor(RGB_WHITE);
	myStcData[21].RedrawWindow();

	CString sVal;
	GetDlgItem(IDC_STC_187)->GetWindowText(sVal);
	pDoc->WorkingInfo.LastJob.nJudgeMkRatio = _ttoi(sVal);
	pDoc->SetVerifyPunchScore(_ttof(sVal));

	::WritePrivateProfileString(_T("Last Job"), _T("Judge Marking Ratio"), sVal, PATH_WORKING_INFO);

}

void CDlgInfo::DispDevicePartial()
{
	CString sMsg;
	BOOL bDualTest = pDoc->WorkingInfo.LastJob.bDualTest;
#ifdef USE_MPE
	bDualTest = pView->MpeRead(pView->Plc.DlgInfo.TwoMetal) > 0 ? TRUE : FALSE;
#endif

	if (!pDoc->WorkingInfo.LastJob.nTestMode && bDualTest)
	{
		pDoc->WorkingInfo.System.bUseDevicePartial = TRUE;
		GetDlgItem(IDC_CHK_USE_ENGRAVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHK_USE_AOI_UP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHK_USE_AOI_DN)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHK_USE_PUNCH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHK_USE_PUNCH_ONLY)->ShowWindow(SW_SHOW);
	}
	else
	{
		pDoc->WorkingInfo.System.bUseDevicePartial = FALSE;
		GetDlgItem(IDC_CHK_USE_ENGRAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHK_USE_AOI_UP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHK_USE_AOI_DN)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHK_USE_PUNCH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHK_USE_PUNCH_ONLY)->ShowWindow(SW_HIDE);
	}
	if (pDoc->WorkingInfo.LastJob.bDualTest != bDualTest)
	{
		sMsg.Format(_T("PLC 양면모드: %d , PC 양면모드: %d \r\n설정 오류입니다."), bDualTest ? 1 : 0, pDoc->WorkingInfo.LastJob.bDualTest ? 1 : 0);
		pView->MsgBox(sMsg);
	}

	BOOL bOn;
	if (pDoc->WorkingInfo.System.bUseDevicePartial)
	{
		bOn = pDoc->WorkingInfo.System.bUsePunchOnly;
		if (bOn)
		{
			pDoc->WorkingInfo.System.bUseEngrave = FALSE;
			pDoc->WorkingInfo.System.bUseAoiUp = FALSE;
			pDoc->WorkingInfo.System.bUseAoiDn = FALSE;
			pDoc->WorkingInfo.System.bUsePunch = TRUE;
		}

		if (pDoc->WorkingInfo.System.bUseEngrave)
		{
			myBtn[29].SetCheck(TRUE);
			myBtn[29].SetTextColor(RGB_BLACK);
			myBtn[29].SetWindowText(_T("각인부\r사용"));
			pDoc->SetMkInfo(_T("Signal"), _T("UseEngrave"), TRUE);
			pView->MpeWrite(pView->Plc.DlgInfo.UseEngrave, 1);
		}
		else
		{
			myBtn[29].SetCheck(FALSE);
			myBtn[29].SetTextColor(RGB_BLUE);
			myBtn[29].SetWindowText(_T("각인부\r미사용"));
			pDoc->SetMkInfo(_T("Signal"), _T("UseEngrave"), FALSE);
			pView->MpeWrite(pView->Plc.DlgInfo.UseEngrave, 0);
		}

		if (pDoc->WorkingInfo.System.bUseAoiUp)
		{
			myBtn[30].SetCheck(TRUE);
			myBtn[30].SetTextColor(RGB_BLACK);
			myBtn[30].SetWindowText(_T("상부AOI\r사용"));
			pDoc->SetMkInfo(_T("Signal"), _T("UseAoiUp"), TRUE);
			pView->MpeWrite(pView->Plc.DlgInfo.UseAoiUp, 1);
		}
		else
		{
			myBtn[30].SetCheck(FALSE);
			myBtn[30].SetTextColor(RGB_BLUE);
			myBtn[30].SetWindowText(_T("상부AOI\r미사용"));
			pDoc->SetMkInfo(_T("Signal"), _T("UseAoiUp"), FALSE);
			pView->MpeWrite(pView->Plc.DlgInfo.UseAoiUp, 0);
		}

		if (pDoc->WorkingInfo.System.bUseAoiDn)
		{
			myBtn[31].SetCheck(TRUE);
			myBtn[31].SetTextColor(RGB_BLACK);
			myBtn[31].SetWindowText(_T("하부AOI\r사용"));
			pDoc->SetMkInfo(_T("Signal"), _T("UseAoiDn"), TRUE);
			pView->MpeWrite(pView->Plc.DlgInfo.UseAoiDn, 1);
		}
		else
		{
			myBtn[31].SetCheck(FALSE);
			myBtn[31].SetTextColor(RGB_BLUE);
			myBtn[31].SetWindowText(_T("하부AOI\r미사용"));
			pDoc->SetMkInfo(_T("Signal"), _T("UseAoiDn"), FALSE);
			pView->MpeWrite(pView->Plc.DlgInfo.UseAoiDn, 0);
		}

		if (pDoc->WorkingInfo.System.bUsePunch)
		{
			myBtn[32].SetCheck(TRUE);
			myBtn[32].SetTextColor(RGB_BLACK);
			myBtn[32].SetWindowText(_T("펀칭부\r사용"));
			pDoc->SetMkInfo(_T("Signal"), _T("UsePunch"), TRUE);
			pView->MpeWrite(pView->Plc.DlgInfo.UsePunch, 1);
		}
		else
		{
			myBtn[32].SetCheck(FALSE);
			myBtn[32].SetTextColor(RGB_BLUE);
			myBtn[32].SetWindowText(_T("펀칭부\r미사용"));
			pDoc->SetMkInfo(_T("Signal"), _T("UsePunch"), FALSE);
			pView->MpeWrite(pView->Plc.DlgInfo.UsePunch, 0);
		}

		if (pDoc->WorkingInfo.System.bUsePunchOnly)
		{
			myBtn[33].SetCheck(TRUE);
			myBtn[33].SetTextColor(RGB_BLACK);
			myBtn[33].SetWindowText(_T("펀칭부만\r사용On"));
			pDoc->SetMkInfo(_T("Signal"), _T("UsePunchOnly"), TRUE);
			pView->MpeWrite(pView->Plc.DlgInfo.UsePunchOnly, 1);
		}
		else
		{
			myBtn[33].SetCheck(FALSE);
			myBtn[33].SetTextColor(RGB_BLUE);
			myBtn[33].SetWindowText(_T("펀칭부만\r사용Off"));
			pDoc->SetMkInfo(_T("Signal"), _T("UsePunchOnly"), FALSE);
			pView->MpeWrite(pView->Plc.DlgInfo.UsePunchOnly, 0);
		}
	}
}

void CDlgInfo::OnChkUseEngrave()
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[29].GetCheck();
	if (bOn)
		pDoc->WorkingInfo.System.bUseEngrave = TRUE;
	else
		pDoc->WorkingInfo.System.bUseEngrave = FALSE;

	CString sData = pDoc->WorkingInfo.System.bUseEngrave ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("System"), _T("UseEngrave"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("UseEngrave"), pDoc->WorkingInfo.System.bUseEngrave);			// 각인부\r미사용
	pView->MpeWrite(pView->Plc.DlgInfo.UseEngrave, pDoc->WorkingInfo.System.bUseEngrave ? 1 : 0);	// 각인부\r미사용
	DispDevicePartial();
}

void CDlgInfo::OnChkUseAoiUp()
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[30].GetCheck();
	if (bOn)
		pDoc->WorkingInfo.System.bUseAoiUp = TRUE;
	else
		pDoc->WorkingInfo.System.bUseAoiUp = FALSE;

	CString sData = pDoc->WorkingInfo.System.bUseAoiUp ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("System"), _T("UseAoiUp"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("UseAoiUp"), pDoc->WorkingInfo.System.bUseAoiUp);			// 상부AOI\r미사용
	pView->MpeWrite(pView->Plc.DlgInfo.UseAoiUp, pDoc->WorkingInfo.System.bUseAoiUp ? 1 : 0);	// 상부AOI\r미사용
	DispDevicePartial();
}

void CDlgInfo::OnChkUseAoiDn()
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[31].GetCheck();
	if (bOn)
		pDoc->WorkingInfo.System.bUseAoiDn = TRUE;
	else
		pDoc->WorkingInfo.System.bUseAoiDn = FALSE;

	CString sData = pDoc->WorkingInfo.System.bUseAoiDn ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("System"), _T("UseAoiDn"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("UseAoiDn"), pDoc->WorkingInfo.System.bUseAoiDn);			// 하부AOI\r미사용
	pView->MpeWrite(pView->Plc.DlgInfo.UseAoiDn, pDoc->WorkingInfo.System.bUseAoiDn ? 1 : 0);	// 하부AOI\r미사용
	DispDevicePartial();
}

void CDlgInfo::OnChkUsePunch()
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[32].GetCheck();
	if (bOn)
		pDoc->WorkingInfo.System.bUsePunch = TRUE;
	else
		pDoc->WorkingInfo.System.bUsePunch = FALSE;

	CString sData = pDoc->WorkingInfo.System.bUsePunch ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("System"), _T("UsePunch"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("UsePunch"), pDoc->WorkingInfo.System.bUsePunch);			// 펀칭부\r미사용
	pView->MpeWrite(pView->Plc.DlgInfo.UsePunch, pDoc->WorkingInfo.System.bUsePunch ? 1 : 0);	// 펀칭부\r미사용
	DispDevicePartial();
}

void CDlgInfo::OnChkUsePunchOnly()
{
	// TODO: Add your control notification handler code here
	BOOL bOn = myBtn[33].GetCheck();
	if (bOn)
	{
		pDoc->WorkingInfo.System.bUsePunchOnly = TRUE;
		ShowDlg(IDD_DLG_UTIL_08);
	}
	else
		pDoc->WorkingInfo.System.bUsePunchOnly = FALSE;

	CString sData = pDoc->WorkingInfo.System.bUsePunchOnly ? _T("1") : _T("0");
	::WritePrivateProfileString(_T("System"), _T("UsePunchOnly"), sData, PATH_WORKING_INFO);
	pDoc->SetMkInfo(_T("Signal"), _T("UsePunchOnly"), pDoc->WorkingInfo.System.bUsePunchOnly);			// 펀칭부만\r사용Off
	pView->MpeWrite(pView->Plc.DlgInfo.UsePunchOnly, pDoc->WorkingInfo.System.bUsePunchOnly ? 1 : 0);	// 펀칭부만\r사용Off
	DispDevicePartial();
}

void CDlgInfo::ShowDlg(int nID)
{
	switch (nID)
	{
	case IDD_DLG_UTIL_08:
		CDlgUtil08 dlg;
		dlg.DoModal();
		break;
	}
}
