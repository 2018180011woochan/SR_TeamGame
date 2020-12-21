#include "stdafx.h"
#include "NubBoss.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "sqrNub.h"

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
	m_fFireSpeed = 3.f;

	m_fWalkDeltaTime = 0.f;
	m_fWalkSpeed = 0.5f;

	m_FLightningDeltaTime = 0.f;
	m_fLightningSpeed = 1.f;

	m_bisDash = false;
	m_bisStop = false;

	m_fMoveSpeed = 8.f;

	m_pTransform->Set_Scale(_vector(20, 20, 20));
	//m_pTransform->Add_Position(_vector(-5.f, 5.f, 10.f));
	m_pTransform->Set_Position(_vector(-5.f, 10.f, 50.f));
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CNubBoss::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CNubBoss::Update(const float _fDeltaTime)
{
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

	// Nub�����ε� ���߿� �÷��̾� �Ѿ� ���������� �ϳ��� ������ �ұ� �������Դϴ�!!!�������������Ūũ���ͻ�
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

	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CNubBoss::LateUpdate(const float _fDeltaTime)
{
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

	if (fDistance < 40.f)
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
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}