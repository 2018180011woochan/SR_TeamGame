#include "stdafx.h"
#include "ArmorPrice.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "Camera.h"
#include "ShopKeeper.h"
#include "Armor.h"

CArmorPrice::CArmorPrice()
	: m_pTexturePool(nullptr)
{
}

CArmorPrice::CArmorPrice(const CArmorPrice & other)
	: CGameObject(other)
{
}

HRESULT CArmorPrice::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CArmorPrice::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Price"));
	SafeAddRef(m_pTexturePool);

	m_pTransform->Set_Scale(_vector(5, 3, 5));
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CArmorPrice::Start()
{
	CGameObject::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pShopKeeper = (CShopKeeper*)FindGameObjectOfType<CShopKeeper>();
	m_pArmor = (CArmor*)FindGameObjectOfType<CArmor>();
	return S_OK;
}

UINT CArmorPrice::Update(const float _fDeltaTime)
{
	if (m_pArmor->GetIsDead())
		return OBJ_DEAD;

	CGameObject::Update(_fDeltaTime);
	MoveMent(_fDeltaTime);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CArmorPrice::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CArmorPrice::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CArmorPrice::OnCollision(CGameObject * _pGameObject)
{

}

HRESULT CArmorPrice::IsBillboarding()
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

void CArmorPrice::Set_ItemPos(const _vector _vItemPos)
{
	m_pTransform->Set_Position(_vItemPos);
}

void CArmorPrice::MoveMent(float _fDeltaTime)
{
	CTransform* pTransform = (CTransform*)m_pShopKeeper->GetComponent<CTransform>();
	
	m_pTransform->Set_Position(_vector(pTransform->Get_Position().x + 5.f,
		pTransform->Get_Position().y + 1.f,
		pTransform->Get_Position().z));
}


CGameObject * CArmorPrice::Clone()
{
	CArmorPrice* pClone = new CArmorPrice(*this);
	return pClone;
}

CArmorPrice * CArmorPrice::Create()
{
	CArmorPrice* pInstance = new CArmorPrice();
	return pInstance;
}

void CArmorPrice::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
