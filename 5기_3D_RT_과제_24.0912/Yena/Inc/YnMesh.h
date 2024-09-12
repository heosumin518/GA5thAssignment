//
// ynMesh.h : ���� �⺻ �޽� ���� ���
//
// DirectX ���� ���� : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once

namespace YENA {




/////////////////////////////////////////////////////////////////////////////
//
// ���� �⺻ �޽� �� ��� Ŭ����.
// 
/////////////////////////////////////////////////////////////////////////////
//
// ���� �⺻ �޽�
//  - XYZ + NORMAL ����
//  - VERTEX BUFFER ����
//  - INDEX BUFFER ����
//  - ���� ���� �ڵ�ȭ ���
//  - �޽� ��� API ����
//
// * �ܺ�(�����) ó�� �κ�.
//  - ����, ���� ����. 
//  - �ؽ��� �ε� �� ����.
//  - ���̴� ����. 
//
/////////////////////////////////////////////////////////////////////////////

class ynMesh
{
public:
	// ���� ����ü 
	struct VERTEX
	{
		float x, y, z; 			//��ǥ(Position)
		float nx, ny, nz;		//��� : Normal 	 
	};


	//�ε��� ����ü.
	struct INDEX
	{
		DWORD A, B, C;
	};


protected:

	LPDEVICE m_pDev;
	LPDXDC   m_pDXDC;

	VERTEX*	m_pVtxList;
	INDEX*  m_pIdxList;

	LPVERTEXBUFFER m_pVB;
	LPINDEXBUFFER  m_pIB;
	LPLAYOUT	   m_pLayOut;


	DWORD m_VtxCnt;			//���� ����.
	DWORD m_FaceCnt;		//���̽� ���� (Index ����)	 
	DWORD m_IndexedVtxCnt;	//(IB �� ����Ͽ� �׸�) �� ��������.

	//float m_fScale;		//������
	TCHAR m_TypeName[20];	//�� �̸�

protected:
	int _Create();
	int _CreateVB(VERTEX vtx[], DWORD vcnt, DWORD vbsize);
	int _CreateIB(INDEX idx[], DWORD fcnt);
	int _CreateLayout();
	
public:

	ynMesh();
	virtual ~ynMesh();	 

	virtual int  Create(LPDEVICE pDev, LPDXDC pDXDC, 
					   VERTEX* pVtxList, DWORD vtxCnt, 
		               INDEX* pIdxList, DWORD faceCnt,
					   TCHAR* name);
	virtual void Release();
	virtual int  Update(float dTime = 0);
	virtual int  Draw(float dTime = 0);

	virtual void Info(int& x, int& y, COLOR col);

	DWORD GetVtxCnt() { return m_VtxCnt; }
	DWORD GetFaceCnt() { return m_FaceCnt; }
 
};

typedef ynMesh*	 LPYNMESH;




/////////////////////////////////////////////////////////////////////////////
//
// �޽� ���� �Լ���.
//
YENA_API int ynMeshCreateBox(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
YENA_API int ynMeshCreateSphere(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
YENA_API int ynMeshCreateCorn(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
YENA_API int ynMeshCreateTeapot(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
YENA_API int ynMeshCreateTeapotHD(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
//int ynMeshCreateWoodBox	(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
//int ynMeshCreateCheckBox(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
//YENA_API int ynMeshCreateFromFile(LPDEVICE pDev, TCHAR* filename, ynMesh** ppMesh);


//extern VERTEX g_VtxData[];		//���������� : "�޽�"
//extern DWORD g_VtxCnt;			//��������.


} // end of namespace YENA


/****************** end of file "ynMesh.h" ***********************************/