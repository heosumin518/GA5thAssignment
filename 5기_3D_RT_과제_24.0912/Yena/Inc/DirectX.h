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
// DirectX.h: DirectX API 필수 헤더.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. B3Yena / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning(disable:4996)

#include "Windows.h" 

///////////////////////////////////////////////////////////////////////////////
//
// DirectX 표준 라이브러리 
//
///////////////////////////////////////////////////////////////////////////////
// < DirectX 버전별 개발 라이브러리 >
// DX9         : DirectX SDK 2002 Nov.
// DX10.x/DX11 : DirectX SDK 2010 June (DX 독립 배포 마지막 버전)
// DX11.X/DX12 : Windows SDK 8.x  (DX 가 포함되어 배포됨)

// DX 표준 헤더
#include "D3D11.h"					//DX 표준 헤더. (DX 11.0)
//#include "D3D11_1.h"				//DX 표준 헤더. (DX 11.1)
#pragma comment(lib, "D3D11")		//DX 라이브러리 로딩.  D3D11.dll 필요.

//DirectX Math : Dx 수학 라이브러리
//#include "XNAMath.h"				//XNA Math 라이브러리.(ver.2.0) DirectX SDK 2010 June. 대응.
#include "DirectXMath.h"			//XNA Math 라이브러리.(ver.3.0) VS2012 (Windows SDK 8) 이상.
using namespace DirectX;

//DXGI : DirectX Graphics Infrastructure
//DX 의 버전에 독립적인 하드웨어 저수준 접근 방식을 제공합니다.
//#include "DXGI.h"					//DXGI 헤더. (d3d11.h 에 포함됨)
#pragma comment(lib, "dxgi")		//DXGI 라이브러리. DXGI.dll 필요.

//DX 셰이더 운용 라이브러리.
#include "D3DCompiler.h"				//DX 셰이더 컴파일러 헤더.
#pragma comment(lib, "d3dcompiler")		//DX 셰이더 컴파일러 라이브러리.  D3DCompiler.dll 필요.



///////////////////////////////////////////////////////////////////////////////
//
// DirectX 확장 라이브러리 : DirectX Extension
//
// DX 확장 라이브러리 및 유틸리티 모음집.
// 사용시 프로젝트/속성에 추가설정이 필요합니다.
// 각 확장 라이브러리 사용법을 참조. 
//
///////////////////////////////////////////////////////////////////////////////

// ■ DirectXTK  (ver. February 7, 2018)
// DX 유틸리티 및 일반화 라이브러리 모음집. 
// DX 사용시 필요한 여러 기능 클래스 및 서비스를 제공합니다.
// 설명1 :  DXTK/ReadMe.txt
// 설명2 :  Yena/DXTK/DirecrtXTK 사용법.txt
// 참조 : https://blogs.msdn.microsoft.com/chuckw/2012/03/02/directxtk/
//
#include "DXTK.h"



// ■ DirectXTex  (ver. February 9, 2018)
// DX 텍스처 유틸리티 및 확장 라이브러리 모음집.  
// DX 텍스처 운용시 필요한 여러 기능 클래스 및 서비스를 제공합니다.
// 설명1 :  DXTex/ReadMe.txt
// 설명2 :  Yena/DXTex/DirecrtXTex 사용법.txt
// 참조 : https://blogs.msdn.microsoft.com/chuckw/2011/10/27/directxtex/
// 
#include "DXTex.h"								//프로젝트 속성설정 필요. DirectXTex 사용법.txt 참조.

/*// 프로젝트 속성설정없이 수동접근, 사용하는 경우 
#include "../../Yena/DXTex/Inc/DirectXTex.h"		//구형 배포버전과의 프로젝트 호환성을 위해 상대경로 지정.
#ifdef _DEBUG
#pragma comment(lib, "../Yena/DXTex/Lib/DirectXTexd.lib")
#else
#pragma comment(lib, "../Yena/DXTex/Lib/DirectXTex.lib")
#endif
*/


// ■ DXErr  (ver. Nov, 2015)
// DirectX 에러 메제지 처리 라이브러리
// HRESULT 값을 문자열로 출력합니다.
// 참조 : https://blogs.msdn.microsoft.com/chuckw/2012/04/24/wheres-dxerr-lib/
//
#include "DXErr.h"


 
/****************** end of file "DirectX.h" *******************************/