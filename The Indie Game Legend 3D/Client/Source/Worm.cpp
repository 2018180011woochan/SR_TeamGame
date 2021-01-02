#include "stdafx.h"
#include "..\Header\Worm.h"
#include "TexturePoolManager.h"
#include "WormBody.h"
#include "WormConnector.h"
#include "WormTail.h"
#include "WormBullet.h"
#include "Player.h"
#include "BossHP.h"

CWorm::CWorm()
	: m_nMaxCount(0)
	, m_nIndex(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_ePattern(PATTERN::IDLE)
	, m_pBossHP(nullptr)
{
}

CWorm::CWorm(const CWorm & _rOther)
	: CWormPart(_rOther)
	, m_nMaxCount(0)
	, m_nIndex(0)
	, m_fTime(0.f)
	, m_fAnimationSpeed(0.1f)
	, m_ePattern(PATTERN::IDLE)
	, m_pBossHP(nullptr)
{
}

void CWorm::Free()
{
	CWormPart::Free();
	SafeRelease(m_pBossHP);
}

CWorm * CWorm::Create()
{
	CWorm* pInstance = new CWorm;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CWorm::Clone()
{
	CWorm* pClone = new CWorm(*this);
	return pClone;
}

HRESULT CWorm::InitializePrototype()
{
	CWormPart::InitializePrototype();
	return S_OK;
}

HRESULT CWorm::Awake()
{
	CWormPart::Awake();
	m_pMeshRenderer = (CMeshRenderer*)AddComponent<CMeshRenderer>();
	m_pMeshRenderer->SetMesh(TEXT("Quad"));

	m_pTransform->Set_Scale(D3DXVECTOR3(8.6f, 5.6f, 1.f));
	m_pTransform->Set_Position(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pTransform->UpdateTransform();

	m_pCollider = (CCollider*)AddComponent<CCollider>();

	m_pCollider->SetMesh(TEXT("Quad"), BOUND::BOUNDTYPE::SPHERE);
	m_pCollider->SetRadius(3.f);
	m_vMove = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_tAreaRect = { -60,52,60,-52 };

	m_eRenderID = ERenderID::Alpha;
	return S_OK;
}

HRESULT CWorm::Start()
{
	CWormPart::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("WormHead"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("Side");

	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[0]);

	m_nMaxCount = m_pTexturePool->GetTexture(m_sTextureKey).size();

	ConnectBody();

	m_pPlayerTransform = (CTransform*)(FindGameObjectOfType<CPlayer>()->GetComponent<CTransform>());
	
	m_pBossHP = (CBossHP*)FindGameObjectOfType<CBossHP>();
	SafeAddRef(m_pBossHP);
	return S_OK;
}

UINT CWorm::Update(const float _fDeltaTime)
{
	CWormPart::Update(_fDeltaTime);

	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	{
		SetPattern(PATTERN::BURROW);
	}

	DoPattern(_fDeltaTime);
	SetTextureKey();
	Animate(_fDeltaTime);
	return 0;
}

UINT CWorm::LateUpdate(const float _fDeltaTime)
{
	CWormPart::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CWorm::Render()
{
	CWormPart::Render();
	return S_OK;
}

void CWorm::ConnectBody()
{
	list<CGameObject*>		pBodyList;
	list<CGameObject*>		pConnectorList;
	CWormTail*				pWormTail = nullptr;
	pBodyList = FindGameObjectsOfType<CWormBody>();
	pConnectorList = FindGameObjectsOfType<CWormConnector>();
	pWormTail = (CWormTail*)FindGameObjectOfType<CWormTail>();
	UINT nSize = pBodyList.size() + pConnectorList.size() + 1;

	CWormPart* pPrevPart = this;
	CWormPart* pCurrPart = this;

	for (UINT i = 0; i < nSize; ++i)
	{
		if (nSize - 1 == i)
		{
			pCurrPart = pWormTail;
		}
		else if (i % 2 == 0)
		{
			pCurrPart = (CWormPart*)pConnectorList.front();
			pConnectorList.pop_front();
		}
		else
		{
			pCurrPart = (CWormPart*)pBodyList.front();
			pBodyList.pop_front();
		}

		pPrevPart->SetChild(pCurrPart);
		pCurrPart->SetParent(pPrevPart);

		pPrevPart = pCurrPart;
	}

	pBodyList.clear();
	pConnectorList.clear();
}

void CWorm::Animate(const float _fDeltaTime)
{
	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fAnimationSpeed)
	{
		m_fTime -= m_fAnimationSpeed;

		++m_nIndex;

		if (m_nIndex >= m_nMaxCount)
			m_nIndex = 0;

		m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(m_sTextureKey)[m_nIndex]);
	}
}

void CWorm::Move()
{
	m_pTransform->Add_Position(m_vMove);

	float fMove = D3DXVec3Length(&m_vMove);

	if (fabs(fMove - 0.f) > 0.000001f)
		RotationZ(m_vMove);


	D3DXVECTOR3 vDistance = m_pTransform->Get_Position() - m_vPrevPosition;
	float fDistance = D3DXVec3Length(&vDistance);

	m_pChild->Move(fDistance);

	if (fDistance >= m_fInterval)
		SetPrevPosition(m_pTransform->Get_Position());

	m_pTransform->UpdateTransform();
}

void CWorm::SetPattern(PATTERN _ePattern)
{
	m_ePattern = _ePattern;
	switch (m_ePattern)
	{
	case CWorm::IDLE:
		break;
	case CWorm::BURROW:
		m_vBurrowStart = m_pTransform->Get_Position();
		m_vBurrowEnd = m_pPlayerTransform->Get_Position();
		m_fInterpolation = 0.f;
		break;
	case CWorm::FARAWAY:
		m_vFarAwayStart = D3DXVECTOR3(0.f,0.f,0.f);
		m_vFarAwayEnd = CalculateFarAwayEnd();
		m_fInterpolation = 0.f;
		break;
	case CWorm::TRACE:
		m_fTraceTime = 0.f;
		break;
	case CWorm::FIRE:
		m_nFireCount = 0;
		m_fFireInterval = 0.f;
		break;
	default:
		break;
	}
}

void CWorm::DoPattern(const float _fDeltaTime)
{
	switch (m_ePattern)
	{
	case CWorm::IDLE:
		Idle(_fDeltaTime);
		break;
	case CWorm::BURROW:
		Burrow(_fDeltaTime);
		break;
	case CWorm::FARAWAY:
		FarAway(_fDeltaTime);
		break;
	case CWorm::TRACE:
		Trace(_fDeltaTime);
		break;
	case CWorm::FIRE:
		Fire(_fDeltaTime);
		break;
	default:
		break;
	}
}

void CWorm::Idle(const float _fDeltaTime)
{
	int nRand = rand() % int(PATTERN::PATTERN_END);

	switch (nRand)
	{
	case CWorm::IDLE:
		SetPattern(CWorm::IDLE);
		break;
	case CWorm::BURROW:
		SetPattern(CWorm::BURROW);
		break;
	case CWorm::FARAWAY:
		SetPattern(CWorm::IDLE);
		break;
	case CWorm::TRACE:
		SetPattern(CWorm::TRACE);
		break;
	case CWorm::FIRE:
		SetPattern(CWorm::FIRE);
		break;
	case CWorm::PATTERN_END:
		SetPattern(CWorm::IDLE);
		break;
	default:
		break;
	}
}

void CWorm::Burrow(const float _fDeltaTime)
{
	D3DXVECTOR3 vVia = 0.5f * (m_vBurrowStart + m_vBurrowEnd);
	vVia.y += 30.f;
	D3DXVECTOR3 vResult = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_fInterpolation += _fDeltaTime;
	vResult = powf(1 - m_fInterpolation, 2) * m_vBurrowStart + 2 * m_fInterpolation * (1 - m_fInterpolation) * vVia + powf(m_fInterpolation, 2)* m_vBurrowEnd;

	m_vMove = vResult - m_pTransform->Get_Position();

	Move();

	if (m_fInterpolation > 1.f)
		SetPattern(PATTERN::FARAWAY);
}

void CWorm::FarAway(const float _fDeltaTime)
{

	if (-2.f < m_pTransform->Get_Position().y)
	{
		D3DXVECTOR3 vDir = D3DXVECTOR3(0.f, -1.f, 0.f);

		float fRiseSpeed = 30.f;
		m_vMove = vDir * _fDeltaTime * fRiseSpeed;
		Move();
	}
	else
	{
		if (D3DXVECTOR3(0.f, 0.f, 0.f) == m_vFarAwayStart)
			m_vFarAwayStart = m_pTransform->Get_Position();

		D3DXVECTOR3 vVia = 0.5f * (m_vFarAwayStart + m_vFarAwayEnd);
		vVia.y = -30.f;
		D3DXVECTOR3 vResult = D3DXVECTOR3(0.f, 0.f, 0.f);

		m_fInterpolation += _fDeltaTime;
		vResult = powf(1 - m_fInterpolation, 2) * m_vFarAwayStart + 2 * m_fInterpolation * (1 - m_fInterpolation) * vVia + powf(m_fInterpolation, 2)* m_vFarAwayEnd;

		m_vMove = vResult - m_pTransform->Get_Position();

		Move();

		if (m_fInterpolation > 1.f)
			SetPattern(PATTERN::IDLE);
	}
}

void CWorm::Trace(const float _fDeltaTime)
{
	float fDeltaY = fabs(m_pTransform->Get_Position().y - (m_pPlayerTransform->Get_Position().y + 4.f));
	if (fDeltaY > 0.5f)
	{
		D3DXVECTOR3 vPlayerPos = m_pPlayerTransform->Get_Position();
		vPlayerPos.y += 4.f;
		D3DXVECTOR3 vDir = vPlayerPos - m_pTransform->Get_Position();

		vDir.x = 0.f;
		vDir.z = 0.f;

		float fTraceSpeed = 20.f;
		m_vMove = vDir * _fDeltaTime * fTraceSpeed;
	}
	else
	{
		D3DXVECTOR3 vPlayerPos = m_pPlayerTransform->Get_Position();
		vPlayerPos.y += 4.f;
		D3DXVECTOR3 vDir = vPlayerPos - m_pTransform->Get_Position();

		D3DXVec3Normalize(&vDir, &vDir);

		float fTraceSpeed = 20.f;

		m_vMove = vDir * _fDeltaTime * fTraceSpeed;
	}


	Move();

	m_fTraceTime += _fDeltaTime;

	if (m_fTraceTime > 4.f)
		SetPattern(PATTERN::FARAWAY);
}

void CWorm::Fire(const float _fDeltaTime)
{
	float fFirePosY = 30.f;

	if (fFirePosY > m_pTransform->Get_Position().y)
	{
		D3DXVECTOR3 vDir = D3DXVECTOR3(0.f, 1.f, 0.f);

		float fRiseSpeed = 30.f;
		m_vMove = vDir * _fDeltaTime * fRiseSpeed;
		Move();
	}
	else
	{
		m_fFireInterval += _fDeltaTime;

		if (m_fFireInterval > 0.5f)
		{
			m_fFireInterval -= 0.5f;

			CWormBullet* pWormBullet = (CWormBullet*)AddGameObject<CWormBullet>();

			pWormBullet->SetBullet(m_pTransform->Get_Position(), m_pPlayerTransform->Get_Position());

			++m_nFireCount;
		}

		m_sTextureKey = TEXT("Front");
		m_pTransform->Set_Scale(D3DXVECTOR3(6.4f, 6.4f, 1.f));
	}

	if (m_nFireCount == 5)
		SetPattern(PATTERN::FARAWAY);
}

D3DXVECTOR3 CWorm::CalculateFarAwayEnd()
{
	D3DXVECTOR3 vCurrPos = m_pTransform->Get_Position();

	//임의의 x 값.
	float fMinX = vCurrPos.x;
	float fMaxX = 0.f;

	if (fabs(m_tAreaRect.left - vCurrPos.x) < fabs(m_tAreaRect.right - vCurrPos.x))
		fMaxX = (float)m_tAreaRect.right;
	else
		fMaxX = (float)m_tAreaRect.left;

	int nRand = rand() % 5 + 3;

	float fResultX = fMinX + (fMaxX - fMinX) * nRand * 0.1f;

	//임의의 z 값
	float fMinZ = vCurrPos.z;
	float fMaxZ = 0.f;

	if (fabs(m_tAreaRect.top - vCurrPos.z) < fabs(m_tAreaRect.bottom - vCurrPos.z))
		fMinZ = (float)m_tAreaRect.bottom;
	else
		fMaxZ = (float)m_tAreaRect.top;

	nRand = rand() % 5 + 3;

	float fResultZ = fMinZ + (fMaxZ - fMinZ) * nRand * 0.1f;

	return D3DXVECTOR3(fResultX, 0.f, fResultZ);
}

void CWorm::SetTextureKey()
{
	if (PATTERN::FIRE == m_ePattern)
		return;

	D3DXVECTOR3 vDir = m_vMove;
	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVECTOR3 vPlayerLook = m_pPlayerTransform->Get_Look();
	vPlayerLook = -vPlayerLook;
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	float fDot = D3DXVec3Dot(&vDir, &vPlayerLook);
	float fRadian = acosf(fDot);
	float fDegree = D3DXToDegree(fRadian);

	if (fDegree < 90.f)
	{
		m_sTextureKey = TEXT("Front");
		m_pTransform->Set_Scale(D3DXVECTOR3(6.4f, 6.4f, 1.f));
	}
	else
	{
		m_sTextureKey = TEXT("Side");
		m_pTransform->Set_Scale(D3DXVECTOR3(8.6f, 5.6f, 1.f));
	}
	m_pTransform->UpdateTransform();
}


