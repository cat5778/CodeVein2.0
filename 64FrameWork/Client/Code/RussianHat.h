#pragma once
#include "Defines.h"
#include "GameObject.h"
#include "DynamicObject.h"
BEGIN(Engine)
class CNaviMesh;
END
class CRussianHat : public CDynamicObject
{
public:
	explicit				CRussianHat(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx,_uint uiStageIdx=1);
	explicit				CRussianHat(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo, _uint uiStageIdx=1);
	virtual					~CRussianHat(void);

public:
	HRESULT					Ready_GameObject();
	virtual	HRESULT			LateReady_GameObject()override;
	virtual _int			Update_GameObject(const _float& fTimeDelta) override;
	virtual void			Render_GameObject(void) override;
	


private:
	virtual HRESULT			Add_Component(void);
	void					Set_TransformData();
	void					StateMachine()override;

private://Pattern
	void					Battle_Start(_float fTimeDelta);
	void					Pattern(_float fTimeDelta);

	void					Chaing_Target(_float fTimeDelta);
	//Phase1
	void					Phase1(_float fTimeDelta);
	void					TshieldAttack_Ready(_float fTimeDelta);
	void					TshieldAttack(_float fTimeDelta);

	void					Tshield_Whirlwind(_float fTimeDelta);
	void					TshieldFist(_float fTimeDelta);

	void					TshieldSlide_S(_float fTimeDelta);
	void					TshieldSlide_L(_float fTimeDelta);
	void					TshieldSlide_E(_float fTimeDelta);

	void					Dodge(_float fTimeDelta);
	void					Tshield_JumpAttack(_float fTimeDelta);

	void					HoneAttack1(_float fTimeDelta);
	void					Idle(_float fTimeDelta);

	void					Deformation(_float fTimeDelta);
	
	//Phase2
	void					Phase2(_float fTimeDelta);

	void					BoostAttack_S(_float fTimeDelta);
	void					BoostAttack_L(_float fTimeDelta);
	void					BoostAttack_E(_float fTimeDelta);

	void					FistAttack_N(_float fTimeDelta);

	void					BoostJump_S(_float fTimeDelta);
	void					BoostJump_L(_float fTimeDelta);
	void					BoostJump_E(_float fTimeDelta);



	//패턴 추가할것 원거리 마법 ,얼음검 휘두르기 

	//void					

public:
	static CRussianHat*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx = 0, _uint uiStageIdx = 0);
	static CRussianHat*	Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx ,  TRANSFORM_INFO tInfo, _uint uiStageIdx = 0);

private:
	_bool						m_bIsStart = false;
	_float						m_fJump=0.f;
	RUSSIANTSTATE				m_eCurState;
	RUSSIANTSTATE				m_ePreState;
	_bool						m_bIsPhase2 = false;
	_uint						m_uiFist=0;
private:
	virtual void			Free(void) override;
};

/* Russian Hat Ani Index */
// 0. TFist_KetsugiAllRange_E
// 1. TFist_KetsugiAllRange_L
// 2. TFist_KetsugiAllRange_S
// 3. TFist_BoostUP02_S
// 4. TFist_AttackJump01_E
// 5. TFist_AttackJump01_L
// 6. TFist_AttackJump01_S
// 7. TFist_AttackNormal01_N
// 8. TFist_KetsugiIceBlade_N
// 9. TFist_AttackBoost01B_E
//10. TFist_AttackBoost01A_E
//11. TFist_AttackBoost01_L
//12. TFist_AttackBoost01_S

//13. TShield_Deformation_N
//14. TShield_AttackJump01_N
//15. TShield_AttackHorn02_N
//16. TShield_AttackHorn01_N
//17. TShield_KetsugiAimShoot_N
//18. TShield_AttackSlide_E
//19. TShield_AttackSlide_L
//20. TShield_AttackSlide_S
//21. TShield_AttackNormal02_N
//22. TShield_AttackNormal01B_N
//23. TShield_AttackNormal01A_N
//24. TShield_AttackNormal01_N
//25. Down_P
//26. Damage_FL
//27. Damage_BL
//28. Dodge_B
//29. Walk_R_L
//30. Walk_L_L
//31. Walk_F_L
//32. Walk_B_L
//33. Run_F_L     
//34. IdleTurn_R180
//35. IdleTurn_R90
//36. IdleTurn_L180
//37. IdleTurn_L90
//38. Idle_N
//39. TShield_Apperance_L
//40. TShield_Apperance_N
//41