#include "stdafx.h"
#include "RootAttack.h"
#include "Player.h"
#include "Camera.h"

CRootAttack::CRootAttack()
	: m_pTexturePool(nullptr)
{
}

CRootAttack::CRootAttack(const CRootAttack & other)
	: CGameObject(other)
{
}



HRESULT CRootAttack::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRootAttack::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Scale(_vector(10.f, 10.f, 10.f));
	m_fBulletSpeed = 20.f;

	m_fWalkSpeed = 0.3f;
	m_fWalkDeltaTime = 0.f;

	m_eRenderID = ERenderID::Alpha;
	m_bDead = false;
	return S_OK;
}

HRESULT CRootAttack::Start()
{
	CGameObject::Start();

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("RootAttack1"));
	SafeAddRef(m_pTexturePool);
	//  [1/4/2021 wades]
	m_sName = L"Monster";

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	//m_pCollider->m_bIsRigid = false;
	return S_OK;
}

UINT CRootAttack::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	CGameObject::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		nIndex++;
		if (nIndex >= 3)
			nIndex = 0;
		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x, 2.f, m_pTransform->Get_Position().z));

	m_pTransform->UpdateTransform();


	return _uint();
}

UINT CRootAttack::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CRootAttack::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CRootAttack::OnCollision(CGameObject * _pGameObject)
{
	if (L"Obstacle" == _pGameObject->GetName()
		|| L"Floor" == _pGameObject->GetName() || L"Wall" == _pGameObject->GetName())
	{
		m_bDead = true;
	}
}

CGameObject * CRootAttack::Clone()
{
	CRootAttack* pClone = new CRootAttack(*this);
	return pClone;
}

CRootAttack * CRootAttack::Create()
{
	CRootAttack* pInstance = new CRootAttack();
	return pInstance;
}

HRESULT CRootAttack::Movement(float fDeltaTime)
{
	m_pTransform->Add_Position(m_vecDir * fDeltaTime * m_fBulletSpeed);
	return S_OK;
}

void CRootAttack::SetRootPos(const _vector _TurretPos)
{
	m_pTransform->Set_Position(_TurretPos);
	D3DXVECTOR3 vecTarget;
	CGameObject* pGameObject = FindGameObjectOfType<CPlayer>();

	if (nullptr == pGameObject)
		return;

	vecTarget = ((CTransform*)pGameObject->GetComponent<CTransform>())->Get_Position();

	m_vecDir = vecTarget - _TurretPos;
	m_vecDir.y = 0.f;
	D3DXVec3Normalize(&m_vecDir, &m_vecDir);
}

HRESULT CRootAttack::IsBillboarding()
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

void CRootAttack::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
