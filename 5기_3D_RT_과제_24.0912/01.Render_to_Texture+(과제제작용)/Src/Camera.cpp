//
// Camera.cpp : ī�޶� Ŭ���� �ҽ�
//
// DirectX ���� ���� : DX11
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
#include "Shader.h"		//���̴� ���� ���.
//#include "Light.h"		//���� ���� ���.
//#include "YnBox.h"		//���� ����.
//#include "Map.h"		//���� ���.
//#include "Shadow.h"		//�׸��� ���.



/////////////////////////////////////////////////////////////////////////////
//
//  ���� �����͵�.
//  

extern cbDEFAULT g_cbDEf;
extern VECTOR3   g_vPos;


 /////////////////////////////////////////////////////////////////////////////
//
//  ī�޶� �����͵� : Ŭ����ȭ �ʿ�.
//  

/*
XMFLOAT3		g_vEye(2.0f, 1.5f, -5.0f);		//ī�޶� ��ġ1.(Position)
//XMFLOAT3		g_vEye(3.0f, 2.5f, -6.0f);		//ī�޶� ��ġ1.(Position)
//XMFLOAT3		g_vEye(12.0f, 14.0f, -24.0f);		//ī�޶� ��ġ1.(Position)
XMFLOAT3		g_vLookAt(0.0f, 1.0f, 0.0f);	//�ٶ󺸴� ��1.(Position)
XMFLOAT3		g_vUp(0.0f, 1.0f, 0.0f);		//ī�޶� ��� ����1.(Direction)
*/
XMVECTOR		g_vEye = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, -5.0f));	//ī�޶� ��ġ1.(Position)
XMVECTOR		g_vLookAt = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0, 0.0f));	//�ٶ󺸴� ��1.(Position)
XMVECTOR		g_vUp = XMLoadFloat3(&XMFLOAT3(0.0f, 1.0f, 0.0f));		//ī�޶� ��� ����1.(Direction)
XMVECTOR		g_vHorz = XMLoadFloat3(&XMFLOAT3(1, 0, 0));			//�������.
XMVECTOR		g_vView = XMLoadFloat3(&XMFLOAT3(0, 0, 1));			//�ü�����.

// ���� ��ȯ ����. 
float g_fFov   = XMConvertToRadians(45);	//�⺻ FOV �ޱ�. Field of View (Y) 
float g_fAspect =  1.333f;		//����:���� ����. 800:600 = 1.33:1 (4:3)   960:600 = 1.6:1 (16:10 HD)
float g_fZnear = 1.0f;			//�þ� �ּ� �Ÿ� (1.0m) 
float g_fZfar = 1000.0f;		//�þ� �ִ� �Ÿ� (100m) 


//ī�޶� ���¿� ���������.
XMVECTOR	g_vEyeBk;
XMVECTOR	g_vLookAtBk;
XMVECTOR	g_vUpBk;


BOOL g_bCamTrackHero = TRUE;		//���ΰ� ���󰡱� On/Off.






/////////////////////////////////////////////////////////////////////////////// 
//
// ī�޶� ����.
//
void CameraSetup(VECTOR3 eye, VECTOR3 lookat, VECTOR3 up)
{
 
	/*//�� �߽ɺ� �ֽ�. 
	VECTOR3 lookat = g_pModel->GetCenter();
	g_vLookAt = XMLoadFloat3(&g_pXFile->GetCenter());

	//ī�޶� ��ġ : �� ��ġ�� ���� �����Ÿ� ������.
	VECTOR3 cpos = (2, 1, -5);
	VECTOR3 eye = lookat + g_pModel->GetRadius() * 0.5f * cpos;		
	*/	

	//ī�޶� ��ġ : ���� ��ġ�� ���� �����Ÿ� ������.
	//VECTOR3 eye(0, 3, -6);
	//VECTOR3 lookat(0, 0, 0);
	

	//ī�޶� �ʱ� ��ġ ���� ����.
	g_vLookAt = XMLoadFloat3(&lookat);
	g_vEye = XMLoadFloat3(&eye);
	CameraUpdate(0);


	//ī�޶� ���¿� ���������.
	g_vLookAtBk = g_vLookAt;
	g_vEyeBk = g_vEye;
	g_vUpBk = g_vUp;
}




/////////////////////////////////////////////////////////////////////////////// 
//
// ī�޶� ����.
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
// ī�޶� ����.
//
void CameraUpdate(float dTime/*=0*/)
{
 
	//-----------------
	// �� ��ȯ ���� 
	//-----------------
	//���ΰ� �ֽ� �׽�Ʈ
	//g_vLookAt = XMLoadFloat3(&g_vPos);

	/*//ī�޶�, ���ΰ��� �Բ� �����̱�.
	XMFLOAT3 cpos(0, 2, -10);
	g_vEye = g_vPos + cpos;			//ī�޶� ��ġ : ���ΰ� '���� ��ġ�� ���� �����Ÿ� ������.
	g_vLookAt = g_vPos;				//ī�޶� ���� : ���ΰ� '����' ��ġ.
	*/

	// �� ��ȯ ��� ���� :  View Transform 
	/*XMVECTOR eye	= XMLoadFloat3(&g_vEye);	//ī�޶� ��ġ 
	XMVECTOR lookat = XMLoadFloat3(&g_vLookAt);	//�ٶ󺸴� ��.��ġ.
	XMVECTOR up		= XMLoadFloat3(&g_vUp);		//ī�޶� ��� ����.	
	XMMATRIX mView = XMMatrixLookAtLH(eye, lookat, up);			
	*/
	XMMATRIX mView = XMMatrixLookAtLH(g_vEye, g_vLookAt, g_vUp);			
 
 
	//-----------------
	// ���� ��ȯ ����.
	//----------------- 
	// ���� ���� ��ȯ ��� ���� : Projection Transform.
	g_fAspect = g_Mode.Width/(float)g_Mode.Height;	 //���� �ػ� ���� FOV ����. 		
	XMMATRIX mProj = XMMatrixPerspectiveFovLH(g_fFov, g_fAspect, g_fZnear, g_fZfar);	
	

	//----------------------------
	// ��� ���� ����.
	//----------------------------
	g_cbDef.mView = mView;
	g_cbDef.mProj = mProj;


	//----------------------------
	// Yena �� ī�޶� ������ ���� 
	//----------------------------
	// Yena ���� ��ü���� ���Ž� ī�޶� ������ �ʿ��մϴ�. 
	// ����� ī�޶� ����Ѵٸ�, �ݵ�� Yena �� �����ؾ� �մϴ�.
	yn_View = mView;
	yn_Proj = mProj;

}



 

/////////////////////////////////////////////////////////////////////////////// 
//
// ī�޶� ����. (�ڵ� ȸ��)
//
void CameraUpdateA(float dTime/*=0*/)
{

	//����.
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


	//ī�޶� �ڵ� ȸ��ó��.
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

	// (�ڵ�ȸ�� ��) ���� ī�޶� ��ġ ����.
	yvCamView = yvCamLookAt - yvCamEye;
	yvCamView = XMVector3Normalize(yvCamView);
	yvCamHorz = XMVector3Cross(yvCamUp, yvCamView);
	yvCamHorz = XMVector3Normalize(yvCamHorz);


	// ī�޶� ��ȯ �̼����� ��.
	float scale = 1.0f;
	if (IsKeyDown(VK_SHIFT)) scale = 0.25f;

	//----------------------
	// ī�޶� �̵��ϱ� (��/��)
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
	// ī�޶� ȸ���ϱ�.
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
	//���� ī�޶� ���� ���.
	yvCamEye = XMVector3TransformCoord(yvCamEye, mRot);




	//----------------------
	// ī�޶� ��� ����.
	//----------------------
	CameraUpdate(dTime);
	//ynCameraUpdateMtx();		 

	//ī�޶� ����� Ȯ��.
	//ybCamOn = TRUE;

	#undef yvCamEye
	#undef yvCamView
	#undef yvCamLookAt
	#undef yvCamUp
	#undef yvCamHorz

}



/////////////////////////////////////////////////////////////////////////////// 
//
// ī�޶� ����. : ��ǥ�� �ֽ�/���� 
//
void CameraUpdateATK(float dTime/*=0*/)
{
  
	//-----------------
	// ī�޶� ��ȯ. (+�ڵ�ȸ��)
	//-----------------
	CameraUpdateA(dTime);
	

	//-----------------
	// ���ΰ� �����ϱ�.
	//-----------------
	if (IsKeyUp('C')) g_bCamTrackHero ^= TRUE;

	static XMMATRIX mTGBk = XMMatrixIdentity();	
	XMMATRIX mTG;	
	VECTOR3  Tgt = g_vPos;		//��ǥ��ǥ : ���� ���ΰ� ��ġ

	if (g_bCamTrackHero)		//���ΰ� ������..	
	{
		mTG = XMMatrixTranslation(Tgt.x, Tgt.y, Tgt.z);
		mTGBk = mTG;
	}
	else
	{							//���� ����. (���ΰ� �ֽ�)
		mTG = mTGBk;			//������ ��ġ ����.
	}
	
	// ī�޶� �� ��ġ ���.
	VECTOR vEye, vLookAt;
	vEye = XMVector3Transform(g_vEye, mTG);			//ī�޶� ��ġ : ���ΰ� '���� ��ġ�� ���� �����Ÿ� ������.
	vLookAt = XMVector3Transform(g_vLookAt, mTG);		//ī�޶� ���� : ���ΰ� '����' ��ġ.
		
	
	//-----------------
	// ���ο� ī�޶� ��ȯ��� ����.
	//-----------------
	XMMATRIX mView = XMMatrixLookAtLH(vEye, vLookAt, g_vUp);			
 	// ���� ��ȯ ����. (����)
 	g_fAspect = g_Mode.Width/(float)g_Mode.Height;	 //���� �ػ� ���� FOV ����. 		
	XMMATRIX mProj = XMMatrixPerspectiveFovLH(g_fFov, g_fAspect, g_fZnear, g_fZfar);	
	

	//----------------------------
	// ��� ���� ����.
	//----------------------------
	g_cbDef.mView = mView;
	g_cbDef.mProj = mProj;


	//----------------------------
	// Yena �� ī�޶� ������ ���� 
	//----------------------------
	// Yena ���� ��ü���� ���Ž� ī�޶� ������ �ʿ��մϴ�. 
	// ����� ī�޶� ����Ѵٸ�, �ݵ�� Yena �� �����ؾ� �մϴ�.
	yn_View = mView;
	yn_Proj = mProj;

}






 
/////////////////////////////////////////////////////////////////////////////// 
//
//  ����. 
//
void CameraInfo(int& x, int& y, COLOR col, float dTime/*=0*/)
{ 
/*	//int x = 1, y = 150;
	COLOR col(1, 1, 1, 1);
	COLOR col2(1, 1, 0, 1);
	COLOR col3(0.5f, 0.5f, 0.5f, 1);
 
	//ī�޶� ���� ���.
	y += 14;
	ynTextDraw(x, y += 14, col2, L"[Camera]");
	ynTextDraw(x, y += 14, col2, L"���� : F9");
	ynTextDraw(x, y += 14, col2, L"ȸ�� : JKLI UO");
	ynTextDraw(x, y += 14, col2, L"�ڵ� : P");	
	ynTextDraw(x, y += 14, g_bCamTrackHero?col2:col3, L"���� : C (%s)", g_bCamTrackHero?L"ON":L"OFF");

 
	//ī�޶� ���� ���.
	x = 200;  y = g_Mode.Height - 14 * 5;
	//col = COLOR(1, 1, 0, 1)*0.8f;
	XMFLOAT3 eye, lookat, up;
	XMStoreFloat3(&eye, g_vEye);
	XMStoreFloat3(&lookat, g_vLookAt);
	XMStoreFloat3(&up, g_vUp);
	ynTextDraw(x, y+=14, col, L"[ī�޶�]");  
	ynTextDraw(x, y+=14, col, L"Eye={%.2f, %.2f, %.2f}", eye.x, eye.y, eye.z);  
	ynTextDraw(x, y+=14, col, L"Look={%.2f, %.2f, %.2f}", lookat.x, lookat.y, lookat.z);  
	ynTextDraw(x, y+=14, col, L"Up={%.2f, %.2f, %.2f}", up.x, up.y, up.z);  

	//ī�޶� - "����" : ������ȯ ���� ���.
	x = 400;  y = g_Mode.Height - 14 * 5;
	//col = COLOR(1, 1, 0, 1);
	ynTextDraw(x, y+=14, col, L"[����]");  
	ynTextDraw(x, y+=14, col, L"FOV=%.2f (%.2f��)", g_fFov, XMConvertToDegrees(g_fFov)); 
	ynTextDraw(x, y+=14, col, L"Aspect=%.2f", g_fAspect);  
	ynTextDraw(x, y+=14, col, L"Zn=%.1f  Zf = %.1f", g_fZnear, g_fZfar);
*/
}

 





/****************** end of file "Camera.cpp" *********************************/