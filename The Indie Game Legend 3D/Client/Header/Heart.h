#pragma once
#ifndef __HEART_H__
#include "GameObject.h"
USING(Engine)
class CTexturePool;
class CHeart final : public CGameObject
{
private:
	Image*			m_pImage;
	CTexturePool*	m_pTexturePool;
	TSTRING			m_sTextureKey;
	UINT			m_nMaxFrame;
private:
	explicit CHeart();
	explicit CHeart(const CHeart& _rOther);
	virtual ~CHeart() = default;
	// CBase을(를) 통해 상속됨
	virtual void			Free() override;
public:
	static CHeart* Create();
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

	void SetGauge(const UINT _nValue);
};

#define __HEART_H__
#endif // !__HEART_H__
