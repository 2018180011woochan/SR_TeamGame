#pragma once
#ifndef __BOSS_HP_BAR_H__
#include "GameObject.h"
USING(Engine)
class CBossHPBar final : public CGameObject
{
private:
	explicit CBossHPBar();
	explicit CBossHPBar(const CBossHPBar& _rOther);
	virtual ~CBossHPBar() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CBossHPBar* Create();
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
};
#define __BOSS_HP_BAR_H__
#endif // !__BOSS_HP_BAR_H__
