#include "stdafx.h"
#include "..\Header\BattleShip.h"
#include "KeyManager.h"
#include "TexturePoolManager.h"
#include "BattleShipBullet.h"

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
{
}

void CBattleShip::Free()
{
	CGameObject::Free();
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


	return S_OK;
}

UINT CBattleShip::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	Input(_fDeltaTime);
	Fire(_fDeltaTime);
	Move(_fDeltaTime);
	Animate(_fDeltaTime);
	return 0;
}

UINT CBattleShip::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CBattleShip::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
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

