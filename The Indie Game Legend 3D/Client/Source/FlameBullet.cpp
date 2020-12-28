#include "stdafx.h"
#include "..\Header\FlameBullet.h"




CFlameBullet::CFlameBullet()
{
}

CFlameBullet::CFlameBullet(const CFlameBullet & _rOther)
	:CBullet(_rOther)
{
}

HRESULT CFlameBullet::Animate(float _fDeltaTime)
{

	m_fTextureIndex += _fDeltaTime * m_fAnimateSpeed;
	if (m_nMaxTexture <= m_fTextureIndex)
	{
		m_fTextureIndex = 0.f;// 디버깅작업 생각해서 아예 0으로 초기화 
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("FlameBullet"))[(_int)m_fTextureIndex]);
	return S_OK;
}

void CFlameBullet::UpdateScale(float _fDeltaTime)
{
	m_fScale += m_fScaleDelta * _fDeltaTime;
	if (m_fMaxScale > m_fScale)
	{
		m_fScale = m_fMaxScale;
	}
	m_pTransform->Set_Scale(_vector(m_fScale, m_fScale, m_fScale));
}


HRESULT CFlameBullet::InitializePrototype()
{
	if (FAILED(CBullet::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFlameBullet::Awake()
{
	CBullet::Awake();

	m_sName = L"PlayerBullet";

	return S_OK;
}

HRESULT CFlameBullet::Start()
{
	m_pTransform->Set_Scale(_vector(8, 8, 8));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bullet"));
	SafeAddRef(m_pTexturePool);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("FlameBullet"))[0]);
	m_fMoveSpeed = 50.f;

	m_nMaxTexture = m_pTexturePool->GetTexture(TEXT("FlameBullet")).size();

	m_fLive = 0.8f;
	m_fAnimateOneCycleTime = 1.f;

	m_fAnimateSpeed = (m_nMaxTexture ) / 1.f * m_fAnimateOneCycleTime;
	m_pTransform->UpdateTransform();

	m_fMaxScale = 15.f;
	m_fScale = 8.f;
	m_fScaleDelta = 2.f;
	return S_OK;
}

UINT CFlameBullet::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	
	Animate(_fDeltaTime);
	UpdateScale(_fDeltaTime);

	m_pTransform->Add_Position(m_vDiraction* m_fMoveSpeed * _fDeltaTime);
	m_pTransform->UpdateTransform();
	m_pTransform->UpdateWorld();

	return OBJ_NOENVET;
}

UINT CFlameBullet::LateUpdate(const float _fDeltaTime)
{
	CBullet::LateUpdate(_fDeltaTime);

	m_fLivetime += _fDeltaTime;
	if (m_fLivetime >= m_fLive)
		m_bDead = true;

	return OBJ_NOENVET;
}

HRESULT CFlameBullet::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	IsBillboarding();
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();

	return S_OK;
}

HRESULT CFlameBullet::Fire()
{
	CBullet::Fire();
	return S_OK;
}

void CFlameBullet::OnCollision(CGameObject * _pGameObject)
{
}

void CFlameBullet::Free()
{
	CBullet::Free();
}

CGameObject * CFlameBullet::Clone()
{
	CGameObject* pIns = new CFlameBullet(*this);
	return pIns;
}

CFlameBullet * CFlameBullet::Create()
{
	CFlameBullet* pIns = new CFlameBullet();
	return pIns;
}

