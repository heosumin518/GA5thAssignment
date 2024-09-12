//
// Map.cpp : ���� ���� �ҽ�
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
#include "Map.h"
#include "Shader.h"		//���̴� ���� ���.
#include "Light.h"		//���� ���� ���.
//#include "XFile.h"	//XFile ��� Ŭ���� ���.
//#include "YnModel.h"	//���� �� Ŭ���� ���.

#include "Map.h"		//���� ���.






/////////////////////////////////////////////////////////////////////////////
//
//  ���� �����͵�.
//

 
// �𵨿� �����Է±��� (�ܺ� ����)
extern ID3D11InputLayout*  g_pVBLayout;
//int CreateVBLayout();





///////////////////////////////////////////////////////////////////////////// 
//
// ���� ������.
//

// ���� ����ü 
struct MAP_VTX
{
	float x, y, z; 			//��ǥ(Position)
	float nx, ny, nz;		//��� : Normal 	 
	float u, v;				//�ؽ�ó ��ǥ: Texture-Coordinates 
};


// ���� �𵨿� ���Ϲ���.
ID3D11Buffer*		g_pVBMap = nullptr;			//���� ���� ������.
//ID3D11InputLayout*  g_pVBMapLayout = nullptr;	//���� �Է±��� -> ť�� �� ����.

// ���� �� �ؽ�ó
ID3D11ShaderResourceView*           g_pTexrvMap = nullptr;		
TCHAR g_TexNameMap[256]=L"N/A";					 

// ������ ������� : �� ������ ��ü(����)�� CB ����� ����.
cbDEFAULT		g_cbMap;
ID3D11Buffer*	g_pCBMap = nullptr;


 
 



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// ���� ��
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
//  ���� : ���� �� �ʿ� ���ҽ� �ε�.
//
int MapCreate()
{
	HRESULT hr = S_OK;
	
	//����: �ݺ�����
	MAP_VTX	Quad[4]={
		{ -0.5, 0.0, 0.5,  0.0, 1.0, 0.0,   0.0,  0.0 },			
		{  0.5, 0.0, 0.5,  0.0, 1.0, 0.0,  20.0,  0.0 },			
		{ -0.5, 0.0,-0.5,  0.0, 1.0, 0.0,   0.0, 20.0 },		
		{  0.5, 0.0,-0.5,  0.0, 1.0, 0.0,  20.0, 20.0 },	
	};//*/

	 /*//����: ���ϸ���
	COLVTX	Quad[4]={
		{ -1.0, 0.0, 1.0,  0.0, 1.0, 0.0,  0.0,  0.0 },			
		{  1.0, 0.0, 1.0,  0.0, 1.0, 0.0,  1.0,  0.0 },			
		{ -1.0, 0.0,-1.0,  0.0, 1.0, 0.0,  0.0,  1.0 },		
		{  1.0, 0.0,-1.0,  0.0, 1.0, 0.0,  1.0,  1.0 },	
	};//*/


	// ���� ���� Vertex Buffer ����
	DWORD size = sizeof(Quad);
	CreateVB(g_pDevice, Quad, sizeof(Quad), &g_pVBMap);	  

	// ���� �Է±��� Input layout ����.
	// ... --> Cube �� ����. 
		

	// �ؽ�ó �ε� Load the Texture 
	//TCHAR* filename = L"../data/ground.jpg";
	TCHAR* filename = L"../data/Floor3.jpg";
	LoadTexture(filename, &g_pTexrvMap);
	

	// ��� ���� ����.
	ZeroMemory(&g_cbMap, sizeof(cbDEFAULT)); 
	CreateDynamicConstantBuffer(sizeof(cbDEFAULT), &g_cbMap, &g_pCBMap); 


	return TRUE;
}



////////////////////////////////////////////////////////////////////////////// 
//
// ���� : ���� 
//
void MapRelease()
{
	SAFE_RELEASE(g_pVBMap);			//�� ����.
	//SAFE_RELEASE(g_pVBMapLayout);
	SAFE_RELEASE(g_pTexrvMap);		//�ؽ�ó ����.
	SAFE_RELEASE(g_pCBMap);			//������� ����.
}



////////////////////////////////////////////////////////////////////////////// 
//
// ���� ����
//
void MapUpdate(float dTime)
{
	//�� ���� 
	//...
	

	//-----------------------
	// ��ȯ ���.
	//-----------------------
	//XMMATRIX mScale = XMMatrixScaling(30, 1, 30);		//30x30 (m) ����.
	//XMMATRIX mScale = XMMatrixScaling(80, 1, 80);		//80x80 (m) ����.
	//XMMATRIX mScale = XMMatrixScaling(100, 1, 100);	//100x100 (m) ����.
	XMMATRIX mScale = XMMatrixScaling(256, 1, 256);		//256x256 (m) ����.
	//XMMATRIX mRot = XMMatrixIdentity();	 	
	XMMATRIX mTM = mScale;
	

	//-----------------------
	// ������۷� ���.
	//-----------------------
	g_cbMap.mTM = mTM;
	g_cbMap.mWV = mTM * yn_View; 
	g_cbMap.mView = yn_View;		//���� ī�޶� ���� ����. 
	g_cbMap.mProj = yn_Proj;		//�Ϲ� �ڽ��� ī�޶� ������ �����մϴ�.


	//���̴� ��� ���� ����.(��������)
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMap, &g_cbMap, sizeof(cbDEFAULT));
	//���̴� ��� ���� ����.(��������)
	//g_pDXDC->UpdateSubresource(g_pCBDef, 0, nullptr, &g_cbMap, 0, 0);


	//-----------------------
	// ���� ���� ����.
	//-----------------------
	//.. �̹� ���𿡼��� cube �𵨰� ����.


}




////////////////////////////////////////////////////////////////////////////// 
//
// ���� �׸���.
//
void MapDraw(float dTime)
{	

	//-----------------------
	// ������� ����
	//-----------------------
	//��ȯ��� ����
	//g_cbMap.mTM = mTM;
	//g_cbMap.mWV = mTM * g_cbMap.mView;	
	//UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMap, &g_cbMap, sizeof(cbDEFAULT));

	// ���� ���� ���� : ������ Specular �� ������� �ʰڽ��ϴ�.
	cbLIGHT cbLit = g_cbLit;
	cbLit.SpecOn = FALSE;
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &cbLit, sizeof(cbLIGHT));
	
	//���� ���� ����.
	extern BOOL g_bTexOn;
	g_cbMtrl.Diffuse = XMLoadFloat4(&COLOR(1,1,1,1)); 
	g_cbMtrl.Ambient = XMLoadFloat4(&COLOR(1,1,1,1));
	//g_cbMtrl.Specular = XMLoadFloat4(&COLOR(1,1,1,1));  --> ������ Specular �� ������� �ʰڽ��ϴ�.
	//g_cbMtrl.Power = 30;
	//g_cbMtrl.bTexOn = g_bTexOn ? TRUE : FALSE;			//�ؽ�ó ���� On/Off.
	g_cbMtrl.bTexOn = g_bWireFrame ? FALSE : g_bTexOn;		//�ؽ�ó ���� On/Off.
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMtrl, &g_cbMtrl, sizeof(cbMATERIAL));
	
	

  	//-----------------------
	// ���� �� ���̴� ����.
	//-----------------------
	UINT stride = sizeof(MAP_VTX);
	UINT offset = 0;
	g_pDXDC->IASetVertexBuffers(0, 1, &g_pVBMap, &stride, &offset);				//�������� ����
	g_pDXDC->IASetInputLayout(g_pVBLayout);										//�Է·��̾ƿ� ����.  
	g_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	//���������� ���� 	
	
	//���̴� ����. : ǥ�� ���� ���̴�
	g_pDXDC->VSSetShader(g_pVS, nullptr, 0);
	g_pDXDC->PSSetShader(g_pPS, nullptr, 0);		
	//vs ���̴� ���ҽ� ����.
	g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBMap);		//��� ���� ����.(���������� ����)
	//g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBDef);	//��� ���� ����. 
	g_pDXDC->VSSetConstantBuffers(1, 1, &g_pCBLit);		//���� ���� ����. 
	g_pDXDC->VSSetConstantBuffers(2, 1, &g_pCBMtrl);	//���� ���� ����. 
	//PS ���̴� ���ҽ� ����.
	g_pDXDC->PSSetConstantBuffers(2, 1, &g_pCBMtrl);	//���� ���� ����.(PS)
	g_pDXDC->PSSetShaderResources(0, 1, &g_pTexrvMap);	//PS �� �ؽ�ó ����. (�����ؽ�ó)
 	
														
	//-----------------------
	//�׸���! 
	//-----------------------
	g_pDXDC->Draw(4, 0);		


	// ���� ���� 
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &g_cbLit, sizeof(cbLIGHT));


	//��ġ ���� ���� 
	//g_pDXDC->ClearState();
}











/****************** end of file "Map.cpp" *********************************/