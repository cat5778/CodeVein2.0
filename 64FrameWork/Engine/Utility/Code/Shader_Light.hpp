texture			g_NormalTexture;

sampler NormalSampler = sampler_state		// 필터링을 하지 말아야 한다. 노말 벡터를 필터링하게 되면 명암 값이 어긋날 가능성이 있다.
{
	texture = g_NormalTexture;
};

texture			g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};


vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;


vector		g_vCamPos;
float		g_fPower;

matrix		g_matViewInv;
matrix		g_matProjInv;

vector		g_vLightPos;
float		g_fRange;


struct PS_IN					// screen 정점은 xyzrhwf 포지션과 텍스쳐 uv로 구성되어 있었다. 픽셀세이더에서 위치 값을 사용할 수 없기 때문에 텍스쳐 uv값만 가져오고 있다.
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vShade : COLOR0;
	vector			vSpecular : COLOR1;
};

PS_OUT		PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	vector vDir = g_vLightDir * -1.f;

	Out.vShade = max(dot(normalize(vDir), vNormal), 0.f)
		* (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient);

	Out.vShade.a = 1.f;

	vector		vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);

	vector		vDepth = tex2D(DepthSampler, In.vTexUV);
	float		vViewZ = vDepth.y * 1000.f;	// 뷰 스페이스 원래값 1000

	vector		vPosition;

	vPosition.x = (In.vTexUV * 2.f - 1.f) * vViewZ;
	vPosition.y = (In.vTexUV * -2.f + 1.f) * vViewZ;
	vPosition.z = vDepth.x * vViewZ;
	vPosition.w = vViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector		vLook = normalize(g_vCamPos - vPosition);


	Out.vSpecular = pow(saturate(dot(vReflect, vLook)), g_fPower);


	return Out;
}

PS_OUT		PS_MAIN_POINT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);

	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	vector		vDepth = tex2D(DepthSampler, In.vTexUV);
	float		vViewZ = vDepth.y * 1000.f;	// 뷰 스페이스

	vector		vPosition;

	vPosition.x = (In.vTexUV * 2.f - 1.f) * vViewZ;
	vPosition.y = (In.vTexUV * -2.f + 1.f) * vViewZ;
	vPosition.z = vDepth.x * vViewZ;
	vPosition.w = vViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector vDir = vPosition - g_vLightPos;

	float fDistance = length(vDir);

	float	fAtt = saturate((g_fRange - fDistance) / g_fRange);

	Out.vShade = (max(dot(normalize(vDir), vNormal), 0.f)
		* (g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;

	Out.vShade.a = 1.f;

	vector		vReflect = reflect(normalize(vector(g_vLightDir.xyz, 0.f)), vNormal);
	vector		vLook = normalize(g_vCamPos - vPosition);

	Out.vSpecular = (pow(saturate(dot(vReflect, vLook)), g_fPower)) * fAtt;

	return Out;
}

technique Default_Device
{
	pass	Directional
{
	alphablendenable = true;
srcblend = one;
destblend = one;

zwriteEnable = false;
vertexshader = NULL;
pixelshader = compile ps_3_0 PS_MAIN_DIRECTIONAL();
}

pass Point
{
	alphablendenable = true;
srcblend = one;
destblend = one;

zwriteEnable = false;
vertexshader = NULL;
pixelshader = compile ps_3_0 PS_MAIN_POINT();
}

};