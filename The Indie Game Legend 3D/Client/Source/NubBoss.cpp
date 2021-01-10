#include "stdafx.h"
#include "NubBoss.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "sqrNub.h"
#include "Item.h"
#include "Blood.h"
#include "Explosion.h"
#include "ExplosionBlue.h"
#include "BossHP.h"

CNubBoss::CNubBoss()
	: m_pTexturePool(nullptr)
{
}

CNubBoss::CNubBoss(const CNubBoss & other)
	: CMonster(other)
{
}

HRESULT CNubBoss::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNubBoss::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("NubBoss"));
	SafeAddRef(m_pTexturePool);

	m_fFireDeltaTime = 0.f;
	m_fFireSpeed = 30.f;

	m_fWalkDeltaTime = 0.f;
	m_fWalkSpeed = 0.5f;

	m_FLightningDeltaTime = 0.f;
	m_fLightningSpeed = 1.f;

	m_bisDash = false;
	m_bisStop = false;
	m_bDead = false;

	m_fMoveSpeed = 8.f;

	m_pTransform->Set_Scale(_vector(15,15,15));

	m_iHP =15;
	m_iMaxHP = m_iHP;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CNubBoss::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;

	//Test
	m_pBossHP = (CBossHP*)FindGameObjectOfType<CBossHP>();
	SafeAddRef(m_pBossHP);
	//m_pBossHP->Start();
	//m_pBossHP->SetEnable(true);

	m_pTransform->Add_Position(_vector(0, 10.f, 0));
	return S_OK;
}

UINT CNubBoss::Update(const float _fDeltaTime)
{

	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);

	/* 보스 hp 업데이트 */
	m_pBossHP->SetHPBar(float(m_iHP) / float(m_iMaxHP));
	if (m_bDead)
	{
		_vector vPos = m_pTransform->Get_WorldPosition();
		CItem* pItem = (CItem*)AddGameObject<CItem>();
		pItem->SetPosition(_vector(vPos.x, 15.f, vPos.z));
		pItem->SetItemType(EItemID::Disc);
		return OBJ_DEAD;
	}

	CMonster::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		nIndex++;
		m_fWalkDeltaTime -= m_fWalkSpeed;
		if (nIndex >= 4)
			nIndex = 0;
	}

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	// Nub생성인데 나중에 플레이어 총알 맞을때마다 하나씩 나오게 할까 생각중입니다!!!헤헤헤헤끌끌끌큭크루삥뽕빵
	m_fFireDeltaTime += _fDeltaTime;
	if (m_fFireSpeed <= m_fFireDeltaTime)
	{
		m_fFireDeltaTime -= m_fFireSpeed;
		CreateBabyNub();
	}

	if (isCloseToPlayer())
	{
		Dash(_fDeltaTime);
	}

	if (!isCloseToPlayer())
		m_fMoveSpeed = 8.f;
	m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 10.f, m_pTransform->Get_Position().z));

	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CNubBoss::LateUpdate(const float _fDeltaTime)
{
	CMsgManager::GetInstance()->Freeze(&_fDeltaTime);
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CNubBoss::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CNubBoss::OnCollision(CGameObject * _pGameObject)
{
	if (m_bHit == false && L"PlayerBullet" == _pGameObject->GetName())
	{
		m_bHit = true;
		//AddHp(-((CBullet*)_pGameObject)->GetBulletDmg());

		m_iHP -= ((CBullet*)_pGameObject)->GetBulletDmg();

	}
	else if (m_bHit == false && L"ExplosionBlue" == _pGameObject->GetName())
	{
		m_bHit = true;
		AddHp(-8);
	}
	if (m_iHP <= 0)
	{
		for (int i = 0; i < 10; i++)
		{
			int iRandX = rand() % 10;
			int iRandY = rand() % 10;
			int iRandZ = rand() % 10;

			CExplosion* pExplosion1 = (CExplosion*)AddGameObject<CExplosion>();
			pExplosion1->SetPos(_vector(m_pTransform->Get_Position().x + iRandX,
				m_pTransform->Get_Position().y + iRandY
				, m_pTransform->Get_Position().z + iRandZ));

		}

		for (int i = 0; i < 10; i++)
		{
			int iRandX = rand() % 10;
			int iRandY = rand() % 10;
			int iRandZ = rand() % 10;

			CExplosionBlue* pExplosion3 = (CExplosionBlue*)AddGameObject<CExplosionBlue>();
			pExplosion3->SetPos(_vector(m_pTransform->Get_Position().x + iRandX,
				m_pTransform->Get_Position().y + iRandY
				, m_pTransform->Get_Position().z + iRandZ));
		}


		CItem* pHeart = (CItem*)AddGameObject<CItem>();
		pHeart->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		pHeart->SetItemType(EItemID::Heart);
	    pHeart = (CItem*)AddGameObject<CItem>();
		pHeart->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		pHeart->SetItemType(EItemID::Heart);

		m_pBossHP->SetEnable(false);
		CSoundMgr::GetInstance()->Play(L"sfxKill.wav", CSoundMgr::MonsterKill);

		m_bDead = true;
	}	
}

void CNubBoss::OnEnable()
{
	if (nullptr != m_pBossHP)
	{
		m_pBossHP->SetEnable(true);
		m_pBossHP->SetHPBar(float(m_iHP) / float(m_iMaxHP));
	}
}

void CNubBoss::OnDisable()
{
	if (nullptr != m_pBossHP)
		m_pBossHP->SetEnable(false);
}


HRESULT CNubBoss::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CNubBoss::Dash(float _fDeltaTime)
{
	if (!m_bisDash)
		m_bisStop = true;
	while (m_bisStop)
	{
		iCnt++;
		if (iCnt <= 300)
		{
			m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
			m_fMoveSpeed = 0.f;
			return;
		}
		if (iCnt > 200)
		{
			iCnt = 0;
			m_bisDash = true;
			m_bisStop = false;
		}
	}
	while (m_bisDash)
	{
		if (iDashCnt < 50)
		{
			iDashCnt++;
			m_fMoveSpeed = 100.f;
			return;
		}
		if (iDashCnt >= 50)
		{
			iDashCnt = 0;
			m_fMoveSpeed = 8.f;
			m_bisDash = false;
		}
	}
	
}

bool CNubBoss::isCloseToPlayer()
{
	_vector vecPlayerPos = m_pPlayerTransform->Get_Position();
	float fDistance = 0.f;

	float fWidth = abs(vecPlayerPos.x - m_pTransform->Get_Position().x);
	float fHeight = abs(vecPlayerPos.z - m_pTransform->Get_Position().z);
	fDistance = sqrt((fWidth * fWidth) + (fHeight * fHeight));

	if (fDistance < 15.f)
		return true;

	return false;
}

void CNubBoss::CreateBabyNub()
{
	CsqrNub* pNubObject = (CsqrNub*)AddGameObject<CsqrNub>();
	pNubObject->SetEggPos(m_pTransform->Get_Position());
}


CGameObject * CNubBoss::Clone()
{
	CNubBoss* pClone = new CNubBoss(*this);
	return pClone;
}

CNubBoss * CNubBoss::Create()
{
	CNubBoss* pInstance = new CNubBoss();
	return pInstance;
}

void CNubBoss::Free()
{
	SafeAddRef(m_pBossHP);
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
