//
// Device.h : D3D 디바이스 셋업 코드.
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
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
// < DirectX 버전별 개발 라이브러리 >
// DX9         : DirectX SDK 2002 Nov. 
// DX10.x/DX11 : DirectX SDK 2010 June (DX 독립 배포 마지막 버전)
// DX11.X/DX12 : Windows SDK 8.x  (DX 가 포함되어 배포됨) 

// DX 표준 헤더 
#include "D3D11.h"					//DX 표준 헤더. (DX 11.0)
//#include "D3D11_1.h"				//DX 표준 헤더. (DX 11.1)
#pragma comment(lib, "D3D11")		//DX 라이브러리 로딩.  D3D11.dll 필요.

//DirectX Math : Dx 수학 라이브러리 
//#include "XNAMath.h"				//XNA Math 라이브러리.(ver.2.0) DirectX SDK 2010 June. 대응.
#include "DirectXMath.h"			//XNA Math 라이브러리.(ver.3.0) VS2012 (Windows SDK 8) 이상.
using namespace DirectX;

//DXGI : DirectX Graphics Infrastructure 
//DX 의 버전에 독립적인 하드웨어 저수준 접근 방식을 제공합니다.
//#include "DXGI.h"					//DXGI 헤더. (d3d11.h 에 포함됨)
#pragma comment(lib, "dxgi")		//DXGI 라이브러리. DXGI.dll 필요.


///////////////////////////////////////////////////////////////////////////////
//
// DirectX Toolkit : DX 확장 라이브러리  --> Yena 에 포함됨.
//
///////////////////////////////////////////////////////////////////////////////
// DX 확장 라이브러리 및 유틸리티 모음집.
// DX 사용시 필요한 여러 기능 클래스 및 부가 서비스를 제공합니다.
// DirectXTK 를 보다 편하게 사용하기 위해서 Project 속성 추가가 필요합니다.
// DirectXTK.h 또는  Yena/DXTK/Readme.txt 을 참조하세요.  
//#include "DirectXTK.h"

//DirectXTK 의 텍스처 API 사용.
//예나에 포함됨 생략 가능.
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h" 


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
//DX11 장치관련 재정의.											
typedef ID3D11Device*			LPDEVICE;
typedef ID3D11DeviceContext*	LPDXDC;
typedef IDXGISwapChain*			LPSWAPCHAIN;
typedef ID3D11RenderTargetView*	LPRTVIEW;
typedef ID3D11DepthStencilView*	LPDSVIEW;
typedef ID3D11InputLayout*		LPINPUTLAYOUT;
typedef ID3D11Buffer*			LPBUFFER;
typedef LPBUFFER				LPCBUFFER;
typedef ID3D11ShaderResourceView*  LPSRVIEW;
typedef LPSRVIEW	LPTEXTURERV;
typedef D3D11_TEXTURE2D_DESC	TEXDESC;
//typedef ID3DBlob*				LPXDATA;


/*
// DirectX Math 타입 재정의 : 호환성 향상
// DX9/DX10 : DirectX SDK June.2010 이하
typedef D3DXMATRIXA16	MATRIXA;	//행렬 : 16바이트 정렬 버전.
typedef D3DXMATRIX		MATRIX;		//행렬 : 일반 버전.
typedef D3DXVECTOR4		VECTOR4;
typedef D3DXVECTOR3		VECTOR3;
typedef D3DXVECTOR2		VECTOR2;
*/
// DirectX Math 타입 재정의 : 구형 소스와의 호환성 향상이 목적.
// DX10/11 "XNAMath ver.2": DirectX SDK june.2010
// DX11/12 "XNAMath ver.3": DirectXMath, Windows SDK 8.x 에 포함  
// 링크 : https://msdn.microsoft.com/ko-kr/library/windows/desktop/ee418728(v=vs.85).aspx
//
typedef XMMATRIX		MATRIXA;	//행렬 : 16바이트 정렬, SIMD 버전. 전역/지역 변수용. "Register Type"
typedef XMFLOAT4X4		MATRIX;		//행렬 : 일반 버전. SIMD 미지원, Class 데이터 저장용. "Storage Type"
typedef XMVECTOR		VECTOR;		//4성분 벡터 : 16바이트 정렬, SIMD 버전. 전역/지역 변수용. "Register Type"
typedef XMFLOAT4		VECTOR4;	//4성분 벡터 : 일반 버전, SIMD 미지원, Class 데이터 저장용. "Storage Type"
typedef XMFLOAT3		VECTOR3;
typedef XMFLOAT2		VECTOR2;

//색상 타입: 2가지.
//typedef XMCOLOR		COLOR;		// r, g, b, a.  [정수형 0~255]
typedef XMFLOAT4		COLOR;		// r, g, b, a.  [실수형 0~1.0]
typedef XMFLOAT4A		COLORA;		// r, g, b, a.  [실수형 0~1.0] + 16비트 정렬
typedef XMVECTOR        XMCOLOR;	// r, g, b, a.  [실수형 0~1.0] + 16비트 정렬 SIMD 형.


//평면 타입 재정의.
typedef XMFLOAT4		PLANE;		//일반 버전
typedef XMVECTOR		XMPLANE;	//SIMD 버전.


///////////////////////////////////////////////////////////////////////////////
//
// DX 및 장치 관련 전역 데이터들  
//
///////////////////////////////////////////////////////////////////////////////

int		DXSetup(HWND hwnd);
void	DXRelease(); 

int		ClearBackBuffer(UINT flag, COLOR col, float depth = 1.0f, UINT stencil = 0);
int     Flip();

float	GetEngineTime();
void	PutFPS(int x, int y);
//void  ynTextDraw( int x, int y, COLOR col, char* msg, ...);

void	GetDeviceInfo();
HRESULT GetAdapterInfo(DXGI_ADAPTER_DESC1* pAd);
void	SystemUpdate(float dTime);
void	SystemInfo(int x, int y, COLOR col);

// 엔진 DX 기능 레벨 구하기. 
void	GetFeatureLevel();

//외부 설정 파일 읽기.
//bool	LoadIniData(TCHAR* pModelName);
bool	LoadIniData(TCHAR* pModelName, TCHAR* pTexName);
//bool	LoadIniData(TCHAR* pTexNameCube, TCHAR* pTexNameMap);

//텍스처 함수들.
HRESULT LoadTexture(TCHAR* filename, ID3D11ShaderResourceView** ppTexRV);
HRESULT GetTextureDesc(ID3D11ShaderResourceView* pTexRv, D3D11_TEXTURE2D_DESC* pTexDesc);

//기하버퍼 생성.
HRESULT CreateVB(LPDEVICE pDev, void* pBuff, UINT size, LPVERTEXBUFFER* ppVB);
HRESULT CreateIB(LPDEVICE pDev, void* pBuff, UINT size, LPINDEXBUFFER* ppIB);


//렌더링 모드 전환.
void Mode_2D();
void Mode_3D();
void Mode_25D();


// 렌더타겟, 깊이/스텐실 버퍼 클리어.
void RTClear(COLOR& col);

//색상 처리.
void  Saturate	 (COLOR* col);
COLOR InvertColor(COLOR& col);



extern ID3D11Device*           g_pDevice;
extern ID3D11DeviceContext*	   g_pDXDC;
extern IDXGISwapChain*         g_pSwapChain;
extern ID3D11RenderTargetView* g_pRTView;
extern ID3D11DepthStencilView* g_pDSView;



// 장치 설정 정보 구조체. (DX9/11 구형 호환성 유지용)
typedef DXGI_MODE_DESC	  DISPLAYMODE;	//DX11 대응
//typedef DXGI_MODE_DESC1 DISPLAYMODE;	//DX11.1 대응
//typedef D3DDISPLAYMODE DISPLAYMODE;   //DX9 용.

extern DISPLAYMODE g_Mode;
extern HWND g_hWnd;
extern BOOL g_bShowFrame;

extern DWORD g_dwAA;
extern DWORD g_dwAF;

//전체화면 사용여부.
extern BOOL g_bWindowMode;
//수직동기화 사용여부.
extern BOOL g_bVSync;

//현재 디바이스 DX 기능 레벨.
extern TCHAR* g_strFeatureLevel;




///////////////////////////////////////////////////////////////////////////////
//
// 렌더링 상태 객체 관련  
//
///////////////////////////////////////////////////////////////////////////////

extern BOOL g_bCullBack;		//뒷면 제거...On/Off.
extern BOOL g_bWireFrame;		//와이어 프레임 On/Off.
extern BOOL g_bZEnable;			//깊이 버퍼 연산 On/Off.
extern BOOL g_bZWrite;			//깊이버퍼 쓰기 On/Off.	

extern COLOR g_ClearColor;		//배경색.
extern COLOR g_ClearColor2;		//배경색2.

//
// 렌더링 상태 객체 생성 / 운용 함수들.
//
//int  RenderStateObjectCreate();
//void RenderStateObjectRelease();
void RenderStateUpdate();
void RenderModeUpdate();


//
// 깊이/스텐실 테스트 상태들.
//
enum {
	DS_DEPTH_ON,		//깊이버퍼 ON! (기본값), 스텐실버퍼 OFF.
	DS_DEPTH_OFF,		//깊이버퍼 OFF!, 스텐실 OFF.
	DS_DEPTH_WRITE_OFF,	//깊이버퍼 쓰기 끄기, 스텐실 OFF. 

	//스텐실 버퍼 연산용.
	DS_DEPTH_ON_STENCIL_ON,				//깊이버퍼 On (Write On), 스텐실버퍼 ON (참조값 쓰기) : "깊이/스텐실 기록".
	DS_DEPTH_ON_STENCIL_EQUAL_KEEP,		//깊이버퍼 On (Write On), 스텐실버퍼 ON (동일비교, 성공시 유지) : "지정 위치에만 그리기".
	DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP,	//깊이버퍼 On (Write On), 스텐실버퍼 ON (다름비교, 성공시 유지) : "지정 위치 이외에 그리기".
	
	//DS_DEPTH_OFF_STENCIL_ON,			//깊이버퍼 Off, 스텐실버퍼 ON (참조값 쓰기) : "스텐실만 기록"
	//DS_DEPTH_ON_STENCIL_EQUAL_INCR,	//깊이버퍼 On (Write On), 스텐실버퍼 ON (동일비교, 성공시 증가) : "이중그리기 방지".
	DS_DEPTH_WRITE_OFF_STENCIL_ON,		//깊이버퍼 On (Write Off), 스텐실버퍼 ON (항상기록, 성공시 증가) :" 스텐실만 기록".
	DS_DEPTH_OFF_STENCIL_EQUAL_INCR,	//깊이버퍼 Off (Write Off), 스텐실버퍼 ON (동일비교, 성공시 증가) : "이중그리기 방지.
	

	//열거 상수 최대값.
	DS_MAX_,	

	//상수 재정의. MAX 값 뒷쪽에 정의해야 함. 주의.
	DS_DEPTH_ON_STENCIL_OFF = DS_DEPTH_ON,			//깊이버퍼 ON, 스텐실버퍼 OFF
	DS_DEPTH_ONLY	= DS_DEPTH_ON_STENCIL_OFF,		//깊이버퍼 ON, 스텐실버퍼 OFF
	//DS_STENCIL_ONLY = DS_DEPTH_OFF_STENCIL_ON,	//깊이버퍼 OFF, 스텐실버퍼 ON
};

//렌더링 상태 객체들 : 엔진 전체 공유함. "Device.cpp"
extern ID3D11DepthStencilState* g_DSState[DS_MAX_];




// 
// 레스터라이져 상태 객체 Rasterizer State Objects
// 
enum {
	RS_SOLID,				//삼각형 채우기 : Fill Mode - Soild.
	RS_WIREFRM,				//삼각형 채우기 : Fill Mode - Wireframe.
	RS_CULLBACK,			//뒷면 컬링 (ON) : Back-Face Culling - "CCW" 
	RS_CULLFRONT,			//정면 컬링 (ON) : Front-Face Culling - "CW" 	
	RS_WIRECULLBACK,		//와이어 프레임 + 뒷면 컬링 (ON) 
	RS_WIRECULLFRONT,		//와이어 프레임 + 앞면 컬링 (ON) 	
	
	//열거 상수 최대값.
	RS_MAX_,

	//상수 재정의. MAX 값 뒷쪽에 정의해야 함. 주의.
	RS_CULL_CCW = RS_CULLBACK,
	RS_CULL_CW = RS_CULLFRONT,
	RS_WIRE_CULL_CCW = RS_WIRECULLBACK,
	RS_WIRE_CULL_CW = RS_WIRECULLFRONT,	

};
//레스터라이져 상태 객체 배열
extern ID3D11RasterizerState*	g_RState[RS_MAX_];

//void RasterStateCreate();
//void RasterStateRelease();
//void RenderModeUpdate();


//
// 렌더링 모드 : 다수의 렌더링 상태 조합 및 운용을 위한 정의.
//
enum {
	RM_SOLID		= 0x0000,		// 삼각형채우기 : ON, Solid
	RM_WIREFRAME	= 0x0001,		// 삼각형채우기 : OFF, Wire-frame
	RM_CULLBACK		= 0x0002,		// 뒷면 컬링 : ON, "CCW"
	
	//렌더링 기본모드 : Solid + Cull-On.
	RM_DEFAULT		= RM_SOLID | RM_CULLBACK,	
	
};
extern DWORD g_RMode;		//'현재' 렌더링 모드.



//
// 색상/알파 혼합 상태 객체 Blending State Objects 
//
enum {
	BS_DEFAULT,				//기본혼합 객체.
	BS_ALPHA_BLEND,			//알파 블랜딩. (Src.a + (1-Src.a))
	BS_COLOR_BLEND,			//색상 블랜딩. ( Src.Color + (1-Src.Color))
	BS_COLOR_BLEND_ONE,		//1:1 블랜딩. ( Src.Color + Dest.Color) 

	BS_MAX_,

	BS_AB_OFF = BS_DEFAULT,
	BS_AB_ON = BS_ALPHA_BLEND,
	//BS_AB_ALPHA = BS_ALPHA_BLEND,
	//BS_AB_COLOR = BS_COLOR_BLEND,
};

//색상/알파 혼합 관리 객체 
extern ID3D11BlendState* g_BState[BS_MAX_];

//void BlendStateCreate();
//void BlendStateRelease();
//void BlendModeUpdate();



//
// 텍스처 셈플러 상태 객체 : Texture Sampler State Objects.
//
enum {
	SS_CLAMP,			//텍스처 어드레스 모드 : 클램핑 (DX 기본값) 
	SS_WRAP,			//텍스처 어드레스 모드 : 렙핑 
	//SS_BOARDER,
	//SS_MIRROR,
	//SS_MIRRROONCE,
	
	SS_MAX,
	SS_DEFAULT = SS_WRAP,	//기본 셈플러
	
};
extern ID3D11SamplerState* g_pSampler[SS_MAX];
//void SamplerCreate();
//void SamplerRelease();





///////////////////////////////////////////////////////////////////////////////
//
// 기타 시스템 관련 함수 및 재정의.
//
///////////////////////////////////////////////////////////////////////////////

//비동기 키보드 입력 처리.
#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(pBuff) if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; }
#define SAFE_DELETE(pBuff)	if((pBuff)){ delete (pBuff); (pBuff) = NULL; }
#define SAFE_DELARRY(pBuff) if((pBuff)){ delete [] (pBuff); (pBuff) = NULL; }
#endif

#ifndef YES_
#define YES_ TRUE
#define NO_  FALSE
#endif

/* Yena 에 포함됨
#ifndef YN_OK
#define YN_OK	   0					//에러 없음. 특별한 문제 없음.
#define YN_FAIL   -1					//에러 발생.
#define YN_FAILED(v)   ((v) != YN_OK_)	//에러 체크 매크로.
#endif

// 색상 * 스칼라 곱  (ex)  col = col * 0.5f
COLOR operator * (COLOR& lhs, float s);
*/



//마우스 입력 상태.
#define MS_STANDBY   0x0000		//대기중.
#define MS_LBTNDOWN  0x0100		//버튼 눌림.
#define MS_LBTNUP	 0x8100		//버튼 눌린후 떨어짐.
#define MS_RBTNDOWN  0x0001		//버튼 눌림.
#define MS_RBTNUP	 0x0081		//버튼 눌린후 떨어짐.




///////////////////////////////////////////////////////////////////////////////
//
// 에러 출력 함수
//
///////////////////////////////////////////////////////////////////////////////

//int ynError(BOOL bBox, TCHAR* msg, ...);
int ynErrorW(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob, TCHAR* filename, char* EntryPoint, char* ShaderModel);
int ynErrorW(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob);
int ynErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...);
int ynErrorW(TCHAR* msg, ...);
 
#define ynError(hr, msg, ...)  \
ynErrorW( __FILEW__, __LINE__, __FUNCTIONW__, TRUE, hr, msg, __VA_ARGS__ )

//#define ynError(hr, msg, pBlob)  ynErrorW( TRUE, hr, msg, pBlob)



///////////////////////////////////////////////////////////////////////////////
//
// 폰트 엔진 : 일반 함수 버전. --> Yena 에 포함됨
//
// DirectXTK : SpriteFont 기반 폰트 출력 클래스
// 2D Texture / Sprite 기반의 폰트 출력 라이브러리.
// 빠르고 경쾌하지만, CJK 계열의 폰트의 경우 완성형만 지원됩니다.
// 참조 : https://directxtk.codeplex.com/wikipage?title=SpriteFont
// #include "SpriteFont.h"	
//
// 자세한 것은 DirectXTK.h 또는  Yena/DXTK/Readme.txt 을 참조. 
//
///////////////////////////////////////////////////////////////////////////////
/*
int  ynFontCreate(LPDEVICE pDev);
void ynFontRelease();
void ynTextDraw(int x, int y, COLOR col, TCHAR* msg, ...);
*/


/**************** end of "Device.h" ***********************************/