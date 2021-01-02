#include "stdafx.h"
#include "PlayerShip.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"


CPlayerShip::CPlayerShip()
	:m_pTexturePool(nullptr)
{
}

CPlayerShip::CPlayerShip(const CPlayerShip & other)
	: CGameObject(other)
{
}

HRESULT CPlayerShip::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerShip::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Ship"));
	//m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bub"));
	SafeAddRef(m_pTexturePool);
	m_eRenderID = ERenderID::Alpha;
	m_nTag = 0;
	return S_OK;
}

HRESULT CPlayerShip::Start()
{
	CGameObject::Start();
	m_pTransform->Set_Scale(_vector(4, 4, 4));
	m_pTransform->Set_Position(_vector(10.f, 0.f, 0.f));
	//m_pTransform->Add_Position(_vector(0, -3, 0));
	//m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	//Test
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Ship"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;

	return S_OK;
}

UINT CPlayerShip::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;


	m_pTransform->UpdateTransform();


	return _uint();
}

UINT CPlayerShip::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CPlayerShip::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CPlayerShip::OnCollision(CGameObject * _pGameObject)
{

}


HRESULT CPlayerShip::Movement(float fDeltaTime)
{

	return S_OK;
}


CGameObject * CPlayerShip::Clone()
{
	CPlayerShip* pClone = new CPlayerShip(*this);
	return pClone;
}

CPlayerShip * CPlayerShip::Create()
{
	CPlayerShip* pInstance = new CPlayerShip();
	return pInstance;
}

void CPlayerShip::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
