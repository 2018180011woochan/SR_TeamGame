#include "stdafx.h"
#include "..\Header\Turret.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "TurretBullet.h"
#include "Item.h"
#include "SmallExlode.h"

CTurret::CTurret()
	: m_pTexturePool(nullptr)
{
}

CTurret::CTurret(const CTurret & other)
	: CMonster(other)
{
}

HRESULT CTurret::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTurret::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Turret"));
	SafeAddRef(m_pTexturePool);

	m_fFireDeltaTime = 0;
	m_fFireSpeed = 3;
	m_pTransform->Set_Scale(_vector(10, 10, 10));


	m_iHP = 5;
	m_bDead = false;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CTurret::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;

	return S_OK;
}

UINT CTurret::Update(const float _fDeltaTime)
{
	if (m_bDead)
	{
		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[1]);
	}

	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	CMonster::Update(_fDeltaTime);



	m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 5.f, m_pTransform->Get_Position().z));


	if (!m_bDead)
	{
		//off Test
		m_fFireDeltaTime += _fDeltaTime;
		if (m_fFireSpeed <= m_fFireDeltaTime)
		{
			m_fFireDeltaTime -= m_fFireSpeed;
			BulletFire();
			CSoundMgr::GetInstance()->Play(L"Fat.wav", CSoundMgr::MONSTER_BULLET);

		}
	}

	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CTurret::LateUpdate(const float _fDeltaTime)
{
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CTurret::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CTurret::OnCollision(CGameObject * _pGameObject)
{


	if (m_bHit == false && L"PlayerBullet" == _pGameObject->GetName())
	{
		m_bHit = true;
		AddHp(-((CBullet*)_pGameObject)->GetBulletDmg());
		int iRandX = rand() % 5;
		int iRandY = rand() % 5;
		int iRandZ = rand() % 5;

		CSmallExlode* pSmallExlode = (CSmallExlode*)AddGameObject<CSmallExlode>();
		pSmallExlode->SetPos(_vector(m_pTransform->Get_Position().x + iRandX,
			m_pTransform->Get_Position().y + iRandY
			, m_pTransform->Get_Position().z + iRandZ));
		CSoundMgr::GetInstance()->Play(L"sfxMetalHit1.mp3", CSoundMgr::MonsterHitM);

	}
	else if (m_bHit == false && L"ExplosionBlue" == _pGameObject->GetName())
	{
		m_bHit = true;
		AddHp(-8);
	}

	if (m_iHP <= 0)
	{
		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[1]);
		m_bDead = true;
		CSoundMgr::GetInstance()->Play(L"sfxExplode.wav", CSoundMgr::MonsterKill);
	}
}

void CTurret::BulletFire()
{
	CTurretBullet* pGameObject = (CTurretBullet*)AddGameObject<CTurretBullet>();
	pGameObject->SetTurretPos(m_pTransform->Get_Position());
}


CGameObject * CTurret::Clone()
{
	CTurret* pClone = new CTurret(*this);
	return pClone;
}

CTurret * CTurret::Create()
{
	CTurret* pInstance = new CTurret();
	return pInstance;
}

void CTurret::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
