//
// Shader.cpp : 기본 렌더링/프레임웍 소스
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

#include "d3dcompiler.h"				//DX 셰이더 컴파일러 헤더.
#pragma comment(lib, "d3dcompiler")		//DX 셰이더 컴파일러 라이브러리.  D3DCompiler.dll 필요.
#include "Shader.h"


/////////////////////////////////////////////////////////////////////////////
//
//  전역 데이터들.
//  
 

//셰이더 1번 객체 선언.
ID3D11VertexShader*     g_pVS = nullptr;		//정점 셰이더 Vertex Shader 인터페이스.
ID3D11PixelShader*      g_pPS = nullptr;		//픽셀 셰이더 Pixel Shader 인터페이스.
ID3DBlob* g_pVSCode = nullptr;					//정점 셰이더 컴파일 코드 개체.(임시)

//셰이더 2번 객체 선언.
ID3D11VertexShader*     g_pVS2 = nullptr; 
ID3D11PixelShader*      g_pPS2 = nullptr; 
ID3DBlob* g_pVSCode2 = nullptr;

//셰이더 3번 객체 선언.
ID3D11VertexShader*     g_pVS3 = nullptr; 
ID3D11PixelShader*      g_pPS3 = nullptr; 
ID3DBlob* g_pVSCode3 = nullptr; 

//셰이더 4번 객체 선언.
ID3D11VertexShader*     g_pVS4 = nullptr; 
ID3D11PixelShader*      g_pPS4 = nullptr; 
ID3DBlob* g_pVSCode4 = nullptr; 


// 기본 상수버퍼 갱신용 (공유,임시버퍼) 
cbDEFAULT		g_cbDef;

//셰이더 상수 버퍼.
ID3D11Buffer*	g_pCBDef = nullptr;		//기본정보 운용용 상수버퍼.


// 정점 입력레이아웃 (공유) : 셰이더별 개별운용 필요. 현재 데모에서는 공유 중.
ID3D11InputLayout*  g_pVBLayout = nullptr;	
ID3D11InputLayout*  g_pVBLayout2 = nullptr;	

int CreateVBLayout();
int CreateVBLayout2();



//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//
// Shader Functions
//
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 


////////////////////////////////////////////////////////////////////////////// 
//
// 셰이더 셋업 : 셰이더 관련 객체/버퍼 생성, 초기 1회 호출,
//
HRESULT ShaderLoad()
{
	HRESULT res = S_OK;


	// 셰이더1 생성 : 기본셰이더 (표준조명+멥핑) 
	ShaderLoad(L"./fx/Demo.fx", &g_pVS, &g_pPS, &g_pVSCode);

	// 셰이더2 생성 : 기본셰이더 (표준조명 만) 
	ShaderLoad(L"./fx/Demo2.fx", &g_pVS2, &g_pPS2, &g_pVSCode2);

	// 셰이더3 생성 : 그림자셰이더
	//ShaderLoad(L"./fx/Demo3.fx", &g_pVS3, &g_pPS3, &g_pVSCode3);

	// 셰이더4 생성 : 쿼드셰이더. (No.Lit + Diff.color + Diff.map)
	ShaderLoad(L"./fx/Demo4.fx", &g_pVS4, &g_pPS4, &g_pVSCode4);


	// 정점 입력구조 생성.(전역,공유)
	// 셰이더별 개별운용 필요. 현재 데모에서는 공유 중.
	CreateVBLayout(); 
	CreateVBLayout2(); 

	
	// 기본 상수 버퍼 생성.(전역,공유)
	ZeroMemory(&g_cbDef, sizeof(cbDEFAULT)); 
	CreateDynamicConstantBuffer(sizeof(cbDEFAULT), &g_cbDef, &g_pCBDef); 
	
	/*// 조명용 상수버퍼 생성. --> Light 에서 처리.
	ZeroMemory(&g_cbLit, sizeof(cbLIGHT)); 
	CreateDynamicConstantBuffer(sizeof(cbLIGHT), &g_cbLit, &g_pCBLit); 
	
	// 재질용 상수버퍼 생성. --> Light 에서 처리.
	ZeroMemory(&g_cbMtrl, sizeof(cbMATERIAL)); 
	CreateDynamicConstantBuffer(sizeof(cbMATERIAL), &g_cbMtrl, &g_pCBMtrl);  
	*/

	return res;
}




////////////////////////////////////////////////////////////////////////////// 
//
// 셰이더 로드. 
//
HRESULT ShaderLoad(TCHAR* fxname, LPVERTEXSHADER* ppVS, LPPIXELSHADER* ppPS, LPXDATA* ppCode )
{	 
	//TCHAR* fxname = L"./fx/Demo.fx";

	HRESULT hr = S_OK;

	// 정점 셰이더 생성.
 	ShaderLoad(fxname, "VS_Main", "vs_5_0", ppVS, ppCode);

	// 픽셀 셰이더 생성.
 	ShaderLoad(fxname, "PS_Main", "ps_5_0", ppPS); 

	return hr;
}




////////////////////////////////////////////////////////////////////////////// 
//
//  정점 셰이더 로드.
//
HRESULT ShaderLoad(TCHAR* fxname, char* entry, char* target, LPVERTEXSHADER* ppVS, LPXDATA* ppCode/*=NULL*/ )
{	 
	//TCHAR* fxname = L"./fx/Demo.fx";
	HRESULT hr = S_OK;

	// 정점 셰이더 컴파일 Compile the vertex shader
	LPVERTEXSHADER pVS = nullptr;
	ID3DBlob* pVSCode = nullptr;
	hr = ShaderCompile( fxname, entry, target, &pVSCode);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"[실패] ShaderLoad :: Vertex Shader 컴파일 실패", L"Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// 정점 셰이더 객체 생성 Create a VS Object 
	hr = g_pDevice->CreateVertexShader(	pVSCode->GetBufferPointer(), 
									    pVSCode->GetBufferSize(), 
									    nullptr, 
	  								    &pVS	    
			                            );
	if (FAILED(hr))
	{
		SAFE_RELEASE(pVSCode);			//임시 개체 제거.
		return hr;
	}



	//--------------------------
	// 생성된 객체 외부 리턴
	//--------------------------
	*ppVS = pVS;
	if (ppCode) *ppCode = pVSCode;
	return hr;

}





////////////////////////////////////////////////////////////////////////////// 
//
//  픽셀 셰이더 로드.
//
HRESULT ShaderLoad(TCHAR* fxname, char* entry, char* target, LPPIXELSHADER* ppPS)
{	 
	//TCHAR* fxname = L"./fx/Demo.fx";
	HRESULT hr = S_OK;

	// 픽셀 셰이더 컴파일 Compile the pixel shader
	LPPIXELSHADER pPS = nullptr;
	ID3DBlob* pPSCode = nullptr;
	hr = ShaderCompile( fxname, entry, target, &pPSCode);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"[실패] ShaderLoad :: Pixel Shader 컴파일 실패", L"Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// 픽셀 셰이더 객체 생성 Create a PS Object 
	hr = g_pDevice->CreatePixelShader(	pPSCode->GetBufferPointer(), 
										pPSCode->GetBufferSize(), 
										nullptr,
										&pPS
									 );

	SAFE_RELEASE(pPSCode);				//임시 개체 제거.	
	if (FAILED(hr))	return hr;


	//--------------------------
	// 생성된 객체 외부 리턴
	//--------------------------
	*ppPS = pPS;

	return hr;

}







////////////////////////////////////////////////////////////////////////////// 
//
// 셰이더 제거. 
//
void ShaderRelease()
{
	//셰이더 1번 제거.
	SAFE_RELEASE(g_pVS);			//정점 셰이더 제거.
	SAFE_RELEASE(g_pPS);			//픽셀 셰이더 제거.	
	SAFE_RELEASE(g_pVSCode);		//정점 셰이더 컴파일 코드 (버퍼) 제거.	
	
	//셰이더 2번 제거.
	SAFE_RELEASE(g_pVS2); 
	SAFE_RELEASE(g_pPS2); 
	SAFE_RELEASE(g_pVSCode2);
	
	//셰이더 3번 제거.
	SAFE_RELEASE(g_pVS3); 
	SAFE_RELEASE(g_pPS3); 
	SAFE_RELEASE(g_pVSCode3);

	//셰이더 4번 제거.
	SAFE_RELEASE(g_pVS4); 
	SAFE_RELEASE(g_pPS4); 
	SAFE_RELEASE(g_pVSCode4);
	

	SAFE_RELEASE(g_pCBDef);			//기본 상수버퍼 제거.
	//SAFE_RELEASE(g_pCBLit);		//조명 상수버퍼 제거. --> Light 에서 처리.
	//SAFE_RELEASE(g_pCBMtrl);		//재질 상수버퍼 제거. --> Light 에서 처리.

	SAFE_RELEASE(g_pVBLayout);		//정점 입력레아아웃 제거.	
	SAFE_RELEASE(g_pVBLayout2);
}





////////////////////////////////////////////////////////////////////////////// 
//
// 셰이더 소스 컴파일 : 셰이더 소스(*.fx)를 GPU 용 기계어로 변환합니다. 
//
HRESULT ShaderCompile(
						WCHAR* FileName,		//소스파일이름.
						char* EntryPoint,		//메인함수 진입점.
						char* ShaderModel,		//셰이더 모델.
						ID3DBlob** ppCode		//[출력] 컴파일된 셰이더 코드.
						)
{
	HRESULT hr = S_OK; 
	ID3DBlob* pError = nullptr;

	//컴파일 옵션1.
	UINT Flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		//열우선 행렬 처리. 구형 DX9 이전까지의 전통적인 방식. 속도가 요구된다면, "행우선" 으로 처리할 것.
	//UINT Flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;	//행우선 행렬 처리. 속도의 향상이 있지만, 행렬을 전치 후 GPU 에 공급해야 합니다.
	//UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
	Flags |=  D3DCOMPILE_DEBUG;							//디버깅 모드시 옵션 추가.
#endif

	//셰이더 소스 컴파일.
	hr = D3DCompileFromFile(FileName, 
							nullptr, 
							D3D_COMPILE_STANDARD_FILE_INCLUDE, //외부 파일 Include 허용. 
							EntryPoint, 
							ShaderModel,
							Flags,				//컴파일 옵션1
							0,					//컴파일 옵션2,  Effect 파일 컴파일시 적용됨. 이외에는 무시됨.
							ppCode,				//[출력] 컴파일된 셰이더 코드.
							&pError				//[출력] 컴파일 에러 코드.
							);
	if (FAILED(hr))
	{
		//컴파일 에러확인을 위해 pError 를 출력합니다.
		ynErrorW(TRUE, L"셰이더 컴파일 실패", hr, pError, FileName, EntryPoint, ShaderModel );
	}
	
	SAFE_RELEASE(pError);
	return hr;
}

 



////////////////////////////////////////////////////////////////////////////// 
//
// (정적) 상수 버퍼 생성
//
HRESULT CreateConstantBuffer( UINT size, ID3D11Buffer** ppCB ) 
{	 
	HRESULT hr = S_OK;
	ID3D11Buffer* pCB = nullptr;

	//상수 버퍼 정보 설정.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;  
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	//상수 버퍼 생성.
	hr = g_pDevice->CreateBuffer(&bd, nullptr, &pCB);		
	if (FAILED(hr))
	{
		ynError(hr, L"CreateConstantBuffer : 상수버퍼 생성 실패");		 
		return hr;
	}

	//외부로 전달.
	*ppCB = pCB;
	
	return hr;
}





////////////////////////////////////////////////////////////////////////////// 
//
// (동적) 상수 버퍼 생성
//
HRESULT CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pCB = nullptr;

	//상수 버퍼 정보 설정.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;				//동적 정점버퍼 설정.
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPU 접근 설정.

	//서브리소스 설정.
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = pData;				//(외부) 상수 데이터 설정.
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	//상수 버퍼 생성.
	hr = g_pDevice->CreateBuffer(&bd, &sd, &pCB);
	if (FAILED(hr))
	{
		ynError(hr, L"CreateDynamicConstantBuffer : 동적 상수버퍼 생성 실패");
		return hr;
	}

	//외부로 전달.
	*ppCB = pCB;

	return hr;
}




////////////////////////////////////////////////////////////////////////////// 
//
// 동적 버퍼 갱신.
//
HRESULT UpdateDynamicConstantBuffer(LPDXDC pDXDC, ID3D11Resource* pBuff, LPVOID pData, UINT size )
{
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(mr));
	
	//상수버퍼 접근
	hr = pDXDC->Map( pBuff,	0, D3D11_MAP_WRITE_DISCARD,	0, &mr);
	if (FAILED(hr))
	{
		ynError(hr, L"UpdateDynamicConstantBuffer : Map 실패");
		return hr;
	}

	//상수 버퍼 갱신.
	memcpy(mr.pData, pData, size);

	//상수버퍼 닫기.
	pDXDC->Unmap(pBuff, 0);
	  

	return hr;
}





////////////////////////////////////////////////////////////////////////////// 
//
// 정점 입력구조 생성 : 셰이더별 개별운용 필요. 현재 데모에서는 공유 중.	
//
int CreateVBLayout()
{
	HRESULT hr = S_OK;	

	// 정점 입력구조 Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                    offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
	};
	UINT numElements = ARRAYSIZE(layout);

	// 정접 입력구조 객체 생성 Create the input layout
	hr = g_pDevice->CreateInputLayout(  layout,
										numElements,
										g_pVSCode->GetBufferPointer(),
										g_pVSCode->GetBufferSize(),
										&g_pVBLayout
									);
	if (FAILED(hr))	return hr;  

	return hr;
}

 
 
////////////////////////////////////////////////////////////////////////////// 
//
// 정점 입력구조 생성 : 셰이더별 개별운용 필요. 현재 데모에서는 공유 중.	
//
int CreateVBLayout2()
{
	HRESULT hr = S_OK;	

	// 정점 입력구조 Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                    offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
	};
	UINT numElements = ARRAYSIZE(layout);

	// 정접 입력구조 객체 생성 Create the input layout
	hr = g_pDevice->CreateInputLayout(  layout,
										numElements,
										g_pVSCode2->GetBufferPointer(),
										g_pVSCode2->GetBufferSize(),
										&g_pVBLayout2
									);
	if (FAILED(hr))	return hr;  

	return hr;
}

 



/****************** end of file "Shader.cpp" *********************************/



