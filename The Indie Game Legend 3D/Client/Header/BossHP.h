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
	// CBase��(��) ���� ��ӵ�
	virtual void			Free() override;
public:
	static CBossHP* Create();
	// CGameObject��(��) ���� ��ӵ�
	virtual CGameObject * Clone() override;
public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT InitializePrototype() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual HRESULT Render() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;
public:
	//�Ű����� ������ ���� ü�� / �ִ� ü�� �� �Ѱ��ָ� ��.
	void SetHPBar(float _fRatio);
};
#define __BOSS_HP_H__
#endif // !__BOSS_HP_H__
