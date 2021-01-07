#include "stdafx.h"
#include "..\Header\BattleShip.h"
#include "KeyManager.h"
#include "TexturePoolManager.h"
#include "BattleShipBullet.h"
#include "HeartManager.h"
#include "FinalBoss.h"
#include "Management.h"
#include "Ending.h"

CBattleShip::CBattleShip()
	: m_pKeyManager(nullptr)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_vMoveDir(D3DXVECTOR3(0.f, 0.f, 0.f))
	, m_fMoveSpeed(25.f)
	, m_fFireTime(0.f)
	, m_fFireInterval(0.1f)
	, m_vBulletOffset(D3DXVECTOR3(3.8f, 1.2f, 0.f))
	, m_pCollider(nullptr)
	, m_pFinalBoss(nullptr)
{
}

CBattleShip::CBattleShip(const CBattleShip & _rOther)
	: CGameObject(_rOther)
	, m_pKeyManager(nullptr)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_vMoveDir(D3DXVECTOR3(0.f, 0.f, 0.f))
	, m_fMoveSpeed(25.f)
	, m_fFireTime(0.f)
	, m_fFireInterval(0.05f)
	, m_vBulletOffset(D3DXVECTOR3(2.0f, 0.f, 0.6f))
	, m_pCollider(nullptr)
	, m_bHit(false)
	, m_fHitDelay(1.f)
	, m_fHitDelayTime(0.f)
{
}

void CBattleShip::Free()
{
	CGameObject::Free();
	SafeRelease(m_pFinalBoss);
	SafeRelease(m_pKeyManager);
	SafeRelease(m_pTexturePool);
}

CBattleShip * CBattleShip::Create()
{
	CBattleShip* pInstance = new CBattleShip;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CBattleShip::Clone()
{
	CBattleShip* pClone = new CBattleShip(*this);
	return pClone;
}

HRESULT CBattleShip::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CBattleShip::Awake()
{
	CGameObject::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Rotation(D3DXVECTOR3(90.f, 0.f, 0.f));
	m_pTransform->Set_Scale(D3DXVECTOR3(7.6f, 4.8f, 1.f));
	m_pTransform->UpdateTransform();

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::BOX);
	m_pCollider->m_bExcept = true;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CBattleShip::Start()
{
	CGameObject::Start();
	m_pKeyManager = CKeyManager::GetInstance();

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("BattleShip"));
	SafeAddRef(m_pTexturePool);

	m_sTextureKey = TEXT("Body");
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();

	m_pHeartManager = (CHeartManager*)(FindGameObjectOfType<CHeartManager>());
	m_nHP = 12;
	m_pHeartManager->SetHeartCount(3);
	m_pHeartManager->SetGauge(m_nHP);

	m_sName = TEXT("Player");

	m_pFinalBoss = (CFinalBoss*)FindGameObjectOfType<CFinalBoss>();
	SafeAddRef(m_pFinalBoss);
	return S_OK;
}

UINT CBattleShip::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	if (m_pFinalBoss->GetHP() <= 0)
	{
		Animate(_fDeltaTime);
		ClearMission(_fDeltaTime);
		return 0;
	}
	Input(_fDeltaTime);
	Fire(_fDeltaTime);
	Move(_fDeltaTime);
	Animate(_fDeltaTime);
	return 0;
}

UINT CBattleShip::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	if (m_bHit)
		m_fHitDelayTime += _fDeltaTime;


	if (m_bHit  && m_fHitDelay < m_fHitDelayTime)
	{
		m_bHit = false;
		m_fHitDelayTime = 0.f;
	}
	return 0;
}

HRESULT CBattleShip::Render()
{
	CGameObject::Render();

	if (m_bHit)
	{
		int nCount = int(m_fHitDelayTime * 10);
		if (nCount % 2 == 0)
			return S_OK;
	}

	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

void CBattleShip::OnCollision(CGameObject * _pGameObject)
{
	if (m_bHit == false && (L"FinalBullet" == _pGameObject->GetName()))
	{
		m_bHit = true;
		SetHP(-1);
	}
}

void CBattleShip::SetHP(int _nHP)
{
	m_nHP += _nHP;
	m_pHeartManager->SetGauge(m_nHP);
}

void CBattleShip::Input(const float _fDeltaTime)
{
	m_vMoveDir = vZero;

	if (m_pKeyManager->Key_Press(KEY_W))
	{
		m_vMoveDir += D3DXVECTOR3(0.f, 0.f, 1.f);
	}
	else if (m_pKeyManager->Key_Press(KEY_S))
	{
		m_vMoveDir -= D3DXVECTOR3(0.f, 0.f, 1.f);
	}

	if (m_pKeyManager->Key_Press(KEY_A))
	{
		m_vMoveDir -= D3DXVECTOR3(1.f, 0.f, 0.f);
	}
	else if (m_pKeyManager->Key_Press(KEY_D))
	{
		m_vMoveDir += D3DXVECTOR3(1.f, 0.f, 0.f);
	}

	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);

}

void CBattleShip::Move(const float _fDeltaTime)
{
	m_pTransform->Add_Position(m_vMoveDir * _fDeltaTime * m_fMoveSpeed);
	m_pTransform->UpdateTransform();
}

void CBattleShip::Animate(const float _fDeltaTime)
{
	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimationSpeed)
	{
		m_fTime -= m_fAnimationSpeed;

		++m_nIndex;

		if (m_nIndex >= m_nMaxFrame)
			m_nIndex = 0;

		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}
}

void CBattleShip::Fire(const float _fDeltaTime)
{
	m_fFireTime -= _fDeltaTime;

	if (0.f < m_fFireTime)
		return;

	if (m_pKeyManager->Key_Press(KEY_LBUTTON))
	{
		CBattleShipBullet* pBullet = (CBattleShipBullet*)AddGameObject<CBattleShipBullet>();
		D3DXVECTOR3 vPosition = m_pTransform->Get_Position() + m_vBulletOffset;
		pBullet->SetBullet(vPosition);
		m_vBulletOffset.z = -m_vBulletOffset.z;
		m_fFireTime = m_fFireInterval;
	}
}

void CBattleShip::ClearMission(const float _fDeltaTime)
{
	static D3DXVECTOR3 vStart = m_pTransform->Get_Position();
	static float fInterpolation = 0.f;
	static D3DXVECTOR3 vEnd = D3DXVECTOR3(-25.f, 0.f, 0.f);

	if (fInterpolation < 1.f)
	{
		D3DXVECTOR3 vResult = (1.f - fInterpolation) * vStart + fInterpolation * vEnd;
		m_pTransform->Set_Position(vResult);
		m_pTransform->UpdateTransform();
	}
	else
	{
		if (fInterpolation > 1.5f)
		{
			m_vMoveDir = D3DXVECTOR3(1.f, 0.f, 0.f);
			m_fMoveSpeed = 40.f;
			Move(_fDeltaTime);

			if (m_pTransform->Get_Position().x > 60.f)
				CManagement::GetInstance()->SetUpCurrentScene(CEnding::Create());
		}
	}

	fInterpolation += _fDeltaTime * 1.f;

}

