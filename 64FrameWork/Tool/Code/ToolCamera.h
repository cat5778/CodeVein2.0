#ifndef Cameara_h__
#define Cameara_h__

#include "Defines.h"
#include "GameObject.h"
#include "Transform.h"
BEGIN(Engine)

class CTransform;
class CKeyMgr;
END


class CToolCamera : public Engine::CGameObject
{


private:
	explicit CToolCamera(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CToolCamera();

public:
	HRESULT Ready_GameObject(void) override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void Render_GameObject(void) override;

public:

	static CToolCamera* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	HRESULT		Add_Component(void);
	void Key_Input(const _float& fTimeDelta);

private:

	_matrix						m_matView, m_matProj;

	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CKeyMgr*        m_pKeyMgr = nullptr;
	_vec3					m_vDir;
	_float					m_fSpeed = 5.f;
	
private:
	void Free(void) override;

};





#endif // Cameara_h__
