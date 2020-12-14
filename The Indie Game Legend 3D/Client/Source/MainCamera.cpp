#include "stdafx.h"
#include "..\Header\MainCamera.h"
#include "Player.h"

CMainCamera::CMainCamera()
{
}

CMainCamera::CMainCamera(const CMainCamera & other)
	: CCamera(other)
	, m_fDistanceToTarget(other.m_fDistanceToTarget)
	, m_fCameraAngle(other.m_fCameraAngle)
{
}

HRESULT CMainCamera::InitializePrototype()
{
	if (FAILED(CCamera::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainCamera::Awake()
{
	if (FAILED(CCamera::Awake()))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainCamera::Start()
{
	if (FAILED(CCamera::Start()))
		return E_FAIL;

	return S_OK;
}

UINT CMainCamera::Update(const float _fDeltaTime)
{
	CCamera::Update(_fDeltaTime);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	return _uint();
}

UINT CMainCamera::LateUpdate(const float _fDeltaTime)
{
	CCamera::LateUpdate(_fDeltaTime);

	return _uint();
}

HRESULT CMainCamera::Render()
{
	if (FAILED(CCamera::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainCamera::Movement(float fDeltaTime)
{
	CTransform* m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());
	
	/* 카메라는 플레이어를 바라보게 */
	m_tCameraDesc.vAt = m_pPlayerTransform->Get_Position();

	/* 카메라는 일정거리만큼 플레이어 뒤에 배치 */
	_vector vPlayerLook;
	memcpy(&vPlayerLook, &m_pPlayerTransform->Get_WorldMatrix().m[2][0], sizeof(_vector));
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	/* 카메라는 플레이어 방향과 반대에 떨어져있어야함 */
	vPlayerLook *= -m_fDistanceToTarget;

	/* vPlayerLook을 플레이어 Right축 기준으로 회전 */
	_vector vPlayerRight;
	memcpy(&vPlayerRight, &m_pPlayerTransform->Get_WorldMatrix().m[0][0], sizeof(_vector));
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	_matrix matRotAxis;

	D3DXMatrixRotationAxis(&matRotAxis, &vPlayerRight, D3DXToRadian(m_fCameraAngle));
	D3DXVec3TransformNormal(&vPlayerLook, &vPlayerLook, &matRotAxis);

	m_tCameraDesc.vEye = m_tCameraDesc.vAt + vPlayerLook;
	//m_tCameraDesc.vUp = _vector(0.f, 1.f, 0.f);
	

	return S_OK;
}

CMainCamera * CMainCamera::Create()
{
	CMainCamera* pInstance = new CMainCamera;

	return pInstance;
}

CGameObject * CMainCamera::Clone()
{
	CMainCamera* pClone = new CMainCamera(*this);

	return pClone;
}

void CMainCamera::Free()
{
	CCamera::Free();
}
