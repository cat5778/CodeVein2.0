#ifndef ThirdPerson_h__
#define ThirdPerson_h__

#include "Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CTransform;
END


class CThirdPersonCamera : public Engine::CCamera
{
private:
	explicit				CThirdPersonCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual					~CThirdPersonCamera(void);

public:
	HRESULT					Ready_GameObject(const _vec3* pEye,
											const _vec3* pAt,
											const _vec3* pUp,
											const _float& fFovY,
											const _float& fAspect,
											const _float& fNear,
											const _float& fFar);

	virtual _int					Update_GameObject(const _float& fTimeDelta) override;
	HRESULT							Ready_Component();


public:
	void							SetUp_Target(Engine::CTransform* pTargetInfo);
	void							LockOn(_bool bIsLockOn);
	_vec3							Get_LockOnLook() { return m_vAt - m_vHeadPos; }
	_bool							Is_Lock() { return m_bIsLockOn; }
	const Engine::CTransform*		Get_MonTransform() { return m_pMonTransform; }

private:
	void							Key_Input(const _float& fTimeDelta);
	void							Target_Renewal(const _float& fTimeDelta);
	void							Mouse_Move(const _float& fTimeDelta);
	void							Lerp_Cam(_float fTimeDelta);
	void							Mouse_Fix(void);
	float							Get_Angle(const D3DXVECTOR3& a, const D3DXVECTOR3& b);
private:
	_bool					m_bIsLockOn = false;
	Engine::CTransform*		m_pTargetInfo = nullptr;
	Engine::CTransform*		m_pMonTransform = nullptr;

	Engine::CTransform*		m_pTransformCom = nullptr;
	_bool					m_bIsFix = false;	
	_float					m_fDistance = 1.5f;
	_float					m_fVerticalAngle = 240.f;
	_float					m_fHorizonAngle = 0.f;
	const	_matrix*		m_pParentBoneMatrix = nullptr;
	const	_matrix*		m_pParentWorldMatrix = nullptr;
	_matrix					m_matWorld;
	_vec3					m_vHeadPos;
	_vec3					m_vInitPos;

public:
	static CThirdPersonCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
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


#endif // ThirdPerson_h__
