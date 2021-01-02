#include "stdafx.h"
#include "BloodHand.h"
#include "Player.h"
#include "Camera.h"

CBloodHand::CBloodHand()
	: m_pTexturePool(nullptr)
{
}

CBloodHand::CBloodHand(const CBloodHand & other)
	: CGameObject(other)
{
}



HRESULT CBloodHand::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBloodHand::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Scale(_vector(3, 3, 3));
	m_fBulletSpeed = 20.f;
	m_eRenderID = ERenderID::Alpha;
	m_bDead = false;
	return S_OK;
}

HRESULT CBloodHand::Start()
{
	CGameObject::Start();

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Bloodhand"));
	SafeAddRef(m_pTexturePool);

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;
	m_nTag = 0;

	return S_OK;
}

UINT CBloodHand::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update(_fDeltaTime);

	if (nIndex >= 4)
		nIndex = 0;
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	m_pTransform->UpdateTransform();


	return _uint();
}

UINT CBloodHand::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CBloodHand::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CBloodHand::OnCollision(CGameObject * _pGameObject)
{
	if (L"Player" == _pGameObject->GetName() || L"Obstacle" == _pGameObject->GetName()
		|| L"Floor" == _pGameObject->GetName())
	{
		m_bDead = true;
	}
}

CGameObject * CBloodHand::Clone()
{
	CBloodHand* pClone = new CBloodHand(*this);
	return pClone;
}

CBloodHand * CBloodHand::Create()
{
	CBloodHand* pInstance = new CBloodHand();
	return pInstance;
}

HRESULT CBloodHand::Movement(float fDeltaTime)
{
	m_pTransform->Add_Position(m_vecDir * fDeltaTime * m_fBulletSpeed);
	return S_OK;
}

void CBloodHand::SetTurretPos(const _vector _TurretPos, const _vector _PlayerPos)
{
	m_pTransform->Set_Position(_TurretPos);
	D3DXVECTOR3 vecTarget;

	vecTarget = _PlayerPos;

	m_vecDir = vecTarget - _TurretPos;

	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
}

HRESULT CBloodHand::IsBillboarding()
{
	CCamera* pCamera = (CCamera*)FindGameObjectOfType<CCamera>();
	if (pCamera == nullptr)
		return E_FAIL;

	_matrix matBillY, matView;
	D3DXMatrixIdentity(&matBillY);
	matView = pCamera->Get_Camera().matView;

	matBillY._11 = matView._11;
	matBillY._13 = matView._13;
	matBillY._31 = matView._31;
	matBillY._33 = matView._33;

	D3DXMatrixInverse(&matBillY, 0, &matBillY);

	_matrix matWorld = m_pTransform->Get_WorldMatrix();

	_matrix NewWorld = matBillY * matWorld;
	m_pTransform->Set_WorldMatrix(NewWorld);

	return S_OK;
}

void CBloodHand::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
