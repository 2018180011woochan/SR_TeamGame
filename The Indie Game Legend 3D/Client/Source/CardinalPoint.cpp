#include "stdafx.h"
#include "..\Header\CardinalPoint.h"
#include "TexturePoolManager.h"

CCardinalPoint::CCardinalPoint()
	: m_pTexturePool(nullptr)
	, m_pImage(nullptr)
	, m_sTextureKey(TEXT(""))
	, m_fDegree(0.f)
{
}

CCardinalPoint::CCardinalPoint(const CCardinalPoint & _rOther)
	: CGameObject(_rOther)
{
}

void CCardinalPoint::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
}

CCardinalPoint * CCardinalPoint::Create()
{
	CCardinalPoint* pInstance = new CCardinalPoint;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CCardinalPoint::Clone()
{
	CCardinalPoint* pClone = new CCardinalPoint(*this);
	return pClone;
}

HRESULT CCardinalPoint::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CCardinalPoint::Awake()
{
	CGameObject::Awake();
	m_pTransform->Set_Position(D3DXVECTOR3(520.f, -137.5f, 0.f));
	m_pTransform->UpdateTransform();
	
	m_pImage = (Image*)AddComponent<Image>();

	m_eRenderID = ERenderID::UI;

	m_fDegree = 0.f;
	return S_OK;
}

HRESULT CCardinalPoint::Start()
{
	CGameObject::Start();
	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("UI"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("CardinalPoint");
	m_pImage->SetTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]);
	m_pImage->SetWidth(170.f);
	m_pImage->SetHeight(35.f);
	m_pImage->SetTiling(D3DXVECTOR2(0.5f, 1.f));
	return S_OK;
}

UINT CCardinalPoint::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CCardinalPoint::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);
	SetOffset();
	return 0;
}

HRESULT CCardinalPoint::Render()
{
	CGameObject::Render();
	m_pDevice->SetTransform(D3DTS_WORLD, &m_pTransform->Get_WorldMatrix());
	m_pImage->Render();
	return S_OK;
}

void CCardinalPoint::SetOffset()
{
	D3DXMATRIX	matView;
	D3DXVECTOR3	vCamLook;
	D3DXVECTOR3 vZAxis = D3DXVECTOR3(0.f, 0.f, 1.f);
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);

	vCamLook.x = matView._31;
	vCamLook.y = 0.f;
	vCamLook.z = matView._33;

	D3DXVec3Normalize(&vCamLook, &vCamLook);

	float fDotResult = 0.f;
	float fRadian = 0.f;

	fDotResult = D3DXVec3Dot(&vZAxis, &vCamLook);

	fRadian = acosf(fDotResult);

	if (vCamLook.x < 0.f)
		fRadian = D3DXToRadian(360.f) - fRadian;

	float fDegree = D3DXToDegree(fRadian);

	float fDelta = fabs(m_fDegree - fDegree);

	if (fDelta < 1.0f)
		return;

	D3DXVECTOR2 vOffSet = D3DXVECTOR2(0.f, 0.f);
	vOffSet.x = -fDegree / 360.f;

	m_pImage->SetOffset(vOffSet);

}
