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
// ynGrid.cpp : 그리드 / 격자 시스템 헤더
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. B3Yena / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////
#pragma once

//#include "Device.h"
#include "ynDefine.h"
//#include "d3dx9.h"
//#include "dxerr.h"
//#include "stdio.h"
#include "assert.h"
 


//////////////////////////////////////////////////////////////////////////////
//
// class ynGrid
//
//////////////////////////////////////////////////////////////////////////////

class ynGrid
{
public:

	struct VTX_GRID {
			VECTOR3 vPos;
			COLOR	Color;
	};

	struct GRIDINFO {
		int width; 
		int height;
		float scale;
		COLOR color;
		DWORD res[20];

		GRIDINFO(int w = 100, int h = 100, float s = 1.0f, 
				COLOR c = COLOR(0.4f, 0.4f, 0.4f, 1.0f))
				:width(w), height(h), scale(s), color(c){}				
	};

	//정점 구조.
	//enum { FVF_GRID = D3DFVF_XYZ | D3DFVF_DIFFUSE, };

	

protected:

	LPDEVICE		m_pDev;
	LPVERTEXBUFFER	m_pVB;
	VTX_GRID*		m_pGrids;			//그리드 정적 버퍼.
	DWORD			m_TotByte;			//그리드 정점 버퍼 크기 (전체 바이트 수)
	UINT			m_VtxCnt;			//전체 정점 개수.
	UINT			m_uLineCnt;			//전체 라인 개수.

	MATRIX			m_mTM;
	
	GRIDINFO		m_Info;

protected:  //for DX11
	//LPLAYOUT		m_pLayout;			//정점 레이아웃(DX11) -> ynShader 에서 처리.

protected:

	int		CreateVtxData(GRIDINFO* pInfo);		//정점 데이터 구성.
	HRESULT CreateVB(void* pBuff, UINT size);	//정점버퍼 생성.
	//HRESULT CreateLayout();					//레이아웃 생성 -> ynShader 에서 처리.
	//int		ShaderLoad();					//셰이더 생성. -> ynShader 에서 처리.

	void Release();

public:

	int Init(LPDEVICE pDev, GRIDINFO* pGrid);
	
	virtual int Update(LPDEVICE pDev = NULL, GRIDINFO* pGrid  = NULL);
	virtual int Render(float dTime, float sx = 0.0f, float sy = 0.0f);


	const GRIDINFO& GetInfo() const { return m_Info; }


	ynGrid(void);
	virtual ~ynGrid(void);

};




/***************** end of file "Grid.h" ********************************/
