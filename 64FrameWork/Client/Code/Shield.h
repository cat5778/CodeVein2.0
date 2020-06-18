#ifndef Shield_h__
#define Shield_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CShader;

END

class CShield : public Engine::CGameObject
{
private:
	explicit				CShield(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual					~CShield(void);

public:

public:
	HRESULT					Ready_GameObject(const _uint& iFlag);
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	HRESULT					Add_Component(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);

public:

	_bool					Get_Equip() { return m_bIsEquip; }
	void					Set_Equip(_bool bIsEquip);
	void					Set_Throw();
	void					Set_CellIdx(_uint uiCellIdx);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CNaviMesh*		m_pNaviCom = nullptr;


	_vec3					m_vThrow = { INIT_VEC3 };
	_float					m_fAccThrow = 4.f;
	_uint					m_iFlag = 0;
	_bool					m_bColl = false;
	_bool					m_bIsEquip = false;
	_vec3					m_vOldPos;
	_vec3					m_vOldRotation;
	_matrix					m_MatOldWorld;
	const	_matrix*		m_pParentBoneMatrix = nullptr;
	const	_matrix*		m_pParentWorldMatrix = nullptr;

public:
	static CShield*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);

private:
	virtual void Free(void) override;
};

#endif // Sword_h__
