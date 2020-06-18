#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3			vPos;
		_ulong			dwColor;

	}VTXCOL;



	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef	struct tagVertexTexture
	{
		_vec3			vPos;
		_vec3			vNormal;
		_vec2		 	vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef	struct tagVertexCubeTexture
	{
		_vec3			vPos;
		_vec3		 	vTex;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagScreenVertex
	{
		_vec4				vPos;
		_vec2				vTexUV;

	}VTXSCREEN;

	const _ulong		FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;
	typedef struct tagResolution
	{
		_float fWidth, fHeigth;
	}RESOLUTION;
	typedef struct tagSphere
	{
		_vec3 vPosition;
		_float fRadius;
	}SPHERE_DATA;
	typedef struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_matrix				CombinedTransformationMatrix;

	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*		ppTexture;

		LPD3DXMESH				pOriMesh;		// 애니메이션 스키닝을 위한 원본 메쉬 정보를 보관하기 위함(최초의 메쉬의 상태 값을 갖고 있는 메쉬 정보 객체(값은 불변))

		_ulong					dwNumBones;		// 뼈의 개수

		
		
		_matrix*				pFrameOffsetMatrix;			// 최초의 뼈대들이 갖고 있는 행렬(애니메이션을 포함한 행렬)


		_matrix**				ppFrameCombinedMatrix;		// 뼈들이 갖고 있는 CombinedTransformationMatrix의 주소값을 보관하기 위한 이중 포인터(매 프레임마다 갱신을 해야하기 때문에 매번 갱신 시점마다 행렬을 얻어오는 행위는 비 합리적이다. 그래서 한꺼번에 보관하려 하는 것이다.)
		_matrix*				pRenderingMatrix;		   // 결국 최종 뼈의 상태 값을 보관하는 행렬(pFrameOffsetMatrix * ppFrameCombinedMatrix[i])


	}D3DXMESHCONTAINER_DERIVED;

	typedef struct Navi_Data
	{
		_vec3 vPosition1;
		_vec3 vPosition2;
		_vec3 vPosition3;
		_uint uiIdx;
	}NAVI_DATA;

	typedef struct Coll_data
	{
		_float	fRadius;
		_vec3	vPosition;
	}COLL_DATA;
}


#endif // Engine_Struct_h__
