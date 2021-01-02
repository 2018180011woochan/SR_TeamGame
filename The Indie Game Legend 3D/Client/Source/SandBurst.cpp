#include "stdafx.h"
#include "SandBurst.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "Item.h"

CSandBurst::CSandBurst()
	:m_pTexturePool(nullptr)
{
}

CSandBurst::CSandBurst(const CSandBurst & other)
	: CGameObject(other)
{
}

HRESULT CSandBurst::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSandBurst::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Sparkle"));
	SafeAddRef(m_pTexturePool);

	m_pTransform->Set_Scale(_vector(10, 10, 10));

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

HRESULT CSandBurst::Start()
{
	CGameObject::Start();


	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CSandBurst::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		nIndex++;
		if (nIndex >= 5)
			m_bDead = true;

		m_fWalkDeltaTime -= m_fWalkSpeed;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	Spreading(_fDeltaTime);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CSandBurst::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillBording();
	return _uint();
}

HRESULT CSandBurst::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CSandBurst::Spreading(float _fDeltaTime)
{
	float fY = 0.f;

	_vector vRandDir = _vector(float(rand() % 3), float(rand() % 3), float(rand() % 3));
	D3DXVec3Normalize(&vRandDir, &vRandDir);
	if (m_bJump)
	{
		fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			fY,
			m_pTransform->Get_Position().z));

		m_pTransform->Add_Position(vRandDir * _fDeltaTime * 5.f);

		m_fJumpTime += 0.05f;

		if (fY < 2.f)
		{
			m_bJump = false;
			m_fJumpPower = 0.f;
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
				2.f,
				m_pTransform->Get_Position().z));
			m_fJumpTime = 0.f;
		}
	}
}

void CSandBurst::IsBillBording()
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

void CSandBurst::SetPos(const _vector _EggPos)
{
	m_pTransform->Set_Position(_EggPos);
}

CGameObject * CSandBurst::Clone()
{
	CSandBurst* pClone = new CSandBurst(*this);
	return pClone;
}

CSandBurst * CSandBurst::Create()
{
	CSandBurst* pInstance = new CSandBurst();
	return pInstance;
}

void CSandBurst::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
