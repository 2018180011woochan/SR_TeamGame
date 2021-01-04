#include "stdafx.h"
#include "WalkerBullet.h"
#include "Player.h"
#include "Camera.h"

CWalkerBullet::CWalkerBullet()
	: m_pTexturePool(nullptr)
{
}

CWalkerBullet::CWalkerBullet(const CWalkerBullet & other)
	: CGameObject(other)
{
}



HRESULT CWalkerBullet::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWalkerBullet::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	//  [1/4/2021 wades]
	m_sName = L"Monster";
	m_pTransform->Set_Scale(_vector(1, 1, 1));
	m_fBulletSpeed = 100.f;
	m_eRenderID = ERenderID::Alpha;
	m_bDead = false;
	return S_OK;
}

HRESULT CWalkerBullet::Start()
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

UINT CWalkerBullet::Update(const float _fDeltaTime)
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

UINT CWalkerBullet::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CWalkerBullet::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CWalkerBullet::OnCollision(CGameObject * _pGameObject)
{
	if (L"Obstacle" == _pGameObject->GetName() || L"Floor" == _pGameObject->GetName() || L"Wall" == _pGameObject->GetName())
	{
		m_bDead = true;
	}
}

CGameObject * CWalkerBullet::Clone()
{
	CWalkerBullet* pClone = new CWalkerBullet(*this);
	return pClone;
}

CWalkerBullet * CWalkerBullet::Create()
{
	CWalkerBullet* pInstance = new CWalkerBullet();
	return pInstance;
}

HRESULT CWalkerBullet::Movement(float fDeltaTime)
{
	m_pTransform->Add_Position(m_vecDir * fDeltaTime * m_fBulletSpeed);
	return S_OK;
}

void CWalkerBullet::SetTurretPos(const _vector _TurretPos)
{
	m_pTransform->Set_Position(_TurretPos);
	D3DXVECTOR3 vecTarget;
	CGameObject* pGameObject = FindGameObjectOfType<CPlayer>();

	if (nullptr == pGameObject)
		return;

	vecTarget = ((CTransform*)pGameObject->GetComponent<CTransform>())->Get_Position();

	m_vecDir = vecTarget - _TurretPos;
	m_vecDir.y = 5.f;
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
}

HRESULT CWalkerBullet::IsBillboarding()
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

void CWalkerBullet::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
