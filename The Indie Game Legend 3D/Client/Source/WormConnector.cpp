#include "stdafx.h"
#include "..\Header\WormConnector.h"
#include "TexturePoolManager.h"

CWormConnector::CWormConnector()
{
}

CWormConnector::CWormConnector(const CWormConnector & _rOther)
	: CWormPart(_rOther)
{
}

void CWormConnector::Free()
{
	CWormPart::Free();
}

CWormConnector * CWormConnector::Create()
{
	CWormConnector* pInstance = new CWormConnector;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CWormConnector::Clone()
{
	CWormConnector* pClone = new CWormConnector(*this);
	return pClone;
}

HRESULT CWormConnector::InitializePrototype()
{
	CWormPart::InitializePrototype();
	return S_OK;
}

HRESULT CWormConnector::Awake()
{
	CWormPart::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Scale(D3DXVECTOR3(4.6f, 4.0f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(240.f, -5.f, 208.f));
	m_pTransform->UpdateTransform();

	m_pCollider = (CCollider*)AddComponent<CCollider>();

	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->SetRadius(3.f);
	m_pCollider->m_bExcept = true;
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWormConnector::Start()
{
	CWormPart::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("WormConnector"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("Side");

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CWormConnector::Update(const float _fDeltaTime)
{
	CWormPart::Update(_fDeltaTime);
	if (true == m_bRemove)
		return OBJ_DEAD;
	return 0;
}

UINT CWormConnector::LateUpdate(const float _fDeltaTime)
{
	CWormPart::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CWormConnector::Render()
{
	CWormPart::Render();
	return S_OK;
}

