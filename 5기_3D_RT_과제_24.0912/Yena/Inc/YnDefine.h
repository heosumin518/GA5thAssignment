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
// ynDefine.h:  엔진 공통 키워드 데이터 재정의.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. B3Yena / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#pragma warning(disable:4996)


// DX11 Header 및 라이브러리 --> "Yena.h" 로 이동.
// ...
 

///////////////////////////////////////////////////////////////////////////////
//
// 타입 과 상수 재정의 : 버전별 호환성 향상 증대가 목적.
//
///////////////////////////////////////////////////////////////////////////////

// DX11 장치관련 재정의.											
//
#ifndef LPDEVICE

//디바이스 및 장치 (DX11) Windows 7 / Vista
typedef ID3D11Device*			LPDEVICE;
typedef ID3D11DeviceContext*	LPDXDC;
typedef IDXGISwapChain*			LPSWAPCHAIN;
typedef ID3D11RenderTargetView*	LPRTVIEW;
typedef ID3D11DepthStencilView*	LPDSVIEW;

/*//디바이스 및 장치 (DX11.1, DXGI1.2)  Windows 8 / 7
#include "D3D11_1.h"
typedef ID3D11Device1*			LPDEVICE1;
typedef ID3D11DeviceContext1*	LPDXDC1;
typedef IDXGISwapChain1*		LPSWAPCHAIN1;
*/
/*//디바이스 및 장치 (DX11.2, DXGI1.3), Windows 8.1
#include "D3D11_2.h"
typedef ID3D11Device2*			LPDEVICE2;
typedef ID3D11DeviceContext2*	LPDXDC2;
typedef IDXGISwapChain2*		LPSWAPCHAIN2;
*/
/*//디바이스 및 장치 (DX11.3, DXGI1.4), Windows 10
#include "D3D11_3.h"
typedef ID3D11Device3*			LPDEVICE3;
typedef ID3D11DeviceContext3*	LPDXDC3;
typedef IDXGISwapChain3*		LPSWAPCHAIN3;
*/
/*
//디바이스 및 장치 (DX12) Windows 10 device (cross-platform : PC, tablet, console or phone)
typedef ID3D12Device*			LPDEVICE;
typedef ID3D12DeviceContext*	LPDXDC;
typedef IDXGISwapChain3*		LPSWAPCHAIN;
*/

// 장치 설정 정보 구조체. (DX9/11 구형 호환성 유지용)
typedef DXGI_MODE_DESC	  DISPLAYMODE;	//DX11 대응
//typedef DXGI_MODE_DESC1 DISPLAYMODE;	//DX11.1 대응
//typedef D3DDISPLAYMODE DISPLAYMODE;   //DX9 용.


//리소스 및 버퍼. (DX11)
typedef ID3D11ShaderResourceView* LPSRVIEW;			//셰이더 리소스 뷰.
typedef LPSRVIEW				  LPTEXTURERV;		//텍스쳐 리소스 뷰.
typedef ID3D11Texture2D*		  LPTEXTURE;		//2D 텍스처 
//typedef ID3D11Texture2D1*		  LPTEXTURE1;		//2D 텍스처 (DX11.3, Windows 10 이상)

typedef ID3D11Buffer*			LPXBUFFER;			//DX 공통 메모리 버퍼. (통상적으로 Vertex/Index/Constant 데이터를 저장)
typedef LPXBUFFER				LPVERTEXBUFFER;		//정점 버퍼.
typedef LPXBUFFER				LPINDEXBUFFER;		//인덱스 버퍼
typedef LPXBUFFER				LPCONSTBUFFER;		//상수 버퍼.
//입력 레이아웃
typedef ID3D11InputLayout*		LPLAYOUT;
typedef ID3D11RasterizerState*	LPRASTERSTATE;
//뷰포트
typedef D3D11_VIEWPORT			VIEWPORT;

//셰이더 관련.(DX11)
typedef ID3D11VertexShader*   LPVERTEXSHADER;
typedef ID3D11PixelShader*    LPPIXELSHADER;
typedef ID3DBlob*			  LPXDATA;			//임의 데이터 저장용 버퍼.(Vertex, Adjacency, material, binary code, etc..)

#endif


//상태 객체 재정의.
typedef ID3D11DepthStencilState* LPDEPTHSTENCILSTATE;
typedef ID3D11RasterizerState*   LPRASTERSTATE;
typedef ID3D11BlendState*	     LPBLENDSTATE;
typedef ID3D11SamplerState*      LPSAMPLERSTATE;
//typedef LPDEPTHSTENCILSTATE    LPDSSTATE;
//typedef LPRASTERSTATE			 LPRSTATE;
//typedef LPBLENDSTATE           LPBSTATE;
//typedef LPSAMPLERSTATE         LPSSTATE;


/*
typedef LPD3DXMESH		LPMESH;

typedef D3DMATERIAL9	MTRL;
typedef D3DMATERIAL9*	LPMTRL;
typedef D3DXMATERIAL	XMTRL;
typedef D3DXMATERIAL*	LPXMTRL;

typedef LPD3DXBUFFER	LPBUFF;

#ifndef LPTEXTURE
typedef LPDIRECT3DTEXTURE9	LPTEXTURE;
#endif

typedef LPD3DXFONT		LPDXFONT;
*/

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
#ifndef MATRIX
typedef XMMATRIX		MATRIXA;	//행렬 : 16바이트 정렬, SIMD 버전. 전역/지역 변수용. "Register Type"
typedef XMFLOAT4X4		MATRIX;		//행렬 : 일반 버전. SIMD 미지원, Class 데이터 저장용. "Storage Type"
typedef XMVECTOR		VECTOR;		//4성분 벡터 : 16바이트 정렬, SIMD 버전. 전역/지역 변수용. "Register Type"
typedef XMFLOAT4		VECTOR4;	//4성분 벡터 : 일반 버전, SIMD 미지원, Class 데이터 저장용. "Storage Type"
typedef XMFLOAT3		VECTOR3;
typedef XMFLOAT2		VECTOR2;

//색상 타입: 2가지.
//typedef XMCOLOR		COLOR;		// r, g, b, a.  [정수형 0~255]
typedef XMFLOAT4		COLOR;		// r, g, b, a.  [실수형 0~1.0]
#endif



///////////////////////////////////////////////////////////////////////////////
//
// 기타 시스템 관련 함수 및 재정의.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(pBuff) if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; }
#define SAFE_DELETE(pBuff)	if((pBuff)){ delete (pBuff); (pBuff) = NULL; }
#define SAFE_DELARRY(pBuff) if((pBuff)){ delete [] (pBuff); (pBuff) = NULL; }
#endif

#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif 

#ifndef YES_
#define YES_ TRUE
#define NO_  FALSE
#endif

#ifndef YN_OK
#define YN_OK	   0					//에러 없음. 특별한 문제 없음.
#define YN_FAIL   -1					//에러 발생.
#define YN_FAILED(v)   ((v) != YN_OK)	//에러 체크 매크로.
#endif
 




 
/****************** end of file "B3Mesh.h" *******************************/