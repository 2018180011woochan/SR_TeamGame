#include "stdafx.h"
#include "LazerGun.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "Camera.h"
#include "SoundMgr.h"

CLazerGun::CLazerGun()
	: m_pTexturePool(nullptr)
{
}

CLazerGun::CLazerGun(const CLazerGun & other)
	: CGameObject(other)
{
}

HRESULT CLazerGun::InitializePrototype()
{
	if (FAILED(CGameObject::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLazerGun::Awake()
{
	if (FAILED(CGameObject::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);
	m_bDead = false;
	m_pTransform->Set_Scale(_vector(8, 8, 8));
	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CLazerGun::Start()
{

	CGameObject::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("WeaponHUD"))[4]);
	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

	return S_OK;
}

UINT CLazerGun::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update(_fDeltaTime);

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("WeaponHUD"))[4]);
	m_pTransform->Add_RotationY(0.5f);
	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CLazerGun::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	IsBillboarding();
	return _uint();
}

HRESULT CLazerGun::Render()
{
	if (FAILED(CGameObject::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CLazerGun::OnCollision(CGameObject * _pGameObject)
{
	if (L"Player" == _pGameObject->GetName())
	{
		CPlayer* pPlayer = (CPlayer*)FindGameObjectOfType<CPlayer>();
		pPlayer->AddWeapon(EWeaponType::Lazer);

		CSoundMgr::GetInstance()->Play(L"sfxCoin.wav", CSoundMgr::Item_Coin);
		m_bDead = true;
	}
}

HRESULT CLazerGun::IsBillboarding()
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

void CLazerGun::Set_ItemPos(const _vector _vItemPos)
{
	m_pTransform->Set_Position(_vItemPos);
}


CGameObject * CLazerGun::Clone()
{
	CLazerGun* pClone = new CLazerGun(*this);
	return pClone;
}

CLazerGun * CLazerGun::Create()
{
	CLazerGun* pInstance = new CLazerGun();
	return pInstance;
}

void CLazerGun::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
