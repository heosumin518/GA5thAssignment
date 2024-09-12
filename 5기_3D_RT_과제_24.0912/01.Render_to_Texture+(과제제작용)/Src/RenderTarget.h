//
// RenderTarget.h : 렌더타겟 운용 헤더
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once


//extern BOOL g_bRTargetOn;
//extern BOOL g_bRTargetUpdate;
//extern BOOL g_bShowScene;

int  RenderTargetCreate();
void RenderTargetSceneGenerate();
void RenderTargetDraw();
void RenderTargetRelease();


/****************** end of file "RenderTarget.h" ***********************************/