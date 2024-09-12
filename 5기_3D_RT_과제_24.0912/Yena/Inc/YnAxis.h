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
/////////////////////////////////////////////////////////////////////////////
//
// ynAxis.h: 그리드 클래스 관련 헤더 파일.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. B3Yena / DX11.1 (Windows SDK 8.1)
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

//#include "../engine/include/b3x.h"
#include "ynDefine.h"
//#include "assert.h"
//#include "../define.h"
//#include "../Engine/Bfont.h"
//#include "../Engine/BLog.h"



//////////////////////////////////////////////////////////////////////////////
//
// class ynAxis
//
//////////////////////////////////////////////////////////////////////////////

class ynAxis
{
public:

	struct VTX_AXIS {
			Vector3 vPos;
			COLOR	Color;
	};

	struct AXISINFO {
		//int width; 
		//int height;
		float length;		//각 막대의 길이.
		float scale;
		//COLOR color;
		DWORD res[20];

		AXISINFO(float len = 1.0f, float s = 1.0f ):length(len), scale(s){} 
	};

	//정점 구조.
	//enum {	FVF_AXIS = D3DFVF_XYZ | D3DFVF_DIFFUSE	};	 

protected:

	LPDEVICE		m_pDev;
	LPVERTEXBUFFER	m_pVB;
	VTX_AXIS*		m_pVtxs;			//변환본.
	Vector3			m_vTextPos[3];		//방향축 텍스트 출력위치 (Screen)

	Matrix			m_mTM;				//카메라의 역행로 처리...
	DWORD			m_TotByte;			//그리드 정점 버퍼 크기 (전체 바이트 수)
	UINT			m_VtxCnt;			//전체 정점 개수.
	//UINT			m_uLineCnt;
	VIEWPORT		m_ViewPort;
	BOOL			m_bUseViewPort;
	MATRIX			m_mOrthoProj;

	AXISINFO		m_Info;

protected:  //for DX11
	//LPLAYOUT		m_pLayout;			//정점 레이아웃(DX11) -> ynShader 에서 처리.

protected:

	int		CreateVtxData(AXISINFO* pInfo);		//정점 데이터 구성.
	HRESULT CreateVB(void* pBuff, UINT size);	//정점버퍼 생성.
	//HRESULT CreateLayout();					//레이아웃 생성 -> ynShader 에서 처리.
	//int		ShaderLoad();					//셰이더 생성. -> ynShader 에서 처리.

 	void Release();
	
public:

	int Init(LPDEVICE pDev, AXISINFO* pAi, DWORD width, DWORD height);
	
	 
	//방향축 갱신 : 카메라에 고정
	virtual int Update(LPDEVICE pDev, VECTOR3 vCamEye, VECTOR3 vCamNView, 
					  DWORD width  = 0 , DWORD height  = 0);		
	//방향축 갱신 : 위치에 고정하기.
	virtual int Update(LPDEVICE pDev, VECTOR3 vPos, DWORD width  = 0 , DWORD height  = 0 );		
	 
	virtual int Render(LPDEVICE pDev, 
						//B3Font* pFont,  
						//LPDXFONT pFont,
						DWORD width, DWORD heigh );

	ynAxis(void);
	virtual ~ynAxis(void);

};


/***************** end of file "Axis.h" ********************************/
