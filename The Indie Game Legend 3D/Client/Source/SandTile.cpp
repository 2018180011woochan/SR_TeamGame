#include "stdafx.h"
#include "SandTile.h"



CSandTile::CSandTile()
{
}

CSandTile::CSandTile(const CSandTile & _rOther)
	:CTile(_rOther)
{

}

HRESULT CSandTile::Animate(const float _fDeltaTime)
{
	m_fTextureIndex += _fDeltaTime * m_fAnimateSpeed;
	if (m_nMaxFrame < m_fTextureIndex)
	{
		m_fTextureIndex = 0.f;// 디버깅작업 생각해서 아예 0으로 초기화 
	}
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Sand"))[(_int)m_fTextureIndex]);
	return S_OK;
}

HRESULT CSandTile::InitializePrototype()
{
	CTile::InitializePrototype();
	return S_OK;
}

HRESULT CSandTile::Awake()
{
	CTile::Awake();
	return S_OK;
}

HRESULT CSandTile::Start()
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
	m_pMeshRenderer->SetTexture(0, m_pTexturePool->GetTexture(TEXT("Sand"))[0]);
	m_nMaxFrame = m_pTexturePool->GetTexture(TEXT("Sand")).size();
	m_fAnimateOneCycleTime = 1.5f;

	m_fAnimateSpeed = m_nMaxFrame / 1.f * m_fAnimateOneCycleTime;

	m_pTransform->UpdateTransform();
	return S_OK;
}

UINT CSandTile::Update(const float _fDeltaTime)
{
	if (FAILED(Animate(_fDeltaTime)))
		return OBJ_NOENVET;

	return OBJ_NOENVET;
}

UINT CSandTile::LateUpdate(const float _fDeltaTime)
{
	CTile::LateUpdate(_fDeltaTime);
	return OBJ_NOENVET;
}

HRESULT CSandTile::Render()
{
	m_pTransform->UpdateWorld();
	m_pMeshRenderer->Render();
	return S_OK;
}

CGameObject * CSandTile::Clone()
{
	CSandTile* pInstance = new CSandTile(*this);
	return pInstance;
}

CSandTile * CSandTile::Create()
{
	CSandTile* pInstance = new CSandTile();
	return pInstance;
}

void CSandTile::Free()
{
	SafeRelease(m_pTexturePool);
	CTile::Free();
}