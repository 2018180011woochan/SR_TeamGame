#include "stdafx.h"
#include "..\Header\Heart.h"
#include "TexturePoolManager.h"

CHeart::CHeart()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

CHeart::CHeart(const CHeart & _rOther)
	: CGameObject(_rOther)
{
}

void CHeart::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CHeart * CHeart::Create()
{
	CHeart* pInstance = new CHeart;
	if (FAILED(pInstance->InitializePrototype()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

CGameObject * CHeart::Clone()
{
	CHeart* pClone = new CHeart(*this);
	return pClone;
}

HRESULT CHeart::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CHeart::Awake()
{
	CGameObject::Awake();

	m_pImage = (Image*)AddComponent<Image>();
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CHeart::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	m_sTextureKey = TEXT("Heart");
	SafeAddRef(m_pTexturePool);

	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CHeart::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CHeart::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CHeart::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CHeart::SetGauge(const UINT _nValue)
{
	UINT nIndex = 0;
	if (_nValue >= m_nMaxFrame)
		nIndex = m_nMaxFrame - 1;
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[nIndex]);
}

