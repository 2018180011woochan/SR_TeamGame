#include "stdafx.h"
#include "..\Header\MiniMapRenderer.h"
#include "TexturePoolManager.h"
#include "Player.h"

CMiniMapRenderer::CMiniMapRenderer()
	: m_pTexturePool(nullptr)
	, m_pExploredArea(nullptr)
	, m_pEntireMap(nullptr)
	, m_pPartialMap(nullptr)
	, m_pPlayer(nullptr)
	, m_nCurrentTag(-1)
	, m_nBlockCX(120)
	, m_nBlockCZ(104)
	, m_nCX(40)
	, m_nCY(40)
	, m_nPartialCX(120)
	, m_nPartialCY(120)
{
}

CMiniMapRenderer::CMiniMapRenderer(const CMiniMapRenderer & _rOther)
	: CGameObject(_rOther)
	, m_nBlockCX(120)
	, m_nBlockCZ(104)
	, m_nCX(40)
	, m_nCY(40)
	, m_nPartialCX(120)
	, m_nPartialCY(120)
	, m_pTexturePool(nullptr)
	, m_pExploredArea(nullptr)
	, m_pEntireMap(nullptr)
	, m_pPartialMap(nullptr)
	, m_pPlayer(nullptr)
{
}

void CMiniMapRenderer::Free()
{
	CGameObject::Free();
	SafeRelease(m_pTexturePool);
	SafeRelease(m_pExploredArea);
	SafeRelease(m_pEntireMap);
	SafeRelease(m_pPartialMap);
	SafeRelease(m_pPlayer);
}

CMiniMapRenderer * CMiniMapRenderer::Create()
{
	CMiniMapRenderer* pInstance = new CMiniMapRenderer;
	if (FAILED(pInstance->InitializePrototype()))
	{
		SafeRelease(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject * CMiniMapRenderer::Clone()
{
	CMiniMapRenderer* pClone = new CMiniMapRenderer(*this);
	return pClone;
}

HRESULT CMiniMapRenderer::InitializePrototype()
{
	CGameObject::InitializePrototype();
	return S_OK;
}

HRESULT CMiniMapRenderer::Awake()
{
	CGameObject::Awake();
	m_eRenderID = ERenderID::UI;
	return S_OK;
}

HRESULT CMiniMapRenderer::Start()
{
	CGameObject::Start();
	m_pPlayer = (CPlayer*)(FindGameObjectOfType<CPlayer>());

	m_pTexturePool = CTexturePoolManager::GetInstance()->GetTexturePool(TEXT("MiniMap"));
	SafeAddRef(m_pTexturePool);
	m_sTextureKey = TEXT("Sector1");


	UINT nWidth = m_pTexturePool->GetTexture(m_sTextureKey)[0]->GetDesc().Width;
	UINT nHeight = m_pTexturePool->GetTexture(m_sTextureKey)[0]->GetDesc().Height;

	m_nX = nWidth / m_nCX;
	m_nY = nHeight / m_nCY;

	m_vStartPoint.x = -float(m_nX) * 0.5f * m_nBlockCX;
	m_vStartPoint.z =  3.5f * m_nBlockCZ;

	m_pEntireMap = CRenderTexture::Create();
	m_pEntireMap->Initialize(nWidth, nHeight);

	m_pExploredArea = CRenderTexture::Create();
	m_pExploredArea->Initialize(nWidth, nHeight);

	m_pPartialMap = CRenderTexture::Create();
	m_pPartialMap->Initialize(m_nPartialCX, m_nPartialCY);


	return S_OK;
}

UINT CMiniMapRenderer::Update(const float _fDeltaTime)
{
	CGameObject::Update(_fDeltaTime);
	return 0;
}

UINT CMiniMapRenderer::LateUpdate(const float _fDeltaTime)
{
	CGameObject::LateUpdate(_fDeltaTime);

	DrawExploredArea();

	D3DXMATRIX matI;
	D3DXMatrixIdentity(&matI);
	m_pEntireMap->DrawOnRenderTexture(m_pTexturePool->GetTexture(m_sTextureKey)[0]->GetTexture(), nullptr, D3DXVECTOR3(0.f, 0.f, 0.f), matI, true);
	m_pEntireMap->DrawOnRenderTexture(m_pExploredArea->GetTexture(), nullptr, D3DXVECTOR3(0.f, 0.f, 0.f), matI, false);

	WorldToMiniMap();
	DrawMapPoint();

	DrawPartialMap();
	return 0;
}

HRESULT CMiniMapRenderer::Render()
{
	CGameObject::Render();
	return S_OK;
}

CRenderTexture * CMiniMapRenderer::GetEntireMap()
{
	return m_pEntireMap;
}

CRenderTexture * CMiniMapRenderer::GetPartialMap()
{
	return m_pPartialMap;
}

void CMiniMapRenderer::DrawExploredArea()
{
	const int nPlayerTag = m_pPlayer->GetTag();

	if (nPlayerTag == m_nCurrentTag)
		return;
	if (0 > m_nCurrentTag)
		return;
	m_nCurrentTag = nPlayerTag;
	UINT nIndex = UINT(m_nCurrentTag);

	UINT nIndexX = nIndex % m_nX;
	UINT nIndexY = nIndex / m_nX;

	RECT tRect;
	tRect.left = nIndexX * m_nCX;
	tRect.top = nIndexY * m_nCX;
	tRect.right = tRect.left + m_nCX;
	tRect.bottom = tRect.top + m_nCX;

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, float(tRect.left), float(tRect.top), 0.f);

	m_pExploredArea->DrawOnRenderTexture(m_pTexturePool->GetTexture(m_sTextureKey)[1]->GetTexture(), &tRect, D3DXVECTOR3(0.f, 0.f, 0.f), matTrans, false);
}

void CMiniMapRenderer::DrawMapPoint()
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

	D3DXMATRIX matScale;
	D3DXMATRIX matRZ;
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;
	
	D3DXMatrixTranslation(&matTrans, m_vMapPointPos.x, m_vMapPointPos.z, 0.f);
	D3DXMatrixScaling(&matScale, 2.5f, 2.5f, 1.f);
	D3DXMatrixRotationZ(&matRZ, -fRadian);
	
	matWorld = matScale * matRZ * matTrans;
	
	m_pEntireMap->DrawOnRenderTexture(m_pTexturePool->GetTexture(TEXT("MapPos"))[0]->GetTexture(), nullptr, D3DXVECTOR3(8.f, 8.f, 0.f), matWorld, false);
}

void CMiniMapRenderer::DrawPartialMap()
{
	RECT tRect;
	tRect.left = LONG(m_vMapPointPos.x - (m_nPartialCX * 0.5f));
	tRect.top = LONG(m_vMapPointPos.z - (m_nPartialCY * 0.5f));
	tRect.right = LONG(m_vMapPointPos.x + (m_nPartialCX * 0.5f));
	tRect.bottom = LONG(m_vMapPointPos.z + (m_nPartialCY * 0.5f));

	D3DXVECTOR3 vPosition = D3DXVECTOR3(0.f, 0.f, 0.f);

	LONG nDelta;
	if (tRect.left < 0)
	{
		nDelta = -tRect.left;
		tRect.left = 0;
		tRect.right = m_nPartialCX - nDelta;
		vPosition.x = float(nDelta);
	}
	else if (tRect.right > LONG(m_nX * m_nCX))
	{
		nDelta = tRect.right - LONG(m_nX * m_nCX);
		tRect.right = LONG(m_nX * m_nCX);
		tRect.left = tRect.right - m_nPartialCX + nDelta;
	}
	if (tRect.top < 0)
	{
		nDelta = -tRect.top;
		tRect.top = 0;
		tRect.bottom = m_nPartialCY - nDelta;
		vPosition.y = float(nDelta);
	}
	else if (tRect.bottom > LONG(m_nY * m_nCY))
	{
		nDelta = tRect.bottom - LONG(m_nY * m_nCY);
		tRect.bottom = LONG(m_nY * m_nCY);
		tRect.top = tRect.bottom - m_nPartialCY + nDelta;
	}

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, vPosition.x, vPosition.y, vPosition.z);


	m_pPartialMap->DrawOnRenderTexture(m_pEntireMap->GetTexture(), &tRect, D3DXVECTOR3(0.f, 0.f, 0.f), matTrans, true);
}

void CMiniMapRenderer::WorldToMiniMap()
{
	D3DXVECTOR3 vWorldPos = ((CTransform*)m_pPlayer->GetComponent<CTransform>())->Get_Position();

	//플레이어의 좌표를 맵 좌측 상단을 원점으로 한 좌표로 변경.
	vWorldPos = vWorldPos - m_vStartPoint;

	//맵 전체 길이
	D3DXVECTOR3 vWorldSize;
	vWorldSize.x = float(m_nBlockCX * m_nX);
	vWorldSize.z = float(m_nBlockCZ * m_nY);

	//월드 좌표 비율
	float m_fRatioX = vWorldPos.x / vWorldSize.x;
	float m_fRatioZ = -vWorldPos.z / vWorldSize.z;

	D3DXVECTOR3 vMapPos;
	D3DXVECTOR3 vMapSize;

	vMapSize.x = float(m_nCX * m_nX);
	vMapSize.z = float(m_nCY * m_nY);

	vMapPos.x = m_fRatioX * vMapSize.x;
	vMapPos.z = m_fRatioZ * vMapSize.z;

	m_vMapPointPos = vMapPos;
}



