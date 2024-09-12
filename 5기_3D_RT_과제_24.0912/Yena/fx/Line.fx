//
//
// Line.fx : �⺻ ���̴� �ҽ� "���� �׸���""
//
// 2003.11.4. Kihong Kim / mad_dog@hanmail.net
// 2010.12.01. Update. (DX11.Jun.2010)
// 2016.12.27. Update. (DX11. Windows SDK 8.1)
//


// ��� ���� Constant Buffer Variables
cbuffer ConstBuffer //: register(b0) //������� 0���� ��� (�ִ� 14��. 0~13) 
{
    matrix mWorld;
    matrix mView;
    matrix mProj;
    matrix mWVP;

    float4 Color0 = float4(1, 1, 1, 1); //�ܺ� ������ (�⺻���� ���)
}



//VS ��� ����ü.
struct VSOutput
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
};



////////////////////////////////////////////////////////////////////////////// 
//
// Vertex Shader Main
//
VSOutput VS_Main( 
				  float4 pos : POSITION,	//[�Է�] ������ǥ. Vertex Position (Model Space, 3D)
				  float4 col : COLOR0		//[�Է�] ������ Vertex Color	: "Diffuse"	
				)
{

	VSOutput o = (VSOutput)0;	 
    pos.w = 1.0f;
    //��ȯ.
    pos = mul(pos, mWVP);
    
    //���� ȥ��.
    col = col * Color0;
    	
    //���.
	o.pos = pos;						//��ȯ�� ���� ���..
	o.col = col;						//������ ��ȯ ���� ���.

    return o;
}





////////////////////////////////////////////////////////////////////////////// 
//
// Pixel Shader Main
//
float4 PS_Main( 
				float4 pos : SV_POSITION,	//[�Է�] ������ǥ. (Screen, 2D)
				float4 col : COLOR0		//[�Է�] ������. (Vertex Color : "Diffuse")
				) : SV_TARGET
{
	//float4 col2 = {1, 0, 1, 1};
	 
	return col;
}



/**************** end of file "Line.fx" ***********************/
