#include "stdafx.h"
#include "ExplosionBlue.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"


CExplosionBlue::CExplosionBlue()
	:m_pTexturePool(nullptr)
	, m_pCollider(nullptr)

{
}

CExplosionBlue::CExplosionBlue(const CExplosionBlue & other)
	: CGameObject(other)
	, m_pCollider(nullptr)
{
}

HRESULT CExplosionBlue::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;
	m_sName = L"ExplosionBlue";
	return S_OK;
}

HRESULT CExplosionBlue::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("ExplosionBlue"));
	SafeAddRef(m_pTexturePool);

	//animate
	m_nMaxFrame = m_pTexturePool->GetTexture(TEXT("Idle")).size();
	m_fTextureIndex = 0.f;
	m_fAnimateOneCycleTime = 3.f;
	m_fAnimateSpeed = (m_nMaxFrame) / 1.f * m_fAnimateOneCycleTime;
	//animate

	m_pTransform->Set_Scale(_vector(20.f, 20.f, 20.f));

	m_fJumpPower = 10.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 8.f;
	m_bJump = false;
	nIndex = 0;

	m_fWalkSpeed = 0.1f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;

	m_bDead = false;

	nIndex = 0;

	m_eRenderID = ERenderID::Alpha;

	m_bDead = false;

	return S_OK;
}

HRESULT CExplosionBlue::Start()
{
	CGameObject::Start();


	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->SetRadius(7.f);
	return S_OK;
}

UINT CExplosionBlue::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update(_fDeltaTime);

	m_fTextureIndex += _fDeltaTime * m_fAnimateSpeed;
	if (m_nMaxFrame <= m_fTextureIndex)
	{
		m_bDead = true;
		return OBJ_DEAD;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[(_int)m_fTextureIndex]);

	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CExplosionBlue::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillBording();
	return _uint();
}

HRESULT CExplosionBlue::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}


void CExplosionBlue::IsBillBording()
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

void CExplosionBlue::SetPos(const _vector _EggPos)
{
	m_pTransform->Set_Position(_EggPos);
}

CGameObject * CExplosionBlue::Clone()
{
	CExplosionBlue* pClone = new CExplosionBlue(*this);
	return pClone;
}

CExplosionBlue * CExplosionBlue::Create()
{
	CExplosionBlue* pInstance = new CExplosionBlue();
	return pInstance;
}

void CExplosionBlue::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
