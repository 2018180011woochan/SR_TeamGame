#include "stdafx.h"
#include "DoomBird.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "Item.h"
#include "Blood.h"

CDoomBird::CDoomBird()
	:m_pTexturePool(nullptr)
{
}

CDoomBird::CDoomBird(const CDoomBird & other)
	: CMonster(other)
{
}

HRESULT CDoomBird::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoomBird::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Doombird"));
	SafeAddRef(m_pTexturePool);

	float fNum = float(rand() % 5);

	m_fJumpPower = 10.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 15.f - fNum;
	m_bJump = false;
	nIndex = 0;

	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 0.6f;

	m_fWalkSpeed = 0.1f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;

	m_iHP = 2;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CDoomBird::Start()
{
	CMonster::Start();
	m_pTransform->Set_Scale(_vector(3, 3, 3));
	// Test
	m_pTransform->Set_Position(_vector(0.f, 10.f, 0.f));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;

	return S_OK;
}

UINT CDoomBird::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CMonster::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		nIndex++;
		if (nIndex >= 2)
			nIndex = 0;
		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);


	if (FAILED(Movement(_fDeltaTime)))
		return 0;

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
	return _uint();
}

UINT CDoomBird::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CDoomBird::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CDoomBird::OnCollision(CGameObject * _pGameObject)
{
	if (m_bHit == false && (L"PlayerBullet" == _pGameObject->GetName() || L"ExplosionBlue" == _pGameObject->GetName()))
	{
		m_iHP--;
		m_bHit = true;
		CBlood* pBlood = (CBlood*)AddGameObject<CBlood>();
		pBlood->SetPos(m_pTransform->Get_Position());
	}
	if (L"Player" == _pGameObject->GetName())
	{
		for (int i = 0; i < 10; i++)
		{
			int iRandX = rand() % 5;
			int iRandY = rand() % 5;
			int iRandZ = rand() % 2;

			CBlood* pBlood = (CBlood*)AddGameObject<CBlood>();
			pBlood->SetPos(_vector(m_pTransform->Get_Position().x + iRandX,
				m_pTransform->Get_Position().y + iRandY
				, m_pTransform->Get_Position().z + iRandZ));

		}

		m_bDead = true;
	}
	if (m_iHP <= 0)
	{
		CSoundMgr::GetInstance()->Play(L"sfxKill.wav", CSoundMgr::MonsterKill);
		((CPlayer*)FindGameObjectOfType<CPlayer>())->AddSkillGauge(1);

		m_bDead = true;
	}
}


HRESULT CDoomBird::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CDoomBird::Jumping(float fDeltaTime)
{
	float fY = 0.f;
	if (m_bJump)
	{
		fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			fY,
			m_pTransform->Get_Position().z));

		m_fJumpTime += 0.05f;

		if (fY < 8.f)
		{
			m_bJump = false;
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
				8.f,
				m_pTransform->Get_Position().z));
			m_fJumpTime = 0.f;
		}
	}
}

void CDoomBird::SetEggPos(const _vector _EggPos)
{
	m_pTransform->Set_Position(_EggPos);
}

CGameObject * CDoomBird::Clone()
{
	CDoomBird* pClone = new CDoomBird(*this);
	return pClone;
}

CDoomBird * CDoomBird::Create()
{
	CDoomBird* pInstance = new CDoomBird();
	return pInstance;
}

void CDoomBird::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
