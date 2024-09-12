//
// YnLight.cpp : 기본 렌더링/프레임웍 소스
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

#include "Light.h"


 
/////////////////////////////////////////////////////////////////////////////
//
// 조명 정보
//
BOOL  g_bDiffOn = TRUE;			//확산광 조명 On/Off.
BOOL  g_bSpecOn = TRUE;			//정반사광 조명 On/Off.

LIGHT			g_Light;				//조명 정보.
cbLIGHT			g_cbLit;					//조명 상수버퍼 갱신용. (공유,임시버퍼)
ID3D11Buffer*	g_pCBLit = nullptr;		//조명용 상수버퍼.(공유)

void LightsUpdate(float dTime);

 


/////////////////////////////////////////////////////////////////////////////
//
// 재질 정보
//
//MATERIAL		g_Mtrl;					//재질 정보.
cbMATERIAL		g_cbMtrl;				//재질 상수버퍼 갱신용.(공유,임시버퍼)
ID3D11Buffer*	g_pCBMtrl = nullptr;	//재질용 상수버퍼.(공유)





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
// 조명 갱신.
//
void LightsInit()
{ 

	//---------------- 
	// 광원 설정
	//---------------- 	 
	// 주광원 설정  
	g_Light.Direction = XMFLOAT3( 1.0f, -1.0f, 1.0f );		//빛 방향.(Normalize 하는 것을 추천. 이번 예제에서만 이렇게 사용)
	g_Light.Range = 1000.0f;								//광선의 최대 도달 거리 : Directional Light 무한대.(생략가능)
	g_Light.Diffuse = COLOR(1, 1, 1, 1);					//광량 (100% 흰색).
	g_Light.Specular = COLOR(1, 1, 1, 1);					//정반사광량 (100% 흰색).
	g_Light.Ambient = COLOR(0.2f, 0.2f, 0.2f, 1.0f);		//20% 회색 : 표준 주변광원량
		

	//---------------- 
	// 조명용 상수버퍼 생성.
	//---------------- 
	// 조명정보가 실시간으로 변하지 않는다면, 일반(정적) 상수버퍼로도 충분합니다.
	ZeroMemory(&g_cbLit, sizeof(cbLIGHT)); 
	CreateDynamicConstantBuffer(sizeof(cbLIGHT), &g_cbLit, &g_pCBLit); 
	

	//---------------- 
	// 재질 설정
	//---------------- 	 
	//... 재질은  빛에 반응하는 개체별 고유의 성질이므로 
	//... 각 개체별로 분리, 운용하는 것이 좋겠습니다. 

	//---------------- 
	// 재질용 상수버퍼 생성.
	//---------------- 
	// 재질정보가 실시간으로 변하지 않는다면, 일반(정적) 상수버퍼로도 충분합니다.
	ZeroMemory(&g_cbMtrl, sizeof(cbMATERIAL)); 
	CreateDynamicConstantBuffer(sizeof(cbMATERIAL), &g_cbMtrl, &g_pCBMtrl); 
 

	//조명 정보 갱신.
	//LightsUpdate();


}




////////////////////////////////////////////////////////////////////////////// 
//
// 조명 정보 제거. 
//
void LightsRelease()
{
	SAFE_RELEASE(g_pCBLit);			//조명 상수버퍼 제거.
	SAFE_RELEASE(g_pCBMtrl);		//재질 상수버퍼 제거.
}




//////////////////////////////////////////////////////////////////////////////// 
//
// 조명 갱신 : 조명정보가 실시간으로 변하지 않는다면, 갱신은 불필요.
//
void LightsUpdate(float dTime)
{ 
	//if(!g_bDiffOn) return;

	//렌더링 상태 결정.
	//if (IsKeyUp(VK_F5))	g_bDiffOn ^= TRUE;		
	//if (IsKeyUp(VK_F6))	g_bSpecOn ^= TRUE;		
	

	//---------------- 
	// 광원 정보 갱신.
	//---------------- 	 
	// 조명정보가 실시간으로 변하지 않는다면, 갱신할 필요 없습니다.
	// ...
	
 
	//---------------- 
	// 상수버퍼 갱신.
	//---------------- 
	XMVECTOR dir = XMLoadFloat3(&g_Light.Direction);	//방향은 normalize 되어야 합니다.
	g_cbLit.Direction = XMVector3Normalize(-dir);		//빛방향 주의.
	g_cbLit.Diffuse = XMLoadFloat4(&g_Light.Diffuse);
	g_cbLit.Ambient = XMLoadFloat4(&g_Light.Ambient);
	g_cbLit.Specular= XMLoadFloat4(&g_Light.Specular);
	g_cbLit.Range = g_Light.Range;
	g_cbLit.DiffOn = g_bDiffOn;	
	g_cbLit.SpecOn = g_bSpecOn;	
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &g_cbLit, sizeof(cbLIGHT));
   
	//---------------- 
	// 재질 설정 Material
	//---------------- 
	//... 재질은 렌더링 개체별 빛에 반응하는 고유 성질이므로 
	//... 각 개체별로 분리, 운용하는 것이 좋겠습니다. 

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
 	
	//조명 정보 출력.
	ynTextDraw(x, y += 14, col, L"[Light]");
	ynTextDraw(x, y += 14, col, L"Dir = { %.2f, %.2f, %.2f }", lt.Direction.x, lt.Direction.y, lt.Direction.z);
	ynTextDraw(x, y += 14, col, L"Diff = { %.2f, %.2f, %.2f, %.2f }", lt.Diffuse.x, lt.Diffuse.y, lt.Diffuse.z, lt.Diffuse.w);
	ynTextDraw(x, y += 14, col, L"Amb = { %.2f, %.2f, %.2f, %.2f }", lt.Ambient.x, lt.Ambient.y, lt.Ambient.z, lt.Ambient.w);
	//ynTextDraw(x, y += 14, col, L"Spec = { %.2f, %.2f, %.2f, %.2f }", lt.Specular.x, lt.Specular.y, lt.Specular.z, lt.Specular.w);

	//SIMD 타입을 사용시 접근법.
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

	//재질 정보 출력.
	//y += 14;
	ynTextDraw(x, y += 14, col, L"[Material]");
	ynTextDraw(x, y += 14, col, L"Diff = { %.2f, %.2f, %.2f, %.2f }", m.Diffuse.x, m.Diffuse.y, m.Diffuse.z, m.Diffuse.w);
	ynTextDraw(x, y += 14, col, L"Amb = { %.2f, %.2f, %.2f, %.2f }", m.Ambient.x, m.Ambient.y, m.Ambient.z, m.Ambient.w);
	ynTextDraw(x, y += 14, col, L"Pow = %.2f", m.Power); 

}

 




/****************** end of file "Render.cpp" *********************************/