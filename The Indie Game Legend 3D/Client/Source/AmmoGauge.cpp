#include "stdafx.h"
#include "..\Header\AmmoGauge.h"
#include "TexturePoolManager.h"

CAmmoGauge::CAmmoGauge()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
{
}

CAmmoGauge::CAmmoGauge(const CAmmoGauge & _rOther)
	:CGameObject(_rOther)
{
}

void CAmmoGauge::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CAmmoGauge * CAmmoGauge::Create()
{
	CAmmoGauge* pInstance = new CAmmoGauge;
	if (FAILED(pInstance->InitializePrototype()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

CGameObject * CAmmoGauge::Clone()
{
	CAmmoGauge* pClone = new CAmmoGauge(*this);

	return pClone;
}

HRESULT CAmmoGauge::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CAmmoGauge::Awake()
{
	CGameObject::Awake();
	
	m_pImage = (Image*)AddComponent<Image>();
	m_pImage->SetPivot(0.f, 0.5f);
	m_pImage->SetImageType(Image::ImageType::Fill);
	m_eRenderID = ERenderID::UI;

	m_pTransform->Set_Position(D3DXVECTOR3(-480.5f, -286.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	m_pTransform->UpdateTransform();
	return S_OK;
}

HRESULT CAmmoGauge::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	m_sTextureKey = TEXT("AmmoGauge");
	SafeAddRef(m_pTexturePool);

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);

	return S_OK;
}

UINT CAmmoGauge::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CAmmoGauge::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CAmmoGauge::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

