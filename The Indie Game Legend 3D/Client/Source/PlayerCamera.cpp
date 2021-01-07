#include "stdafx.h"
#include "PlayerCamera.h"
#include "Mouse.h"
#include "Player.h"
#include "Transform.h"
#include "MsgManager.h"



CPlayerCamera::CPlayerCamera()
{
}

CPlayerCamera::CPlayerCamera(const CPlayerCamera & _rOther)
	:CCamera(_rOther)
	, m_fDistanceToAt(_rOther.m_fDistanceToAt)
	, m_fCameraAngleX(_rOther.m_fCameraAngleX)
	, m_fCameraAngleSpeed(_rOther.m_fCameraAngleSpeed)
	, m_pPlayer(_rOther.m_pPlayer)
	, m_pMouse(_rOther.m_pMouse)
	, m_bAnimate(false)
	, m_fFovAnimateMax(0.f)
	, m_fFovAnimateMin(0.f)
	, m_eCurCameraWork(eCameraWork::End)
	, m_ePreCameraWork(eCameraWork::End)
	, m_fFovAmount(0.f)
	, m_fFov(0.f)
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
	/*CameraDesc setting*/
	m_tCameraDesc.fAspect = (float)g_nWinCX / g_nWinCY;
	m_tCameraDesc.fFar = 500.f;
	m_tCameraDesc.fNear = 1.f;
	m_tCameraDesc.fFovY = D3DXToRadian(m_fFov);
	m_tCameraDesc.vUp = _vector(0, 1, 0);
	m_fFov = 90.f;

	memcpy(&m_tOriCamera_Desc, &m_tCameraDesc, sizeof(CAMERA_DESC));

	m_fCameraAngleX = 0.f;
	m_fDistanceToAt = 500.f;
	m_fCameraAngleSpeed = 200.f;

	m_fCameraHeight = 8.f;
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
	CameraWorking(_fDeltaTime);

	if (FAILED(UpdateCamera(_fDeltaTime)))
		return OBJ_NOENVET;

	CCamera::LateUpdate(_fDeltaTime);

	D3DXMATRIX matInverseView;
	D3DXMatrixInverse(&matInverseView, 0, &m_tCameraDesc.matView);
	m_pTransform->Set_WorldMatrix(matInverseView);
	return OBJ_NOENVET;
}

UINT CPlayerCamera::LateUpdate(const float _fDeltaTime)
{
	return OBJ_NOENVET;
}

HRESULT CPlayerCamera::Render()
{
	return S_OK	;
}


/*카메라 세팅*/
HRESULT CPlayerCamera::UpdateCamera(float _fDeletaTime)
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

	//Cheat
	static float h = 0.f;

	if (GetAsyncKeyState('7') & 0x0001)
	{
		h = 0.f;
	}
	if (GetAsyncKeyState('8') & 0x0001)
	{
		h += 1.f;
	}
	//Cheat
	if (CMsgManager::GetInstance()->GetMouseMode() == false)
	{
		m_fCameraAngleX += m_pMouse->Get_MouseDir().y  * m_fCameraAngleSpeed * _fDeletaTime;
		m_fCameraAngleX = CLAMP(m_fCameraAngleX, CameraYMin, CameraYMax);
	}


	CTransform* pPlayerTransFomr = (CTransform*)(m_pPlayer->GetComponent<CTransform>());
	TRANSFORM_DESC PlayerTransFormDesc = pPlayerTransFomr->Get_TransformDesc();
	_vector vPlayerLook;
	_vector vPlayerRight;
	_matrix matRotAxis;

	/*Eye*/
	m_tCameraDesc.vEye = pPlayerTransFomr->Get_Position();
	m_tCameraDesc.vEye.y = m_fCameraHeight + h;// m_fCameraHeight;//점프 없을거라고 봄
	m_tCameraDesc.vEye += pPlayerTransFomr->Get_Right()* m_fcameraVertical;

	/*At . RotY */
	memcpy(&vPlayerLook, &PlayerTransFormDesc.matWorld.m[2][0], sizeof(_vector));
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);
	vPlayerLook *= m_fDistanceToAt;

	memcpy(&vPlayerRight, &PlayerTransFormDesc.matWorld.m[0][0], sizeof(_vector));
	D3DXVec3Normalize(&vPlayerRight, &vPlayerRight);

	/*At . RotX*/
	D3DXMatrixRotationAxis(&matRotAxis, &vPlayerRight, D3DXToRadian(m_fCameraAngleX));
	D3DXVec3TransformNormal(&vPlayerLook, &vPlayerLook, &matRotAxis);

	// 임시 [1/6/2021 wades]
	m_tCameraDesc.fFovY = D3DXToRadian(m_fFov);
	m_tCameraDesc.vAt = m_tCameraDesc.vEye + vPlayerLook;
	return S_OK;
}

void CPlayerCamera::CameraWorking(const float & _fTimeDelta)
{
	switch (m_eCurCameraWork)
	{
	case CPlayerCamera::eCameraWork::ZoomIn:
		m_fFov -= m_fFovAmount * _fTimeDelta;
		m_fFov = CLAMP(m_fFov, m_fFovAnimateMin, D3DXToDegree(m_tOriCamera_Desc.fFovY));
		break;
	case CPlayerCamera::eCameraWork::ZoomOut:
		m_fFov += m_fFovAmount * _fTimeDelta;
		m_fFov = CLAMP(m_fFov, D3DXToDegree(m_tOriCamera_Desc.fFovY), m_fFovAnimateMax);
		break;
	}
}

// 수정해야할듯 통일 시킬려면 [1/6/2021 wades]
void CPlayerCamera::SetHeghitPersent(const float & _fHeight)
{
	m_fCameraHeight = PlayerCameraEye * _fHeight;
}

void CPlayerCamera::SetShakeVertical(const float & _fVertical)
{
	m_fcameraVertical = _fVertical;
}

void CPlayerCamera::SetCameraZoomIn(const float & _fMinFov, const float & _fAmont)
{
	m_eCurCameraWork = eCameraWork::ZoomIn;
	m_fFovAnimateMin = _fMinFov;
	m_fFovAmount = _fAmont;
}

void CPlayerCamera::SetCameraZoomOut(const float & _fMaxFov, const float & _fAmont)
{
	m_eCurCameraWork = eCameraWork::ZoomOut;
	m_fFovAnimateMax = _fMaxFov;
	m_fFovAmount = _fAmont;
}

void CPlayerCamera::ResetCamera()
{
	memcpy(&m_tCameraDesc, &m_tOriCamera_Desc, sizeof(CAMERA_DESC)); 
	m_fFov = D3DXToDegree(m_tOriCamera_Desc.fFovY);
	m_eCurCameraWork = eCameraWork::End;
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
