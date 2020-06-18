matrix		g_matWorld;			// 상수 테이블
matrix		g_matView;
matrix		g_matProj;

texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;

minfilter = linear;
magfilter = linear;
mipfilter = linear;

/*addressU = mirror;
addressV = mirror;*/
};

struct VS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

};

// 버텍스 쉐이더 함수
VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN // 픽셀 쉐이더 구조체에서 POSITION이란 Semantic은 사용할 수 없다.
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 상태의 텍스쳐의 UV좌표에 해당하는 값을 추출하는 함수(반환 타입은 VECTOR)

												//Out.vColor.rg = 0.5f;

	return Out;
}

technique Default_Device
{
	// 기능의 캡슐화
	pass		Temp
{
	ALPHABLENDENABLE = true;
SrcBlENd = srcalpha;
DESTblend = invsrcalpha;

vertexshader = compile vs_3_0 VS_MAIN();		// 버텍스 쉐이더의 진입점 함수 정의
pixelshader = compile ps_3_0 PS_MAIN();		// 버텍스 쉐이더의 진입점 함수 정의
}

pass
{
	vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}

};