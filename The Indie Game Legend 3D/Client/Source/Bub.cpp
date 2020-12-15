#include "stdafx.h"
#include "Bub.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
CBub::CBub()
	:m_pTexturePool(nullptr)
{
}

CBub::CBub(const CBub & other)
	: CGameObject(other)
{
}

HRESULT CBub::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBub::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bub"));
	SafeAddRef(m_pTexturePool);
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CBub::Start()
{
	CGameObject::Start();
	m_pTransform->Set_Scale(_vector(5, 5, 1));
	m_pTransform->Add_Position(_vector(0, 3, 0));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CBub::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	m_pTransform->UpdateTransform();
	return _uint();
}

UINT CBub::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CBub::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CBub::Clone()
{
	CBub* pClone = new CBub(*this);
	return pClone;
}

CBub * CBub::Create()
{
	CBub* pInstance = new CBub();
	return pInstance;
}

void CBub::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
