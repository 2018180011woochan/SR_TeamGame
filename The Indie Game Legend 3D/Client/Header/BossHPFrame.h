#pragma once
#ifndef __BOSS_HP_FRAME_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CBossHPFrame final : public CGameObject
{
private:
	CTexturePool*	m_pTexturePool;
	Image*			m_pImage;
	TSTRING			m_sTextureKey;
private:
	explicit CBossHPFrame();
	explicit CBossHPFrame(const CBossHPFrame& _rOther);
	virtual ~CBossHPFrame() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CBossHPFrame* Create();
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
#define __BOSS_HP_FRAME_H__
#endif // !__BOSS_HP_FRAME_H__
