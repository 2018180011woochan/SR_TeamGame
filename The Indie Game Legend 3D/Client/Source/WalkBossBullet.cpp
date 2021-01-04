#include "stdafx.h"
#include "WalkBossBullet.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"

CWalkBossBullet::CWalkBossBullet()
	:m_pTexturePool(nullptr)
{
}

CWalkBossBullet::CWalkBossBullet(const CWalkBossBullet & other)
	: CGameObject(other)
	
{
}

HRESULT CWalkBossBullet::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWalkBossBullet::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));
	//  [1/4/2021 wades]
	m_sName = L"Monster";

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Laser"));
	SafeAddRef(m_pTexturePool);

	m_fJumpPower = 15.f;
	m_fJumpTime = 0.f;	
	m_fMoveSpeed = 3.f;
	m_bJump = true;
	nIndex = 0;
	
	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 0.4f;

	m_fWalkSpeed = 0.5f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;
	m_fBulletSpeed = 30.f;
	m_bDead = false;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWalkBossBullet::Start()
{
	CGameObject::Start();

	m_pTransform->Set_Scale(_vector(0.5f, 0.5f, 100.f));
	// Test
	//m_pTransform->Add_Position(_vector(0.f, 0.f, 50.f));
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"),BOUND::BOUNDTYPE::SPHERE);
	m_nTag = 0;

	return S_OK;
}

UINT CWalkBossBullet::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	CGameObject::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		//nIndex++;
		if (nIndex >= 7)
			nIndex = 0;
		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);


	if (FAILED(Movement(_fDeltaTime)))
		return 0;


	//Jumping(_fDeltaTime);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CWalkBossBullet::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CWalkBossBullet::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CWalkBossBullet::OnCollision(CGameObject * _pGameObject)
{
	if (L"Obstacle" == _pGameObject->GetName()
		|| L"Floor" == _pGameObject->GetName() || L"Wall" == _pGameObject->GetName())
	{
		m_bDead = true;
	}
}


HRESULT CWalkBossBullet::Movement(float fDeltaTime)
{
	m_pTransform->Add_Position(m_vecDir * fDeltaTime * m_fBulletSpeed);
	return S_OK;
}

void CWalkBossBullet::Jumping(float fDeltaTime)
{
	float fY = 0.f;
	if (m_bJump)
	{
		fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			fY,
			m_pTransform->Get_Position().z));

		m_fJumpTime += 0.1f;

		if (fY < 2.f)
		{
			m_bJump = false;
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
				2.f,
				m_pTransform->Get_Position().z));
			m_fJumpTime = 0.f;
		}
	}
}


void CWalkBossBullet::SetWalkBossPos(const _vector _TurretPos)
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
	//_vector vecLaserPos = _TurretPos + m_vecDir * 10.f;
	//m_pTransform->Set_Position(vecLaserPos);

}

HRESULT CWalkBossBullet::IsBillboarding()
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

CGameObject * CWalkBossBullet::Clone()
{
	CWalkBossBullet* pClone = new CWalkBossBullet(*this);
	return pClone;
}

CWalkBossBullet * CWalkBossBullet::Create()
{
	CWalkBossBullet* pInstance = new CWalkBossBullet();
	return pInstance;
}

void CWalkBossBullet::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
