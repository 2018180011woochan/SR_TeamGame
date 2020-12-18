#include "stdafx.h"
#include "LavaTile.h"

CLavaTile::CLavaTile()
{
}

CLavaTile::CLavaTile(const CLavaTile & _rOther)
	:CTile(_rOther)
{
}

HRESULT CLavaTile::Animate(const float _fDeltaTime)
{
	m_fTextureIndex += _fDeltaTime * m_fAnimateSpeed;
	if (m_nMaxFrame < m_fTextureIndex)
	{
		m_fTextureIndex = 0.f;// 디버깅작업 생각해서 아예 0으로 초기화 
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Lava"))[(_int)m_fTextureIndex]);
	return S_OK;
}
HRESULT CLavaTile::InitializePrototype()
{
	CTile::InitializePrototype();
	return S_OK;
}

HRESULT CLavaTile::Awake()
{
	CTile::Awake();
	return S_OK;
}

HRESULT CLavaTile::Start()
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
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Lava"))[0]);
	m_nMaxFrame = m_pTexturePool->GetTexture(TEXT("Lava")).size();
	m_fAnimateOneCycleTime = 1.5f;

	m_fAnimateSpeed = (m_nMaxFrame + 1) / 1.f * m_fAnimateOneCycleTime;
	m_pTransform->UpdateTransform();

	return S_OK;
}

UINT CLavaTile::Update(const float _fDeltaTime)
{
	if (FAILED(Animate(_fDeltaTime)))
		return OBJ_NOENVET;

	return OBJ_NOENVET;
}

UINT CLavaTile::LateUpdate(const float _fDeltaTime)
{
	CTile::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CLavaTile::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CLavaTile::Clone()
{
	CLavaTile* pInstance = new CLavaTile(*this);
	return pInstance;
}

CLavaTile * CLavaTile::Create()
{
	CLavaTile* pInstance = new CLavaTile();
	return pInstance;
}

void CLavaTile::Free()
{
	SafeRelease(m_pTexturePool);
	CTile::Free();
}