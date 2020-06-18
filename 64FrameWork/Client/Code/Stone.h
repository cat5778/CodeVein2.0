#ifndef Stone_h__
#define Stone_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;

END

class CStone : public Engine::CGameObject
{
private:
	explicit				CStone(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual					~CStone(void);

public:

public:
	HRESULT							Ready_GameObject(const _uint& iFlag);
	virtual _int					Update_GameObject(const _float& fTimeDelta) override;
	virtual void					Render_GameObject(void) override;

private:
	HRESULT							Add_Component(void);
	//void							SetUp_OnTerrain(void);
	//_vec3							PickUp_OnTerrain(void);
	void							Key_Input(const _float& fTimeDelta);
	_bool							Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);

private:
	Engine::CTransform*				m_pTransformCom = nullptr;
	Engine::CRenderer*				m_pRendererCom = nullptr;
	Engine::CCalculator*			m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*			m_pMeshCom = nullptr;
	Engine::CCollider*				m_pColliderCom = nullptr;

	_vec3							m_vDir;
	_float							m_fSpeed = 20.f;
	_uint							m_iFlag = 0;
	_bool							m_bColl = false;





public:
	static CStone*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);

private:
	virtual void Free(void) override;
};

#endif // Stone_h__
