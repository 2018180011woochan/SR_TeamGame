#include "stdafx.h"
#include "..\Header\WormTail.h"
#include "TexturePoolManager.h"

CWormTail::CWormTail()
{
}

CWormTail::CWormTail(const CWormTail & _rOther)
	: CWormPart(_rOther)
{
}

void CWormTail::Free()
{
	CWormPart::Free();
}

CWormTail * CWormTail::Create()
{
	CWormTail* pInstance = new CWormTail;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CWormTail::Clone()
{
	CWormTail* pClone = new CWormTail(*this);
	return pClone;
}

HRESULT CWormTail::InitializePrototype()
{
	CWormPart::InitializePrototype();
	return S_OK;
}

HRESULT CWormTail::Awake()
{
	CWormPart::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Scale(D3DXVECTOR3(9.6f, 5.6f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(0.f, -5.f, 0.f));
	m_pTransform->UpdateTransform();

	m_pCollider = (CCollider*)AddComponent<CCollider>();

	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->SetRadius(3.f);
	m_pCollider->m_bExcept = true;
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWormTail::Start()
{
	CWormPart::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("WormTail"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("Side");

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CWormTail::Update(const float _fDeltaTime)
{
	CWormPart::Update(_fDeltaTime);
	if (true == m_bRemove)
		return OBJ_DEAD;
	return 0;
}

UINT CWormTail::LateUpdate(const float _fDeltaTime)
{
	CWormPart::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CWormTail::Render()
{
	CWormPart::Render();
	return S_OK;
}

