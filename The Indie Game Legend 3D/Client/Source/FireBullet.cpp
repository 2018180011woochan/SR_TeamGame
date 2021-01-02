#include "stdafx.h"
#include "FireBullet.h"
#include "Player.h"
#include "Camera.h"

CFireBullet::CFireBullet()
	: m_pTexturePool(nullptr)
{
}

CFireBullet::CFireBullet(const CFireBullet & other)
	: CGameObject(other)
{
}



HRESULT CFireBullet::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFireBullet::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Scale(_vector(3, 3, 3));
	m_fBulletSpeed = 50.f;
	m_eRenderID = ERenderID::Alpha;
	m_bDead = false;
	return S_OK;
}

HRESULT CFireBullet::Start()
{
	CGameObject::Start();

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("FireBall"));
	SafeAddRef(m_pTexturePool);

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;
	m_nTag = 0;

	return S_OK;
}

UINT CFireBullet::Update(const float _fDeltaTime)
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

UINT CFireBullet::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CFireBullet::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CFireBullet::OnCollision(CGameObject * _pGameObject)
{
	if (L"Player" == _pGameObject->GetName() || L"Obstacle" == _pGameObject->GetName()
		|| L"Floor" == _pGameObject->GetName())
	{
		m_bDead = true;
	}
}

CGameObject * CFireBullet::Clone()
{
	CFireBullet* pClone = new CFireBullet(*this);
	return pClone;
}

CFireBullet * CFireBullet::Create()
{
	CFireBullet* pInstance = new CFireBullet();
	return pInstance;
}

HRESULT CFireBullet::Movement(float fDeltaTime)
{
	m_pTransform->Add_Position(m_vecDir * fDeltaTime * m_fBulletSpeed);
	return S_OK;
}

void CFireBullet::SetTurretPos(const _vector _TurretPos)
{
	m_pTransform->Set_Position(_TurretPos);
	D3DXVECTOR3 vecTarget;
	CGameObject* pGameObject = FindGameObjectOfType<CPlayer>();

	if (nullptr == pGameObject)
		return;

	CTransform* pTransform = ((CTransform*)pGameObject->GetComponent<CTransform>());

	vecTarget = _vector(pTransform->Get_Position().x, pTransform->Get_Position().y - 1.f, pTransform->Get_Position().z);

	m_vecDir = vecTarget - _TurretPos;

	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
}

HRESULT CFireBullet::IsBillboarding()
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

void CFireBullet::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
