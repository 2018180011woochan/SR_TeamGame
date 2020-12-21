#include "stdafx.h"
#include "..\Header\WormBoss.h"
#include "Player.h"
#include "MeshRenderer.h"

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
	m_pTransform->Set_Position(_vector(-5.f, 10.f, 10.f));


	vRightDir = (_vector(m_pTransform->Get_Position().x + 1, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z));
	vLeftDir = (_vector(m_pTransform->Get_Position().x - 1, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z));

	m_fMoveSpeed = 2.f;

	m_fJumpPower = 0.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 8.f;
	m_bJump = false;
	nIndex = 0;

	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 2.f;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWormBoss::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("FaceSide"))[0]);

	m_fFrameSpeed = 0.5;
	m_fFrameDeltaTime = 0.f;

	return S_OK;
}

UINT CWormBoss::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	m_fFrameDeltaTime += _fDeltaTime;
	if (m_fFrameSpeed <= m_fFrameDeltaTime)
	{
		nIndex++;
		if (false == m_bJump)
			m_bJump = true;

		m_fYTest = m_pTransform->Get_Position().y;

		if (nIndex >= 4)
			nIndex = 0;

		m_fFrameDeltaTime -= m_fFrameSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("FaceSide"))[nIndex]);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	Waving(_fDeltaTime);
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

HRESULT CWormBoss::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());
	_vector vPlayerTransPos = _vector(m_pPlayerTransform->Get_Position().x + 10.f, m_pPlayerTransform->Get_Position().y, m_pPlayerTransform->Get_Position().z + 10.f);
	_vector vDir;
	vDir = vPlayerTransPos - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CWormBoss::Waving(float fDeltaTime)
{
	float fY = 0.f;
	if (m_bJump)
	{
		fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			fY,
			m_pTransform->Get_Position().z));

		m_fJumpTime += 0.001f;

		if (fY < -10.f)
		{
			m_fJumpPower = 10.f;
			m_bJump = false;
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
				-10.f,
				m_pTransform->Get_Position().z));
			m_fJumpTime = 0.f;
		}

		if (fY > -5.f && fY < 5.f)
		{
			m_pTransform->Set_RotationZ(45.f);
			//m_pTransform->Add_RotationZ(45.f);
		}
		if (fY > 20.f)
		{
			m_fJumpPower = 0.f;
		}
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
