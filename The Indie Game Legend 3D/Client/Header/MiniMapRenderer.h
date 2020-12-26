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

	//�� ��� ���� ���� ����.
	UINT m_nX;
	UINT m_nY;
	//�� ��� ���� ���� ũ��
	const UINT m_nCX;
	const UINT m_nCY;
	//�κ� �̴ϸ� �⺻ ũ��
	const UINT m_nPartialCX;
	const UINT m_nPartialCY;
	//��ī��Ʈ ��ǥ ���� �� ���� ��� ��ǥ
	D3DXVECTOR3 m_vStartPoint;

	//
	D3DXVECTOR3 m_vMapPointPos;
private:
	explicit CMiniMapRenderer();
	explicit CMiniMapRenderer(const CMiniMapRenderer& _rOther);
	virtual ~CMiniMapRenderer() = default;
	// CBase��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static CMiniMapRenderer* Create();
	// CGameObject��(��) ���� ��ӵ�
	virtual CGameObject * Clone() override;
public:
	// CGameObject��(��) ���� ��ӵ�
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
