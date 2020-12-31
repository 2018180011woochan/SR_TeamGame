#pragma once
#ifndef __BIGBULLET_H__
#include "Bullet.h"
class CBigBullet final	: public CBullet
{
public:
	explicit CBigBullet();
	explicit CBigBullet(const CBigBullet& _rOther);
	virtual ~CBigBullet() =default;

	// CBullet을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
	virtual HRESULT Fire() override;
	virtual HRESULT Fire(const _vector& _vPos , const _vector& _vDir);

	
	virtual void Free() override;
	static CBigBullet* Create();
	virtual void OnCollision(CGameObject * _pGameObject) override;
};

#define __BIGBULLET_H__
#endif
