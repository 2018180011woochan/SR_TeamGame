#include "stdafx.h"
#include "WalkerBoss.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "WalkerBullet.h"
#include "WalkBossBullet.h"

CWalkerBoss::CWalkerBoss()
	: m_pTexturePool(nullptr)
{
}

CWalkerBoss::CWalkerBoss(const CWalkerBoss & other)
	: CMonster(other)
{
}

HRESULT CWalkerBoss::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWalkerBoss::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("WallkerBoss"));
	SafeAddRef(m_pTexturePool);

	m_fFireDeltaTime = 0.f;
	m_fFireSpeed = 3.f;

	m_fWalkDeltaTime = 0.f;
	m_fWalkSpeed = 0.5f;

	m_fMoveSpeed = 3.f;

	m_pTransform->Set_Scale(_vector(10, 10, 10));
	//m_pTransform->Add_Position(_vector(-5.f, 5.f, 10.f));
	m_pTransform->Set_Position(_vector(-5.f, 5.f, 10.f));
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWalkerBoss::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CWalkerBoss::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	//원래 위치
	//if (FAILED(Movement(_fDeltaTime)))
	//	return 0;


	if (m_pTransform->Get_Position().y > 5.f || m_pTransform->Get_Position().y < 3.f)
	{
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 4.f, m_pTransform->Get_Position().z));
	}


	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		m_fWalkDeltaTime -= m_fWalkSpeed;
		if (nIndex >= 7)
			nIndex = 0;
		nIndex++;
	}

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	m_fFireDeltaTime += _fDeltaTime;
	if (m_fFireSpeed <= m_fFireDeltaTime)
	{
		m_fFireDeltaTime -= m_fFireSpeed;
		BulletFire();
	}


	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CWalkerBoss::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CWalkerBoss::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CWalkerBoss::BulletFire()
{
	CWalkBossBullet* pGameObject = (CWalkBossBullet*)AddGameObject<CWalkBossBullet>();
	pGameObject->SetWalkBossPos(m_pTransform->Get_Position());
}

HRESULT CWalkerBoss::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}


CGameObject * CWalkerBoss::Clone()
{
	CWalkerBoss* pClone = new CWalkerBoss(*this);
	return pClone;
}

CWalkerBoss * CWalkerBoss::Create()
{
	CWalkerBoss* pInstance = new CWalkerBoss();
	return pInstance;
}

void CWalkerBoss::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
