#ifndef PrototypeMgr_h__
#define PrototypeMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CPrototypeMgr : public CBase
{
	DECLARE_SINGLETON(CPrototypeMgr)

private:
	explicit	CPrototypeMgr(void);
	virtual		~CPrototypeMgr(void);

public:
	HRESULT			Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
	CComponent*		Clone_Prototype(const _tchar* pProtoTag);

private:
	CComponent*		Find_Prototype(const _tchar* pProtoTag);

private:
	map<const _tchar*, CComponent*>			m_mapPrototype;
public:
	virtual void	Free(void);
};

END
#endif // PrototypeMgr_h__
