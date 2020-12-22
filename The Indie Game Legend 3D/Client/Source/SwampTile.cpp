#include "stdafx.h"
#include "..\Header\SwampTile.h"



CSwampTile::CSwampTile()
{
}

CSwampTile::CSwampTile(const CSwampTile & _rOther)
	:CTile(_rOther)
{
}

HRESULT CSwampTile::Animate(const float _fDeltaTime)
{
	m_fTextureIndex += _fDeltaTime * m_fAnimateSpeed;
	if (m_nMaxFrame < m_fTextureIndex)
	{
		m_fTextureIndex = 0.f;// 디버깅작업 생각해서 아예 0으로 초기화 
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Swamp"))[(_int)m_fTextureIndex]);
	return S_OK;
}

HRESULT CSwampTile::InitializePrototype()
{
	CTile::InitializePrototype();
	return S_OK;
}

HRESULT CSwampTile::Awake()
{
	CTile::Awake();

	m_sName = L"Swamp";

	return S_OK;
}

HRESULT CSwampTile::Start()
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
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Swamp"))[0]);
	m_nMaxFrame = m_pTexturePool->GetTexture(TEXT("Swamp")).size();
	m_fAnimateOneCycleTime = 1.5f;

	m_fAnimateSpeed = (m_nMaxFrame + 1) / 1.f * m_fAnimateOneCycleTime;
	m_pTransform->UpdateTransform();

	return S_OK;
}

UINT CSwampTile::Update(const float _fDeltaTime)
{
	if (FAILED(Animate(_fDeltaTime)))
		return OBJ_NOENVET;

	return OBJ_NOENVET;
}

UINT CSwampTile::LateUpdate(const float _fDeltaTime)
{
	CTile::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CSwampTile::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CSwampTile::Clone()
{
	CSwampTile* pInstance = new CSwampTile(*this);
	return pInstance;
}

CSwampTile * CSwampTile::Create()
{
	CSwampTile* pInstance = new CSwampTile();
	return pInstance;
}

void CSwampTile::Free()
{
	SafeRelease(m_pTexturePool);
	CTile::Free();
}