//
// Map.h : 지형 헤더
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once

 

int  MapCreate();
void MapRelease();
void MapUpdate(float dTime = 0);
void MapDraw(float dTime = 0);



/****************** end of file "Map.h" ***********************************/