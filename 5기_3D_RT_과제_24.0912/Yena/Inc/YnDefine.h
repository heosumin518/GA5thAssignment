/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  B3/YENA : 2D/3D ������ ����.						                       // 
//                                                                         //
//  �� �� : Kihong Kim / mad_dog@hanmail.net                               // 
//          Zero-G Tech,Inc http://www.zero-g.kr						   //	
//  �ʱⰳ�� : 2003.12 - 2004.03.                                          //
//  ����ȯ�� : OS - Windows 2k SP4 / Visual Studio 6.0/2003/2005/2008       //
//                  DirectX 9.0b(2003, summer)                             //
//                  DirectX 9.0c(2007, November)                           //
//                  OpenGL 1.x / 2.x                                       //
//             H/W - AMD Athlon XP(Barton) 2500+@2300                      //    
//                   ATI Radeon 9600 Pro                                   //
//                   ABit NF7-II /w SAMSUNG DDR 512MB                      //
//                   SB Live! DE 5.1                                       //
//  �׽�Ʈ ȯ�� : CPU - AMD Athlon										   //	 
//                      AMD Athlonx64 w/q4                                 // 
//                      Intel Pentium III/IV & Celeron                     //
//                      Core2 w/Quad                                       //
//                VGA - ATI Radeon 9xxx Series                             //
//                          Radeon 1xxx Series                             //
//                          Radeon HD 3/4/5/6 Series                       //
//                      nVidia GeForce 2 MX/Ti Series.                     //  
//                             GeForce 3 Ti Series.                        //
//                             GeForce 4 MX/Ti Series.                     //
//                             GeForce FX Series.                          //
//                             GeForce 5/6/7/8/9xxx Series.                //
//                             GeForce 2/4xx GTS Series.                   //
//                             GeForce 7xx GTX Series.                     //
//                             GeForce 2Way-SLI / 3Way-SLI		   	       //
//                OS - Windows 98 / 2K / XP / VISTA / Win7 x32:x64         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//
// ynDefine.h:  ���� ���� Ű���� ������ ������.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. B3Yena / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#pragma warning(disable:4996)


// DX11 Header �� ���̺귯�� --> "Yena.h" �� �̵�.
// ...
 

///////////////////////////////////////////////////////////////////////////////
//
// Ÿ�� �� ��� ������ : ������ ȣȯ�� ��� ���밡 ����.
//
///////////////////////////////////////////////////////////////////////////////

// DX11 ��ġ���� ������.											
//
#ifndef LPDEVICE

//����̽� �� ��ġ (DX11) Windows 7 / Vista
typedef ID3D11Device*			LPDEVICE;
typedef ID3D11DeviceContext*	LPDXDC;
typedef IDXGISwapChain*			LPSWAPCHAIN;
typedef ID3D11RenderTargetView*	LPRTVIEW;
typedef ID3D11DepthStencilView*	LPDSVIEW;

/*//����̽� �� ��ġ (DX11.1, DXGI1.2)  Windows 8 / 7
#include "D3D11_1.h"
typedef ID3D11Device1*			LPDEVICE1;
typedef ID3D11DeviceContext1*	LPDXDC1;
typedef IDXGISwapChain1*		LPSWAPCHAIN1;
*/
/*//����̽� �� ��ġ (DX11.2, DXGI1.3), Windows 8.1
#include "D3D11_2.h"
typedef ID3D11Device2*			LPDEVICE2;
typedef ID3D11DeviceContext2*	LPDXDC2;
typedef IDXGISwapChain2*		LPSWAPCHAIN2;
*/
/*//����̽� �� ��ġ (DX11.3, DXGI1.4), Windows 10
#include "D3D11_3.h"
typedef ID3D11Device3*			LPDEVICE3;
typedef ID3D11DeviceContext3*	LPDXDC3;
typedef IDXGISwapChain3*		LPSWAPCHAIN3;
*/
/*
//����̽� �� ��ġ (DX12) Windows 10 device (cross-platform : PC, tablet, console or phone)
typedef ID3D12Device*			LPDEVICE;
typedef ID3D12DeviceContext*	LPDXDC;
typedef IDXGISwapChain3*		LPSWAPCHAIN;
*/

// ��ġ ���� ���� ����ü. (DX9/11 ���� ȣȯ�� ������)
typedef DXGI_MODE_DESC	  DISPLAYMODE;	//DX11 ����
//typedef DXGI_MODE_DESC1 DISPLAYMODE;	//DX11.1 ����
//typedef D3DDISPLAYMODE DISPLAYMODE;   //DX9 ��.


//���ҽ� �� ����. (DX11)
typedef ID3D11ShaderResourceView* LPSRVIEW;			//���̴� ���ҽ� ��.
typedef LPSRVIEW				  LPTEXTURERV;		//�ؽ��� ���ҽ� ��.
typedef ID3D11Texture2D*		  LPTEXTURE;		//2D �ؽ�ó 
//typedef ID3D11Texture2D1*		  LPTEXTURE1;		//2D �ؽ�ó (DX11.3, Windows 10 �̻�)

typedef ID3D11Buffer*			LPXBUFFER;			//DX ���� �޸� ����. (��������� Vertex/Index/Constant �����͸� ����)
typedef LPXBUFFER				LPVERTEXBUFFER;		//���� ����.
typedef LPXBUFFER				LPINDEXBUFFER;		//�ε��� ����
typedef LPXBUFFER				LPCONSTBUFFER;		//��� ����.
//�Է� ���̾ƿ�
typedef ID3D11InputLayout*		LPLAYOUT;
typedef ID3D11RasterizerState*	LPRASTERSTATE;
//����Ʈ
typedef D3D11_VIEWPORT			VIEWPORT;

//���̴� ����.(DX11)
typedef ID3D11VertexShader*   LPVERTEXSHADER;
typedef ID3D11PixelShader*    LPPIXELSHADER;
typedef ID3DBlob*			  LPXDATA;			//���� ������ ����� ����.(Vertex, Adjacency, material, binary code, etc..)

#endif


//���� ��ü ������.
typedef ID3D11DepthStencilState* LPDEPTHSTENCILSTATE;
typedef ID3D11RasterizerState*   LPRASTERSTATE;
typedef ID3D11BlendState*	     LPBLENDSTATE;
typedef ID3D11SamplerState*      LPSAMPLERSTATE;
//typedef LPDEPTHSTENCILSTATE    LPDSSTATE;
//typedef LPRASTERSTATE			 LPRSTATE;
//typedef LPBLENDSTATE           LPBSTATE;
//typedef LPSAMPLERSTATE         LPSSTATE;


/*
typedef LPD3DXMESH		LPMESH;

typedef D3DMATERIAL9	MTRL;
typedef D3DMATERIAL9*	LPMTRL;
typedef D3DXMATERIAL	XMTRL;
typedef D3DXMATERIAL*	LPXMTRL;

typedef LPD3DXBUFFER	LPBUFF;

#ifndef LPTEXTURE
typedef LPDIRECT3DTEXTURE9	LPTEXTURE;
#endif

typedef LPD3DXFONT		LPDXFONT;
*/

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
#ifndef MATRIX
typedef XMMATRIX		MATRIXA;	//��� : 16����Ʈ ����, SIMD ����. ����/���� ������. "Register Type"
typedef XMFLOAT4X4		MATRIX;		//��� : �Ϲ� ����. SIMD ������, Class ������ �����. "Storage Type"
typedef XMVECTOR		VECTOR;		//4���� ���� : 16����Ʈ ����, SIMD ����. ����/���� ������. "Register Type"
typedef XMFLOAT4		VECTOR4;	//4���� ���� : �Ϲ� ����, SIMD ������, Class ������ �����. "Storage Type"
typedef XMFLOAT3		VECTOR3;
typedef XMFLOAT2		VECTOR2;

//���� Ÿ��: 2����.
//typedef XMCOLOR		COLOR;		// r, g, b, a.  [������ 0~255]
typedef XMFLOAT4		COLOR;		// r, g, b, a.  [�Ǽ��� 0~1.0]
#endif



///////////////////////////////////////////////////////////////////////////////
//
// ��Ÿ �ý��� ���� �Լ� �� ������.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(pBuff) if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; }
#define SAFE_DELETE(pBuff)	if((pBuff)){ delete (pBuff); (pBuff) = NULL; }
#define SAFE_DELARRY(pBuff) if((pBuff)){ delete [] (pBuff); (pBuff) = NULL; }
#endif

#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif 

#ifndef YES_
#define YES_ TRUE
#define NO_  FALSE
#endif

#ifndef YN_OK
#define YN_OK	   0					//���� ����. Ư���� ���� ����.
#define YN_FAIL   -1					//���� �߻�.
#define YN_FAILED(v)   ((v) != YN_OK)	//���� üũ ��ũ��.
#endif
 




 
/****************** end of file "B3Mesh.h" *******************************/