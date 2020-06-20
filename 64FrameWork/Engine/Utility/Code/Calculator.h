#ifndef Calculator_h__
#define Calculator_h__

#include "Component.h"


BEGIN(Engine)

class CTerrainTex;
class CTransform;
class CSphereColliderCom;
class ENGINE_DLL CCalculator : public CComponent
{
private:
	typedef	struct tagOBB
	{
		_vec3		vPoint[8];		// 큐브를 이루는 정점
		_vec3		vCenter;		// 큐브의 중점
		_vec3		vProjAxis[3];	// 중점에서 각 면을 향해 뻗어나가는 방향 벡터 
		_vec3		vAxis[3];		// 큐브의 면과 평행한 축, 방향 벡터

	}OBB;

private:
	explicit CCalculator(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCalculator(void);

public:
	HRESULT		Ready_Calculator(void);

	_float		Compute_HeightOnTerrain(const _vec3* pPos, 
										const _vec3* pTerrainVtxPos,
										const _ulong& dwCntX,
										const _ulong& dwCntZ);

	_float		Compute_HeightOnTerrain1(_vec3* pPos,
										const _vec3* pTerrainVtxPos,
										const _ulong& dwCntX,
										const _ulong& dwCntZ);

	_vec3		Picking_OnTerrain(HWND hWnd,
									const CTerrainTex* pTerrainBufferCom,
									const CTransform*	pTerrainTransformCom);


	_bool		Collsion_Sphere(const vector<Engine::CSphereColliderCom*>* pSourceVec,
								_bool bIsSourCollEnable,
								const vector<Engine::CSphereColliderCom*>* pDestVec,
								_bool bIsDestCollEnable);
	_bool		Bounding_Sphere(const vector<Engine::CSphereColliderCom*>* pSourceVec, const vector<Engine::CSphereColliderCom*>* pDestVec, _float *pOutfPower);


	_bool		Collision_AABB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
								const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);

	_bool		Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _matrix* pDestWorld,
								const _vec3* pSourMin, const _vec3* pSourMax, const _matrix* pSourWorld);


	void		Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);
	void		Set_Axis(OBB* pObb);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;

public:
	static CCalculator*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void				Free(void);

};

END
#endif // Calculator_h__
