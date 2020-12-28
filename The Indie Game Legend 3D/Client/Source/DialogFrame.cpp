#include "stdafx.h"
#include "..\Header\DialogFrame.h"
#include "TexturePoolManager.h"

CDialogFrame::CDialogFrame()
	: m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

CDialogFrame::CDialogFrame(const CDialogFrame & _rOther)
	: CGameObject(_rOther)
	, m_pImage(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
{
}

void CDialogFrame::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CDialogFrame * CDialogFrame::Create()
{
	CDialogFrame* pInstance = new CDialogFrame;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CDialogFrame::Clone()
{
	CDialogFrame* pClone = new CDialogFrame(*this);
	return pClone;
}

HRESULT CDialogFrame::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CDialogFrame::Awake()
{
	CGameObject::Awake();
	m_pImage = (Image*)AddComponent<Image>();
	
	m_pTransform->Set_Scale(D3DXVECTOR3(5.f, 5.f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(-480.f, 200.f, 0.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CDialogFrame::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("DialogHUD");

	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	m_pImage->SetPivot(0.f, 0.f);
	return S_OK;
}

UINT CDialogFrame::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CDialogFrame::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CDialogFrame::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}
