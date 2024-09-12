//
//
// Light.fx : 조명 셰이더 소스.
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

 
//조명 정보용 상수버퍼
struct LIGHT
{
    float3 Direction;   //빛의 방향.
    float4 Diffuse;     //주 광량 : 확산광 Diffuse Light.
    float4 Ambient;     //보조 광량 : 주변광 Ambient Light.
    float4 Specular;    //정반사 광량 : Specular Light.
    float  Range;       //빛 도달 거리.
    bool   bDiffOn;     //조명 적용여부.
    bool   bSpecOn;     //정반사 적용여부.
};

//재질 정보용 상수버퍼
struct MATERIAL
{
    float4 Diffuse;    //확산광 반사율(%) 
    float4 Ambient;    //주변광 반사율(%)
    float4 Specular;   //정반사 반사율(%)
    float  Power;      //정반사 보정값 : "거칠기" 
    bool   bTexOn;     //텍스처 사용여부.
};


//조명 정보용 상수버퍼
cbuffer cbLIGHT : register(b1)
{
    LIGHT g_Lit;
};

//재질 정보용 상수버퍼
cbuffer cbMATERIAL : register(b2)
{
    MATERIAL g_Mtrl;
};



 
// 조명 처리 함수.
float4 DiffLight(float4 nrm);
float4 DiffLight(float4 nrm, float4x4 mWV, float4x4 mView);
float4 SpecLight(float4 pos, float4 nrm);



//extern float4x4 mWV;
//extern float4x4 mV;


////////////////////////////////////////////////////////////////////////////// 
//
// 확산광 조명 계산 : 램버트 라이팅 모델 적용. Lambert Lighting Model
//                 : 뷰 공간 View Space 기준 처리.
//
/*float4 DiffLight(float4 nrm)
{
    float4 N = nrm;    N.w = 0;
    float4 L = float4(g_Lit.Direction, 0);

    //뷰공간으로 정보를 변환.
    N = mul(N, mWV);
    L = mul(L, mView);

    //조명 계산 
    float4 diff = max(dot(N, L), 0) * g_Lit.Diffuse * g_Mtrl.Diffuse;
    float4 amb = g_Lit.Ambient * g_Mtrl.Ambient;
    
    return diff + amb;
}
*/



////////////////////////////////////////////////////////////////////////////// 
//
// 확산광 조명 계산 : 램버트 라이팅 모델 적용. Lambert Lighting Model
//                 : 뷰 공간 View Space 기준 처리.
//
float4 DiffLight(float4 nrm, float4x4 mWV, float4x4 mView)
{
    float4 N = nrm;    N.w = 0;
    float4 L = float4(g_Lit.Direction, 0);

    //뷰공간으로 정보를 변환.
    N = mul(N, mWV);
    L = mul(L, mView);

    //조명 계산 
    float4 diff = max(dot(N, L), 0) * g_Lit.Diffuse * g_Mtrl.Diffuse;
    float4 amb = g_Lit.Ambient * g_Mtrl.Ambient;
    
    return diff + amb;
}




////////////////////////////////////////////////////////////////////////////// 
//
// 정반사광 조명 계산 : 블린퐁 모델 적용. Blinn-Phong Lighting Model
//                   : 뷰 공간 View Space 기준 처리.
float4 SpecLight(float4 pos, float4 nrm, float4x4 mWV, float4x4 mView)
{
    float4 N = nrm;    N.w = 0;
    float4 L = float4(g_Lit.Direction, 0);

    //뷰공간으로 정보를 변환.
    N = mul(N, mWV);
    L = mul(L, mView);     

    //시선백터 계산.
    float4 E = normalize(-pos);

    //하프벡터 계산.
    float4 H = normalize(L + E);

    //조명 계산 
    float4 spec = pow(max(dot(N, H), 0), g_Mtrl.Power) * g_Lit.Specular * g_Mtrl.Specular;
    spec.w = 1;
    
    return spec;
}







/**************** end of file "Light.fx" ***********************/
