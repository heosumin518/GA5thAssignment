//
//
// Demo2.fx : �⺻ ���̴� �ҽ�.
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
// < ������� >
// 1. ǥ�� ����.
//


// �⺻ �������
cbuffer cbDEFAULT : register(b0)
{
    matrix mTM;     //���� ���. 
    matrix mView;   //�� ��ȯ ���. 
    matrix mWV;     //����-�� ��ȯ ���. 
    matrix mProj;   //���� ��ȯ ���. 
};



 
//VS ��� ����ü.
struct VSOutput
{
    float4 pos  : SV_POSITION;
    float4 diff : COLOR0;
    float4 spec : COLOR1;
};


 
//���� ���̴� �߰�.
#include "Light.fx"




////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main : ���� ���̴� ���� �Լ�.
//
////////////////////////////////////////////////////////////////////////////// 

VSOutput VS_Main(
				  float4 pos : POSITION,    //[�Է�] ������ǥ. Vertex Position (Model Space, 3D)
				  float4 nrm : NORMAL       //[�Է�] ��� normal 
				)
{
	//* �Ʒ��� �׽�Ʈ�� �����ϱ� ����  
    //* VS �� ��� ���۰� �����Ǿ� �־�� �մϴ�.    
    pos.w = 1;

	//���� ��ȯ.(World Transform) 
    pos = mul(pos, mTM);        //pos = pos * mTM
	
    //�þ�-�� ��ȯ (View Transform)
    pos = mul(pos, mView);
 
    
    //���� ��� 1 (Diffuse Lighting)
    float4 diff = 1;
    if(g_Lit.bDiffOn) diff = DiffLight(nrm, mWV, mView);
 
    //���� ��� 2 (Specular Lighting)
    float4 spec = 0;
    if (g_Lit.bSpecOn) spec = SpecLight(pos, nrm, mWV, mView);

    //������ ��� ������� ������ �⺻�� ó��.
    //if(!g_Lit.bDiffOn && !g_Lit.bSpecOn) diff = 1;

    
    //���� ���� ��ȯ (Projection Transform)
    pos = mul(pos, mProj);


    //���� ���.
    VSOutput o = (VSOutput) 0;
    o.pos = pos; 
    o.diff = diff;
	o.spec = spec;

    return o;
}






////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader ���� ������.
//



////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main : �ȼ� ���̴� ���� �Լ�.
//
////////////////////////////////////////////////////////////////////////////// 


float4 PS_Main(
				float4 pos  : SV_POSITION,  //[�Է�] (������) �ȼ��� ��ǥ. (Screen, 2D)
				float4 diff : COLOR0,       //[�Է�] (������) �ȼ��� Ȯ���. (Diffuse)
				float4 spec : COLOR1        //[�Է�] (������) �ȼ��� ���ݻ��. ("Specular")
				) : SV_TARGET               //[���] ����.(�ʼ�), "����Ÿ��" ���� ����մϴ�.
{
	//Ư���� ��ȯ ���� �Էµ� ������ �״�� ����մϴ�.
    //...

    //��»� ����.
    float4 col = diff + spec;
	 
    return col;
}



/**************** end of file "Demo2.fx" ***********************/
