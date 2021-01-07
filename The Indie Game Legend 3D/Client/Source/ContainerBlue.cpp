#include "stdafx.h"
#include "ContainerBlue.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "Camera.h"
#include "SpreadGun.h"

CContainerBlue::CContainerBlue()
	: m_pTexturePool(nullptr)
{
}

CContainerBlue::CContainerBlue(const CContainerBlue & other)
	: CGameObject(other)
{
}

HRESULT CContainerBlue::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CContainerBlue::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Container"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Container_Blue"));
	SafeAddRef(m_pTexturePool);

	m_pTransform->Set_Position(_vector(0.f, 0.f, 0.f));
	m_pTransform->Set_Scale(_vector(1, 1, 1));
	m_bIsHit = false;
	nIndex = 0;
	m_fWalkSpeed = 0.1f;
	m_fWalkDeltaTime = 0.f;
	//m_nTag = 0;
	m_bEnable = true;
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CContainerBlue::Start()
{
	CGameObject::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Container"), BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bIsRigid = true;

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;
}

UINT CContainerBlue::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	if (m_bIsHit)
	{
		m_fWalkDeltaTime += _fDeltaTime;
		if (m_fWalkSpeed <= m_fWalkDeltaTime)
		{
			nIndex++;
			if (nIndex >= 6)
				return OBJ_DEAD;
			m_fWalkDeltaTime -= m_fWalkSpeed;
		}
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CContainerBlue::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	//IsBillboarding();
	return _uint();
}

HRESULT CContainerBlue::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CContainerBlue::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
		CSpreadGun* pItem = (CSpreadGun*)AddGameObject<CSpreadGun>();
		pItem->Set_ItemPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 5.f, m_pTransform->Get_Position().z));

		m_bIsHit = true;
	}
}

HRESULT CContainerBlue::IsBillboarding()
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


CGameObject * CContainerBlue::Clone()
{
	CContainerBlue* pClone = new CContainerBlue(*this);
	return pClone;
}

CContainerBlue * CContainerBlue::Create()
{
	CContainerBlue* pInstance = new CContainerBlue();
	return pInstance;
}

void CContainerBlue::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
