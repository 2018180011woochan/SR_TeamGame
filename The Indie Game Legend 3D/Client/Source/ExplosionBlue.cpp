#include "stdafx.h"
#include "ExplosionBlue.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"


CExplosionBlue::CExplosionBlue()
	:m_pTexturePool(nullptr)
{
}

CExplosionBlue::CExplosionBlue(const CExplosionBlue & other)
	: CGameObject(other)
{
}

HRESULT CExplosionBlue::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

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

	m_pTransform->Set_Scale(_vector(7.f, 7.f, 7.f));

	m_fJumpPower = 10.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 8.f;
	m_bJump = false;
	nIndex = 0;

	m_fWalkSpeed = 0.1f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;

	nIndex = 0;

	m_eRenderID = ERenderID::Alpha;

	m_bDead = false;

	return S_OK;
}

HRESULT CExplosionBlue::Start()
{
	CGameObject::Start();


	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CExplosionBlue::Update(const float _fDeltaTime)
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
