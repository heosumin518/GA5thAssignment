//
//
// Demo.fx : �⺻ ���̴� �ҽ�.
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//
// < ������� >
// 1. ǥ�� ����.
// 2. �ؽ�ó ȥ��.
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
    float2 uv   : TEXCOORD0;
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
				  float4 nrm : NORMAL,      //[�Է�] ��� normal 
                  float2 uv  : TEXCOORD0    //[�Է�] �ؽ�ó ��ǥ Texture Coordiates.
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

    
    //���� ���� ��ȯ (Projection Transform)
    pos = mul(pos, mProj);


    //���� ���.
    VSOutput o = (VSOutput) 0;
    o.pos = pos; 
    o.diff = diff;
	o.spec = spec;
    o.uv = uv;

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

//�ؽ�ó ��ü ����: �������� ���޵�.
//Texture2D texDiffuse;    
Texture2D texDiffuse : register(t0);     //�������� ���� ����. (�⺻���� t0)

//�ؽ�ó ���÷�.(��������)
SamplerState smpLinear;
/*
SamplerState smpLinear;
{
    Filter = MIN_MAG_MIP_LINEAR; //���߼��� ���͸� (Tril-Linear Filter)
    AddressU = Wrap;
    AddressV = Wrap;
};
*/


////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main : �ȼ� ���̴� ���� �Լ�.
//
////////////////////////////////////////////////////////////////////////////// 


float4 PS_Main(
				float4 pos  : SV_POSITION,  //[�Է�] (������) �ȼ��� ��ǥ. (Screen, 2D)
				float4 diff : COLOR0,       //[�Է�] (������) �ȼ��� Ȯ���. (Diffuse)
				float4 spec : COLOR1,        //[�Է�] (������) �ȼ��� ���ݻ��. ("Specular")
				float2 uv   : TEXCOORD0     //[�Է�] �ؽ�ó ��ǥ 
				) : SV_TARGET               //[���] ����.(�ʼ�), "����Ÿ��" ���� ����մϴ�.
{
	   
    //�ؽ��� ���ø�.
    float4 tex = 1;
    if (g_Mtrl.bTexOn)
        tex = texDiffuse.Sample(smpLinear, uv);

    //[�׽�Ʈ1] ��»� ���� 
    //float4 col = diff + spec;

    //[�׽�Ʈ2] �ؽ�ó ȥ�� ��»� ����
    float4 col = tex * diff + spec;

     
    return col;
}



/**************** end of file "Demo.fx" ***********************/
