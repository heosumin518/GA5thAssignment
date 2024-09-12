//
// Device.cpp : D3D 객체 및 디바이스 설정/제거 소스.
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
#pragma warning(disable:4996)
#include "Device.h"
#include "Tchar.h"  

#include "Shader.h"

///////////////////////////////////////////////////////////////////////////////
//
// DX 및 장치 관련 전역 데이터들  
//
///////////////////////////////////////////////////////////////////////////////

// D3D 관련 개체 인터페이스 포인터.
ID3D11Device*           g_pDevice = NULL;
ID3D11DeviceContext*	g_pDXDC = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRTView = NULL;

//깊이 스텐실 버퍼 관련.
ID3D11Texture2D*		 g_pDS = NULL;			//깊이-스텐실 버퍼.
ID3D11DepthStencilView*  g_pDSView = NULL;		//깊이-스텐실 뷰.



//장치 설정 기본 정보.
DISPLAYMODE g_Mode = { 960, 600, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };		//16:10
//DISPLAYMODE g_Mode = { 1200, 600, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };		//2:1
//DISPLAYMODE g_Mode = { 1280, 720, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };		//16:9
//DISPLAYMODE g_Mode = { 1920, 1080, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9 FHD
//DISPLAYMODE g_Mode = { 3840, 2160, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9 UHD 4K

//AA & AF Option.
DWORD		g_dwAA = 1;		//AA 배수 (최대 8), AA=1 (OFF)
DWORD		g_dwAF = 8;		//Anisotropic Filter 배수.(최대 16)

//D3D 기능 레벨 (Direct3D feature level) 
//현재 사용할 DX 버전 지정. DX 렌더링 장치의 호환성 향상
D3D_FEATURE_LEVEL g_FeatureLevels = D3D_FEATURE_LEVEL_11_0;		//DX11 대응.
TCHAR* g_strFeatureLevel = L"N/A";

//장치 정보
DXGI_ADAPTER_DESC1 g_Adc;



//전체화면 사용여부.
BOOL g_bWindowMode = TRUE;
//수직동기화 사용여부.
BOOL g_bVSync = FALSE;


// 초기 장치 설정 관련 함수들.
//
HRESULT CreateDeviceSwapChain(HWND hwnd);
HRESULT CreateRenderTarget();
HRESULT CreateDepthStencil();
void	SetViewPort();



///////////////////////////////////////////////////////////////////////////////
//
// 렌더링 상태 객체 관련  
//
///////////////////////////////////////////////////////////////////////////////


BOOL g_bCullBack = FALSE;		//뒷면 제거...On/Off.
BOOL g_bWireFrame = FALSE;		//와이어 프레임 On/Off.
BOOL g_bZEnable = TRUE;			//깊이 버퍼 연산 On/Off.
BOOL g_bZWrite = TRUE;			//깊이버퍼 쓰기 On/Off.	
 

//배경색
COLOR g_ClearColor(0, 0.125f, 0.3f, 1.0f);
COLOR g_ClearColor2(0.25f, 0.25f, 0.25f, 1.0);


//
// 렌더링 상태 객체 생성 / 운용 함수들.
//
int  RenderStateObjectCreate();
void RenderStateObjectRelease();
void RenderStateUpdate();



//
// 깊이/스텐실 상태 객체들 : 엔진 전체 공유함.
//
ID3D11DepthStencilState* g_DSState[DS_MAX_];

int  DepthStencilStateCreate();
void DepthStencilStateRelease();



// 
// 레스터라이져 상태 객체 Rasterizer State Objects
// 
ID3D11RasterizerState*	g_RState[RS_MAX_] = { NULL, };

void RasterStateCreate();
void RasterStateRelease();
void RenderModeUpdate();


//
// 렌더링 모드 : 다수의 렌더링 모드 조합 및 운용을 위한 정의.
//
 
DWORD g_RMode = RM_DEFAULT;		//'현재' 렌더링 모드.



//
// 색상/알파 혼합 상태 객체 Blending State Objects 
//
ID3D11BlendState* g_BState[BS_MAX_] = { nullptr, };

void BlendStateCreate();
void BlendStateRelease();
void BlendModeUpdate();



//
// 텍스처 셈플러.
//
ID3D11SamplerState* g_pSampler[SS_MAX] = { nullptr, };
void SamplerCreate();
void SamplerRelease();






 

///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 

///////////////////////////////////////////////////////////////////////////// 
//
// DX 및 관련 객체 초기화 : 어플리케이션 시작시 1회 호출.
//
// 1.디바이스 생성 
// 2.스왑체인 및 렌더타겟 생성.
// 3.깊이/스텐실 버퍼 생성.
// 4.기타 렌더링 관련 장치 생성.
//
int DXSetup(HWND hwnd)
{
	
	//----------------------------------------
	// 1단계 : 렌더링 장치 관련 필수 설정
	//----------------------------------------
	// D3D 렌더링 장치 Device 및 스왑체인 Swap Chain 생성. 
	CreateDeviceSwapChain(hwnd);

	// 장치-스왑체인의 렌더타겟(백버퍼) 획득
	CreateRenderTarget();
	
	// 깊이/스텐실 버퍼 생성.
	CreateDepthStencil();
	
	// 장치 출력병합기(Output Merger) 에 렌터링 타겟 및 깊이-스텐실 버퍼 등록.
	g_pDXDC->OMSetRenderTargets(
				1,				// 렌더타겟 개수.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
				&g_pRTView,		// 렌더타겟("백버퍼") 등록.	
				g_pDSView		// 깊이/스텐실 버퍼 등록.
				);
	
	// 뷰포트 설정.
	SetViewPort();


	//장치 정보 획득
	GetDeviceInfo();


	//----------------------------------------
	// 2단계 : 기타 렌더링 관련 추가 설정.
	//----------------------------------------
	// 렌더링에 필요한 사용자 객체등을 생성/관리 합니다.
	// 카메라, 조명, 폰트, 셰이더 등등...
	
	//렌더링 상태 객체 생성.
	RenderStateObjectCreate(); 


	// 폰트 생성.--> Yena 에 포함됨.
	//ynFontCreate(g_pDevice);
	 
	// Yena 셋업 : 시스템 공통 객체 제공 
	// 내장 기본 카메라, 조명, 폰트, 방향축, 그리드, 등등..
	YenaSetup(g_pDevice, g_Mode, 5.0f);
	
	 
	//작업 완료, 장치 준비 완료.
	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////// 
//
// 디바이스 개체 및 관련 장치 제거 : 종료시 1회 호출.  
// 기본 렌더링 개체들 제거시 생성된 역순으로 처리합니다.(권장)
//
void DXRelease()
{ 	 
	//장치 상태 리셋 : 제거 전에 초기화를 해야 합니다. (메모리 누수 방지)
	if (g_pDXDC) g_pDXDC->ClearState();
	//if (g_pSwapChain) g_pSwapChain->SetFullscreenState(false, NULL);

	//예나 제거.
	YenaRelease(); 
	//ynFontRelease();					//폰트 제거.--> Yena 에 포함됨.

	//상태 객체 제거.
	RenderStateObjectRelease();


	SAFE_RELEASE(g_pDS);				//깊이/스텐실 버퍼 제거.
	SAFE_RELEASE(g_pDSView);			
	SAFE_RELEASE(g_pRTView);			//렌더타겟 제거.
	SAFE_RELEASE(g_pSwapChain);			//스왑체인 제거.
	SAFE_RELEASE(g_pDXDC);
	SAFE_RELEASE(g_pDevice);			//디바이스 제거. 맨 나중에 제거합니다.
}
 




/////////////////////////////////////////////////////////////////////////////
//
// D3D 렌더링 '장치(Device)' 및 스왑체인 생성. 
//
HRESULT CreateDeviceSwapChain(HWND hwnd)
{
	HRESULT hr = S_OK;

	// 장치 (Device) 및 스왑체인(SwapChain) 정보 구성.
	// 스왑체인은 다중버퍼링 시스템을 말하며
	// 고전적인 '플립핑Flipping' 체인과 동일한 의미입니다.  
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Windowed = g_bWindowMode;				//풀스크린 또는 창모드 선택.
	sd.OutputWindow = hwnd;						//출력할 윈도우 핸들.
	sd.BufferCount = 1;							//백버퍼 개수.
	sd.BufferDesc.Width = g_Mode.Width;			//해상도 결정.(백버퍼 크기)
	sd.BufferDesc.Height = g_Mode.Height;
	sd.BufferDesc.Format = g_Mode.Format;		//백버퍼 색상규격 (A8R8G8B8) 창모드에서는 생략 가능 
	sd.BufferDesc.RefreshRate.Numerator = g_bVSync ? 60 : 0;   //버퍼 갱신율.(수직동기화 VSync 활성화시 표준갱신율 적용 : 60hz)
	//sd.BufferDesc.RefreshRate.Numerator = 0;	//버퍼 갱신율.(수직동기화 VSync Off)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//용도 설정: '렌더타겟' 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//sd.Flags = 0;
	sd.SampleDesc.Count = g_dwAA;		//AA 설정
	sd.SampleDesc.Quality = 0;
	
	
	
	// D3D 렌더링 '장치(Device)' 및 스왑체인 생성. 
	//
	hr = D3D11CreateDeviceAndSwapChain(
			NULL,						//비디오 어뎁터 포인터 (기본장치는 NULL)
			D3D_DRIVER_TYPE_HARDWARE,	//HW 가속 
			NULL,						//SW Rasterizer DLL 핸들.  HW 가속시에는 NULL.
			0,							//디바이스 생성 플래그.(기본값)
			//D3D11_CREATE_DEVICE_DEBUG,//디바이스 생성 플래그.(디버그)
			&g_FeatureLevels,			//(생성할) 디바이스 기능 레벨(Feature Level) 배열
			1,							//(생성할) 디바이스 기능 레벨(Feature Level) 배열 크기.
			D3D11_SDK_VERSION,			//DX SDK 버전.
			&sd,						//디바이스 생성 상세 옵션.
			&g_pSwapChain,				//[출력] 스왑체인 인터페이스 얻기.
			&g_pDevice,					//[출력] 디바이스 인터페이스 얻기.
			NULL,						//[출력] (생성된) 디바이스 기능 레벨. 필요없다면 NULL 설정. 
			&g_pDXDC					//[출력] 디바이스 컨텍스트 얻기.
	);
	if (FAILED(hr))
	{
		ynError(hr, L"디바이스 / 스왑체인 생성 실패.");
	}
	
	return hr;
}





/////////////////////////////////////////////////////////////////////////////
//
// 장치-스왑체인의 렌더타겟(백버퍼) 획득
//
// 리소스뷰(Resource View)  
// DX11 의 리소스는 "공유" 목적으로 유연하게 설계되어 있습니다. 
// (메모리 관리의 효율성 증대 및 다용도 활용이 가능..)
// 따라서 사용전 각 용도에 맞는 접근방식(인터페이스)를 획득(생성)해야 합니다.
// 이 작업을 "리소스 뷰 Resource View" 를 통해 처리합니다.
//                     
// 스왑체인에 연결(Bind)된 렌더링 출력버퍼 - "백버퍼 BackBuffer" 는 2D 기반 리소스(텍스처) 이며
// "렌더타겟 RenderTarget" 용으로 리소스뷰를 생성, 해당 버퍼를 접근/운용 하겠습니다.
//
HRESULT CreateRenderTarget()
{
	HRESULT hr = S_OK;

	// 벡버퍼 획득.
	ID3D11Texture2D *pBackBuffer;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);		// 백버퍼 얻기, 2D 텍스쳐 행태, 화면출력용
	if (FAILED(hr))	return hr;

	//획득한 백버퍼에 렌더타겟 뷰 생성.(렌더타겟'형'으로 설정함)
	hr = g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRTView);		// '렌더타겟뷰'를 생성.	
	if (FAILED(hr)) 
	{
		ynError(hr, L"백버퍼-렌더타겟뷰 생성 실패.");
		//return hr;
	}

	//리소스 뷰 생성 후, 불필요한 DX 핸들은 해제해야 합니다.(메모리 누수 방지)
	SAFE_RELEASE(pBackBuffer);

	return hr;
}





/////////////////////////////////////////////////////////////////////////////
//
// 깊이-스텐실버퍼 생성. : DX11 에서는 렌더링버퍼-렌더타겟뷰- 와 마찬가지로, 
//                       깊이-스텐실버퍼역시 직접 만들어야 합니다.
//                       디바이스에 등록도 역시 수동입니다.
//
HRESULT CreateDepthStencil()
{
	HRESULT hr = S_OK;

	//깊이/스텐실 버퍼용 정보 구성.
	D3D11_TEXTURE2D_DESC   td;					
	ZeroMemory(&td, sizeof(td));
	td.Width = g_Mode.Width;
	td.Height = g_Mode.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	//td.Format = DXGI_FORMAT_D32_FLOAT;			// 32BIT. 깊이 버퍼.
	//td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// 깊이 버퍼 (24bit) + 스텐실 (8bit) / 구형 하드웨어 (DX9)
	td.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;	// 깊이 버퍼 (32bit) + 스텐실 (8bit) / 신형 하드웨어 (DX11)
	td.SampleDesc.Count = g_dwAA;				// AA 설정 - RT 과 동일 규격 준수.
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// 깊이-스텐실 버퍼용으로 설정.
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	// 깊이 버퍼 생성.
	//ID3D11Texture2D* pDS = NULL;						 
	hr = g_pDevice->CreateTexture2D(&td, NULL, &g_pDS);	
	if (FAILED(hr))
	{
		ynError(hr, L"깊이/스텐실 버퍼용 CreateTexture 실패.");		 
		return hr;
	}
	

	// 깊이-스텐실버퍼용 리소스 뷰 정보 설정. 
	D3D11_DEPTH_STENCIL_VIEW_DESC  dd;
	ZeroMemory(&dd, sizeof(dd));
	dd.Format = td.Format;
	dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; //AA 없음.
	//dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; //AA 적용.
	dd.Texture2D.MipSlice = 0;

	//깊이-스텐실 버퍼 뷰 생성.
	hr = g_pDevice->CreateDepthStencilView(g_pDS, &dd, &g_pDSView);
	if (FAILED(hr))
	{
		ynError(hr, L"깊이/스텐실뷰 생성 실패.");		 
		return hr;
	}

	//리소스 뷰 생성 후, 불필요한 DX 핸들은 해제해야 합니다.(메모리 누수 방지)
	//SAFE_RELEASE(pDS);
		
	return hr;
}




/////////////////////////////////////////////////////////////////////////////
//
// 뷰포트 설정 :  DX11 에서는 기본처리 되지 않으며 사용자가 직접 설정해야합니다 
//			     (OpenGL 에서는 예전부터 이렇게 해 왔습니다.)
//
void SetViewPort()
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)g_Mode.Width;
	vp.Height = (FLOAT)g_Mode.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pDXDC->RSSetViewports(1, &vp);

}




/////////////////////////////////////////////////////////////////////////////
//
// 장치-스왑체인/렌더타겟/깊이/스텐실 버퍼 클리어 
//
int ClearBackBuffer(UINT flag, COLOR col, float depth, UINT stencil)
{
	g_pDXDC->ClearRenderTargetView(g_pRTView, (float*)&col);			//렌더타겟 지우기.
	g_pDXDC->ClearDepthStencilView(g_pDSView, flag, depth, stencil);	//깊이/스텐실 지우기.
	
	return YN_OK;
}



/////////////////////////////////////////////////////////////////////////////
//
// 장치-스왑체인 전환 : 장면 그리기 완료시 꼭 호출해야 함.
//
int Flip()
{
	g_pSwapChain->Present(g_bVSync, 0);			//화면출력 : Flip! (+수직동기화)

	return YN_OK;
}








////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// 장치 렌더링 상태 객체 생성.
//
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//----------------------------------------------------------------------------
// 상태 객체 State Objects (DX10/11)
// DX10 부터 구형 TnL 의 RenderState 가 제거되었습니다.
// 이를 대신하는 것이 상태객체 State Objects 인터페이스로, 렌더링 상태를 하나의 그룹으로 
// 묶고 렌더링시 디바이스에 설정합니다.  이를 통해 장치의 어려 상태 변화를 한번에 설정하여 
// 불필요한 연산부하(Overhead) 를 줄이고 보다 효과적인 렌더링을 가능케 합니다.
//
// 상태객체는 엔진 초기시 제작후 사용하기를 권장하며 종료시 제거(Release) 해야 합니다.
// 상태객체는 수정불가능(Immutable, 읽기전용) 개체 입니다.
// DX9 에서는 State-Block 이 이와 유사한 기능을 담당했었습니다.
//
// 상태 객체 인터페이스 중 '레스터라이즈 스테이지 Rasterize Stage' 상태 조절은 
// ID3D11RasterizerState 인터페이스를 통해 처리합니다.  
// 간단하게 렌더링 설정/기능 모듬 정도로 생각합시다.  자세한 것은 다음을 참조하십시오. 
// 링크1 : 상태 객체 https://msdn.microsoft.com/en-us/library/windows/desktop/bb205071(v=vs.85).aspx
// 링크2 : 깊이버퍼 상태 구성하기 https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb205074(v=vs.85).aspx#Create_Depth_Stencil_State 
//----------------------------------------------------------------------------
//
int RenderStateObjectCreate()
{

	//깊이/스텐실 상태 객체 생성.
	DepthStencilStateCreate();

	//레스터 상태 개체 생성.
	RasterStateCreate();

	//셈플러 상태 개체 생성.
	SamplerCreate();

	//블렌드 상태 객체 생성.
	BlendStateCreate();


	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//  상태객체 제거하기 : 엔진 종료시 1회 호출.
//
void RenderStateObjectRelease()
{
	DepthStencilStateRelease();		//깊이/스텐실 상태 객체 제거.
	RasterStateRelease();			//레스터 상태 객체 제거.
	SamplerRelease();				//셈플러 상태 객체 제거.
	BlendStateRelease();			//블렌드 상태 객체 제거.
}





///////////////////////////////////////////////////////////////////////////////
//
//  렌더링 상태 갱신하기. 
//
void RenderStateUpdate()
{
	// 렌더링 옵션 조절 	 
	if (IsKeyUp(VK_SPACE))	g_bWireFrame ^= TRUE;
	if (IsKeyUp(VK_F4))		g_bCullBack ^= TRUE;
	
	//if (IsKeyUp(VK_F5))		g_bDiffOn ^= TRUE;		
	//if (IsKeyUp(VK_F6))		g_bSpecOn ^= TRUE;		
	//if (IsKeyUp(VK_F5))		g_bZEnable ^= TRUE;		
	//if (IsKeyUp(VK_F6))		g_bZWrite ^= TRUE;		

	// 배경색 설정.
	if (g_bWireFrame) g_ClearColor = COLOR(0.15f, 0.15f, 0.15f, 1.0f);
	else			  g_ClearColor = COLOR(0, 0.125f, 0.3f, 1.0f); 
	
	// 렌더링 모드 전환.	  
	RenderModeUpdate();
	 	
	// 깊이 연산 모드 전환.	 
	if (g_bZEnable)
	{
		if (g_bZWrite)  
			  g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON], 0);			//깊이 버퍼 동작 (기본값)
		else  g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_WRITE_OFF], 0);	//깊이 버퍼  : Z-Test On + Z-Write Off.
	}
	else  g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF], 0);	//깊이 버퍼 비활성화 : Z-Test Off + Z-Write Off.
	 

	//셈플러 설정.(기본설정)
	g_pDXDC->PSSetSamplers(0, 1, &g_pSampler[SS_DEFAULT]); 

}






/////////////////////////////////////////////////////////////////////////////// 
//
// 렌더링 모드 체크 : 각 렌더링 모드 상태별 On/Off 처리.
//
#define CheckRMode(k, v) if((k)) g_RMode |= (v); else g_RMode &= ~(v);


/////////////////////////////////////////////////////////////////////////////// 
//
// 렌더링 모드 전환
//
void RenderModeUpdate()
{	
	// 렌더링 모드 체크 : 사용자가 지정한 렌더링 상태 조합.
	CheckRMode(g_bCullBack, RM_CULLBACK);
	CheckRMode(g_bWireFrame, RM_WIREFRAME);


	// 레스터 모드 전환 : 지정된 모드의 조합을 통해 렌더링 상태를 조절.
	switch (g_RMode)
	{
	default:
	case RM_SOLID:
		g_pDXDC->RSSetState(g_RState[RS_SOLID]);
		break;
	case RM_WIREFRAME:
		g_pDXDC->RSSetState(g_RState[RS_WIREFRM]);
		break;
	case RM_CULLBACK:
		g_pDXDC->RSSetState(g_RState[RS_CULLBACK]);
		break;
	case RM_WIREFRAME | RM_CULLBACK:
		g_pDXDC->RSSetState(g_RState[RS_WIRECULLBACK]);
		break;
	} 

}














///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  깊이/스텐실 버퍼 상태객체 생성하기
//
int DepthStencilStateCreate()
{
	
	//----------------------------
	// 깊이/스텐실 상태 개체 생성.: "출력병합기 Output Merger" 상태 조절. 
	//----------------------------
	//...	 
	D3D11_DEPTH_STENCIL_DESC  ds;
	//깊이 버퍼 설정 (기본값)
	ds.DepthEnable = TRUE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS;
	//스텐실 버퍼 설정 (기본값) 
	ds.StencilEnable = FALSE;								//스텐실 버퍼 OFF.
	ds.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;	//스텐실 읽기 마스크 (8bit: 0xff)
	ds.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK; //스텐실 쓰기 마스크 (8bit: 0xff)
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;	//[앞면] 스텐실 비교 함수 : "Always" 즉, 항상 성공 (통과, pass)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;	//[앞면] 스텐실 비교 성공시 동작 : 기존값 유지.
	ds.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;	//[앞면] 스텐실 비교 실패시 동작 : 기존값 유지.	
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//[앞면] 스텐실/깊이 비교 실패시 동작 : 기존값 유지.
	ds.BackFace = ds.FrontFace;									//[뒷면] 설정 동일. 필요시 개별 설정이 가능.
 
	
	//----------------------------------------------------------------------
	// 깊이 버퍼 연산 객체들.
	//----------------------------------------------------------------------
	// DS 상태 객체 #0 : Z-Test ON! (기본값)
	ds.DepthEnable = TRUE;					//깊이 버퍼 On.
	ds.StencilEnable = FALSE;				//스텐실 버퍼 Off!
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_ON]);

	// DS 상태 객체 #1 : Z-Test OFF 상태.
	ds.DepthEnable = FALSE;
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_OFF]);

	// DS 상태 객체 #2 : Z-Test On + Z-Write OFF.
	// Z-Test (ZEnable, DepthEnable) 이 꺼지면, Z-Write 역시 비활성화 됩니다.
	ds.DepthEnable = TRUE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;		//깊이값 쓰기 끔.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_WRITE_OFF]);


	//----------------------------------------------------------------------
	// 스텐실 버퍼 연산 객체들 생성.
	//----------------------------------------------------------------------
	// 스텐실 버퍼 비트 연산 공식.
	// (Stencil.Ref & Stencil.Mask) Comparison-Func ( StencilBuffer.Value & Stencil.Mask)
	//
	// *StencilBufferValue : 현재 검사할 픽셀의 스텐실값.
	// *ComFunc : 비교 함수. ( > < >= <= ==  Always Never)
	//----------------------------------------------------------------------
	// DS 상태객체 #4 :  깊이버퍼 On, 스텐실버퍼 ON (항상, 참조값 쓰기) : "깊이/스텐실 기록" 
	ds.DepthEnable	  = TRUE;										//깊이버퍼 ON! (기본값)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc	  = D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//비교함수 : "항상 통과" (성공)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//성공시 : 참조값(Stencil Reference Value) 로 교체.
	//ds.FrontFace.StencilFailOp	  = D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.(기본값, 생략)
	//ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.(기본값, 생략)
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_ON_STENCIL_ON]);
	

	// DS 상태객체 #5 : 깊이버퍼 On, 스텐실버퍼 ON (동일비교, 성공시 유지) : "지정 위치에만 그리기" 
	//ds.DepthEnable	= TRUE;										//깊이버퍼 ON! (기본값)(생략)
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//비교함수 : "동일한가?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;		//성공시 : 유지.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_ON_STENCIL_EQUAL_KEEP]);


	// DS 상태객체 #6 : 깊이버퍼 On, 스텐실버퍼 ON (다름비교, 성공시 유지) : "지정 위치 이외에 그리기" 
	//ds.DepthEnable	= TRUE;										//깊이버퍼 ON! (기본값)(생략)
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON!
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_NOT_EQUAL;	//비교함수 : "같이 않은가?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;		//성공시 : 유지.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP]);


	/*// DS 상태객체 #7 : 깊이버퍼 Off, 스텐실버퍼 ON (참조값 쓰기) : "스텐실만 기록" 
	ds.DepthEnable	  = FALSE;										//깊이버퍼 OFF!
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//비교함수 : "항상 통과" (성공)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//성공시 : 참조값(Stencil Reference Value) 로 교체.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_OFF_STENCIL_ON]);
	*/
	
	/*// DS 상태객체 #8 : 깊이버퍼 On, 스텐실버퍼 ON (동일비교, 성시 증가) : "이중그리기 방지용" 
	//ds.DepthEnable	= TRUE;										//깊이버퍼 ON! (기본값)(생략)
	//ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//ds.DepthFunc		= D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//비교함수 : "동일한가?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_INCR;		//성공시 : 증가 (+1) 
	ds.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_ON_STENCIL_EQUAL_INCR]);
	*/

	// DS 상태객체 #9 : 깊이버퍼 On, 스텐실버퍼 ON (항상, 성공시 증가) : "스텐실만 기록" 
	ds.DepthEnable	  = TRUE;										//깊이버퍼 ON! (기본값)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				//깊이버퍼 쓰기 OFF.
	ds.DepthFunc	  = D3D11_COMPARISON_LESS;						//깊이연산 ON. (기본값)
	ds.StencilEnable  = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//비교함수 : "항상 통과" (성공)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//성공시 : 참조값(Stencil Reference Value) 로 교체.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_WRITE_OFF_STENCIL_ON]);
	

	//깊이버퍼 Off (Write Off), 스텐실버퍼 ON (동일비교, 성공시 증가) : "이중그리기 방지.
	ds.DepthEnable	= FALSE;										//깊이테스트 Off!
	//ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				//깊이값 쓰기 Off.
	//ds.DepthFunc		= D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//비교함수 : "동일한가?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_INCR;		//성공시 : 증가 (+1) 
	ds.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_OFF_STENCIL_EQUAL_INCR]);


	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//  상태객체 제거하기 : 엔진 종료시 1회 호출.
//
void DepthStencilStateRelease()
{
	for (int i = 0; i < DS_MAX_; i++)
		SAFE_RELEASE(g_DSState[i]);
}






////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
// 
// 레스터 상태 객체 생성.
//
void RasterStateCreate()
{
	//[상태객체 1] 기본 렌더링 상태 개체.
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;		//삼각형 색상 채우기.(기본값)
	rd.CullMode = D3D11_CULL_NONE;		//컬링 없음. (기본값은 컬링 Back)		
	rd.FrontCounterClockwise = false;   //이하 기본값...
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;		//AA 적용.
	rd.AntialiasedLineEnable = false;
	//레스터라이져 상태 객체 생성.
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_SOLID]);


	//[상태객체2] 와이어 프레임 그리기. 
	rd.FillMode = D3D11_FILL_WIREFRAME; 
	rd.CullMode = D3D11_CULL_NONE;
	//레스터라이져 객체 생성.
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_WIREFRM]);

	//[상태객체3] 후면 컬링 On! "CCW"
	rd.FillMode = D3D11_FILL_SOLID; 
	rd.CullMode = D3D11_CULL_BACK;
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_CULLBACK]);
	
	//[상태객체4] 와이어 프레임 + 후면컬링 On! "CCW"
	rd.FillMode = D3D11_FILL_WIREFRAME; 
	rd.CullMode = D3D11_CULL_BACK; 
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_WIRECULLBACK]);

	//[상태객체5] 정면 컬링 On! "CW"
	rd.FillMode = D3D11_FILL_SOLID; 
	rd.CullMode = D3D11_CULL_FRONT;
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_CULLFRONT]);
	
	//[상태객체6] 와이어 프레임 + 정면컬링 On! "CW" 
	rd.FillMode = D3D11_FILL_WIREFRAME; 
	rd.CullMode = D3D11_CULL_FRONT;
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_WIRECULLFRONT]);
	

}




////////////////////////////////////////////////////////////////////////////// 
//
// 레스터 상태 객체 제거 : 어플리케이션 종료시 호출.
//
void RasterStateRelease()
{
	for (int i = 0; i<RS_MAX_; i++)
	{
		SAFE_RELEASE(g_RState[i]);
	}

}






////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
// 
// 블렌딩 상태 객체 생성.
//
void BlendStateCreate()
{
	
	//----------------------------------------------------------------
	// [알림] 다중 렌터타겟(RenderTarget) 사용시 각 RT 별 지정이 가능하나
	// 당분간 RT #0 (기본)-백버퍼-만을 사용하겠습니다.
	//----------------------------------------------------------------
	
	//------------------------------
	// 혼합객체1 : 색상혼합 없음. "Blending OFF"
	//------------------------------
	//블렌딩 상태 객체 구성 옵션 : 이하 기본값들.
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.RenderTarget[0].BlendEnable = FALSE;							//블렌딩 동작 결정. 기본값은 FALSE(OFF)		
	//색상 성분 혼합 : Color Blending.(기본값)
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//색상 혼합 연산(Color - Operation), 기본값은 덧셈 : 최종색 = Src.Color + Dest.Color 	
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//소스(원본) 혼합 비율, 원본 100% : Src.Color = Src * 1;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;				//타겟(대상) 혼합 비율, 대상 0%   : Dest.Color = Dest * 0;  통상 0번 RT 는 "백버퍼"를 의미합니다.	
 	//알파 성분 혼합 : Alpha Blending.(기본값)
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			//알파 혼합 함수(Alpha - Opertion), 기본값은 덧셈.
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;				//소스(원본) 알파 혼합 비율.
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//타겟(대상) 알파 혼합 비율.
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//렌더타겟에 쓰기 옵션.
	//bd.AlphaToCoverageEnable = FALSE;								//이하 추가 옵션.(기본값, 생략)
	//bd.IndependentBlendEnable = FALSE;
	g_pDevice->CreateBlendState(&bd, &g_BState[BS_DEFAULT]);		//상태 개체.생성.
	

	//------------------------------
	// 혼합객체2 : 알파 혼합 "Alpha Blending"
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//색상 혼합 ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//색상 혼합 연산 (덧셈, 기본값) : 최종색 = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//소스(원본) 혼합 비율, 원본 알파비율  : Src.Color = Src * Src.a;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		//타겟(대상) 혼합 비율, 원본 알파비율 반전 : Dest.Color = Dest * (1-Src.a);			 		
	g_pDevice->CreateBlendState(&bd, &g_BState[BS_ALPHA_BLEND]);

		
	//------------------------------
	// 혼합객체3 : 색상 혼합 "Color Blending"
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//색상 혼합 ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//색상 혼합 연산 (덧셈, 기본값) : 최종색 = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;			//소스(원본) 혼합 비율, 원본 색상  : Src.Color = Src * Src.Color;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;		//타겟(대상) 혼합 비율, 원본 색상 반전 : Dest.Color = Dest * (1-Src.Color);			 	
	g_pDevice->CreateBlendState(&bd, &g_BState[BS_COLOR_BLEND]);
	

	//------------------------------
	// 혼합객체3 : 색상 혼합 "밝기 강조 Blending 1:1" 
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//색상 혼합 ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//색상 혼합 연산 "밝기강조" : 최종색 = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//소스(원본) 혼합 비율, 원본 색상  : Src.Color = Src * 1;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;					//타겟(대상) 혼합 비율, 원본 색상 반전 : Dest.Color = Dest * 1;			 	
	g_pDevice->CreateBlendState(&bd, &g_BState[BS_COLOR_BLEND_ONE]);

}




////////////////////////////////////////////////////////////////////////////// 
//
// 블렌딩 상태 객체 제거 : 어플리케이션 종료시 호출.
//
void BlendStateRelease()
{
	for (int i = 0; i<BS_MAX_; i++)
	{
		SAFE_RELEASE(g_BState[i]);
	}

}




 


////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
// 
// 텍스처 셈플러 상태 객체 생성.
//
void SamplerCreate()
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));		

	//텍스처 필터 : 비등방 필터링
	sd.Filter = D3D11_FILTER_ANISOTROPIC; 
	sd.MaxAnisotropy = g_dwAF; 
	//밉멥핑	
	sd.MinLOD = 0;							
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	//이하 기본값 처리..
 	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.BorderColor[0] = 1;
	sd.BorderColor[1] = 1;
	sd.BorderColor[2] = 1;
	sd.BorderColor[3] = 1;
	
	//샘플러 객체1 생성. (DX 기본값)
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		
	hr = g_pDevice->CreateSamplerState(&sd, &g_pSampler[SS_CLAMP]);

	//샘플러 객체2 생성.
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = g_pDevice->CreateSamplerState(&sd, &g_pSampler[SS_WRAP]);
	
}




////////////////////////////////////////////////////////////////////////////// 
// 
// 텍스처 셈플러 상태 객체 제거.
//
void SamplerRelease()
{
	for(int i=0; i<SS_MAX; i++)
		SAFE_RELEASE(g_pSampler[i]); 
}











///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// 엔진 타이머 함수들
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// 초당 프레임수 출력.
//
// 인자 : int x, y : 출력할 화면 좌표.
//
void PutFPS(int x, int y)
{
	static int oldtime = GetTickCount();
	int nowtime = GetTickCount();

	static int frmcnt = 0;
	static float fps = 0.0f;

	++frmcnt;
	
	int time = nowtime - oldtime;
	if( time >= 999)
	{
		oldtime = nowtime;

		fps = (float)frmcnt*1000/(float)time;
		frmcnt = 0;
	}
	
	//ynTextDraw(x, y, RGB(0, 0, 255), L"FPS:%.1f/%d", fps, frmcnt );
	ynTextDraw(x, y, COLOR(0, 1, 0, 1), L"FPS:%.1f/%d", fps, frmcnt);
}




/////////////////////////////////////////////////////////////////////////////
//
// 엔진 동기화용 경과시간 획득.
//
float GetEngineTime()
{	 
	static int oldtime = GetTickCount();
	int nowtime = GetTickCount();
	float dTime = (nowtime - oldtime) * 0.001f;
	oldtime = nowtime;

	return dTime; 
}





////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// 에러 처리 함수들.
//
////////////////////////////////////////////////////////////////////////////// 

////////////////////////////////////////////////////////////////////////////// 
//
// 에러 메세지 처리
//
int ynErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...)
{
	TCHAR msgva[1024] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, msg, vl);
	va_end(vl);
	

	//HRESULT 에서 에러 메세지 얻기.
	TCHAR herr[1024] = L"";
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0,  hr, 
				   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				   herr, 1024, NULL);

	//에러 메세지 붙이기.
	TCHAR errmsg[2048] = L"";
	_stprintf(errmsg, L"%s \n에러코드(0x%X) : %s \nFile = %s (%d)\nFunc = %s\n\n",
		msgva, hr, herr,
		file, line, func );
	 

	//(디버깅 중) VS 출력창으로 출력..
	OutputDebugString(L"\n");
	OutputDebugString(errmsg);

	//로그파일로 출력.
	//...

	//메세지 창 출력..
	if (bMBox)
	{
		MessageBox(NULL, errmsg, L"Yena::Error", MB_OK | MB_ICONERROR);
		//GetLastError(hr);
	}

	return YN_OK;
}



////////////////////////////////////////////////////////////////////////////// 
//
// 에러 메세지 처리 : 셰이더 에러 처리용.
// 
int ynErrorW(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob, 
			TCHAR* filename, char* EntryPoint, char* ShaderModel )
{

	/*//가변매개변수 처리.
	TCHAR msgva[2048] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, msg, vl);
	va_end(vl);
	*/
	//파라미터, 유니코드로 전환.
	TCHAR func[80] = L"";
	::mbstowcs(func, EntryPoint, strlen(EntryPoint));
	TCHAR sm[20] = L"";
	::mbstowcs(sm, ShaderModel, strlen(ShaderModel));


	//셰이더 오류 메세지 읽기.
	TCHAR errw[4096] = L"";
 	::mbstowcs(errw, (char*)pBlob->GetBufferPointer(), pBlob->GetBufferSize());
 

	//HRESULT 에서 에러 메세지 얻기 
	//시스템으로 부터 얻는 셰이더 오류메세지는 부정확하므로 생략.
	TCHAR herr[1024] = L"아래의 오류를 확인하십시오.";
	/*FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		herr, 1024, NULL);
	*/
	

	//에러 메세지 붙이기.
	TCHAR errmsg[1024];
	_stprintf(errmsg, L"%s \nFile=%s  Entry=%s  Target=%s  \n에러코드(0x%08X) : %s \n\n%s", 
				msg, filename, func, sm,
				hr, herr, errw ); 
	

	//(디버깅 중) VS 출력창으로 출력..
	OutputDebugString(L"\n");
	OutputDebugString(errmsg);
	//OutputDebugString(errw);


	//로그파일로 출력.
	//...


	//메세지 창 출력..
	if (bMBox)
	{
		MessageBox(NULL, errmsg, L"Yena::Error", MB_OK | MB_ICONERROR);
		//MessageBox(NULL, errw, L"Yena::Error", MB_OK | MB_ICONERROR);
	}
		
	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////// 
//
// 에러 메세지 처리
//
int ynErrorW(TCHAR* msg, ...)
{
	TCHAR msgva[2048] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, msg, vl);
	va_end(vl);
	

	//(디버깅 중) VS 출력창으로 출력..
	OutputDebugString(L"\n");
	OutputDebugString(msgva);

	//로그파일로 출력.
	//...

	//메세지 창 출력..
	{
		MessageBox(NULL, msgva, L"Yena::Error", MB_OK | MB_ICONEXCLAMATION);
		//GetLastError(hr);
	}

	return YN_OK;
}




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// 장치/GPU 정보 획득 함수들
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// 장치/GPU 정보 획득
//
void GetDeviceInfo()
{
 	//장치 기능레벨 확인.
	GetFeatureLevel();

	//GPU 정보 얻기.
	GetAdapterInfo(&g_Adc);

	//모니터 정보 얻기.
	//...
		 
}




/////////////////////////////////////////////////////////////////////////////
//
//  DX 기능 레벨 구하기.
//
static TCHAR* strFeature[4][4] =
{
	{ L"DX9",   L"DX9.1",  L"DX9.2", L"DX9.3" },
	{ L"DX10",  L"DX10.1", L"N/A",   L"N/A" },
	{ L"DX11",  L"DX11.1", L"N/A",   L"N/A" },
	{ L"DX12",  L"DX12.1"  L"N/A",   L"N/A" }
};

void GetFeatureLevel()
{
	UINT feat = g_FeatureLevels;
	UINT ver = 0;
	UINT sub = 0;

	#define OFFSET 0x9;

	ver = ((feat & 0xf000) >> 12) - OFFSET;	//메인 버전 산출.   	
	sub = ((feat & 0x0f00) >> 8);			//하위 버전 산출.
	
	g_strFeatureLevel = strFeature[ver][sub];
}




/////////////////////////////////////////////////////////////////////////////
//
// 장치/GPU 정보 획득 
//
// 기본 장치만 처리함.다중 GPU 구성시에는 별도의 열거 처리가 필요.
// 관련항목1 : DirectX Graphics Infrastructure (DXGI) /  https://msdn.microsoft.com/ko-kr/library/windows/desktop/ee417025(v=vs.85).aspx
// 관련항목2 : How To: Enumerate Adapters  /  https://msdn.microsoft.com/en-us/library/windows/desktop/ff476877(v=vs.85).aspx 
// 관련항목3 : DXGI_ADAPTER_DESC structure /  https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb173058(v=vs.85).aspx
//

// 디바이스/GPU 정보 획득 <방법 #1> 
// DXGI 1.1 / DXGI Factory 사용 / DX11 이상 권장
//
HRESULT GetAdapterInfo(DXGI_ADAPTER_DESC1* pAd)
{
	IDXGIAdapter1* pAdapter;
	IDXGIFactory1* pFactory = NULL;
	//DXGI_ADAPTER_DESC ad;

	//DXGIFactory 개체 생성. (DXGI.lib 필요)
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
	{
		return E_FAIL;
	}

	pFactory->EnumAdapters1(0, &pAdapter);		//어뎁터 획득.
	pAdapter->GetDesc1(pAd);					//어뎁터 정보 획득.
	//*pAd = ad;								//외부로 복사.

	//정보 취득후, 접근한 인터페이스를 해제합니다. (메모리 누수 방지)
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);

	return S_OK;
}

/*
// 디바이스/GPU 정보 획득 <방법 #2>  
// DXGI 1.1 / 지정 장치 Interface 사용 / DX11 이상 권장
//
HRESULT GetAdapterInfo(DXGI_ADAPTER_DESC1* pAd)
{
	IDXGIDevice* pDXGIDev;
	IDXGIAdapter1* pAdapter;

	//DXGIFactory 개체 생성. (DXGI.lib 필요)
	if (FAILED(g_pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGIDev)))
	{
		return E_FAIL;
	}

	pDXGIDev->GetAdapter((IDXGIAdapter**)&pAdapter);		//어뎁터 획득.
	pAdapter->GetDesc1(pAd);					//어뎁터 정보 획득.

	//정보 취득후, 접근한 인터페이스를 해제합니다. (메모리 누수 방지)
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pDXGIDev);

	return S_OK;
}
*/




/////////////////////////////////////////////////////////////////////////////
//
// 장치/GPU 정보 출력
//
// 관련항목1 : DXGI_ADAPTER_DESC structure
// 참조링크1 : https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb173058(v=vs.85).aspx
//
void AdapterInfo(int x, int y, COLOR col)
{
	#define ToMB(a) (a/1024/1024)

	DXGI_ADAPTER_DESC1& dc = g_Adc;
	ynTextDraw(x, y += 14, COLOR(0, 1, 0, 1), L"[GPU #%d]", 0);
	//GPU 정보.
	ynTextDraw(x, y += 14, col, L"Adapter: %u", 0 );		//기본 장치만 처리함. 다중 GPU 구성시에는  열거처리가 필요.
	ynTextDraw(x, y += 14, col, L"Description: %s", dc.Description);
	ynTextDraw(x, y += 14, col, L"VendorID: %u", dc.VendorId);
	ynTextDraw(x, y += 14, col, L"DeviceID: %u", dc.DeviceId);
	ynTextDraw(x, y += 14, col, L"SubSysID: %u", dc.SubSysId);
	ynTextDraw(x, y += 14, col, L"Revision: %u", dc.Revision);
	ynTextDraw(x, y += 14, col, L"VideoMem: %lu MB", ToMB(dc.DedicatedVideoMemory));
	ynTextDraw(x, y += 14, col, L"SystemMem: %lu MB", ToMB(dc.DedicatedSystemMemory));
	ynTextDraw(x, y += 14, col, L"SharedSysMem: %lu MB", ToMB(dc.SharedSystemMemory));
	ynTextDraw(x, y += 14, col, L"AdpaterLuid: %u.%d", dc.AdapterLuid.HighPart, dc.AdapterLuid.LowPart);
		 
}

 


/////////////////////////////////////////////////////////////////////////////
//
// 장치/GPU 및 시스템 정보 출력
//
void SystemInfo(int x, int y, COLOR col)
{
	COLOR col2 = col * 0.7f;

	ynTextDraw(x, y += 14, col, L"[SYSTEM]");
	ynTextDraw(x, y += 14, col, L"VGA: %s", g_Adc.Description);
	ynTextDraw(x, y += 14, col, L"Feat: %s", g_strFeatureLevel);
	ynTextDraw(x, y += 14, col, L"해상도: %dx%d", g_Mode.Width, g_Mode.Height);
	
	//GPU 정보 출력.
	//AdapterInfo(x, y += 14, col);
	
	//Yena 정보 출력.
	YenaSysInfo(x, y, col);

	//그래픽 옵션 출력.
	{
		x = g_Mode.Width - 80;
		y = 50;
		//col = COLOR(1, 1, 0, 1);
		ynTextDraw(x, y += 14, col, L"[Graphics]");
		ynTextDraw(x, y += 14, col, L"AA = x%d", g_dwAA);
		ynTextDraw(x, y += 14, col, L"AF = x%d", g_dwAF);
	}

}








/////////////////////////////////////////////////////////////////////////////
//
// 외부파일에서 데이터 로드하기. 
//
//bool LoadIniData(TCHAR* pTexName0, TCHAR* pTexName1)
bool LoadIniData(TCHAR* pModelName, TCHAR* pTexName)
{
	FILE* fp = _tfopen(L"data.ini", L"rt");
 	if(fp == NULL) return false;

	TCHAR token[256];
	TCHAR line[256];
 
	while(1)
	{
		if(feof(fp)) break;

		//한줄 읽기.
 		_fgetts(line, 256, fp);	//라인의 '공백'때문에 fscanf 는 쓸수 없다..
		_stscanf(line, L"%s", token);

		//주석 검사.  
		if(!_tcsnicmp(token, L"//", 2)) continue;	//주석. 
 
		if(!_tcsnicmp(token, L"Model", 5) && pModelName )
		{
			swscanf(line, L" Model = %s ", pModelName);
			continue;
		}



		if(!_tcsnicmp(token, L"Tex", 3) && pTexName )
		{
			 swscanf(line, L" Tex = %s", pTexName);
			//strcpy(pTextureName, pTexNameCube);
			continue; 
		}


		/*if(!_tcsnicmp(token, L"Tex0", 4))
		{
			_stscanf(line, L"Tex0 = %s", pTexName0);		 	
			//strcpy(pTextureName, pTexNameCube);	 
			continue;
		}

		if(!_tcsnicmp(token, L"Tex1", 4))
		{
			_stscanf(line, L"Tex1 = %s", pTexName1);		 	
			//strcpy(pTextureName, pTexNameFloor);
			continue; 
		}*/
		 
	}

	return true;
}






////////////////////////////////////////////////////////////////////////////// 
//
// 텍스처 정보 얻기.
//
HRESULT GetTextureDesc(ID3D11ShaderResourceView* pRV, D3D11_TEXTURE2D_DESC* pDesc)
{
	HRESULT res = S_OK;

	//셰이더 리소스 뷰 정보 획득.
	D3D11_SHADER_RESOURCE_VIEW_DESC dc;
	pRV->GetDesc(&dc);

	//텍스처 정보 획득.
	ID3D11Resource* pRes = nullptr;
	pRV->GetResource(&pRes);
	ID3D11Texture2D* pTex = nullptr;
	pRes->QueryInterface<ID3D11Texture2D>(&pTex);
	if (pTex)
	{
		pTex->GetDesc(pDesc);

		//정보 획득후 인터페이스 제거.
		pTex->Release();			
	}
		
	//정보 획득후 인터페이스 제거.
	//SAFE_RELEASE(pRes);

	return res;
}



////////////////////////////////////////////////////////////////////////////// 
//
// 텍스처 로드
//
HRESULT LoadTexture(TCHAR* filename, ID3D11ShaderResourceView** ppTexRV)
{
	HRESULT hr = S_OK;	
	 
	ID3D11ShaderResourceView* pTexRV = nullptr;

	//TCHAR* fileName = L"../data/sonim.jpg";
	
	//WIC 파일 로드.
	//hr = DirectX::CreateWICTextureFromFile( g_pDevice, fileName, nullptr, &g_pTextureRV );	
	
	//WIC 파일 로드 (+밉멥)
	hr = DirectX::CreateWICTextureFromFileEx( g_pDevice, g_pDXDC, filename, 0, 
				D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET,
				0,	D3D11_RESOURCE_MISC_GENERATE_MIPS,	WIC_LOADER_DEFAULT, 
				nullptr, &pTexRV );	

	if (FAILED(hr))
	{
		//실패시, DDS 파일로 로드 시도 (+밉멥)
		/*hr = DirectX::CreateDDSTextureFromFileEx( g_pDevice, g_pDXDC, filename, 0, 
				D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET,
				0,	D3D11_RESOURCE_MISC_GENERATE_MIPS,	false,	nullptr, &pTexRV );	 */
		hr = DirectX::CreateDDSTextureFromFile( g_pDevice, filename, nullptr, &pTexRV );	
		if (FAILED(hr))
		{
			ynError(hr, L"텍스처 로드 실패 \nFile=%s", filename);
			return hr;
		}
	}

	

	//텍스처 밉멥생성.
	//g_pDXDC->GenerateMips(g_pTextureRV);
	
	//외부로 리턴.
	*ppTexRV = pTexRV;

	return hr;
}




/*  Yena Math 에 포함됨.
/////////////////////////////////////////////////////////////////////////////
//
// 색상 연산자 오버로딩.
//
COLOR operator * (COLOR& lhs, float rhs)
{
	COLOR col;
	col.x = lhs.x * rhs;
	col.y = lhs.y * rhs;
	col.z = lhs.z * rhs;
	col.w = lhs.w * rhs;

	return col;
}
*/




//////////////////////////////////////////////////////////////////////////////// 
//
//int CreateVB(void* vtx, DWORD vcnt, DWORD vbsize)
HRESULT CreateVB(LPDEVICE pDev, void* pBuff, UINT size, LPVERTEXBUFFER* ppVB)
{ 
	//int res = YN_OK;
	
	//버퍼 정보 확인.  
	//...

	//m_pDev = pDev;
	LPVERTEXBUFFER pVB = nullptr;
	
	//정점 버퍼 생성
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					//버퍼 사용방식
	bd.ByteWidth = size;							//버퍼 크기
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//버퍼 용도 : "정점 버퍼" 용로 설정 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA rd;				
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = pBuff;								//버퍼에 들어갈 데이터 설정 : "정점들"..
	
	//정점 버퍼 생성.
	HRESULT hr = pDev->CreateBuffer(&bd, &rd, &pVB);
	if (FAILED(hr))
	{
		//에러 검증 및 예외처리..
		//...
		ynError(hr, L"CreateVB : 정점버퍼 생성 실패");
		return hr;
	}
	
	//외부로 리턴.
	*ppVB = pVB;

	return hr;

}//end of CreateVB()






////////////////////////////////////////////////////////////////////////////////
//
HRESULT CreateIB(LPDEVICE pDev, void* pBuff, UINT size, LPINDEXBUFFER* ppIB)
{
	//int res = YN_OK;

	//버퍼 정보 확인. 
	//DWORD ibsize = sizeof(XINDEX)*faceCnt;
		
	LPINDEXBUFFER pIB = nullptr;

	// 인덱스 버퍼 Index Buffer 생성 
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					//버퍼 사용방식
	bd.ByteWidth = size;							//버퍼 크기
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;			//버퍼 용도 : "색인 버퍼" 용로 설정 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA rd;				
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = pBuff;								//버퍼에 들어갈 데이터 설정 : "색인 정보"..
	
	//색인 버퍼 생성.
	HRESULT hr = pDev->CreateBuffer(&bd, &rd, &pIB);
	if (FAILED(hr))
	{
		//에러 검증 및 예외처리..
		//...
		ynError(hr, L"CreateIB : 인덱스버퍼 생성 실패");
		return hr;
	}
	
	//노드 정보 갱신.
	//m_FaceCnt = ibsize / sizeof(XINDEX);		//(IB 를 사용하여 그릴) 총 삼각형 개수 -> XFile 에서 발췌.
	//m_IndexedVtxCnt = m_FaceCnt * 3;			//(IB 를 사용하여 그릴) 총 정점개수.
	//m_FullVtxCnt = m_FaceCnt * 3;
	
	//외부로 리턴.
	*ppIB = pIB;

	return hr;

}//end of CreateIB() 




/////////////////////////////////////////////////////////////////////////////// 
//
//  3D / 통상 그리기 모드로 전환.
//
void Mode_3D()
{ 

	//--------------------------
	// 카메라 정보 복구  : "3D 카메라" 
	//--------------------------
	// 카메라 클래스가 있다면 보다 간편해지겠지요.
	// 현재 데모에서는 yn_View, yn_Proj 에 저장되어있습니다.
	g_cbDef.mView = yn_View;
	g_cbDef.mProj = yn_Proj;
	//UpdateDynamicConstantBuffer(g_pDXDC, g_pCBDef, &g_cbDef, sizeof(cbDEFAULT)); 


	//--------------------------
	// 렌더링 상태 설정
	//--------------------------
	// 조명 켬.
	//... 

	//텍스처 셈플러 복구 
	g_pDXDC->PSSetSamplers(0, 1, &g_pSampler[SS_WRAP]);

	//렌더링 옵션 복구.. 
	RenderModeUpdate();
	//g_pDXDC->RSSetState(g_RState[RS_CULL_CCW]);
	// 깊이/스텐실 연산 복구.
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON], 0);			//깊이버퍼 동작 (기본값) 

}





/////////////////////////////////////////////////////////////////////////////// 
//
//  2D / UI 모드 전환.
//
void Mode_2D()
{

	//--------------------------
	// 원본 "3D 카메라" 정보 백업.
	//--------------------------
	// 카메라 클래스가 있다면 보다 간편해지겠지요.
	// 현재 데모에서는 yn_View, yn_Proj 에 저장되어있습니다.
	//...


	//--------------------------
	// 2D 카메라 설정.
	//--------------------------
	// 뷰 행렬 설정 : 단위 행렬로 설정이 필요. 
	g_cbDef.mView = XMMatrixIdentity();
	// 프로젝션 행렬 설정 : 'Ortho-Off Projection' 행렬 설정.
	g_cbDef.mProj =	XMMatrixOrthographicOffCenterLH(0, (float)g_Mode.Width, (float)g_Mode.Height, 0.0f, 1.0f, 100.0f);		
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBDef, &g_cbDef, sizeof(cbDEFAULT));


	//--------------------------
	// 렌더링 상태 설정
	//--------------------------
	//조명 끔 : 2D UI 에서는 조명 불필요. 
	//... 

	//텍스처 셈플러는 Clamp 모드로.
	g_pDXDC->PSSetSamplers(0, 1, &g_pSampler[SS_CLAMP]);

	//컬링 OFF. 뒷면도 그립니다.
	g_pDXDC->RSSetState(g_bWireFrame?g_RState[RS_WIREFRM]:g_RState[RS_SOLID]);
	//g_pDXDC->RSSetState(g_RState[RS_SOLID]);
	
	//--------------------------
	// 깊이/스텐실 버퍼 연산 설정
	//--------------------------
	//1.깊이/스텐실 버퍼 클리어.(옵션)
	//RTClear(...)
	//2.깊이 쓰기는 필요에 따라...
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_WRITE_OFF], 0);			//깊이버퍼 쓰기 off

}





/////////////////////////////////////////////////////////////////////////////// 
//
//  2.5D / 원근+UI 모드 전환. 
//
void Mode_25D()
{

	//--------------------------
	// 원본 "3D 카메라" 정보 백업.
	//--------------------------
	// 카메라 클래스가 있다면 보다 간편해지겠지요.
	// 현재 데모에서는 yn_View, yn_Proj 에 저장되어있습니다.
	//...


	//--------------------------
	// 2.5D 카메라 설정.
	//--------------------------
	// 뷰 행렬 설정 : 단위 행렬로 설정이 필요. 
	g_cbDef.mView = XMMatrixIdentity();
	// 프로젝션 행렬 설정 --> 3D 원근 카메라 사용.
	g_cbDef.mProj = yn_Proj;
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBDef, &g_cbDef, sizeof(cbDEFAULT));
	
	
	//--------------------------
	// 렌더링 상태 설정
	//--------------------------
	//조명 끔 : 2D UI 에서는 조명 불필요. 
	//... 

	//텍스처 셈플러는 Wrap 모드로 : Scroll 처리를 위해 필요. 
	g_pDXDC->PSSetSamplers(0, 1, &g_pSampler[SS_WRAP]);

	//컬링 OFF. 뒷면도 그립니다.
	g_pDXDC->RSSetState(g_bWireFrame?g_RState[RS_WIREFRM]:g_RState[RS_SOLID]);
	//g_pDXDC->RSSetState(g_RState[RS_SOLID]);

	//--------------------------
	// 깊이/스텐실 버퍼 연산 설정
	//--------------------------
	//1.깊이/스텐실 버퍼 클리어.(옵션)
	//RTClear(...)	
	//2.깊이 쓰기는 필요에 따라...
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF], 0);			//깊이버퍼 off
	//g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_WRITE_OFF], 0);	//깊이버퍼 쓰기 off

}


 
 

//////////////////////////////////////////////////////////////////////////////// 
//
// 렌더타겟, 깊이/스텐실 버퍼 클리어.
//
void RTClear(COLOR& col)
{ 
	//[주의] 렌터타겟과 뷰포트 설정.
	// DX9 과 다르게 DX10/11 에서의 ClearRenderTarget 메소드는
	// 렌더타겟뷰 전체를 지우며 Viewport 및 Scissor 설정은 적용되지 않습니다.
	// ...
	// 해결1 : DX11.1 / ID3D11DeviceContext1::ClearView 메소드 사용.
	// 해결2 : "Drawing Quad" , 직교투영/사각형 메쉬로 영역 지우기 
	
	//렌더타겟(백버퍼)/ 깊이버퍼 및 스텐실버퍼 지우기..
	ClearBackBuffer(D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	//초기화 플래그.
					col,			//배경색.
					1.0f,			//깊이 버퍼 초기값.
					0				//스텐실 버퍼 초기값.
				    );
	
}




/////////////////////////////////////////////////////////////////////////////// 
//
// 색상값 제한 : 0 ~ 1 사이로 값을 제한한다.
//
void Saturate(COLOR* col)
{	
	col->x = max(col->x, 0);
	col->y = max(col->y, 0);
	col->z = max(col->z, 0);
	//col->w = max(col->w, 0);

	col->x = min(col->x, 1);
	col->y = min(col->y, 1);
	col->z = min(col->z, 1);		 
	//col->w = min(col->w, 1);
}




/////////////////////////////////////////////////////////////////////////////// 
//
// 색상반전 : 연산자 오버로딩 방식도 추천.
//
COLOR InvertColor(COLOR& col)
{	
	COLOR newCol;
	newCol.x = 1 - col.x;
	newCol.y = 1 - col.y;
	newCol.z = 1 - col.z;
	newCol.w = col.w;

	Saturate(&newCol);
	
	return newCol;
}







//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
//
// 폰트 엔진 --> Yena 에 포함됨 
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
SpriteBatch* g_pFontBatch = nullptr;
SpriteFont*  g_pFont = nullptr;


///////////////////////////////////////////////////////////////////////////////
// 
int ynFontCreate(LPDEVICE pDev)
{
	
	//장치 목록 획득.
	LPDXDC pDXDC = nullptr;	
	pDev->GetImmediateContext(&pDXDC);


	//Sprite Batch 개체 생성.
	g_pFontBatch = new SpriteBatch(pDXDC);

	//DirectX Toolkit : Sprite Font 객체 생성.
	TCHAR* filename = L"../Yena/Font/굴림9k.sfont";	//ASCII 0 ~ 255 + 특수문자'■' + Unicode 한글 완성형 총 11,440 글자, 크기:9		
	try {
		g_pFont = new SpriteFont(pDev, filename );	
		g_pFont->SetLineSpacing(14.0f);				//폰트9 기준, 줄간격 설정. '다중라인 출력시 흐려짐 방지용'
		g_pFont->SetDefaultCharacter('_');			//출력 글자값 미검색시 대신 출력할 키값.
	}
	catch (std::exception& e)
	{
		//ynError(0, L"폰트 생성 실패 : File=%s", filename);
		TCHAR msg[1024] = L"";	
		::mbstowcs(msg, e.what(), strlen(e.what()));
		ynError(0, L"폰트 생성 실패 : %s \n File=%s", msg, filename);
	}

	//사용후, 장치목록 해제.
	SAFE_RELEASE(pDXDC);

	return YN_OK;
}



///////////////////////////////////////////////////////////////////////////////
//
void ynFontRelease()
{
	SAFE_DELETE(g_pFontBatch);
	SAFE_DELETE(g_pFont);
}




/////////////////////////////////////////////////////////////////////////////// 
//
// 문자열 출력 : DX11 (유니코드) 대응.
//
void ynTextDraw(int x, int y, COLOR col, TCHAR* msg, ...)
{
	const DWORD _size = 2048;

	TCHAR buff[_size] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);

	g_pFontBatch->Begin();
	//g_pFontBatch->Begin(SpriteSortMode_Deferred, nullptr, nullptr, ypStateObj[YNDS_DEPTH_ON_]);		//깊이연산 추가.
	g_pFont->DrawString(g_pFontBatch, buff, Vector2((float)x, (float)y), Vector4(col));					//원본 크기.
	//g_pFont->DrawString(ypFontBatch, buff, Vector2((float)x, (float)y), Vector4(col), 0, Vector4::Zero, 0.8f);	//80% 축소 출력.
	g_pFontBatch->End();

}
*/

 
/***************** End of "Device.cpp" *******************************/


 
