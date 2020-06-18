#include "Shader_Base.hpp"

texture g_BaseTexture1;

sampler BaseSampler1 = sampler_state
{
	texture = g_BaseTexture1;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_FilterTexture;

sampler FilterSampler = sampler_state
{
	texture = g_FilterTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};

texture g_AuraTexture;

sampler AuraSampler = sampler_state
{
	texture = g_AuraTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;
};


float		g_fDetail;

vector		g_vLightDir = (vector)0.f;

vector		g_vLightDiffuse;
vector		g_vLightSpecular;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse;
vector		g_vMtrlSpecular;
vector		g_vMtrlAmbient;

vector		g_vCamPos;
float		g_fPower;

vector		g_vAuraPos;
float		g_fRange;


struct VS_IN
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;

};

struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vWorldPos : TEXCOORD1;
	vector		vProjPos : TEXCOORD2;

};

// 버텍스 쉐이더 함수
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	vector		vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = vWorldPos;

	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN // 픽셀 쉐이더 구조체에서 POSITION이란 Semantic은 사용할 수 없다.
{

	vector			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
	vector			vWorldPos : TEXCOORD1;
	vector			vProjPos : TEXCOORD2;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
	vector			vNormal : COLOR1;
	vector			vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = tex2D(BaseSampler, In.vTexUV * g_fDetail);
	vector vColor1 = tex2D(BaseSampler1, In.vTexUV* g_fDetail);

	vector	vFilter = tex2D(FilterSampler, In.vTexUV);

	Out.vColor = vColor * vFilter + vColor1 * (1.f - vFilter);
	Out.vColor.a = 1.f;

	vector		vBrushColor = (vector)0.f;

	if (g_vAuraPos.x - g_fRange < In.vWorldPos.x &&
		g_vAuraPos.x + g_fRange > In.vWorldPos.x &&
		g_vAuraPos.z - g_fRange < In.vWorldPos.z &&
		g_vAuraPos.z + g_fRange > In.vWorldPos.z)
	{
		float2		vTexUV;

		vTexUV.x = (In.vWorldPos.x - (g_vAuraPos.x - g_fRange)) / (g_fRange * 2.f);
		vTexUV.y = ((g_vAuraPos.z + g_fRange) - In.vWorldPos.z) / (g_fRange * 2.f);

		vBrushColor = tex2D(AuraSampler, vTexUV);
	}

	Out.vColor += vBrushColor;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, // z나누기를 수행하여 0~1사이의 투영 z값을 만들고, 이를 텍스쳐 uv좌표로 판단
		In.vProjPos.w * 0.001f,  // 뷰스페이스 상태의 z값을 텍스쳐의 uv로 변환
		0.f,
		0.f);


	return Out;
}

technique Default_Device
{
	pass
{
	vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}
};