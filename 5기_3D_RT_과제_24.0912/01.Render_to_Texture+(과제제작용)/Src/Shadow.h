//
// Shadow.h : 기본 렌더링/프레임웍 헤더
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once


extern XMMATRIX g_mShadow;


void ShadowON();
//void ShadowDraw(LPMODEL pModel, float dTime =0);
void ShadowOFF();


void StencilON();
void StencilOFF();
void DepthOFF();
void DepthON();


/****************** end of file "Shadow.h" ***********************************/