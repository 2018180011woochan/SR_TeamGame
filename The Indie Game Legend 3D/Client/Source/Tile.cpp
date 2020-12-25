#include "stdafx.h"
#include "..\Header\Tile.h"


CTile::CTile()
{
}

CTile::CTile(const CTile& _rOther)
	:CGameObject(_rOther)
{
}


HRESULT CTile::InitializePrototype()
{
	return S_OK;
}

HRESULT CTile::Awake()
{
	CGameObject::Awake();
	m_pTransform->Set_Rotation(_vector(90, 0, 0));

	m_pTransform->Set_Scale(_vector(8, 8, 8));

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(L"Quad",BOUND::BOUNDTYPE::BOX);
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CTile::Start()
{
	m_pTransform->Add_Position(_vector(0, -0.5f, 0));

	return S_OK;
}

UINT CTile::Update(const float _fDeltaTime)
{
	return OBJ_NOENVET;
}

UINT CTile::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CTile::Render()
{

	return S_OK;
}


void CTile::Free()
{
	CGameObject::Free();
}
