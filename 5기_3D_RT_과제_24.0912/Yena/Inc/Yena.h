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
// Yena.h: 엔진 필수 헤더.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. Yena / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning(disable:4996)

///////////////////////////////////////////////////////////////////////////////
//
// 플렛폼 및 그래픽 API 라이브러리 선언.
//
///////////////////////////////////////////////////////////////////////////////
#include "Windows.h" 
#include "DirectX.h"		//DX 관련 종합 헤더.
//#include "DirectXTK.h"	//DirectX Toolkit 헤더. 
//#include "OpenGL.h"		//GL 헤더. 
#include "Tchar.h"


///////////////////////////////////////////////////////////////////////////////
//
// Yena 엔진 / 라이브러리 공통 헤더  
//
///////////////////////////////////////////////////////////////////////////////
#include "Yena_API.h"			//엔진 라이브러리 Load 및 Import 처리.
#include "ynDefine.h"			//각종 자료/클래스/구문 재정의.
#include "ynVersion.h"			//버전 관리.
#include "YnError.h"			//에러 메세지 관리.
#include "YnDevice.h"			//렌더링 장치 운용.
#include "ynFont.h"				//폰트 운용 클래스.
#include "ynMath.h"				//수학 라이브러리 확장.
#include "ynCamera.h"			//카메라 클래스.
#include "ynStateObject.h"		//상태객체 확장 클래스.
#include "ynShader.h"			//셰이더 확장 클래스.
#include "ynGrid.h"				//그리드 클래스.
#include "ynAxis.h"				//방향 축 클래스.
#include "ynMesh.h"				//기본 메쉬 클래스.
#include "ynTex.h"				//텍스처 헤더.



///////////////////////////////////////////////////////////////////////////////
//
// 엔진 전역 공유 데이터들.
//
///////////////////////////////////////////////////////////////////////////////

// Yena 엔진 전역 공유 네임스페이스 
//
namespace YENA
{
	extern YENA_APIX LPDEVICE	  ypDevice; 
	extern YENA_APIX LPDXDC		  ypDXDC;
	extern YENA_APIX DISPLAYMODE  ynMode;

	//extern DWORD	ynVersion;		//버전.
	//extern TCHAR*	ypBuild;		//빌드 일자.

	//변환 정보 : 엔진에 설정된 "현재" 카메라 기준.
	extern YENA_APIX MATRIXA  ymView;		//뷰 변환 행렬.
	extern YENA_APIX MATRIXA  ymProj;		//투영 변환 행렬.

	//공간 정보 가시화 장치
	extern ynAxis* ypAxis;			//방향축.(월드)
	extern ynGrid* ypGrid;			//그리드.(월드) 

}
using namespace YENA;


//---------------------------------------------------
// 예나 시스템 기본공유 데이터 (System Value)  재정의.
//---------------------------------------------------
// 각 객체는 초기 엔진 시동시 생성되며, 엔진 종료시점에 제거됩니다. 
// 각 객체는 전체 엔진의 시스템을 유기적으로 연결하기 위한 공유 목적으로 사용되는 전역 데이터 입니다. 
// 각 객체는 기본적으로 1개씩 존재하며 필요시 사용자가 추가 할 수 있습니다.
// 각 객체 생성방법을 참조하십시오.
//
#define SV_Dev		 YENA::ypDevice		// 장치/디바이스 객체
#define SV_DC		 YENA::ypDXDC		// 디바이스 컨텍스트 객체
#define SV_SwapChain YENA::ypSwapChain	// 스왑체인 객체.
#define SV_Mode   YENA::ynMode			// 출력장치 설정 정보.
#define SV_AA     YENA::ynAA
#define SV_AF     YENA::ynAF
#define SV_View   YENA::ymView			// 현재카메라 - 뷰 변환 행렬.	
#define SV_Proj   YENA::ymProj			// 현재카메라 - 투영 변환 행렬.
//#define SV_Ver	YENA::ynVersion
//#define SV_Build  YENA::ypBuild
#define SV_Font	  YENA::ypFont			// 기본 폰트.(한글완성형,크기:9)
#define SV_FXLine YENA::ypFXLine		// 기본 셰이더(라인용)

// 구형 호환을 위한 재정의.
#define yn_Dev		  SV_Dev
#define yn_DC		  SV_DC
#define yn_SwapChain  SV_SwapChain
#define yn_Mode    SV_Mode
#define yn_AA	   SV_AA
#define yn_AF	   SV_AF
#define yn_View    SV_View
#define yn_Proj    SV_Proj
#define yn_Font	   SV_Font
#define yn_FXLine  SV_FXLine




///////////////////////////////////////////////////////////////////////////////
//
// 엔진 및 시스템 함수들
//
///////////////////////////////////////////////////////////////////////////////
namespace YENA {


//-----------------------------------------------------------------------------
//
// 엔진 기반 함수들 : 엔진 초기 구성 및 공통 개체 생성.
//
//-----------------------------------------------------------------------------
// Yena 엔진 설정 : 초기 시작시 1회 호출.
// 공용장치 생성 : Camera, Grid/Axis, FX, Timer 등. 
// 상태객체 설정 : Depth-ON, Stencil-OFF, AB_OFF, Fill+Cull(CCW), Clamp mode
// pDev : 장치 핸들.
// Mode : 장치 / 출력 정보.
YENA_API int  YenaSetup(LPDEVICE pDev, DISPLAYMODE& Mode, 
					    float GridOffset = 10.0f, float GridSize = 100.0f,
						float AxisLength = 5.0f ); 

// Yena 엔진 갱신 : 매 프레임마다 1회 호출.
// 공용장치 갱신 : Camera, Grid/Axis, FX, Timer 등.
// 상태객체 갱신 : Depth-ON
YENA_API void YenaUpdate(float dTime);						

//Yena 엔진 해제 : 종료시 1회 호출.
YENA_API void YenaRelease();								

// 시스템 정보 / 도움말 출력.
YENA_API void YenaSysInfo(int& x, int& y, COLOR col);		//간단 버전.
YENA_API void YenaSysInfoEx(int& x, int& y, COLOR col);		//상세 버전.



//-----------------------------------------------------------------------------
//
// 카메라 시스템
//
//-----------------------------------------------------------------------------
//MATRIXA ynCameraView();			//현재 카메라 뷰 변환 행렬 얻기.
//MATRIXA ynCameraProj();			//현재 카메라 투영 변환 행렬 얻기.



//-----------------------------------------------------------------------------
//
// 그리드 & 방향축 표시관련. 
//
//-----------------------------------------------------------------------------
// 축-그리드 개체 생성. (엔진 초기설정시 자동호출)
// pDev : 장치 핸들.
// bGridOn : 그리드 개체 생성 여부. 기본값 true.
// bAxisOn : 방향축 개체 생성 여부. 기본값 true. 
YENA_API void ynGridAxisCreate(LPDEVICE pDev, BOOL bGridOn = TRUE, BOOL bAxisOn = TRUE, 
							   float GridOffset = 10.0f, float GridSize = 100.0f,
							   float AxisLength = 5.0f );

// 축-그리드 개체 제거. (엔진 제거시 자동호출) 
YENA_API void ynGridAxisRelease();

// 축-그리드 개별 on/off 설정 
// ynGridAxisUpdate 가 사용되지 않으면, 무시됨. 
YENA_API void ynGridAxisState(BOOL bGridOn = TRUE, BOOL bAxisOn = TRUE);

// 축-그리드 상태 갱신
// F2 : 그리드 (ON/OFF), 각칸의 크기는 10.0 x 10.0f 
// F3 : 방향축 (ON/OFF) 
// 이 기능을 사용하지 않으려면 해당 코드를 주석처리 하십시오.  
YENA_API void ynGridAxisUpdate(float dTime);

//축-그리드 그리기 : ynGridAxisUpdate 가 사용되지 않으면, 무시됨.
YENA_API void ynGridAxisDraw(float dTime = 0);



//-----------------------------------------------------------------------------
//
// 문자열 출력 
//
//-----------------------------------------------------------------------------
//
// ynTextDraw   :  지정된 화면좌표에 문자열을 출력합니다.
//
// 파라미터 : x,y = 화면좌표.   
//            col = 출력색상  
//            msg = 출력할 문자열(+형식화된문자열 지원), 
//					
// 출력 최대 문자수는 2048. 문자수를 넘기면 overflow 에러 발생.
// 출력 문자의 크기 : 6x12 (pixel)
//
YENA_API void  ynTextDraw(int x, int y, COLOR col, TCHAR* msg, ...);



//  ynTextDrawSV   : 지정된 화면좌표에 연속적으로 문자열을 출력합니다.
//                   
// 파라미터 : x,y = 화면좌표.   
//            col = 출력색상  
//            msg = 출력할 문자열 
//            dTime = 문자 출력 속도처리용 엔진 경과 시간값 (1sec/1000ms)
//					
// 글자를 일정속도로 연속적으로 출력합니다.  
// 가로 80글자 이상, 또는 개행문자(CR_LF) 를 만나는 경우 다음 줄로 자동으로 내려갑니다.   
// 가로 80 글자 세로 40줄 까지 표현할 수있습니다.
// 출력 최대 문자수는 4096. 문자수를 넘기면 overflow 에러 발생. 
// 출력 문자의 크기 : Size 9 기준. 구형은 6x12 (pixel).
//
YENA_API void ynTextDrawSV(int x, int y, COLOR col, TCHAR* msg, float dTime);



// ynError :  에러 메세지 출력 함수.
//            입력된 에러코드를 문자열로 전환, 메세지 박스형태로 출력합니다.
//            디버깅 중이라면, Visual Studio 의 출력창도 동일 내용을 출력합니다.
//            DXErr.h 가 필요하다.
//
// 파라미터 : bBox = 메세지박스 출력 여부. 
//           msg = 출력할 문자열
//           hr =  HRESULT 값, 문자열로 전환후 출력됩니다.
//           pBlob = 에러 메세지를 담은 버퍼 포인터.
//
YENA_API int ynError(BOOL bBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob);




} // end of namespace YENA

/****************** end of file "Yena.h" *******************************/