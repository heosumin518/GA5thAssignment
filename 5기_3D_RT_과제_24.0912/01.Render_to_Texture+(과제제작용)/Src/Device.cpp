//
// Device.cpp : D3D ��ü �� ����̽� ����/���� �ҽ�.
//
// DirectX ���� ���� : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
#pragma warning(disable:4996)
#include "Device.h"
#include "Tchar.h"  

#include "Shader.h"

///////////////////////////////////////////////////////////////////////////////
//
// DX �� ��ġ ���� ���� �����͵�  
//
///////////////////////////////////////////////////////////////////////////////

// D3D ���� ��ü �������̽� ������.
ID3D11Device*           g_pDevice = NULL;
ID3D11DeviceContext*	g_pDXDC = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRTView = NULL;

//���� ���ٽ� ���� ����.
ID3D11Texture2D*		 g_pDS = NULL;			//����-���ٽ� ����.
ID3D11DepthStencilView*  g_pDSView = NULL;		//����-���ٽ� ��.



//��ġ ���� �⺻ ����.
DISPLAYMODE g_Mode = { 960, 600, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };		//16:10
//DISPLAYMODE g_Mode = { 1200, 600, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };		//2:1
//DISPLAYMODE g_Mode = { 1280, 720, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };		//16:9
//DISPLAYMODE g_Mode = { 1920, 1080, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9 FHD
//DISPLAYMODE g_Mode = { 3840, 2160, 0, 1, DXGI_FORMAT_R8G8B8A8_UNORM };	//16:9 UHD 4K

//AA & AF Option.
DWORD		g_dwAA = 1;		//AA ��� (�ִ� 8), AA=1 (OFF)
DWORD		g_dwAF = 8;		//Anisotropic Filter ���.(�ִ� 16)

//D3D ��� ���� (Direct3D feature level) 
//���� ����� DX ���� ����. DX ������ ��ġ�� ȣȯ�� ���
D3D_FEATURE_LEVEL g_FeatureLevels = D3D_FEATURE_LEVEL_11_0;		//DX11 ����.
TCHAR* g_strFeatureLevel = L"N/A";

//��ġ ����
DXGI_ADAPTER_DESC1 g_Adc;



//��üȭ�� ��뿩��.
BOOL g_bWindowMode = TRUE;
//��������ȭ ��뿩��.
BOOL g_bVSync = FALSE;


// �ʱ� ��ġ ���� ���� �Լ���.
//
HRESULT CreateDeviceSwapChain(HWND hwnd);
HRESULT CreateRenderTarget();
HRESULT CreateDepthStencil();
void	SetViewPort();



///////////////////////////////////////////////////////////////////////////////
//
// ������ ���� ��ü ����  
//
///////////////////////////////////////////////////////////////////////////////


BOOL g_bCullBack = FALSE;		//�޸� ����...On/Off.
BOOL g_bWireFrame = FALSE;		//���̾� ������ On/Off.
BOOL g_bZEnable = TRUE;			//���� ���� ���� On/Off.
BOOL g_bZWrite = TRUE;			//���̹��� ���� On/Off.	
 

//����
COLOR g_ClearColor(0, 0.125f, 0.3f, 1.0f);
COLOR g_ClearColor2(0.25f, 0.25f, 0.25f, 1.0);


//
// ������ ���� ��ü ���� / ��� �Լ���.
//
int  RenderStateObjectCreate();
void RenderStateObjectRelease();
void RenderStateUpdate();



//
// ����/���ٽ� ���� ��ü�� : ���� ��ü ������.
//
ID3D11DepthStencilState* g_DSState[DS_MAX_];

int  DepthStencilStateCreate();
void DepthStencilStateRelease();



// 
// �����Ͷ����� ���� ��ü Rasterizer State Objects
// 
ID3D11RasterizerState*	g_RState[RS_MAX_] = { NULL, };

void RasterStateCreate();
void RasterStateRelease();
void RenderModeUpdate();


//
// ������ ��� : �ټ��� ������ ��� ���� �� ����� ���� ����.
//
 
DWORD g_RMode = RM_DEFAULT;		//'����' ������ ���.



//
// ����/���� ȥ�� ���� ��ü Blending State Objects 
//
ID3D11BlendState* g_BState[BS_MAX_] = { nullptr, };

void BlendStateCreate();
void BlendStateRelease();
void BlendModeUpdate();



//
// �ؽ�ó ���÷�.
//
ID3D11SamplerState* g_pSampler[SS_MAX] = { nullptr, };
void SamplerCreate();
void SamplerRelease();






 

///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////// 

///////////////////////////////////////////////////////////////////////////// 
//
// DX �� ���� ��ü �ʱ�ȭ : ���ø����̼� ���۽� 1ȸ ȣ��.
//
// 1.����̽� ���� 
// 2.����ü�� �� ����Ÿ�� ����.
// 3.����/���ٽ� ���� ����.
// 4.��Ÿ ������ ���� ��ġ ����.
//
int DXSetup(HWND hwnd)
{
	
	//----------------------------------------
	// 1�ܰ� : ������ ��ġ ���� �ʼ� ����
	//----------------------------------------
	// D3D ������ ��ġ Device �� ����ü�� Swap Chain ����. 
	CreateDeviceSwapChain(hwnd);

	// ��ġ-����ü���� ����Ÿ��(�����) ȹ��
	CreateRenderTarget();
	
	// ����/���ٽ� ���� ����.
	CreateDepthStencil();
	
	// ��ġ ��º��ձ�(Output Merger) �� ���͸� Ÿ�� �� ����-���ٽ� ���� ���.
	g_pDXDC->OMSetRenderTargets(
				1,				// ����Ÿ�� ����.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
				&g_pRTView,		// ����Ÿ��("�����") ���.	
				g_pDSView		// ����/���ٽ� ���� ���.
				);
	
	// ����Ʈ ����.
	SetViewPort();


	//��ġ ���� ȹ��
	GetDeviceInfo();


	//----------------------------------------
	// 2�ܰ� : ��Ÿ ������ ���� �߰� ����.
	//----------------------------------------
	// �������� �ʿ��� ����� ��ü���� ����/���� �մϴ�.
	// ī�޶�, ����, ��Ʈ, ���̴� ���...
	
	//������ ���� ��ü ����.
	RenderStateObjectCreate(); 


	// ��Ʈ ����.--> Yena �� ���Ե�.
	//ynFontCreate(g_pDevice);
	 
	// Yena �¾� : �ý��� ���� ��ü ���� 
	// ���� �⺻ ī�޶�, ����, ��Ʈ, ������, �׸���, ���..
	YenaSetup(g_pDevice, g_Mode, 5.0f);
	
	 
	//�۾� �Ϸ�, ��ġ �غ� �Ϸ�.
	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////// 
//
// ����̽� ��ü �� ���� ��ġ ���� : ����� 1ȸ ȣ��.  
// �⺻ ������ ��ü�� ���Ž� ������ �������� ó���մϴ�.(����)
//
void DXRelease()
{ 	 
	//��ġ ���� ���� : ���� ���� �ʱ�ȭ�� �ؾ� �մϴ�. (�޸� ���� ����)
	if (g_pDXDC) g_pDXDC->ClearState();
	//if (g_pSwapChain) g_pSwapChain->SetFullscreenState(false, NULL);

	//���� ����.
	YenaRelease(); 
	//ynFontRelease();					//��Ʈ ����.--> Yena �� ���Ե�.

	//���� ��ü ����.
	RenderStateObjectRelease();


	SAFE_RELEASE(g_pDS);				//����/���ٽ� ���� ����.
	SAFE_RELEASE(g_pDSView);			
	SAFE_RELEASE(g_pRTView);			//����Ÿ�� ����.
	SAFE_RELEASE(g_pSwapChain);			//����ü�� ����.
	SAFE_RELEASE(g_pDXDC);
	SAFE_RELEASE(g_pDevice);			//����̽� ����. �� ���߿� �����մϴ�.
}
 




/////////////////////////////////////////////////////////////////////////////
//
// D3D ������ '��ġ(Device)' �� ����ü�� ����. 
//
HRESULT CreateDeviceSwapChain(HWND hwnd)
{
	HRESULT hr = S_OK;

	// ��ġ (Device) �� ����ü��(SwapChain) ���� ����.
	// ����ü���� ���߹��۸� �ý����� ���ϸ�
	// �������� '�ø���Flipping' ü�ΰ� ������ �ǹ��Դϴ�.  
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Windowed = g_bWindowMode;				//Ǯ��ũ�� �Ǵ� â��� ����.
	sd.OutputWindow = hwnd;						//����� ������ �ڵ�.
	sd.BufferCount = 1;							//����� ����.
	sd.BufferDesc.Width = g_Mode.Width;			//�ػ� ����.(����� ũ��)
	sd.BufferDesc.Height = g_Mode.Height;
	sd.BufferDesc.Format = g_Mode.Format;		//����� ����԰� (A8R8G8B8) â��忡���� ���� ���� 
	sd.BufferDesc.RefreshRate.Numerator = g_bVSync ? 60 : 0;   //���� ������.(��������ȭ VSync Ȱ��ȭ�� ǥ�ذ����� ���� : 60hz)
	//sd.BufferDesc.RefreshRate.Numerator = 0;	//���� ������.(��������ȭ VSync Off)
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�뵵 ����: '����Ÿ��' 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//sd.Flags = 0;
	sd.SampleDesc.Count = g_dwAA;		//AA ����
	sd.SampleDesc.Quality = 0;
	
	
	
	// D3D ������ '��ġ(Device)' �� ����ü�� ����. 
	//
	hr = D3D11CreateDeviceAndSwapChain(
			NULL,						//���� ��� ������ (�⺻��ġ�� NULL)
			D3D_DRIVER_TYPE_HARDWARE,	//HW ���� 
			NULL,						//SW Rasterizer DLL �ڵ�.  HW ���ӽÿ��� NULL.
			0,							//����̽� ���� �÷���.(�⺻��)
			//D3D11_CREATE_DEVICE_DEBUG,//����̽� ���� �÷���.(�����)
			&g_FeatureLevels,			//(������) ����̽� ��� ����(Feature Level) �迭
			1,							//(������) ����̽� ��� ����(Feature Level) �迭 ũ��.
			D3D11_SDK_VERSION,			//DX SDK ����.
			&sd,						//����̽� ���� �� �ɼ�.
			&g_pSwapChain,				//[���] ����ü�� �������̽� ���.
			&g_pDevice,					//[���] ����̽� �������̽� ���.
			NULL,						//[���] (������) ����̽� ��� ����. �ʿ���ٸ� NULL ����. 
			&g_pDXDC					//[���] ����̽� ���ؽ�Ʈ ���.
	);
	if (FAILED(hr))
	{
		ynError(hr, L"����̽� / ����ü�� ���� ����.");
	}
	
	return hr;
}





/////////////////////////////////////////////////////////////////////////////
//
// ��ġ-����ü���� ����Ÿ��(�����) ȹ��
//
// ���ҽ���(Resource View)  
// DX11 �� ���ҽ��� "����" �������� �����ϰ� ����Ǿ� �ֽ��ϴ�. 
// (�޸� ������ ȿ���� ���� �� �ٿ뵵 Ȱ���� ����..)
// ���� ����� �� �뵵�� �´� ���ٹ��(�������̽�)�� ȹ��(����)�ؾ� �մϴ�.
// �� �۾��� "���ҽ� �� Resource View" �� ���� ó���մϴ�.
//                     
// ����ü�ο� ����(Bind)�� ������ ��¹��� - "����� BackBuffer" �� 2D ��� ���ҽ�(�ؽ�ó) �̸�
// "����Ÿ�� RenderTarget" ������ ���ҽ��並 ����, �ش� ���۸� ����/��� �ϰڽ��ϴ�.
//
HRESULT CreateRenderTarget()
{
	HRESULT hr = S_OK;

	// ������ ȹ��.
	ID3D11Texture2D *pBackBuffer;
	hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);		// ����� ���, 2D �ؽ��� ����, ȭ����¿�
	if (FAILED(hr))	return hr;

	//ȹ���� ����ۿ� ����Ÿ�� �� ����.(����Ÿ��'��'���� ������)
	hr = g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRTView);		// '����Ÿ�ٺ�'�� ����.	
	if (FAILED(hr)) 
	{
		ynError(hr, L"�����-����Ÿ�ٺ� ���� ����.");
		//return hr;
	}

	//���ҽ� �� ���� ��, ���ʿ��� DX �ڵ��� �����ؾ� �մϴ�.(�޸� ���� ����)
	SAFE_RELEASE(pBackBuffer);

	return hr;
}





/////////////////////////////////////////////////////////////////////////////
//
// ����-���ٽǹ��� ����. : DX11 ������ ����������-����Ÿ�ٺ�- �� ����������, 
//                       ����-���ٽǹ��ۿ��� ���� ������ �մϴ�.
//                       ����̽��� ��ϵ� ���� �����Դϴ�.
//
HRESULT CreateDepthStencil()
{
	HRESULT hr = S_OK;

	//����/���ٽ� ���ۿ� ���� ����.
	D3D11_TEXTURE2D_DESC   td;					
	ZeroMemory(&td, sizeof(td));
	td.Width = g_Mode.Width;
	td.Height = g_Mode.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	//td.Format = DXGI_FORMAT_D32_FLOAT;			// 32BIT. ���� ����.
	//td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	// ���� ���� (24bit) + ���ٽ� (8bit) / ���� �ϵ���� (DX9)
	td.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;	// ���� ���� (32bit) + ���ٽ� (8bit) / ���� �ϵ���� (DX11)
	td.SampleDesc.Count = g_dwAA;				// AA ���� - RT �� ���� �԰� �ؼ�.
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;	// ����-���ٽ� ���ۿ����� ����.
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	// ���� ���� ����.
	//ID3D11Texture2D* pDS = NULL;						 
	hr = g_pDevice->CreateTexture2D(&td, NULL, &g_pDS);	
	if (FAILED(hr))
	{
		ynError(hr, L"����/���ٽ� ���ۿ� CreateTexture ����.");		 
		return hr;
	}
	

	// ����-���ٽǹ��ۿ� ���ҽ� �� ���� ����. 
	D3D11_DEPTH_STENCIL_VIEW_DESC  dd;
	ZeroMemory(&dd, sizeof(dd));
	dd.Format = td.Format;
	dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; //AA ����.
	//dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; //AA ����.
	dd.Texture2D.MipSlice = 0;

	//����-���ٽ� ���� �� ����.
	hr = g_pDevice->CreateDepthStencilView(g_pDS, &dd, &g_pDSView);
	if (FAILED(hr))
	{
		ynError(hr, L"����/���ٽǺ� ���� ����.");		 
		return hr;
	}

	//���ҽ� �� ���� ��, ���ʿ��� DX �ڵ��� �����ؾ� �մϴ�.(�޸� ���� ����)
	//SAFE_RELEASE(pDS);
		
	return hr;
}




/////////////////////////////////////////////////////////////////////////////
//
// ����Ʈ ���� :  DX11 ������ �⺻ó�� ���� ������ ����ڰ� ���� �����ؾ��մϴ� 
//			     (OpenGL ������ �������� �̷��� �� �Խ��ϴ�.)
//
void SetViewPort()
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)g_Mode.Width;
	vp.Height = (FLOAT)g_Mode.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_pDXDC->RSSetViewports(1, &vp);

}




/////////////////////////////////////////////////////////////////////////////
//
// ��ġ-����ü��/����Ÿ��/����/���ٽ� ���� Ŭ���� 
//
int ClearBackBuffer(UINT flag, COLOR col, float depth, UINT stencil)
{
	g_pDXDC->ClearRenderTargetView(g_pRTView, (float*)&col);			//����Ÿ�� �����.
	g_pDXDC->ClearDepthStencilView(g_pDSView, flag, depth, stencil);	//����/���ٽ� �����.
	
	return YN_OK;
}



/////////////////////////////////////////////////////////////////////////////
//
// ��ġ-����ü�� ��ȯ : ��� �׸��� �Ϸ�� �� ȣ���ؾ� ��.
//
int Flip()
{
	g_pSwapChain->Present(g_bVSync, 0);			//ȭ����� : Flip! (+��������ȭ)

	return YN_OK;
}








////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// ��ġ ������ ���� ��ü ����.
//
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//----------------------------------------------------------------------------
// ���� ��ü State Objects (DX10/11)
// DX10 ���� ���� TnL �� RenderState �� ���ŵǾ����ϴ�.
// �̸� ����ϴ� ���� ���°�ü State Objects �������̽���, ������ ���¸� �ϳ��� �׷����� 
// ���� �������� ����̽��� �����մϴ�.  �̸� ���� ��ġ�� ��� ���� ��ȭ�� �ѹ��� �����Ͽ� 
// ���ʿ��� �������(Overhead) �� ���̰� ���� ȿ������ �������� ������ �մϴ�.
//
// ���°�ü�� ���� �ʱ�� ������ ����ϱ⸦ �����ϸ� ����� ����(Release) �ؾ� �մϴ�.
// ���°�ü�� �����Ұ���(Immutable, �б�����) ��ü �Դϴ�.
// DX9 ������ State-Block �� �̿� ������ ����� ����߾����ϴ�.
//
// ���� ��ü �������̽� �� '�����Ͷ����� �������� Rasterize Stage' ���� ������ 
// ID3D11RasterizerState �������̽��� ���� ó���մϴ�.  
// �����ϰ� ������ ����/��� ��� ������ �����սô�.  �ڼ��� ���� ������ �����Ͻʽÿ�. 
// ��ũ1 : ���� ��ü https://msdn.microsoft.com/en-us/library/windows/desktop/bb205071(v=vs.85).aspx
// ��ũ2 : ���̹��� ���� �����ϱ� https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb205074(v=vs.85).aspx#Create_Depth_Stencil_State 
//----------------------------------------------------------------------------
//
int RenderStateObjectCreate()
{

	//����/���ٽ� ���� ��ü ����.
	DepthStencilStateCreate();

	//������ ���� ��ü ����.
	RasterStateCreate();

	//���÷� ���� ��ü ����.
	SamplerCreate();

	//���� ���� ��ü ����.
	BlendStateCreate();


	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//  ���°�ü �����ϱ� : ���� ����� 1ȸ ȣ��.
//
void RenderStateObjectRelease()
{
	DepthStencilStateRelease();		//����/���ٽ� ���� ��ü ����.
	RasterStateRelease();			//������ ���� ��ü ����.
	SamplerRelease();				//���÷� ���� ��ü ����.
	BlendStateRelease();			//���� ���� ��ü ����.
}





///////////////////////////////////////////////////////////////////////////////
//
//  ������ ���� �����ϱ�. 
//
void RenderStateUpdate()
{
	// ������ �ɼ� ���� 	 
	if (IsKeyUp(VK_SPACE))	g_bWireFrame ^= TRUE;
	if (IsKeyUp(VK_F4))		g_bCullBack ^= TRUE;
	
	//if (IsKeyUp(VK_F5))		g_bDiffOn ^= TRUE;		
	//if (IsKeyUp(VK_F6))		g_bSpecOn ^= TRUE;		
	//if (IsKeyUp(VK_F5))		g_bZEnable ^= TRUE;		
	//if (IsKeyUp(VK_F6))		g_bZWrite ^= TRUE;		

	// ���� ����.
	if (g_bWireFrame) g_ClearColor = COLOR(0.15f, 0.15f, 0.15f, 1.0f);
	else			  g_ClearColor = COLOR(0, 0.125f, 0.3f, 1.0f); 
	
	// ������ ��� ��ȯ.	  
	RenderModeUpdate();
	 	
	// ���� ���� ��� ��ȯ.	 
	if (g_bZEnable)
	{
		if (g_bZWrite)  
			  g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON], 0);			//���� ���� ���� (�⺻��)
		else  g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_WRITE_OFF], 0);	//���� ����  : Z-Test On + Z-Write Off.
	}
	else  g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF], 0);	//���� ���� ��Ȱ��ȭ : Z-Test Off + Z-Write Off.
	 

	//���÷� ����.(�⺻����)
	g_pDXDC->PSSetSamplers(0, 1, &g_pSampler[SS_DEFAULT]); 

}






/////////////////////////////////////////////////////////////////////////////// 
//
// ������ ��� üũ : �� ������ ��� ���º� On/Off ó��.
//
#define CheckRMode(k, v) if((k)) g_RMode |= (v); else g_RMode &= ~(v);


/////////////////////////////////////////////////////////////////////////////// 
//
// ������ ��� ��ȯ
//
void RenderModeUpdate()
{	
	// ������ ��� üũ : ����ڰ� ������ ������ ���� ����.
	CheckRMode(g_bCullBack, RM_CULLBACK);
	CheckRMode(g_bWireFrame, RM_WIREFRAME);


	// ������ ��� ��ȯ : ������ ����� ������ ���� ������ ���¸� ����.
	switch (g_RMode)
	{
	default:
	case RM_SOLID:
		g_pDXDC->RSSetState(g_RState[RS_SOLID]);
		break;
	case RM_WIREFRAME:
		g_pDXDC->RSSetState(g_RState[RS_WIREFRM]);
		break;
	case RM_CULLBACK:
		g_pDXDC->RSSetState(g_RState[RS_CULLBACK]);
		break;
	case RM_WIREFRAME | RM_CULLBACK:
		g_pDXDC->RSSetState(g_RState[RS_WIRECULLBACK]);
		break;
	} 

}














///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//  ����/���ٽ� ���� ���°�ü �����ϱ�
//
int DepthStencilStateCreate()
{
	
	//----------------------------
	// ����/���ٽ� ���� ��ü ����.: "��º��ձ� Output Merger" ���� ����. 
	//----------------------------
	//...	 
	D3D11_DEPTH_STENCIL_DESC  ds;
	//���� ���� ���� (�⺻��)
	ds.DepthEnable = TRUE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS;
	//���ٽ� ���� ���� (�⺻��) 
	ds.StencilEnable = FALSE;								//���ٽ� ���� OFF.
	ds.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;	//���ٽ� �б� ����ũ (8bit: 0xff)
	ds.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK; //���ٽ� ���� ����ũ (8bit: 0xff)
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;	//[�ո�] ���ٽ� �� �Լ� : "Always" ��, �׻� ���� (���, pass)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;	//[�ո�] ���ٽ� �� ������ ���� : ������ ����.
	ds.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;	//[�ո�] ���ٽ� �� ���н� ���� : ������ ����.	
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//[�ո�] ���ٽ�/���� �� ���н� ���� : ������ ����.
	ds.BackFace = ds.FrontFace;									//[�޸�] ���� ����. �ʿ�� ���� ������ ����.
 
	
	//----------------------------------------------------------------------
	// ���� ���� ���� ��ü��.
	//----------------------------------------------------------------------
	// DS ���� ��ü #0 : Z-Test ON! (�⺻��)
	ds.DepthEnable = TRUE;					//���� ���� On.
	ds.StencilEnable = FALSE;				//���ٽ� ���� Off!
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_ON]);

	// DS ���� ��ü #1 : Z-Test OFF ����.
	ds.DepthEnable = FALSE;
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_OFF]);

	// DS ���� ��ü #2 : Z-Test On + Z-Write OFF.
	// Z-Test (ZEnable, DepthEnable) �� ������, Z-Write ���� ��Ȱ��ȭ �˴ϴ�.
	ds.DepthEnable = TRUE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;		//���̰� ���� ��.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_WRITE_OFF]);


	//----------------------------------------------------------------------
	// ���ٽ� ���� ���� ��ü�� ����.
	//----------------------------------------------------------------------
	// ���ٽ� ���� ��Ʈ ���� ����.
	// (Stencil.Ref & Stencil.Mask) Comparison-Func ( StencilBuffer.Value & Stencil.Mask)
	//
	// *StencilBufferValue : ���� �˻��� �ȼ��� ���ٽǰ�.
	// *ComFunc : �� �Լ�. ( > < >= <= ==  Always Never)
	//----------------------------------------------------------------------
	// DS ���°�ü #4 :  ���̹��� On, ���ٽǹ��� ON (�׻�, ������ ����) : "����/���ٽ� ���" 
	ds.DepthEnable	  = TRUE;										//���̹��� ON! (�⺻��)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc	  = D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//���Լ� : "�׻� ���" (����)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//������ : ������(Stencil Reference Value) �� ��ü.
	//ds.FrontFace.StencilFailOp	  = D3D11_STENCIL_OP_KEEP;		//���н� : ����.(�⺻��, ����)
	//ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//���н� : ����.(�⺻��, ����)
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_ON_STENCIL_ON]);
	

	// DS ���°�ü #5 : ���̹��� On, ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���� ��ġ���� �׸���" 
	//ds.DepthEnable	= TRUE;										//���̹��� ON! (�⺻��)(����)
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//���Լ� : "�����Ѱ�?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;		//������ : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_ON_STENCIL_EQUAL_KEEP]);


	// DS ���°�ü #6 : ���̹��� On, ���ٽǹ��� ON (�ٸ���, ������ ����) : "���� ��ġ �̿ܿ� �׸���" 
	//ds.DepthEnable	= TRUE;										//���̹��� ON! (�⺻��)(����)
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON!
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_NOT_EQUAL;	//���Լ� : "���� ������?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;		//������ : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP]);


	/*// DS ���°�ü #7 : ���̹��� Off, ���ٽǹ��� ON (������ ����) : "���ٽǸ� ���" 
	ds.DepthEnable	  = FALSE;										//���̹��� OFF!
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//���Լ� : "�׻� ���" (����)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//������ : ������(Stencil Reference Value) �� ��ü.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_OFF_STENCIL_ON]);
	*/
	
	/*// DS ���°�ü #8 : ���̹��� On, ���ٽǹ��� ON (���Ϻ�, ���� ����) : "���߱׸��� ������" 
	//ds.DepthEnable	= TRUE;										//���̹��� ON! (�⺻��)(����)
	//ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//ds.DepthFunc		= D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//���Լ� : "�����Ѱ�?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_INCR;		//������ : ���� (+1) 
	ds.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;		//���н� : ����.
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//���н� : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_ON_STENCIL_EQUAL_INCR]);
	*/

	// DS ���°�ü #9 : ���̹��� On, ���ٽǹ��� ON (�׻�, ������ ����) : "���ٽǸ� ���" 
	ds.DepthEnable	  = TRUE;										//���̹��� ON! (�⺻��)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				//���̹��� ���� OFF.
	ds.DepthFunc	  = D3D11_COMPARISON_LESS;						//���̿��� ON. (�⺻��)
	ds.StencilEnable  = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//���Լ� : "�׻� ���" (����)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//������ : ������(Stencil Reference Value) �� ��ü.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_WRITE_OFF_STENCIL_ON]);
	

	//���̹��� Off (Write Off), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���߱׸��� ����.
	ds.DepthEnable	= FALSE;										//�����׽�Ʈ Off!
	//ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				//���̰� ���� Off.
	//ds.DepthFunc		= D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//���Լ� : "�����Ѱ�?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_INCR;		//������ : ���� (+1) 
	ds.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;		//���н� : ����.
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//���н� : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	g_pDevice->CreateDepthStencilState(&ds, &g_DSState[DS_DEPTH_OFF_STENCIL_EQUAL_INCR]);


	return YN_OK;
}




///////////////////////////////////////////////////////////////////////////////
//
//  ���°�ü �����ϱ� : ���� ����� 1ȸ ȣ��.
//
void DepthStencilStateRelease()
{
	for (int i = 0; i < DS_MAX_; i++)
		SAFE_RELEASE(g_DSState[i]);
}






////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
// 
// ������ ���� ��ü ����.
//
void RasterStateCreate()
{
	//[���°�ü 1] �⺻ ������ ���� ��ü.
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;		//�ﰢ�� ���� ä���.(�⺻��)
	rd.CullMode = D3D11_CULL_NONE;		//�ø� ����. (�⺻���� �ø� Back)		
	rd.FrontCounterClockwise = false;   //���� �⺻��...
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = true;		//AA ����.
	rd.AntialiasedLineEnable = false;
	//�����Ͷ����� ���� ��ü ����.
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_SOLID]);


	//[���°�ü2] ���̾� ������ �׸���. 
	rd.FillMode = D3D11_FILL_WIREFRAME; 
	rd.CullMode = D3D11_CULL_NONE;
	//�����Ͷ����� ��ü ����.
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_WIREFRM]);

	//[���°�ü3] �ĸ� �ø� On! "CCW"
	rd.FillMode = D3D11_FILL_SOLID; 
	rd.CullMode = D3D11_CULL_BACK;
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_CULLBACK]);
	
	//[���°�ü4] ���̾� ������ + �ĸ��ø� On! "CCW"
	rd.FillMode = D3D11_FILL_WIREFRAME; 
	rd.CullMode = D3D11_CULL_BACK; 
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_WIRECULLBACK]);

	//[���°�ü5] ���� �ø� On! "CW"
	rd.FillMode = D3D11_FILL_SOLID; 
	rd.CullMode = D3D11_CULL_FRONT;
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_CULLFRONT]);
	
	//[���°�ü6] ���̾� ������ + �����ø� On! "CW" 
	rd.FillMode = D3D11_FILL_WIREFRAME; 
	rd.CullMode = D3D11_CULL_FRONT;
	g_pDevice->CreateRasterizerState(&rd, &g_RState[RS_WIRECULLFRONT]);
	

}




////////////////////////////////////////////////////////////////////////////// 
//
// ������ ���� ��ü ���� : ���ø����̼� ����� ȣ��.
//
void RasterStateRelease()
{
	for (int i = 0; i<RS_MAX_; i++)
	{
		SAFE_RELEASE(g_RState[i]);
	}

}






////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
// 
// ���� ���� ��ü ����.
//
void BlendStateCreate()
{
	
	//----------------------------------------------------------------
	// [�˸�] ���� ����Ÿ��(RenderTarget) ���� �� RT �� ������ �����ϳ�
	// ��а� RT #0 (�⺻)-�����-���� ����ϰڽ��ϴ�.
	//----------------------------------------------------------------
	
	//------------------------------
	// ȥ�հ�ü1 : ����ȥ�� ����. "Blending OFF"
	//------------------------------
	//���� ���� ��ü ���� �ɼ� : ���� �⺻����.
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	bd.RenderTarget[0].BlendEnable = FALSE;							//���� ���� ����. �⺻���� FALSE(OFF)		
	//���� ���� ȥ�� : Color Blending.(�⺻��)
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//���� ȥ�� ����(Color - Operation), �⺻���� ���� : ������ = Src.Color + Dest.Color 	
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//�ҽ�(����) ȥ�� ����, ���� 100% : Src.Color = Src * 1;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;				//Ÿ��(���) ȥ�� ����, ��� 0%   : Dest.Color = Dest * 0;  ��� 0�� RT �� "�����"�� �ǹ��մϴ�.	
 	//���� ���� ȥ�� : Alpha Blending.(�⺻��)
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			//���� ȥ�� �Լ�(Alpha - Opertion), �⺻���� ����.
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;				//�ҽ�(����) ���� ȥ�� ����.
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//Ÿ��(���) ���� ȥ�� ����.
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//����Ÿ�ٿ� ���� �ɼ�.
	//bd.AlphaToCoverageEnable = FALSE;								//���� �߰� �ɼ�.(�⺻��, ����)
	//bd.IndependentBlendEnable = FALSE;
	g_pDevice->CreateBlendState(&bd, &g_BState[BS_DEFAULT]);		//���� ��ü.����.
	

	//------------------------------
	// ȥ�հ�ü2 : ���� ȥ�� "Alpha Blending"
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//���� ȥ�� ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//���� ȥ�� ���� (����, �⺻��) : ������ = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//�ҽ�(����) ȥ�� ����, ���� ���ĺ���  : Src.Color = Src * Src.a;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		//Ÿ��(���) ȥ�� ����, ���� ���ĺ��� ���� : Dest.Color = Dest * (1-Src.a);			 		
	g_pDevice->CreateBlendState(&bd, &g_BState[BS_ALPHA_BLEND]);

		
	//------------------------------
	// ȥ�հ�ü3 : ���� ȥ�� "Color Blending"
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//���� ȥ�� ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//���� ȥ�� ���� (����, �⺻��) : ������ = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;			//�ҽ�(����) ȥ�� ����, ���� ����  : Src.Color = Src * Src.Color;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;		//Ÿ��(���) ȥ�� ����, ���� ���� ���� : Dest.Color = Dest * (1-Src.Color);			 	
	g_pDevice->CreateBlendState(&bd, &g_BState[BS_COLOR_BLEND]);
	

	//------------------------------
	// ȥ�հ�ü3 : ���� ȥ�� "��� ���� Blending 1:1" 
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//���� ȥ�� ON! 	
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//���� ȥ�� ���� "��Ⱝ��" : ������ = Src.Color + Dest.Color 		
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//�ҽ�(����) ȥ�� ����, ���� ����  : Src.Color = Src * 1;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;					//Ÿ��(���) ȥ�� ����, ���� ���� ���� : Dest.Color = Dest * 1;			 	
	g_pDevice->CreateBlendState(&bd, &g_BState[BS_COLOR_BLEND_ONE]);

}




////////////////////////////////////////////////////////////////////////////// 
//
// ���� ���� ��ü ���� : ���ø����̼� ����� ȣ��.
//
void BlendStateRelease()
{
	for (int i = 0; i<BS_MAX_; i++)
	{
		SAFE_RELEASE(g_BState[i]);
	}

}




 


////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
// 
// �ؽ�ó ���÷� ���� ��ü ����.
//
void SamplerCreate()
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));		

	//�ؽ�ó ���� : ���� ���͸�
	sd.Filter = D3D11_FILTER_ANISOTROPIC; 
	sd.MaxAnisotropy = g_dwAF; 
	//�Ӹ���	
	sd.MinLOD = 0;							
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	//���� �⺻�� ó��..
 	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.BorderColor[0] = 1;
	sd.BorderColor[1] = 1;
	sd.BorderColor[2] = 1;
	sd.BorderColor[3] = 1;
	
	//���÷� ��ü1 ����. (DX �⺻��)
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;		
	hr = g_pDevice->CreateSamplerState(&sd, &g_pSampler[SS_CLAMP]);

	//���÷� ��ü2 ����.
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = g_pDevice->CreateSamplerState(&sd, &g_pSampler[SS_WRAP]);
	
}




////////////////////////////////////////////////////////////////////////////// 
// 
// �ؽ�ó ���÷� ���� ��ü ����.
//
void SamplerRelease()
{
	for(int i=0; i<SS_MAX; i++)
		SAFE_RELEASE(g_pSampler[i]); 
}











///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// ���� Ÿ�̸� �Լ���
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// �ʴ� �����Ӽ� ���.
//
// ���� : int x, y : ����� ȭ�� ��ǥ.
//
void PutFPS(int x, int y)
{
	static int oldtime = GetTickCount();
	int nowtime = GetTickCount();

	static int frmcnt = 0;
	static float fps = 0.0f;

	++frmcnt;
	
	int time = nowtime - oldtime;
	if( time >= 999)
	{
		oldtime = nowtime;

		fps = (float)frmcnt*1000/(float)time;
		frmcnt = 0;
	}
	
	//ynTextDraw(x, y, RGB(0, 0, 255), L"FPS:%.1f/%d", fps, frmcnt );
	ynTextDraw(x, y, COLOR(0, 1, 0, 1), L"FPS:%.1f/%d", fps, frmcnt);
}




/////////////////////////////////////////////////////////////////////////////
//
// ���� ����ȭ�� ����ð� ȹ��.
//
float GetEngineTime()
{	 
	static int oldtime = GetTickCount();
	int nowtime = GetTickCount();
	float dTime = (nowtime - oldtime) * 0.001f;
	oldtime = nowtime;

	return dTime; 
}





////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// ���� ó�� �Լ���.
//
////////////////////////////////////////////////////////////////////////////// 

////////////////////////////////////////////////////////////////////////////// 
//
// ���� �޼��� ó��
//
int ynErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...)
{
	TCHAR msgva[1024] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, msg, vl);
	va_end(vl);
	

	//HRESULT ���� ���� �޼��� ���.
	TCHAR herr[1024] = L"";
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0,  hr, 
				   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				   herr, 1024, NULL);

	//���� �޼��� ���̱�.
	TCHAR errmsg[2048] = L"";
	_stprintf(errmsg, L"%s \n�����ڵ�(0x%X) : %s \nFile = %s (%d)\nFunc = %s\n\n",
		msgva, hr, herr,
		file, line, func );
	 

	//(����� ��) VS ���â���� ���..
	OutputDebugString(L"\n");
	OutputDebugString(errmsg);

	//�α����Ϸ� ���.
	//...

	//�޼��� â ���..
	if (bMBox)
	{
		MessageBox(NULL, errmsg, L"Yena::Error", MB_OK | MB_ICONERROR);
		//GetLastError(hr);
	}

	return YN_OK;
}



////////////////////////////////////////////////////////////////////////////// 
//
// ���� �޼��� ó�� : ���̴� ���� ó����.
// 
int ynErrorW(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob, 
			TCHAR* filename, char* EntryPoint, char* ShaderModel )
{

	/*//�����Ű����� ó��.
	TCHAR msgva[2048] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, msg, vl);
	va_end(vl);
	*/
	//�Ķ����, �����ڵ�� ��ȯ.
	TCHAR func[80] = L"";
	::mbstowcs(func, EntryPoint, strlen(EntryPoint));
	TCHAR sm[20] = L"";
	::mbstowcs(sm, ShaderModel, strlen(ShaderModel));


	//���̴� ���� �޼��� �б�.
	TCHAR errw[4096] = L"";
 	::mbstowcs(errw, (char*)pBlob->GetBufferPointer(), pBlob->GetBufferSize());
 

	//HRESULT ���� ���� �޼��� ��� 
	//�ý������� ���� ��� ���̴� �����޼����� ����Ȯ�ϹǷ� ����.
	TCHAR herr[1024] = L"�Ʒ��� ������ Ȯ���Ͻʽÿ�.";
	/*FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		herr, 1024, NULL);
	*/
	

	//���� �޼��� ���̱�.
	TCHAR errmsg[1024];
	_stprintf(errmsg, L"%s \nFile=%s  Entry=%s  Target=%s  \n�����ڵ�(0x%08X) : %s \n\n%s", 
				msg, filename, func, sm,
				hr, herr, errw ); 
	

	//(����� ��) VS ���â���� ���..
	OutputDebugString(L"\n");
	OutputDebugString(errmsg);
	//OutputDebugString(errw);


	//�α����Ϸ� ���.
	//...


	//�޼��� â ���..
	if (bMBox)
	{
		MessageBox(NULL, errmsg, L"Yena::Error", MB_OK | MB_ICONERROR);
		//MessageBox(NULL, errw, L"Yena::Error", MB_OK | MB_ICONERROR);
	}
		
	return YN_OK;
}




////////////////////////////////////////////////////////////////////////////// 
//
// ���� �޼��� ó��
//
int ynErrorW(TCHAR* msg, ...)
{
	TCHAR msgva[2048] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, msg, vl);
	va_end(vl);
	

	//(����� ��) VS ���â���� ���..
	OutputDebugString(L"\n");
	OutputDebugString(msgva);

	//�α����Ϸ� ���.
	//...

	//�޼��� â ���..
	{
		MessageBox(NULL, msgva, L"Yena::Error", MB_OK | MB_ICONEXCLAMATION);
		//GetLastError(hr);
	}

	return YN_OK;
}




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// ��ġ/GPU ���� ȹ�� �Լ���
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// ��ġ/GPU ���� ȹ��
//
void GetDeviceInfo()
{
 	//��ġ ��ɷ��� Ȯ��.
	GetFeatureLevel();

	//GPU ���� ���.
	GetAdapterInfo(&g_Adc);

	//����� ���� ���.
	//...
		 
}




/////////////////////////////////////////////////////////////////////////////
//
//  DX ��� ���� ���ϱ�.
//
static TCHAR* strFeature[4][4] =
{
	{ L"DX9",   L"DX9.1",  L"DX9.2", L"DX9.3" },
	{ L"DX10",  L"DX10.1", L"N/A",   L"N/A" },
	{ L"DX11",  L"DX11.1", L"N/A",   L"N/A" },
	{ L"DX12",  L"DX12.1"  L"N/A",   L"N/A" }
};

void GetFeatureLevel()
{
	UINT feat = g_FeatureLevels;
	UINT ver = 0;
	UINT sub = 0;

	#define OFFSET 0x9;

	ver = ((feat & 0xf000) >> 12) - OFFSET;	//���� ���� ����.   	
	sub = ((feat & 0x0f00) >> 8);			//���� ���� ����.
	
	g_strFeatureLevel = strFeature[ver][sub];
}




/////////////////////////////////////////////////////////////////////////////
//
// ��ġ/GPU ���� ȹ�� 
//
// �⺻ ��ġ�� ó����.���� GPU �����ÿ��� ������ ���� ó���� �ʿ�.
// �����׸�1 : DirectX Graphics Infrastructure (DXGI) /  https://msdn.microsoft.com/ko-kr/library/windows/desktop/ee417025(v=vs.85).aspx
// �����׸�2 : How To: Enumerate Adapters  /  https://msdn.microsoft.com/en-us/library/windows/desktop/ff476877(v=vs.85).aspx 
// �����׸�3 : DXGI_ADAPTER_DESC structure /  https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb173058(v=vs.85).aspx
//

// ����̽�/GPU ���� ȹ�� <��� #1> 
// DXGI 1.1 / DXGI Factory ��� / DX11 �̻� ����
//
HRESULT GetAdapterInfo(DXGI_ADAPTER_DESC1* pAd)
{
	IDXGIAdapter1* pAdapter;
	IDXGIFactory1* pFactory = NULL;
	//DXGI_ADAPTER_DESC ad;

	//DXGIFactory ��ü ����. (DXGI.lib �ʿ�)
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
	{
		return E_FAIL;
	}

	pFactory->EnumAdapters1(0, &pAdapter);		//��� ȹ��.
	pAdapter->GetDesc1(pAd);					//��� ���� ȹ��.
	//*pAd = ad;								//�ܺη� ����.

	//���� �����, ������ �������̽��� �����մϴ�. (�޸� ���� ����)
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);

	return S_OK;
}

/*
// ����̽�/GPU ���� ȹ�� <��� #2>  
// DXGI 1.1 / ���� ��ġ Interface ��� / DX11 �̻� ����
//
HRESULT GetAdapterInfo(DXGI_ADAPTER_DESC1* pAd)
{
	IDXGIDevice* pDXGIDev;
	IDXGIAdapter1* pAdapter;

	//DXGIFactory ��ü ����. (DXGI.lib �ʿ�)
	if (FAILED(g_pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGIDev)))
	{
		return E_FAIL;
	}

	pDXGIDev->GetAdapter((IDXGIAdapter**)&pAdapter);		//��� ȹ��.
	pAdapter->GetDesc1(pAd);					//��� ���� ȹ��.

	//���� �����, ������ �������̽��� �����մϴ�. (�޸� ���� ����)
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pDXGIDev);

	return S_OK;
}
*/




/////////////////////////////////////////////////////////////////////////////
//
// ��ġ/GPU ���� ���
//
// �����׸�1 : DXGI_ADAPTER_DESC structure
// ������ũ1 : https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb173058(v=vs.85).aspx
//
void AdapterInfo(int x, int y, COLOR col)
{
	#define ToMB(a) (a/1024/1024)

	DXGI_ADAPTER_DESC1& dc = g_Adc;
	ynTextDraw(x, y += 14, COLOR(0, 1, 0, 1), L"[GPU #%d]", 0);
	//GPU ����.
	ynTextDraw(x, y += 14, col, L"Adapter: %u", 0 );		//�⺻ ��ġ�� ó����. ���� GPU �����ÿ���  ����ó���� �ʿ�.
	ynTextDraw(x, y += 14, col, L"Description: %s", dc.Description);
	ynTextDraw(x, y += 14, col, L"VendorID: %u", dc.VendorId);
	ynTextDraw(x, y += 14, col, L"DeviceID: %u", dc.DeviceId);
	ynTextDraw(x, y += 14, col, L"SubSysID: %u", dc.SubSysId);
	ynTextDraw(x, y += 14, col, L"Revision: %u", dc.Revision);
	ynTextDraw(x, y += 14, col, L"VideoMem: %lu MB", ToMB(dc.DedicatedVideoMemory));
	ynTextDraw(x, y += 14, col, L"SystemMem: %lu MB", ToMB(dc.DedicatedSystemMemory));
	ynTextDraw(x, y += 14, col, L"SharedSysMem: %lu MB", ToMB(dc.SharedSystemMemory));
	ynTextDraw(x, y += 14, col, L"AdpaterLuid: %u.%d", dc.AdapterLuid.HighPart, dc.AdapterLuid.LowPart);
		 
}

 


/////////////////////////////////////////////////////////////////////////////
//
// ��ġ/GPU �� �ý��� ���� ���
//
void SystemInfo(int x, int y, COLOR col)
{
	COLOR col2 = col * 0.7f;

	ynTextDraw(x, y += 14, col, L"[SYSTEM]");
	ynTextDraw(x, y += 14, col, L"VGA: %s", g_Adc.Description);
	ynTextDraw(x, y += 14, col, L"Feat: %s", g_strFeatureLevel);
	ynTextDraw(x, y += 14, col, L"�ػ�: %dx%d", g_Mode.Width, g_Mode.Height);
	
	//GPU ���� ���.
	//AdapterInfo(x, y += 14, col);
	
	//Yena ���� ���.
	YenaSysInfo(x, y, col);

	//�׷��� �ɼ� ���.
	{
		x = g_Mode.Width - 80;
		y = 50;
		//col = COLOR(1, 1, 0, 1);
		ynTextDraw(x, y += 14, col, L"[Graphics]");
		ynTextDraw(x, y += 14, col, L"AA = x%d", g_dwAA);
		ynTextDraw(x, y += 14, col, L"AF = x%d", g_dwAF);
	}

}








/////////////////////////////////////////////////////////////////////////////
//
// �ܺ����Ͽ��� ������ �ε��ϱ�. 
//
//bool LoadIniData(TCHAR* pTexName0, TCHAR* pTexName1)
bool LoadIniData(TCHAR* pModelName, TCHAR* pTexName)
{
	FILE* fp = _tfopen(L"data.ini", L"rt");
 	if(fp == NULL) return false;

	TCHAR token[256];
	TCHAR line[256];
 
	while(1)
	{
		if(feof(fp)) break;

		//���� �б�.
 		_fgetts(line, 256, fp);	//������ '����'������ fscanf �� ���� ����..
		_stscanf(line, L"%s", token);

		//�ּ� �˻�.  
		if(!_tcsnicmp(token, L"//", 2)) continue;	//�ּ�. 
 
		if(!_tcsnicmp(token, L"Model", 5) && pModelName )
		{
			swscanf(line, L" Model = %s ", pModelName);
			continue;
		}



		if(!_tcsnicmp(token, L"Tex", 3) && pTexName )
		{
			 swscanf(line, L" Tex = %s", pTexName);
			//strcpy(pTextureName, pTexNameCube);
			continue; 
		}


		/*if(!_tcsnicmp(token, L"Tex0", 4))
		{
			_stscanf(line, L"Tex0 = %s", pTexName0);		 	
			//strcpy(pTextureName, pTexNameCube);	 
			continue;
		}

		if(!_tcsnicmp(token, L"Tex1", 4))
		{
			_stscanf(line, L"Tex1 = %s", pTexName1);		 	
			//strcpy(pTextureName, pTexNameFloor);
			continue; 
		}*/
		 
	}

	return true;
}






////////////////////////////////////////////////////////////////////////////// 
//
// �ؽ�ó ���� ���.
//
HRESULT GetTextureDesc(ID3D11ShaderResourceView* pRV, D3D11_TEXTURE2D_DESC* pDesc)
{
	HRESULT res = S_OK;

	//���̴� ���ҽ� �� ���� ȹ��.
	D3D11_SHADER_RESOURCE_VIEW_DESC dc;
	pRV->GetDesc(&dc);

	//�ؽ�ó ���� ȹ��.
	ID3D11Resource* pRes = nullptr;
	pRV->GetResource(&pRes);
	ID3D11Texture2D* pTex = nullptr;
	pRes->QueryInterface<ID3D11Texture2D>(&pTex);
	if (pTex)
	{
		pTex->GetDesc(pDesc);

		//���� ȹ���� �������̽� ����.
		pTex->Release();			
	}
		
	//���� ȹ���� �������̽� ����.
	//SAFE_RELEASE(pRes);

	return res;
}



////////////////////////////////////////////////////////////////////////////// 
//
// �ؽ�ó �ε�
//
HRESULT LoadTexture(TCHAR* filename, ID3D11ShaderResourceView** ppTexRV)
{
	HRESULT hr = S_OK;	
	 
	ID3D11ShaderResourceView* pTexRV = nullptr;

	//TCHAR* fileName = L"../data/sonim.jpg";
	
	//WIC ���� �ε�.
	//hr = DirectX::CreateWICTextureFromFile( g_pDevice, fileName, nullptr, &g_pTextureRV );	
	
	//WIC ���� �ε� (+�Ӹ�)
	hr = DirectX::CreateWICTextureFromFileEx( g_pDevice, g_pDXDC, filename, 0, 
				D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET,
				0,	D3D11_RESOURCE_MISC_GENERATE_MIPS,	WIC_LOADER_DEFAULT, 
				nullptr, &pTexRV );	

	if (FAILED(hr))
	{
		//���н�, DDS ���Ϸ� �ε� �õ� (+�Ӹ�)
		/*hr = DirectX::CreateDDSTextureFromFileEx( g_pDevice, g_pDXDC, filename, 0, 
				D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_RENDER_TARGET,
				0,	D3D11_RESOURCE_MISC_GENERATE_MIPS,	false,	nullptr, &pTexRV );	 */
		hr = DirectX::CreateDDSTextureFromFile( g_pDevice, filename, nullptr, &pTexRV );	
		if (FAILED(hr))
		{
			ynError(hr, L"�ؽ�ó �ε� ���� \nFile=%s", filename);
			return hr;
		}
	}

	

	//�ؽ�ó �Ӹ����.
	//g_pDXDC->GenerateMips(g_pTextureRV);
	
	//�ܺη� ����.
	*ppTexRV = pTexRV;

	return hr;
}




/*  Yena Math �� ���Ե�.
/////////////////////////////////////////////////////////////////////////////
//
// ���� ������ �����ε�.
//
COLOR operator * (COLOR& lhs, float rhs)
{
	COLOR col;
	col.x = lhs.x * rhs;
	col.y = lhs.y * rhs;
	col.z = lhs.z * rhs;
	col.w = lhs.w * rhs;

	return col;
}
*/




//////////////////////////////////////////////////////////////////////////////// 
//
//int CreateVB(void* vtx, DWORD vcnt, DWORD vbsize)
HRESULT CreateVB(LPDEVICE pDev, void* pBuff, UINT size, LPVERTEXBUFFER* ppVB)
{ 
	//int res = YN_OK;
	
	//���� ���� Ȯ��.  
	//...

	//m_pDev = pDev;
	LPVERTEXBUFFER pVB = nullptr;
	
	//���� ���� ����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					//���� �����
	bd.ByteWidth = size;							//���� ũ��
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//���� �뵵 : "���� ����" ��� ���� 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA rd;				
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = pBuff;								//���ۿ� �� ������ ���� : "������"..
	
	//���� ���� ����.
	HRESULT hr = pDev->CreateBuffer(&bd, &rd, &pVB);
	if (FAILED(hr))
	{
		//���� ���� �� ����ó��..
		//...
		ynError(hr, L"CreateVB : �������� ���� ����");
		return hr;
	}
	
	//�ܺη� ����.
	*ppVB = pVB;

	return hr;

}//end of CreateVB()






////////////////////////////////////////////////////////////////////////////////
//
HRESULT CreateIB(LPDEVICE pDev, void* pBuff, UINT size, LPINDEXBUFFER* ppIB)
{
	//int res = YN_OK;

	//���� ���� Ȯ��. 
	//DWORD ibsize = sizeof(XINDEX)*faceCnt;
		
	LPINDEXBUFFER pIB = nullptr;

	// �ε��� ���� Index Buffer ���� 
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;					//���� �����
	bd.ByteWidth = size;							//���� ũ��
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;			//���� �뵵 : "���� ����" ��� ���� 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA rd;				
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = pBuff;								//���ۿ� �� ������ ���� : "���� ����"..
	
	//���� ���� ����.
	HRESULT hr = pDev->CreateBuffer(&bd, &rd, &pIB);
	if (FAILED(hr))
	{
		//���� ���� �� ����ó��..
		//...
		ynError(hr, L"CreateIB : �ε������� ���� ����");
		return hr;
	}
	
	//��� ���� ����.
	//m_FaceCnt = ibsize / sizeof(XINDEX);		//(IB �� ����Ͽ� �׸�) �� �ﰢ�� ���� -> XFile ���� ����.
	//m_IndexedVtxCnt = m_FaceCnt * 3;			//(IB �� ����Ͽ� �׸�) �� ��������.
	//m_FullVtxCnt = m_FaceCnt * 3;
	
	//�ܺη� ����.
	*ppIB = pIB;

	return hr;

}//end of CreateIB() 




/////////////////////////////////////////////////////////////////////////////// 
//
//  3D / ��� �׸��� ���� ��ȯ.
//
void Mode_3D()
{ 

	//--------------------------
	// ī�޶� ���� ����  : "3D ī�޶�" 
	//--------------------------
	// ī�޶� Ŭ������ �ִٸ� ���� ��������������.
	// ���� ���𿡼��� yn_View, yn_Proj �� ����Ǿ��ֽ��ϴ�.
	g_cbDef.mView = yn_View;
	g_cbDef.mProj = yn_Proj;
	//UpdateDynamicConstantBuffer(g_pDXDC, g_pCBDef, &g_cbDef, sizeof(cbDEFAULT)); 


	//--------------------------
	// ������ ���� ����
	//--------------------------
	// ���� ��.
	//... 

	//�ؽ�ó ���÷� ���� 
	g_pDXDC->PSSetSamplers(0, 1, &g_pSampler[SS_WRAP]);

	//������ �ɼ� ����.. 
	RenderModeUpdate();
	//g_pDXDC->RSSetState(g_RState[RS_CULL_CCW]);
	// ����/���ٽ� ���� ����.
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON], 0);			//���̹��� ���� (�⺻��) 

}





/////////////////////////////////////////////////////////////////////////////// 
//
//  2D / UI ��� ��ȯ.
//
void Mode_2D()
{

	//--------------------------
	// ���� "3D ī�޶�" ���� ���.
	//--------------------------
	// ī�޶� Ŭ������ �ִٸ� ���� ��������������.
	// ���� ���𿡼��� yn_View, yn_Proj �� ����Ǿ��ֽ��ϴ�.
	//...


	//--------------------------
	// 2D ī�޶� ����.
	//--------------------------
	// �� ��� ���� : ���� ��ķ� ������ �ʿ�. 
	g_cbDef.mView = XMMatrixIdentity();
	// �������� ��� ���� : 'Ortho-Off Projection' ��� ����.
	g_cbDef.mProj =	XMMatrixOrthographicOffCenterLH(0, (float)g_Mode.Width, (float)g_Mode.Height, 0.0f, 1.0f, 100.0f);		
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBDef, &g_cbDef, sizeof(cbDEFAULT));


	//--------------------------
	// ������ ���� ����
	//--------------------------
	//���� �� : 2D UI ������ ���� ���ʿ�. 
	//... 

	//�ؽ�ó ���÷��� Clamp ����.
	g_pDXDC->PSSetSamplers(0, 1, &g_pSampler[SS_CLAMP]);

	//�ø� OFF. �޸鵵 �׸��ϴ�.
	g_pDXDC->RSSetState(g_bWireFrame?g_RState[RS_WIREFRM]:g_RState[RS_SOLID]);
	//g_pDXDC->RSSetState(g_RState[RS_SOLID]);
	
	//--------------------------
	// ����/���ٽ� ���� ���� ����
	//--------------------------
	//1.����/���ٽ� ���� Ŭ����.(�ɼ�)
	//RTClear(...)
	//2.���� ����� �ʿ信 ����...
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_WRITE_OFF], 0);			//���̹��� ���� off

}





/////////////////////////////////////////////////////////////////////////////// 
//
//  2.5D / ����+UI ��� ��ȯ. 
//
void Mode_25D()
{

	//--------------------------
	// ���� "3D ī�޶�" ���� ���.
	//--------------------------
	// ī�޶� Ŭ������ �ִٸ� ���� ��������������.
	// ���� ���𿡼��� yn_View, yn_Proj �� ����Ǿ��ֽ��ϴ�.
	//...


	//--------------------------
	// 2.5D ī�޶� ����.
	//--------------------------
	// �� ��� ���� : ���� ��ķ� ������ �ʿ�. 
	g_cbDef.mView = XMMatrixIdentity();
	// �������� ��� ���� --> 3D ���� ī�޶� ���.
	g_cbDef.mProj = yn_Proj;
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBDef, &g_cbDef, sizeof(cbDEFAULT));
	
	
	//--------------------------
	// ������ ���� ����
	//--------------------------
	//���� �� : 2D UI ������ ���� ���ʿ�. 
	//... 

	//�ؽ�ó ���÷��� Wrap ���� : Scroll ó���� ���� �ʿ�. 
	g_pDXDC->PSSetSamplers(0, 1, &g_pSampler[SS_WRAP]);

	//�ø� OFF. �޸鵵 �׸��ϴ�.
	g_pDXDC->RSSetState(g_bWireFrame?g_RState[RS_WIREFRM]:g_RState[RS_SOLID]);
	//g_pDXDC->RSSetState(g_RState[RS_SOLID]);

	//--------------------------
	// ����/���ٽ� ���� ���� ����
	//--------------------------
	//1.����/���ٽ� ���� Ŭ����.(�ɼ�)
	//RTClear(...)	
	//2.���� ����� �ʿ信 ����...
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF], 0);			//���̹��� off
	//g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_WRITE_OFF], 0);	//���̹��� ���� off

}


 
 

//////////////////////////////////////////////////////////////////////////////// 
//
// ����Ÿ��, ����/���ٽ� ���� Ŭ����.
//
void RTClear(COLOR& col)
{ 
	//[����] ����Ÿ�ٰ� ����Ʈ ����.
	// DX9 �� �ٸ��� DX10/11 ������ ClearRenderTarget �޼ҵ��
	// ����Ÿ�ٺ� ��ü�� ����� Viewport �� Scissor ������ ������� �ʽ��ϴ�.
	// ...
	// �ذ�1 : DX11.1 / ID3D11DeviceContext1::ClearView �޼ҵ� ���.
	// �ذ�2 : "Drawing Quad" , ��������/�簢�� �޽��� ���� ����� 
	
	//����Ÿ��(�����)/ ���̹��� �� ���ٽǹ��� �����..
	ClearBackBuffer(D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,	//�ʱ�ȭ �÷���.
					col,			//����.
					1.0f,			//���� ���� �ʱⰪ.
					0				//���ٽ� ���� �ʱⰪ.
				    );
	
}




/////////////////////////////////////////////////////////////////////////////// 
//
// ���� ���� : 0 ~ 1 ���̷� ���� �����Ѵ�.
//
void Saturate(COLOR* col)
{	
	col->x = max(col->x, 0);
	col->y = max(col->y, 0);
	col->z = max(col->z, 0);
	//col->w = max(col->w, 0);

	col->x = min(col->x, 1);
	col->y = min(col->y, 1);
	col->z = min(col->z, 1);		 
	//col->w = min(col->w, 1);
}




/////////////////////////////////////////////////////////////////////////////// 
//
// ������� : ������ �����ε� ��ĵ� ��õ.
//
COLOR InvertColor(COLOR& col)
{	
	COLOR newCol;
	newCol.x = 1 - col.x;
	newCol.y = 1 - col.y;
	newCol.z = 1 - col.z;
	newCol.w = col.w;

	Saturate(&newCol);
	
	return newCol;
}







//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
//////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////
//
// ��Ʈ ���� --> Yena �� ���Ե� 
//
// DirectXTK : SpriteFont ��� ��Ʈ ��� Ŭ����
// 2D Texture / Sprite ����� ��Ʈ ��� ���̺귯��.
// ������ ����������, CJK �迭�� ��Ʈ�� ��� �ϼ����� �����˴ϴ�.
// ���� : https://directxtk.codeplex.com/wikipage?title=SpriteFont
// #include "SpriteFont.h"	
//
// �ڼ��� ���� DirectXTK.h �Ǵ�  Yena/DXTK/Readme.txt �� ����. 
//
///////////////////////////////////////////////////////////////////////////////
/*
SpriteBatch* g_pFontBatch = nullptr;
SpriteFont*  g_pFont = nullptr;


///////////////////////////////////////////////////////////////////////////////
// 
int ynFontCreate(LPDEVICE pDev)
{
	
	//��ġ ��� ȹ��.
	LPDXDC pDXDC = nullptr;	
	pDev->GetImmediateContext(&pDXDC);


	//Sprite Batch ��ü ����.
	g_pFontBatch = new SpriteBatch(pDXDC);

	//DirectX Toolkit : Sprite Font ��ü ����.
	TCHAR* filename = L"../Yena/Font/����9k.sfont";	//ASCII 0 ~ 255 + Ư������'��' + Unicode �ѱ� �ϼ��� �� 11,440 ����, ũ��:9		
	try {
		g_pFont = new SpriteFont(pDev, filename );	
		g_pFont->SetLineSpacing(14.0f);				//��Ʈ9 ����, �ٰ��� ����. '���߶��� ��½� ����� ������'
		g_pFont->SetDefaultCharacter('_');			//��� ���ڰ� �̰˻��� ��� ����� Ű��.
	}
	catch (std::exception& e)
	{
		//ynError(0, L"��Ʈ ���� ���� : File=%s", filename);
		TCHAR msg[1024] = L"";	
		::mbstowcs(msg, e.what(), strlen(e.what()));
		ynError(0, L"��Ʈ ���� ���� : %s \n File=%s", msg, filename);
	}

	//�����, ��ġ��� ����.
	SAFE_RELEASE(pDXDC);

	return YN_OK;
}



///////////////////////////////////////////////////////////////////////////////
//
void ynFontRelease()
{
	SAFE_DELETE(g_pFontBatch);
	SAFE_DELETE(g_pFont);
}




/////////////////////////////////////////////////////////////////////////////// 
//
// ���ڿ� ��� : DX11 (�����ڵ�) ����.
//
void ynTextDraw(int x, int y, COLOR col, TCHAR* msg, ...)
{
	const DWORD _size = 2048;

	TCHAR buff[_size] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);

	g_pFontBatch->Begin();
	//g_pFontBatch->Begin(SpriteSortMode_Deferred, nullptr, nullptr, ypStateObj[YNDS_DEPTH_ON_]);		//���̿��� �߰�.
	g_pFont->DrawString(g_pFontBatch, buff, Vector2((float)x, (float)y), Vector4(col));					//���� ũ��.
	//g_pFont->DrawString(ypFontBatch, buff, Vector2((float)x, (float)y), Vector4(col), 0, Vector4::Zero, 0.8f);	//80% ��� ���.
	g_pFontBatch->End();

}
*/

 
/***************** End of "Device.cpp" *******************************/


 
