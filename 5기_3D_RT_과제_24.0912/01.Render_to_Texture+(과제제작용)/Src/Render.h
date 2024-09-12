//
// Render.h : �⺻ ������/�����ӿ� ���
//
// DirectX ���� ���� : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once

 
int		DataLoading();
void	DataRelease();

void	SceneRender();	

// ��� �׸��� (�κ�) : ����Ÿ�� / �׸��ڸ� ������ ���� �����Լ�.
void	_SceneRender();


//extern BOOL g_bBBoxOn;			//������ ǥ��(�� ��ü) 
//extern BOOL g_bBBoxNOn;			//�ڰ���� ǥ��(��庰) 

//���ٽ� ���� ó��.
extern BOOL g_bStencilOn;		
extern BOOL g_bShowCutOff;		

//������ ���� ������.
extern BOOL	g_bShowHelp; 
//extern BOOL	g_bWireFrame;		//������ ��� : ���̾�������/�ָ��� ����.
extern BOOL	g_bSpecOn; 
extern BOOL	g_bShadowOn; 



/****************** end of file "Render.h" ***********************************/