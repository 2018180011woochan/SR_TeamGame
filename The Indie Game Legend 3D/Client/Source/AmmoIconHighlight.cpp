#include "stdafx.h"
#include "..\Header\AmmoIconHighlight.h"
#include "TexturePoolManager.h"

CAmmoIconHighlight::CAmmoIconHighlight()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

CAmmoIconHighlight::CAmmoIconHighlight(const CAmmoIconHighlight & _rOther)
	: CGameObject(_rOther)
	, m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

void CAmmoIconHighlight::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CAmmoIconHighlight * CAmmoIconHighlight::Create()
{
	CAmmoIconHighlight* pInstance = new CAmmoIconHighlight;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CAmmoIconHighlight::Clone()
{
	CAmmoIconHighlight* pClone = new CAmmoIconHighlight(*this);
	return pClone;
}

HRESULT CAmmoIconHighlight::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CAmmoIconHighlight::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();

	m_pTransform->Set_Scale(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_pImage->SetPivot(0.f, 0.5f);
	m_pTransform->Set_Position(D3DXVECTOR3(-575.5f, -285.f, 0.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CAmmoIconHighlight::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("AmmoIconHighlight");

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CAmmoIconHighlight::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CAmmoIconHighlight::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CAmmoIconHighlight::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}


