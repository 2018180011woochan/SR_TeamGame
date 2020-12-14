#include "stdafx.h"
#include "PlayerCamera.h"
#include "Mouse.h"
#include "Player.h"
#include "Transform.h"

/*카메라 세팅*/
HRESULT CPlayerCamera::UpdateMove(float _fDeletaTime)
{
	if (nullptr == m_pMouse)
	{
		PrintLog(L"Error", L"pMouse nullptr");
		return E_FAIL;
	}
	if (nullptr == m_pPlayer)
	{
		PrintLog(L"Error", L"m_pPlayer nullptr");
		return E_FAIL;
	}

	m_fCameraAngleX += m_pMouse->Get_MouseDir().y  * m_fCameraAngleSpeed * _fDeletaTime;

	CTransform* pPlayerTransFomr = (CTransform*)(m_pPlayer->GetComponent<CTransform>());
	TRANSFORM_DESC PlayerTransFormDesc = pPlayerTransFomr->Get_TransformDesc();
	_vector vPlayerLook;
	_vector vPlayerRight;
	_matrix matRotAxis;
	/*Eye*/
	m_tCameraDesc.vEye = pPlayerTransFomr->Get_Position();
	m_tCameraDesc.vEye.y += 3.f;

	/*At*/
	memcpy(&vPlayerLook, &PlayerTransFormDesc.matWorld.m[2][0], sizeof(_vector));
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	vPlayerLook *= m_fDistanceToAt;

	memcpy(&vPlayerRight, &PlayerTransFormDesc.matWorld.m[0][0], sizeof(_vector));
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	/* 임의의 축 기준으로 회전 */
	D3DXMatrixRotationAxis(&matRotAxis, &vPlayerRight, D3DXToRadian(m_fCameraAngleX));
	D3DXVec3TransformNormal(&vPlayerLook, &vPlayerLook, &matRotAxis);

	m_tCameraDesc.vAt = m_tCameraDesc.vEye + vPlayerLook;

	return E_NOTIMPL;
}

CPlayerCamera::CPlayerCamera()
	:CCamera()
{
}

CPlayerCamera::CPlayerCamera(const CPlayerCamera & _rOther)
	:CCamera()
	,m_fDistanceToAt(_rOther.m_fDistanceToAt)
	,m_fCameraAngleX(_rOther.m_fCameraAngleX)
	,m_fCameraAngleSpeed(_rOther.m_fCameraAngleSpeed)
	,m_pPlayer(_rOther.m_pPlayer)
	,m_pMouse(_rOther.m_pMouse)
{
}


HRESULT CPlayerCamera::InitializePrototype()
{
	m_fDistanceToAt = 0.f;
	m_fCameraAngleX = 0.f;
	m_fCameraAngleSpeed = 0.f;
	m_pPlayer = nullptr;
	m_pMouse = nullptr;
	return S_OK;
}

HRESULT CPlayerCamera::Awake()
{
	m_fDistanceToAt = 10.f;
	m_fCameraAngleX = 0.f;
	m_fCameraAngleSpeed = 90.f;
	return S_OK;
}

HRESULT CPlayerCamera::Start()
{
	m_pPlayer = (CPlayer*)FindGameObjectOfType<CPlayer>();
	SafeAddRef(m_pPlayer);
	m_pMouse = (CMouse*)FindGameObjectOfType<CMouse>();
	SafeAddRef(m_pMouse);
	return S_OK;
}

UINT CPlayerCamera::Update(const float _fDeltaTime)
{
	if (FAILED(UpdateMove(_fDeltaTime)))
		return OBJ_NOENVET;

	return OBJ_NOENVET;
}

UINT CPlayerCamera::LateUpdate(const float _fDeltaTime)
{
	CCamera::LateUpdate(_fDeltaTime);

	return OBJ_NOENVET;
}

HRESULT CPlayerCamera::Render()
{
	return S_OK	;
}

CGameObject * CPlayerCamera::Clone()
{
	CPlayerCamera* pInstance = new CPlayerCamera(*this);
	return pInstance;
}

CPlayerCamera * CPlayerCamera::Create()
{
	CPlayerCamera* pInstance = new CPlayerCamera();
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		PrintLog(L"Error",L"Failed PlaeyrCamera Create")
	}
	return pInstance;
}

void CPlayerCamera::Free()
{
	SafeRelease(m_pPlayer);
	SafeRelease(m_pMouse);

}
