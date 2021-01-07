#pragma once
#ifndef __BATTLE_SHIP_BULLET_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CBattleShipBullet final : public CGameObject
{
private:
	CMeshRenderer*		m_pMeshRenderer;

	CTexturePool*		m_pTexturePool;
	TSTRING				m_sTextureKey;

	UINT				m_nIndex;
	UINT				m_nMaxFrame;

	float				m_fTime;
	float				m_fAnimationSpeed;

	float				m_fMoveSpeed;

	bool				m_bRemove;

	CCollider*			m_pCollider;
private:
	explicit CBattleShipBullet();
	explicit CBattleShipBullet(const CBattleShipBullet& _rOther);
	virtual ~CBattleShipBullet() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CBattleShipBullet* Create();
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
	virtual void OnCollision(CGameObject* _pGameObject) override;

public:
	void	SetBullet(D3DXVECTOR3 _vStart);
private:
	void	Animate(const float _fDeltaTime);
	void	Move(const float _fDeltaTime);
};
#define __BATTLE_SHIP_BULLET_H__
#endif // !__BATTLE_SHIP_BULLET_H__
