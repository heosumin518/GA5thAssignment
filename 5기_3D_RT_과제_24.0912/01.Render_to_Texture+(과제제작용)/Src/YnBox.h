//
// ynYena :: 예나 상자 클래스
// 2003-2018. Kihong Kim  / mad_dog@hanmail.net
// Zero-G Interactive / http://www.zero-g.kr 
//
// DirectX 기준 버전 : DX90c.Nov.2007
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
// 인터페이스 재정의 (DX9)
typedef LPDIRECT3DDEVICE9			LPDEVICE;
typedef LPDIRECT3DTEXTURE9			LPTEXTURE;
typedef LPDIRECT3DVERTEXBUFFER9		LPVB;
typedef LPDIRECT3DINDEXBUFFER9		LPIB;
*/
// 인터페이스 재정의 (DX11)
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
// ynBox 클래스
//
///////////////////////////////////////////////////////////////////////////////

class ynBox {

public:
	//
	// Box 용 정점 구조체 
	//
	struct _VTX
	{
		float x, y, z;				//위치.
		float nx, ny, nz;			//노멀
		//DWORD color;				// diffuse color 
		float u, v;					// 텍스처 좌표.

	};

	//
	// 인덱스 버퍼용 구조체. (32bit 기준)
	//
	struct _INDEX
	{
		DWORD A, B, C;		//0~4,194,967,295 
		//WORD A, B, C;		//0~65,535
	};


	//#define ynBox_FVF (D3DFVF_XYZ| D3DFVF_NORMAL | D3DFVF_TEX1)	 
	//#define ynBox_FVF (D3DFVF_XYZ| D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	 

protected:

	//렌더링 장치 인터페이스.
	LPDEVICE	m_pDev;
	LPDXDC      m_pDXDC;

	//메쉬/기하 정보.
	LPVB		m_pVB; 
	LPIB		m_pIB; 

	DWORD		m_VtxCnt;
	DWORD		m_FaceCnt;
	DWORD		m_IndexCnt;
	//DWORD		m_Stride;
	//DWORD		m_Offset;
 
	//매쉬/재질 정보.
	MATERIAL	m_Mtrl; 	
	LPTEXTURERV	m_pTextureRV;	 
	TCHAR		m_TexName[256];	
	//D3DSURFACE_DESC	 m_TexSurfDesc;	 
	//D3DXIMAGE_INFO	 m_TexImgInfo;	 
	BOOL		m_bTexOn;


	//각 유닛(모델) 별 고유 변환 정보.
	MATRIX		m_mTM; 
	//MATRIX  m_mTrans, m_mRot, m_mScl;
	VECTOR3		m_vPos;
	VECTOR3		m_vRot;
	VECTOR3		m_vScl;

		
	
	//----------------------------------------------
	// 외부 참조(공유) 데이터들 : 추후 클래스화 필요.
	//----------------------------------------------
	//셰이더 객체.(외부참조)
	LPVERTEXSHADER m_pVS;
	LPPIXELSHADER  m_pPS;

	//셰이더 객체3 : 그림자용. (외부참조)
	//LPVERTEXSHADER m_pVS3;
	//LPPIXELSHADER  m_pPS3;
		
	//정점 입력구조.(외부참조)
	LPLAYOUT  m_pLayout;
		
	//셰이더 상수 임수 버퍼들 (외부 참조)
	cbDEFAULT*  m_pcbDef;
	cbMATERIAL* m_pcbMtrl;
	cbLIGHT*	m_pcbLit;

	//셰이더 상수 버퍼들 (외부 참조)
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