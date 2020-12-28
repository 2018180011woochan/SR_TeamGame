#include "stdafx.h"
#include "..\Header\IntroBackGround.h"
#include "TexturePoolManager.h"

CIntroBackGround::CIntroBackGround()
	: m_pImage(nullptr)
{
}

CIntroBackGround::CIntroBackGround(const CIntroBackGround & _rOther)
	: CGameObject(_rOther)
{
}

void CIntroBackGround::Free()
{
	CGameObject::Free();
}

CIntroBackGround * CIntroBackGround::Create()
{
	CIntroBackGround* pInstance = new CIntroBackGround;

	if (FAILED(pInstance->InitializePrototype()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

CGameObject * CIntroBackGround::Clone()
{
	CIntroBackGround* pInstance = new CIntroBackGround(*this);
	if (nullptr == pInstance)
	{
		return nullptr;
	}
	return pInstance;
}

HRESULT CIntroBackGround::InitializePrototype()
{
	return S_OK;
}

HRESULT CIntroBackGround::Awake()
{
	CGameObject::Awake();

	m_eRenderID = ERenderID::UI;
	m_pImage = (Image*)AddComponent<Image>();
	if (nullptr == m_pImage)
		return E_FAIL;
	m_pImage->SetTexture(CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"))->GetTexture(TEXT("BackGround"))[0]);
	return S_OK;
}

HRESULT CIntroBackGround::Start()
{
	CGameObject::Start();
	return S_OK;
}

UINT CIntroBackGround::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CIntroBackGround::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CIntroBackGround::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	if (nullptr != m_pImage)
		m_pImage->Render();
	return S_OK;
}

