#include "stdafx.h"
#include "..\Header\MiniMapPartial.h"
#include "MiniMapRenderer.h"

CMiniMapPartial::CMiniMapPartial()
	: m_pImage(nullptr)
	, m_pMiniMapRenderer(nullptr)
{
}

CMiniMapPartial::CMiniMapPartial(const CMiniMapPartial & _rOther)
	: CGameObject(_rOther)
{
}

void CMiniMapPartial::Free()
{
	CGameObject::Free();
	SafeRelease(m_pMiniMapRenderer);
}

CMiniMapPartial * CMiniMapPartial::Create()
{
	CMiniMapPartial* pInstance = new CMiniMapPartial;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CMiniMapPartial::Clone()
{
	CMiniMapPartial* pClone = new CMiniMapPartial(*this);
	return pClone;
}

HRESULT CMiniMapPartial::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CMiniMapPartial::Awake()
{
	CGameObject::Awake();
	m_pTransform->Set_Scale(D3DXVECTOR3(1.f, 1.f, 1.f));
	m_pTransform->UpdateTransform();
	m_pImage = (Image*)AddComponent<Image>();
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CMiniMapPartial::Start()
{
	CGameObject::Start();
	m_pMiniMapRenderer = (CMiniMapRenderer*)(FindGameObjectOfType<CMiniMapRenderer>());

	m_pImage->SetTexture(m_pMiniMapRenderer->GetPartialMap());
	m_pImage->SetWidth(170.f);
	m_pImage->SetHeight(170.f);

	m_pTransform->Set_Position(D3DXVECTOR3(521.f, -246.f, 0.f));
	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CMiniMapPartial::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CMiniMapPartial::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CMiniMapPartial::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

