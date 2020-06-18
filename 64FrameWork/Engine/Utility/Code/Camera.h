#ifndef Camera_h__
#define Camera_h__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CCamera : public CGameObject
{
protected:
	explicit CCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCamera(void);

public:
	const _matrix*		Get_ViewSpace(void) { return &m_matView; }
	const _matrix*		Get_Projection(void) { return &m_matProj; }
	_matrix				Get_OrthoMat() { return m_matOrtho; }

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

protected:
	_matrix			m_matOrtho;
	_matrix			m_matPerspective;
	_vec3			m_vEye, m_vAt, m_vUp;
	_float			m_fFovY, m_fAspect, m_fNear, m_fFar;
	_matrix			m_matView, m_matProj;
	_float			m_fSpeed = 5.f;

protected:
	virtual void Free(void) override;
};

END
#endif // Camera_h__
