//
// Render.cpp : �⺻ ������/�����ӿ� �ҽ�
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
#include "Render.h"
#include "Shader.h"		//���̴� ���� ���.
#include "Light.h"		//���� ���� ���.
#include "Map.h"		//���� ���.
#include "Camera.h"		//ī�޶� ���.

#include "RenderTarget.h"	//����Ÿ�� ��� ���.��


TCHAR* g_WindowName = L"Yena::D3D T22 RenderTarget (I) 01 RenderToTexture (���� ���ۿ�)";



/////////////////////////////////////////////////////////////////////////////
//
//  ���� �����͵�.
//  

//���� �⺻ �޽�. : Yena/ynMesh.h ����
ynMesh*  g_pMesh = nullptr;

//�ִϸ��̼�. On/Off.
BOOL	g_bAni = TRUE;

// �⺻ ������� ������ : �� ������ ��ü(����)�� �⺻ ������ ���.
//cbDEFAULT g_cbDef;
//#define g_cbCube  g_cbDef

// �� ���� ���� 
XMMATRIX g_mTM;
XMFLOAT3 g_vPos(0, 0, 0);		//��ġ : "���� ����"
XMFLOAT3 g_vRot(0, 0, 0);		//ȸ�� 
XMFLOAT3 g_vScale(1, 1, 1);		//������ 

MATERIAL g_Mtrl;				//���� ����.


// ���� : 10��
// 
#define DUMMY_MAX_ 20
VECTOR3 g_vDummy[DUMMY_MAX_] = {

	{ -5.0f, 0,  5.0f },		//������ ���ڵ�.
	{-20.0f, 0, 27.0f },
	{ 30.0f, 0, 34.0f },
	{ 40.0f, 0, 44.0f },
	{ 20.0f, 0, 27.0f },
	{  5.0f, 0,  5.0f },
	{-10.0f, 0, 15.0f }, 
	{-40.0f, 0, 44.0f },
	{-30.0f, 0, 34.0f },
	
	{ 10.0f, 0, 15.0f },		//�߾� ����.

	{ 40.0f, 0, 34.0f },		//������ ���ڵ�.
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


//���� �ִϰ�.
float g_fDummyAng[DUMMY_MAX_] = {0.0f};	
float g_fDummySin[DUMMY_MAX_] = {0.0f};	

//���� ����.
COLOR g_DummyColor[DUMMY_MAX_] = {

	{ 0.70f, 0.82f, 0.34f, 1.0f},	//������ ���ڵ�.
	{ 0.92f, 0.56f, 0.77f, 1.0f},
	{ 0.11f, 0.40f, 0.01f, 1.0f},
	{ 0.59f, 0.36f, 0.85f, 1.0f},
	{ 0.09f, 0.77f, 0.49f, 1.0f},
	{ 0.27f, 0.73f, 0.77f, 1.0f},
	{ 0.96f, 0.24f, 0.50f, 1.0f},
	{ 0.55f, 0.81f, 0.75f, 1.0f},
	{ 0.47f, 0.63f, 0.28f, 1.0f},

	{ 0.56f, 0.06f, 0.20f, 1.0f},	//�߾� ����.

	{ 0.04f, 0.02f, 0.35f, 1.0f},	//������ ���ڵ�.
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
// ������ ���� ���� ������/�Լ���.
//

//������ ��� ó��.
BOOL g_bShowMap = TRUE;		
BOOL g_bShowObj = TRUE;
BOOL g_bShowScene = TRUE;

//����Ÿ�� ���� ó��.
BOOL g_bRTargetOn = TRUE;
BOOL g_bRTargetUpdate = TRUE;
 
//�ؽ�ó ���� On/Off.
BOOL g_bTexOn = TRUE;			





/////////////////////////////////////////////////////////////////////////////
//
int DataLoading()
{ 
	// ������ �ε�/���� �ڵ�� ���⿡...
	// ...
	
	//���̴� �ε�.
	ShaderLoad();
	
	//���� ����.
	LightsInit();	


	//������Ʈ �ε�.
	ObjLoad();
	MapCreate();

	// ������ Ÿ�� �����.��
	RenderTargetCreate();

	//ī�޶� ����.
	CameraSetup(VECTOR3(0, 3, -10.0f), VECTOR3(0, 1.5f, 0), VECTOR3(0, 1, 0));


	//�׸��� ON, ������ OFF.
	ynGridAxisState(FALSE, FALSE);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
//
void DataRelease()
{
	// ������ ���� �ڵ�� ���⿡..
	// ... 
	
	RenderTargetRelease();	//����Ÿ�� ����.��

	ObjRelease();			//������Ʈ ���� 
	MapRelease();

	LightsRelease();		//��������.
	ShaderRelease();		//���̴� ����.
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
//  ������Ʈ : ���� �� �ʿ� ���ҽ� �ε�.
//
int ObjLoad()
{
	HRESULT hr = S_OK;	
	
	//�ܺ� ���� ���� �б�.
	//LoadIniData(NULL, g_BoxTexName);

	//���� �޽� ����.
	ynMeshCreateTeapotHD(g_pDevice, g_pDXDC, &g_pMesh);


	//���̵�.
	srand(::GetTickCount());
	for(int i=0; i<DUMMY_MAX_; )
	{		
		/*//������ ��ġ ����..
		float x = rand()%80 - 40.0f;
		float z = rand()%80 - 40.0f;				
		VECTOR pos = XMVectorSet(x, 0, z);
		if(XMVector3Length(pos) < 4.0f)     //�߾�-�ݰ� 4m �̳������� �缳��...
		{ continue; }	 
		g_vDummyPos[i] = pos + VECTOR3(0, 1, 0);
		*/

		//�ִ� �� ��ȯ���  ���.	
 		g_DummyTM[i] = XMMatrixTranslation(g_vDummy[i].x, g_vDummy[i].y, g_vDummy[i].z); 		
		// ���� ����.
		//g_DummyColor[i] = COLOR( RANDOM_, RANDOM_, RANDOM_, 1); 

		i++;
	}


	return TRUE;
}



////////////////////////////////////////////////////////////////////////////// 
//
// ������Ʈ : ���� 
//
void ObjRelease()
{
	SAFE_DELETE(g_pMesh);
}




////////////////////////////////////////////////////////////////////////////// 
//
// ������Ʈ ����
//
void ObjUpdate(float dTime)
{
	if (IsKeyDown(VK_F9))		//�� ���� ����.
	{
		g_vPos = XMFLOAT3(0, 0, 0);
		g_vRot = XMFLOAT3(0, 0, 0);
		g_vScale = XMFLOAT3(1, 1, 1);
	}

	//�ڵ�ȸ��ó��..
	if(IsKeyUp('R')){  g_bAni ^= TRUE; }	

	
	//������ ó��.
	XMMATRIX mScale = XMMatrixScaling(g_vScale.x, g_vScale.y, g_vScale.z);

	//�̵� ó��.
	float mov = 10.0f * dTime;					//10 m/s �̵���.
	if (IsKeyDown(VK_LEFT))  g_vPos.x -= mov;
	if (IsKeyDown(VK_RIGHT)) g_vPos.x += mov;
	if (IsKeyDown(VK_UP))	 g_vPos.z += mov;
	if (IsKeyDown(VK_DOWN))  g_vPos.z -= mov;
	if (IsKeyDown('Q'))		 g_vPos.y -= mov * 0.2f;			 
	if (IsKeyDown('E'))		 g_vPos.y += mov * 0.2f; 
	XMMATRIX mTrans = XMMatrixTranslation(g_vPos.x, g_vPos.y, g_vPos.z);

	//ȸ�� ó��.  
	float rot = XM_PI * 0.5f * dTime;		//90��/sec �� ȸ��.(����)
	if (IsKeyDown('A'))  g_vRot.y += rot;
	if (IsKeyDown('D'))  g_vRot.y -= rot;
	if (IsKeyDown('W'))  g_vRot.x += rot;
	if (IsKeyDown('S'))  g_vRot.x -= rot;
	if(g_bAni) g_vRot.y += rot * 0.5f;				
	XMMATRIX mRot = XMMatrixRotationRollPitchYaw(g_vRot.x, g_vRot.y, g_vRot.z);
	 
	
	//-----------------------
	// ��ȯ ��� ���� 
	//-----------------------
	//XMMATRIX mTM = mScale * mRot * mTrans;
	g_mTM = mScale * mRot * mTrans;

	//-----------------------
	// ������۷� ����
	//-----------------------
	// --> ��Ŭ�������� ó��.

	//-----------------------
	// ���� ���� ����. 
	//-----------------------
	// --> ��Ŭ�������� ó��.

	//-----------------------
	//�� ��ȯ ���� ����
	//-----------------------
	//�ؽ�ó ����.
	//g_pBox->TexOn(g_bWireFrame?FALSE:g_bTexOn);
	//�� ����
	//g_pBox->Update(dTime);


}




////////////////////////////////////////////////////////////////////////////// 
//
// ������Ʈ �׸���.
//
void ObjDraw()
{	
	
	//�׸���!
	ObjDraw(g_mTM, COLOR(1, 0, 0, 1));			//��
	//g_pBox->Draw(&g_mTM); 


	//��ġ ���� ���� 
	//g_pDXDC->ClearState();
}




////////////////////////////////////////////////////////////////////////////// 
//
// ������Ʈ �׸���.
//
void ObjDraw(XMMATRIX& mTM, COLOR col)
{	

	//-----------------------
	// ������� ����
	//-----------------------
	//��ȯ ��� ����.
	g_cbDef.mTM = mTM;
	g_cbDef.mWV = mTM * g_cbDef.mView;	
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBDef, &g_cbDef, sizeof(cbDEFAULT));


	// ���� ���� ����. 
	g_Mtrl.Diffuse = col;
	g_Mtrl.Ambient = col;
	g_Mtrl.Specular = COLOR(1, 1, 1, 1);
	g_Mtrl.Power = 30.0f;
	//���� ���� ������� ����.
	g_cbMtrl.Diffuse = XMLoadFloat4(&g_Mtrl.Diffuse); 
	g_cbMtrl.Ambient = XMLoadFloat4(&g_Mtrl.Ambient);
	g_cbMtrl.Specular = XMLoadFloat4(&g_Mtrl.Specular);
	g_cbMtrl.Power = g_Mtrl.Power;
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMtrl, &g_cbMtrl, sizeof(cbMATERIAL));



	//-----------------------
	// ���� �� ���̴� ����.
	//-----------------------
  	//g_pDXDC->IASetVertexBuffers(0, 1, &g_pVBMap, &stride, &offset);			//�������� ���� --> ynMesh ���� ó��.
	g_pDXDC->IASetInputLayout(g_pVBLayout2);									//�Է·��̾ƿ� ����. 
	//g_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	//���������� ���� --> ynMesh ���� ó��.

	//���̴� ���� 
	g_pDXDC->VSSetShader(g_pVS2, nullptr, 0);			//���̴�2�� ���.
	g_pDXDC->PSSetShader(g_pPS2, nullptr, 0);
	//���̴� ���ҽ� ����
	g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBDef);		//VS �� ��� ���� ����.
	g_pDXDC->VSSetConstantBuffers(1, 1, &g_pCBLit);		//VS �� ���� ���� ����. 
	g_pDXDC->VSSetConstantBuffers(2, 1, &g_pCBMtrl);	//VS �� ���� ���� ����. 
	
	
	//-----------------------
	// �׸���! 
	//-----------------------
	g_pMesh->Draw();
	//g_pBox->Draw(&g_mTM); 


	// ��ġ ���� ���� 
	//g_pDXDC->ClearState();
}



 


////////////////////////////////////////////////////////////////////////////// 
//
// ���� ���� ���� 
//
void DummyUpdate(float dTime)
{
	/*//����.
	if (IsKeyDown(VK_F9))
	{
		for (int i = 0; i < DUMMYMAX_; i++)
		{
			g_fDummySin[i] = 0;
			g_fDummyAng[i] = 0;
		}
	}*/

	/*//�ִ� ����.
	if (!g_bAni) return;


	//�ִ� �� ��ȯ���  ���.	
	for(int i=0; i<DUMMY_MAX_; i++)
	{ 	 
		g_fDummyAng[i] += 3.14f * 0.3f * dTime;    

		if( i % 2 == 0)
			g_fDummySin[i] = sin(g_fDummyAng[i]); 
		else
			g_fDummySin[i] = cos(g_fDummyAng[i]); 

		
		//��İ��
		float y = g_vDummy[i].y + g_fDummySin[i];
		if(y <= 0.9999f) y = 1.0f; 
		g_DummyTM[i] = XMMatrixTranslation(g_vDummy[i].x, y-1.0f, g_vDummy[i].z); 		
		
	}  */

}




////////////////////////////////////////////////////////////////////////////// 
//
// ���� �׸��� 
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
// ����-�ý��� ���� ������  ����.. : �׷����� �սô�. ���� ������ ���Դϴ�.
// 
float EngineUpdate()
{
	//���� �ð� / Ÿ�̸� ���.
	float dTime = GetEngineTime();

	//Yena ���� : ���ʿ��� ����� �ּ�ó���Ͽ� ����� ��Ȱ��ȭ �Ͻʽÿ�.
	//ynCameraUpdate(dTime);		// ī�޶� ����	
	//ynLightsUpdate(dTime);		// ���� ����						
	ynGridAxisUpdate(dTime);		// ������, �׸��� ����.
	//YenaUpdate(dTime);			// ���� �ý��� ��ü �ϰ� ����.


	//������ ���� ����.
	RenderStateUpdate();


	// ������ �ɼ� ���� 	 
	if (IsKeyUp(VK_SPACE))	g_bWireFrame ^= TRUE;
	if (IsKeyUp(VK_F4))		g_bCullBack ^= TRUE;
	if (IsKeyUp(VK_F5))		g_bDiffOn ^= TRUE;
	//if (IsKeyUp(VK_F6))		g_bSpecOn ^= TRUE;		
	//if (IsKeyUp(VK_F7))		g_bStencilOn ^= TRUE;		
	//if (IsKeyUp(VK_F7))		g_bShadowOn ^= TRUE;	


	//��� ���� on/off.
	/*if(IsKeyDown(VK_F9))		//����.
	{
		g_bShowMap = TRUE;
		g_bShowObj = TRUE;
		g_bShowScene = TRUE;
	}*/
	if(IsKeyUp('1')) g_bShowMap ^= TRUE;
	if(IsKeyUp('2')) g_bShowObj ^= TRUE;
	if(IsKeyUp('3')) g_bShowScene ^= TRUE;
	//����Ÿ�� ��� On/Off.
	if(IsKeyUp('4')) g_bRTargetOn ^= TRUE;	
	if(IsKeyUp('5')) g_bRTargetUpdate ^= TRUE;	
	//�׸��� ���.
	//ynGridAxisState(!g_bShowScene||!g_bShowMap, 0);



	return dTime;
}





/////////////////////////////////////////////////////////////////////////////// 
//
//  ����. 
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
		ynTextDraw(x, y, col, L"�� %s", g_WindowName);

		x = g_Mode.Width / 2 + 50;
		y = 50;
		TCHAR msg[512] =
			L"> ����Ÿ�ٿ� ����� ����(������)�ϰ� \n"
			L"> �̸� �̿��� �پ��� �׷���ȿ���� ���� �� �ֽ��ϴ�. \n"
			L"> \n"
			L"> ��ü����� ����ϴ� '������(TV)'�� ����� ���ô�. \n"
			L"> ";
		ynTextDrawSV(x, y += 14, col2, msg, dTime);
	}
	
	//�ý��� ����.
	{
		int x = 1, y = 14;
		COLOR col(0, 1, 0, 1);
		SystemInfo(x, y, col);
	}

	
	//�߰� ����..
	int x = 1, y = 120;
	COLOR col(1, 1, 1, 1);
	COLOR col2(1, 1, 0, 1);
	COLOR col3(0.5f, 0.5f, 0.5f, 1);
	y += 14;
	ynTextDraw(x, y += 14, col, L"Fill: Space (%s)", g_bWireFrame ? L"WIRE" : L"SOLID");
	ynTextDraw(x, y += 14, col, L"Cull: F4 (%s)", g_bCullBack ? L"ON" : L"OFF");
	ynTextDraw(x, y += 14, g_bDiffOn ? col : col*0.5f, L"Lit: F5 (%s)", g_bDiffOn ? L"ON" : L"OFF");
	//ynTextDraw(x, y += 14, col, L"�����׽�Ʈ: F5 (%s)", g_bZEnable?L"ON":L"OFF");
	//ynTextDraw(x, y += 14, g_bZEnable?col:col*0.5f, L"���̾���: F6 (%s)", g_bZWrite?L"ON":L"OFF");
	//ynTextDraw(x, y += 14, g_bShadowOn ? col2 : col3, L"�׸���: F7 (%s)", g_bShadowOn?L"ON":L"OFF");
	y += 14;
	ynTextDraw(x, y+=14, g_bShowMap?col2:col*0.8f, L"����: 1 (%s)", g_bShowMap?L"ON":L"OFF");
	ynTextDraw(x, y+=14, g_bShowObj?col2:col*0.8f, L"������: 2 (%s)", g_bShowObj?L"ON":L"OFF");
	ynTextDraw(x, y+=14, g_bShowScene?col2:col*0.8f, L"�������: 3 (%s)", g_bShowScene ? L"ON" : L"OFF");

	{
		y += 14;
		COLOR col = g_bRTargetOn ? COLOR(1, 1, 0, 1) : col3;
		ynTextDraw(x, y += 14, col, L"[����Ÿ��]");
		ynTextDraw(x, y += 14, col, L"����: 4 (%s)", g_bRTargetOn ? L"ON" : L"OFF");
		ynTextDraw(x, y += 14, g_bRTargetUpdate ? col : col*0.8f, L"����: 5 (%s)", g_bRTargetUpdate ? L"ON" : L"OFF");
	}

	//ī�޶� ���� ���.
	y += 14;
	COLOR col4(0, 1, 1, 1);
	ynTextDraw(x, y += 14, col4, L"[Camera]");
	ynTextDraw(x, y += 14, col4, L"���� : F9");
	ynTextDraw(x, y += 14, col4, L"ȸ�� : JKLI UO");
	ynTextDraw(x, y += 14, col4, L"�ڵ� : P");	
	ynTextDraw(x, y += 14, g_bCamTrackHero?col4:col3, L"���� : C (%s)", g_bCamTrackHero?L"ON":L"OFF");
		 

	//�� ���� ���.
	y += 14;
	ynTextDraw(x, y += 14, col, L"[Model]");
	ynTextDraw(x, y += 14, col, L"���� : F9");
	ynTextDraw(x, y += 14, col, L"�̵� : ����Ű/QE");
	ynTextDraw(x, y += 14, col, L"ȸ�� : ADWS");
	ynTextDraw(x, y += 14, col, L"�ڵ� : R");

	//�� �߰� ����.
	//g_pMesh->Info(x, y += 14, COLOR(1, 0.5f, 0.5f, 1));


	col = COLOR(0, 1, 1, 1);
	y = g_Mode.Height - 14 * 5;
	ynTextDraw(x, y += 14, col, L"[Model]");
	ynTextDraw(x, y += 14, col, L"Pos = { %.2f, %.2f, %.2f }", g_vPos.x, g_vPos.y, g_vPos.z);	
	ynTextDraw(x, y += 14, col, L"Rot = { %.2f, %.2f, %.2f }", g_vRot.x, g_vRot.y, g_vRot.z);	
	ynTextDraw(x, y += 14, col, L"Scl = { %.2f, %.2f, %.2f }", g_vScale.x, g_vScale.y, g_vScale.z);	
	 

	//ī�޶� ���� ���.
	x += 200;  y = g_Mode.Height - 14 * 5;
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
	x += 200;  y = g_Mode.Height - 14 * 5;
	//col = COLOR(1, 1, 0, 1);
	ynTextDraw(x, y+=14, col, L"[����]");  
	ynTextDraw(x, y+=14, col, L"FOV=%.2f (%.2f��)", g_fFov, XMConvertToDegrees(g_fFov)); 
	ynTextDraw(x, y+=14, col, L"Aspect=%.2f", g_fAspect);  
	ynTextDraw(x, y+=14, col, L"Zn=%.1f  Zf = %.1f", g_fZnear, g_fZfar);

}

 


////////////////////////////////////////////////////////////////////////////// 
//
// ��ü ��� �׸���
//
void SceneRender()
{
	//-------------------------------
	// ����/�ý��� ����.
	//------------------------------- 
	float dTime = EngineUpdate();

	// ��� ���� 
	//
	LightsUpdate(dTime);	  //���� ����
	
	// ���ΰ�, ����, ������ �̵�, �浹�˻� ���..
	// ...
	ObjUpdate(dTime);
	DummyUpdate(dTime);
	MapUpdate(dTime);


	//CameraUpdate(dTime);	  //ī�޶󰻽�
	//CameraUpdateA(dTime);	  //ī�޶󰻽� : (+�ڵ�ķ)
	CameraUpdateATK(dTime);	  //ī�޶󰻽� : (+�ڵ�ķ+����)


	// �� ���� Ÿ�� : ��� �׸��� (����)��
	if(g_bRTargetOn && g_bRTargetUpdate) 
		RenderTargetSceneGenerate();		

	// ���� ����, �̺�Ʈ, ����, �������..
	// ...	 
	

	//-------------------------------
	// ��� �׸��� ����.. 
	//-------------------------------
 	RTClear(g_ClearColor);			//����Ÿ��(�����)/ ���̹��� �����..
			
	//�ý��� ��ü ���.
	//ynGridAxisDraw();

	
	//��� �׸���.
	if(g_bShowScene) _SceneRender();


	//����Ÿ�� ����ȭ : "������" ��
	if(g_bRTargetOn) RenderTargetDraw();		


	//�ý��� ��ü & ���� ���..
	ynGridAxisDraw();
	ShowInfo(dTime);

	//-------------------------------
	// ��� �׸��� ����.
	//------------------------------- 
	Flip();
	

}//end of void SceneRender()





////////////////////////////////////////////////////////////////////////////// 
//
// ��� �׸��� (�κ�) : ����Ÿ�� / �׸��ڸ� ������ ���� �����Լ�.
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