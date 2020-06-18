#ifndef NaviMesh_h__
#define NaviMesh_h__

#include "Mesh.h"
#include "Cell.h"

BEGIN(Engine)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh(void);

public:
	void				Set_Index(const _ulong& dwIndex) { m_dwIndex = dwIndex; }
	HRESULT				Add_Cell(NAVI_DATA* naviData);
	vector<CCell*>&		Get_CellVec();
	_ulong				Get_CurIndex() { return m_dwIndex; }
public:
	HRESULT				Ready_NaviMesh(void);
	void				Render_NaviMesh(void);
	_vec3				Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);
	bool				Move_OnNaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir, _vec3* _out);
	bool				Jump_OnNaviMesh(const _vec3 * pTargetPos, const _vec3 * pTargetDir, _vec3* _out);

	HRESULT				Link_Cell(void);
//private:

private:
#ifdef _DEBUG
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[COL_END];

#endif
	vector<CCell*>			m_vecCell;
	_ulong					m_dwIndex;

public:
	static	CNaviMesh*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual CResources*		Clone(void);
	virtual void			Free(void);
};

END
#endif // NaviMesh_h__
