#include "stdafx.h"
#include "Phubans.h"
#include "MeshRenderer.h"
#include "Player.h"
#include "Item.h"
#include "Blood.h"

CPhubans::CPhubans()
	: m_pTexturePool(nullptr)
{
}

CPhubans::CPhubans(const CPhubans & other)
	: CMonster(other)
{
}

HRESULT CPhubans::InitializePrototype()
{
	if (FAILED(CMonster::InitializePrototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPhubans::Awake()
{
	if (FAILED(CMonster::Awake()))
		return E_FAIL;

	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Phubans"));
	SafeAddRef(m_pTexturePool);

	m_fWalkDeltaTime = 0.f;
	m_fWalkSpeed = 1.f;

	m_FLightningDeltaTime = 0.f;
	m_fLightningSpeed = 1.f;

	m_bisDash = false;
	m_bisStop = false;

	m_fMoveSpeed = 8.f;

	m_pTransform->Set_Scale(_vector(5.f, 5.f, 5.f));
	//m_pTransform->Add_Position(_vector(-5.f, 5.f, 10.f));
	m_pTransform->Set_Position(_vector(-5.f, 3.f, 50.f));

	m_iHP = 10;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CPhubans::Start()
{
	CMonster::Start();

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[0]);

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->m_bIsRigid = true;
	m_nTag = 0;

	return S_OK;
}

UINT CPhubans::Update(const float _fDeltaTime)
{
	if (m_bDead)
		return OBJ_DEAD;

	CMonster::Update(_fDeltaTime);

	m_fWalkDeltaTime += _fDeltaTime;
	if (m_fWalkSpeed <= m_fWalkDeltaTime)
	{
		nIndex++;
		m_fWalkDeltaTime -= m_fWalkSpeed;
		if (nIndex >= 7)
			nIndex = 0;
	}

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[nIndex]);

	if (FAILED(Movement(_fDeltaTime)))
		return 0;

	if (isCloseToPlayer())
	{
		Dash(_fDeltaTime);
	}

	if (!isCloseToPlayer())
		m_fMoveSpeed = 8.f;

	m_pTransform->UpdateTransform();

	return _uint();
}

UINT CPhubans::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return _uint();
}

HRESULT CPhubans::Render()
{
	if (FAILED(CMonster::Render()))
		return E_FAIL;

	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

void CPhubans::OnCollision(CGameObject * _pGameObject)
{
	if (L"PlayerBullet" == _pGameObject->GetName())
	{
		m_iHP--;
		CBlood* pBlood = (CBlood*)AddGameObject<CBlood>();
		pBlood->SetPos(m_pTransform->Get_Position());
	}
	if (m_iHP <= 0)
	{
		CItem* pHeart = (CItem*)AddGameObject<CItem>();
		pHeart->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		pHeart->SetItemType(EItemID::sprBigCoin);

		CItem* psqrCoin = (CItem*)AddGameObject<CItem>();
		psqrCoin->SetPos(_vector(m_pTransform->Get_Position().x, m_pTransform->Get_Position().y + 3.f, m_pTransform->Get_Position().z));
		psqrCoin->SetItemType(EItemID::sprCoin);
		m_bDead = true;
	}
}


HRESULT CPhubans::Movement(float fDeltaTime)
{
	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());

	_vector vDir;
	vDir = m_pPlayerTransform->Get_Position() - m_pTransform->Get_Position();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransform->Add_Position(vDir * fDeltaTime * m_fMoveSpeed);

	return S_OK;
}

void CPhubans::Dash(float _fDeltaTime)
{
	if (!m_bisDash)
		m_bisStop = true;
	while (m_bisStop)
	{
		iCnt++;
		if (iCnt <= 300)
		{
			m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[1]);
			m_fMoveSpeed = 0.f;
			return;
		}
		if (iCnt > 200)
		{
			iCnt = 0;
			m_bisDash = true;
			m_bisStop = false;
		}
	}
	while (m_bisDash)
	{
		if (iDashCnt < 70)
		{
			iDashCnt++;
			m_fMoveSpeed = 50.f;
			m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Idle"))[2]);
			return;
		}
		if (iDashCnt >= 70)
		{
			iDashCnt = 0;
			m_fMoveSpeed = 8.f;
			m_bisDash = false;
		}
	}
	
}

bool CPhubans::isCloseToPlayer()
{
	_vector vecPlayerPos = m_pPlayerTransform->Get_Position();
	float fDistance = 0.f;

	float fWidth = abs(vecPlayerPos.x - m_pTransform->Get_Position().x);
	float fHeight = abs(vecPlayerPos.z - m_pTransform->Get_Position().z);
	fDistance = sqrt((fWidth * fWidth) + (fHeight * fHeight));

	if (fDistance < 40.f)
		return true;

	return false;
}


CGameObject * CPhubans::Clone()
{
	CPhubans* pClone = new CPhubans(*this);
	return pClone;
}

CPhubans * CPhubans::Create()
{
	CPhubans* pInstance = new CPhubans();
	return pInstance;
}

void CPhubans::Free()
{
	SafeRelease(m_pTexturePool);
	CGameObject::Free();
}
