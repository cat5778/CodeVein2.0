#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit	CTransform(void);
	virtual		~CTransform(void);

public:
	void			Get_Info(INFO eType, _vec3* pInfo);
	const _vec3*	Get_Info(INFO eType);
	void			Move_Pos(const _vec3* const pDir);
	void			Move_TargetPos(const _vec3* pPos, const _float& fSpeed, const _float& fTimeDelta);
	void			Rotation(ROTATION eType, const _float& fAngle);

	const	_matrix*	Compute_LookAtTarget(const _vec3* pTargetPos);
	
	void				Chase_Target(const _vec3* pTargetPos,
										const _float& fSpeed);

	void			Set_Scale(const _float& fX, const _float& fY, const _float& fZ);
	void			Set_Pos(const _float& fX, const _float& fY, const _float& fZ);
	void			Set_Pos(const _vec3* pPos);
	void			Get_WorldMatrix(_matrix* pWorld) const;

	void			Set_ParentMatrix(const _matrix* pMatrix);
	void			Set_WorldMatrix(const _matrix* pMatrix);
	const _matrix*	Get_WorldMatrixPointer(void)const { return &m_matWorld; }

public:
	HRESULT				Ready_Component(void);
	virtual _int		Update_Component(const _float& fTimeDelta);

public:
	_vec3			m_vInfo[INFO_END];
	_vec3			m_vScale;
	_vec3			m_vAngle;
	_matrix			m_matWorld;

public:
	static CTransform*		Create(void);
private:
	virtual void	Free(void);

};
END
#endif // Transform_h__
