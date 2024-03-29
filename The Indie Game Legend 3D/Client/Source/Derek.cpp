#include "stdafx.h"
#include "Derek.h"
#include "VIBuffer_Rect.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Player.h"
#include "Camera.h"
#include "Item.h"
#include "SandBurst.h"

CDerek::CDerek()
	:m_pTexturePool(nullptr)
{
}

CDerek::CDerek(const CDerek & other)
	: CGameObject(other)
{
}

HRESULT CDerek::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDerek::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("NPC_Derek"));
	SafeAddRef(m_pTexturePool);

	m_fJumpPower = 10.f;
	m_fJumpTime = 0.f;
	m_fMoveSpeed = 8.f;
	m_bJump = false;
	nIndex = 0;

	m_fJumpDeltaTime = 0.f;
	m_fJumpSpeed = 5.f;

	m_fWalkSpeed = 0.5f;
	m_fWalkDeltaTime = 0.f;
	m_fYTest = 0.f;

	nIndex = 0;
	m_bEffect = false;
	m_bisSave = false;

	m_eRenderID = ERenderID::Alpha;

	
	return S_OK;
}

HRESULT CDerek::Start()
{
	CGameObject::Start();
	m_pTransform->Set_Scale(_vector(5, 5, 5));

	m_pTransform->Add_Position(_vector(0.f, -3.f, 0.f));
	//Test
	m_pTransform->Set_Position(_vector(5.f, 4.f, 0.f));

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	
	//test
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;

	//Test
	m_nTag = 0;

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;
}

UINT CDerek::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	if (m_bisSave)
	{
		if (FAILED(Movement(_fDeltaTime)))
			return 0;


		m_fJumpDeltaTime += _fDeltaTime;
		if (m_fJumpSpeed <= m_fJumpDeltaTime)
		{
			m_fJumpDeltaTime -= m_fJumpSpeed;
			m_bEffect = true;
			if (false == m_bJump)
				m_bJump = true;

			m_fYTest = m_pTransform->Get_Position().y;

		}

		Jumping(_fDeltaTime);
	}
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CDerek::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CDerek::Render()
{
	/*엔진에서 호출하는 식으로*/
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CDerek::OnCollision(CGameObject * _pGameObject)
{
	if (L"Player" == _pGameObject->GetName())
	{
		m_bisSave = true;
	}
}


HRESULT CDerek::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	_vector vPlayerDir = _vector(m_pPlayerTransform->Get_Position().x + 8.f, m_pPlayerTransform->Get_Position().y, m_pPlayerTransform->Get_Position().z);

	vDir = vPlayerDir - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CDerek::Jumping(float fDeltaTime)
{
	float fY = 0.f;
	if (m_bJump)
	{
		fY = m_fYTest + (m_fJumpPower * m_fJumpTime - 9.8f * m_fJumpTime * m_fJumpTime * 0.5f);
		m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
			fY,
			m_pTransform->Get_Position().z));

		m_fJumpTime += 0.05f;
		if (m_bEffect)
		{
			CSandBurst* pSandBurst = (CSandBurst*)AddGameObject<CSandBurst>();
			pSandBurst->SetPos(m_pTransform->Get_Position());
			m_bEffect = false;
		}

		if (fY < 2.f)
		{
			m_bJump = false;
			m_pTransform->Set_Position(_vector(m_pTransform->Get_Position().x,
				4.f,
				m_pTransform->Get_Position().z));
			m_fJumpTime = 0.f;

			// 한번 뛸때마다 체력회복

			CPlayer* pPlayer = (CPlayer*)FindGameObjectOfType<CPlayer>();
			pPlayer->AddHp(1);
		}
	}
}

void CDerek::SetNPCPos(const _vector _EggPos)
{
	m_pTransform->Set_Position(_EggPos);
}

HRESULT CDerek::IsBillboarding()
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

	D3DXVECTOR3 vPos = m_pTransform->Get_Position();
	D3DXVECTOR3 vScale = m_pTransform->Get_TransformDesc().vScale;
	D3DXVECTOR3 vRot = m_pTransform->Get_TransformDesc().vRotation;

	D3DXMATRIX matScale, matRX, matRY, matRZ, matTrans;

	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixRotationX(&matRX, D3DXToRadian(vRot.x));
	D3DXMatrixRotationY(&matRY, D3DXToRadian(vRot.y));
	D3DXMatrixRotationZ(&matRZ, D3DXToRadian(vRot.z));
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);



	_matrix NewWorld = matScale * matRX * matRY * matRZ * matBillY * matTrans;
	m_pTransform->Set_WorldMatrix(NewWorld);

	return S_OK;
}

CGameObject * CDerek::Clone()
{
	CDerek* pClone = new CDerek(*this);
	return pClone;
}

CDerek * CDerek::Create()
{
	CDerek* pInstance = new CDerek();
	return pInstance;
}

void CDerek::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
