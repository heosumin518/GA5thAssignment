//
// Shadow.cpp : 기본 렌더링/프레임웍 소스
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
#pragma warning(disable:4996)
#include "Windows.h" 
#include "stdio.h"
#include "Device.h"
#include "Shader.h"		//셰이더 관련 헤더.
#include "Light.h"		//조명 관련 헤더.
//#include "XFile.h"	//XFile 운용 클래스 헤더.
//#include "YnModel.h"	//예나 모델 클래스 헤더.

//#include "Map.h"		//지형 헤더.
#include "Shadow.h"		//그림자 헤더.




/////////////////////////////////////////////////////////////////////////////
//
//  전역 데이터들.
//

extern BOOL g_bStencilOn;
extern VECTOR3 g_vTorchPosW;

//그림자 행렬
XMMATRIX g_mShadow;


 
  
/////////////////////////////////////////////////////////////////////////////// 
//
// 그림자 설정 : 그림자를 그릴 모든 개체에 공통 기능 설정.
//
void ShadowON()
{ 

	//-----------------------
	// 그림자 방향 결정.
	//-----------------------	
	// 그림자 행렬 계산에 필요한  빛 정보 확인.
	cbLIGHT cbLit = g_cbLit;	//공유 정보 복사.
	
	//< 조명 방향성 테스트1 > 
	//빛이 월드에 일정 방향으로 조사됨 : "태양광" (정오)(Directrional Light)
 	//VECTOR lt = VECTOR(g_cbLit.Direction, 0);		// W=0 (방향) "Directional Light" 	
 	VECTOR lt = cbLit.Direction;		// 조명 방향 복사.
	XMVectorSetW(lt, 0);				// W=0 (방향) "Directional Light" 	
	//lt.w = 0;							// W=0 (방향) "Directional Light" 	
	//lt = -lt;							// 방향을 바꾸어 주어야 합니다. 			
	//*/	
	
	/*//<조명 방향성 테스트2 > 
	//빛이 월드에 고정 되어 있다는 조건 : "가로등" (Spot/Point Light)
	VECTOR3 pos(0, 5.0f, 0);
	VECTOR lt = XMLoadFloat4(&VECTOR4(pos.x, pos.y, pos.z, 1));	// W=1 (좌표) "Point Light"
	//*/

	/*//< 조명 방향성 #3>
	//빛이 주인공 어깨 위에 있음 ("횟불")
	VECTOR3& pos = g_vTorchPosW + VECTOR3(0.5f, 2, 0);			// 그림자의 품질을 위해 실제 높이 보다 조금 높게 처리.
	VECTOR lt = XMLoadFloat4(&VECTOR4(pos.x, pos.y, pos.z, 1));	// W=1 (좌표) "Point Light" 	 
	//*/


	//상수버퍼 갱신.
	cbLit.Direction = lt;
	//cbLit.SpecOn = FALSE;			//정반사광 off.
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &cbLit, sizeof(cbLIGHT));
	

	//-----------------------
	// 그림자용 투영행렬 만들기.
	//-----------------------
	//XMVECTOR pos = XMLoadFloat3(&VECTOR3(0, 0.01f, 0));	//평면의 위치를 조금 올린다. (Z-Fighting 방지용)
	//XMVECTOR dir = XMLoadFloat3(&VECTOR3(0, 1, 0));
	//XMPLANE  pln = XMPlaneFromPointNormal( pos, dir);		//평면 생성.
	XMPLANE pln = XMLoadFloat4(&VECTOR4(0, 1, 0, 0));		//그림자가 그려질 평면.
	XMMATRIX mShadow = XMMatrixShadow( pln, lt);			//평면 그림자 변환행렬 생성.
	
	//그림자 행렬 저장.
	g_mShadow = mShadow;


	//-----------------------
	// 렌더링 옵션 설정.
	//-----------------------
	g_pDXDC->OMSetBlendState(g_BState[BS_AB_ON], NULL, 0xFFFFFFFF);
	//g_pDXDC->RSSetState(g_RState[RS_CULL_CCW]);


	//-----------------------
	// 깊이/스텐실 연산 설정.
	//-----------------------
	if(g_bStencilOn)  
			g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF_STENCIL_EQUAL_INCR], 0);		// "중복 그리기 방지"
	else	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF], 0);						//깊이 기록 OFF.
	

	
	//-----------------------
	// 준비 완료!
	//-----------------------
	// 그림자 그리기!
	//...
}




/////////////////////////////////////////////////////////////////////////////// 
//
// 그림자 그리기 : YnModel 버전. 셰이더는 그림자 전용.
//
/*
void ShadowDraw(LPMODEL pModel, float dTime/=0/)
{

	//-----------------------
	// 그림자용 재질 설정 
	//-----------------------
	//  반투명, 텍스처 없음.--> Model 클래스에서 처리함. 
	// ... 
	
	//-----------------------
	// 그림자용 셰이더 설정  
	//-----------------------
	// 특별한 조명이나 텍스처 처리 없이 단순 색상 및 투명도(Alpha) 만 유지합니다.
	// 이번 데모에서는 셰이더를 추가 제작하지 않고 Demo3.fx 를 사용하며
	// AB 을 위해 Mtrl.Diffuse.a 를 활용함에 주의해 주십시요.	
	//-----------------------	
	//셰이더 백업.
	LPVERTEXSHADER pVS = pModel->GetVS();
	LPPIXELSHADER  pPS = pModel->GetPS();
	//그림자 셰이더 설정.
	pModel->SetVS(g_pVS3);
	pModel->SetPS(g_pPS3); 
		  

	//-----------------------
	//'그림자' 그리기...
	//-----------------------
	XMMATRIX mTM = XMLoadFloat4x4(&pModel->GetTM());
	XMMATRIX mPSTM = mTM * g_mShadow;						// Planar Shadow TM  
	pModel->SetTM(mPSTM); 
	pModel->DrawShadow();		


	//모델 행렬 복구.
	pModel->Update();
	
	//세이더 복구.
	pModel->SetVS(pVS);
	pModel->SetPS(pPS);

}
*/


 


/////////////////////////////////////////////////////////////////////////////// 
//
// 그림자 끄기, 렌더링 옵션 복구. 
//
void ShadowOFF()
{
	 
	//세이더 복구.
	//...	 

	//깊이/스텐실 복구.
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON], 0);
	
	// 렌더링 옵션 복구 
	g_pDXDC->OMSetBlendState(g_BState[BS_AB_OFF], NULL, 0xFFFFFFFF);
	
	// 조명 복구.
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &g_cbLit, sizeof(cbLIGHT));
	//UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMtrl, &g_cbMtrl, sizeof(cbLIGHT));

}







////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// 깊이/스텐실 버퍼 연산 함수들. 
//
////////////////////////////////////////////////////////////////////////////// 
//
// 스텐실 테스트 활성화.
//
void StencilON()
{	
	// 스텐실 활성화 및 참조값(Stencil Reference Value) 설정(1) 
	// 이후의 "그리기" 는 스텐실 버퍼에 '1' 을 기록합니다. 
	//g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON_STENCIL_ON], 1);	 

	//Depth-OFF , Stencil-ON, 참조값 "0" 인 곳에만 그리고 +1증가 (이중그리기 방지)
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF_STENCIL_EQUAL_INCR], 0);
	 
}





/////////////////////////////////////////////////////////////////////////////// 
//
// 스텐실 끄기, 렌더링 옵션 복구. 
//
void StencilOFF()
{ 	  

	//깊이/스텐실 연산 복구 : Depth-ON, Stencil-OFF (기본값)
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON_STENCIL_OFF], 0);


	// 기타 렌더링 옵션 복구...
	// 
  	// AB OFF... 

	// Depth Buffer 클리어 (옵션)
	// 이후 렌더링될 오브제들의 깊이간섭(Z-Fighting) 을 제거하기위해 깊이버퍼를 초기화합니다.
	// 필요하다면, 스텐실도 함께 초기화합니다.
	//g_pDXDC->ClearDepthStencilView(g_pDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);	//깊이값만 초기화. 
	//g_pDXDC->ClearDepthStencilView(g_pDSView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);	//깊이값/스텐실값 초기화. 

	//렌더타겟 지우기 (옵션)
	//g_pDXDC->ClearShadowTargetView(g_pRTView, ...);						
	
}



////////////////////////////////////////////////////////////////////////////// 
//
// 깊이 테스트 OFF.
//
void DepthOFF()
{	
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF], 0);	 
}





/////////////////////////////////////////////////////////////////////////////// 
//
// 깊이 테스트 ON (기본값) 
//
void DepthON()
{ 
	//깊이/스텐실 연산 복구 : Depth-ON, Stencil-OFF (기본값)
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON], 0); 	
}






/****************** end of file "Shadow.cpp" *********************************/