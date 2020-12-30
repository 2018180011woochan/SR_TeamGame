#include "stdafx.h"
#include "..\Header\LoadingBackGround.h"
#include "TexturePoolManager.h"

CLoadingBackGround::CLoadingBackGround()
	: m_pTexturePool(nullptr)
{
	m_pTransform = (CTransform*)AddComponent<CTransform>();
}

CLoadingBackGround::CLoadingBackGround(const CLoadingBackGround & _rOther)
	: CGameObject(_rOther)
	, m_pTexturePool(nullptr)
{
}

void CLoadingBackGround::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CLoadingBackGround * CLoadingBackGround::Create()
{
	CLoadingBackGround* pInstance = new CLoadingBackGround;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CLoadingBackGround::Clone()
{
	CLoadingBackGround* pClone = new CLoadingBackGround(*this);
	return pClone;
}

HRESULT CLoadingBackGround::InitializePrototype()
{
	return S_OK;
}

HRESULT CLoadingBackGround::Awake()
{
	m_pImage = (Image*)AddComponent<Image>();
	return S_OK;
}

HRESULT CLoadingBackGround::Start()
{
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);
	m_pImage->SetTexture(m_pTexturePool->GetTexture(TEXT("BackGround"))[1]);
	return S_OK;
}

UINT CLoadingBackGround::Update(const float _fDeltaTime)
{
	return 0;
}

UINT CLoadingBackGround::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CLoadingBackGround::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

