#include "stdafx.h"
#include "RussianHat.h"
#include "Export_Function.h"
#include "ColliderManager.h"
#include "Shield.h"

CRussianHat::CRussianHat(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx, _uint uiStageIdx )
	: CDynamicObject(pGraphicDev,wstrName,uiIdx, uiStageIdx)
{
	m_bIsMonster = true;

}

CRussianHat::CRussianHat(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo, _uint uiStageIdx)
	:CDynamicObject(pGraphicDev,wstrName,uiIdx,tInfo, uiStageIdx)

{
	m_bIsMonster = true;
}

CRussianHat::~CRussianHat()
{
}

HRESULT CRussianHat::Ready_GameObject()
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_uiAni = 39;

	m_fCurHp = m_fMaxHp = 100.f;
	m_fAttackRange = 10.f;
	Set_TransformData();

	if (LOAD_MODE != 0)
		m_uiStageIdx = LOAD_MODE;

	switch ((LOADMODE)m_uiStageIdx)
	{
	case LOAD_NOMAL:
		m_pNaviCom->Set_Index(38);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/BaseCompleteNav.txt");
		break;
	case LOAD_NOMAL2:
		m_pTransformCom->Set_Pos(-20.2f, 1.9f, -49.f);
		m_pNaviCom->Set_Index(111);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");
		break;
	case LOAD_NOMAL3:
		break;

	case LOAD_PLAYER:
		break;
	case LOAD_MONSTER:
		break;
	case LOAD_BATTLE:
		//눈맵 플레이어바로앞
		m_pTransformCom->Set_Pos(-12.32f, 2.73f, -25.3241f);
		m_pNaviCom->Set_Index(76);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");


		//보스 정위치
		//m_pTransformCom->Set_Pos(-20.2f, 1.9f, -49.f);
		//m_pNaviCom->Set_Index(111);// Base Init Idx 38 
		//Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");

		//m_pTransformCom->Set_Pos(18.36f, 0.47f, -0.674f);
		//m_pNaviCom->Set_Index(58);// Base Init Idx 38  //43 Idle

		//m_pTransformCom->Set_Pos(5.3f, 0.19f, -0.95f);
		//m_pNaviCom->Set_Index(38);// Snow Init Idx 38  //43 Idle
		//Load_Text(L"../../Resource/Data/NavMash/BaseCompleteNav.txt");
		break;
	case LOAD_END:
		break;
	default:
		break;
	}
	m_eCurState = RUSSIAN_START_IDLE;


	




	return S_OK;
}

HRESULT CRussianHat::LateReady_GameObject()
{
	m_pTargetTransformCom = dynamic_cast<Engine::CTransform*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Transform", Engine::ID_DYNAMIC));
	//m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Get_Component(L"GameLogic", L"Player", L"Com_Navi", Engine::ID_STATIC));
	//m_pNaviCom->Set_Index(38);// Base Init Idx 38 

	return S_OK;
}

_int CRussianHat::Update_GameObject(const _float & fTimeDelta)
{
	if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM1))
	{
		m_uiAni++;
		cout << m_uiAni << endl;
	}
		StateMachine();

	//srand((unsigned int)time(NULL));
	//
	//if (!m_bIsStart)
	//{
	//	Battle_Start(fTimeDelta);
	//	StateMachine();

	//}
	//else
	//{
	//	if (CKeyMgr::GetInstance()->KeyDown(KEY_NUM1))
	//		m_eCurState = RUSSIAN_DEFORMATION;

	//	StateMachine();
	//	m_fDistance=Get_TargetDist();
	//	Pattern(fTimeDelta);

	//}
	
	





	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pMeshCom->Play_Animation(m_fAnimSpeed*fTimeDelta);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);
	return S_OK;
}

void CRussianHat::Render_GameObject(void)
{

	LPD3DXEFFECT	pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	_uint	iPassMax = 0;

	SetUp_ConstantTable(pEffect);

	pEffect->Begin(&iPassMax, 0);

	pEffect->BeginPass(0);

	m_pMeshCom->Render_Meshes(pEffect);

	pEffect->EndPass();

	pEffect->End();

	Safe_Release(pEffect);

	m_pColliderGroupCom->Render_Collider();



}

HRESULT CRussianHat::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"RussianHat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);// 변경

	pComponent = m_pTransformCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	pComponent = m_pRendererCom = Engine::Get_Renderer();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->AddRef();
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Renderer", pComponent);

	pComponent = m_pCalculatorCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Shader_Mesh"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pColliderGroupCom = Engine::CColliderGroup::Create(m_pGraphicDev, m_wstrInstName, m_pTransformCom, m_pMeshCom);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_ColliderGroup", pComponent);
	for (int i = 0; i < Engine::COLOPT_END; i++)
		m_pColliderGroupCom->Set_ColliderEnable((Engine::COLLOPTION)i, true);



	pComponent = m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::CNaviMesh::Create(m_pGraphicDev));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);


	CColliderManager::GetInstance()->Get_ObjCollider(m_pColliderGroupCom, m_ObjName);

	return S_OK;
}

void CRussianHat::Set_TransformData()
{
	if (m_bIsRespawn)
	{
		m_pTransformCom->m_vScale = m_tTransformInfo.vScale;
		m_pTransformCom->m_vAngle = m_tTransformInfo.vRotation;
		m_pTransformCom->m_vInfo[Engine::INFO_POS] = m_tTransformInfo.vPosition;
	}
	else
	{
		m_pTransformCom->Set_Scale(DEFAULT_MESH_SCALE);
		m_pTransformCom->Set_Pos(INIT_VEC3);
	}
}

void CRussianHat::StateMachine()
{
	if (m_ePreState != m_eCurState)
	{
		m_ePreState = m_eCurState;
		m_fAnimSpeed = 1.0f;

		switch (m_eCurState)
		{
		case RUSSIAN_START_IDLE:  
		{
			m_fAttackRange = 4.0f;
			m_fAnimSpeed = 1.0f;
			m_pMeshCom->Set_AnimationSet(39);
			break;
		}
		case RUSSIAN_BATTLE_START:
		{
			m_fAnimSpeed = 1.0f;
			m_pMeshCom->Set_AnimationSet(40);
		}
			break;
		case RUSSIAN_BATTLE_IDLE:
		{
			m_fAnimSpeed = 1.0f;
			m_fRotSpeed = 3.0f;
			m_fAttackRange = 7.f;
			m_pMeshCom->Set_AnimationSet(38);
		}
			break;
		case RUSSIAN_WALK://29~32
		{
			m_fSpeed = 1.f;
			m_fAnimSpeed = 1.0f;
			m_fRotSpeed = 3.0f;
			m_fAttackRange = 20.f;

			m_pMeshCom->Set_AnimationSet(31);
		}
		break;
		case RUSSIAN_RUN: //
		{
			m_fSpeed = 3.f;
			m_fRotSpeed = 6.0f;
			m_fAnimSpeed = 1.5f;
			m_fAttackRange = 4.f;
			m_pMeshCom->Set_AnimationSet(33);
		}
			break;
		case RUSSIAN_HURT://26~27
			break;
		case RUSSIAN_APPERANCE_N:
			m_pMeshCom->Set_AnimationSet(40);
			break;
		case RUSSIAN_DOWN:
			m_pMeshCom->Set_AnimationSet(25);
			break;
		case RUSSIAN_DODGE:
		{
			m_fAttackRange = 40.f;
			m_fAnimSpeed = 2.f;

			m_pMeshCom->Set_AnimationSet(28);
		}
			break;
		case RUSSIAN_ATTACK_READY:
		{
			m_fAnimSpeed = 2.0f;
			m_fRotSpeed = 5.0f;

			m_fAttackRange = 40.f;
			m_pMeshCom->Set_AnimationSet(24);
		}
			break;
		case RUSSIAN_ATTACK1:
		{
			m_fAnimSpeed = 2.0f;
			m_fRotSpeed = 5.0f;
			m_fAttackRange = 20.f;
			m_pMeshCom->Set_AnimationSet(23);
		}
			break;
		case RUSSIAN_ATTACK2:
			m_fAnimSpeed = 3.0f;

			m_pMeshCom->Set_AnimationSet(22);
			break;
		case RUSSIAN_ATTACK3:
			m_pMeshCom->Set_AnimationSet(21);
			break;
		case RUSSIAN_ATTACK_SLIDE_S:
			m_fRotSpeed = 17.0f;
			m_fAttackRange = 40.f;
			m_fAnimSpeed = 2.0f;

			m_pMeshCom->Set_AnimationSet(20);
			break;
		case RUSSIAN_ATTACK_SLIDE_L:
			m_fAttackRange = 40.f;
			m_fAnimSpeed = 2.0f;

			m_pMeshCom->Set_AnimationSet(19);
			break;
		case RUSSIAN_ATTACK_SLIDE_E:
			m_fAttackRange = 40.f;

			m_pMeshCom->Set_AnimationSet(18);
			break;
		case RUSSIAN_ATTACK_HORN1:
		{
			m_fAttackRange = 10.f;

			m_fRotSpeed = 15.0f;
			m_pMeshCom->Set_AnimationSet(16);
		}
			break;
		case RUSSIAN_ATTACK_HORN2:
			m_pMeshCom->Set_AnimationSet(15);
			break;
		case RUSSIAN_ATTACK_JUMP:
		{
			m_fRotSpeed = 17.0f;

			m_fAnimSpeed = 3.0f;
			m_fAttackRange = 40.f;
			m_pMeshCom->Set_AnimationSet(14);
		}
			break;
		case RUSSIAN_DEFORMATION:
		{
			m_fAnimSpeed = 3.0f;

			m_pMeshCom->Set_AnimationSet(13);
		}
			break;
		case RUSSIAN_KETSUGI_AIMSHOOT:
			m_pMeshCom->Set_AnimationSet(17);
			break;
		case RUSSIAN_FIST_ATTACKBOOST1_S:
			m_pMeshCom->Set_AnimationSet(12);
			break;
		case RUSSIAN_FIST_ATTACKBOOST1_L:
			m_pMeshCom->Set_AnimationSet(11);
			break;
		case RUSSIAN_FIST_ATTACKBOOST1_A_E:
			m_pMeshCom->Set_AnimationSet(10);
			break;
		case RUSSIAN_FIST_ATTACKBOOST1_B_E:
			m_pMeshCom->Set_AnimationSet(9);
			break;
		case RUSSIAN_FIST_ATTACKNORMAL1:
			m_pMeshCom->Set_AnimationSet(7);
			break;
		case RUSSIAN_FIST_ATTACKJUMP_S:
			m_pMeshCom->Set_AnimationSet(6);
			break;
		case RUSSIAN_FIST_ATTACKJUMP_L:
			m_pMeshCom->Set_AnimationSet(5);
			break;
		case RUSSIAN_FIST_ATTACKJUMP_E:
			m_pMeshCom->Set_AnimationSet(4);
			break;
		case RUSSIAN_FIST_BOOSTUP_S:
			m_pMeshCom->Set_AnimationSet(3);
			break;
		case RUSSIAN_FIST_KEYSUGI_ALLRANGE_S:
			m_pMeshCom->Set_AnimationSet(2);
			break;
		case RUSSIAN_FIST_KEYSUGI_ALLRANGE_L:
			m_pMeshCom->Set_AnimationSet(1);
			break;
		case RUSSIAN_FIST_KEYSUGI_ALLRANGE_E:
			m_pMeshCom->Set_AnimationSet(0);
			break;
		case RUSSIAN_END:
			break;
		default:
			break;
		}

	}

}

void CRussianHat::Battle_Start(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_BATTLE_START)
	{
		_float fRatio = Get_AniRatio();
		if (Get_AniRatio()>=0.7f)
		{
			m_bIsStart = true;
		}
	}
	if (m_eCurState == RUSSIAN_START_IDLE)
	{
		//cout << Get_TargetDist() << endl;
		if (Get_TargetDist() <= 6.0f)
			m_eCurState = RUSSIAN_BATTLE_START;
	}

}

void CRussianHat::Pattern(_float fTimeDelta)
{
	if (m_fCurHp / m_fMaxHp >= 0.6f)
		Phase1(fTimeDelta);
	else
	{
		Phase2(fTimeDelta);
	}

}


void CRussianHat::Chaing_Target(_float fTimeDelta)
{
	if (m_pNaviCom != nullptr)
	{
		if (m_eCurState == RUSSIAN_RUN||m_eCurState == RUSSIAN_WALK)
		{
			_vec3	vPos, vDir, vDiagonalDir, vOutPos;
			_float	fDgree = Get_AngleOnTheTarget();
			vDir = Get_TargetPos() - Get_Pos();
			m_fDistance = D3DXVec3Length(&vDir);
			vDir.y = 0.f;
			D3DXVec3Normalize(&vDir, &vDir);
			RotateToTarget(fTimeDelta, 0.0f);

			m_pNaviCom->Move_OnNaviMesh(&Get_Pos(), &(vDir * m_fSpeed* fTimeDelta), &vOutPos);
			m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
		}
	}

}

void CRussianHat::Phase1(_float fTimeDelta)
{

	if (m_fDistance >= m_fAttackRange)
	{
		if (m_fDistance >= 7.f)
		{
			if (rand() & 1)
				m_eCurState = RUSSIAN_ATTACK_JUMP;
			else
				m_eCurState = RUSSIAN_ATTACK_READY;

		}
		else if (m_fDistance < 7.f)
			m_eCurState = RUSSIAN_RUN;
		else if (m_fDistance < 4.f)
			m_fAttackRange =10.f;
		//else if(m_fDistance<4.5f)
		//	m_eCurState = RUSSIAN_WALK;

		Chaing_Target(fTimeDelta);

	}
	else
	{
		if (m_fDistance <= m_fAttackRange)
		{
			switch (m_eCurState)
			{
			case RUSSIAN_BATTLE_IDLE:
				Idle(fTimeDelta);
				break;
			case RUSSIAN_DODGE:
				Dodge(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_READY:
				TshieldAttack_Ready(fTimeDelta);
				break;
			case RUSSIAN_ATTACK1:
				TshieldAttack(fTimeDelta);
				break;
			case RUSSIAN_ATTACK2:
				Tshield_Whirlwind(fTimeDelta);
				break;
			case RUSSIAN_ATTACK3:
				TshieldFist(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_SLIDE_S:
				TshieldSlide_S(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_SLIDE_L:
				TshieldSlide_L(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_SLIDE_E:
				TshieldSlide_E(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_JUMP:
				Tshield_JumpAttack(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_HORN1:
				HoneAttack1(fTimeDelta);
				break;
			case RUSSIAN_ATTACK_HORN2:
				break;
			case RUSSIAN_DEFORMATION:
				Deformation(fTimeDelta);
				break;
			case RUSSIAN_KETSUGI_AIMSHOOT:
				break;
			default:
				Idle(fTimeDelta);
				break;
			}

		}
	}

}

void CRussianHat::TshieldAttack_Ready(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_READY)
	{
		if (m_fAttackRange == 4.0f)
			m_fAttackRange = 8.0f;
		
		m_fRotSpeed = 3.0f;
		RotateToTarget(fTimeDelta, 0.0f, 0.4f);
		m_fRotSpeed = 9.0f;
		RotateToTarget(fTimeDelta, 0.5f, 0.58f);


		if (Get_AniRatio() >= 0.58f)
		{
			if(Get_TargetDist()>=6.0f||rand() &1)
				m_eCurState = RUSSIAN_ATTACK1;
			else
				m_eCurState = RUSSIAN_ATTACK2;

		}
	}
}

void CRussianHat::TshieldAttack(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK1)
	{
		if (Get_AniRatio() >= 0.9f)
			m_eCurState = RUSSIAN_BATTLE_IDLE;
		else
		{
			MoveAni(fTimeDelta, 0.0f, 0.1f, 30.0f, Get_Look());
			MoveAni(fTimeDelta, 0.1f, 0.2f, 10.0f, Get_Look());
			MoveAni(fTimeDelta, 0.2f, 0.3f, 30.0f, Get_Look());
		}
	}
}

void CRussianHat::Tshield_Whirlwind(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK2)
	{
		if (Get_AniRatio() >= 0.9f)
			m_eCurState = RUSSIAN_BATTLE_IDLE;
		else
		{
			RotateToTarget(fTimeDelta, 0.0f, 0.2f);
			MoveAni(fTimeDelta, 0.2f, 0.6f , 6.0f, Get_Look());
		}
	}
}

void CRussianHat::TshieldFist(_float fTimeDelta)
{
	if(m_eCurState == RUSSIAN_ATTACK3)
	{
		if (m_fAttackRange == 4.0f)
			m_fAttackRange = 8.0f;

		if (Get_AniRatio() >= 0.8f)
		{
				
			m_eCurState = RUSSIAN_DODGE;

		}
		else
		{
			RotateToTarget(fTimeDelta, 0.f, 0.4f);
			MoveAni(fTimeDelta, 0.3f, 0.4f, 6.f, Get_Look());
			if (Get_AniRatio() >= 0.4f)
				m_fAnimSpeed = 3.5f;
		}

	}
}

void CRussianHat::TshieldSlide_S(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_SLIDE_S)
	{
		if (Get_AniRatio() >= 0.8f)
		{
			m_eCurState = RUSSIAN_ATTACK_SLIDE_L;
		}
		else
		{
			RotateToTarget(fTimeDelta, 0.f, 0.6f);
			MoveAni(fTimeDelta, 0.6f, 0.8f, 4.0f, Get_Look());

		}
	}


}

void CRussianHat::TshieldSlide_L(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_SLIDE_L)
	{
		if (Get_AniRatio() >= 0.8f)
		{
			m_eCurState = RUSSIAN_ATTACK_SLIDE_E;
		}
		else
		{
			MoveAni(fTimeDelta, 0.0f, 0.7f, 25.0f, Get_Look());
			MoveAni(fTimeDelta, 0.7f, 0.8f, 15.0f, Get_Look());
		}
	}

}

void CRussianHat::TshieldSlide_E(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_SLIDE_E)
	{
		if (Get_AniRatio() >= 0.8f)
		{
			m_eCurState = RUSSIAN_BATTLE_IDLE;
		}
		else
		{
			MoveAni(fTimeDelta, 0.0f, 0.2f, 8.0f, Get_Look());

		}
	}
}

void CRussianHat::Dodge(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_DODGE)
	{
		if (Get_AniRatio() >= 0.65f)
		{
			m_eCurState = RUSSIAN_ATTACK_SLIDE_S;
		}
		else
		{
			MoveAni(fTimeDelta, 0.1f, 0.4f, 10.f, -Get_Look());
			MoveAni(fTimeDelta, 0.4f, 0.5f, 4.f, -Get_Look());
			MoveAni(fTimeDelta, 0.5f, 0.6f, 1.f, -Get_Look());


		}
	}
}

void CRussianHat::Tshield_JumpAttack(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_JUMP)
	{
		if (Get_AniRatio() >= 0.65f)
		{
			m_eCurState = RUSSIAN_ATTACK2;
		}
		else
		{

			RotateToTarget(fTimeDelta, 0.f, 0.1f);
			MoveAni(fTimeDelta, 0.1f, 0.125f, 12.f, _vec3(0.f, 1.0f, 0.f), true);
			MoveAni(fTimeDelta, 0.125f, 0.3f, 8.f, _vec3(0.f, 1.0f, 0.f), true);
			MoveAni(fTimeDelta, 0.3f, 0.35f, 10.f, _vec3(0.f, -1.f, 0.f), true);
			MoveAni(fTimeDelta, 0.35f, 0.5f, 50.f, _vec3(0.f, -1.f, 0.f), true);
			MoveAni(fTimeDelta, 0.1f, 0.2f, 20.f, Get_Look(), true);
			MoveAni(fTimeDelta, 0.2f, 0.5f, 10.f, Get_Look(), true);
		}
		
	
	
	}
}

void CRussianHat::HoneAttack1(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_ATTACK_HORN1)
	{
		if (Get_AniRatio() >= 0.8f)
		{
			m_fAnimSpeed = 1.0f;
			if (m_uiPattern & 1)
				m_eCurState = RUSSIAN_ATTACK3;
			else
				m_eCurState = RUSSIAN_ATTACK_READY;
		
		}
		else
		{
			RotateToTarget(fTimeDelta, 0.f, 0.1f);
			if (Get_AniRatio() <= 0.4f)
				m_fAnimSpeed = 2.0f;

			if (Get_AniRatio() >= 0.4f)
				m_fAnimSpeed = 3.0f;

		}



	}
}

void CRussianHat::Idle(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_WALK||m_eCurState== RUSSIAN_RUN)
	{
		m_eCurState = RUSSIAN_BATTLE_IDLE;
	
	}
	else if (m_eCurState == RUSSIAN_BATTLE_IDLE)
	{
		if (Get_AniRatio() >= 0.05f)
		{
			
			if (m_uiPattern == 0)
				m_eCurState = RUSSIAN_ATTACK_HORN1;
			else if (m_uiPattern == 1)
				m_eCurState = RUSSIAN_ATTACK_READY;
			else if (m_uiPattern == 2)
				m_eCurState = RUSSIAN_ATTACK3;
			else if (m_uiPattern == 3)
				m_eCurState = RUSSIAN_ATTACK_SLIDE_S;
			else if(m_uiPattern >=4)
				m_uiPattern = -1;
			m_uiPattern++;
			
		}
			

	}
	else if (m_pMeshCom->Is_AnimationSetEnd())
	{
		m_eCurState = RUSSIAN_BATTLE_IDLE;
	}
}

void CRussianHat::Deformation(_float fTimeDelta)
{
	if (m_eCurState == RUSSIAN_DEFORMATION)
	{
		if (Get_AniRatio() >= 0.45f)
		{
			CShield* pShield= dynamic_cast<CShield*>(Engine::Get_GameObject(L"GameLogic", L"Shield"));
			if (pShield != nullptr)
			{
				pShield->Set_Equip(false);
				pShield->Set_CellIdx(m_pNaviCom->Get_CurIndex());
				pShield->Set_Throw();

			}
		}
		else
		{

		}

	}
}

void CRussianHat::Phase2(_float fTimeDelta)
{
}


CRussianHat * CRussianHat::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName,_uint uiIdx, _uint uiStageIdx)
{
	CRussianHat*	pInstance = new CRussianHat(pGraphicDev, wstrName, uiIdx, uiStageIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}
CRussianHat * CRussianHat::Create(LPDIRECT3DDEVICE9 pGraphicDev, wstring wstrName, _uint uiIdx, TRANSFORM_INFO tInfo, _uint uiStageIdx)
{
	CRussianHat*	pInstance = new CRussianHat(pGraphicDev, wstrName, uiIdx, tInfo, uiStageIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;

}
void CRussianHat::Free(void)
{
	Engine::CGameObject::Free();
}
