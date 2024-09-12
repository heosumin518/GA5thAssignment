/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//  B3/YENA : 2D/3D ������ ����.						                       // 
//                                                                         //
//  �� �� : Kihong Kim / mad_dog@hanmail.net                               // 
//          Zero-G Tech,Inc http://www.zero-g.kr						   //	
//  �ʱⰳ�� : 2003.12 - 2004.03.                                          //
//  ����ȯ�� : OS - Windows 2k SP4 / Visual Studio 6.0/2003/2005/2008       //
//                  DirectX 9.0b(2003, summer)                             //
//                  DirectX 9.0c(2007, November)                           //
//                  OpenGL 1.x / 2.x                                       //
//             H/W - AMD Athlon XP(Barton) 2500+@2300                      //    
//                   ATI Radeon 9600 Pro                                   //
//                   ABit NF7-II /w SAMSUNG DDR 512MB                      //
//                   SB Live! DE 5.1                                       //
//  �׽�Ʈ ȯ�� : CPU - AMD Athlon										   //	 
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
// ynFont.h: ��Ʈ ���.
//
// 2003. Kihong Kim / mad_dog@hanmail.net
// 2006.08. Update.
// 2011.04. Update.
// 2017.01. Update. ynFont / DX11.1 (Windows SDK 8.1)
//
//////////////////////////////////////////////////////////////////////////////
#pragma once
#pragma warning(disable:4996)
 
// DirectXTK : SpriteFont ������.
typedef SpriteFont  DXFONT;
typedef SpriteFont* LPDXFONT;
typedef SpriteBatch DXFONTBATCH;
typedef SpriteBatch* LPDXFONTBATCH;



/////////////////////////////////////////////////////////////////////////////
//
// class ynFont : ��Ʈ ��� Ŭ����.
//
// DirectXTK : SpriteFont ��� ��Ʈ ��� Ŭ����
// 2D Texture ����� ��Ʈ ��� ���̺귯��.
// ���� : https://directxtk.codeplex.com/wikipage?title=SpriteFont
// #include "SpriteFont.h"	
//
/////////////////////////////////////////////////////////////////////////////

class ynFont
{
protected:

	LPDEVICE		m_pDev;	
	//LPDXFONT		m_pFont;
	LPDXFONT		m_pFont;		// DirectXTK : SpriteFont Ŭ���� �����.	
	LPDXFONTBATCH	m_pFontBatch;


	TCHAR  m_FontName[80];
	/* �ܺο��� �ε�� �ϼ��� ���ڸ� �����. �Ʒ� �����ʹ� ����.
	UINT   m_Width,
		   m_Height,
		   m_Weight;
	BOOL   m_bUseItalic;
	*/

	//	 
	// ���� �޼��� ��� ����. 2009.08.17
	//
	enum {
		MAX_W_ = 80 * 8, //���� 80����.
		MAX_H_ = 40 * 16, //���� 40��.
		MAX_CHARSIZE_ = 4096,
	};
	char _PrintMsg[MAX_CHARSIZE_];	//���ڿ� �����. 
	int	 _PrintCntX;		//���ڿ� ��� ���� ������ġ
	int	 _PrintCntY;		//���ڿ� ��� ���� ������ġ 
	int  _PrintPosX;		//���ڿ� ��� ��ġ (ȭ��) 
	int  _PrintPosY;		//���ڿ� ��� ��ġ (ȭ��)  
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
	
	//virtual void Restore();		// ��Ʈ ���� �Լ�	
	//virtual void Remove();		// ����̽����� ��Ʈ ����
	
	// ���� �Լ�.
	//static void Log(BOOL bShowMsgBox, LPCTSTR msg, ...);
	
	// ��Ʈ ���.
	LPDXFONT GetFont() { return m_pFont; }

	// ��Ʈ ����Լ�.
	INT Draw(int x, int y, TCHAR* msg, COLOR col );
	INT Draw(int x, int y, COLOR col, TCHAR* msg, ...);
	//INT Draw(RECT& rRect, TCHAR* msg, COLOR color);
	
	//������� ���� �޼ҵ�.2009.08.17.
	INT DrawSV(int x, int y, COLOR col, TCHAR* msg, float dTime);
	//INT CalcRect(int width, int height, TCHAR* msg);

};

//typedef ynFont ynShader;



 
///////////////////////////////////////////////////////////////////////////////
//
//  ��Ʈ ���� �����͵�.
//  
///////////////////////////////////////////////////////////////////////////////

namespace YENA
{
	// DirectXTK : Sprite Font Ŭ����
	extern ynFont* ypFont;
	//extern SpriteFont* ypFont;
	//SpriteBatch* ypFontBatch;

	// DX10 �� ��Ʈ ��ü
	//ID3DX10Font* ypFont = NULL;
	// DX9 �� ��Ʈ. 2004.10.21. 
	//LPD3DXFONT	ypFont;


	// ��Ʈ ���� �Լ��� : ��Ŭ���� �Լ� ����.
	int  ynFontCreate(LPDEVICE pDev);
	void ynFontRelease();
	YENA_API void ynTextDraw(int x, int y, COLOR col, TCHAR* msg, ...);
	YENA_API void ynTextDrawSV(int x, int y, COLOR col, TCHAR* msg, float dTime);



}
using namespace YENA;



/****************** end of file "ynFont.h" *******************************/