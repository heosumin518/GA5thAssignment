//
//
// Light.fx : ���� ���̴� �ҽ�.
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//

 
//���� ������ �������
struct LIGHT
{
    float3 Direction;   //���� ����.
    float4 Diffuse;     //�� ���� : Ȯ�걤 Diffuse Light.
    float4 Ambient;     //���� ���� : �ֺ��� Ambient Light.
    float4 Specular;    //���ݻ� ���� : Specular Light.
    float  Range;       //�� ���� �Ÿ�.
    bool   bDiffOn;     //���� ���뿩��.
    bool   bSpecOn;     //���ݻ� ���뿩��.
};

//���� ������ �������
struct MATERIAL
{
    float4 Diffuse;    //Ȯ�걤 �ݻ���(%) 
    float4 Ambient;    //�ֺ��� �ݻ���(%)
    float4 Specular;   //���ݻ� �ݻ���(%)
    float  Power;      //���ݻ� ������ : "��ĥ��" 
    bool   bTexOn;     //�ؽ�ó ��뿩��.
};


//���� ������ �������
cbuffer cbLIGHT : register(b1)
{
    LIGHT g_Lit;
};

//���� ������ �������
cbuffer cbMATERIAL : register(b2)
{
    MATERIAL g_Mtrl;
};



 
// ���� ó�� �Լ�.
float4 DiffLight(float4 nrm);
float4 DiffLight(float4 nrm, float4x4 mWV, float4x4 mView);
float4 SpecLight(float4 pos, float4 nrm);



//extern float4x4 mWV;
//extern float4x4 mV;


////////////////////////////////////////////////////////////////////////////// 
//
// Ȯ�걤 ���� ��� : ����Ʈ ������ �� ����. Lambert Lighting Model
//                 : �� ���� View Space ���� ó��.
//
/*float4 DiffLight(float4 nrm)
{
    float4 N = nrm;    N.w = 0;
    float4 L = float4(g_Lit.Direction, 0);

    //��������� ������ ��ȯ.
    N = mul(N, mWV);
    L = mul(L, mView);

    //���� ��� 
    float4 diff = max(dot(N, L), 0) * g_Lit.Diffuse * g_Mtrl.Diffuse;
    float4 amb = g_Lit.Ambient * g_Mtrl.Ambient;
    
    return diff + amb;
}
*/



////////////////////////////////////////////////////////////////////////////// 
//
// Ȯ�걤 ���� ��� : ����Ʈ ������ �� ����. Lambert Lighting Model
//                 : �� ���� View Space ���� ó��.
//
float4 DiffLight(float4 nrm, float4x4 mWV, float4x4 mView)
{
    float4 N = nrm;    N.w = 0;
    float4 L = float4(g_Lit.Direction, 0);

    //��������� ������ ��ȯ.
    N = mul(N, mWV);
    L = mul(L, mView);

    //���� ��� 
    float4 diff = max(dot(N, L), 0) * g_Lit.Diffuse * g_Mtrl.Diffuse;
    float4 amb = g_Lit.Ambient * g_Mtrl.Ambient;
    
    return diff + amb;
}




////////////////////////////////////////////////////////////////////////////// 
//
// ���ݻ籤 ���� ��� : ���� �� ����. Blinn-Phong Lighting Model
//                   : �� ���� View Space ���� ó��.
float4 SpecLight(float4 pos, float4 nrm, float4x4 mWV, float4x4 mView)
{
    float4 N = nrm;    N.w = 0;
    float4 L = float4(g_Lit.Direction, 0);

    //��������� ������ ��ȯ.
    N = mul(N, mWV);
    L = mul(L, mView);     

    //�ü����� ���.
    float4 E = normalize(-pos);

    //�������� ���.
    float4 H = normalize(L + E);

    //���� ��� 
    float4 spec = pow(max(dot(N, H), 0), g_Mtrl.Power) * g_Lit.Specular * g_Mtrl.Specular;
    spec.w = 1;
    
    return spec;
}







/**************** end of file "Light.fx" ***********************/
