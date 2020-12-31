#pragma once
#include "Bullet.h"
class CLaserBullet final : public CBullet
{
private:
	_matrix	 m_matDest;
	bool	m_bAutoEnable = false;
	float  m_fAxisAngle = 0.f;
public:
	explicit CLaserBullet();
	explicit CLaserBullet(const CLaserBullet& _rOther);
	virtual ~CLaserBullet() = default;

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
	virtual HRESULT IsBillboarding() override;
	static CLaserBullet* Create();
};

