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
// ynCamera.h : 카메라 헤더
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once



/////////////////////////////////////////////////////////////////////////////
//
// class ynCamera : 카메라 운용 클래스.
//
class ynCamera
{
protected:

public:
	ynCamera();
	virtual ~ynCamera();

	virtual void Release();
	virtual int  Create(LPDEVICE pDev);
	virtual void Update(LPDXDC pDXDC);
	virtual void Apply(LPDXDC pDXDC);
	virtual void Clear();
	
};






//////////////////////////////////////////////////////////////////////////////// 
//
// 카메라 전역 데이터들.
//
//////////////////////////////////////////////////////////////////////////////// 
namespace YENA
{

// 카메라 데이터들.
extern VECTOR	yvCamEye;
extern VECTOR	yvCamLookAt;
extern VECTOR	yvCamUp;
extern VECTOR	yvCamHorz;
extern VECTOR	yvCamView;

//카메라 활성화 판정 플래그 : 내장 카메라를 사용하지 않으면 FALSE.
extern BOOL	ybCamOn;

//카메라 변환 행렬.
//MATRIXA		ymCamView;
//MATRIXA		ymCamProj;

// 카메라 전역 함수 Camera Functions
int  ynCameraSetup();
void ynCameraUpdateMtx();
void ynCameraUpdate(float dTime);
void ynCameraRelease();


// 카메라 설정 정보 출력. 
YENA_API void ynCameraInfo(int& x, int& y, COLOR col);



} using namespace YENA;
 


/****************** end of file "ynCamera.h" ***********************************/



