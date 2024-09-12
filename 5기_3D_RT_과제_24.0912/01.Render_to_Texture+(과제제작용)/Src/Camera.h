//
// Camera.h : �⺻ ������/�����ӿ� ���
//
// DirectX ���� ���� : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once


/////////////////////////////////////////////////////////////////////////////
//
// ī�޶� Ŭ����.
//
/////////////////////////////////////////////////////////////////////////////
class Camera
{
protected:

public:
	Camera();
	virtual ~Camera();	
};





/////////////////////////////////////////////////////////////////////////////
//
// ī�޶� ���� �Լ��� ����.
//
void CameraSetup(VECTOR3 eye, VECTOR3 lookat, VECTOR3 up);
void CameraUpdate(float dTime = 0);
void CameraUpdateA(float dTime = 0);
void CameraUpdateATK(float dTime = 0);
void CameraUpdateOrtho(float dTime = 0);

void CameraInfo(int& x, int& y, COLOR col, float dTime = 0);



/////////////////////////////////////////////////////////////////////////////
//
// ���� ������.
//
extern XMVECTOR g_vEye;
extern XMVECTOR g_vLookAt;
extern XMVECTOR g_vUp;
extern XMVECTOR g_vHorz;
extern XMVECTOR g_vView;

extern float g_fFov;
extern float g_fAspect;
extern float g_fZnear;
extern float g_fZfar;

extern BOOL g_bCamTrackHero;


/****************** end of file "Camera.h" ***********************************/