#include "stdafx.h"
#include "Player.h"
#include <fstream>
#include "Export_Function.h"
#include "ThirdPersonCamera.h"
#include "ColliderManager.h"
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiIdx,_uint uiStageIdx)
	: Engine::CGameObject(pGraphicDev)
{
	m_uiIdx = uiIdx;
	m_ObjName = L"Player2";
	m_uiStageSet = uiStageIdx;
	m_wstrInstName = m_ObjName + L"_" + to_wstring(m_uiIdx);

}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	if (LOAD_MODE != 0)
		m_uiStageSet = LOAD_MODE;
	switch ((LOADMODE)m_uiStageSet)
	{
	case LOAD_NOMAL:
		m_pNaviCom->Set_Index(38);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/BaseCompleteNav.txt");
		break;
	case LOAD_NOMAL2:
		m_pTransformCom->Set_Pos(-14.7f, 2.16f, -20.2f); //Boss
		m_pNaviCom->Set_Index(70);// Base Init Idx 38 

		//m_pTransformCom->Set_Pos(-85.f, 1.3f, 0.01f);Start
		//m_pNaviCom->Set_Index(0);// Base Init Idx 38 
		//Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");
		break;
	case LOAD_NOMAL3:
		m_pMeshCom->Set_AnimationSet(46);
		break;

	case LOAD_PLAYER:
		break;
	case LOAD_MONSTER:
		break;
	case LOAD_BATTLE:
		m_pTransformCom->Set_Pos(-14.7f, 2.16f, -20.2f); //Boss
		m_pNaviCom->Set_Index(70);// Base Init Idx 38 
		Load_Text(L"../../Resource/Data/NavMash/Temp5.txt");

		//m_pTransformCom->Set_Pos(-8.f, 0.8f, -2.6f);
		//m_pNaviCom->Set_Index(18);// Base Init Idx 38 
		//Load_Text(L"../../Resource/Data/NavMash/BaseCompleteNav.txt");

		break;
	case LOAD_END:
		break;
	default:
		break;
	}


	m_eCurState = OBJ_START;

	m_pTransformCom->Set_Scale(0.01f, 0.01f, 0.01f);

	
	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{

	Key_Input(fTimeDelta);

	//_vec3 vPos2 = *m_pTransformCom->Get_Info(Engine::INFO_POS);
	//cout <<"X=" <<vPos.x << "y="<< vPos.y <<"Z="<< vPos.z << endl;
	//cout << "Cur Cell " << m_pNaviCom->Get_CurIndex() << endl;

/*
	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pMeshCom->Play_Animation(fTimeDelta*m_fAnimSpeed);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);*/


	
	//몬스터들 전부다 들고오기
	
	Engine::CColliderGroup* pCollCom = dynamic_cast<Engine::CColliderGroup*>
		(Engine::Get_Component(L"GameLogic", L"RussianHat", L"Com_ColliderGroup", Engine::ID_DYNAMIC));
	_float fPower=0.f;
	if (pCollCom != nullptr)
	{
		Engine::CTransform* pTransCom = dynamic_cast<Engine::CTransform*>
			(Engine::Get_Component(L"GameLogic", L"RussianHat", L"Com_Transform", Engine::ID_DYNAMIC));

		_bool bIsColl = m_pCalculatorCom->Collsion_Sphere(m_pColliderGroupCom->Get_CollVec(Engine::COLOPT_ATTACK),
															pCollCom->Get_CollVec(Engine::COLOPT_HURT));

		_bool bIsColl2 = m_pCalculatorCom->Bounding_Sphere(m_pColliderGroupCom->Get_CollVec(Engine::COLOPT_STEP),
			pCollCom->Get_CollVec(Engine::COLOPT_STEP),&fPower);

		if (bIsColl2)
		{
			cout << fPower << endl;
			_vec3 vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);   //플레이어위치 
			_vec3 vTargetPos = *pTransCom->Get_Info(Engine::INFO_POS);	//몬스터 위치


			 _vec3 vDir =vPos - vTargetPos;		//미는 방향
			 D3DXVec3Normalize(&vDir, &vDir);  // 노말
			 vDir =vDir*-fPower;

			 vPos = vPos-vDir;						//내포지션에 미는방향으로 더해주기
			 m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);	//적용
			 _vec3 vPos3 = *m_pTransformCom->Get_Info(Engine::INFO_POS);

		}

		if (m_pColliderGroupCom->IsColl(Engine::COLOPT_ATTACK, Engine::STATE_ENTER))
			cout << "공격 발생 " << endl;
		if (m_pColliderGroupCom->IsColl(Engine::COLOPT_ATTACK, Engine::STATE_STAY))
			cout << "공격중 " << endl;
		if (m_pColliderGroupCom->IsColl(Engine::COLOPT_ATTACK, Engine::STATE_EXIT))
			cout << "공격 끝 " << endl;
	}

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	m_pMeshCom->Play_Animation(fTimeDelta*m_fAnimSpeed);
	m_pRendererCom->Add_RenderGroup(Engine::RENDER_NONALPHA, this);


	return 0;
}

void CPlayer::Render_GameObject(void)
{
	//m_pSpherColliderCom->Render_Collider(Engine::COLLTYPE::COL_TRUE);
	m_pNaviCom->Render_NaviMesh();

	_matrix mat = m_pTransformCom->m_matWorld;

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

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*		pComponent = nullptr;

	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Player"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	pComponent = m_pNaviCom = dynamic_cast<Engine::CNaviMesh*>(Engine::Clone(RESOURCE_STAGE, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Navi", pComponent);


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

	CColliderManager::GetInstance()->Get_ObjCollider(m_pColliderGroupCom, m_ObjName);

	//m_pSpherColliderCom->Set_Radius(14.0f);
	//m_pSpherColliderCom->Set_Pos(_vec3(0.f,250.f,0.f));

	//pComponent=m_pSpherColliderCom = Engine::CSphereColliderCom::Create(m_pGraphicDev,L"Mesh_Player",m_pTransformCom);
	//const Engine::D3DXFRAME_DERIVED* pBone = m_pMeshCom->Get_FrameByName("RightHandAttach");

	//m_pSpherColliderCom->Set_DMParentMatrix(pBone);

	//m_pComponentMap[Engine::ID_DYNAMIC].emplace(L"Com_SphereColl", pComponent);


	m_pKeyMgr = CKeyMgr::GetInstance();

	//// collider
	//pComponent = m_pColliderCom = Engine::CCollider::Create(m_pGraphicDev,
	//	m_pMeshCom->Get_VtxPos(),
	//	m_pMeshCom->Get_NumVtx(),
	//	m_pMeshCom->Get_Stride());
	//NULL_CHECK_RETURN(pComponent, E_FAIL);
	//m_pComponentMap[Engine::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{

	m_dwDirectionFlag = 0;
	m_bIsShift = false;
	_bool Test = false;

	IdleOption();

	if (m_pKeyMgr->KeyDown(KEY_Q))
	{
		
		m_bIsLockOn ? m_bIsLockOn = false : m_bIsLockOn = true;
		m_pCam = dynamic_cast<CThirdPersonCamera*>(Engine::Get_GameObject(L"UI", L"ThirdPersonCamera"));
		m_pCam->LockOn(m_bIsLockOn);
	}



	if (m_pKeyMgr->KeyPressing(KEY_W))
	{
		m_dwDirectionFlag |= FRONT;
		if (CheckEnableState())
		{
			m_eCurState = OBJ_WALK;
		}
	}

	if (m_pKeyMgr->KeyPressing(KEY_S))
	{
		m_dwDirectionFlag |= BACK;
		if (CheckEnableState())
		{
			m_eCurState = OBJ_WALK;
		}

	}

	if (m_pKeyMgr->KeyPressing(KEY_A))
	{
		m_dwDirectionFlag |= LEFT;
		if (CheckEnableState())
		{
			m_eCurState = OBJ_WALK;
		}

	}
	if (m_pKeyMgr->KeyPressing(KEY_D))
	{
		m_dwDirectionFlag |= RIGHT;
		if (CheckEnableState())
		{
			m_eCurState = OBJ_WALK;
		}

	}
	if (m_pKeyMgr->KeyPressing(KEY_SHIFT))
	{
		if(m_eCurState ==OBJ_WALK)
			m_eCurState = OBJ_RUN;
	}
	if (m_pKeyMgr->KeyDown(KEY_LBUTTON))
	{
		if (m_eCurState == OBJ_ATTACK)
		{
			_double dRatio = m_pMeshCom->Get_TrackPosition() / m_pMeshCom->Get_Period();
			if (dRatio >= 0.2&&dRatio < 0.7)
			{
				m_uiCombo++;
				switch (m_uiCombo)
				{
				case 2:
					m_pMeshCom->Set_AnimationSet(30);
					break;
				case 3:
					m_pMeshCom->Set_AnimationSet(29);
					break;
				case 4:
					m_pMeshCom->Set_AnimationSet(28);
					break;
				case 5:
					m_pMeshCom->Set_AnimationSet(27);
					break;
				case 6:
				{
					m_pMeshCom->Set_AnimationSet(31);
					m_uiCombo = 1;
				}
					break;
				default:
					break;
				}
			}
		}
		else
		{
			if (m_eCurState == OBJ_DODGE)
			{
				_double dRatio = m_pMeshCom->Get_TrackPosition() / m_pMeshCom->Get_Period();
				if (dRatio >= 0.2&&dRatio < 0.9)
				{
					m_eCurState = OBJ_ATTACK;
					m_pMeshCom->Set_AnimationSet(18);
				}

			}
			else
			{
				m_eCurState = OBJ_ATTACK;
				m_pMeshCom->Set_AnimationSet(31);
				m_uiCombo = 1;
			}
		}

	}
	if (m_pKeyMgr->KeyPressing(KEY_ALT))
	{
		m_eCurState = OBJ_GUARD;
	}
	if (m_pKeyMgr->KeyPressing(KEY_RBUTTON))
	{
		m_eCurState = OBJ_STRONG_ATTACK;

		m_pMeshCom->Set_AnimationSet(26);
		if(m_fChargeTime<1.0f)
		m_fChargeTime += fTimeDelta;
	}
	if (m_pKeyMgr->KeyUp(KEY_RBUTTON))
	{
		if (m_fChargeTime >= 0.75f)
		{
			m_eCurState = OBJ_CHARGE_ATTACK;

		}
		else
			m_eCurState = OBJ_STRONG_ATTACK;

	}
	if (m_pKeyMgr->KeyDown(KEY_SPACE))
	{
		if (m_eCurState != OBJ_DODGE)
		{
			m_eCurState = OBJ_DODGE;
			if (m_dwDodge_DirectionFlag == 0)
				m_dwDodge_DirectionFlag = m_dwDirectionFlag;
		}
	}

	if (m_eCurState == OBJ_ATTACK || m_eCurState == OBJ_STRONG_ATTACK || m_eCurState == OBJ_CHARGE_ATTACK)
	{
		AttackMoveSet(fTimeDelta);
		StorngAttackMoveSet(fTimeDelta);
		ChargeAttackMoveSet(fTimeDelta);
		m_dwDirectionFlag = 0;

	}
	if (Engine::Get_DIKeyState(DIK_RETURN) & 0x80)
	{
		m_pMeshCom->Set_AnimationSet(0);
	}


	if (m_pCameraTransformCom != nullptr)
	{
		Check_Direction(fTimeDelta);
		StateMachine();
	}

}

HRESULT CPlayer::SetUp_ConstantTable(LPD3DXEFFECT & pEffect)
{
	_matrix			matWorld, matView, matProj;

	m_pTransformCom->Get_WorldMatrix(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	const D3DLIGHT9*		pLight = Engine::Get_LightInfo(0);
	NULL_CHECK_RETURN(pLight, S_OK);

	pEffect->SetVector("g_vLightDir", &_vec4(pLight->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (_vec4*)&pLight->Diffuse);
	pEffect->SetVector("g_vLightSpecular", (_vec4*)&pLight->Specular);
	pEffect->SetVector("g_vLightAmbient", (_vec4*)&pLight->Ambient);

	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 100.f;

	pEffect->SetVector("g_vMtrlDiffuse", (_vec4*)&tMtrlInfo.Diffuse);
	pEffect->SetVector("g_vMtrlSpecular", (_vec4*)&tMtrlInfo.Specular);
	pEffect->SetVector("g_vMtrlAmbient", (_vec4*)&tMtrlInfo.Ambient);

	_vec4		vCamPos;
	D3DXMatrixInverse(&matView, NULL, &matView);
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec4));

	pEffect->SetVector("g_vCamPos", &vCamPos);
	pEffect->SetFloat("g_fPower", tMtrlInfo.Power);

	return S_OK;

}

HRESULT CPlayer::Load_Text(const TCHAR * pFilePath)
{
	ifstream fin;
	fin.open(pFilePath);
	if (fin.fail())
		return E_FAIL;
	wstring wstrTemp;
	char cTemp[MIN_STR];
	_uint uidx = 0;
	while (!fin.eof())
	{
		fin.getline(cTemp, MIN_STR);
		wchar_t* ppwchar = CharToWChar(cTemp);
		wstrTemp = ppwchar;
		delete ppwchar;
		
		if (wstrTemp.compare(L"") == 0)
			break;

		Engine::NAVI_DATA* pNaviData = new Engine::NAVI_DATA;

		pNaviData->vPosition1.x = _wtof(wstrTemp.c_str()); 
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition1.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition1.z = atof(cTemp);

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition2.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition2.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition2.z = atof(cTemp);

		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition3.x = atof(cTemp);
		fin.getline(cTemp, MIN_STR); // 공백을 포함한 문장 단위(개행 단위)로 읽어오기.
		pNaviData->vPosition3.y = atof(cTemp);
		fin.getline(cTemp, MIN_STR);
		pNaviData->vPosition3.z = atof(cTemp);

		pNaviData->uiIdx = uidx;

		m_pNaviCom->Add_Cell(pNaviData);

		delete pNaviData;
		uidx++;
	}
	fin.close();
	return S_OK;
}

wchar_t * CPlayer::CharToWChar(const char * pstrSrc)
{
	int nLen = strlen(pstrSrc) + 1;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	mbstowcs(pwstr, pstrSrc, nLen);
	return pwstr;
}

void CPlayer::Check_Direction(_float fTimeDelta)
{
	_vec3 vCamLook = *m_pCameraTransformCom->Get_Info(Engine::INFO_LOOK);
	_vec3 vPlayerLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	_vec3 vPlayerRight = { 1.0f,0.f,0.f };
	vCamLook.y = 0;
	vPlayerLook.y = 0;

	D3DXVec3TransformNormal(&vPlayerRight, &vPlayerRight, &m_pTransformCom->m_matWorld);
	D3DXVec3Normalize(&vCamLook, &vCamLook);
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	float fDgree = Get_Angle(vCamLook, vPlayerLook) + 180.f;

	DWORD dwDirectionFlag;

	if (m_eCurState == OBJ_DODGE)
		dwDirectionFlag = m_dwDodge_DirectionFlag;
	else		
		dwDirectionFlag = m_dwDirectionFlag;
	if (m_bIsLockOn)
	{
		if (m_pCam != nullptr &&m_pCam->Get_MonTransform() !=nullptr)
		{

			_vec3 vPos, vMonPos;

			vPos = *m_pTransformCom->Get_Info(Engine::INFO_POS);
			memcpy(&vMonPos, &m_pCam->Get_MonTransform()->m_matWorld, sizeof(_vec3));

			_float fTargetDist = D3DXVec3Length(&(vPos - vMonPos));

			if (fTargetDist > 1.f)
				RotateToLook(fTimeDelta);
			_vec3	vDir;

			switch (dwDirectionFlag)
			{
			case DIR_F:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_F;
				vDir = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);
			}
				break;
			case DIR_FR:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_FR;
				vDir = *m_pTransformCom->Get_Info(Engine::INFO_LOOK)+ *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
			
				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);

			}
				break;
			case DIR_R:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_R;
					//if (CheckEnableState())
				
				vDir = *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);
			}
				break;
			case DIR_BR:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_BR;
				vDir = -*m_pTransformCom->Get_Info(Engine::INFO_LOOK) + *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);

				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);
			}
				break;
			case DIR_B:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_B;

				vDir = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
				CheckMoveMesh(fTimeDelta, -vDir, false, m_fSpeed);
			}
			break;
			case DIR_BL:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_BL;
				vDir = -*m_pTransformCom->Get_Info(Engine::INFO_LOOK) - *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);

				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);
			}
				break;
			case DIR_L:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_L;

				vDir = *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
				CheckMoveMesh(fTimeDelta, -vDir, false, m_fSpeed);
			}
				break;
			case DIR_FL:
			{
				if (m_eCurState == OBJ_WALK)
					m_eCurState = OBJ_LOCK_WALK_FL;
				vDir = *m_pTransformCom->Get_Info(Engine::INFO_LOOK) - *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);

				CheckMoveMesh(fTimeDelta, vDir, false, m_fSpeed);
			}
			break;
			//default:
				//m_eCurState =OBJ_IDLE;

				break;
			}
		}
	}

	else
	{
		switch (dwDirectionFlag)
		{
		case DIR_F:
		{
			if (fDgree > 5.f&&fDgree < 180.f) //왼쪽으로 외전
				m_pTransformCom->Rotation(Engine::ROT_Y, m_fRotSpeed* fTimeDelta);
			else if (fDgree < 355.f && fDgree >= 180.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

		
				CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, false, m_fSpeed);
		

		}
		break;
		case DIR_R:
		{
			if (fDgree >= 90.f&&fDgree <= 265.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
			else if ((fDgree >= 275.f && fDgree <= 360.f) || (fDgree >= 0.f&& fDgree < 90.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);


				_vec3	vRight = *m_pCameraTransformCom->Get_Info(Engine::INFO_RIGHT);
				//D3DXVec3Cross(&vRight, &_vec3(0.f, 1.f, 0.f), &m_pCam->Get_LockOnLook()) ;
				CheckMoveMesh(fTimeDelta, vRight, false, m_fSpeed);

		}
		break;
		case DIR_FR:
		{
			if ((fDgree <= 310.f && fDgree >= 135.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
			else if ((fDgree >= 320.f&&fDgree <= 360.f) || (fDgree >= 0.f&& fDgree < 135.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, false, m_fSpeed, Engine::INFO_RIGHT, false);
		}
		break;
		case DIR_B:
			if (fDgree > 185.f&& fDgree < 360.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
			else if (fDgree <= 175.f&&fDgree >= 0.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, true, m_fSpeed);
			break;
		case DIR_BR:
		{
			if ((fDgree >= 45.f&&fDgree <= 220.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
			else if ((fDgree <= 360.f && fDgree >= 230.f) || (fDgree >= 0.f&& fDgree < 45.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, true, m_fSpeed, Engine::INFO_RIGHT, false);
		}
		break;
		case DIR_L:
		{
			if (fDgree >= 95.f&&fDgree <= 270.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
			else if ((fDgree <= 85.f&& fDgree >= 0.f) || (fDgree >= 270.f&& fDgree < 360.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_RIGHT, false, m_fSpeed);
		}
		break;
		case DIR_FL:
		{
			if ((fDgree >= 50.f&&fDgree <= 225.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
			else if ((fDgree <= 360.f && fDgree >= 225.f) || (fDgree >= 0.f&& fDgree < 40.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, false, m_fSpeed, Engine::INFO_RIGHT, true);
		}
		break;
		case DIR_BL:
		{
			if (fDgree >= 140.f&& fDgree <= 315.f)
				m_pTransformCom->Rotation(Engine::ROT_Y, +m_fRotSpeed* fTimeDelta);
			else if ((fDgree <= 360.f&&fDgree >= 315.f) || (fDgree >= 0.f&& fDgree < 130.f))
				m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);

			CheckMoveMesh(fTimeDelta, m_pCameraTransformCom, Engine::INFO_LOOK, true, m_fSpeed, Engine::INFO_RIGHT, true);
		}
		break;
		default:
			break;
		}
	}
}

void CPlayer::StateMachine()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case OBJ_IDLE:
		{
			m_fAnimSpeed = 1.0f;
			m_fRotSpeed = 6.f;
			m_pMeshCom->Set_AnimationSet(44);
		}
			break;
		case OBJ_WALK:
		{
			m_fAnimSpeed = 1.0f;
			m_fSpeed = 0.6f;
			m_fRotSpeed = 6.f;

			//if (m_bIsLockOn)
			//{
			//	switch (m_dwDirectionFlag)
			//	{
			//	case DIR_F:
			//		m_pMeshCom->Set_AnimationSet(42);
			//		break;
			//	case DIR_R:
			//		m_pMeshCom->Set_AnimationSet(40);
			//		break;
			//	case DIR_FR:
			//		m_pMeshCom->Set_AnimationSet(40);
			//		break;
			//	case DIR_B:
			//		m_pMeshCom->Set_AnimationSet(43);
			//		break;
			//	case DIR_BR:
			//		m_pMeshCom->Set_AnimationSet(40);
			//		break;
			//	case DIR_L:
			//		m_pMeshCom->Set_AnimationSet(41);
			//		break;
			//	case DIR_FL:
			//		m_pMeshCom->Set_AnimationSet(41);
			//		break;
			//	case DIR_BL:
			//		m_pMeshCom->Set_AnimationSet(41);
			//		break;
			//	default:
			//		break;
			//	}
			//}
			//else
			{
				m_pMeshCom->Set_AnimationSet(42);
			}
		}
			break;
		case OBJ_RUN:
		{	
			m_fAnimSpeed = 1.0f;

			m_fSpeed = 3.5f;
			m_fRotSpeed = 10.f;

			if (m_bIsLockOn)
			{
				switch (m_dwDirectionFlag)
				{
				case DIR_F:
					m_pMeshCom->Set_AnimationSet(37);
					break;
				case DIR_R:
					m_pMeshCom->Set_AnimationSet(33);
					break;
				case DIR_FR:
					m_pMeshCom->Set_AnimationSet(33);
					break;
				case DIR_B:
					m_pMeshCom->Set_AnimationSet(39);
					break;
				case DIR_BR:
					m_pMeshCom->Set_AnimationSet(33);
					break;
				case DIR_L:
					m_pMeshCom->Set_AnimationSet(35);
					break;
				case DIR_FL:
					m_pMeshCom->Set_AnimationSet(35);
					break;
				case DIR_BL:
					m_pMeshCom->Set_AnimationSet(35);
					break;
				default:
					break;
				}
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(37);

			}
		}
			break;
		case OBJ_DODGE:
		{
			m_fAnimSpeed = 2.25f;
			m_fSpeed = 10.f;
			m_fRotSpeed = 10.f;

			if (m_bIsLockOn)
			{
				switch (m_dwDirectionFlag)
				{
				case DIR_F:
					m_pMeshCom->Set_AnimationSet(21);
					break;
				case DIR_R:
					m_pMeshCom->Set_AnimationSet(19);
					break;
				case DIR_FR:
					m_pMeshCom->Set_AnimationSet(19);
					break;
				case DIR_B:
					m_pMeshCom->Set_AnimationSet(22);
					break;
				case DIR_BR:
					m_pMeshCom->Set_AnimationSet(19);
					break;
				case DIR_L:
					m_pMeshCom->Set_AnimationSet(20);
					break;
				case DIR_FL:
					m_pMeshCom->Set_AnimationSet(20);
					break;
				case DIR_BL:
					m_pMeshCom->Set_AnimationSet(20);
					break;
				default:
					break;
				}
			}
			else
			{
				m_pMeshCom->Set_AnimationSet(21);
			}
		}
			break;
		case OBJ_JUMP:
			break;
		case OBJ_FALL:
			break;
		case OBJ_ATTACK:
		{
			m_fAnimSpeed = 1.75f;
		}
			break;
		case OBJ_STRONG_ATTACK:
		{
			m_fAnimSpeed = 2.f;

			m_pMeshCom->Set_AnimationSet(26);
			m_fChargeTime = 0.f;

		}
			break;
		case OBJ_CHARGE_ATTACK:
		{
			m_fAnimSpeed = 2.f;
			m_pMeshCom->Set_AnimationSet(25);
			m_pMeshCom->Set_AddTrackTime(m_fChargeTime+0.3f); //0.3f <-다음 Animation  보정값 
			m_fChargeTime = 0.f;
		}
			break;
		case OBJ_GUARD:
			m_pMeshCom->Set_AnimationSet(16);
			break;
		case OBJ_HURT:
			break;
		case OBJ_DEAD:
			break;
		case OBJ_START:
			break;
		case OBJ_LOCK_WALK_F:
		{
			m_fSpeed = 1.5f;
			m_fAnimSpeed = 1.5f;
			m_pMeshCom->Set_AnimationSet(42);
		}
			break;
		case OBJ_LOCK_WALK_FR:
		case OBJ_LOCK_WALK_R:
		case OBJ_LOCK_WALK_BR:
		{
			m_fSpeed = 1.5f;
			m_fAnimSpeed = 1.5f;

			m_pMeshCom->Set_AnimationSet(40);
		}
			break;

		case OBJ_LOCK_WALK_B:
		{
			m_fSpeed = 1.5f;
			m_fAnimSpeed = 1.5f;
			m_pMeshCom->Set_AnimationSet(43);
		}
			break;
		case OBJ_LOCK_WALK_BL:
		case OBJ_LOCK_WALK_L:
		case OBJ_LOCK_WALK_FL:
		{	
			m_fSpeed = 1.5f;
			m_fAnimSpeed = 1.5f;
			m_pMeshCom->Set_AnimationSet(41);
		}
		break;
		case OBJ_END:
			break;
		default:
			break;
		}
		m_ePreState = m_eCurState;

	}

}

void CPlayer::IdleOption()
{
	if (m_eCurState == OBJ_DODGE)
	{
		if (m_pMeshCom->Get_TrackPosition() >= 1.0)
			m_dwDodge_DirectionFlag = 0;
		if (m_pMeshCom->Get_TrackPosition() >= 2.26)
			m_eCurState = OBJ_IDLE;
	}
	else if (m_eCurState != OBJ_IDLE )
	{
		if (m_pMeshCom->Is_AnimationSetEnd())
		{
			m_eCurState = OBJ_IDLE;
			m_uiCombo = 0;
		}
	}

}

void CPlayer::CheckMoveMesh(_float fTimeDelta, Engine::CTransform * pTransform, Engine::INFO eDir, _bool bIsDirRvc, _float fSpeed, Engine::INFO eDir2, _bool bIsDir2Rvc)
{

	_vec3	vPos, vDir, vDiagonalDir, vOutPos;
	
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	pTransform->Get_Info(eDir, &vDir);
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	if (eDir2 == Engine::INFO::INFO_END)
	{

		if (bIsDirRvc)
			m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * fSpeed* fTimeDelta), &vOutPos);
		else
			m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir * fSpeed* fTimeDelta), &vOutPos);
		
	}
	else
	{
		pTransform->Get_Info(eDir2, &vDiagonalDir);
		D3DXVec3Normalize(&vDiagonalDir, &vDiagonalDir);
		if (!bIsDirRvc)
			vDir *= -1.f;
		if (bIsDir2Rvc)
			vDiagonalDir *= -1.f;

		vDir += vDiagonalDir;
	
		D3DXVec3Normalize(&vDir, &vDir);

		m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * fSpeed* fTimeDelta), &vOutPos);

	}

	m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);

}

void CPlayer::CheckMoveMesh(_float fTimeDelta, _vec3 vDir, _bool bIsDirRvc, _float fSpeed)
{

	_vec3	vPos, vTempDir, vDiagonalDir, vOutPos;
	m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);

	vTempDir = vDir;
	vTempDir.y = 0.f;
	D3DXVec3Normalize(&vTempDir, &vTempDir);

	m_pNaviCom->Move_OnNaviMesh(&vPos, &(vTempDir* fSpeed* fTimeDelta), &vOutPos);

	m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);
	
	
	
	
	//m_pTransformCom->Get_Info(Engine::INFO_POS, &vPos);
	//pTransform->Get_Info(eDir, &vDir);
	//vDir.y = 0.f;
	//D3DXVec3Normalize(&vDir, &vDir);
	//if (eDir2 == Engine::INFO::INFO_END)
	//{

	//	if (bIsDirRvc)
	//	else
	//		m_pNaviCom->Move_OnNaviMesh(&vPos, &(-vDir * fSpeed* fTimeDelta), &vOutPos);

	//}
	//else
	//{
	//	pTransform->Get_Info(eDir2, &vDiagonalDir);
	//	D3DXVec3Normalize(&vDiagonalDir, &vDiagonalDir);
	//	if (!bIsDirRvc)
	//		vDir *= -1.f;
	//	if (bIsDir2Rvc)
	//		vDiagonalDir *= -1.f;

	//	vDir += vDiagonalDir;

	//	D3DXVec3Normalize(&vDir, &vDir);

	//	m_pNaviCom->Move_OnNaviMesh(&vPos, &(vDir * fSpeed* fTimeDelta), &vOutPos);

	//}

	//m_pTransformCom->Set_Pos(vOutPos.x, vOutPos.y, vOutPos.z);

}

void CPlayer::AttackMoveSet(_float fTimeDelta)
{
	if (m_eCurState == OBJ_ATTACK)
	{
		switch (m_uiCombo)
		{
		case 1:
			MoveAni(fTimeDelta, 0.1f, 0.15f, 4.f, _vec3{ 1.f,1.f,1.f });
			break;
		case 2:
			MoveAni(fTimeDelta, 0.1f, 0.2f, 4.f, _vec3{ 1.f,1.f,1.f });
			break;
		case 3:
			MoveAni(fTimeDelta, 0.18f, 0.3f, 3.f, _vec3{ 1.f,1.f,1.f });
			break;
		case 4:
			MoveAni(fTimeDelta, 0.11f, 0.2f, 3.f, _vec3{ 1.f,1.f,1.f });

			break;
		case 5:
			MoveAni(fTimeDelta, 0.01f, 0.14f, 5.0f, _vec3{ 1.f,1.f,1.f });
			break;
		default:
			break;
		}
	}

}

void CPlayer::StorngAttackMoveSet(_float fTimeDelta)
{
	if (m_eCurState == OBJ_STRONG_ATTACK)
	{
		MoveAni(fTimeDelta, 0.0f, 0.025f, 5.0f, _vec3{ 1.f,1.f,1.f });
		MoveAni(fTimeDelta, 0.25f, 0.285f, 5.0f, _vec3{ 1.f,1.f,1.f });

	}
}

void CPlayer::ChargeAttackMoveSet(_float fTimeDelta)
{
	if (m_eCurState == OBJ_CHARGE_ATTACK)
	{
		MoveAni(fTimeDelta, 0.15f, 0.2f, 4.0f, _vec3{ 1.f,1.f,1.f });

		MoveAni(fTimeDelta, 0.2f, 0.26f, 8.0f, _vec3{ 1.f,1.f,1.f });
	}

}

void CPlayer::MoveAni(_float fTimeDelta, _float fMinRatio, _float fMaxRatio, _float fSpeed, _vec3 vDir)
{
	_float fCurRatio = m_pMeshCom->Get_TrackPosition() / m_pMeshCom->Get_Period();
	if (fCurRatio >= fMinRatio && fCurRatio <= fMaxRatio)
	{
		CheckMoveMesh(fTimeDelta, m_pTransformCom, Engine::INFO_LOOK, true, fSpeed);
	}
}

_float CPlayer::Get_AngleOnTheLook()
{
	_vec3	vPos, vDir, vDiagonalDir, vOutPos, vLook;
	_float	fDgree;
	vLook = *m_pTransformCom->Get_Info(Engine::INFO_LOOK);
	vDir = m_pCam->Get_LockOnLook();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	fDgree = Get_Angle(vDir, vLook);

	return fDgree;
}

void CPlayer::RotateToLook(_float fTimeDelta)
{
	_vec3 vRight, vCamLook;
	_float fCurRatio, fDgree, fRotateDir;
	vRight = *m_pTransformCom->Get_Info(Engine::INFO_RIGHT);
	vRight.y = 0.f;
	D3DXVec3Normalize(&vRight, &vRight);

	vCamLook = m_pCam->Get_LockOnLook();
	vCamLook.y = 0.f;
	D3DXVec3Normalize(&vCamLook, &vCamLook);



	fDgree = Get_AngleOnTheLook(); //캐릭터 룩과 카메라의 룩 각도 

	fRotateDir = Get_Angle(vCamLook, vRight);  //회전방향 결정


	if (cosf(D3DXToRadian(fDgree)) >= 0.97f)
		return;
	else
	{
		if (cosf(D3DXToRadian(fRotateDir)) >= 0.f)
			m_pTransformCom->Rotation(Engine::ROT_Y, m_fRotSpeed* fTimeDelta);
		else
			m_pTransformCom->Rotation(Engine::ROT_Y, -m_fRotSpeed* fTimeDelta);
	}

}

_bool CPlayer::CheckEnableState()
{
	if (m_eCurState != OBJ_ATTACK&& m_eCurState != OBJ_GUARD&&m_eCurState != OBJ_STRONG_ATTACK&&
			m_eCurState != OBJ_CHARGE_ATTACK&&m_eCurState != OBJ_HURT && m_eCurState != OBJ_DODGE)
	{
		return true;
	}
	else
		return false;

}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint uiIdx , _uint uiStageIdx)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev, uiIdx, uiStageIdx);

	if (FAILED(pInstance->Ready_GameObject()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free(void)
{
	Engine::CGameObject::Free();
}

float CPlayer::Get_Angle(const D3DXVECTOR3& a, const D3DXVECTOR3& b)
{
	float fRadian = acosf(D3DXVec3Dot(&a, &b) / (D3DXVec3Length(&a) * D3DXVec3Length(&b)));

	fRadian = D3DXToDegree(fRadian);
	float fDgree = (a.x * b.z - a.z * b.x > 0.0f) ? fRadian : -fRadian;

	return fDgree;

}
