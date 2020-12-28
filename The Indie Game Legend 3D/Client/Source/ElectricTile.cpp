#include "stdafx.h"
#include "ElectricTile.h"


CElectricTile::CElectricTile()
{
}

CElectricTile::CElectricTile(const CElectricTile & _rOther)
	:CTile(_rOther)
{
}

HRESULT CElectricTile::Animate(const float _fDeltaTime)
{
	m_fTextureIndex += _fDeltaTime * m_fAnimateSpeed;
	if (m_nMaxFrame < m_fTextureIndex)
	{
		m_fTextureIndex = 0.f;// 디버깅작업 생각해서 아예 0으로 초기화 
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Electric"))[(_int)m_fTextureIndex]);
	return S_OK;
}

HRESULT CElectricTile::InitializePrototype()
{
	CTile::InitializePrototype();
	return S_OK;
}

HRESULT CElectricTile::Awake()
{
	CTile::Awake();

	m_sName = L"Electric";
	return S_OK;
}

HRESULT CElectricTile::Start()
{
	CTile::Start();

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("Floor"));
	if (nullptr == m_pTexturePool)
	{
		PrintLog(L"Error", L"pTexturePool is null by CElectricTile::Start()");
		return E_FAIL;
	}
	else
		SafeAddRef(m_pTexturePool);
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Electric"))[0]);
	m_nMaxFrame = m_pTexturePool->GetTexture(TEXT("Electric")).size();
	m_fAnimateOneCycleTime = 1.5f;

	m_fAnimateSpeed = (m_nMaxFrame) / 1.f * m_fAnimateOneCycleTime;
	m_pTransform->UpdateTransform();

	return S_OK;
}

UINT CElectricTile::Update(const float _fDeltaTime)
{
	if (FAILED(Animate(_fDeltaTime)))
		return OBJ_NOENVET;
	
	return OBJ_NOENVET;
}

UINT CElectricTile::LateUpdate(const float _fDeltaTime)
{
	CTile::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CElectricTile::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();

	return S_OK;
}

CGameObject * CElectricTile::Clone()
{
	CElectricTile* pInstance = new CElectricTile(*this);
	return pInstance;
}

CElectricTile * CElectricTile::Create()
{
	CElectricTile* pInstance = new CElectricTile();
	return pInstance;
}

void CElectricTile::Free()
{
	SafeRelease(m_pTexturePool);
	CTile::Free();
}
