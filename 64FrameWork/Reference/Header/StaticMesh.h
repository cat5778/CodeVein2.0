#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit	CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit	CStaticMesh(const CStaticMesh& rhs);
	virtual		~CStaticMesh(void);

public:
	_ulong			Get_NumVtx(void) const { return m_dwNumVtx; }
	_ulong			Get_Stride(void) const { return m_dwStride; }
	_bool			Find_Alpha(const char* pFileName);
	const _vec3*	Get_VtxPos(void) const { return m_pVtxPos; }
	LPD3DXMESH		Get_Mesh(void) { return m_pMesh; }

public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void		Render_Meshes(void);
	void		Render_Meshes(LPD3DXEFFECT& pEffect);

private:
	LPD3DXMESH				m_pMesh;
	LPD3DXBUFFER			m_pAdjacency;
	LPD3DXBUFFER			m_pSubsetBuffer;
	D3DXMATERIAL*			m_pMtrl;
	_ulong					m_dwSubsetCnt;
	LPDIRECT3DTEXTURE9*		m_ppTexture;

	LPD3DXMESH				m_pOriMesh;
	_ulong					m_dwNumVtx;
	_ulong					m_dwStride;
	_vec3*					m_pVtxPos;


public:
	static CStaticMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
									const _tchar* pFilePath,
									const _tchar* pFileName);
	
	virtual CResources*		Clone(void);
	virtual void			Free(void);

};

END
#endif // StaticMesh_h__
