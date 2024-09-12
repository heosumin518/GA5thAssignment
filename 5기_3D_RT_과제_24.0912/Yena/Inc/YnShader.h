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
// ynShader.h : �⺻ ���̴� �����ӿ� ���
//
// DirectX ���� ���� : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

#pragma once

#ifndef LPVERTEXSHADER
typedef ID3D11VertexShader*   LPVERTEXSHADER;
typedef ID3D11GeometryShader* LPGEOMETRYSHADER;
typedef ID3D11PixelShader*    LPPIXELSHADER;
typedef ID3D11ComputeShader*  LPCOMPUTESHADER;
//typedef ID3DBlob*			  LPBUFFER; --> YnDefine.h ������ "LPXDATA"
#endif


/////////////////////////////////////////////////////////////////////////////
//
// class ynShader : ���̴� ��� Ŭ����.
//
class ynShader
{
public:

	//��� ���� : ���̴� ���� ���꿡 ���� �����͵�. 
	struct ConstBuffer
	{
		MATRIXA mWorld;
		MATRIXA mView;
		MATRIXA mProj;
		MATRIXA mWVP;

		COLOR   Color0;		//�ܺ� ������, �⺻���� (1,1,1,1).

		ConstBuffer() { mWorld = mView = mProj = mWVP = XMMatrixIdentity();  Color0 = COLOR(1, 1, 1, 1); }		//�⺻�� ó�� 2017.11.22.
	};


protected:
	LPDEVICE		m_pDev;				//����̽� ��ġ �ڵ�.

 	LPVERTEXSHADER	m_pVS;				//�������̴�.
	LPPIXELSHADER   m_pPS;				//�ȼ����̴�.

	LPLAYOUT		m_pLayout;			//���� ���̾ƿ� Input-Layout
	LPXDATA			m_pVSCode;			//���� ���̴� ������ �ڵ� ��ü.(�ӽ�/Input Layout ���ۿ�)
	
	LPCONSTBUFFER	m_pCB;				//��� ����.(���̴� ������ ���޿�)
	
	

protected:
	//���̴� ������.
	virtual HRESULT Compile( WCHAR* FileName, char* EntryPoint, char* ShaderModel, 	ID3DBlob** ppCode );
	//���̴� ��ü ����.
	virtual HRESULT CreateVS(TCHAR* filename, char* EntryPoint, char* ShaderModel);
	virtual HRESULT CreatePS(TCHAR* filename, char* EntryPoint, char* ShaderModel);
	//virtual HRESULT CreateGS(char* EntryPoint, char* ShaderModel);
	//virtual HRESULT CreateCS(char* EntryPoint, char* ShaderModel);
	//���̾ƿ� ����.
	virtual HRESULT CreateLayout();
	virtual HRESULT CreateConstBuffer();

public:
	ynShader();
	virtual ~ynShader();

	virtual void Release();
	virtual int  Load(LPDEVICE pDev, TCHAR* filename);
	virtual void Update(LPDXDC pDXDC, ConstBuffer* pCB);
	virtual void Apply(LPDXDC pDXDC);
	virtual void Clear();


	LPVERTEXSHADER GetVS() { return m_pVS; }
	LPPIXELSHADER  GetPS() { return m_pPS; }
	LPLAYOUT	   GetLayout() { return m_pLayout; }
	LPXDATA		   GetVSCode() { return m_pVSCode;  }
	LPCONSTBUFFER  GetCB() { return m_pCB; }

};

//typedef ynShader ynShader;





//////////////////////////////////////////////////////////////////////////////// 
//
// ���̴� ���� �����͵�.
//
//////////////////////////////////////////////////////////////////////////////// 
namespace YENA {

//���� ���� ���̴� ��ü��.
extern YENA_APIX ynShader*  ypFXLine;			//���� ���̴� (+������)


// ���̴� ���� �Լ� Shader Functions
int  ynShaderLoad(LPDEVICE pDev);
void ynShaderRelease();


} // end of namespace YENA

//#define SV_FXLine  YENA::ypFXLine




/****************** end of file "ynShader.h" ***********************************/