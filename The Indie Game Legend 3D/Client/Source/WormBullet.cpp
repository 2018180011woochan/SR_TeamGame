#include "stdafx.h"
#include "..\Header\WormBullet.h"
#include "TexturePoolManager.h"

CWormBullet::CWormBullet()
	: m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nMaxFrame(0)
	, m_nIndex(0)
	, m_fTime(0.f)
	, m_fAnimateSpeed(0.1f)
	, m_vDir(0.f,0.f,0.f)
	, m_fMoveSpeed(75.f)
{
}

CWormBullet::CWormBullet(const CWormBullet & _rOther)
	: CMonster(_rOther)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nMaxFrame(0)
	, m_nIndex(0)
	, m_fTime(0.f)
	, m_fAnimateSpeed(0.1f)
	, m_vDir(0.f, 0.f, 0.f)
	, m_fMoveSpeed(75.f)
{
}

void CWormBullet::Free()
{
	CMonster::Free();
	SafeRelease(m_pTexturePool);
}

CWormBullet * CWormBullet::Create()
{
	CWormBullet* pInstance = new CWormBullet;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CWormBullet::Clone()
{
	CWormBullet* pClone = new CWormBullet(*this);
	return pClone;
}

HRESULT CWormBullet::InitializePrototype()
{
	CMonster::InitializePrototype();
	return S_OK;
}

HRESULT CWormBullet::Awake()
{
	CMonster::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	m_pTransform->Set_Scale(D3DXVECTOR3(3.2f, 3.2f, 1.f));
	m_pTransform->UpdateTransform();
	m_pCollider = (CCollider*)AddComponent<CCollider>();

	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->SetRadius(2.f);
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWormBullet::Start()
{
	CMonster::Start();

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("FireBall"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("Idle");

	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);

	//////////////
	m_bEnable = true;
	return S_OK;
}

UINT CWormBullet::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CMonster::Update(_fDeltaTime);
	Move(_fDeltaTime);
	Animate(_fDeltaTime);
	return 0;
}

UINT CWormBullet::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CWormBullet::Render()
{
	CMonster::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

void CWormBullet::OnCollision(CGameObject * _pGameObject)
{
	if (L"Obstacle" == _pGameObject->GetName()	|| L"Player" == _pGameObject->GetName()
		|| L"Floor" == _pGameObject->GetName() || L"Wall" == _pGameObject->GetName())
	{
		m_bDead = true;
	}
}

void CWormBullet::SetBullet(D3DXVECTOR3 _vStart, D3DXVECTOR3 _vTarget)
{
	m_pTransform->Set_Position(_vStart);

	m_vDir = _vTarget - _vStart;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
}

void CWormBullet::Move(const float _fDeltaTime)
{
	if (D3DXVECTOR3(0.f, 0.f, 0.f) == m_vDir)
		return;

	m_pTransform->Add_Position(m_vDir * m_fMoveSpeed * _fDeltaTime);

}

void CWormBullet::Animate(const float _fDeltaTime)
{
	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimateSpeed)
	{
		m_fTime -= m_fAnimateSpeed;

		++m_nIndex;

		if (m_nIndex >= m_nMaxFrame)
			m_nIndex = 0;

		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}
}
