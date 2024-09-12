//
// Quad.cpp : ����Ʈ���ο� ���� �簢 �޽�
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

#include "assert.h"
#include "Device.h"
#include "Shader.h"		//���̴� ���� ���.
//#include "Light.h"	//���� ���� ���.
#include "Render.h"

#include "Quad1.h"


/////////////////////////////////////////////////////////////////////////////
//
//  ���� �����͵�.
//
struct QUAD_VTX
{
	float x, y, z;
	float u0, v0;		//0�� uv ��Ʈ.
	//float u1, v1;		//1�� uv.��Ʈ.
};


// ���� �𵨿� ���Ϲ���.
ID3D11Buffer*		g_pVBQuad = nullptr;			//���� ���� ������.
ID3D11InputLayout*  g_pVBQuadLayout = nullptr;		//���� �Է±��� 

// ���� �� �ؽ�ó
ID3D11ShaderResourceView*           g_pTexrvQuadDiff = nullptr;		//DiffuseMap �ܺ�����, �ܼ�������.
//TCHAR g_TexNameQuad[256]=L"N/A";					 
//ID3D11ShaderResourceView*           g_pTexrvQuadLit = nullptr;	//LightMap �ܺ�����, �ܼ�������.
//TCHAR g_TexNameQuad[256]=L"N/A";					 

// ����� ������� : �� ������ ��ü(����)�� CB ����� ����.
cbDEFAULT		g_cbQuad;
ID3D11Buffer*	g_pCBQuad = nullptr;

// �𵨿� �����Է±���
//ID3D11InputLayout*  g_pVBLayout;
int QuadCreateVBLayout();



 
////////////////////////////////////////////////////////////////////////////
//
int QuadCreate() 
{ 	 

	HRESULT hr = S_OK;

	QUAD_VTX Quad[] = {
	
		// ���� ������ ����... 
		// [�׸�] �� ���� ���� ����.
		//  v0 --- v1
		//   |    / | 
		//   |   /  |
		//   |  /   |
		//   | /    |
		//  v2 --- v3
	
		{-0.5f,  0.5f, 0,   0.0f,  0.0f, }, 
		{ 0.5f,  0.5f, 0,   1.0f,  0.0f, }, 
		{-0.5f, -0.5f, 0,   0.0f,  1.0f, },   
		{ 0.5f, -0.5f, 0,   1.0f,  1.0f, },  
	
	};


	// ���� ���� Vertex Buffer ����
	DWORD size = sizeof(Quad);
	CreateVB(g_pDevice, Quad, sizeof(Quad), &g_pVBQuad);	  

	// ���� �Է±��� Input layout ����.
	QuadCreateVBLayout();
		

	// �ؽ�ó �ε� Load the Texture 
	//TCHAR* filename = L"../data/ground.jpg";
	//LoadTexture(filename, &g_pTexrvQuadDiff);
	//TCHAR* filename2 = L"../data/fx/Flare0.dds";
	//LoadTexture(filename2, &g_pTexrvQuadLit);
	

	// ��� ���� ����.
	ZeroMemory(&g_cbQuad, sizeof(cbDEFAULT)); 
	CreateDynamicConstantBuffer(sizeof(cbDEFAULT), &g_cbQuad, &g_pCBQuad); 
	

	return TRUE;
}





////////////////////////////////////////////////////////////////////////////
//
void QuadRelease() 
{ 	 
	//...
	SAFE_RELEASE(g_pVBQuad);			//�� ����.
	SAFE_RELEASE(g_pVBQuadLayout);
	//SAFE_RELEASE(g_pTexrvQuadDiff);	//�ؽ�ó ����.
	//SAFE_RELEASE(g_pTexrvQuadLit);	//�ؽ�ó ����.
	SAFE_RELEASE(g_pCBQuad);			//������� ����.
}






////////////////////////////////////////////////////////////////////////////// 
//
// ���� �Է±��� ���� : ���̴��� ������� �ʿ�. ���� ���𿡼��� ���� ��.	
//
int QuadCreateVBLayout()
{
	HRESULT hr = S_OK;	

	// ���� �Է±��� Input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                    offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
		//{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT,     0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },	
	};
	UINT numElements = ARRAYSIZE(layout);

	// ���� �Է±��� ��ü ���� Create the input layout
	hr = g_pDevice->CreateInputLayout(  layout,
										numElements,
										g_pVSCode4->GetBufferPointer(),
										g_pVSCode4->GetBufferSize(),
										&g_pVBQuadLayout
									);
	if (FAILED(hr))	return hr;  

	return hr;
}

 
 





////////////////////////////////////////////////////////////////////////////
//
void QuadUpdate(float dTime) 
{ 
	//�� ���� 
	//...
	
	/*
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
	g_cbQuad.mTM = mTM;
	g_cbQuad.mWV = mTM * yn_View; 
	g_cbQuad.mView = yn_View;		//���� ī�޶� ���� ����. 
	g_cbQuad.mProj = yn_Proj;		//�Ϲ� �ڽ��� ī�޶� ������ �����մϴ�.


	//���̴� ��� ���� ����.(��������)
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBQuad, &g_cbQuad, sizeof(cbDEFAULT));
	//���̴� ��� ���� ����.(��������)
	//g_pDXDC->UpdateSubresource(g_pCBDef, 0, nullptr, &g_cbQuad, 0, 0);


	//-----------------------
	// ���� ���� ����.
	//-----------------------
	//...
	*/

}




////////////////////////////////////////////////////////////////////////////
//
// �簢���޽� �׸��� 
//
void QuadDraw(float dTime)
{ 
	  
	//-----------------------
	// ������� ����
	//-----------------------
	//..����� �ܺο���.
	//..���� ����. ����Diffuse �� �������.

	//-----------------------
	// ���� �� ���̴� ����.
	//-----------------------
	UINT stride = sizeof(QUAD_VTX);
	UINT offset = 0;
	g_pDXDC->IASetVertexBuffers(0, 1, &g_pVBQuad, &stride, &offset);			//���� ���� ����	
	g_pDXDC->IASetInputLayout(g_pVBQuadLayout);									//�Է� ���̾ƿ� ����. 
	g_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	// ���� ���� ���� ����

	//���̴� ����. : �簢�޽� ����, �������.
	g_pDXDC->VSSetShader(g_pVS4, nullptr, 0);
	g_pDXDC->PSSetShader(g_pPS4, nullptr, 0);		
	//���� ���̴� ���ҽ� ���� (���� ����)
	g_pDXDC->VSSetConstantBuffers(0, 1, &g_pCBQuad);			//��ȯ��� ����. 
	//g_pDXDC->VSSetConstantBuffers(1, 1, &g_pCBLit);			//�������� ����. 
	g_pDXDC->VSSetConstantBuffers(2, 1, &g_pCBMtrl);			//�������� ����.
	//�ȼ� ���̴� ���ҽ� ����.
	//g_pDXDC->PSSetConstantBuffers(2, 1, &g_pCBMtrl);			//���� ���� ����.(PS)
	g_pDXDC->PSSetShaderResources(0, 1, &g_pTexrvQuadDiff);		//�ؽ�ó ����. (Diff) --> �ܺο��� ����.
	//g_pDXDC->PSSetShaderResources(0, 1, &g_pTexrvQuadLit);	//�ؽ�ó ����. (LitMap) ---> �ܺο��� ����.
		
														
	//-----------------------
	//�׸���!
	//-----------------------
	g_pDXDC->Draw(4, 0);		


	// ���� ���� 
	//...

	//��ġ ���� ���� 
	//g_pDXDC->ClearState();
}




////////////////////////////////////////////////////////////////////////////
//
// �簢���޽� �׸��� 
//
// pmTM : �ؽ�ó ��ȯ���.
// pDiffMap : Ȯ���.
// pLitMap  : ����Ʈ��.
// col      : �߰����� (�����)
//
void QuadDraw(XMMATRIX& mTM, LPTEXTURERV pDiffMap, COLOR col)
{ 
	  
	//------------------------ 
	// ��ȯ��� ����.
	//------------------------ 	
 	//XMMATRIX mS, mR, mT, mTM;
	//mS = XMMatrixScaling(scale, 1, scale);
	//mR = XMMatrixRotationY(vRot.y);
	//mT = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
	//mTM = mS * mR * mT;
	cbDEFAULT cbDef = g_cbDef;
	cbDef.mTM = mTM;
	cbDef.mWV = mTM * cbDef.mView;
	//cbDef.mTex = mTexTM;
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBQuad, &cbDef, sizeof(cbDEFAULT));
 

	//------------------------ 
	// ���� OFF...
	//------------------------ 
	/*cbLIGHT cbLit = g_cbLit;
	cbLit.DiffOn = FALSE;
	cbLit.SpecOn = FALSE;
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &cbLit, sizeof(cbLIGHT));*/

	//------------------------ 
	// ���� ���� ����.
	//------------------------ 
	extern BOOL g_bTexOn;
	cbMATERIAL cbMtrl = g_cbMtrl;
	cbMtrl.Diffuse = XMLoadFloat4(&col);
	//cbMtrl.Ambient = XMLoadFloat4(&COLOR(1,1,1,1));
	cbMtrl.bTexOn = g_bWireFrame ? FALSE : g_bTexOn;		//�ؽ�ó ���� On/Off.
	UpdateDynamicConstantBuffer(g_pDXDC, g_pCBMtrl, &cbMtrl, sizeof(cbMATERIAL));



	//------------------------ 
	// ���̴� ���ҽ� ����.
	//------------------------ 
	g_pTexrvQuadDiff = pDiffMap;
	//g_pTexrvQuadLit = pLitMap;

	
	//------------------------ 
	// ���÷� ����.
	//------------------------ 
	//g_pDXDC->PSSetSamplers(0, 1, &g_pSampler[SS_CLAMP]); 
	//g_pDXDC->PSSetSamplers(1, 1, &g_pSampler[SS_WRAP]); 

	//------------------------ 
	// ��Ÿ ������ ���� ����.
	//------------------------ 
	// �����׽�Ʈ : ���̴����� ó��.
	//AlphaTest = 0.01f;
	// ���ĺ��� ON.
	//g_pDXDC->OMSetBlendState(g_BState[BS_AB_ON], NULL, 0xFFFFFFFF);
	//�����׽�Ʈ OFF : Z-Fighting ����.
	//g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_OFF], 0);
	//�޺� �׸���. OFF.
	//g_pDXDC->RSSetState(g_RState[RM_SOLID]);
	

	//------------------------ 
	// �׸���.
	//------------------------ 
	QuadDraw();


	//------------------------ 
	// �ɼ� ���� 
	//------------------------ 
	//g_pDXDC->OMSetBlendState(g_BState[BS_AB_OFF], NULL, 0xFFFFFFFF);
	//g_pDXDC->OMSetDepthStencilState(g_DSState[DS_DEPTH_ON], 0);
	//UpdateDynamicConstantBuffer(g_pDXDC, g_pCBLit, &g_cbLit, sizeof(cbLIGHT));
	//RenderModeUpdate();
	
	//��ġ ���� ���� 
	//g_pDXDC->ClearState();

}






////////////////////////////////////////////////////////////////////////////
//
void QuadDraw(MATRIX* pmTM, float dTime) 
{ 	   
/*	//����� �ܺο���.
	if(pmTM) g_pDevice->SetTransform(D3DTS_WORLD, pmTM);	

	QuadDraw(dTime); 
*/
}


 

/*********************** end of file "Quad.h" *****************************/
