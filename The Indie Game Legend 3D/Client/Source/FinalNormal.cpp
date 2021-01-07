#include "stdafx.h"
#include "..\Header\FinalNormal.h"
#include "TexturePoolManager.h"

CFinalNormal::CFinalNormal()
	: m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_fMoveSpeed(75.f)
	, m_bRemove(false)
{
}

CFinalNormal::CFinalNormal(const CFinalNormal & _rOther)
	: CGameObject(_rOther)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_fMoveSpeed(75.f)
	, m_bRemove(false)
{
}

void CFinalNormal::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CFinalNormal * CFinalNormal::Create()
{
	CFinalNormal* pInstance = new CFinalNormal;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CFinalNormal::Clone()
{
	CFinalNormal* pClone = new CFinalNormal(*this);
	return pClone;
}

HRESULT CFinalNormal::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CFinalNormal::Awake()
{
	CGameObject::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Rotation(D3DXVECTOR3(90.f, 180.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(1.8f, 1.65f, 1.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::Alpha;

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	return S_OK;
}

HRESULT CFinalNormal::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("FinalBossBullet"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("Normal");
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	return S_OK;
}

UINT CFinalNormal::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	if (true == m_bRemove)
		return OBJ_DEAD;
	Animate(_fDeltaTime);
	Move(_fDeltaTime);
	return 0;
}

UINT CFinalNormal::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CFinalNormal::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

void CFinalNormal::SetBullet(D3DXVECTOR3 _vStart)
{
	m_pTransform->Set_Position(_vStart);
	m_pTransform->UpdateTransform();
	
	D3DXVECTOR3 vDefault = D3DXVECTOR3(-1.f, 0.f, 0.f);

	float fDegree = -10.f + rand() % 20;

	D3DXMATRIX matRY;
	D3DXMatrixRotationY(&matRY, D3DXToRadian(fDegree));

	D3DXVec3TransformNormal(&vDefault, &vDefault, &matRY);

	D3DXVec3Normalize(&m_vMoveDirection, &vDefault);
}

void CFinalNormal::Animate(const float _fDeltaTime)
{
	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimationSpeed)
	{
		m_fTime -= m_fAnimationSpeed;

		++m_nIndex;

		if (m_nIndex >= m_nMaxFrame)
			m_nIndex = 0;

		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}
}

void CFinalNormal::Move(const float _fDeltaTime)
{
	m_pTransform->Add_Position(m_vMoveDirection * m_fMoveSpeed * _fDeltaTime);
	m_pTransform->UpdateTransform();

	if (m_pTransform->Get_Position().x < -45.f)
		m_bRemove = true;
	
}

