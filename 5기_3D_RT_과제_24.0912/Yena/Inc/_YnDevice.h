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
//
// Device.h : D3D 기본 헤더 파일.
//
// 2003.11.4. Mad dog / SonimJJang
// 2004.05.07. Update
// 2005.09.23. Update (.Net)
//
#pragma once
#pragma warning(disable:4996)

#include "d3dx9.h"
#include "dxerr.h"
#include "stdio.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr.lib")


//HRESULT	ynDeviceSetup(HWND hwnd);
//void	ynDeviceRelease();

//void SetupMatrices();

//2004.05.07. 추가.
//void  InitAnimation();
//void  Animate(float dTime);

void  CameraSetup();

//void  CameraUpdate(float dTime);
//void  LightUpdate(float dTime);

//float ynGetElapseTime();
//void  ynPutFPS(int x, int y);
//void  DrawText( int x, int y, D3DXCOLOR col, char* msg, ...);

//void  ynSystemUpdate(float dTime);
//void  ynSystemInfoShow(); 

/*
#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif


#ifndef SAFE_RELEASE
#define SAFE_RELEASE(pBuff) if((pBuff)){ (pBuff)->Release(); (pBuff) = NULL; }
#define SAFE_DELETE(pBuff)	if((pBuff)){ delete (pBuff); (pBuff) = NULL; }
#define SAFE_DELARRY(pBuff) if((pBuff)){ delete [] (pBuff); (pBuff) = NULL; }
#endif


#define YES_ TRUE
#define NO_  FALSE
*/

//extern LPDIRECT3DDEVICE9		yng_pDevice;
//extern BOOL yng_bWireFrame;
//extern LPD3DXFONT yng_pFont;
//extern LPD3DXFONT yng_pFont;
//extern D3DDISPLAYMODE yng_Mode; 


//extern LPDIRECT3DVERTEXBUFFER9	g_pVB;			//정점 버퍼용 포인터.
//extern LPDIRECT3DINDEXBUFFER9	g_pIB;				//인덱스 버퍼용 포인터.

extern HWND g_hWnd;

// 카메라 데이터들.
//
extern D3DXVECTOR3		yng_vCamEye;
extern D3DXVECTOR3		yng_vCamLookAt;
extern D3DXVECTOR3		yng_vCamUp;
extern D3DXMATRIX		yng_mCamView;
extern D3DXVECTOR3		yng_vCamHorz;
extern D3DXVECTOR3		yng_vCamView;


// 설정파일 로드.
bool LoadInitData(char* pPath, char* pMeshName );



////////////////////////////////////////////////////////////////////////////
//
// 추가 확장 헤더들.
//
//#include "B3Mesh.h"	
//#include "Unit.h"	
#include "assert.h"
//#include "B3Yena/BGrid.h"
//#include "B3Yena/BAxis.h"
#include "BGrid.h"
#include "BAxis.h"
#include "BFont.h"

extern B3Grid* yng_pGrid;
extern B3Axis* yng_pAxis;


void GridAxisCreate();
void GridAxisRelease();
//void ynGridAxisUpdate();
//void ynGridAxisDraw();




// 윈도우 창 생성 및 크기 조절..
//
//void ynResizeWindow(HWND hWnd, UINT NewWidth, UINT NewHeight);

/**************** end of "Device.h" ***********************************/