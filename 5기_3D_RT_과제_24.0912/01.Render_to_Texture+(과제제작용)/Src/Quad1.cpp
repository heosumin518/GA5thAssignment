//
// Quad.cpp : 라이트멥핑용 공용 사각 메쉬
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

#include "assert.h"
#include "Device.h"
#include "Shader.h"		//셰이더 관련 헤더.
//#include "Light.h"	//조명 관련 헤더.
#include "Render.h"

#include "Quad1.h"


/////////////////////////////////////////////////////////////////////////////
//
//  전역 데이터들.
//
struct QUAD_VTX
{
	float x, y, z;
	float u0, v0;		//0번 uv 세트.
	//float u1, v1;		//1번 uv.셋트.
};


// 쿼드 모델용 기하버퍼.
ID3D11Buffer*		g_pVBQuad = nullptr;			//정점 버퍼 포인터.
ID3D11InputLayout*  g_pVBQuadLayout = nullptr;		//정점 입력구조 

// 쿼드 모델 텍스처
ID3D11ShaderResourceView*           g_pTexrvQuadDiff = nullptr;		//DiffuseMap 외부지정, 단순참조용.
//TCHAR g_TexNameQuad[256]=L"N/A";					 
//ID3D11ShaderResourceView*           g_pTexrvQuadLit = nullptr;	//LightMap 외부지정, 단순참조용.
//TCHAR g_TexNameQuad[256]=L"N/A";					 

// 쿼드용 상수버퍼 : 각 렌더링 개체(유닛)별 CB 운용이 유리.
cbDEFAULT		g_cbQuad;
ID3D11Buffer*	g_pCBQuad = nullptr;

// 모델용 기하입력구조
//ID3D11InputLayout*  g_pVBLayout;
int QuadCreateVBLayout();



 
////////////////////////////////////////////////////////////////////////////
//
int QuadCreate() 
{ 	 

	HRESULT hr = S_OK;

	QUAD_VTX Quad[] = {
	
		// 정점 데이터 순서... 
		// [그림] 각 면의 정점 구성.
		//  v0 --- v1
		//   |    / | 
		//   |   /  |
		//   |  /   |
		//   | /    |
		//  v2 --- v3
	
		{-0.5f,  0.5f, 0,   0.0f,  0.0f, }, 
		{ 0.5f,  0.5f, 0,   1.0f,  0.0f, }, 
		{-0.5f, -0.5f, 0,   0.0f,  1.0f, },   
		{ 0.5f, -0.5f, 0,   1.0f,  1.0f, },  
	
	};


	// 정점 버퍼 Vertex Buffer 생성
	DWORD size = sizeof(Quad);
	CreateVB(g_pDevice, Quad, sizeof(Quad), &g_pVBQuad);	  

	// 정점 입력구조 Input layout 생성.
	QuadCreateVBLayout();
		

	// 텍스처 로드 Load the Texture 
	//TCHAR* filename = L"../data/ground.jpg";
	//LoadTexture(filename, &g_pTexrvQuadDiff);
	//TCHAR* filename2 = L"../data/fx/Flare0.dds";
	//LoadTexture(filename2, &g_pTexrvQuadLit);
	

	// 상수 버퍼 생성.
	ZeroMemory(&g_cbQuad, sizeof(cbDEFAULT)); 
	CreateDynamicConstantBuffer(sizeof(cbDEFAULT), &g_cbQuad, &g_pCBQuad); 
	

	return TRUE;
}





////////////////////////////////////////////////////////////////////////////
//
void QuadRelease() 
{ 	 
	//...
	SAFE_RELEASE(g_pVBQuad);			//모델 제거.
	SAFE_RELEASE(g_pVBQuadLayout);
	//SAFE_RELEASE(g_pTexrvQuadDiff);	//텍스처 제거.
	//SAFE_RELEASE(g_pTexrvQuadLit);	//텍스처 제거.
	SAFE_RELEASE(g_pCBQuad);			//상수버퍼 제거.
}






////////////////////////////////////////////////////////////////////////////// 
//
// 정점 입력구조 생성 : 셰이더별 개별운용 필요. 현재 데모에서는 공유 중.	
//
int QuadCreateVBLayout()
{
	HRESULT hr = S_OK;	

	// 정점 입력구조 Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                    offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
		//{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,     0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
	};
	UINT numElements = ARRAYSIZE(layout);

	// 정접 입력구조 객체 생성 Create the input layout
	hr = g_pDevice->CreateInputLayout(  layout,
										numElements,
										g_pVSCode4->GetBufferPointer(),
										g_pVSCode4->GetBufferSize(),
										&g_pVBQuadLayout
									);
	if (FAILED(hr))	return hr;  

	return hr;
}

 
 





////////////////////////////////////////////////////////////////////////////
//
void QuadUpdate(float dTime) 
{ 
	//모델 리셋 
	//...
	
	/*
	//-----------------------
	// 변환 계산.
	//-----------------------
	//XMMATRIX mScale = XMMatrixScaling(30, 1, 30);		//30x30 (m) 쿼드.
	//XMMATRIX mScale = XMMatrixScaling(80, 1, 80);		//80x80 (m) 쿼드.
	//XMMATRIX mScale = XMMatrixScaling(100, 1, 100);	//100x100 (m) 쿼드.
	XMMATRIX mScale = XMMatrixScaling(256, 1, 256);		//256x256 (m) 쿼드.
	//XMMATRIX mRot = XMMatrixIdentity();	 	
	XMMATRIX mTM = mScale;
	

	//-----------------------
	// 상수버퍼로 기록.
	//-----------------------
	g_cbQuad.mTM = mTM;
	g_cbQuad.mWV = mTM * yn_View; 
	g_cbQuad.mView = yn_View;		//전역 카메라 정보 설정. 
	g_cbQuad.mProj = yn_Proj;		//일반 박스와 카메라 정보는 동일합니다.


	//셰이더 상수 버퍼 갱신.(동적버퍼)
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBQuad, &g_cbQuad, sizeof(cbDEFAULT));
	//셰이더 상수 버퍼 갱신.(정적버퍼)
	//g_pDXDC->UpdateSubresource(g_pCBDef, 0, nullptr, &g_cbQuad, 0, 0);


	//-----------------------
	// 재질 정보 갱신.
	//-----------------------
	//...
	*/

}




////////////////////////////////////////////////////////////////////////////
//
// 사각형메쉬 그리기 
//
void QuadDraw(float dTime)
{ 
	  
	//-----------------------
	// 상수버퍼 갱신
	//-----------------------
	//..행렬은 외부에서.
	//..조명 없음. 재질Diffuse 로 색상결정.

	//-----------------------
	// 기하 및 셰이더 설정.
	//-----------------------
	UINT stride = sizeof(QUAD_VTX);
	UINT offset = 0;
	g_pDXDC->IASetVertexBuffers(0, 1, &g_pVBQuad, &stride, &offset);			//기하 버퍼 설정	
	g_pDXDC->IASetInputLayout(g_pVBQuadLayout);									//입력 레이아웃 설정. 
	g_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// 기하 위상 구조 설정

	//셰이더 설정. : 사각메쉬 전용, 조명없음.
	g_pDXDC->VSSetShader(g_pVS4, nullptr, 0);
	g_pDXDC->PSSetShader(g_pPS4, nullptr, 0);		
	//정점 셰이더 리소스 설정 (이하 공통)
	g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBQuad);			//변환행렬 설정. 
	//g_pDXDC->VSSetConstantBuffers(1, 1, &g_pCBLit);			//조명정보 설정. 
	g_pDXDC->VSSetConstantBuffers(2, 1, &g_pCBMtrl);			//재질정보 설정.
	//픽셀 셰이더 리소스 설정.
	//g_pDXDC->PSSetConstantBuffers(2, 1, &g_pCBMtrl);			//재질 정보 설정.(PS)
	g_pDXDC->PSSetShaderResources(0, 1, &g_pTexrvQuadDiff);		//텍스처 설정. (Diff) --> 외부에서 설정.
	//g_pDXDC->PSSetShaderResources(0, 1, &g_pTexrvQuadLit);	//텍스처 설정. (LitMap) ---> 외부에서 설정.
		
														
	//-----------------------
	//그리기!
	//-----------------------
	g_pDXDC->Draw(4, 0);		


	// 조명 복구 
	//...

	//장치 상태 리셋 
	//g_pDXDC->ClearState();
}




////////////////////////////////////////////////////////////////////////////
//
// 사각형메쉬 그리기 
//
// pmTM : 텍스처 변환행렬.
// pDiffMap : 확산멥.
// pLitMap  : 라이트멥.
// col      : 추가색상 (조명색)
//
void QuadDraw(XMMATRIX& mTM, LPTEXTURERV pDiffMap, COLOR col)
{ 
	  
	//------------------------ 
	// 변환행렬 설정.
	//------------------------ 	
 	//XMMATRIX mS, mR, mT, mTM;
	//mS = XMMatrixScaling(scale, 1, scale);
	//mR = XMMatrixRotationY(vRot.y);
	//mT = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	//mTM = mS * mR * mT;
	cbDEFAULT cbDef = g_cbDef;
	cbDef.mTM = mTM;
	cbDef.mWV = mTM * cbDef.mView;
	//cbDef.mTex = mTexTM;
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBQuad, &cbDef, sizeof(cbDEFAULT));
 

	//------------------------ 
	// 조명 OFF...
	//------------------------ 
	/*cbLIGHT cbLit = g_cbLit;
	cbLit.DiffOn = FALSE;
	cbLit.SpecOn = FALSE;
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &cbLit, sizeof(cbLIGHT));*/

	//------------------------ 
	// 재질 정보 설정.
	//------------------------ 
	extern BOOL g_bTexOn;
	cbMATERIAL cbMtrl = g_cbMtrl;
	cbMtrl.Diffuse = XMLoadFloat4(&col);
	//cbMtrl.Ambient = XMLoadFloat4(&COLOR(1,1,1,1));
	cbMtrl.bTexOn = g_bWireFrame ? FALSE : g_bTexOn;		//텍스처 적용 On/Off.
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMtrl, &cbMtrl, sizeof(cbMATERIAL));



	//------------------------ 
	// 셰이더 리소스 설정.
	//------------------------ 
	g_pTexrvQuadDiff = pDiffMap;
	//g_pTexrvQuadLit = pLitMap;

	
	//------------------------ 
	// 셈플러 설정.
	//------------------------ 
	//g_pDXDC->PSSetSamplers(0, 1, &g_pSampler[SS_CLAMP]); 
	//g_pDXDC->PSSetSamplers(1, 1, &g_pSampler[SS_WRAP]); 

	//------------------------ 
	// 기타 렌더링 연산 설정.
	//------------------------ 
	// 알파테스트 : 셰이더에서 처리.
	//AlphaTest = 0.01f;
	// 알파블렌딩 ON.
	//g_pDXDC->OMSetBlendState(g_BState[BS_AB_ON], NULL, 0xFFFFFFFF);
	//깊이테스트 OFF : Z-Fighting 방지.
	//g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF], 0);
	//뒷변 그리기. OFF.
	//g_pDXDC->RSSetState(g_RState[RM_SOLID]);
	

	//------------------------ 
	// 그리기.
	//------------------------ 
	QuadDraw();


	//------------------------ 
	// 옵션 복구 
	//------------------------ 
	//g_pDXDC->OMSetBlendState(g_BState[BS_AB_OFF], NULL, 0xFFFFFFFF);
	//g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON], 0);
	//UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &g_cbLit, sizeof(cbLIGHT));
	//RenderModeUpdate();
	
	//장치 상태 리셋 
	//g_pDXDC->ClearState();

}






////////////////////////////////////////////////////////////////////////////
//
void QuadDraw(MATRIX* pmTM, float dTime) 
{ 	   
/*	//행렬은 외부에서.
	if(pmTM) g_pDevice->SetTransform(D3DTS_WORLD, pmTM);	

	QuadDraw(dTime); 
*/
}


 

/*********************** end of file "Quad.h" *****************************/
