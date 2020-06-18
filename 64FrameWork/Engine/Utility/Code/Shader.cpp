#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pEffect(nullptr)
	, m_pErrMsg(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CShader::CShader(const CShader& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pEffect(rhs.m_pEffect)
	, m_pErrMsg(rhs.m_pErrMsg)
{
	m_pGraphicDev->AddRef();
	m_pEffect->AddRef();
}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath)
{
	// m_pEffect 값이 있고, m_pErrMsg 값이 없는 경우 : 정상적으로 쉐이더 파일이 작동하는 경우

	// m_pEffect 값이 없고, m_pErrMsg 값이 있는 경우 : 컴파일 에러
	// m_pEffect 값이 있고, m_pErrMsg 값이 있는 경우 : 경고


	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev,
		pFilePath,
		NULL,				// 쉐이더에서 사용할 매크로 파일
		NULL,				// 쉐이더에서 사용할 포함 파일
		D3DXSHADER_DEBUG,	// 쉐이더릴 디버깅하겠다는 옵션(디버깅 지원의 외부라이브러리를 포함해야하지만 만약을 대비해서 설정해 놓은 인자 값)
		NULL,				// 각종 리소스 정보를 어디에 보관할 것인가(null인 경우 그래픽 메모리에 보관 됨)
		&m_pEffect,			// 쉐이더 객체
		&m_pErrMsg)))		// 디버깅 불가능하지만 에러 및 경고가 있을 경우 저장하기 위한 메모리 공간
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Error", MB_OK);
		return E_FAIL;
	}

	else if (nullptr != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "Shader_Warning", MB_OK);
	}

	return S_OK;
}

Engine::CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CShader*	pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

Engine::CComponent* Engine::CShader::Clone(void)
{
	return new CShader(*this);
}

void Engine::CShader::Free(void)
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pGraphicDev);
}

