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
// ynError.h: 에러처리 헤더.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. ynError / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning(disable:4996)
 

/////////////////////////////////////////////////////////////////////////////
//
//  에러처리 전역 데이터들.
//  

namespace YENA
{
	//....
}




//////////////////////////////////////////////////////////////////////////////// 
//
// 에러처리 전역 함수들
//
//////////////////////////////////////////////////////////////////////////////// 

// ynError :  에러 메세지 출력 함수.
//            입력된 에러코드를 문자열로 전환, 메세지 박스형태로 출력합니다.
//            디버깅 중이라면, Visual Studio 의 출력창도 동일 내용을 출력합니다.
//            DXErr.h 가 필요하다.
//
// 파라미터 : bBox = 메세지박스 출력 여부. 
//           msg = 출력할 문자열
//           hr =  HRESULT 값, 문자열로 전환후 출력됩니다.
//           pBlob = 에러 메세지를 담은 버퍼 포인터.
//
YENA_API int ynError(BOOL bBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob);




/*
int  ynErrorCreate(LPDEVICE pDev);
void ynErrorRelease();
void ynErrorUpdate(float dTime);
void ynErrorDraw(float dTime);
*/






/****************** end of file "ynError.h" *******************************/