#include "stdafx.h"
#include "..\Header\Logo.h"
#include "TexturePoolManager.h"

CLogo::CLogo()
	: m_pImage(nullptr)
{
}

CLogo::CLogo(const CLogo & _rOther)
	: CGameObject(_rOther)
{
}

void CLogo::Free()
{
	CGameObject::Free();
	SafeRelease(m_pImage);
}

CLogo * CLogo::Create()
{
	CLogo* pInstance = new CLogo;

	return pInstance;
}

CGameObject * CLogo::Clone()
{
	CLogo* pClone = new CLogo(*this);
	return pClone;
}

HRESULT CLogo::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CLogo::Awake()
{
	CGameObject::Awake();
	m_eRenderID = ERenderID::UI;
	m_pImage = (Image*)AddComponent<Image>();
	m_pTransform->Set_Scale(D3DXVECTOR3(2.8f, 2.8f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(0.f, 145.f, 0.f));
	m_pTransform->UpdateTransform();
	return S_OK;
}

HRESULT CLogo::Start()
{
	CGameObject::Start();
	m_pImage->SetTexture(CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"))->GetTexture(TEXT("Logo"))[0]);
	return S_OK;
}

UINT CLogo::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CLogo::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CLogo::Render()
{
	//CGameObject::Render();
	//m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	//m_pImage->Render();
	return S_OK;
}

