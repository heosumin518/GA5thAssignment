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
// ynFont.h: 폰트 헤더.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. ynFont / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning(disable:4996)
 
// DirectXTK : SpriteFont 재정의.
typedef SpriteFont  DXFONT;
typedef SpriteFont* LPDXFONT;
typedef SpriteBatch DXFONTBATCH;
typedef SpriteBatch* LPDXFONTBATCH;



/////////////////////////////////////////////////////////////////////////////
//
// class ynFont : 폰트 운용 클래스.
//
// DirectXTK : SpriteFont 기반 폰트 운용 클래스
// 2D Texture 기반의 폰트 출력 라이브러리.
// 참조 : https://directxtk.codeplex.com/wikipage?title=SpriteFont
// #include "SpriteFont.h"	
//
/////////////////////////////////////////////////////////////////////////////

class ynFont
{
protected:

	LPDEVICE		m_pDev;	
	//LPDXFONT		m_pFont;
	LPDXFONT		m_pFont;		// DirectXTK : SpriteFont 클래스 사용함.	
	LPDXFONTBATCH	m_pFontBatch;


	TCHAR  m_FontName[80];
	/* 외부에서 로등된 완성형 글자를 사용함. 아래 데이터는 무시.
	UINT   m_Width,
		   m_Height,
		   m_Weight;
	BOOL   m_bUseItalic;
	*/

	//	 
	// 연속 메세지 출력 관련. 2009.08.17
	//
	enum {
		MAX_W_ = 80 * 8, //가로 80글자.
		MAX_H_ = 40 * 16, //세로 40줄.
		MAX_CHARSIZE_ = 4096,
	};
	char _PrintMsg[MAX_CHARSIZE_];	//문자열 저장소. 
	int	 _PrintCntX;		//문자열 출력 현재 가로위치
	int	 _PrintCntY;		//문자열 출력 현재 세로위치 
	int  _PrintPosX;		//문자열 출력 위치 (화면) 
	int  _PrintPosY;		//문자열 출력 위치 (화면)  
	float	_PrintHoldTime;
	INT		_PrintHeight;
	bool	_PrintHold;
	float	_PrintTime;
	TCHAR	_wmsg[MAX_CHARSIZE_];
	float	_CursorTime;
	BOOL	_bShowCursor;

public:
	ynFont();
	virtual ~ynFont();

	virtual int  Create(LPDEVICE pDev, TCHAR* filename, float lineSpace = 14.0f);
	virtual void Release();
	virtual void Update(float dTime);
	virtual void Draw(float dTime);
	//virtual void Apply(LPDXDC pDXDC);
	//virtual void Clear();
	
	//virtual void Restore();		// 폰트 복구 함수	
	//virtual void Remove();		// 디바이스에서 폰트 제거
	
	// 정적 함수.
	//static void Log(BOOL bShowMsgBox, LPCTSTR msg, ...);
	
	// 폰트 얻기.
	LPDXFONT GetFont() { return m_pFont; }

	// 폰트 출력함수.
	INT Draw(int x, int y, TCHAR* msg, COLOR col );
	INT Draw(int x, int y, COLOR col, TCHAR* msg, ...);
	//INT Draw(RECT& rRect, TCHAR* msg, COLOR color);
	
	//세로출력 관련 메소드.2009.08.17.
	INT DrawSV(int x, int y, COLOR col, TCHAR* msg, float dTime);
	//INT CalcRect(int width, int height, TCHAR* msg);

};

//typedef ynFont ynShader;



 
///////////////////////////////////////////////////////////////////////////////
//
//  폰트 전역 데이터들.
//  
///////////////////////////////////////////////////////////////////////////////

namespace YENA
{
	// DirectXTK : Sprite Font 클래스
	extern ynFont* ypFont;
	//extern SpriteFont* ypFont;
	//SpriteBatch* ypFontBatch;

	// DX10 용 폰트 객체
	//ID3DX10Font* ypFont = NULL;
	// DX9 용 폰트. 2004.10.21. 
	//LPD3DXFONT	ypFont;


	// 폰트 전역 함수들 : 비클래스 함수 버전.
	int  ynFontCreate(LPDEVICE pDev);
	void ynFontRelease();
	YENA_API void ynTextDraw(int x, int y, COLOR col, TCHAR* msg, ...);
	YENA_API void ynTextDrawSV(int x, int y, COLOR col, TCHAR* msg, float dTime);



}
using namespace YENA;



/****************** end of file "ynFont.h" *******************************/