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
	m_pTransform->Set_Position(_vector(25, 5, 5));
	m_pTransform->Set_Scale(_vector(10, 20, 0.1f));


	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();
	m_eRenderID = ERenderID::Alpha;

	return S_OK;
}

HRESULT CRoomTrigger::Start()
{
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Object"));
	SafeAddRef(m_pTexturePool);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("RoomTirgger"))[0]);


	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(L"SkyBox");
	m_pCollider->m_bIsRigid = true;

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
	m_pCollider->Draw();
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
