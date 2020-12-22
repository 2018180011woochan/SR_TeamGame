#pragma once
#ifndef __Piramide_H__


#include "Obstacle.h"
USING(Engine)
class CPiramide final: public CObstacle
{
public:
	explicit CPiramide();
	explicit CPiramide(const CPiramide& _rOther);
	virtual ~CPiramide() = default;

	// CObstacle을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
	
	virtual void Free() override;
	virtual void OnCollision(CGameObject* _pGameObject) override;
public:
	static CPiramide* Create();

private:
	bool		m_bDead;

};
#define __Piramide_H__
#endif // !__Piramide_H__
