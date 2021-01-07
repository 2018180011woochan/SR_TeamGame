#include "stdafx.h"
#include "..\Header\WormPart.h"
#include "TexturePoolManager.h"
#include "Worm.h"
#include "Explosion.h"

CWormPart::CWormPart()
	: m_pParent(nullptr)
	, m_pChild(nullptr)
	, m_pTexturePool(nullptr)
	, m_pMeshRenderer(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_fInterval(4.f)
{
}

CWormPart::CWormPart(const CWormPart & _rOther)
	: CMonster(_rOther)
	, m_pParent(nullptr)
	, m_pChild(nullptr)
	, m_pTexturePool(nullptr)
	, m_pMeshRenderer(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_fInterval(4.f)
	, m_fEffectTime(0.f)
	, m_nEffectCount(0)
	, m_bRemove(false)
{
}

void CWormPart::Free()
{
	CMonster::Free();
	SafeRelease(m_pParent);
	//SafeRelease(m_pChild);
	SafeRelease(m_pTexturePool);
}

CWormPart * CWormPart::Create()
{
	CWormPart* pInstance = new CWormPart;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CWormPart::Clone()
{
	CWormPart* pClone = new CWormPart(*this);
	return pClone;
}

HRESULT CWormPart::InitializePrototype()
{
	CMonster::InitializePrototype();
	return S_OK;
}

HRESULT CWormPart::Awake()
{
	CMonster::Awake();
	m_pTransform->Set_Position(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_vPrevPosition = m_pTransform->Get_Position();
	return S_OK;
}

HRESULT CWormPart::Start()
{
	CMonster::Start();
	m_pWorm = (CWorm*)FindGameObjectOfType<CWorm>();
	return S_OK;
}

UINT CWormPart::Update(const float _fDeltaTime)
{
	CMonster::Update(_fDeltaTime);
	return 0;
}

UINT CWormPart::LateUpdate(const float _fDeltaTime)
{
	CMonster::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CWormPart::Render()
{
	CMonster::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pMeshRenderer->Render();
	return S_OK;
}

void CWormPart::OnEnable()
{
	if (nullptr != m_pChild)
		m_pChild->SetEnable(true);
}

void CWormPart::OnDisable()
{
	if (nullptr != m_pChild)
		m_pChild->SetEnable(false);
}

void CWormPart::OnCollision(CGameObject * _pGameObject)
{
	if (m_bHit == false && (L"PlayerBullet" == _pGameObject->GetName()))
	{
		m_pWorm->SetHP(-1);
		m_bHit = true;
	}
	else if (m_bHit == false && L"ExplosionBlue" == _pGameObject->GetName())
	{
		m_pWorm->SetHP(-1);
		m_bHit = true;
	}
}

void CWormPart::RotationZ(D3DXVECTOR3 _vMoveDir)
{
	D3DXVec3Normalize(&_vMoveDir, &_vMoveDir);

	D3DXVECTOR3 vLook = m_pTransform->Get_Look();
	D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);
	D3DXVECTOR3 vRight = D3DXVECTOR3(1.f, 0.f, 0.f);

	//���� ���� ���͸� ���� ��ǥ���� x-y ��鿡 ����.
	D3DXVECTOR3 vProjectToPlane = _vMoveDir - D3DXVec3Dot(&_vMoveDir, &vLook) * vLook;
	//������ ���� ����ȭ
	D3DXVec3Normalize(&vProjectToPlane, &vProjectToPlane);

	//���� ��ǥ���� x�� ���� ���� ���.
	D3DXVec3Cross(&vRight, &vUp, &vLook);
	D3DXVec3Normalize(&vRight, &vRight);
	//������ ���Ϳ� x����� ���� ���
	float fDot = D3DXVec3Dot(&vProjectToPlane, &vRight);
	float fRadian = acosf(fDot);
	float fDegree = D3DXToDegree(fRadian);

	if (vProjectToPlane.y < 0.f)
		fDegree = -fDegree;

	//���� ������ ȸ��
	m_pTransform->Set_RotationZ(fDegree);
}

void CWormPart::Move(const float _fDistance)
{
	if (m_pParent->GetPrevPosition() == m_vPrevPosition)
		return;

	m_vMoveDir = D3DXVECTOR3(0.f, 0.f, 0.f);

	m_vMoveDir = m_pParent->GetPrevPosition() - m_vPrevPosition;
	D3DXVec3Normalize(&m_vMoveDir, &m_vMoveDir);

	m_pTransform->Set_Position(m_vPrevPosition + m_vMoveDir * _fDistance);

	RotationZ(m_vMoveDir);

	if (nullptr != m_pChild)
		m_pChild->Move(_fDistance);

}

void CWormPart::Dead(const float _fDeltaTime)
{
	if (m_pChild)
		m_pChild->Dead(_fDeltaTime);
	m_fEffectTime += _fDeltaTime;

	if (m_fEffectTime > 0.5f)
	{
		m_fEffectTime -= 0.5f;

		if (m_nEffectCount < 5)
		{
			m_pEffect = (CExplosion*)AddGameObject<CExplosion>();
			int iRandX = -5 + rand() % 10;
			int iRandY = -5 + rand() % 10;
			int iRandZ = -5 + rand() % 10;
			D3DXVECTOR3 vPosition = m_pTransform->Get_Position();
			vPosition.x += iRandX;
			vPosition.y += iRandY;
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

void CWormPart::SetParent(CWormPart * _pParent)
{
	//SafeRelease(m_pParent);
	m_pParent = _pParent;
	//SafeAddRef(m_pParent);
}

void CWormPart::SetChild(CWormPart * _pChild)
{
	//SafeRelease(m_pChild);
	m_pChild = _pChild;
	//SafeAddRef(m_pChild);
}

void CWormPart::SetPrevPosition(D3DXVECTOR3 _vPrevPosition)
{
	if (nullptr != m_pChild)
		m_pChild->SetPrevPosition(m_vPrevPosition);

	m_vPrevPosition = _vPrevPosition;
}

D3DXVECTOR3 CWormPart::GetPrevPosition()
{
	return m_vPrevPosition;
}
