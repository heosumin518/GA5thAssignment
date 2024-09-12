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
// ynMath.h: 수학 헤더.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. ynMath / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning(disable:4996)
 

/////////////////////////////////////////////////////////////////////////////
//
//  수학 전역 데이터들.
//  

namespace YENA
{
	//....
}




//////////////////////////////////////////////////////////////////////////////// 
//
// 연산자 오버로딩.
//
//////////////////////////////////////////////////////////////////////////////// 
// 수학 라이브러리 확장 : 네임스페이스와 전역연산자와의 모호성 문제로 
// 연산자 오버로딩은 전역으로 처리함.
//
YENA_APIX VECTOR3 operator * (VECTOR3& lhs, float s);
YENA_APIX VECTOR3 operator + (VECTOR3& lhs, VECTOR3& rhs);

// 색상 * 스칼라 곱  (ex)  col = col * 0.5f
YENA_APIX COLOR operator * (COLOR& lhs, float s);


//////////////////////////////////////////////////////////////////////////////// 
//
// 수학 전역 함수들
//
//////////////////////////////////////////////////////////////////////////////// 
namespace YENA	{


YENA_APIX MATRIX ynMatrixIdentity();

/*
int  ynMathCreate(LPDEVICE pDev);
void ynMathRelease();
void ynMathUpdate(float dTime);
void ynMathDraw(float dTime);
*/


} // end of namespace YENA


/****************** end of file "ynMath.h" *******************************/