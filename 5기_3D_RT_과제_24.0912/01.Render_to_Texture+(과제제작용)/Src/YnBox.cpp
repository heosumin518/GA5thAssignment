//
// ynYena :: Direct3D Basic Frame-works 
// 2003-2011. Kihong Kim  / mad_dog@hanmail.net
// Zero-G Interactive / http://www.zero-g.kr 
// DirectX 기준 버전 : DX90c.Nov.2007
//
// ynBox.cpp :  
//
// 2003.11.4. 
// 2009.12.01. Update. (DX90c.Nov.2007)
//
#pragma warning(disable:4996)
#include "Windows.h" 
#include "stdio.h"
#include "Device.h"
#include "Shader.h"		//셰이더 관련 헤더.
#include "Light.h"		//조명 관련 헤더.
#include "Yena.h"

#include "YnBox.h"	 






/////////////////////////////////////////////////////////////////////////////
//
 


// 큐브 + 노멀 ( 정점색 없음)
//
// [그림] 각 면의 정점 구성.
//  v0 --- v1
//   |    / | 
//   |   /  |
//   |  /   |
//   | /    |
//  v2 --- v3
//


//------------------------------------------------------------------//
// 정점 데이터 : 최적화 되어 있지 않은 Triangle List 用 정점 데이터.    //
//             삼각형 12개  정점 36 개								//
//          :  인덱스 버퍼 필요 없음.①								//
//------------------------------------------------------------------//
/*
//g_strIBMode = "Not Used";
ynBox::_VTX	boxVtx[] = {
	// 정면.  ( Face#0) :    
	// 좌표 ,             노멀,       UV (TextureCoordinate) 
	{ -1, 1,-1,  0, 0,-1,  0, 0 },			
	{  1, 1,-1,  0, 0,-1,  1, 0 },			
	{ -1,-1,-1,  0, 0,-1,  0, 1 },		
	//       ( Face#1)
	{ -1,-1,-1,  0, 0,-1,  0, 1 },			
	{  1, 1,-1,  0, 0,-1,  1, 0 },		
	{  1,-1,-1,  0, 0,-1,  1, 1 },		

	//뒷면.  (Face#3) :  
	{  1, 1, 1,  0, 0, 1,  0, 0 },		
	{ -1, 1, 1,  0, 0, 1,  1, 0 },		
	{ -1,-1, 1,  0, 0, 1,  1, 1 },		
	//		  (Face#4)
	{  1, 1, 1,  0, 0, 1,  0, 0 },		
	{ -1,-1, 1,  0, 0, 1,  1, 1 },		
	{  1,-1, 1,  0, 0, 1,  0, 1 },		


	// 우측면. (Face#5)
	{  1, 1,-1,  1, 0, 0,  0, 0 },		
	{  1, 1, 1,  1, 0, 0,  1, 0 },		
	{  1,-1,-1,  1, 0, 0,  0, 1 },		
	//			(Face#6)
	{  1,-1,-1,  1, 0, 0,  0, 1 },		
	{  1, 1, 1,  1, 0, 0,  1, 0 },		
	{  1,-1, 1,  1, 0, 0,  1, 1 },		


	// 좌측면. (Face#7)
	{ -1, 1, 1,  -1, 0, 0,  0, 0 },		
	{ -1, 1,-1,  -1, 0, 0,  1, 0 },		
	{ -1,-1,-1,  -1, 0, 0,  1, 1 },		
	//			(Face#8)
	{ -1, 1, 1,  -1, 0, 0,  0, 0 },		
	{ -1,-1,-1,  -1, 0, 0,  1, 1 },		
	{ -1,-1, 1,  -1, 0, 0,  0, 1 },		


	//상부.  ( Face#9)
	{ -1, 1, 1,  0, 1, 0,  0, 0 },		
	{  1, 1, 1,  0, 1, 0,  1, 0 },		
	{ -1, 1,-1,  0, 1, 0,  0, 1 },		
	//		 ( Face#10)
	{ -1, 1,-1,  0, 1, 0,  0, 1 },		
	{  1, 1, 1,  0, 1, 0,  1, 0 },		
	{  1, 1,-1,  0, 1, 0,  1, 1 },		


	//하부.  ( Face#11)
	{  1,-1, 1,  0,-1, 0,  0, 0 },		
	{ -1,-1, 1,  0,-1, 0,  1, 0 },		
	{ -1,-1,-1,  0,-1, 0,  1, 1 },		
	//		 ( Face#12)
	{  1,-1, 1,  0,-1, 0,  0, 0 },		
	{ -1,-1,-1,  0,-1, 0,  1, 1 },		
	{  1,-1,-1,  0,-1, 0,  0, 1 },		 
};
//*/



//------------------------------------------------------------------//
// 정점 데이터 : 인덱스 버퍼 / Triangle-List 用 정점 데이터.			//
//             삼각형 12개  정점 24 개								//
//------------------------------------------------------------------//
ynBox::_VTX boxVtx[] =					
{
	// 정면에서 시계 반대 방향으로 각각 0~3번 면.
	//0번 면 : 정면.
	{ -1, 1, -1,  0, 0, -1,  0, 0 },	//v0	
	{  1, 1, -1,  0, 0, -1,  1, 0 },	//v1	
	{ -1,-1, -1,  0, 0, -1,  0, 1 },	//v2
	{  1,-1, -1,  0, 0, -1,  1, 1 },	//v3	

	//1번 면 : 우측면
	{  1, 1, -1,  1, 0, 0,  0, 0 },		//v4	
	{  1, 1,  1,  1, 0, 0,  1, 0 },		//v5	
	{  1,-1, -1,  1, 0, 0,  0, 1 },		//v6	
	{  1,-1,  1,  1, 0, 0,  1, 1 },		//v7	

	//2번 면 : 뒷면
	{  1, 1, 1,  0, 0, 1,  0, 0 },		//v8
	{ -1, 1, 1,  0, 0, 1,  1, 0 },		//v9
	{  1,-1, 1,  0, 0, 1,  0, 1 },		//v10
	{ -1,-1, 1,  0, 0, 1,  1, 1 },		//v11

	//3번 면 : 좌측면
	{ -1, 1,  1,  -1, 0, 0,  0, 0 },	//v12
	{ -1, 1, -1,  -1, 0, 0,  1, 0 },	//v13
	{ -1,-1,  1,  -1, 0, 0,  0, 1 },	//v14
	{ -1,-1, -1,  -1, 0, 0,  1, 1 },	//v15


	//4번 면 : 윗면.
	{ -1, 1,  1,  0, 1, 0,  0, 0 },		//v16
	{  1, 1,  1,  0, 1, 0,  1, 0 },		//v17
	{ -1, 1, -1,  0, 1, 0,  0, 1 },		//v18
	{  1, 1, -1,  0, 1, 0,  1, 1 },		//v19

	//5번 면 : 아랫면.
	{ -1, -1, -1,  0, -1, 0,  0, 0 },	//v20
	{  1, -1, -1,  0, -1, 0,  1, 0 },	//v21
	{ -1, -1,  1,  0, -1, 0,  0, 1 },	//v22
	{  1, -1,  1,  0, -1, 0,  1, 1 },	//v23


};


//------------------------------------------------------------------//
// 정점 24개의 인덱스 버퍼.	②		   							    //		
//------------------------------------------------------------------//

ynBox::_INDEX	boxIdx[] = {
	{  0,  1,  2 }, {  2,  1,  3 },
	{  4,  5,  6 }, {  6,  5,  7 },
	{  8,  9, 10 },	{ 10,  9, 11 },
	{ 12, 13, 14 }, { 14, 13, 15 },
	{ 16, 17, 18 }, { 18, 17, 19 },
	{ 20, 21, 22 }, { 22, 21, 23 }
};
 


/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
ynBox::ynBox()
{
	_InitDatas();

}

/////////////////////////////////////////////////////////////////////////////
//
ynBox::~ynBox()
{
	Release();
}




////////////////////////////////////////////////////////////////////////////// 
//
// 박스 : 제거 
//
void ynBox::Release()
{
	SAFE_RELEASE(m_pVB); 
	SAFE_RELEASE(m_pIB); 
 	SAFE_RELEASE(m_pTextureRV); 
}





/////////////////////////////////////////////////////////////////////////////
//
void ynBox::_InitDatas()
{
	m_pDev = nullptr;
	m_pDXDC = nullptr;
	m_pVB = nullptr;
	m_pIB = nullptr;

	m_pTextureRV = nullptr;
	_tcscpy(m_TexName, L"N/A");
	m_bTexOn = FALSE;

	XMStoreFloat4x4(&m_mTM, XMMatrixIdentity());
	m_vPos = VECTOR3(0,0,0);
	m_vRot = VECTOR3(0,0,0);
	m_vScl = VECTOR3(1,1,1);

	//m_Color = COLOR(1, 1, 1, 1);
}




/////////////////////////////////////////////////////////////////////////////
//
// 모델 리셋 
//
void ynBox::Reset()
{ 	 
	XMMATRIX m = XMMatrixIdentity();
	XMStoreFloat4x4(&m_mTM, m);
	//m_mTrans = m_mRot = m_mScl = m_mTM;
	m_vPos = VECTOR3(0, 0, 0);
	m_vRot = VECTOR3(0, 0, 0);
	m_vScl = VECTOR3(1, 1, 1);

	//m_Color = COLOR(1, 1, 1, 1);
}






/////////////////////////////////////////////////////////////////////////////
//
//  박스 : 생성 및 필요 리소스 로딩.
//
int ynBox::Create(LPDEVICE pDev, LPDXDC pDXDC, TCHAR* texName)
{  

	m_pDev = pDev;
	m_pDXDC = pDXDC;

	//정점 구성.
	_CreateVB();
	_CreateIB();
 
	//텍스처 로드 
	if (texName)
	{
		ynLoadTexture(pDev, pDXDC, texName, &m_pTextureRV);		
		if (m_pTextureRV)
		{
			//텍스처 정보 획득. 
			//m_pTextureRv->GetLevelDesc(0, &m_TexSurfDesc);				//텍스처 - 서피스 정보 획득.
			//GetImageInfoFromFile(pBox->m_TexName, &pBox->m_TexImgInfo);	//텍스처 - 원본 이미지 정보 획득.
			_tcscpy(m_TexName, texName);
			m_bTexOn = TRUE;
		}
	}
	
  
	//재질 설정.
	ZeroMemory(&m_Mtrl, sizeof(MATERIAL));
	m_Mtrl.Diffuse = m_Mtrl.Ambient = COLOR(1,1,1,1);
	m_Mtrl.Specular = COLOR(1, 1, 1, 1);
	m_Mtrl.Power = 30;


	//----------------------------------------------
	// 외부 참조(공유) 데이터들 : 추후 클래스화 필요.
	//----------------------------------------------
	// 이번 데모에서는 외부 (공유)객체(데이터)를 참조중으로 상정,
	// 추가 인터페이스 설계없이 단순하게 처리하겠습니다. 
	extern cbDEFAULT  g_cbDef;
	extern cbMATERIAL g_cbMtrl;
	extern cbLIGHT    g_cbLit;
	extern ID3D11InputLayout*  g_pVBLayout;
	//extern LPXBUFFER  g_pCBDef;
	//extern LPXBUFFER  g_pCBMtrl;
	//extern LPXBUFFER  g_pCBLit;
	m_pVS = g_pVS;
	m_pPS = g_pPS;
	m_pLayout = g_pVBLayout;
	m_pcbDef = &g_cbDef;
	m_pcbMtrl = &g_cbMtrl;
	m_pcbLit = &g_cbLit;
	m_pCBDef = g_pCBDef;
	m_pCBMtrl = g_pCBMtrl;
	m_pCBLit = g_pCBLit;


	 
	return TRUE;
}
 





//////////////////////////////////////////////////////////////////////////////// 
//   
int ynBox::_CreateVB()
{  
	//버퍼 정보 확인.  
	DWORD vbSize = sizeof(boxVtx);				//버퍼 크기.
	m_VtxCnt = vbSize / sizeof(_VTX);			//정점 개수.
	
	//정점 버퍼 생성
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				//버퍼 사용방식
	bd.ByteWidth = vbSize;						//버퍼 크기
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//버퍼 용도 : "정점 버퍼" 용로 설정 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA rd;				
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = boxVtx;						//버퍼에 들어갈 데이터 설정 : "정점들"..
	
	//정점 버퍼 생성.
	HRESULT hr = g_pDevice->CreateBuffer(&bd, &rd, &m_pVB);
	if (FAILED(hr))	return hr;

	//노멀라인 생성.. 
	//g_NLine.Create(m_pDev, boxVtx, sizeof(boxVtx), 
	//			   sizeof(CUBE_VTX), COLOR(0, 1, 0, 1)); 
 
	return YN_OK;
} 





////////////////////////////////////////////////////////////////////////////////
//
int ynBox::_CreateIB()
{
	//버퍼 정보 확인.  
	DWORD ibSize = sizeof(boxIdx);				//버퍼 크기.
	m_FaceCnt = ibSize / sizeof(_INDEX);		//색인 개수.
	m_IndexCnt = m_FaceCnt * 3;					//(IB 를 사용하여 그릴) 총 정점개수.
		
	// 인덱스 버퍼 Index Buffer 생성 
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				//버퍼 사용방식
	bd.ByteWidth = sizeof(boxIdx);				//버퍼 크기
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//버퍼 용도 : "색인 버퍼" 용로 설정 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA rd;				
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = boxIdx;						//버퍼에 들어갈 데이터 설정 : "색인 정보"..
	
	//색인 버퍼 생성.
	HRESULT hr = g_pDevice->CreateBuffer(&bd, &rd, &m_pIB);
	if (FAILED(hr))	return hr;

	return YN_OK;
} 






/////////////////////////////////////////////////////////////////////////////
//
int ynBox::Update(float dTime /*=0*/)
{

	/*//모델 정보 리셋.
	if (IsKeyDown(VK_F9))		
	{
		Reset();
	}*/

	
	//-----------------------
	// 변환 행렬 계산.
	//-----------------------
	XMMATRIX mScale = XMMatrixScaling(m_vScl.z, m_vScl.y, m_vScl.z);
	XMMATRIX mTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMMATRIX mRot = XMMatrixRotationRollPitchYaw(m_vRot.x, m_vRot.y, m_vRot.z);		
	XMMATRIX mTM = mScale * mRot * mTrans;

	//정보 저장 (디버깅용)
	XMStoreFloat4x4(&m_mTM, mTM);


	//-----------------------
	// 상수버퍼 갱신 --> 렌더링시 처리.
	//-----------------------
	// 이번 데모에서는 외부 (공유)객체(데이터)를 참조중으로 상정,
	// 추가 인터페이스 설계없이 단순하게 처리하겠습니다. 	
	/*m_cbDef = g_cbDef;
	m_pcbDef->mTM = mTM;  
	m_pcbDef->mWV = mTM * m_pcbDef->mView;	

	//셰이더 상수 버퍼 갱신.(동적버퍼) --> 렌더링시 처리.
	UpdateDynamicConstantBuffer(m_pDXDC, g_pCBDef, &m_cbDef, sizeof(cbDEFAULT));
	//셰이더 상수 버퍼 갱신.(정적버퍼)
	//m_pDXDC->UpdateSubresource(m_pCBDef, 0, nullptr, &m_cbDef, 0, 0);
	*/

	//-----------------------
	// 재질 정보 갱신 : 실시간 변화가 없다면 생략 가능. --> 렌더링시 처리. 
	//-----------------------
	/*m_Mtrl.Diffuse = COLOR(1, 1, 1, 1);
	m_Mtrl.Ambient = COLOR(1, 1, 1, 1);
	m_Mtrl.Power = 30.0f;					

	//재질 정보 상수버퍼 갱신.
	m_cbMtrl.Diffuse = XMLoadFloat4(&m_Mtrl.Diffuse); 
	m_cbMtrl.Ambient = XMLoadFloat4(&m_Mtrl.Ambient);
	m_cbMtrl.Power = m_Mtrl.Power;						
	//상수버퍼 갱신 : --> 렌더링시 처리. 
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBMtrl, &m_cbMtrl, sizeof(cbMATERIAL));
	*/
 

	return YN_OK;
}




/////////////////////////////////////////////////////////////////////////////
//
void ynBox::SetTM(XMMATRIX* pTM)
{ 	
	if (pTM == nullptr) return;
	
	XMStoreFloat4x4(&m_mTM, *pTM);

}





/////////////////////////////////////////////////////////////////////////////
//
int ynBox::Draw(XMMATRIX* pTM/*=NULL*/)
{  	

	if (pTM)			//외부 지정 행렬 적용.
	{
		SetTM(pTM);
	}


	//--------------------------------------
	// 외부 지정 렌더링 옵션 및 상수 버퍼 갱신.
	//--------------------------------------
	// 장면내의 유닛/모델별 렌더링 옵션이 상이할 수 있기에 개별 처리가 필요합니다.
	// 모델 각 객체들은 외부의 상수 버퍼를 공유중이므로 그리기 전에 갱신해야 합니다. 
	// 만일 유닛별 상수버퍼가 준비되어 있다면 이런 동작은 불필요 합니다. 
	m_pcbDef->mTM = XMLoadFloat4x4(&m_mTM);
	m_pcbDef->mWV = m_pcbDef->mTM * m_pcbDef->mView;	
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBDef, m_pcbDef, sizeof(cbDEFAULT));
	 
	//재질 정보 상수버퍼 갱신.
	m_pcbMtrl->Diffuse = XMLoadFloat4(&m_Mtrl.Diffuse); 
	m_pcbMtrl->Ambient = XMLoadFloat4(&m_Mtrl.Ambient);
	m_pcbMtrl->Specular = XMLoadFloat4(&m_Mtrl.Specular);
	m_pcbMtrl->Power   = m_Mtrl.Power;				
	m_pcbMtrl->bTexOn = m_bTexOn ? TRUE : FALSE;			//텍스처 적용 On/Off.
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBMtrl, m_pcbMtrl, sizeof(cbMATERIAL));
	
	


  	//기하 버퍼 설정
	UINT stride = sizeof(_VTX);
	UINT offset = 0;
	g_pDXDC->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);		//정점버퍼 설정.
	m_pDXDC->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, offset);		//색인버퍼 설정.

	//입력 레이아웃 설정. Set the input layout 
	m_pDXDC->IASetInputLayout(m_pLayout);

	// 기하 위상 구조 설정 Set primitive topology
	m_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//셰이더 설정. : 표준 조명 셰이더 사용. 
	m_pDXDC->VSSetShader(m_pVS, nullptr, 0);
	m_pDXDC->PSSetShader(m_pPS, nullptr, 0);

	//상수버퍼 설정 
	m_pDXDC->VSSetConstantBuffers(0, 1, &m_pCBDef);		//상수 버퍼 설정.(VS)
	m_pDXDC->VSSetConstantBuffers(1, 1, &m_pCBLit);		//조명 정보 설정.(VS)
	m_pDXDC->VSSetConstantBuffers(2, 1, &m_pCBMtrl);	//재질 정보 설정.(VS)
	m_pDXDC->PSSetConstantBuffers(2, 1, &m_pCBMtrl);	//재질 정보 설정.(PS)
	//셰이더 리소스 설정.
	m_pDXDC->PSSetShaderResources(0, 1, &m_pTextureRV);	//PS 에 텍스처 설정.

	
	
	//그리기! Render a triangle 
	m_pDXDC->DrawIndexed(m_IndexCnt, 0, 0);
	//m_pDXDC->Draw(m_VtxCnt, 0);


	//노멀라인 그리기.
	//if(g_bNLine) g_NLine.Draw(&g_mTM);

	//장치 상태 리셋 
	//m_pDXDC->ClearState();

	
	return YN_OK;
}


 
/////////////////////////////////////////////////////////////////////////////
//
// 그림자 출력용 : 재질, 텍스처 등은외부에서 결정.
//
int ynBox::DrawShadow(XMMATRIX* pTM/*=NULL*/)
{  	
	if (pTM)			//외부 지정 행렬 적용.
	{
		SetTM(pTM);
	}


	//--------------------------------------
	// 상수 버퍼 갱신 : 그림자 행렬 적용.
	//--------------------------------------
	m_pcbDef->mTM = XMLoadFloat4x4(&m_mTM);
	m_pcbDef->mWV = m_pcbDef->mTM * m_pcbDef->mView;	
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBDef, m_pcbDef, sizeof(cbDEFAULT));
	 
	//재질 정보 상수버퍼 갱신 : 50% 투명.
	m_pcbMtrl->Diffuse = XMLoadFloat4(&COLOR(0, 0, 0, 0.5f)); 
	//m_pcbMtrl->Ambient = XMLoadFloat4(&m_Mtrl.Ambient);
	//m_pcbMtrl->Specular = XMLoadFloat4(&m_Mtrl.Specular);
	//m_pcbMtrl->Power   = m_Mtrl.Power;				
	m_pcbMtrl->bTexOn = m_bTexOn ? TRUE : FALSE;			//텍스처 적용 On/Off.
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBMtrl, m_pcbMtrl, sizeof(cbMATERIAL));
	
	


  	//기하 버퍼 설정
	UINT stride = sizeof(_VTX);
	UINT offset = 0;
	g_pDXDC->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);		//정점버퍼 설정.
	m_pDXDC->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, offset);		//색인버퍼 설정.

	//입력 레이아웃 설정. Set the input layout 
	m_pDXDC->IASetInputLayout(m_pLayout);

	// 기하 위상 구조 설정 Set primitive topology
	m_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//셰이더 설정. : 그림자 셰이더 적용. (외부지정)
	m_pDXDC->VSSetShader(m_pVS, nullptr, 0);
	m_pDXDC->PSSetShader(m_pPS, nullptr, 0);

	//상수버퍼 설정 
	m_pDXDC->VSSetConstantBuffers(0, 1, &m_pCBDef);		//상수 버퍼 설정.(VS)
	m_pDXDC->VSSetConstantBuffers(1, 1, &m_pCBLit);		//조명 정보 설정.(VS)
	m_pDXDC->VSSetConstantBuffers(2, 1, &m_pCBMtrl);	//재질 정보 설정.(VS)
	m_pDXDC->PSSetConstantBuffers(2, 1, &m_pCBMtrl);	//재질 정보 설정.(PS)
	//셰이더 리소스 설정.
	m_pDXDC->PSSetShaderResources(0, 1, &m_pTextureRV);	//PS 에 텍스처 설정.

	
	
	//그리기! Render a triangle 
	m_pDXDC->DrawIndexed(m_IndexCnt, 0, 0);
	//m_pDXDC->Draw(m_VtxCnt, 0);


	//노멀라인 그리기.
	//if(g_bNLine) g_NLine.Draw(&g_mTM);

	//장치 상태 리셋 
	//m_pDXDC->ClearState();



	return YN_OK;
}














/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// ynBox 전용 전역 함수들
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
//  ynBoxCreate : 예나 상자 생성 및 필요 리소스 로딩.
//
int ynBoxCreate(LPDEVICE pDev, LPDXDC pDXDC, TCHAR* texName, ynBox** ppBox)
{ 

	//박스 생성..
	ynBox* pBox = new ynBox;
	pBox->Create(pDev, pDXDC, texName); 
	  

	//외부로 리턴..
	*ppBox = pBox;
	

	return TRUE;

}






 

////////////////////////////////////////////////////////////////////////////// 
//
// ynLoadTexture : 텍스처 로드
//
int ynLoadTexture(LPDEVICE pDev, LPDXDC pDXDC, TCHAR* filename, LPTEXTURERV* ppTexRV)
{
	HRESULT hr = S_OK;	
	 
	LPTEXTURERV pTexRV = nullptr;

	//TCHAR* fileName = L"../data/sonim.jpg";
	
	//일반 텍스처 로드.
	//hr = DirectX::CreateWICTextureFromFile( pDev, fileName, nullptr, &g_pTextureRV );	
	
	//밉멥+텍스처 로드.
	hr = DirectX::CreateWICTextureFromFileEx( pDev, pDXDC, filename, 0, 
				D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, //| D3D11_BIND_RENDER_TARGET,
				0,	D3D11_RESOURCE_MISC_GENERATE_MIPS,	WIC_LOADER_DEFAULT, 
				nullptr, &pTexRV );	

	if (FAILED(hr))
	{
		//DDS 파일로 로드 시도. : +밉멥
	/*	hr = DirectX::CreateDDSTextureFromFileEx( pDev, pDXDC, filename, 0, 
				D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, //| D3D11_BIND_RENDER_TARGET,
				0,	D3D11_RESOURCE_MISC_GENERATE_MIPS,	false, //_LOADER_DEFAULT, 
				nullptr, &pTexRV );	 */
		hr = DirectX::CreateDDSTextureFromFile( pDev, filename, nullptr, &pTexRV );	
		if (FAILED(hr))
		{
			ynError(hr, L"텍스처 로드 실패 \nFile=%s", filename);
			return hr;
		}
	}

	

	//텍스처 밉멥생성.
	//pDXDC->GenerateMips(g_pTextureRV);
	
	//외부로 리턴.
	*ppTexRV = pTexRV;

	return YN_OK;
}





/////////////////////////////////////////////////////////////////////////////
//




/************************* end of file "ynBox.cpp " **************************/