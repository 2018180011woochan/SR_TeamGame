#pragma once
#ifndef __FLAMEBULLET_H__
#include "Bullet.h"
class CFlameBullet final : public CBullet
{
private:
	float	m_fMaxScale = 0.f;
	float	m_fScale = 0.f;
	float	m_fScaleDelta = 0.f;
private:
	HRESULT Animate(float _fDeltaTime);
	void	UpdateScale(float _fDeltaTime);
public:
	explicit CFlameBullet();
	explicit CFlameBullet(const CFlameBullet& _rOther);
	virtual ~CFlameBullet() = default;
	
	// CBullet을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
	virtual HRESULT Fire() override;
	virtual void OnCollision(CGameObject * _pGameObject) override;

	virtual void Free() override;
	static CFlameBullet* Create();
};

#define __FLAMEBULLET_H__
#endif
