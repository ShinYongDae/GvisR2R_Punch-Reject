// GlobalDefine.h : interface of the CGvisR2RView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALDEFINE_H_INCLUDED_)
#define AFX_GLOBALDEFINE_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Device/MotionParam.h"


#define TEST_MODE		1

#ifndef MAX_STRIP
	#define MAX_STRIP				4
#endif

#define MSG_BOX_PATH				_T("C:\\R2RSet\\MsgBox.exe")

#define MAX_STRIP_NUM				4
#define MAX_PCE_COL					100
#define MAX_PCE_ROW					100

#ifdef TEST_MODE
	#define TEST_SHOT				2
	#define PATH_REELMAP			_T("C:\\R2RSet\\Test\\ReelMap.txt")
	//#define PATH_REELMAP			_T("C:\\R2RSet\\Test\\ReelMapData.txt")
	#define PATH_REELMAP_UP			PATH_REELMAP	//_T("C:\\R2RSet\\Test\\ReelMapDataUp.txt")
	#define PATH_REELMAP_DN			PATH_REELMAP	//_T("C:\\R2RSet\\Test\\ReelMapDataDn.txt")
	#define PATH_REELMAP_ALL_UP		PATH_REELMAP	//_T("C:\\R2RSet\\Test\\ReelMapDataAllUp.txt")
	#define PATH_REELMAP_ALL_DN		PATH_REELMAP	//_T("C:\\R2RSet\\Test\\ReelMapDataAllDn.txt")

	#define PATH_STRIP_RGN			_T("C:\\R2RSet\\Test\\Layer.mst")			// CAM Strip Region
	#define PATH_PCS_RGN			_T("C:\\R2RSet\\Test\\Region.Pcs")			// AOI PCS Region
	#define PATH_CELL_RGN			_T("C:\\R2RSet\\Test\\Cell.rgn")			// CAM PCS Region
	#define PATH_CELL_MST			_T("C:\\R2RSet\\Test\\1STRIP-2-050.mst")	// CAM Cell Img Region Data
	#define PATH_PCS_IMG_			_T("C:\\R2RSet\\Test\\Piece.tif")
	#define PATH_CAD_IMG			_T("C:\\R2RSet\\Test\\CORGO1.tif")
	#define PATH_DEF_IMG			_T("C:\\R2RSet\\Test\\")
	#define PATH_PCR				_T("C:\\R2RSet\\Test\\0004.pcr")
	#define PATH_PIN_IMG_			_T("C:\\R2RSet\\Test\\Pin-330.tif")

	#define PATH_PIN_IMG			_T("C:\\R2RSet\\Test\\Pin-330.TIF")
	#define PATH_ALIGN0_IMG			_T("C:\\R2RSet\\Test\\Align0.TIF")
	#define PATH_ALIGN1_IMG			_T("C:\\R2RSet\\Test\\Align1.TIF")
	#define PATH_ALIGN2_IMG			_T("C:\\R2RSet\\Test\\Align2.TIF")
	#define PATH_ALIGN3_IMG			_T("C:\\R2RSet\\Test\\Align3.TIF")
	#define PATH_PCS_IMG			_T("C:\\R2RSet\\Test\\Piece.tif")
	#define PATH_ORDERING_Mk		_T("C:\\R2RSet\\Test\\WriteOrderingMkRotate90ccw.txt")
	#define PATH_ORDERING_Mk_MIRROR	_T("C:\\R2RSet\\Test\\WriteOrderingMkRotate90ccwMirror.txt")
	#define PATH_REJECT_IMG_		_T("C:\\R2RSet\\Test\\Reject.TIF")

	//#define	USE_MIL
	//#define	USE_VISION

	//#define USE_TCPIP
	//#define USE_ENGRAVE
	//#define USE_DTS

	//#define USE_CAM_MASTER
	//#define	USE_NMC
	//#define	USE_MPE
	//#define	USE_IRAYPLE
	//#define	USE_LIGHT
	//#define	USE_SMAC

	//#define	USE_SR1000W

#else
	#define	USE_CAM_MASTER
	#define	USE_MIL
	#define	USE_VISION

	#define	USE_NMC
	#define	USE_MPE
	#define	USE_IRAYPLE
	#define	USE_TCPIP
	#define USE_ENGRAVE

	#define	USE_LIGHT
	#define	USE_SMAC
	#define USE_DTS


// 	#define	USE_SR1000W

//	#define	USE_SONY
//	#define	USE_IDS
//	#define USE_CREVIS

//	#define	USE_FLUCK
//	#define	USE_XMP

	#define PATH_PIN_IMG			_T("C:\\R2RSet\\Pin\\Pin.TIF")
	#define PATH_REJECT_IMG			_T("C:\\R2RSet\\Reject\\Reject.TIF")
	#define PATH_ALIGN0_IMG			_T("C:\\R2RSet\\Align\\Align0.TIF")
	#define PATH_ALIGN1_IMG			_T("C:\\R2RSet\\Align\\Align1.TIF")
	#define PATH_ALIGN2_IMG			_T("C:\\R2RSet\\Align\\Align2.TIF")
	#define PATH_ALIGN3_IMG			_T("C:\\R2RSet\\Align\\Align3.TIF")
	#define PATH_PCS_IMG			_T("C:\\R2RSet\\Pcs\\Piece.tif")

	#define PATH_PIN_IMG_			_T("C:\\R2RSet\\Test\\Pin-330.tif")
	#define PATH_REJECT_IMG_		_T("C:\\R2RSet\\Test\\Reject.TIF")
	#define PATH_CELL_RGN			_T("C:\\R2RSet\\Test\\Cell.rgn")			// CAM PCS Region
	#define PATH_CELL_MST			_T("C:\\R2RSet\\Test\\1STRIP-2-050.mst")	// CAM Cell Img Region Data
	#define PATH_PCS_IMG_			_T("C:\\R2RSet\\Test\\Piece.tif")
	#define PATH_CAD_IMG			_T("C:\\R2RSet\\Test\\CORGO1.tif")
	#define PATH_PCR				_T("C:\\R2RSet\\Test\\0004.pcr")

#endif


#define MAX_MS				6	
#define MAX_AXIS			4	
typedef enum {MS_X0=0, MS_Y0=1, MS_X1=2, MS_Y1=3, MS_X0Y0=4, MS_X1Y1=5} MS_ID; 
typedef enum {AXIS_X0=0, AXIS_Y0=1, AXIS_X1=2, AXIS_Y1=3} AXIS_ID; 
typedef enum {BTN_UP=1, BTN_DN=2} BTN_ACT; 
typedef enum {OFF=0, ON=1} SWITCH_ACT; 
typedef enum {TO_NONE=0x00, TO_LEFT=0x01, TO_TOP=0x02, TO_RIGHT=0x04, TO_BOTTOM=0x08} POS_DIR; 
typedef enum {M_CCW=-1, M_CW=1} MOTION_DIR; 
typedef enum {YEAR=0, MONTH=1, DAY=2, HOUR=3, MINUTE=4, SEC=5} TIME_PARAM;
typedef enum {UP=0, DN=1, ALL_UP=2, ALL_DN=3, ALL=4} SEL_MAP;
#define MAX_ENC_CNT			2147483648

// Language select
typedef enum {KOREAN=0, ENGLISH=1, JAPANESE=2} LANG; 

#define LANGUAGE	KOREAN

#define Pi		3.14159265358979

#define MAX_LIGHT_CH	4
#define MAX_VOICE_COIL	2

// Define Voice Coil ID & Cmd
#define VOICE_COIL_FIRST_CAM			0
#define VOICE_COIL_SECOND_CAM			1
#define FIRST_CAM_AND_SECOND_CAM		2

#define DELAY_TIME_MSG			2000
#define DELAY_TIME_ONE_MINUTE	6000

#define MB_TIME_OUT					-1
#define DEFAULT_TIME_OUT			120000		// [mSec]


#define ROT_CCW_90		-2
#define ROT_CCW_180		-1
#define ROT_NONE		0
#define ROT_CW_90		1
#define ROT_CW_180		2

#define PATH_MP2100_IO			_T("C:\\R2RSet\\IO.ini")
#define PATH_MP2100_SIGNAL		_T("C:\\R2RSet\\Signal.ini")
#define PATH_MP2100_DATA		_T("C:\\R2RSet\\Data.ini")
// #define PATH_MP2100_IO			_T("C:\\R2RSet\\Mp2100IoList.ini")
// #define PATH_SLICE_IO			_T("C:\\R2RSet\\SliceIoList.ini")
#define PATH_MOTION_PARAM		_T("C:\\R2RSet\\MotionParam.ini")
#define PATH_CONFIG				_T("C:\\R2RSet\\Config.ini")
#define PATH_WORKING_INFO		_T("C:\\R2RSet\\WorkingInfo.ini")
#define PATH_SMAC				_T("C:\\R2RSet\\SMAC.ini")
#define PATH_ALARM				_T("C:\\R2RSet\\Alarm.ini")
#define PATH_GUI_REGISTER		_T("C:\\R2RSet\\GuiRegister.ini")

#define RGB_BLACK       RGB(0,0,0)
#define RGB_WHITE       RGB(255,255,255)
#define RGB_GRAY        RGB(0x80, 0x80, 0x80)
#define RGB_RED         RGB(255,0,0)
#define RGB_YELLOW      RGB(255,255,0)
#define RGB_DARKYELLOW  RGB(128,128,0)
#define RGB_GREEN       RGB(0,255,0)
#define RGB_LTGREEN     RGB(0,255,128)
#define RGB_DARKGREEN   RGB(64,128,128)
#define RGB_CYAN        RGB(0,255,255)
#define RGB_BLUE        RGB(0,0,255)
#define RGB_LTBLUE		RGB(82, 200, 200)
#define RGB_CLOUDBLUE	RGB(128,184,223)
#define RGB_MAGENTA     RGB(255,0,255)
#define RGB_LTMAGENTA   RGB(255,64,255)
#define RGB_DARKMAGENTA RGB(193,0,130)
#define RGB_LTGRAY      RGB(192,192,192)
#define RGB_DARKGRAY    RGB(128,128,128)
#define RGB_DARKCYAN    RGB(0,128,128)
#define RGB_LTCYAN      RGB(128,255,255)
#define RGB_BOON        RGB(255,0,128)
#define RGB_DARKBOON    RGB(64,0,128)
#define RGB_DARKBLUE    RGB(0,0,128)
#define RGB_PINK        RGB(255,0,128)
#define RGB_LTPINK      RGB(255,64,128)
#define RGB_SKYBLUE		RGB(128,255,255)
#define RGB_PURPLE		RGB(128,0,255)
#define RGB_LTPURPLE	RGB(226,233,251)
#define RGB_LTPURPLE2   RGB(255,128,255)
#define RGB_YELLOWGREEN RGB(128,255,0)
#define RGB_LTYELLOW    RGB(255,255,128)
#define RGB_BROWN       RGB(255,128,0)
#define RGB_CHESTNUT	RGB(128,64,64)
#define RGB_NAVY		RGB(0,0,128)
#define RGB_LLTGREEN    RGB(128,255,128)
#define RGB_LTRED       RGB(255,128,64)
#define RGB_DARKFINGER	RGB(128,0,128)
#define RGB_LAVENDER	RGB(0xe6,0xe6,0xfa)
#define RGB_PLURM		RGB(0xdd,0xa0,0xdd)

#define MRGB_BLACK       M_RGB888(0,0,0)
#define MRGB_WHITE       M_RGB888(255,255,255)
#define MRGB_GRAY        M_RGB888(0x80, 0x80, 0x80)
#define MRGB_RED         M_RGB888(255,0,0)
#define MRGB_YELLOW      M_RGB888(255,255,0)
#define MRGB_DARKYELLOW  M_RGB888(128,128,0)
#define MRGB_GREEN       M_RGB888(0,255,0)
#define MRGB_LTGREEN     M_RGB888(0,255,128)
#define MRGB_DARKGREEN   M_RGB888(0,64,64)
#define MRGB_CYAN        M_RGB888(0,255,255)
#define MRGB_BLUE        M_RGB888(0,0,255)
#define MRGB_MAGENTA     M_RGB888(255,0,255)
#define MRGB_DARKMAGENTA M_RGB888(193,0,130)
#define MRGB_LTGRAY      M_RGB888(192,192,192)
#define MRGB_DARKGRAY    M_RGB888(128,128,128)
#define MRGB_DARKCYAN    M_RGB888(0,128,128)
#define MRGB_LTCYAN      M_RGB888(128,255,255)
#define MRGB_BOON        M_RGB888(255,0,128)
#define MRGB_DARKBOON    M_RGB888(64,0,128)
#define MRGB_DARKBLUE    M_RGB888(0,0,128)
#define MRGB_DARKFINGER	 M_RGB888(128,0,128)

#define RGB_BLACK       RGB(0,0,0)
#define RGB_WHITE       RGB(255,255,255)
#define RGB_GRAY        RGB(128, 128, 128)
#define RGB_RED         RGB(255,0,0)
#define RGB_DARKRED     RGB(207,0,0)
#define RGB_YELLOW      RGB(255,255,0)
#define RGB_DARKYELLOW  RGB(128,128,0)
#define RGB_GREEN       RGB(0,255,0)
#define RGB_LTGREEN     RGB(0,255,128)
#define RGB_CYAN        RGB(0,255,255)
#define RGB_BLUE        RGB(0,0,255)
#define RGB_LTBLUE		RGB(82, 200, 200)
#define RGB_CLOUDBLUE	RGB(128,184,223)
#define RGB_MAGENTA     RGB(255,0,255)
#define RGB_DARKMAGENTA RGB(193,0,130)
#define RGB_WHGRAY      RGB(218,218,218)
#define RGB_LTGRAY      RGB(192,192,192)
#define RGB_DARKCYAN    RGB(0,128,128)
#define RGB_LTCYAN      RGB(128,255,255)
#define RGB_BOON        RGB(255,0,128)
#define RGB_DARKBOON    RGB(64,0,128)
#define RGB_DARKBLUE    RGB(0,0,128)
#define RGB_PINK        RGB(255,0,128)
#define RGB_DARKPINK	RGB(227,64,128)
#define RGB_SKYBLUE		RGB(128,255,255)
#define RGB_DKSKYBLUE	RGB(82,200,200)
#define RGB_PURPLE		RGB(128,0,255)
#define RGB_YELLOWGREEN RGB(128,255,0)
#define RGB_WHYELLOW    RGB(255,255,128)
#define RGB_ORANGE      RGB(255,128,0)
#define RGB_LTDKORANGE  RGB(255,172,89)
#define RGB_DARKORANGE  RGB(220,70,0)
#define RGB_CHESTNUT	RGB(128,64,64)
#define RGB_NAVY		RGB(0,0,128)
#define RGB_LLTGREEN    RGB(128,255,128)
#define RGB_LT_ORANGE   RGB(255,128,64)
#define RGB_DARKFINGER	RGB(128,0,128)
#define RGB_DLG_FRM		RGB(212,208,200)
#define RGB_DLG_FRM2	RGB(240,240,240)
#define RGB_DARKBROWN	RGB(64,0,0)
#define RGB_DIALOG		RGB(200,200,192)
#define RGB_POSTIT		RGB(240,233,134)
#define RGB_DARK_BLUE	RGB(68,136,204)
#define RGB_FRAME		RGB(212,208,200)

#define RGB_DARK_GREEN		RGB(0,125,60)
#define RGB_LT_PURPLE		RGB(179,179,255)
#define RGB_LT_YELLOW		RGB(255,255,150)
#define RGB_LT_PINK			RGB(255,200,200)
#define RGB_LT_GREEN		RGB(195,255,195)
#define RGB_LT_BLUE			RGB(210,255,255)
#define RGB_WH_ORANGE		RGB(255,220,190)
#define RGB_PURPLE			RGB(147,112,216)
#define RGB_DARKPURPLE		RGB(64,0,64)
#define RGB_LT_DARKMAGENTA	RGB(204,50,153)
#define RGB_MUSTARD			RGB(217,217,25)
#define RGB_SKY				RGB(173, 234, 234)
#define RGB_LT_BROWN		RGB(209, 146, 117)
#define RGB_PEACOCK_GREEN	RGB(100, 186, 146)
#define RGB_FOREST_GREEN	RGB(85, 158, 124)
#define RGB_EVER_GREEN		RGB(74, 118, 98)
#define	RGB_PCS_OUT			RGB_LTGRAY


#define PATH_LOCAL_SPEC					_T("C:\\R2RSet\\LocalSpec")


#define MSG_KEYPAD_UP					_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Key_Up.bmp")
#define MSG_BTN_BK_DN					_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Btn_Bk_Dn.bmp")
#define MSG_BTN_BK_UP					_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Btn_Bk_Up.bmp")
#define ICO_BTN_CANCEL					_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Cancel3.ico")
#define ICO_BTN_EXIT					_T("C:\\R2RSet\\Pic\\DlgMyMsg\\EXIT2.ico")

#define MSG_SUB00_KEYPAD_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub00\\Key_Up.bmp")
#define MSG_SUB00_BTN_BK_DN				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub00\\Btn_Bk_Dn.bmp")
#define MSG_SUB00_BTN_BK_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub00\\Btn_Bk_Up.bmp")
#define ICO_SUB00_BTN_CANCEL			_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub00\\Cancel3.ico")
#define ICO_SUB00_BTN_EXIT				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub00\\EXIT2.ico")

#define MSG_SUB01_KEYPAD_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub01\\Key_Up.bmp")
#define MSG_SUB01_BTN_BK_DN				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub01\\Btn_Bk_Dn.bmp")
#define MSG_SUB01_BTN_BK_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub01\\Btn_Bk_Up.bmp")
#define ICO_SUB01_BTN_CANCEL			_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub01\\Cancel3.ico")
#define ICO_SUB01_BTN_EXIT				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub01\\EXIT2.ico")

#define MSG_SUB02_KEYPAD_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub02\\Key_Up.bmp")
#define MSG_SUB02_BTN_BK_DN				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub02\\Btn_Bk_Dn.bmp")
#define MSG_SUB02_BTN_BK_UP				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub02\\Btn_Bk_Up.bmp")
#define ICO_SUB02_BTN_CANCEL			_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub02\\Cancel3.ico")
#define ICO_SUB02_BTN_EXIT				_T("C:\\R2RSet\\Pic\\DlgMyMsg\\Sub02\\EXIT2.ico")

#define ICO_CHK_DlgFrameHigh			_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\button_ok.ico")
#define IMG_BK_DlgFrameHigh				_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\bk.bmp")
#define IMG_BTN_DN_DlgFrameHigh			_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgFrameHigh			_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\Btn_Bk_Up.bmp")
#define ICO_LED_BLU_DlgFrameHigh		_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\3dBallBlue.ico")
#define ICO_LED_RED_DlgFrameHigh		_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\3dBallRed.ico")
#define ICO_LED_GRY_DlgFrameHigh		_T("C:\\R2RSet\\Pic\\DlgFrameHigh\\3DBallGray.ico")

#define IMG_BTN_DN_DlgMenu01			_T("C:\\R2RSet\\Pic\\DlgMenu01\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgMenu01			_T("C:\\R2RSet\\Pic\\DlgMenu01\\Btn_Bk_Up.bmp")
#define NI_BTN_DN_DlgMenu01				_T("C:\\R2RSet\\Pic\\DlgMenu01\\Ni_Bk_Dn2.bmp")
#define NI_BTN_UP_DlgMenu01				_T("C:\\R2RSet\\Pic\\DlgMenu01\\Ni_Bk_Up1.bmp")
#define ICO_LED_BLU_DlgMenu01			_T("C:\\R2RSet\\Pic\\DlgMenu01\\3dBallBlue.ico")
#define ICO_LED_RED_DlgMenu01			_T("C:\\R2RSet\\Pic\\DlgMenu01\\3dBallRed.ico")
#define ICO_LED_GRY_DlgMenu01			_T("C:\\R2RSet\\Pic\\DlgMenu01\\3DBallGray.ico")

#define IMG_BTN_DN_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\Btn_Bk_Up.bmp")
#define IMG_JOG_DN_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\arrow_dn_blue.bmp")
#define IMG_JOG_UP_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\arrow_up_blue.bmp")
#define IMG_JOG_LF_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\arrow_lf_blue.BMP")
#define IMG_JOG_RT_DlgMenu02			_T("C:\\R2RSet\\Pic\\DlgMenu02\\arrow_rt_blue.BMP")
#define NI_BTN_DN_DlgMenu02				_T("C:\\R2RSet\\Pic\\DlgMenu02\\Ni_Bk_Dn.bmp")
#define NI_BTN_UP_DlgMenu02				_T("C:\\R2RSet\\Pic\\DlgMenu02\\Ni_Bk_Up.bmp")

#define IMG_CIR_BLU_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_BLU_OFF.bmp")
#define IMG_CIR_BLU_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_BLU_ON.bmp")
#define IMG_CIR_BLUE_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_BLUE_OFF.bmp")
#define IMG_CIR_BLUE_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_BLUE_ON.bmp")
#define IMG_CIR_DGN_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_DGN_OFF.bmp")
#define IMG_CIR_DGN_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_DGN_ON.bmp")
#define IMG_CIR_GRN_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_GRN_OFF.bmp")
#define IMG_CIR_GRN_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_GRN_ON.bmp")
#define IMG_CIR_GRY_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_GRY_OFF.bmp")
#define IMG_CIR_GRY_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_GRY_ON.bmp")
#define IMG_CIR_ORG_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_ORG_OFF.bmp")
#define IMG_CIR_ORG_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_ORG_ON.bmp")
#define IMG_CIR_PNK_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_PNK_OFF.bmp")
#define IMG_CIR_PNK_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_PNK_ON.bmp")
#define IMG_CIR_RED_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_RED_OFF.bmp")
#define IMG_CIR_RED_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_RED_ON.bmp")
#define IMG_CIR_YLO_OFF_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_YLO_OFF.bmp")
#define IMG_CIR_YLO_ON_DlgMenu03		_T("C:\\R2RSet\\Pic\\DlgMenu03\\BTN_CIR_YLO_ON.bmp")

#define IMG_BTN_DN_DlgMenu04			_T("C:\\R2RSet\\Pic\\DlgMenu04\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgMenu04			_T("C:\\R2RSet\\Pic\\DlgMenu04\\Btn_Bk_Up.bmp")
#define IMG_JOG_UP_DlgMenu04			_T("C:\\R2RSet\\Pic\\DlgMenu04\\arrow_up_blue.bmp")
#define IMG_JOG_DN_DlgMenu04			_T("C:\\R2RSet\\Pic\\DlgMenu04\\arrow_dn_blue.bmp")
#define IMG_CIR_GRN_OFF_DlgMenu04		_T("C:\\R2RSet\\Pic\\DlgMenu04\\BTN_CIR_GRN_OFF.bmp")
#define IMG_CIR_GRN_ON_DlgMenu04		_T("C:\\R2RSet\\Pic\\DlgMenu04\\BTN_CIR_GRN_ON.bmp")

#define IMG_BTN_DN_DlgMenu06			_T("C:\\R2RSet\\Pic\\DlgMenu06\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgMenu06			_T("C:\\R2RSet\\Pic\\DlgMenu06\\Btn_Bk_Up.bmp")
#define NI_BTN_DN_DlgMenu06				_T("C:\\R2RSet\\Pic\\DlgMenu06\\Ni_Bk_Dn2.bmp")
#define NI_BTN_UP_DlgMenu06				_T("C:\\R2RSet\\Pic\\DlgMenu06\\Ni_Bk_Up1.bmp")

#define IMG_BTN_DN_DlgMyPassword		_T("C:\\R2RSet\\Pic\\DlgMyPassword\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgMyPassword		_T("C:\\R2RSet\\Pic\\DlgMyPassword\\Btn_Bk_Up.bmp")
#define IMG_BK_LT_YELLOW				_T("C:\\R2RSet\\Pic\\DlgMyPassword\\bk_light_yellow.bmp")
#define IMG_BK_LT_RED					_T("C:\\R2RSet\\Pic\\DlgMyPassword\\bk_light_red.bmp")

#define IMG_BTN_DN_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\Btn_Bk_Up.bmp")
#define IMG_JOG_DN_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\arrow_dn_blue.bmp")
#define IMG_JOG_UP_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\arrow_up_blue.bmp")
#define IMG_JOG_LF_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\arrow_lf_blue.BMP")
#define IMG_JOG_RT_DlgUtil02			_T("C:\\R2RSet\\Pic\\DlgUtil02\\arrow_rt_blue.BMP")

#define IMG_BTN_DN_DlgUtil03			_T("C:\\R2RSet\\Pic\\DlgUtil03\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgUtil03			_T("C:\\R2RSet\\Pic\\DlgUtil03\\Btn_Bk_Up.bmp")

#define IMG_BTN_DN_DlgUtil04			_T("C:\\R2RSet\\Pic\\DlgUtil04\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgUtil04			_T("C:\\R2RSet\\Pic\\DlgUtil04\\Btn_Bk_Up.bmp")
#define ICO_LED_GRY_DlgUtil04			_T("C:\\R2RSet\\Pic\\DlgUtil04\\3DBallGray.ico")
#define ICO_LED_BLU_DlgUtil04			_T("C:\\R2RSet\\Pic\\DlgUtil04\\3dBallBlue.ico")
#define ICO_LED_RED_DlgUtil04			_T("C:\\R2RSet\\Pic\\DlgUtil04\\3DBallRed.ico")

#define IMG_BTN_DN_DlgUtil05			_T("C:\\R2RSet\\Pic\\DlgUtil05\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgUtil05			_T("C:\\R2RSet\\Pic\\DlgUtil05\\Btn_Bk_Up.bmp")
#define ICO_LED_GRY_DlgUtil05			_T("C:\\R2RSet\\Pic\\DlgUtil05\\3DBallGray.ico")
#define ICO_LED_RED_DlgUtil05			_T("C:\\R2RSet\\Pic\\DlgUtil05\\3DBallRed.ico")

#define IMG_BTN_DN_DlgInfo				_T("C:\\R2RSet\\Pic\\DlgInfo\\Btn_Bk_Dn.bmp")
#define IMG_BTN_UP_DlgInfo				_T("C:\\R2RSet\\Pic\\DlgInfo\\Btn_Bk_Up.bmp")

#define IMG_BK_BLACK					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\bk_black.bmp")
#define IMG_KEYPAD_BK					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\background2.bmp")
#define IMG_KEYPAD_NUM0					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_0_Up.bmp")
#define IMG_KEYPAD_NUM1					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_1_Up.bmp")
#define IMG_KEYPAD_NUM2					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_2_Up.bmp")
#define IMG_KEYPAD_NUM3					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_3_Up.bmp")
#define IMG_KEYPAD_NUM4					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_4_Up.bmp")
#define IMG_KEYPAD_NUM5					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_5_Up.bmp")
#define IMG_KEYPAD_NUM6					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_6_Up.bmp")
#define IMG_KEYPAD_NUM7					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_7_Up.bmp")
#define IMG_KEYPAD_NUM8					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_8_Up.bmp")
#define IMG_KEYPAD_NUM9					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Num_9_Up.bmp")

#define IMG_KEYPAD_DEL					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Del_Up.bmp")
#define IMG_KEYPAD_DOT					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Dot_Up.bmp")
#define IMG_KEYPAD_ENTER				_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Enter_Up.bmp")
#define IMG_KEYPAD_DN					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Dn.bmp")
#define IMG_KEYPAD_UP					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Up.bmp")
#define IMG_KEYPAD_ESC					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Esc_Up.bmp")
#define IMG_KEYPAD_LEFT					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Left_Up1.bmp")
#define IMG_KEYPAD_RIGHT				_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Right_Up1.bmp")
#define IMG_KEYPAD_MINUS				_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Minus_Up0.bmp")
#define IMG_KEYPAD_BS					_T("C:\\R2RSet\\Pic\\DlgKeyNum\\Key_Bs_Up.bmp")

#define IMG_BTN_UP_GRAY_L				_T("C:\\R2RSet\\Pic\\BtnImg\\UpGrayL.bmp")
#define IMG_BTN_UP_GRAY_S				_T("C:\\R2RSet\\Pic\\BtnImg\\UpGrayS.bmp")
#define IMG_BTN_UP_RED_L				_T("C:\\R2RSet\\Pic\\BtnImg\\UpRedL.bmp")
#define IMG_BTN_UP_RED_S				_T("C:\\R2RSet\\Pic\\BtnImg\\UpRedS.bmp")
#define IMG_BTN_UP_PURPLE_L				_T("C:\\R2RSet\\Pic\\BtnImg\\UpPurpleL.bmp")
#define IMG_BTN_UP_PURPLE_S				_T("C:\\R2RSet\\Pic\\BtnImg\\UpPurpleS.bmp")

#define IMG_BTN_DN_RED_L				_T("C:\\R2RSet\\Pic\\BtnImg\\DnRedL.bmp")
#define IMG_BTN_DN_RED_S				_T("C:\\R2RSet\\Pic\\BtnImg\\DnRedS.bmp")
#define IMG_BTN_DN_PURPLE_L				_T("C:\\R2RSet\\Pic\\BtnImg\\DnPurpleL.bmp")
#define IMG_BTN_DN_PURPLE_S				_T("C:\\R2RSet\\Pic\\BtnImg\\DnPurpleS.bmp")
#define IMG_BTN_DN_BLUE_L				_T("C:\\R2RSet\\Pic\\BtnImg\\DnBlueL.bmp")
#define IMG_BTN_DN_BLUE_S				_T("C:\\R2RSet\\Pic\\BtnImg\\DnBlueS.bmp")

#define WM_MYBTN_DOWN					(WM_USER+1)
#define WM_MYBTN_UP						(WM_USER+2)
#define WM_MYEDIT_VSCROLL				(WM_USER+3)
#define WM_MYNETWORK_END				(WM_USER+4)
#define WM_MYGROUP_REDRAW				(WM_USER+5)
#define WM_REFRESH						(WM_USER+6)
#define WM_MY_KEYPADNUM					(WM_USER+7)
#define WM_MYSTATIC_REDRAW				(WM_USER+8)
#define WM_MYSTATIC_MOUSE_MOVE			(WM_USER+9)

#define WM_DLG_INFO						(WM_USER+10)
#define WM_MY_IO						(WM_USER+11)
#define WM_DRAW_REELMAP					(WM_USER+12)
#define WM_DRAW_PCSIMG					(WM_USER+13)
#define WM_DRAW_PINIMG					(WM_USER+14)
#define WM_RECEIVE_RS232				(WM_USER+15)

#define WM_USER_INC_PROGRESS			(WM_USER +20)
#define WM_USER_TASK_COMPLETED			(WM_USER +21)
#define WM_USER_TASK_THREAD_COMPLETED	(WM_USER +22)

#define WM_MYMSG						(WM_USER+30)
#define WM_MYMSG00						(WM_USER+31)
#define WM_MYMSG01						(WM_USER+32)
#define WM_MYMSG02						(WM_USER+33)
#define WM_MYMSG_EXIT					(WM_USER+34)
//#define WM_MYMSG_RTN					(WM_USER+35)

#define WM_BUF_THREAD_DONE				(WM_USER+40)
#define WM_MY_PW						(WM_USER+41)

#define WM_DRAW_REELMAP_INNER			(WM_USER+50)
#define WM_DRAW_REELMAP_OUTER			(WM_USER+51)

#define WM_CLIENT_RECEIVED				(WM_USER + 100)
#define WM_CLIENT_CLOSED				(WM_USER + 101)
#define WM_CLIENT_RECEIVED_MDX			(WM_USER + 102)
#define WM_CLIENT_RECEIVED_SR			(WM_USER + 103)

#define WM_SERVER_RECEIVED				(WM_USER + 110)
#define WM_SERVER_CLOSED				(WM_USER + 111)

#define WM_MYBTN_DBLCLK					(WM_USER + 120)


// Flow control flags

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ascii definitions

#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

#define DEF_UPPER					19

#define MAX_DEF						30
#define DEF_NONE					0		// NONE 	
#define DEF_NICK					1		// Nick 	
#define DEF_PROTRUSION				2		// Excessive line width
#define DEF_SPACE					3		// Minimum space violation
#define DEF_OPEN					4		// Pattern open
#define DEF_SHORT					5		// Pattern short
#define DEF_USHORT					6		// Micro short
#define DEF_PINHOLE					7		// Void, pinhole
#define DEF_HOLE_MISS				8		// Missing Hole
#define DEF_EXTRA					9		// Island
#define DEF_PAD						10		// Precision Pattern Error
#define DEF_HOLE_POSITION			11      // Location Hole 
#define DEF_POI						12		// Point of interest (Virtual defect by operator specfied)
#define DEF_VH_POSITION			    13		// Via hole positionning
#define DEF_VH_MISS					14		// Via hole missing
#define DEF_HOLE_DEFECT		    	15		// Hole burr..
#define DEF_HOLE_OPEN				16		// Hole open
#define DEF_VH_OPEN					17	    // VH open
#define DEF_VH_DEF					18	    // Reserved
#define DEF_EDGE_NICK				19		// Edge Nick
#define DEF_EDGE_PROT				20	    // Edge Protrusion
#define DEF_EDGE_SPACE				21	    // Edge Space
#define DEF_USER_DEFINE_1			22	    // User Define 1
#define DEF_NARROW					23	    // User Define 2
#define DEF_WIDE					24	    // User Define 3
#define DEF_FIXED_DEF				25	    // User Define 3
#define DEF_VH_SIZE					26	    // User Define 3
#define DEF_VH_EDGE					27	    // User Define 3
#define DEF_LIGHT					28	
#define DEF_INNER					29	

enum SAPP3_CODE{	SAPP3_OPEN = 0,
//					SAPP3_SHORT_USHORT = 1,
					SAPP3_SHORT = 1,
					SAPP3_NICK = 2,
					SAPP3_SPACE_EXTRA_PROTRUSION = 3,
//					SAPP3_PINHOLE_PAD = 4,
					SAPP3_PINHOLE = 4,
					SAPP3_HOPEN = 5,
					SAPP3_HMISS_HPOS_HBAD = 6,
					SAPP3_USHORT = 7,
					SAPP3_PAD = 8,
					SAPP3_VHOPEN_NOVH_VHALIGN_VHDEF = 9
				};


#define DLY_INK_MK			300

#define  MAX_PROCNODENUM			60    // Max ProcNodeNum : 600mm/2.5um = 48 
#define  MAX_MOTOR_SWATH_NUM		35    // MAX Motor swath num : 500mm/(8k*2.5um) = 25  
#define  MAX_CELL_NUM				(MAX_PROCNODENUM * MAX_MOTOR_SWATH_NUM) * 3    // 4500 ea, 3IPU


#define PATH_ERROR			-1
#define PATH_NOT_FOUND		0
#define PATH_IS_FILE		1
#define PATH_IS_FOLDER		2

#ifndef ID_NONE
#define ID_NONE			-1
#endif

#ifndef ID_SR1000W
#define ID_SR1000W		0
#endif

#ifndef ID_MDX2500
#define ID_MDX2500		1
#endif

#ifndef ID_PUNCH
#define ID_PUNCH		2
#endif

#ifndef ID_ENGRAVE
#define ID_ENGRAVE		2
#endif

typedef struct 
{
	int iStartX, iStartY;
	int iEndX, iEndY;
	int FMirror;	//0 : Defult 1 : Up to Down Mirroring  2 : Left to Right Mirroring
	int FRotate;	//0 : 0  1 : 90  2 : 180  3 : 270 [Degree]
} REGIONS_PIECE;

typedef struct {
	int nId;
	int iStartX, iStartY;
	int iEndX, iEndY;
	int FMirror;	//0 : 원본 1 : 상하미러  2 : 좌퓖E肩?
	int FRotate;	//0 : 0도  1 : 90도  2 : 180도  3 : 270도
	int Row;
	int Col;
} REGIONS_PIECE_2;


#define MYGL_GAP_PNL				5
#define MAX_DISP_PNL				6
#define MAX_PCR_PNL					500	//24

#define MENU01_STC_DEFINFO_HARF		6
#define MAX_PCR						4

#define RMAP_PCS_SCALE				0.85
#define TOT_M_IO					30

#define BUF_SZ						50

#define PNL_TOT						2500
#define PNLBUF_Y					50
#define PNLBUF_X					50

enum SEL_RMAP {
	RMAP_NONE = -1, RMAP_UP = 0, RMAP_DN = 1, RMAP_ALLUP = 2, RMAP_ALLDN = 3, 
	RMAP_INNER_UP = 4, RMAP_INNER_DN = 5, RMAP_INNER_ALLUP = 6, RMAP_INNER_ALLDN = 7,
	RMAP_INOUTER_UP = 8, RMAP_INOUTER_DN = 9, RMAP_INNER = 10, RMAP_ITS = 11
};

enum SEL_CAM { CAM_LF=0, CAM_RT=1, CAM_BOTH=2 };
enum MAIN_BTN { MN_RST=0, MN_RDY=1, MN_STOP=2, MN_RUN=3 };

enum IMG_KIND { CAD_IMG=0, DEF_IMG=1 };
enum ALIGN_METHODE { ONE_POINT = 1, TWO_POINT, THREE_POINT, FOUR_POINT };

enum WORK_MODE { MODE_NONE = 0, MODE_INNER = 1, MODE_OUTER = 2, MODE_MIDDLE = 3 };


struct stSystem
{
	CString sMcName;
	CString sPathCamSpecDir;
	CString sPathTorqSpecDir;

	CString sPathAoiUp, sPathAoiUpDts, sPathAoiUpCurrInfo, sPathAoiUpVrsData, sPathAoiUpOffset, sPathAoiUpDefImg;
	CString sPathAoiUpLocalSpec, sPathAoiUpCamInfo;
	CString sPathVrsShareUp, sPathVrsBufUp;
	CString sPathVsShareUp, sPathVsDummyBufUp;
	CString sPathAoiDn, sPathAoiDnDts, sPathAoiDnCurrInfo, sPathAoiDnVrsData, sPathAoiDnOffset, sPathAoiDnDefImg;
	CString sPathAoiDnLocalSpec, sPathAoiDnCamInfo;
	CString sPathVrsShareDn, sPathVrsBufDn;
	CString sPathVsShareDn, sPathVsDummyBufDn;

	CString sPathEng, sPathEngCurrInfo, sPathEngOffset, sPathMkCurrInfo, sPathMkCurrInfoBuf;
	CString sPathMkSignalInfo, sPathEngSignalInfo, sPathAoiUpStatusInfo, sPathAoiDnStatusInfo;
	CString sPathMkMenu01, sPathMkMenu03, sPathMkInfo, sPathMonDispMain;
	CString sPathMkWork, sPathMkStatus;

	CString sPathOldFile, sPathItsFile, sPathItsInner, sPathItsOuter; //sPathIts, 
	CString sIpPathOldFile, sIpPathItsFile, sIpPathIts;
	CString sPathSapp3;
	BOOL bSaveLog, bSaveReelmapTable, bRemakeReelmapInner, bDuplicateRmap, bInsertPunchingToDts, bDebugEngSig;
	BOOL bNoMk;	// 0 : 마킹모드, 1 : 비젼모드
	CString sReViewMkLen;
	BOOL bReViewMk;
	BOOL bVerifyPunching;	// 0 : 마킹여부 확인 않함, 1 : 마킹여부 확인

	CString sMaxDispDefImg;
	CString sCamSn[2];

	CString sIpClient[3];	// ID_SR1000W, ID_MDX2500, ID_PUNCH
	CString sIpServer[3];	// ID_SR1000W, ID_MDX2500, ID_ENGRAVE
	CString sPort[3];		// ID_SR1000W, ID_MDX2500, ID_ENGRAVE(ID_PUNCH)
	BOOL bSaveMkImg, bSaveGrabImg;
	BOOL bStripPcsRgnBin;
	BOOL bUseDTS, bUseITS;
	BOOL bUseDevicePartial;
	BOOL bUseEngrave, bUseAoiUp, bUseAoiDn, bUsePunch;
	BOOL bUsePunchOnly;

	stSystem()
	{
		sMcName = _T("");
		sPathCamSpecDir = _T("");
		sPathTorqSpecDir = _T("");

		sPathAoiUp = _T(""); sPathAoiUpDts = _T(""); sPathAoiUpCurrInfo = _T(""); sPathAoiUpDefImg = _T("");
		sPathAoiUpOffset = _T(""); sPathAoiUpVrsData = _T(""); sPathAoiUpCamInfo = _T("");
		sPathAoiUpLocalSpec = _T("");
		sPathVrsShareUp = _T(""); sPathVrsBufUp = _T("");
		sPathVsShareUp = _T(""); sPathVsDummyBufUp = _T("");
		sPathAoiDn = _T(""); sPathAoiDnDts = _T(""); sPathAoiDnCurrInfo = _T(""); sPathAoiDnDefImg = _T("");
		sPathAoiDnOffset = _T(""); sPathAoiDnVrsData = _T(""); sPathAoiDnCamInfo = _T("");
		sPathAoiDnLocalSpec = _T("");
		sPathVrsShareDn = _T(""); sPathVrsBufDn = _T("");
		sPathVsShareDn = _T(""); sPathVsDummyBufDn = _T("");

		sPathEng = _T(""); sPathEngCurrInfo = _T(""); sPathEngOffset = _T(""); sPathMkCurrInfo = _T("");
		sPathMkSignalInfo = _T(""); sPathEngSignalInfo = _T("");
		sPathAoiUpStatusInfo = _T(""); sPathAoiDnStatusInfo = _T("");
		sPathMkCurrInfoBuf = _T(""); sPathMkMenu01 = _T(""); sPathMkMenu03 = _T(""); sPathMkInfo = _T(""); sPathMonDispMain = _T("");
		sPathMkWork = _T(""); sPathMkStatus = _T("");

		sPathOldFile = _T("");
		sPathItsFile = _T("");
		//sPathIts = _T("");
		sPathItsInner = _T("");
		sPathItsOuter = _T("");
		sIpPathOldFile = _T("");
		sIpPathItsFile = _T("");
		sIpPathIts = _T("");
		bSaveLog = FALSE;
		bSaveReelmapTable = FALSE;
		bRemakeReelmapInner = FALSE;
		bDuplicateRmap = FALSE;
		bInsertPunchingToDts = FALSE;
		bDebugEngSig = FALSE;
		bNoMk = FALSE;	// 0 : 마킹모드, 1 : 비젼모드
		sReViewMkLen = _T("");
		bReViewMk = FALSE;
		sPathSapp3 = _T("");
		bVerifyPunching = FALSE;

		sCamSn[0] = _T(""); sCamSn[1] = _T("");
		sMaxDispDefImg = _T("");

		sIpClient[ID_SR1000W] = _T(""); sIpClient[ID_MDX2500] = _T(""); sIpClient[ID_PUNCH] = _T("");	// ID_SR1000W, ID_MDX2500, ID_PUNCH
		sIpServer[ID_SR1000W] = _T(""); sIpServer[ID_MDX2500] = _T(""); sIpServer[ID_ENGRAVE] = _T("");	// ID_SR1000W, ID_MDX2500, ID_ENGRAVE
		sPort[ID_SR1000W] = _T(""); sPort[ID_MDX2500] = _T(""); sPort[ID_ENGRAVE] = _T("");				// ID_SR1000W, ID_MDX2500, ID_ENGRAVE(ID_PUNCH)

		bSaveMkImg = FALSE;
		bSaveGrabImg = FALSE;
		bStripPcsRgnBin = FALSE;
		bUseDTS = FALSE;
		bUseITS = FALSE;

		bUseDevicePartial = FALSE;
		bUseEngrave = FALSE; bUseAoiUp = TRUE; bUseAoiDn = TRUE; bUsePunch = TRUE;
		bUsePunchOnly = FALSE;
	}
};

struct stEngraveInfo
{
	CString sProcessNum;
	CString sModel, sLayerUp, sLot, sSerialUp, sCompletedSerialUp;
	CString sLayerDn, sSerialDn, sCompletedSerialDn;
	CString sSerialEng;
	CString sInnerModel, sInnerLayerUp, sInnerLot;
	CString sInnerLayerDn;
	CString sEngItsCode;

	stEngraveInfo()
	{
		sProcessNum = _T("");
		sModel = _T(""); sLayerUp = _T(""); sLot = _T(""); sSerialUp = _T(""); sCompletedSerialUp = _T("");
		sLayerDn = _T(""); sSerialDn = _T(""); sCompletedSerialDn = _T("");
		sSerialEng = _T("");
		sInnerModel = _T(""); sInnerLayerUp = _T(""); sInnerLot = _T("");
		sInnerLayerDn = _T("");
		sEngItsCode = _T("");
	}
};

struct stLastJob
{
	CString sProcessNum;
	CString sModel, sLayerUp, sLot, sSerialUp, sCompletedSerialUp;
	CString sLayerDn, sSerialDn, sCompletedSerialDn;
	CString sSerialEng;
	CString sInnerModel, sInnerLayerUp, sInnerLot;
	CString sInnerLayerDn;

	CString sSelUserName, sReelTotLen, sOnePnlLen;
	BOOL bLotSep;
	CString sLotSepLen, sLotCutPosLen;
	BOOL bTempPause;	// [0]:AOI-Up , [1]:AOI-Dn
	int nMergingLayer;
	CString sTempPauseLen, sLmtTotYld, sLmtPatlYld;
	CString sStripOutRatio, sCustomNeedRatio;
	BOOL bContFixDef;
	CString sNumRangeFixDef, sNumContFixDef, sUltraSonicCleannerStTim, sEngItsCode;
	BOOL bRclDrSen, bMkDrSen, bBufDrSen, bAoiUpDrSen, bAoiDnDrSen, bEngvDrSen, bUclDrSen;
	BOOL bUse380mm, bUse346mm, bUse340mm;
	BOOL bDispMkPcs, bStopFixDef, bMkSftySen, bAoiSftySen;
	CString sJogSpd, sLotSerial; //sLightVal, 
	BOOL bLightOn, bMkOnePnl, bAoiOnePnl, bEngraveOnePnl;
	BOOL bVerify, bReview;
	int nVerifyPeriod;
	CString sVerifyLen;
	//CString sDistAoiMk;
	CString sFurgeNum, sFurgeRun;
	CString sMkLastShot;
	CString sAoiLastShot[2]; // [Up/Dn]
	CString sPartialSpd;
	BOOL bOneMetal, bTwoMetal;
	BOOL bFeedRecoilerCcw, bFeedUncoilerCcw;
	BOOL bDualTest, bDualTestInner, bSampleTest;
	int nTestMode;
	BOOL bFeedCcwRecoiler, bFeedCcwUncoiler;
	BOOL bCore150Recoiler, bCore150Uncoiler;
	CString sSampleTestShotNum;
	BOOL bUse2Layer;
	CString sEngraveOrderNum, sEngraveLastShot;
	CString sEngraveOrgX, sEngraveOrgY, sEngravePosOffsetX, sEngravePosOffsetY, sEngravePosTheta;
	int nAlignMethode;
	BOOL bUseAoiUpCleanRoler, bUseAoiDnCleanRoler;
	BOOL bUseEngraveUltrasonic, bUseAoiDnUltrasonic;
	BOOL bVelEngraveUltrasonic, bVelAoiDnUltrasonic;
	CString sCurrentShotNum, sSettingShotNum;
	int nAlarmTimePunch, nAlarmTimeAoi;
	BOOL bDispContRun, bDispLotEnd;
	BOOL bUseJudgeMk;
	int nJudgeMkRatio;

	stLastJob()
	{
		sProcessNum = _T("");
		sModel = _T(""); sLayerUp = _T(""); sLot = _T(""); sSerialUp = _T(""); sCompletedSerialUp = _T("");
		sLayerDn = _T(""); sSerialDn = _T(""); sCompletedSerialDn = _T("");
		sSerialEng = _T("");
		sInnerModel = _T(""); sInnerLayerUp = _T(""); sInnerLot = _T("");
		sInnerLayerDn = _T(""); 

		sSelUserName = _T(""); sReelTotLen = _T(""); sOnePnlLen = _T("");
		bLotSep = FALSE;
		sLotSepLen = _T(""); sLotCutPosLen = _T("");
		bTempPause = FALSE;
		nMergingLayer = 0; 	// [0]:AOI-Up , [1]:AOI-Dn
		sTempPauseLen = _T(""); sLmtTotYld = _T(""); sLmtPatlYld = _T("");
		sStripOutRatio = _T("20.0"); // Minimum 20%
		sCustomNeedRatio = _T("");
		bContFixDef = FALSE;
		sNumRangeFixDef = _T("");
		sNumContFixDef = _T("");
		sUltraSonicCleannerStTim = _T("5.0"); // AOI_Dn : MW05940, AOI_Up : MW05942
		bRclDrSen = FALSE; bMkDrSen = FALSE; bBufDrSen = FALSE; bAoiUpDrSen = FALSE; bAoiDnDrSen = FALSE; bEngvDrSen = FALSE; bUclDrSen = FALSE; 
		bUse380mm = FALSE; bUse346mm = FALSE; bUse340mm = FALSE;
		bDispMkPcs = FALSE; bStopFixDef = FALSE; bMkSftySen = FALSE; bAoiSftySen = FALSE;
		sJogSpd = _T(""); sLotSerial = _T(""); //sLightVal=""); 
		bLightOn = FALSE; bMkOnePnl = FALSE; bAoiOnePnl = FALSE; bEngraveOnePnl = FALSE;
		bVerify = FALSE; bReview = FALSE;
		sVerifyLen = _T("");
		nVerifyPeriod = 0;
		//sDistAoiMk = _T("");
		sFurgeNum = _T(""); sFurgeRun = _T("");
		sMkLastShot = _T("0");
		sAoiLastShot[0] = _T("0");
		sAoiLastShot[1] = _T("0");
		sPartialSpd = _T("10");

		bOneMetal = FALSE; bTwoMetal = FALSE;
		bFeedRecoilerCcw = FALSE; bFeedUncoilerCcw = FALSE;
		bFeedCcwRecoiler = FALSE; bFeedCcwUncoiler = FALSE;
		bDualTest = TRUE; bDualTestInner = TRUE; bSampleTest = FALSE; nTestMode = 0;
		bCore150Recoiler = FALSE; bCore150Uncoiler = FALSE;
		sSampleTestShotNum = _T("");
		bUseEngraveUltrasonic = FALSE; bUseAoiDnUltrasonic = FALSE;
		bVelEngraveUltrasonic = FALSE; bVelAoiDnUltrasonic = FALSE;

		bUse2Layer = FALSE;

		sEngraveOrderNum = _T(""); sEngraveLastShot = _T("");
		sEngraveOrgX = _T(""); sEngraveOrgY = _T(""); sEngravePosOffsetX = _T(""); sEngravePosOffsetY = _T(""); sEngravePosTheta = _T("");
		nAlignMethode = TWO_POINT;

		bUseAoiUpCleanRoler = FALSE; bUseAoiDnCleanRoler = FALSE;
		sEngItsCode = _T("");
		sCurrentShotNum = _T(""); sSettingShotNum = _T("");

		nAlarmTimePunch = 10800; nAlarmTimeAoi = 10800;
		bDispContRun = FALSE; bDispLotEnd = FALSE;
		bUseJudgeMk = TRUE;
		nJudgeMkRatio = 85;
	}
};

struct stMarking
{
	CString sWaitPos;
	CString sWaitVel;
	CString sWaitAcc;
	CString sMarkingPos;
	CString sMarkingVel;
	CString sMarkingAcc;
	CString sMarkingToq;
	CString sMarkingToqOffset;
	CString sMeasurePosX[4], sMeasurePosY[4];
	CString sAverDist;
	CString sMarkingDisp1Toq, sMarkingDisp2Toq, sMarkingDisp3Toq;
	int nMkCnt;
	int nMkLimit;

	stMarking()
	{
		sWaitPos = _T("");
		sWaitPos = _T("");
		sWaitVel = _T("");
		sWaitAcc = _T("");
		sMarkingPos = _T("");
		sMarkingVel = _T("");
		sMarkingAcc = _T("");
		sMarkingToq = _T("");
		sMarkingToqOffset = _T("");
		sMarkingDisp1Toq = _T("");
		sMarkingDisp2Toq = _T("");
		sMarkingDisp3Toq = _T("");

		for (int i = 0; i < 4; i++)
		{
			sMeasurePosX[i] = _T("");
			sMeasurePosY[i] = _T("");
		}

		sAverDist = _T("");

		nMkCnt = 0;
		nMkLimit = 0;
	}
};

struct stElecChk
{
	BOOL bUse, bStopOnOpen;
	CString sWaitPos;
	CString sWaitVel;
	CString sWaitAcc;
	CString sProbingPos;
	CString sProbingVel;
	CString sProbingAcc;
	CString sProbingToq;
	CString sMeasurePosX, sMeasurePosY;
	CString sRepeatTestNum;
	int nRepeatTest;

	stElecChk()
	{
		bUse = TRUE;
		bStopOnOpen = TRUE;
		sWaitPos = _T("");
		sWaitPos = _T("");
		sWaitVel = _T("");
		sWaitAcc = _T("");
		sProbingPos = _T("");
		sProbingVel = _T("");
		sProbingAcc = _T("");
		sProbingToq = _T("");

		sMeasurePosX = _T("");
		sMeasurePosY = _T("");
		sRepeatTestNum = _T("3");
		nRepeatTest = 0;
	}
};

struct stVocieCoil
{
	CString sPort, sBaudRate, sVal;
	BOOL bCr, bLf;
	CString sSmacHeightAtPos[4];
	CString sSmacAverageDist;
	//	CString sSmacFinalPos, sSmacFinalVel, sSmacFinalAcc, sSmacFinalToq;
	//	CString sSmacShiftPos, sSmacShiftVel, sSmacShiftAcc, sSmacShiftToq;
	stVocieCoil()
	{
		sPort = _T(""); sBaudRate = _T(""); sVal = _T("");
		bCr = FALSE; bLf = FALSE;
		sSmacAverageDist = _T("");

		for (int i = 0; i < 4; i++)
		{
			sSmacHeightAtPos[i] = _T("");
		}
	}
};

struct stLight
{
	CString sPort, sBaudRate, sCh[MAX_LIGHT_CH], sVal[MAX_LIGHT_CH];
	BOOL bCr, bLf;

	stLight()
	{
		sPort = _T(""); sBaudRate = _T("");
		bCr = FALSE; bLf = FALSE;
		for (int i = 0; i < MAX_LIGHT_CH; i++)
		{
			sCh[i] = _T("");
			sVal[i] = _T("");
		}
	}
};

struct stFluck
{
	BOOL bUse;
	CString sPort, sBaudRate;
	BOOL bCr, bLf;
	CString sThreshold;
	CString sRejectMkNum; // in Strip
	double dMeasPosX[2], dMeasPosY[2];

	stFluck()
	{
		bUse = FALSE;
		sPort = _T(""); sBaudRate = _T("");
		bCr = TRUE; bLf = FALSE;

		sThreshold = _T("");
		sRejectMkNum = _T("");

		dMeasPosX[0] = 0.0; dMeasPosY[0] = 0.0;
		dMeasPosX[1] = 0.0; dMeasPosY[1] = 0.0;
	}
};

struct stVision
{
	CString sCamPxlRes;
	CString sPinImgSz;
	CString sResX, sResY;
	CString sMkOffsetX, sMkOffsetY;
	CString sAlgnPosX[2], sAlgnPosY[2];
	CString sPtMatScr, sStdScr;

	stVision()
	{
		sCamPxlRes = _T("");
		sPinImgSz = _T("");
		sResX = _T(""); sResY = _T("");
		sMkOffsetX = _T(""); sMkOffsetY = _T("");
		sAlgnPosX[2] = _T(""); sAlgnPosY[2] = _T("");
		sPtMatScr = _T("");
		sPtMatScr = _T("");
	}
};

struct stLotTime
{
	int nYear, nMonth, nDay;
	int nHour, nMin, nSec;

	stLotTime()
	{
		nYear = 0; nMonth = 0; nDay = 0;
		nHour = 0; nMin = 0; nSec = 0;
	}
};

struct stLot
{
	stLotTime StTime, CurTime, EdTime;
	DWORD dwStTick;
	CString sTotalReelDist;
	CString sSeparateDist;
	CString sCuttingDist;
	CString sStopDist;

	stLot()
	{
		sTotalReelDist = _T("");
		sSeparateDist = _T("");
		sCuttingDist = _T("");
		sStopDist = _T("");
	}
};

#include "../Device/MotionParam.h"

struct stWorkingInfo
{
	stSystem System;
	stLastJob LastJob;
	stMotionMotion Motion;
	stMarking Marking[2];
	stVocieCoil VoiceCoil[2];
	stLight Light;
	stFluck Fluck;
	stVision Vision[2];
	stLot Lot;
	stElecChk Probing[2];
	stEngraveInfo EngInfo;
};

struct stYield
{
	int nTot, nGood, nDef;
	int nTotSriptOut;
	int nDefStrip[MAX_STRIP_NUM], nDefA[MAX_DEF], nDefPerStrip[MAX_STRIP_NUM][MAX_DEF], nStripOut[MAX_STRIP_NUM];

	stYield()
	{
		nTot = 0; nGood = 0; nDef = 0;
		nTotSriptOut = 0;
		nDefStrip[0] = 0; nDefStrip[1] = 0; nDefStrip[2] = 0; nDefStrip[3] = 0;
		nStripOut[0] = 0; nStripOut[1] = 0; nStripOut[2] = 0; nStripOut[3] = 0;

		for (int k = 0; k < MAX_DEF; k++)
		{
			nDefA[k] = 0;

			for (int i = 0; i < MAX_STRIP_NUM; i++)
			{
				nDefPerStrip[i][k] = 0;
			}
		}
	}

	~stYield()
	{
		;
	}
};



struct stSliceIo
{
	int nMaxSeg, nInSeg, nOutSeg;
	CString **pAddrIn, **pAddrOut;

	stSliceIo()
	{
		nMaxSeg = 0; nInSeg = 0; nOutSeg = 0;
		pAddrIn = NULL;
		pAddrOut = NULL;
	}

	~stSliceIo()
	{
		if (pAddrIn)
		{
			for (int k = 0; k < nInSeg; k++)
				delete[] pAddrIn[k];
			delete[] pAddrIn;
			pAddrIn = NULL;
		}

		if (pAddrOut)
		{
			for (int k = 0; k < nOutSeg; k++)
				delete[] pAddrOut[k];
			delete[] pAddrOut;
			pAddrOut = NULL;
		}
	}
};

struct stMpeIo
{
	int nMaxSeg, nInSeg, nOutSeg;
	CString **pAddrIn, **pAddrOut;
	CString **pSymIn;

	int nGrpStep;
	int nGrpIn, nGrpOut;
	int nGrpInSt, nGrpOutSt;

	stMpeIo()
	{
		pAddrIn = NULL;
		pAddrOut = NULL;
		pSymIn = NULL;
	}

	~stMpeIo()
	{
		if (pAddrIn)
		{
			for (int k = 0; k < nInSeg; k++)
				delete[] pAddrIn[k];
			delete[] pAddrIn;
			pAddrIn = NULL;
		}

		if (pAddrOut)
		{
			for (int k = 0; k < nOutSeg; k++)
				delete[] pAddrOut[k];
			delete[] pAddrOut;
			pAddrOut = NULL;
		}

		if (pSymIn)
		{
			for (int k = 0; k < nInSeg; k++)
				delete[] pSymIn[k];
			delete[] pSymIn;
			pSymIn = NULL;
		}
	}
};

struct stMkIo
{
	// 	stSliceIo SliceIo;
	stMpeIo MpeIo, MpeSignal, MpeData;
};


struct stBtnMain
{
	BOOL Ready, Run, Reset, Stop, Auto, Manual;
	BOOL PrevReady, PrevRun, PrevReset, PrevStop, PrevAuto, PrevManual;

	BOOL IsReady, IsRun, IsReset, IsStop, IsAuto, IsManual;
	BOOL IsPrevReady, IsPrevRun, IsPrevReset, IsPrevStop, IsPrevAuto, IsPrevManual;

	stBtnMain()
	{
		Init();
	}

	void Init()
	{
		Ready = FALSE; Run = FALSE; Reset = FALSE; Stop = FALSE; Auto = FALSE; Manual = FALSE;
		PrevReady = FALSE; PrevRun = FALSE; PrevReset = FALSE; PrevStop = FALSE; PrevAuto = FALSE; PrevManual = FALSE;

		IsReady = FALSE; IsRun = FALSE; IsReset = FALSE; IsStop = FALSE; IsAuto = FALSE; IsManual = FALSE;
		IsPrevReady = FALSE; IsPrevRun = FALSE; IsPrevReset = FALSE; IsPrevStop = FALSE; IsPrevAuto = FALSE; IsPrevManual = FALSE;
	}
};

struct stBtnRunMode
{
	BOOL ConnectModule, FeedOnePanel;
	BOOL PrevConnectModule, PrevFeedOnePanel;
	BOOL IsConnectModule, IsFeedOnePanel;
	BOOL IsPrevConnectModule, IsPrevFeedOnePanel;

	stBtnRunMode()
	{
		Init();
	}

	void Init()
	{
		ConnectModule = FALSE; FeedOnePanel = FALSE;
		PrevConnectModule = FALSE; PrevFeedOnePanel = FALSE;
		IsConnectModule = FALSE; IsFeedOnePanel = FALSE;
		IsPrevConnectModule = FALSE; IsPrevFeedOnePanel = FALSE;
	}
};

struct stBtnLaser
{
	BOOL Laser380mm, Laser346mm, Laser340mm;
	BOOL PrevLaser380mm, PrevLaser346mm, PrevLaser340mm;
	BOOL IsLaser380mm, IsLaser346mm, IsLaser340mm;
	BOOL IsPrevLaser380mm, IsPrevLaser346mm, IsPrevLaser340mm;

	stBtnLaser()
	{
		Init();
	}

	void Init()
	{
		Laser380mm = FALSE; Laser346mm = FALSE; Laser340mm = FALSE;
		PrevLaser380mm = FALSE; PrevLaser346mm = FALSE; PrevLaser340mm = FALSE;
		IsLaser380mm = FALSE; IsLaser346mm = FALSE; IsLaser340mm = FALSE;
		IsPrevLaser380mm = FALSE; IsPrevLaser346mm = FALSE; IsPrevLaser340mm = FALSE;
	}
};

struct stBtnFeedDir
{
	BOOL FeedCcwRecoiler, FeedCcwUncoiler;
	BOOL PrevFeedCcwRecoiler, PrevFeedCcwUncoiler;
	BOOL IsFeedCcwRecoiler, IsFeedCcwUncoiler;
	BOOL IsPrevFeedCcwRecoiler, IsPrevFeedCcwUncoiler;

	stBtnFeedDir()
	{
		Init();
	}

	void Init()
	{
		FeedCcwRecoiler = FALSE; FeedCcwUncoiler = FALSE;
		PrevFeedCcwRecoiler = FALSE; PrevFeedCcwUncoiler = FALSE;
		IsFeedCcwRecoiler = FALSE; IsFeedCcwUncoiler = FALSE;
		IsPrevFeedCcwRecoiler = FALSE; IsPrevFeedCcwUncoiler = FALSE;
	}
};

struct stBtnDancerRoll
{
	BOOL AllDancerUpDn, AllDancerFixOnOff;
	BOOL PrevAllDancerUpDn, PrevAllDancerFixOnOff;
	BOOL IsAllDancerUpDn, IsAllDancerFixOnOff;
	BOOL IsPrevAllDancerUpDn, IsPrevAllDancerFixOnOff;

	stBtnDancerRoll()
	{
		Init();
	}

	void Init()
	{
		AllDancerUpDn = FALSE; AllDancerFixOnOff = FALSE;
		PrevAllDancerUpDn = FALSE; PrevAllDancerFixOnOff = FALSE;
		IsAllDancerUpDn = FALSE; IsAllDancerFixOnOff = FALSE;
		IsPrevAllDancerUpDn = FALSE; IsPrevAllDancerFixOnOff = FALSE;
	}
};


struct stBtnMsg
{
	BOOL Yes, No, Ok, Cancel;
	BOOL IsYes, IsNo, IsOk, IsCancel;

	stBtnMsg()
	{
		Init();
	}

	void Init()
	{
		Yes = FALSE; No = FALSE; Ok = FALSE; Cancel = FALSE;
		IsYes = FALSE; IsNo = FALSE; IsOk = FALSE; IsCancel = FALSE;
	}
};

struct stBtnDisp
{
	BOOL DualSample, SingleSample, DualTest, SingleTest, InitRun, Ready, Run, Stop;
	BOOL PrevDualSample, PrevSingleSample, PrevDualTest, PrevSingleTest, PrevInitRun, PrevReady, PrevRun, PrevStop;

	BOOL IsDualSample, IsSingleSample, IsDualTest, IsSingleTest, IsInitRun, IsReady, IsRun, IsStop;
	BOOL IsPrevDualSample, IsPrevSingleSample, IsPrevDualTest, IsPrevSingleTest, IsPrevInitRun, IsPrevReady, IsPrevRun, IsPrevStop;

	stBtnDisp()
	{
		Init();
	}

	void Init()
	{
		DualSample = FALSE; SingleSample = FALSE; DualTest = FALSE; SingleTest = FALSE; InitRun = FALSE;
		Ready = FALSE; Run = FALSE; Stop = FALSE;
		PrevDualSample = FALSE; PrevSingleSample = FALSE; PrevDualTest = FALSE; PrevSingleTest = FALSE;
		PrevInitRun = FALSE; PrevReady = FALSE; PrevRun = FALSE; PrevStop = FALSE;

		IsDualSample = FALSE; IsSingleSample = FALSE; IsDualTest = FALSE; IsSingleTest = FALSE; IsInitRun = FALSE;
		IsReady = FALSE; IsRun = FALSE; IsStop = FALSE;
		IsPrevDualSample = FALSE; IsPrevSingleSample = FALSE; IsPrevDualTest = FALSE; IsPrevSingleTest = FALSE;
		IsPrevInitRun = FALSE; IsPrevReady = FALSE; IsPrevRun = FALSE; IsPrevStop = FALSE;
	}
};

struct stBtnTqMotor
{
	BOOL Mk, Aoi, Eng;
	BOOL PrevMk, PrevAoi, PrevEng;

	BOOL IsMk, IsAoi, IsEng;
	BOOL IsPrevMk, IsPrevAoi, IsPrevEng;

	stBtnTqMotor()
	{
		Mk = FALSE; Aoi = FALSE; Eng = FALSE;
		PrevMk = FALSE; PrevAoi = FALSE; PrevEng = FALSE;

		IsMk = FALSE; IsAoi = FALSE; IsEng = FALSE;
		IsPrevMk = FALSE; IsPrevAoi = FALSE; IsPrevEng = FALSE;
	}
};

struct stBtnInductMotor
{
	BOOL Uc, Rc;
	BOOL PrevUc, PrevRc;

	BOOL IsUc, IsRc;
	BOOL IsPrevUc, IsPrevRc;

	stBtnInductMotor()
	{
		Uc = FALSE; Rc = FALSE;
		PrevUc = FALSE; PrevRc = FALSE;

		IsUc = FALSE; IsRc = FALSE;
		IsPrevUc = FALSE; IsPrevRc = FALSE;
	}
};

struct stBtnCore150
{
	BOOL Uc, Rc;
	BOOL PrevUc, PrevRc;

	BOOL IsUc, IsRc;
	BOOL IsPrevUc, IsPrevRc;

	stBtnCore150()
	{
		Uc = FALSE; Rc = FALSE;
		PrevUc = FALSE; PrevRc = FALSE;

		IsUc = FALSE; IsRc = FALSE;
		IsPrevUc = FALSE; IsPrevRc = FALSE;
	}
};

struct stBtnEtc
{
	BOOL BufR, EmgAoi;
	BOOL PrevBufR, PrevEmgAoi;

	BOOL IsBufR, IsEmgAoi;
	BOOL IsPrevBufR, IsPrevEmgAoi;

	stBtnEtc()
	{
		BufR = FALSE; EmgAoi = FALSE;
		PrevBufR = FALSE; PrevEmgAoi = FALSE;

		IsBufR = FALSE; IsEmgAoi = FALSE;
		IsPrevBufR = FALSE; IsPrevEmgAoi = FALSE;
	}
};

struct stBtnRecoiler
{
	BOOL ChuckPcb, ChuckPaper, JoinClamp, PcbShaftSupport;
	BOOL EpcActHome, EpcActFirst, EpcActSecond, EpcActThird;
	BOOL PcbInverterCw, PcbInverterCcw, PaperInverterCw, PaperInverterCcw;
	BOOL DancerUpper, DancerFixer, EpcAuto, EpcManual;
	BOOL EpcIn, EpcOut, EpcHome;

	BOOL PrevChuckPcb, PrevChuckPaper, PrevJoinClamp, PrevPcbShaftSupport;
	BOOL PrevEpcActHome, PrevEpcActFirst, PrevEpcActSecond, PrevEpcActThird;
	BOOL PrevPcbInverterCw, PrevPcbInverterCcw, PrevPaperInverterCw, PrevPaperInverterCcw;
	BOOL PrevDancerUpper, PrevDancerFixer, PrevEpcAuto, PrevEpcManual;
	BOOL PrevEpcIn, PrevEpcOut, PrevEpcHome;

	BOOL IsChuckPcb, IsChuckPaper, IsJoinClamp, IsPcbShaftSupport;
	BOOL IsEpcActHome, IsEpcActFirst, IsEpcActSecond, IsEpcActThird;
	BOOL IsPcbInverterCw, IsPcbInverterCcw, IsPaperInverterCw, IsPaperInverterCcw;
	BOOL IsDancerUpper, IsDancerFixer, IsEpcAuto, IsEpcManual;
	BOOL IsEpcIn, IsEpcOut, IsEpcHome;

	BOOL IsPrevChuckPcb, IsPrevChuckPaper, IsPrevJoinClamp, IsPrevPcbShaftSupport;
	BOOL IsPrevEpcActHome, IsPrevEpcActFirst, IsPrevEpcActSecond, IsPrevEpcActThird;
	BOOL IsPrevPcbInverterCw, IsPrevPcbInverterCcw, IsPrevPaperInverterCw, IsPrevPaperInverterCcw;
	BOOL IsPrevDancerUpper, IsPrevDancerFixer, IsPrevEpcAuto, IsPrevEpcManual;
	BOOL IsPrevEpcIn, IsPrevEpcOut, IsPrevEpcHome;

	stBtnRecoiler()
	{
		ChuckPcb = FALSE; ChuckPaper = FALSE; JoinClamp = FALSE; PcbShaftSupport = FALSE;
		EpcActHome = FALSE; EpcActFirst = FALSE; EpcActSecond = FALSE; EpcActThird = FALSE;
		PcbInverterCw = FALSE; PcbInverterCcw = FALSE; PaperInverterCw = FALSE; PaperInverterCcw = FALSE;
		DancerUpper = FALSE; DancerFixer = FALSE; EpcAuto = FALSE; EpcManual = FALSE;
		EpcIn = FALSE; EpcOut = FALSE; EpcHome = FALSE;

		PrevChuckPcb = FALSE; PrevChuckPaper = FALSE; PrevJoinClamp = FALSE; PrevPcbShaftSupport = FALSE;
		PrevEpcActHome = FALSE; PrevEpcActFirst = FALSE; PrevEpcActSecond = FALSE; PrevEpcActThird = FALSE;
		PrevPcbInverterCw = FALSE; PrevPcbInverterCcw = FALSE; PrevPaperInverterCw = FALSE; PrevPaperInverterCcw = FALSE;
		PrevDancerUpper = FALSE; PrevDancerFixer = FALSE; PrevEpcAuto = FALSE; PrevEpcManual = FALSE;
		PrevEpcIn = FALSE; PrevEpcOut = FALSE; PrevEpcHome = FALSE;

		IsChuckPcb = FALSE; IsChuckPaper = FALSE; IsJoinClamp = FALSE; IsPcbShaftSupport = FALSE;
		IsEpcActHome = FALSE; IsEpcActFirst = FALSE; IsEpcActSecond = FALSE; IsEpcActThird = FALSE;
		IsPcbInverterCw = FALSE; IsPcbInverterCcw = FALSE; IsPaperInverterCw = FALSE; IsPaperInverterCcw = FALSE;
		IsDancerUpper = FALSE; IsDancerFixer = FALSE; IsEpcAuto = FALSE; IsEpcManual = FALSE;
		IsEpcIn = FALSE; IsEpcOut = FALSE; IsEpcHome = FALSE;

		IsPrevChuckPcb = FALSE; IsPrevChuckPaper = FALSE; IsPrevJoinClamp = FALSE; IsPrevPcbShaftSupport = FALSE;
		IsPrevEpcActHome = FALSE; IsPrevEpcActFirst = FALSE; IsPrevEpcActSecond = FALSE; IsPrevEpcActThird = FALSE;
		IsPrevPcbInverterCw = FALSE; IsPrevPcbInverterCcw = FALSE; IsPrevPaperInverterCw = FALSE; IsPrevPaperInverterCcw = FALSE;
		IsPrevDancerUpper = FALSE; IsPrevDancerFixer = FALSE; IsPrevEpcAuto = FALSE; IsPrevEpcManual = FALSE;
		IsPrevEpcIn = FALSE; IsPrevEpcOut = FALSE; IsPrevEpcHome = FALSE;
	}
};

struct stBtnPunch
{
	BOOL FeedCw, FeedCcw, TensionCw, TensionCcw;
	BOOL FeedHome, FeedVacuum, FeedClamp, TensionClamp;
	BOOL TableVacuum, TableBlower, TableCylinder;
	BOOL TableClampDn, TableClampForward, PunchStart;

	BOOL PrevFeedCw, PrevFeedCcw, PrevTensionCw, PrevTensionCcw;
	BOOL PrevFeedHome, PrevFeedVacuum, PrevFeedClamp, PrevTensionClamp;
	BOOL PrevTableVacuum, PrevTableBlower, PrevTableCylinder;
	BOOL PrevTableClampDn, PrevTableClampForward, PrevPunchStart;

	BOOL IsFeedCw, IsFeedCcw, IsTensionCw, IsTensionCcw;
	BOOL IsFeedHome, IsFeedVacuum, IsFeedClamp, IsTensionClamp;
	BOOL IsTableVacuum, IsTableBlower, IsTableCylinder;
	BOOL IsTableClampDn, IsTableClampForward, IsPunchStart;

	BOOL IsPrevFeedCw, IsPrevFeedCcw, IsPrevTensionCw, IsPrevTensionCcw;
	BOOL IsPrevFeedHome, IsPrevFeedVacuum, IsPrevFeedClamp, IsPrevTensionClamp;
	BOOL IsPrevTableVacuum, IsPrevTableBlower, IsPrevTableCylinder;
	BOOL IsPrevTableClampDn, IsPrevTableClampForward, IsPrevPunchStart;

	stBtnPunch()
	{
		FeedCw = FALSE; FeedCcw = FALSE; TensionCw = FALSE; TensionCcw = FALSE;
		FeedHome = FALSE; FeedVacuum = FALSE; FeedClamp = FALSE; TensionClamp = FALSE;
		TableVacuum = FALSE; TableBlower = FALSE; TableCylinder = FALSE;
		TableClampDn = FALSE; TableClampForward = FALSE; PunchStart = FALSE;

		PrevFeedCw = FALSE; PrevFeedCcw = FALSE; PrevTensionCw = FALSE; PrevTensionCcw = FALSE;
		PrevFeedHome = FALSE; PrevFeedVacuum = FALSE; PrevFeedClamp = FALSE; PrevTensionClamp = FALSE;
		PrevTableVacuum = FALSE; PrevTableBlower = FALSE; PrevTableCylinder = FALSE;
		PrevTableClampDn = FALSE; PrevTableClampForward = FALSE; PrevPunchStart = FALSE;

		IsFeedCw = FALSE; IsFeedCcw = FALSE; IsTensionCw = FALSE; IsTensionCcw = FALSE;
		IsFeedHome = FALSE; IsFeedVacuum = FALSE; IsFeedClamp = FALSE; IsTensionClamp = FALSE;
		IsTableVacuum = FALSE; IsTableBlower = FALSE; IsTableCylinder = FALSE;
		IsTableClampDn = FALSE; IsTableClampForward = FALSE; IsPunchStart = FALSE;

		IsPrevFeedCw = FALSE; IsPrevFeedCcw = FALSE; IsPrevTensionCw = FALSE; IsPrevTensionCcw = FALSE;
		IsPrevFeedHome = FALSE; IsPrevFeedVacuum = FALSE; IsPrevFeedClamp = FALSE; IsPrevTensionClamp = FALSE;
		IsPrevTableVacuum = FALSE; IsPrevTableBlower = FALSE; IsPrevTableCylinder = FALSE;
		IsPrevTableClampDn = FALSE; IsPrevTableClampForward = FALSE; IsPrevPunchStart = FALSE;
	}
};

struct stBtnAOIDn
{
	BOOL FeedCw, FeedCcw, TensionCw, TensionCcw;
	BOOL FeedHome, FeedVacuum, FeedClamp, TensionClamp;
	BOOL TableVacuum, TableBlower, TableCylinder;
	BOOL CleanRollerUp, CleanRollerDn, CleanRollerPush;
	BOOL TestStart, BufferRollerUp, BufferRollerFix;

	BOOL PrevFeedCw, PrevFeedCcw, PrevTensionCw, PrevTensionCcw;
	BOOL PrevFeedHome, PrevFeedVacuum, PrevFeedClamp, PrevTensionClamp;
	BOOL PrevTableVacuum, PrevTableBlower, PrevTableCylinder;
	BOOL PrevCleanRollerUp, PrevCleanRollerDn, PrevCleanRollerPush;
	BOOL PrevTestStart, PrevBufferRollerUp, PrevBufferRollerFix;

	BOOL IsFeedCw, IsFeedCcw, IsTensionCw, IsTensionCcw;
	BOOL IsFeedHome, IsFeedVacuum, IsFeedClamp, IsTensionClamp;
	BOOL IsTableVacuum, IsTableBlower, IsTableCylinder;
	BOOL IsCleanRollerUp, IsCleanRollerDn, IsCleanRollerPush;
	BOOL IsTestStart, IsBufferRollerUp, IsBufferRollerFix;

	BOOL IsPrevFeedCw, IsPrevFeedCcw, IsPrevTensionCw, IsPrevTensionCcw;
	BOOL IsPrevFeedHome, IsPrevFeedVacuum, IsPrevFeedClamp, IsPrevTensionClamp;
	BOOL IsPrevTableVacuum, IsPrevTableBlower, IsPrevTableCylinder;
	BOOL IsPrevCleanRollerUp, IsPrevCleanRollerDn, IsPrevCleanRollerPush;
	BOOL IsPrevTestStart, IsPrevBufferRollerUp, IsPrevBufferRollerFix;

	stBtnAOIDn()
	{
		FeedCw = FALSE; FeedCcw = FALSE; TensionCw = FALSE; TensionCcw = FALSE;
		FeedHome = FALSE; FeedVacuum = FALSE; FeedClamp = FALSE; TensionClamp = FALSE;
		TableVacuum = FALSE; TableBlower = FALSE; TableCylinder = FALSE;
		CleanRollerUp = FALSE; CleanRollerDn = FALSE; CleanRollerPush = FALSE;
		TestStart = FALSE; BufferRollerUp = FALSE; BufferRollerFix = FALSE;

		PrevFeedCw = FALSE; PrevFeedCcw = FALSE; PrevTensionCw = FALSE; PrevTensionCcw = FALSE;
		PrevFeedHome = FALSE; PrevFeedVacuum = FALSE; PrevFeedClamp = FALSE; PrevTensionClamp = FALSE;
		PrevTableVacuum = FALSE; PrevTableBlower = FALSE; PrevTableCylinder = FALSE;
		PrevCleanRollerUp = FALSE; PrevCleanRollerDn = FALSE; PrevCleanRollerPush = FALSE;
		PrevTestStart = FALSE; PrevBufferRollerUp = FALSE; PrevBufferRollerFix = FALSE;

		IsFeedCw = FALSE; IsFeedCcw = FALSE; IsTensionCw = FALSE; IsTensionCcw = FALSE;
		IsFeedHome = FALSE; IsFeedVacuum = FALSE; IsFeedClamp = FALSE; IsTensionClamp = FALSE;
		IsTableVacuum = FALSE; IsTableBlower = FALSE; IsTableCylinder = FALSE;
		IsCleanRollerUp = FALSE; IsCleanRollerDn = FALSE; IsCleanRollerPush = FALSE;
		IsTestStart = FALSE; IsBufferRollerUp = FALSE; IsBufferRollerFix = FALSE;

		IsPrevFeedCw = FALSE; IsPrevFeedCcw = FALSE; IsPrevTensionCw = FALSE; IsPrevTensionCcw= FALSE;
		IsPrevFeedHome = FALSE; IsPrevFeedVacuum = FALSE; IsPrevFeedClamp = FALSE; IsPrevTensionClamp = FALSE;
		IsPrevTableVacuum = FALSE; IsPrevTableBlower = FALSE; IsPrevTableCylinder = FALSE;
		IsPrevCleanRollerUp = FALSE; IsPrevCleanRollerDn = FALSE; IsPrevCleanRollerPush = FALSE;
		IsPrevTestStart = FALSE; IsPrevBufferRollerUp = FALSE; IsPrevBufferRollerFix = FALSE;
	}
};

struct stBtnAOIUp
{
	BOOL FeedCw, FeedCcw, TensionCw, TensionCcw;
	BOOL FeedHome, FeedVacuum, FeedClamp, TensionClamp;
	BOOL TableVacuum, TableBlower, TableCylinder; 
	BOOL CleanRollerUp, CleanRollerDn, CleanRollerPush;
	BOOL TestStart;
	BOOL UltrasonicDn, UltrasonicRun, UltrasonicSpeed;

	BOOL PrevFeedCw, PrevFeedCcw, PrevTensionCw, PrevTensionCcw;
	BOOL PrevFeedHome, PrevFeedVacuum, PrevFeedClamp, PrevTensionClamp;
	BOOL PrevTableVacuum, PrevTableBlower, PrevTableCylinder;
	BOOL PrevCleanRollerUp, PrevCleanRollerDn, PrevCleanRollerPush;
	BOOL PrevTestStart;
	BOOL PrevUltrasonicDn, PrevUltrasonicRun, PrevUltrasonicSpeed;

	BOOL IsFeedCw, IsFeedCcw, IsTensionCw, IsTensionCcw;
	BOOL IsFeedHome, IsFeedVacuum, IsFeedClamp, IsTensionClamp;
	BOOL IsTableVacuum, IsTableBlower, IsTableCylinder;
	BOOL IsCleanRollerUp, IsCleanRollerDn, IsCleanRollerPush;
	BOOL IsTestStart;
	BOOL IsUltrasonicDn, IsUltrasonicRun, IsUltrasonicSpeed;

	BOOL IsPrevFeedCw, IsPrevFeedCcw, IsPrevTensionCw, IsPrevTensionCcw;
	BOOL IsPrevFeedHome, IsPrevFeedVacuum, IsPrevFeedClamp, IsPrevTensionClamp;
	BOOL IsPrevTableVacuum, IsPrevTableBlower, IsPrevTableCylinder;
	BOOL IsPrevCleanRollerUp, IsPrevCleanRollerDn, IsPrevCleanRollerPush;
	BOOL IsPrevTestStart;
	BOOL IsPrevUltrasonicDn, IsPrevUltrasonicRun, IsPrevUltrasonicSpeed;

	stBtnAOIUp()
	{
		FeedCw = FALSE; FeedCcw = FALSE; TensionCw = FALSE; TensionCcw = FALSE;
		FeedHome = FALSE; FeedVacuum = FALSE; FeedClamp = FALSE; TensionClamp = FALSE;
		TableVacuum = FALSE; TableBlower = FALSE; TableCylinder = FALSE;
		CleanRollerUp = FALSE; CleanRollerDn = FALSE; CleanRollerPush = FALSE;
		TestStart = FALSE;
		UltrasonicDn = FALSE; UltrasonicRun = FALSE; UltrasonicSpeed = FALSE;

		PrevFeedCw = FALSE; PrevFeedCcw = FALSE; PrevTensionCw = FALSE; PrevTensionCcw = FALSE;
		PrevFeedHome = FALSE; PrevFeedVacuum = FALSE; PrevFeedClamp = FALSE; PrevTensionClamp = FALSE;
		PrevTableVacuum = FALSE; PrevTableBlower = FALSE; PrevTableCylinder = FALSE;
		PrevCleanRollerUp = FALSE; PrevCleanRollerDn = FALSE; PrevCleanRollerPush = FALSE;
		PrevTestStart = FALSE;
		PrevUltrasonicDn = FALSE; PrevUltrasonicRun = FALSE; PrevUltrasonicSpeed = FALSE;

		IsFeedCw = FALSE; IsFeedCcw = FALSE; IsTensionCw = FALSE; IsTensionCcw = FALSE;
		IsFeedHome = FALSE; IsFeedVacuum = FALSE; IsFeedClamp = FALSE; IsTensionClamp = FALSE;
		IsTableVacuum = FALSE; IsTableBlower = FALSE; IsTableCylinder = FALSE;
		IsCleanRollerUp = FALSE; IsCleanRollerDn = FALSE; IsCleanRollerPush = FALSE;
		IsTestStart = FALSE;
		IsUltrasonicDn = FALSE; IsUltrasonicRun = FALSE; IsUltrasonicSpeed = FALSE;

		IsPrevFeedCw = FALSE; IsPrevFeedCcw = FALSE; IsPrevTensionCw = FALSE; IsPrevTensionCcw = FALSE;
		IsPrevFeedHome = FALSE; IsPrevFeedVacuum = FALSE; IsPrevFeedClamp = FALSE; IsPrevTensionClamp = FALSE;
		IsPrevTableVacuum = FALSE; IsPrevTableBlower = FALSE; IsPrevTableCylinder = FALSE;
		IsPrevCleanRollerUp = FALSE; IsPrevCleanRollerDn = FALSE; IsPrevCleanRollerPush = FALSE;
		IsPrevTestStart = FALSE;
		IsPrevUltrasonicDn = FALSE; IsPrevUltrasonicRun = FALSE; IsPrevUltrasonicSpeed = FALSE;
	}
};

struct stBtnEngrave
{
	BOOL FeedCw, FeedCcw, TensionCw, TensionCcw;
	BOOL FeedHome, FeedVacuum, FeedClamp, TensionClamp;
	BOOL TableVacuum, TableBlower, TableCylinder;
	BOOL UltrasonicDown, UltrasonicRun, UltrasonicSpeed;
	BOOL DancerUpper, DancerFixer;
	BOOL AlignStart, LaserStart, ReadStart;

	BOOL PrevFeedCw, PrevFeedCcw, PrevTensionCw, PrevTensionCcw;
	BOOL PrevFeedHome, PrevFeedVacuum, PrevFeedClamp, PrevTensionClamp;
	BOOL PrevTableVacuum, PrevTableBlower, PrevTableCylinder;
	BOOL PrevUltrasonicDown, PrevUltrasonicRun, PrevUltrasonicSpeed;
	BOOL PrevDancerUpper, PrevDancerFixer;
	BOOL PrevAlignStart, PrevLaserStart, PrevReadStart;

	BOOL IsFeedCw, IsFeedCcw, IsTensionCw, IsTensionCcw;
	BOOL IsFeedHome, IsFeedVacuum, IsFeedClamp, IsTensionClamp;
	BOOL IsTableVacuum, IsTableBlower, IsTableCylinder;
	BOOL IsUltrasonicDown, IsUltrasonicRun, IsUltrasonicSpeed;
	BOOL IsDancerUpper, IsDancerFixer;
	BOOL IsAlignStart, IsLaserStart, IsReadStart;

	BOOL IsPrevFeedCw, IsPrevFeedCcw, IsPrevTensionCw, IsPrevTensionCcw;
	BOOL IsPrevFeedHome, IsPrevFeedVacuum, IsPrevFeedClamp, IsPrevTensionClamp;
	BOOL IsPrevTableVacuum, IsPrevTableBlower, IsPrevTableCylinder;
	BOOL IsPrevUltrasonicDown, IsPrevUltrasonicRun, IsPrevUltrasonicSpeed;
	BOOL IsPrevDancerUpper, IsPrevDancerFixer;
	BOOL IsPrevAlignStart, IsPrevLaserStart, IsPrevReadStart;

	stBtnEngrave()
	{
		FeedCw = FALSE; FeedCcw = FALSE; TensionCw = FALSE; TensionCcw = FALSE;
		FeedHome = FALSE; FeedVacuum = FALSE; FeedClamp = FALSE; TensionClamp = FALSE;
		TableVacuum = FALSE; TableBlower = FALSE; TableCylinder = FALSE;
		UltrasonicDown = FALSE; UltrasonicRun = FALSE; UltrasonicSpeed = FALSE;
		DancerUpper = FALSE; DancerFixer = FALSE;
		AlignStart = FALSE; LaserStart = FALSE; ReadStart = FALSE;

		PrevFeedCw = FALSE; PrevFeedCcw = FALSE; PrevTensionCw = FALSE; PrevTensionCcw = FALSE;
		PrevFeedHome = FALSE; PrevFeedVacuum = FALSE; PrevFeedClamp = FALSE; PrevTensionClamp = FALSE;
		PrevTableVacuum = FALSE; PrevTableBlower = FALSE; PrevTableCylinder = FALSE;
		PrevUltrasonicDown = FALSE; PrevUltrasonicRun = FALSE; PrevUltrasonicSpeed = FALSE;
		PrevDancerUpper = FALSE; PrevDancerFixer = FALSE;
		PrevAlignStart = FALSE; PrevLaserStart = FALSE; PrevReadStart = FALSE;

		IsFeedCw = FALSE; IsFeedCcw = FALSE; IsTensionCw = FALSE; IsTensionCcw = FALSE;
		IsFeedHome = FALSE; IsFeedVacuum = FALSE; IsFeedClamp = FALSE; IsTensionClamp = FALSE;
		IsTableVacuum = FALSE; IsTableBlower = FALSE; IsTableCylinder = FALSE;
		IsUltrasonicDown = FALSE; IsUltrasonicRun = FALSE; IsUltrasonicSpeed = FALSE;
		IsDancerUpper = FALSE; IsDancerFixer = FALSE;
		IsAlignStart = FALSE; IsLaserStart = FALSE; IsReadStart = FALSE;

		IsPrevFeedCw = FALSE; IsPrevFeedCcw = FALSE; IsPrevTensionCw = FALSE; IsPrevTensionCcw = FALSE;
		IsPrevFeedHome = FALSE; IsPrevFeedVacuum = FALSE; IsPrevFeedClamp = FALSE; IsPrevTensionClamp = FALSE;
		IsPrevTableVacuum = FALSE; IsPrevTableBlower = FALSE; IsPrevTableCylinder = FALSE;
		IsPrevUltrasonicDown = FALSE; IsPrevUltrasonicRun = FALSE; IsPrevUltrasonicSpeed = FALSE;
		IsPrevDancerUpper = FALSE; IsPrevDancerFixer = FALSE;
		IsPrevAlignStart = FALSE; IsPrevLaserStart = FALSE; IsPrevReadStart = FALSE;
	}
};

struct stBtnUncoiler
{
	BOOL ChuckPcb, ChuckPaper, JoinClamp, PcbShaftSupport;
	BOOL EpcActHome, EpcActFirst, EpcActSecond, EpcActThird;
	BOOL PcbInverterCw, PcbInverterCcw, PaperInverterCw, PaperInverterCcw;
	BOOL EpcAuto, EpcManual, EpcIn, EpcOut, EpcHome;
	BOOL DancerUpper, DancerFixer;

	BOOL PrevChuckPcb, PrevChuckPaper, PrevJoinClamp, PrevPcbShaftSupport;
	BOOL PrevEpcActHome, PrevEpcActFirst, PrevEpcActSecond, PrevEpcActThird;
	BOOL PrevPcbInverterCw, PrevPcbInverterCcw, PrevPaperInverterCw, PrevPaperInverterCcw;
	BOOL PrevEpcAuto, PrevEpcManual, PrevEpcIn, PrevEpcOut, PrevEpcHome;
	BOOL PrevDancerUpper, PrevDancerFixer;

	BOOL IsChuckPcb, IsChuckPaper, IsJoinClamp, IsPcbShaftSupport;
	BOOL IsEpcActHome, IsEpcActFirst, IsEpcActSecond, IsEpcActThird;
	BOOL IsPcbInverterCw, IsPcbInverterCcw, IsPaperInverterCw, IsPaperInverterCcw;
	BOOL IsEpcAuto, IsEpcManual, IsEpcIn, IsEpcOut, IsEpcHome;
	BOOL IsDancerUpper, IsDancerFixer;

	BOOL IsPrevChuckPcb, IsPrevChuckPaper, IsPrevJoinClamp, IsPrevPcbShaftSupport;
	BOOL IsPrevEpcActHome, IsPrevEpcActFirst, IsPrevEpcActSecond, IsPrevEpcActThird;
	BOOL IsPrevPcbInverterCw, IsPrevPcbInverterCcw, IsPrevPaperInverterCw, IsPrevPaperInverterCcw;
	BOOL IsPrevEpcAuto, IsPrevEpcManual, IsPrevEpcIn, IsPrevEpcOut, IsPrevEpcHome;
	BOOL IsPrevDancerUpper, IsPrevDancerFixer;

	stBtnUncoiler()
	{
		ChuckPcb = FALSE; ChuckPaper = FALSE; JoinClamp = FALSE; PcbShaftSupport = FALSE;
		EpcActHome = FALSE; EpcActFirst = FALSE; EpcActSecond = FALSE; EpcActThird = FALSE;
		PcbInverterCw = FALSE; PcbInverterCcw = FALSE; PaperInverterCw = FALSE; PaperInverterCcw = FALSE;
		EpcAuto = FALSE; EpcManual = FALSE; EpcIn = FALSE; EpcOut = FALSE; EpcHome = FALSE;
		DancerUpper = FALSE; DancerFixer = FALSE;

		PrevChuckPcb = FALSE; PrevChuckPaper = FALSE; PrevJoinClamp = FALSE; PrevPcbShaftSupport = FALSE;
		PrevEpcActHome = FALSE; PrevEpcActFirst = FALSE; PrevEpcActSecond = FALSE; PrevEpcActThird = FALSE;
		PrevPcbInverterCw = FALSE; PrevPcbInverterCcw = FALSE; PrevPaperInverterCw = FALSE; PrevPaperInverterCcw = FALSE;
		PrevEpcAuto = FALSE; PrevEpcManual = FALSE; PrevEpcIn = FALSE; PrevEpcOut = FALSE; PrevEpcHome = FALSE;
		PrevDancerUpper = FALSE; PrevDancerFixer = FALSE;

		IsChuckPcb = FALSE; IsChuckPaper = FALSE; IsJoinClamp = FALSE; IsPcbShaftSupport = FALSE;
		IsEpcActHome = FALSE; IsEpcActFirst = FALSE; IsEpcActSecond = FALSE; IsEpcActThird = FALSE;
		IsPcbInverterCw = FALSE; IsPcbInverterCcw = FALSE; IsPaperInverterCw = FALSE; IsPaperInverterCcw = FALSE;
		IsEpcAuto = FALSE; IsEpcManual = FALSE; IsEpcIn = FALSE; IsEpcOut = FALSE; IsEpcHome = FALSE;
		IsDancerUpper = FALSE; IsDancerFixer = FALSE;

		IsPrevChuckPcb = FALSE; IsPrevChuckPaper = FALSE; IsPrevJoinClamp = FALSE; IsPrevPcbShaftSupport = FALSE;
		IsPrevEpcActHome = FALSE; IsPrevEpcActFirst = FALSE; IsPrevEpcActSecond = FALSE; IsPrevEpcActThird = FALSE;
		IsPrevPcbInverterCw = FALSE; IsPrevPcbInverterCcw = FALSE; IsPrevPaperInverterCw = FALSE; IsPrevPaperInverterCcw = FALSE;
		IsPrevEpcAuto = FALSE; IsPrevEpcManual = FALSE; IsPrevEpcIn = FALSE; IsPrevEpcOut = FALSE; IsPrevEpcHome = FALSE;
		IsPrevDancerUpper = FALSE; IsPrevDancerFixer = FALSE;
	}
};

struct stBtnEngAuto
{
	BOOL Init, MkSt, OnMking, MkDone, Read2dSt, OnRead2d, Read2dDone, FdDone, GetCurrentInfoSignal, GetMonDispMainSignal;
	BOOL InitF, MkStF, OnMkingF, MkDoneF, Read2dStF, OnRead2dF, Read2dDoneF, FdDoneF, GetCurrentInfoSignalF, GetMonDispMainSignalF;

	BOOL IsInit, IsMkSt, IsOnMking, IsMkDone, IsRead2dSt, IsOnRead2d, IsRead2dDone, IsFdDone, IsGetCurrentInfoSignal, IsGetMonDispMainSignal;
	BOOL IsInitF, IsMkStF, IsOnMkingF, IsMkDoneF, IsRead2dStF, IsOnRead2dF, IsRead2dDoneF, IsFdDoneF, IsGetCurrentInfoSignalF, IsGetMonDispMainSignalF;

	stBtnEngAuto()
	{
		_Init();
	}

	void _Init()
	{
		Init = FALSE; MkSt = FALSE; OnMking = FALSE; MkDone = FALSE; Read2dSt = FALSE; OnRead2d = FALSE; Read2dDone = FALSE; FdDone = FALSE; GetCurrentInfoSignal = FALSE; GetMonDispMainSignal = FALSE;
		InitF = FALSE; MkStF = FALSE; OnMkingF = FALSE; MkDoneF = FALSE; Read2dStF = FALSE; OnRead2dF = FALSE; Read2dDoneF = FALSE; FdDoneF = FALSE; GetCurrentInfoSignalF = FALSE; GetMonDispMainSignalF = FALSE;

		IsInit = FALSE; IsMkSt = FALSE; IsOnMking = FALSE; IsMkDone = FALSE; IsRead2dSt = FALSE; IsOnRead2d = FALSE; IsRead2dDone = FALSE; IsFdDone = FALSE; IsGetCurrentInfoSignal = FALSE; IsGetMonDispMainSignal = FALSE;
		IsInitF = FALSE; IsMkStF = FALSE; IsOnMkingF = FALSE; IsMkDoneF = FALSE; IsRead2dStF = FALSE; IsOnRead2dF = FALSE; IsRead2dDoneF = FALSE; IsFdDoneF = FALSE; IsGetCurrentInfoSignalF = FALSE; IsGetMonDispMainSignalF = FALSE;
	}
};

struct stBtnSettingEng
{
	BOOL OffsetInitPosMove;

	stBtnSettingEng()
	{
		_Init();
	}

	void _Init()
	{
		OffsetInitPosMove = FALSE;
	}
};

struct stBtnStatus
{
	stBtnMsg Msg;
	stBtnMain Main;
	stBtnDisp Disp;
	stBtnTqMotor Tq;
	stBtnInductMotor Induct;
	stBtnCore150 Core150;
	stBtnEtc Etc;
	stBtnRecoiler Rc;
	stBtnPunch Mk;
	stBtnAOIDn AoiDn;
	stBtnAOIUp AoiUp;
	stBtnEngrave Eng;
	stBtnUncoiler Uc;
	stBtnEngAuto EngAuto;
	stBtnSettingEng SettingEng;
	stBtnRunMode RunMode;
	stBtnLaser Lsr;
	stBtnFeedDir FeedDir;
	stBtnDancerRoll Dancer;
};


struct stMenu01Info
{
	CString sOperator, sModel, sLot, sLayerUp, sLayerDn;
	int nTotShot, nVerifyImgNum;
	double dTotWorkRto, dLotWorkRto, dTotSpd, dPartSpd, dDoneLenMk, dDoneLenAoiUp, dDoneLengthAoiDn, dDoneLengthEng;

	stMenu01Info()
	{
		_Init();
	}

	void _Init()
	{
		sOperator = _T(""); sModel = _T(""); sLot = _T(""); sLayerUp = _T(""); sLayerDn = _T("");
		nTotShot = 0; dTotWorkRto = 0.0; dLotWorkRto = 0.0; dTotSpd = 0.0; dPartSpd = 0.0; dDoneLenMk = 0.0;
		dDoneLenAoiUp = 0.0; dDoneLengthAoiDn = 0.0; dDoneLengthEng = 0.0; nVerifyImgNum = 0;
	}

};

struct stMenu01TotalTest
{
	double nUp, nDn, nTotal;

	stMenu01TotalTest()
	{
		_Init();
	}

	void _Init()
	{
		nUp = 0; nDn = 0; nTotal = 0;
	}

};

struct stMenu01GoodRatio
{
	double dUp, dDn, dTotal;

	stMenu01GoodRatio()
	{
		_Init();
	}

	void _Init()
	{
		dUp = 0.0; dDn = 0.0; dTotal = 0.0;
	}

};

struct stMenu01Good
{
	int nUp, nDn, nTotal;

	stMenu01Good()
	{
		_Init();
	}

	void _Init()
	{
		nUp = 0; nDn = 0; nTotal = 0;
	}

};

struct stMenu01BedRatio
{
	double dUp, dDn, dTotal;

	stMenu01BedRatio()
	{
		_Init();
	}

	void _Init()
	{
		dUp = 0.0; dDn = 0.0; dTotal = 0.0;
	}

};

struct stMenu01Bed
{
	int nUp, nDn, nTotal;

	stMenu01Bed()
	{
		_Init();
	}

	void _Init()
	{
		nUp = 0; nDn = 0; nTotal = 0;
	}

};

struct stMenu01YieldTotal
{
	double dUp, dDn, dTotal;

	stMenu01YieldTotal()
	{
		_Init();
	}

	void _Init()
	{
		dUp = 0.0; dDn = 0.0; dTotal = 0.0;
	}

};

struct stMenu01YieldStrip
{
	double dUp, dDn, dTotal;

	stMenu01YieldStrip()
	{
		_Init();
	}

	void _Init()
	{
		dUp = 0.0; dDn = 0.0; dTotal = 0.0;
	}

};

struct stMenu01Defect
{
	int nDefNum[MAX_DEF];

	stMenu01Defect()
	{
		_Init();
	}

	void _Init()
	{
		for (int i = 0; i < MAX_DEF; i++)
			nDefNum[i] = 0;
	}

};

struct stMenu01Data
{
	int nMkNumLf, nMkNumRt;
	double dVerifyLen;

	stMenu01Data()
	{
		_Init();
	}

	void _Init()
	{
		nMkNumLf = 0; nMkNumRt = 0; dVerifyLen = 0.0;
	}

};

struct stMenu01Status
{
	stMenu01Info Info;
	stMenu01TotalTest TotTest;
	stMenu01GoodRatio GoodRto;
	stMenu01Good Good;
	stMenu01BedRatio BedRto;
	stMenu01Bed Bed;
	stMenu01YieldTotal YieldTot;
	stMenu01YieldStrip YieldStrip[MAX_STRIP];
	stMenu01Defect Defect;
	stMenu01Data Data;
};

struct stSerialInfo
{
	int nSerial;
	int nErrorCode; // Digit(4)
	CString sModel, sLayer, sLot, sItsCode;
	int nTotalBadPcs; // Digit(4)
	int *pCamId, *pPcsId, *pDefectPosX, *pDefectPosY, *pDefectNameNum, *pCellId, *pDefImgSize, *pDefImgId, *pPunchCode;

	stSerialInfo()
	{
		_Init();
	}

	~stSerialInfo()
	{
		if (pCamId)
		{
			delete pCamId;
			pCamId = NULL;
		}
		if (pPcsId)
		{
			delete pPcsId;
			pPcsId = NULL;
		}
		if (pDefectPosX)
		{
			delete pDefectPosX;
			pDefectPosX = NULL;
		}
		if (pDefectPosY)
		{
			delete pDefectPosY;
			pDefectPosY = NULL;
		}
		if (pDefectNameNum)
		{
			delete pDefectNameNum;
			pDefectNameNum = NULL;
		}
		if (pCellId)
		{
			delete pCellId;
			pCellId = NULL;
		}
		if (pDefImgSize)
		{
			delete pDefImgSize;
			pDefImgSize = NULL;
		}
		if (pDefImgId)
		{
			delete pDefImgId;
			pDefImgId = NULL;
		}
		if (pPunchCode)
		{
			delete pPunchCode;
			pPunchCode = NULL;
		}
	}

	void _Init()
	{
		nSerial = 0;
		nErrorCode = 1;		// Digit(4)
		sModel = _T(""); sLayer = _T(""); sLot = _T(""); sItsCode = _T("");
		nTotalBadPcs = 0;	// Digit(4)
		pCamId = NULL; pPcsId = NULL; pDefectPosX = NULL; pDefectPosY = NULL; pDefectNameNum = NULL; pCellId = NULL; pDefImgSize = NULL; pDefImgId = NULL; pPunchCode = NULL;
	}

	void _AllocPcr(int nTotPcs)
	{
		nTotalBadPcs = nTotPcs;

		if (nTotalBadPcs > 0)
		{
			//pCamId = new int[nTotalBadPcs];				// Default : 0
			pPcsId = new int[nTotalBadPcs];
			//pDefectPosX = new int[nTotalBadPcs];			// Default : 0
			//pDefectPosY = new int[nTotalBadPcs];			// Default : 0
			pDefectNameNum = new int[nTotalBadPcs];
			//pCellId = new int[nTotalBadPcs];				// Default : 0
			//pDefImgSize = new int[nTotalBadPcs];			// Default : 0
			//pDefImgId = new int[nTotalBadPcs];			// Default : 0
			//pPunchCode = new int[nTotalBadPcs];			// Default : 0
		}
	}
};

struct stReelmapInfo
{
	CString sModel, sLot, sLayer, sLayerUp, sLayerDn, sItsCode, sProcessCode;
	CString sPath;
	int nSerialStart, nSerialEnd;
	int nTotalShot;
	int nNodeX, nNodeY;
	stSerialInfo *pSerial;

	stReelmapInfo()
	{
		_Init();
	}

	~stReelmapInfo()
	{
		if (pSerial)
		{
			delete pSerial;
			pSerial = NULL;
		}
	}

	void _Init()
	{
		pSerial = NULL;
		nTotalShot = 0;
		nNodeX = 0;
		nNodeY = 0;
	}

	void _AllocSerial(int nTotShot)
	{
		nTotalShot = nTotShot;
		if(nTotalShot > 0)
			pSerial = new stSerialInfo[nTotalShot];
	}

};

typedef enum {
	DOOR_FL_MK = 0, DOOR_FR_MK = 1,
	DOOR_BL_MK = 2, DOOR_BR_MK = 3
}  DOOR_MK;
typedef enum {
	DOOR_FM_AOI_UP = 0, DOOR_FL_AOI_UP = 1, DOOR_FR_AOI_UP = 2,
	DOOR_BM_AOI_UP = 3, DOOR_BL_AOI_UP = 4, DOOR_BR_AOI_UP = 5
}  DOOR_AOI_UP;
typedef enum {
	DOOR_FM_AOI_DN = 6, DOOR_FL_AOI_DN = 7, DOOR_FR_AOI_DN = 8,
	DOOR_BM_AOI_DN = 9, DOOR_BL_AOI_DN = 10, DOOR_BR_AOI_DN = 11
}  DOOR_AOI_DN;
typedef enum {
	DOOR_FL_UC = 0, DOOR_FR_UC = 1,
	DOOR_BL_UC = 2, DOOR_BR_UC = 3
}  DOOR_UC;
typedef enum {
	DOOR_FL_RC = 0, DOOR_FR_RC = 1,
	DOOR_S_RC = 2, DOOR_BL_RC = 3, DOOR_BR_RC = 4
}  DOOR_RC;
typedef enum {
	DOOR_FL_ENGV = 0, DOOR_FR_ENGV = 1,
	DOOR_BL_ENGV = 2, DOOR_BR_ENGV = 3
}  DOOR_ENGV;//DOOR_S_ENGV = 2, 

typedef enum { EMG_M_MK = 0, EMG_B_MK = 1 }  EMG_MK;
typedef enum { EMG_F_AOI_UP = 0, EMG_B_AOI_UP = 1 }  EMG_AOI_UP;
typedef enum { EMG_F_AOI_DN = 2, EMG_B_AOI_DN = 3 }  EMG_AOI_DN;
typedef enum { LMT_NEG = 0, LMT_POS = 1 }  SENS_LIMIT;

typedef enum { UNIT_UNCOILER = 0, UNIT_ENGRAVE, UNIT_AOIUP, UNIT_AOIDN, UNIT_PUNCH, UNIT_RECOILER }  UNIT_IDX;

struct stPcrShare
{
	BOOL bExist, bExistF;
	int nSerial;
	CString sModel, sLayer, sLot, sItsCode, sPrcsCode;

	stPcrShare()
	{
		bExist = FALSE;
		bExistF = FALSE;
		nSerial = 0;
		sModel = _T(""); sLayer = _T(""); sLot = _T(""); sItsCode = _T(""); sPrcsCode = _T("");
	}
};

struct stStatus
{
	BOOL bAuto, bManual, bOneCycle;								// Mode 스위치
	BOOL bSwJogLeft, bSwJogFast, bSwJogStep;								// Jog 판넬 선택 스위치
	BOOL bDoorMk[4], bDoorAoi[12];								// 도어 센서
	BOOL bDoorMkF[4], bDoorAoiF[12];							// 도어 센서
	BOOL bEmgMk[2], bEmgAoi[4];									// 비상정지 스위치
	BOOL bEmgUc, bEmgRc;										// 비상정지 스위치
	BOOL bEmgEngv[2], bEmgEngvF[2];								// 비상정지 스위치
	BOOL bEmgMkF[2], bEmgAoiF[4];								// 비상정지 스위치
	BOOL bEmgUcF, bEmgRcF;										// 비상정지 스위치
	BOOL bMainAirMk, bMainAirAoi;								// 메인 에어
	BOOL bSensTblVacMk, bSensTblVacAoi;							// 테이블 진공 센서
	BOOL bSensTblPrsMk, bSensTblPrsAoi;							// 테이블 압력 센서
	BOOL bSensSaftyMk, bSensSaftyAoi;							// 안전 센서
	BOOL bSensSaftyMkF, bSensSaftyAoiF;							// 안전 센서
	BOOL bSensLmtBufMk[2], bSensLmtBufRc[2], bSensLmtBufUc[2];	// 버퍼 롤러 리미트 센서
	BOOL bSigTestDoneAoi, bSigTblAirAoi;						// 검사부 신호
	BOOL bDoorUc[4], bDoorRe[5];								// 도어 센서
	BOOL bDoorUcF[4], bDoorReF[5];								// 도어 센서
	BOOL bDoorEngv[4], bDoorEngvF[4];							// 도어 센서

	stPcrShare PcrShare[2], PcrShareVs[2];

	stStatus()
	{
		bAuto = FALSE; bManual = FALSE; bOneCycle = FALSE; bSwJogLeft = FALSE; bSwJogFast = FALSE; bSwJogStep = FALSE;
		bDoorEngv[0] = FALSE; bDoorEngv[1] = FALSE; bDoorEngv[2] = FALSE; bDoorEngv[3] = FALSE;
		bDoorEngvF[0] = FALSE; bDoorEngvF[1] = FALSE; bDoorEngvF[2] = FALSE; bDoorEngvF[3] = FALSE;
		bDoorMk[0] = FALSE; bDoorMk[1] = FALSE; bDoorMk[2] = FALSE; bDoorMk[3] = FALSE;
		bDoorMkF[0] = FALSE; bDoorMkF[1] = FALSE; bDoorMkF[2] = FALSE; bDoorMkF[3] = FALSE;
		bDoorAoi[0] = FALSE; bDoorAoi[1] = FALSE; bDoorAoi[2] = FALSE; bDoorAoi[3] = FALSE; bDoorAoi[4] = FALSE; bDoorAoi[5] = FALSE;
		bDoorAoiF[0] = FALSE; bDoorAoiF[1] = FALSE; bDoorAoiF[2] = FALSE; bDoorAoiF[3] = FALSE; bDoorAoiF[4] = FALSE; bDoorAoiF[5] = FALSE;
		bEmgMk[0] = FALSE; bEmgMk[1] = FALSE;
		bEmgAoi[0] = FALSE; bEmgAoi[1] = FALSE;
		bEmgUc = FALSE; bEmgRc = FALSE;
		bEmgEngv[0] = FALSE; bEmgEngvF[0] = FALSE;
		bEmgEngv[1] = FALSE; bEmgEngvF[1] = FALSE;
		bEmgMkF[0] = FALSE; bEmgMkF[1] = FALSE;
		bEmgAoiF[0] = FALSE; bEmgAoiF[1] = FALSE;
		bEmgUcF = FALSE; bEmgRcF = FALSE;
		bMainAirMk = FALSE; bMainAirAoi = FALSE;
		bSensTblVacMk = FALSE; bSensTblVacAoi = FALSE;
		bSensTblPrsMk = FALSE; bSensTblPrsAoi = FALSE;
		bSensSaftyMk = FALSE; bSensSaftyAoi = FALSE;
		bSensSaftyMkF = FALSE; bSensSaftyAoiF = FALSE;
		bSensLmtBufMk[0] = FALSE; bSensLmtBufMk[1] = FALSE;
		bSensLmtBufUc[0] = FALSE; bSensLmtBufUc[1] = FALSE;
		bSensLmtBufRc[0] = FALSE; bSensLmtBufRc[1] = FALSE;
		bSigTestDoneAoi = FALSE; bSigTblAirAoi = FALSE;
		bDoorUc[0] = FALSE; bDoorUc[1] = FALSE; bDoorUc[2] = FALSE; bDoorUc[3] = FALSE;
		bDoorUcF[0] = FALSE; bDoorUcF[1] = FALSE; bDoorUcF[2] = FALSE; bDoorUcF[3] = FALSE;
		bDoorRe[0] = FALSE; bDoorRe[1] = FALSE; bDoorRe[2] = FALSE; bDoorRe[3] = FALSE; bDoorRe[4] = FALSE;
		bDoorReF[0] = FALSE; bDoorReF[1] = FALSE; bDoorReF[2] = FALSE; bDoorReF[3] = FALSE; bDoorReF[4] = FALSE;
	}
};

struct stMpeIoWrite
{
	int nIdx;
	CString sAddr;
	long lData;

	stMpeIoWrite()
	{
		nIdx = -1;
		sAddr = _T("");
		lData = 0;
	}
};

struct stPcrMerge
{
	int nIdx, nIdxUp, nIdxDn, nIdxUpInner, nIdxDnInner;

	stPcrMerge()
	{
		nIdx = -1; nIdxUp = -1; nIdxDn = -1; nIdxUpInner = -1; nIdxDnInner = -1;
	}
};

struct stModelInfo
{
	CString sModel, sLayer, sLot, sItsCode;

	stModelInfo()
	{
		sModel = _T("");
		sLayer = _T("");
		sLot = _T("");
		sItsCode = _T("");
	}
};

struct stListBuf
{
	int nTot;
	int nSerial[BUF_SZ];

	stListBuf()
	{
		nTot = 0;
		for (int i = 0; i < BUF_SZ; i++)
			nSerial[BUF_SZ] = 0;
	}

	BOOL stListBuf::Push(int nS)
	{
		if (nS < 1 || (nTot + 1) > BUF_SZ)
			return FALSE;
		nSerial[nTot] = nS;
		nTot++;
		return TRUE;
	}

	int stListBuf::Pop()
	{
		if (nSerial[0] < 1 || (nTot - 1) < 0)
			return 0;

		int nS = nSerial[0];
		for (int i = 0; i < (nTot - 1); i++)
			nSerial[i] = nSerial[i + 1];
		nTot--;
		return nS;
	}

	int stListBuf::GetLast()
	{
		if (nSerial[0] < 1 || (nTot - 1) < 0)
		{
			if (nSerial[0] > 0)
				return nSerial[0];
			else
				return 0;
		}
		return nSerial[nTot - 1];
	}

	void stListBuf::Clear()
	{
		nTot = 0;
		for (int i = 0; i < BUF_SZ; i++)
			nSerial[i] = 0;
	}

};


struct stDlgInfoReg
{
	CString TwoMetal, OneMetal, SampleTest, SampleTestShotNum;
	CString ModeInner, ModeOutter;
	CString FeedCcwRecoiler, FeedCcwUncoiler;
	CString DoorSensorUncoiler, DoorSensorEngrave, DoorSensorAoiUp, DoorSensorAoiDn, DoorSensorPunch, DoorSensorRecoiler;
	CString TotalReelLength, LotLength, LotCutLength, StopLength, UseStopLength, UseLotLength, OneShotLength;
	CString CleanRollerAoiUp, CleanRollerAoiDn, Laser380mm;
	CString UltraSonicEngrave, UltraSonicAoi;
	CString UltraSonicStTimeAoi, Laser346mm, Laser340mm;
	CString SenserSaftyPunch;
	CString UseEngrave, UseAoiUp, UseAoiDn, UsePunch;
	CString UsePunchOnly;

	CString LampCleanRollerAoiUp, LampCleanRollerAoiDn, LampUltraSonicAoi, LampUltraSonicEngrave, LampTwoMetal, LampOneMetal;
	CString LampSampleTest, LampModeInner, LampModeOutter, LampFeedCcwRecoiler, LampFeedCcwUncoiler, LampDoorSensorUncoiler;
	CString LampDoorSensorEngrave, LampDoorSensorAoiUp, LampDoorSensorAoiDn, LampDoorSensorPunch, LampDoorSensorRecoiler;
	CString LampSenserSaftyPunch, LampUseStopLength, LampUseLotLength;

	CString LampLaser340mm, LampLaser346mm, LampLaser380mm;

	stDlgInfoReg()
	{
		TwoMetal = _T("MB400020");
		OneMetal = _T("MB400021");
		SampleTest = _T("MB400022");
		SampleTestShotNum = _T("ML41112");
		//SampleTestShotNum = _T("MW41111");
		ModeInner = _T("MB400023");
		ModeOutter = _T("MB400024");
		FeedCcwRecoiler = _T("MB400051");
		FeedCcwUncoiler = _T("MB400050");
		DoorSensorUncoiler = _T("MB400014");
		DoorSensorEngrave = _T("MB400015");
		DoorSensorAoiUp = _T("MB400016");
		DoorSensorAoiDn = _T("MB400017");
		DoorSensorPunch = _T("MB400018");
		DoorSensorRecoiler = _T("MB400019");
		CleanRollerAoiUp = _T("MB400065");
		CleanRollerAoiDn = _T("MB400066");
		TotalReelLength = _T("ML41000");
		LotLength = _T("ML41002");
		LotCutLength = _T("ML41004");
		StopLength = _T("ML41006");
		UseStopLength = _T("MB40000B");
		UseLotLength = _T("MB40000C");
		OneShotLength = _T("ML41030");
		UltraSonicStTimeAoi = _T("MW41130");
		Laser380mm = _T("MB400054");
		Laser346mm = _T("MB400053");
		Laser340mm = _T("MB400052");
		UltraSonicEngrave = _T("MB400062");
		UltraSonicAoi = _T("MB400067");
		SenserSaftyPunch = _T("MB40001A");

		UseEngrave = _T("MB40009A");
		UseAoiUp = _T("MB40009B");
		UseAoiDn = _T("MB40009C");
		UsePunch = _T("MB40009D");
		UsePunchOnly = _T("MB40009E");

		LampCleanRollerAoiUp = _T("MB400265");
		LampCleanRollerAoiDn = _T("MB400266");
		LampUltraSonicAoi = _T("MB400267");
		LampUltraSonicEngrave = _T("MB400262");
		LampTwoMetal = _T("MB400220");
		LampOneMetal = _T("MB400221");
		LampSampleTest = _T("MB400222");
		LampModeInner = _T("MB400223");
		LampModeOutter = _T("MB400224");
		LampFeedCcwRecoiler = _T("MB400251");
		LampFeedCcwUncoiler = _T("MB400250");
		LampDoorSensorUncoiler = _T("MB400214");
		LampDoorSensorEngrave = _T("MB400215");
		LampDoorSensorAoiUp = _T("MB400216");
		LampDoorSensorAoiDn = _T("MB400217");
		LampDoorSensorPunch = _T("MB400218");
		LampDoorSensorRecoiler = _T("MB400219");
		LampSenserSaftyPunch = _T("MB40021A");
		LampUseStopLength = _T("MB40020B");
		LampUseLotLength = _T("MB40020C");

		LampLaser380mm = _T("MB400254");
		LampLaser346mm = _T("MB400253");
		LampLaser340mm = _T("MB400252");
	}

	void Load()
	{
		TCHAR szData[200];
		CString sMenu = _T("IDD_DLG_INFO");
		CString sPath = PATH_GUI_REGISTER;

		if (0 < ::GetPrivateProfileString(sMenu, _T("TwoMetal"), NULL, szData, sizeof(szData), sPath))
			TwoMetal = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("OneMetal"), NULL, szData, sizeof(szData), sPath))
			OneMetal = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("SampleTest"), NULL, szData, sizeof(szData), sPath))
			SampleTest = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("SampleTestShotNum"), NULL, szData, sizeof(szData), sPath))
			SampleTestShotNum = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ModeInner"), NULL, szData, sizeof(szData), sPath))
			ModeInner = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ModeOutter"), NULL, szData, sizeof(szData), sPath))
			ModeOutter = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCcwRecoiler"), NULL, szData, sizeof(szData), sPath))
			FeedCcwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCcwUncoiler"), NULL, szData, sizeof(szData), sPath))
			FeedCcwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DoorSensorUncoiler"), NULL, szData, sizeof(szData), sPath))
			DoorSensorUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DoorSensorEngrave"), NULL, szData, sizeof(szData), sPath))
			DoorSensorEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DoorSensorAoiUp"), NULL, szData, sizeof(szData), sPath))
			DoorSensorAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DoorSensorAoiDn"), NULL, szData, sizeof(szData), sPath))
			DoorSensorAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DoorSensorPunch"), NULL, szData, sizeof(szData), sPath))
			DoorSensorPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DoorSensorRecoiler"), NULL, szData, sizeof(szData), sPath))
			DoorSensorRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TotalReelLength"), NULL, szData, sizeof(szData), sPath))
			TotalReelLength = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LotLength"), NULL, szData, sizeof(szData), sPath))
			LotLength = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LotCutLength"), NULL, szData, sizeof(szData), sPath))
			LotCutLength = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("StopLength"), NULL, szData, sizeof(szData), sPath))
			StopLength = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UseStopLength"), NULL, szData, sizeof(szData), sPath))
			UseStopLength = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UseLotLength"), NULL, szData, sizeof(szData), sPath))
			UseLotLength = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("OneShotLength"), NULL, szData, sizeof(szData), sPath))
			OneShotLength = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("CleanRollerAoiUp"), NULL, szData, sizeof(szData), sPath))
			CleanRollerAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("CleanRollerAoiDn"), NULL, szData, sizeof(szData), sPath))
			CleanRollerAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Laser380mm"), NULL, szData, sizeof(szData), sPath))
			Laser380mm = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UltraSonicEngrave"), NULL, szData, sizeof(szData), sPath))
			UltraSonicEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UltraSonicAoi"), NULL, szData, sizeof(szData), sPath))
			UltraSonicAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UltraSonicStTimeAoi"), NULL, szData, sizeof(szData), sPath))
			UltraSonicStTimeAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Laser346mm"), NULL, szData, sizeof(szData), sPath))
			Laser346mm = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Laser340mm"), NULL, szData, sizeof(szData), sPath))
			Laser340mm = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("SenserSaftyPunch"), NULL, szData, sizeof(szData), sPath))
			SenserSaftyPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UseEngrave"), NULL, szData, sizeof(szData), sPath))
			UseEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UseAoiUp"), NULL, szData, sizeof(szData), sPath))
			UseAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UseAoiDn"), NULL, szData, sizeof(szData), sPath))
			UseAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UsePunch"), NULL, szData, sizeof(szData), sPath))
			UsePunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UsePunchOnly"), NULL, szData, sizeof(szData), sPath))
			UsePunchOnly = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampCleanRollerAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampCleanRollerAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampCleanRollerAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampCleanRollerAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampUltraSonicAoi"), NULL, szData, sizeof(szData), sPath))
			LampUltraSonicAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampUltraSonicEngrave"), NULL, szData, sizeof(szData), sPath))
			LampUltraSonicEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTwoMetal"), NULL, szData, sizeof(szData), sPath))
			LampTwoMetal = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampOneMetal"), NULL, szData, sizeof(szData), sPath))
			LampOneMetal = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampSampleTest"), NULL, szData, sizeof(szData), sPath))
			LampSampleTest = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampModeInner"), NULL, szData, sizeof(szData), sPath))
			LampModeInner = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampModeOutter"), NULL, szData, sizeof(szData), sPath))
			LampModeOutter = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCcwRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampFeedCcwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCcwUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampFeedCcwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDoorSensorUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampDoorSensorUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDoorSensorEngrave"), NULL, szData, sizeof(szData), sPath))
			LampDoorSensorEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDoorSensorAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampDoorSensorAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDoorSensorAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampDoorSensorAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDoorSensorPunch"), NULL, szData, sizeof(szData), sPath))
			LampDoorSensorPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDoorSensorRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampDoorSensorRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampSenserSaftyPunch"), NULL, szData, sizeof(szData), sPath))
			LampSenserSaftyPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampUseStopLength"), NULL, szData, sizeof(szData), sPath))
			LampUseStopLength = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampUseLotLength"), NULL, szData, sizeof(szData), sPath))
			LampUseLotLength = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampLaser380mm "), NULL, szData, sizeof(szData), sPath))
			LampLaser380mm = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampLaser346mm "), NULL, szData, sizeof(szData), sPath))
			LampLaser346mm = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampLaser340mm "), NULL, szData, sizeof(szData), sPath))
			LampLaser340mm = CString(szData);
	}
};

struct stDlgFrameHighReg
{
	CString FeedOffsetPunch, FeedOffsetAoiUp, FeedOffsetAoiDn, FeedOffsetEngrave;
	CString FeedOffsetWriteDoneAoiUp, FeedOffsetWriteDoneAoiDn, FeedOffsetWriteDoneEngrave, FeedOffsetWriteDonePunch;

	stDlgFrameHighReg()
	{
		FeedOffsetPunch = _T("ML41066");
		FeedOffsetAoiUp = _T("ML41062");
		FeedOffsetAoiDn = _T("ML45064");
		FeedOffsetEngrave = _T("ML41060");
		FeedOffsetWriteDoneAoiUp = _T("MB40024D");
		FeedOffsetWriteDoneAoiDn = _T("MB40024E");
		FeedOffsetWriteDoneEngrave = _T("MB40024C");
		FeedOffsetWriteDonePunch = _T("MB40024F");
	}

	void Load()
	{
		TCHAR szData[200];
		CString sMenu = _T("IDD_DLG_FRAME_HIGH");
		CString sPath = PATH_GUI_REGISTER;

		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOffsetPunch"), NULL, szData, sizeof(szData), sPath))
			FeedOffsetPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOffsetAoiUp"), NULL, szData, sizeof(szData), sPath))
			FeedOffsetAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOffsetAoiDn"), NULL, szData, sizeof(szData), sPath))
			FeedOffsetAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOffsetEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedOffsetEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOffsetWriteDoneAoiUp"), NULL, szData, sizeof(szData), sPath))
			FeedOffsetWriteDoneAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOffsetWriteDoneAoiDn"), NULL, szData, sizeof(szData), sPath))
			FeedOffsetWriteDoneAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOffsetWriteDoneEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedOffsetWriteDoneEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOffsetWriteDonePunch"), NULL, szData, sizeof(szData), sPath))
			FeedOffsetWriteDonePunch = CString(szData);
	}
};

struct stDlgMenu01
{
	CString DoLastJob, LastShotNum, JobEnd, FeedCwPunch, FeedCwAoi, FeedCwEngrave;
	CString FeedTotalLengthPunch, FeedTotalLengthAoiDn, FeedTotalLengthAoiUp, FeedTotalLengthEngrave;
	CString MarkingTotalLeft, MarkingCurrLeft, MarkingTotalRight, MarkingCurrRight, PlcReadyDone;
	CString ResetAoiUp, ResetAoiDn, TestStartAoiUp, TestStartAoiDn, MarkingStart, MarkingDoing, MarkingDone;
	CString JoinJob, JoinCont, JoinContOffset, AutoInitStatus, MarkingInitDone, FeedingDonePunch, TableVacuumAoiUp, LotEndAoiUp;
	CString TableVacuumAoiDn, LotEndAoiDn, MarkingStartEngrave, FeedingDoneEngrave, Reading2dStartEngrave;
	CString MarkingOnEngrave, MarkingDoneEngrave, Reading2dOnEngrave, Reading2dDoneEngrave, ReTestAlarmAoiUp;
	CString ReTestAlarmAoiDn, TensionOnPunch, TensionOnAoi, TensionOnEngrave;
	CString AlarmMonitor, AlarmMonitorRtn, AlarmClear, AlarmClearRtn, AlarmPage;
	CString ProhibitFeedingPunch, MainMc, Buzzer1, Buzzer2;
	CString LampJobEnd, LampDoLastJob;
	CString InitPlcData;


	stDlgMenu01()
	{
		DoLastJob = _T("MB400010");
		LastShotNum = _T("MW41116");
		JoinJob = _T("MB400009");
		JobEnd = _T("MB40000A");
		FeedCwPunch = _T("MB440161");
		FeedCwAoi = _T("MB440160");
		FeedCwEngrave = _T("MB440199");
		FeedTotalLengthPunch = _T("ML41086");
		FeedTotalLengthAoiDn = _T("ML41084");
		FeedTotalLengthAoiUp = _T("ML41082");
		FeedTotalLengthEngrave = _T("ML41080");
		MarkingTotalLeft = _T("ML41072");
		MarkingCurrLeft = _T("ML41074");
		MarkingTotalRight = _T("ML41076");
		MarkingCurrRight = _T("ML41078");
		PlcReadyDone = _T("MB400203");
		ResetAoiUp = _T("MB40011F");
		ResetAoiDn = _T("MB40012F");
		TestStartAoiUp = _T("MB400120");
		TestStartAoiDn = _T("MB400136");
		MarkingStart = _T("MB400350");
		MarkingDoing = _T("MB400151");
		MarkingDone = _T("MB400152");
		JoinCont = _T("MB400190"); 
		JoinContOffset  = _T("ML41070");
		AutoInitStatus = _T("MB400208");
		MarkingInitDone = _T("MB40017D");
		FeedingDonePunch = _T("MB400243");
		TableVacuumAoiUp = _T("MB400113");
		LotEndAoiUp = _T("MB400124");
		TableVacuumAoiDn = _T("MB40012C");
		LotEndAoiDn = _T("MB40013A");
		MarkingStartEngrave = _T("MB400303");
		FeedingDoneEngrave = _T("MB400240");
		Reading2dStartEngrave = _T("MB400306");
		MarkingOnEngrave = _T("MB400104");
		MarkingDoneEngrave = _T("MB400105");
		Reading2dOnEngrave = _T("MB400107");
		Reading2dDoneEngrave = _T("MB400108");
		ReTestAlarmAoiUp = _T("MB40018D");
		ReTestAlarmAoiDn = _T("MB40018E");
		TensionOnPunch = _T("MB40017C");
		TensionOnAoi = _T("MB40017B");
		TensionOnEngrave = _T("MB40017B");
		
		AlarmMonitor = _T("MB400382");
		AlarmMonitorRtn = _T("MB400182");
		AlarmClear = _T("MB400383");
		AlarmClearRtn = _T("MB400183");
		AlarmPage = _T("ML41290");

		ProhibitFeedingPunch = _T("MB40015C");
		MainMc = _T("MB40035F");
		Buzzer1 = _T("MB40001D");
		Buzzer2 = _T("MB40001E");

		InitPlcData = _T("MB400180");

		LampJobEnd = _T("MB400209");
		LampDoLastJob = _T("MB400210");
	}

	void Load()
	{
		TCHAR szData[200];
		CString sMenu = _T("IDD_DLG_MENU_01");
		CString sPath = PATH_GUI_REGISTER;

		if (0 < ::GetPrivateProfileString(sMenu, _T("DoLastJob"), NULL, szData, sizeof(szData), sPath))
			DoLastJob = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LastShotNum"), NULL, szData, sizeof(szData), sPath))
			LastShotNum = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("JobEnd"), NULL, szData, sizeof(szData), sPath))
			JobEnd = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCwPunch"), NULL, szData, sizeof(szData), sPath))
			FeedCwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCwAoi"), NULL, szData, sizeof(szData), sPath))
			FeedCwAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCwEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedCwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedTotalLengthPunch"), NULL, szData, sizeof(szData), sPath))
			FeedTotalLengthPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedTotalLengthAoiDn"), NULL, szData, sizeof(szData), sPath))
			FeedTotalLengthAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedTotalLengthAoiUp"), NULL, szData, sizeof(szData), sPath))
			FeedTotalLengthAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedTotalLengthEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedTotalLengthEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingTotalLeft"), NULL, szData, sizeof(szData), sPath))
			MarkingTotalLeft = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingCurrLeft"), NULL, szData, sizeof(szData), sPath))
			MarkingCurrLeft = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingTotalRight"), NULL, szData, sizeof(szData), sPath))
			MarkingTotalRight = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingCurrRight"), NULL, szData, sizeof(szData), sPath))
			MarkingCurrRight = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PlcReadyDone"), NULL, szData, sizeof(szData), sPath))
			PlcReadyDone = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ResetAoiUp"), NULL, szData, sizeof(szData), sPath))
			ResetAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ResetAoiDn"), NULL, szData, sizeof(szData), sPath))
			ResetAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TestStartAoiUp"), NULL, szData, sizeof(szData), sPath))
			TestStartAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TestStartAoiDn"), NULL, szData, sizeof(szData), sPath))
			TestStartAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingStart"), NULL, szData, sizeof(szData), sPath))
			MarkingStart = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingDoing"), NULL, szData, sizeof(szData), sPath))
			MarkingDoing = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingDone"), NULL, szData, sizeof(szData), sPath))
			MarkingDone = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("InitPlcData"), NULL, szData, sizeof(szData), sPath))
			InitPlcData = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("JoinJob"), NULL, szData, sizeof(szData), sPath))
			JoinJob = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("JoinCont"), NULL, szData, sizeof(szData), sPath))
			JoinCont = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("JoinContOffset"), NULL, szData, sizeof(szData), sPath))
			JoinContOffset = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("AutoInitStatus"), NULL, szData, sizeof(szData), sPath))
			AutoInitStatus = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingInitDone"), NULL, szData, sizeof(szData), sPath))
			MarkingInitDone = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedingDonePunch"), NULL, szData, sizeof(szData), sPath))
			FeedingDonePunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableVacuumAoiUp"), NULL, szData, sizeof(szData), sPath))
			TableVacuumAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LotEndAoiUp"), NULL, szData, sizeof(szData), sPath))
			LotEndAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableVacuumAoiDn"), NULL, szData, sizeof(szData), sPath))
			TableVacuumAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LotEndAoiDn"), NULL, szData, sizeof(szData), sPath))
			LotEndAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingStartEngrave"), NULL, szData, sizeof(szData), sPath))
			MarkingStartEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedingDoneEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedingDoneEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Reading2dStartEngrave"), NULL, szData, sizeof(szData), sPath))
			Reading2dStartEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingOnEngrave"), NULL, szData, sizeof(szData), sPath))
			MarkingOnEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MarkingDoneEngrave"), NULL, szData, sizeof(szData), sPath))
			MarkingDoneEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Reading2dOnEngrave"), NULL, szData, sizeof(szData), sPath))
			Reading2dOnEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Reading2dDoneEngrave"), NULL, szData, sizeof(szData), sPath))
			Reading2dDoneEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ReTestAlarmAoiUp"), NULL, szData, sizeof(szData), sPath))
			ReTestAlarmAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ReTestAlarmAoiDn"), NULL, szData, sizeof(szData), sPath))
			ReTestAlarmAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionOnPunch"), NULL, szData, sizeof(szData), sPath))
			TensionOnPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionOnAoi"), NULL, szData, sizeof(szData), sPath))
			TensionOnAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionOnEngrave"), NULL, szData, sizeof(szData), sPath))
			TensionOnEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("AlarmMonitor"), NULL, szData, sizeof(szData), sPath))
			AlarmMonitor = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("AlarmMonitorRtn"), NULL, szData, sizeof(szData), sPath))
			AlarmMonitorRtn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("AlarmClear"), NULL, szData, sizeof(szData), sPath))
			AlarmClear = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("AlarmClearRtn"), NULL, szData, sizeof(szData), sPath))
			AlarmClearRtn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("AlarmPage"), NULL, szData, sizeof(szData), sPath))
			AlarmPage = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ProhibitFeedingPunch"), NULL, szData, sizeof(szData), sPath))
			ProhibitFeedingPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("MainMc"), NULL, szData, sizeof(szData), sPath))
			MainMc = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Buzzer1"), NULL, szData, sizeof(szData), sPath))
			Buzzer1 = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Buzzer2"), NULL, szData, sizeof(szData), sPath))
			Buzzer2 = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampJobEnd"), NULL, szData, sizeof(szData), sPath))
			LampJobEnd = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDoLastJob"), NULL, szData, sizeof(szData), sPath))
			LampDoLastJob = CString(szData);
	}
};

struct stDlgMenu03
{
	CString Ready, Run, Reset, Stop;
	CString ConnectModule;
	CString ChuckPcbRecoiler, ChuckPaperRecoiler, JoinClampRecoiler, PcbShaftSupportRecoiler;
	CString EpcActHomeRecoiler, EpcActFirstRecoiler, EpcActSecondRecoiler, EpcActThirdRecoiler, EpcAutoRecoiler, EpcManualRecoiler;
	CString EpcHomeRecoiler, EpcInRecoiler, EpcOutRecoiler, PcbInverterCwRecoiler, PcbInverterCcwRecoiler;
	CString PaperInverterCwRecoiler, PaperInverterCcwRecoiler, DancerUpperRecoiler, DancerFixerRecoiler;
	CString FeedCwPunch, FeedCcwPunch, TensionCwPunch, TensionCcwPunch, FeedHomePunch, FeedVacuumPunch, FeedClampPunch, TensionClampPunch; //PcbPushPunch 
	CString TableBlowerPunch, TableVacuumPunch, TableCylinderPunch, TableClampDnPunch, TableClampForwardPunch, PunchStartPunch;
	CString FeedCwAoiDn, FeedCcwAoiDn, TensionCwAoiDn, TensionCcwAoiDn, FeedHomeAoiDn, FeedVacuumAoiDn, TableBlowerAoiDn, TableVacuumAoiDn, TableCylinderAoiDn;
	CString FeedClampAoiDn, TensionClampAoiDn, CleanRollerUpAoiDn, CleanRollerDnAoiDn, CleanRollerPushAoiDn, TestStartAoiDn;
	CString BufferRollerUpAoiDn, BufferRollerFixAoiDn; //PcbPushAoiDn, 
	CString FeedCwAoiUp, FeedCcwAoiUp, TensionCwAoiUp, TensionCcwAoiUp, FeedHomeAoiUp, FeedVacuumAoiUp, TableBlowerAoiUp, TableVacuumAoiUp, TableCylinderAoiUp;
	CString FeedClampAoiUp, TensionClampAoiUp, CleanRollerUpAoiUp, CleanRollerDnAoiUp, CleanRollerPushAoiUp, TestStartAoiUp;
	CString UltrasonicDnAoiUp, UltrasonicRunAoiUp, UltrasonicSpeedAoiUp; // PcbPushAoiUp, 
	CString FeedCwEngrave, FeedCcwEngrave, TensionCwEngrave, TensionCcwEngrave, FeedHomeEngrave, FeedVacuumEngrave, PcbPushEngrave;
	CString TableBlowerEngrave, TableVacuumEngrave, TableCylinderEngrave, FeedClampEngrave, TensionClampEngrave;
	CString UltrasonicDownEngrave, UltrasonicRunEngrave, UltrasonicSpeedEngrave, DancerUpperEngrave, DancerFixerEngrave;
	CString AlignStartEngrave, LaserStartEngrave, ReadStartEngrave;
	CString ChuckPcbUncoiler, DancerUpperUncoiler, DancerFixerUncoiler, JoinClampUncoiler, PcbShaftSupportUncoiler, ChuckPaperUncoiler;
	CString EpcActHomeUncoiler, EpcActFirstUncoiler, EpcActSecondUncoiler, EpcActThirdUncoiler, EpcAutoUncoiler, EpcManualUncoiler;
	CString EpcHomeUncoiler, EpcInUncoiler, EpcOutUncoiler, PcbInverterCwUncoiler, PcbInverterCcwUncoiler;
	CString PaperInverterCwUncoiler, PaperInverterCcwUncoiler;
	CString FeedOnePanel, AllDancerUpDn, AllDancerFixOnOff;

	CString PcrReceivedAoiUp, PcrReceivedAoiDn, PcrReceivedSerialAoiUp, PcrReceivedSerialAoiDn;
	CString FeedingReadyEngrave;
	CString FeedingReadyPunch, PcrMarkedSerialLeft, PcrMarkedSerialRight;

	CString LampAlignDoneEngrave, LampMarkDoneEngrave, LampReadDoneEngrave, LampJobDoneEngrave, LampTestDoneAoiUp;
	CString LampTestDoneAoiDn, LampMarkDonePunch, LampLaser340mm, LampLaser346mm, LampLaser380mm;

	CString LampReady, LampRun, LampReset, LampStop, LampAuto, LampManual, LampConnectModule;
	CString LampFeedOnePanel, LampAllDancerUpDn, LampAllDancerFixOnOff, LampFeedCcwRecoiler, LampFeedCcwUncoiler;

	CString LampChuckPcbRecoiler, LampChuckPaperRecoiler, LampJoinClampRecoiler, LampPcbShaftSupportRecoiler, LampEpcActHomeRecoiler;
	CString LampEpcActFirstRecoiler, LampEpcActSecondRecoiler, LampEpcActThirdRecoiler, LampPcbInverterCwRecoiler, LampPcbInverterCcwRecoiler;
	CString LampPaperInverterCwRecoiler, LampPaperInverterCcwRecoiler, LampDancerUpperRecoiler, LampDancerFixerRecoiler, LampEpcAutoRecoiler;
	CString LampEpcManualRecoiler, LampEpcInRecoiler, LampEpcOutRecoiler, LampEpcHomeRecoiler;

	CString LampFeedCwPunch, LampFeedCcwPunch, LampTensionCwPunch, LampTensionCcwPunch, LampFeedHomePunch, LampFeedVacuumPunch;
	CString LampFeedClampPunch, LampTensionClampPunch, LampTableVacuumPunch, LampTableBlowerPunch, LampTableCylinderPunch;
	CString LampTableClampDnPunch, LampTableClampForwardPunch, LampPunchStartPunch;

	CString LampFeedCwAoiDn, LampFeedCcwAoiDn, LampTensionCwAoiDn, LampTensionCcwAoiDn, LampFeedHomeAoiDn, LampFeedVacuumAoiDn;
	CString LampFeedClampAoiDn, LampTensionClampAoiDn, LampTableVacuumAoiDn, LampTableBlowerAoiDn, LampTableCylinderAoiDn, LampCleanRollerUpAoiDn;
	CString LampCleanRollerDnAoiDn, LampCleanRollerPushAoiDn, LampTestStartAoiDn, LampBufferRollerUpAoiDn, LampBufferRollerFixAoiDn;

	CString LampFeedCwAoiUp, LampFeedCcwAoiUp, LampTensionCwAoiUp, LampTensionCcwAoiUp, LampFeedHomeAoiUp, LampFeedVacuumAoiUp, LampFeedClampAoiUp;
	CString LampTensionClampAoiUp, LampTableVacuumAoiUp, LampTableBlowerAoiUp, LampTableCylinderAoiUp, LampCleanRollerUpAoiUp, LampCleanRollerDnAoiUp;
	CString LampCleanRollerPushAoiUp, LampTestStartAoiUp, LampUltrasonicDnAoiUp, LampUltrasonicRunAoiUp, LampUltrasonicSpeedAoiUp;

	CString LampFeedCwEngrave, LampFeedCcwEngrave, LampTensionCwEngrave, LampTensionCcwEngrave, LampFeedHomeEngrave, LampFeedVacuumEngrave;
	CString LampFeedClampEngrave, LampTensionClampEngrave, LampTableVacuumEngrave, LampTableBlowerEngrave, LampTableCylinderEngrave, LampUltrasonicDownEngrave;
	CString LampUltrasonicRunEngrave, LampUltrasonicSpeedEngrave, LampDancerUpperEngrave, LampDancerFixerEngrave, LampAlignStartEngrave;
	CString LampLaserStartEngrave, LampReadStartEngrave;

	CString LampChuckPcbUncoiler, LampChuckPaperUncoiler, LampJoinClampUncoiler, LampPcbShaftSupportUncoiler, LampEpcActHomeUncoiler, LampEpcActFirstUncoiler;
	CString LampEpcActSecondUncoiler, LampEpcActThirdUncoiler, LampPcbInverterCwUncoiler, LampPcbInverterCcwUncoiler, LampPaperInverterCwUncoiler;
	CString LampPaperInverterCcwUncoiler, LampDancerUpperUncoiler, LampDancerFixerUncoiler, LampEpcAutoUncoiler, LampEpcManualUncoiler;
	CString LampEpcInUncoiler, LampEpcOutUncoiler, LampEpcHomeUncoiler;
	
	stDlgMenu03()
	{
		Ready = _T("MB400003");
		Run = _T("MB400006");
		Reset = _T("MB400182");
		Stop = _T("MB400007");

		PcrReceivedAoiUp = _T("MB400041");
		PcrReceivedAoiDn = _T("MB400042");
		PcrReceivedSerialAoiUp = _T("ML41124");
		PcrReceivedSerialAoiDn = _T("ML41126");

		FeedingReadyEngrave = _T("MB400040");

		FeedingReadyPunch = _T("MB400043");
		PcrMarkedSerialLeft = _T("ML41130");
		PcrMarkedSerialRight = _T("ML41132");


		ConnectModule = _T("MB40000F");

		ChuckPcbRecoiler = _T("MB400160");
		ChuckPaperRecoiler = _T("MB400161");
		JoinClampRecoiler = _T("MB400162");
		PcbShaftSupportRecoiler = _T("MB400163");
		EpcActHomeRecoiler = _T("MB400164");
		EpcActFirstRecoiler = _T("MB400165");
		EpcActSecondRecoiler = _T("MB400166");
		EpcActThirdRecoiler = _T("MB400167");
		EpcAutoRecoiler = _T("MB400169");
		EpcManualRecoiler = _T("MB40016A");
		EpcHomeRecoiler = _T("MB40016B");
		EpcInRecoiler = _T("MB40016C");
		EpcOutRecoiler = _T("MB40016D");
		PcbInverterCwRecoiler = _T("MB400170");
		PcbInverterCcwRecoiler = _T("MB400171");
		PaperInverterCwRecoiler = _T("MB400172");
		PaperInverterCcwRecoiler = _T("MB400173");
		DancerUpperRecoiler = _T("MB400174");
		DancerFixerRecoiler = _T("MB400175");

		FeedCwPunch = _T("MB400140");
		FeedCcwPunch = _T("MB400141");
		TensionCwPunch = _T("MB400142");
		TensionCcwPunch = _T("MB400143");
		FeedHomePunch = _T("MB400144");
		FeedVacuumPunch = _T("MB400145");
		FeedClampPunch = _T("MB400146");
		TensionClampPunch = _T("MB400147");
		TableVacuumPunch = _T("MB400148");
		TableBlowerPunch = _T("MB400149");
		TableCylinderPunch = _T("MB40014A");
		TableClampDnPunch = _T("MB40014B");
		TableClampForwardPunch = _T("MB40014C");
		PunchStartPunch = _T("MB400150");

		FeedCwAoiDn = _T("MB40010A");
		FeedCcwAoiDn = _T("MB40010B");
		TensionCwAoiDn = _T("MB40010C");
		TensionCcwAoiDn = _T("MB40010D");
		FeedHomeAoiDn = _T("MB40010E");
		FeedVacuumAoiDn = _T("MB400129");
		TableBlowerAoiDn = _T("MB40012D");
		TableVacuumAoiDn = _T("MB40012C");
		TableCylinderAoiDn = _T("MB400130");
		FeedClampAoiDn = _T("MB40012A");
		TensionClampAoiDn = _T("MB40012B");
		CleanRollerUpAoiDn = _T("MB400113");
		CleanRollerDnAoiDn = _T("MB400134");
		CleanRollerPushAoiDn = _T("MB400135");
		TestStartAoiDn = _T("MB400136");
		BufferRollerUpAoiDn = _T("MB40013C");
		BufferRollerFixAoiDn = _T("MB40013D");

		FeedCwAoiUp = _T("MB40010A");
		FeedCcwAoiUp = _T("MB40010B");
		TensionCwAoiUp = _T("MB40010C");
		TensionCcwAoiUp = _T("MB40010D");
		FeedHomeAoiUp = _T("MB40010E");
		FeedVacuumAoiUp = _T("MB400129");
		TableBlowerAoiUp = _T("MB400114");
		TableVacuumAoiUp = _T("MB400113");
		TableCylinderAoiUp = _T("MB400115");
		FeedClampAoiUp = _T("MB400111");
		TensionClampAoiUp = _T("MB400112");
		CleanRollerUpAoiUp = _T("MB400118");
		CleanRollerDnAoiUp = _T("MB400119");
		CleanRollerPushAoiUp = _T("MB40011A");
		TestStartAoiUp = _T("MB400120");
		UltrasonicDnAoiUp = _T("MB400126");
		UltrasonicRunAoiUp = _T("MB400127");
		UltrasonicSpeedAoiUp = _T("MB400128");

		FeedCwEngrave = _T("MB400086");
		FeedCcwEngrave = _T("MB400087");
		TensionCwEngrave = _T("MB400088");
		TensionCcwEngrave = _T("MB400089");
		FeedHomeEngrave = _T("MB40008A");
		FeedVacuumEngrave = _T("MB40008B");
		PcbPushEngrave = _T("MB005516");
		TableBlowerEngrave = _T("MB400091");
		TableVacuumEngrave = _T("MB400090");
		TableCylinderEngrave = _T("MB400092");
		FeedClampEngrave = _T("MB40008C");
		TensionClampEngrave = _T("MB40008D");
		UltrasonicDownEngrave = _T("MB400093");
		UltrasonicRunEngrave = _T("MB400094");
		UltrasonicSpeedEngrave = _T("MB400095");
		DancerUpperEngrave = _T("MB400096");
		DancerFixerEngrave = _T("MB400097");
		AlignStartEngrave = _T("MB400100");
		LaserStartEngrave = _T("MB400103");
		ReadStartEngrave = _T("MB400106");

		ChuckPcbUncoiler = _T("MB400070");
		DancerUpperUncoiler = _T("MB400084");
		DancerFixerUncoiler = _T("MB400085");
		JoinClampUncoiler = _T("MB400072");
		PcbShaftSupportUncoiler = _T("MB400073");
		ChuckPaperUncoiler = _T("MB400071");
		EpcActHomeUncoiler = _T("MB400074");
		EpcActFirstUncoiler = _T("MB400075");
		EpcActSecondUncoiler = _T("MB400076");
		EpcActThirdUncoiler = _T("MB400077");
		EpcAutoUncoiler = _T("MB400079");
		EpcManualUncoiler = _T("MB40007A");
		EpcHomeUncoiler = _T("MB40007B");
		EpcInUncoiler = _T("MB40007C");
		EpcOutUncoiler = _T("MB40007D");
		PcbInverterCwUncoiler = _T("MB400080");
		PcbInverterCcwUncoiler = _T("MB400081");
		PaperInverterCwUncoiler = _T("MB400082");
		PaperInverterCcwUncoiler = _T("MB400083");

		FeedOnePanel = _T("MB40000E");
		AllDancerUpDn  = _T("MB40005B");
		AllDancerFixOnOff = _T("MB40005C");

		LampAlignDoneEngrave = _T("MB400244");
		LampMarkDoneEngrave = _T("MB400245");
		LampReadDoneEngrave = _T("MB400246");
		LampJobDoneEngrave = _T("MB400247");
		LampTestDoneAoiUp = _T("MB400248");
		LampTestDoneAoiDn = _T("MB400249");
		LampMarkDonePunch = _T("MB40024A");
		LampLaser340mm = _T("MB400252");
		LampLaser346mm = _T("MB400253");
		LampLaser380mm = _T("MB400254");

		LampReady = _T("MB400203");
		LampRun = _T("MB400206");
		LampReset = _T("MB400382");
		LampStop = _T("MB400207");
		LampAuto = _T("MB400205");
		LampManual = _T("MB400201");
		LampConnectModule = _T("MB40020F");
		LampFeedOnePanel = _T("MB40020E");
		LampAllDancerUpDn = _T("MB40025B");
		LampAllDancerFixOnOff = _T("MB40025C");
		LampFeedCcwRecoiler = _T("MB400251");
		LampFeedCcwUncoiler = _T("MB400250");

		LampChuckPcbRecoiler = _T("MB400360");
		LampChuckPaperRecoiler = _T("MB400361");
		LampJoinClampRecoiler = _T("MB400362");
		LampPcbShaftSupportRecoiler = _T("MB400363");
		LampEpcActHomeRecoiler = _T("MB400364");
		LampEpcActFirstRecoiler = _T("MB400365");
		LampEpcActSecondRecoiler = _T("MB400366");
		LampEpcActThirdRecoiler = _T("MB400367");
		LampPcbInverterCwRecoiler = _T("MB400370");
		LampPcbInverterCcwRecoiler = _T("MB400371");
		LampPaperInverterCwRecoiler = _T("MB400372");
		LampPaperInverterCcwRecoiler = _T("MB400373");
		LampDancerUpperRecoiler = _T("MB400374");
		LampDancerFixerRecoiler = _T("MB400375");
		LampEpcAutoRecoiler = _T("MB400369");
		LampEpcManualRecoiler = _T("MB40036A");
		LampEpcInRecoiler = _T("MB40036C");
		LampEpcOutRecoiler = _T("MB40036D");
		LampEpcHomeRecoiler = _T("MB40036B");

		LampFeedCwPunch = _T("MB400340");
		LampFeedCcwPunch = _T("MB400341");
		LampTensionCwPunch = _T("MB400342");
		LampTensionCcwPunch = _T("MB400343");
		LampFeedHomePunch = _T("MB400344");
		LampFeedVacuumPunch = _T("MB400345");
		LampFeedClampPunch = _T("MB400346");
		LampTensionClampPunch = _T("MB400347");
		LampTableVacuumPunch = _T("MB400348");
		LampTableBlowerPunch = _T("MB400349");
		LampTableCylinderPunch = _T("MB40034A");
		LampTableClampDnPunch = _T("MB40034B");
		LampTableClampForwardPunch = _T("MB40034C");
		LampPunchStartPunch = _T("MB400350");

		LampFeedCwAoiDn = _T("MB40030A");
		LampFeedCcwAoiDn = _T("MB40030B");
		LampTensionCwAoiDn = _T("MB40030C");
		LampTensionCcwAoiDn = _T("MB40030D");
		LampFeedHomeAoiDn = _T("MB40030E");
		LampFeedVacuumAoiDn = _T("MB400329");
		LampFeedClampAoiDn = _T("MB40032A");
		LampTensionClampAoiDn = _T("MB40032B");
		LampTableVacuumAoiDn = _T("MB40032C");
		LampTableBlowerAoiDn = _T("MB40032D");
		LampTableCylinderAoiDn = _T("MB400330");
		LampCleanRollerUpAoiDn = _T("MB400333");
		LampCleanRollerDnAoiDn = _T("MB400334");
		LampCleanRollerPushAoiDn = _T("MB400335");
		LampTestStartAoiDn = _T("MB400336");
		LampBufferRollerUpAoiDn = _T("MB40033C");
		LampBufferRollerFixAoiDn = _T("MB40033D");

		LampFeedCwAoiUp = _T("MB40030A");
		LampFeedCcwAoiUp = _T("MB40030B");
		LampTensionCwAoiUp = _T("MB40030C");
		LampTensionCcwAoiUp = _T("MB40030D");
		LampFeedHomeAoiUp = _T("MB40030E");
		LampFeedVacuumAoiUp = _T("MB400329");
		LampFeedClampAoiUp = _T("MB400311");
		LampTensionClampAoiUp = _T("MB400312");
		LampTableVacuumAoiUp = _T("MB400313");
		LampTableBlowerAoiUp = _T("MB400314");
		LampTableCylinderAoiUp = _T("MB400315");
		LampCleanRollerUpAoiUp = _T("MB400318");
		LampCleanRollerDnAoiUp = _T("MB400319");
		LampCleanRollerPushAoiUp = _T("MB40031A");
		LampTestStartAoiUp = _T("MB400320");
		LampUltrasonicDnAoiUp = _T("MB400326");
		LampUltrasonicRunAoiUp = _T("MB400327");
		LampUltrasonicSpeedAoiUp = _T("MB400328");

		LampFeedCwEngrave = _T("MB400286");
		LampFeedCcwEngrave = _T("MB400287");
		LampTensionCwEngrave = _T("MB400288");
		LampTensionCcwEngrave = _T("MB400289");
		LampFeedHomeEngrave = _T("MB40028A");
		LampFeedVacuumEngrave = _T("MB40028B");
		LampFeedClampEngrave = _T("MB40028C");
		LampTensionClampEngrave = _T("MB40028D");
		LampTableVacuumEngrave = _T("MB400290");
		LampTableBlowerEngrave = _T("MB400291");
		LampTableCylinderEngrave = _T("MB400292");
		LampUltrasonicDownEngrave = _T("MB400293");
		LampUltrasonicRunEngrave = _T("MB400294");
		LampUltrasonicSpeedEngrave = _T("MB400295");
		LampDancerUpperEngrave = _T("MB400296");
		LampDancerFixerEngrave = _T("MB400297");
		LampAlignStartEngrave = _T("MB400300");
		LampLaserStartEngrave = _T("MB400303");
		LampReadStartEngrave = _T("MB400306");

		LampChuckPcbUncoiler = _T("MB400270");
		LampChuckPaperUncoiler = _T("MB400271");
		LampJoinClampUncoiler = _T("MB400272");
		LampPcbShaftSupportUncoiler = _T("MB400273");
		LampEpcActHomeUncoiler = _T("MB400274");
		LampEpcActFirstUncoiler = _T("MB400275");
		LampEpcActSecondUncoiler = _T("MB400276");
		LampEpcActThirdUncoiler = _T("MB400277");
		LampPcbInverterCwUncoiler = _T("MB400280");
		LampPcbInverterCcwUncoiler = _T("MB400281");
		LampPaperInverterCwUncoiler = _T("MB400282");
		LampPaperInverterCcwUncoiler = _T("MB400283");
		LampDancerUpperUncoiler = _T("MB400284");
		LampDancerFixerUncoiler = _T("MB400285");
		LampEpcAutoUncoiler = _T("MB400279");
		LampEpcManualUncoiler = _T("MB40027A");
		LampEpcInUncoiler = _T("MB40027C");
		LampEpcOutUncoiler = _T("MB40027D");
		LampEpcHomeUncoiler = _T("MB40027B");
	}

	void Load()
	{
		TCHAR szData[200];
		CString sMenu = _T("IDD_DLG_MENU_03");
		CString sPath = PATH_GUI_REGISTER;

		if (0 < ::GetPrivateProfileString(sMenu, _T("Ready"), NULL, szData, sizeof(szData), sPath))
			Ready = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Run"), NULL, szData, sizeof(szData), sPath))
			Run = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Reset"), NULL, szData, sizeof(szData), sPath))
			Reset = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("Stop"), NULL, szData, sizeof(szData), sPath))
			Stop = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ConnectModule"), NULL, szData, sizeof(szData), sPath))
			ConnectModule = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ChuckPcbRecoiler"), NULL, szData, sizeof(szData), sPath))
			ChuckPcbRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ChuckPaperRecoiler"), NULL, szData, sizeof(szData), sPath))
			ChuckPaperRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("JoinClampRecoiler"), NULL, szData, sizeof(szData), sPath))
			JoinClampRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcbShaftSupportRecoiler"), NULL, szData, sizeof(szData), sPath))
			PcbShaftSupportRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcActHomeRecoiler"), NULL, szData, sizeof(szData), sPath))
			EpcActHomeRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcActFirstRecoiler"), NULL, szData, sizeof(szData), sPath))
			EpcActFirstRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcActSecondRecoiler"), NULL, szData, sizeof(szData), sPath))
			EpcActSecondRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcActThirdRecoiler"), NULL, szData, sizeof(szData), sPath))
			EpcActThirdRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcAutoRecoiler"), NULL, szData, sizeof(szData), sPath))
			EpcAutoRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcManualRecoiler"), NULL, szData, sizeof(szData), sPath))
			EpcManualRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcHomeRecoiler"), NULL, szData, sizeof(szData), sPath))
			EpcHomeRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcInRecoiler"), NULL, szData, sizeof(szData), sPath))
			EpcInRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcOutRecoiler"), NULL, szData, sizeof(szData), sPath))
			EpcOutRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcbInverterCwRecoiler"), NULL, szData, sizeof(szData), sPath))
			PcbInverterCwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcbInverterCcwRecoiler"), NULL, szData, sizeof(szData), sPath))
			PcbInverterCcwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PaperInverterCwRecoiler"), NULL, szData, sizeof(szData), sPath))
			PaperInverterCwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PaperInverterCcwRecoiler"), NULL, szData, sizeof(szData), sPath))
			PaperInverterCcwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DancerUpperRecoiler"), NULL, szData, sizeof(szData), sPath))
			DancerUpperRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DancerFixerRecoiler"), NULL, szData, sizeof(szData), sPath))
			DancerFixerRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCwPunch"), NULL, szData, sizeof(szData), sPath))
			FeedCwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCcwPunch"), NULL, szData, sizeof(szData), sPath))
			FeedCcwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionCwPunch"), NULL, szData, sizeof(szData), sPath))
			TensionCwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionCcwPunch"), NULL, szData, sizeof(szData), sPath))
			TensionCcwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedHomePunch"), NULL, szData, sizeof(szData), sPath))
			FeedHomePunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedVacuumPunch"), NULL, szData, sizeof(szData), sPath))
			FeedVacuumPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedClampPunch"), NULL, szData, sizeof(szData), sPath))
			FeedClampPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionClampPunch"), NULL, szData, sizeof(szData), sPath))
			TensionClampPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableBlowerPunch"), NULL, szData, sizeof(szData), sPath))
			TableBlowerPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableVacuumPunch"), NULL, szData, sizeof(szData), sPath))
			TableVacuumPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableCylinderPunch"), NULL, szData, sizeof(szData), sPath))
			TableCylinderPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableClampDnPunch"), NULL, szData, sizeof(szData), sPath))
			TableClampDnPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableClampForwardPunch"), NULL, szData, sizeof(szData), sPath))
			TableClampForwardPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PunchStartPunch"), NULL, szData, sizeof(szData), sPath))
			PunchStartPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCwAoiDn"), NULL, szData, sizeof(szData), sPath))
			FeedCwAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCcwAoiDn"), NULL, szData, sizeof(szData), sPath))
			FeedCcwAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionCwAoiDn"), NULL, szData, sizeof(szData), sPath))
			TensionCwAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionCcwAoiDn"), NULL, szData, sizeof(szData), sPath))
			TensionCcwAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedHomeAoiDn"), NULL, szData, sizeof(szData), sPath))
			FeedHomeAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedVacuumAoiDn"), NULL, szData, sizeof(szData), sPath))
			FeedVacuumAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableBlowerAoiDn"), NULL, szData, sizeof(szData), sPath))
			TableBlowerAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableVacuumAoiDn"), NULL, szData, sizeof(szData), sPath))
			TableVacuumAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableCylinderAoiDn"), NULL, szData, sizeof(szData), sPath))
			TableCylinderAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedClampAoiDn"), NULL, szData, sizeof(szData), sPath))
			FeedClampAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionClampAoiDn"), NULL, szData, sizeof(szData), sPath))
			TensionClampAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("CleanRollerUpAoiDn"), NULL, szData, sizeof(szData), sPath))
			CleanRollerUpAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("CleanRollerDnAoiDn"), NULL, szData, sizeof(szData), sPath))
			CleanRollerDnAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("CleanRollerPushAoiDn"), NULL, szData, sizeof(szData), sPath))
			CleanRollerPushAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TestStartAoiDn"), NULL, szData, sizeof(szData), sPath))
			TestStartAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("BufferRollerUpAoiDn"), NULL, szData, sizeof(szData), sPath))
			BufferRollerUpAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("BufferRollerFixAoiDn"), NULL, szData, sizeof(szData), sPath))
			BufferRollerFixAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCwAoiUp"), NULL, szData, sizeof(szData), sPath))
			FeedCwAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCcwAoiUp"), NULL, szData, sizeof(szData), sPath))
			FeedCcwAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionCwAoiUp"), NULL, szData, sizeof(szData), sPath))
			TensionCwAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionCcwAoiUp"), NULL, szData, sizeof(szData), sPath))
			TensionCcwAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedHomeAoiUp"), NULL, szData, sizeof(szData), sPath))
			FeedHomeAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedVacuumAoiUp"), NULL, szData, sizeof(szData), sPath))
			FeedVacuumAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableBlowerAoiUp"), NULL, szData, sizeof(szData), sPath))
			TableBlowerAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableVacuumAoiUp"), NULL, szData, sizeof(szData), sPath))
			TableVacuumAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableCylinderAoiUp"), NULL, szData, sizeof(szData), sPath))
			TableCylinderAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedClampAoiUp"), NULL, szData, sizeof(szData), sPath))
			FeedClampAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionClampAoiUp"), NULL, szData, sizeof(szData), sPath))
			TensionClampAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("CleanRollerUpAoiUp"), NULL, szData, sizeof(szData), sPath))
			CleanRollerUpAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("CleanRollerDnAoiUp"), NULL, szData, sizeof(szData), sPath))
			CleanRollerDnAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("CleanRollerPushAoiUp"), NULL, szData, sizeof(szData), sPath))
			CleanRollerPushAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TestStartAoiUp"), NULL, szData, sizeof(szData), sPath))
			TestStartAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UltrasonicDnAoiUp"), NULL, szData, sizeof(szData), sPath))
			UltrasonicDnAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UltrasonicRunAoiUp"), NULL, szData, sizeof(szData), sPath))
			UltrasonicRunAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UltrasonicSpeedAoiUp"), NULL, szData, sizeof(szData), sPath))
			UltrasonicSpeedAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCwEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedCwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedCcwEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedCcwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionCwEngrave"), NULL, szData, sizeof(szData), sPath))
			TensionCwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionCcwEngrave"), NULL, szData, sizeof(szData), sPath))
			TensionCcwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedHomeEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedHomeEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedVacuumEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedVacuumEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcbPushEngrave"), NULL, szData, sizeof(szData), sPath))
			PcbPushEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableBlowerEngrave"), NULL, szData, sizeof(szData), sPath))
			TableBlowerEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableVacuumEngrave"), NULL, szData, sizeof(szData), sPath))
			TableVacuumEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TableCylinderEngrave"), NULL, szData, sizeof(szData), sPath))
			TableCylinderEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedClampEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedClampEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("TensionClampEngrave"), NULL, szData, sizeof(szData), sPath))
			TensionClampEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UltrasonicDownEngrave"), NULL, szData, sizeof(szData), sPath))
			UltrasonicDownEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UltrasonicRunEngrave"), NULL, szData, sizeof(szData), sPath))
			UltrasonicRunEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("UltrasonicSpeedEngrave"), NULL, szData, sizeof(szData), sPath))
			UltrasonicSpeedEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DancerUpperEngrave"), NULL, szData, sizeof(szData), sPath))
			DancerUpperEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DancerFixerEngrave"), NULL, szData, sizeof(szData), sPath))
			DancerFixerEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("AlignStartEngrave"), NULL, szData, sizeof(szData), sPath))
			AlignStartEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LaserStartEngrave"), NULL, szData, sizeof(szData), sPath))
			LaserStartEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ReadStartEngrave"), NULL, szData, sizeof(szData), sPath))
			ReadStartEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ChuckPcbUncoiler"), NULL, szData, sizeof(szData), sPath))
			ChuckPcbUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DancerUpperUncoiler"), NULL, szData, sizeof(szData), sPath))
			DancerUpperUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("DancerFixerUncoiler"), NULL, szData, sizeof(szData), sPath))
			DancerFixerUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("JoinClampUncoiler"), NULL, szData, sizeof(szData), sPath))
			JoinClampUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcbShaftSupportUncoiler"), NULL, szData, sizeof(szData), sPath))
			PcbShaftSupportUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ChuckPaperUncoiler"), NULL, szData, sizeof(szData), sPath))
			ChuckPaperUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcActHomeUncoiler"), NULL, szData, sizeof(szData), sPath))
			EpcActHomeUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcActFirstUncoiler"), NULL, szData, sizeof(szData), sPath))
			EpcActFirstUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcActSecondUncoiler"), NULL, szData, sizeof(szData), sPath))
			EpcActSecondUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcActThirdUncoiler"), NULL, szData, sizeof(szData), sPath))
			EpcActThirdUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcAutoUncoiler"), NULL, szData, sizeof(szData), sPath))
			EpcAutoUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcManualUncoiler"), NULL, szData, sizeof(szData), sPath))
			EpcManualUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcHomeUncoiler"), NULL, szData, sizeof(szData), sPath))
			EpcHomeUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcInUncoiler"), NULL, szData, sizeof(szData), sPath))
			EpcInUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("EpcOutUncoiler"), NULL, szData, sizeof(szData), sPath))
			EpcOutUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcbInverterCwUncoiler"), NULL, szData, sizeof(szData), sPath))
			PcbInverterCwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcbInverterCcwUncoiler"), NULL, szData, sizeof(szData), sPath))
			PcbInverterCcwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PaperInverterCwUncoiler"), NULL, szData, sizeof(szData), sPath))
			PaperInverterCwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PaperInverterCcwUncoiler"), NULL, szData, sizeof(szData), sPath))
			PaperInverterCcwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOnePanel"), NULL, szData, sizeof(szData), sPath))
			FeedOnePanel = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("AllDancerUpDn"), NULL, szData, sizeof(szData), sPath))
			AllDancerUpDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("AllDancerFixOnOff"), NULL, szData, sizeof(szData), sPath))
			AllDancerFixOnOff = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcrReceivedAoiUp"), NULL, szData, sizeof(szData), sPath))
			PcrReceivedAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcrReceivedAoiDn"), NULL, szData, sizeof(szData), sPath))
			PcrReceivedAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcrReceivedSerialAoiUp"), NULL, szData, sizeof(szData), sPath))
			PcrReceivedSerialAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcrReceivedSerialAoiDn"), NULL, szData, sizeof(szData), sPath))
			PcrReceivedSerialAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedingReadyEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedingReadyEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedingReadyPunch"), NULL, szData, sizeof(szData), sPath))
			FeedingReadyPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcrMarkedSerialLeft"), NULL, szData, sizeof(szData), sPath))
			PcrMarkedSerialLeft = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("PcrMarkedSerialRight"), NULL, szData, sizeof(szData), sPath))
			PcrMarkedSerialRight = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampAlignDoneEngrave"), NULL, szData, sizeof(szData), sPath))
			LampAlignDoneEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampMarkDoneEngrave"), NULL, szData, sizeof(szData), sPath))
			LampMarkDoneEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampReadDoneEngrave"), NULL, szData, sizeof(szData), sPath))
			LampReadDoneEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampJobDoneEngrave"), NULL, szData, sizeof(szData), sPath))
			LampJobDoneEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTestDoneAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampTestDoneAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTestDoneAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampTestDoneAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampMarkDonePunch"), NULL, szData, sizeof(szData), sPath))
			LampMarkDonePunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampLaser340mm"), NULL, szData, sizeof(szData), sPath))
			LampLaser340mm = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampLaser346mm"), NULL, szData, sizeof(szData), sPath))
			LampLaser346mm = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampLaser380mm"), NULL, szData, sizeof(szData), sPath))
			LampLaser380mm = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampReady"), NULL, szData, sizeof(szData), sPath))
			LampReady = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampRun"), NULL, szData, sizeof(szData), sPath))
			LampRun = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampReset"), NULL, szData, sizeof(szData), sPath))
			LampReset = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampStop"), NULL, szData, sizeof(szData), sPath))
			LampStop = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampAuto"), NULL, szData, sizeof(szData), sPath))
			LampAuto = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampManual"), NULL, szData, sizeof(szData), sPath))
			LampManual = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampConnectModule"), NULL, szData, sizeof(szData), sPath))
			LampConnectModule = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedOnePanel"), NULL, szData, sizeof(szData), sPath))
			LampFeedOnePanel = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampAllDancerUpDn"), NULL, szData, sizeof(szData), sPath))
			LampAllDancerUpDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampAllDancerFixOnOff"), NULL, szData, sizeof(szData), sPath))
			LampAllDancerFixOnOff = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCcwRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampFeedCcwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCcwUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampFeedCcwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampChuckPcbRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampChuckPcbRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampChuckPaperRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampChuckPaperRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampJoinClampRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampJoinClampRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPcbShaftSupportRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampPcbShaftSupportRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcActHomeRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcActHomeRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcActFirstRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcActFirstRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcActSecondRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcActSecondRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcActThirdRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcActThirdRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPcbInverterCwRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampPcbInverterCwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPcbInverterCcwRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampPcbInverterCcwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPaperInverterCwRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampPaperInverterCwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPaperInverterCcwRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampPaperInverterCcwRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDancerUpperRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampDancerUpperRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDancerFixerRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampDancerFixerRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcAutoRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcAutoRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcManualRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcManualRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcInRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcInRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcOutRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcOutRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcHomeRecoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcHomeRecoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCwPunch"), NULL, szData, sizeof(szData), sPath))
			LampFeedCwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCcwPunch"), NULL, szData, sizeof(szData), sPath))
			LampFeedCcwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionCwPunch"), NULL, szData, sizeof(szData), sPath))
			LampTensionCwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionCcwPunch"), NULL, szData, sizeof(szData), sPath))
			LampTensionCcwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedHomePunch"), NULL, szData, sizeof(szData), sPath))
			LampFeedHomePunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedVacuumPunch"), NULL, szData, sizeof(szData), sPath))
			LampFeedVacuumPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedClampPunch"), NULL, szData, sizeof(szData), sPath))
			LampFeedClampPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionClampPunch"), NULL, szData, sizeof(szData), sPath))
			LampTensionClampPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableVacuumPunch"), NULL, szData, sizeof(szData), sPath))
			LampTableVacuumPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableBlowerPunch"), NULL, szData, sizeof(szData), sPath))
			LampTableBlowerPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableCylinderPunch"), NULL, szData, sizeof(szData), sPath))
			LampTableCylinderPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableClampDnPunch"), NULL, szData, sizeof(szData), sPath))
			LampTableClampDnPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableClampForwardPunch"), NULL, szData, sizeof(szData), sPath))
			LampTableClampForwardPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPunchStartPunch"), NULL, szData, sizeof(szData), sPath))
			LampPunchStartPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCwAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampFeedCwAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCcwAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampFeedCcwAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionCwAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampTensionCwAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionCcwAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampTensionCcwAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedHomeAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampFeedHomeAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedVacuumAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampFeedVacuumAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedClampAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampFeedClampAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionClampAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampTensionClampAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableVacuumAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampTableVacuumAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableBlowerAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampTableBlowerAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableCylinderAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampTableCylinderAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampCleanRollerUpAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampCleanRollerUpAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampCleanRollerDnAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampCleanRollerDnAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampCleanRollerPushAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampCleanRollerPushAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTestStartAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampTestStartAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampBufferRollerUpAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampBufferRollerUpAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampBufferRollerFixAoiDn"), NULL, szData, sizeof(szData), sPath))
			LampBufferRollerFixAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCwAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampFeedCwAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCcwAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampFeedCcwAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionCwAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampTensionCwAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionCcwAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampTensionCcwAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedHomeAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampFeedHomeAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedVacuumAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampFeedVacuumAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedClampAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampFeedClampAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionClampAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampTensionClampAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableVacuumAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampTableVacuumAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableBlowerAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampTableBlowerAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableCylinderAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampTableCylinderAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampCleanRollerUpAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampCleanRollerUpAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampCleanRollerDnAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampCleanRollerDnAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampCleanRollerPushAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampCleanRollerPushAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTestStartAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampTestStartAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampUltrasonicDnAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampUltrasonicDnAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampUltrasonicRunAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampUltrasonicRunAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampUltrasonicSpeedAoiUp"), NULL, szData, sizeof(szData), sPath))
			LampUltrasonicSpeedAoiUp = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCwEngrave"), NULL, szData, sizeof(szData), sPath))
			LampFeedCwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedCcwEngrave"), NULL, szData, sizeof(szData), sPath))
			LampFeedCcwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionCwEngrave"), NULL, szData, sizeof(szData), sPath))
			LampTensionCwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionCcwEngrave"), NULL, szData, sizeof(szData), sPath))
			LampTensionCcwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedHomeEngrave"), NULL, szData, sizeof(szData), sPath))
			LampFeedHomeEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedVacuumEngrave"), NULL, szData, sizeof(szData), sPath))
			LampFeedVacuumEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampFeedClampEngrave"), NULL, szData, sizeof(szData), sPath))
			LampFeedClampEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTensionClampEngrave"), NULL, szData, sizeof(szData), sPath))
			LampTensionClampEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableVacuumEngrave"), NULL, szData, sizeof(szData), sPath))
			LampTableVacuumEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableBlowerEngrave"), NULL, szData, sizeof(szData), sPath))
			LampTableBlowerEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampTableCylinderEngrave"), NULL, szData, sizeof(szData), sPath))
			LampTableCylinderEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampUltrasonicDownEngrave"), NULL, szData, sizeof(szData), sPath))
			LampUltrasonicDownEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampUltrasonicRunEngrave"), NULL, szData, sizeof(szData), sPath))
			LampUltrasonicRunEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampUltrasonicSpeedEngrave"), NULL, szData, sizeof(szData), sPath))
			LampUltrasonicSpeedEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDancerUpperEngrave"), NULL, szData, sizeof(szData), sPath))
			LampDancerUpperEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDancerFixerEngrave"), NULL, szData, sizeof(szData), sPath))
			LampDancerFixerEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampAlignStartEngrave"), NULL, szData, sizeof(szData), sPath))
			LampAlignStartEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampLaserStartEngrave"), NULL, szData, sizeof(szData), sPath))
			LampLaserStartEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampReadStartEngrave"), NULL, szData, sizeof(szData), sPath))
			LampReadStartEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampChuckPcbUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampChuckPcbUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampChuckPaperUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampChuckPaperUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampJoinClampUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampJoinClampUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPcbShaftSupportUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampPcbShaftSupportUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcActHomeUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcActHomeUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcActFirstUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcActFirstUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcActSecondUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcActSecondUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcActThirdUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcActThirdUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPcbInverterCwUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampPcbInverterCwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPcbInverterCcwUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampPcbInverterCcwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPaperInverterCwUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampPaperInverterCwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampPaperInverterCcwUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampPaperInverterCcwUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDancerUpperUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampDancerUpperUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampDancerFixerUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampDancerFixerUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcAutoUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcAutoUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcManualUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcManualUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcInUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcInUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcOutUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcOutUncoiler = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("LampEpcHomeUncoiler"), NULL, szData, sizeof(szData), sPath))
			LampEpcHomeUncoiler = CString(szData);
	}
};

struct stDlgMenu04
{
	CString FeedTorqueCwPunch, FeedTorqueCcwPunch, FeedTorqueCwAoi, FeedTorqueCcwAoi, FeedTorqueCwEngrave, FeedTorqueCcwEngrave;
	CString FeedSpeed, FeedAcc, FeedOnSpeed;//FeedTorquePunch, FeedTorqueAoi, FeedTorqueEngrave, 
	CString FeedOnAcc, FeedLeadPitchAoi, FeedLeadPitchPunch, FeedLeadPitchEngrave;
	CString BufferPosStAoi, BufferPosStEngrave, FeedLengthFromAoiToPunch, ShotNumFromAoiUpToAoiDn;
	CString FeedLengthFromEngraveToAoi, FeedSpeed2dCode, FeedAcc2dCode;
	CString ShotNumFromAoiToPunch, FeedLengthFromAoiUpToAoiDn, ShotNumFromEngraveToAoi;
	CString BufferInitPosMove, BufferHomming;
	CString FeedLengthFromLaserToReader;

	stDlgMenu04()
	{
		FeedTorqueCwPunch = _T("ML41050");
		FeedTorqueCcwPunch = _T("ML41052");
		FeedTorqueCwAoi = _T("ML41044");
		FeedTorqueCcwAoi = _T("ML41046");
		FeedTorqueCwEngrave = _T("ML41040");
		FeedTorqueCcwEngrave = _T("ML41042");

		FeedSpeed = _T("ML41032");
		FeedAcc = _T("ML41034");
		FeedOnSpeed = _T("ML45038");
		FeedOnAcc = _T("ML41036");
		FeedLeadPitchAoi = _T("ML41022");
		FeedLeadPitchPunch = _T("ML41024");
		FeedLeadPitchEngrave = _T("ML41020");
		BufferPosStAoi = _T("ML41028");
		BufferPosStEngrave = _T("ML41026");
		FeedLengthFromAoiToPunch = _T("ML41014");
		ShotNumFromAoiToPunch = _T("MW41102");

		FeedLengthFromAoiUpToAoiDn = _T("ML41012");
		ShotNumFromAoiUpToAoiDn = _T("MW41101");
		FeedLengthFromEngraveToAoi = _T("ML41010");
		ShotNumFromEngraveToAoi = _T("MW41100");

		FeedSpeed2dCode = _T("");
		FeedAcc2dCode = _T("");

		BufferInitPosMove = _T("MB40015A");
		BufferHomming = _T("MB40015B");
		FeedLengthFromLaserToReader = _T("ML41060");
	}

	void Load()
	{
		TCHAR szData[200];
		CString sMenu = _T("IDD_DLG_MENU_04");
		CString sPath = PATH_GUI_REGISTER;

		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedTorqueCwPunch"), NULL, szData, sizeof(szData), sPath))
			FeedTorqueCwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedTorqueCcwPunch"), NULL, szData, sizeof(szData), sPath))
			FeedTorqueCcwPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedTorqueCwAoi"), NULL, szData, sizeof(szData), sPath))
			FeedTorqueCwAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedTorqueCcwAoi"), NULL, szData, sizeof(szData), sPath))
			FeedTorqueCcwAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedTorqueCwEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedTorqueCwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedTorqueCcwEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedTorqueCcwEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedSpeed"), NULL, szData, sizeof(szData), sPath))
			FeedSpeed = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedAcc"), NULL, szData, sizeof(szData), sPath))
			FeedAcc = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOnSpeed"), NULL, szData, sizeof(szData), sPath))
			FeedOnSpeed = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedOnAcc"), NULL, szData, sizeof(szData), sPath))
			FeedOnAcc = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedLeadPitchAoi"), NULL, szData, sizeof(szData), sPath))
			FeedLeadPitchAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedLeadPitchPunch"), NULL, szData, sizeof(szData), sPath))
			FeedLeadPitchPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedLeadPitchEngrave"), NULL, szData, sizeof(szData), sPath))
			FeedLeadPitchEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("BufferPosStAoi"), NULL, szData, sizeof(szData), sPath))
			BufferPosStAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("BufferPosStEngrave"), NULL, szData, sizeof(szData), sPath))
			BufferPosStEngrave = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedLengthFromAoiToPunch"), NULL, szData, sizeof(szData), sPath))
			FeedLengthFromAoiToPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ShotNumFromAoiUpToAoiDn"), NULL, szData, sizeof(szData), sPath))
			ShotNumFromAoiUpToAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedLengthFromEngraveToAoi"), NULL, szData, sizeof(szData), sPath))
			FeedLengthFromEngraveToAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedSpeed2dCode"), NULL, szData, sizeof(szData), sPath))
			FeedSpeed2dCode = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedAcc2dCode"), NULL, szData, sizeof(szData), sPath))
			FeedAcc2dCode = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ShotNumFromAoiToPunch"), NULL, szData, sizeof(szData), sPath))
			ShotNumFromAoiToPunch = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedLengthFromAoiUpToAoiDn"), NULL, szData, sizeof(szData), sPath))
			FeedLengthFromAoiUpToAoiDn = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("ShotNumFromEngraveToAoi"), NULL, szData, sizeof(szData), sPath))
			ShotNumFromEngraveToAoi = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("BufferInitPosMove"), NULL, szData, sizeof(szData), sPath))
			BufferInitPosMove = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("BufferHomming"), NULL, szData, sizeof(szData), sPath))
			BufferHomming = CString(szData);
		if (0 < ::GetPrivateProfileString(sMenu, _T("FeedLengthFromLaserToReader"), NULL, szData, sizeof(szData), sPath))
			FeedLengthFromLaserToReader = CString(szData);
	}
};

struct stGuiRegister
{
	stDlgInfoReg DlgInfo;
	stDlgFrameHighReg DlgFrameHigh;
	stDlgMenu01 DlgMenu01;
	stDlgMenu03 DlgMenu03;
	stDlgMenu04 DlgMenu04;

	void Load()
	{
		CString sPath = PATH_GUI_REGISTER;
		CFileFind findfile;
		if (findfile.FindFile(sPath))
		{
			DlgInfo.Load();
			DlgFrameHigh.Load();
			DlgMenu01.Load();
			DlgMenu03.Load();
			DlgMenu04.Load();
		}
	}
};




#endif // !defined(AFX_GLOBALDEFINE_H_INCLUDED_)
