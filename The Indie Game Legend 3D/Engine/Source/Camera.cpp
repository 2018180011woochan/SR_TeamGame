#include "Camera.h"

USING(Engine)
CCamera::CCamera()
	:CGameObject()
{
	ZeroMemory(&m_tCameraDesc, sizeof(CAMERA_DESC));
	D3DXMatrixIdentity(&m_tCameraDesc.matView);
	D3DXMatrixIdentity(&m_tCameraDesc.matProj);
}

CCamera::CCamera(const CCamera & _rOther)
	:CGameObject(_rOther)
	,m_tCameraDesc(_rOther.m_tCameraDesc)
{
}

HRESULT CCamera::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Start()
{
	if (FAILED(CGameObject::Start()))
		return E_FAIL;

	return S_OK;
}

UINT CCamera::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	return _uint();
}

UINT CCamera::LateUpdate(const float _fDeltaTime)
{
	D3DXMatrixLookAtLH(&m_tCameraDesc.matView, &m_tCameraDesc.vEye,
		&m_tCameraDesc.vAt, &m_tCameraDesc.vUp);

	D3DXMatrixPerspectiveFovLH(&m_tCameraDesc.matProj, m_tCameraDesc.fFovY,
		m_tCameraDesc.fAspect, m_tCameraDesc.fNear, m_tCameraDesc.fFar);

	if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, &m_tCameraDesc.matView)))
		return 0;

	if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, &m_tCameraDesc.matProj)))
		return 0;

	return _uint();
}

HRESULT CCamera::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	return S_OK;
}

void CCamera::Free()
{
	CGameObject::Free();
}

void CCamera::Set_Camera(CAMERA_DESC _CameraDesc)
{
	m_tCameraDesc = _CameraDesc;
}
