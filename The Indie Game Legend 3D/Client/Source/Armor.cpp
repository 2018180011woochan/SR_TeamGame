#include "stdafx.h"
#include "Armor.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "Camera.h"
#include "ShopKeeper.h"

CArmor::CArmor()
	: m_pTexturePool(nullptr)
{
}

CArmor::CArmor(const CArmor & other)
	: CGameObject(other)
{
}

HRESULT CArmor::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CArmor::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Armor"));
	SafeAddRef(m_pTexturePool);

	m_pTransform->Set_Scale(_vector(5, 5, 5));
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CArmor::Start()
{
	CGameObject::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pShopKeeper = (CShopKeeper*)FindGameObjectOfType<CShopKeeper>();
	return S_OK;
}

UINT CArmor::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	MoveMent(_fDeltaTime);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CArmor::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CArmor::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CArmor::OnCollision(CGameObject * _pGameObject)
{

}

HRESULT CArmor::IsBillboarding()
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

void CArmor::Set_ItemPos(const _vector _vItemPos)
{
	m_pTransform->Set_Position(_vItemPos);
}

void CArmor::MoveMent(float _fDeltaTime)
{
	CTransform* pTransform = (CTransform*)m_pShopKeeper->GetComponent<CTransform>();

	m_pTransform->Set_Position(_vector(pTransform->Get_Position().x + 20.f,
		pTransform->Get_Position().y - 5.f,
		pTransform->Get_Position().z - 20.f));
}


CGameObject * CArmor::Clone()
{
	CArmor* pClone = new CArmor(*this);
	return pClone;
}

CArmor * CArmor::Create()
{
	CArmor* pInstance = new CArmor();
	return pInstance;
}

void CArmor::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
