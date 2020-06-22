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
	_float		Get_AniRatio();

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
	void		Guard(_float fTimeDelta);
	void		Guard_H(_float fTimeDelta);

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
	_bool					m_bIsGuard = false;

	OBJECTSTATE				m_eCurState;
	OBJECTSTATE				m_ePreState;
	_bool					m_bIsAttack=false;
	_float					m_fChargeTime = 0.f;
	_uint					m_uiCombo = 0;

	_float					m_fDamage = 10.f;


public:
	static CPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiIdx=0, _uint uiStageIdx = 0);

private:
	virtual void Free(void) override;
};

#endif // Player_h__
/*Player Anima Set */
/*

0. Renketsu_Search
1. Renketsu_Ground
2. Renketsu_Enchant
3. Renketsu_Buff
4. Renketsu_Barrier
5. DropItem
6. PickItem
7. CheckPoint_E
8. CheckPoint_S
9. Activation_CheckPoint
10. Death
11. Strong_Damage_R
12. Strong_Damage_L
13. Strong_Damage_F
14. Strong_Damage_B
15. Damage_R
16. Damage_L
17. Damage_F
18. Damage_B
19. Sword_Guard_H
20. Sword_Guard_E
21. Sword_Guard_L
22. Sword_Guard_S
23. Sword_Dodge_Attack
24. Sword_Dodge_R
25. Sword_Dodge_L
26. Sword_Dodge_B
27. Sword_Dodge_F
28. Sword_Strong_Attack_N
29. Sword_Charge_Attack_N
30. Sword_Attack_5N
31. Sword_Attack_4N
32. Sword_Attack_3N
33. Sword_Attack_2N
34. Sword_Attack_1N
35. Run_R_E
36. Run_R_L
37. Run_L_E
38. Run_L_L
39. Run_F_E
40. Run_F_L
41. Run_B_E
42. Run_B_L
43. Walk_R_L
44. Walk_L_L
45. Walk_F_L
46. Walk_B_L
47. Sword_Idle_N_L
48. Idle_N
49. GameStart_N

*/