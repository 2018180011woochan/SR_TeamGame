#include "stdafx.h"
#include "..\Header\Floor.h"


CFloor::CFloor()
	: m_pCollider(nullptr)
{
}

CFloor::CFloor(const CFloor & _rOther)
	: CGameObject(_rOther)
{
}

void CFloor::Free()
{
	CGameObject::Free();
}

CFloor * CFloor::Create()
{
	CFloor* pInstance = new CFloor;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CFloor::Clone()
{
	CFloor* pClone = new CFloor(*this);
	return pClone;
}

HRESULT CFloor::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CFloor::Awake()
{
	CGameObject::Awake();
	m_pTransform->Set_Scale(D3DXVECTOR3(112.f, 0.1f, 96.f));
	m_pTransform->UpdateTransform();
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Floor"), BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;

	m_sName = TEXT("Floor");
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CFloor::Start()
{
	CGameObject::Start();
	return S_OK;
}

UINT CFloor::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CFloor::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CFloor::Render()
{
	return S_OK;
}


