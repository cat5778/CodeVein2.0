texture				g_AlbedoTexture;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};
texture g_LightTexture;

sampler LightSampler = sampler_state
{
	texture = g_LightTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

texture g_SpecularTexture;

sampler SpecularSampler = sampler_state
{
	texture = g_SpecularTexture;
	
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};


struct PS_IN
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

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vLight = tex2D(LightSampler, In.vTexUV);
	vector		vSpecular = tex2D(SpecularSampler, In.vTexUV);

	Out.vColor = vAlbedo * vLight + vSpecular;

	return Out;
}

technique Default_Device
{
	pass	Blend
	{
		zwriteEnable = false;

		//alphablendenable = true;
		//srcblend = srcalpha;
		//destblend = invsrcalpha;

		//vertexshader = NULL;
		pixelshader = compile ps_3_0 PS_MAIN();
	}

};