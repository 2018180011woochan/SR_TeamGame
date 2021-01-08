#include "stdafx.h"
#include "SkillRunning.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "Camera.h"
#include "ShopKeeper.h"
#include "SoundMgr.h"

CSkillRunning::CSkillRunning()
	: m_pTexturePool(nullptr)
{
}

CSkillRunning::CSkillRunning(const CSkillRunning & other)
	: CGameObject(other)
{
}

HRESULT CSkillRunning::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkillRunning::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("SkillRunning"));
	SafeAddRef(m_pTexturePool);
	m_nPrice = 50;
	m_isBuyItem = false;
	m_bDead = false;
	m_pTransform->Set_Scale(_vector(5, 5, 5));
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CSkillRunning::Start()
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

UINT CSkillRunning::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;
	CGameObject::Update(_fDeltaTime);
	MoveMent(_fDeltaTime);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CSkillRunning::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CSkillRunning::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CSkillRunning::OnCollision(CGameObject * _pGameObject)
{
	if (L"Player" == _pGameObject->GetName())
	{
		CPlayer* pPlayer = (CPlayer*)FindGameObjectOfType<CPlayer>();
		if (pPlayer->GetGem() < m_nPrice)
			return;
		CSoundMgr::GetInstance()->Play(L"sfxCoin.wav", CSoundMgr::Item_Coin);
		pPlayer->SetBuyItem(m_nPrice);
		pPlayer->SetBySkillRunning();
		m_isBuyItem = true;
		m_bDead = true;
	}
} 

HRESULT CSkillRunning::IsBillboarding()
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

void CSkillRunning::Set_ItemPos(const _vector _vItemPos)
{
	m_pTransform->Set_Position(_vItemPos);
}

void CSkillRunning::MoveMent(float _fDeltaTime)
{
	CTransform* pTransform = (CTransform*)m_pShopKeeper->GetComponent<CTransform>();
	
	m_pTransform->Set_Position(_vector(pTransform->Get_Position().x + 5.f,
		pTransform->Get_Position().y - 5.f,
		pTransform->Get_Position().z + 32.f));
}


CGameObject * CSkillRunning::Clone()
{
	CSkillRunning* pClone = new CSkillRunning(*this);
	return pClone;
}

CSkillRunning * CSkillRunning::Create()
{
	CSkillRunning* pInstance = new CSkillRunning();
	return pInstance;
}

void CSkillRunning::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
