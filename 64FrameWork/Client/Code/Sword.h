#ifndef Sword_h__
#define Sword_h__

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

class CSword : public Engine::CGameObject
{
private:
	explicit				CSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual					~CSword(void);

public:

public:
	HRESULT					Ready_GameObject(const _uint& iFlag);
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& pEffect);

private:
	HRESULT					Add_Component(void);
	_bool					Collision_ToObject(const _tchar* pLayerTag, const _tchar* pObjTag);

private:
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;


	_uint					m_iFlag = 0;
	_bool					m_bColl = false;

	const	_matrix*		m_pParentBoneMatrix = nullptr;
	const	_matrix*		m_pParentWorldMatrix = nullptr;

public:
	static CSword*		Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iFlag);

private:
	virtual void Free(void) override;
};

#endif // Sword_h__
