#include "stdafx.h"
#include "..\Header\FinalLaser.h"
#include "TexturePoolManager.h"

CFinalLaser::CFinalLaser()
	: m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
{
}

CFinalLaser::CFinalLaser(const CFinalLaser & _rOther)
	: CGameObject(_rOther)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
{
}

void CFinalLaser::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CFinalLaser * CFinalLaser::Create()
{
	CFinalLaser* pInstance = new CFinalLaser;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CFinalLaser::Clone()
{
	CFinalLaser* pClone = new CFinalLaser(*this);
	return pClone;
}

HRESULT CFinalLaser::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CFinalLaser::Awake()
{
	CGameObject::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Rotation(D3DXVECTOR3(90.f, 0.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(100.f, 2.1f, 1.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CFinalLaser::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("FinalBossBullet"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("Laser");
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();
	m_nIndex = 0;
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	m_bRender = true;

	m_bRemove = false;
	return S_OK;
}

UINT CFinalLaser::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	if (true == m_bRemove)
		return OBJ_DEAD;
	Animate(_fDeltaTime);
	Explosion(_fDeltaTime);
	return 0;
}

UINT CFinalLaser::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CFinalLaser::Render()
{
	CGameObject::Render();
	if (false == m_bRender)
		return S_OK;
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

void CFinalLaser::SetLaser(DIRECTION _eDirection)
{
	float fDegree = 0.f;
	float fPos = 0.f;
	switch (_eDirection)
	{
	case CFinalLaser::LEFT:
	case CFinalLaser::RIGHT:
		fDegree = 90.f;
		break;
	case CFinalLaser::TOP:
	case CFinalLaser::BOTTOM:
		fDegree = 0.f;
		break;
	default:
		break;
	}

	switch (_eDirection)
	{
	case CFinalLaser::LEFT:
		fPos = -(10.f + float(rand() % 25));
		m_pTransform->Set_Position(D3DXVECTOR3(fPos, 0.f, 0.f));
		break;
	case CFinalLaser::RIGHT:
		fPos = (10.f + float(rand() % 25));
		m_pTransform->Set_Position(D3DXVECTOR3(fPos, 0.f, 0.f));
		break;
	case CFinalLaser::TOP:
		fPos = (5.f + float(rand() % 15));
		m_pTransform->Set_Position(D3DXVECTOR3(0.f, 0.f, fPos));
		break;
	case CFinalLaser::BOTTOM:
		fPos = -(5.f + float(rand() % 15));
		m_pTransform->Set_Position(D3DXVECTOR3(0.f, 0.f, fPos));
		break;
	default:
		break;
	}
	float fAdded = 10.f - float(rand() % 20);

	fDegree += fAdded;

	m_pTransform->Set_Rotation(D3DXVECTOR3(90.f, fDegree, 0.f));
	m_pTransform->UpdateTransform();

	m_vDirection = D3DXVECTOR3(1.f, 0.f, 0.f);
	D3DXMATRIX matRY;
	D3DXMatrixRotationY(&matRY, D3DXToRadian(fDegree));

	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matRY);
}

void CFinalLaser::Animate(const float _fDeltaTime)
{
	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimationSpeed)
	{
		m_fTime -= m_fAnimationSpeed;

		++m_nIndex;

		if (m_nIndex >= m_nMaxFrame)
		{
			m_bRender = false;
			return;
		}

		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}
}

void CFinalLaser::Explosion(const float _fDeltaTime)
{
	if (true == m_bRender)
		return;
	m_bRemove = true;
}

