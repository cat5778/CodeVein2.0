#ifndef Stage_h__
#define Stage_h__

#include "Defines.h"
#include "Scene.h"
#include "ToolView.h"

#include "DynamicCamera.h"
#include "ToolCamera.h"
class CNaviMesh;
class CDynamicCamera;
class CTestStage : public Engine::CScene
{
private:
	explicit CTestStage(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestStage(void);


public:
	virtual HRESULT Ready_Scene(void) override;
	virtual HRESULT LateReady_Scene(void) override;
	virtual _int Update_Scene(const _float& fTimeDelta) override;
	virtual void Render_Scene(void) override;

private:
	HRESULT		Ready_Environment_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT		Ready_GameLogic_Layer(const _tchar* pLayerTag);
	HRESULT		Ready_UI_Layer(const _tchar* pLayerTag) { return S_OK; }
	HRESULT		Ready_Resource(LPDIRECT3DDEVICE9& pGraphicDev, RESOURCEID eMax);

public:
	HRESULT     Add_StaticObject(wstring wstrObjName, TRANSFORM_INFO tInfo);
	void		Set_MeshVec(vector<NavMesh*>& pMeshVec);
//	HRESULT     Copy_GameObject(Engine::CGameObject* object, const _tchar* pObjName);
//	HRESULT     Copy_GameObject(Engine::CGameObject* object,const _tchar* pObjName,_vec3 tempPos);
//
//
//	HRESULT     Add_ColObject(const _tchar * pObjName);
//
//	void        Save(const TCHAR * pFilePath);
//	void        Load(const TCHAR * pFilePath,  int &count);
//	void        Col_Save(const TCHAR * pFilePath);
//	void        Col_Load(const TCHAR * pFilePath, int &count);
//	HRESULT     Delete_Object(const _tchar* pObjectName, int &count);
//	HRESULT     Delete_Col_Object(const _tchar* pObjectName, int &count);
private:
	_matrix							m_matView, m_matProj;
	Engine::CLayer*					m_pLayer;
	Engine::CLayer*					m_pColLayer;
	//Engine::CKeyMgr*				m_pKeyMgr = nullptr;
	vector<NAV_MESH*>				m_pNavMeshVec;
	map<wstring, TRANSFORM_INFO>	m_StaticObjMap;
	D3DXVECTOR3						m_vOrigin;
	D3DXVECTOR3						m_vDirection;
	
private: //Tool Debug
	_tchar							m_szFPS[256] = L"FPS =";
	_tchar							m_szMousePos[256] = L"";
	_tchar							m_szPickPos[256] = L"";
	_tchar							m_szPickObjName[256] = L"";

	_float							m_fTime = 0;
	_ulong							m_dwRenderCnt = 0;
	CDynamicCamera*					m_pCamera;
	list<NAV_MESH>					m_Navlist;
	CNaviMesh*						m_pNavMesh;

//	

public:
	static CTestStage*			Create(LPDIRECT3DDEVICE9 pGraphicDev,CToolView* m_pView);
private:
	CToolView*					m_pView;
private:
	virtual void			Free(void) override;
};


#endif // Stage_h__
