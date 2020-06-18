#ifndef Pipeline_h__
#define Pipeline_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CPipeline
{
private:
	explicit CPipeline(void);
	~CPipeline(void);

public:
	static void MakeWorldMatrix(_matrix* pMatrix, 
									const _vec3* pScale,
									const _float* pAngle,
									const _vec3* pPos);

	static void MakeViewSpaceMatrix(_matrix* pMatrix,
									const _vec3* pEye,
									const _vec3* pAt,
									const _vec3* pUp);

	static void	MakeTransformMatrix(_matrix* pMatrix,
									const _vec3* pRight,
									const _vec3* pUp,
									const _vec3* pLook,
									const _vec3* pPos);

	static void	MakeProjectionMatrix(_matrix* pMatrix,
									const _float& fFovY,
									const _float& fAspect,
									const _float& fNear,
									const _float& fFar);

	static void MyRotationX(_vec3* pOut, _vec3* pIn, _float fAngle);
	static void MyRotationY(_vec3* pOut, _vec3* pIn, _float fAngle);
	static void MyRotationZ(_vec3* pOut, _vec3* pIn, _float fAngle);

	static void MyTransformCoord(_vec3* pOut, const _vec3* pVector, const _matrix* pMatrix);

};


END
#endif // Pipeline_h__
