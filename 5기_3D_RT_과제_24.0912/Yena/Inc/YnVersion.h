/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  B3D/YENA : DX / GL 하드웨어 가속 2D/3D 엔진.                           //
//                                                                         //
//  제 작 : Kihong Kim / mad_dog@hanmail.net							   //
//         Zero-G Interactive  http://www.Zero-G.kr                        //
//                                                                         //
//  초기제작 : 2003.12 - 2004.03.                                          //
//  개발환경 : OS - Windows 2k SP4 / Visual Studio 6.0/2003/2005           //
//                  DirectX 9.0b(2003, summer)                             //
//             H/W - AMD Athlon XP(Barton) 2500+@2300                      //    
//                   ATI Radeon 9600 Pro                                   //
//                   ABit NF7-II /w SAMSUNG DDR 512MB                      //
//                   SB Live! DE 5.1                                       //
//  테스트 환경 : CPU - AMD Athlon										   //	 
//                      AMD Athlonx64 w/q4                                 // 
//                      Intel Pentium III/IV & Celeron                     //
//                            Core2 w/Quad                                 //
//                VGA - ATI Radeon 9xxx Series                             //
//                          Radeon 1xxx Series                             //
//                          Radeon HD 3/4 Series                           //
//                      nVidia GeForce 2 MX/Ti Series.                     //  
//                             GeForce 3 Ti Series.                        //
//                             GeForce 4 MX/Ti Series.                     //
//                             GeForce FX Series.                          //
//                             GeForce 5/6/7/8/9 Series.                   //
//                             GeForce SLI / 3Way-SLI					   //
//                OS - Windows 98 / 2K / XP / VISTA / Win7 x32:x64         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// BVersion.h: 버전 정보 판독 소스 
//
// 2004.06. Kihong Kim / mad_dog@hanmail.net / Blue3D / Hero3D
// 2004.10. update.
// 2010.02. Update. B3Yena / Zero-G Interactive
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

 

// --> B3Yena.h 에서 처리.
namespace YENA {

//////////////////////////////////////////////////////////////////////////
//
// Version 관련. 
//
//////////////////////////////////////////////////////////////////////////
//
//extern B3DAPI_ TCHAR ysEngineVer[12];
//extern B3DAPI_ TCHAR g_B3BuildVer[12]; 

//버전 출력.(간단)
//YENA_API TCHAR* ynVersion();		//버전 출력. (ex. YENA.2.0A 11.0622)
//YENA_API TCHAR* ynCopyRight();		//저작권 출력. (ex. Copyright @ 2004-2017 Zero-G Interactive)

//버전 출력 개별 버전.
YENA_API TCHAR* ynGetEngineVersion();		//engine ver. (ex. YENA.2.0A)
YENA_API TCHAR* ynGetBuildVersion();		//Build ver. (11.06.22 time )
YENA_API TCHAR* ynGetProductVersion();		//Product Version. (ex 1.00 )
//YENA_API TCHAR* B3D_GetBuildVersion();	//build date. (ex. Apr 25 2005)
YENA_API TCHAR* ynGetTimeStamp();			//build time stamp (ex. Mon Apr 25 16:40:45 2005)
YENA_API TCHAR* ynGetCopyRight();			//(ex. Copyright @ 2004-2017)
YENA_API TCHAR* ynGetCompanyName();			//Company Name (Zero-G Interactive, Inc)



//////////////////////////////////////////////////////////////////////////
//
// BOOL ynVersionLoad(...); 
// 
// 엔진 버전을 얻는다. : 2005.04.25.
// 버전 정보는 Yena.dll , YenaRes.dll 에 기록된 버전정보를 읽어오게 됩니다.
// 
// 성공時 TRUE 를 리턴하며,  pFileVersion, pProductVersion 에 버전정보가 저장됩니다.  
// 실패時에는 FALSE 리턴.
//
// 2005.08.21. : 만약 인자가 NULL 이면, 그 인자는 무시됩니다.
//             : 함수내부에서는 버전관리용 전역데이터를 초기화 합니다.
//
YENA_API BOOL ynVersionLoad(TCHAR* pFileVersion = NULL, TCHAR* pProductVersion = NULL, TCHAR* pCopyrights = NULL);



} //end of namesapce YENA



/***************** end of file "BVersion.h" ********************************/
