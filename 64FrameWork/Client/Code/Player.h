#ifndef Player_h__
#define Player_h__

#include "Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CNaviMesh;
class CDynamicMesh;
class CTransform;
class CRenderer;
class CCalculator;
class CCollider;
class CShader;
class CColliderGroup;
END
class CThirdPersonCamera;
class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiIdx = 0, _uint uiStageIdx = 0);
	virtual ~CPlayer(void);

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;

public:
	void		Set_CamTransform(Engine::CTransform** pTransform) { m_pCameraTransformCom=*pTransform; }

private:
	HRESULT		Add_Component(void);
	void		Key_Input(const _float& fTimeDelta);
	HRESULT		SetUp_ConstantTable(LPD3DXEFFECT& pEffect);
	HRESULT		Load_Text(const TCHAR * pFilePath);
	wchar_t*	CharToWChar(const char* pstrSrc);
	float		Get_Angle(const D3DXVECTOR3& a, const D3DXVECTOR3& b);
	void		Check_Direction(_float fTimeDelta);
	void		StateMachine();
	void		IdleOption();
	void		CheckMoveMesh(_float fTimeDelta, Engine::CTransform * pTransform, Engine::INFO eDir, _bool bIsDirRvc, _float fSpeed, Engine::INFO eDir2 = Engine::INFO::INFO_END, _bool bIsDir2Rvc = false);
	void		CheckMoveMesh(_float fTimeDelta,  _vec3 vDir,_bool bIsDirRvc, _float fSpeed);
	void		AttackMoveSet(_float fTimeDelta);
	void		StorngAttackMoveSet(_float fTimeDelta);
	void		ChargeAttackMoveSet(_float fTimeDelta);
	void		MoveAni(_float fTimeDelta, _float fMinRatio, _float fMaxRatio, _float fSpeed, _vec3 vDir);
	_float		Get_AngleOnTheLook();
	void		RotateToLook(_float fTimeDelta);
	_bool		CheckEnableState();

private:
	_uint					m_uiStageSet;
	Engine::CTransform*		m_pCameraTransformCom = nullptr;
	Engine::CTransform*		m_pTransformCom = nullptr;
	Engine::CRenderer*		m_pRendererCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	Engine::CDynamicMesh*	m_pMeshCom = nullptr;
	Engine::CNaviMesh*		m_pNaviCom = nullptr;
	Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;
	Engine::CColliderGroup*	m_pColliderGroupCom=nullptr;

	CKeyMgr*				m_pKeyMgr = nullptr;
	CThirdPersonCamera*		m_pCam = nullptr;
	_int					m_iAnim = 0;
	_vec3					m_vDir;
	_float					m_fAnimSpeed = 1.0f;
	_float					m_fRotSpeed = 6.f;
	_float					m_fSpeed = 0.6f; //Walk 0.6f
	DWORD					m_dwDirectionFlag = 0;
	DWORD					m_dwDodge_DirectionFlag = 0;
	_bool					m_bIsLockOn = false;
	_bool					m_bIsShift = false;
	
	OBJECTSTATE				m_eCurState;
	OBJECTSTATE				m_ePreState;
	_bool					m_bIsAttack=false;
	_float					m_fChargeTime = 0.f;
	_uint					m_uiCombo = 0;

public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiIdx=0, _uint uiStageIdx = 0);

private:
	virtual void Free(void) override;
};

#endif // Player_h__
