//
// Light.h : ���� ó�� ���
//
// DirectX ���� ���� : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once


/////////////////////////////////////////////////////////////////////////////
//
// ���� ���� Lighting 
//
// DX9 �� ���뱸��ü(D3DLIGHT9) �� ���� HW ������ �����Ͽ����ϴ�. 
// DX10 ���ʹ� ����ڰ� �ʿ��� ������ �����Ͽ� ����ؾ� �մϴ�.
// ���� ��� ���� ���̴��� ���� �������� ���� ó���ؾ� �մϴ�.
//

//���� ����ü : DX9 �� ���������, �ʿ��� ������ �����մϴ�.
struct LIGHT
{
	XMFLOAT3 Direction;		//���� ����.
	COLOR    Diffuse;		//�� ���� : Ȯ�걤 Diffuse Light.
	COLOR    Ambient;		//���� ���� : �ֺ��� Ambient Light.
	COLOR    Specular;		//���ݻ� ���� : Specular Light.
	FLOAT    Range;			//�� ���� �Ÿ�.
};



void LightsInit();
void LightsRelease();
void LightsUpdate(float dTime = 0);
void LightUpdate(LIGHT& lt, float dTime = 0);
void LightInfo(LIGHT& lt, int& x, int& y, COLOR& col);


extern LIGHT g_Light;			//���� ����.
extern BOOL  g_bDiffOn;			//Ȯ�걤 ���� On/Off.
extern BOOL  g_bSpecOn;			//���ݻ籤 ���� On/Off.

extern cbLIGHT g_cbLit;			//���� ������� ���ſ�. 



/////////////////////////////////////////////////////////////////////////////
//
// ���� ���� Material 
//
// ���� Material �� ������ ��ü�� ���� �����ϴ� ���� �����Դϴ�. 
// ���� �� ��ü���� �и�, ����ϴ� ���� ���ڽ��ϴ�. 
// DX9 �� ���뱸��ü(D3DMATERIAL9) �� ���� HW ����� ���� ������ ����Ͽ����� 
// DX10 ���ʹ� ����ڰ� �ʿ��� ������ �����Ͽ� ����ؾ� �մϴ�.
// ���� ��� ���� ���̴��� ���� �������� ���� ó���ؾ� �մϴ�.
//

//���� ����ü : DX9 �� ���������, �ʿ��� ������ �����մϴ�.
struct MATERIAL
{
	COLOR    Diffuse;		//Ȯ�걤 �ݻ���(%) 
	COLOR    Ambient;		//�ֺ��� �ݻ���(%) 
	COLOR    Specular;		//���ݻ� �ݻ���(%) 
	float    Power;			//���ݻ� ������ : "��ĥ��" 
};

//extern MATERIAL g_Mtrl;		//���� ����.
//extern cbMATERIAL g_cbMtrl;	//���� ������ۿ� ���ſ�.


void MaterialInit();
void MaterialRelease();
void MaterialUpdate(MATERIAL& m, float dTime = 0);
void MaterialInfo(MATERIAL& m, int& x, int& y, COLOR& col);

extern cbMATERIAL g_cbMtrl;			//���� ������ۿ� ���ſ�.



/****************** end of file "Light.h" ***********************************/