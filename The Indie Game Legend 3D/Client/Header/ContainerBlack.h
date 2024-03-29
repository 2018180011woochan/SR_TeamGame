#pragma once
#ifndef __CONTAINERBLACK_H__
#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CContainerBlack final : public CGameObject
{
private:
	explicit CContainerBlack();
	explicit CContainerBlack(const CContainerBlack& other);
	virtual ~CContainerBlack() = default;

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

public:
	virtual CGameObject * Clone() override;
	static CContainerBlack* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*		m_pCollider;
	bool m_bIsHit;
	

	float				m_fWalkSpeed;
	float				m_fWalkDeltaTime;
	UINT nIndex;
};

#define __CONTAINERBLACK_H__
#endif // !__CONTAINERBLACK_H__

