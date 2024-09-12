//
//
// Demo2.fx : 기본 셰이더 소스.
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
// < 지원기능 >
// 1. 표준 조명.
//


// 기본 상수버퍼
cbuffer cbDEFAULT : register(b0)
{
    matrix mTM;     //월드 행렬. 
    matrix mView;   //뷰 변환 행렬. 
    matrix mWV;     //월드-뷰 변환 행렬. 
    matrix mProj;   //투영 변환 행렬. 
};



 
//VS 출력 구조체.
struct VSOutput
{
    float4 pos  : SV_POSITION;
    float4 diff : COLOR0;
    float4 spec : COLOR1;
};


 
//조명 셰이더 추가.
#include "Light.fx"




////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

VSOutput VS_Main(
				  float4 pos : POSITION,    //[입력] 정점좌표. Vertex Position (Model Space, 3D)
				  float4 nrm : NORMAL       //[입력] 노멀 normal 
				)
{
	//* 아래의 테스트를 수행하기 전에  
    //* VS 에 상수 버퍼가 설정되어 있어야 합니다.    
    pos.w = 1;

	//월드 변환.(World Transform) 
    pos = mul(pos, mTM);        //pos = pos * mTM
	
    //시야-뷰 변환 (View Transform)
    pos = mul(pos, mView);
 
    
    //조명 계산 1 (Diffuse Lighting)
    float4 diff = 1;
    if(g_Lit.bDiffOn) diff = DiffLight(nrm, mWV, mView);
 
    //조명 계산 2 (Specular Lighting)
    float4 spec = 0;
    if (g_Lit.bSpecOn) spec = SpecLight(pos, nrm, mWV, mView);

    //조명을 모두 사용하지 않으면 기본값 처리.
    //if(!g_Lit.bDiffOn && !g_Lit.bSpecOn) diff = 1;

    
    //원근 투영 변환 (Projection Transform)
    pos = mul(pos, mProj);


    //정보 출력.
    VSOutput o = (VSOutput) 0;
    o.pos = pos; 
    o.diff = diff;
	o.spec = spec;

    return o;
}






////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader 전역 데이터.
//



////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main : 픽셀 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 


float4 PS_Main(
				float4 pos  : SV_POSITION,  //[입력] (보간된) 픽셀별 좌표. (Screen, 2D)
				float4 diff : COLOR0,       //[입력] (보간된) 픽셀별 확산색. (Diffuse)
				float4 spec : COLOR1        //[입력] (보간된) 픽셀별 정반사색. ("Specular")
				) : SV_TARGET               //[출력] 색상.(필수), "렌더타겟" 으로 출력합니다.
{
	//특별한 변환 없이 입력된 정보를 그대로 출력합니다.
    //...

    //출력색 결정.
    float4 col = diff + spec;
	 
    return col;
}



/**************** end of file "Demo2.fx" ***********************/
