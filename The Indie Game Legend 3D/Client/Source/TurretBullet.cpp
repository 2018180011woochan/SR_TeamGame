#include "stdafx.h"
#include "..\Header\TurretBullet.h"
#include "Player.h"
#include "Camera.h"

CTurretBullet::CTurretBullet()
	: m_pTexturePool(nullptr)
{
}

CTurretBullet::CTurretBullet(const CTurretBullet & other)
	: CGameObject(other)
{
}



HRESULT CTurretBullet::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CTurretBullet::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	m_sName = L"Monster";
	m_pTransform->Set_Scale(_vector(1, 1, 1));
	m_fBulletSpeed = 100.f;
	m_eRenderID = ERenderID::Alpha;
	m_bDead = false;
	return S_OK;
}

HRESULT CTurretBullet::Start()
{
	CGameObject::Start();

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("FireBall"));
	SafeAddRef(m_pTexturePool);

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);


	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;

	return S_OK;
}

UINT CTurretBullet::Update(const float _fDeltaTime)
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

UINT CTurretBullet::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CTurretBullet::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CTurretBullet::OnCollision(CGameObject * _pGameObject)
{
	if (L"Obstacle" == _pGameObject->GetName() || L"Player" == _pGameObject->GetName()
		|| L"Floor" == _pGameObject->GetName() || L"Wall" == _pGameObject->GetName())
	{
		m_bDead = true;
	}
}

CGameObject * CTurretBullet::Clone()
{
	CTurretBullet* pClone = new CTurretBullet(*this);
	return pClone;
}

CTurretBullet * CTurretBullet::Create()
{
	CTurretBullet* pInstance = new CTurretBullet();
	return pInstance;
}

HRESULT CTurretBullet::Movement(float fDeltaTime)
{
	m_pTransform->Add_Position(m_vecDir * fDeltaTime * m_fBulletSpeed);
	return S_OK;
}

void CTurretBullet::SetTurretPos(const _vector _TurretPos)
{
	m_pTransform->Set_Position(_TurretPos);
	D3DXVECTOR3 vecTarget;
	CGameObject* pGameObject = FindGameObjectOfType<CPlayer>();

	if (nullptr == pGameObject)
		return;

	vecTarget = ((CTransform*)pGameObject->GetComponent<CTransform>())->Get_Position();

	m_vecDir = vecTarget - _TurretPos;
	m_vecDir.y = 4.f;
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
}

HRESULT CTurretBullet::IsBillboarding()
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

void CTurretBullet::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
