//
// Map.cpp : 지형 관련 소스
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
#include "Map.h"
#include "Shader.h"		//셰이더 관련 헤더.
#include "Light.h"		//조명 관련 헤더.
//#include "XFile.h"	//XFile 운용 클래스 헤더.
//#include "YnModel.h"	//예나 모델 클래스 헤더.

#include "Map.h"		//지형 헤더.






/////////////////////////////////////////////////////////////////////////////
//
//  전역 데이터들.
//

 
// 모델용 기하입력구조 (외부 참조)
extern ID3D11InputLayout*  g_pVBLayout;
//int CreateVBLayout();





///////////////////////////////////////////////////////////////////////////// 
//
// 지형 데이터.
//

// 정점 구조체 
struct MAP_VTX
{
	float x, y, z; 			//좌표(Position)
	float nx, ny, nz;		//노멀 : Normal 	 
	float u, v;				//텍스처 좌표: Texture-Coordinates 
};


// 지형 모델용 기하버퍼.
ID3D11Buffer*		g_pVBMap = nullptr;			//정점 버퍼 포인터.
//ID3D11InputLayout*  g_pVBMapLayout = nullptr;	//정점 입력구조 -> 큐브 모델 공유.

// 지형 모델 텍스처
ID3D11ShaderResourceView*           g_pTexrvMap = nullptr;		
TCHAR g_TexNameMap[256]=L"N/A";					 

// 지형용 상수버퍼 : 각 렌더링 개체(유닛)별 CB 운용이 유리.
cbDEFAULT		g_cbMap;
ID3D11Buffer*	g_pCBMap = nullptr;


 
 



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// 지형 모델
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
//  지형 : 생성 및 필요 리소스 로딩.
//
int MapCreate()
{
	HRESULT hr = S_OK;
	
	//지형: 반복멥핑
	MAP_VTX	Quad[4]={
		{ -0.5, 0.0, 0.5,  0.0, 1.0, 0.0,   0.0,  0.0 },			
		{  0.5, 0.0, 0.5,  0.0, 1.0, 0.0,  20.0,  0.0 },			
		{ -0.5, 0.0,-0.5,  0.0, 1.0, 0.0,   0.0, 20.0 },		
		{  0.5, 0.0,-0.5,  0.0, 1.0, 0.0,  20.0, 20.0 },	
	};//*/

	 /*//지형: 단일멥핑
	COLVTX	Quad[4]={
		{ -1.0, 0.0, 1.0,  0.0, 1.0, 0.0,  0.0,  0.0 },			
		{  1.0, 0.0, 1.0,  0.0, 1.0, 0.0,  1.0,  0.0 },			
		{ -1.0, 0.0,-1.0,  0.0, 1.0, 0.0,  0.0,  1.0 },		
		{  1.0, 0.0,-1.0,  0.0, 1.0, 0.0,  1.0,  1.0 },	
	};//*/


	// 정점 버퍼 Vertex Buffer 생성
	DWORD size = sizeof(Quad);
	CreateVB(g_pDevice, Quad, sizeof(Quad), &g_pVBMap);	  

	// 정점 입력구조 Input layout 생성.
	// ... --> Cube 와 공유. 
		

	// 텍스처 로드 Load the Texture 
	//TCHAR* filename = L"../data/ground.jpg";
	TCHAR* filename = L"../data/Floor3.jpg";
	LoadTexture(filename, &g_pTexrvMap);
	

	// 상수 버퍼 생성.
	ZeroMemory(&g_cbMap, sizeof(cbDEFAULT)); 
	CreateDynamicConstantBuffer(sizeof(cbDEFAULT), &g_cbMap, &g_pCBMap); 


	return TRUE;
}



////////////////////////////////////////////////////////////////////////////// 
//
// 지형 : 제거 
//
void MapRelease()
{
	SAFE_RELEASE(g_pVBMap);			//모델 제거.
	//SAFE_RELEASE(g_pVBMapLayout);
	SAFE_RELEASE(g_pTexrvMap);		//텍스처 제거.
	SAFE_RELEASE(g_pCBMap);			//상수버퍼 제거.
}



////////////////////////////////////////////////////////////////////////////// 
//
// 지형 갱신
//
void MapUpdate(float dTime)
{
	//모델 리셋 
	//...
	

	//-----------------------
	// 변환 계산.
	//-----------------------
	//XMMATRIX mScale = XMMatrixScaling(30, 1, 30);		//30x30 (m) 지형.
	//XMMATRIX mScale = XMMatrixScaling(80, 1, 80);		//80x80 (m) 지형.
	//XMMATRIX mScale = XMMatrixScaling(100, 1, 100);	//100x100 (m) 지형.
	XMMATRIX mScale = XMMatrixScaling(256, 1, 256);		//256x256 (m) 지형.
	//XMMATRIX mRot = XMMatrixIdentity();	 	
	XMMATRIX mTM = mScale;
	

	//-----------------------
	// 상수버퍼로 기록.
	//-----------------------
	g_cbMap.mTM = mTM;
	g_cbMap.mWV = mTM * yn_View; 
	g_cbMap.mView = yn_View;		//전역 카메라 정보 설정. 
	g_cbMap.mProj = yn_Proj;		//일반 박스와 카메라 정보는 동일합니다.


	//셰이더 상수 버퍼 갱신.(동적버퍼)
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMap, &g_cbMap, sizeof(cbDEFAULT));
	//셰이더 상수 버퍼 갱신.(정적버퍼)
	//g_pDXDC->UpdateSubresource(g_pCBDef, 0, nullptr, &g_cbMap, 0, 0);


	//-----------------------
	// 재질 정보 갱신.
	//-----------------------
	//.. 이번 데모에서는 cube 모델과 공유.


}




////////////////////////////////////////////////////////////////////////////// 
//
// 지형 그리기.
//
void MapDraw(float dTime)
{	

	//-----------------------
	// 상수버퍼 갱신
	//-----------------------
	//변환행렬 갱신
	//g_cbMap.mTM = mTM;
	//g_cbMap.mWV = mTM * g_cbMap.mView;	
	//UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMap, &g_cbMap, sizeof(cbDEFAULT));

	// 조명 정보 갱신 : 지형은 Specular 를 사용하지 않겠습니다.
	cbLIGHT cbLit = g_cbLit;
	cbLit.SpecOn = FALSE;
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &cbLit, sizeof(cbLIGHT));
	
	//재질 정보 갱신.
	extern BOOL g_bTexOn;
	g_cbMtrl.Diffuse = XMLoadFloat4(&COLOR(1,1,1,1)); 
	g_cbMtrl.Ambient = XMLoadFloat4(&COLOR(1,1,1,1));
	//g_cbMtrl.Specular = XMLoadFloat4(&COLOR(1,1,1,1));  --> 지형은 Specular 를 사용하지 않겠습니다.
	//g_cbMtrl.Power = 30;
	//g_cbMtrl.bTexOn = g_bTexOn ? TRUE : FALSE;			//텍스처 적용 On/Off.
	g_cbMtrl.bTexOn = g_bWireFrame ? FALSE : g_bTexOn;		//텍스처 적용 On/Off.
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMtrl, &g_cbMtrl, sizeof(cbMATERIAL));
	
	

  	//-----------------------
	// 기하 및 셰이더 설정.
	//-----------------------
	UINT stride = sizeof(MAP_VTX);
	UINT offset = 0;
	g_pDXDC->IASetVertexBuffers(0, 1, &g_pVBMap, &stride, &offset);				//정점버퍼 설정
	g_pDXDC->IASetInputLayout(g_pVBLayout);										//입력레이아웃 설정.  
	g_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	//기하위상구조 설정 	
	
	//셰이더 설정. : 표준 조명 셰이더
	g_pDXDC->VSSetShader(g_pVS, nullptr, 0);
	g_pDXDC->PSSetShader(g_pPS, nullptr, 0);		
	//vs 셰이더 리소스 설정.
	g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBMap);		//행렬 정보 설정.(지형정보로 갱신)
	//g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBDef);	//상수 버퍼 설정. 
	g_pDXDC->VSSetConstantBuffers(1, 1, &g_pCBLit);		//조명 정보 설정. 
	g_pDXDC->VSSetConstantBuffers(2, 1, &g_pCBMtrl);	//재질 정보 설정. 
	//PS 셰이더 리소스 설정.
	g_pDXDC->PSSetConstantBuffers(2, 1, &g_pCBMtrl);	//재질 정보 설정.(PS)
	g_pDXDC->PSSetShaderResources(0, 1, &g_pTexrvMap);	//PS 에 텍스처 설정. (지형텍스처)
 	
														
	//-----------------------
	//그리기! 
	//-----------------------
	g_pDXDC->Draw(4, 0);		


	// 조명 복구 
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &g_cbLit, sizeof(cbLIGHT));


	//장치 상태 리셋 
	//g_pDXDC->ClearState();
}











/****************** end of file "Map.cpp" *********************************/