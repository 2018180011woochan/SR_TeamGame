#pragma once
#ifndef __TRIPLEBULLET_H__
#include "Bullet.h"
class CTripleBullet final : public CBullet
{
public:
	explicit CTripleBullet();
	explicit CTripleBullet(const CTripleBullet& _rOther);
	virtual ~CTripleBullet() =default;

	// CBullet을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;

	virtual HRESULT Fire() override;
	virtual HRESULT Fire(_vector _Pos, _vector _vDir);

	virtual void Free();
	static CTripleBullet* Create();
	virtual void OnCollision(CGameObject * _pGameObject) override;
};

#define __TRIPLEBULLET_H__
#endif
