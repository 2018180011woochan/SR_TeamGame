#pragma once
#ifndef __SKILLRUNNING_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CShopKeeper;
class CSkillRunning final : public CGameObject
{
private:
	explicit CSkillRunning();
	explicit CSkillRunning(const CSkillRunning& other);
	virtual ~CSkillRunning() = default;

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
	bool GetIsDead() { return m_bDead; }

public:
	virtual CGameObject * Clone() override;
	static CSkillRunning* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*		m_pCollider;
	CShopKeeper*	m_pShopKeeper;
	bool			m_isBuyItem;
	bool			m_bDead;
	_int			m_nPrice;
};

#define __SKILLRUNNING_H__
#endif // !__SKILLRUNNING_H__

