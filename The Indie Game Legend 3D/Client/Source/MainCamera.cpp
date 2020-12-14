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
	
	/* ī�޶�� �÷��̾ �ٶ󺸰� */
	m_tCameraDesc.vAt = m_pPlayerTransform->Get_Position();

	/* ī�޶�� �����Ÿ���ŭ �÷��̾� �ڿ� ��ġ */
	_vector vPlayerLook;
	memcpy(&vPlayerLook, &m_pPlayerTransform->Get_WorldMatrix().m[2][0], sizeof(_vector));
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	/* ī�޶�� �÷��̾� ����� �ݴ뿡 �������־���� */
	vPlayerLook *= -m_fDistanceToTarget;

	/* vPlayerLook�� �÷��̾� Right�� �������� ȸ�� */
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
