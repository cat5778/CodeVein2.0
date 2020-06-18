#pragma once


#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
END


class CDynamicObject : public Engine::CGameObject
{
public:
	explicit				CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx);
	explicit				CDynamicObject(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo);
	virtual					~CDynamicObject(void);

public:
	HRESULT					Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	
public:
	_uint					Get_AnimationIdx() { return m_uiAni; }
	void					Set_AnimationIdx(_uint uiAnim) { m_uiAni= uiAnim; }
private:
	HRESULT					Add_Component(void);
private:
	void					Set_TransformData();
	
private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CColliderGroup* m_pColliderGroupCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	TRANSFORM_INFO			m_tTransformInfo;
	_bool					m_bIsRespawn = false;
	_uint					m_uiAni = 0;
public:
	static CDynamicObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx = 0);
	static CDynamicObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx ,  TRANSFORM_INFO tInfo);

private:
	virtual void			Free(void) override;
};

