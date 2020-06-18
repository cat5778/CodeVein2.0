#include "Pipeline.h"

USING(Engine)

Engine::CPipeline::CPipeline(void)
{

}

Engine::CPipeline::~CPipeline(void)
{

}

void Engine::CPipeline::MakeTransformMatrix(_matrix* pMatrix, 
											const _vec3* pRight, 
											const _vec3* pUp, 
											const _vec3* pLook, 
											const _vec3* pPos)
{
	memcpy(&pMatrix->m[0][0], pRight, sizeof(_float) * 3);
	memcpy(&pMatrix->m[1][0], pUp, sizeof(_float) * 3);
	memcpy(&pMatrix->m[2][0], pLook, sizeof(_float) * 3);
	memcpy(&pMatrix->m[3][0], pPos, sizeof(_float) * 3);

}

void Engine::CPipeline::MakeProjectionMatrix(_matrix* pMatrix, 
											const _float& fFovY, 
											const _float& fAspect, 
											const _float& fNear,
											const _float& fFar)
{
	D3DXMatrixIdentity(pMatrix);

	pMatrix->_11 = (1.f / tanf(fFovY / 2.f)) / fAspect;
	pMatrix->_22 = 1.f / tanf(fFovY / 2.f);

	pMatrix->_33 = fFar / (fFar - fNear);
	pMatrix->_34 = 1.f;

	pMatrix->_43 = (-fNear * fFar) / (fFar - fNear);
	pMatrix->_44 = 0.f; 
}

void CPipeline::MyRotationX(_vec3 * pOut, _vec3 * pIn, _float fAngle)
{
								/*

								1	0	0	0
		(x,y,z,w)		*		0	C	S	0
								0	-S	C	0
								0	0	0	1
	
								*/

	_vec3	vTemp = *pIn;

	pOut->x = vTemp.x;
	pOut->y = vTemp.y * cosf(fAngle) - vTemp.z * sinf(fAngle);
	pOut->z = vTemp.y * sinf(fAngle) + vTemp.z * cosf(fAngle);
}

void CPipeline::MyRotationY(_vec3 * pOut, _vec3 * pIn, _float fAngle)
{
	/*

							c	0	-s	0
	(x,y,z,w)		*		0	1	0	0
							s	0	c	0
							0	0	0	1

	*/

	_vec3	vTemp = *pIn;

	pOut->x = vTemp.x * cosf(fAngle) + vTemp.z * sinf(fAngle);
	pOut->y = vTemp.y;
	pOut->z = vTemp.x * -sinf(fAngle) + vTemp.z * cosf(fAngle);
}

void CPipeline::MyRotationZ(_vec3 * pOut, _vec3 * pIn, _float fAngle)
{
	/*

							c	s	0	0
	(x,y,z,w)		*		-s	c	0	0
							0	0	1	0
							0	0	0	1
	*/
	_vec3	vTemp = *pIn;

	pOut->x = vTemp.x * cosf(fAngle) - vTemp.y * sinf(fAngle);
	pOut->y = vTemp.x * sinf(fAngle) + vTemp.y * cosf(fAngle);
	pOut->z = vTemp.z;
}

void Engine::CPipeline::MyTransformCoord(_vec3* pOut, const _vec3* pVector, const _matrix* pMatrix)
{
	_vec4		vVector(pVector->x, pVector->y, pVector->z, 1.f);

	pOut->x = vVector.x * pMatrix->_11 + vVector.y * pMatrix->_21 + vVector.z * pMatrix->_31 + vVector.w * pMatrix->_41;
	pOut->y = vVector.x * pMatrix->_12 + vVector.y * pMatrix->_22 + vVector.z * pMatrix->_32 + vVector.w * pMatrix->_42;
	pOut->z = vVector.x * pMatrix->_13 + vVector.y * pMatrix->_23 + vVector.z * pMatrix->_33 + vVector.w * pMatrix->_43;

	_float	fZ = vVector.x * pMatrix->_14 + vVector.y * pMatrix->_24 + vVector.z * pMatrix->_34 + vVector.w * pMatrix->_44;

	pOut->x /= fZ;
	pOut->y /= fZ;
	pOut->z /= fZ;
}

void Engine::CPipeline::MakeWorldMatrix(_matrix* pMatrix, 
										const _vec3* pScale, 
										const _float* pAngle, 
										const _vec3* pPos)
{
	D3DXMatrixIdentity(pMatrix);

	_vec3	vRight	= _vec3(1.f, 0.f, 0.f);
	_vec3	vUp		= _vec3(0.f, 1.f, 0.f);
	_vec3	vLook	= _vec3(0.f, 0.f, 1.f);
	_vec3	vPos	= _vec3(0.f, 0.f, 0.f);

	// scale
	vRight	*= pScale->x;
	vUp		*= pScale->y;
	vLook	*= pScale->z;

	// Rotation
	MyRotationX(&vRight, &vRight, pAngle[ANGLE_X]);
	MyRotationX(&vUp, &vUp, pAngle[ANGLE_X]);
	MyRotationX(&vLook, &vLook, pAngle[ANGLE_X]);

	MyRotationY(&vRight, &vRight, pAngle[ANGLE_Y]);
	MyRotationY(&vUp, &vUp, pAngle[ANGLE_Y]);
	MyRotationY(&vLook, &vLook, pAngle[ANGLE_Y]);

	MyRotationZ(&vRight, &vRight, pAngle[ANGLE_Z]);
	MyRotationZ(&vUp, &vUp, pAngle[ANGLE_Z]);
	MyRotationZ(&vLook, &vLook, pAngle[ANGLE_Z]);

	vPos = *pPos;

	MakeTransformMatrix(pMatrix, &vRight, &vUp, &vLook, &vPos);
}

void Engine::CPipeline::MakeViewSpaceMatrix(_matrix* pMatrix,
											const _vec3* pEye, 
											const _vec3* pAt, 
											const _vec3* pUp)
{
	D3DXMatrixIdentity(pMatrix);

	_vec3	vRight	= _vec3(1.f, 0.f, 0.f);
	_vec3	vUp		= _vec3(0.f, 1.f, 0.f);
	_vec3	vLook	= _vec3(0.f, 0.f, 1.f);
	_vec3	vPos	= _vec3(0.f, 0.f, 0.f);

	// look
	vLook = *pAt - *pEye;
	D3DXVec3Normalize(&vLook, &vLook);

	// right
	D3DXVec3Cross(&vRight, pUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);

	// vUp
	D3DXVec3Cross(&vUp, &vLook, &vRight);
	D3DXVec3Normalize(&vUp, &vUp);

	// vPos
	vPos = *pEye;

	// 카메라 월드 행렬
	MakeTransformMatrix(pMatrix, &vRight, &vUp, &vLook, &vPos);

	// 뷰스페이스 변환 행렬
	D3DXMatrixInverse(pMatrix, 0, pMatrix);
}

