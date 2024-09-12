//
// Light.h : 조명 처리 헤더
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once


/////////////////////////////////////////////////////////////////////////////
//
// 조명 정보 Lighting 
//
// DX9 은 전용구조체(D3DLIGHT9) 를 통해 HW 조명을 수행하였습니다. 
// DX10 부터는 사용자가 필요한 정보를 구성하여 운용해야 합니다.
// 실제 계산 역시 셰이더를 통해 여러분이 직접 처리해야 합니다.
//

//조명 구조체 : DX9 과 비슷하지만, 필요한 정보만 구성합니다.
struct LIGHT
{
	XMFLOAT3 Direction;		//빛의 방향.
	COLOR    Diffuse;		//주 광량 : 확산광 Diffuse Light.
	COLOR    Ambient;		//보조 광량 : 주변광 Ambient Light.
	COLOR    Specular;		//정반사 광량 : Specular Light.
	FLOAT    Range;			//빛 도달 거리.
};



void LightsInit();
void LightsRelease();
void LightsUpdate(float dTime = 0);
void LightUpdate(LIGHT& lt, float dTime = 0);
void LightInfo(LIGHT& lt, int& x, int& y, COLOR& col);


extern LIGHT g_Light;			//조명 정보.
extern BOOL  g_bDiffOn;			//확산광 조명 On/Off.
extern BOOL  g_bSpecOn;			//정반사광 조명 On/Off.

extern cbLIGHT g_cbLit;			//조명 상수버퍼 갱신용. 



/////////////////////////////////////////////////////////////////////////////
//
// 재질 정보 Material 
//
// 재질 Material 은 렌더링 개체별 빛에 반응하는 고유 성질입니다. 
// 따라서 각 개체별로 분리, 운용하는 것이 좋겠습니다. 
// DX9 은 전용구조체(D3DMATERIAL9) 를 통해 HW 조명시 재질 정보를 사용하였으나 
// DX10 부터는 사용자가 필요한 정보를 구성하여 운용해야 합니다.
// 실제 계산 역시 셰이더를 통해 여러분이 직접 처리해야 합니다.
//

//재질 구조체 : DX9 과 비슷하지만, 필요한 정보만 구성합니다.
struct MATERIAL
{
	COLOR    Diffuse;		//확산광 반사율(%) 
	COLOR    Ambient;		//주변광 반사율(%) 
	COLOR    Specular;		//정반사 반사율(%) 
	float    Power;			//정반사 보정값 : "거칠기" 
};

//extern MATERIAL g_Mtrl;		//재질 정보.
//extern cbMATERIAL g_cbMtrl;	//재질 상수버퍼용 갱신용.


void MaterialInit();
void MaterialRelease();
void MaterialUpdate(MATERIAL& m, float dTime = 0);
void MaterialInfo(MATERIAL& m, int& x, int& y, COLOR& col);

extern cbMATERIAL g_cbMtrl;			//재질 상수버퍼용 갱신용.



/****************** end of file "Light.h" ***********************************/