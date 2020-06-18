#ifndef Component_h__
#define Component_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent(void);
	virtual ~CComponent(void);

public:
	virtual _int		Update_Component(const _float& fTimeDelta) {	return 0;	}

public:
	virtual CComponent*		Clone(void) { return nullptr; }

protected:
	virtual void	Free(void)PURE;
};

END
#endif // Component_h__
