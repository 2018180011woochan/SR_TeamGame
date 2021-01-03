#include "stdafx.h"
#include "Walker.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "WalkerBullet.h"
#include "Item.h"
#include "Blood.h"
#include "SmallExlode.h"

CWalker::CWalker()
	: m_pTexturePool(nullptr)
{
}

CWalker::CWalker(const CWalker & other)
	: CMonster(other)
{
}

HRESULT CWalker::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWalker::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Walker"));
	SafeAddRef(m_pTexturePool);

	m_fFireDeltaTime = 0.f;
	m_fFireSpeed = 3.f;

	m_fWalkDeltaTime = 0.f;
	m_fWalkSpeed = 0.5f;

	m_fMoveSpeed = 3.f;

	m_pTransform->Set_Scale(_vector(10, 10, 10));

	m_iHP = 10;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWalker::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;

	return S_OK;
}

UINT CWalker::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CMonster::Update(_fDeltaTime);

	//원래 위치
	if (FAILED(Movement(_fDeltaTime)))
		return 0;


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
		if (nIndex >= 3)
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

UINT CWalker::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CWalker::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CWalker::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
		m_iHP--;
		m_bHit = true;
		int iRandX = rand() % 5;
		int iRandY = rand() % 5;
		int iRandZ = rand() % 5;

		CSmallExlode* pSmallExlode = (CSmallExlode*)AddGameObject<CSmallExlode>();
		pSmallExlode->SetPos(_vector(m_pTransform->Get_Position().x + iRandX,
			m_pTransform->Get_Position().y + iRandY
			, m_pTransform->Get_Position().z + iRandZ));
	}
	if (m_iHP <= 0)
	{
		CItem* pHeart = (CItem*)AddGameObject<CItem>();
		pHeart->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		pHeart->SetItemType(EItemID::sprBigCoin);
		m_bDead = true;
		CSoundMgr::GetInstance()->Play(L"sfxKill.wav", CSoundMgr::MonsterKill);
		((CPlayer*)FindGameObjectOfType<CPlayer>())->AddSkillGauge(2);

	}
}

void CWalker::BulletFire()
{
	CWalkerBullet* pGameObject = (CWalkerBullet*)AddGameObject<CWalkerBullet>();
	pGameObject->SetTurretPos(m_pTransform->Get_Position());
}

HRESULT CWalker::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}


CGameObject * CWalker::Clone()
{
	CWalker* pClone = new CWalker(*this);
	return pClone;
}

CWalker * CWalker::Create()
{
	CWalker* pInstance = new CWalker();
	return pInstance;
}

void CWalker::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
