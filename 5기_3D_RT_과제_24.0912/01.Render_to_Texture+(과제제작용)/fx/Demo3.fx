//
//
// Demo3.fx : 라인/평면 그림자 셰이더. 
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
// < 지원기능 >
// 1. 조명 없음.
// 2. 텍스처 없음. 
// 4. 그림자/라인 셰이더.


// 기본 상수버퍼
cbuffer cbDEFAULT : register(b0)
{
    matrix mTM;     //월드 행렬. 
    matrix mView;   //뷰 변환 행렬. 
    matrix mWV;     //월드-뷰 변환 행렬. 
    matrix mProj;   //투영 변환 행렬. 
};



// 재질 상수버퍼
// 그림자의 색상을 결정. Diffuse 이외의 성분은 미사용.
// 인터페이스의 통일성을 위해 별도로 정의하지 않고 기존의 재질정보를 재활용 함.
//
cbuffer cbMATERIAL : register(b2)
{
    float4 Diffuse;     //확산광 반사율(%) 
    //float4 Ambient;   //주변광 반사율(%)
    //float4 Specular;  //정반사 반사율(%)
    //float  Power;
    //bool   bTexOn;    //텍스처 적용여부.
};



 
//VS 출력 구조체.
struct VSOutput
{
    float4 pos  : SV_POSITION;
    float4 diff : COLOR0;
   // float2 uv   : TEXCOORD0;
};





////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main : 정점 셰이더 메인 함수.
//
////////////////////////////////////////////////////////////////////////////// 

VSOutput VS_Main(
				  float4 pos : POSITION      //[입력] 정점좌표. Vertex Position (Model Space, 3D)
				)
{
	//* 아래의 테스트를 수행하기 전에  
    //* VS 에 상수 버퍼가 설정되어 있어야 합니다.    
    pos.w = 1;

	//월드 변환.(World Transform) 
    pos = mul(pos, mTM);        //pos = pos * mTM
	
    //시야-뷰 변환 (View Transform)
    pos = mul(pos, mView);

    //조명 계산 : 그림자는 조명 없음.
    //...

    //원근 투영 변환 (Projection Transform)
    pos = mul(pos, mProj);
     

    //정보 출력.
    VSOutput o = (VSOutput) 0;
    o.pos = pos; 
    o.diff = Diffuse;       //외부 지정 색으로 출력.
    //o.uv = uv;

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
				float4 diff : COLOR0        //[입력] (보간된) 픽셀별 색상 
				//float2 uv   : TEXCOORD0     //[입력] 텍스처 좌표
				) : SV_TARGET              //[출력] 색상.(필수), "렌더타겟" 으로 출력합니다.
{
    return diff;
}



/**************** end of file "Demo3.fx" ***********************/
