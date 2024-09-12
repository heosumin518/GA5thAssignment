//
// ynResource.h : 예나 공통 리소스 운용 함수들.
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
#pragma warning(disable:4996)

#include "Yena.h"		//엔진 필수 헤더.
#include "ynMesh.h"		//메쉬 정보 헤더.



 

/////////////////////////////////////////////////////////////////////////////
//
// 전역 자원 데이터들.
//

// 리소스 파일 : Project 속성에  리소스 폴더 추가. 2017.12.14.
//#include "../Res/Sphere.ynmesh"  


//------------------------------------------------------------------------------
//
// 기본 메쉬 타입별 기하정보 정의.
//
// 메쉬파일을 파싱하지 않고 소스형태로 참조합니다. 
// 모든 메쉬는 기본크기 (1x1m) 를 유지.
// 정점+노멀 구성을 기본으로 합니다.
// 셰이더/텍스처/재질/조명등은 사용자가 설정하는 것을 상정하고
// 최소한의 구성 : 기하데이터/생성/그리기만 지원합니다.
// 자세한 것은 class ynMesh 를 참조하십시오.
//------------------------------------------------------------------------------
//FVF = XYZ|NORMAL
#include "Box.ynmesh"  
#include "Corn.ynmesh"  
#include "Sphere.ynmesh"  
#include "Teapot.ynmesh"		
#include "Teapot_HD.ynmesh"		//주전자 고폴리곤.

//FVF = XYZ|NORMAL|TEX
//#include "WoodBox.ynmesh"		//예나 나무상자.
//#include "CheckBox.ynmesh"		//예나 체크상자.







 

/****************** end of file "ynResource.cpp" ***********************************/