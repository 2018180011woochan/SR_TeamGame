#include "stdafx.h"
#include "..\Header\BossHPBar.h"
#include "TexturePoolManager.h"

CBossHPBar::CBossHPBar()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

CBossHPBar::CBossHPBar(const CBossHPBar & _rOther)
	: CGameObject(_rOther)
	, m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

void CBossHPBar::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CBossHPBar * CBossHPBar::Create()
{
	CBossHPBar* pInstance = new CBossHPBar;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CBossHPBar::Clone()
{
	CBossHPBar* pClone = new CBossHPBar(*this);
	return pClone;
}

HRESULT CBossHPBar::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CBossHPBar::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();

	m_pTransform->Set_Position(D3DXVECTOR3(-40.f, 317.f, 0.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CBossHPBar::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("HUD_BossHP");
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[1]);
	m_pImage->SetTiling(D3DXVECTOR2(0.9375f, 0.f));
	m_pImage->SetPivot(0.f, 0.f);
	m_pImage->SetWidth(240.f);
	m_pImage->SetHeight(30.f);
	m_pImage->SetImageType(Image::ImageType::Fill);
	return S_OK;
}

UINT CBossHPBar::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CBossHPBar::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CBossHPBar::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CBossHPBar::SetHPBar(float _fRatio)
{
	m_pImage->SetFillAmount(_fRatio);
}

