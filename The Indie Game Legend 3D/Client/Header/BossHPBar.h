#pragma once
#ifndef __BOSS_HP_BAR_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CBossHPBar final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;
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

public:
	//매개변수 값으로 현재 체력 / 최대 체력 값 넘겨주면 됨.
	void SetHPBar(float _fRatio);
};
#define __BOSS_HP_BAR_H__
#endif // !__BOSS_HP_BAR_H__
