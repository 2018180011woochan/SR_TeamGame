#pragma once
#ifndef __PLAYERSHIP_H__

#include "GameObject.h"
#include "TexturePoolManager.h"
USING(Engine)
class CPlayerShip final : public CGameObject
{
private:
	explicit CPlayerShip();
	explicit CPlayerShip(const CPlayerShip& other);
	virtual ~CPlayerShip() = default;

public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	//Test
	virtual void OnCollision(CGameObject* _pGameObject) override;


private:
	HRESULT Movement(float fDeltaTime);


public:
	virtual CGameObject * Clone() override;
	static CPlayerShip* Create();
private:
	virtual void Free() override;

private:
	CMeshRenderer* m_pMeshRenderer = nullptr;
	CTexturePool*  m_pTexturePool;
	CCollider*		m_pCollider;


	UINT nIndex = 0;

	bool m_bIsTPS;
};

#define __PLAYERSHIP_H__
#endif // !__PLAYERSHIP_H__
