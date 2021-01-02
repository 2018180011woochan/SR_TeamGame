#include "stdafx.h"
#include "..\Header\SkillGauge.h"
#include "TexturePoolManager.h"

CSkillGauge::CSkillGauge()
	: m_pTexturePool(nullptr)
	, m_pImage(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

CSkillGauge::CSkillGauge(const CSkillGauge & _rOther)
	: CGameObject(_rOther)
	, m_pTexturePool(nullptr)
	, m_pImage(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

void CSkillGauge::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CSkillGauge * CSkillGauge::Create()
{
	CSkillGauge* pInstacne = new CSkillGauge;
	if (FAILED(pInstacne->InitializePrototype()))
	{
		SafeRelease(pInstacne);
		return nullptr;
	}
	return pInstacne;
}

CGameObject * CSkillGauge::Clone()
{
	CSkillGauge* pClone = new CSkillGauge(*this);
	return pClone;
}

HRESULT CSkillGauge::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CSkillGauge::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();
	m_pImage->SetImageType(Image::ImageType::Fill);

	m_pTransform->Set_RotationZ(90.f);
	m_pTransform->Set_Position(D3DXVECTOR3(-595.f, -286.f, 0.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CSkillGauge::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);
	
	m_sTextureKey = TEXT("SkillGauge");

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CSkillGauge::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CSkillGauge::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CSkillGauge::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CSkillGauge::SetSkillGauge(const float _fValue)
{
	float fValue = _fValue;

	if (0.f > fValue)
		fValue = 0.f;
	else if (1.f < fValue)
		fValue = 1.f;

	m_pImage->SetFillAmount(fValue);
}
