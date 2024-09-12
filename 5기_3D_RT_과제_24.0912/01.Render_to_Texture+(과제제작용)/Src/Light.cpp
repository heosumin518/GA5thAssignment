//
// YnLight.cpp : �⺻ ������/�����ӿ� �ҽ�
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

#include "Light.h"


 
/////////////////////////////////////////////////////////////////////////////
//
// ���� ����
//
BOOL  g_bDiffOn = TRUE;			//Ȯ�걤 ���� On/Off.
BOOL  g_bSpecOn = TRUE;			//���ݻ籤 ���� On/Off.

LIGHT			g_Light;				//���� ����.
cbLIGHT			g_cbLit;					//���� ������� ���ſ�. (����,�ӽù���)
ID3D11Buffer*	g_pCBLit = nullptr;		//����� �������.(����)

void LightsUpdate(float dTime);

 


/////////////////////////////////////////////////////////////////////////////
//
// ���� ����
//
//MATERIAL		g_Mtrl;					//���� ����.
cbMATERIAL		g_cbMtrl;				//���� ������� ���ſ�.(����,�ӽù���)
ID3D11Buffer*	g_pCBMtrl = nullptr;	//������ �������.(����)





/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
//
// Light Interfaces
//
/////////////////////////////////////////////////////////////////////////////// 

//////////////////////////////////////////////////////////////////////////////// 
//
// ���� ����.
//
void LightsInit()
{ 

	//---------------- 
	// ���� ����
	//---------------- 	 
	// �ֱ��� ����  
	g_Light.Direction = XMFLOAT3( 1.0f, -1.0f, 1.0f );		//�� ����.(Normalize �ϴ� ���� ��õ. �̹� ���������� �̷��� ���)
	g_Light.Range = 1000.0f;								//������ �ִ� ���� �Ÿ� : Directional Light ���Ѵ�.(��������)
	g_Light.Diffuse = COLOR(1, 1, 1, 1);					//���� (100% ���).
	g_Light.Specular = COLOR(1, 1, 1, 1);					//���ݻ籤�� (100% ���).
	g_Light.Ambient = COLOR(0.2f, 0.2f, 0.2f, 1.0f);		//20% ȸ�� : ǥ�� �ֺ�������
		

	//---------------- 
	// ����� ������� ����.
	//---------------- 
	// ���������� �ǽð����� ������ �ʴ´ٸ�, �Ϲ�(����) ������۷ε� ����մϴ�.
	ZeroMemory(&g_cbLit, sizeof(cbLIGHT)); 
	CreateDynamicConstantBuffer(sizeof(cbLIGHT), &g_cbLit, &g_pCBLit); 
	

	//---------------- 
	// ���� ����
	//---------------- 	 
	//... ������  ���� �����ϴ� ��ü�� ������ �����̹Ƿ� 
	//... �� ��ü���� �и�, ����ϴ� ���� ���ڽ��ϴ�. 

	//---------------- 
	// ������ ������� ����.
	//---------------- 
	// ���������� �ǽð����� ������ �ʴ´ٸ�, �Ϲ�(����) ������۷ε� ����մϴ�.
	ZeroMemory(&g_cbMtrl, sizeof(cbMATERIAL)); 
	CreateDynamicConstantBuffer(sizeof(cbMATERIAL), &g_cbMtrl, &g_pCBMtrl); 
 

	//���� ���� ����.
	//LightsUpdate();


}




////////////////////////////////////////////////////////////////////////////// 
//
// ���� ���� ����. 
//
void LightsRelease()
{
	SAFE_RELEASE(g_pCBLit);			//���� ������� ����.
	SAFE_RELEASE(g_pCBMtrl);		//���� ������� ����.
}




//////////////////////////////////////////////////////////////////////////////// 
//
// ���� ���� : ���������� �ǽð����� ������ �ʴ´ٸ�, ������ ���ʿ�.
//
void LightsUpdate(float dTime)
{ 
	//if(!g_bDiffOn) return;

	//������ ���� ����.
	//if (IsKeyUp(VK_F5))	g_bDiffOn ^= TRUE;		
	//if (IsKeyUp(VK_F6))	g_bSpecOn ^= TRUE;		
	

	//---------------- 
	// ���� ���� ����.
	//---------------- 	 
	// ���������� �ǽð����� ������ �ʴ´ٸ�, ������ �ʿ� �����ϴ�.
	// ...
	
 
	//---------------- 
	// ������� ����.
	//---------------- 
	XMVECTOR dir = XMLoadFloat3(&g_Light.Direction);	//������ normalize �Ǿ�� �մϴ�.
	g_cbLit.Direction = XMVector3Normalize(-dir);		//������ ����.
	g_cbLit.Diffuse = XMLoadFloat4(&g_Light.Diffuse);
	g_cbLit.Ambient = XMLoadFloat4(&g_Light.Ambient);
	g_cbLit.Specular= XMLoadFloat4(&g_Light.Specular);
	g_cbLit.Range = g_Light.Range;
	g_cbLit.DiffOn = g_bDiffOn;	
	g_cbLit.SpecOn = g_bSpecOn;	
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &g_cbLit, sizeof(cbLIGHT));
   
	//---------------- 
	// ���� ���� Material
	//---------------- 
	//... ������ ������ ��ü�� ���� �����ϴ� ���� �����̹Ƿ� 
	//... �� ��ü���� �и�, ����ϴ� ���� ���ڽ��ϴ�. 

}





/////////////////////////////////////////////////////////////////////////////// 
//
void LightUpdate(LIGHT& lt, float dTime/*=0*/)
{
	//...
}




/////////////////////////////////////////////////////////////////////////////// 
//
void LightInfo(LIGHT& lt, int& x, int& y, COLOR& col)
{   
	//int x = 1, y = 150;
	//COLOR col(1, 1, 1, 1);
	//COLOR col2(1, 1, 0, 1);
 	
	//���� ���� ���.
	ynTextDraw(x, y += 14, col, L"[Light]");
	ynTextDraw(x, y += 14, col, L"Dir = { %.2f, %.2f, %.2f }", lt.Direction.x, lt.Direction.y, lt.Direction.z);
	ynTextDraw(x, y += 14, col, L"Diff = { %.2f, %.2f, %.2f, %.2f }", lt.Diffuse.x, lt.Diffuse.y, lt.Diffuse.z, lt.Diffuse.w);
	ynTextDraw(x, y += 14, col, L"Amb = { %.2f, %.2f, %.2f, %.2f }", lt.Ambient.x, lt.Ambient.y, lt.Ambient.z, lt.Ambient.w);
	//ynTextDraw(x, y += 14, col, L"Spec = { %.2f, %.2f, %.2f, %.2f }", lt.Specular.x, lt.Specular.y, lt.Specular.z, lt.Specular.w);

	//SIMD Ÿ���� ���� ���ٹ�.
	//ynTextDraw(x, y += 14, col, L"Amb = { %.2f, %.2f, %.2f, %.2f }", XMVectorGetX(g_cbLit.Ambient), XMVectorGetY(g_cbLit.Ambient), XMVectorGetZ(g_cbLit.Ambient), XMVectorGetW(g_cbLit.Ambient));
}






/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
//
// Material Interfaces
//
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
void MaterialUpdate(MATERIAL& m, float dTime/*=0*/)
{
	//...
}




/////////////////////////////////////////////////////////////////////////////// 
//
void MaterialInfo(MATERIAL& m, int& x, int& y, COLOR& col)
{   
	//int x = 1, y = 150;
	//COLOR col(1, 1, 1, 1);
	//COLOR col2(1, 1, 0, 1); 

	//���� ���� ���.
	//y += 14;
	ynTextDraw(x, y += 14, col, L"[Material]");
	ynTextDraw(x, y += 14, col, L"Diff = { %.2f, %.2f, %.2f, %.2f }", m.Diffuse.x, m.Diffuse.y, m.Diffuse.z, m.Diffuse.w);
	ynTextDraw(x, y += 14, col, L"Amb = { %.2f, %.2f, %.2f, %.2f }", m.Ambient.x, m.Ambient.y, m.Ambient.z, m.Ambient.w);
	ynTextDraw(x, y += 14, col, L"Pow = %.2f", m.Power); 

}

 




/****************** end of file "Render.cpp" *********************************/