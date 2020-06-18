#ifndef TestPlayer_h__
#define TestPlayer_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

class CTestPlayer : public Engine::CGameObject
{
private:
	explicit CTestPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestPlayer(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);

private:
	Engine::CRcTex*			m_pBufferCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;

	_vec3					m_vDir;
	_float					m_fSpeed = 5.f;
public:
	static CTestPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

#endif // TestPlayer_h__
