#include "stdafx.h"
#include "..\Header\StarPool.h"
#include "StarParticle.h"

CStarPool::CStarPool()
	: m_fScatterInterval(0.1f)
	, m_fTime(0.f)
{
}

CStarPool::CStarPool(const CStarPool & _rOther)
	: CGameObject(_rOther)
	, m_fScatterInterval(0.1f)
	, m_fTime(0.f)
{
}

void CStarPool::Free()
{
	CGameObject::Free();
	//for (auto pStarParticle : m_StarContainer)
	//{
	//	SafeRelease(pStarParticle);
	//}
	m_StarParticles.clear();
}

CStarPool * CStarPool::Create()
{
	CStarPool* pInstance = new CStarPool;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}


CGameObject * CStarPool::Clone()
{
	CStarPool* pClone = new CStarPool(*this);
	return pClone;
}

HRESULT CStarPool::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CStarPool::Awake()
{
	CGameObject::Awake();
	return S_OK;
}

HRESULT CStarPool::Start()
{
	CGameObject::Start();
	list<CGameObject*> Particles = FindGameObjectsOfType<CStarParticle>();

	for (auto pGameObject : Particles)
	{
		m_StarParticles.push_back((CStarParticle*)pGameObject);
		//m_StarContainer.push_back((CStarParticle*)pGameObject);
		pGameObject->SetEnable(false);
		//SafeAddRef(pGameObject);
	}

	return S_OK;
}

UINT CStarPool::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	Scatter(_fDeltaTime);
	return 0;
}

UINT CStarPool::LateUpdate(const float _fDeltaTime)
{
	return 0;
}

HRESULT CStarPool::Render()
{
	return S_OK;
}

void CStarPool::Scatter(const float _fDeltaTime)
{
	m_fTime += _fDeltaTime;

	if (m_fTime >= m_fScatterInterval)
	{
		m_fTime -= m_fScatterInterval;

		if (0 == m_StarParticles.size())
			return;

		CStarParticle* pStarParticle = m_StarParticles.front();
		pStarParticle->SetEnable(true);

		m_StarParticles.pop_front();
	}
}

void CStarPool::PushToPool(CStarParticle * _pStarParticle)
{
	m_StarParticles.push_back(_pStarParticle);

}

