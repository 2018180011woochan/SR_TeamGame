#pragma once
#ifndef __FINAL_EXPLOSION_H__
#include "GameObject.h"
USING(Engine)
class CFinalExplosion : public CGameObject
{
public:
	CFinalExplosion();
	virtual ~CFinalExplosion();

	// CGameObject을(를) 통해 상속됨
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;
	virtual CGameObject * Clone() override;
};
#define __FINAL_EXPLOSION_H__
#endif // !__FINAL_EXPLOSION_H__
