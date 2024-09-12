//
// ynMesh.h : 예나 기본 메쉬 정보 헤더
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once

namespace YENA {




/////////////////////////////////////////////////////////////////////////////
//
// 예나 기본 메쉬 모델 운용 클래스.
// 
/////////////////////////////////////////////////////////////////////////////
//
// 예나 기본 메쉬
//  - XYZ + NORMAL 구조
//  - VERTEX BUFFER 지원
//  - INDEX BUFFER 지원
//  - 각종 버퍼 자동화 운용
//  - 메쉬 운용 API 지원
//
// * 외부(사용자) 처리 부분.
//  - 조명, 재질 설정. 
//  - 텍스쳐 로드 및 설정.
//  - 셰이더 설정. 
//
/////////////////////////////////////////////////////////////////////////////

class ynMesh
{
public:
	// 정점 구조체 
	struct VERTEX
	{
		float x, y, z; 			//좌표(Position)
		float nx, ny, nz;		//노멀 : Normal 	 
	};


	//인덱스 구조체.
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


	DWORD m_VtxCnt;			//정점 개수.
	DWORD m_FaceCnt;		//페이스 개수 (Index 개수)	 
	DWORD m_IndexedVtxCnt;	//(IB 를 사용하여 그릴) 총 정점개수.

	//float m_fScale;		//스케일
	TCHAR m_TypeName[20];	//모델 이름

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
// 메쉬 전역 함수들.
//
YENA_API int ynMeshCreateBox(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
YENA_API int ynMeshCreateSphere(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
YENA_API int ynMeshCreateCorn(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
YENA_API int ynMeshCreateTeapot(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
YENA_API int ynMeshCreateTeapotHD(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
//int ynMeshCreateWoodBox	(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
//int ynMeshCreateCheckBox(LPDEVICE pDev, LPDXDC pDXDC, ynMesh** ppMesh);
//YENA_API int ynMeshCreateFromFile(LPDEVICE pDev, TCHAR* filename, ynMesh** ppMesh);


//extern VERTEX g_VtxData[];		//정점데이터 : "메쉬"
//extern DWORD g_VtxCnt;			//정점개수.


} // end of namespace YENA


/****************** end of file "ynMesh.h" ***********************************/