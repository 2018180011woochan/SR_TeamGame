#pragma once
#ifndef __MINIMAP_RENDERER_H__
#include "GameObject.h"
#include "RenderTexture.h"
USING(Engine)
class CTexturePool;
class CPlayer;
class CMiniMapRenderer final : public CGameObject
{
private:
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;
	CRenderTexture* m_pExploredArea;
	CRenderTexture* m_pEntireMap;
	CRenderTexture* m_pPartialMap;
	CPlayer*		m_pPlayer;

	int				m_nCurrentTag;

	const UINT		m_nBlockCX;
	const UINT		m_nBlockCZ;

	//맵 블록 가로 세로 개수.
	UINT m_nX;
	UINT m_nY;
	//맵 블록 가로 세로 크기
	const UINT m_nCX;
	const UINT m_nCY;
	//부분 미니맵 기본 크기
	const UINT m_nPartialCX;
	const UINT m_nPartialCY;
	//데카르트 좌표 기준 맵 좌측 상단 좌표
	D3DXVECTOR3 m_vStartPoint;

	//
	D3DXVECTOR3 m_vMapPointPos;
private:
	explicit CMiniMapRenderer();
	explicit CMiniMapRenderer(const CMiniMapRenderer& _rOther);
	virtual ~CMiniMapRenderer() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CMiniMapRenderer* Create();
	// CGameObject을(를) 통해 상속됨
	virtual CGameObject * Clone() override;
public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

public:
	CRenderTexture* GetEntireMap();
	CRenderTexture* GetPartialMap();
private:
	void	DrawExploredArea();
	void	DrawMapPoint();
	void	DrawPartialMap();
	void	WorldToMiniMap();
};
#define __MINIMAP_RENDERER_H__
#endif // !__MINIMAP_RENDERER_H__
