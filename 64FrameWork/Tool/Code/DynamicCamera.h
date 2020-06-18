#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Defines.h"
#include "Camera.h"


BEGIN(Engine)
class CKeyMgr;
END

class CDynamicCamera : public Engine::CCamera
{
private:
	explicit					CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual						~CDynamicCamera(void);

public:
	HRESULT						Ready_GameObject(const _vec3* pEye,
												const _vec3* pAt,
												const _vec3* pUp,
												const _float& fFovY,
												const _float& fAspect,
												const _float& fNear,
												const _float& fFar, 
												const _bool& bMainCamera);

	virtual _int				Update_GameObject(const _float& fTimeDelta) override;
public:
	_vec3						Get_PickPos() { return m_vPickPos; }
	wstring						Get_PickName() { return m_wstrPickName; }
	void						Init_PickName();
	_int						Get_PickMode() { return m_iPickMode; }
	_bool						IsNavPick() { return m_bIsNavPick; }
	void						Set_PickMode(_int iPickMode) { m_iPickMode = iPickMode; }
	Engine::CTransform*			Get_PickTransform() {return m_pPickTransform;}
	_bool						IsPick() { return m_bIsPick; }


private:
	void						Key_Input(const _float& fTimeDelta);
	void						Mouse_Move(const _float& fTimeDelta);
	void						Mouse_Fix(void);
	void						Picking_Mesh();
	void						Pickint_Nav();

private :
	_int						m_iPickMode=0;
	_bool						m_bClick;
	_bool						m_bMouseFix;
	CKeyMgr*					m_pKeyMgr = nullptr;
	map<wstring, CGameObject*>*	m_pGameObjectMap=nullptr;
	Engine::CTransform*			m_pPickTransform = nullptr;
	_vec3						m_vPickPos = { 0.f, 0.f, 0.f };
	wstring						m_wstrPickName = L"No Pick";
	_bool						m_bIsPick=false;
	_bool						m_bIsNavPick = false;
	vector<Engine::CCell*>*		m_ppCellVec = nullptr;
	_matrix						m_matPickWorldNav;
public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
										const _vec3* pEye,
										const _vec3* pAt,
										const _vec3* pUp,
										const _float& fFovY,
										const _float& fAspect,
										const _float& fNear,
										const _float& fFar,
										const _bool& bMainCamera);

private:
	virtual void Free(void) override;

};

#endif // DynamicCamera_h__
