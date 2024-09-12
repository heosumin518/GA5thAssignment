//
// Device.h : D3D ����̽� �¾� �ڵ�.
//
// DirectX ���� ���� : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
#pragma once
#pragma warning(disable:4996)

// Platform Header �� Library
//
#include "Windows.h"
#include "stdio.h"

///////////////////////////////////////////////////////////////////////////////
//
// DirectX ǥ�� ���̺귯�� 
//
///////////////////////////////////////////////////////////////////////////////
// < DirectX ������ ���� ���̺귯�� >
// DX9         : DirectX SDK 2002 Nov. 
// DX10.x/DX11 : DirectX SDK 2010 June (DX ���� ���� ������ ����)
// DX11.X/DX12 : Windows SDK 8.x  (DX �� ���ԵǾ� ������) 

// DX ǥ�� ��� 
#include "D3D11.h"					//DX ǥ�� ���. (DX 11.0)
//#include "D3D11_1.h"				//DX ǥ�� ���. (DX 11.1)
#pragma comment(lib, "D3D11")		//DX ���̺귯�� �ε�.  D3D11.dll �ʿ�.

//DirectX Math : Dx ���� ���̺귯�� 
//#include "XNAMath.h"				//XNA Math ���̺귯��.(ver.2.0) DirectX SDK 2010 June. ����.
#include "DirectXMath.h"			//XNA Math ���̺귯��.(ver.3.0) VS2012 (Windows SDK 8) �̻�.
using namespace DirectX;

//DXGI : DirectX Graphics Infrastructure 
//DX �� ������ �������� �ϵ���� ������ ���� ����� �����մϴ�.
//#include "DXGI.h"					//DXGI ���. (d3d11.h �� ���Ե�)
#pragma comment(lib, "dxgi")		//DXGI ���̺귯��. DXGI.dll �ʿ�.


///////////////////////////////////////////////////////////////////////////////
//
// DirectX Toolkit : DX Ȯ�� ���̺귯��  --> Yena �� ���Ե�.
//
///////////////////////////////////////////////////////////////////////////////
// DX Ȯ�� ���̺귯�� �� ��ƿ��Ƽ ������.
// DX ���� �ʿ��� ���� ��� Ŭ���� �� �ΰ� ���񽺸� �����մϴ�.
// DirectXTK �� ���� ���ϰ� ����ϱ� ���ؼ� Project �Ӽ� �߰��� �ʿ��մϴ�.
// DirectXTK.h �Ǵ�  Yena/DXTK/Readme.txt �� �����ϼ���.  
//#include "DirectXTK.h"

//DirectXTK �� �ؽ�ó API ���.
//������ ���Ե� ���� ����.
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h" 


////////////////////////////////////////////////////////////////////////////
//
// Yena : ���� ���.
//
////////////////////////////////////////////////////////////////////////////
// ���� ���߿� �ʿ��� �⺻ ��ġ �� Ȯ�� ����� ����.
// �����ӿ�ũ,��Ʈ,����,ī�޶�,����,���̴�,���� ��ü ���..
// �ڼ��� ���� Yena ����.txt ����.
//
#include "Yena.h"			// Yena ���� ���.




///////////////////////////////////////////////////////////////////////////////
//
// Ÿ�� �� ��� ������ : ������ ȣȯ�� ��� ���밡 ����.
//
///////////////////////////////////////////////////////////////////////////////
//DX11 ��ġ���� ������.											
typedef ID3D11Device*			LPDEVICE;
typedef ID3D11DeviceContext*	LPDXDC;
typedef IDXGISwapChain*			LPSWAPCHAIN;
typedef ID3D11RenderTargetView*	LPRTVIEW;
typedef ID3D11DepthStencilView*	LPDSVIEW;
typedef ID3D11InputLayout*		LPINPUTLAYOUT;
typedef ID3D11Buffer*			LPBUFFER;
typedef LPBUFFER				LPCBUFFER;
typedef ID3D11ShaderResourceView*  LPSRVIEW;
typedef LPSRVIEW	LPTEXTURERV;
typedef D3D11_TEXTURE2D_DESC	TEXDESC;
//typedef ID3DBlob*				LPXDATA;


/*
// DirectX Math Ÿ�� ������ : ȣȯ�� ���
// DX9/DX10 : DirectX SDK June.2010 ����
typedef D3DXMATRIXA16	MATRIXA;	//��� : 16����Ʈ ���� ����.
typedef D3DXMATRIX		MATRIX;		//��� : �Ϲ� ����.
typedef D3DXVECTOR4		VECTOR4;
typedef D3DXVECTOR3		VECTOR3;
typedef D3DXVECTOR2		VECTOR2;
*/
// DirectX Math Ÿ�� ������ : ���� �ҽ����� ȣȯ�� ����� ����.
// DX10/11 "XNAMath ver.2": DirectX SDK june.2010
// DX11/12 "XNAMath ver.3": DirectXMath, Windows SDK 8.x �� ����  
// ��ũ : https://msdn.microsoft.com/ko-kr/library/windows/desktop/ee418728(v=vs.85).aspx
//
typedef XMMATRIX		MATRIXA;	//��� : 16����Ʈ ����, SIMD ����. ����/���� ������. "Register Type"
typedef XMFLOAT4X4		MATRIX;		//��� : �Ϲ� ����. SIMD ������, Class ������ �����. "Storage Type"
typedef XMVECTOR		VECTOR;		//4���� ���� : 16����Ʈ ����, SIMD ����. ����/���� ������. "Register Type"
typedef XMFLOAT4		VECTOR4;	//4���� ���� : �Ϲ� ����, SIMD ������, Class ������ �����. "Storage Type"
typedef XMFLOAT3		VECTOR3;
typedef XMFLOAT2		VECTOR2;

//���� Ÿ��: 2����.
//typedef XMCOLOR		COLOR;		// r, g, b, a.  [������ 0~255]
typedef XMFLOAT4		COLOR;		// r, g, b, a.  [�Ǽ��� 0~1.0]
typedef XMFLOAT4A		COLORA;		// r, g, b, a.  [�Ǽ��� 0~1.0] + 16��Ʈ ����
typedef XMVECTOR        XMCOLOR;	// r, g, b, a.  [�Ǽ��� 0~1.0] + 16��Ʈ ���� SIMD ��.


//��� Ÿ�� ������.
typedef XMFLOAT4		PLANE;		//�Ϲ� ����
typedef XMVECTOR		XMPLANE;	//SIMD ����.


///////////////////////////////////////////////////////////////////////////////
//
// DX �� ��ġ ���� ���� �����͵�  
//
///////////////////////////////////////////////////////////////////////////////

int		DXSetup(HWND hwnd);
void	DXRelease(); 

int		ClearBackBuffer(UINT flag, COLOR col, float depth = 1.0f, UINT stencil = 0);
int     Flip();

float	GetEngineTime();
void	PutFPS(int x, int y);
//void  ynTextDraw( int x, int y, COLOR col, char* msg, ...);

void	GetDeviceInfo();
HRESULT GetAdapterInfo(DXGI_ADAPTER_DESC1* pAd);
void	SystemUpdate(float dTime);
void	SystemInfo(int x, int y, COLOR col);

// ���� DX ��� ���� ���ϱ�. 
void	GetFeatureLevel();

//�ܺ� ���� ���� �б�.
//bool	LoadIniData(TCHAR* pModelName);
bool	LoadIniData(TCHAR* pModelName, TCHAR* pTexName);
//bool	LoadIniData(TCHAR* pTexNameCube, TCHAR* pTexNameMap);

//�ؽ�ó �Լ���.
HRESULT LoadTexture(TCHAR* filename, ID3D11ShaderResourceView** ppTexRV);
HRESULT GetTextureDesc(ID3D11ShaderResourceView* pTexRv, D3D11_TEXTURE2D_DESC* pTexDesc);

//���Ϲ��� ����.
HRESULT CreateVB(LPDEVICE pDev, void* pBuff, UINT size, LPVERTEXBUFFER* ppVB);
HRESULT CreateIB(LPDEVICE pDev, void* pBuff, UINT size, LPINDEXBUFFER* ppIB);


//������ ��� ��ȯ.
void Mode_2D();
void Mode_3D();
void Mode_25D();


// ����Ÿ��, ����/���ٽ� ���� Ŭ����.
void RTClear(COLOR& col);

//���� ó��.
void  Saturate	 (COLOR* col);
COLOR InvertColor(COLOR& col);



extern ID3D11Device*           g_pDevice;
extern ID3D11DeviceContext*	   g_pDXDC;
extern IDXGISwapChain*         g_pSwapChain;
extern ID3D11RenderTargetView* g_pRTView;
extern ID3D11DepthStencilView* g_pDSView;



// ��ġ ���� ���� ����ü. (DX9/11 ���� ȣȯ�� ������)
typedef DXGI_MODE_DESC	  DISPLAYMODE;	//DX11 ����
//typedef DXGI_MODE_DESC1 DISPLAYMODE;	//DX11.1 ����
//typedef D3DDISPLAYMODE DISPLAYMODE;   //DX9 ��.

extern DISPLAYMODE g_Mode;
extern HWND g_hWnd;
extern BOOL g_bShowFrame;

extern DWORD g_dwAA;
extern DWORD g_dwAF;

//��üȭ�� ��뿩��.
extern BOOL g_bWindowMode;
//��������ȭ ��뿩��.
extern BOOL g_bVSync;

//���� ����̽� DX ��� ����.
extern TCHAR* g_strFeatureLevel;




///////////////////////////////////////////////////////////////////////////////
//
// ������ ���� ��ü ����  
//
///////////////////////////////////////////////////////////////////////////////

extern BOOL g_bCullBack;		//�޸� ����...On/Off.
extern BOOL g_bWireFrame;		//���̾� ������ On/Off.
extern BOOL g_bZEnable;			//���� ���� ���� On/Off.
extern BOOL g_bZWrite;			//���̹��� ���� On/Off.	

extern COLOR g_ClearColor;		//����.
extern COLOR g_ClearColor2;		//����2.

//
// ������ ���� ��ü ���� / ��� �Լ���.
//
//int  RenderStateObjectCreate();
//void RenderStateObjectRelease();
void RenderStateUpdate();
void RenderModeUpdate();


//
// ����/���ٽ� �׽�Ʈ ���µ�.
//
enum {
	DS_DEPTH_ON,		//���̹��� ON! (�⺻��), ���ٽǹ��� OFF.
	DS_DEPTH_OFF,		//���̹��� OFF!, ���ٽ� OFF.
	DS_DEPTH_WRITE_OFF,	//���̹��� ���� ����, ���ٽ� OFF. 

	//���ٽ� ���� �����.
	DS_DEPTH_ON_STENCIL_ON,				//���̹��� On (Write On), ���ٽǹ��� ON (������ ����) : "����/���ٽ� ���".
	DS_DEPTH_ON_STENCIL_EQUAL_KEEP,		//���̹��� On (Write On), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���� ��ġ���� �׸���".
	DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP,	//���̹��� On (Write On), ���ٽǹ��� ON (�ٸ���, ������ ����) : "���� ��ġ �̿ܿ� �׸���".
	
	//DS_DEPTH_OFF_STENCIL_ON,			//���̹��� Off, ���ٽǹ��� ON (������ ����) : "���ٽǸ� ���"
	//DS_DEPTH_ON_STENCIL_EQUAL_INCR,	//���̹��� On (Write On), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���߱׸��� ����".
	DS_DEPTH_WRITE_OFF_STENCIL_ON,		//���̹��� On (Write Off), ���ٽǹ��� ON (�׻���, ������ ����) :" ���ٽǸ� ���".
	DS_DEPTH_OFF_STENCIL_EQUAL_INCR,	//���̹��� Off (Write Off), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���߱׸��� ����.
	

	//���� ��� �ִ밪.
	DS_MAX_,	

	//��� ������. MAX �� ���ʿ� �����ؾ� ��. ����.
	DS_DEPTH_ON_STENCIL_OFF = DS_DEPTH_ON,			//���̹��� ON, ���ٽǹ��� OFF
	DS_DEPTH_ONLY	= DS_DEPTH_ON_STENCIL_OFF,		//���̹��� ON, ���ٽǹ��� OFF
	//DS_STENCIL_ONLY = DS_DEPTH_OFF_STENCIL_ON,	//���̹��� OFF, ���ٽǹ��� ON
};

//������ ���� ��ü�� : ���� ��ü ������. "Device.cpp"
extern ID3D11DepthStencilState* g_DSState[DS_MAX_];




// 
// �����Ͷ����� ���� ��ü Rasterizer State Objects
// 
enum {
	RS_SOLID,				//�ﰢ�� ä��� : Fill Mode - Soild.
	RS_WIREFRM,				//�ﰢ�� ä��� : Fill Mode - Wireframe.
	RS_CULLBACK,			//�޸� �ø� (ON) : Back-Face Culling - "CCW" 
	RS_CULLFRONT,			//���� �ø� (ON) : Front-Face Culling - "CW" 	
	RS_WIRECULLBACK,		//���̾� ������ + �޸� �ø� (ON) 
	RS_WIRECULLFRONT,		//���̾� ������ + �ո� �ø� (ON) 	
	
	//���� ��� �ִ밪.
	RS_MAX_,

	//��� ������. MAX �� ���ʿ� �����ؾ� ��. ����.
	RS_CULL_CCW = RS_CULLBACK,
	RS_CULL_CW = RS_CULLFRONT,
	RS_WIRE_CULL_CCW = RS_WIRECULLBACK,
	RS_WIRE_CULL_CW = RS_WIRECULLFRONT,	

};
//�����Ͷ����� ���� ��ü �迭
extern ID3D11RasterizerState*	g_RState[RS_MAX_];

//void RasterStateCreate();
//void RasterStateRelease();
//void RenderModeUpdate();


//
// ������ ��� : �ټ��� ������ ���� ���� �� ����� ���� ����.
//
enum {
	RM_SOLID		= 0x0000,		// �ﰢ��ä��� : ON, Solid
	RM_WIREFRAME	= 0x0001,		// �ﰢ��ä��� : OFF, Wire-frame
	RM_CULLBACK		= 0x0002,		// �޸� �ø� : ON, "CCW"
	
	//������ �⺻��� : Solid + Cull-On.
	RM_DEFAULT		= RM_SOLID | RM_CULLBACK,	
	
};
extern DWORD g_RMode;		//'����' ������ ���.



//
// ����/���� ȥ�� ���� ��ü Blending State Objects 
//
enum {
	BS_DEFAULT,				//�⺻ȥ�� ��ü.
	BS_ALPHA_BLEND,			//���� ����. (Src.a + (1-Src.a))
	BS_COLOR_BLEND,			//���� ����. ( Src.Color + (1-Src.Color))
	BS_COLOR_BLEND_ONE,		//1:1 ����. ( Src.Color + Dest.Color) 

	BS_MAX_,

	BS_AB_OFF = BS_DEFAULT,
	BS_AB_ON = BS_ALPHA_BLEND,
	//BS_AB_ALPHA = BS_ALPHA_BLEND,
	//BS_AB_COLOR = BS_COLOR_BLEND,
};

//����/���� ȥ�� ���� ��ü 
extern ID3D11BlendState* g_BState[BS_MAX_];

//void BlendStateCreate();
//void BlendStateRelease();
//void BlendModeUpdate();



//
// �ؽ�ó ���÷� ���� ��ü : Texture Sampler State Objects.
//
enum {
	SS_CLAMP,			//�ؽ�ó ��巹�� ��� : Ŭ���� (DX �⺻��) 
	SS_WRAP,			//�ؽ�ó ��巹�� ��� : ���� 
	//SS_BOARDER,
	//SS_MIRROR,
	//SS_MIRRROONCE,
	
	SS_MAX,
	SS_DEFAULT = SS_WRAP,	//�⺻ ���÷�
	
};
extern ID3D11SamplerState* g_pSampler[SS_MAX];
//void SamplerCreate();
//void SamplerRelease();





///////////////////////////////////////////////////////////////////////////////
//
// ��Ÿ �ý��� ���� �Լ� �� ������.
//
///////////////////////////////////////////////////////////////////////////////

//�񵿱� Ű���� �Է� ó��.
#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(pBuff) if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; }
#define SAFE_DELETE(pBuff)	if((pBuff)){ delete (pBuff); (pBuff) = NULL; }
#define SAFE_DELARRY(pBuff) if((pBuff)){ delete [] (pBuff); (pBuff) = NULL; }
#endif

#ifndef YES_
#define YES_ TRUE
#define NO_  FALSE
#endif

/* Yena �� ���Ե�
#ifndef YN_OK
#define YN_OK	   0					//���� ����. Ư���� ���� ����.
#define YN_FAIL   -1					//���� �߻�.
#define YN_FAILED(v)   ((v) != YN_OK_)	//���� üũ ��ũ��.
#endif

// ���� * ��Į�� ��  (ex)  col = col * 0.5f
COLOR operator * (COLOR& lhs, float s);
*/



//���콺 �Է� ����.
#define MS_STANDBY   0x0000		//�����.
#define MS_LBTNDOWN  0x0100		//��ư ����.
#define MS_LBTNUP	 0x8100		//��ư ������ ������.
#define MS_RBTNDOWN  0x0001		//��ư ����.
#define MS_RBTNUP	 0x0081		//��ư ������ ������.




///////////////////////////////////////////////////////////////////////////////
//
// ���� ��� �Լ�
//
///////////////////////////////////////////////////////////////////////////////

//int ynError(BOOL bBox, TCHAR* msg, ...);
int ynErrorW(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob, TCHAR* filename, char* EntryPoint, char* ShaderModel);
int ynErrorW(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob);
int ynErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...);
int ynErrorW(TCHAR* msg, ...);
 
#define ynError(hr, msg, ...)  \
ynErrorW( __FILEW__, __LINE__, __FUNCTIONW__, TRUE, hr, msg, __VA_ARGS__ )

//#define ynError(hr, msg, pBlob)  ynErrorW( TRUE, hr, msg, pBlob)



///////////////////////////////////////////////////////////////////////////////
//
// ��Ʈ ���� : �Ϲ� �Լ� ����. --> Yena �� ���Ե�
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
int  ynFontCreate(LPDEVICE pDev);
void ynFontRelease();
void ynTextDraw(int x, int y, COLOR col, TCHAR* msg, ...);
*/


/**************** end of "Device.h" ***********************************/