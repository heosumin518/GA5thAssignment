/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  B3/YENA : 2D/3D 렌더링 엔진.						                       // 
//                                                                         //
//  제 작 : Kihong Kim / mad_dog@hanmail.net                               // 
//          Zero-G Tech,Inc http://www.zero-g.kr						   //	
//  초기개발 : 2003.12 - 2004.03.                                          //
//  개발환경 : OS - Windows 2k SP4 / Visual Studio 6.0/2003/2005/2008       //
//                  DirectX 9.0b(2003, summer)                             //
//                  DirectX 9.0c(2007, November)                           //
//                  OpenGL 1.x / 2.x                                       //
//             H/W - AMD Athlon XP(Barton) 2500+@2300                      //    
//                   ATI Radeon 9600 Pro                                   //
//                   ABit NF7-II /w SAMSUNG DDR 512MB                      //
//                   SB Live! DE 5.1                                       //
//  테스트 환경 : CPU - AMD Athlon										   //	 
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
// YnFont.h: 폰트 헤더.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. YnFont / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning(disable:4996)
 


 
///////////////////////////////////////////////////////////////////////////////
//
// 폰트 전역 함수들 : 비클래스 함수 버전.
//
///////////////////////////////////////////////////////////////////////////////
namespace YENA {
	

int  YnFontCreate(LPDEVICE pDev);
void YnFontRelease();
YENA_API void YnDrawText(int x, int y, COLOR col, TCHAR* msg, ...);

// DX10 용 폰트 객체
//ID3DX10Font* ypFont = NULL;
// DX9 용 폰트. 2004.10.21. 
//LPD3DXFONT	ypFont;

} // end of namespace YENA


 
///////////////////////////////////////////////////////////////////////////////
//
//  폰트 전역 데이터들.
//  
namespace YENA
{
	// DirectXTK : Sprite Font 클래스
	extern SpriteFont* ypFont;
	//SpriteBatch* ypFontBatch;
}
using namespace YENA;




/****************** end of file "YnFont.h" *******************************/