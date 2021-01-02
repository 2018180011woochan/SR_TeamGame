#include "stdafx.h"
#include "..\Header\AmmoIcon.h"
#include "TexturePoolManager.h"

CAmmoIcon::CAmmoIcon()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

CAmmoIcon::CAmmoIcon(const CAmmoIcon & _rOther)
	: CGameObject(_rOther)
	, m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

void CAmmoIcon::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CAmmoIcon * CAmmoIcon::Create()
{
	CAmmoIcon* pInstance = new CAmmoIcon;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CAmmoIcon::Clone()
{
	CAmmoIcon* pClone = new CAmmoIcon(*this);
	return pClone;
}

HRESULT CAmmoIcon::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CAmmoIcon::Awake()
{
	CGameObject::Awake();

	m_pImage = (Image*)AddComponent<Image>();

	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	m_pImage->SetPivot(0.f, 0.5f);
	m_pTransform->Set_Position(D3DXVECTOR3(-575.5f, -285.f, 0.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CAmmoIcon::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("WeaponHUD");

	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CAmmoIcon::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CAmmoIcon::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CAmmoIcon::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CAmmoIcon::SetAmmoIcon(const UINT _nWeaponID)
{
	if (_nWeaponID >= m_nMaxFrame)
		return;
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[_nWeaponID]);
}
