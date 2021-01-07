#pragma once
#ifndef __SKILLRUNNINGPRICE_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CShopKeeper;
class CSkillRunning;
class CSkillRunningPrice final : public CGameObject
{
private:
	explicit CSkillRunningPrice();
	explicit CSkillRunningPrice(const CSkillRunningPrice& other);
	virtual ~CSkillRunningPrice() = default;

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
	static CSkillRunningPrice* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*		m_pCollider;
	CShopKeeper*	m_pShopKeeper;
	CSkillRunning*	m_pSkillRunning;
};

#define __SKILLRUNNINGPRICE_H__
#endif // !__SKILLRUNNINGPRICE_H__

