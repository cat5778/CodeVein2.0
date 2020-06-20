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
	virtual HRESULT		Ready_GameObject(void) override;
	virtual HRESULT		LateReady_GameObject(void) override;
	virtual _int		Update_GameObject(const _float& fTimeDelta) override;


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
	void		Collision_Check(_float fTimeDelta);
	void		Hurt(_float fTimeDelta, _vec3 vPos , _vec3 vTargetPos,_float fDamage);
	void		KnockBack(_float fTimeDelta);
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
	_bool					m_bIsLockOn = false;
	_bool					m_bIsShift = false;
	DWORD					m_dwDirectionFlag = 0;
	DWORD					m_dwDodge_DirectionFlag = 0;
	// 피격관련
	DWORD					m_dwHurtDirectionFlag = 0;
	DWORD					m_dwOldHurtDirectionFlag = 0;
	_vec3					m_vKnockBackDir = { INIT_VEC3 };

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
/*Player Anima Set */
/*
0. DropItem
1. PickItem
2. CheckPoint_E
3. CheckPoint_S
4. Activation_CheckPoint
5. Death
6. Strong_Damage_R
7. Strong_Damage_L
8. Strong_Damage_F
9. Strong_Damage_B
10. Damage_R
11. Damage_L
12. Damage_F
13. Damage_B
14. Sword_Guard_H
15. Sword_Guard_E
16. Sword_Guard_L
17. Sword_Guard_S
18. Sword_Dodge_Attack
19. Sword_Dodge_R
20. Sword_Dodge_L
21. Sword_Dodge_F
22. Sword_Dodge_B
23. Sword_Fall_Attack_E
24. Sword_Fall_Attack_S
25. Sword_Charge_Attack_1N
26. Sword_Strong_Attack_1N
27. Sword_Attack_5N
28. Sword_Attack_4N
29. Sword_Attack_3N
30. Sword_Attack_2N
31. Sword_Attack_1N
32. Run_R_E
33. Run_R_L
34. Run_L_E
35. Run_L_L
36. Run_F_E
37. Run_F_L
38. Run_B_E
39. Run_B_L
40. Walk_R_L
41. Walk_L_L
42. Walk_F_L
43. Walk_B_L
44. Sword_Idle_N_L
45. Idle_N
46. GameStart_N


*/