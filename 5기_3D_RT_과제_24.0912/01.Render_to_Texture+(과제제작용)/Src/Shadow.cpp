//
// Shadow.cpp : �⺻ ������/�����ӿ� �ҽ�
//
// DirectX ���� ���� : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
#pragma warning(disable:4996)
#include "Windows.h" 
#include "stdio.h"
#include "Device.h"
#include "Shader.h"		//���̴� ���� ���.
#include "Light.h"		//���� ���� ���.
//#include "XFile.h"	//XFile ��� Ŭ���� ���.
//#include "YnModel.h"	//���� �� Ŭ���� ���.

//#include "Map.h"		//���� ���.
#include "Shadow.h"		//�׸��� ���.




/////////////////////////////////////////////////////////////////////////////
//
//  ���� �����͵�.
//

extern BOOL g_bStencilOn;
extern VECTOR3 g_vTorchPosW;

//�׸��� ���
XMMATRIX g_mShadow;


 
  
/////////////////////////////////////////////////////////////////////////////// 
//
// �׸��� ���� : �׸��ڸ� �׸� ��� ��ü�� ���� ��� ����.
//
void ShadowON()
{ 

	//-----------------------
	// �׸��� ���� ����.
	//-----------------------	
	// �׸��� ��� ��꿡 �ʿ���  �� ���� Ȯ��.
	cbLIGHT cbLit = g_cbLit;	//���� ���� ����.
	
	//< ���� ���⼺ �׽�Ʈ1 > 
	//���� ���忡 ���� �������� ����� : "�¾籤" (����)(Directrional Light)
 	//VECTOR lt = VECTOR(g_cbLit.Direction, 0);		// W=0 (����) "Directional Light" 	
 	VECTOR lt = cbLit.Direction;		// ���� ���� ����.
	XMVectorSetW(lt, 0);				// W=0 (����) "Directional Light" 	
	//lt.w = 0;							// W=0 (����) "Directional Light" 	
	//lt = -lt;							// ������ �ٲپ� �־�� �մϴ�. 			
	//*/	
	
	/*//<���� ���⼺ �׽�Ʈ2 > 
	//���� ���忡 ���� �Ǿ� �ִٴ� ���� : "���ε�" (Spot/Point Light)
	VECTOR3 pos(0, 5.0f, 0);
	VECTOR lt = XMLoadFloat4(&VECTOR4(pos.x, pos.y, pos.z, 1));	// W=1 (��ǥ) "Point Light"
	//*/

	/*//< ���� ���⼺ #3>
	//���� ���ΰ� ��� ���� ���� ("Ƚ��")
	VECTOR3& pos = g_vTorchPosW + VECTOR3(0.5f, 2, 0);			// �׸����� ǰ���� ���� ���� ���� ���� ���� ���� ó��.
	VECTOR lt = XMLoadFloat4(&VECTOR4(pos.x, pos.y, pos.z, 1));	// W=1 (��ǥ) "Point Light" 	 
	//*/


	//������� ����.
	cbLit.Direction = lt;
	//cbLit.SpecOn = FALSE;			//���ݻ籤 off.
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &cbLit, sizeof(cbLIGHT));
	

	//-----------------------
	// �׸��ڿ� ������� �����.
	//-----------------------
	//XMVECTOR pos = XMLoadFloat3(&VECTOR3(0, 0.01f, 0));	//����� ��ġ�� ���� �ø���. (Z-Fighting ������)
	//XMVECTOR dir = XMLoadFloat3(&VECTOR3(0, 1, 0));
	//XMPLANE  pln = XMPlaneFromPointNormal( pos, dir);		//��� ����.
	XMPLANE pln = XMLoadFloat4(&VECTOR4(0, 1, 0, 0));		//�׸��ڰ� �׷��� ���.
	XMMATRIX mShadow = XMMatrixShadow( pln, lt);			//��� �׸��� ��ȯ��� ����.
	
	//�׸��� ��� ����.
	g_mShadow = mShadow;


	//-----------------------
	// ������ �ɼ� ����.
	//-----------------------
	g_pDXDC->OMSetBlendState(g_BState[BS_AB_ON], NULL, 0xFFFFFFFF);
	//g_pDXDC->RSSetState(g_RState[RS_CULL_CCW]);


	//-----------------------
	// ����/���ٽ� ���� ����.
	//-----------------------
	if(g_bStencilOn)  
			g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF_STENCIL_EQUAL_INCR], 0);		// "�ߺ� �׸��� ����"
	else	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF], 0);						//���� ��� OFF.
	

	
	//-----------------------
	// �غ� �Ϸ�!
	//-----------------------
	// �׸��� �׸���!
	//...
}




/////////////////////////////////////////////////////////////////////////////// 
//
// �׸��� �׸��� : YnModel ����. ���̴��� �׸��� ����.
//
/*
void ShadowDraw(LPMODEL pModel, float dTime/=0/)
{

	//-----------------------
	// �׸��ڿ� ���� ���� 
	//-----------------------
	//  ������, �ؽ�ó ����.--> Model Ŭ�������� ó����. 
	// ... 
	
	//-----------------------
	// �׸��ڿ� ���̴� ����  
	//-----------------------
	// Ư���� �����̳� �ؽ�ó ó�� ���� �ܼ� ���� �� ����(Alpha) �� �����մϴ�.
	// �̹� ���𿡼��� ���̴��� �߰� �������� �ʰ� Demo3.fx �� ����ϸ�
	// AB �� ���� Mtrl.Diffuse.a �� Ȱ���Կ� ������ �ֽʽÿ�.	
	//-----------------------	
	//���̴� ���.
	LPVERTEXSHADER pVS = pModel->GetVS();
	LPPIXELSHADER  pPS = pModel->GetPS();
	//�׸��� ���̴� ����.
	pModel->SetVS(g_pVS3);
	pModel->SetPS(g_pPS3); 
		  

	//-----------------------
	//'�׸���' �׸���...
	//-----------------------
	XMMATRIX mTM = XMLoadFloat4x4(&pModel->GetTM());
	XMMATRIX mPSTM = mTM * g_mShadow;						// Planar Shadow TM  
	pModel->SetTM(mPSTM); 
	pModel->DrawShadow();		


	//�� ��� ����.
	pModel->Update();
	
	//���̴� ����.
	pModel->SetVS(pVS);
	pModel->SetPS(pPS);

}
*/


 


/////////////////////////////////////////////////////////////////////////////// 
//
// �׸��� ����, ������ �ɼ� ����. 
//
void ShadowOFF()
{
	 
	//���̴� ����.
	//...	 

	//����/���ٽ� ����.
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON], 0);
	
	// ������ �ɼ� ���� 
	g_pDXDC->OMSetBlendState(g_BState[BS_AB_OFF], NULL, 0xFFFFFFFF);
	
	// ���� ����.
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &g_cbLit, sizeof(cbLIGHT));
	//UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMtrl, &g_cbMtrl, sizeof(cbLIGHT));

}







////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// ����/���ٽ� ���� ���� �Լ���. 
//
////////////////////////////////////////////////////////////////////////////// 
//
// ���ٽ� �׽�Ʈ Ȱ��ȭ.
//
void StencilON()
{	
	// ���ٽ� Ȱ��ȭ �� ������(Stencil Reference Value) ����(1) 
	// ������ "�׸���" �� ���ٽ� ���ۿ� '1' �� ����մϴ�. 
	//g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON_STENCIL_ON], 1);	 

	//Depth-OFF , Stencil-ON, ������ "0" �� ������ �׸��� +1���� (���߱׸��� ����)
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF_STENCIL_EQUAL_INCR], 0);
	 
}





/////////////////////////////////////////////////////////////////////////////// 
//
// ���ٽ� ����, ������ �ɼ� ����. 
//
void StencilOFF()
{ 	  

	//����/���ٽ� ���� ���� : Depth-ON, Stencil-OFF (�⺻��)
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON_STENCIL_OFF], 0);


	// ��Ÿ ������ �ɼ� ����...
	// 
  	// AB OFF... 

	// Depth Buffer Ŭ���� (�ɼ�)
	// ���� �������� ���������� ���̰���(Z-Fighting) �� �����ϱ����� ���̹��۸� �ʱ�ȭ�մϴ�.
	// �ʿ��ϴٸ�, ���ٽǵ� �Բ� �ʱ�ȭ�մϴ�.
	//g_pDXDC->ClearDepthStencilView(g_pDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);	//���̰��� �ʱ�ȭ. 
	//g_pDXDC->ClearDepthStencilView(g_pDSView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);	//���̰�/���ٽǰ� �ʱ�ȭ. 

	//����Ÿ�� ����� (�ɼ�)
	//g_pDXDC->ClearShadowTargetView(g_pRTView, ...);						
	
}



////////////////////////////////////////////////////////////////////////////// 
//
// ���� �׽�Ʈ OFF.
//
void DepthOFF()
{	
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF], 0);	 
}





/////////////////////////////////////////////////////////////////////////////// 
//
// ���� �׽�Ʈ ON (�⺻��) 
//
void DepthON()
{ 
	//����/���ٽ� ���� ���� : Depth-ON, Stencil-OFF (�⺻��)
	g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON], 0); 	
}






/****************** end of file "Shadow.cpp" *********************************/