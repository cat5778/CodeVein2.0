#ifndef Cell_h__
#define Cell_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CLine;

class ENGINE_DLL CCell : public CBase
{
public:
	enum POINT		{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR	{ NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum LINE		{ LINE_AB, LINE_BC, LINE_CA, LINE_END };
	enum COMPARE	{ COMPARE_MOVE, COMPARE_STOP };

private:
	explicit CCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCell(void);

public:
	const _ulong*		Get_Index(void) { return &m_dwIndex; }
	const _vec3*		Get_Point(POINT eType) const		{ return &m_vPoint[eType]; }
	CCell*				Get_Neighbor(NEIGHBOR eType) const	{ return m_pNeighbor[eType]; }
	void				Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor) { m_pNeighbor[eType] = pNeighbor; }
	void				Set_NaviData(Engine::NAVI_DATA naviData);
	void				Set_NaviData(Engine::CCell::POINT ePoint, _vec3 vPos);
	Engine::NAVI_DATA 	Get_NaviData();
	LPD3DXMESH			Get_SphereMesh(int iPointNum) { return m_pSphereMesh[iPointNum]; }
	_matrix				Get_MatWorldSphere(int iPointNum) { return m_matSphereWorld[iPointNum]; }
	void				Set_CellIdx(_uint uiIdx) { m_uiIdx = uiIdx; }
public:
	HRESULT		Ready_Cell(	const _ulong& dwIndex,
							const _vec3* pPointA,
							const _vec3* pPointB,
							const _vec3* pPointC);

	_bool		Compare_Point(const _vec3* pPointF, 
							  const _vec3* pPointS, 
							  CCell* pCell);

	void		Render_Cell(void);

	COMPARE		Compare(const _vec3* pEndPos, _ulong* pIndex);


private:
	_vec3					m_vPoint[POINT_END];
	CCell*					m_pNeighbor[NEIGHBOR_END];
	CLine*					m_pLine[LINE_END];

	LPD3DXMESH				m_pSphereMesh[POINT_END] = { nullptr,nullptr,nullptr };
	SPHERE_DATA				m_tSphereData[POINT_END];
	_matrix					m_matSphereWorld[POINT_END];
	_uint					m_uiIdx=0;
	LPD3DXLINE				m_pD3DXLine;
	_ulong					m_dwIndex;
	LPDIRECT3DDEVICE9		m_pGraphicDev;

public:
	static CCell*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	virtual void		Free(void);		
};

END
#endif // Cell_h__
