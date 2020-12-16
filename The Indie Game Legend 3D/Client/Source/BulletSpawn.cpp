#include "stdafx.h"
#include "..\Header\BulletSpawn.h"
#include "PlayerCamera.h"
#include "Player.h"
#include "Mouse.h"
CBulletSpawn::CBulletSpawn()
{
}

CBulletSpawn::CBulletSpawn(const CBulletSpawn & _rOther)
	:CGizmo(_rOther)
{
}

HRESULT CBulletSpawn::InitializePrototype()
{
	if (FAILED(CGizmo::InitializePrototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CBulletSpawn::Awake()
{
	CGizmo::Awake();
	return S_OK;
}

HRESULT CBulletSpawn::Start()
{
	m_pTransform->Set_Position(_vector(1.5f,-3.f,4.5f));
	return S_OK;
}

UINT CBulletSpawn::Update(const float _fDeltaTime)
{
	UpdatePos(_fDeltaTime);
	CGizmo::Update(_fDeltaTime);
	return OBJ_NOENVET;
}

UINT CBulletSpawn::LateUpdate(const float _fDeltaTime)
{
	CGizmo::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CBulletSpawn::Render()
{
	CGizmo::Render();
	return S_OK;
}

CGameObject * CBulletSpawn::Clone()
{
	CBulletSpawn * pInstance = new CBulletSpawn(*this);
	return pInstance;
}

CBulletSpawn * CBulletSpawn::Create()
{
	CBulletSpawn * pInstance = new CBulletSpawn();
	return pInstance;
}

void CBulletSpawn::Free()
{
	CGizmo::Free();
}

HRESULT CBulletSpawn::UpdatePos(const float _fDeltaTime)
{
	CPlayerCamera* pCamera = (CPlayerCamera*)FindGameObjectOfType<CPlayerCamera>();
	CAMERA_DESC CameraDesc = pCamera->Get_Camera();
	m_pTransform->Set_Parent(CameraDesc.vEye);

	
	CMouse* m_pMouse = (CMouse*)FindGameObjectOfType<CMouse>();
	if (nullptr == m_pMouse)
		return E_FAIL;
	m_pTransform->Add_RevolutionX(
		m_pMouse->Get_MouseDir().y  * 200.f * _fDeltaTime);
	m_pTransform->Add_RevolutionY(
		m_pMouse->Get_MouseDir().x  * 200.f * _fDeltaTime);
	return S_OK;
}

