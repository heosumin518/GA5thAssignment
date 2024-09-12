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
// ynStateObject.h : ���°�ü ���
//
// DirectX ���� ���� : DX11
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
//----------------------------------------------------------------------------
// ���� ��ü State Objects (DX10/11)
// DX10 ���� ���� TnL �� RenderState �� ���ŵǾ����ϴ�.
// �̸� ����ϴ� ���� ���°�ü State Objects �������̽���, ������ ���º� �׷����� ����
// �������� ����̽��� �����մϴ�.  �̸� ���� ��ġ�� ���� ���� ��ȭ�� �ѹ��� �����Ͽ� 
// ���ʿ��� �������(Overhead) �� ���̰� ���� ȿ������ �������� ������ �մϴ�.
// ���°�ü�� ���� �ʱ�� ������ ����ϱ⸦ �����ϸ� ����� ����(Release) �ؾ� �մϴ�.
// ���°�ü�� �����Ұ���(Immutable, �б�����) ��ü �Դϴ�.
// DX9 ������ State-Block �� �̿� ������ ����� ����մϴ�. 
//
// ���� ��ü �������̽� �� '�����Ͷ����� �������� Rasterize Stage' ���� ������ 
// ID3D11RasterizerState �������̽��� ���� ó���մϴ�.  
// ������ ����/��� ��� ������ �����սô�.
//----------------------------------------------------------------------------

#pragma once



/////////////////////////////////////////////////////////////////////////////
//
// class ynStateObject : ���°�ü ��� Ŭ����.
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
// ���°�ü ���� �����͵�.
//
//////////////////////////////////////////////////////////////////////////////// 

//���� ���� ���°�ü��.
namespace YENA
{

//
// ���°�ü ���� �Լ� StateObject Functions
//
int  ynStateObjectCreate(LPDEVICE pDev);
void ynStateObjectRelease();
int  ynStateObjectUpdate();

int  ynStateObjectBackup();			//�������� ����.
int  ynStateObjectRestore();		//�������� ����.

//
// ������ ���� ��ü ���� / ��� �Լ���.
//
//int  ynRenderStateObjectCreate(LPDEVICE pDev);
//void ynRenderStateObjectRelease();
//void ynRenderStateUpdate();


//
// ����/���ٽ� �׽�Ʈ ���µ�.
//
enum {
	YNDS_DEPTH_ON,		//���̹��� ON! (�⺻��), ���ٽǹ��� OFF.
	YNDS_DEPTH_OFF,		//���̹��� OFF!, ���ٽ� OFF.
	YNDS_DEPTH_ON_WRITE_OFF,	//���̹��� ���� ����, ���ٽ� OFF. 

	//���ٽ� ���� �����.
	YNDS_DEPTH_ON_STENCIL_ON,				//���̹��� On (Write On), ���ٽǹ��� ON (������ ����) : "����/���ٽ� ���".
	YNDS_DEPTH_ON_STENCIL_EQUAL_KEEP,		//���̹��� On (Write On), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���� ��ġ���� �׸���".
	YNDS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP,	//���̹��� On (Write On), ���ٽǹ��� ON (�ٸ���, ������ ����) : "���� ��ġ �̿ܿ� �׸���".
	
	//YNDS_DEPTH_OFF_STENCIL_ON,			//���̹��� Off, ���ٽǹ��� ON (������ ����) : "���ٽǸ� ���"
	//YNDS_DEPTH_ON_STENCIL_EQUAL_INCR,	//���̹��� On (Write On), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���߱׸��� ����".
	YNDS_DEPTH_WRITE_OFF_STENCIL_ON,		//���̹��� On (Write Off), ���ٽǹ��� ON (�׻���, ������ ����) :" ���ٽǸ� ���".
	YNDS_DEPTH_OFF_STENCIL_EQUAL_INCR,	//���̹��� Off (Write Off), ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���߱׸��� ����.
	

	//���� ��� �ִ밪.
	YNDS_MAX_,	

	//��� ������. MAX �� ���ʿ� �����ؾ� ��. ����.
	YNDS_DEPTH_ON_STENCIL_OFF = YNDS_DEPTH_ON,			//���̹��� ON, ���ٽǹ��� OFF
	YNDS_DEPTH_ONLY	= YNDS_DEPTH_ON_STENCIL_OFF,		//���̹��� ON, ���ٽǹ��� OFF
	//YNDS_STENCIL_ONLY = YNDS_DEPTH_OFF_STENCIL_ON,	//���̹��� OFF, ���ٽǹ��� ON
};

//����/���ٽ� ���� ��ü��
extern YENA_APIX LPDEPTHSTENCILSTATE ypDSState[YNDS_MAX_];

int  ynDepthStencilStateCreate(LPDEVICE pDev);
void ynDepthStencilStateRelease();



// 
// �����Ͷ����� ���� ��ü Rasterizer State Objects
// 
enum {
	YNRS_SOLID,				//�ﰢ�� ä��� : Fill Mode - Soild.
	YNRS_WIREFRM,			//�ﰢ�� ä��� : Fill Mode - Wireframe.
	YNRS_CULLBACK,			//�޸� �ø� (ON) : Back-Face Culling - "CCW" 
	YNRS_CULLFRONT,			//���� �ø� (ON) : Front-Face Culling - "CW" 	
	YNRS_WIRECULLBACK,		//���̾� ������ + �޸� �ø� (ON) 
	YNRS_WIRECULLFRONT,		//���̾� ������ + �ո� �ø� (ON) 	
	
	//���� ��� �ִ밪.
	YNRS_MAX_,

	//��� ������. MAX �� ���ʿ� �����ؾ� ��. ����.
	YNRS_CULL_CCW = YNRS_CULLBACK,
	YNRS_CULL_CW = YNRS_CULLFRONT,
	YNRS_WIRE_CULL_CCW = YNRS_WIRECULLBACK,
	YNRS_WIRE_CULL_CW = YNRS_WIRECULLFRONT,	

};
//�����Ͷ����� ���� ��ü �迭
extern YENA_APIX LPRASTERSTATE	ypRState[YNRS_MAX_];

void ynRasterStateCreate(LPDEVICE pDev);
void ynRasterStateRelease();
//void ynRasterModeUpdate();


//
// ������ ��� : �ټ��� ������ ���� ���� �� ����� ���� ����.
//
enum {
	YNRM_SOLID		= 0x0000,		// �ﰢ��ä��� : ON, Solid
	YNRM_WIREFRAME	= 0x0001,		// �ﰢ��ä��� : OFF, Wire-frame
	YNRM_CULLBACK	= 0x0002,		// �޸� �ø� : ON, "CCW"
	
	//������ �⺻��� : Solid + Cull-On.
	YNRM_DEFAULT		= YNRM_SOLID | YNRM_CULLBACK,	
	
};
extern YENA_APIX DWORD ynRMode;		//'����' ������ ���.



//
// ����/���� ȥ�� ���� ��ü Blending State Objects 
//
enum {
	YNBS_DEFAULT,				//�⺻ȥ�� ��ü.
	YNBS_ALPHA_BLEND,			//���� ����. (Src.a + (1-Src.a))
	YNBS_COLOR_BLEND,			//���� ����. ( Src.Color + (1-Src.Color))
	YNBS_COLOR_BLEND_ONE,		//1:1 ����. ( Src.Color + Dest.Color) 

	YNBS_MAX_,

	YNBS_AB_OFF = YNBS_DEFAULT,
	YNBS_AB_ON = YNBS_ALPHA_BLEND,
	//YNBS_AB_ALPHA = YNBS_ALPHA_BLEND,
	//YNBS_AB_COLOR = YNBS_COLOR_BLEND,
};

//����/���� ȥ�� ���� ��ü 
extern YENA_APIX LPBLENDSTATE ypBState[YNBS_MAX_];

void ynBlendStateCreate(LPDEVICE pDev);
void ynBlendStateRelease();
//void ynBlendModeUpdate();



//
// �ؽ�ó ���÷� ���� ��ü : Texture Sampler State Objects.
//
enum {
	YNSS_CLAMP,			//�ؽ�ó ��巹�� ��� : Ŭ���� (DX �⺻��) 
	YNSS_WRAP,			//�ؽ�ó ��巹�� ��� : ���� 
	//YNSS_BOARDER,
	//YNSS_MIRROR,
	//YNSS_MIRRROONCE,
	
	YNSS_MAX,
	YNSS_DEFAULT = YNSS_WRAP,	//�⺻ ���÷�
	
};
//�ؽ�ó ���÷� ���� ��ü.
extern YENA_APIX LPSAMPLERSTATE ypSState[YNSS_MAX];
void ynSamplerCreate(LPDEVICE pDev);
void ynSamplerRelease();




} // end of namespace YENA
//using namespace YENA;




/****************** end of file "ynStateObject.h" ***********************************/