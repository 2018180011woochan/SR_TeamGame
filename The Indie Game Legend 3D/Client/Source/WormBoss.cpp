#include "stdafx.h"
#include "..\Header\WormBoss.h"
#include "Player.h"
#include "MeshRenderer.h"
#include "WormBossBody.h"

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
	m_pTransform->Set_Position(_vector(-20.f, 10.f, -20.f));


	vRightDir = (_vector(m_pTransform->Get_Position().x + 1, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z));
	vLeftDir = (_vector(m_pTransform->Get_Position().x - 1, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z));

	m_fMoveSpeed = 2.f;

	m_fJumpPower = 0.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 8.f;
	m_bJump = false;
	nIndex = 0;
	m_bDashAttack = false;

	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 2.f;
	m_eCurDirState = LEFT;
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWormBoss::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("FaceSide"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	//m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;
	m_nTag = 0;

	m_fFrameSpeed = 0.5;
	m_fFrameDeltaTime = 0.f;

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	pWormBody = (CWormBossBody*)AddGameObject<CWormBossBody>();
	pWormBody->Set_BodyPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 2.f, m_pTransform->Get_Position().z + 1.f), 1);


	return S_OK;
}

UINT CWormBoss::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	m_fFrameDeltaTime += _fDeltaTime;
	if (m_fFrameSpeed <= m_fFrameDeltaTime)
	{
		nIndex++;

		if (nIndex >= 4)
			nIndex = 0;

		m_fFrameDeltaTime -= m_fFrameSpeed;
	}
	m_fYTest = m_pTransform->Get_Position().y;
	

	if (m_bDashAttack)
		ChaseAttack(_fDeltaTime);

	if (!m_bDashAttack)
	{
		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("FaceSide"))[nIndex]);
		if (FAILED(Movement(_fDeltaTime)))
			return 0;

		Waving(_fDeltaTime);
	}
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CWormBoss::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CWormBoss::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CWormBoss::OnCollision(CGameObject * _pGameObject)
{
}

HRESULT CWormBoss::Movement(float fDeltaTime)
{
	_vector vDir = { 0.f, 0.f, 0.f };

	if (m_eCurDirState == LEFT)
		vDir = _vector(m_pTransform->Get_Position().x + 100000, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z);
	if (m_eCurDirState == RIGHT)
		vDir = _vector(m_pTransform->Get_Position().x - 100000, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z);
	if (m_eCurDirState == UP)
		vDir = _vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z + 100000);
	if (m_eCurDirState == DOWN)
		vDir = _vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z - 100000);
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CWormBoss::Waving(float fDeltaTime)
{
	if (m_bJump)
		m_pTransform->Add_Position(_vector(0.f, 0.1f, 0.f));
	if (!m_bJump)
		m_pTransform->Add_Position(_vector(0.f, -0.1f, 0.f));

	float fY = m_pTransform->Get_Position().y;
	
	//fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
	//m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
	//	fY,
	//	m_pTransform->Get_Position().z));

	//m_fJumpTime += 0.0005f;

	if (fY < -10.f)
	{
		//m_fJumpPower = 5.f;
		//m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
		//	-10.f,
		//	m_pTransform->Get_Position().z));
		//m_fJumpTime = 0.f;

		m_bJump = true;

		if (m_eCurDirState == LEFT)
			m_eCurDirState = RIGHT;
		else if (m_eCurDirState == RIGHT)
			m_eCurDirState = UP;
		else if (m_eCurDirState == UP)
			m_eCurDirState = DOWN;
		else if (m_eCurDirState == DOWN)
			m_eCurDirState = LEFT;
	}


	if (fY > 20.f)
	{
		m_fJumpPower = 0.f;
		m_bJump = false;
	}

	if (/*m_fJumpPower == 0.f*/!m_bJump)
	{
		if (m_eCurDirState == LEFT || m_eCurDirState == UP)
		{
			if (fY > 5.f && fY < 10.f)
				m_pTransform->Set_RotationZ(-45.f);
			if (fY > 0.f && fY < 5.f)
				m_pTransform->Set_RotationZ(-90.f);
			if (fY > 10.f && fY < 20.f)
				m_pTransform->Set_RotationZ(0.f);
		}
		if (m_eCurDirState == RIGHT || m_eCurDirState == DOWN)
		{
			if (fY > 5.f && fY < 10.f)
				m_pTransform->Set_RotationZ(-135.f);
			if (fY > 0.f && fY < 5.f)
				m_pTransform->Set_RotationZ(-90.f);
			if (fY > 10.f && fY < 20.f)
				m_pTransform->Set_RotationZ(180.f);
		}
		if (m_eCurDirState == RIGHT)
		{
			if (fY > 7.f && fY < 8.f)
			{
				m_bDashAttack = true;
				m_fMoveSpeed = 25.f;
			}
		}
	}

	if (/*m_fJumpPower == 5.f*/m_bJump)
	{
		if (m_eCurDirState == LEFT || m_eCurDirState == UP)
		{
			if (fY > -5.f && fY < 5.f)
				m_pTransform->Set_RotationZ(90.f);
			//if (fY > 5.f && fY < 10.f)
			//	m_pTransform->Set_RotationZ(135.f);
			if (fY > 5.f && fY < 10.f)
				m_pTransform->Set_RotationZ(45.f);
			if (fY > 10.f && fY < 20.f)
				m_pTransform->Set_RotationZ(0.f);
		}
		if (m_eCurDirState == RIGHT || m_eCurDirState == DOWN)
		{
			if (fY > -5.f && fY < 5.f)
				m_pTransform->Set_RotationZ(90.f);
			if (fY > 5.f && fY < 10.f)
				m_pTransform->Set_RotationZ(-135.f);
			if (fY > 10.f && fY < 20.f)
				m_pTransform->Set_RotationZ(180.f);
		}



	}



}

void CWormBoss::ChaseAttack(float fDeltaTime)
{
	m_fFrameDeltaTime += fDeltaTime;
	if (m_fFrameSpeed <= m_fFrameDeltaTime)
	{
		nIndex++;

		if (nIndex >= 4)
			nIndex = 0;

		m_fFrameDeltaTime -= m_fFrameSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("FaceDrill"))[nIndex]);

	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	m_iChaseTime++;
	
	if (m_iChaseTime > 1000)
	{
		m_iChaseTime = 0;
		m_bDashAttack = false;
		m_eCurDirState = UP;
		m_fMoveSpeed = 8.f;
	}
}

CGameObject * CWormBoss::Clone()
{
	CWormBoss* pClone = new CWormBoss(*this);
	return pClone;
}

CWormBoss * CWormBoss::Create()
{
	CWormBoss* pInstance = new CWormBoss();
	return pInstance;
}

void CWormBoss::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
