/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  B3/YENA : 2D/3D 렌더링 엔진.						                       // 
//                                                                         //
//  제 작 : Kihong Kim / mad_dog@hanmail.net                               // 
//          Zero-G Tech,Inc http://www.zero-g.kr						   //	
//  초기개발 : 2003.12 - 2004.03.                                          //
//  개발환경 : OS - Windows 2k SP4 / Visual Studio 6.0/2003/2005/2008       //
//                  DirectX 9.0b(2003, summer)                             //
//                  DirectX 9.0c(2007, November)                           //
//                  OpenGL 1.x / 2.x                                       //
//             H/W - AMD Athlon XP(Barton) 2500+@2300                      //    
//                   ATI Radeon 9600 Pro                                   //
//                   ABit NF7-II /w SAMSUNG DDR 512MB                      //
//                   SB Live! DE 5.1                                       //
//  테스트 환경 : CPU - AMD Athlon										   //	 
//                      AMD Athlonx64 w/q4                                 // 
//                      Intel Pentium III/IV & Celeron                     //
//                      Core2 w/Quad                                       //
//                VGA - ATI Radeon 9xxx Series                             //
//                          Radeon 1xxx Series                             //
//                          Radeon HD 3/4/5/6 Series                       //
//                      nVidia GeForce 2 MX/Ti Series.                     //  
//                             GeForce 3 Ti Series.                        //
//                             GeForce 4 MX/Ti Series.                     //
//                             GeForce FX Series.                          //
//                             GeForce 5/6/7/8/9xxx Series.                //
//                             GeForce 2/4xx GTS Series.                   //
//                             GeForce 7xx GTX Series.                     //
//                             GeForce 2Way-SLI / 3Way-SLI		   	       //
//                OS - Windows 98 / 2K / XP / VISTA / Win7 x32:x64         //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////
//
// ynStateObject.h : 상태객체 헤더
//
// DirectX 기준 버전 : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
//----------------------------------------------------------------------------
// 상태 객체 State Objects (DX10/11)
// DX10 부터 구형 TnL 의 RenderState 가 제거되었습니다.
// 이를 대신하는 것이 상태객체 State Objects 인터페이스로, 렌더링 상태별 그룹으로 묶고
// 렌더링시 디바이스에 설정합니다.  이를 통해 장치의 여러 상태 변화를 한번에 설정하여 
// 불필요한 연산부하(Overhead) 를 줄이고 보다 효과적인 렌더링을 가능케 합니다.
// 상태객체는 엔진 초기시 제작후 사용하기를 권장하며 종료시 제거(Release) 해야 합니다.
// 상태객체는 수정불가능(Immutable, 읽기전용) 개체 입니다.
// DX9 에서는 State-Block 이 이와 유사한 기능을 담당합니다. 
//
// 상태 객체 인터페이스 중 '레스터라이즈 스테이지 Rasterize Stage' 상태 조절은 
// ID3D11RasterizerState 인터페이스를 통해 처리합니다.  
// 렌더링 설정/기능 모듬 정도로 생각합시다.
//----------------------------------------------------------------------------

#pragma once



/////////////////////////////////////////////////////////////////////////////
//
// class ynStateObject : 상태객체 운용 클래스.
//
class ynStateObject
{
protected:

public:
	ynStateObject();
	virtual ~ynStateObject();

	virtual void Release();
	virtual int  Load(LPDEVICE pDev);
	virtual void Update(LPDXDC pDXDC);
	virtual void Apply(LPDXDC pDXDC);
	virtual void Clear();
	
};

//typedef ynStateObject ynShader;





//////////////////////////////////////////////////////////////////////////////// 
//
// 상태객체 전역 데이터들.
//
//////////////////////////////////////////////////////////////////////////////// 

//전역 공통 상태객체들.
namespace YENA
{

//
// 상태객체 전역 함수 StateObject Functions
//
int  ynStateObjectCreate(LPDEVICE pDev);
void ynStateObjectRelease();
int  ynStateObjectUpdate();

int  ynStateObjectBackup();			//이전상태 저장.
int  ynStateObjectRestore();		//이전상태 복구.

//
// 렌더링 상태 객체 생성 / 운용 함수들.
//
//int  ynRenderStateObjectCreate(LPDEVICE pDev);
//void ynRenderStateObjectRelease();
//void ynRenderStateUpdate();


//
// 깊이/스텐실 테스트 상태들.
//
enum {
	YNDS_DEPTH_ON,		//깊이버퍼 ON! (기본값), 스텐실버퍼 OFF.
	YNDS_DEPTH_OFF,		//깊이버퍼 OFF!, 스텐실 OFF.
	YNDS_DEPTH_ON_WRITE_OFF,	//깊이버퍼 쓰기 끄기, 스텐실 OFF. 

	//스텐실 버퍼 연산용.
	YNDS_DEPTH_ON_STENCIL_ON,				//깊이버퍼 On (Write On), 스텐실버퍼 ON (참조값 쓰기) : "깊이/스텐실 기록".
	YNDS_DEPTH_ON_STENCIL_EQUAL_KEEP,		//깊이버퍼 On (Write On), 스텐실버퍼 ON (동일비교, 성공시 유지) : "지정 위치에만 그리기".
	YNDS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP,	//깊이버퍼 On (Write On), 스텐실버퍼 ON (다름비교, 성공시 유지) : "지정 위치 이외에 그리기".
	
	//YNDS_DEPTH_OFF_STENCIL_ON,			//깊이버퍼 Off, 스텐실버퍼 ON (참조값 쓰기) : "스텐실만 기록"
	//YNDS_DEPTH_ON_STENCIL_EQUAL_INCR,	//깊이버퍼 On (Write On), 스텐실버퍼 ON (동일비교, 성공시 증가) : "이중그리기 방지".
	YNDS_DEPTH_WRITE_OFF_STENCIL_ON,		//깊이버퍼 On (Write Off), 스텐실버퍼 ON (항상기록, 성공시 증가) :" 스텐실만 기록".
	YNDS_DEPTH_OFF_STENCIL_EQUAL_INCR,	//깊이버퍼 Off (Write Off), 스텐실버퍼 ON (동일비교, 성공시 증가) : "이중그리기 방지.
	

	//열거 상수 최대값.
	YNDS_MAX_,	

	//상수 재정의. MAX 값 뒷쪽에 정의해야 함. 주의.
	YNDS_DEPTH_ON_STENCIL_OFF = YNDS_DEPTH_ON,			//깊이버퍼 ON, 스텐실버퍼 OFF
	YNDS_DEPTH_ONLY	= YNDS_DEPTH_ON_STENCIL_OFF,		//깊이버퍼 ON, 스텐실버퍼 OFF
	//YNDS_STENCIL_ONLY = YNDS_DEPTH_OFF_STENCIL_ON,	//깊이버퍼 OFF, 스텐실버퍼 ON
};

//깊이/스텐실 상태 객체들
extern YENA_APIX LPDEPTHSTENCILSTATE ypDSState[YNDS_MAX_];

int  ynDepthStencilStateCreate(LPDEVICE pDev);
void ynDepthStencilStateRelease();



// 
// 레스터라이져 상태 객체 Rasterizer State Objects
// 
enum {
	YNRS_SOLID,				//삼각형 채우기 : Fill Mode - Soild.
	YNRS_WIREFRM,			//삼각형 채우기 : Fill Mode - Wireframe.
	YNRS_CULLBACK,			//뒷면 컬링 (ON) : Back-Face Culling - "CCW" 
	YNRS_CULLFRONT,			//정면 컬링 (ON) : Front-Face Culling - "CW" 	
	YNRS_WIRECULLBACK,		//와이어 프레임 + 뒷면 컬링 (ON) 
	YNRS_WIRECULLFRONT,		//와이어 프레임 + 앞면 컬링 (ON) 	
	
	//열거 상수 최대값.
	YNRS_MAX_,

	//상수 재정의. MAX 값 뒷쪽에 정의해야 함. 주의.
	YNRS_CULL_CCW = YNRS_CULLBACK,
	YNRS_CULL_CW = YNRS_CULLFRONT,
	YNRS_WIRE_CULL_CCW = YNRS_WIRECULLBACK,
	YNRS_WIRE_CULL_CW = YNRS_WIRECULLFRONT,	

};
//레스터라이져 상태 객체 배열
extern YENA_APIX LPRASTERSTATE	ypRState[YNRS_MAX_];

void ynRasterStateCreate(LPDEVICE pDev);
void ynRasterStateRelease();
//void ynRasterModeUpdate();


//
// 렌더링 모드 : 다수의 렌더링 상태 조합 및 운용을 위한 정의.
//
enum {
	YNRM_SOLID		= 0x0000,		// 삼각형채우기 : ON, Solid
	YNRM_WIREFRAME	= 0x0001,		// 삼각형채우기 : OFF, Wire-frame
	YNRM_CULLBACK	= 0x0002,		// 뒷면 컬링 : ON, "CCW"
	
	//렌더링 기본모드 : Solid + Cull-On.
	YNRM_DEFAULT		= YNRM_SOLID | YNRM_CULLBACK,	
	
};
extern YENA_APIX DWORD ynRMode;		//'현재' 렌더링 모드.



//
// 색상/알파 혼합 상태 객체 Blending State Objects 
//
enum {
	YNBS_DEFAULT,				//기본혼합 객체.
	YNBS_ALPHA_BLEND,			//알파 블랜딩. (Src.a + (1-Src.a))
	YNBS_COLOR_BLEND,			//색상 블랜딩. ( Src.Color + (1-Src.Color))
	YNBS_COLOR_BLEND_ONE,		//1:1 블랜딩. ( Src.Color + Dest.Color) 

	YNBS_MAX_,

	YNBS_AB_OFF = YNBS_DEFAULT,
	YNBS_AB_ON = YNBS_ALPHA_BLEND,
	//YNBS_AB_ALPHA = YNBS_ALPHA_BLEND,
	//YNBS_AB_COLOR = YNBS_COLOR_BLEND,
};

//색상/알파 혼합 상태 객체 
extern YENA_APIX LPBLENDSTATE ypBState[YNBS_MAX_];

void ynBlendStateCreate(LPDEVICE pDev);
void ynBlendStateRelease();
//void ynBlendModeUpdate();



//
// 텍스처 셈플러 상태 객체 : Texture Sampler State Objects.
//
enum {
	YNSS_CLAMP,			//텍스처 어드레스 모드 : 클램핑 (DX 기본값) 
	YNSS_WRAP,			//텍스처 어드레스 모드 : 렙핑 
	//YNSS_BOARDER,
	//YNSS_MIRROR,
	//YNSS_MIRRROONCE,
	
	YNSS_MAX,
	YNSS_DEFAULT = YNSS_WRAP,	//기본 셈플러
	
};
//텍스처 셈플러 상태 객체.
extern YENA_APIX LPSAMPLERSTATE ypSState[YNSS_MAX];
void ynSamplerCreate(LPDEVICE pDev);
void ynSamplerRelease();




} // end of namespace YENA
//using namespace YENA;




/****************** end of file "ynStateObject.h" ***********************************/