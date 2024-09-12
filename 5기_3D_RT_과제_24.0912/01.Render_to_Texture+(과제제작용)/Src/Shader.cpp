//
// Shader.cpp : �⺻ ������/�����ӿ� �ҽ�
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

#include "d3dcompiler.h"				//DX ���̴� �����Ϸ� ���.
#pragma comment(lib, "d3dcompiler")		//DX ���̴� �����Ϸ� ���̺귯��.  D3DCompiler.dll �ʿ�.
#include "Shader.h"


/////////////////////////////////////////////////////////////////////////////
//
//  ���� �����͵�.
//  
 

//���̴� 1�� ��ü ����.
ID3D11VertexShader*     g_pVS = nullptr;		//���� ���̴� Vertex Shader �������̽�.
ID3D11PixelShader*      g_pPS = nullptr;		//�ȼ� ���̴� Pixel Shader �������̽�.
ID3DBlob* g_pVSCode = nullptr;					//���� ���̴� ������ �ڵ� ��ü.(�ӽ�)

//���̴� 2�� ��ü ����.
ID3D11VertexShader*     g_pVS2 = nullptr; 
ID3D11PixelShader*      g_pPS2 = nullptr; 
ID3DBlob* g_pVSCode2 = nullptr;

//���̴� 3�� ��ü ����.
ID3D11VertexShader*     g_pVS3 = nullptr; 
ID3D11PixelShader*      g_pPS3 = nullptr; 
ID3DBlob* g_pVSCode3 = nullptr; 

//���̴� 4�� ��ü ����.
ID3D11VertexShader*     g_pVS4 = nullptr; 
ID3D11PixelShader*      g_pPS4 = nullptr; 
ID3DBlob* g_pVSCode4 = nullptr; 


// �⺻ ������� ���ſ� (����,�ӽù���) 
cbDEFAULT		g_cbDef;

//���̴� ��� ����.
ID3D11Buffer*	g_pCBDef = nullptr;		//�⺻���� ���� �������.


// ���� �Է·��̾ƿ� (����) : ���̴��� ������� �ʿ�. ���� ���𿡼��� ���� ��.
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
// ���̴� �¾� : ���̴� ���� ��ü/���� ����, �ʱ� 1ȸ ȣ��,
//
HRESULT ShaderLoad()
{
	HRESULT res = S_OK;


	// ���̴�1 ���� : �⺻���̴� (ǥ������+����) 
	ShaderLoad(L"./fx/Demo.fx", &g_pVS, &g_pPS, &g_pVSCode);

	// ���̴�2 ���� : �⺻���̴� (ǥ������ ��) 
	ShaderLoad(L"./fx/Demo2.fx", &g_pVS2, &g_pPS2, &g_pVSCode2);

	// ���̴�3 ���� : �׸��ڼ��̴�
	//ShaderLoad(L"./fx/Demo3.fx", &g_pVS3, &g_pPS3, &g_pVSCode3);

	// ���̴�4 ���� : ������̴�. (No.Lit + Diff.color + Diff.map)
	ShaderLoad(L"./fx/Demo4.fx", &g_pVS4, &g_pPS4, &g_pVSCode4);


	// ���� �Է±��� ����.(����,����)
	// ���̴��� ������� �ʿ�. ���� ���𿡼��� ���� ��.
	CreateVBLayout(); 
	CreateVBLayout2(); 

	
	// �⺻ ��� ���� ����.(����,����)
	ZeroMemory(&g_cbDef, sizeof(cbDEFAULT)); 
	CreateDynamicConstantBuffer(sizeof(cbDEFAULT), &g_cbDef, &g_pCBDef); 
	
	/*// ����� ������� ����. --> Light ���� ó��.
	ZeroMemory(&g_cbLit, sizeof(cbLIGHT)); 
	CreateDynamicConstantBuffer(sizeof(cbLIGHT), &g_cbLit, &g_pCBLit); 
	
	// ������ ������� ����. --> Light ���� ó��.
	ZeroMemory(&g_cbMtrl, sizeof(cbMATERIAL)); 
	CreateDynamicConstantBuffer(sizeof(cbMATERIAL), &g_cbMtrl, &g_pCBMtrl);  
	*/

	return res;
}




////////////////////////////////////////////////////////////////////////////// 
//
// ���̴� �ε�. 
//
HRESULT ShaderLoad(TCHAR* fxname, LPVERTEXSHADER* ppVS, LPPIXELSHADER* ppPS, LPXDATA* ppCode )
{	 
	//TCHAR* fxname = L"./fx/Demo.fx";

	HRESULT hr = S_OK;

	// ���� ���̴� ����.
 	ShaderLoad(fxname, "VS_Main", "vs_5_0", ppVS, ppCode);

	// �ȼ� ���̴� ����.
 	ShaderLoad(fxname, "PS_Main", "ps_5_0", ppPS); 

	return hr;
}




////////////////////////////////////////////////////////////////////////////// 
//
//  ���� ���̴� �ε�.
//
HRESULT ShaderLoad(TCHAR* fxname, char* entry, char* target, LPVERTEXSHADER* ppVS, LPXDATA* ppCode/*=NULL*/ )
{	 
	//TCHAR* fxname = L"./fx/Demo.fx";
	HRESULT hr = S_OK;

	// ���� ���̴� ������ Compile the vertex shader
	LPVERTEXSHADER pVS = nullptr;
	ID3DBlob* pVSCode = nullptr;
	hr = ShaderCompile( fxname, entry, target, &pVSCode);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"[����] ShaderLoad :: Vertex Shader ������ ����", L"Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// ���� ���̴� ��ü ���� Create a VS Object 
	hr = g_pDevice->CreateVertexShader(	pVSCode->GetBufferPointer(), 
									    pVSCode->GetBufferSize(), 
									    nullptr, 
	  								    &pVS	    
			                            );
	if (FAILED(hr))
	{
		SAFE_RELEASE(pVSCode);			//�ӽ� ��ü ����.
		return hr;
	}



	//--------------------------
	// ������ ��ü �ܺ� ����
	//--------------------------
	*ppVS = pVS;
	if (ppCode) *ppCode = pVSCode;
	return hr;

}





////////////////////////////////////////////////////////////////////////////// 
//
//  �ȼ� ���̴� �ε�.
//
HRESULT ShaderLoad(TCHAR* fxname, char* entry, char* target, LPPIXELSHADER* ppPS)
{	 
	//TCHAR* fxname = L"./fx/Demo.fx";
	HRESULT hr = S_OK;

	// �ȼ� ���̴� ������ Compile the pixel shader
	LPPIXELSHADER pPS = nullptr;
	ID3DBlob* pPSCode = nullptr;
	hr = ShaderCompile( fxname, entry, target, &pPSCode);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"[����] ShaderLoad :: Pixel Shader ������ ����", L"Error", MB_OK | MB_ICONERROR);
		return hr;
	}
	// �ȼ� ���̴� ��ü ���� Create a PS Object 
	hr = g_pDevice->CreatePixelShader(	pPSCode->GetBufferPointer(), 
										pPSCode->GetBufferSize(), 
										nullptr,
										&pPS
									 );

	SAFE_RELEASE(pPSCode);				//�ӽ� ��ü ����.	
	if (FAILED(hr))	return hr;


	//--------------------------
	// ������ ��ü �ܺ� ����
	//--------------------------
	*ppPS = pPS;

	return hr;

}







////////////////////////////////////////////////////////////////////////////// 
//
// ���̴� ����. 
//
void ShaderRelease()
{
	//���̴� 1�� ����.
	SAFE_RELEASE(g_pVS);			//���� ���̴� ����.
	SAFE_RELEASE(g_pPS);			//�ȼ� ���̴� ����.	
	SAFE_RELEASE(g_pVSCode);		//���� ���̴� ������ �ڵ� (����) ����.	
	
	//���̴� 2�� ����.
	SAFE_RELEASE(g_pVS2); 
	SAFE_RELEASE(g_pPS2); 
	SAFE_RELEASE(g_pVSCode2);
	
	//���̴� 3�� ����.
	SAFE_RELEASE(g_pVS3); 
	SAFE_RELEASE(g_pPS3); 
	SAFE_RELEASE(g_pVSCode3);

	//���̴� 4�� ����.
	SAFE_RELEASE(g_pVS4); 
	SAFE_RELEASE(g_pPS4); 
	SAFE_RELEASE(g_pVSCode4);
	

	SAFE_RELEASE(g_pCBDef);			//�⺻ ������� ����.
	//SAFE_RELEASE(g_pCBLit);		//���� ������� ����. --> Light ���� ó��.
	//SAFE_RELEASE(g_pCBMtrl);		//���� ������� ����. --> Light ���� ó��.

	SAFE_RELEASE(g_pVBLayout);		//���� �Է·��ƾƿ� ����.	
	SAFE_RELEASE(g_pVBLayout2);
}





////////////////////////////////////////////////////////////////////////////// 
//
// ���̴� �ҽ� ������ : ���̴� �ҽ�(*.fx)�� GPU �� ����� ��ȯ�մϴ�. 
//
HRESULT ShaderCompile(
						WCHAR* FileName,		//�ҽ������̸�.
						char* EntryPoint,		//�����Լ� ������.
						char* ShaderModel,		//���̴� ��.
						ID3DBlob** ppCode		//[���] �����ϵ� ���̴� �ڵ�.
						)
{
	HRESULT hr = S_OK; 
	ID3DBlob* pError = nullptr;

	//������ �ɼ�1.
	UINT Flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		//���켱 ��� ó��. ���� DX9 ���������� �������� ���. �ӵ��� �䱸�ȴٸ�, "��켱" ���� ó���� ��.
	//UINT Flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;	//��켱 ��� ó��. �ӵ��� ����� ������, ����� ��ġ �� GPU �� �����ؾ� �մϴ�.
	//UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#ifdef _DEBUG
	Flags |=  D3DCOMPILE_DEBUG;							//����� ���� �ɼ� �߰�.
#endif

	//���̴� �ҽ� ������.
	hr = D3DCompileFromFile(FileName, 
							nullptr, 
							D3D_COMPILE_STANDARD_FILE_INCLUDE, //�ܺ� ���� Include ���. 
							EntryPoint, 
							ShaderModel,
							Flags,				//������ �ɼ�1
							0,					//������ �ɼ�2,  Effect ���� �����Ͻ� �����. �̿ܿ��� ���õ�.
							ppCode,				//[���] �����ϵ� ���̴� �ڵ�.
							&pError				//[���] ������ ���� �ڵ�.
							);
	if (FAILED(hr))
	{
		//������ ����Ȯ���� ���� pError �� ����մϴ�.
		ynErrorW(TRUE, L"���̴� ������ ����", hr, pError, FileName, EntryPoint, ShaderModel );
	}
	
	SAFE_RELEASE(pError);
	return hr;
}

 



////////////////////////////////////////////////////////////////////////////// 
//
// (����) ��� ���� ����
//
HRESULT CreateConstantBuffer( UINT size, ID3D11Buffer** ppCB ) 
{	 
	HRESULT hr = S_OK;
	ID3D11Buffer* pCB = nullptr;

	//��� ���� ���� ����.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;  
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	//��� ���� ����.
	hr = g_pDevice->CreateBuffer(&bd, nullptr, &pCB);		
	if (FAILED(hr))
	{
		ynError(hr, L"CreateConstantBuffer : ������� ���� ����");		 
		return hr;
	}

	//�ܺη� ����.
	*ppCB = pCB;
	
	return hr;
}





////////////////////////////////////////////////////////////////////////////// 
//
// (����) ��� ���� ����
//
HRESULT CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB)
{
	HRESULT hr = S_OK;
	ID3D11Buffer* pCB = nullptr;

	//��� ���� ���� ����.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;				//���� �������� ����.
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPU ���� ����.

	//���긮�ҽ� ����.
	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = pData;				//(�ܺ�) ��� ������ ����.
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	//��� ���� ����.
	hr = g_pDevice->CreateBuffer(&bd, &sd, &pCB);
	if (FAILED(hr))
	{
		ynError(hr, L"CreateDynamicConstantBuffer : ���� ������� ���� ����");
		return hr;
	}

	//�ܺη� ����.
	*ppCB = pCB;

	return hr;
}




////////////////////////////////////////////////////////////////////////////// 
//
// ���� ���� ����.
//
HRESULT UpdateDynamicConstantBuffer(LPDXDC pDXDC, ID3D11Resource* pBuff, LPVOID pData, UINT size )
{
	HRESULT hr = S_OK;
	D3D11_MAPPED_SUBRESOURCE mr;
	ZeroMemory(&mr, sizeof(mr));
	
	//������� ����
	hr = pDXDC->Map( pBuff,	0, D3D11_MAP_WRITE_DISCARD,	0, &mr);
	if (FAILED(hr))
	{
		ynError(hr, L"UpdateDynamicConstantBuffer : Map ����");
		return hr;
	}

	//��� ���� ����.
	memcpy(mr.pData, pData, size);

	//������� �ݱ�.
	pDXDC->Unmap(pBuff, 0);
	  

	return hr;
}





////////////////////////////////////////////////////////////////////////////// 
//
// ���� �Է±��� ���� : ���̴��� ������� �ʿ�. ���� ���𿡼��� ���� ��.	
//
int CreateVBLayout()
{
	HRESULT hr = S_OK;	

	// ���� �Է±��� Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                    offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
	};
	UINT numElements = ARRAYSIZE(layout);

	// ���� �Է±��� ��ü ���� Create the input layout
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
// ���� �Է±��� ���� : ���̴��� ������� �ʿ�. ���� ���𿡼��� ���� ��.	
//
int CreateVBLayout2()
{
	HRESULT hr = S_OK;	

	// ���� �Է±��� Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                    offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
		//{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
	};
	UINT numElements = ARRAYSIZE(layout);

	// ���� �Է±��� ��ü ���� Create the input layout
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



