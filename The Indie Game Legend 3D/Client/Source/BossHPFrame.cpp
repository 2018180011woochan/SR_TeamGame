#include "stdafx.h"
#include "..\Header\BossHPFrame.h"
#include "TexturePoolManager.h"

CBossHPFrame::CBossHPFrame()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

CBossHPFrame::CBossHPFrame(const CBossHPFrame & _rOther)
	: CGameObject(_rOther)
	, m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

void CBossHPFrame::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CBossHPFrame * CBossHPFrame::Create()
{
	CBossHPFrame* pInstance = new CBossHPFrame;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CBossHPFrame::Clone()
{
	CBossHPFrame* pClone = new CBossHPFrame(*this);
	return pClone;
}

HRESULT CBossHPFrame::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CBossHPFrame::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();

	m_pTransform->Set_Position(D3DXVECTOR3(-50.f, 327.f, 0.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CBossHPFrame::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("HUD_BossHP");
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	m_pImage->SetPivot(0.f, 0.f);
	return S_OK;
}

UINT CBossHPFrame::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CBossHPFrame::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CBossHPFrame::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}


