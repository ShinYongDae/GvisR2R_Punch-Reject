// MySpec.cpp : implementation file
//

#include "stdafx.h"
#include "../GvisR2R_Punch.h"
#include "MySpec.h"

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
// CMySpec

CMySpec::CMySpec() // AOI ��� ����.
{
	m_sModel = pDoc->WorkingInfo.LastJob.sModel;
	m_sLayer = pDoc->WorkingInfo.LastJob.sLayerUp;
	m_sPath.Format(_T("%s\\%s\\%s\\LocalSpec.ini"), PATH_LOCAL_SPEC, m_sModel, m_sLayer);
	m_dPinPosX[0] = 0.0;
	m_dPinPosY[0] = 0.0;
	m_dPinPosX[1] = 0.0;
	m_dPinPosY[1] = 0.0;
}

CMySpec::CMySpec(CString sModel, CString sLayer)
{
	if(!sModel.IsEmpty() && !sLayer.IsEmpty())
	{
		m_sModel = sModel;
		m_sLayer = sLayer;
	}
	else
	{
		m_sModel = pDoc->WorkingInfo.LastJob.sModel;
		m_sLayer = pDoc->WorkingInfo.LastJob.sLayerUp;
	}
#ifdef TEST_MODE
	m_sPath.Format(_T("%s\\%s\\%s\\LocalSpec.ini"), PATH_LOCAL_SPEC, _T("TestModel"), _T("TestLayel"));
#else
	m_sPath.Format(_T("%s\\%s\\%s\\LocalSpec.ini"), PATH_LOCAL_SPEC, m_sModel, m_sLayer);
#endif
	m_dPinPosX[0] = 0.0;
	m_dPinPosY[0] = 0.0;
	m_dPinPosX[1] = 0.0;
	m_dPinPosY[1] = 0.0;
}

CMySpec::~CMySpec()
{
}


BEGIN_MESSAGE_MAP(CMySpec, CWnd)
	//{{AFX_MSG_MAP(CMySpec)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMySpec message handlers

BOOL CMySpec::MakeDir()
{
	CString sMsg=_T("");
	CFileFind finder;
	CString sPath;
#ifdef TEST_MODE
	if (!pDoc->DirectoryExists(PATH_LOCAL_SPEC))
		CreateDirectory(PATH_LOCAL_SPEC, NULL);

	if(m_sModel.IsEmpty() || m_sLayer.IsEmpty())
	{
		sMsg.Format(_T("���̳� ���̾���� �����ϴ�."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s\\%s"), PATH_LOCAL_SPEC, m_sModel);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s\\%s\\%s"), PATH_LOCAL_SPEC, m_sModel, m_sLayer);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
#else
	sPath.Format(_T("%s"), PATH_LOCAL_SPEC);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if(m_sModel.IsEmpty() || m_sLayer.IsEmpty())
	{
		sMsg.Format(_T("���̳� ���̾���� �����ϴ�."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s\\%s"), PATH_LOCAL_SPEC, m_sModel);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s\\%s\\%s"), PATH_LOCAL_SPEC, m_sModel, m_sLayer);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
#endif
	return TRUE;
}

BOOL CMySpec::MakeDir(CString sModel, CString sLayer)
{
	CString sMsg= _T("");
	CFileFind finder;
	CString sPath;
#ifdef TEST_MODE
	if (!pDoc->DirectoryExists(PATH_LOCAL_SPEC))
		CreateDirectory(PATH_LOCAL_SPEC, NULL);

	if(sModel.IsEmpty() || sLayer.IsEmpty())
	{
		sMsg.Format(_T("���̳� ���̾���� �����ϴ�."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s\\%s"), PATH_LOCAL_SPEC, sModel);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s\\%s\\%s"), PATH_LOCAL_SPEC, sModel, sLayer);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
#else
	sPath.Format(_T("%s"), PATH_LOCAL_SPEC);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	if(sModel.IsEmpty() || sLayer.IsEmpty())
	{
		sMsg.Format(_T("���̳� ���̾���� �����ϴ�."));
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}

	sPath.Format(_T("%s\\%s"), PATH_LOCAL_SPEC, sModel);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);

	sPath.Format(_T("%s\\%s\\%s"), PATH_LOCAL_SPEC, sModel, sLayer);
	if (!pDoc->DirectoryExists(sPath))
		CreateDirectory(sPath, NULL);
#endif
	return TRUE;
}

BOOL CMySpec::Load() 
{
	CString sMsg= _T("");
	CFileFind finder;
	if(!finder.FindFile(m_sPath))
	{
		sMsg.Format(_T("���� ������ ã�� ���߽��ϴ�.\r\n%s"), m_sPath);
		pView->ClrDispMsg();
		AfxMessageBox(sMsg);
		return FALSE;
	}

	TCHAR szData[MAX_PATH];
 
	if (0 < ::GetPrivateProfileString(_T("PIN0"), _T("X"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosX[0] = _tstof(szData);
	else
		m_dPinPosX[0] = 0.0;

	if (0 < ::GetPrivateProfileString(_T("PIN0"), _T("Y"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosY[0] = _tstof(szData);
	else
		m_dPinPosY[0] = 0.0;
 
	if (0 < ::GetPrivateProfileString(_T("PIN1"), _T("X"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosX[1] = _tstof(szData);
	else
		m_dPinPosX[1] = 0.0;

	if (0 < ::GetPrivateProfileString(_T("PIN1"), _T("Y"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosY[1] = _tstof(szData);
	else
		m_dPinPosY[1] = 0.0;

	return TRUE;
}

BOOL CMySpec::Save() 
{
	if(!MakeDir())
		return FALSE;

	if(!SavePinPos())
		return FALSE;

	return TRUE;
}

BOOL CMySpec::SavePinPos()
{
	if(!MakeDir())
		return FALSE;

	CString sData;

	sData.Format(_T("%.3f"), m_dPinPosX[0]);
	::WritePrivateProfileString(_T("PIN0"), _T("X"), sData, m_sPath);

	sData.Format(_T("%.3f"), m_dPinPosY[0]);
	::WritePrivateProfileString(_T("PIN0"), _T("Y"), sData, m_sPath);

	sData.Format(_T("%.3f"), m_dPinPosX[1]);
	::WritePrivateProfileString(_T("PIN1"), _T("X"), sData, m_sPath);

	sData.Format(_T("%.3f"), m_dPinPosY[1]);
	::WritePrivateProfileString(_T("PIN1"), _T("Y"), sData, m_sPath);

	return TRUE;
}

BOOL CMySpec::SavePinPos(int nCam, CfPoint fPt)
{
	m_dPinPosX[nCam] = fPt.x;
	m_dPinPosY[nCam] = fPt.y;

	if(!MakeDir())
		return FALSE;

	CString sData[2];

	sData[0].Format(_T("%.3f"), m_dPinPosX[nCam]);
	sData[1].Format(_T("%.3f"), m_dPinPosY[nCam]);
	if(nCam==0)
	{
		::WritePrivateProfileString(_T("PIN0"), _T("X"), sData[0], m_sPath);
		::WritePrivateProfileString(_T("PIN0"), _T("Y"), sData[1], m_sPath);
	}
	else if(nCam==1)
	{
		::WritePrivateProfileString(_T("PIN1"), _T("X"), sData[0], m_sPath);
		::WritePrivateProfileString(_T("PIN1"), _T("Y"), sData[1], m_sPath);
	}

	return TRUE;
}

BOOL CMySpec::IsPinMkData() 
{
	CString sMsg= _T("");
	CFileFind finder;
	if(!finder.FindFile(m_sPath))
		return FALSE;

	TCHAR szData[MAX_PATH];

	if (0 < ::GetPrivateProfileString(_T("PIN0"), _T("X"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosX[0] = _tstof(szData);
	else
	{
		m_dPinPosX[0] = 0.0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("PIN0"), _T("Y"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosY[0] = _tstof(szData);
	else
	{
		m_dPinPosY[0] = 0.0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("PIN1"), _T("X"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosX[1] = _tstof(szData);
	else
	{
		m_dPinPosX[1] = 0.0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("PIN1"), _T("Y"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosY[1] = _tstof(szData);
	else
	{
		m_dPinPosY[1] = 0.0;
		return FALSE;
	}

	return TRUE;
}

BOOL CMySpec::IsPinData() 
{
	CString sMsg= _T("");
	CFileFind finder;
	if(!finder.FindFile(m_sPath))
		return FALSE;

	TCHAR szData[MAX_PATH];

	if (0 < ::GetPrivateProfileString(_T("PIN0"), _T("X"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosX[0] = _tstof(szData);
	else
	{
		m_dPinPosX[0] = 0.0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("PIN0"), _T("Y"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosY[0] = _tstof(szData);
	else
	{
		m_dPinPosY[0] = 0.0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("PIN1"), _T("X"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosX[1] = _tstof(szData);
	else
	{
		m_dPinPosX[1] = 0.0;
		return FALSE;
	}

	if (0 < ::GetPrivateProfileString(_T("PIN1"), _T("Y"), NULL, szData, sizeof(szData), m_sPath))
		m_dPinPosY[1] = _tstof(szData);
	else
	{
		m_dPinPosY[1] = 0.0;
		return FALSE;
	}

	return TRUE;
}

