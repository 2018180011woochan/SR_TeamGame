#include "stdafx.h"
#include "..\Header\TurretBullet.h"
#include "Player.h"

CTurretBullet::CTurretBullet()
	: m_pTexturePool(nullptr)
{
}

CTurretBullet::CTurretBullet(const CTurretBullet & other)
	: CGameObject(other)
{
}



HRESULT CTurretBullet::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTurretBullet::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("FireBall"));
	SafeAddRef(m_pTexturePool);

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CTurretBullet::Start()
{
	CGameObject::Start();
	m_pTransform->Set_Scale(_vector(1, 1, 1));
	//m_pTransform->Add_Position(_vector(-5.f, 5.f, 10.f));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_vecPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	return S_OK;
}

UINT CTurretBullet::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	if (nIndex >= 4)
		nIndex = 0;
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	m_pTransform->UpdateTransform();


	return _uint();
}

UINT CTurretBullet::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CTurretBullet::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CTurretBullet::Clone()
{
	CTurretBullet* pClone = new CTurretBullet(*this);
	return pClone;
}

CTurretBullet * CTurretBullet::Create()
{
	CTurretBullet* pInstance = new CTurretBullet();
	return pInstance;
}

HRESULT CTurretBullet::Movement(float fDeltaTime)
{
	_vector vDir;
	vDir = m_vecPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fBulletSpeed);

	return S_OK;
}

void CTurretBullet::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
