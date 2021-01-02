#include "stdafx.h"
#include "..\Header\AmmoFrame.h"
#include "TexturePoolManager.h"

CAmmoFrame::CAmmoFrame()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
{
}

CAmmoFrame::CAmmoFrame(const CAmmoFrame & _rOther)
	:CGameObject(_rOther)
{
}

void CAmmoFrame::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}

CAmmoFrame * CAmmoFrame::Create()
{
	CAmmoFrame* pInstance = new CAmmoFrame;
	if (FAILED(pInstance->InitializePrototype()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

CGameObject * CAmmoFrame::Clone()
{
	CAmmoFrame* pClone = new CAmmoFrame(*this);
	return pClone;
}

HRESULT CAmmoFrame::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CAmmoFrame::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();
	m_pImage->SetPivot(0.f, 0.5f);

	m_eRenderID = ERenderID::UI;

	m_pTransform->Set_Position(D3DXVECTOR3(-615.5f, -285.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	m_pTransform->UpdateTransform();
	return S_OK;
}

HRESULT CAmmoFrame::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));	
	m_sTextureKey = TEXT("HUD_Secondary");
	SafeAddRef(m_pTexturePool);

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CAmmoFrame::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CAmmoFrame::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CAmmoFrame::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CAmmoFrame::SetAmmoLevel(const UINT _nLevel)
{
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[_nLevel]);
}

