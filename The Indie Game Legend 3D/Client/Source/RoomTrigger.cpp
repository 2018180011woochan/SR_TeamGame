#include "stdafx.h"
#include "..\Header\RoomTrigger.h"
#include "TexturePoolManager.h"


CRoomTrigger::CRoomTrigger()
{
}

CRoomTrigger::CRoomTrigger(const CRoomTrigger & _rOther)
	:CTrigger(_rOther)
{
}

HRESULT CRoomTrigger::InitializePrototype()
{
	return S_OK;
}

HRESULT CRoomTrigger::Awake()
{
	CGameObject::Awake();
	m_sName = L"RoomTrigger";
	//Test
	m_pTransform->Set_Scale(_vector(8.f, 20.f, 0.1f));
	m_pTransform->Add_Position(_vector(0.f, 2.f, 0.f));
	m_eRenderID = ERenderID::AlphaCullNone;

	return S_OK;
}

HRESULT CRoomTrigger::Start()
{
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Object"));
	SafeAddRef(m_pTexturePool);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("RoomTirgger"))[0]);

	m_pTransform->UpdateTransform();

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Cube"), BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = false;

	m_pTransform->UpdateWorld();
	return S_OK;
}

UINT CRoomTrigger::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CRoomTrigger::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CRoomTrigger::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CRoomTrigger::Clone()
{
	CGameObject* pInstance = new CRoomTrigger(*this);
	return pInstance;
}

void CRoomTrigger::OnCollision(CGameObject * _pGameObject)
{
}

CRoomTrigger * CRoomTrigger::Create()
{
	CRoomTrigger* pInstance = new CRoomTrigger();
	return pInstance;
}

void CRoomTrigger::Free()
{
	SafeRelease(m_pTexturePool);
	CTrigger::Free();
}
