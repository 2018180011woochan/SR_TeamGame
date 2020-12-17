#include "stdafx.h"
#include "Walker.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "WalkerBullet.h"

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
	//m_pTransform->Add_Position(_vector(-5.f, 5.f, 10.f));
	m_pTransform->Set_Position(_vector(-5.f, 5.f, 10.f));
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWalker::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CWalker::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	//���� ��ġ
	//if (FAILED(Movement(_fDeltaTime)))
	//	return 0;


	if (m_pTransform->Get_Position().y > 5.f || m_pTransform->Get_Position().y < 3.f)
	{
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 4.f, m_pTransform->Get_Position().z));
	}
	//����ȸ��
	if (GetKeyState(VK_F3))
	{

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
