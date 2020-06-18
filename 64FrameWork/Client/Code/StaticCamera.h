#ifndef StaticCamera_h__
#define StaticCamera_h__

#include "Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CTransform;

END
class CStaticCamera : public Engine::CCamera
{
private:
	explicit CStaticCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticCamera(void);

public:
	void	SetUp_Target(Engine::CTransform* pTargetInfo);

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
							const _vec3* pAt,
							const _vec3* pUp,
							const _float& fFovY,
							const _float& fAspect,
							const _float& fNear,
							const _float& fFar);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

private:
	void	Key_Input(const _float& fTimeDelta);
	void	Target_Renewal(const _float& fTimeDelta);
	void	Mouse_Move(const _float& fTimeDelta);
private:
	Engine::CTransform*		m_pTargetInfo = nullptr;
	_float					m_fDistance = 1.f;
	_float					m_fVerticalAngle = 0.f;
	_float					m_fHorizonAngle = 0.f;
	_vec3					m_vOldEye;
	_float					m_fUp = 1.f;
	_float					m_fZ = 1.f;


	const	_matrix*		m_pParentBoneMatrix = nullptr;
	const	_matrix*		m_pParentWorldMatrix = nullptr;
	_matrix					m_matWorld;

public:
	static CStaticCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										Engine::CTransform* pTargetInfo,
										const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										const _float& fFovY,
										const _float& fAspect,
										const _float& fNear,
										const _float& fFar);

private:
	virtual void Free(void) override;

};


#endif // StaticCamera_h__
