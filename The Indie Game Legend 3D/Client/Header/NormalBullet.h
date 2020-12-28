#pragma once
#include "Bullet.h"
class CNormalBullet final : public CBullet
{
public:
	explicit CNormalBullet();
	explicit CNormalBullet(const CNormalBullet& _rOther);
	virtual ~CNormalBullet() = default;

	// CBullet을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
	virtual HRESULT Fire() override;

public:
	virtual void Free() override;
	static CNormalBullet* Create();

	// CBullet을(를) 통해 상속됨
	virtual void OnCollision(CGameObject * _pGameObject) override;
};

