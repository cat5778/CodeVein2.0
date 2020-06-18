#ifndef Line_h__
#define Line_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLine : public CBase
{
public:
	enum POINT		{ POINT_START, POINT_FINISH, POINT_END };
	enum COMPARE	{ COMPARE_LEFT, COMPARE_RIGHT };

private:
	explicit CLine(void);
	virtual ~CLine(void);

public:
	HRESULT			Ready_Line(const _vec3* pPointA, const _vec3* pPointB);
	COMPARE			Compare(const _vec3* pEndPos);

private:
	_vec3			m_vPoint[POINT_END];
	_vec3			m_vDirection;
	_vec3			m_vNormal;

public:
	static CLine*		Create(const _vec3* pPointA, const _vec3* pPointB);
	virtual void		Free(void);
};

END
#endif // Line_h__
