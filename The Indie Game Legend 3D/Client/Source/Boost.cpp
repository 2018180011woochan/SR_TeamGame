#include "stdafx.h"
#include "..\Header\Boost.h"
#include "TexturePoolManager.h"
#include "BattleShip.h"

CBoost::CBoost()
	: m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_pBattleShipTransform(nullptr)
{
}

CBoost::CBoost(const CBoost & _rOther)
	: CGameObject(_rOther)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_pBattleShipTransform(nullptr)
{
}

void CBoost::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
	SafeRelease(m_pBattleShipTransform);
}

CBoost * CBoost::Create()
{
	CBoost* pInstance = new CBoost;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CBoost::Clone()
{
	CBoost* pClone = new CBoost(*this);
	return pClone;
}

HRESULT CBoost::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CBoost::Awake()
{
	CGameObject::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Rotation(D3DXVECTOR3(90.f, 0.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(2.4f, 0.8f, 1.f));
	m_pTransform->UpdateTransform();

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CBoost::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("BattleShip"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("Boost");
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pBattleShipTransform = (CTransform*)(FindGameObjectOfType<CBattleShip>()->GetComponent<CTransform>());
	SafeAddRef(m_pBattleShipTransform);

	m_vOffset = (m_pTransform->Get_TransformDesc().vScale + m_pBattleShipTransform->Get_TransformDesc().vScale) * 0.5f;
	m_vOffset.x -= 0.5f;
	m_vOffset.y = 0.f;
	m_vOffset.z = 0.f;
	return S_OK;
}

UINT CBoost::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	Animate(_fDeltaTime);
	return 0;
}

UINT CBoost::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	SetPosition();
	return 0;
}

HRESULT CBoost::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

void CBoost::Animate(const float _fDeltaTime)
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

void CBoost::SetPosition()
{
	D3DXVECTOR3 vPosition = m_pBattleShipTransform->Get_Position();

	vPosition = vPosition - m_vOffset;

	m_pTransform->Set_Position(vPosition);

	m_pTransform->UpdateTransform();
}

