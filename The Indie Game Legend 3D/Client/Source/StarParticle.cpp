#include "stdafx.h"
#include "..\Header\StarParticle.h"
#include "StarPool.h"

CStarParticle::CStarParticle()
	: m_pStarPool(nullptr)
	, m_pMeshRenderer(nullptr)
{
}

CStarParticle::CStarParticle(const CStarParticle & _rOther)
	: CGameObject(_rOther)
	, m_pStarPool(nullptr)
	, m_pMeshRenderer(nullptr)
{
}

void CStarParticle::Free()
{
	CGameObject::Free();
	SafeRelease(m_pStarPool);
}

CStarParticle * CStarParticle::Create()
{
	CStarParticle* pInstance = new CStarParticle;
	if (FAILED(pInstance->InitializePrototype()))
	{
		//SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CStarParticle::Clone()
{
	CStarParticle* pClone = new CStarParticle(*this);
	return pClone;
}

HRESULT CStarParticle::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CStarParticle::Awake()
{
	CGameObject::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Scale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));
	m_pTransform->Set_Position(D3DXVECTOR3(0.f, 0.f, -100.f));
	m_pTransform->Set_Rotation(D3DXVECTOR3(90.f, 0.f, 0.f));
	m_pTransform->UpdateTransform();
	m_eRenderID = ERenderID::NoAlpha;

	m_fMoveSpeed = 100.f;
	return S_OK;
}

HRESULT CStarParticle::Start()
{
	m_pStarPool = (CStarPool*)FindGameObjectOfType<CStarPool>();
	//SafeAddRef(m_pStarPool);
	return S_OK;
}

UINT CStarParticle::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	Move(_fDeltaTime);
	return 0;
}

UINT CStarParticle::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CStarParticle::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

void CStarParticle::OnEnable()
{	//시작 위치 및 방향 잡기.
	float fZ = float(-25.f + rand() % 50);

	//방향
	m_vMoveDirection = D3DXVECTOR3(-1.f, 0.f, 0.f);

	//float fDegreeY = (fX < 0.f ? 10.f : -10.f);
	//float fDegreeX = (fY < 0.f ? -10.f : 10.f);

	//D3DXMATRIX matRY, matRX, matRotation;
	//D3DXMatrixRotationY(&matRY, D3DXToRadian(fDegreeY));
	//D3DXMatrixRotationX(&matRX, D3DXToRadian(fDegreeX));
	//matRotation = matRX * matRY;

	//D3DXVec3TransformNormal(&m_vMoveDirection, &m_vMoveDirection, &matRotation);
	//D3DXVec3Normalize(&m_vMoveDirection, &m_vMoveDirection);

	//위치
	//D3DXVECTOR3 vProjection = D3DXVECTOR3(fX, 1.f, 1.f);

	//D3DXMATRIX matProjection;
	//m_pDevice->GetTransform(D3DTS_PROJECTION, &matProjection);

	//D3DXMatrixInverse(&matProjection, 0, &matProjection);

	//D3DXVec3TransformCoord(&vProjection, &vProjection, &matProjection);

	//vProjection.z = vProjection.y;
	//vProjection.y = 0.f;

	D3DXVECTOR3 vPosition = D3DXVECTOR3(50.f, 0.f, fZ);

	m_pTransform->Set_Position(vPosition);

}

void CStarParticle::Move(const float _fDeltaTime)
{
	m_pTransform->Add_Position(m_vMoveDirection * m_fMoveSpeed * _fDeltaTime);
	m_pTransform->UpdateTransform();

	if (-50.f > m_pTransform->Get_Position().x)
	{
		m_pStarPool->PushToPool(this);
		SetEnable(false);
	}
}
