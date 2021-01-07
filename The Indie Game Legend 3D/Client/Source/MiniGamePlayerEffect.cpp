#include "stdafx.h"
#include "MiniGamePlayerEffect.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "Item.h"

CMiniGamePlayerEffect::CMiniGamePlayerEffect()
	:m_pTexturePool(nullptr)
{
}

CMiniGamePlayerEffect::CMiniGamePlayerEffect(const CMiniGamePlayerEffect & other)
	: CGameObject(other)
{
}

HRESULT CMiniGamePlayerEffect::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMiniGamePlayerEffect::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("MiniGamePlayerEffect"));
	SafeAddRef(m_pTexturePool);

	m_pTransform->Set_Scale(_vector(3, 3, 3));

	m_fJumpPower = 10.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 8.f;
	m_bJump = false;
	nIndex = 0;

	m_fWalkSpeed = 0.05f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;

	m_bDead = false;

	nIndex = 0;

	m_eRenderID = ERenderID::Alpha;

	m_bDead = false;

	return S_OK;
}

HRESULT CMiniGamePlayerEffect::Start()
{
	CGameObject::Start();


	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	return S_OK;
}

UINT CMiniGamePlayerEffect::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		nIndex++;
		if (nIndex >= 6)
		{
			nIndex = 5;
			m_bDead = true;
		}
		m_fWalkDeltaTime = 0;
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	Spreading(_fDeltaTime);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CMiniGamePlayerEffect::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillBording();
	return _uint();
}

HRESULT CMiniGamePlayerEffect::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CMiniGamePlayerEffect::Spreading(float _fDeltaTime)
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

void CMiniGamePlayerEffect::IsBillBording()
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

void CMiniGamePlayerEffect::SetPos(const _vector _EggPos)
{
	m_pTransform->Set_Position(_EggPos);
}

CGameObject * CMiniGamePlayerEffect::Clone()
{
	CMiniGamePlayerEffect* pClone = new CMiniGamePlayerEffect(*this);
	return pClone;
}

CMiniGamePlayerEffect * CMiniGamePlayerEffect::Create()
{
	CMiniGamePlayerEffect* pInstance = new CMiniGamePlayerEffect();
	return pInstance;
}

void CMiniGamePlayerEffect::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
