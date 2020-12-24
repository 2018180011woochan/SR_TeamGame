#include "stdafx.h"
#include "WeaponHUD.h"
#include "TexturePoolManager.h"

CWeaponHUD::CWeaponHUD()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

CWeaponHUD::CWeaponHUD(const CWeaponHUD & _rOther)
	:CGameObject(_rOther)
{
}

void CWeaponHUD::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CWeaponHUD * CWeaponHUD::Create()
{
	CWeaponHUD* pInstance = new CWeaponHUD;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CWeaponHUD::Clone()
{
	CWeaponHUD* pClone = new CWeaponHUD(*this);
	return pClone;
}

HRESULT CWeaponHUD::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CWeaponHUD::Awake()
{
	CGameObject::Awake();

	m_pImage = (Image*)AddComponent<Image>();

	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	m_pImage->SetPivot(0.f, 0.5f);
	m_pTransform->Set_Position(D3DXVECTOR3(-575.5f, -285.f, 0.f));
	m_pTransform->UpdateTransform();
	//Set RenderID
	m_eRenderID = ERenderID::UI;

	return S_OK;
}

HRESULT CWeaponHUD::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("WeaponHUD");

	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CWeaponHUD::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CWeaponHUD::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CWeaponHUD::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CWeaponHUD::ChangeWeapon(const UINT _nWeaponID)
{
	if (_nWeaponID >= m_nMaxFrame)
		return;
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[_nWeaponID]);
}



