//
// Shader.h : �⺻ ���̴� �����ӿ� ���
//
// DirectX ���� ���� : DX11
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
						WCHAR* FileName,		//�ҽ������̸�.
						char* EntryPoint,		//�����Լ� ������.
						char* ShaderModel,		//���̴� ��.
						ID3DBlob** ppCode		//[���] �����ϵ� ���̴� �ڵ�.
					 );



//���̴� 1��.
extern ID3D11VertexShader*     g_pVS;
extern ID3D11PixelShader*      g_pPS;
extern ID3DBlob* g_pVSCode;

//���̴� 2��.
extern ID3D11VertexShader*     g_pVS2;
extern ID3D11PixelShader*      g_pPS2;
extern ID3DBlob* g_pVSCode2;

//���̴� 3��.
extern ID3D11VertexShader*     g_pVS3;
extern ID3D11PixelShader*      g_pPS3;
extern ID3DBlob* g_pVSCode3;

//���̴� 4��.
extern ID3D11VertexShader*     g_pVS4;
extern ID3D11PixelShader*      g_pPS4;
extern ID3DBlob* g_pVSCode4;


//�⺻ ��� ���� ����ü.
//�� ������ ��ü�� �⺻ ���� - ��ȯ ��.. - �� ���.
struct cbDEFAULT
{
	XMMATRIX mTM;		//"World" ��ȯ ��� : DirectXMath, 16����Ʈ ���� ����. 
	XMMATRIX mView;		//"View" ��ȯ ���
	XMMATRIX mWV;		//"World * View" ��ȯ ���
	XMMATRIX mProj;		//"Projection" ��ȯ ���
	//XMMATRIX mTex;		//"Texture-Coordinate Transform" ��ȯ ��� ���� 
	
};

//���� ������ ������� ����ü
struct cbLIGHT
{
	XMVECTOR Direction;		//���� ����.
	XMVECTOR Diffuse;		//�� ���� : Ȯ�걤 Diffuse Light.
	XMVECTOR Ambient;		//���� ���� : �ֺ��� Ambient Light.
	XMVECTOR Specular;		//���ݻ� ���� : Speuclar Light.
	FLOAT    Range;			//�� ���� �Ÿ�.
	BOOL	 DiffOn;		//���� ���뿩��.
    BOOL	 SpecOn;		//���ݻ� ���뿩��.
};

//���� ������ ������� ����ü
struct cbMATERIAL
{
	XMVECTOR Diffuse;		//Ȯ�걤 �ݻ���(%) 
	XMVECTOR Ambient;		//�ֺ��� �ݻ���(%) 
	XMVECTOR Specular;		//���ݻ� �ݻ���(%)
	FLOAT    Power;			//���ݻ� ������ : "��ĥ��" 
	BOOL	 bTexOn;		//�ؽ�ó ��뿩��.
};


//���̴� ��� ���ſ� (�ӽ�) ����.
extern cbDEFAULT	g_cbDef;		//�⺻ ������ ���ſ� : ���, ��ȯ ���� ���.. 
extern cbLIGHT		g_cbLit;		//���� ���ſ�. 
extern cbMATERIAL	g_cbMtrl;		//���� ���ſ�.

//���̴� ��� ����.
extern ID3D11Buffer*	g_pCBDef;		
extern ID3D11Buffer*	g_pCBLit;		
extern ID3D11Buffer*	g_pCBMtrl;		

//(����) ��� ���� ����.
HRESULT CreateConstantBuffer(UINT size, ID3D11Buffer** ppCB);
//���� ������� ����.
HRESULT CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB);
//���� ������� ����.
HRESULT UpdateDynamicConstantBuffer(LPDXDC pDXDC, ID3D11Resource* pBuff, LPVOID pData, UINT size);



//
// ���� �Է·��̾ƿ�.
//
extern LPINPUTLAYOUT  g_pVBLayout;	
extern LPINPUTLAYOUT  g_pVBLayout2;	
//int CreateVBLayout();
//int CreateVBLayout2();



/****************** end of file "Shader.h" ***********************************/