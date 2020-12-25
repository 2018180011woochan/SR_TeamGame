#include "stdafx.h"
#include "WormBossBody.h"
#include "WormBoss.h"
#include "MeshRenderer.h"

CWormBossBody::CWormBossBody()
	: m_pTexturePool(nullptr)
{
}

CWormBossBody::CWormBossBody(const CWormBossBody & other)
	: CMonster(other)
{
}

HRESULT CWormBossBody::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWormBossBody::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("WormBoss"));
	SafeAddRef(m_pTexturePool);

	m_pTransform->Set_Scale(_vector(5.f, 5.f, 5.f));
	m_pTransform->Set_Position(_vector(-15.f, 10.f, 10.f));


	vRightDir = (_vector(m_pTransform->Get_Position().x + 1, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z));
	vLeftDir = (_vector(m_pTransform->Get_Position().x - 1, m_pTransform->Get_Position().y, m_pTransform->Get_Position().z));

	m_fMoveSpeed = 2.f;

	nIndex = 0;
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWormBossBody::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Body"))[1]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Sphere"),BOUND::SPHERE);
	m_pCollider->m_bIsRigid = true;
	m_nTag = 0;

	m_fFrameSpeed = 0.5;
	m_fFrameDeltaTime = 0.f;

	return S_OK;
}

UINT CWormBossBody::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);

	//m_fFrameDeltaTime += _fDeltaTime;
	//if (m_fFrameSpeed <= m_fFrameDeltaTime)
	//{
	//	nIndex++;
	//	if (nIndex >= 4)
	//		nIndex = 0;
	//	m_fFrameDeltaTime -= m_fFrameSpeed;
	//}
	//m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("FaceDrill"))[nIndex]);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CWormBossBody::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CWormBossBody::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CWormBossBody::OnCollision(CGameObject * _pGameObject)
{
}

HRESULT CWormBossBody::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CWormBoss>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

CGameObject * CWormBossBody::Clone()
{
	CWormBossBody* pClone = new CWormBossBody(*this);
	return pClone;
}

CWormBossBody * CWormBossBody::Create()
{
	CWormBossBody* pInstance = new CWormBossBody();
	return pInstance;
}

void CWormBossBody::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
