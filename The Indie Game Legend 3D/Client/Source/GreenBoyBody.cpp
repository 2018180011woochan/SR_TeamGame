#include "stdafx.h"
#include "GreenBoyBody.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "Item.h"
#include "Blood.h"
#include "Flame.h"
#include "GreenBoyLeftHand.h"
#include "GreenBoyRightHand.h"
#include "GreenBoyUpHand.h"
#include "GreenBoyDownHand.h"
#include "GreenBoyHead.h"
#include "GreenBoyFace.h"
#include "Explosion.h"
#include "ExplosionBlue.h"
#include "BossHP.h"

CGreenBoyBody::CGreenBoyBody()
	:m_pTexturePool(nullptr)
{
}

CGreenBoyBody::CGreenBoyBody(const CGreenBoyBody & other)
	: CMonster(other)
{
}

HRESULT CGreenBoyBody::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGreenBoyBody::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("GreenBoy"));
	SafeAddRef(m_pTexturePool);

	m_fJumpPower = 4.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 8.f;
	m_bJump = false;
	nIndex = 0;

	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 1.f;

	m_fWalkSpeed = 0.5f;
	m_fWalkDeltaTime = 0.f;

	m_fAttackSpeed = 3.f;
	m_fAttackDeltaTime = 0.f;

	m_fYTest = 0.f;

	m_bDead = false;

	m_iHP = 30;
	m_iMaxHP = m_iHP;
	m_bIsAttack = false;
	m_eAttackBeem = LEFTBEEM;

	m_eRenderID = ERenderID::Alpha;

	
	return S_OK;
}

HRESULT CGreenBoyBody::Start()
{
	CMonster::Start();
	m_pTransform->Set_Scale(_vector(10, 10, 10));
	// Test
	m_pTransform->Set_Position(_vector(-240.f, 12.f, -240.f));
	//m_pTransform->Set_Position(_vector(0, 12.f, 0.f));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Body"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;

	pRightHand = (CGreenBoyRightHand*)FindGameObjectOfType<CGreenBoyRightHand>();
	pLeftHand = (CGreenBoyLeftHand*)FindGameObjectOfType<CGreenBoyLeftHand>();
	pUpHand = (CGreenBoyUpHand*)FindGameObjectOfType<CGreenBoyUpHand>();
	pDownHand = (CGreenBoyDownHand*)FindGameObjectOfType<CGreenBoyDownHand>();
	pHead = (CGreenBoyHead*)FindGameObjectOfType<CGreenBoyHead>();
	pFace = (CGreenBoyFace*)FindGameObjectOfType<CGreenBoyFace>();
	pFlame = (CFlame*)FindGameObjectOfType<CFlame>();

	m_pBossHP = (CBossHP*)FindGameObjectOfType<CBossHP>();
	SafeAddRef(m_pBossHP);
	//m_nTag = 0;
	return S_OK;
}

UINT CGreenBoyBody::Update(const float _fDeltaTime)
{
	/* 보스 hp 업데이트 */
	m_pBossHP->SetHPBar(float(m_iHP / m_iMaxHP));

	if (m_bDead)
		return OBJ_DEAD;
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());
	CMonster::Update(_fDeltaTime);

	// 나중에 head 다른 조각 찾으면??
	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		//nIndex++;
		//if (nIndex >= 2)
		//	nIndex = 0;
		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Body"))[nIndex]);

	if (!isCloseToPlayer())
	{
		if (FAILED(Movement(_fDeltaTime)))
			return 0;
	}

	m_fJumpDeltaTime += _fDeltaTime;
	if (m_fJumpSpeed <= m_fJumpDeltaTime)
	{
		m_fJumpDeltaTime -= m_fJumpSpeed;

		if (false == m_bJump)
			m_bJump = true;

		m_fYTest = m_pTransform->Get_Position().y;
	}
	m_pTransform->UpdateTransform();
	Jumping(_fDeltaTime);

	m_fAttackDeltaTime += _fDeltaTime;
	if (m_fAttackSpeed <= m_fAttackDeltaTime)
	{
		if (m_eAttackBeem == LEFTBEEM)
			m_eAttackBeem = RIGHTBEEM;
		else if (m_eAttackBeem == RIGHTBEEM)
			m_eAttackBeem = DOUBLEBEEM;
		else if (m_eAttackBeem == DOUBLEBEEM)
			m_eAttackBeem = BEEMEND;
		else if (m_eAttackBeem == BEEMEND)
			m_eAttackBeem = LEFTBEEM;

		m_fAttackDeltaTime -= m_fAttackSpeed;
	}

	if (m_eAttackBeem == LEFTBEEM)
	{
		pLeftHand->SetIsCrush(false);
		pRightHand->SetIsCrush(false);
		pUpHand->SetIsCrush(false);
		pDownHand->SetIsCrush(false);

		pLeftHand->SetIsAttack(true);
		pRightHand->SetIsAttack(false);
		pUpHand->SetIsAttack(true);
		pDownHand->SetIsAttack(false);
	}
	if (m_eAttackBeem == RIGHTBEEM)
	{
		pLeftHand->SetIsAttack(false);
		pRightHand->SetIsAttack(true);

		pUpHand->SetIsAttack(false);
		pDownHand->SetIsAttack(true);
	}
	if (m_eAttackBeem == DOUBLEBEEM)
	{
		pLeftHand->SetIsAttack(true);
		pRightHand->SetIsAttack(true);

		pUpHand->SetIsAttack(true);
		pDownHand->SetIsAttack(true);
	}
	if (m_eAttackBeem == BEEMEND)
	{
		pLeftHand->SetIsAttack(false);
		pRightHand->SetIsAttack(false);
		
		pUpHand->SetIsAttack(false);
		pDownHand->SetIsAttack(false);

		pLeftHand->SetIsCrush(true);
		pRightHand->SetIsCrush(true);

		pUpHand->SetIsCrush(true);
		pDownHand->SetIsCrush(true);
	}

	return _uint();
}

UINT CGreenBoyBody::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CGreenBoyBody::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CGreenBoyBody::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
		m_iHP--;
		CBlood* pBlood = (CBlood*)AddGameObject<CBlood>();
		pBlood->SetPos(m_pTransform->Get_Position());
	}
	if (m_iHP <= 0)
	{
		pRightHand->SetBossDead(true);
		pLeftHand->SetBossDead(true);
		pUpHand->SetBossDead(true);
		pDownHand->SetBossDead(true);
		pHead->SetBossDead(true);
		pFace->SetBossDead(true);
		pFlame->SetBossDead(true);

		for (int i = 0; i < 10; i++)
		{
			int iRandX = rand() % 15;
			int iRandY = rand() % 15;
			int iRandZ = rand() % 15;

			CExplosion* pExplosion1 = (CExplosion*)AddGameObject<CExplosion>();
			pExplosion1->SetPos(_vector(m_pTransform->Get_Position().x + iRandX,
				m_pTransform->Get_Position().y + iRandY
				, m_pTransform->Get_Position().z + iRandZ));

		}

		for (int i = 0; i < 10; i++)
		{
			int iRandX = rand() % 15;
			int iRandY = rand() % 15;
			int iRandZ = rand() % 15;

			CExplosionBlue* pExplosion3 = (CExplosionBlue*)AddGameObject<CExplosionBlue>();
			pExplosion3->SetPos(_vector(m_pTransform->Get_Position().x + iRandX,
				m_pTransform->Get_Position().y + iRandY
				, m_pTransform->Get_Position().z + iRandZ));
		}
		m_pBossHP->SetEnable(false);
		m_bDead = true;
	}
}


HRESULT CGreenBoyBody::Movement(float fDeltaTime)
{
	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CGreenBoyBody::Jumping(float fDeltaTime)
{
	float fY = 0.f;
	if (m_bJump)
	{
		fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			fY,
			m_pTransform->Get_Position().z));

		m_fJumpTime += 0.01f;

		if (fY < 11.f)
		{
			m_bJump = false;
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
				11.f,
				m_pTransform->Get_Position().z));
			m_fJumpTime = 0.f;
		}
	}
}

bool CGreenBoyBody::isCloseToPlayer()
{
	_vector vecPlayerPos = m_pPlayerTransform->Get_Position();
	float fDistance = 0.f;

	float fWidth = abs(vecPlayerPos.x - m_pTransform->Get_Position().x);
	float fHeight = abs(vecPlayerPos.z - m_pTransform->Get_Position().z);
	fDistance = sqrt((fWidth * fWidth) + (fHeight * fHeight));

	if (fDistance < 25.f)
		return true;

	return false;
}

void CGreenBoyBody::SetPos(const _vector _Pos)
{
	m_pTransform->Set_Position(_Pos);
}

CGameObject * CGreenBoyBody::Clone()
{
	CGreenBoyBody* pClone = new CGreenBoyBody(*this);
	return pClone;
}

CGreenBoyBody * CGreenBoyBody::Create()
{
	CGreenBoyBody* pInstance = new CGreenBoyBody();
	return pInstance;
}

void CGreenBoyBody::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
