#include "stdafx.h"
#include "..\Header\HeartManager.h"
#include "Heart.h"

CHeartManager::CHeartManager()
	: m_nMaxHeart(6)
	, m_pHeart(nullptr)
{
}

CHeartManager::CHeartManager(const CHeartManager & _rOther)
	:CGameObject(_rOther)
	,m_nMaxHeart(6)
{
}

void CHeartManager::Free()
{
	CGameObject::Free();
	if (nullptr != m_pHeart)
	{
		for (UINT i = 0; i < m_nMaxHeart; ++i)
		{
			SafeRelease(m_pHeart[i]);
		}
		SafeDeleteArray(m_pHeart);
	}
}

CHeartManager * CHeartManager::Create()
{
	CHeartManager* pInstance = new CHeartManager;

	if (FAILED(pInstance->InitializePrototype()))
	{
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

CGameObject * CHeartManager::Clone()
{
	CHeartManager* pClone = new CHeartManager(*this);
	return pClone;
}

HRESULT CHeartManager::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CHeartManager::Awake()
{
	CGameObject::Awake();
	return S_OK;
}

HRESULT CHeartManager::Start()
{
	CGameObject::Start();
	m_pHeart = new CHeart*[m_nMaxHeart];
	for (UINT i = 0; i < m_nMaxHeart; ++i)
	{
		m_pHeart[i] = (CHeart*)(AddGameObject<CHeart>());
		SafeAddRef(m_pHeart[i]);
		((CTransform*)(m_pHeart[i]->GetComponent<CTransform>()))->Set_Position(D3DXVECTOR3(-575.f + 59.5f * i, 302.5f, 0.f));
		((CTransform*)(m_pHeart[i]->GetComponent<CTransform>()))->UpdateTransform();
	}
	return S_OK;
}

UINT CHeartManager::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);

	static UINT nMax = 3;
	static UINT nHP = 12;
	if (GetAsyncKeyState('L') & 0x0001)
	{
		nMax++;
	}
	else if (GetAsyncKeyState('K') & 0x0001)
	{
		nMax--;
	}
	if (GetAsyncKeyState('J') & 0x0001)
	{
		nHP++;

	}
	else if (GetAsyncKeyState('H') & 0x0001)
	{
		nHP--;
	}
	nHP = CLAMP(nHP,12, 24);
	nMax = CLAMP(nMax, 3, 6);

	SetHeartCount(nMax);
	SetGauge(nHP);
	return 0;
}

UINT CHeartManager::LateUpdate(const float _fDeltaTime)
{
	//CGameObject::LateUpdate(_fDeltaTime);
	return 0;
}

HRESULT CHeartManager::Render()
{
	CGameObject::Render();
	return S_OK;
}

void CHeartManager::SetHeartCount(const UINT _nHeartCount)
{
	if (_nHeartCount > m_nMaxHeart)
		return;

	m_nHeartCount = _nHeartCount;

	for (UINT i = 0; i < m_nMaxHeart; ++i)
	{
		if (m_nHeartCount > i)
			m_pHeart[i]->SetEnable(true);
		else
			m_pHeart[i]->SetEnable(false);

	}
}

void CHeartManager::SetGauge(const UINT _nGauge)
{
	m_nGauge = _nGauge;

	int nGauge = _nGauge;

	for (UINT i = 0; i < m_nHeartCount; ++i)
	{
		if (4 < nGauge)
			m_pHeart[i]->SetGauge(4);
		else if (0 >= nGauge)
			m_pHeart[i]->SetGauge(0);
		else
			m_pHeart[i]->SetGauge(nGauge);

		nGauge -= 4;
	}
}

