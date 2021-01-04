#pragma once
#ifndef __PRICE_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CShopKeeper;
class CPrice final : public CGameObject
{
private:
	explicit CPrice();
	explicit CPrice(const CPrice& other);
	virtual ~CPrice() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	virtual void OnCollision(CGameObject* _pGameObject) override;

public:
	HRESULT IsBillboarding();
	void Set_ItemPos(const _vector _vItemPos);
	void MoveMent(float _fDeltaTime);

public:
	virtual CGameObject * Clone() override;
	static CPrice* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*		m_pCollider;
	CShopKeeper*	m_pShopKeeper;
};

#define __PRICE_H__
#endif // !__PRICE_H__

