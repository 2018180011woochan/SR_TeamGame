#include "stdafx.h"
#include "LapidGun.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "Camera.h"
#include "SoundMgr.h"

CLapidGun::CLapidGun()
	: m_pTexturePool(nullptr)
{
}

CLapidGun::CLapidGun(const CLapidGun & other)
	: CGameObject(other)
{
}

HRESULT CLapidGun::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLapidGun::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("WeaponHUD"));
	SafeAddRef(m_pTexturePool);
	m_bDead = false;
	m_pTransform->Set_Scale(_vector(8, 8, 8));
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CLapidGun::Start()
{
	CGameObject::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("On"))[2]);
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;
}

UINT CLapidGun::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update(_fDeltaTime);

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("On"))[2]);
	m_pTransform->Add_RotationY(0.5f);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CLapidGun::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CLapidGun::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CLapidGun::OnCollision(CGameObject * _pGameObject)
{
	if (L"Player" == _pGameObject->GetName())
	{
		CPlayer* pPlayer = (CPlayer*)FindGameObjectOfType<CPlayer>();
		pPlayer->AddWeapon(EWeaponType::Rapid);

		CSoundMgr::GetInstance()->Play(L"sfxCoin.wav", CSoundMgr::Item_Coin);
		m_bDead = true;
	}
}

HRESULT CLapidGun::IsBillboarding()
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

void CLapidGun::Set_ItemPos(const _vector _vItemPos)
{
	m_pTransform->Set_Position(_vItemPos);
}


CGameObject * CLapidGun::Clone()
{
	CLapidGun* pClone = new CLapidGun(*this);
	return pClone;
}

CLapidGun * CLapidGun::Create()
{
	CLapidGun* pInstance = new CLapidGun();
	return pInstance;
}

void CLapidGun::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
