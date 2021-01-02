#pragma once
#ifndef __WORM_H__
#include "WormPart.h"
USING(Engine)
class CBossHP;
class CWorm final : public CWormPart
{
private:
	enum PATTERN { IDLE, BURROW, FARAWAY, TRACE, FIRE, PATTERN_END};
private:
	CBossHP* m_pBossHP;

	UINT m_nMaxCount;
	UINT m_nIndex;

	float m_fTime;
	float m_fAnimationSpeed;

	float	m_fInterpolation;

	float	m_fTraceTime;

	D3DXVECTOR3	m_vMove;

	D3DXVECTOR3	m_vBurrowStart;
	D3DXVECTOR3 m_vBurrowEnd;

	D3DXVECTOR3	m_vFarAwayStart;
	D3DXVECTOR3 m_vFarAwayEnd;

	PATTERN		m_ePattern;

	RECT		m_tAreaRect;

	float		m_fFireInterval;
	UINT		m_nFireCount;
private:
	explicit CWorm();
	explicit CWorm(const CWorm& _rOther);
	virtual ~CWorm() = default;
	// CBase을(를) 통해 상속됨
	virtual void Free() override;
public:
	static CWorm* Create();
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

private:
	void ConnectBody();
	void Animate(const float _fDeltaTime);
	void Move();

	void DoPattern(const float _fDeltaTime);
	void SetPattern(PATTERN _ePattern);

	//Pattern
	void Idle(const float _fDeltaTime);
	void Burrow(const float _fDeltaTime);
	void FarAway(const float _fDeltaTime);
	void Trace(const float _fDeltaTime);
	void Fire(const float _fDeltaTime);

	D3DXVECTOR3 CalculateFarAwayEnd();

	void SetTextureKey();
};
#define __WORM_H__
#endif // !__WORM_H__
