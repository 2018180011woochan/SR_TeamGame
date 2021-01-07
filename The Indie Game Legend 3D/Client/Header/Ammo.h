#pragma once
#ifndef __AMMO_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CShopKeeper;
class CAmmo final : public CGameObject
{
private:
	explicit CAmmo();
	explicit CAmmo(const CAmmo& other);
	virtual ~CAmmo() = default;

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
	static CAmmo* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*		m_pCollider;
	CShopKeeper*	m_pShopKeeper;
	bool			m_isBuyItem;
};

#define __AMMO_H__
#endif // !__AMMO_H__

