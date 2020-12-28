#pragma once
#ifndef __BOSS_HP_H__
#include "GameObject.h"
USING(Engine)
class CBossHPFrame;
class CBossHPBar;
class CBossText;
class CBossHP final : public CGameObject
{
private:
	CBossHPFrame*	m_pBossHpFrame;
	CBossHPBar*		m_pBossHpBar;
	CBossText*		m_pBossText;
private:
	explicit CBossHP();
	explicit CBossHP(const CBossHP& _rOther);
	virtual ~CBossHP() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CBossHP* Create();
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

	virtual void OnEnable() override;
	virtual void OnDisable() override;
public:
	//매개변수 값으로 현재 체력 / 최대 체력 값 넘겨주면 됨.
	void SetHPBar(float _fRatio);
};
#define __BOSS_HP_H__
#endif // !__BOSS_HP_H__
