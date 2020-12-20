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
	m_tCameraDesc.vEye.y =/* m_fCameraHeight*/m_fValue; //점프 없을거라고 봄
	
	/*At . RotY */
	memcpy(&vPlayerLook, &PlayerTransFormDesc.matWorld.m[2][0], sizeof(_vector));
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	vPlayerLook *= m_fDistanceToAt;

	memcpy(&vPlayerRight, &PlayerTransFormDesc.matWorld.m[0][0], sizeof(_vector));
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	/*At . RotX*/
	D3DXMatrixRotationAxis(&matRotAxis, &vPlayerRight, D3DXToRadian(m_fCameraAngleX));
	D3DXVec3TransformNormal(&vPlayerLook, &vPlayerLook, &matRotAxis);


	m_tCameraDesc.vAt = m_tCameraDesc.vEye + vPlayerLook;
	return S_OK;
}

CPlayerCamera::CPlayerCamera()
{
}

CPlayerCamera::CPlayerCamera(const CPlayerCamera & _rOther)
	:CCamera(_rOther)
	,m_fDistanceToAt(_rOther.m_fDistanceToAt)
	,m_fCameraAngleX(_rOther.m_fCameraAngleX)
	,m_fCameraAngleSpeed(_rOther.m_fCameraAngleSpeed)
	,m_pPlayer(_rOther.m_pPlayer)
	,m_pMouse(_rOther.m_pMouse)
{
}

void CPlayerCamera::SetHeghitPersent(const float & _fHeight)
{
	m_fCameraHeight = PlayerCameraEye * _fHeight;
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
	/*CameraDesc setting*/
	m_tCameraDesc.fAspect = (float)g_nWinCX / g_nWinCY;
	m_tCameraDesc.fFar = 500.f;
	m_tCameraDesc.fNear = 1.f;
	m_tCameraDesc.fFovY = D3DXToRadian(90.f);
	m_tCameraDesc.vUp = _vector(0, 1, 0);
	m_fCameraAngleX = 0.f;
	m_fDistanceToAt = 500.f;
	m_fCameraAngleSpeed = 200.f;
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

	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	{
		m_fValue += 1.0f;
	}
	else if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
	{
		m_fValue -= 1.0f;
	}
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
	CCamera::Free();
}
