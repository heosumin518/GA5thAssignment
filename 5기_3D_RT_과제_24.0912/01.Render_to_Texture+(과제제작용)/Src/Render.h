//
// Render.h : 기본 렌더링/프레임웍 헤더
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once

 
int		DataLoading();
void	DataRelease();

void	SceneRender();	

// 장면 그리기 (부분) : 렌더타겟 / 그림자멥 생성을 위한 공유함수.
void	_SceneRender();


//extern BOOL g_bBBoxOn;			//경계상자 표시(모델 전체) 
//extern BOOL g_bBBoxNOn;			//곙계상자 표시(노드별) 

//스텐실 상태 처리.
extern BOOL g_bStencilOn;		
extern BOOL g_bShowCutOff;		

//렌더링 상태 변수들.
extern BOOL	g_bShowHelp; 
//extern BOOL	g_bWireFrame;		//렌더링 모드 : 와이어프레임/솔리드 선택.
extern BOOL	g_bSpecOn; 
extern BOOL	g_bShadowOn; 



/****************** end of file "Render.h" ***********************************/