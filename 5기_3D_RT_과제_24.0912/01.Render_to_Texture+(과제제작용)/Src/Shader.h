//
// Shader.h : 기본 셰이더 프레임웍 헤더
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once

 
HRESULT ShaderLoad();
HRESULT ShaderLoad(TCHAR* fxname, LPVERTEXSHADER* ppVS, LPPIXELSHADER* ppPS, LPXDATA* ppCode);
HRESULT ShaderLoad(TCHAR* fxname, char* entry, char* target, LPVERTEXSHADER* ppPS, LPXDATA* ppCode = NULL);
HRESULT ShaderLoad(TCHAR* fxname, char* entry, char* target, LPPIXELSHADER* ppPS);

void	ShaderRelease();
HRESULT ShaderCompile(
						WCHAR* FileName,		//소스파일이름.
						char* EntryPoint,		//메인함수 진입점.
						char* ShaderModel,		//셰이더 모델.
						ID3DBlob** ppCode		//[출력] 컴파일된 셰이더 코드.
					 );



//셰이더 1번.
extern ID3D11VertexShader*     g_pVS;
extern ID3D11PixelShader*      g_pPS;
extern ID3DBlob* g_pVSCode;

//셰이더 2번.
extern ID3D11VertexShader*     g_pVS2;
extern ID3D11PixelShader*      g_pPS2;
extern ID3DBlob* g_pVSCode2;

//셰이더 3번.
extern ID3D11VertexShader*     g_pVS3;
extern ID3D11PixelShader*      g_pPS3;
extern ID3DBlob* g_pVSCode3;

//셰이더 4번.
extern ID3D11VertexShader*     g_pVS4;
extern ID3D11PixelShader*      g_pPS4;
extern ID3DBlob* g_pVSCode4;


//기본 상수 버퍼 구조체.
//각 렌더링 개체의 기본 정보 - 변환 등.. - 를 운용.
struct cbDEFAULT
{
	XMMATRIX mTM;		//"World" 변환 행렬 : DirectXMath, 16바이트 정렬 버전. 
	XMMATRIX mView;		//"View" 변환 행렬
	XMMATRIX mWV;		//"World * View" 변환 행렬
	XMMATRIX mProj;		//"Projection" 변환 행렬
	//XMMATRIX mTex;		//"Texture-Coordinate Transform" 변환 행렬 변수 
	
};

//조명 정보용 상수버퍼 구조체
struct cbLIGHT
{
	XMVECTOR Direction;		//빛의 방향.
	XMVECTOR Diffuse;		//주 광량 : 확산광 Diffuse Light.
	XMVECTOR Ambient;		//보조 광량 : 주변광 Ambient Light.
	XMVECTOR Specular;		//정반사 광량 : Speuclar Light.
	FLOAT    Range;			//빛 도달 거리.
	BOOL	 DiffOn;		//조명 적용여부.
    BOOL	 SpecOn;		//정반사 적용여부.
};

//재질 정보용 상수버퍼 구조체
struct cbMATERIAL
{
	XMVECTOR Diffuse;		//확산광 반사율(%) 
	XMVECTOR Ambient;		//주변광 반사율(%) 
	XMVECTOR Specular;		//정반사 반사율(%)
	FLOAT    Power;			//정반사 보정값 : "거칠기" 
	BOOL	 bTexOn;		//텍스처 사용여부.
};


//셰이더 상수 갱신용 (임시) 버퍼.
extern cbDEFAULT	g_cbDef;		//기본 데이터 갱신용 : 행렬, 변환 정도 등등.. 
extern cbLIGHT		g_cbLit;		//조명 갱신용. 
extern cbMATERIAL	g_cbMtrl;		//재질 갱신용.

//셰이더 상수 버퍼.
extern ID3D11Buffer*	g_pCBDef;		
extern ID3D11Buffer*	g_pCBLit;		
extern ID3D11Buffer*	g_pCBMtrl;		

//(정적) 상수 버퍼 생성.
HRESULT CreateConstantBuffer(UINT size, ID3D11Buffer** ppCB);
//동적 상수버퍼 생성.
HRESULT CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB);
//동적 상수버퍼 갱신.
HRESULT UpdateDynamicConstantBuffer(LPDXDC pDXDC, ID3D11Resource* pBuff, LPVOID pData, UINT size);



//
// 정점 입력레이아웃.
//
extern LPINPUTLAYOUT  g_pVBLayout;	
extern LPINPUTLAYOUT  g_pVBLayout2;	
//int CreateVBLayout();
//int CreateVBLayout2();



/****************** end of file "Shader.h" ***********************************/