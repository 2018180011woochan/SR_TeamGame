#include "stdafx.h"
#include "..\Header\Item.h"
#include "TexturePoolManager.h"
#include "Player.h"
#include "Camera.h"

CItem::CItem()
{
}

CItem::CItem(const CItem & _rOther)
	:CGameObject(_rOther)
	, m_eType(EItemID::End)
{
}


HRESULT CItem::Animate(const float _fDeltaTime)
{
	if (m_nMaxFrame < 1)
		return E_FAIL;

	m_fTextureIndex += _fDeltaTime * m_fAnimateSpeed;
	if (m_nMaxFrame < m_fTextureIndex)
	{
		m_fTextureIndex = 0.f;// 디버깅작업 생각해서 아예 0으로 초기화 
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureName)[(_int)m_fTextureIndex]);
	return S_OK;
}

HRESULT CItem::BillBord()
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

void CItem::Blink(const float _fDeltaTime)
{
	m_fLiveTime += _fDeltaTime;

	if (m_fBlinkStart <  m_fLiveTime)
		m_fBlinkTime += _fDeltaTime;

	if (m_fBlinkInterval < m_fBlinkTime)
	{
		m_bRenderOff = !m_bRenderOff;
		m_fBlinkTime = 0.f;
	}
	if (m_fLive < m_fLiveTime)
		m_bDelete = true;
}

void CItem::SetItemType(const EItemID & _eID)
{
	m_eType = _eID;
	switch (_eID)
	{
	case EItemID::Heart:
		m_sTextureName = L"sprHeart";
		break;
	case EItemID::Ammo:
		m_sTextureName = L"Ammo";
		break;
	case EItemID::sprCoin:
		m_sTextureName = L"sprCoin";
		break;
	case EItemID::Disc:
		m_sTextureName = L"Disc";
		break;
	case EItemID::sprBigCoin:
		m_sTextureName = L"sprBigCoin";
		m_pTransform->Set_Scale(_vector(2, 2, 2));
		break;
	}

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureName)[0]);
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureName).size();
	m_fAnimateOneCycleTime = 1.f;
	m_fAnimateSpeed = (m_nMaxFrame + 1) / 1.f * m_fAnimateOneCycleTime;
}

void CItem::SetItemRand()
{
	int randValue = 0;

	randValue = rand() % 10;

	if (randValue < 2)
		SetItemType(EItemID::Heart);
	else if(randValue < 6)
		SetItemType(EItemID::sprCoin);
	else if (randValue < 8)
		SetItemType(EItemID::sprBigCoin);
	else
		SetItemType(EItemID::Ammo);
}

void CItem::SetPos(const _vector _vPos)
{
	m_pTransform->Set_Position(_vPos);
}

void CItem::Bounce(float _fDeltaTime)
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

		m_pTransform->Add_Position(vRandDir * _fDeltaTime * 40.f);

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


HRESULT CItem::InitializePrototype()
{
	m_sName = L"Item";
	return S_OK;
}

HRESULT CItem::Awake()
{
	CGameObject::Awake();

	m_eRenderID = ERenderID::Alpha;
	m_bDelete = false;
	m_sTextureName = L"";

	// Prod By Woochan
	m_fJumpPower = 5.f;
	m_fJumpTime = 0.f;
	m_fYTest = 3.f;
	m_bJump = true;

	//  [1/4/2021 wades]
	m_fLiveTime = 0.f;
	m_fLive = 10.f;
	m_fBlinkStart = 6.f;
	m_bRenderOff = false;
	m_fBlinkInterval = 0.2f;
	m_fBlinkTime = 0.f;

	return S_OK;
}

HRESULT CItem::Start()
{
	CGameObject::Start();
	
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Item"));
	//m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Heart"))[0]);
	if (nullptr == m_pTexturePool)
	{
		PrintLog(L"Error", L"pTexturePool is null by CElectricTile::Start()");
		return E_FAIL;
	}
	else
		SafeAddRef(m_pTexturePool);



	m_pTransform->Set_Scale(_vector(3, 3, 3));
	m_pTransform->UpdateTransform();

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Cube"),BOUND::SPHERE);
	m_pCollider->m_bIsRigid = true;
	return S_OK;
}

UINT CItem::Update(const float _fDeltaTime)
{
	if (m_bDelete)
		return OBJ_DEAD;


	if (FAILED(Animate(_fDeltaTime)))
		return OBJ_NOENVET;

	Bounce(_fDeltaTime);

	return OBJ_NOENVET;
}

UINT CItem::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);


	//  [1/4/2021 wades]
	Blink(_fDeltaTime);


	return OBJ_NOENVET;
}

HRESULT CItem::Render()
{
	//  [1/4/2021 wades]
	if (m_bRenderOff)
		return S_OK;


	BillBord();
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CItem::Clone()
{
	CGameObject* pInstance = new CItem(*this);
	return pInstance;
}

void CItem::OnCollision(CGameObject * _pGameObject)
{
	if (L"Player" == _pGameObject->GetName())
	{
		m_bDelete = true;
	}
}

void CItem::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}

CItem * CItem::Create()
{
	CItem* pInstance = new CItem();
	return pInstance;
}
