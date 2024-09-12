//
// RenderTarget.cpp : 렌더타겟 운용 소스
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
#pragma warning(disable:4996)
#include "Windows.h" 
#include "stdio.h"
#include "Device.h"
#include "Shader.h"
#include "Render.h"

#include "RenderTarget.h"



/////////////////////////////////////////////////////////////////////////////
//
//  전역 데이터들.
//  



////////////////////////////////////////////////////////////////////////////// 
//
// ■ D3D 와 자원들. Resources 
//
// 자원( 리소스, Resource ) 는 3D 장면의 해석 또는 렌더링을 위한 정보를 
// 저장하는 데이터 저장소이며 그래픽파이프라인의 접근(읽기/쓰기)이 가능한 
// HW 메모리 영역입니다. (보통 그래픽 어뎁터내의 비디오메모리 VRAM)
//
// 여러분은 2가지 타입 - 형식(Type) 과 무형식(Typeless) - 자원을 생성할 수 있습니다.
//
// * 형식자원 (Strong Typed Resource) 
//    : 자원생성시 형식(포멧 Format) 이 결정.
//    : 하드웨어에 의한 최적화된 접근속도 (GPU Read Only)
//    : 정점버퍼, 인덱스버퍼, 상수버퍼, 텍스처, 등..
// 
// * 무형식자원 (Weak Typing / Typeless Resource)
//    : 자원생성시 형식이 미결정(UnKnown :  DXGI_FORMAT_R8G8B8A8_TYPELESS 등..)
//    : 자원이 파이프라인에 연결시 형식을 결정 (리소스 뷰 Resource View 이용)
//    : 유연한 자원 관리,접근(R/W), 공유, 재활용
//    : 유연한 형식 변환가능 ( D3DX_FORMAT_R8G8B8A8_FLOAT, D3DX_FORMAT_R8G8B8A8_UINT 등.., 포멧은 유지하고 TYPE 만 변경가능)
//    : 렌더타겟(텍스처), 깊이/스텐실버퍼(텍스처), UAV(텍스처), 셰이더자원(텍스처) 등..
//
// 자원의 접근방식(CPU 또는 GPU 의 읽기/쓰기)를 결정할 수 있습니다.
// 최대 128개의 자원들을 파이프라인의 각 스테이지에 연결(Blnding), 사용이 가능합니다.
// 
// D3D 자원의 운용과정은 아래와 같습니다.
// 1. 생성 : ID3D11Device 의 생성메소드를 이용
// 2. 연결 : ID3D11DeviceContext 의 설정메소드를 이용.
// 3. 제거 : Release 호출.
//
// [참고] Introduce of a Resource in D3D11 : https://msdn.microsoft.com/en-us/library/windows/desktop/ff476900(v=vs.85).aspx
//
//
////////////////////////////////////////////////////////////////////////////// 
//
// ■ 리소스 뷰 Resource View
// 
// 자원들은 다목적 메모리형식(Generic Memory Format) 으로 저장될 수 있기에 
// 다수의 파이프라인 스테이지에서 공유될 수 있습니다.
// 파이프라인 스테이지는 리소스뷰 View 를 통해 연결된 자원데이터를 해석합니다.
//
// 리소스뷰는 개념적으로 자료 형변환(Casting) 과 유사하기에 여러 특정 장치목록 
// (Context : 렌더링 스테이지 기능목록) 에서 사용될 수 있습니다.
// 
// 리소스뷰는 무형식자원(Typeless Resource) 을 위해 사용되며 파이프라인 스테이지와
// 연결시 뷰를 통해 특정형식을 결정할 수 있습니다. 그러나 리소스뷰는 연결하려는 
// 해당 자원의 비트구성(Bits per Component)과 상이한 형식은 선택할 수 없습니다.
// 예를 들어 DXGI_R8G8B8A8_TYPELESS 형의 무형식자원은 리소스뷰를 통해 동일 비트구성의
// DXGI_R8G8B8A8_FLOAT 형은 가능하나 DXGI_R32_FLOAT 는 불가능합니다.
//
// 리소스뷰는 또한 여러 특별한 기능을 지원합니다.
//  : 셰이더에서 깊이/스텐실 버퍼(Surface) 의 읽기.
//  : 단일패스(Single Pass)로 동적큐브멥 생성.
//  : 대형모델/장면을 세분화후 동시에 그리기.
// 
// 리소스뷰의 용도별 인터페이스는 아래와 같습니다.
//  : ID3D11DepthStencilView - 깊이/스텐실 테스트용 자원 (텍스쳐)
//  : ID3D11RenderTargetView - 렌더타겟용 자원 (텍스쳐)
//  : ID3D11ShaderResourceView - 셰이더 스테이지 자원 (상수버퍼, 텍스처버퍼, 텍스처, 또는 셈플러)
//  : ID3D11UnorderedAccessView - 픽셀셰이더 또는 컴퓨터셰이더 용 무순위자원 (Unordered Resource)
//
// [참고1] Resource Views : https://msdn.microsoft.com/en-us/library/windows/desktop/ff476900(v=vs.85).aspx#Views
// [참고2] Unordered Resources : https://msdn.microsoft.com/ko-kr/library/windows/desktop/ff476335(v=vs.85)#Unordered_Access
//
//
////////////////////////////////////////////////////////////////////////////// 
//
// ■ 렌더타겟 운용 순서
//
// 1.렌더타겟 생성.
//    1-1. 렌더링용 텍스쳐 생성			   : CreateTexture ( D3D11_BIND_RENDER_TARGET )
//    1-2.생성된 텍스쳐의 렌더타겟뷰 생성	   : CreateRenderTargetView
//    1-3.생성된 텍스쳐의 셰이더리소스뷰 생성 : CreateShaderResourceView
// 2.새로운 렌더링 타겟으로 설정		       : OMSetRednerTarget
//    2-1.새 깊이/스텐실 버퍼 설정. (옵션)
//    2-2.새 뷰포트 멥핑 설정.(옵션)
// 3.장면 그리기.
//   ...
// 4.이전 렌더링 타겟으로 복구. (option)     : OMSetRednerTarget
//
// * AA 및 ViewPort 설정에 유의.
//
////////////////////////////////////////////////////////////////////////////// 
//
// 렌더타겟 구성용 데이터들.
//
LPTEXTURE		g_pTexScene = NULL;			//장면이 렌더링될 새 렌더타겟용 텍스쳐 (리소스)
LPTEXTURERV		g_pTexRvScene = NULL;		//장면이 렌더링될 새 렌더타겟용 텍스쳐-리소스뷰 (멥핑용)
LPRTVIEW        g_pRTScene = NULL;			//장면이 렌더링될 새 렌더타겟뷰 (렌더링용) 

//LPRTSVIEW		g_pSurfOrgBB = NULL;		//원본 색상버퍼(백업용)


int  RenderTargetCreate();
void RenderTargetSceneGenerate();
void RenderTargetDraw();
void RenderTargetRelease();

HRESULT RTTexCreate(UINT width, UINT height, DXGI_FORMAT fmt, LPTEXTURE* ppTex);
HRESULT RTViewCreate(DXGI_FORMAT fmt, LPTEXTURE pTex, LPRTVIEW* ppRTView);
HRESULT RTSRViewCreate(DXGI_FORMAT fmt, LPTEXTURE pTex, LPTEXTURERV* ppTexRV);




//---------------------------------
// 렌더 타겟 시현용 쿼드 메쉬.
//---------------------------------
#include "Quad1.h"
//LPVERTEXBUFFER g_pVBQuad = NULL;
//HRESULT QuadCreate(LPDEVICE pDev);
//void    QuadRelease();

//장면 구성 
//BOOL	g_bRTargetOn = TRUE;
//BOOL	g_bRTargetUpdate = TRUE;
//BOOL	g_bShowScene = TRUE;



// 렌더타겟 해상도 ★
SIZE g_RTSize = {  512,	 512 };






/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
//
// Render Target 관련 함수들.
//
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
int RenderTargetCreate()
{

	//---------------------------- 
	// 렌더타겟 만들기 ★
	//---------------------------- 
	//렌더타겟 생성시 크기, 포멧, AA, Mipmap 등의 설정이 필요합니다.
	//이에 따라 성능, 품질 및 메모리 요구량이 달라집니다. 
	//성능과 품질에서의 적절한 타협이 필요합니다.
	//SIZE size = { 512, 512 };
	DXGI_FORMAT fmt = DXGI_FORMAT_R8G8B8A8_UNORM;

	//1. 렌더 타겟용 빈 텍스처로 만들기.★
	//...
	// 
	//2. 렌더타겟뷰 생성.★
	//...

	//3. 렌더타겟 셰이더 리소스뷰 생성 (멥핑용)★
	//...

	//4. 렌더타겟 전용 깊이/스텐실 버퍼 생성.(옵션)★
	//...


	//---------------------------- 
	// 렌더타겟 시현용 매쉬 생성. ("TV")
	//---------------------------- 
	QuadCreate();


	return YN_OK;
}




/////////////////////////////////////////////////////////////////////////////// 
//
void RenderTargetRelease()
{
	//렌더타겟 리소스 제거.★
 	SAFE_RELEASE( g_pTexScene );
 	SAFE_RELEASE( g_pTexRvScene );
 	SAFE_RELEASE( g_pRTScene );


	QuadRelease();
}




/////////////////////////////////////////////////////////////////////////////// 
//
// 렌더타겟용 빈 텍스처 만들기. 
//
HRESULT RTTexCreate(UINT width, UINT height, DXGI_FORMAT fmt, LPTEXTURE* ppTex)
{
	HRESULT hr = S_OK;

	//텍스처 정보 구성.★
	//...

	//텍스처 생성.★
	//...

	//성공후 외부로 리턴.
	//if (ppTex) *ppTex = pTex;

	return hr;
}




/////////////////////////////////////////////////////////////////////////////// 
//
// 렌더타겟 리소스뷰 만들기. 
//
HRESULT RTViewCreate(DXGI_FORMAT fmt, LPTEXTURE pTex, LPRTVIEW* ppRTView)
{
	HRESULT hr = S_OK;

	//렌더타겟 정보 구성.★
	//...

	//성공후 외부로 리턴.
	//if (ppRTView) *ppRTView = pRTView;

	return hr;
}




/////////////////////////////////////////////////////////////////////////////// 
//
// 렌더타겟 셰이더 리소스뷰 만들기. 
//
HRESULT RTSRViewCreate(DXGI_FORMAT fmt, LPTEXTURE pTex, LPTEXTURERV* ppTexRV)
{
	HRESULT hr = S_OK;

	//셰이더리소스뷰 정보 구성.★
	//...
	// 
	//성공후 외부로 리턴.
	//if (ppTexRV) *ppTexRV = pTexRV;

	return hr;
}




/////////////////////////////////////////////////////////////////////////////// 
//
// 렌더타겟으로 장면을 렌더링합니다.
//
void RenderTargetSceneGenerate()
{ 	 
	//기존 렌더타겟 (백버퍼) 백업 
	//LPRTVIEW  pRTOrg;		
	//LPDSVIEW  pDSOrg;
	//g_pDXDC->OMGetRenderTargets(1, &pRTOrg, &pDSOrg);	  

	//-------------------------------
	// 렌더타겟 장면 구성.
	//-------------------------------
	//새 렌더타겟 설정.★
	g_pDXDC->OMSetRenderTargets(1, &g_pRTScene, g_pDSView);		//깊이버퍼는 기존것을 공유.★
	


 	//새 렌더타겟(백버퍼) 및  깊이스텐실버퍼 지우기..
	//새 렌더타겟은 명확한 식별을 위해 분홍색으로 처리합니다.
	g_pDXDC->ClearRenderTargetView(g_pRTScene, (float*)&COLOR(1, 0, 1, 1));						//RT 분홍색으로 지우기.(식별용)★				
	g_pDXDC->ClearDepthStencilView(g_pDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);	//깊이버퍼는 기존것을 공유.★
 
	
	//장면 렌더링...
	_SceneRender();
	

	//-------------------------------
	// 원본 시스템 복구.
	//-------------------------------
	//백버퍼, 깊이/스텐실버퍼 복구.
	g_pDXDC->OMSetRenderTargets(1, &g_pRTView, g_pDSView);
	//g_pDXDC->OMSetRenderTargets(1, &pRTOrg, pDSOrg);


	//획득한 인터페이스는 사용후 Release 해야 합니다.
	//SAFE_RELEASE(pRTOrg);
	//SAFE_RELEASE(pDSOrg);
}




/////////////////////////////////////////////////////////////////////////////// 
//
// 렌더타겟 출력 : 생성된 렌더타겟의 이미지를 사각메쉬에 멥핑하여
//                원본 장면(BackBuffer)에 렌더링합니다.
//
void RenderTargetDraw()
{
	//if(!g_bRTargetOn) return;
	

	//0 옵션 설정 : 조명off, 컬링off.
	//...	

	//1.메쉬 설정.(쿼드)
	//..

	//2.렌더타겟을 텍스쳐로 설정 : QuadDraw 함수를 참조.
	//pDev->SetTexture(0, g_pTexScene);		
	 

	//3.그리기 : 메쉬를 월드에..("TV")
	XMMATRIX mS, mT, mR, mTM;
	mS = XMMatrixScaling( 5.0f, 5.0f, 1.0f);		//512 x 512 대응 비율 (1:1)
 	mR = XMMatrixIdentity();
	mT = XMMatrixTranslation(-2.0f, 2.5f, 2.0f);		 
	mTM = mS * mR * mT;

	QuadDraw(mTM, g_pTexRvScene );
	
	
	//옵션 복구.
 	//조명ON, 컬링ON. 
	//...

}




/****************** end of file "RenderTarget.cpp" *********************************/