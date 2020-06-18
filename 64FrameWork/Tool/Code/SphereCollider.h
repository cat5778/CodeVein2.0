#pragma once
#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CStaticMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
END


class CSphereCollider : public Engine::CGameObject
{
public:

	explicit				CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit				CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjName, wstring wstBone);
	explicit				CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjName, wstring wstBone,_bool bisStaticMesh);
	virtual					~CSphereCollider(void);

public:
	HRESULT					Ready_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;


public:
	//void					Set_Position(_vec3 vPos) { m_pTransformCom->m_vInfo[Engine::INFO_POS] = vPos; }
	//const Engine::_vec3*	Get_Position() { return m_pTransformCom->Get_Info(Engine::INFO_POS); }
	//const Engine::_vec3*	Get_AddPosition() { return &m_tCollData.vPos; }


	const Engine::_vec3*	Get_Position() { return &m_pTransformCom->m_vInfo[Engine::INFO_POS];}
	void					Set_Position(_vec3 vPos) { m_pTransformCom->m_vInfo[Engine::INFO_POS] = vPos; }
	// ¼öÁ¤
	void					Set_Radius(_float fRadius) { m_tCollData.fRadius = fRadius; }
	_float 					Get_Radius() { return m_tCollData.fRadius; }

	void					Set_Option(COLLIDEROPTION eOption) { m_tCollData.eCollOpt= eOption; }
	COLLIDEROPTION 			Get_Option() { return m_tCollData.eCollOpt; }


private:
	HRESULT					Add_Component(void);
	void					Ready_SphereMesh();
	HRESULT					Set_DMParentMatrix(_float fTimeDelta);
	HRESULT					Set_SMParentMatrix(_float fTimeDelta);
private:
	_bool					m_bIsStatic = false;
	LPD3DXMESH				m_pSphereMesh = nullptr;
	wstring					m_wstrBone = L"";

	Engine::CTransform*		m_pTransformCom = nullptr;
	//_vec3					m_vPos = { INIT_VEC3 }; //
	//_float				m_fRadius = 1.0f;		//
	_bool					m_bColl = false;
	const	_matrix*		m_pParentBoneMatrix = nullptr;
	const	_matrix*		m_pParentWorldMatrix = nullptr;

	Collider_data			m_tCollData;
#ifdef _DEBUG
	_matrix					m_matSphereWorld;		//
	Engine::CRenderer*		m_pRendererCom = nullptr;
#endif // _DEBUG

public:
	static CSphereCollider*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CSphereCollider*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjName, wstring wstBone);
	static CSphereCollider*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrObjName, wstring wstBone, _bool bisStaticMesh);

private:
	virtual void			Free(void) override;
};

