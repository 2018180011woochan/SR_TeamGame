#include "stdafx.h"
#include "..\Header\RoomTrigger.h"


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
	m_pTransform->Set_Scale(_vector(4, 4, 4));


	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();
	m_eRenderID = ERenderID::Alpha;

	return S_OK;
}

HRESULT CRoomTrigger::Start()
{

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Cube"), BOUND::BOUNDTYPE::BOX);
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
	CGameObject::Free();
}
