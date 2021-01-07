#include "stdafx.h"
#include "ShopKeeper.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "Camera.h"
#include "Ammo.h"
#include "Armor.h"
#include "BigShot.h"

CShopKeeper::CShopKeeper()
	: m_pTexturePool(nullptr)
{
}

CShopKeeper::CShopKeeper(const CShopKeeper & other)
	: CGameObject(other)
{
}

HRESULT CShopKeeper::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CShopKeeper::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI_ShopKeeper"));
	SafeAddRef(m_pTexturePool);

	//m_pTransform->Set_Position(_vector(0.f, 7.f, 220.f));
	m_pTransform->Set_Scale(_vector(15, 15, 15));
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CShopKeeper::Start()
{
	CGameObject::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	//m_pCollider = (CCollider*)AddComponent<CCollider>();
	//m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	//m_pCollider->m_bIsRigid = true;


	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;
}

UINT CShopKeeper::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CShopKeeper::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CShopKeeper::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CShopKeeper::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[2]);
	}
}

HRESULT CShopKeeper::IsBillboarding()
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


CGameObject * CShopKeeper::Clone()
{
	CShopKeeper* pClone = new CShopKeeper(*this);
	return pClone;
}

CShopKeeper * CShopKeeper::Create()
{
	CShopKeeper* pInstance = new CShopKeeper();
	return pInstance;
}

void CShopKeeper::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
