//
// Camera.cpp : 카메라 클래스 소스
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
#include "Camera.h"
#include "Shader.h"		//셰이더 관련 헤더.
//#include "Light.h"		//조명 관련 헤더.
//#include "YnBox.h"		//예나 상자.
//#include "Map.h"		//지형 헤더.
//#include "Shadow.h"		//그림자 헤더.



/////////////////////////////////////////////////////////////////////////////
//
//  전역 데이터들.
//  

extern cbDEFAULT g_cbDEf;
extern VECTOR3   g_vPos;


 /////////////////////////////////////////////////////////////////////////////
//
//  카메라 데이터들 : 클래스화 필요.
//  

/*
XMFLOAT3		g_vEye(2.0f, 1.5f, -5.0f);		//카메라 위치1.(Position)
//XMFLOAT3		g_vEye(3.0f, 2.5f, -6.0f);		//카메라 위치1.(Position)
//XMFLOAT3		g_vEye(12.0f, 14.0f, -24.0f);		//카메라 위치1.(Position)
XMFLOAT3		g_vLookAt(0.0f, 1.0f, 0.0f);	//바라보는 곳1.(Position)
XMFLOAT3		g_vUp(0.0f, 1.0f, 0.0f);		//카메라 상방 벡터1.(Direction)
*/
XMVECTOR		g_vEye = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, -5.0f));	//카메라 위치1.(Position)
XMVECTOR		g_vLookAt = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0, 0.0f));	//바라보는 곳1.(Position)
XMVECTOR		g_vUp = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));		//카메라 상방 벡터1.(Direction)
XMVECTOR		g_vHorz = XMLoadFloat3(&XMFLOAT3(1, 0, 0));			//수평백터.
XMVECTOR		g_vView = XMLoadFloat3(&XMFLOAT3(0, 0, 1));			//시선벡터.

// 투영 변환 정보. 
float g_fFov   = XMConvertToRadians(45);	//기본 FOV 앵글. Field of View (Y) 
float g_fAspect =  1.333f;		//가로:세로 비율. 800:600 = 1.33:1 (4:3)   960:600 = 1.6:1 (16:10 HD)
float g_fZnear = 1.0f;			//시야 최소 거리 (1.0m) 
float g_fZfar = 1000.0f;		//시야 최대 거리 (100m) 


//카메라 리셋용 백업데이터.
XMVECTOR	g_vEyeBk;
XMVECTOR	g_vLookAtBk;
XMVECTOR	g_vUpBk;


BOOL g_bCamTrackHero = TRUE;		//주인공 따라가기 On/Off.






/////////////////////////////////////////////////////////////////////////////// 
//
// 카메라 갱신.
//
void CameraSetup(VECTOR3 eye, VECTOR3 lookat, VECTOR3 up)
{
 
	/*//모델 중심부 주시. 
	VECTOR3 lookat = g_pModel->GetCenter();
	g_vLookAt = XMLoadFloat3(&g_pXFile->GetCenter());

	//카메라 위치 : 모델 위치로 부터 일정거리 떨어짐.
	VECTOR3 cpos = (2, 1, -5);
	VECTOR3 eye = lookat + g_pModel->GetRadius() * 0.5f * cpos;		
	*/	

	//카메라 위치 : 지정 위치로 부터 일정거리 떨어짐.
	//VECTOR3 eye(0, 3, -6);
	//VECTOR3 lookat(0, 0, 0);
	

	//카메라 초기 위치 정보 갱신.
	g_vLookAt = XMLoadFloat3(&lookat);
	g_vEye = XMLoadFloat3(&eye);
	CameraUpdate(0);


	//카메라 리셋용 백업데이터.
	g_vLookAtBk = g_vLookAt;
	g_vEyeBk = g_vEye;
	g_vUpBk = g_vUp;
}




/////////////////////////////////////////////////////////////////////////////// 
//
// 카메라 리셋.
//
void CameraReset()
{
	g_vLookAt = g_vLookAtBk;
	g_vEye = g_vEyeBk;
	g_vUp = g_vUpBk;

	CameraUpdate(0);
}





/////////////////////////////////////////////////////////////////////////////// 
//
// 카메라 갱신.
//
void CameraUpdate(float dTime/*=0*/)
{
 
	//-----------------
	// 뷰 변환 설정 
	//-----------------
	//주인공 주시 테스트
	//g_vLookAt = XMLoadFloat3(&g_vPos);

	/*//카메라, 주인공과 함께 움직이기.
	XMFLOAT3 cpos(0, 2, -10);
	g_vEye = g_vPos + cpos;			//카메라 위치 : 주인공 '현재 위치로 부터 일정거리 떨어짐.
	g_vLookAt = g_vPos;				//카메라 시점 : 주인공 '현재' 위치.
	*/

	// 뷰 변환 행렬 생성 :  View Transform 
	/*XMVECTOR eye	= XMLoadFloat3(&g_vEye);	//카메라 위치 
	XMVECTOR lookat = XMLoadFloat3(&g_vLookAt);	//바라보는 곳.위치.
	XMVECTOR up		= XMLoadFloat3(&g_vUp);		//카메라 상방 벡터.	
	XMMATRIX mView = XMMatrixLookAtLH(eye, lookat, up);			
	*/
	XMMATRIX mView = XMMatrixLookAtLH(g_vEye, g_vLookAt, g_vUp);			
 
 
	//-----------------
	// 투영 변환 설정.
	//----------------- 
	// 원근 투영 변환 행렬 생성 : Projection Transform.
	g_fAspect = g_Mode.Width/(float)g_Mode.Height;	 //현재 해상도 기준 FOV 설정. 		
	XMMATRIX mProj = XMMatrixPerspectiveFovLH(g_fFov, g_fAspect, g_fZnear, g_fZfar);	
	

	//----------------------------
	// 상수 버퍼 갱신.
	//----------------------------
	g_cbDef.mView = mView;
	g_cbDef.mProj = mProj;


	//----------------------------
	// Yena 로 카메라 정보를 전달 
	//----------------------------
	// Yena 공통 객체들은 갱신시 카메라 정보가 필요합니다. 
	// 사용자 카메라를 사용한다면, 반드시 Yena 로 전달해야 합니다.
	yn_View = mView;
	yn_Proj = mProj;

}



 

/////////////////////////////////////////////////////////////////////////////// 
//
// 카메라 갱신. (자동 회전)
//
void CameraUpdateA(float dTime/*=0*/)
{

	//리셋.
	if (IsKeyDown(VK_F9))
	{
		CameraReset();
	}


	static BOOL bAni = FALSE;
	if (IsKeyUp('P'))  bAni ^= TRUE;

	#define yvCamEye  g_vEye
	#define yvCamView g_vView
	#define yvCamLookAt g_vLookAt
	#define yvCamUp   g_vUp
	#define yvCamHorz g_vHorz


	//카메라 자동 회전처리.
	if (bAni)
	{
		VECTOR vPrev = yvCamEye;
		VECTOR vNew = XMVectorZero();

		float angle = 3.14f * 0.05f * dTime;
		//vNew.x = sinf(angle) + vPrev.x;  		vNew.y = vPrev.y; 		vNew.z = cosf(angle) + vPrev.z;
		MATRIXA mRot = XMMatrixRotationY(angle);
		vNew = XMVector3TransformCoord(vPrev, mRot);
		yvCamEye = vNew;
	}

	// (자동회전 후) 현재 카메라 위치 교정.
	yvCamView = yvCamLookAt - yvCamEye;
	yvCamView = XMVector3Normalize(yvCamView);
	yvCamHorz = XMVector3Cross(yvCamUp, yvCamView);
	yvCamHorz = XMVector3Normalize(yvCamHorz);


	// 카메라 변환 미세조정 값.
	float scale = 1.0f;
	if (IsKeyDown(VK_SHIFT)) scale = 0.25f;

	//----------------------
	// 카메라 이동하기 (앞/뒤)
	//----------------------
	VECTOR  camPos = XMVectorZero();
	static float mov = 20.0f;
	//	if(IsKeyDown('A')) camPos -= yvCamHorz * mov * dTime;
	//	if(IsKeyDown('D')) camPos += yvCamHorz * mov * dTime;
	if (IsKeyDown('U')) camPos += yvCamView * mov * dTime * scale;
	if (IsKeyDown('O')) camPos -= yvCamView * mov * dTime * scale;
	//	if(IsKeyDown('R')) camPos += yvCamUp * mov * dTime;
	//	if(IsKeyDown('F')) camPos -= yvCamUp * mov * dTime;
	yvCamEye = camPos + yvCamEye;


	//----------------------
	// 카메라 회전하기.
	//----------------------
	VECTOR3 camRot(0, 0, 0);
	static const float rot = XM_PI / 4.0f;
	if (IsKeyDown('I'))	camRot.x += rot * dTime * scale;
	if (IsKeyDown('K'))	camRot.x -= rot * dTime * scale;
	if (IsKeyDown('J'))	camRot.y += rot * dTime * scale;
	if (IsKeyDown('L'))	camRot.y -= rot * dTime * scale;
	//	if(IsKeyDown(DIK_INSERT)) camRot.z += rot * dTime;
	//	if(IsKeyDown(DIK_PRIOR)) camRot.z -= rot * dTime;
	MATRIXA mRotX, mRotY;
	mRotX = XMMatrixRotationAxis(yvCamHorz, camRot.x);
	mRotY = XMMatrixRotationAxis(yvCamUp, camRot.y);
	MATRIXA mRot = mRotX * mRotY;
	//최종 카메라 시점 계산.
	yvCamEye = XMVector3TransformCoord(yvCamEye, mRot);




	//----------------------
	// 카메라 행렬 갱신.
	//----------------------
	CameraUpdate(dTime);
	//ynCameraUpdateMtx();		 

	//카메라 사용중 확인.
	//ybCamOn = TRUE;

	#undef yvCamEye
	#undef yvCamView
	#undef yvCamLookAt
	#undef yvCamUp
	#undef yvCamHorz

}



/////////////////////////////////////////////////////////////////////////////// 
//
// 카메라 갱신. : 목표물 주시/추적 
//
void CameraUpdateATK(float dTime/*=0*/)
{
  
	//-----------------
	// 카메라 변환. (+자동회전)
	//-----------------
	CameraUpdateA(dTime);
	

	//-----------------
	// 주인공 추적하기.
	//-----------------
	if (IsKeyUp('C')) g_bCamTrackHero ^= TRUE;

	static XMMATRIX mTGBk = XMMatrixIdentity();	
	XMMATRIX mTG;	
	VECTOR3  Tgt = g_vPos;		//목표좌표 : 현재 주인공 위치

	if (g_bCamTrackHero)		//주인공 추적중..	
	{
		mTG = XMMatrixTranslation(Tgt.x, Tgt.y, Tgt.z);
		mTGBk = mTG;
	}
	else
	{							//추적 안함. (주인공 주시)
		mTG = mTGBk;			//마지막 위치 고정.
	}
	
	// 카메라 새 위치 계산.
	VECTOR vEye, vLookAt;
	vEye = XMVector3Transform(g_vEye, mTG);			//카메라 위치 : 주인공 '현재 위치로 부터 일정거리 떨어짐.
	vLookAt = XMVector3Transform(g_vLookAt, mTG);		//카메라 시점 : 주인공 '현재' 위치.
		
	
	//-----------------
	// 새로운 카메라 변환행렬 생성.
	//-----------------
	XMMATRIX mView = XMMatrixLookAtLH(vEye, vLookAt, g_vUp);			
 	// 투영 변환 설정. (생략)
 	g_fAspect = g_Mode.Width/(float)g_Mode.Height;	 //현재 해상도 기준 FOV 설정. 		
	XMMATRIX mProj = XMMatrixPerspectiveFovLH(g_fFov, g_fAspect, g_fZnear, g_fZfar);	
	

	//----------------------------
	// 상수 버퍼 갱신.
	//----------------------------
	g_cbDef.mView = mView;
	g_cbDef.mProj = mProj;


	//----------------------------
	// Yena 로 카메라 정보를 전달 
	//----------------------------
	// Yena 공통 객체들은 갱신시 카메라 정보가 필요합니다. 
	// 사용자 카메라를 사용한다면, 반드시 Yena 로 전달해야 합니다.
	yn_View = mView;
	yn_Proj = mProj;

}






 
/////////////////////////////////////////////////////////////////////////////// 
//
//  도움말. 
//
void CameraInfo(int& x, int& y, COLOR col, float dTime/*=0*/)
{ 
/*	//int x = 1, y = 150;
	COLOR col(1, 1, 1, 1);
	COLOR col2(1, 1, 0, 1);
	COLOR col3(0.5f, 0.5f, 0.5f, 1);
 
	//카메라 정보 출력.
	y += 14;
	ynTextDraw(x, y += 14, col2, L"[Camera]");
	ynTextDraw(x, y += 14, col2, L"리셋 : F9");
	ynTextDraw(x, y += 14, col2, L"회전 : JKLI UO");
	ynTextDraw(x, y += 14, col2, L"자동 : P");	
	ynTextDraw(x, y += 14, g_bCamTrackHero?col2:col3, L"추적 : C (%s)", g_bCamTrackHero?L"ON":L"OFF");

 
	//카메라 정보 출력.
	x = 200;  y = g_Mode.Height - 14 * 5;
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
	x = 400;  y = g_Mode.Height - 14 * 5;
	//col = COLOR(1, 1, 0, 1);
	ynTextDraw(x, y+=14, col, L"[렌즈]");  
	ynTextDraw(x, y+=14, col, L"FOV=%.2f (%.2fº)", g_fFov, XMConvertToDegrees(g_fFov)); 
	ynTextDraw(x, y+=14, col, L"Aspect=%.2f", g_fAspect);  
	ynTextDraw(x, y+=14, col, L"Zn=%.1f  Zf = %.1f", g_fZnear, g_fZfar);
*/
}

 





/****************** end of file "Camera.cpp" *********************************/