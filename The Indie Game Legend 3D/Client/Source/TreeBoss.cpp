#include "stdafx.h"
#include "TreeBoss.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "RoboBird.h"
#include "DoomBird.h"
#include "RootAttack.h"
#include "Item.h"
#include "Blood.h"
#include "Explosion.h"
#include "ExplosionBlue.h"
#include "BossHP.h"

CTreeBoss::CTreeBoss()
	: m_pTexturePool(nullptr)
{
}

CTreeBoss::CTreeBoss(const CTreeBoss & other)
	: CMonster(other)
{
}

HRESULT CTreeBoss::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreeBoss::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("TreeBoss"));
	SafeAddRef(m_pTexturePool);

	m_fFireDeltaTime = 0.f;
	m_fFireSpeed = 7.f;
	m_pTransform->Set_Scale(_vector(20, 20, 20));

	m_fWinckDeltaTime = 0.f;
	m_fWinckSpeed = 2.5f;

	m_fCreateDeltaTime = 0.f;
	m_fCreateSpeed = 0.5f;

	m_fAttackDeltaTime = 0.f;
	m_fAttackSpeed = 0.25f;

	m_bisCreate = false;

	nIndex = 0;
	nCreateIndex = 0;
	//m_bEnable = true;
	m_nTag = 0;
	m_iHP = 30;
	m_iMaxHP = m_iHP;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CTreeBoss::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = false;

	m_pTransform->Add_Position(_vector(10, 10, 10));

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	//Test
	m_pBossHP = (CBossHP*)FindGameObjectOfType<CBossHP>();
	SafeAddRef(m_pBossHP);

	return S_OK;
}

UINT CTreeBoss::Update(const float _fDeltaTime)
{
	/* 보스 hp 업데이트 */
	m_pBossHP->SetHPBar(float(m_iHP / m_iMaxHP));

	if (m_bDead)
		return OBJ_DEAD;

	CMonster::Update(_fDeltaTime);

	m_fWinckDeltaTime += _fDeltaTime;
	if (m_fWinckSpeed <= m_fWinckDeltaTime)
	{
		nIndex++;
		if (nIndex >= 7)
			nIndex = 0;
		m_fWinckDeltaTime -= m_fWinckSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	RootAttack(_fDeltaTime);
	YukPoongHyul(_fDeltaTime);

	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CTreeBoss::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CTreeBoss::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CTreeBoss::OnCollision(CGameObject * _pGameObject)
{
	if (m_bHit == false && (L"PlayerBullet" == _pGameObject->GetName() || L"ExplosionBlue" == _pGameObject->GetName()))
	{
		m_iHP--;
		m_bHit = true;
		CSoundMgr::GetInstance()->Play(L"sfxPlantHit.mp3", CSoundMgr::MonsterHitP);
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
		pHeart->SetItemType(EItemID::Ammo);
		pHeart = (CItem*)AddGameObject<CItem>();
		pHeart->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		pHeart->SetItemType(EItemID::Ammo);
		pHeart = (CItem*)AddGameObject<CItem>();
		pHeart->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		pHeart->SetItemType(EItemID::Ammo);

		CSoundMgr::GetInstance()->Play(L"sfxKill.wav", CSoundMgr::MonsterKill);
		m_pBossHP->SetEnable(false);
		m_bDead = true;
	}
}

void CTreeBoss::YukPoongHyul(float _fDeltaTime)
{
	if (nIndex == 6)
	{
		int iNum = rand() % 5;
		m_fCreateDeltaTime += _fDeltaTime;
		if (m_fCreateSpeed <= m_fCreateDeltaTime)
		{
			CRoboBird* pRoboBird = (CRoboBird*)AddGameObject<CRoboBird>();
			pRoboBird->SetEggPos(_vector(m_pTransform->Get_Position().x + iNum, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z));

			CDoomBird* pDoomBird = (CDoomBird*)AddGameObject<CDoomBird>();
			pDoomBird->SetEggPos(_vector(m_pTransform->Get_Position().x - iNum, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z));

			m_fCreateDeltaTime -= m_fCreateSpeed;
		}
	}
}

void CTreeBoss::RootAttack(float _fDeltaTime)
{
	_vector RootPos;
	if (nIndex == 0)
	{
		m_fAttackDeltaTime += _fDeltaTime;
		if (m_fAttackSpeed <= m_fAttackDeltaTime)
		{
			CRootAttack* pRootAttack = (CRootAttack*)AddGameObject<CRootAttack>();
			RootPos = { m_pTransform->Get_Position().x, m_pTransform->Get_Position().y - 7.f, m_pTransform->Get_Position().z };
			pRootAttack->SetRootPos(RootPos);

			m_fAttackDeltaTime -= m_fAttackSpeed;
		}
	}
}


CGameObject * CTreeBoss::Clone()
{
	CTreeBoss* pClone = new CTreeBoss(*this);
	return pClone;
}

CTreeBoss * CTreeBoss::Create()
{
	CTreeBoss* pInstance = new CTreeBoss();
	return pInstance;
}

void CTreeBoss::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
