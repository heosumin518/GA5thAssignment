//
// Quad.h  : 공용 사각 메쉬
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once

#ifndef MATRIX
typedef XMMATRIX	MATRIXA;
typedef XMFLOAT4X4	MATRIX;
typedef XMFLOAT3	VECTOR3;
typedef XMFLOAT2	VECTOR2;
typedef XMFLOAT4	COLOR;
typedef ID3D11ShaderResourceView*   LPTEXTURERV;
#endif
/* for DX9 
typedef D3DXMATRIX	MATRIX;
typedef D3DXVECTOR3 VECTOR3;
typedef D3DXVECTOR2 VECTOR2;
typedef D3DXCOLOR	COLOR;
*/




//
//  공용 사각 메쉬 데이터. / UI, 주인공 횃불/풍차 조명처리용
//
//extern ID3D11Buffer*  g_pVBQuad;

 
int  QuadCreate(); 
void QuadUpdate(float dTime = 0);
void QuadDraw(float dTime = 0);
void QuadDraw(MATRIX* pmTM, float dTime = 0); 
void QuadDraw(XMMATRIX& mTM, LPTEXTURERV pDiffMap, COLOR col = COLOR(1,1,1,1));
//void QuadDraw(XMMATRIX& pmTM, LPTEXTURERV pDiffMap, LPTEXTURERV pLitMap, COLOR col, float dTime =0);
void QuadRelease();




/****************** end of file "Quad.h" ************************/