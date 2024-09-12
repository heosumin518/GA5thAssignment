//
// ynYena :: Direct3D Basic Frame-works 
// 2003-2011. Kihong Kim  / mad_dog@hanmail.net
// Zero-G Interactive / http://www.zero-g.kr 
// DirectX ���� ���� : DX90c.Nov.2007
//
// ynBox.cpp :  
//
// 2003.11.4. 
// 2009.12.01. Update. (DX90c.Nov.2007)
//
#pragma warning(disable:4996)
#include "Windows.h" 
#include "stdio.h"
#include "Device.h"
#include "Shader.h"		//���̴� ���� ���.
#include "Light.h"		//���� ���� ���.
#include "Yena.h"

#include "YnBox.h"	 






/////////////////////////////////////////////////////////////////////////////
//
 


// ť�� + ��� ( ������ ����)
//
// [�׸�] �� ���� ���� ����.
//  v0 --- v1
//   |    / | 
//   |   /  |
//   |  /   |
//   | /    |
//  v2 --- v3
//


//------------------------------------------------------------------//
// ���� ������ : ����ȭ �Ǿ� ���� ���� Triangle List �� ���� ������.    //
//             �ﰢ�� 12��  ���� 36 ��								//
//          :  �ε��� ���� �ʿ� ����.��								//
//------------------------------------------------------------------//
/*
//g_strIBMode = "Not Used";
ynBox::_VTX	boxVtx[] = {
	// ����.  ( Face#0) :    
	// ��ǥ ,             ���,       UV (TextureCoordinate) 
	{ -1, 1,-1,  0, 0,-1,  0, 0 },			
	{  1, 1,-1,  0, 0,-1,  1, 0 },			
	{ -1,-1,-1,  0, 0,-1,  0, 1 },		
	//       ( Face#1)
	{ -1,-1,-1,  0, 0,-1,  0, 1 },			
	{  1, 1,-1,  0, 0,-1,  1, 0 },		
	{  1,-1,-1,  0, 0,-1,  1, 1 },		

	//�޸�.  (Face#3) :  
	{  1, 1, 1,  0, 0, 1,  0, 0 },		
	{ -1, 1, 1,  0, 0, 1,  1, 0 },		
	{ -1,-1, 1,  0, 0, 1,  1, 1 },		
	//		  (Face#4)
	{  1, 1, 1,  0, 0, 1,  0, 0 },		
	{ -1,-1, 1,  0, 0, 1,  1, 1 },		
	{  1,-1, 1,  0, 0, 1,  0, 1 },		


	// ������. (Face#5)
	{  1, 1,-1,  1, 0, 0,  0, 0 },		
	{  1, 1, 1,  1, 0, 0,  1, 0 },		
	{  1,-1,-1,  1, 0, 0,  0, 1 },		
	//			(Face#6)
	{  1,-1,-1,  1, 0, 0,  0, 1 },		
	{  1, 1, 1,  1, 0, 0,  1, 0 },		
	{  1,-1, 1,  1, 0, 0,  1, 1 },		


	// ������. (Face#7)
	{ -1, 1, 1,  -1, 0, 0,  0, 0 },		
	{ -1, 1,-1,  -1, 0, 0,  1, 0 },		
	{ -1,-1,-1,  -1, 0, 0,  1, 1 },		
	//			(Face#8)
	{ -1, 1, 1,  -1, 0, 0,  0, 0 },		
	{ -1,-1,-1,  -1, 0, 0,  1, 1 },		
	{ -1,-1, 1,  -1, 0, 0,  0, 1 },		


	//���.  ( Face#9)
	{ -1, 1, 1,  0, 1, 0,  0, 0 },		
	{  1, 1, 1,  0, 1, 0,  1, 0 },		
	{ -1, 1,-1,  0, 1, 0,  0, 1 },		
	//		 ( Face#10)
	{ -1, 1,-1,  0, 1, 0,  0, 1 },		
	{  1, 1, 1,  0, 1, 0,  1, 0 },		
	{  1, 1,-1,  0, 1, 0,  1, 1 },		


	//�Ϻ�.  ( Face#11)
	{  1,-1, 1,  0,-1, 0,  0, 0 },		
	{ -1,-1, 1,  0,-1, 0,  1, 0 },		
	{ -1,-1,-1,  0,-1, 0,  1, 1 },		
	//		 ( Face#12)
	{  1,-1, 1,  0,-1, 0,  0, 0 },		
	{ -1,-1,-1,  0,-1, 0,  1, 1 },		
	{  1,-1,-1,  0,-1, 0,  0, 1 },		 
};
//*/



//------------------------------------------------------------------//
// ���� ������ : �ε��� ���� / Triangle-List �� ���� ������.			//
//             �ﰢ�� 12��  ���� 24 ��								//
//------------------------------------------------------------------//
ynBox::_VTX boxVtx[] =					
{
	// ���鿡�� �ð� �ݴ� �������� ���� 0~3�� ��.
	//0�� �� : ����.
	{ -1, 1, -1,  0, 0, -1,  0, 0 },	//v0	
	{  1, 1, -1,  0, 0, -1,  1, 0 },	//v1	
	{ -1,-1, -1,  0, 0, -1,  0, 1 },	//v2
	{  1,-1, -1,  0, 0, -1,  1, 1 },	//v3	

	//1�� �� : ������
	{  1, 1, -1,  1, 0, 0,  0, 0 },		//v4	
	{  1, 1,  1,  1, 0, 0,  1, 0 },		//v5	
	{  1,-1, -1,  1, 0, 0,  0, 1 },		//v6	
	{  1,-1,  1,  1, 0, 0,  1, 1 },		//v7	

	//2�� �� : �޸�
	{  1, 1, 1,  0, 0, 1,  0, 0 },		//v8
	{ -1, 1, 1,  0, 0, 1,  1, 0 },		//v9
	{  1,-1, 1,  0, 0, 1,  0, 1 },		//v10
	{ -1,-1, 1,  0, 0, 1,  1, 1 },		//v11

	//3�� �� : ������
	{ -1, 1,  1,  -1, 0, 0,  0, 0 },	//v12
	{ -1, 1, -1,  -1, 0, 0,  1, 0 },	//v13
	{ -1,-1,  1,  -1, 0, 0,  0, 1 },	//v14
	{ -1,-1, -1,  -1, 0, 0,  1, 1 },	//v15


	//4�� �� : ����.
	{ -1, 1,  1,  0, 1, 0,  0, 0 },		//v16
	{  1, 1,  1,  0, 1, 0,  1, 0 },		//v17
	{ -1, 1, -1,  0, 1, 0,  0, 1 },		//v18
	{  1, 1, -1,  0, 1, 0,  1, 1 },		//v19

	//5�� �� : �Ʒ���.
	{ -1, -1, -1,  0, -1, 0,  0, 0 },	//v20
	{  1, -1, -1,  0, -1, 0,  1, 0 },	//v21
	{ -1, -1,  1,  0, -1, 0,  0, 1 },	//v22
	{  1, -1,  1,  0, -1, 0,  1, 1 },	//v23


};


//------------------------------------------------------------------//
// ���� 24���� �ε��� ����.	��		   							    //		
//------------------------------------------------------------------//

ynBox::_INDEX	boxIdx[] = {
	{  0,  1,  2 }, {  2,  1,  3 },
	{  4,  5,  6 }, {  6,  5,  7 },
	{  8,  9, 10 },	{ 10,  9, 11 },
	{ 12, 13, 14 }, { 14, 13, 15 },
	{ 16, 17, 18 }, { 18, 17, 19 },
	{ 20, 21, 22 }, { 22, 21, 23 }
};
 


/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
ynBox::ynBox()
{
	_InitDatas();

}

/////////////////////////////////////////////////////////////////////////////
//
ynBox::~ynBox()
{
	Release();
}




////////////////////////////////////////////////////////////////////////////// 
//
// �ڽ� : ���� 
//
void ynBox::Release()
{
	SAFE_RELEASE(m_pVB); 
	SAFE_RELEASE(m_pIB); 
 	SAFE_RELEASE(m_pTextureRV); 
}





/////////////////////////////////////////////////////////////////////////////
//
void ynBox::_InitDatas()
{
	m_pDev = nullptr;
	m_pDXDC = nullptr;
	m_pVB = nullptr;
	m_pIB = nullptr;

	m_pTextureRV = nullptr;
	_tcscpy(m_TexName, L"N/A");
	m_bTexOn = FALSE;

	XMStoreFloat4x4(&m_mTM, XMMatrixIdentity());
	m_vPos = VECTOR3(0,0,0);
	m_vRot = VECTOR3(0,0,0);
	m_vScl = VECTOR3(1,1,1);

	//m_Color = COLOR(1, 1, 1, 1);
}




/////////////////////////////////////////////////////////////////////////////
//
// �� ���� 
//
void ynBox::Reset()
{ 	 
	XMMATRIX m = XMMatrixIdentity();
	XMStoreFloat4x4(&m_mTM, m);
	//m_mTrans = m_mRot = m_mScl = m_mTM;
	m_vPos = VECTOR3(0, 0, 0);
	m_vRot = VECTOR3(0, 0, 0);
	m_vScl = VECTOR3(1, 1, 1);

	//m_Color = COLOR(1, 1, 1, 1);
}






/////////////////////////////////////////////////////////////////////////////
//
//  �ڽ� : ���� �� �ʿ� ���ҽ� �ε�.
//
int ynBox::Create(LPDEVICE pDev, LPDXDC pDXDC, TCHAR* texName)
{  

	m_pDev = pDev;
	m_pDXDC = pDXDC;

	//���� ����.
	_CreateVB();
	_CreateIB();
 
	//�ؽ�ó �ε� 
	if (texName)
	{
		ynLoadTexture(pDev, pDXDC, texName, &m_pTextureRV);		
		if (m_pTextureRV)
		{
			//�ؽ�ó ���� ȹ��. 
			//m_pTextureRv->GetLevelDesc(0, &m_TexSurfDesc);				//�ؽ�ó - ���ǽ� ���� ȹ��.
			//GetImageInfoFromFile(pBox->m_TexName, &pBox->m_TexImgInfo);	//�ؽ�ó - ���� �̹��� ���� ȹ��.
			_tcscpy(m_TexName, texName);
			m_bTexOn = TRUE;
		}
	}
	
  
	//���� ����.
	ZeroMemory(&m_Mtrl, sizeof(MATERIAL));
	m_Mtrl.Diffuse = m_Mtrl.Ambient = COLOR(1,1,1,1);
	m_Mtrl.Specular = COLOR(1, 1, 1, 1);
	m_Mtrl.Power = 30;


	//----------------------------------------------
	// �ܺ� ����(����) �����͵� : ���� Ŭ����ȭ �ʿ�.
	//----------------------------------------------
	// �̹� ���𿡼��� �ܺ� (����)��ü(������)�� ���������� ����,
	// �߰� �������̽� ������� �ܼ��ϰ� ó���ϰڽ��ϴ�. 
	extern cbDEFAULT  g_cbDef;
	extern cbMATERIAL g_cbMtrl;
	extern cbLIGHT    g_cbLit;
	extern ID3D11InputLayout*  g_pVBLayout;
	//extern LPXBUFFER  g_pCBDef;
	//extern LPXBUFFER  g_pCBMtrl;
	//extern LPXBUFFER  g_pCBLit;
	m_pVS = g_pVS;
	m_pPS = g_pPS;
	m_pLayout = g_pVBLayout;
	m_pcbDef = &g_cbDef;
	m_pcbMtrl = &g_cbMtrl;
	m_pcbLit = &g_cbLit;
	m_pCBDef = g_pCBDef;
	m_pCBMtrl = g_pCBMtrl;
	m_pCBLit = g_pCBLit;


	 
	return TRUE;
}
 





//////////////////////////////////////////////////////////////////////////////// 
//   
int ynBox::_CreateVB()
{  
	//���� ���� Ȯ��.  
	DWORD vbSize = sizeof(boxVtx);				//���� ũ��.
	m_VtxCnt = vbSize / sizeof(_VTX);			//���� ����.
	
	//���� ���� ����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				//���� �����
	bd.ByteWidth = vbSize;						//���� ũ��
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���� �뵵 : "���� ����" ��� ���� 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA rd;				
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = boxVtx;						//���ۿ� �� ������ ���� : "������"..
	
	//���� ���� ����.
	HRESULT hr = g_pDevice->CreateBuffer(&bd, &rd, &m_pVB);
	if (FAILED(hr))	return hr;

	//��ֶ��� ����.. 
	//g_NLine.Create(m_pDev, boxVtx, sizeof(boxVtx), 
	//			   sizeof(CUBE_VTX), COLOR(0, 1, 0, 1)); 
 
	return YN_OK;
} 





////////////////////////////////////////////////////////////////////////////////
//
int ynBox::_CreateIB()
{
	//���� ���� Ȯ��.  
	DWORD ibSize = sizeof(boxIdx);				//���� ũ��.
	m_FaceCnt = ibSize / sizeof(_INDEX);		//���� ����.
	m_IndexCnt = m_FaceCnt * 3;					//(IB �� ����Ͽ� �׸�) �� ��������.
		
	// �ε��� ���� Index Buffer ���� 
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;				//���� �����
	bd.ByteWidth = sizeof(boxIdx);				//���� ũ��
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;		//���� �뵵 : "���� ����" ��� ���� 
	bd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA rd;				
	ZeroMemory(&rd, sizeof(rd));
	rd.pSysMem = boxIdx;						//���ۿ� �� ������ ���� : "���� ����"..
	
	//���� ���� ����.
	HRESULT hr = g_pDevice->CreateBuffer(&bd, &rd, &m_pIB);
	if (FAILED(hr))	return hr;

	return YN_OK;
} 






/////////////////////////////////////////////////////////////////////////////
//
int ynBox::Update(float dTime /*=0*/)
{

	/*//�� ���� ����.
	if (IsKeyDown(VK_F9))		
	{
		Reset();
	}*/

	
	//-----------------------
	// ��ȯ ��� ���.
	//-----------------------
	XMMATRIX mScale = XMMatrixScaling(m_vScl.z, m_vScl.y, m_vScl.z);
	XMMATRIX mTrans = XMMatrixTranslation(m_vPos.x, m_vPos.y, m_vPos.z);
	XMMATRIX mRot = XMMatrixRotationRollPitchYaw(m_vRot.x, m_vRot.y, m_vRot.z);		
	XMMATRIX mTM = mScale * mRot * mTrans;

	//���� ���� (������)
	XMStoreFloat4x4(&m_mTM, mTM);


	//-----------------------
	// ������� ���� --> �������� ó��.
	//-----------------------
	// �̹� ���𿡼��� �ܺ� (����)��ü(������)�� ���������� ����,
	// �߰� �������̽� ������� �ܼ��ϰ� ó���ϰڽ��ϴ�. 	
	/*m_cbDef = g_cbDef;
	m_pcbDef->mTM = mTM;  
	m_pcbDef->mWV = mTM * m_pcbDef->mView;	

	//���̴� ��� ���� ����.(��������) --> �������� ó��.
	UpdateDynamicConstantBuffer(m_pDXDC, g_pCBDef, &m_cbDef, sizeof(cbDEFAULT));
	//���̴� ��� ���� ����.(��������)
	//m_pDXDC->UpdateSubresource(m_pCBDef, 0, nullptr, &m_cbDef, 0, 0);
	*/

	//-----------------------
	// ���� ���� ���� : �ǽð� ��ȭ�� ���ٸ� ���� ����. --> �������� ó��. 
	//-----------------------
	/*m_Mtrl.Diffuse = COLOR(1, 1, 1, 1);
	m_Mtrl.Ambient = COLOR(1, 1, 1, 1);
	m_Mtrl.Power = 30.0f;					

	//���� ���� ������� ����.
	m_cbMtrl.Diffuse = XMLoadFloat4(&m_Mtrl.Diffuse); 
	m_cbMtrl.Ambient = XMLoadFloat4(&m_Mtrl.Ambient);
	m_cbMtrl.Power = m_Mtrl.Power;						
	//������� ���� : --> �������� ó��. 
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBMtrl, &m_cbMtrl, sizeof(cbMATERIAL));
	*/
 

	return YN_OK;
}




/////////////////////////////////////////////////////////////////////////////
//
void ynBox::SetTM(XMMATRIX* pTM)
{ 	
	if (pTM == nullptr) return;
	
	XMStoreFloat4x4(&m_mTM, *pTM);

}





/////////////////////////////////////////////////////////////////////////////
//
int ynBox::Draw(XMMATRIX* pTM/*=NULL*/)
{  	

	if (pTM)			//�ܺ� ���� ��� ����.
	{
		SetTM(pTM);
	}


	//--------------------------------------
	// �ܺ� ���� ������ �ɼ� �� ��� ���� ����.
	//--------------------------------------
	// ��鳻�� ����/�𵨺� ������ �ɼ��� ������ �� �ֱ⿡ ���� ó���� �ʿ��մϴ�.
	// �� �� ��ü���� �ܺ��� ��� ���۸� �������̹Ƿ� �׸��� ���� �����ؾ� �մϴ�. 
	// ���� ���ֺ� ������۰� �غ�Ǿ� �ִٸ� �̷� ������ ���ʿ� �մϴ�. 
	m_pcbDef->mTM = XMLoadFloat4x4(&m_mTM);
	m_pcbDef->mWV = m_pcbDef->mTM * m_pcbDef->mView;	
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBDef, m_pcbDef, sizeof(cbDEFAULT));
	 
	//���� ���� ������� ����.
	m_pcbMtrl->Diffuse = XMLoadFloat4(&m_Mtrl.Diffuse); 
	m_pcbMtrl->Ambient = XMLoadFloat4(&m_Mtrl.Ambient);
	m_pcbMtrl->Specular = XMLoadFloat4(&m_Mtrl.Specular);
	m_pcbMtrl->Power   = m_Mtrl.Power;				
	m_pcbMtrl->bTexOn = m_bTexOn ? TRUE : FALSE;			//�ؽ�ó ���� On/Off.
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBMtrl, m_pcbMtrl, sizeof(cbMATERIAL));
	
	


  	//���� ���� ����
	UINT stride = sizeof(_VTX);
	UINT offset = 0;
	g_pDXDC->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);		//�������� ����.
	m_pDXDC->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, offset);		//���ι��� ����.

	//�Է� ���̾ƿ� ����. Set the input layout 
	m_pDXDC->IASetInputLayout(m_pLayout);

	// ���� ���� ���� ���� Set primitive topology
	m_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//���̴� ����. : ǥ�� ���� ���̴� ���. 
	m_pDXDC->VSSetShader(m_pVS, nullptr, 0);
	m_pDXDC->PSSetShader(m_pPS, nullptr, 0);

	//������� ���� 
	m_pDXDC->VSSetConstantBuffers(0, 1, &m_pCBDef);		//��� ���� ����.(VS)
	m_pDXDC->VSSetConstantBuffers(1, 1, &m_pCBLit);		//���� ���� ����.(VS)
	m_pDXDC->VSSetConstantBuffers(2, 1, &m_pCBMtrl);	//���� ���� ����.(VS)
	m_pDXDC->PSSetConstantBuffers(2, 1, &m_pCBMtrl);	//���� ���� ����.(PS)
	//���̴� ���ҽ� ����.
	m_pDXDC->PSSetShaderResources(0, 1, &m_pTextureRV);	//PS �� �ؽ�ó ����.

	
	
	//�׸���! Render a triangle 
	m_pDXDC->DrawIndexed(m_IndexCnt, 0, 0);
	//m_pDXDC->Draw(m_VtxCnt, 0);


	//��ֶ��� �׸���.
	//if(g_bNLine) g_NLine.Draw(&g_mTM);

	//��ġ ���� ���� 
	//m_pDXDC->ClearState();

	
	return YN_OK;
}


 
/////////////////////////////////////////////////////////////////////////////
//
// �׸��� ��¿� : ����, �ؽ�ó �����ܺο��� ����.
//
int ynBox::DrawShadow(XMMATRIX* pTM/*=NULL*/)
{  	
	if (pTM)			//�ܺ� ���� ��� ����.
	{
		SetTM(pTM);
	}


	//--------------------------------------
	// ��� ���� ���� : �׸��� ��� ����.
	//--------------------------------------
	m_pcbDef->mTM = XMLoadFloat4x4(&m_mTM);
	m_pcbDef->mWV = m_pcbDef->mTM * m_pcbDef->mView;	
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBDef, m_pcbDef, sizeof(cbDEFAULT));
	 
	//���� ���� ������� ���� : 50% ����.
	m_pcbMtrl->Diffuse = XMLoadFloat4(&COLOR(0, 0, 0, 0.5f)); 
	//m_pcbMtrl->Ambient = XMLoadFloat4(&m_Mtrl.Ambient);
	//m_pcbMtrl->Specular = XMLoadFloat4(&m_Mtrl.Specular);
	//m_pcbMtrl->Power   = m_Mtrl.Power;				
	m_pcbMtrl->bTexOn = m_bTexOn ? TRUE : FALSE;			//�ؽ�ó ���� On/Off.
	UpdateDynamicConstantBuffer(m_pDXDC, m_pCBMtrl, m_pcbMtrl, sizeof(cbMATERIAL));
	
	


  	//���� ���� ����
	UINT stride = sizeof(_VTX);
	UINT offset = 0;
	g_pDXDC->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);		//�������� ����.
	m_pDXDC->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, offset);		//���ι��� ����.

	//�Է� ���̾ƿ� ����. Set the input layout 
	m_pDXDC->IASetInputLayout(m_pLayout);

	// ���� ���� ���� ���� Set primitive topology
	m_pDXDC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//���̴� ����. : �׸��� ���̴� ����. (�ܺ�����)
	m_pDXDC->VSSetShader(m_pVS, nullptr, 0);
	m_pDXDC->PSSetShader(m_pPS, nullptr, 0);

	//������� ���� 
	m_pDXDC->VSSetConstantBuffers(0, 1, &m_pCBDef);		//��� ���� ����.(VS)
	m_pDXDC->VSSetConstantBuffers(1, 1, &m_pCBLit);		//���� ���� ����.(VS)
	m_pDXDC->VSSetConstantBuffers(2, 1, &m_pCBMtrl);	//���� ���� ����.(VS)
	m_pDXDC->PSSetConstantBuffers(2, 1, &m_pCBMtrl);	//���� ���� ����.(PS)
	//���̴� ���ҽ� ����.
	m_pDXDC->PSSetShaderResources(0, 1, &m_pTextureRV);	//PS �� �ؽ�ó ����.

	
	
	//�׸���! Render a triangle 
	m_pDXDC->DrawIndexed(m_IndexCnt, 0, 0);
	//m_pDXDC->Draw(m_VtxCnt, 0);


	//��ֶ��� �׸���.
	//if(g_bNLine) g_NLine.Draw(&g_mTM);

	//��ġ ���� ���� 
	//m_pDXDC->ClearState();



	return YN_OK;
}














/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// ynBox ���� ���� �Լ���
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
//  ynBoxCreate : ���� ���� ���� �� �ʿ� ���ҽ� �ε�.
//
int ynBoxCreate(LPDEVICE pDev, LPDXDC pDXDC, TCHAR* texName, ynBox** ppBox)
{ 

	//�ڽ� ����..
	ynBox* pBox = new ynBox;
	pBox->Create(pDev, pDXDC, texName); 
	  

	//�ܺη� ����..
	*ppBox = pBox;
	

	return TRUE;

}






 

////////////////////////////////////////////////////////////////////////////// 
//
// ynLoadTexture : �ؽ�ó �ε�
//
int ynLoadTexture(LPDEVICE pDev, LPDXDC pDXDC, TCHAR* filename, LPTEXTURERV* ppTexRV)
{
	HRESULT hr = S_OK;	
	 
	LPTEXTURERV pTexRV = nullptr;

	//TCHAR* fileName = L"../data/sonim.jpg";
	
	//�Ϲ� �ؽ�ó �ε�.
	//hr = DirectX::CreateWICTextureFromFile( pDev, fileName, nullptr, &g_pTextureRV );	
	
	//�Ӹ�+�ؽ�ó �ε�.
	hr = DirectX::CreateWICTextureFromFileEx( pDev, pDXDC, filename, 0, 
				D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, //| D3D11_BIND_RENDER_TARGET,
				0,	D3D11_RESOURCE_MISC_GENERATE_MIPS,	WIC_LOADER_DEFAULT, 
				nullptr, &pTexRV );	

	if (FAILED(hr))
	{
		//DDS ���Ϸ� �ε� �õ�. : +�Ӹ�
	/*	hr = DirectX::CreateDDSTextureFromFileEx( pDev, pDXDC, filename, 0, 
				D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, //| D3D11_BIND_RENDER_TARGET,
				0,	D3D11_RESOURCE_MISC_GENERATE_MIPS,	false, //_LOADER_DEFAULT, 
				nullptr, &pTexRV );	 */
		hr = DirectX::CreateDDSTextureFromFile( pDev, filename, nullptr, &pTexRV );	
		if (FAILED(hr))
		{
			ynError(hr, L"�ؽ�ó �ε� ���� \nFile=%s", filename);
			return hr;
		}
	}

	

	//�ؽ�ó �Ӹ����.
	//pDXDC->GenerateMips(g_pTextureRV);
	
	//�ܺη� ����.
	*ppTexRV = pTexRV;

	return YN_OK;
}





/////////////////////////////////////////////////////////////////////////////
//




/************************* end of file "ynBox.cpp " **************************/