//
// Render.cpp : 기본 렌더링/프레임웍 소스
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
#include "Render.h"
#include "Shader.h"		//셰이더 관련 헤더.
#include "Light.h"		//조명 관련 헤더.
#include "Map.h"		//지형 헤더.
#include "Camera.h"		//카메라 헤더.

#include "RenderTarget.h"	//렌더타겟 운용 헤더.★


TCHAR* g_WindowName = L"Yena::D3D T22 RenderTarget (I) 01 RenderToTexture (과제 제작용)";



/////////////////////////////////////////////////////////////////////////////
//
//  전역 데이터들.
//  

//예나 기본 메쉬. : Yena/ynMesh.h 참조
ynMesh*  g_pMesh = nullptr;

//애니메이션. On/Off.
BOOL	g_bAni = TRUE;

// 기본 상수버퍼 데이터 : 각 렌더링 개체(유닛)별 기본 정보를 운용.
//cbDEFAULT g_cbDef;
//#define g_cbCube  g_cbDef

// 모델 기하 정보 
XMMATRIX g_mTM;
XMFLOAT3 g_vPos(0, 0, 0);		//위치 : "월드 공간"
XMFLOAT3 g_vRot(0, 0, 0);		//회전 
XMFLOAT3 g_vScale(1, 1, 1);		//스케일 

MATERIAL g_Mtrl;				//재질 정보.


// 더미 : 10개
// 
#define DUMMY_MAX_ 20
VECTOR3 g_vDummy[DUMMY_MAX_] = {

	{ -5.0f, 0,  5.0f },		//좌측열 상자들.
	{-20.0f, 0, 27.0f },
	{ 30.0f, 0, 34.0f },
	{ 40.0f, 0, 44.0f },
	{ 20.0f, 0, 27.0f },
	{  5.0f, 0,  5.0f },
	{-10.0f, 0, 15.0f }, 
	{-40.0f, 0, 44.0f },
	{-30.0f, 0, 34.0f },
	
	{ 10.0f, 0, 15.0f },		//중앙 상자.

	{ 40.0f, 0, 34.0f },		//우측열 상자들.
	{ 20.0f, 0, 37.0f },
	{-20.0f, 0, 37.0f },
	{-10.0f, 0, 25.0f }, 
	{ 30.0f, 0, 44.0f },
	{-30.0f, 0, 44.0f },
	{  5.0f, 0, 10.0f },
	{-40.0f, 0, 34.0f },
	{ -5.0f, 0, 20.0f },
	{ 10.0f, 0, 25.0f },

};

XMMATRIX g_DummyTM[DUMMY_MAX_];
//XMMATRIX g_DummyTMInv[DUMMYMAX_];


//더미 애니값.
float g_fDummyAng[DUMMY_MAX_] = {0.0f};	
float g_fDummySin[DUMMY_MAX_] = {0.0f};	

//더미 색상.
COLOR g_DummyColor[DUMMY_MAX_] = {

	{ 0.70f, 0.82f, 0.34f, 1.0f},	//좌측열 상자들.
	{ 0.92f, 0.56f, 0.77f, 1.0f},
	{ 0.11f, 0.40f, 0.01f, 1.0f},
	{ 0.59f, 0.36f, 0.85f, 1.0f},
	{ 0.09f, 0.77f, 0.49f, 1.0f},
	{ 0.27f, 0.73f, 0.77f, 1.0f},
	{ 0.96f, 0.24f, 0.50f, 1.0f},
	{ 0.55f, 0.81f, 0.75f, 1.0f},
	{ 0.47f, 0.63f, 0.28f, 1.0f},

	{ 0.56f, 0.06f, 0.20f, 1.0f},	//중앙 상자.

	{ 0.04f, 0.02f, 0.35f, 1.0f},	//우측열 상자들.
	{ 0.62f, 0.26f, 0.98f, 1.0f},
	{ 0.40f, 0.56f, 0.68f, 1.0f},
	{ 0.87f, 0.83f, 0.87f, 1.0f},
	{ 0.90f, 0.18f, 0.65f, 1.0f},
	{ 0.43f, 0.09f, 0.77f, 1.0f},
	{ 0.72f, 0.62f, 0.95f, 1.0f},
	{ 0.37f, 0.08f, 0.42f, 1.0f},
	{ 0.72f, 0.14f, 0.93f, 1.0f},
	{ 0.14f, 0.84f, 0.63f, 1.0f},
};

#define RANDOM_ (rand()%100 * 0.01f)

int  ObjLoad();
void ObjRelease();
void ObjUpdate(float dTime);
void ObjDraw(float dTime);
void ObjDraw(XMMATRIX& mTM, COLOR col);




////////////////////////////////////////////////////////////////////////////// 
//
// 렌더링 상태 조절 데이터/함수들.
//

//오브제 출력 처리.
BOOL g_bShowMap = TRUE;		
BOOL g_bShowObj = TRUE;
BOOL g_bShowScene = TRUE;

//렌더타겟 갱신 처리.
BOOL g_bRTargetOn = TRUE;
BOOL g_bRTargetUpdate = TRUE;
 
//텍스처 멥핑 On/Off.
BOOL g_bTexOn = TRUE;			





/////////////////////////////////////////////////////////////////////////////
//
int DataLoading()
{ 
	// 데이터 로딩/생성 코드는 여기에...
	// ...
	
	//셰이더 로드.
	ShaderLoad();
	
	//조명 설정.
	LightsInit();	


	//오브젝트 로딩.
	ObjLoad();
	MapCreate();

	// 렌더링 타겟 만들기.★
	RenderTargetCreate();

	//카메라 설정.
	CameraSetup(VECTOR3(0, 3, -10.0f), VECTOR3(0, 1.5f, 0), VECTOR3(0, 1, 0));


	//그리드 ON, 방향축 OFF.
	ynGridAxisState(FALSE, FALSE);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
//
void DataRelease()
{
	// 데이터 해제 코드는 여기에..
	// ... 
	
	RenderTargetRelease();	//렌더타겟 제거.★

	ObjRelease();			//오브젝트 삭제 
	MapRelease();

	LightsRelease();		//조명제거.
	ShaderRelease();		//셰이더 삭제.
}







//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//
// Rendering Object.
//
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////
//
//  오브젝트 : 생성 및 필요 리소스 로딩.
//
int ObjLoad()
{
	HRESULT hr = S_OK;	
	
	//외부 설정 파일 읽기.
	//LoadIniData(NULL, g_BoxTexName);

	//예나 메쉬 생성.
	ynMeshCreateTeapotHD(g_pDevice, g_pDXDC, &g_pMesh);


	//더미들.
	srand(::GetTickCount());
	for(int i=0; i<DUMMY_MAX_; )
	{		
		/*//오브제 위치 랜덤..
		float x = rand()%80 - 40.0f;
		float z = rand()%80 - 40.0f;				
		VECTOR pos = XMVectorSet(x, 0, z);
		if(XMVector3Length(pos) < 4.0f)     //중앙-반경 4m 이내에서는 재설정...
		{ continue; }	 
		g_vDummyPos[i] = pos + VECTOR3(0, 1, 0);
		*/

		//애니 및 변환행렬  계산.	
 		g_DummyTM[i] = XMMatrixTranslation(g_vDummy[i].x, g_vDummy[i].y, g_vDummy[i].z); 		
		// 색상 랜덤.
		//g_DummyColor[i] = COLOR( RANDOM_, RANDOM_, RANDOM_, 1); 

		i++;
	}


	return TRUE;
}



////////////////////////////////////////////////////////////////////////////// 
//
// 오브젝트 : 제거 
//
void ObjRelease()
{
	SAFE_DELETE(g_pMesh);
}




////////////////////////////////////////////////////////////////////////////// 
//
// 오브젝트 갱신
//
void ObjUpdate(float dTime)
{
	if (IsKeyDown(VK_F9))		//모델 정보 리셋.
	{
		g_vPos = XMFLOAT3(0, 0, 0);
		g_vRot = XMFLOAT3(0, 0, 0);
		g_vScale = XMFLOAT3(1, 1, 1);
	}

	//자동회전처리..
	if(IsKeyUp('R')){  g_bAni ^= TRUE; }	

	
	//스케일 처리.
	XMMATRIX mScale = XMMatrixScaling(g_vScale.x, g_vScale.y, g_vScale.z);

	//이동 처리.
	float mov = 10.0f * dTime;					//10 m/s 이동량.
	if (IsKeyDown(VK_LEFT))  g_vPos.x -= mov;
	if (IsKeyDown(VK_RIGHT)) g_vPos.x += mov;
	if (IsKeyDown(VK_UP))	 g_vPos.z += mov;
	if (IsKeyDown(VK_DOWN))  g_vPos.z -= mov;
	if (IsKeyDown('Q'))		 g_vPos.y -= mov * 0.2f;			 
	if (IsKeyDown('E'))		 g_vPos.y += mov * 0.2f; 
	XMMATRIX mTrans = XMMatrixTranslation(g_vPos.x, g_vPos.y, g_vPos.z);

	//회전 처리.  
	float rot = XM_PI * 0.5f * dTime;		//90º/sec 씩 회전.(라디안)
	if (IsKeyDown('A'))  g_vRot.y += rot;
	if (IsKeyDown('D'))  g_vRot.y -= rot;
	if (IsKeyDown('W'))  g_vRot.x += rot;
	if (IsKeyDown('S'))  g_vRot.x -= rot;
	if(g_bAni) g_vRot.y += rot * 0.5f;				
	XMMATRIX mRot = XMMatrixRotationRollPitchYaw(g_vRot.x, g_vRot.y, g_vRot.z);
	 
	
	//-----------------------
	// 변환 행렬 결합 
	//-----------------------
	//XMMATRIX mTM = mScale * mRot * mTrans;
	g_mTM = mScale * mRot * mTrans;

	//-----------------------
	// 상수버퍼로 갱신
	//-----------------------
	// --> 모델클래스에서 처리.

	//-----------------------
	// 재질 정보 갱신. 
	//-----------------------
	// --> 모델클래스에서 처리.

	//-----------------------
	//모델 변환 정보 갱신
	//-----------------------
	//텍스처 상태.
	//g_pBox->TexOn(g_bWireFrame?FALSE:g_bTexOn);
	//모델 갱신
	//g_pBox->Update(dTime);


}




////////////////////////////////////////////////////////////////////////////// 
//
// 오브젝트 그리기.
//
void ObjDraw()
{	
	
	//그리기!
	ObjDraw(g_mTM, COLOR(1, 0, 0, 1));			//★
	//g_pBox->Draw(&g_mTM); 


	//장치 상태 리셋 
	//g_pDXDC->ClearState();
}




////////////////////////////////////////////////////////////////////////////// 
//
// 오브젝트 그리기.
//
void ObjDraw(XMMATRIX& mTM, COLOR col)
{	

	//-----------------------
	// 상수버퍼 갱신
	//-----------------------
	//변환 행렬 갱신.
	g_cbDef.mTM = mTM;
	g_cbDef.mWV = mTM * g_cbDef.mView;	
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBDef, &g_cbDef, sizeof(cbDEFAULT));


	// 재질 정보 갱신. 
	g_Mtrl.Diffuse = col;
	g_Mtrl.Ambient = col;
	g_Mtrl.Specular = COLOR(1, 1, 1, 1);
	g_Mtrl.Power = 30.0f;
	//재질 정보 상수버퍼 갱신.
	g_cbMtrl.Diffuse = XMLoadFloat4(&g_Mtrl.Diffuse); 
	g_cbMtrl.Ambient = XMLoadFloat4(&g_Mtrl.Ambient);
	g_cbMtrl.Specular = XMLoadFloat4(&g_Mtrl.Specular);
	g_cbMtrl.Power = g_Mtrl.Power;
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMtrl, &g_cbMtrl, sizeof(cbMATERIAL));



	//-----------------------
	// 기하 및 셰이더 설정.
	//-----------------------
  	//g_pDXDC->IASetVertexBuffers(0, 1, &g_pVBMap, &stride, &offset);			//정점버퍼 설정 --> ynMesh 에서 처리.
	g_pDXDC->IASetInputLayout(g_pVBLayout2);									//입력레이아웃 설정. 
	//g_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	//기하위상구조 설정 --> ynMesh 에서 처리.

	//셰이더 설정 
	g_pDXDC->VSSetShader(g_pVS2, nullptr, 0);			//셰이더2번 사용.
	g_pDXDC->PSSetShader(g_pPS2, nullptr, 0);
	//셰이더 리소스 설정
	g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBDef);		//VS 에 상수 버퍼 설정.
	g_pDXDC->VSSetConstantBuffers(1, 1, &g_pCBLit);		//VS 에 조명 정보 설정. 
	g_pDXDC->VSSetConstantBuffers(2, 1, &g_pCBMtrl);	//VS 에 재질 정보 설정. 
	
	
	//-----------------------
	// 그리기! 
	//-----------------------
	g_pMesh->Draw();
	//g_pBox->Draw(&g_mTM); 


	// 장치 상태 리셋 
	//g_pDXDC->ClearState();
}



 


////////////////////////////////////////////////////////////////////////////// 
//
// 더미 상자 갱신 
//
void DummyUpdate(float dTime)
{
	/*//리셋.
	if (IsKeyDown(VK_F9))
	{
		for (int i = 0; i < DUMMYMAX_; i++)
		{
			g_fDummySin[i] = 0;
			g_fDummyAng[i] = 0;
		}
	}*/

	/*//애니 중지.
	if (!g_bAni) return;


	//애니 및 변환행렬  계산.	
	for(int i=0; i<DUMMY_MAX_; i++)
	{ 	 
		g_fDummyAng[i] += 3.14f * 0.3f * dTime;    

		if( i % 2 == 0)
			g_fDummySin[i] = sin(g_fDummyAng[i]); 
		else
			g_fDummySin[i] = cos(g_fDummyAng[i]); 

		
		//행렬계산
		float y = g_vDummy[i].y + g_fDummySin[i];
		if(y <= 0.9999f) y = 1.0f; 
		g_DummyTM[i] = XMMatrixTranslation(g_vDummy[i].x, y-1.0f, g_vDummy[i].z); 		
		
	}  */

}




////////////////////////////////////////////////////////////////////////////// 
//
// 더미 그리기 
//	
void DummyDraw()
{	
	for(int i=0; i<DUMMY_MAX_; i++)
	{
		ObjDraw(g_DummyTM[i], g_DummyColor[i]);
		//g_pBox->Draw(&g_DummyTM[i]);
	} 
}





 

//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//
// System Functions
//
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//
// 엔진-시스템 공통 데이터  갱신.. : 그려려니 합시다. 차차 공부할 것입니다.
// 
float EngineUpdate()
{
	//엔진 시간 / 타이머 얻기.
	float dTime = GetEngineTime();

	//Yena 갱신 : 불필요한 기능은 주석처리하여 기능을 비활성화 하십시요.
	//ynCameraUpdate(dTime);		// 카메라 갱신	
	//ynLightsUpdate(dTime);		// 조명 갱신						
	ynGridAxisUpdate(dTime);		// 방향축, 그리드 갱신.
	//YenaUpdate(dTime);			// 예나 시스템 전체 일괄 갱신.


	//렌더링 상태 갱신.
	RenderStateUpdate();


	// 렌더링 옵션 조절 	 
	if (IsKeyUp(VK_SPACE))	g_bWireFrame ^= TRUE;
	if (IsKeyUp(VK_F4))		g_bCullBack ^= TRUE;
	if (IsKeyUp(VK_F5))		g_bDiffOn ^= TRUE;
	//if (IsKeyUp(VK_F6))		g_bSpecOn ^= TRUE;		
	//if (IsKeyUp(VK_F7))		g_bStencilOn ^= TRUE;		
	//if (IsKeyUp(VK_F7))		g_bShadowOn ^= TRUE;	


	//장면 연출 on/off.
	/*if(IsKeyDown(VK_F9))		//리셋.
	{
		g_bShowMap = TRUE;
		g_bShowObj = TRUE;
		g_bShowScene = TRUE;
	}*/
	if(IsKeyUp('1')) g_bShowMap ^= TRUE;
	if(IsKeyUp('2')) g_bShowObj ^= TRUE;
	if(IsKeyUp('3')) g_bShowScene ^= TRUE;
	//렌더타겟 사용 On/Off.
	if(IsKeyUp('4')) g_bRTargetOn ^= TRUE;	
	if(IsKeyUp('5')) g_bRTargetUpdate ^= TRUE;	
	//그리드 출력.
	//ynGridAxisState(!g_bShowScene||!g_bShowMap, 0);



	return dTime;
}





/////////////////////////////////////////////////////////////////////////////// 
//
//  도움말. 
//
void ShowInfo(float dTime)
{
	PutFPS(0, 0);

	static bool bShow = true;
	if (IsKeyUp(VK_F1)) bShow ^= true;

	if (!bShow)
	{
		//ynTextDraw(1,20, COLOR(1, 1, 0, 1), "[Help] F1"); 
		return;
	}


	// Today's Topic.
	{
		int x = g_Mode.Width / 2 - 150, y = 1;
		COLOR col(1, 1, 1, 1);
		COLOR col2(1, 1, 0, 1);
		COLOR col3(0.4f, 0.4f, 0.4f, 1);
		ynTextDraw(x, y, col, L"■ %s", g_WindowName);

		x = g_Mode.Width / 2 + 50;
		y = 50;
		TCHAR msg[512] =
			L"> 렌더타겟에 장면을 저장(렌더링)하고 \n"
			L"> 이를 이용해 다양한 그래픽효과를 만들 수 있습니다. \n"
			L"> \n"
			L"> 전체장면을 출력하는 '전광판(TV)'을 만들어 봅시다. \n"
			L"> ";
		ynTextDrawSV(x, y += 14, col2, msg, dTime);
	}
	
	//시스템 정보.
	{
		int x = 1, y = 14;
		COLOR col(0, 1, 0, 1);
		SystemInfo(x, y, col);
	}

	
	//추가 도움말..
	int x = 1, y = 120;
	COLOR col(1, 1, 1, 1);
	COLOR col2(1, 1, 0, 1);
	COLOR col3(0.5f, 0.5f, 0.5f, 1);
	y += 14;
	ynTextDraw(x, y += 14, col, L"Fill: Space (%s)", g_bWireFrame ? L"WIRE" : L"SOLID");
	ynTextDraw(x, y += 14, col, L"Cull: F4 (%s)", g_bCullBack ? L"ON" : L"OFF");
	ynTextDraw(x, y += 14, g_bDiffOn ? col : col*0.5f, L"Lit: F5 (%s)", g_bDiffOn ? L"ON" : L"OFF");
	//ynTextDraw(x, y += 14, col, L"깊이테스트: F5 (%s)", g_bZEnable?L"ON":L"OFF");
	//ynTextDraw(x, y += 14, g_bZEnable?col:col*0.5f, L"깊이쓰기: F6 (%s)", g_bZWrite?L"ON":L"OFF");
	//ynTextDraw(x, y += 14, g_bShadowOn ? col2 : col3, L"그림자: F7 (%s)", g_bShadowOn?L"ON":L"OFF");
	y += 14;
	ynTextDraw(x, y+=14, g_bShowMap?col2:col*0.8f, L"지형: 1 (%s)", g_bShowMap?L"ON":L"OFF");
	ynTextDraw(x, y+=14, g_bShowObj?col2:col*0.8f, L"오브제: 2 (%s)", g_bShowObj?L"ON":L"OFF");
	ynTextDraw(x, y+=14, g_bShowScene?col2:col*0.8f, L"월드장면: 3 (%s)", g_bShowScene ? L"ON" : L"OFF");

	{
		y += 14;
		COLOR col = g_bRTargetOn ? COLOR(1, 1, 0, 1) : col3;
		ynTextDraw(x, y += 14, col, L"[렌더타겟]");
		ynTextDraw(x, y += 14, col, L"보기: 4 (%s)", g_bRTargetOn ? L"ON" : L"OFF");
		ynTextDraw(x, y += 14, g_bRTargetUpdate ? col : col*0.8f, L"갱신: 5 (%s)", g_bRTargetUpdate ? L"ON" : L"OFF");
	}

	//카메라 정보 출력.
	y += 14;
	COLOR col4(0, 1, 1, 1);
	ynTextDraw(x, y += 14, col4, L"[Camera]");
	ynTextDraw(x, y += 14, col4, L"리셋 : F9");
	ynTextDraw(x, y += 14, col4, L"회전 : JKLI UO");
	ynTextDraw(x, y += 14, col4, L"자동 : P");	
	ynTextDraw(x, y += 14, g_bCamTrackHero?col4:col3, L"추적 : C (%s)", g_bCamTrackHero?L"ON":L"OFF");
		 

	//모델 정보 출력.
	y += 14;
	ynTextDraw(x, y += 14, col, L"[Model]");
	ynTextDraw(x, y += 14, col, L"리셋 : F9");
	ynTextDraw(x, y += 14, col, L"이동 : 방향키/QE");
	ynTextDraw(x, y += 14, col, L"회전 : ADWS");
	ynTextDraw(x, y += 14, col, L"자동 : R");

	//모델 추가 정보.
	//g_pMesh->Info(x, y += 14, COLOR(1, 0.5f, 0.5f, 1));


	col = COLOR(0, 1, 1, 1);
	y = g_Mode.Height - 14 * 5;
	ynTextDraw(x, y += 14, col, L"[Model]");
	ynTextDraw(x, y += 14, col, L"Pos = { %.2f, %.2f, %.2f }", g_vPos.x, g_vPos.y, g_vPos.z);	
	ynTextDraw(x, y += 14, col, L"Rot = { %.2f, %.2f, %.2f }", g_vRot.x, g_vRot.y, g_vRot.z);	
	ynTextDraw(x, y += 14, col, L"Scl = { %.2f, %.2f, %.2f }", g_vScale.x, g_vScale.y, g_vScale.z);	
	 

	//카메라 정보 출력.
	x += 200;  y = g_Mode.Height - 14 * 5;
	//col = COLOR(1, 1, 0, 1)*0.8f;
	XMFLOAT3 eye, lookat, up;
	XMStoreFloat3(&eye, g_vEye);
	XMStoreFloat3(&lookat, g_vLookAt);
	XMStoreFloat3(&up, g_vUp);
	ynTextDraw(x, y+=14, col, L"[카메라]");  
	ynTextDraw(x, y+=14, col, L"Eye={%.2f, %.2f, %.2f}", eye.x, eye.y, eye.z);  
	ynTextDraw(x, y+=14, col, L"Look={%.2f, %.2f, %.2f}", lookat.x, lookat.y, lookat.z);  
	ynTextDraw(x, y+=14, col, L"Up={%.2f, %.2f, %.2f}", up.x, up.y, up.z);  

	//카메라 - "렌즈" : 투영변환 정보 출력.
	x += 200;  y = g_Mode.Height - 14 * 5;
	//col = COLOR(1, 1, 0, 1);
	ynTextDraw(x, y+=14, col, L"[렌즈]");  
	ynTextDraw(x, y+=14, col, L"FOV=%.2f (%.2fº)", g_fFov, XMConvertToDegrees(g_fFov)); 
	ynTextDraw(x, y+=14, col, L"Aspect=%.2f", g_fAspect);  
	ynTextDraw(x, y+=14, col, L"Zn=%.1f  Zf = %.1f", g_fZnear, g_fZfar);

}

 


////////////////////////////////////////////////////////////////////////////// 
//
// 전체 장면 그리기
//
void SceneRender()
{
	//-------------------------------
	// 엔진/시스템 갱신.
	//------------------------------- 
	float dTime = EngineUpdate();

	// 장면 갱신 
	//
	LightsUpdate(dTime);	  //조명 갱신
	
	// 주인공, 몬스터, 오브제 이동, 충돌검사 등등..
	// ...
	ObjUpdate(dTime);
	DummyUpdate(dTime);
	MapUpdate(dTime);


	//CameraUpdate(dTime);	  //카메라갱신
	//CameraUpdateA(dTime);	  //카메라갱신 : (+자동캠)
	CameraUpdateATK(dTime);	  //카메라갱신 : (+자동캠+추적)


	// 새 렌더 타겟 : 장면 그리기 (갱신)★
	if(g_bRTargetOn && g_bRTargetUpdate) 
		RenderTargetSceneGenerate();		

	// 게임 로직, 이벤트, 층돌, 점수계산..
	// ...	 
	

	//-------------------------------
	// 장면 그리기 시작.. 
	//-------------------------------
 	RTClear(g_ClearColor);			//렌더타겟(백버퍼)/ 깊이버퍼 지우기..
			
	//시스템 객체 출력.
	//ynGridAxisDraw();

	
	//장면 그리기.
	if(g_bShowScene) _SceneRender();


	//렌더타겟 가시화 : "전광판" ★
	if(g_bRTargetOn) RenderTargetDraw();		


	//시스템 객체 & 도움말 출력..
	ynGridAxisDraw();
	ShowInfo(dTime);

	//-------------------------------
	// 장면 그리기 종료.
	//------------------------------- 
	Flip();
	

}//end of void SceneRender()





////////////////////////////////////////////////////////////////////////////// 
//
// 장면 그리기 (부분) : 렌더타겟 / 그림자멥 생성을 위한 공용함수.
//
void _SceneRender()
{
	if(g_bShowMap) MapDraw();
 
	if (g_bShowObj)
	{
		ObjDraw();
		DummyDraw();
	}
}



/****************** end of file "Render.cpp" *********************************/