#include "stdafx.h"
#include "Explosion.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"


CExplosion::CExplosion()
	:m_pTexturePool(nullptr)
{
}

CExplosion::CExplosion(const CExplosion & other)
	: CGameObject(other)
{
}

HRESULT CExplosion::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CExplosion::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Explosion"));
	SafeAddRef(m_pTexturePool);

	m_pTransform->Set_Scale(_vector(7.f, 7.f, 7.f));

	m_fJumpPower = 10.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 8.f;
	m_bJump = false;
	nIndex = 0;

	m_fWalkSpeed = 0.3f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;

	nIndex = 0;

	m_eRenderID = ERenderID::Alpha;

	
	return S_OK;
}

HRESULT CExplosion::Start()
{
	CGameObject::Start();


	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CExplosion::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		nIndex++;
		if (nIndex >= 4)
			m_bDead = true;
		if (nIndex >= 4)
			nIndex = 0;
		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CExplosion::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillBording();
	return _uint();
}

HRESULT CExplosion::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}


void CExplosion::IsBillBording()
{
	CCamera* pCamera = (CCamera*)FindGameObjectOfType<CCamera>();
	if (nullptr == pCamera)
		return;

	_matrix matBill, matView;
	D3DXMatrixIdentity(&matBill);
	matView = pCamera->Get_Camera().matView;

	memcpy(&matBill.m[0][0], &matView.m[0][0], sizeof(_vector));
	memcpy(&matBill.m[1][0], &matView.m[1][0], sizeof(_vector));
	memcpy(&matBill.m[2][0], &matView.m[2][0], sizeof(_vector));

	D3DXMatrixInverse(&matBill, 0, &matBill);

	_matrix matWorld = m_pTransform->Get_WorldMatrix();

	/*
	matBill * 스 * 자 * 이 * 공 * 부
	*/
	_matrix NewWorld = matBill * matWorld;
	m_pTransform->Set_WorldMatrix(NewWorld);
}

void CExplosion::SetPos(const _vector _EggPos)
{
	m_pTransform->Set_Position(_EggPos);
}

CGameObject * CExplosion::Clone()
{
	CExplosion* pClone = new CExplosion(*this);
	return pClone;
}

CExplosion * CExplosion::Create()
{
	CExplosion* pInstance = new CExplosion();
	return pInstance;
}

void CExplosion::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
