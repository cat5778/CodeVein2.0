#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Defines.h"
#include "Base.h"

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit							CGameObject(LPDIRECT3DDEVICE9	pGraphicDev);
	virtual								~CGameObject();
public:
	_float								Get_ViewZ(void) const { return m_fViewZ; }
	virtual void						Set_Enable(bool bEnable);
	virtual void						Set_ObjType(OBJECTTYPE objType);
	virtual void						Set_MonsterType(MONSTERTYPE monsterType);
	virtual void						Set_ItemType(ITEMTYPE itemType);
	virtual void						Set_ObjName(const _tchar* objName);
	_uint								Get_ObjIdx();
	void								Set_ObjIdx(_uint uiIDx);
public:
	bool								Get_Enable(void) const;
	OBJECTTYPE							Get_ObjType();
	MONSTERTYPE							Get_MonsterType();
	ITEMTYPE							Get_ItemType();
	wstring								Get_ObjName();
	CComponent*							Find_Component(const _tchar* pComponentTag, COMPONENTID eID);
	CComponent*							Get_Component(const _tchar* pComponentTag, COMPONENTID eID);
	wstring								Get_InstName();
	_bool								IsMonster() {return m_bIsMonster;}
public:
	virtual HRESULT						Ready_GameObject(void);
	virtual HRESULT						LateReady_GameObject(void);
	virtual _int						Update_GameObject(const _float& fTimeDelta);
	virtual _int						LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void						Render_GameObject(void);

public:
	void								Compute_ViewZ(const _vec3* pPos);

protected:
	void								ExtractY_NormalDir(_vec3 vDest, _vec3 vSour, _vec3* pOut);


protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<wstring, CComponent*>			m_pComponentMap[ID_END];
	_float								m_fViewZ;
	wstring								m_wstrInstName;
	_bool								m_bIsMonster = false;
	bool								m_bEnable=true;
	wstring								m_ObjName = L"";
	MONSTERTYPE							m_MonsterType;
	ITEMTYPE							m_ItemType;
	_uint								m_uiIdx=0;
	map<wstring, Engine::CGameObject*>*	m_ppGameObjectMap = nullptr;

public:
	virtual void					Free(void);


};

END
#endif // GameObject_h__
