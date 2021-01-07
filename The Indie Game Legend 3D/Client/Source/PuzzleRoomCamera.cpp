#include "stdafx.h"
#include "..\Header\PuzzleRoomCamera.h"


CPuzzleRoomCamera::CPuzzleRoomCamera()
{
}

CPuzzleRoomCamera::CPuzzleRoomCamera(const CPuzzleRoomCamera & _rOther)
	:CCamera(_rOther)
{
}

HRESULT CPuzzleRoomCamera::InitializePrototype()
{
	return S_OK;
}

HRESULT CPuzzleRoomCamera::Awake()
{
	/*CameraDesc setting*/
	m_tCameraDesc.fAspect = (float)g_nWinCX / g_nWinCY;
	m_tCameraDesc.fFar = 500.f;
	m_tCameraDesc.fNear = 1.f;
	m_tCameraDesc.fFovY = D3DXToRadian(90.f);
	return S_OK;
}

HRESULT CPuzzleRoomCamera::Start()
{
	m_pTransform->Set_Rotation(_vector(90, 0, 0));
	m_pTransform->Set_Position(_vector(0, 50, 0));

	return S_OK;
}

UINT CPuzzleRoomCamera::Update(const float _fDeltaTime)
{
	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();

	if (FAILED(UpdateCamera(_fDeltaTime)))
		return OBJ_NOENVET;

	return OBJ_NOENVET;
}

UINT CPuzzleRoomCamera::LateUpdate(const float _fDeltaTime)
{
	CCamera::LateUpdate(_fDeltaTime);

	return OBJ_NOENVET;
}

HRESULT CPuzzleRoomCamera::Render()
{
	return S_OK;
}

CGameObject * CPuzzleRoomCamera::Clone()
{
	CPuzzleRoomCamera* pInstance = new CPuzzleRoomCamera(*this);
	return pInstance;
}

CPuzzleRoomCamera * CPuzzleRoomCamera::Create()
{
	CPuzzleRoomCamera* pInstance = new CPuzzleRoomCamera();
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		PrintLog(L"Error", L"Failed PuzzleRoomCamera Create")
	}
	return pInstance;
}

HRESULT CPuzzleRoomCamera::UpdateCamera(float _fDeletaTime)
{
	// test [1/7/2021 wades]
	m_tCameraDesc.vEye = m_pTransform->Get_WorldPosition();
	m_tCameraDesc.vUp = m_pTransform->Get_Up();
	m_tCameraDesc.vAt = m_tCameraDesc.vEye + (m_pTransform->Get_Look() *5.f);
	return S_OK;
}

void CPuzzleRoomCamera::Free()
{
	CCamera::Free();
}

