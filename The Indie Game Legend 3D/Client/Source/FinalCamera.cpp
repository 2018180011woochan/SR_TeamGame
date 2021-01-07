#include "stdafx.h"
#include "..\Header\FinalCamera.h"


CFinalCamera::CFinalCamera()
{
}

CFinalCamera::CFinalCamera(const CFinalCamera & _rOther)
	: CCamera(_rOther)
{
}

void CFinalCamera::Free()
{
	CCamera::Free();
}

CFinalCamera * CFinalCamera::Create()
{
	CFinalCamera* pInstance = new CFinalCamera;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CFinalCamera::Clone()
{
	CFinalCamera* pClone = new CFinalCamera(*this);
	return pClone;
}

HRESULT CFinalCamera::InitializePrototype()
{
	CCamera::InitializePrototype();
	return S_OK;
}

HRESULT CFinalCamera::Awake()
{
	CCamera::Awake();
	m_tCameraDesc.fAspect = (float)g_nWinCX / g_nWinCY;
	m_tCameraDesc.fFar = 500.f;
	m_tCameraDesc.fNear = 1.f;
	m_tCameraDesc.fFovY = D3DXToRadian(90.f);
	m_tCameraDesc.vUp = _vector(0.f, 0.f, 1.f);
	m_tCameraDesc.vAt = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_tCameraDesc.vEye = D3DXVECTOR3(0.f, 25.f, 0.f);

	return S_OK;
}

HRESULT CFinalCamera::Start()
{
	CCamera::Start();
	return S_OK;
}

UINT CFinalCamera::Update(const float _fDeltaTime)
{
	CCamera::Update(_fDeltaTime);
	return 0;
}

UINT CFinalCamera::LateUpdate(const float _fDeltaTime)
{
	CCamera::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CFinalCamera::Render()
{
	return S_OK;
}

