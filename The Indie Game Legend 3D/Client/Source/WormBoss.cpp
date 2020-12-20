#include "stdafx.h"
#include "..\Header\WormBoss.h"
#include "Player.h"
#include "MeshRenderer.h"

CWormBoss::CWormBoss()
	: m_pTexturePool(nullptr)
{
}

CWormBoss::CWormBoss(const CWormBoss & other)
	: CMonster(other)
{
}

HRESULT CWormBoss::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWormBoss::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("WormBoss"));
	SafeAddRef(m_pTexturePool);

	m_pTransform->Set_Scale(_vector(5.f, 5.f, 5.f));
	m_pTransform->Set_Position(_vector(-5.f, 10.f, 10.f));

	nIndex = 0;
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWormBoss::Start()
{
	return E_NOTIMPL;
}

UINT CWormBoss::Update(const float _fDeltaTime)
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("FaceSide"))[0]);

	return S_OK;
}

UINT CWormBoss::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CWormBoss::Render()
{
	return E_NOTIMPL;
}

CGameObject * CWormBoss::Clone()
{
	return nullptr;
}

CWormBoss * CWormBoss::Create()
{
	return nullptr;
}

void CWormBoss::Free()
{
}
