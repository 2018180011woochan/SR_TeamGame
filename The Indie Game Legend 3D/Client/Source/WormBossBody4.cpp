#include "stdafx.h"
#include "WormBossBody4.h"
#include "Player.h"
#include "MeshRenderer.h"
#include "WormBossBody3.h"
#include "WormBossTail.h"

CWormBossBody4::CWormBossBody4()
	: m_pTexturePool(nullptr)
{
}

CWormBossBody4::CWormBossBody4(const CWormBossBody4 & other)
	: CMonster(other)
{
}

HRESULT CWormBossBody4::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWormBossBody4::Awake()
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

	m_fMoveSpeed = 30.f;

	m_fJumpPower = 0.f;
	m_fJumpTime = 0.f;
	m_bJump = false;
	nIndex = 0;

	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 2.f;
	m_eCurDirState = LEFT;
	m_eCurPos = DOWNPOS;
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWormBossBody4::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Body"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	//m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;
	m_nTag = 0;

	m_fFrameSpeed = 0.5;
	m_fFrameDeltaTime = 0.f;

	pWormTail = (CWormBossTail*)AddGameObject<CWormBossTail>();
	pWormTail->Set_BodyPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 2.f, m_pTransform->Get_Position().z + 1.f), 1);


	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;
}

UINT CWormBossBody4::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	m_pWormBossTransform = (CTransform*)(FindGameObjectOfType<CWormBossBody3>()->GetComponent<CTransform>());

	m_fFrameDeltaTime += _fDeltaTime;
	if (m_fFrameSpeed <= m_fFrameDeltaTime)
	{
		if (false == m_bJump)
			m_bJump = true;

		m_fYTest = m_pTransform->Get_Position().y;

		m_fFrameDeltaTime -= m_fFrameSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Body"))[0]);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	Waving(_fDeltaTime);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CWormBossBody4::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CWormBossBody4::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CWormBossBody4::OnCollision(CGameObject * _pGameObject)
{
}

HRESULT CWormBossBody4::Movement(float fDeltaTime)
{
	_vector vDir = {0.f, 0.f, 0.f};
	_vector vMoveDir;

	if (m_eCurPos == UPPOS)
	{
		if (m_eCurDirState == LEFT || m_eCurDirState == UP)
			vDir = _vector(m_pWormBossTransform->Get_Position().x - nBodyIndex * 3.f, m_pWormBossTransform->Get_Position().y + nBodyIndex * 3.f, m_pWormBossTransform->Get_Position().z + 1.f);
		if (m_eCurDirState == RIGHT || m_eCurDirState == DOWN)
			vDir = _vector(m_pWormBossTransform->Get_Position().x + nBodyIndex * 3.f, m_pWormBossTransform->Get_Position().y + nBodyIndex * 3.f, m_pWormBossTransform->Get_Position().z + 1.f);
	}
		
		
	if (m_eCurPos == DOWNPOS)
	{
		if (m_eCurDirState == LEFT || m_eCurDirState == UP)
			vDir = _vector(m_pWormBossTransform->Get_Position().x - 3.f, m_pWormBossTransform->Get_Position().y - 3.f, m_pWormBossTransform->Get_Position().z + 1.f);
		if (m_eCurDirState == RIGHT || m_eCurDirState == DOWN)
			vDir = _vector(m_pWormBossTransform->Get_Position().x + 3.f, m_pWormBossTransform->Get_Position().y - 3.f, m_pWormBossTransform->Get_Position().z + 1.f);
	}

	vMoveDir = vDir - m_pTransform->Get_Position();
	D3DXVec3Normalize(&vMoveDir, &vMoveDir);
	m_pTransform->Add_Position(vMoveDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CWormBossBody4::Waving(float fDeltaTime)
{
	float fY = m_pTransform->Get_Position().y;

	if (fY < -2.f)
	{
		m_fJumpPower = 10.f;

		if (m_eCurDirState == LEFT)
			m_eCurDirState = RIGHT;
		else if (m_eCurDirState == RIGHT)
			m_eCurDirState = UP;
		else if (m_eCurDirState == UP)
			m_eCurDirState = DOWN;
		else if (m_eCurDirState == DOWN)
			m_eCurDirState = LEFT;

		m_eCurPos = DOWNPOS;
	}

	if (fY > 14.f)
	{
		m_fJumpPower = 0.f;
		m_eCurPos = UPPOS;
	}

	if (m_fJumpPower == 0.f)
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
	}

	if (m_fJumpPower == 10.f)
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

void CWormBossBody4::Set_BodyPos(const _vector _vPos, UINT _nBodyIndex)
{
	m_pTransform->Set_Position(_vPos);
	nBodyIndex = _nBodyIndex;
}

CGameObject * CWormBossBody4::Clone()
{
	CWormBossBody4* pClone = new CWormBossBody4(*this);
	return pClone;
}

CWormBossBody4 * CWormBossBody4::Create()
{
	CWormBossBody4* pInstance = new CWormBossBody4();
	return pInstance;
}

void CWormBossBody4::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
