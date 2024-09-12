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
// YnDevice.h : ������ ����̽� �¾� �ڵ�.
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
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
//... Yena.h ���� ó��.

///////////////////////////////////////////////////////////////////////////////
//
// DirectX Toolkit : DX Ȯ�� ���̺귯��  --> Yena �� ���Ե�.
//
///////////////////////////////////////////////////////////////////////////////
//... Yena.h ���� ó��.


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
//... ynDefine.h ���� ó��.


///////////////////////////////////////////////////////////////////////////////
//
// DX �� ��ġ ���� ���� �����͵�  
//
///////////////////////////////////////////////////////////////////////////////
namespace YENA {

YENA_API int	ynDXSetup(HWND hwnd);
YENA_API void	ynDXRelease();

YENA_API int	ynClearBackBuffer(UINT flag, COLOR col, float depth = 1.0f, UINT stencil = 0);
YENA_API int    ynFlip();

YENA_API float	ynGetEngineTime();
YENA_API void	ynPutFPS(int x, int y);
//void  ynTextDraw( int x, int y, COLOR col, char* msg, ...);

// ����/�ý��� ���� ��ġ ���� : ī�޶�, ����, ��������ȭ ��� ���. --> YenaUpdate ���� ó����.
//YENA_API void	 ynSystemUpdate(float dTime);

// ���� DX ��� ���� ���ϱ�. 
YENA_API TCHAR*	ynGetFeatureLevelStr(D3D_FEATURE_LEVEL fv);
YENA_API void	ynGetFeatureLevel(D3D_FEATURE_LEVEL* fv);

// ����/������ ��ġ ���� ȹ��.
//YENA_API void	 ynGetDeviceInfo();
YENA_API HRESULT ynGetAdapterInfo(DXGI_ADAPTER_DESC1* pAd);

// ����/�ý��� ���� �� ���� ���.
//YENA_API void	 ynSystemInfo(int x, int y, COLOR col);
YENA_API void	 ynAdapterInfo(int x, int y, COLOR col);		//GPU ���� ��.
YENA_API void	 ynDeviceInfo(int& x, int& y, COLOR col);		//������ ��ġ ����.(����)
YENA_API void	 ynGraphicsInfo(int& x, int& y, COLOR col);		//�׷��� ���� ����.(����)



//���� �ý��� ���� �����͵�.
extern YENA_APIX LPDEVICE       ypDevice;
extern YENA_APIX LPDXDC			ypDXDC;
extern YENA_APIX LPSWAPCHAIN    ypSwapChain;
extern YENA_APIX LPRTVIEW		ypRTView;
extern YENA_APIX LPDSVIEW		ypDSView;
extern YENA_APIX LPTEXTURE		ypDS;


// ��ġ ���� ���� ����ü. (DX9/11 ���� ȣȯ�� ������) --> YnDefine.h �� �̵�.
//typedef DXGI_MODE_DESC  DISPLAYMODE;	//DX11 ���� 
//typedef DXGI_MODE_DESC1 DISPLAYMODE;	//DX11.1 ����
//typedef D3DDISPLAYMODE DISPLAYMODE;   //DX9 ��.

extern YENA_APIX DISPLAYMODE ynMode;
//extern YENA_APIX HWND yhWnd;
//extern BOOL ybShowFrame;

//���� ����̽� DX ��� ����.
extern TCHAR* ysFeatureLevel;

extern DWORD ynAA;
extern DWORD ynAF;
extern BOOL  ybMipMap;


//��üȭ�� ��뿩��.
extern BOOL ybWindowMode;
//��������ȭ ��뿩��.
extern BOOL ybVSync;







///////////////////////////////////////////////////////////////////////////////
//
// ������ ���� ��ü ����  
//
///////////////////////////////////////////////////////////////////////////////
//... Yena.h ���� ó��. 




///////////////////////////////////////////////////////////////////////////////
//
// ��Ÿ �ý��� ���� �Լ� �� ������.
//
///////////////////////////////////////////////////////////////////////////////
//... Yena.h ���� ó��. 



///////////////////////////////////////////////////////////////////////////////
//
// ���� ��� �Լ� : DXext/DXErr ���̺귯�� ���.
//
///////////////////////////////////////////////////////////////////////////////
//... ynError.h ���� ó��. 





///////////////////////////////////////////////////////////////////////////////
//
// ��Ʈ ���� : Yena ���
//
///////////////////////////////////////////////////////////////////////////////
//... Yena.h ���� ó��. 




}// end of namespace YENA

/**************** end of "YnDevice.h" ***********************************/