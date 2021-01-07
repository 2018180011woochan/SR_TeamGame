#include "stdafx.h"
#include "..\Header\FinalBoss.h"
#include "TexturePoolManager.h"
#include "BattleShip.h"

#include "FinalNormal.h"
#include "FinalLaser.h"

#include "BossHP.h"
#include "Explosion.h"
CFinalBoss::CFinalBoss()
	: m_pPlayer(nullptr)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_fNormalInterval(0.1f)
	, m_fCreateTime(0.f)
{
}

CFinalBoss::CFinalBoss(const CFinalBoss & _rOther)
	: CMonster(_rOther)
	, m_pPlayer(nullptr)
	, m_pMeshRenderer(nullptr)
	, m_pTexturePool(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_fNormalInterval(0.1f)
	, m_fCreateTime(0.f)
	, m_nIndex(0)
	, m_nMaxFrame(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_bLoop(true)
	, m_bReverse(false)
	, m_fMoveSpeed(10.f)
	, m_fEffectTime(0.f)
	, m_nEffectCount(0)
{
}

void CFinalBoss::Free()
{
	CMonster::Free();
	SafeRelease(m_pPlayer);
	SafeRelease(m_pTexturePool);
}

CFinalBoss * CFinalBoss::Create()
{
	CFinalBoss* pInstance = new CFinalBoss;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CFinalBoss::Clone()
{
	CFinalBoss* pClone = new CFinalBoss(*this);
	return pClone;
}

HRESULT CFinalBoss::InitializePrototype()
{
	CMonster::InitializePrototype();
	return S_OK;
}

HRESULT CFinalBoss::Awake()
{
	CMonster::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Scale(D3DXVECTOR3(25.f, 25.f, 25.f));
	m_pTransform->Set_Position(D3DXVECTOR3(30.f, 0.f, 0.f));
	m_pTransform->Set_Rotation(D3DXVECTOR3(90.f, 0.f, 0.f));
	m_pTransform->UpdateTransform();

	m_pCollider = (CCollider*)AddComponent<CCollider>();
	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->SetRadius(9.f);
	m_pCollider->m_bExcept = true;

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CFinalBoss::Start()
{
	CMonster::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("FinalBoss"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("Idle");

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[0]);

	m_pPlayerTransform = ((CTransform*)(FindGameObjectOfType<CBattleShip>()->GetComponent<CTransform>()));

	SetPattern(PATTERN::NORMAL);

	m_bTest = true;

	m_sName = true;

	m_nHP = 100;
	m_nMaxHP = 100;

	m_pBossHP = (CBossHP*)FindGameObjectOfType<CBossHP>();
	m_pBossHP->SetHPBar(float(m_nHP) / float(m_nMaxHP));

	m_sName = TEXT("FinalBoss");

	m_bRemove = false;
	return S_OK;
}

UINT CFinalBoss::Update(const float _fDeltaTime)
{
	//CMonster::Update(_fDeltaTime);
	if (true == m_bRemove)
		return OBJ_DEAD;

	m_vMoveDirection = D3DXVECTOR3(0.f, 0.f, 0.f);
	InputTest(_fDeltaTime);
	DoPattern(_fDeltaTime);
	Animate(_fDeltaTime);
	Move(_fDeltaTime);

	ExplosionLaser(_fDeltaTime);
	return 0;
}

UINT CFinalBoss::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CFinalBoss::Render()
{
	CMonster::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

void CFinalBoss::OnCollision(CGameObject * _pGameObject)
{
	if ((L"PlayerBullet" == _pGameObject->GetName()))
	{
		SetHP(-1);
	}
}

void CFinalBoss::Animate(const float _fDeltaTime)
{
	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimationSpeed)
	{
		m_fTime -= m_fAnimationSpeed;

		if (false == m_bReverse)
		{
			++m_nIndex;
			if (m_nIndex >= m_nMaxFrame)
			{
				if (m_bLoop)
					m_nIndex = 0;
				else
					m_nIndex = m_nMaxFrame - 1;
			}
		}
		else
		{
			--m_nIndex;
			if (m_nIndex < 0)
			{
				if (m_bLoop)
					m_nIndex = m_nMaxFrame;
				else
					m_nIndex = 0;
			}
		}
		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}
}

void CFinalBoss::Move(const float _fDeltaTime)
{
	m_pTransform->Add_Position(m_vMoveDirection * m_fMoveSpeed * _fDeltaTime);
	m_pTransform->UpdateTransform();
}

void CFinalBoss::SetTextureKey(const TSTRING _sTextureKey)
{
	m_sTextureKey = _sTextureKey;
	m_nMaxFrame = m_pTexturePool->GetTexture(m_sTextureKey).size();
	if (m_bReverse)
		m_nIndex = m_nMaxFrame - 1;
	else
		m_nIndex = 0;
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
}

void CFinalBoss::DoPattern(const float _fDeltaTime)
{
	switch (m_ePattern)
	{
	case CFinalBoss::IDLE:
		Idle(_fDeltaTime);
		break;
	case CFinalBoss::NORMAL:
		Normal(_fDeltaTime);
		break;
	case CFinalBoss::DEAD:
		Dead(_fDeltaTime);
		break;
	default:
		break;
	}
}

void CFinalBoss::SetPattern(PATTERN _ePattern)
{
	m_ePattern = _ePattern;
	switch (m_ePattern)
	{
	case CFinalBoss::IDLE:
		SetTextureKey(TEXT("Idle"));
		m_vViaStart = m_pTransform->Get_Position();
		m_vVia[0] = D3DXVECTOR3(34.f, 0.f, 14.f);
		m_vVia[1] = D3DXVECTOR3(14.f, 0.f, 0.f);
		m_vVia[2] = D3DXVECTOR3(34.f, 0.f, -14.f);
		{
			float fMin = -1.f;
			float fLength = 0.f;
			D3DXVECTOR3 vLength;
			for (UINT i = 0; i < 3; ++i)
			{
				vLength = m_pTransform->Get_Position() - m_vVia[i];
				fLength = D3DXVec3Length(&vLength);
				if (fMin < 0.f || fLength < fMin)
				{
					fMin = fLength;
					m_nViaIndex = i;
				}
			}
		}
		break;
	case CFinalBoss::NORMAL:
		SetTextureKey(TEXT("Normal"));
		m_nIndex = 0;
		m_fMoveSpeed = 10.f;
		m_bLoop = false;
		m_bReverse = false;
		break;
	default:
		break;
	}
}

void CFinalBoss::Idle(const float _fDeltaTime)
{
	static float fContinuanceTime = 0.f;
	fContinuanceTime += _fDeltaTime;

	static float fInterpolation = 0.f;
	fInterpolation += _fDeltaTime;

	D3DXVECTOR3 vEnd = m_vVia[(m_nViaIndex) % 3];

	D3DXVECTOR3 vResult = (1.f - fInterpolation) * m_vViaStart + fInterpolation * vEnd;

	m_pTransform->Set_Position(vResult);

	if (1.f < fInterpolation)
	{
		fInterpolation = 0.f;
		m_vViaStart = m_pTransform->Get_Position();
		++m_nViaIndex;
	}

	if (fContinuanceTime > 1.f)
	{
		SetPattern((PATTERN)(1 + rand() % (PATTERN::PATTERN_END - 2)));
		fContinuanceTime = 0.f;
		fInterpolation = 0.f;
	}
}

void CFinalBoss::Normal(const float _fDeltaTime)
{
	static float fContinuanceTime = 0.f;
	static bool  bBackWard = false;
	m_fCreateTime -= _fDeltaTime;
	
	if (0.f > m_fCreateTime)
	{
		if (m_nIndex == m_nMaxFrame - 1)
		{
			CFinalNormal* pFinalNormal = (CFinalNormal*)AddGameObject<CFinalNormal>();

			D3DXVECTOR3 vPosition = m_pTransform->Get_Position();
			vPosition.z -= 7.f;
			pFinalNormal->SetBullet(vPosition);
			m_fCreateTime = m_fNormalInterval;

			fContinuanceTime += _fDeltaTime;
		}
	}

	if (fContinuanceTime > 0.5f)
	{
		m_bReverse = true;

		if (0 == m_nIndex)
		{
			fContinuanceTime = 0.f;
			SetPattern(PATTERN::IDLE);
		}
	}

	if (m_pTransform->Get_Position().x < 14.f)
		bBackWard = true;
	else if (m_pTransform->Get_Position().x > 35.f)
		bBackWard = false;

	D3DXVECTOR3 vPosition = m_pTransform->Get_Position();
	vPosition.z -= 7.f;

	m_vMoveDirection = m_pPlayerTransform->Get_Position() - vPosition;
	m_vMoveDirection.z = 2 * m_vMoveDirection.z;
	D3DXVec3Normalize(&m_vMoveDirection, &m_vMoveDirection);

	m_vMoveDirection.x = (bBackWard ? -m_vMoveDirection.x : m_vMoveDirection.x);

}

void CFinalBoss::Dead(const float _fDeltaTime)
{
	m_fEffectTime += _fDeltaTime;

	if (m_fEffectTime > 0.2f)
	{
		m_fEffectTime -= 0.2f;

		if (m_nEffectCount < 10)
		{
			CExplosion * m_pEffect = (CExplosion*)AddGameObject<CExplosion>();
			int iRandX = -10 + rand() % 20;
			int iRandZ = -10 + rand() % 20;
			D3DXVECTOR3 vPosition = m_pTransform->Get_Position();
			vPosition.x += iRandX;
			vPosition.z += iRandZ;
			m_pEffect->SetPosition(vPosition);
			m_nEffectCount++;
		}
		else
		{
			m_bRemove = true;
		}
	}
}

void CFinalBoss::ExplosionLaser(const float _fDeltaTime)
{
	static int nTest = 0;
	static float fTime = 0;
	fTime += _fDeltaTime;
	if (fTime > 10.f)
	{
		fTime -= 10.f;
		nTest = 0;
	}
	if (nTest != 0)
		return;

	CFinalLaser* pFinalLaser = nullptr;
	for (UINT i = 0; i < 4; ++i)
	{
		pFinalLaser = (CFinalLaser*)AddGameObject<CFinalLaser>();
		pFinalLaser->SetLaser((CFinalLaser::DIRECTION)i);
	}
	nTest = 4;
	m_bTest = false;
}

void CFinalBoss::InputTest(const float _fDeltaTime)
{
	D3DXVECTOR3 vDir = D3DXVECTOR3(0.f, 0.f, 0.f);

	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
	{
		vDir.x = -1.f;
	}
	else if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
	{
		vDir.x = 1.f;
	}

	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	{
		vDir.z = 1.f;
	}
	else if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
	{
		vDir.z = -1.f;
	}

	if (GetAsyncKeyState('0') & 0x8000)
	{
		m_bTest = true;
	}

	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransform->Add_Position(vDir * 30.f * _fDeltaTime);
	m_pTransform->UpdateTransform();
}

void CFinalBoss::SetHP(int _nHP)
{
	m_nHP += _nHP;
	m_pBossHP->SetHPBar(float(m_nHP) / float(m_nMaxHP));
	
	if (m_nHP <= 0)
	{
		SetPattern(PATTERN::DEAD);
		m_pBossHP->SetEnable(false);
	}
}
