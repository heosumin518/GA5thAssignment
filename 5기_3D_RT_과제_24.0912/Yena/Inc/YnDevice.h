/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  B3/YENA : 2D/3D 렌더링 엔진.						                       // 
//                                                                         //
//  제 작 : Kihong Kim / mad_dog@hanmail.net                               // 
//          Zero-G Tech,Inc http://www.zero-g.kr						   //	
//  초기개발 : 2003.12 - 2004.03.                                          //
//  개발환경 : OS - Windows 2k SP4 / Visual Studio 6.0/2003/2005/2008       //
//                  DirectX 9.0b(2003, summer)                             //
//                  DirectX 9.0c(2007, November)                           //
//                  OpenGL 1.x / 2.x                                       //
//             H/W - AMD Athlon XP(Barton) 2500+@2300                      //    
//                   ATI Radeon 9600 Pro                                   //
//                   ABit NF7-II /w SAMSUNG DDR 512MB                      //
//                   SB Live! DE 5.1                                       //
//  테스트 환경 : CPU - AMD Athlon										   //	 
//                      AMD Athlonx64 w/q4                                 // 
//                      Intel Pentium III/IV & Celeron                     //
//                      Core2 w/Quad                                       //
//                VGA - ATI Radeon 9xxx Series                             //
//                          Radeon 1xxx Series                             //
//                          Radeon HD 3/4/5/6 Series                       //
//                      nVidia GeForce 2 MX/Ti Series.                     //  
//                             GeForce 3 Ti Series.                        //
//                             GeForce 4 MX/Ti Series.                     //
//                             GeForce FX Series.                          //
//                             GeForce 5/6/7/8/9xxx Series.                //
//                             GeForce 2/4xx GTS Series.                   //
//                             GeForce 7xx GTX Series.                     //
//                             GeForce 2Way-SLI / 3Way-SLI		   	       //
//                OS - Windows 98 / 2K / XP / VISTA / Win7 x32:x64         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//
// YnDevice.h : 렌더링 디바이스 셋업 코드.
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
#pragma once
#pragma warning(disable:4996)

// Platform Header 및 Library
//
#include "Windows.h"
#include "stdio.h"

///////////////////////////////////////////////////////////////////////////////
//
// DirectX 표준 라이브러리 
//
///////////////////////////////////////////////////////////////////////////////
//... Yena.h 에서 처리.

///////////////////////////////////////////////////////////////////////////////
//
// DirectX Toolkit : DX 확장 라이브러리  --> Yena 에 포함됨.
//
///////////////////////////////////////////////////////////////////////////////
//... Yena.h 에서 처리.


////////////////////////////////////////////////////////////////////////////
//
// Yena : 예나 사용.
//
////////////////////////////////////////////////////////////////////////////
// 게임 개발에 필요한 기본 장치 및 확장 기능을 제공.
// 프레임워크,폰트,수학,카메라,조명,셰이더,상태 객체 등등..
// 자세한 것은 Yena 설명서.txt 참조.
//
#include "Yena.h"			// Yena 메인 헤더.



///////////////////////////////////////////////////////////////////////////////
//
// 타입 과 상수 재정의 : 버전별 호환성 향상 증대가 목적.
//
///////////////////////////////////////////////////////////////////////////////
//... ynDefine.h 에서 처리.


///////////////////////////////////////////////////////////////////////////////
//
// DX 및 장치 관련 전역 데이터들  
//
///////////////////////////////////////////////////////////////////////////////
namespace YENA {

YENA_API int	ynDXSetup(HWND hwnd);
YENA_API void	ynDXRelease();

YENA_API int	ynClearBackBuffer(UINT flag, COLOR col, float depth = 1.0f, UINT stencil = 0);
YENA_API int    ynFlip();

YENA_API float	ynGetEngineTime();
YENA_API void	ynPutFPS(int x, int y);
//void  ynTextDraw( int x, int y, COLOR col, char* msg, ...);

// 엔진/시스템 공통 장치 갱신 : 카메라, 조명, 공간가시화 장비 등등. --> YenaUpdate 에서 처리함.
//YENA_API void	 ynSystemUpdate(float dTime);

// 엔진 DX 기능 레벨 구하기. 
YENA_API TCHAR*	ynGetFeatureLevelStr(D3D_FEATURE_LEVEL fv);
YENA_API void	ynGetFeatureLevel(D3D_FEATURE_LEVEL* fv);

// 엔진/렌더링 장치 정보 획득.
//YENA_API void	 ynGetDeviceInfo();
YENA_API HRESULT ynGetAdapterInfo(DXGI_ADAPTER_DESC1* pAd);

// 엔진/시스템 도움말 및 정보 출력.
//YENA_API void	 ynSystemInfo(int x, int y, COLOR col);
YENA_API void	 ynAdapterInfo(int x, int y, COLOR col);		//GPU 정보 상세.
YENA_API void	 ynDeviceInfo(int& x, int& y, COLOR col);		//렌더링 장치 정보.(간략)
YENA_API void	 ynGraphicsInfo(int& x, int& y, COLOR col);		//그래픽 설정 정보.(간략)



//엔진 시스템 공유 데이터들.
extern YENA_APIX LPDEVICE       ypDevice;
extern YENA_APIX LPDXDC			ypDXDC;
extern YENA_APIX LPSWAPCHAIN    ypSwapChain;
extern YENA_APIX LPRTVIEW		ypRTView;
extern YENA_APIX LPDSVIEW		ypDSView;
extern YENA_APIX LPTEXTURE		ypDS;


// 장치 설정 정보 구조체. (DX9/11 구형 호환성 유지용) --> YnDefine.h 로 이동.
//typedef DXGI_MODE_DESC  DISPLAYMODE;	//DX11 대응 
//typedef DXGI_MODE_DESC1 DISPLAYMODE;	//DX11.1 대응
//typedef D3DDISPLAYMODE DISPLAYMODE;   //DX9 용.

extern YENA_APIX DISPLAYMODE ynMode;
//extern YENA_APIX HWND yhWnd;
//extern BOOL ybShowFrame;

//현재 디바이스 DX 기능 레벨.
extern TCHAR* ysFeatureLevel;

extern DWORD ynAA;
extern DWORD ynAF;
extern BOOL  ybMipMap;


//전체화면 사용여부.
extern BOOL ybWindowMode;
//수직동기화 사용여부.
extern BOOL ybVSync;







///////////////////////////////////////////////////////////////////////////////
//
// 렌더링 상태 객체 관련  
//
///////////////////////////////////////////////////////////////////////////////
//... Yena.h 에서 처리. 




///////////////////////////////////////////////////////////////////////////////
//
// 기타 시스템 관련 함수 및 재정의.
//
///////////////////////////////////////////////////////////////////////////////
//... Yena.h 에서 처리. 



///////////////////////////////////////////////////////////////////////////////
//
// 에러 출력 함수 : DXext/DXErr 라이브러리 사용.
//
///////////////////////////////////////////////////////////////////////////////
//... ynError.h 에서 처리. 





///////////////////////////////////////////////////////////////////////////////
//
// 폰트 엔진 : Yena 사용
//
///////////////////////////////////////////////////////////////////////////////
//... Yena.h 에서 처리. 




}// end of namespace YENA

/**************** end of "YnDevice.h" ***********************************/