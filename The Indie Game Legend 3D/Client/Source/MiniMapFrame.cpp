#include "stdafx.h"
#include "..\Header\MiniMapFrame.h"
#include "TexturePoolManager.h"

CMiniMapFrame::CMiniMapFrame()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

CMiniMapFrame::CMiniMapFrame(const CMiniMapFrame & _rOther)
	: CGameObject(_rOther)
{
}

void CMiniMapFrame::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CMiniMapFrame * CMiniMapFrame::Create()
{
	CMiniMapFrame* pInstance = new CMiniMapFrame;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CMiniMapFrame::Clone()
{
	CMiniMapFrame* pClone = new CMiniMapFrame(*this);
	return pClone;
}

HRESULT CMiniMapFrame::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CMiniMapFrame::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();

	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(520.f, -225.f, 0.f));
	m_pTransform->UpdateTransform();
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CMiniMapFrame::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("MiniMapFrame");

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	return S_OK;
}

UINT CMiniMapFrame::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CMiniMapFrame::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CMiniMapFrame::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}
