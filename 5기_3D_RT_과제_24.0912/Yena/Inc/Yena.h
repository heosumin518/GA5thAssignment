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
// Yena.h: ���� �ʼ� ���.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. Yena / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning(disable:4996)

///////////////////////////////////////////////////////////////////////////////
//
// �÷��� �� �׷��� API ���̺귯�� ����.
//
///////////////////////////////////////////////////////////////////////////////
#include "Windows.h" 
#include "DirectX.h"		//DX ���� ���� ���.
//#include "DirectXTK.h"	//DirectX Toolkit ���. 
//#include "OpenGL.h"		//GL ���. 
#include "Tchar.h"


///////////////////////////////////////////////////////////////////////////////
//
// Yena ���� / ���̺귯�� ���� ���  
//
///////////////////////////////////////////////////////////////////////////////
#include "Yena_API.h"			//���� ���̺귯�� Load �� Import ó��.
#include "ynDefine.h"			//���� �ڷ�/Ŭ����/���� ������.
#include "ynVersion.h"			//���� ����.
#include "YnError.h"			//���� �޼��� ����.
#include "YnDevice.h"			//������ ��ġ ���.
#include "ynFont.h"				//��Ʈ ��� Ŭ����.
#include "ynMath.h"				//���� ���̺귯�� Ȯ��.
#include "ynCamera.h"			//ī�޶� Ŭ����.
#include "ynStateObject.h"		//���°�ü Ȯ�� Ŭ����.
#include "ynShader.h"			//���̴� Ȯ�� Ŭ����.
#include "ynGrid.h"				//�׸��� Ŭ����.
#include "ynAxis.h"				//���� �� Ŭ����.
#include "ynMesh.h"				//�⺻ �޽� Ŭ����.
#include "ynTex.h"				//�ؽ�ó ���.



///////////////////////////////////////////////////////////////////////////////
//
// ���� ���� ���� �����͵�.
//
///////////////////////////////////////////////////////////////////////////////

// Yena ���� ���� ���� ���ӽ����̽� 
//
namespace YENA
{
	extern YENA_APIX LPDEVICE	  ypDevice; 
	extern YENA_APIX LPDXDC		  ypDXDC;
	extern YENA_APIX DISPLAYMODE  ynMode;

	//extern DWORD	ynVersion;		//����.
	//extern TCHAR*	ypBuild;		//���� ����.

	//��ȯ ���� : ������ ������ "����" ī�޶� ����.
	extern YENA_APIX MATRIXA  ymView;		//�� ��ȯ ���.
	extern YENA_APIX MATRIXA  ymProj;		//���� ��ȯ ���.

	//���� ���� ����ȭ ��ġ
	extern ynAxis* ypAxis;			//������.(����)
	extern ynGrid* ypGrid;			//�׸���.(����) 

}
using namespace YENA;


//---------------------------------------------------
// ���� �ý��� �⺻���� ������ (System Value)  ������.
//---------------------------------------------------
// �� ��ü�� �ʱ� ���� �õ��� �����Ǹ�, ���� ��������� ���ŵ˴ϴ�. 
// �� ��ü�� ��ü ������ �ý����� ���������� �����ϱ� ���� ���� �������� ���Ǵ� ���� ������ �Դϴ�. 
// �� ��ü�� �⺻������ 1���� �����ϸ� �ʿ�� ����ڰ� �߰� �� �� �ֽ��ϴ�.
// �� ��ü ��������� �����Ͻʽÿ�.
//
#define SV_Dev		 YENA::ypDevice		// ��ġ/����̽� ��ü
#define SV_DC		 YENA::ypDXDC		// ����̽� ���ؽ�Ʈ ��ü
#define SV_SwapChain YENA::ypSwapChain	// ����ü�� ��ü.
#define SV_Mode   YENA::ynMode			// �����ġ ���� ����.
#define SV_AA     YENA::ynAA
#define SV_AF     YENA::ynAF
#define SV_View   YENA::ymView			// ����ī�޶� - �� ��ȯ ���.	
#define SV_Proj   YENA::ymProj			// ����ī�޶� - ���� ��ȯ ���.
//#define SV_Ver	YENA::ynVersion
//#define SV_Build  YENA::ypBuild
#define SV_Font	  YENA::ypFont			// �⺻ ��Ʈ.(�ѱۿϼ���,ũ��:9)
#define SV_FXLine YENA::ypFXLine		// �⺻ ���̴�(���ο�)

// ���� ȣȯ�� ���� ������.
#define yn_Dev		  SV_Dev
#define yn_DC		  SV_DC
#define yn_SwapChain  SV_SwapChain
#define yn_Mode    SV_Mode
#define yn_AA	   SV_AA
#define yn_AF	   SV_AF
#define yn_View    SV_View
#define yn_Proj    SV_Proj
#define yn_Font	   SV_Font
#define yn_FXLine  SV_FXLine




///////////////////////////////////////////////////////////////////////////////
//
// ���� �� �ý��� �Լ���
//
///////////////////////////////////////////////////////////////////////////////
namespace YENA {


//-----------------------------------------------------------------------------
//
// ���� ��� �Լ��� : ���� �ʱ� ���� �� ���� ��ü ����.
//
//-----------------------------------------------------------------------------
// Yena ���� ���� : �ʱ� ���۽� 1ȸ ȣ��.
// ������ġ ���� : Camera, Grid/Axis, FX, Timer ��. 
// ���°�ü ���� : Depth-ON, Stencil-OFF, AB_OFF, Fill+Cull(CCW), Clamp mode
// pDev : ��ġ �ڵ�.
// Mode : ��ġ / ��� ����.
YENA_API int  YenaSetup(LPDEVICE pDev, DISPLAYMODE& Mode, 
					    float GridOffset = 10.0f, float GridSize = 100.0f,
						float AxisLength = 5.0f ); 

// Yena ���� ���� : �� �����Ӹ��� 1ȸ ȣ��.
// ������ġ ���� : Camera, Grid/Axis, FX, Timer ��.
// ���°�ü ���� : Depth-ON
YENA_API void YenaUpdate(float dTime);						

//Yena ���� ���� : ����� 1ȸ ȣ��.
YENA_API void YenaRelease();								

// �ý��� ���� / ���� ���.
YENA_API void YenaSysInfo(int& x, int& y, COLOR col);		//���� ����.
YENA_API void YenaSysInfoEx(int& x, int& y, COLOR col);		//�� ����.



//-----------------------------------------------------------------------------
//
// ī�޶� �ý���
//
//-----------------------------------------------------------------------------
//MATRIXA ynCameraView();			//���� ī�޶� �� ��ȯ ��� ���.
//MATRIXA ynCameraProj();			//���� ī�޶� ���� ��ȯ ��� ���.



//-----------------------------------------------------------------------------
//
// �׸��� & ������ ǥ�ð���. 
//
//-----------------------------------------------------------------------------
// ��-�׸��� ��ü ����. (���� �ʱ⼳���� �ڵ�ȣ��)
// pDev : ��ġ �ڵ�.
// bGridOn : �׸��� ��ü ���� ����. �⺻�� true.
// bAxisOn : ������ ��ü ���� ����. �⺻�� true. 
YENA_API void ynGridAxisCreate(LPDEVICE pDev, BOOL bGridOn = TRUE, BOOL bAxisOn = TRUE, 
							   float GridOffset = 10.0f, float GridSize = 100.0f,
							   float AxisLength = 5.0f );

// ��-�׸��� ��ü ����. (���� ���Ž� �ڵ�ȣ��) 
YENA_API void ynGridAxisRelease();

// ��-�׸��� ���� on/off ���� 
// ynGridAxisUpdate �� ������ ������, ���õ�. 
YENA_API void ynGridAxisState(BOOL bGridOn = TRUE, BOOL bAxisOn = TRUE);

// ��-�׸��� ���� ����
// F2 : �׸��� (ON/OFF), ��ĭ�� ũ��� 10.0 x 10.0f 
// F3 : ������ (ON/OFF) 
// �� ����� ������� �������� �ش� �ڵ带 �ּ�ó�� �Ͻʽÿ�.  
YENA_API void ynGridAxisUpdate(float dTime);

//��-�׸��� �׸��� : ynGridAxisUpdate �� ������ ������, ���õ�.
YENA_API void ynGridAxisDraw(float dTime = 0);



//-----------------------------------------------------------------------------
//
// ���ڿ� ��� 
//
//-----------------------------------------------------------------------------
//
// ynTextDraw   :  ������ ȭ����ǥ�� ���ڿ��� ����մϴ�.
//
// �Ķ���� : x,y = ȭ����ǥ.   
//            col = ��»���  
//            msg = ����� ���ڿ�(+����ȭ�ȹ��ڿ� ����), 
//					
// ��� �ִ� ���ڼ��� 2048. ���ڼ��� �ѱ�� overflow ���� �߻�.
// ��� ������ ũ�� : 6x12 (pixel)
//
YENA_API void  ynTextDraw(int x, int y, COLOR col, TCHAR* msg, ...);



//  ynTextDrawSV   : ������ ȭ����ǥ�� ���������� ���ڿ��� ����մϴ�.
//                   
// �Ķ���� : x,y = ȭ����ǥ.   
//            col = ��»���  
//            msg = ����� ���ڿ� 
//            dTime = ���� ��� �ӵ�ó���� ���� ��� �ð��� (1sec/1000ms)
//					
// ���ڸ� �����ӵ��� ���������� ����մϴ�.  
// ���� 80���� �̻�, �Ǵ� ���๮��(CR_LF) �� ������ ��� ���� �ٷ� �ڵ����� �������ϴ�.   
// ���� 80 ���� ���� 40�� ���� ǥ���� ���ֽ��ϴ�.
// ��� �ִ� ���ڼ��� 4096. ���ڼ��� �ѱ�� overflow ���� �߻�. 
// ��� ������ ũ�� : Size 9 ����. ������ 6x12 (pixel).
//
YENA_API void ynTextDrawSV(int x, int y, COLOR col, TCHAR* msg, float dTime);



// ynError :  ���� �޼��� ��� �Լ�.
//            �Էµ� �����ڵ带 ���ڿ��� ��ȯ, �޼��� �ڽ����·� ����մϴ�.
//            ����� ���̶��, Visual Studio �� ���â�� ���� ������ ����մϴ�.
//            DXErr.h �� �ʿ��ϴ�.
//
// �Ķ���� : bBox = �޼����ڽ� ��� ����. 
//           msg = ����� ���ڿ�
//           hr =  HRESULT ��, ���ڿ��� ��ȯ�� ��µ˴ϴ�.
//           pBlob = ���� �޼����� ���� ���� ������.
//
YENA_API int ynError(BOOL bBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob);




} // end of namespace YENA

/****************** end of file "Yena.h" *******************************/