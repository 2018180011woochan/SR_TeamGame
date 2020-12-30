#include "stdafx.h"
#include "..\Header\BulletSpawn.h"
#include "PlayerCamera.h"
CBulletSpawn::CBulletSpawn()
	: m_pPlayerCamera(nullptr)
{
}

CBulletSpawn::CBulletSpawn(const CBulletSpawn & _rOther)
	: CGizmo(_rOther)
	, m_pPlayerCamera(nullptr)
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
	m_fRevAngleY = 0.f;
	m_fRevAngleX = 0.f;
	return S_OK;
}

HRESULT CBulletSpawn::Start()
{
	m_pTransform->Set_Scale(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(1.09485126f, -1.45688009f, 3.5f));

	m_pPlayerCamera = (CPlayerCamera*)(FindGameObjectOfType<CPlayerCamera>());
	SafeAddRef(m_pPlayerCamera);
	m_pTransform->SetParent(m_pPlayerCamera);
	return S_OK;
}

UINT CBulletSpawn::Update(const float _fDeltaTime)
{
	return OBJ_NOENVET;
}

UINT CBulletSpawn::LateUpdate(const float _fDeltaTime)
{
	CGizmo::LateUpdate(_fDeltaTime);
	m_pTransform->UpdateTransform();
	return OBJ_NOENVET;
}

HRESULT CBulletSpawn::Render()
{
	//CGizmo::Render();
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
	SafeRelease(m_pPlayerCamera);
}


