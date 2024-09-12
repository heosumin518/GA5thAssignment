//
// RenderTarget.cpp : ����Ÿ�� ��� �ҽ�
//
// DirectX ���� ���� : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net / onlysoim@gmail.com
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
#pragma warning(disable:4996)
#include "Windows.h" 
#include "stdio.h"
#include "Device.h"
#include "Shader.h"
#include "Render.h"

#include "RenderTarget.h"



/////////////////////////////////////////////////////////////////////////////
//
//  ���� �����͵�.
//  



////////////////////////////////////////////////////////////////////////////// 
//
// �� D3D �� �ڿ���. Resources 
//
// �ڿ�( ���ҽ�, Resource ) �� 3D ����� �ؼ� �Ǵ� �������� ���� ������ 
// �����ϴ� ������ ������̸� �׷��������������� ����(�б�/����)�� ������ 
// HW �޸� �����Դϴ�. (���� �׷��� ��ͳ��� �����޸� VRAM)
//
// �������� 2���� Ÿ�� - ����(Type) �� ������(Typeless) - �ڿ��� ������ �� �ֽ��ϴ�.
//
// * �����ڿ� (Strong Typed Resource) 
//    : �ڿ������� ����(���� Format) �� ����.
//    : �ϵ��� ���� ����ȭ�� ���ټӵ� (GPU Read Only)
//    : ��������, �ε�������, �������, �ؽ�ó, ��..
// 
// * �������ڿ� (Weak Typing / Typeless Resource)
//    : �ڿ������� ������ �̰���(UnKnown :  DXGI_FORMAT_R8G8B8A8_TYPELESS ��..)
//    : �ڿ��� ���������ο� ����� ������ ���� (���ҽ� �� Resource View �̿�)
//    : ������ �ڿ� ����,����(R/W), ����, ��Ȱ��
//    : ������ ���� ��ȯ���� ( D3DX_FORMAT_R8G8B8A8_FLOAT, D3DX_FORMAT_R8G8B8A8_UINT ��.., ������ �����ϰ� TYPE �� ���氡��)
//    : ����Ÿ��(�ؽ�ó), ����/���ٽǹ���(�ؽ�ó), UAV(�ؽ�ó), ���̴��ڿ�(�ؽ�ó) ��..
//
// �ڿ��� ���ٹ��(CPU �Ǵ� GPU �� �б�/����)�� ������ �� �ֽ��ϴ�.
// �ִ� 128���� �ڿ����� ������������ �� ���������� ����(Blnding), ����� �����մϴ�.
// 
// D3D �ڿ��� �������� �Ʒ��� �����ϴ�.
// 1. ���� : ID3D11Device �� �����޼ҵ带 �̿�
// 2. ���� : ID3D11DeviceContext �� �����޼ҵ带 �̿�.
// 3. ���� : Release ȣ��.
//
// [����] Introduce of a Resource in D3D11 : https://msdn.microsoft.com/en-us/library/windows/desktop/ff476900(v=vs.85).aspx
//
//
////////////////////////////////////////////////////////////////////////////// 
//
// �� ���ҽ� �� Resource View
// 
// �ڿ����� �ٸ��� �޸�����(Generic Memory Format) ���� ����� �� �ֱ⿡ 
// �ټ��� ���������� ������������ ������ �� �ֽ��ϴ�.
// ���������� ���������� ���ҽ��� View �� ���� ����� �ڿ������͸� �ؼ��մϴ�.
//
// ���ҽ���� ���������� �ڷ� ����ȯ(Casting) �� �����ϱ⿡ ���� Ư�� ��ġ��� 
// (Context : ������ �������� ��ɸ��) ���� ���� �� �ֽ��ϴ�.
// 
// ���ҽ���� �������ڿ�(Typeless Resource) �� ���� ���Ǹ� ���������� ����������
// ����� �並 ���� Ư�������� ������ �� �ֽ��ϴ�. �׷��� ���ҽ���� �����Ϸ��� 
// �ش� �ڿ��� ��Ʈ����(Bits per Component)�� ������ ������ ������ �� �����ϴ�.
// ���� ��� DXGI_R8G8B8A8_TYPELESS ���� �������ڿ��� ���ҽ��並 ���� ���� ��Ʈ������
// DXGI_R8G8B8A8_FLOAT ���� �����ϳ� DXGI_R32_FLOAT �� �Ұ����մϴ�.
//
// ���ҽ���� ���� ���� Ư���� ����� �����մϴ�.
//  : ���̴����� ����/���ٽ� ����(Surface) �� �б�.
//  : �����н�(Single Pass)�� ����ť��� ����.
//  : ������/����� ����ȭ�� ���ÿ� �׸���.
// 
// ���ҽ����� �뵵�� �������̽��� �Ʒ��� �����ϴ�.
//  : ID3D11DepthStencilView - ����/���ٽ� �׽�Ʈ�� �ڿ� (�ؽ���)
//  : ID3D11RenderTargetView - ����Ÿ�ٿ� �ڿ� (�ؽ���)
//  : ID3D11ShaderResourceView - ���̴� �������� �ڿ� (�������, �ؽ�ó����, �ؽ�ó, �Ǵ� ���÷�)
//  : ID3D11UnorderedAccessView - �ȼ����̴� �Ǵ� ��ǻ�ͼ��̴� �� �������ڿ� (Unordered Resource)
//
// [����1] Resource Views : https://msdn.microsoft.com/en-us/library/windows/desktop/ff476900(v=vs.85).aspx#Views
// [����2] Unordered Resources : https://msdn.microsoft.com/ko-kr/library/windows/desktop/ff476335(v=vs.85)#Unordered_Access
//
//
////////////////////////////////////////////////////////////////////////////// 
//
// �� ����Ÿ�� ��� ����
//
// 1.����Ÿ�� ����.
//    1-1. �������� �ؽ��� ����			   : CreateTexture ( D3D11_BIND_RENDER_TARGET )
//    1-2.������ �ؽ����� ����Ÿ�ٺ� ����	   : CreateRenderTargetView
//    1-3.������ �ؽ����� ���̴����ҽ��� ���� : CreateShaderResourceView
// 2.���ο� ������ Ÿ������ ����		       : OMSetRednerTarget
//    2-1.�� ����/���ٽ� ���� ����. (�ɼ�)
//    2-2.�� ����Ʈ ���� ����.(�ɼ�)
// 3.��� �׸���.
//   ...
// 4.���� ������ Ÿ������ ����. (option)     : OMSetRednerTarget
//
// * AA �� ViewPort ������ ����.
//
////////////////////////////////////////////////////////////////////////////// 
//
// ����Ÿ�� ������ �����͵�.
//
LPTEXTURE		g_pTexScene = NULL;			//����� �������� �� ����Ÿ�ٿ� �ؽ��� (���ҽ�)
LPTEXTURERV		g_pTexRvScene = NULL;		//����� �������� �� ����Ÿ�ٿ� �ؽ���-���ҽ��� (���ο�)
LPRTVIEW        g_pRTScene = NULL;			//����� �������� �� ����Ÿ�ٺ� (��������) 

//LPRTSVIEW		g_pSurfOrgBB = NULL;		//���� �������(�����)


int  RenderTargetCreate();
void RenderTargetSceneGenerate();
void RenderTargetDraw();
void RenderTargetRelease();

HRESULT RTTexCreate(UINT width, UINT height, DXGI_FORMAT fmt, LPTEXTURE* ppTex);
HRESULT RTViewCreate(DXGI_FORMAT fmt, LPTEXTURE pTex, LPRTVIEW* ppRTView);
HRESULT RTSRViewCreate(DXGI_FORMAT fmt, LPTEXTURE pTex, LPTEXTURERV* ppTexRV);




//---------------------------------
// ���� Ÿ�� ������ ���� �޽�.
//---------------------------------
#include "Quad1.h"
//LPVERTEXBUFFER g_pVBQuad = NULL;
//HRESULT QuadCreate(LPDEVICE pDev);
//void    QuadRelease();

//��� ���� 
//BOOL	g_bRTargetOn = TRUE;
//BOOL	g_bRTargetUpdate = TRUE;
//BOOL	g_bShowScene = TRUE;



// ����Ÿ�� �ػ� ��
SIZE g_RTSize = {  512,	 512 };






/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
//
// Render Target ���� �Լ���.
//
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 
/////////////////////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////////////////// 
//
int RenderTargetCreate()
{

	//---------------------------- 
	// ����Ÿ�� ����� ��
	//---------------------------- 
	//����Ÿ�� ������ ũ��, ����, AA, Mipmap ���� ������ �ʿ��մϴ�.
	//�̿� ���� ����, ǰ�� �� �޸� �䱸���� �޶����ϴ�. 
	//���ɰ� ǰ�������� ������ Ÿ���� �ʿ��մϴ�.
	//SIZE size = { 512, 512 };
	DXGI_FORMAT fmt = DXGI_FORMAT_R8G8B8A8_UNORM;

	//1. ���� Ÿ�ٿ� �� �ؽ�ó�� �����.��
	//...
	// 
	//2. ����Ÿ�ٺ� ����.��
	//...

	//3. ����Ÿ�� ���̴� ���ҽ��� ���� (���ο�)��
	//...

	//4. ����Ÿ�� ���� ����/���ٽ� ���� ����.(�ɼ�)��
	//...


	//---------------------------- 
	// ����Ÿ�� ������ �Ž� ����. ("TV")
	//---------------------------- 
	QuadCreate();


	return YN_OK;
}




/////////////////////////////////////////////////////////////////////////////// 
//
void RenderTargetRelease()
{
	//����Ÿ�� ���ҽ� ����.��
 	SAFE_RELEASE( g_pTexScene );
 	SAFE_RELEASE( g_pTexRvScene );
 	SAFE_RELEASE( g_pRTScene );


	QuadRelease();
}




/////////////////////////////////////////////////////////////////////////////// 
//
// ����Ÿ�ٿ� �� �ؽ�ó �����. 
//
HRESULT RTTexCreate(UINT width, UINT height, DXGI_FORMAT fmt, LPTEXTURE* ppTex)
{
	HRESULT hr = S_OK;

	//�ؽ�ó ���� ����.��
	//...

	//�ؽ�ó ����.��
	//...

	//������ �ܺη� ����.
	//if (ppTex) *ppTex = pTex;

	return hr;
}




/////////////////////////////////////////////////////////////////////////////// 
//
// ����Ÿ�� ���ҽ��� �����. 
//
HRESULT RTViewCreate(DXGI_FORMAT fmt, LPTEXTURE pTex, LPRTVIEW* ppRTView)
{
	HRESULT hr = S_OK;

	//����Ÿ�� ���� ����.��
	//...

	//������ �ܺη� ����.
	//if (ppRTView) *ppRTView = pRTView;

	return hr;
}




/////////////////////////////////////////////////////////////////////////////// 
//
// ����Ÿ�� ���̴� ���ҽ��� �����. 
//
HRESULT RTSRViewCreate(DXGI_FORMAT fmt, LPTEXTURE pTex, LPTEXTURERV* ppTexRV)
{
	HRESULT hr = S_OK;

	//���̴����ҽ��� ���� ����.��
	//...
	// 
	//������ �ܺη� ����.
	//if (ppTexRV) *ppTexRV = pTexRV;

	return hr;
}




/////////////////////////////////////////////////////////////////////////////// 
//
// ����Ÿ������ ����� �������մϴ�.
//
void RenderTargetSceneGenerate()
{ 	 
	//���� ����Ÿ�� (�����) ��� 
	//LPRTVIEW  pRTOrg;		
	//LPDSVIEW  pDSOrg;
	//g_pDXDC->OMGetRenderTargets(1, &pRTOrg, &pDSOrg);	  

	//-------------------------------
	// ����Ÿ�� ��� ����.
	//-------------------------------
	//�� ����Ÿ�� ����.��
	g_pDXDC->OMSetRenderTargets(1, &g_pRTScene, g_pDSView);		//���̹��۴� �������� ����.��
	


 	//�� ����Ÿ��(�����) ��  ���̽��ٽǹ��� �����..
	//�� ����Ÿ���� ��Ȯ�� �ĺ��� ���� ��ȫ������ ó���մϴ�.
	g_pDXDC->ClearRenderTargetView(g_pRTScene, (float*)&COLOR(1, 0, 1, 1));						//RT ��ȫ������ �����.(�ĺ���)��				
	g_pDXDC->ClearDepthStencilView(g_pDSView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);	//���̹��۴� �������� ����.��
 
	
	//��� ������...
	_SceneRender();
	

	//-------------------------------
	// ���� �ý��� ����.
	//-------------------------------
	//�����, ����/���ٽǹ��� ����.
	g_pDXDC->OMSetRenderTargets(1, &g_pRTView, g_pDSView);
	//g_pDXDC->OMSetRenderTargets(1, &pRTOrg, pDSOrg);


	//ȹ���� �������̽��� ����� Release �ؾ� �մϴ�.
	//SAFE_RELEASE(pRTOrg);
	//SAFE_RELEASE(pDSOrg);
}




/////////////////////////////////////////////////////////////////////////////// 
//
// ����Ÿ�� ��� : ������ ����Ÿ���� �̹����� �簢�޽��� �����Ͽ�
//                ���� ���(BackBuffer)�� �������մϴ�.
//
void RenderTargetDraw()
{
	//if(!g_bRTargetOn) return;
	

	//0 �ɼ� ���� : ����off, �ø�off.
	//...	

	//1.�޽� ����.(����)
	//..

	//2.����Ÿ���� �ؽ��ķ� ���� : QuadDraw �Լ��� ����.
	//pDev->SetTexture(0, g_pTexScene);		
	 

	//3.�׸��� : �޽��� ���忡..("TV")
	XMMATRIX mS, mT, mR, mTM;
	mS = XMMatrixScaling( 5.0f, 5.0f, 1.0f);		//512 x 512 ���� ���� (1:1)
 	mR = XMMatrixIdentity();
	mT = XMMatrixTranslation(-2.0f, 2.5f, 2.0f);		 
	mTM = mS * mR * mT;

	QuadDraw(mTM, g_pTexRvScene );
	
	
	//�ɼ� ����.
 	//����ON, �ø�ON. 
	//...

}




/****************** end of file "RenderTarget.cpp" *********************************/