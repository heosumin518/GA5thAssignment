//
// ynYena :: ���� ���� Ŭ����
// 2003-2018. Kihong Kim  / mad_dog@hanmail.net
// Zero-G Interactive / http://www.zero-g.kr 
//
// DirectX ���� ���� : DX90c.Nov.2007
//
// ynBox.h :  
//
// 2003.11.4. 
// 2009.12.01. Update. (DX90c.Nov.2007)
// 2017.0.110. Update (DX11)
//

#pragma once
 
#ifndef YENA
/*
// �������̽� ������ (DX9)
typedef LPDIRECT3DDEVICE9			LPDEVICE;
typedef LPDIRECT3DTEXTURE9			LPTEXTURE;
typedef LPDIRECT3DVERTEXBUFFER9		LPVB;
typedef LPDIRECT3DINDEXBUFFER9		LPIB;
*/
// �������̽� ������ (DX11)
typedef ID3D11Device*				LPDEVICE;
typedef ID3D11DeviceContext*		LPDXDC;
typedef ID3D11InputLayout*			LPINPUTLAYOUT;
typedef ID3D11Buffer*				LPVB;
typedef ID3D11Buffer*				LPIB;
typedef ID3D11ShaderResourceView*	LPSRVIEW;
typedef LPSRVIEW					LPTEXURERV;

#endif


///////////////////////////////////////////////////////////////////////////////
//
// ynBox Ŭ����
//
///////////////////////////////////////////////////////////////////////////////

class ynBox {

public:
	//
	// Box �� ���� ����ü 
	//
	struct _VTX
	{
		float x, y, z;				//��ġ.
		float nx, ny, nz;			//���
		//DWORD color;				// diffuse color 
		float u, v;					// �ؽ�ó ��ǥ.

	};

	//
	// �ε��� ���ۿ� ����ü. (32bit ����)
	//
	struct _INDEX
	{
		DWORD A, B, C;		//0~4,194,967,295 
		//WORD A, B, C;		//0~65,535
	};


	//#define ynBox_FVF (D3DFVF_XYZ| D3DFVF_NORMAL | D3DFVF_TEX1)	 
	//#define ynBox_FVF (D3DFVF_XYZ| D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	 

protected:

	//������ ��ġ �������̽�.
	LPDEVICE	m_pDev;
	LPDXDC      m_pDXDC;

	//�޽�/���� ����.
	LPVB		m_pVB; 
	LPIB		m_pIB; 

	DWORD		m_VtxCnt;
	DWORD		m_FaceCnt;
	DWORD		m_IndexCnt;
	//DWORD		m_Stride;
	//DWORD		m_Offset;
 
	//�Ž�/���� ����.
	MATERIAL	m_Mtrl; 	
	LPTEXTURERV	m_pTextureRV;	 
	TCHAR		m_TexName[256];	
	//D3DSURFACE_DESC	 m_TexSurfDesc;	 
	//D3DXIMAGE_INFO	 m_TexImgInfo;	 
	BOOL		m_bTexOn;


	//�� ����(��) �� ���� ��ȯ ����.
	MATRIX		m_mTM; 
	//MATRIX  m_mTrans, m_mRot, m_mScl;
	VECTOR3		m_vPos;
	VECTOR3		m_vRot;
	VECTOR3		m_vScl;

		
	
	//----------------------------------------------
	// �ܺ� ����(����) �����͵� : ���� Ŭ����ȭ �ʿ�.
	//----------------------------------------------
	//���̴� ��ü.(�ܺ�����)
	LPVERTEXSHADER m_pVS;
	LPPIXELSHADER  m_pPS;

	//���̴� ��ü3 : �׸��ڿ�. (�ܺ�����)
	//LPVERTEXSHADER m_pVS3;
	//LPPIXELSHADER  m_pPS3;
		
	//���� �Է±���.(�ܺ�����)
	LPLAYOUT  m_pLayout;
		
	//���̴� ��� �Ӽ� ���۵� (�ܺ� ����)
	cbDEFAULT*  m_pcbDef;
	cbMATERIAL* m_pcbMtrl;
	cbLIGHT*	m_pcbLit;

	//���̴� ��� ���۵� (�ܺ� ����)
	LPXBUFFER   m_pCBDef;
	LPXBUFFER   m_pCBMtrl;
	LPXBUFFER   m_pCBLit;
	

protected:
	void _InitDatas();
	int	 _CreateVB();
	int  _CreateIB();
	

public:
	
	ynBox();
	~ynBox();

	int  Create	(LPDEVICE pDev, LPDXDC pDXDC, TCHAR* texName = NULL);
	void Release(); 
	int  Update	(float dTime = 0);
	int  Draw	(XMMATRIX* pTM = NULL);
	void Reset();

	int  DrawShadow	(XMMATRIX* pTM = NULL);
		
	void SetTM(XMMATRIX* pTM);
	void TexOn(BOOL bUse){ m_bTexOn = bUse; }
	
	void SetVS(LPVERTEXSHADER pVS) { m_pVS = pVS; }
	void SetPS(LPPIXELSHADER pPS) { m_pPS = pPS; }


	friend int ynBoxCreate(LPDEVICE pDev, LPDXDC pDXDC, TCHAR* texName, ynBox** ppBox);
};
 

int ynBoxCreate(LPDEVICE pDev, LPDXDC pDXDC, TCHAR* texName, ynBox** ppBox);
int ynLoadTexture(LPDEVICE pDev, LPDXDC pDXDC, TCHAR* texName, LPTEXTURERV* ppTexRv);




/**************** "end of file "ynBox.h" ************************************/